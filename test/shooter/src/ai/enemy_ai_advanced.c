/*
 * Advanced Enemy AI System
 * Sophisticated AI with behavior trees, neural networks, prediction, and AVX batch processing.
 * This implementation maximizes AVX/AVX2 usage for testing the IDA lifter.
 */

#include <immintrin.h>
#include <math.h>
#include <string.h>
#include "ai/enemy_ai.h"
#include "ai/behavior_tree.h"
#include "ai/neural_net.h"
#include "ai/prediction.h"
#include "ai/influence_avx.h"
#include "math/avx_ai_math.h"
#include "config.h"
#include "core/rng.h"
#include "level/level.h"
#include "combat/bullet.h"
#include "entity/entity.h"
#include "math/sse_math.h"

/* ==========================================================================
 * GLOBAL AI STATE - Per-entity extended AI data
 * ========================================================================== */

typedef struct __attribute__((aligned(32))) {
    /* Movement prediction */
    MovementHistory target_history;

    /* Neural network for decisions */
    NeuralNet brain;

    /* Behavior tree state */
    BehaviorTree behavior_tree;
    BTBlackboard blackboard;

    /* Memory system */
    AIMemory memory;

    /* Tactical state */
    float last_attack_time;
    float last_damage_time;
    float suppression_level;
    int consecutive_misses;

    /* Coordination */
    int assigned_flank_side;    /* -1 = left, 0 = center, 1 = right */
    bool is_suppressing;
    int suppress_target_x;
    int suppress_target_y;

    /* Prediction */
    float predicted_target_x;
    float predicted_target_y;
    float prediction_confidence;
} EnemyAIState;

static EnemyAIState enemy_ai_states[MAX_ENTITIES];
static bool ai_states_initialized = false;

/* Global advanced influence map */
static InfluenceMapAdvanced g_influence_map;

/* Forward declarations for functions from original enemy_ai.c that we reuse */
extern void execute_patrol(Entity* e, GameState* game);
extern void execute_hiding(Entity* e, GameState* game);
extern void execute_retreat(Entity* e, GameState* game);
extern void execute_healing(Entity* e, GameState* game);
extern void execute_reload(Entity* e, GameState* game);
extern void execute_hunting(Entity* e, GameState* game);
extern void execute_supporting(Entity* e, GameState* game);

/* Forward declaration for our own function */
void update_enemy_ai_advanced(GameState* game, int entity_id);

/* ==========================================================================
 * AVX BATCH ENTITY STATE EXTRACTION
 * ========================================================================== */

typedef struct __attribute__((aligned(32))) {
    float x[8], y[8];
    float vx[8], vy[8];
    float health[8];
    float facing_x[8], facing_y[8];
    float view_dist[8];
    int id[8];
    int state[8];
    int count;
} BatchEntityState;

/* Extract state from up to 8 entities using AVX */
static void avx_extract_entity_batch(
    const GameState* game,
    const int* entity_ids,
    int count,
    BatchEntityState* batch
) {
    batch->count = count;

    for (int i = 0; i < count && i < 8; i++) {
        const Entity* e = &game->entities[entity_ids[i]];
        batch->x[i] = e->x;
        batch->y[i] = e->y;
        batch->vx[i] = e->vx;
        batch->vy[i] = e->vy;
        batch->health[i] = e->health / e->max_health;
        batch->facing_x[i] = cosf(e->facing_angle);
        batch->facing_y[i] = sinf(e->facing_angle);
        batch->view_dist[i] = e->view_distance;
        batch->id[i] = entity_ids[i];
        batch->state[i] = e->state;
    }

    /* Pad remaining slots */
    for (int i = count; i < 8; i++) {
        batch->x[i] = 0;
        batch->y[i] = 0;
        batch->vx[i] = 0;
        batch->vy[i] = 0;
        batch->health[i] = 0;
        batch->facing_x[i] = 1;
        batch->facing_y[i] = 0;
        batch->view_dist[i] = 0;
        batch->id[i] = -1;
        batch->state[i] = STATE_DEAD;
    }
}

/* ==========================================================================
 * AVX THREAT ASSESSMENT
 * ========================================================================== */

