/*
 * Enemy AI System Implementation
 * Utility-based AI with tactical decision making, squad coordination, and steering behaviors.
 */

#include "ai/enemy_ai.h"
#include "config.h"
#include "core/rng.h"
#include "level/level.h"
#include "combat/bullet.h"
#include "entity/entity.h"
#include "math/sse_math.h"
#include <math.h>
#include <string.h>

/* ==========================================================================
 * UTILITY AI - DECISION SCORING
 * ========================================================================== */

typedef struct {
    float attack;
    float take_cover;
    float flank;
    float retreat;
    float heal;
    float reload;
    float investigate;
    float patrol;
    float support_ally;
} ActionScores;

/* Calculate utility score for attacking */
static float score_attack(const Entity* e, const GameState* game) {
    if (e->primary_threat < 0) return 0.0f;

    const ThreatInfo* threat = &e->threats[e->primary_threat];
    if (!threat->is_visible) return 0.0f;

    float score = e->ai_weights.attack;

    /* Boost if target is visible and we have ammo */
    if (e->weapon.mag_current > 0) {
        score *= 1.5f;
    } else {
        score *= 0.1f;  /* No ammo, low priority */
    }

    /* Boost if target is within weapon range */
    WeaponStats stats = weapon_get_stats(&e->weapon);
    if (threat->distance < stats.range) {
        score *= 1.3f;
    }

    /* Archetype aggression boost */
    if (e->archetype) {
        score *= (0.5f + e->archetype->aggression);
    }

    /* Boost if we have sight advantage */
    if (!threat->is_aware_of_us) {
        score *= 1.5f;  /* Ambush opportunity */
    }

    /* Reduce if health is low */
    float health_ratio = e->health / e->max_health;
    if (health_ratio < 0.3f) {
        score *= 0.5f;
    }

    return score;
}

/* Calculate utility score for taking cover */
static float score_take_cover(const Entity* e, const GameState* game) {
    if (!e->has_cover_nearby) return 0.0f;

    float score = e->ai_weights.defend;

    /* Higher priority when health is low */
    float health_ratio = e->health / e->max_health;
    score *= (2.0f - health_ratio);  /* 1x at full health, 2x at 0 health */

    /* Higher if we're being targeted */
    if (e->primary_threat >= 0) {
        const ThreatInfo* threat = &e->threats[e->primary_threat];
        if (threat->is_aware_of_us) {
            score *= 1.5f;
        }
    }

    /* Lower for aggressive archetypes */
    if (e->archetype) {
        score *= (1.5f - e->archetype->aggression);
    }

    /* Higher when recently damaged */
    if (e->damage_react_timer > 0) {
        score *= 1.8f;
    }

    return score;
}

/* Calculate utility score for flanking */
static float score_flank(const Entity* e, const GameState* game) {
    if (e->primary_threat < 0) return 0.0f;

    const ThreatInfo* threat = &e->threats[e->primary_threat];

    float score = e->ai_weights.flank;

    /* Only flank if we know where they are but can't see them */
    if (threat->is_visible) {
        score *= 0.3f;  /* Already visible, no need to flank */
    } else if (threat->frames_visible > 0) {
        score *= 1.5f;  /* Recently visible, good time to flank */
    }

    /* Higher for aggressive archetypes */
    if (e->archetype) {
        score *= (0.5f + e->archetype->aggression);
    }

    /* Stalemate boost */
    if (e->stalemate_timer > 60) {
        score *= 2.0f;
    }

    return score;
}

/* Calculate utility score for retreating */
static float score_retreat(const Entity* e, const GameState* game) {
    float score = e->ai_weights.retreat;

    float health_ratio = e->health / e->max_health;

    /* High priority when health is critical */
    if (health_ratio < AI_RETREAT_HEALTH_PCTG) {
        score *= 3.0f;
    } else if (health_ratio < 0.5f) {
        score *= 1.5f;
    }

    /* Lower for courageous archetypes */
    if (e->archetype) {
        score *= (1.5f - e->archetype->courage);
    }

    /* Squad presence reduces retreat desire */
    if (e->squad_id >= 0) {
        score *= (1.0f - AI_COURAGE_BONUS_SQUAD);
    }

    /* Higher when outnumbered */
    if (e->threat_count > 1) {
        score *= 1.3f * e->threat_count;
    }

    return score;
}

