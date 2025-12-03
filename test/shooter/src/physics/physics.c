/*
 * Physics System Implementation
 * Entity movement, bullet updates, and collision detection.
 */

#include "physics/physics.h"
#include "config.h"
#include "level/level.h"
#include "combat/bullet.h"
#include "math/avx_math.h"
#include "math/sse_math.h"
#include <stdlib.h>

void update_physics(GameState* game) {
    /* Update entity positions with collision */
    for (int i = 0; i < game->entity_count; i++) {
        Entity* e = &game->entities[i];
        if (!e->alive) continue;

        /* First, check if entity is currently stuck in a wall and push them out */
        if (!is_walkable(&game->level, (int)e->x, (int)e->y)) {
            /* Find nearest walkable tile */
            for (int r = 1; r < 10; r++) {
                bool found = false;
                for (int dy = -r; dy <= r && !found; dy++) {
                    for (int dx = -r; dx <= r && !found; dx++) {
                        if (abs(dx) == r || abs(dy) == r) {
                            int nx = (int)e->x + dx;
                            int ny = (int)e->y + dy;
                            if (is_walkable(&game->level, nx, ny)) {
                                e->x = nx + 0.5f;
                                e->y = ny + 0.5f;
                                found = true;
                            }
                        }
                    }
                }
                if (found) break;
            }
        }

        float new_x = e->x + e->vx;
        float new_y = e->y + e->vy;

        /* Try full movement first */
        if (is_walkable(&game->level, (int)new_x, (int)new_y)) {
            e->x = new_x;
            e->y = new_y;
        } else {
            /* Try X only */
            if (is_walkable(&game->level, (int)new_x, (int)e->y)) {
                e->x = new_x;
            }
            /* Try Y only */
            if (is_walkable(&game->level, (int)e->x, (int)new_y)) {
                e->y = new_y;
            }
        }

        /* Stuck detection - check if entity is oscillating/not moving */
        float move_dist = sse_distance(e->x, e->y, e->prev_x, e->prev_y);
        if (move_dist < 0.05f && (e->vx != 0 || e->vy != 0)) {
            e->stuck_counter++;

            /* If stuck for too long, try to break free */
            if (e->stuck_counter > 15) {
                /* Try moving in a random valid direction */
                const int dir_x[] = {1, 1, 0, -1, -1, -1, 0, 1};
                const int dir_y[] = {0, 1, 1, 1, 0, -1, -1, -1};

                /* Start from a random direction to avoid always picking same escape */
                int start_dir = (game->frame + i) % 8;
                for (int d = 0; d < 8; d++) {
                    int dir = (start_dir + d) % 8;
                    float test_x = e->x + dir_x[dir] * 1.5f;
                    float test_y = e->y + dir_y[dir] * 1.5f;

                    if (is_walkable(&game->level, (int)test_x, (int)test_y)) {
                        /* Force movement in this direction */
                        e->move_target_x = test_x;
                        e->move_target_y = test_y;
                        e->move_commit_timer = 20;  /* Strong commit to break free */
                        e->stuck_counter = 0;
                        break;
                    }
                }
            }
        } else {
            e->stuck_counter = 0;
        }

        /* Save position for next frame's stuck check */
        e->prev_x = e->x;
        e->prev_y = e->y;

        /* Friction using SSE - less aggressive to allow smoother movement */
        {
            __m128 vel = _mm_set_ps(0, 0, e->vy, e->vx);
            __m128 friction = _mm_set1_ps(0.85f);  /* Was 0.9, now smoother */
            vel = _mm_mul_ps(vel, friction);
            ALIGN32 float vel_out[4];
            _mm_store_ps(vel_out, vel);
            e->vx = vel_out[0];
            e->vy = vel_out[1];
        }

        /* Generate footstep sounds based on movement speed */
        float move_speed = sse_distance(0, 0, e->vx, e->vy);
        if (move_speed > 0.05f) {
            float base_sound = e->is_running ? SOUND_RUN_BASE : SOUND_WALK_BASE;
            /* Crouching reduces sound */
            if (e->is_crouching) base_sound *= 0.3f;
            /* Calculate sound radius with corridor amplification */
            float sound_radius = calculate_sound_radius(&game->level, e->x, e->y, base_sound);
            /* Only propagate sound periodically (every ~10 frames based on speed) */
            e->steps_since_sound++;
            if (e->steps_since_sound > (int)(5.0f / (move_speed + 0.1f))) {
                propagate_sound(game, e->x, e->y, sound_radius);
                e->steps_since_sound = 0;
            }
        }
    }

    /* Update bullets using AVX (batches of 8) */
    ALIGN32 float bx[8], by[8], bvx[8], bvy[8];

    for (int i = 0; i < game->bullet_count; i += 8) {
        int count = (game->bullet_count - i < 8) ? game->bullet_count - i : 8;

        for (int j = 0; j < count; j++) {
            Bullet* b = &game->bullets[i + j];
            bx[j] = b->x;
            by[j] = b->y;
            bvx[j] = b->vx;
            bvy[j] = b->vy;
        }
        for (int j = count; j < 8; j++) {
            bx[j] = by[j] = bvx[j] = bvy[j] = 0;
        }

        avx_update_bullets_8(bx, by, bvx, bvy, 1.0f);

        for (int j = 0; j < count; j++) {
            Bullet* b = &game->bullets[i + j];
            b->x = bx[j];
            b->y = by[j];
        }
    }

    /* Bullet collision checks */
    for (int bi = game->bullet_count - 1; bi >= 0; bi--) {
        Bullet* b = &game->bullets[bi];
        if (!b->active) continue;

        /* Wall collision */
        int tx = (int)b->x;
        int ty = (int)b->y;
        if (tx < 0 || tx >= LEVEL_WIDTH || ty < 0 || ty >= LEVEL_HEIGHT ||
            game->level.tiles[tx + ty * LEVEL_WIDTH] == TILE_WALL) {
            b->active = false;
            continue;
        }

        /* Entity collision using SSE for distance */
        for (int ei = 0; ei < game->entity_count; ei++) {
            Entity* e = &game->entities[ei];
            if (!e->alive || e->team == b->team || ei == b->owner_id) continue;

            /* SSE distance squared calculation */
            __m128 bpos = _mm_set_ps(0, 0, b->y, b->x);
            __m128 epos = _mm_set_ps(0, 0, e->y, e->x);
            __m128 diff = _mm_sub_ps(bpos, epos);
            __m128 sq = _mm_mul_ps(diff, diff);
            __m128 dist2_vec = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, 1));
            float dist2 = _mm_cvtss_f32(dist2_vec);

            if (dist2 < 1.0f) {  /* Hit radius 1.0 */
                e->health -= b->damage;
                b->active = false;

                /* Record where the damage came from (bullet trajectory) */
                Entity* shooter = (b->owner_id >= 0 && b->owner_id < game->entity_count)
                                  ? &game->entities[b->owner_id] : NULL;
                if (shooter) {
                    e->last_damage_x = shooter->x;
                    e->last_damage_y = shooter->y;
                } else {
                    /* Estimate from bullet velocity */
                    e->last_damage_x = e->x - b->vx * 10.0f;
                    e->last_damage_y = e->y - b->vy * 10.0f;
                }
                e->damage_react_timer = 60;  /* React for 2 seconds */

                if (e->health <= 0) {
                    e->alive = false;
                    e->state = STATE_DEAD;
                }
                break;
            }
        }
    }

    /* Compact bullet array (remove inactive) */
    int write_idx = 0;
    for (int i = 0; i < game->bullet_count; i++) {
        if (game->bullets[i].active) {
            if (i != write_idx) {
                game->bullets[write_idx] = game->bullets[i];
            }
            write_idx++;
        }
    }
    game->bullet_count = write_idx;
}