/* Calculate threat levels for 8 potential threats at once */
static void avx_batch_threat_assessment(
    const Entity* self,
    const BatchEntityState* threats,
    float* out_threat_levels
) {
    __m256 self_x = _mm256_set1_ps(self->x);
    __m256 self_y = _mm256_set1_ps(self->y);

    __m256 threat_x = _mm256_loadu_ps(threats->x);
    __m256 threat_y = _mm256_loadu_ps(threats->y);
    __m256 threat_health = _mm256_loadu_ps(threats->health);
    __m256 threat_fx = _mm256_loadu_ps(threats->facing_x);
    __m256 threat_fy = _mm256_loadu_ps(threats->facing_y);

    /* Direction from self to threat */
    __m256 dx = _mm256_sub_ps(threat_x, self_x);
    __m256 dy = _mm256_sub_ps(threat_y, self_y);

    /* Distance */
    __m256 dist_sq = fmadd_ps(dy, dy, _mm256_mul_ps(dx, dx));
    __m256 dist = _mm256_sqrt_ps(dist_sq);
    __m256 inv_dist = _mm256_div_ps(_mm256_set1_ps(1.0f),
        _mm256_add_ps(dist, _mm256_set1_ps(0.001f)));

    /* Normalize direction */
    __m256 norm_dx = _mm256_mul_ps(dx, inv_dist);
    __m256 norm_dy = _mm256_mul_ps(dy, inv_dist);

    /* How much is threat facing us? */
    __m256 facing_dot = fmadd_ps(norm_dx, threat_fx, _mm256_mul_ps(norm_dy, threat_fy));
    facing_dot = _mm256_max_ps(facing_dot, _mm256_setzero_ps());

    /* Threat level = health * facing_factor * (50 / distance) */
    __m256 threat_level = _mm256_mul_ps(threat_health, facing_dot);
    threat_level = _mm256_mul_ps(threat_level,
        _mm256_mul_ps(inv_dist, _mm256_set1_ps(50.0f)));

    /* Clamp */
    threat_level = _mm256_max_ps(threat_level, _mm256_setzero_ps());
    threat_level = _mm256_min_ps(threat_level, _mm256_set1_ps(100.0f));

    _mm256_storeu_ps(out_threat_levels, threat_level);
}

/* ==========================================================================
 * AVX UTILITY SCORING
 * ========================================================================== */

typedef struct __attribute__((aligned(32))) {
    float attack[8];
    float defend[8];
    float flank[8];
    float retreat[8];
    float heal[8];
    float reload[8];
    float suppress[8];
    float investigate[8];
} BatchUtilityScores;