/* Calculate utility score for healing */
static float score_heal(const Entity* e, const GameState* game) {
    if (e->medpens <= 0) return 0.0f;

    float score = e->ai_weights.heal;

    float health_ratio = e->health / e->max_health;

    /* Higher priority when health is low */
    if (health_ratio < 0.3f) {
        score *= 3.0f;
    } else if (health_ratio < 0.5f) {
        score *= 2.0f;
    } else if (health_ratio > 0.8f) {
        score *= 0.1f;  /* Don't heal when near full */
    }

    /* Prefer healing when in cover */
    if (e->has_cover_nearby) {
        score *= 1.5f;
    }

    /* Avoid healing if enemies are very close */
    if (e->primary_threat >= 0) {
        const ThreatInfo* threat = &e->threats[e->primary_threat];
        if (threat->distance < 5.0f) {
            score *= 0.3f;
        }
    }

    return score;
}

/* Calculate utility score for reloading */
static float score_reload(const Entity* e, const GameState* game) {
    if (e->weapon.reserve <= 0) return 0.0f;

    WeaponStats stats = weapon_get_stats(&e->weapon);
    float ammo_ratio = (float)e->weapon.mag_current / (float)stats.mag_size;

    float score = 0.0f;

    if (e->weapon.mag_current <= 0) {
        score = 5.0f;  /* Must reload, empty mag */
    } else if (ammo_ratio < 0.25f) {
        score = 2.0f;  /* Low ammo */
    } else if (ammo_ratio < 0.5f) {
        score = 1.0f;  /* Half empty */
    } else {
        score = 0.1f;  /* Still have ammo */
    }

    /* Prefer reloading when in cover */
    if (e->has_cover_nearby) {
        score *= 1.3f;
    }

    /* Avoid reloading when enemy is very close and visible */
    if (e->primary_threat >= 0) {
        const ThreatInfo* threat = &e->threats[e->primary_threat];
        if (threat->is_visible && threat->distance < 8.0f) {
            score *= 0.3f;
        }
    }

    return score;
}

/* Calculate utility score for investigating */
static float score_investigate(const Entity* e, const GameState* game) {
    if (e->alert_timer <= 0) return 0.0f;

    float score = e->ai_weights.investigate;

    /* Higher when recently alerted */
    if (e->alert_timer > 120) {
        score *= 1.5f;
    }

    /* Lower when in combat */
    if (e->state == STATE_COMBAT) {
        score *= 0.2f;
    }

    return score;
}

/* Calculate utility score for patrolling */
static float score_patrol(const Entity* e, const GameState* game) {
    float score = e->ai_weights.patrol;

    /* Only patrol when no threats */
    if (e->threat_count > 0) {
        score *= 0.1f;
    }

    /* Lower when alerted */
    if (e->alert_timer > 0) {
        score *= 0.3f;
    }

    return score;
}

/* Calculate utility score for supporting an ally */
static float score_support_ally(const Entity* e, const GameState* game) {
    if (e->squad_id < 0) return 0.0f;

    float score = e->ai_weights.support;

    /* Check if any squad member needs help */
    const SquadInfo* squad = &game->squads.squads[e->squad_id];
    bool ally_in_trouble = false;

    for (int i = 0; i < squad->member_count; i++) {
        int ally_id = squad->member_ids[i];
        if (ally_id == e->id) continue;

        const Entity* ally = &game->entities[ally_id];
        if (!ally->alive) continue;

        float ally_health_ratio = ally->health / ally->max_health;
        if (ally_health_ratio < 0.3f) {
            ally_in_trouble = true;
            break;
        }
    }

    if (ally_in_trouble) {
        score *= 2.0f;
    } else {
        score *= 0.3f;
    }

    return score;
}

