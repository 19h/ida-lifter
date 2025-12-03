/*
 * Player AI System Implementation
 * Smart AI controller for the player entity.
 */

#include "ai/player_ai.h"
#include "config.h"
#include "core/rng.h"
#include "level/level.h"
#include "entity/entity.h"
#include "combat/bullet.h"
#include "math/sse_math.h"
#include <math.h>

/* Helper: Calculate centroid of nearby threats for retreat direction */
static void calculate_threat_centroid(const GameState* game, const Entity* player,
                                       float* cx, float* cy, int* count) {
    *cx = 0; *cy = 0; *count = 0;
    for (int i = 0; i < game->entity_count; i++) {
        Entity* e = &game->entities[i];
        if (e->team == 0 || !e->alive) continue;
        if (e->state >= STATE_ALERT) {  /* Only count aware enemies */
            float dist = sse_distance(player->x, player->y, e->x, e->y);
            if (dist < 25.0f) {  /* Within threat radius */
                *cx += e->x;
                *cy += e->y;
                (*count)++;
            }
        }
    }
    if (*count > 0) {
        *cx /= *count;
        *cy /= *count;
    }
}

/* Helper: Find retreat position away from threat centroid */
static bool find_retreat_position(const GameState* game, const Entity* player,
                                   float threat_cx, float threat_cy,
                                   float* retreat_x, float* retreat_y) {
    /* Direction away from threats */
    float dx = player->x - threat_cx;
    float dy = player->y - threat_cy;
    float len = sse_distance(0, 0, dx, dy);
    if (len < 0.1f) {
        dx = 1.0f; dy = 0;
    } else {
        dx /= len;
        dy /= len;
    }

    /* Try to find a position 10-15 tiles away in retreat direction */
    for (float dist = 15.0f; dist >= 5.0f; dist -= 2.0f) {
        float test_x = player->x + dx * dist;
        float test_y = player->y + dy * dist;

        /* Clamp to level bounds */
        if (test_x < 1) test_x = 1;
        if (test_y < 1) test_y = 1;
        if (test_x >= LEVEL_WIDTH - 1) test_x = LEVEL_WIDTH - 2;
        if (test_y >= LEVEL_HEIGHT - 1) test_y = LEVEL_HEIGHT - 2;

        if (is_walkable(&game->level, (int)test_x, (int)test_y)) {
            /* Prefer positions with cover */
            float cover_x, cover_y;
            if (find_cover_from_threat(&game->level, test_x, test_y, threat_cx, threat_cy,
                                        &cover_x, &cover_y)) {
                *retreat_x = cover_x;
                *retreat_y = cover_y;
                return true;
            }
            *retreat_x = test_x;
            *retreat_y = test_y;
            return true;
        }
    }
    return false;
}

/* Helper: Check if player should retreat (outnumbered or surrounded) */
static bool should_retreat(const Entity* player, int enemies_aware, float closest_dist,
                           int enemies_flanking, bool has_cover) {
    /* Don't retreat if we have good cover - stand and fight */
    if (has_cover && player->health > 50.0f) return false;

    /* Don't retreat if only 1-2 enemies - we can handle them */
    if (enemies_aware <= 2 && player->health > 60.0f) return false;

    /* Retreat conditions (only when situation is really bad):
     * 1. Outnumbered 4+ to 1 with low health
     * 2. Very low health and any enemies
     * 3. Being flanked from 3+ directions (truly surrounded)
     */
    if (enemies_aware >= 4 && player->health < 120.0f) return true;
    if (player->health < 40.0f && enemies_aware >= 2) return true;
    if (enemies_flanking >= 3 && player->health < 100.0f) return true;

    return false;
}

/* Helper: Count enemies flanking (on different sides of player) */
static int count_flanking_enemies(const GameState* game, const Entity* player) {
    int quadrants[4] = {0, 0, 0, 0};  /* NE, NW, SW, SE */

    for (int i = 0; i < game->entity_count; i++) {
        Entity* e = &game->entities[i];
        if (e->team == 0 || !e->alive || e->state < STATE_ALERT) continue;

        float dist = sse_distance(player->x, player->y, e->x, e->y);
        if (dist > 20.0f) continue;

        float dx = e->x - player->x;
        float dy = e->y - player->y;

        int q = 0;
        if (dx >= 0 && dy >= 0) q = 0;      /* NE */
        else if (dx < 0 && dy >= 0) q = 1;  /* NW */
        else if (dx < 0 && dy < 0) q = 2;   /* SW */
        else q = 3;                          /* SE */

        quadrants[q]++;
    }

    int occupied = 0;
    for (int i = 0; i < 4; i++) {
        if (quadrants[i] > 0) occupied++;
    }
    return occupied;
}

