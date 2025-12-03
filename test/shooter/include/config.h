/*
 * Shooter Game Configuration Constants
 * All game-wide configuration values are centralized here.
 */

#ifndef SHOOTER_CONFIG_H
#define SHOOTER_CONFIG_H

#include <math.h>

/* ==========================================================================
 * MATHEMATICAL CONSTANTS
 * ========================================================================== */

#ifndef PI
#define PI 3.14159265359f
#endif

/* ==========================================================================
 * MEMORY ALIGNMENT
 * ========================================================================== */

#define ALIGN32 __attribute__((aligned(32)))

/* ==========================================================================
 * FMA INTRINSIC FALLBACKS (for platforms without FMA support, e.g., Emscripten)
 * ========================================================================== */

#ifndef __FMA__
#include <immintrin.h>

/* Emulate FMA intrinsics using separate multiply and add operations */
#ifndef _mm256_fmadd_ps
#define _mm256_fmadd_ps(a, b, c)  _mm256_add_ps(_mm256_mul_ps(a, b), c)
#endif
#ifndef _mm256_fnmadd_ps
#define _mm256_fnmadd_ps(a, b, c) _mm256_sub_ps(c, _mm256_mul_ps(a, b))
#endif
#ifndef _mm256_fmsub_ps
#define _mm256_fmsub_ps(a, b, c)  _mm256_sub_ps(_mm256_mul_ps(a, b), c)
#endif
#ifndef _mm256_fnmsub_ps
#define _mm256_fnmsub_ps(a, b, c) _mm256_sub_ps(_mm256_xor_ps(_mm256_mul_ps(a, b), _mm256_set1_ps(-0.0f)), c)
#endif

/* SSE FMA fallbacks */
#ifndef _mm_fmadd_ps
#define _mm_fmadd_ps(a, b, c)  _mm_add_ps(_mm_mul_ps(a, b), c)
#endif
#ifndef _mm_fmadd_ss
#define _mm_fmadd_ss(a, b, c)  _mm_add_ss(_mm_mul_ss(a, b), c)
#endif

#endif /* __FMA__ */

/* ==========================================================================
 * EMSCRIPTEN SIMD COMPATIBILITY HELPERS
 * ========================================================================== */

#ifdef __EMSCRIPTEN__
/* Emscripten's _mm256_storeu_si256 expects __m256i_u* type */
#define STOREU_SI256(ptr, val) _mm256_storeu_si256((__m256i_u*)(ptr), val)

/* Extract first float from __m256 - Emscripten doesn't support [0] indexing */
static inline float mm256_extract_first_ps(__m256 v) {
    return _mm_cvtss_f32(_mm256_castps256_ps128(v));
}
#define AVX_EXTRACT_F0(v) mm256_extract_first_ps(v)
#else
#define STOREU_SI256(ptr, val) _mm256_storeu_si256((__m256i*)(ptr), val)
#define AVX_EXTRACT_F0(v) (v)[0]
#endif

/* ==========================================================================
 * LEVEL CONFIGURATION
 * ========================================================================== */

#define LEVEL_WIDTH     256
#define LEVEL_HEIGHT    128
#define LEVEL_SIZE      (LEVEL_WIDTH * LEVEL_HEIGHT)

#define MAX_ROOMS       24

/* Tile types */
#define TILE_FLOOR          0
#define TILE_WALL           1
#define TILE_COVER          2   /* Half-height cover (crates, barriers) */
#define TILE_WALL_H         3   /* Horizontal wall segment */
#define TILE_WALL_V         4   /* Vertical wall segment */
#define TILE_CORNER_TL      5   /* Top-left corner */
#define TILE_CORNER_TR      6   /* Top-right corner */
#define TILE_CORNER_BL      7   /* Bottom-left corner */
#define TILE_CORNER_BR      8   /* Bottom-right corner */
#define TILE_DOOR           9   /* Doorway */
#define TILE_PILLAR         10  /* Structural pillar */
#define TILE_WATER          11  /* Water/hazard (impassable) */
#define TILE_GRATE          12  /* Metal grating floor */
#define TILE_CARPET         13  /* Carpeted floor */
#define TILE_TERMINAL       14  /* Computer terminal (cover) */
#define TILE_CRATE          15  /* Wooden crate (cover) */
#define TILE_BARREL         16  /* Barrel (cover) */
#define TILE_VENT           17  /* Vent grating */
#define TILE_DEBRIS         18  /* Floor debris */
#define TILE_GRASS          19  /* Outdoor grass */
#define TILE_CONCRETE       20  /* Concrete floor */

/* ==========================================================================
 * ENTITY LIMITS
 * ========================================================================== */

#define MAX_ENTITIES        64
#define MAX_BULLETS         256
#define MAX_PATH_NODES      512
#define MAX_TRACKED_THREATS 8       /* Max threats per entity */
#define MAX_SQUAD_SIZE      4       /* Max entities per squad */
#define MAX_SQUADS          8       /* Max squads in game */

/* ==========================================================================
 * SOUND PROPAGATION
 * ========================================================================== */

#define SOUND_RADIUS_SHOT    25.0f
#define SOUND_RADIUS_WALK    5.0f
#define SOUND_RADIUS_RUN     12.0f

#define SOUND_WALK_BASE      4.0f
#define SOUND_RUN_BASE       10.0f
#define SOUND_SHOT_BASE      30.0f
#define CORRIDOR_SOUND_MULT  1.5f  /* Sound travels further in corridors */

/* ==========================================================================
 * PLAYER CONFIGURATION
 * ========================================================================== */

#define PLAYER_MAX_HEALTH    250.0f
#define PLAYER_MAX_STAMINA   100.0f
#define PLAYER_WALK_SPEED    0.15f
#define PLAYER_RUN_SPEED     0.3f
#define STAMINA_DRAIN_RATE   0.5f
#define STAMINA_REGEN_RATE   0.2f