/* Choose best action based on utility scores */
static int choose_best_action(const ActionScores* scores) {
    float best = scores->attack;
    int action = STATE_COMBAT;

    if (scores->take_cover > best) {
        best = scores->take_cover;
        action = STATE_HIDING;
    }
    if (scores->flank > best) {
        best = scores->flank;
        action = STATE_FLANKING;
    }
    if (scores->retreat > best) {
        best = scores->retreat;
        action = STATE_RETREATING;
    }
    if (scores->heal > best) {
        best = scores->heal;
        action = STATE_HEALING;
    }
    if (scores->reload > best) {
        best = scores->reload;
        action = STATE_RELOAD;
    }
    if (scores->investigate > best) {
        best = scores->investigate;
        action = STATE_ALERT;
    }
    if (scores->patrol > best) {
        best = scores->patrol;
        action = STATE_PATROL;
    }
    if (scores->support_ally > best) {
        best = scores->support_ally;
        action = STATE_SUPPORTING;
    }

    return action;
}

/* ==========================================================================
 * STATE EXECUTION
 * ========================================================================== */

/* Helper to apply committed movement toward a target */
static void apply_committed_movement(Entity* e, const Level* level, float target_x, float target_y, float speed) {
    /* Only recalculate path if commit timer expired or no target set */
    if (e->move_commit_timer <= 0 || (e->move_target_x == 0 && e->move_target_y == 0)) {
        float next_x, next_y;
        if (find_path(level, e->x, e->y, target_x, target_y, &next_x, &next_y)) {
            e->move_target_x = next_x;
            e->move_target_y = next_y;
            e->move_commit_timer = 10;  /* Commit for 10 frames */
        }
    }

    /* Move toward committed target */
    if (e->move_target_x != 0 || e->move_target_y != 0) {
        float dx = e->move_target_x - e->x;
        float dy = e->move_target_y - e->y;
        float dist = sse_distance(0, 0, dx, dy);

        if (dist < 0.5f) {
            /* Reached committed target, allow new path calc */
            e->move_commit_timer = 0;
            e->move_target_x = 0;
            e->move_target_y = 0;
        } else {
            e->vx = (dx / dist) * speed;
            e->vy = (dy / dist) * speed;
            e->facing_angle = atan2f(e->vy, e->vx);
        }
    }
}

static void execute_patrol(Entity* e, GameState* game) {
    /* Pick a destination if needed */
    if (e->patrol_x == 0 && e->patrol_y == 0) {
        for (int attempts = 0; attempts < 10; attempts++) {
            int room_idx = randi_range(&game->rng_state, 0, game->level.room_count - 1);
            Room* r = &game->level.rooms[room_idx];
            float px = r->x + 1 + randf(&game->rng_state) * (r->width - 2);
            float py = r->y + 1 + randf(&game->rng_state) * (r->height - 2);
            if (is_walkable(&game->level, (int)px, (int)py)) {
                e->patrol_x = px;
                e->patrol_y = py;
                e->move_target_x = 0;
                e->move_target_y = 0;
                e->move_commit_timer = 0;
                e->stalemate_timer = 0;
                break;
            }
        }
    }

    float pdx = e->patrol_x - e->x;
    float pdy = e->patrol_y - e->y;
    float pdist = sse_distance(0, 0, pdx, pdy);

    if (pdist < 2.0f) {
        e->patrol_x = 0;
        e->patrol_y = 0;
        e->move_target_x = 0;
        e->move_target_y = 0;
        e->stalemate_timer = 0;
        /* Occasionally stop and idle */
        if (randf(&game->rng_state) < 0.3f) {
            e->state = STATE_IDLE;
        }
    } else {
        apply_committed_movement(e, &game->level, e->patrol_x, e->patrol_y, e->max_speed * 0.6f);
    }
}

