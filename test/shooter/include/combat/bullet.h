/*
 * Combat System - Bullets
 * Bullet spawning, damage application, and sound propagation.
 */

#ifndef SHOOTER_BULLET_H
#define SHOOTER_BULLET_H

#include "../types.h"

/*
 * Spawn a bullet from an entity at the given position and angle.
 * Uses the entity's weapon stats for damage, pellet count (shotgun), etc.
 */
void spawn_bullet(GameState* game, int owner_id, float x, float y, float angle, float accuracy);

/*
 * Spawn a bullet with explicit damage (for special cases).
 */
void spawn_bullet_with_damage(GameState* game, int owner_id, float x, float y,
                              float angle, float damage);

/*
 * Propagate sound to alert nearby enemies.
 * Used when shots are fired or loud actions occur.
 */
void propagate_sound(GameState* game, float x, float y, float radius);

/*
 * Apply bullet damage to target, considering weapon mods and archetype.
 * Returns the actual damage dealt.
 */
float apply_bullet_damage(const Bullet* bullet, Entity* target);

/*
 * Fire weapon from entity, handling all mechanics:
 * - Accuracy calculation with archetype modifier
 * - Bullet spawning (including shotgun pellets)
 * - Ammo consumption
 * - Sound propagation with weapon-specific radius
 * Returns true if weapon was fired.
 */
bool fire_weapon(GameState* game, int entity_id);

/*
 * Start reload for entity.
 */
void start_reload(Entity* e);

/*
 * Complete reload for entity (called when reload timer reaches 0).
 */
void complete_reload(Entity* e);

#endif /* SHOOTER_BULLET_H */
