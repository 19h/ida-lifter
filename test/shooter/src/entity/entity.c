/*
 * Entity System Implementation
 * Entity spawning, weapon management, archetype system, and entity utilities.
 */

#include "entity/entity.h"
#include "config.h"
#include "core/rng.h"
#include "math/sse_math.h"
#include <string.h>

/* ==========================================================================
 * GLOBAL WEAPON DEFINITIONS (Static Templates)
 * ========================================================================== */

const WeaponDef WEAPON_DEFS[WEAPON_COUNT] = {
    [WEAPON_PISTOL] = {
        .type = WEAPON_PISTOL,
        .name = "Pistol",
        .base_mag_size = 12,
        .base_reload_time = 45,
        .base_fire_rate = 12,
        .base_damage = 20.0f,
        .base_accuracy = 0.9f,
        .base_range = 30.0f,
        .sound_radius = 25.0f,
        .pellet_count = 1,
        .spread_angle = 0.0f
    },
    [WEAPON_SMG] = {
        .type = WEAPON_SMG,
        .name = "SMG",
        .base_mag_size = 30,
        .base_reload_time = 60,
        .base_fire_rate = 4,
        .base_damage = 12.0f,
        .base_accuracy = 0.7f,
        .base_range = 20.0f,
        .sound_radius = 28.0f,
        .pellet_count = 1,
        .spread_angle = 0.0f
    },
    [WEAPON_RIFLE] = {
        .type = WEAPON_RIFLE,
        .name = "Rifle",
        .base_mag_size = 20,
        .base_reload_time = 75,
        .base_fire_rate = 8,
        .base_damage = 25.0f,
        .base_accuracy = 0.85f,
        .base_range = 50.0f,
        .sound_radius = 35.0f,
        .pellet_count = 1,
        .spread_angle = 0.0f
    },
    [WEAPON_SHOTGUN] = {
        .type = WEAPON_SHOTGUN,
        .name = "Shotgun",
        .base_mag_size = 6,
        .base_reload_time = 90,
        .base_fire_rate = 30,
        .base_damage = 15.0f,    /* Per pellet */
        .base_accuracy = 0.5f,
        .base_range = 10.0f,
        .sound_radius = 40.0f,
        .pellet_count = 6,       /* 6 pellets per shot */
        .spread_angle = 0.2f     /* Spread angle in radians */
    }
};

/* ==========================================================================
 * GLOBAL ENTITY ARCHETYPE DEFINITIONS (Static Templates)
 * ========================================================================== */

const EntityArchetypeDef ENTITY_ARCHETYPES[ARCHETYPE_COUNT] = {
    [ARCHETYPE_PLAYER] = {
        .type = ARCHETYPE_PLAYER,
        .name = "Player",
        .base_health = 250.0f,
        .base_stamina = 100.0f,
        .base_speed = 1.0f,
        .view_cone_mult = 1.2f,
        .view_dist_mult = 1.2f,
        .accuracy_mult = 1.1f,
        .damage_mult = 1.4f,
        .preferred_weapon = WEAPON_RIFLE,
        .ideal_combat_range = 20.0f,
        .aggression = 0.7f,
        .courage = 0.9f,
        .medpen_chance = 100   /* Always starts with medpens */
    },
    [ARCHETYPE_GRUNT] = {
        .type = ARCHETYPE_GRUNT,
        .name = "Grunt",
        .base_health = 40.0f,
        .base_stamina = 80.0f,
        .base_speed = 0.9f,
        .view_cone_mult = 0.8f,
        .view_dist_mult = 0.7f,
        .accuracy_mult = 0.55f,
        .damage_mult = 0.4f,
        .preferred_weapon = WEAPON_PISTOL,
        .ideal_combat_range = 12.0f,
        .aggression = 0.6f,
        .courage = 0.4f,
        .medpen_chance = 30
    },
    [ARCHETYPE_SNIPER] = {
        .type = ARCHETYPE_SNIPER,
        .name = "Sniper",
        .base_health = 30.0f,
        .base_stamina = 60.0f,
        .base_speed = 0.7f,
        .view_cone_mult = 0.5f,     /* Narrow but focused */
        .view_dist_mult = 1.5f,     /* Long range */
        .accuracy_mult = 0.8f,
        .damage_mult = 0.6f,
        .preferred_weapon = WEAPON_RIFLE,
        .ideal_combat_range = 25.0f,
        .aggression = 0.3f,         /* Patient, waits for shots */
        .courage = 0.5f,
        .medpen_chance = 30
    },
    [ARCHETYPE_RUSHER] = {
        .type = ARCHETYPE_RUSHER,
        .name = "Rusher",
        .base_health = 25.0f,
        .base_stamina = 120.0f,
        .base_speed = 1.3f,         /* Fast */
        .view_cone_mult = 1.0f,
        .view_dist_mult = 0.6f,     /* Short sighted */
        .accuracy_mult = 0.45f,
        .damage_mult = 0.3f,
        .preferred_weapon = WEAPON_SMG,
        .ideal_combat_range = 5.0f,  /* Close range */
        .aggression = 0.9f,          /* Very aggressive */
        .courage = 0.7f,
        .medpen_chance = 30
    },
    [ARCHETYPE_HEAVY] = {
        .type = ARCHETYPE_HEAVY,
        .name = "Heavy",
        .base_health = 80.0f,
        .base_stamina = 50.0f,
        .base_speed = 0.6f,         /* Slow */
        .view_cone_mult = 0.9f,
        .view_dist_mult = 0.5f,
        .accuracy_mult = 0.45f,
        .damage_mult = 0.5f,
        .preferred_weapon = WEAPON_SHOTGUN,
        .ideal_combat_range = 8.0f,
        .aggression = 0.5f,
        .courage = 0.8f,            /* Brave due to armor */
        .medpen_chance = 100        /* Always has medpen */
    }
};

