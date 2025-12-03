/*
 * Shooter Game Type Definitions
 * All game data structures are defined here.
 */

#ifndef SHOOTER_TYPES_H
#define SHOOTER_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

/* ==========================================================================
 * VECTOR TYPES
 * ========================================================================== */

/* 8-wide packed 2D vector for AVX operations */
typedef struct ALIGN32 {
    float x[8];
    float y[8];
} Vec2x8;

/* Simple 2D vector */
typedef struct {
    float x, y;
} Vec2;

/* ==========================================================================
 * WEAPON SYSTEM - MODULAR DESIGN
 * ========================================================================== */

/* Weapon type enumeration */
typedef enum {
    WEAPON_PISTOL = 0,
    WEAPON_SMG,
    WEAPON_RIFLE,
    WEAPON_SHOTGUN,
    WEAPON_COUNT
} WeaponType;

/* Weapon modifier flags (can be combined) */
typedef enum {
    MOD_NONE           = 0,
    MOD_SUPPRESSOR     = 1 << 0,   /* Reduces sound radius by 70% */
    MOD_EXTENDED_MAG   = 1 << 1,   /* +50% magazine size */
    MOD_LASER_SIGHT    = 1 << 2,   /* +15% accuracy */
    MOD_SCOPE          = 1 << 3,   /* +30% range, -10% fire rate */
    MOD_COMPENSATOR    = 1 << 4,   /* +10% accuracy, louder */
    MOD_HEAVY_BARREL   = 1 << 5,   /* +20% damage, -5% accuracy */
    MOD_HAIR_TRIGGER   = 1 << 6,   /* +20% fire rate */
    MOD_AP_ROUNDS      = 1 << 7    /* +25% damage vs heavy enemies */
} WeaponMod;

/* Weapon definition (static template - shared) */
typedef struct {
    WeaponType type;
    const char* name;
    int base_mag_size;      /* Base bullets per magazine */
    int base_reload_time;   /* Base frames to reload */
    int base_fire_rate;     /* Base frames between shots */
    float base_damage;
    float base_accuracy;    /* 0-1, affects spread */
    float base_range;
    float sound_radius;     /* Base sound emission */
    int pellet_count;       /* For shotguns */
    float spread_angle;     /* For shotguns */
} WeaponDef;

/* Weapon instance (per-entity, references a WeaponDef) */
typedef struct {
    const WeaponDef* def;   /* Pointer to shared definition */
    int mag_current;        /* Current bullets in mag */
    int reserve;            /* Reserve ammo */
    uint32_t mods;          /* Modifier flags */
    float condition;        /* 0-1, affects reliability (future use) */
} WeaponInstance;

/* Global weapon definitions (initialized in entity.c) */
extern const WeaponDef WEAPON_DEFS[WEAPON_COUNT];

/* Computed weapon stats (after applying modifiers) */
typedef struct {
    int mag_size;
    int reload_time;
    int fire_rate;
    float damage;
    float accuracy;
    float range;
    float sound_radius;
} WeaponStats;

/* ==========================================================================
 * ENTITY ARCHETYPE SYSTEM
 * ========================================================================== */

/* Entity archetype type enumeration */
typedef enum {
    ARCHETYPE_PLAYER = 0,
    ARCHETYPE_GRUNT,
    ARCHETYPE_SNIPER,
    ARCHETYPE_RUSHER,
    ARCHETYPE_HEAVY,
    ARCHETYPE_COUNT
} EntityArchetype;

/* Entity archetype definition (static template) */
typedef struct {
    EntityArchetype type;
    const char* name;
    float base_health;
    float base_stamina;
    float base_speed;           /* Movement speed multiplier */
    float view_cone_mult;       /* View cone angle multiplier */
    float view_dist_mult;       /* View distance multiplier */
    float accuracy_mult;        /* Accuracy multiplier */
    float damage_mult;          /* Damage multiplier */
    WeaponType preferred_weapon;
    float ideal_combat_range;   /* Preferred engagement distance */
    float aggression;           /* 0-1, affects tactical decisions */
    float courage;              /* 0-1, affects retreat threshold */
    int medpen_chance;          /* % chance to spawn with medpen */
} EntityArchetypeDef;

/* Global archetype definitions */
extern const EntityArchetypeDef ENTITY_ARCHETYPES[ARCHETYPE_COUNT];

/* ==========================================================================
 * AI BEHAVIOR SYSTEM
 * ========================================================================== */

