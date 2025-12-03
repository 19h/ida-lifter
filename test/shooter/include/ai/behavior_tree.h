/*
 * Behavior Tree System with AVX Batch Evaluation
 * Implements a hierarchical behavior tree for sophisticated AI decision making.
 * Uses AVX/AVX2 for batch node evaluation where applicable.
 */

#ifndef SHOOTER_BEHAVIOR_TREE_H
#define SHOOTER_BEHAVIOR_TREE_H

#include <immintrin.h>
#include <stdbool.h>
#include "../config.h"
#include "../types.h"

/* ==========================================================================
 * BEHAVIOR TREE NODE TYPES
 * ========================================================================== */

typedef enum {
    BT_SUCCESS = 0,
    BT_FAILURE = 1,
    BT_RUNNING = 2
} BTStatus;

typedef enum {
    /* Control flow nodes */
    BT_NODE_SEQUENCE,          /* Run children in order until one fails */
    BT_NODE_SELECTOR,          /* Run children until one succeeds */
    BT_NODE_PARALLEL,          /* Run all children simultaneously */
    BT_NODE_DECORATOR,         /* Modify child's result */
    BT_NODE_RANDOM_SELECTOR,   /* Randomly pick a child */

    /* Condition nodes (leaf) */
    BT_NODE_COND_CAN_SEE_TARGET,
    BT_NODE_COND_TARGET_IN_RANGE,
    BT_NODE_COND_HAS_AMMO,
    BT_NODE_COND_HEALTH_LOW,
    BT_NODE_COND_HEALTH_CRITICAL,
    BT_NODE_COND_HAS_COVER,
    BT_NODE_COND_BEING_TARGETED,
    BT_NODE_COND_ALLY_NEEDS_HELP,
    BT_NODE_COND_OUTNUMBERED,
    BT_NODE_COND_HAS_MEDPEN,
    BT_NODE_COND_IN_STALEMATE,
    BT_NODE_COND_TARGET_FLANKING,
    BT_NODE_COND_SUPPRESSED,
    BT_NODE_COND_TARGET_RETREATING,
    BT_NODE_COND_HEARD_SOUND,

    /* Action nodes (leaf) */
    BT_NODE_ACT_ATTACK,
    BT_NODE_ACT_MOVE_TO_TARGET,
    BT_NODE_ACT_TAKE_COVER,
    BT_NODE_ACT_RELOAD,
    BT_NODE_ACT_HEAL,
    BT_NODE_ACT_RETREAT,
    BT_NODE_ACT_FLANK,
    BT_NODE_ACT_PATROL,
    BT_NODE_ACT_INVESTIGATE,
    BT_NODE_ACT_SUPPORT_ALLY,
    BT_NODE_ACT_SUPPRESS,
    BT_NODE_ACT_HUNT,
    BT_NODE_ACT_AMBUSH,
    BT_NODE_ACT_STRAFE,
    BT_NODE_ACT_COMMUNICATE,  /* Alert squad */
    BT_NODE_ACT_IDLE,

    BT_NODE_COUNT
} BTNodeType;

/* ==========================================================================
 * BEHAVIOR TREE NODE STRUCTURE
 * ========================================================================== */

#define BT_MAX_CHILDREN 8
#define BT_MAX_NODES 64

typedef struct BTNode BTNode;

struct BTNode {
    BTNodeType type;
    int children[BT_MAX_CHILDREN];   /* Indices into node array */
    int child_count;
    int parent;
    float weight;                     /* For weighted random selection */
    float parameter;                  /* Generic parameter (threshold, etc) */
    BTStatus last_status;
    int running_child;               /* For resuming sequences/selectors */
};

typedef struct {
    BTNode nodes[BT_MAX_NODES];
    int node_count;
    int root;
    BTStatus tree_status;
    int current_running_node;
} BehaviorTree;

/* ==========================================================================
 * BLACKBOARD - Shared state for behavior tree
 * ========================================================================== */