static void execute_combat(Entity* e, GameState* game) {
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

    /* Face the target */
    if (threat->is_visible) {
        e->facing_angle = threat->angle_to;
        e->last_seen_x = target->x;
        e->last_seen_y = target->y;
        threat->last_known_pos.x = target->x;
        threat->last_known_pos.y = target->y;
    }

    /* Movement based on archetype ideal range */
    float ideal_range = e->archetype ? e->archetype->ideal_combat_range : 12.0f;

    WeaponStats stats = weapon_get_stats(&e->weapon);
    float norm_dx = target->x - e->x;
    float norm_dy = target->y - e->y;
    sse_normalize(&norm_dx, &norm_dy);

    if (threat->is_visible) {
        if (threat->distance > ideal_range + 2.0f) {
            /* Advance toward target */
            float next_x, next_y;
            if (find_path(&game->level, e->x, e->y, target->x, target->y, &next_x, &next_y)) {
                float speed = e->max_speed;
                if (e->archetype && e->archetype->type == ARCHETYPE_RUSHER) {
                    speed *= 1.3f;  /* Rushers run */
                    e->is_running = true;
                }
                e->vx = (next_x - e->x) * speed;
                e->vy = (next_y - e->y) * speed;
            }
        } else if (threat->distance < ideal_range - 2.0f &&
                   e->archetype && e->archetype->type != ARCHETYPE_RUSHER) {
            /* Back away (except rushers) */
            e->vx = -norm_dx * e->max_speed * 0.5f;
            e->vy = -norm_dy * e->max_speed * 0.5f;
        } else {
            /* Strafe */
            float strafe_dir = (game->frame % 90 < 45) ? 1.0f : -1.0f;
            e->vx = -norm_dy * e->max_speed * 0.3f * strafe_dir;
            e->vy = norm_dx * e->max_speed * 0.3f * strafe_dir;
        }

        /* Fire if in range and have been tracking */
        if (threat->distance < stats.range && threat->frames_visible > 10) {
            fire_weapon(game, e->id);
        }
    } else {
        /* Lost sight - move toward last known position */
        e->state = STATE_HUNTING;
        e->alert_timer = 180;
    }
}

static void execute_hunting(Entity* e, GameState* game) {
    e->stalemate_timer++;

    float hdx = e->last_seen_x - e->x;
    float hdy = e->last_seen_y - e->y;
    float hdist = sse_distance(0, 0, hdx, hdy);

    if (hdist < 3.0f) {
        /* Reached last known position */
        e->stalemate_timer++;
        e->move_target_x = 0;
        e->move_target_y = 0;

        if (e->stalemate_timer > 30) {
            /* Search around */
            e->facing_angle += 0.15f;
            e->alert_timer--;
        }
    } else {
        apply_committed_movement(e, &game->level, e->last_seen_x, e->last_seen_y, e->max_speed * 0.9f);
    }

    e->alert_timer--;

    /* Found target again? */
    if (e->primary_threat >= 0 && e->threats[e->primary_threat].is_visible) {
        e->state = STATE_COMBAT;
        e->move_target_x = 0;
        e->move_target_y = 0;
        e->stalemate_timer = 0;
    } else if (e->alert_timer <= 0) {
        e->state = STATE_PATROL;
        e->patrol_x = 0;
        e->patrol_y = 0;
        e->stalemate_timer = 0;
    }
}

static void execute_hiding(Entity* e, GameState* game) {
    e->is_crouching = true;
    e->stalemate_timer++;

    /* Move toward cover */
    if (e->has_cover_nearby) {
        float cdx = e->cover_x - e->x;
        float cdy = e->cover_y - e->y;
        float cdist = sse_distance(0, 0, cdx, cdy);

        if (cdist > 1.0f) {
            sse_normalize(&cdx, &cdy);
            e->vx = cdx * e->max_speed * 0.5f;
            e->vy = cdy * e->max_speed * 0.5f;
        } else {
            e->vx *= 0.3f;
            e->vy *= 0.3f;
        }
    }

    /* Peek at threat */
    if (e->primary_threat >= 0) {
        ThreatInfo* threat = &e->threats[e->primary_threat];

        float target_angle = atan2f(threat->last_known_pos.y - e->y,
                                    threat->last_known_pos.x - e->x);
        float angle_diff = target_angle - e->facing_angle;
        while (angle_diff > PI) angle_diff -= 2*PI;
        while (angle_diff < -PI) angle_diff += 2*PI;
        e->facing_angle += angle_diff * 0.1f;
    }

    /* Timeout - re-evaluate */
    if (e->stalemate_timer > 90) {
        e->state = STATE_COMBAT;
        e->is_crouching = false;
        e->stalemate_timer = 0;
    }
}

