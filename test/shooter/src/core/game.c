/*
 * Game Loop Implementation
 * Main game initialization and simulation loop.
 */

#include "game/game.h"
#include "config.h"
#include "types.h"
#include "core/rng.h"
#include "level/level.h"
#include "entity/entity.h"
#include "ai/enemy_ai.h"
#include "ai/player_ai.h"
#include "combat/bullet.h"
#include "physics/physics.h"
#include "render/render.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

/* Initialize objective based on level layout */
static void init_objective(GameState* game) {
    /* Default: Eliminate all enemies then reach exit */
    game->objective.type = OBJECTIVE_REACH_EXIT;
    game->objective.completed = false;

    /* Place exit in the room farthest from spawn */
    float max_dist = 0;
    int exit_room = 0;

    for (int r = 1; r < game->level.room_count; r++) {
        Room* room = &game->level.rooms[r];
        float cx = room->x + room->width / 2.0f;
        float cy = room->y + room->height / 2.0f;
        float dist = (cx - game->level.spawn_x) * (cx - game->level.spawn_x) +
                     (cy - game->level.spawn_y) * (cy - game->level.spawn_y);
        if (dist > max_dist) {
            max_dist = dist;
            exit_room = r;
        }
    }

    Room* room = &game->level.rooms[exit_room];
    game->objective.exit_x = room->x + room->width / 2.0f;
    game->objective.exit_y = room->y + room->height / 2.0f;

    game->game_won = false;
    game->game_over = false;
}

/* Check objective completion */
static void update_objective(GameState* game) {
    Entity* player = &game->entities[game->player_id];

    switch (game->objective.type) {
        case OBJECTIVE_ELIMINATE_ALL: {
            int enemies_alive = 0;
            for (int i = 0; i < game->entity_count; i++) {
                if (game->entities[i].team != 0 && game->entities[i].alive) {
                    enemies_alive++;
                }
            }
            if (enemies_alive == 0) {
                game->objective.completed = true;
                game->game_won = true;
            }
            break;
        }

        case OBJECTIVE_REACH_EXIT: {
            /* Must eliminate all enemies first */
            int enemies_alive = 0;
            for (int i = 0; i < game->entity_count; i++) {
                if (game->entities[i].team != 0 && game->entities[i].alive) {
                    enemies_alive++;
                }
            }

            if (enemies_alive == 0) {
                /* Check if player reached exit */
                float dx = player->x - game->objective.exit_x;
                float dy = player->y - game->objective.exit_y;
                float dist = dx * dx + dy * dy;
                if (dist < 4.0f) {  /* Within 2 tiles */
                    game->objective.completed = true;
                    game->game_won = true;
                }
            }
            break;
        }

        case OBJECTIVE_SURVIVE_TIME:
            if (game->frame >= game->objective.survive_frames) {
                game->objective.completed = true;
                game->game_won = true;
            }
            break;

        case OBJECTIVE_COLLECT_INTEL:
            if (game->objective.intel_collected >= game->objective.intel_required) {
                game->objective.completed = true;
                game->game_won = true;
            }
            break;
    }

    /* Check for game over */
    if (!player->alive) {
        game->game_over = true;
    }
}

void run_bullet_sim(int max_frames) {
    /* Switch to alternate screen buffer (like vim/htop do) */
    printf("\033[?1049h\033[H");
    fflush(stdout);

    Viewport* vp = create_viewport();

    GameState game;
    memset(&game, 0, sizeof(game));
    game.rng_state = (uint32_t)time(NULL);

    /* Generate level */
    generate_level(&game.level, &game.rng_state);

    /* Spawn player in first room */
    game.player_id = spawn_entity(&game, 0, game.level.spawn_x, game.level.spawn_y, 0);

    /* Spawn enemies in other rooms */
    for (int r = 1; r < game.level.room_count; r++) {
        Room* room = &game.level.rooms[r];

        /* Spawn 1-3 enemies per room */
        int enemy_count = randi_range(&game.rng_state, 1, 3);

        for (int e = 0; e < enemy_count; e++) {
            float ex = room->x + 1 + randf(&game.rng_state) * (room->width - 2);
            float ey = room->y + 1 + randf(&game.rng_state) * (room->height - 2);

            int enemy_type = randi_range(&game.rng_state, 0, 3);
            spawn_entity(&game, enemy_type, ex, ey, 1);
        }
    }

    /* Initialize objective */
    init_objective(&game);

    /* Main game loop */
    int prev_bullet_count = 0;

    for (game.frame = 0; max_frames < 0 || game.frame < max_frames; game.frame++) {
        /* Update AI */
        update_player_ai(&game);

        for (int i = 0; i < game.entity_count; i++) {
            if (game.entities[i].team != 0) {
                update_enemy_ai(&game, i);
            }
        }

        /* Check for new shots (sound propagation) */
        if (game.bullet_count > prev_bullet_count) {
            for (int i = prev_bullet_count; i < game.bullet_count; i++) {
                Bullet* b = &game.bullets[i];
                propagate_sound(&game, b->x, b->y, SOUND_RADIUS_SHOT);
            }
        }
        prev_bullet_count = game.bullet_count;

        /* Update physics */
        update_physics(&game);

        /* Update objective */
        update_objective(&game);

        /* Render */
        render_game(&game, vp);
        render_buffer(vp);

        /* Check end conditions */
        if (game.game_over) {
            draw_string(vp, vp->width/2 - 6, vp->height/2, "MISSION FAILED");
            render_buffer(vp);
            sleep_ms(2000);
            break;
        }

        if (game.game_won) {
            draw_string(vp, vp->width/2 - 8, vp->height/2, "MISSION COMPLETE!");
            render_buffer(vp);
            sleep_ms(2000);
            break;
        }

        sleep_ms(1000 / FPS);
    }

    free_viewport(vp);

    /* Switch back to normal screen buffer */
    printf("\033[?1049l");
    fflush(stdout);
}