typedef struct __attribute__((aligned(32))) {
    /* Self state */
    float health_ratio;
    float ammo_ratio;
    float stamina_ratio;
    int medpen_count;
    bool has_cover;
    bool is_crouching;
    bool is_running;

    /* Target state */
    bool has_target;
    bool can_see_target;
    bool target_can_see_us;
    float target_distance;
    float target_health_ratio;
    bool target_in_range;
    bool target_retreating;
    bool target_flanking;

    /* Combat state */
    int threat_count;
    bool being_suppressed;
    int frames_in_combat;
    int frames_target_visible;
    int stalemate_timer;
    float damage_taken_recently;

    /* Squad state */
    bool has_squad;
    bool ally_needs_help;
    int squad_size;
    float squad_avg_health;

    /* Environment */
    float nearest_cover_dist;
    bool heard_sound;
    float sound_direction_x;
    float sound_direction_y;

    /* Computed by AVX batch ops */
    float attack_utility;
    float defend_utility;
    float flank_utility;
    float retreat_utility;
    float heal_utility;
    float reload_utility;
} BTBlackboard;

/* ==========================================================================
 * AVX BATCH CONDITION EVALUATION
 * Evaluate multiple conditions across 8 entities simultaneously
 * ========================================================================== */

typedef struct __attribute__((aligned(32))) {
    /* Entity state arrays (8 entities) */
    float health_ratio[8];
    float ammo_ratio[8];
    float target_distance[8];
    float cover_distance[8];
    float threat_count[8];
    float stalemate_time[8];
    float damage_taken[8];

    /* Boolean conditions as float masks (1.0 or 0.0) */
    float can_see_target[8];
    float has_cover[8];
    float has_medpen[8];
    float has_squad[8];
    float being_targeted[8];
    float target_in_range[8];
    float heard_sound[8];
} BatchBlackboard;

/* Batch evaluate "health low" condition for 8 entities */
static inline __m256 bt_avx_cond_health_low_8(const BatchBlackboard* bb, float threshold) {
    __m256 health = _mm256_loadu_ps(bb->health_ratio);
    __m256 thresh = _mm256_set1_ps(threshold);
    return _mm256_cmp_ps(health, thresh, _CMP_LT_OQ);
}

/* Batch evaluate "can attack" for 8 entities */
static inline __m256 bt_avx_cond_can_attack_8(const BatchBlackboard* bb) {
    __m256 can_see = _mm256_loadu_ps(bb->can_see_target);
    __m256 ammo = _mm256_loadu_ps(bb->ammo_ratio);
    __m256 in_range = _mm256_loadu_ps(bb->target_in_range);

    __m256 has_ammo = _mm256_cmp_ps(ammo, _mm256_setzero_ps(), _CMP_GT_OQ);

    /* Can attack if can_see AND has_ammo AND in_range */
    return _mm256_and_ps(_mm256_and_ps(can_see, has_ammo), in_range);
}

/* Batch evaluate "should take cover" for 8 entities */
static inline __m256 bt_avx_cond_should_cover_8(const BatchBlackboard* bb) {
    __m256 health = _mm256_loadu_ps(bb->health_ratio);
    __m256 has_cover = _mm256_loadu_ps(bb->has_cover);
    __m256 being_targeted = _mm256_loadu_ps(bb->being_targeted);
    __m256 damage = _mm256_loadu_ps(bb->damage_taken);

    /* Need cover if: low health AND cover available OR being targeted with damage */
    __m256 low_health = _mm256_cmp_ps(health, _mm256_set1_ps(0.5f), _CMP_LT_OQ);
    __m256 health_need = _mm256_and_ps(low_health, has_cover);

    __m256 has_damage = _mm256_cmp_ps(damage, _mm256_set1_ps(10.0f), _CMP_GT_OQ);
    __m256 combat_need = _mm256_and_ps(_mm256_and_ps(being_targeted, has_damage), has_cover);

    return _mm256_or_ps(health_need, combat_need);
}

/* Batch evaluate "should retreat" for 8 entities */
static inline __m256 bt_avx_cond_should_retreat_8(const BatchBlackboard* bb) {
    __m256 health = _mm256_loadu_ps(bb->health_ratio);
    __m256 threat_count = _mm256_loadu_ps(bb->threat_count);

    /* Retreat if: health < 0.25 OR (health < 0.5 AND threats >= 3) */
    __m256 critical = _mm256_cmp_ps(health, _mm256_set1_ps(0.25f), _CMP_LT_OQ);
    __m256 low = _mm256_cmp_ps(health, _mm256_set1_ps(0.5f), _CMP_LT_OQ);
    __m256 outnumbered = _mm256_cmp_ps(threat_count, _mm256_set1_ps(2.5f), _CMP_GT_OQ);

    return _mm256_or_ps(critical, _mm256_and_ps(low, outnumbered));
}