/* Calculate all utility scores for 8 entities at once */
static void avx_batch_utility_scoring(
    const BatchEntityState* entities,
    const GameState* game,
    BatchUtilityScores* scores
) {
    /* Load entity state */
    __m256 health = _mm256_loadu_ps(entities->health);
    __m256 one = _mm256_set1_ps(1.0f);
    __m256 zero = _mm256_setzero_ps();

    /* Gather additional state for each entity */
    float __attribute__((aligned(32))) has_ammo[8];
    float __attribute__((aligned(32))) has_cover[8];
    float __attribute__((aligned(32))) has_medpen[8];
    float __attribute__((aligned(32))) target_visible[8];
    float __attribute__((aligned(32))) target_in_range[8];
    float __attribute__((aligned(32))) aggression[8];
    float __attribute__((aligned(32))) courage[8];
    float __attribute__((aligned(32))) threat_count[8];

    for (int i = 0; i < 8; i++) {
        if (entities->id[i] < 0) {
            has_ammo[i] = 0;
            has_cover[i] = 0;
            has_medpen[i] = 0;
            target_visible[i] = 0;
            target_in_range[i] = 0;
            aggression[i] = 0.5f;
            courage[i] = 0.5f;
            threat_count[i] = 0;
            continue;
        }

        const Entity* e = &game->entities[entities->id[i]];
        has_ammo[i] = (e->weapon.mag_current > 0) ? 1.0f : 0.0f;
        has_cover[i] = e->has_cover_nearby ? 1.0f : 0.0f;
        has_medpen[i] = (e->medpens > 0) ? 1.0f : 0.0f;

        /* Check primary threat visibility */
        if (e->primary_threat >= 0 && e->primary_threat < e->threat_count) {
            const ThreatInfo* t = &e->threats[e->primary_threat];
            target_visible[i] = t->is_visible ? 1.0f : 0.0f;

            WeaponStats ws = weapon_get_stats(&e->weapon);
            target_in_range[i] = (t->distance < ws.range) ? 1.0f : 0.0f;
        } else {
            target_visible[i] = 0;
            target_in_range[i] = 0;
        }

        if (e->archetype) {
            aggression[i] = e->archetype->aggression;
            courage[i] = e->archetype->courage;
        } else {
            aggression[i] = 0.5f;
            courage[i] = 0.5f;
        }

        threat_count[i] = (float)e->threat_count;
    }

    /* Load gathered data into AVX registers */
    __m256 v_has_ammo = _mm256_loadu_ps(has_ammo);
    __m256 v_has_cover = _mm256_loadu_ps(has_cover);
    __m256 v_has_medpen = _mm256_loadu_ps(has_medpen);
    __m256 v_target_vis = _mm256_loadu_ps(target_visible);
    __m256 v_target_range = _mm256_loadu_ps(target_in_range);
    __m256 v_aggro = _mm256_loadu_ps(aggression);
    __m256 v_courage = _mm256_loadu_ps(courage);
    __m256 v_threats = _mm256_loadu_ps(threat_count);

    /* ========== ATTACK UTILITY ========== */
    /* Base = 1.0 * has_ammo * (0.5 + aggression) * in_range * visible */
    __m256 attack = _mm256_mul_ps(v_has_ammo, _mm256_add_ps(_mm256_set1_ps(0.5f), v_aggro));
    attack = _mm256_mul_ps(attack, v_target_range);
    attack = _mm256_mul_ps(attack, v_target_vis);

    /* Health penalty: *0.5 if health < 0.3 */
    __m256 low_health = _mm256_cmp_ps(health, _mm256_set1_ps(0.3f), _CMP_LT_OQ);
    attack = _mm256_blendv_ps(attack, _mm256_mul_ps(attack, _mm256_set1_ps(0.5f)), low_health);

    _mm256_storeu_ps(scores->attack, attack);

    /* ========== DEFEND UTILITY ========== */
    /* (2 - health) * has_cover * (1.5 - aggression) */
    __m256 defend = _mm256_sub_ps(_mm256_set1_ps(2.0f), health);
    defend = _mm256_mul_ps(defend, v_has_cover);
    defend = _mm256_mul_ps(defend, _mm256_sub_ps(_mm256_set1_ps(1.5f), v_aggro));

    _mm256_storeu_ps(scores->defend, defend);

    /* ========== FLANK UTILITY ========== */
    /* 0.6 * (0.5 + aggression) * !visible */
    __m256 flank = _mm256_mul_ps(_mm256_set1_ps(0.6f),
        _mm256_add_ps(_mm256_set1_ps(0.5f), v_aggro));
    __m256 not_vis = _mm256_sub_ps(one, v_target_vis);
    flank = _mm256_mul_ps(flank, _mm256_add_ps(not_vis, _mm256_set1_ps(0.3f)));

    _mm256_storeu_ps(scores->flank, flank);

    /* ========== RETREAT UTILITY ========== */
    /* (1.5 - courage) * (1 + threats*0.3) * (health < 0.5 ? 2 : 1) */
    __m256 retreat = _mm256_sub_ps(_mm256_set1_ps(1.5f), v_courage);
    retreat = _mm256_mul_ps(retreat,
        _mm256_add_ps(one, _mm256_mul_ps(v_threats, _mm256_set1_ps(0.3f))));

    __m256 half_health = _mm256_cmp_ps(health, _mm256_set1_ps(0.5f), _CMP_LT_OQ);
    retreat = _mm256_blendv_ps(retreat, _mm256_mul_ps(retreat, _mm256_set1_ps(2.0f)), half_health);

    /* Critical health: 3x */
    __m256 crit_health = _mm256_cmp_ps(health, _mm256_set1_ps(0.25f), _CMP_LT_OQ);
    retreat = _mm256_blendv_ps(retreat, _mm256_set1_ps(3.0f), crit_health);

    _mm256_storeu_ps(scores->retreat, retreat);

    /* ========== HEAL UTILITY ========== */
    /* has_medpen * (1.5 - health) * (health < 0.3 ? 3 : 1) */
    __m256 heal = _mm256_mul_ps(v_has_medpen, _mm256_sub_ps(_mm256_set1_ps(1.5f), health));
    heal = _mm256_blendv_ps(heal, _mm256_mul_ps(heal, _mm256_set1_ps(3.0f)), low_health);

    _mm256_storeu_ps(scores->heal, heal);

    /* ========== RELOAD UTILITY ========== */
    /* !has_ammo ? 5.0 : (1 - ammo_ratio) * 2 */
    __m256 no_ammo = _mm256_cmp_ps(v_has_ammo, _mm256_set1_ps(0.5f), _CMP_LT_OQ);
    __m256 reload = _mm256_blendv_ps(
        _mm256_mul_ps(_mm256_sub_ps(one, v_has_ammo), _mm256_set1_ps(2.0f)),
        _mm256_set1_ps(5.0f),
        no_ammo
    );

    _mm256_storeu_ps(scores->reload, reload);

    /* ========== SUPPRESS UTILITY ========== */
    /* has_ammo * aggression * in_range * 0.5 */
    __m256 suppress = _mm256_mul_ps(v_has_ammo, v_aggro);
    suppress = _mm256_mul_ps(suppress, v_target_range);
    suppress = _mm256_mul_ps(suppress, _mm256_set1_ps(0.5f));

    _mm256_storeu_ps(scores->suppress, suppress);

    /* ========== INVESTIGATE UTILITY ========== */
    /* (1 - visible) * 0.7 */
    __m256 investigate = _mm256_mul_ps(not_vis, _mm256_set1_ps(0.7f));

    _mm256_storeu_ps(scores->investigate, investigate);
}

/* ==========================================================================
 * AVX BEST ACTION SELECTION
 * ========================================================================== */