static void execute_flanking(Entity* e, GameState* game) {
    if (e->primary_threat < 0) {
        e->state = STATE_PATROL;
        return;
    }

    ThreatInfo* threat = &e->threats[e->primary_threat];

    /* Only recalculate flank target occasionally */
    if (e->move_commit_timer <= 0 || (e->move_target_x == 0 && e->move_target_y == 0)) {
        /* Calculate flanking direction (perpendicular to target) */
        float to_target_x = threat->last_known_pos.x - e->x;
        float to_target_y = threat->last_known_pos.y - e->y;
        sse_normalize(&to_target_x, &to_target_y);

        float perp_x = -to_target_y;
        float perp_y = to_target_x;

        /* Choose direction based on which side has more space */
        float test_x1 = e->x + perp_x * 5.0f;
        float test_y1 = e->y + perp_y * 5.0f;

        if (!is_walkable(&game->level, (int)test_x1, (int)test_y1)) {
            perp_x = -perp_x;
            perp_y = -perp_y;
        }

        /* Move perpendicular + forward */
        float flank_x = e->x + perp_x * 8.0f + to_target_x * 4.0f;
        float flank_y = e->y + perp_y * 8.0f + to_target_y * 4.0f;

        float next_x, next_y;
        if (find_path(&game->level, e->x, e->y, flank_x, flank_y, &next_x, &next_y)) {
            e->move_target_x = next_x;
            e->move_target_y = next_y;
            e->move_commit_timer = 15;
        }
    }

    /* Move toward committed target */
    if (e->move_target_x != 0 || e->move_target_y != 0) {
        float dx = e->move_target_x - e->x;
        float dy = e->move_target_y - e->y;
        float dist = sse_distance(0, 0, dx, dy);
        if (dist > 0.5f) {
            e->vx = (dx / dist) * e->max_speed;
            e->vy = (dy / dist) * e->max_speed;
            e->facing_angle = atan2f(e->vy, e->vx);
            e->is_running = true;
        } else {
            e->move_commit_timer = 0;
            e->move_target_x = 0;
            e->move_target_y = 0;
        }
    }

    e->stalemate_timer++;

    /* Re-evaluate after flanking attempt */
    if (e->stalemate_timer > AI_FLANK_TIMEOUT || threat->is_visible) {
        e->state = STATE_COMBAT;
        e->stalemate_timer = 0;
        e->is_running = false;
        e->move_target_x = 0;
        e->move_target_y = 0;
    }
}

static void execute_retreat(Entity* e, GameState* game) {
    /* Run away from primary threat */
    if (e->primary_threat >= 0) {
        ThreatInfo* threat = &e->threats[e->primary_threat];

        /* Only recalculate retreat target occasionally */
        if (e->move_commit_timer <= 0 || (e->move_target_x == 0 && e->move_target_y == 0)) {
            /* Find cover away from threat */
            float retreat_x = e->x - (threat->last_known_pos.x - e->x);
            float retreat_y = e->y - (threat->last_known_pos.y - e->y);

            float next_x, next_y;
            if (find_path(&game->level, e->x, e->y, retreat_x, retreat_y, &next_x, &next_y)) {
                e->move_target_x = next_x;
                e->move_target_y = next_y;
                e->move_commit_timer = 15;
            }
        }

        /* Move toward committed target */
        if (e->move_target_x != 0 || e->move_target_y != 0) {
            float dx = e->move_target_x - e->x;
            float dy = e->move_target_y - e->y;
            float dist = sse_distance(0, 0, dx, dy);
            if (dist > 0.5f) {
                e->vx = (dx / dist) * e->max_speed;
                e->vy = (dy / dist) * e->max_speed;
                e->is_running = true;
            } else {
                e->move_commit_timer = 0;
                e->move_target_x = 0;
                e->move_target_y = 0;
            }
        }
    }

    e->stalemate_timer++;

    /* Stop retreating when safe or exhausted */
    if (e->stalemate_timer > 90 ||
        (e->primary_threat >= 0 && e->threats[e->primary_threat].distance > 20.0f)) {
        e->state = STATE_HIDING;
        e->stalemate_timer = 0;
        e->is_running = false;
        e->move_target_x = 0;
        e->move_target_y = 0;
    }
}