/* Batch evaluate "should heal" for 8 entities */
static inline __m256 bt_avx_cond_should_heal_8(const BatchBlackboard* bb) {
    __m256 health = _mm256_loadu_ps(bb->health_ratio);
    __m256 has_medpen = _mm256_loadu_ps(bb->has_medpen);
    __m256 cover_dist = _mm256_loadu_ps(bb->cover_distance);
    __m256 can_see = _mm256_loadu_ps(bb->can_see_target);

    /* Heal if: has_medpen AND (health < 0.4 OR (health < 0.7 AND safe)) */
    __m256 need_heal = _mm256_cmp_ps(health, _mm256_set1_ps(0.4f), _CMP_LT_OQ);
    __m256 want_heal = _mm256_cmp_ps(health, _mm256_set1_ps(0.7f), _CMP_LT_OQ);
    __m256 safe = _mm256_andnot_ps(can_see, _mm256_set1_ps(-0.0f));  /* NOT can_see */
    safe = _mm256_cmp_ps(_mm256_andnot_ps(can_see, _mm256_set1_ps(1.0f)),
                         _mm256_setzero_ps(), _CMP_GT_OQ);

    __m256 should_heal = _mm256_or_ps(need_heal, _mm256_and_ps(want_heal, safe));
    return _mm256_and_ps(should_heal, has_medpen);
}

/* Batch evaluate "in stalemate" for 8 entities */
static inline __m256 bt_avx_cond_stalemate_8(const BatchBlackboard* bb, float threshold) {
    __m256 stale = _mm256_loadu_ps(bb->stalemate_time);
    __m256 thresh = _mm256_set1_ps(threshold);
    return _mm256_cmp_ps(stale, thresh, _CMP_GT_OQ);
}

/* ==========================================================================
 * AVX BATCH UTILITY CALCULATION FOR BEHAVIOR SELECTION
 * ========================================================================== */

typedef struct __attribute__((aligned(32))) {
    float utility[8];
    int action[8];
} BatchDecision;