static void avx_select_best_actions(
    const BatchUtilityScores* scores,
    int* out_actions,
    float* out_values
) {
    __m256 attack = _mm256_loadu_ps(scores->attack);
    __m256 defend = _mm256_loadu_ps(scores->defend);
    __m256 flank = _mm256_loadu_ps(scores->flank);
    __m256 retreat = _mm256_loadu_ps(scores->retreat);
    __m256 heal = _mm256_loadu_ps(scores->heal);
    __m256 reload = _mm256_loadu_ps(scores->reload);
    __m256 suppress = _mm256_loadu_ps(scores->suppress);
    __m256 investigate = _mm256_loadu_ps(scores->investigate);

    __m256 best = attack;
    __m256i best_action = _mm256_set1_epi32(STATE_COMBAT);

    /* Compare with defend */
    __m256 cmp = _mm256_cmp_ps(defend, best, _CMP_GT_OQ);
    best = _mm256_blendv_ps(best, defend, cmp);
    best_action = _mm256_blendv_epi8(best_action, _mm256_set1_epi32(STATE_HIDING),
        _mm256_castps_si256(cmp));

    /* Compare with flank */
    cmp = _mm256_cmp_ps(flank, best, _CMP_GT_OQ);
    best = _mm256_blendv_ps(best, flank, cmp);
    best_action = _mm256_blendv_epi8(best_action, _mm256_set1_epi32(STATE_FLANKING),
        _mm256_castps_si256(cmp));

    /* Compare with retreat */
    cmp = _mm256_cmp_ps(retreat, best, _CMP_GT_OQ);
    best = _mm256_blendv_ps(best, retreat, cmp);
    best_action = _mm256_blendv_epi8(best_action, _mm256_set1_epi32(STATE_RETREATING),
        _mm256_castps_si256(cmp));

    /* Compare with heal */
    cmp = _mm256_cmp_ps(heal, best, _CMP_GT_OQ);
    best = _mm256_blendv_ps(best, heal, cmp);
    best_action = _mm256_blendv_epi8(best_action, _mm256_set1_epi32(STATE_HEALING),
        _mm256_castps_si256(cmp));

    /* Compare with reload */
    cmp = _mm256_cmp_ps(reload, best, _CMP_GT_OQ);
    best = _mm256_blendv_ps(best, reload, cmp);
    best_action = _mm256_blendv_epi8(best_action, _mm256_set1_epi32(STATE_RELOAD),
        _mm256_castps_si256(cmp));

    /* Compare with investigate */
    cmp = _mm256_cmp_ps(investigate, best, _CMP_GT_OQ);
    best = _mm256_blendv_ps(best, investigate, cmp);
    best_action = _mm256_blendv_epi8(best_action, _mm256_set1_epi32(STATE_ALERT),
        _mm256_castps_si256(cmp));

    _mm256_storeu_ps(out_values, best);
    STOREU_SI256(out_actions, best_action);
}

/* ==========================================================================
 * AVX AIM PREDICTION
 * ========================================================================== */

/* Calculate predicted aim point using target movement history */
static void avx_calculate_aim_prediction(
    const Entity* shooter,
    const Entity* target,
    const MovementHistory* target_hist,
    float bullet_speed,
    float* aim_x, float* aim_y,
    float* confidence
) {
    /* Simple prediction if no history */
    if (target_hist->count < 2) {
        *aim_x = target->x;
        *aim_y = target->y;
        *confidence = 0.5f;
        return;
    }

    /* Calculate average velocity and acceleration from history */
    float __attribute__((aligned(32))) recent_vx[8] = {0};
    float __attribute__((aligned(32))) recent_vy[8] = {0};
    float __attribute__((aligned(32))) recent_ax[8] = {0};
    float __attribute__((aligned(32))) recent_ay[8] = {0};

    int samples = (target_hist->count < 8) ? target_hist->count : 8;
    for (int i = 0; i < samples; i++) {
        int idx = (target_hist->head - 1 - i + PRED_MAX_HISTORY) % PRED_MAX_HISTORY;
        recent_vx[i] = target_hist->vx[idx];
        recent_vy[i] = target_hist->vy[idx];
        recent_ax[i] = target_hist->ax[idx];
        recent_ay[i] = target_hist->ay[idx];
    }

    /* AVX average */
    __m256 vx_sum = _mm256_loadu_ps(recent_vx);
    __m256 vy_sum = _mm256_loadu_ps(recent_vy);
    __m256 ax_sum = _mm256_loadu_ps(recent_ax);
    __m256 ay_sum = _mm256_loadu_ps(recent_ay);

    float avg_vx = hsum256_ps(vx_sum) / samples;
    float avg_vy = hsum256_ps(vy_sum) / samples;
    float avg_ax = hsum256_ps(ax_sum) / samples;
    float avg_ay = hsum256_ps(ay_sum) / samples;

    /* Use intercept calculation */
    float intercept_time = pred_avx_intercept(
        shooter->x, shooter->y,
        target->x, target->y,
        avg_vx, avg_vy,
        bullet_speed,
        aim_x, aim_y
    );

    if (intercept_time > 0) {
        /* Refine with acceleration */
        *aim_x += 0.5f * avg_ax * intercept_time * intercept_time;
        *aim_y += 0.5f * avg_ay * intercept_time * intercept_time;
        *confidence = 0.9f - (intercept_time / 60.0f);  /* Less confident for longer shots */
        if (*confidence < 0.3f) *confidence = 0.3f;
    } else {
        *confidence = 0.4f;
    }
}

/* ==========================================================================
 * AVX STEERING FORCE CALCULATION
 * ========================================================================== */