/* ==========================================================================
 * WEAPON SYSTEM FUNCTIONS
 * ========================================================================== */

WeaponStats weapon_get_stats(const WeaponInstance* weapon) {
    WeaponStats stats;
    const WeaponDef* def = weapon->def;

    /* Start with base stats */
    stats.mag_size = def->base_mag_size;
    stats.reload_time = def->base_reload_time;
    stats.fire_rate = def->base_fire_rate;
    stats.damage = def->base_damage;
    stats.accuracy = def->base_accuracy;
    stats.range = def->base_range;
    stats.sound_radius = def->sound_radius;

    /* Apply modifiers */
    if (weapon->mods & MOD_SUPPRESSOR) {
        stats.sound_radius *= 0.3f;   /* 70% reduction */
    }
    if (weapon->mods & MOD_EXTENDED_MAG) {
        stats.mag_size = (int)(stats.mag_size * 1.5f);
    }
    if (weapon->mods & MOD_LASER_SIGHT) {
        stats.accuracy += 0.15f;
        if (stats.accuracy > 1.0f) stats.accuracy = 1.0f;
    }
    if (weapon->mods & MOD_SCOPE) {
        stats.range *= 1.3f;
        stats.fire_rate = (int)(stats.fire_rate * 1.1f);  /* Slightly slower */
    }
    if (weapon->mods & MOD_COMPENSATOR) {
        stats.accuracy += 0.1f;
        if (stats.accuracy > 1.0f) stats.accuracy = 1.0f;
        stats.sound_radius *= 1.2f;   /* Louder */
    }
    if (weapon->mods & MOD_HEAVY_BARREL) {
        stats.damage *= 1.2f;
        stats.accuracy -= 0.05f;
        if (stats.accuracy < 0.1f) stats.accuracy = 0.1f;
    }
    if (weapon->mods & MOD_HAIR_TRIGGER) {
        stats.fire_rate = (int)(stats.fire_rate * 0.8f);  /* 20% faster */
        if (stats.fire_rate < 1) stats.fire_rate = 1;
    }
    /* AP_ROUNDS damage bonus is applied during hit calculation */

    return stats;
}

void weapon_apply_mod(WeaponInstance* weapon, WeaponMod mod) {
    weapon->mods |= (uint32_t)mod;
}

void weapon_remove_mod(WeaponInstance* weapon, WeaponMod mod) {
    weapon->mods &= ~(uint32_t)mod;
}

bool weapon_has_mod(const WeaponInstance* weapon, WeaponMod mod) {
    return (weapon->mods & (uint32_t)mod) != 0;
}

/* Create a weapon instance from a weapon type */
WeaponInstance create_weapon_instance(WeaponType type, int reserve_ammo) {
    WeaponInstance w;
    w.def = &WEAPON_DEFS[type];
    w.mag_current = w.def->base_mag_size;
    w.reserve = reserve_ammo;
    w.mods = MOD_NONE;
    w.condition = 1.0f;
    return w;
}

/* ==========================================================================
 * ENTITY MANAGEMENT
 * ========================================================================== */

