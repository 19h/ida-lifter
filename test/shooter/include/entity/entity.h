/*
 * Entity System
 * Entity spawning, weapon management, archetypes, and entity utilities.
 */

#ifndef SHOOTER_ENTITY_H
#define SHOOTER_ENTITY_H

#include "../types.h"
#include "../level/level.h"

/* ==========================================================================
 * WEAPON SYSTEM
 * ========================================================================== */

/*
 * Create a weapon instance of the specified type with given reserve ammo.
 * The instance references the global WEAPON_DEFS array.
 */
WeaponInstance create_weapon_instance(WeaponType type, int reserve_ammo);

/* ==========================================================================
 * ENTITY MANAGEMENT
 * ========================================================================== */

/*
 * Spawn a new entity in the game using the archetype system.
 * For enemies, 'type' maps to archetype (ENEMY_GRUNT -> ARCHETYPE_GRUNT, etc.)
 * For player (team=0), archetype is always ARCHETYPE_PLAYER.
 * Returns the entity ID, or -1 if the entity limit is reached.
 */
int spawn_entity(GameState* game, int type, float x, float y, int team);

/*
 * Try to pick up weapon and medpens from nearby dead enemy.
 * Now uses modular weapon system - compares DPS and handles mods.
 */
void try_pickup_weapon(GameState* game, Entity* player);

/* ==========================================================================
 * THREAT ASSESSMENT
 * ========================================================================== */

/*
 * Calculate the threat level of 'other' entity as perceived by 'self'.
 * Takes into account distance, visibility, weapon stats, awareness, etc.
 */
float calculate_threat_level(const Entity* self, const Entity* other, float distance, bool visible);

/*
 * Update the entity's threat list with all nearby enemies.
 * Populates entity->threats array and sets entity->primary_threat.
 */
void update_threat_list(Entity* entity, const GameState* game);

/* ==========================================================================
 * STEERING BEHAVIORS
 * ========================================================================== */

/*
 * Steering: seek toward a target position at max speed.
 */
Vec2 steering_seek(const Entity* entity, Vec2 target);

/*
 * Steering: flee away from a threat position.
 */
Vec2 steering_flee(const Entity* entity, Vec2 threat);

/*
 * Steering: arrive at a target position, slowing down when close.
 */
Vec2 steering_arrive(const Entity* entity, Vec2 target, float slow_radius);

/*
 * Steering: wander randomly (Reynolds wander behavior).
 */
Vec2 steering_wander(const Entity* entity, uint32_t* rng);

/*
 * Steering: separate from nearby allies to avoid crowding.
 */
Vec2 steering_separation(const Entity* entity, const GameState* game);

/*
 * Combine multiple steering forces with priority weighting.
 */
Vec2 steering_combine(const SteeringForces* forces, float max_force);

#endif /* SHOOTER_ENTITY_H */