/* ==========================================================================
 * COMBAT CONFIGURATION
 * ========================================================================== */

#define BULLET_SPEED         1.5f
#define RELOAD_TIME          60  /* frames */

/* ==========================================================================
 * MEDPEN CONFIGURATION
 * ========================================================================== */

#define MEDPEN_HEAL_AMOUNT       75.0f   /* Health restored per medpen */
#define MEDPEN_USE_TIME          8       /* Frames to use medpen (~250ms at 30 FPS) */
#define MEDPEN_MAX               5       /* Maximum medpens player can carry */

/* Movement commitment (prevents jittering) */
#define MOVE_COMMIT_FRAMES       12      /* Min frames to commit to a direction */
#define MOVE_COMMIT_DIST         1.5f    /* Distance to reach before allowing direction change */
#define RAPID_DAMAGE_THRESHOLD   40.0f   /* If lost this much health quickly, seek cover to heal */
#define RAPID_DAMAGE_WINDOW      60      /* Frames to track rapid damage */

/* ==========================================================================
 * ENTITY STATES
 * ========================================================================== */

/* Graduated awareness levels */
#define STATE_IDLE       0   /* Stationary, unaware */
#define STATE_PATROL     1   /* Moving, unaware */
#define STATE_SUSPICIOUS 2   /* Heard something, investigating */
#define STATE_ALERT      3   /* Saw something briefly, searching */
#define STATE_COMBAT     4   /* Actively fighting */
#define STATE_HUNTING    5   /* Lost sight, searching last known position */
#define STATE_RELOAD     6
#define STATE_HIDING     7   /* Behind cover, planning */
#define STATE_DEAD       8
#define STATE_HEALING    9   /* Using medpen */
#define STATE_FLANKING   10  /* Executing flank maneuver */
#define STATE_SUPPORTING 11  /* Moving to support ally */
#define STATE_RETREATING 12  /* Tactical retreat */

/* ==========================================================================
 * ENEMY TYPES (legacy - kept for compatibility)
 * ========================================================================== */

#define ENEMY_GRUNT      0   /* Basic enemy, low HP, aggressive */
#define ENEMY_SNIPER     1   /* Long range, patient, high damage */
#define ENEMY_RUSHER     2   /* Fast, close range, low accuracy */
#define ENEMY_HEAVY      3   /* Slow, high HP, suppressive fire */

/* ==========================================================================
 * VISION CONFIGURATION
 * ========================================================================== */

#define VIEW_CONE_ANGLE      (PI * 0.4f)   /* 72 degrees (36 each side) */
#define VIEW_DISTANCE        30.0f
#define PERIPHERAL_ANGLE     (PI * 0.7f)   /* Wider but less sensitive */
#define PERIPHERAL_DISTANCE  15.0f

/* ==========================================================================
 * STEERING BEHAVIOR CONFIGURATION
 * ========================================================================== */

#define STEERING_MAX_FORCE       0.5f   /* Maximum steering force per frame */
#define STEERING_SEPARATION_DIST 3.0f   /* Distance to maintain from allies */
#define STEERING_ARRIVE_SLOW_RAD 5.0f   /* Start slowing at this distance */
#define STEERING_WANDER_RADIUS   2.0f   /* Wander circle radius */
#define STEERING_WANDER_DIST     4.0f   /* Distance to wander circle */
#define STEERING_WANDER_JITTER   0.5f   /* How much wander target moves */

/* ==========================================================================
 * AI CONFIGURATION
 * ========================================================================== */

#define AI_THREAT_MEMORY_TIME    300    /* Frames to remember threat (10 sec) */
#define AI_FLANK_TIMEOUT         90     /* Frames before trying new flank */
#define AI_COORDINATION_RADIUS   20.0f  /* Distance for squad coordination */
#define AI_SUPPRESSION_THRESHOLD 3      /* Shots needed to count as suppression */
#define AI_RETREAT_HEALTH_PCTG   0.25f  /* Health % to trigger retreat */
#define AI_COURAGE_BONUS_SQUAD   0.2f   /* Courage bonus when squad present */

/* Utility AI weights (base values - modified by archetype) */
#define AI_WEIGHT_ATTACK_BASE    1.0f
#define AI_WEIGHT_DEFEND_BASE    0.8f
#define AI_WEIGHT_HEAL_BASE      1.2f
#define AI_WEIGHT_FLANK_BASE     0.6f
#define AI_WEIGHT_RETREAT_BASE   0.5f
#define AI_WEIGHT_INVESTIGATE    0.7f
#define AI_WEIGHT_PATROL_BASE    0.3f
#define AI_WEIGHT_SUPPORT_BASE   0.9f

/* ==========================================================================
 * PATHFINDING CONFIGURATION
 * ========================================================================== */

#define PATHFIND_MAX_ITERATIONS  2048   /* Max A* iterations */
#define PATHFIND_DIAGONAL_COST   1.414f /* sqrt(2) for diagonal movement */
#define PATHFIND_STRAIGHT_COST   1.0f   /* Cost for cardinal movement */
#define PATHFIND_COVER_BONUS     -0.3f  /* Prefer paths through cover */
#define PATHFIND_EXPOSED_PENALTY 0.5f   /* Avoid exposed tiles */

/* Influence map update frequency */
#define INFLUENCE_UPDATE_INTERVAL 30    /* Frames between influence updates */
#define INFLUENCE_THREAT_DECAY    0.95f /* Threat decay per update */

/* ==========================================================================
 * FRAME RATE
 * ========================================================================== */

#define FPS 30

#endif /* SHOOTER_CONFIG_H */