int spawn_entity(GameState* game, int type, float x, float y, int team) {
    if (game->entity_count >= MAX_ENTITIES) return -1;

    Entity* e = &game->entities[game->entity_count];
    memset(e, 0, sizeof(Entity));

    e->x = x;
    e->y = y;
    e->id = game->entity_count;
    e->type = type;
    e->team = team;
    e->state = STATE_IDLE;
    e->prev_state = STATE_IDLE;
    e->alive = true;
    e->facing_angle = randf(&game->rng_state) * 2.0f * PI;
    e->target_id = -1;
    e->seen_enemy_id = -1;
    e->frames_target_visible = 0;
    e->squad_id = -1;
    e->primary_threat = -1;
    e->threat_count = 0;

    /* Select archetype based on team and type */
    EntityArchetype archetype_id;
    if (team == 0) {
        archetype_id = ARCHETYPE_PLAYER;
    } else {
        /* Map old type enum to archetype */
        switch (type) {
            case ENEMY_SNIPER: archetype_id = ARCHETYPE_SNIPER; break;
            case ENEMY_RUSHER: archetype_id = ARCHETYPE_RUSHER; break;
            case ENEMY_HEAVY:  archetype_id = ARCHETYPE_HEAVY;  break;
            default:           archetype_id = ARCHETYPE_GRUNT;  break;
        }
    }

    /* Apply archetype */
    const EntityArchetypeDef* arch = &ENTITY_ARCHETYPES[archetype_id];
    e->archetype = arch;
    e->health = arch->base_health;
    e->max_health = arch->base_health;
    e->stamina = arch->base_stamina;
    e->max_speed = PLAYER_WALK_SPEED * arch->base_speed;
    e->max_force = STEERING_MAX_FORCE;

    /* Vision based on archetype */
    e->view_cone_angle = VIEW_CONE_ANGLE * arch->view_cone_mult;
    e->view_distance = VIEW_DISTANCE * arch->view_dist_mult;

    /* Create weapon from archetype preference */
    int base_reserve = 60;  /* Default reserve */
    if (team == 0) {
        base_reserve = 200;  /* Player gets more ammo */
    }
    e->weapon = create_weapon_instance(arch->preferred_weapon, base_reserve);

    /* Initialize AI weights based on archetype */
    e->ai_weights.attack = AI_WEIGHT_ATTACK_BASE * arch->aggression;
    e->ai_weights.defend = AI_WEIGHT_DEFEND_BASE * (1.0f - arch->aggression);
    e->ai_weights.heal = AI_WEIGHT_HEAL_BASE;
    e->ai_weights.flank = AI_WEIGHT_FLANK_BASE * arch->aggression;
    e->ai_weights.retreat = AI_WEIGHT_RETREAT_BASE * (1.0f - arch->courage);
    e->ai_weights.investigate = AI_WEIGHT_INVESTIGATE;
    e->ai_weights.patrol = AI_WEIGHT_PATROL_BASE;
    e->ai_weights.support = AI_WEIGHT_SUPPORT_BASE;

    /* Medpens based on archetype chance */
    if (randf(&game->rng_state) * 100.0f < arch->medpen_chance) {
        e->medpens = (team == 0) ? 3 : 1;
    }

    e->health_at_damage_start = e->health;

    /* Non-player entities start patrolling */
    if (team != 0) {
        e->state = STATE_PATROL;
    }

    return game->entity_count++;
}

/* ==========================================================================
 * WEAPON PICKUP AND INTERACTION
 * ========================================================================== */

void try_pickup_weapon(GameState* game, Entity* player) {
    for (int i = 0; i < game->entity_count; i++) {
        Entity* e = &game->entities[i];
        if (e->team == player->team || e->alive) continue;

        float dx = e->x - player->x;
        float dy = e->y - player->y;
        float dist = sse_distance(0, 0, dx, dy);

        if (dist < 2.0f) {
            /* Get effective stats of both weapons */
            WeaponStats player_stats = weapon_get_stats(&player->weapon);
            WeaponStats corpse_stats = weapon_get_stats(&e->weapon);

            /* Check if corpse weapon is better (higher damage output) */
            float player_dps = player_stats.damage / (float)player_stats.fire_rate;
            float corpse_dps = corpse_stats.damage / (float)corpse_stats.fire_rate;

            if (corpse_dps > player_dps * 1.1f) {
                /* Swap weapons - the instance, not just stats */
                WeaponInstance old = player->weapon;
                player->weapon = e->weapon;
                player->weapon.mag_current = weapon_get_stats(&player->weapon).mag_size;  /* Full mag on pickup */
                e->weapon = old;  /* Leave old weapon on corpse */
            }
            /* Also grab ammo if same weapon type */
            else if (e->weapon.def == player->weapon.def && e->weapon.reserve > 0) {
                player->weapon.reserve += e->weapon.reserve;
                e->weapon.reserve = 0;
            }
            /* Different weapon type - grab some reserve as "scrap" */
            else if (e->weapon.reserve > 0) {
                player->weapon.reserve += e->weapon.reserve / 3;
                e->weapon.reserve = 0;
            }

            /* Inherit any mods from the corpse weapon if we took it */
            /* (Already handled by swapping the instance) */

            /* Pick up any medpens from the corpse (max 5) */
            if (e->medpens > 0 && player->medpens < MEDPEN_MAX) {
                int to_take = e->medpens;
                if (player->medpens + to_take > MEDPEN_MAX) {
                    to_take = MEDPEN_MAX - player->medpens;
                }
                player->medpens += to_take;
                e->medpens -= to_take;
            }
        }
    }
}