static void avx_calculate_steering_forces(
    Entity* e,
    const GameState* game,
    float* out_force_x, float* out_force_y
) {
    float total_fx = 0, total_fy = 0;

    /* Gather nearby entities */
    float __attribute__((aligned(32))) neighbor_x[8] = {0};
    float __attribute__((aligned(32))) neighbor_y[8] = {0};
    float __attribute__((aligned(32))) neighbor_vx[8] = {0};
    float __attribute__((aligned(32))) neighbor_vy[8] = {0};
    float __attribute__((aligned(32))) neighbor_active[8] = {0};

    int n = 0;
    for (int i = 0; i < game->entity_count && n < 8; i++) {
        const Entity* other = &game->entities[i];
        if (other->id == e->id || !other->alive) continue;
        if (other->team != e->team) continue;  /* Only same team for flocking */

        float dist = sse_distance(e->x, e->y, other->x, other->y);
        if (dist < AI_COORDINATION_RADIUS) {
            neighbor_x[n] = other->x;
            neighbor_y[n] = other->y;
            neighbor_vx[n] = other->vx;
            neighbor_vy[n] = other->vy;
            neighbor_active[n] = 1.0f;
            n++;
        }
    }

    if (n > 0) {
        __m256 nx = _mm256_loadu_ps(neighbor_x);
        __m256 ny = _mm256_loadu_ps(neighbor_y);
        __m256 nvx = _mm256_loadu_ps(neighbor_vx);
        __m256 nvy = _mm256_loadu_ps(neighbor_vy);
        __m256 na = _mm256_loadu_ps(neighbor_active);

        /* Separation force */
        float sep_x, sep_y;
        avx_steering_separation_single(e->x, e->y, nx, ny, na,
            STEERING_SEPARATION_DIST, &sep_x, &sep_y);
        total_fx += sep_x * 0.5f;
        total_fy += sep_y * 0.5f;

        /* Cohesion force */
        float coh_x, coh_y;
        avx_steering_cohesion_single(e->x, e->y, nx, ny, na, &coh_x, &coh_y);
        total_fx += coh_x * 0.2f;
        total_fy += coh_y * 0.2f;

        /* Alignment force */
        float align_x, align_y;
        avx_steering_alignment_single(nvx, nvy, na, &align_x, &align_y);
        total_fx += align_x * 0.3f;
        total_fy += align_y * 0.3f;
    }

    /* Clamp total force */
    float force_mag = sqrtf(total_fx * total_fx + total_fy * total_fy);
    if (force_mag > STEERING_MAX_FORCE) {
        total_fx = (total_fx / force_mag) * STEERING_MAX_FORCE;
        total_fy = (total_fy / force_mag) * STEERING_MAX_FORCE;
    }

    *out_force_x = total_fx;
    *out_force_y = total_fy;
}

/* ==========================================================================
 * NEURAL NETWORK DECISION MAKING
 * ========================================================================== */

static int neural_net_decide_action(Entity* e, GameState* game) {
    EnemyAIState* ai = &enemy_ai_states[e->id];

    /* Extract features */
    float __attribute__((aligned(32))) features[NN_INPUT_SIZE];
    nn_extract_features(e, game, features);

    /* Run forward pass */
    nn_forward(&ai->brain, features);

    /* Get action */
    int action = nn_get_best_action(&ai->brain);

    return nn_action_to_state(action);
}

/* ==========================================================================
 * BEHAVIOR TREE INTEGRATION
 * ========================================================================== */

static void update_behavior_tree(Entity* e, GameState* game) {
    EnemyAIState* ai = &enemy_ai_states[e->id];

    /* Update blackboard */
    bt_update_blackboard(&ai->blackboard, e, game);

    /* Tick behavior tree */
    BTStatus status = bt_tick(&ai->behavior_tree, e, game, &ai->blackboard);

    /* Map behavior tree result to entity state if needed */
    (void)status;
}

/* ==========================================================================
 * ADVANCED COMBAT EXECUTION
 * ========================================================================== */