static void execute_healing(Entity* e, GameState* game) {
    e->is_crouching = true;
    e->vx *= 0.1f;
    e->vy *= 0.1f;

    if (e->healing_timer == 0) {
        e->healing_timer = MEDPEN_USE_TIME;
    }

    e->healing_timer--;

    if (e->healing_timer <= 0) {
        e->medpens--;
        e->health += MEDPEN_HEAL_AMOUNT;
        if (e->health > e->max_health) e->health = e->max_health;
        e->state = STATE_COMBAT;
        e->is_crouching = false;
    }

    /* Interrupt if taking damage */
    if (e->damage_react_timer > 20) {
        e->healing_timer = 0;
        e->state = STATE_COMBAT;
    }
}

static void execute_reload(Entity* e, GameState* game) {
    e->vx *= 0.3f;
    e->vy *= 0.3f;

    if (e->reload_timer <= 0) {
        start_reload(e);
    }

    e->reload_timer--;

    if (e->reload_timer <= 0) {
        complete_reload(e);
        e->state = (e->primary_threat >= 0 && e->threats[e->primary_threat].is_visible)
                   ? STATE_COMBAT : STATE_HUNTING;
    }
}

static void execute_supporting(Entity* e, GameState* game) {
    if (e->squad_id < 0) {
        e->state = STATE_PATROL;
        return;
    }

    const SquadInfo* squad = &game->squads.squads[e->squad_id];

    /* Find ally that needs help */
    int ally_to_help = -1;
    float best_score = 0;

    for (int i = 0; i < squad->member_count; i++) {
        int ally_id = squad->member_ids[i];
        if (ally_id == e->id) continue;

        const Entity* ally = &game->entities[ally_id];
        if (!ally->alive) continue;

        float health_ratio = ally->health / ally->max_health;
        if (health_ratio < 0.5f) {
            float score = 1.0f - health_ratio;
            if (score > best_score) {
                best_score = score;
                ally_to_help = ally_id;
            }
        }
    }

    if (ally_to_help >= 0) {
        const Entity* ally = &game->entities[ally_to_help];

        /* Move toward ally */
        float next_x, next_y;
        if (find_path(&game->level, e->x, e->y, ally->x, ally->y, &next_x, &next_y)) {
            e->vx = (next_x - e->x) * e->max_speed;
            e->vy = (next_y - e->y) * e->max_speed;
            e->facing_angle = atan2f(e->vy, e->vx);
        }

        /* If ally's threat is visible to us, engage */
        if (ally->primary_threat >= 0) {
            const ThreatInfo* ally_threat = &ally->threats[ally->primary_threat];
            float dist_to_threat = sse_distance(e->x, e->y,
                                                 ally_threat->last_known_pos.x,
                                                 ally_threat->last_known_pos.y);
            WeaponStats stats = weapon_get_stats(&e->weapon);
            if (dist_to_threat < stats.range) {
                e->state = STATE_COMBAT;
            }
        }
    } else {
        e->state = STATE_PATROL;
    }
}

/* ==========================================================================
 * MAIN AI UPDATE
 * ========================================================================== */