/* ==========================================================================
 * THREAT ASSESSMENT
 * ========================================================================== */

float calculate_threat_level(const Entity* self, const Entity* other, float distance, bool visible) {
    if (!other->alive || other->team == self->team) return 0.0f;

    float threat = 0.0f;

    /* Base threat from proximity (closer = more dangerous) */
    float proximity_threat = 1.0f - (distance / (VIEW_DISTANCE * 2.0f));
    if (proximity_threat < 0) proximity_threat = 0;
    threat += proximity_threat * 30.0f;

    /* Threat from weapon damage potential */
    WeaponStats other_stats = weapon_get_stats(&other->weapon);
    threat += other_stats.damage * 0.5f;

    /* Threat multiplier if visible */
    if (visible) {
        threat *= 1.5f;
    }

    /* Threat multiplier if they're aware of us */
    if (other->state >= STATE_ALERT) {
        threat *= 1.3f;
    }

    /* Threat multiplier if they're actively targeting us */
    if (other->target_id == self->id) {
        threat *= 2.0f;
    }

    /* Health factor - wounded enemies are less threatening */
    float health_ratio = other->health / other->max_health;
    threat *= (0.5f + health_ratio * 0.5f);

    /* Archetype-specific threat modifiers */
    if (other->archetype) {
        threat *= (0.8f + other->archetype->aggression * 0.4f);
    }

    return threat;
}

void update_threat_list(Entity* entity, const GameState* game) {
    entity->threat_count = 0;
    entity->primary_threat = -1;
    float highest_threat = 0.0f;

    for (int i = 0; i < game->entity_count && entity->threat_count < MAX_TRACKED_THREATS; i++) {
        const Entity* other = &game->entities[i];
        if (other->team == entity->team || !other->alive) continue;

        float dx = other->x - entity->x;
        float dy = other->y - entity->y;
        float dist = sse_distance(0, 0, dx, dy);

        /* Only track threats within reasonable range */
        if (dist > VIEW_DISTANCE * 2.0f) continue;

        /* Check visibility */
        int visibility = check_view_cone(
            &game->level,
            entity->x, entity->y, entity->facing_angle,
            other->x, other->y,
            entity->view_cone_angle, entity->view_distance,
            true
        );

        /* Calculate threat level */
        float threat = calculate_threat_level(entity, other, dist, visibility > 0);

        /* Add to threat list */
        ThreatInfo* ti = &entity->threats[entity->threat_count];
        ti->entity_id = i;
        ti->distance = dist;
        ti->threat_level = threat;
        ti->angle_to = atan2f(dy, dx);
        ti->is_visible = visibility > 0;
        ti->is_aware_of_us = other->state >= STATE_ALERT && other->target_id == entity->id;

        /* Track visibility frames */
        if (visibility > 0) {
            ti->frames_visible++;
        } else {
            ti->frames_visible = 0;
        }

        ti->last_known_pos.x = other->x;
        ti->last_known_pos.y = other->y;

        /* Track highest threat */
        if (threat > highest_threat) {
            highest_threat = threat;
            entity->primary_threat = entity->threat_count;
        }

        entity->threat_count++;
    }
}

/* ==========================================================================
 * STEERING BEHAVIORS
 * ========================================================================== */

Vec2 steering_seek(const Entity* entity, Vec2 target) {
    Vec2 desired;
    desired.x = target.x - entity->x;
    desired.y = target.y - entity->y;

    float len = sse_distance(0, 0, desired.x, desired.y);
    if (len > 0.01f) {
        desired.x = (desired.x / len) * entity->max_speed;
        desired.y = (desired.y / len) * entity->max_speed;
    }

    Vec2 steer;
    steer.x = desired.x - entity->vx;
    steer.y = desired.y - entity->vy;

    return steer;
}