/* AI behavior weights for utility-based decision making */
typedef struct {
    float attack;           /* Weight for attacking */
    float defend;           /* Weight for taking cover */
    float heal;             /* Weight for healing */
    float flank;            /* Weight for flanking maneuvers */
    float retreat;          /* Weight for retreating */
    float investigate;      /* Weight for investigating sounds/sightings */
    float patrol;           /* Weight for continuing patrol */
    float support;          /* Weight for helping allies (squad) */
} AIWeights;

/* Threat assessment for a single threat */
typedef struct {
    int entity_id;
    float distance;
    float threat_level;     /* Computed threat score */
    float angle_to;         /* Angle from us to threat */
    bool is_visible;
    bool is_aware_of_us;
    int frames_visible;
    Vec2 last_known_pos;
} ThreatInfo;

/* Squad information for coordinated behavior */
typedef struct {
    int member_ids[MAX_SQUAD_SIZE];
    int member_count;
    int leader_id;          /* Squad leader for coordination */
    Vec2 rally_point;       /* Fallback position */
    int active_target;      /* Shared target for focus fire */
    bool suppressing;       /* Squad is suppressing a position */
} SquadInfo;

/* ==========================================================================
 * STEERING BEHAVIORS
 * ========================================================================== */

typedef struct {
    Vec2 seek;              /* Seek target position */
    Vec2 flee;              /* Flee from threat */
    Vec2 arrive;            /* Arrive at position (with deceleration) */
    Vec2 wander;            /* Random wandering */
    Vec2 separation;        /* Avoid crowding allies */
    Vec2 cohesion;          /* Stay with group */
    Vec2 alignment;         /* Match group heading */
    Vec2 obstacle_avoid;    /* Avoid walls */
} SteeringForces;

/* ==========================================================================
 * ENTITY SYSTEM
 * ========================================================================== */

typedef struct {
    /* Position and movement */
    float x, y;
    float vx, vy;

    /* Steering (new) */
    SteeringForces steering;
    float max_force;            /* Maximum steering force */
    float max_speed;            /* Maximum movement speed */

    /* Stats */
    float health;
    float max_health;           /* For healing cap */
    float stamina;

    /* Archetype reference (new) */
    const EntityArchetypeDef* archetype;

    /* State machine */
    int state;
    int prev_state;             /* For returning after reload/hiding */

    /* Identity */
    int type;                   /* Entity type (player=0, enemy types 1-4) - kept for compatibility */
    int team;                   /* 0=player, 1=enemy */
    int id;                     /* Self-reference for squad lookups */

    /* Combat (modular weapon) */
    WeaponInstance weapon;
    int fire_cooldown;
    int reload_timer;

    /* Threat tracking (new) */
    ThreatInfo threats[MAX_TRACKED_THREATS];
    int threat_count;
    int primary_threat;         /* Index into threats array */

    /* AI decision weights (new) */
    AIWeights ai_weights;

    /* Squad membership (new) */
    int squad_id;               /* -1 if no squad */

    /* Awareness */
    float alert_x, alert_y;     /* Last known threat position */
    int alert_timer;
    int suspicious_timer;       /* Time spent investigating */

    /* Movement */
    float patrol_x, patrol_y;   /* Patrol target */
    bool is_running;
    bool is_crouching;          /* Behind cover */
    bool alive;

    /* Vision */
    float facing_angle;
    float view_cone_angle;      /* How wide the view cone is */
    float view_distance;        /* How far they can see */

    /* Target tracking (legacy - kept for compatibility) */
    int target_id;              /* Who this entity is targeting */
    int seen_enemy_id;          /* Last enemy spotted */
    int frames_target_visible;  /* How long current target has been in view */
    float last_seen_x, last_seen_y;  /* Where target was last seen */

    /* Cover system */
    float cover_x, cover_y;     /* Nearby cover position */
    bool has_cover_nearby;

    /* Sound */
    int steps_since_sound;      /* Counter for footstep sound generation */

    /* Damage reaction */
    float last_damage_x, last_damage_y;  /* Direction damage came from */
    int damage_react_timer;     /* Frames since last hit */
    float prev_health;          /* To detect damage taken */

    /* Stalemate detection */
    int stalemate_timer;        /* How long stuck without progress */
    float last_combat_x, last_combat_y;  /* Position when entering combat/hiding */

    /* Committed movement (prevents jittering) */
    float move_target_x, move_target_y;  /* Current committed movement target */
    int move_commit_timer;      /* Frames until we can change direction */

    /* Stuck detection */
    float prev_x, prev_y;       /* Position last frame for stuck detection */
    int stuck_counter;          /* Frames spent not moving significantly */

    /* Healing */
    int medpens;                /* Number of medpens available */
    int healing_timer;          /* Frames remaining in healing animation */
    float health_at_damage_start;  /* Health when damage started (for rapid damage detection) */
    int rapid_damage_timer;     /* Timer to track rapid health loss */
} Entity;