/* Calculate utilities for all primary actions and select best */
static inline void bt_avx_select_behavior_8(
    const BatchBlackboard* bb,
    float aggression[8],
    float courage[8],
    BatchDecision* out
) {
    __m256 health = _mm256_loadu_ps(bb->health_ratio);
    __m256 ammo = _mm256_loadu_ps(bb->ammo_ratio);
    __m256 can_see = _mm256_loadu_ps(bb->can_see_target);
    __m256 has_cover = _mm256_loadu_ps(bb->has_cover);
    __m256 threats = _mm256_loadu_ps(bb->threat_count);
    __m256 has_medpen = _mm256_loadu_ps(bb->has_medpen);
    __m256 in_range = _mm256_loadu_ps(bb->target_in_range);
    __m256 vaggro = _mm256_loadu_ps(aggression);
    __m256 vcourage = _mm256_loadu_ps(courage);

    __m256 one = _mm256_set1_ps(1.0f);
    __m256 half = _mm256_set1_ps(0.5f);

    /* Attack utility */
    __m256 attack_util = _mm256_mul_ps(can_see, in_range);
    attack_util = _mm256_mul_ps(attack_util, _mm256_add_ps(half, vaggro));
    attack_util = _mm256_mul_ps(attack_util, _mm256_add_ps(half, ammo));
    __m256 low_health = _mm256_cmp_ps(health, _mm256_set1_ps(0.3f), _CMP_LT_OQ);
    attack_util = _mm256_blendv_ps(attack_util,
        _mm256_mul_ps(attack_util, half), low_health);

    /* Defend utility */
    __m256 defend_util = _mm256_mul_ps(has_cover,
        _mm256_sub_ps(_mm256_set1_ps(2.0f), health));
    defend_util = _mm256_mul_ps(defend_util,
        _mm256_sub_ps(_mm256_set1_ps(1.5f), vaggro));

    /* Retreat utility */
    __m256 crit_health = _mm256_cmp_ps(health, _mm256_set1_ps(0.25f), _CMP_LT_OQ);
    __m256 retreat_util = _mm256_blendv_ps(
        _mm256_mul_ps(_mm256_sub_ps(_mm256_set1_ps(1.5f), vcourage),
                      _mm256_mul_ps(threats, _mm256_set1_ps(0.3f))),
        _mm256_set1_ps(3.0f),
        crit_health
    );

    /* Heal utility */
    __m256 heal_util = _mm256_mul_ps(has_medpen,
        _mm256_sub_ps(_mm256_set1_ps(1.5f), health));
    __m256 very_low = _mm256_cmp_ps(health, _mm256_set1_ps(0.3f), _CMP_LT_OQ);
    heal_util = _mm256_blendv_ps(heal_util,
        _mm256_mul_ps(heal_util, _mm256_set1_ps(3.0f)), very_low);

    /* Reload utility */
    __m256 need_reload = _mm256_cmp_ps(ammo, _mm256_set1_ps(0.01f), _CMP_LT_OQ);
    __m256 reload_util = _mm256_blendv_ps(
        _mm256_mul_ps(_mm256_sub_ps(one, ammo), _mm256_set1_ps(2.0f)),
        _mm256_set1_ps(5.0f),
        need_reload
    );

    /* Find best action using AVX comparisons */
    __m256 best_util = attack_util;
    __m256i best_action = _mm256_set1_epi32(BT_NODE_ACT_ATTACK);

    __m256 cmp = _mm256_cmp_ps(defend_util, best_util, _CMP_GT_OQ);
    best_util = _mm256_blendv_ps(best_util, defend_util, cmp);
    best_action = _mm256_blendv_epi8(best_action,
        _mm256_set1_epi32(BT_NODE_ACT_TAKE_COVER), _mm256_castps_si256(cmp));

    cmp = _mm256_cmp_ps(retreat_util, best_util, _CMP_GT_OQ);
    best_util = _mm256_blendv_ps(best_util, retreat_util, cmp);
    best_action = _mm256_blendv_epi8(best_action,
        _mm256_set1_epi32(BT_NODE_ACT_RETREAT), _mm256_castps_si256(cmp));

    cmp = _mm256_cmp_ps(heal_util, best_util, _CMP_GT_OQ);
    best_util = _mm256_blendv_ps(best_util, heal_util, cmp);
    best_action = _mm256_blendv_epi8(best_action,
        _mm256_set1_epi32(BT_NODE_ACT_HEAL), _mm256_castps_si256(cmp));

    cmp = _mm256_cmp_ps(reload_util, best_util, _CMP_GT_OQ);
    best_util = _mm256_blendv_ps(best_util, reload_util, cmp);
    best_action = _mm256_blendv_epi8(best_action,
        _mm256_set1_epi32(BT_NODE_ACT_RELOAD), _mm256_castps_si256(cmp));

    _mm256_storeu_ps(out->utility, best_util);
    STOREU_SI256(out->action, best_action);
}

/* ==========================================================================
 * BEHAVIOR TREE API
 * ========================================================================== */

/* Initialize a behavior tree */
void bt_init(BehaviorTree* tree);

/* Add a node to the tree */
int bt_add_node(BehaviorTree* tree, BTNodeType type, int parent, float weight, float param);

/* Set tree root */
void bt_set_root(BehaviorTree* tree, int node_idx);

/* Execute one tick of the behavior tree */
BTStatus bt_tick(BehaviorTree* tree, Entity* entity, GameState* game, BTBlackboard* bb);

/* Update blackboard from entity state */
void bt_update_blackboard(BTBlackboard* bb, const Entity* entity, const GameState* game);

/* Create preset behavior trees for different archetypes */
void bt_create_grunt_tree(BehaviorTree* tree);
void bt_create_sniper_tree(BehaviorTree* tree);
void bt_create_rusher_tree(BehaviorTree* tree);
void bt_create_heavy_tree(BehaviorTree* tree);
void bt_create_player_tree(BehaviorTree* tree);

/* ==========================================================================
 * AVX BEHAVIOR TREE PARALLEL TICK
 * Process 8 entities' behavior trees in parallel where possible
 * ========================================================================== */

typedef struct {
    BehaviorTree* trees[8];
    Entity* entities[8];
    BTBlackboard blackboards[8];
    int count;
} BTBatchContext;

/* Batch update all blackboards using AVX */
void bt_batch_update_blackboards(BTBatchContext* ctx, const GameState* game);

/* Batch evaluate condition nodes across all entities */
void bt_batch_evaluate_conditions(BTBatchContext* ctx, BatchBlackboard* batch_bb);

/* Execute parallel behavior decisions */
void bt_batch_tick(BTBatchContext* ctx, GameState* game);

#endif /* SHOOTER_BEHAVIOR_TREE_H */