static void execute_advanced_combat(Entity* e, GameState* game) {
    EnemyAIState* ai = &enemy_ai_states[e->id];

    if (e->primary_threat < 0) {
        e->state = STATE_PATROL;
        return;
    }

    ThreatInfo* threat = &e->threats[e->primary_threat];
    Entity* target = &game->entities[threat->entity_id];

    if (!target->alive) {
        e->state = STATE_PATROL;
        return;
    }

    /* Update target movement history */
    pred_history_add(&ai->target_history, target->x, target->y,
                     target->vx, target->vy);

    /* Face the target */
    if (threat->is_visible) {
        e->facing_angle = threat->angle_to;
        threat->last_known_pos.x = target->x;
        threat->last_known_pos.y = target->y;
    }

    /* Calculate predicted aim point */
    WeaponStats stats = weapon_get_stats(&e->weapon);
    avx_calculate_aim_prediction(e, target, &ai->target_history,
        BULLET_SPEED, &ai->predicted_target_x, &ai->predicted_target_y,
        &ai->prediction_confidence);

    /* Movement decisions */
    float ideal_range = e->archetype ? e->archetype->ideal_combat_range : 12.0f;

    /* Calculate steering forces for group behavior */
    float steer_fx, steer_fy;
    avx_calculate_steering_forces(e, game, &steer_fx, &steer_fy);

    if (threat->is_visible) {
        float dx = target->x - e->x;
        float dy = target->y - e->y;
        sse_normalize(&dx, &dy);

        if (threat->distance > ideal_range + 3.0f) {
            /* Advance with steering */
            float next_x, next_y;
            if (find_path(&game->level, e->x, e->y, target->x, target->y, &next_x, &next_y)) {
                float speed = e->max_speed;
                if (e->archetype && e->archetype->type == ARCHETYPE_RUSHER) {
                    speed *= 1.4f;
                    e->is_running = true;
                }
                e->vx = (next_x - e->x) * speed + steer_fx;
                e->vy = (next_y - e->y) * speed + steer_fy;
            }
        } else if (threat->distance < ideal_range - 3.0f) {
            /* Back away (except rushers) */
            if (!e->archetype || e->archetype->type != ARCHETYPE_RUSHER) {
                e->vx = -dx * e->max_speed * 0.6f + steer_fx;
                e->vy = -dy * e->max_speed * 0.6f + steer_fy;
            }
        } else {
            /* Strafe with group movement */
            float strafe_dir = (game->frame % 90 < 45) ? 1.0f : -1.0f;
            if (ai->assigned_flank_side != 0) {
                strafe_dir = (float)ai->assigned_flank_side;
            }
            e->vx = -dy * e->max_speed * 0.35f * strafe_dir + steer_fx;
            e->vy = dx * e->max_speed * 0.35f * strafe_dir + steer_fy;
        }

        /* Predictive shooting */
        if (threat->distance < stats.range && threat->frames_visible > 8) {
            /* Aim at predicted position */
            float aim_dx = ai->predicted_target_x - e->x;
            float aim_dy = ai->predicted_target_y - e->y;
            float aim_angle = atan2f(aim_dy, aim_dx);

            /* Only shoot if prediction confidence is good */
            if (ai->prediction_confidence > 0.5f || threat->distance < 8.0f) {
                /* Spawn bullet toward predicted position */
                if (e->fire_cooldown <= 0 && e->weapon.mag_current > 0) {
                    spawn_bullet(game, e->id, e->x, e->y, aim_angle, stats.accuracy);
                    e->weapon.mag_current--;
                    e->fire_cooldown = stats.fire_rate;

                    /* Track consecutive misses for adaptation */
                    ai->last_attack_time = (float)game->frame;
                }
            }
        }
    } else {
        /* Lost sight */
        e->state = STATE_HUNTING;
        e->alert_timer = 180;
    }
}

/* ==========================================================================
 * ADVANCED FLANKING
 * ========================================================================== */

static void execute_advanced_flanking(Entity* e, GameState* game) {
    EnemyAIState* ai = &enemy_ai_states[e->id];

    if (e->primary_threat < 0) {
        e->state = STATE_PATROL;
        return;
    }

    ThreatInfo* threat = &e->threats[e->primary_threat];

    /* Calculate optimal flanking angle using influence map */
    float best_flank_x, best_flank_y, best_value;

    /* Query influence map for best tactical position */
    inf_avx_find_best_tactical(&g_influence_map,
        threat->last_known_pos.x, threat->last_known_pos.y,
        15.0f, &best_flank_x, &best_flank_y, &best_value);

    /* Calculate flanking direction */
    float to_target_x = threat->last_known_pos.x - e->x;
    float to_target_y = threat->last_known_pos.y - e->y;
    sse_normalize(&to_target_x, &to_target_y);

    float perp_x = -to_target_y;
    float perp_y = to_target_x;

    /* Use assigned flank side or choose based on influence */
    float flank_dir = (ai->assigned_flank_side != 0) ?
        (float)ai->assigned_flank_side :
        ((game->frame % 60 < 30) ? 1.0f : -1.0f);

    /* Calculate flank target */
    float flank_x = threat->last_known_pos.x + perp_x * 10.0f * flank_dir;
    float flank_y = threat->last_known_pos.y + perp_y * 10.0f * flank_dir;

    /* Validate path */
    if (!is_walkable(&game->level, (int)flank_x, (int)flank_y)) {
        flank_dir = -flank_dir;
        flank_x = threat->last_known_pos.x + perp_x * 10.0f * flank_dir;
        flank_y = threat->last_known_pos.y + perp_y * 10.0f * flank_dir;
    }

    /* Move toward flank position */
    float next_x, next_y;
    if (find_path(&game->level, e->x, e->y, flank_x, flank_y, &next_x, &next_y)) {
        e->vx = (next_x - e->x) * e->max_speed;
        e->vy = (next_y - e->y) * e->max_speed;
        e->facing_angle = atan2f(e->vy, e->vx);
        e->is_running = true;
    }

    e->stalemate_timer++;

    /* Check if flank complete or target visible */
    float flank_dist = sse_distance(e->x, e->y, flank_x, flank_y);
    if (flank_dist < 2.0f || threat->is_visible || e->stalemate_timer > AI_FLANK_TIMEOUT) {
        e->state = STATE_COMBAT;
        e->stalemate_timer = 0;
        e->is_running = false;
    }
}

/* ==========================================================================
 * SUPPRESSION BEHAVIOR
 * ========================================================================== */