/* ==========================================================================
 * PROJECTILE SYSTEM
 * ========================================================================== */

typedef struct {
    float x, y;
    float vx, vy;
    float damage;
    int owner_id;
    int team;
    bool active;
    const WeaponDef* weapon_def;  /* Reference to weapon that fired it */
} Bullet;

/* ==========================================================================
 * PATHFINDING - A* SUPPORT
 * ========================================================================== */

typedef struct {
    int x, y;
    float g_cost;           /* Cost from start */
    float h_cost;           /* Heuristic to goal */
    float f_cost;           /* g + h */
    int parent_idx;         /* Index of parent in closed set */
} PathNode;

/* Influence map for tactical AI */
typedef struct {
    float threat[LEVEL_SIZE];       /* Threat level at each tile */
    float visibility[LEVEL_SIZE];   /* How exposed each tile is */
    float cover_value[LEVEL_SIZE];  /* Cover quality at each tile */
    int last_update;                /* Frame when last updated */
} InfluenceMap;

/* ==========================================================================
 * LEVEL SYSTEM
 * ========================================================================== */

typedef struct {
    int x, y, width, height;
    bool connected;
} Room;

typedef struct {
    uint8_t tiles[LEVEL_SIZE];
    Room rooms[MAX_ROOMS];
    int room_count;
    float spawn_x, spawn_y;
    InfluenceMap influence;         /* Tactical influence map */
} Level;

/* ==========================================================================
 * SQUAD MANAGEMENT
 * ========================================================================== */

typedef struct {
    SquadInfo squads[MAX_SQUADS];
    int squad_count;
} SquadManager;

/* ==========================================================================
 * OBJECTIVE SYSTEM
 * ========================================================================== */

typedef enum {
    OBJECTIVE_ELIMINATE_ALL,        /* Kill all enemies */
    OBJECTIVE_REACH_EXIT,           /* Reach the exit point */
    OBJECTIVE_SURVIVE_TIME,         /* Survive for N frames */
    OBJECTIVE_COLLECT_INTEL         /* Find intel items */
} ObjectiveType;

typedef struct {
    ObjectiveType type;
    bool completed;
    float exit_x, exit_y;           /* For REACH_EXIT: exit location */
    int survive_frames;             /* For SURVIVE_TIME: target frames */
    int intel_collected;            /* For COLLECT_INTEL: items found */
    int intel_required;             /* For COLLECT_INTEL: items needed */
} Objective;

/* ==========================================================================
 * GAME STATE
 * ========================================================================== */

typedef struct {
    Entity entities[MAX_ENTITIES];
    int entity_count;
    Bullet bullets[MAX_BULLETS];
    int bullet_count;
    Level level;
    float camera_x, camera_y;
    int player_id;
    int frame;
    uint32_t rng_state;
    SquadManager squads;            /* Squad coordination system */

    /* Objective system */
    Objective objective;
    bool game_won;
    bool game_over;
} GameState;

/* ==========================================================================
 * VIEWPORT (Forward declaration - defined in vis module)
 * ========================================================================== */

typedef struct Viewport Viewport;

/* ==========================================================================
 * UTILITY FUNCTIONS
 * ========================================================================== */

/* Weapon system functions */
WeaponStats weapon_get_stats(const WeaponInstance* weapon);
void weapon_apply_mod(WeaponInstance* weapon, WeaponMod mod);
void weapon_remove_mod(WeaponInstance* weapon, WeaponMod mod);
bool weapon_has_mod(const WeaponInstance* weapon, WeaponMod mod);

/* Threat assessment functions */
float calculate_threat_level(const Entity* self, const Entity* other, float distance, bool visible);
void update_threat_list(Entity* entity, const GameState* game);

/* Steering behavior functions */
Vec2 steering_seek(const Entity* entity, Vec2 target);
Vec2 steering_flee(const Entity* entity, Vec2 threat);
Vec2 steering_arrive(const Entity* entity, Vec2 target, float slow_radius);
Vec2 steering_wander(const Entity* entity, uint32_t* rng);
Vec2 steering_separation(const Entity* entity, const GameState* game);
Vec2 steering_combine(const SteeringForces* forces, float max_force);

#endif /* SHOOTER_TYPES_H */