Vec2 steering_flee(const Entity* entity, Vec2 threat) {
    Vec2 desired;
    desired.x = entity->x - threat.x;
    desired.y = entity->y - threat.y;

    float len = sse_distance(0, 0, desired.x, desired.y);
    if (len > 0.01f) {
        desired.x = (desired.x / len) * entity->max_speed;
        desired.y = (desired.y / len) * entity->max_speed;
    }

    Vec2 steer;
    steer.x = desired.x - entity->vx;
    steer.y = desired.y - entity->vy;

    return steer;
}

Vec2 steering_arrive(const Entity* entity, Vec2 target, float slow_radius) {
    Vec2 desired;
    desired.x = target.x - entity->x;
    desired.y = target.y - entity->y;

    float dist = sse_distance(0, 0, desired.x, desired.y);

    if (dist < 0.01f) {
        return (Vec2){0, 0};
    }

    /* Calculate desired speed (ramped down near target) */
    float speed = entity->max_speed;
    if (dist < slow_radius) {
        speed = entity->max_speed * (dist / slow_radius);
    }

    desired.x = (desired.x / dist) * speed;
    desired.y = (desired.y / dist) * speed;

    Vec2 steer;
    steer.x = desired.x - entity->vx;
    steer.y = desired.y - entity->vy;

    return steer;
}

Vec2 steering_wander(const Entity* entity, uint32_t* rng) {
    /* Calculate wander target on circle ahead of entity */
    float heading = atan2f(entity->vy, entity->vx);
    if (entity->vx == 0 && entity->vy == 0) {
        heading = entity->facing_angle;
    }

    /* Circle center is ahead of entity */
    float circle_x = entity->x + cosf(heading) * STEERING_WANDER_DIST;
    float circle_y = entity->y + sinf(heading) * STEERING_WANDER_DIST;

    /* Random point on circle */
    float wander_angle = randf(rng) * 2.0f * PI;
    Vec2 target;
    target.x = circle_x + cosf(wander_angle) * STEERING_WANDER_RADIUS;
    target.y = circle_y + sinf(wander_angle) * STEERING_WANDER_RADIUS;

    return steering_seek(entity, target);
}

Vec2 steering_separation(const Entity* entity, const GameState* game) {
    Vec2 steer = {0, 0};
    int count = 0;

    for (int i = 0; i < game->entity_count; i++) {
        const Entity* other = &game->entities[i];
        if (other == entity || !other->alive || other->team != entity->team) continue;

        float dx = entity->x - other->x;
        float dy = entity->y - other->y;
        float dist = sse_distance(0, 0, dx, dy);

        if (dist < STEERING_SEPARATION_DIST && dist > 0.01f) {
            /* Weight by inverse distance (closer = stronger push) */
            float weight = 1.0f / dist;
            steer.x += (dx / dist) * weight;
            steer.y += (dy / dist) * weight;
            count++;
        }
    }

    if (count > 0) {
        steer.x /= count;
        steer.y /= count;

        /* Scale to max speed */
        float len = sse_distance(0, 0, steer.x, steer.y);
        if (len > 0.01f) {
            steer.x = (steer.x / len) * entity->max_speed;
            steer.y = (steer.y / len) * entity->max_speed;
        }
    }

    return steer;
}

Vec2 steering_combine(const SteeringForces* forces, float max_force) {
    Vec2 combined = {0, 0};

    /* Priority-weighted combination */
    combined.x += forces->seek.x * 1.0f;
    combined.y += forces->seek.y * 1.0f;

    combined.x += forces->flee.x * 1.2f;   /* Flee is high priority */
    combined.y += forces->flee.y * 1.2f;

    combined.x += forces->arrive.x * 0.8f;
    combined.y += forces->arrive.y * 0.8f;

    combined.x += forces->wander.x * 0.3f;
    combined.y += forces->wander.y * 0.3f;

    combined.x += forces->separation.x * 1.0f;
    combined.y += forces->separation.y * 1.0f;

    combined.x += forces->obstacle_avoid.x * 1.5f;  /* Highest priority */
    combined.y += forces->obstacle_avoid.y * 1.5f;

    /* Clamp to max force */
    float len = sse_distance(0, 0, combined.x, combined.y);
    if (len > max_force) {
        combined.x = (combined.x / len) * max_force;
        combined.y = (combined.y / len) * max_force;
    }

    return combined;
}