static void execute_suppression(Entity* e, GameState* game) {
    EnemyAIState* ai = &enemy_ai_states[e->id];

    if (!ai->is_suppressing || e->weapon.mag_current <= 0) {
        e->state = STATE_COMBAT;
        ai->is_suppressing = false;
        return;
    }

    /* Fire at suppression target area */
    float target_x = (float)ai->suppress_target_x;
    float target_y = (float)ai->suppress_target_y;

    float aim_dx = target_x - e->x;
    float aim_dy = target_y - e->y;
    float aim_angle = atan2f(aim_dy, aim_dx);

    /* Add spread for suppression */
    float spread = (randf(&game->rng_state) - 0.5f) * 0.3f;
    aim_angle += spread;

    e->facing_angle = aim_angle;

    /* Fire rapidly */
    if (e->fire_cooldown <= 0) {
        WeaponStats stats = weapon_get_stats(&e->weapon);
        spawn_bullet(game, e->id, e->x, e->y, aim_angle, stats.accuracy * 0.7f);
        e->weapon.mag_current--;
        e->fire_cooldown = stats.fire_rate / 2;  /* Faster fire rate for suppression */
    }

    ai->suppression_level -= 1.0f;

    /* Stop suppressing when out of ammo or done */
    if (ai->suppression_level <= 0 || e->weapon.mag_current <= 2) {
        ai->is_suppressing = false;
        e->state = STATE_RELOAD;
    }
}

/* ==========================================================================
 * BATCH UPDATE FOR MULTIPLE ENEMIES
 * ========================================================================== */

void update_enemies_batch_avx(GameState* game) {
    /* Collect enemy IDs */
    int enemy_ids[MAX_ENTITIES];
    int enemy_count = 0;

    for (int i = 0; i < game->entity_count; i++) {
        Entity* e = &game->entities[i];
        if (e->alive && e->team == 1) {
            enemy_ids[enemy_count++] = i;
        }
    }

    /* Process in batches of 8 */
    for (int batch_start = 0; batch_start < enemy_count; batch_start += 8) {
        int batch_size = (enemy_count - batch_start < 8) ?
            (enemy_count - batch_start) : 8;

        /* Extract batch state */
        BatchEntityState batch;
        avx_extract_entity_batch(game, &enemy_ids[batch_start], batch_size, &batch);

        /* Calculate batch utilities */
        BatchUtilityScores utilities;
        avx_batch_utility_scoring(&batch, game, &utilities);

        /* Select best actions */
        int actions[8];
        float values[8];
        avx_select_best_actions(&utilities, actions, values);

        /* Apply decisions to entities */
        for (int i = 0; i < batch_size; i++) {
            int eid = enemy_ids[batch_start + i];
            Entity* e = &game->entities[eid];

            /* Only update state periodically */
            if (game->frame % 30 == 0) {
                e->state = actions[i];
            }

            /* Execute the actual AI behavior (movement, shooting, etc.) */
            update_enemy_ai_advanced(game, eid);
        }
    }
}

/* ==========================================================================
 * MAIN ADVANCED AI UPDATE
 * ========================================================================== */

void update_enemy_ai_advanced(GameState* game, int entity_id) {
    Entity* e = &game->entities[entity_id];
    if (!e->alive || e->team == 0) return;

    /* Initialize AI state if needed */
    if (!ai_states_initialized) {
        memset(enemy_ai_states, 0, sizeof(enemy_ai_states));
        memset(&g_influence_map, 0, sizeof(g_influence_map));

        /* Initialize neural networks for each entity */
        uint32_t rng = 12345;
        for (int i = 0; i < MAX_ENTITIES; i++) {
            nn_init_3layer(&enemy_ai_states[i].brain,
                          NN_INPUT_SIZE, NN_HIDDEN_SIZE, NN_OUTPUT_SIZE, &rng);
        }

        ai_states_initialized = true;
    }

    EnemyAIState* ai = &enemy_ai_states[entity_id];

    /* Decrement timers */
    if (e->move_commit_timer > 0) e->move_commit_timer--;

    /* Update threat list */
    update_threat_list(e, game);

    /* Update influence map periodically */
    if (game->frame % INF_THREAT_UPDATE_FREQ == 0) {
        inf_avx_update_threat(&g_influence_map, game, 0);  /* Track player */
    }

    /* Check cover */
    e->has_cover_nearby = find_nearby_cover(&game->level, e->x, e->y,
        &e->cover_x, &e->cover_y);

    /* Update memory system */
    if (e->primary_threat >= 0) {
        const ThreatInfo* t = &e->threats[e->primary_threat];
        if (t->is_visible) {
            const Entity* target = &game->entities[t->entity_id];
            int slot = avx_memory_find_slot(&ai->memory);
            ai->memory.x[slot] = target->x;
            ai->memory.y[slot] = target->y;
            ai->memory.time[slot] = (float)game->frame;
            ai->memory.confidence[slot] = 1.0f;
            ai->memory.velocity_x[slot] = target->vx;
            ai->memory.velocity_y[slot] = target->vy;
        }
    }

    /* Decay memories */
    avx_memory_decay(&ai->memory, 0.99f);

    /* Decision making - use neural network every 30 frames */
    if (game->frame % 30 == 0 && e->state >= STATE_ALERT) {
        int nn_action = neural_net_decide_action(e, game);

        /* Blend with utility-based decision */
        float __attribute__((aligned(32))) utility_scores[8];

        /* Quick single-entity utility calculation */
        utility_scores[0] = AVX_EXTRACT_F0(avx_score_attack_8(
            _mm256_set1_ps(e->weapon.mag_current > 0 ? 1.0f : 0.0f),
            _mm256_set1_ps(e->primary_threat >= 0 && e->threats[e->primary_threat].is_visible ? 1.0f : 0.0f),
            _mm256_set1_ps(1.0f),
            _mm256_set1_ps(e->health / e->max_health),
            _mm256_set1_ps(e->archetype ? e->archetype->aggression : 0.5f),
            _mm256_set1_ps(0.0f)
        ));

        /* Use NN action if it agrees with utility, otherwise use utility */
        if (nn_action == e->state || game->frame % 90 == 0) {
            e->state = nn_action;
        }
    }

    /* Execute current state with advanced behaviors */
    switch (e->state) {
        case STATE_IDLE:
            if (randf(&game->rng_state) < 0.01f) {
                e->facing_angle += randf_range(&game->rng_state, -0.5f, 0.5f);
            }
            if (randf(&game->rng_state) < 0.005f) {
                e->state = STATE_PATROL;
            }
            break;

        case STATE_PATROL:
            execute_patrol(e, game);
            break;

        case STATE_COMBAT:
            execute_advanced_combat(e, game);
            break;

        case STATE_FLANKING:
            execute_advanced_flanking(e, game);
            break;

        case STATE_HIDING:
            execute_hiding(e, game);
            break;

        case STATE_RETREATING:
            execute_retreat(e, game);
            break;

        case STATE_HEALING:
            execute_healing(e, game);
            break;

        case STATE_RELOAD:
            execute_reload(e, game);
            break;

        case STATE_HUNTING:
            execute_hunting(e, game);
            break;

        case STATE_SUPPORTING:
            execute_supporting(e, game);
            break;

        case STATE_SUSPICIOUS:
        case STATE_ALERT:
            /* Investigate sounds/sightings */
            {
                float target_angle = atan2f(e->alert_y - e->y, e->alert_x - e->x);
                float angle_diff = target_angle - e->facing_angle;
                while (angle_diff > PI) angle_diff -= 2*PI;
                while (angle_diff < -PI) angle_diff += 2*PI;
                e->facing_angle += angle_diff * 0.1f;
            }
            e->alert_timer--;
            if (e->alert_timer <= 0) {
                e->state = STATE_PATROL;
            }
            break;

        default:
            e->state = STATE_PATROL;
            break;
    }

    /* Suppression behavior (can overlay other states) */
    if (ai->is_suppressing && e->state == STATE_COMBAT) {
        execute_suppression(e, game);
    }

    e->fire_cooldown--;
}

