/*
 * Combat System - Bullets Implementation
 * Bullet spawning, shotgun pellets, and sound propagation.
 */

#include "combat/bullet.h"
#include "config.h"
#include "core/rng.h"
#include "entity/entity.h"
#include "math/avx_math.h"
#include <math.h>

void spawn_bullet(GameState* game, int owner_id, float x, float y, float angle, float accuracy) {
    if (game->bullet_count >= MAX_BULLETS) return;

    Entity* owner = &game->entities[owner_id];
    const WeaponDef* def = owner->weapon.def;
    WeaponStats stats = weapon_get_stats(&owner->weapon);

    /* Handle shotgun pellets */
    int pellets = def->pellet_count;
    if (pellets < 1) pellets = 1;

    for (int p = 0; p < pellets && game->bullet_count < MAX_BULLETS; p++) {
        /* Apply accuracy spread */
        float spread = (1.0f - accuracy) * 0.3f;

        /* Additional spread for shotgun pellets */
        if (pellets > 1) {
            spread += def->spread_angle;
        }

        float actual_angle = angle + randf_range(&game->rng_state, -spread, spread);

        Bullet* b = &game->bullets[game->bullet_count++];
        b->x = x;
        b->y = y;
        b->vx = cosf(actual_angle) * BULLET_SPEED;
        b->vy = sinf(actual_angle) * BULLET_SPEED;

        /* Calculate damage from weapon stats, apply archetype multiplier */
        float damage = stats.damage;
        if (owner->archetype) {
            damage *= owner->archetype->damage_mult;
        }
        b->damage = damage;

        b->owner_id = owner_id;
        b->team = owner->team;
        b->active = true;
        b->weapon_def = def;  /* Reference to weapon for AP rounds check */
    }
}

/* Spawn bullet with explicit damage (for special cases) */
void spawn_bullet_with_damage(GameState* game, int owner_id, float x, float y,
                              float angle, float damage) {
    if (game->bullet_count >= MAX_BULLETS) return;

    Entity* owner = &game->entities[owner_id];

    Bullet* b = &game->bullets[game->bullet_count++];
    b->x = x;
    b->y = y;
    b->vx = cosf(angle) * BULLET_SPEED;
    b->vy = sinf(angle) * BULLET_SPEED;
    b->damage = damage;
    b->owner_id = owner_id;
    b->team = owner->team;
    b->active = true;
    b->weapon_def = owner->weapon.def;
}

void propagate_sound(GameState* game, float x, float y, float radius) {
    /* Process in batches of 8 */
    for (int i = 0; i < game->entity_count; i += 8) {
        ALIGN32 float ex[8], ey[8];

        for (int j = 0; j < 8; j++) {
            int idx = i + j;
            if (idx < game->entity_count) {
                ex[j] = game->entities[idx].x;
                ey[j] = game->entities[idx].y;
            } else {
                ex[j] = -1000.0f;
                ey[j] = -1000.0f;
            }
        }

        __m256 vex = _mm256_load_ps(ex);
        __m256 vey = _mm256_load_ps(ey);

        __m256 heard = avx_sound_detection_8(vex, vey, x, y, radius);

        ALIGN32 float heard_arr[8];
        _mm256_store_ps(heard_arr, heard);

        for (int j = 0; j < 8; j++) {
            int idx = i + j;
            if (idx < game->entity_count && heard_arr[j] != 0.0f) {
                Entity* e = &game->entities[idx];
                if (e->team != 0 && e->alive && e->state < STATE_COMBAT) {
                    e->state = STATE_ALERT;
                    e->alert_x = x;
                    e->alert_y = y;
                    e->alert_timer = 180;
                }
            }
        }
    }
}

/* Apply bullet damage to target, considering weapon mods and archetype */
float apply_bullet_damage(const Bullet* bullet, Entity* target) {
    float damage = bullet->damage;

    /* Check for AP rounds modifier on the weapon that fired this bullet */
    Entity* owner = NULL;
    /* Note: In a real implementation, we'd look up the owner.
       For now, AP rounds bonus is applied based on target archetype */
    if (target->archetype && target->archetype->type == ARCHETYPE_HEAVY) {
        /* AP rounds would deal 25% more damage to heavy enemies */
        /* This would require tracking mods on the bullet, simplified here */
    }

    /* Apply damage */
    target->health -= damage;

    /* Track damage for AI reaction */
    target->last_damage_x = bullet->x;
    target->last_damage_y = bullet->y;
    target->damage_react_timer = 30;
    target->prev_health = target->health;

    /* Update threat info - the attacker just shot us */
    /* This would be done in the AI update, but we mark it here */

    return damage;
}

/* Fire weapon from entity, handling all weapon mechanics */
bool fire_weapon(GameState* game, int entity_id) {
    Entity* e = &game->entities[entity_id];

    /* Check if can fire */
    if (e->fire_cooldown > 0) return false;
    if (e->reload_timer > 0) return false;
    if (e->weapon.mag_current <= 0) return false;

    WeaponStats stats = weapon_get_stats(&e->weapon);

    /* Calculate accuracy with archetype modifier */
    float accuracy = stats.accuracy;
    if (e->archetype) {
        accuracy *= e->archetype->accuracy_mult;
    }
    if (accuracy > 1.0f) accuracy = 1.0f;

    /* Spawn bullet(s) */
    spawn_bullet(game, entity_id, e->x, e->y, e->facing_angle, accuracy);

    /* Consume ammo */
    e->weapon.mag_current--;

    /* Set cooldown */
    e->fire_cooldown = stats.fire_rate;

    /* Propagate sound with weapon-specific radius */
    float sound_radius = stats.sound_radius;
    /* Suppressor already handled in weapon_get_stats */
    propagate_sound(game, e->x, e->y, sound_radius);

    return true;
}

/* Start reload for entity */
void start_reload(Entity* e) {
    if (e->weapon.reserve <= 0) return;
    if (e->reload_timer > 0) return;

    WeaponStats stats = weapon_get_stats(&e->weapon);
    e->reload_timer = stats.reload_time;
    e->prev_state = e->state;
}

/* Complete reload for entity */
void complete_reload(Entity* e) {
    if (e->reload_timer > 0) return;

    WeaponStats stats = weapon_get_stats(&e->weapon);
    int to_load = stats.mag_size - e->weapon.mag_current;
    if (to_load > e->weapon.reserve) {
        to_load = e->weapon.reserve;
    }

    e->weapon.mag_current += to_load;
    e->weapon.reserve -= to_load;
}