void update_enemy_ai(GameState* game, int entity_id) {
    Entity* e = &game->entities[entity_id];
    if (!e->alive || e->team == 0) return;

    /* Decrement movement commit timer */
    if (e->move_commit_timer > 0) {
        e->move_commit_timer--;
    }

    /* Update threat list */
    update_threat_list(e, game);

    /* Check for nearby cover */
    e->has_cover_nearby = find_nearby_cover(&game->level, e->x, e->y, &e->cover_x, &e->cover_y);

    /* Check visibility to player for legacy compatibility */
    Entity* player = &game->entities[game->player_id];
    int player_visibility = check_view_cone(
        &game->level,
        e->x, e->y, e->facing_angle,
        player->x, player->y,
        e->view_cone_angle, e->view_distance,
        true
    );

    bool can_see_player = player_visibility > 0 && player->alive;
    bool player_in_direct_view = player_visibility == 2;

    /* Update target tracking (legacy) */
    if (can_see_player) {
        e->frames_target_visible++;
        e->last_seen_x = player->x;
        e->last_seen_y = player->y;
        e->seen_enemy_id = game->player_id;
        e->target_id = game->player_id;
    } else {
        e->frames_target_visible = 0;
    }

    /* Handle detection transitions for idle/patrol/suspicious states */
    if (e->state == STATE_IDLE || e->state == STATE_PATROL || e->state == STATE_SUSPICIOUS) {
        if (player_visibility == 1 && player->alive) {
            e->state = STATE_SUSPICIOUS;
            e->alert_x = player->x;
            e->alert_y = player->y;
            e->suspicious_timer = 90;
        } else if (player_in_direct_view && player->alive) {
            e->state = STATE_ALERT;
            e->alert_x = player->x;
            e->alert_y = player->y;
            e->alert_timer = 120;
            e->frames_target_visible = 0;
        }
    }

    /* Utility-based decision making for active states */
    if (e->state >= STATE_ALERT && e->state != STATE_DEAD) {
        ActionScores scores;
        scores.attack = score_attack(e, game);
        scores.take_cover = score_take_cover(e, game);
        scores.flank = score_flank(e, game);
        scores.retreat = score_retreat(e, game);
        scores.heal = score_heal(e, game);
        scores.reload = score_reload(e, game);
        scores.investigate = score_investigate(e, game);
        scores.patrol = score_patrol(e, game);
        scores.support_ally = score_support_ally(e, game);

        /* Only reconsider action periodically or when forced */
        if (e->stalemate_timer % 30 == 0 || e->damage_react_timer > 0) {
            int best_action = choose_best_action(&scores);
            if (best_action != e->state) {
                e->prev_state = e->state;
                e->state = best_action;
                e->stalemate_timer = 0;
            }
        }
    }

    /* Execute current state */
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

        case STATE_SUSPICIOUS:
            e->vx *= 0.5f;
            e->vy *= 0.5f;
            {
                float target_angle = atan2f(e->alert_y - e->y, e->alert_x - e->x);
                float angle_diff = target_angle - e->facing_angle;
                while (angle_diff > PI) angle_diff -= 2*PI;
                while (angle_diff < -PI) angle_diff += 2*PI;
                e->facing_angle += angle_diff * 0.05f;
            }
            e->suspicious_timer--;
            if (e->suspicious_timer <= 0) {
                e->state = STATE_PATROL;
            }
            break;

        case STATE_ALERT:
            e->facing_angle = atan2f(e->alert_y - e->y, e->alert_x - e->x);
            e->alert_timer--;
            if (player_in_direct_view && e->frames_target_visible > 15) {
                e->state = STATE_COMBAT;
                e->target_id = game->player_id;
            } else if (e->alert_timer <= 0) {
                e->state = STATE_HUNTING;
                e->alert_timer = 300;
            }
            break;

        case STATE_COMBAT:
            execute_combat(e, game);
            break;

        case STATE_HUNTING:
            execute_hunting(e, game);
            break;

        case STATE_HIDING:
            execute_hiding(e, game);
            break;

        case STATE_FLANKING:
            execute_flanking(e, game);
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

        case STATE_SUPPORTING:
            execute_supporting(e, game);
            break;

        default:
            e->state = STATE_PATROL;
            break;
    }

    e->fire_cooldown--;
}