/* ==========================================================================
 * INITIALIZATION AND SHUTDOWN
 * ========================================================================== */

void init_enemy_ai_advanced(GameState* game) {
    (void)game;  /* May be used for future initialization */

    /* Initialize all AI states */
    memset(enemy_ai_states, 0, sizeof(enemy_ai_states));

    /* Initialize influence map - clear all layers */
    memset(&g_influence_map, 0, sizeof(g_influence_map));

    /* Initialize neural networks with small random weights */
    for (int i = 0; i < MAX_ENTITIES; i++) {
        NeuralNet* nn = &enemy_ai_states[i].brain;
        nn->layer_count = 3;

        /* Layer 0: Input (16 features) -> Hidden (16 neurons) */
        nn->layers[0].input_size = 16;
        nn->layers[0].output_size = 16;
        nn->layers[0].activation = NN_ACT_RELU;
        for (int row = 0; row < 16; row++) {
            for (int col = 0; col < 16; col++) {
                nn->layers[0].weights[row][col] = ((float)((row + col) % 17) - 8.0f) / 16.0f;
            }
            nn->layers[0].biases[row] = 0.1f;
        }

        /* Layer 1: Hidden (16) -> Hidden (8) */
        nn->layers[1].input_size = 16;
        nn->layers[1].output_size = 8;
        nn->layers[1].activation = NN_ACT_RELU;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 16; col++) {
                nn->layers[1].weights[row][col] = ((float)((row + col) % 13) - 6.0f) / 12.0f;
            }
            nn->layers[1].biases[row] = 0.1f;
        }

        /* Layer 2: Hidden (8) -> Output (6 actions) */
        nn->layers[2].input_size = 8;
        nn->layers[2].output_size = 6;
        nn->layers[2].activation = NN_ACT_SOFTMAX;
        for (int row = 0; row < 6; row++) {
            for (int col = 0; col < 8; col++) {
                nn->layers[2].weights[row][col] = ((float)((row + col) % 11) - 5.0f) / 10.0f;
            }
            nn->layers[2].biases[row] = 0.0f;
        }

        /* Initialize behavior tree */
        enemy_ai_states[i].behavior_tree.root = -1;  /* No root node */
        enemy_ai_states[i].behavior_tree.node_count = 0;
        memset(&enemy_ai_states[i].blackboard, 0, sizeof(BTBlackboard));

        /* Initialize memory */
        memset(&enemy_ai_states[i].memory, 0, sizeof(AIMemory));
    }

    ai_states_initialized = true;
}

void shutdown_enemy_ai_advanced(void) {
    /* Clear all AI states */
    memset(enemy_ai_states, 0, sizeof(enemy_ai_states));

    /* Clear influence map */
    memset(&g_influence_map, 0, sizeof(g_influence_map));

    ai_states_initialized = false;
}