void update_player_ai(GameState* game) {
    Entity* player = &game->entities[game->player_id];
    if (!player->alive) return;

    /* Get weapon stats for decisions */
    WeaponStats wstats = weapon_get_stats(&player->weapon);

    /* Decrement movement commit timer */
    if (player->move_commit_timer > 0) {
        player->move_commit_timer--;
    }

    /* Scan for enemies and assess threats */
    int enemies_aware = 0;
    int enemies_visible = 0;
    int enemies_unaware = 0;
    int closest_aware_id = -1;
    int closest_unaware_id = -1;
    float closest_aware_dist = 1e10f;
    float closest_unaware_dist = 1e10f;

    for (int i = 0; i < game->entity_count; i++) {
        Entity* e = &game->entities[i];
        if (e->team == 0 || !e->alive) continue;

        float dx = e->x - player->x;
        float dy = e->y - player->y;
        float dist = sse_distance(0, 0, dx, dy);

        /* Can player see this enemy? */
        int visibility = check_view_cone(
            &game->level,
            player->x, player->y, player->facing_angle,
            e->x, e->y,
            player->view_cone_angle, player->view_distance,
            true
        );

        if (visibility > 0) {
            enemies_visible++;
            player->last_seen_x = e->x;
            player->last_seen_y = e->y;
        }

        /* Is this enemy aware of player? */
        bool is_aware = (e->state >= STATE_ALERT);

        if (is_aware) {
            enemies_aware++;
            if (dist < closest_aware_dist) {
                closest_aware_dist = dist;
                closest_aware_id = i;
            }
        } else {
            enemies_unaware++;
            if (visibility > 0 && dist < closest_unaware_dist) {
                closest_unaware_dist = dist;
                closest_unaware_id = i;
            }
        }
    }

    /* Count flanking enemies */
    int enemies_flanking = count_flanking_enemies(game, player);

    /* Check for nearby cover */
    player->has_cover_nearby = find_nearby_cover(&game->level, player->x, player->y,
                                                  &player->cover_x, &player->cover_y);

    /* Track rapid damage for healing decisions */
    if (player->rapid_damage_timer <= 0) {
        player->health_at_damage_start = player->health;
        player->rapid_damage_timer = RAPID_DAMAGE_WINDOW;
    }
    player->rapid_damage_timer--;
    float damage_taken_recently = player->health_at_damage_start - player->health;

    /* Check if we need healing */
    bool need_heal = false;
    if (player->medpens > 0 && player->state != STATE_HEALING) {
        if (damage_taken_recently >= RAPID_DAMAGE_THRESHOLD && player->health < 200.0f) {
            need_heal = true;
        } else if (player->health < 100.0f && (player->has_cover_nearby || enemies_visible == 0)) {
            need_heal = true;
        } else if (player->health < 50.0f) {
            need_heal = true;
        }
    }

    if (need_heal) {
        player->state = STATE_HEALING;
        player->healing_timer = 0;
        player->stalemate_timer = 0;
    }

    /* Check if we should retreat */
    bool should_retreat_now = should_retreat(player, enemies_aware, closest_aware_dist, enemies_flanking, player->has_cover_nearby);

    /* React to taking damage */
    bool taking_fire = (player->damage_react_timer > 0);
    if (taking_fire) {
        player->damage_react_timer--;

        float threat_dx = player->last_damage_x - player->x;
        float threat_dy = player->last_damage_y - player->y;
        float threat_dist = sse_distance(0, 0, threat_dx, threat_dy);

        if (threat_dist > 0.1f) {
            float target_angle = atan2f(threat_dy, threat_dx);
            float angle_diff = target_angle - player->facing_angle;
            while (angle_diff > PI) angle_diff -= 2*PI;
            while (angle_diff < -PI) angle_diff += 2*PI;
            player->facing_angle += angle_diff * 0.3f;

            player->last_seen_x = player->last_damage_x;
            player->last_seen_y = player->last_damage_y;

            /* Find the attacker */
            for (int i = 0; i < game->entity_count; i++) {
                Entity* e = &game->entities[i];
                if (e->team == 0 || !e->alive) continue;

                float dist_sq = sse_distance_squared(e->x, e->y, player->last_damage_x, player->last_damage_y);
                if (dist_sq < 25.0f) {
                    player->target_id = i;
                    if (should_retreat_now) {
                        player->state = STATE_RETREATING;
                    } else {
                        player->state = STATE_COMBAT;
                    }
                    enemies_aware++;
                    if (threat_dist < closest_aware_dist) {
                        closest_aware_dist = threat_dist;
                        closest_aware_id = i;
                    }
                    break;
                }
            }

            if (player->state != STATE_COMBAT && player->state != STATE_RETREATING && player->has_cover_nearby) {
                player->state = STATE_HIDING;
                player->alert_timer = 90;
            }
        }
    }

    /* Movement variables */
    float move_x = 0, move_y = 0;
    bool should_shoot = false;
    int target_id = -1;
    bool commit_new_target = false;  /* Whether to commit to a new movement target */

    /* State machine */
    switch (player->state) {
        case STATE_PATROL: {
            player->is_running = (enemies_unaware == 0 && enemies_aware == 0);
            player->is_crouching = (enemies_visible > 0);

            if (taking_fire) {
                if (should_retreat_now) {
                    player->state = STATE_RETREATING;
                    player->move_commit_timer = 0;  /* Allow immediate direction change */
                } else if (player->has_cover_nearby) {
                    player->state = STATE_HIDING;
                    player->alert_timer = 60;
                } else {
                    player->state = STATE_COMBAT;
                }
                break;
            }

            if (enemies_visible > 0 && enemies_aware == 0) {
                if (player->has_cover_nearby) {
                    player->state = STATE_HIDING;
                    player->target_id = closest_unaware_id;
                    player->alert_timer = 60;
                } else {
                    player->state = STATE_COMBAT;
                    player->target_id = closest_unaware_id;
                }
            } else if (enemies_aware > 0) {
                if (should_retreat_now) {
                    player->state = STATE_RETREATING;
                    player->target_id = closest_aware_id;
                } else if (player->has_cover_nearby && closest_aware_dist > 10.0f) {
                    player->state = STATE_HIDING;
                    player->target_id = closest_aware_id;
                } else {
                    player->state = STATE_COMBAT;
                    player->target_id = closest_aware_id;
                }
            } else {
                /* Continue patrol - use committed movement */

                /* Count total enemies to check if we should go to exit */
                int total_enemies = 0;
                for (int i = 0; i < game->entity_count; i++) {
                    if (game->entities[i].team != 0 && game->entities[i].alive) {
                        total_enemies++;
                    }
                }

                /* If all enemies dead and objective is reach exit, go there */
                if (total_enemies == 0 && game->objective.type == OBJECTIVE_REACH_EXIT) {
                    player->patrol_x = game->objective.exit_x;
                    player->patrol_y = game->objective.exit_y;
                    player->move_target_x = game->objective.exit_x;
                    player->move_target_y = game->objective.exit_y;
                    player->move_commit_timer = MOVE_COMMIT_FRAMES;
                    player->is_running = true;
                } else if (player->patrol_x == 0 && player->patrol_y == 0) {
                    for (int attempts = 0; attempts < 10; attempts++) {
                        int room_idx = randi_range(&game->rng_state, 0, game->level.room_count - 1);
                        Room* r = &game->level.rooms[room_idx];
                        float px = r->x + 1 + randf(&game->rng_state) * (r->width - 2);
                        float py = r->y + 1 + randf(&game->rng_state) * (r->height - 2);
                        if (is_walkable(&game->level, (int)px, (int)py)) {
                            player->patrol_x = px;
                            player->patrol_y = py;
                            player->move_target_x = px;
                            player->move_target_y = py;
                            player->move_commit_timer = MOVE_COMMIT_FRAMES;
                            player->stalemate_timer = 0;
                            break;
                        }
                    }
                }

                float dx = player->patrol_x - player->x;
                float dy = player->patrol_y - player->y;
                float dist = sse_distance(0, 0, dx, dy);

                if (dist < 2.0f) {
                    player->patrol_x = 0;
                    player->patrol_y = 0;
                    player->stalemate_timer = 0;
                    player->facing_angle += randf_range(&game->rng_state, -0.3f, 0.3f);
                } else {
                    /* Use committed target for smooth movement */
                    move_x = player->move_target_x - player->x;
                    move_y = player->move_target_y - player->y;
                    float move_len = sse_distance(0, 0, move_x, move_y);
                    if (move_len > 0.1f) {
                        move_x /= move_len;
                        move_y /= move_len;
                        player->facing_angle = atan2f(move_y, move_x);
                    }
                }
            }

            try_pickup_weapon(game, player);
            break;
        }

        case STATE_RETREATING: {
            /* Tactical retreat - move away from threat centroid */
            player->is_running = true;
            player->is_crouching = false;
            player->stalemate_timer++;

            float threat_cx, threat_cy;
            int threat_count;
            calculate_threat_centroid(game, player, &threat_cx, &threat_cy, &threat_count);

            /* Check if we found cover - if so, stop retreating and fight */
            if (player->has_cover_nearby) {
                player->state = STATE_HIDING;
                player->alert_timer = 20;
                player->stalemate_timer = 0;
                break;
            }

            /* If we've been retreating too long, just fight */
            if (player->stalemate_timer > 60) {  /* 2 seconds */
                player->state = STATE_COMBAT;
                player->stalemate_timer = 0;
                break;
            }

            /* If situation improved, stop retreating */
            if (!should_retreat_now) {
                player->state = STATE_COMBAT;
                player->stalemate_timer = 0;
                break;
            }

            /* Commit to retreat direction if timer expired */
            if (player->move_commit_timer <= 0 || player->move_target_x == 0) {
                float retreat_x, retreat_y;
                if (find_retreat_position(game, player, threat_cx, threat_cy, &retreat_x, &retreat_y)) {
                    player->move_target_x = retreat_x;
                    player->move_target_y = retreat_y;
                    player->move_commit_timer = MOVE_COMMIT_FRAMES * 2;
                } else {
                    /* Can't find retreat position - cornered! Fight! */
                    player->state = STATE_COMBAT;
                    player->stalemate_timer = 0;
                    break;
                }
            }

            /* Move toward committed retreat position */
            if (player->move_target_x != 0 || player->move_target_y != 0) {
                float dx = player->move_target_x - player->x;
                float dy = player->move_target_y - player->y;
                float dist = sse_distance(0, 0, dx, dy);

                if (dist < MOVE_COMMIT_DIST) {
                    /* Reached retreat point - reassess */
                    player->move_commit_timer = 0;
                    if (player->has_cover_nearby) {
                        player->state = STATE_HIDING;
                        player->alert_timer = 30;
                    } else {
                        player->state = STATE_COMBAT;
                    }
                    player->stalemate_timer = 0;
                } else {
                    move_x = dx / dist;
                    move_y = dy / dist;
                    /* Face toward enemies while retreating (to shoot back) */
                    if (closest_aware_id >= 0) {
                        Entity* target = &game->entities[closest_aware_id];
                        float tdx = target->x - player->x;
                        float tdy = target->y - player->y;
                        player->facing_angle = atan2f(tdy, tdx);
                    }
                }
            }

            /* ALWAYS try to shoot at enemies while retreating */
            if (closest_aware_id >= 0 && closest_aware_dist < wstats.range) {
                Entity* target = &game->entities[closest_aware_id];
                int vis = check_view_cone(&game->level, player->x, player->y, player->facing_angle,
                                          target->x, target->y, PI,  /* Wide angle - 180 degrees */
                                          player->view_distance, true);
                if (vis > 0) {
                    should_shoot = true;
                }
            }

            /* Heal if we got to safety */
            if (player->health < 100.0f && player->medpens > 0 && enemies_visible == 0) {
                player->state = STATE_HEALING;
            }
            break;
        }

        case STATE_HIDING: {
            player->alert_timer--;
            player->stalemate_timer++;
            player->is_crouching = true;

            /* Move toward cover */
            if (player->has_cover_nearby) {
                float cdx = player->cover_x - player->x;
                float cdy = player->cover_y - player->y;
                float cdist = sse_distance(0, 0, cdx, cdy);

                if (cdist > 1.0f) {
                    sse_normalize(&cdx, &cdy);
                    move_x = cdx;
                    move_y = cdy;
                }
            }

            /* Track enemy */
            if (player->target_id >= 0 && player->target_id < game->entity_count) {
                Entity* target = &game->entities[player->target_id];
                if (target->alive) {
                    float tdx = target->x - player->x;
                    float tdy = target->y - player->y;

                    float norm_tdx = tdx, norm_tdy = tdy;
                    sse_normalize(&norm_tdx, &norm_tdy);

                    float target_angle = atan2f(tdy, tdx);
                    float angle_diff = target_angle - player->facing_angle;
                    while (angle_diff > PI) angle_diff -= 2*PI;
                    while (angle_diff < -PI) angle_diff += 2*PI;
                    player->facing_angle += angle_diff * 0.1f;

                    int vis = check_view_cone(&game->level, player->x, player->y, player->facing_angle,
                                              target->x, target->y, player->view_cone_angle, player->view_distance, true);

                    bool target_facing_away = false;
                    {
                        float to_player_x = player->x - target->x;
                        float to_player_y = player->y - target->y;
                        sse_normalize(&to_player_x, &to_player_y);
                        if (to_player_x != 0 || to_player_y != 0) {
                            float facing_x = cosf(target->facing_angle);
                            float facing_y = sinf(target->facing_angle);
                            float dot = sse_dot2(facing_x, facing_y, to_player_x, to_player_y);
                            target_facing_away = (dot < 0.3f);
                        }
                    }

                    if (player->alert_timer <= 0 && vis > 0 && (target_facing_away || target->state < STATE_ALERT)) {
                        player->state = STATE_COMBAT;
                        player->stalemate_timer = 0;
                        player->is_crouching = false;
                    }

                    if (player->stalemate_timer > 90) {
                        float perp_x = -norm_tdy;
                        float perp_y = norm_tdx;
                        if (randf(&game->rng_state) > 0.5f) {
                            perp_x = -perp_x;
                            perp_y = -perp_y;
                        }
                        player->move_target_x = player->x + perp_x * 8.0f + norm_tdx * 4.0f;
                        player->move_target_y = player->y + perp_y * 8.0f + norm_tdy * 4.0f;
                        player->move_commit_timer = MOVE_COMMIT_FRAMES;
                        player->state = STATE_COMBAT;
                        player->stalemate_timer = 0;
                        player->is_crouching = false;
                        player->is_running = true;
                    }
                } else {
                    player->state = STATE_PATROL;
                    player->stalemate_timer = 0;
                    player->is_crouching = false;
                }
            } else {
                player->state = STATE_PATROL;
                player->stalemate_timer = 0;
                player->is_crouching = false;
            }

            if (taking_fire) {
                if (should_retreat_now) {
                    player->state = STATE_RETREATING;
                    player->move_commit_timer = 0;
                } else if (player->target_id >= 0) {
                    player->state = STATE_COMBAT;
                    player->stalemate_timer = 0;
                    player->is_crouching = false;
                }
            } else if (should_retreat_now) {
                player->state = STATE_RETREATING;
                player->move_commit_timer = 0;
            } else if (enemies_aware > 2 || (enemies_aware > 0 && closest_aware_dist < 8.0f)) {
                player->state = STATE_COMBAT;
                player->stalemate_timer = 0;
                player->is_crouching = false;
            }
            break;
        }

        case STATE_COMBAT: {
            player->is_crouching = false;

            /* Check if we should retreat instead */
            if (should_retreat_now && player->stalemate_timer < 30) {
                player->state = STATE_RETREATING;
                player->move_commit_timer = 0;
                break;
            }

            if (player->target_id >= 0 && player->target_id < game->entity_count) {
                Entity* target = &game->entities[player->target_id];
                if (!target->alive) {
                    player->target_id = -1;
                }
            }

            if (player->target_id < 0) {
                if (closest_aware_id >= 0) {
                    player->target_id = closest_aware_id;
                } else if (closest_unaware_id >= 0) {
                    player->target_id = closest_unaware_id;
                } else {
                    player->state = STATE_PATROL;
                    break;
                }
            }

            target_id = player->target_id;
            if (target_id >= 0) {
                Entity* target = &game->entities[target_id];
                float tdx = target->x - player->x;
                float tdy = target->y - player->y;
                float tdist = sse_distance(0, 0, tdx, tdy);

                float norm_tdx = tdx, norm_tdy = tdy;
                sse_normalize(&norm_tdx, &norm_tdy);

                player->facing_angle = atan2f(tdy, tdx);

                int vis = check_view_cone(&game->level, player->x, player->y, player->facing_angle,
                                          target->x, target->y, player->view_cone_angle, player->view_distance, true);

                float ideal_range = wstats.range * 0.5f;

                if (vis > 0) {
                    player->frames_target_visible++;
                    player->stalemate_timer = 0;

                    bool in_cover = has_cover_from_direction(&game->level, player->x, player->y, target->x, target->y);

                    /* Commit to movement direction for smooth motion */
                    if (player->move_commit_timer <= 0) {
                        if (tdist > ideal_range + 5.0f) {
                            player->move_target_x = player->x + norm_tdx * 5.0f;
                            player->move_target_y = player->y + norm_tdy * 5.0f;
                            commit_new_target = true;
                        } else if (tdist < ideal_range - 3.0f && !in_cover) {
                            player->move_target_x = player->x - norm_tdx * 5.0f;
                            player->move_target_y = player->y - norm_tdy * 5.0f;
                            commit_new_target = true;
                        } else if (!in_cover) {
                            /* Strafe - commit to one direction */
                            float strafe_dir = (game->frame % 120 < 60) ? 1.0f : -1.0f;
                            player->move_target_x = player->x - norm_tdy * strafe_dir * 4.0f;
                            player->move_target_y = player->y + norm_tdx * strafe_dir * 4.0f;
                            commit_new_target = true;
                        } else {
                            /* In cover - stay put */
                            player->move_target_x = player->x;
                            player->move_target_y = player->y;
                            player->is_crouching = true;
                        }

                        if (commit_new_target) {
                            player->move_commit_timer = MOVE_COMMIT_FRAMES;
                        }
                    }

                    /* Move toward committed target */
                    float mx = player->move_target_x - player->x;
                    float my = player->move_target_y - player->y;
                    float mlen = sse_distance(0, 0, mx, my);
                    if (mlen > 0.5f) {
                        move_x = mx / mlen;
                        move_y = my / mlen;
                    } else if (mlen < MOVE_COMMIT_DIST) {
                        player->move_commit_timer = 0;  /* Reached target, can change */
                    }

                    if (tdist < wstats.range && player->frames_target_visible > 5) {
                        should_shoot = true;
                    }
                } else {
                    player->frames_target_visible = 0;
                    player->stalemate_timer++;

                    /* Move toward last known position with commitment */
                    if (player->move_commit_timer <= 0) {
                        player->move_target_x = player->last_seen_x;
                        player->move_target_y = player->last_seen_y;
                        player->move_commit_timer = MOVE_COMMIT_FRAMES;
                        player->is_running = true;
                    }

                    float ldx = player->move_target_x - player->x;
                    float ldy = player->move_target_y - player->y;
                    float ldist = sse_distance(0, 0, ldx, ldy);

                    if (ldist > MOVE_COMMIT_DIST) {
                        move_x = ldx / ldist;
                        move_y = ldy / ldist;
                    } else {
                        player->move_commit_timer = 0;

                        if (player->stalemate_timer > 45) {
                            float perp_x = -norm_tdy;
                            float perp_y = norm_tdx;
                            if (game->frame % 60 < 30) {
                                perp_x = -perp_x;
                                perp_y = -perp_y;
                            }
                            player->move_target_x = player->x + perp_x * 6.0f + norm_tdx * 3.0f;
                            player->move_target_y = player->y + perp_y * 6.0f + norm_tdy * 3.0f;
                            player->move_commit_timer = MOVE_COMMIT_FRAMES;
                            player->is_running = true;
                            player->stalemate_timer = 0;
                        }
                    }
                }
            }

            if (enemies_visible > 0) {
                player->stalemate_timer = 0;
            }

            if (player->health < 80.0f && player->has_cover_nearby && enemies_aware > 0) {
                player->state = STATE_HIDING;
                player->alert_timer = 30;
            }

            if (player->weapon.mag_current <= 2 && player->weapon.reserve > 0) {
                if (player->has_cover_nearby) {
                    player->state = STATE_HIDING;
                    player->alert_timer = wstats.reload_time + 20;
                }
                player->reload_timer = wstats.reload_time;
            }
            break;
        }

        case STATE_RELOAD:
            player->reload_timer--;
            move_x = 0;
            move_y = 0;

            if (player->reload_timer <= 0) {
                int to_load = wstats.mag_size;
                if (player->weapon.reserve < to_load) to_load = player->weapon.reserve;
                player->weapon.mag_current = to_load;
                player->weapon.reserve -= to_load;
                player->state = (enemies_aware > 0) ? STATE_COMBAT : STATE_PATROL;
            }
            break;

        case STATE_HEALING: {
            player->is_crouching = true;
            player->is_running = false;

            if (player->has_cover_nearby) {
                float cdx = player->cover_x - player->x;
                float cdy = player->cover_y - player->y;
                float cdist = sse_distance(0, 0, cdx, cdy);

                if (cdist > 1.5f) {
                    sse_normalize(&cdx, &cdy);
                    move_x = cdx;
                    move_y = cdy;
                    player->is_running = true;
                    player->is_crouching = false;
                } else {
                    move_x = 0;
                    move_y = 0;

                    if (player->healing_timer == 0) {
                        player->healing_timer = MEDPEN_USE_TIME;
                    }

                    player->healing_timer--;

                    if (player->healing_timer <= 0) {
                        player->medpens--;
                        player->health += MEDPEN_HEAL_AMOUNT;
                        if (player->health > 250.0f) player->health = 250.0f;
                        player->health_at_damage_start = player->health;
                        player->rapid_damage_timer = RAPID_DAMAGE_WINDOW;
                        player->state = (enemies_aware > 0) ? STATE_COMBAT : STATE_PATROL;
                        player->stalemate_timer = 0;
                    }
                }
            } else {
                if (player->healing_timer == 0) {
                    player->healing_timer = MEDPEN_USE_TIME;
                }
                player->healing_timer--;
                move_x = 0;
                move_y = 0;

                if (player->healing_timer <= 0) {
                    player->medpens--;
                    player->health += MEDPEN_HEAL_AMOUNT;
                    if (player->health > 250.0f) player->health = 250.0f;
                    player->health_at_damage_start = player->health;
                    player->state = (enemies_aware > 0) ? STATE_COMBAT : STATE_PATROL;
                }
            }

            if (taking_fire && player->healing_timer > 0) {
                player->healing_timer = 0;
                if (should_retreat_now) {
                    player->state = STATE_RETREATING;
                } else {
                    player->state = STATE_COMBAT;
                }
            }
            break;
        }

        default:
            player->state = STATE_PATROL;
            break;
    }

    /* Execute shooting */
    if (should_shoot && player->fire_cooldown <= 0 && player->reload_timer <= 0 && player->weapon.mag_current > 0) {
        spawn_bullet(game, game->player_id, player->x, player->y, player->facing_angle, wstats.accuracy);
        player->weapon.mag_current--;
        player->fire_cooldown = wstats.fire_rate;
    }

    /* Apply movement with pathfinding */
    float speed = player->is_running ? PLAYER_RUN_SPEED : (player->is_crouching ? PLAYER_WALK_SPEED * 0.5f : PLAYER_WALK_SPEED);

    if (player->is_running) {
        player->stamina -= STAMINA_DRAIN_RATE;
        if (player->stamina <= 0) {
            player->stamina = 0;
            player->is_running = false;
        }
    } else {
        player->stamina += STAMINA_REGEN_RATE;
        if (player->stamina > PLAYER_MAX_STAMINA) player->stamina = PLAYER_MAX_STAMINA;
    }

    if (move_x != 0 || move_y != 0) {
        float next_x, next_y;
        float target_x = player->x + move_x * 10.0f;
        float target_y = player->y + move_y * 10.0f;

        if (find_path(&game->level, player->x, player->y, target_x, target_y, &next_x, &next_y)) {
            player->vx = (next_x - player->x) * speed;
            player->vy = (next_y - player->y) * speed;
        }
    } else {
        player->vx *= 0.5f;
        player->vy *= 0.5f;
    }

    /* Handle reload completion */
    if (player->reload_timer > 0 && player->state != STATE_RELOAD) {
        player->reload_timer--;
        if (player->reload_timer <= 0) {
            int to_load = wstats.mag_size;
            if (player->weapon.reserve < to_load) to_load = player->weapon.reserve;
            player->weapon.mag_current = to_load;
            player->weapon.reserve -= to_load;
        }
    }

    player->fire_cooldown--;
}
