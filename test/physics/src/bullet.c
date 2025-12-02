/*
 * Comprehensive 2D Shooter Simulation
 * All math operations implemented in AVX/AVX2
 * Features:
 * - Procedurally generated levels with rooms and corridors
 * - Multiple enemy types with different AI behaviors
 * - Sound-based enemy detection system
 * - Health, stamina, ammo, magazine mechanics
 * - Smart player AI that avoids getting shot
 * - Scrolling viewport that follows the player
 */

#include "common.h"
#include <string.h>
#include "math_avx.h"
#include <stdbool.h>

// ============================================================================
// CONFIGURATION CONSTANTS
// ============================================================================

#define LEVEL_WIDTH     128
#define LEVEL_HEIGHT    64
#define LEVEL_SIZE      (LEVEL_WIDTH * LEVEL_HEIGHT)

#define MAX_ENTITIES    64
#define MAX_BULLETS     256
#define MAX_ROOMS       16
#define MAX_PATH_NODES  512

#define SOUND_RADIUS_SHOT    25.0f
#define SOUND_RADIUS_WALK    5.0f
#define SOUND_RADIUS_RUN     12.0f

#define PLAYER_MAX_HEALTH    100.0f
#define PLAYER_MAX_STAMINA   100.0f
#define PLAYER_WALK_SPEED    0.15f
#define PLAYER_RUN_SPEED     0.3f
#define STAMINA_DRAIN_RATE   0.5f
#define STAMINA_REGEN_RATE   0.2f

#define BULLET_SPEED         1.5f
#define RELOAD_TIME          60  // frames

// Tile types
#define TILE_FLOOR      0
#define TILE_WALL       1
#define TILE_COVER      2   // Half-height cover

// Entity states - graduated awareness levels
#define STATE_IDLE       0   // Stationary, unaware
#define STATE_PATROL     1   // Moving, unaware
#define STATE_SUSPICIOUS 2   // Heard something, investigating
#define STATE_ALERT      3   // Saw something briefly, searching
#define STATE_COMBAT     4   // Actively fighting
#define STATE_HUNTING    5   // Lost sight, searching last known position
#define STATE_RELOAD     6
#define STATE_HIDING     7   // Behind cover, planning
#define STATE_DEAD       8
#define STATE_HEALING    9   // Using medpen

// Medpen parameters
#define MEDPEN_HEAL_AMOUNT   75.0f   // Health restored per medpen
#define MEDPEN_USE_TIME      45      // Frames to use medpen (~1.5 seconds)
#define MEDPEN_MAX           5       // Maximum medpens player can carry
#define RAPID_DAMAGE_THRESHOLD 40.0f // If lost this much health quickly, seek cover to heal
#define RAPID_DAMAGE_WINDOW  60      // Frames to track rapid damage

// Enemy types
#define ENEMY_GRUNT      0   // Basic enemy, low HP, aggressive
#define ENEMY_SNIPER     1   // Long range, patient, high damage
#define ENEMY_RUSHER     2   // Fast, close range, low accuracy
#define ENEMY_HEAVY      3   // Slow, high HP, suppressive fire

// Vision cone parameters
#define VIEW_CONE_ANGLE     (PI * 0.4f)   // 72 degrees (36 each side)
#define VIEW_DISTANCE       30.0f
#define PERIPHERAL_ANGLE    (PI * 0.7f)   // Wider but less sensitive
#define PERIPHERAL_DISTANCE 15.0f

// Sound parameters
#define SOUND_WALK_BASE     4.0f
#define SOUND_RUN_BASE      10.0f
#define SOUND_SHOT_BASE     30.0f
#define CORRIDOR_SOUND_MULT 1.5f  // Sound travels further in corridors

// ============================================================================
// DATA STRUCTURES
// ============================================================================

typedef struct ALIGN32 {
    float x[8];
    float y[8];
} Vec2x8;

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    int mag_size;       // Bullets per magazine
    int mag_current;    // Current bullets in mag
    int reserve;        // Reserve ammo
    int reload_time;    // Frames to reload
    int fire_rate;      // Frames between shots
    float damage;
    float accuracy;     // 0-1, affects spread
    float range;
} Weapon;

typedef struct {
    float x, y;
    float vx, vy;
    float health;
    float stamina;
    int state;
    int prev_state;             // For returning after reload/hiding
    int type;                   // Entity type (player=0, enemy types 1-4)
    int team;                   // 0=player, 1=enemy
    Weapon weapon;
    int fire_cooldown;
    int reload_timer;
    float alert_x, alert_y;     // Last known threat position
    int alert_timer;
    int suspicious_timer;       // Time spent investigating
    float patrol_x, patrol_y;   // Patrol target
    bool is_running;
    bool is_crouching;          // Behind cover
    bool alive;
    float facing_angle;
    float view_cone_angle;      // How wide the view cone is
    float view_distance;        // How far they can see
    int target_id;              // Who this entity is targeting
    int seen_enemy_id;          // Last enemy spotted
    int frames_target_visible;  // How long current target has been in view
    float last_seen_x, last_seen_y;  // Where target was last seen
    float cover_x, cover_y;     // Nearby cover position
    bool has_cover_nearby;
    int steps_since_sound;      // Counter for footstep sound generation
    float last_damage_x, last_damage_y;  // Direction damage came from
    int damage_react_timer;     // Frames since last hit
    float prev_health;          // To detect damage taken
    int stalemate_timer;        // How long stuck without progress
    float last_combat_x, last_combat_y;  // Position when entering combat/hiding
    int medpens;                // Number of medpens available
    int healing_timer;          // Frames remaining in healing animation
    float health_at_damage_start;  // Health when damage started (for rapid damage detection)
    int rapid_damage_timer;     // Timer to track rapid health loss
} Entity;

typedef struct {
    float x, y;
    float vx, vy;
    float damage;
    int owner_id;
    int team;
    bool active;
} Bullet;

typedef struct {
    int x, y, width, height;
    bool connected;
} Room;

typedef struct {
    uint8_t tiles[LEVEL_SIZE];
    Room rooms[MAX_ROOMS];
    int room_count;
    float spawn_x, spawn_y;
} Level;

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
} GameState;

// Forward declaration for sound propagation
static void propagate_sound(GameState* game, float x, float y, float radius);

// ============================================================================
// RANDOM NUMBER GENERATOR (xorshift)
// ============================================================================

static inline uint32_t xorshift32(uint32_t* state) {
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

static inline float randf(uint32_t* state) {
    return (float)(xorshift32(state) & 0xFFFFFF) / (float)0xFFFFFF;
}

static inline float randf_range(uint32_t* state, float min, float max) {
    return min + randf(state) * (max - min);
}

static inline int randi_range(uint32_t* state, int min, int max) {
    return min + (int)(xorshift32(state) % (max - min + 1));
}

// ============================================================================
// AVX MATH UTILITIES
// ============================================================================

// Distance calculation for 8 entity pairs at once
static inline __m256 avx_distance_squared_8(
    __m256 x1, __m256 y1,
    __m256 x2, __m256 y2
) {
    __m256 dx = _mm256_sub_ps(x2, x1);
    __m256 dy = _mm256_sub_ps(y2, y1);
    return fmadd_ps(dy, dy, _mm256_mul_ps(dx, dx));
}

// Fast inverse square root using AVX
static inline __m256 avx_rsqrt(__m256 x) {
    __m256 half = _mm256_set1_ps(0.5f);
    __m256 three = _mm256_set1_ps(3.0f);
    __m256 rsqrt = _mm256_rsqrt_ps(x);
    // Newton-Raphson iteration for better precision
    __m256 rsqrt2 = _mm256_mul_ps(rsqrt, rsqrt);
    __m256 muls = _mm256_mul_ps(_mm256_mul_ps(x, rsqrt2), rsqrt);
    return _mm256_mul_ps(_mm256_mul_ps(half, rsqrt), _mm256_sub_ps(three, muls));
}

// Normalize 8 vectors at once
static inline void avx_normalize_8(
    __m256* vx, __m256* vy
) {
    __m256 len_sq = fmadd_ps(*vy, *vy, _mm256_mul_ps(*vx, *vx));
    __m256 inv_len = avx_rsqrt(len_sq);
    // Avoid NaN for zero-length vectors
    __m256 zero_mask = _mm256_cmp_ps(len_sq, _mm256_setzero_ps(), _CMP_EQ_OQ);
    inv_len = _mm256_andnot_ps(zero_mask, inv_len);
    *vx = _mm256_mul_ps(*vx, inv_len);
    *vy = _mm256_mul_ps(*vy, inv_len);
}

// Line-of-sight check using AVX (raycast against walls)
// Returns 8 masks: 1 if blocked, 0 if clear
__attribute__((unused))
static __m256 avx_raycast_8(
    const Level* level,
    __m256 x1, __m256 y1,
    __m256 x2, __m256 y2,
    int steps
) {
    __m256 result = _mm256_setzero_ps();
    __m256 dx = _mm256_sub_ps(x2, x1);
    __m256 dy = _mm256_sub_ps(y2, y1);

    __m256 inv_steps = _mm256_set1_ps(1.0f / (float)steps);
    dx = _mm256_mul_ps(dx, inv_steps);
    dy = _mm256_mul_ps(dy, inv_steps);

    __m256 cx = x1;
    __m256 cy = y1;

    __m256 level_w = _mm256_set1_ps((float)LEVEL_WIDTH);
    __m256 level_h = _mm256_set1_ps((float)LEVEL_HEIGHT);
    __m256 zero = _mm256_setzero_ps();

    for (int i = 0; i < steps; i++) {
        cx = _mm256_add_ps(cx, dx);
        cy = _mm256_add_ps(cy, dy);

        // Clamp to level bounds
        __m256 ix = _mm256_max_ps(zero, _mm256_min_ps(cx, _mm256_sub_ps(level_w, _mm256_set1_ps(1.0f))));
        __m256 iy = _mm256_max_ps(zero, _mm256_min_ps(cy, _mm256_sub_ps(level_h, _mm256_set1_ps(1.0f))));

        // Convert to integer indices and check tiles
        __m256i ixi = _mm256_cvttps_epi32(ix);
        __m256i iyi = _mm256_cvttps_epi32(iy);

        // Extract and check each tile
        ALIGN32 int32_t xi[8], yi[8];
        _mm256_store_si256((__m256i*)xi, ixi);
        _mm256_store_si256((__m256i*)yi, iyi);

        ALIGN32 float blocked[8];
        for (int j = 0; j < 8; j++) {
            int idx = xi[j] + yi[j] * LEVEL_WIDTH;
            if (idx >= 0 && idx < LEVEL_SIZE) {
                blocked[j] = (level->tiles[idx] == TILE_WALL) ? 1.0f : 0.0f;
            } else {
                blocked[j] = 1.0f;  // Out of bounds = blocked
            }
        }

        __m256 blocked_vec = _mm256_load_ps(blocked);
        result = _mm256_or_ps(result, blocked_vec);
    }

    return result;
}

// Update 8 bullet positions using AVX
static void avx_update_bullets_8(
    float* x, float* y,
    float* vx, float* vy,
    float dt
) {
    __m256 px = _mm256_loadu_ps(x);
    __m256 py = _mm256_loadu_ps(y);
    __m256 pvx = _mm256_loadu_ps(vx);
    __m256 pvy = _mm256_loadu_ps(vy);
    __m256 vdt = _mm256_set1_ps(dt);

    px = fmadd_ps(pvx, vdt, px);
    py = fmadd_ps(pvy, vdt, py);

    _mm256_storeu_ps(x, px);
    _mm256_storeu_ps(y, py);
}

// Check bullet-entity collisions (8 bullets vs 1 entity)
// Used for batch collision detection in update_physics
static __m256 avx_bullet_collision_8(
    __m256 bx, __m256 by,
    float ex, float ey,
    float radius
) __attribute__((unused));

static __m256 avx_bullet_collision_8(
    __m256 bx, __m256 by,
    float ex, float ey,
    float radius
) {
    __m256 vex = _mm256_set1_ps(ex);
    __m256 vey = _mm256_set1_ps(ey);
    __m256 vr2 = _mm256_set1_ps(radius * radius);

    __m256 dist2 = avx_distance_squared_8(bx, by, vex, vey);
    return _mm256_cmp_ps(dist2, vr2, _CMP_LE_OQ);
}

// Calculate direction vectors from 8 entities to a single target
// Used for batch AI direction calculations
static void avx_direction_to_target_8(
    __m256 src_x, __m256 src_y,
    float tgt_x, float tgt_y,
    __m256* out_dx, __m256* out_dy
) __attribute__((unused));

static void avx_direction_to_target_8(
    __m256 src_x, __m256 src_y,
    float tgt_x, float tgt_y,
    __m256* out_dx, __m256* out_dy
) {
    __m256 tx = _mm256_set1_ps(tgt_x);
    __m256 ty = _mm256_set1_ps(tgt_y);

    *out_dx = _mm256_sub_ps(tx, src_x);
    *out_dy = _mm256_sub_ps(ty, src_y);
    avx_normalize_8(out_dx, out_dy);
}

// Process 8 sound events - check if entities hear them
static __m256 avx_sound_detection_8(
    __m256 listener_x, __m256 listener_y,
    float sound_x, float sound_y,
    float sound_radius
) {
    __m256 sx = _mm256_set1_ps(sound_x);
    __m256 sy = _mm256_set1_ps(sound_y);
    __m256 r2 = _mm256_set1_ps(sound_radius * sound_radius);

    __m256 dist2 = avx_distance_squared_8(listener_x, listener_y, sx, sy);
    return _mm256_cmp_ps(dist2, r2, _CMP_LE_OQ);
}

// Calculate threat direction and magnitude for player AI (8 threats at once)
__attribute__((unused))
static void avx_calculate_threats_8(
    float player_x, float player_y,
    __m256 enemy_x, __m256 enemy_y,
    __m256 enemy_facing_x, __m256 enemy_facing_y,
    __m256 enemy_alive,
    __m256* threat_x, __m256* threat_y
) {
    __m256 px = _mm256_set1_ps(player_x);
    __m256 py = _mm256_set1_ps(player_y);

    // Direction from enemy to player
    __m256 to_player_x = _mm256_sub_ps(px, enemy_x);
    __m256 to_player_y = _mm256_sub_ps(py, enemy_y);

    // Distance squared
    __m256 dist2 = fmadd_ps(to_player_y, to_player_y, _mm256_mul_ps(to_player_x, to_player_x));
    __m256 inv_dist = avx_rsqrt(dist2);

    // Normalized direction
    __m256 dir_x = _mm256_mul_ps(to_player_x, inv_dist);
    __m256 dir_y = _mm256_mul_ps(to_player_y, inv_dist);

    // Dot product with enemy facing (how much enemy is looking at player)
    __m256 dot = fmadd_ps(dir_y, enemy_facing_y, _mm256_mul_ps(dir_x, enemy_facing_x));

    // Threat is higher when enemy is facing player and closer
    __m256 threat_level = _mm256_mul_ps(
        _mm256_max_ps(dot, _mm256_setzero_ps()),  // Only positive dot products
        _mm256_mul_ps(inv_dist, _mm256_set1_ps(100.0f))  // Scale by inverse distance
    );

    // Zero out dead enemies
    threat_level = _mm256_and_ps(threat_level, enemy_alive);

    // Accumulate flee direction (opposite of threat direction)
    *threat_x = _mm256_mul_ps(_mm256_sub_ps(_mm256_setzero_ps(), dir_x), threat_level);
    *threat_y = _mm256_mul_ps(_mm256_sub_ps(_mm256_setzero_ps(), dir_y), threat_level);
}

// ============================================================================
// AVX BATCH PROCESSING UTILITIES
// ============================================================================

// Process 8 distance calculations at once, returns sqrt distances
__attribute__((unused))
static inline void avx_distances_8(
    const float* src_x, const float* src_y,
    float target_x, float target_y,
    float* out_dist
) {
    __m256 sx = _mm256_loadu_ps(src_x);
    __m256 sy = _mm256_loadu_ps(src_y);
    __m256 tx = _mm256_set1_ps(target_x);
    __m256 ty = _mm256_set1_ps(target_y);

    __m256 dx = _mm256_sub_ps(tx, sx);
    __m256 dy = _mm256_sub_ps(ty, sy);
    __m256 dist_sq = fmadd_ps(dy, dy, _mm256_mul_ps(dx, dx));
    __m256 dist = _mm256_sqrt_ps(dist_sq);
    _mm256_storeu_ps(out_dist, dist);
}

// Batch normalize 8 2D vectors
__attribute__((unused))
static inline void avx_normalize_vectors_8(
    float* vx, float* vy
) {
    __m256 x = _mm256_loadu_ps(vx);
    __m256 y = _mm256_loadu_ps(vy);
    avx_normalize_8(&x, &y);
    _mm256_storeu_ps(vx, x);
    _mm256_storeu_ps(vy, y);
}

// Batch min/max clamping
__attribute__((unused))
static inline void avx_clamp_8(float* values, float min_val, float max_val) {
    __m256 v = _mm256_loadu_ps(values);
    __m256 vmin = _mm256_set1_ps(min_val);
    __m256 vmax = _mm256_set1_ps(max_val);
    v = _mm256_max_ps(vmin, _mm256_min_ps(vmax, v));
    _mm256_storeu_ps(values, v);
}

// Batch multiply-add: out = a * b + c
__attribute__((unused))
static inline void avx_fmadd_8(
    const float* a, const float* b, const float* c, float* out
) {
    __m256 va = _mm256_loadu_ps(a);
    __m256 vb = _mm256_loadu_ps(b);
    __m256 vc = _mm256_loadu_ps(c);
    __m256 result = fmadd_ps(va, vb, vc);
    _mm256_storeu_ps(out, result);
}

// Batch velocity update with friction
__attribute__((unused))
static inline void avx_apply_friction_8(float* vx, float* vy, float friction) {
    __m256 x = _mm256_loadu_ps(vx);
    __m256 y = _mm256_loadu_ps(vy);
    __m256 f = _mm256_set1_ps(friction);
    x = _mm256_mul_ps(x, f);
    y = _mm256_mul_ps(y, f);
    _mm256_storeu_ps(vx, x);
    _mm256_storeu_ps(vy, y);
}

// Batch position update: pos += vel * dt
__attribute__((unused))
static inline void avx_integrate_position_8(
    float* x, float* y,
    const float* vx, const float* vy,
    float dt
) {
    __m256 px = _mm256_loadu_ps(x);
    __m256 py = _mm256_loadu_ps(y);
    __m256 pvx = _mm256_loadu_ps(vx);
    __m256 pvy = _mm256_loadu_ps(vy);
    __m256 vdt = _mm256_set1_ps(dt);

    px = fmadd_ps(pvx, vdt, px);
    py = fmadd_ps(pvy, vdt, py);

    _mm256_storeu_ps(x, px);
    _mm256_storeu_ps(y, py);
}

// Batch angle calculation (atan2 approximation for 8 values)
// Fast approximation, not super precise but good for game AI
__attribute__((unused))
static inline void avx_atan2_approx_8(
    const float* y, const float* x, float* out_angle
) {
    // Use scalar atan2 - vectorized atan2 is complex
    // But we batch the memory access
    ALIGN32 float yy[8], xx[8];
    memcpy(yy, y, 8 * sizeof(float));
    memcpy(xx, x, 8 * sizeof(float));
    for (int i = 0; i < 8; i++) {
        out_angle[i] = atan2f(yy[i], xx[i]);
    }
}

// Single scalar distance using SSE (faster than scalar for single calc)
static inline float sse_distance(float x1, float y1, float x2, float y2) {
    __m128 v1 = _mm_set_ps(0, 0, y1, x1);
    __m128 v2 = _mm_set_ps(0, 0, y2, x2);
    __m128 diff = _mm_sub_ps(v2, v1);
    __m128 sq = _mm_mul_ps(diff, diff);
    // Horizontal add: sq.x + sq.y
    __m128 sum = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, 1));
    __m128 dist = _mm_sqrt_ss(sum);
    return _mm_cvtss_f32(dist);
}

// Fast scalar inverse sqrt using SSE
__attribute__((unused))
static inline float sse_rsqrt(float x) {
    __m128 v = _mm_set_ss(x);
    __m128 r = _mm_rsqrt_ss(v);
    // One Newton-Raphson iteration
    __m128 half = _mm_set_ss(0.5f);
    __m128 three = _mm_set_ss(3.0f);
    __m128 r2 = _mm_mul_ss(r, r);
    __m128 muls = _mm_mul_ss(_mm_mul_ss(v, r2), r);
    r = _mm_mul_ss(_mm_mul_ss(half, r), _mm_sub_ss(three, muls));
    return _mm_cvtss_f32(r);
}

// Fast scalar sqrt using SSE
__attribute__((unused))
static inline float sse_sqrt(float x) {
    __m128 v = _mm_set_ss(x);
    __m128 r = _mm_sqrt_ss(v);
    return _mm_cvtss_f32(r);
}

// SSE distance squared (faster when we don't need the actual distance)
static inline float sse_distance_squared(float x1, float y1, float x2, float y2) {
    __m128 v1 = _mm_set_ps(0, 0, y1, x1);
    __m128 v2 = _mm_set_ps(0, 0, y2, x2);
    __m128 diff = _mm_sub_ps(v2, v1);
    __m128 sq = _mm_mul_ps(diff, diff);
    __m128 sum = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, 1));
    return _mm_cvtss_f32(sum);
}

// SSE normalize a 2D vector in-place
static inline void sse_normalize(float* x, float* y) {
    __m128 v = _mm_set_ps(0, 0, *y, *x);
    __m128 sq = _mm_mul_ps(v, v);
    __m128 sum = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, 1));
    __m128 len = _mm_sqrt_ss(sum);

    // Check for zero length
    if (_mm_cvtss_f32(len) < 0.0001f) {
        *x = 0;
        *y = 0;
        return;
    }

    __m128 inv_len = _mm_rcp_ss(len);
    // Newton-Raphson iteration for precision
    __m128 two = _mm_set_ss(2.0f);
    inv_len = _mm_mul_ss(inv_len, _mm_sub_ss(two, _mm_mul_ss(len, inv_len)));

    __m128 inv_broadcast = _mm_shuffle_ps(inv_len, inv_len, 0);
    __m128 result = _mm_mul_ps(v, inv_broadcast);

    ALIGN32 float out[4];
    _mm_store_ps(out, result);
    *x = out[0];
    *y = out[1];
}

// SSE dot product of two 2D vectors
static inline float sse_dot2(float x1, float y1, float x2, float y2) {
    __m128 v1 = _mm_set_ps(0, 0, y1, x1);
    __m128 v2 = _mm_set_ps(0, 0, y2, x2);
    __m128 mul = _mm_mul_ps(v1, v2);
    __m128 sum = _mm_add_ss(mul, _mm_shuffle_ps(mul, mul, 1));
    return _mm_cvtss_f32(sum);
}

// SSE multiply-add for scalars: a * b + c
static inline float sse_fmadd(float a, float b, float c) {
    __m128 va = _mm_set_ss(a);
    __m128 vb = _mm_set_ss(b);
    __m128 vc = _mm_set_ss(c);
    __m128 result = _mm_fmadd_ss(va, vb, vc);
    return _mm_cvtss_f32(result);
}

// SSE linear interpolation: a + t * (b - a)
static inline float sse_lerp(float a, float b, float t) {
    __m128 va = _mm_set_ss(a);
    __m128 vb = _mm_set_ss(b);
    __m128 vt = _mm_set_ss(t);
    __m128 diff = _mm_sub_ss(vb, va);
    __m128 result = _mm_fmadd_ss(vt, diff, va);
    return _mm_cvtss_f32(result);
}

// SSE clamp a value between min and max
static inline float sse_clamp(float val, float min_val, float max_val) {
    __m128 v = _mm_set_ss(val);
    __m128 vmin = _mm_set_ss(min_val);
    __m128 vmax = _mm_set_ss(max_val);
    v = _mm_max_ss(vmin, _mm_min_ss(vmax, v));
    return _mm_cvtss_f32(v);
}

// Check if a position is walkable (not a wall)
static inline bool is_walkable(const Level* level, int x, int y) {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) return false;
    return level->tiles[x + y * LEVEL_WIDTH] != TILE_WALL;
}

// Check if a position is cover
static inline bool is_cover(const Level* level, int x, int y) {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) return false;
    return level->tiles[x + y * LEVEL_WIDTH] == TILE_COVER;
}

// Check if target is within view cone of observer
// Returns: 0 = not visible, 1 = peripheral vision, 2 = direct vision
static int check_view_cone(
    const Level* level,
    float obs_x, float obs_y, float obs_angle,
    float tgt_x, float tgt_y,
    float cone_angle, float view_dist,
    bool check_los
) {
    // SSE: Calculate dx, dy, and distance in one go
    __m128 obs = _mm_set_ps(0, 0, obs_y, obs_x);
    __m128 tgt = _mm_set_ps(0, 0, tgt_y, tgt_x);
    __m128 delta = _mm_sub_ps(tgt, obs);

    ALIGN32 float delta_arr[4];
    _mm_store_ps(delta_arr, delta);
    float dx = delta_arr[0];
    float dy = delta_arr[1];

    // SSE distance calculation
    __m128 sq = _mm_mul_ps(delta, delta);
    __m128 sum = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, 1));
    __m128 dist_vec = _mm_sqrt_ss(sum);
    float dist = _mm_cvtss_f32(dist_vec);

    // SSE comparison: dist > view_dist * 1.5f
    __m128 max_dist = _mm_set_ss(view_dist * 1.5f);
    if (_mm_comigt_ss(dist_vec, max_dist)) return 0;  // Too far even for peripheral

    // Angle to target
    float angle_to_target = atan2f(dy, dx);
    float angle_diff = angle_to_target - obs_angle;

    // Normalize angle difference to [-PI, PI]
    while (angle_diff > PI) angle_diff -= 2*PI;
    while (angle_diff < -PI) angle_diff += 2*PI;

    // SSE absolute value
    __m128 abs_mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
    __m128 angle = _mm_set_ss(angle_diff);
    angle = _mm_and_ps(angle, abs_mask);
    angle_diff = _mm_cvtss_f32(angle);

    int visibility = 0;

    // SSE comparisons for visibility checks
    __m128 periph_angle = _mm_set_ss(PERIPHERAL_ANGLE);
    __m128 periph_dist = _mm_set_ss(PERIPHERAL_DISTANCE);
    __m128 v_cone_angle = _mm_set_ss(cone_angle);
    __m128 v_view_dist = _mm_set_ss(view_dist);
    __m128 v_angle_diff = _mm_set_ss(angle_diff);

    // Check peripheral vision (wider angle, shorter range)
    if (_mm_comilt_ss(v_angle_diff, periph_angle) && _mm_comilt_ss(dist_vec, periph_dist)) {
        visibility = 1;
    }

    // Check direct vision (narrower cone, longer range)
    if (_mm_comilt_ss(v_angle_diff, v_cone_angle) && _mm_comilt_ss(dist_vec, v_view_dist)) {
        visibility = 2;
    }

    if (visibility == 0) return 0;

    // Line of sight check using SSE for step calculation
    if (check_los) {
        int steps = (int)dist + 1;
        __m128 inv_dist = _mm_rcp_ss(dist_vec);
        __m128 step = _mm_mul_ps(delta, _mm_shuffle_ps(inv_dist, inv_dist, 0));

        ALIGN32 float step_arr[4];
        _mm_store_ps(step_arr, step);
        float step_x = step_arr[0];
        float step_y = step_arr[1];

        for (int i = 1; i < steps; i++) {
            int cx = (int)(obs_x + step_x * i);
            int cy = (int)(obs_y + step_y * i);
            if (!is_walkable(level, cx, cy)) {
                return 0;  // Blocked
            }
        }
    }

    return visibility;
}

// Calculate corridor width at a position (for sound propagation)
static float get_corridor_width(const Level* level, float x, float y) {
    int ix = (int)x;
    int iy = (int)y;

    // Count open tiles in cardinal directions
    int open_h = 0, open_v = 0;

    for (int dx = -5; dx <= 5; dx++) {
        if (is_walkable(level, ix + dx, iy)) open_h++;
        else if (dx < 0) open_h = 0;  // Reset if wall found before center
        else break;
    }

    for (int dy = -5; dy <= 5; dy++) {
        if (is_walkable(level, ix, iy + dy)) open_v++;
        else if (dy < 0) open_v = 0;
        else break;
    }

    // Return minimum dimension (narrower = more corridor-like)
    return (float)(open_h < open_v ? open_h : open_v);
}

// Calculate sound radius based on action and location
static float calculate_sound_radius(const Level* level, float x, float y, float base_radius) {
    float corridor_width = get_corridor_width(level, x, y);

    // Sound travels further in narrow corridors
    float mult = 1.0f;
    if (corridor_width < 4.0f) {
        mult = CORRIDOR_SOUND_MULT;
    }

    return base_radius * mult;
}

// Find nearest cover position
static bool find_nearby_cover(const Level* level, float x, float y, float* cover_x, float* cover_y) {
    float best_dist = 1e10f;
    bool found = false;

    for (int dy = -8; dy <= 8; dy++) {
        for (int dx = -8; dx <= 8; dx++) {
            int cx = (int)x + dx;
            int cy = (int)y + dy;

            if (is_cover(level, cx, cy)) {
                // Check if we can stand next to it
                for (int ady = -1; ady <= 1; ady++) {
                    for (int adx = -1; adx <= 1; adx++) {
                        if (is_walkable(level, cx + adx, cy + ady) && !is_cover(level, cx + adx, cy + ady)) {
                            // SSE distance squared calculation
                            float dist = sse_distance_squared(0, 0, (float)dx, (float)dy);
                            if (dist < best_dist) {
                                best_dist = dist;
                                *cover_x = cx + adx + 0.5f;
                                *cover_y = cy + ady + 0.5f;
                                found = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return found;
}

// Check if position has cover from a threat direction
static bool has_cover_from_direction(const Level* level, float x, float y, float threat_x, float threat_y) {
    float dx = threat_x - x;
    float dy = threat_y - y;
    float dist = sse_distance(0, 0, dx, dy);
    if (dist < 0.1f) return false;

    // SSE normalization
    sse_normalize(&dx, &dy);

    // Check if there's cover between us and threat (within 2 tiles)
    for (float t = 0.5f; t < 2.5f; t += 0.5f) {
        int cx = (int)(x + dx * t);
        int cy = (int)(y + dy * t);
        if (is_cover(level, cx, cy) || !is_walkable(level, cx, cy)) {
            return true;
        }
    }

    return false;
}

// BFS pathfinding - finds next step toward goal
static bool find_path(
    const Level* level,
    float start_x, float start_y,
    float end_x, float end_y,
    float* out_next_x, float* out_next_y
) {
    int sx = (int)start_x;
    int sy = (int)start_y;
    int ex = (int)end_x;
    int ey = (int)end_y;

    // Clamp to bounds
    if (sx < 0) sx = 0; if (sx >= LEVEL_WIDTH) sx = LEVEL_WIDTH - 1;
    if (sy < 0) sy = 0; if (sy >= LEVEL_HEIGHT) sy = LEVEL_HEIGHT - 1;
    if (ex < 0) ex = 0; if (ex >= LEVEL_WIDTH) ex = LEVEL_WIDTH - 1;
    if (ey < 0) ey = 0; if (ey >= LEVEL_HEIGHT) ey = LEVEL_HEIGHT - 1;

    // Already at destination
    if (sx == ex && sy == ey) {
        *out_next_x = end_x;
        *out_next_y = end_y;
        return true;
    }

    // Quick direct line check first
    float dx = end_x - start_x;
    float dy = end_y - start_y;
    float len = sse_distance(0, 0, dx, dy);

    if (len > 0.1f) {
        // SSE normalization
        sse_normalize(&dx, &dy);

        // Check if direct path is clear (check a few points along the way)
        bool clear = true;
        for (float t = 0.5f; t <= len && clear; t += 0.5f) {
            int cx = (int)(start_x + dx * t);
            int cy = (int)(start_y + dy * t);
            if (!is_walkable(level, cx, cy)) {
                clear = false;
            }
        }

        if (clear) {
            *out_next_x = start_x + dx;
            *out_next_y = start_y + dy;
            return true;
        }
    }

    // BFS for pathfinding
    #define MAX_BFS 2048
    static int16_t queue_x[MAX_BFS];
    static int16_t queue_y[MAX_BFS];
    static int8_t came_from[LEVEL_SIZE];  // Direction we came from: 0-7, -1 = not visited

    memset(came_from, -1, sizeof(came_from));

    int head = 0, tail = 0;
    queue_x[tail] = ex;
    queue_y[tail] = ey;
    tail++;
    came_from[ex + ey * LEVEL_WIDTH] = 8;  // Mark as visited (source)

    // Direction offsets (8 directions)
    const int dir_x[] = {1, 1, 0, -1, -1, -1, 0, 1};
    const int dir_y[] = {0, 1, 1, 1, 0, -1, -1, -1};

    bool found = false;

    while (head < tail && !found) {
        int cx = queue_x[head];
        int cy = queue_y[head];
        head++;

        // Check all 8 directions
        for (int d = 0; d < 8; d++) {
            int nx = cx + dir_x[d];
            int ny = cy + dir_y[d];

            if (!is_walkable(level, nx, ny)) continue;

            int nidx = nx + ny * LEVEL_WIDTH;
            if (came_from[nidx] != -1) continue;  // Already visited

            came_from[nidx] = d;

            if (nx == sx && ny == sy) {
                found = true;
                break;
            }

            if (tail < MAX_BFS) {
                queue_x[tail] = nx;
                queue_y[tail] = ny;
                tail++;
            }
        }
    }

    if (found) {
        // Trace back one step from start to find next position
        int d = came_from[sx + sy * LEVEL_WIDTH];
        if (d >= 0 && d < 8) {
            // Move in opposite direction of where we came from
            int opp = (d + 4) % 8;
            *out_next_x = start_x + dir_x[opp] * 0.9f;
            *out_next_y = start_y + dir_y[opp] * 0.9f;
            return true;
        }
    }

    // Fallback: try to move in any valid direction toward goal
    float best_dist = 1e10f;
    float best_x = start_x;
    float best_y = start_y;

    for (int d = 0; d < 8; d++) {
        float nx = start_x + dir_x[d];
        float ny = start_y + dir_y[d];

        if (is_walkable(level, (int)nx, (int)ny)) {
            // SSE distance squared
            float dist = sse_distance_squared(nx, ny, end_x, end_y);
            if (dist < best_dist) {
                best_dist = dist;
                best_x = nx;
                best_y = ny;
            }
        }
    }

    *out_next_x = best_x;
    *out_next_y = best_y;
    return best_dist < 1e9f;
    #undef MAX_BFS
}

// ============================================================================
// LEVEL GENERATION
// ============================================================================

static void generate_level(Level* level, uint32_t* rng) {
    // Initialize all as walls
    memset(level->tiles, TILE_WALL, LEVEL_SIZE);
    level->room_count = 0;

    // Generate random rooms
    int attempts = 0;
    while (level->room_count < MAX_ROOMS && attempts < 100) {
        attempts++;

        int w = randi_range(rng, 10, 20);
        int h = randi_range(rng, 8, 14);
        int x = randi_range(rng, 2, LEVEL_WIDTH - w - 2);
        int y = randi_range(rng, 2, LEVEL_HEIGHT - h - 2);

        // Check overlap with existing rooms
        bool overlap = false;
        for (int i = 0; i < level->room_count && !overlap; i++) {
            Room* r = &level->rooms[i];
            if (x < r->x + r->width + 2 && x + w + 2 > r->x &&
                y < r->y + r->height + 2 && y + h + 2 > r->y) {
                overlap = true;
            }
        }

        if (!overlap) {
            Room* room = &level->rooms[level->room_count++];
            room->x = x;
            room->y = y;
            room->width = w;
            room->height = h;
            room->connected = false;

            // Carve room
            for (int ry = y; ry < y + h; ry++) {
                for (int rx = x; rx < x + w; rx++) {
                    level->tiles[rx + ry * LEVEL_WIDTH] = TILE_FLOOR;
                }
            }

            // Add some cover inside larger rooms
            if (w > 8 && h > 6) {
                int covers = randi_range(rng, 1, 3);
                for (int c = 0; c < covers; c++) {
                    int cx = randi_range(rng, x + 2, x + w - 3);
                    int cy = randi_range(rng, y + 2, y + h - 3);
                    level->tiles[cx + cy * LEVEL_WIDTH] = TILE_COVER;
                }
            }
        }
    }

    // Connect rooms with wide corridors (4 tiles wide)
    #define CORRIDOR_WIDTH 4
    for (int i = 1; i < level->room_count; i++) {
        Room* r1 = &level->rooms[i - 1];
        Room* r2 = &level->rooms[i];

        int x1 = r1->x + r1->width / 2;
        int y1 = r1->y + r1->height / 2;
        int x2 = r2->x + r2->width / 2;
        int y2 = r2->y + r2->height / 2;

        // L-shaped corridor
        if (randf(rng) > 0.5f) {
            // Horizontal first
            int sx = x1 < x2 ? x1 : x2;
            int ex = x1 < x2 ? x2 : x1;
            for (int cx = sx; cx <= ex; cx++) {
                for (int w = 0; w < CORRIDOR_WIDTH; w++) {
                    int cy = y1 - CORRIDOR_WIDTH/2 + w;
                    if (cy >= 0 && cy < LEVEL_HEIGHT)
                        level->tiles[cx + cy * LEVEL_WIDTH] = TILE_FLOOR;
                }
            }
            // Then vertical
            int sy = y1 < y2 ? y1 : y2;
            int ey = y1 < y2 ? y2 : y1;
            for (int cy = sy; cy <= ey; cy++) {
                for (int w = 0; w < CORRIDOR_WIDTH; w++) {
                    int cx = x2 - CORRIDOR_WIDTH/2 + w;
                    if (cx >= 0 && cx < LEVEL_WIDTH)
                        level->tiles[cx + cy * LEVEL_WIDTH] = TILE_FLOOR;
                }
            }
        } else {
            // Vertical first
            int sy = y1 < y2 ? y1 : y2;
            int ey = y1 < y2 ? y2 : y1;
            for (int cy = sy; cy <= ey; cy++) {
                for (int w = 0; w < CORRIDOR_WIDTH; w++) {
                    int cx = x1 - CORRIDOR_WIDTH/2 + w;
                    if (cx >= 0 && cx < LEVEL_WIDTH)
                        level->tiles[cx + cy * LEVEL_WIDTH] = TILE_FLOOR;
                }
            }
            // Then horizontal
            int sx = x1 < x2 ? x1 : x2;
            int ex = x1 < x2 ? x2 : x1;
            for (int cx = sx; cx <= ex; cx++) {
                for (int w = 0; w < CORRIDOR_WIDTH; w++) {
                    int cy = y2 - CORRIDOR_WIDTH/2 + w;
                    if (cy >= 0 && cy < LEVEL_HEIGHT)
                        level->tiles[cx + cy * LEVEL_WIDTH] = TILE_FLOOR;
                }
            }
        }

        r2->connected = true;
    }
    #undef CORRIDOR_WIDTH

    // Set spawn point in first room
    if (level->room_count > 0) {
        Room* spawn_room = &level->rooms[0];
        level->spawn_x = spawn_room->x + spawn_room->width / 2.0f;
        level->spawn_y = spawn_room->y + spawn_room->height / 2.0f;
    }
}

// ============================================================================
// ENTITY MANAGEMENT
// ============================================================================

static Weapon create_weapon(int type) {
    Weapon w = {0};
    switch (type) {
        case 0: // Pistol
            w.mag_size = 12;
            w.mag_current = 12;
            w.reserve = 48;
            w.reload_time = 45;
            w.fire_rate = 12;
            w.damage = 20.0f;
            w.accuracy = 0.9f;
            w.range = 30.0f;
            break;
        case 1: // SMG
            w.mag_size = 30;
            w.mag_current = 30;
            w.reserve = 90;
            w.reload_time = 60;
            w.fire_rate = 4;
            w.damage = 12.0f;
            w.accuracy = 0.7f;
            w.range = 20.0f;
            break;
        case 2: // Rifle
            w.mag_size = 20;
            w.mag_current = 20;
            w.reserve = 60;
            w.reload_time = 75;
            w.fire_rate = 8;
            w.damage = 25.0f;
            w.accuracy = 0.85f;
            w.range = 50.0f;
            break;
        case 3: // Shotgun
            w.mag_size = 6;
            w.mag_current = 6;
            w.reserve = 24;
            w.reload_time = 90;
            w.fire_rate = 30;
            w.damage = 45.0f;
            w.accuracy = 0.5f;
            w.range = 10.0f;
            break;
    }
    return w;
}

static int spawn_entity(GameState* game, int type, float x, float y, int team) {
    if (game->entity_count >= MAX_ENTITIES) return -1;

    Entity* e = &game->entities[game->entity_count];
    memset(e, 0, sizeof(Entity));

    e->x = x;
    e->y = y;
    e->type = type;
    e->team = team;
    e->state = STATE_IDLE;
    e->prev_state = STATE_IDLE;
    e->alive = true;
    e->facing_angle = randf(&game->rng_state) * 2.0f * PI;
    e->target_id = -1;
    e->seen_enemy_id = -1;
    e->frames_target_visible = 0;

    // Default view cone (enemies have narrower cones)
    e->view_cone_angle = VIEW_CONE_ANGLE;
    e->view_distance = VIEW_DISTANCE;

    if (team == 0) {
        // Player - significantly stronger than enemies
        e->health = 250.0f;
        e->stamina = PLAYER_MAX_STAMINA;
        e->weapon = create_weapon(2);  // Rifle
        e->weapon.accuracy = 0.95f;    // Very accurate
        e->weapon.damage = 35.0f;      // High damage
        e->weapon.reserve = 200;       // Lots of ammo
        e->view_cone_angle = VIEW_CONE_ANGLE * 1.2f;  // Wider view
        e->view_distance = VIEW_DISTANCE * 1.2f;
        e->medpens = 3;                // Start with 3 medpens
        e->health_at_damage_start = e->health;
    } else {
        // Enemy types - weaker and less accurate
        switch (type) {
            case ENEMY_GRUNT:
                e->health = 40.0f;
                e->stamina = 80.0f;
                e->weapon = create_weapon(0);  // Pistol
                e->weapon.accuracy = 0.5f;     // Poor accuracy
                e->weapon.damage = 8.0f;
                e->view_cone_angle = VIEW_CONE_ANGLE * 0.8f;
                e->view_distance = VIEW_DISTANCE * 0.7f;
                break;
            case ENEMY_SNIPER:
                e->health = 30.0f;
                e->stamina = 60.0f;
                e->weapon = create_weapon(2);  // Rifle
                e->weapon.accuracy = 0.7f;     // Decent but not great
                e->weapon.damage = 15.0f;
                e->view_cone_angle = VIEW_CONE_ANGLE * 0.5f;  // Narrow but long
                e->view_distance = VIEW_DISTANCE * 1.5f;
                break;
            case ENEMY_RUSHER:
                e->health = 25.0f;
                e->stamina = 120.0f;
                e->weapon = create_weapon(1);  // SMG
                e->weapon.accuracy = 0.4f;     // Spray and pray
                e->weapon.damage = 6.0f;
                e->view_cone_angle = VIEW_CONE_ANGLE * 1.0f;
                e->view_distance = VIEW_DISTANCE * 0.6f;  // Short sighted
                break;
            case ENEMY_HEAVY:
                e->health = 80.0f;
                e->stamina = 50.0f;
                e->weapon = create_weapon(3);  // Shotgun
                e->weapon.accuracy = 0.4f;
                e->weapon.damage = 20.0f;
                e->weapon.reserve = 48;
                e->view_cone_angle = VIEW_CONE_ANGLE * 0.9f;
                e->view_distance = VIEW_DISTANCE * 0.5f;
                break;
        }
        // Some enemies carry medpens (heavies always, others sometimes)
        if (type == ENEMY_HEAVY) {
            e->medpens = 1;
        } else if (randf(&game->rng_state) < 0.3f) {
            e->medpens = 1;
        }
        e->health_at_damage_start = e->health;
        e->state = STATE_PATROL;
    }

    return game->entity_count++;
}

// Try to pick up weapon and medpens from nearby dead enemy
static void try_pickup_weapon(GameState* game, Entity* player) {
    for (int i = 0; i < game->entity_count; i++) {
        Entity* e = &game->entities[i];
        if (e->team == player->team || e->alive) continue;

        float dx = e->x - player->x;
        float dy = e->y - player->y;
        float dist = sse_distance(0, 0, dx, dy);

        if (dist < 2.0f) {
            // Check if enemy weapon is better (higher damage)
            if (e->weapon.damage > player->weapon.damage) {
                // Swap weapons
                Weapon old = player->weapon;
                player->weapon = e->weapon;
                player->weapon.mag_current = player->weapon.mag_size;  // Full mag on pickup
                e->weapon = old;  // Leave old weapon on corpse
            }
            // Also grab ammo even if not taking weapon
            else if (e->weapon.reserve > 0) {
                player->weapon.reserve += e->weapon.reserve / 2;
                e->weapon.reserve = 0;
            }

            // Pick up any medpens from the corpse (max 5)
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

static void spawn_bullet(GameState* game, int owner_id, float x, float y, float angle, float accuracy) {
    if (game->bullet_count >= MAX_BULLETS) return;

    Entity* owner = &game->entities[owner_id];

    // Apply accuracy spread
    float spread = (1.0f - accuracy) * 0.3f;
    float actual_angle = angle + randf_range(&game->rng_state, -spread, spread);

    Bullet* b = &game->bullets[game->bullet_count++];
    b->x = x;
    b->y = y;
    b->vx = cosf(actual_angle) * BULLET_SPEED;
    b->vy = sinf(actual_angle) * BULLET_SPEED;
    b->damage = owner->weapon.damage;
    b->owner_id = owner_id;
    b->team = owner->team;
    b->active = true;
}

// ============================================================================
// AI BEHAVIOR
// ============================================================================

static void update_enemy_ai(GameState* game, int entity_id) {
    Entity* e = &game->entities[entity_id];
    if (!e->alive || e->team == 0) return;

    Entity* player = &game->entities[game->player_id];

    float dx = player->x - e->x;
    float dy = player->y - e->y;
    float dist_to_player = sse_distance(0, 0, dx, dy);

    // Check view cone visibility (not just LOS)
    int player_visibility = check_view_cone(
        &game->level,
        e->x, e->y, e->facing_angle,
        player->x, player->y,
        e->view_cone_angle, e->view_distance,
        true  // Check line of sight
    );

    bool can_see_player = player_visibility > 0 && player->alive;
    bool player_in_direct_view = player_visibility == 2;

    // Update target tracking
    if (can_see_player) {
        e->frames_target_visible++;
        e->last_seen_x = player->x;
        e->last_seen_y = player->y;
        e->seen_enemy_id = game->player_id;
    } else {
        e->frames_target_visible = 0;
    }

    // Check for nearby cover
    e->has_cover_nearby = find_nearby_cover(&game->level, e->x, e->y, &e->cover_x, &e->cover_y);

    // State machine with graduated awareness
    switch (e->state) {
        case STATE_IDLE:
            // Standing still, looking around occasionally
            if (randf(&game->rng_state) < 0.01f) {
                e->facing_angle += randf_range(&game->rng_state, -0.5f, 0.5f);
            }

            // Peripheral vision triggers suspicion
            if (player_visibility == 1 && player->alive) {
                e->state = STATE_SUSPICIOUS;
                e->alert_x = player->x;
                e->alert_y = player->y;
                e->suspicious_timer = 60;
            }
            // Direct view triggers alert
            else if (player_in_direct_view && player->alive) {
                e->state = STATE_ALERT;
                e->alert_x = player->x;
                e->alert_y = player->y;
                e->alert_timer = 90;
            }

            // Chance to start patrolling
            if (randf(&game->rng_state) < 0.005f) {
                e->state = STATE_PATROL;
            }
            break;

        case STATE_PATROL:
            // Move toward patrol point
            if (e->patrol_x == 0 && e->patrol_y == 0) {
                // Pick a walkable destination
                for (int attempts = 0; attempts < 10; attempts++) {
                    int room_idx = randi_range(&game->rng_state, 0, game->level.room_count - 1);
                    Room* r = &game->level.rooms[room_idx];
                    float px = r->x + 1 + randf(&game->rng_state) * (r->width - 2);
                    float py = r->y + 1 + randf(&game->rng_state) * (r->height - 2);
                    if (is_walkable(&game->level, (int)px, (int)py)) {
                        e->patrol_x = px;
                        e->patrol_y = py;
                        e->stalemate_timer = 0;
                        break;
                    }
                }
            }

            {
                float pdx = e->patrol_x - e->x;
                float pdy = e->patrol_y - e->y;
                float pdist = sse_distance(0, 0, pdx, pdy);

                if (pdist < 2.0f) {
                    e->patrol_x = 0;
                    e->patrol_y = 0;
                    e->stalemate_timer = 0;
                    // Sometimes stop and idle
                    if (randf(&game->rng_state) < 0.3f) {
                        e->state = STATE_IDLE;
                    }
                } else {
                    float next_x, next_y;
                    if (find_path(&game->level, e->x, e->y, e->patrol_x, e->patrol_y, &next_x, &next_y)) {
                        float speed = PLAYER_WALK_SPEED * 0.6f;
                        e->vx = (next_x - e->x) * speed;
                        e->vy = (next_y - e->y) * speed;
                        e->facing_angle = atan2f(e->vy, e->vx);
                        e->stalemate_timer = 0;
                    } else {
                        // Can't path - pick new destination
                        e->stalemate_timer++;
                        if (e->stalemate_timer > 30) {
                            e->patrol_x = 0;
                            e->patrol_y = 0;
                        }
                    }
                }
            }

            // Peripheral vision -> suspicious
            if (player_visibility == 1 && player->alive) {
                e->state = STATE_SUSPICIOUS;
                e->alert_x = player->x;
                e->alert_y = player->y;
                e->suspicious_timer = 90;
                e->vx = 0;
                e->vy = 0;
            }
            // Direct view -> alert
            else if (player_in_direct_view && player->alive) {
                e->state = STATE_ALERT;
                e->alert_x = player->x;
                e->alert_y = player->y;
                e->alert_timer = 120;
            }
            break;

        case STATE_SUSPICIOUS:
            // Heard or glimpsed something - investigate slowly
            e->vx *= 0.5f;
            e->vy *= 0.5f;

            // Turn toward suspicious area
            {
                float target_angle = atan2f(e->alert_y - e->y, e->alert_x - e->x);
                float angle_diff = target_angle - e->facing_angle;
                while (angle_diff > PI) angle_diff -= 2*PI;
                while (angle_diff < -PI) angle_diff += 2*PI;
                e->facing_angle += angle_diff * 0.05f;  // Slow turn
            }

            e->suspicious_timer--;

            // Direct sight = full alert
            if (player_in_direct_view && player->alive) {
                e->state = STATE_ALERT;
                e->alert_x = player->x;
                e->alert_y = player->y;
                e->alert_timer = 120;
                e->frames_target_visible = 0;
            }
            // Timer expired = go back to patrol
            else if (e->suspicious_timer <= 0) {
                e->state = STATE_PATROL;
            }
            // Move slowly toward suspicious location
            else if (e->suspicious_timer < 60) {
                float next_x, next_y;
                if (find_path(&game->level, e->x, e->y, e->alert_x, e->alert_y, &next_x, &next_y)) {
                    float speed = PLAYER_WALK_SPEED * 0.4f;
                    e->vx = (next_x - e->x) * speed;
                    e->vy = (next_y - e->y) * speed;
                }
            }
            break;

        case STATE_ALERT:
            // Saw something clearly - looking for target
            e->facing_angle = atan2f(e->alert_y - e->y, e->alert_x - e->x);
            e->alert_timer--;

            // Confirmed sighting for several frames = combat
            if (player_in_direct_view && e->frames_target_visible > 15) {
                e->state = STATE_COMBAT;
                e->target_id = game->player_id;
            }
            // Still see in peripheral = stay alert
            else if (player_visibility == 1) {
                e->alert_timer = 90;  // Reset timer
                e->alert_x = player->x;
                e->alert_y = player->y;
            }
            // Lost sight = hunt
            else if (e->alert_timer <= 0) {
                e->state = STATE_HUNTING;
                e->alert_timer = 300;  // Hunt for 5 seconds
            }
            break;

        case STATE_HUNTING:
            // Lost sight, searching last known position - BE AGGRESSIVE
            e->stalemate_timer++;
            {
                float hdx = e->last_seen_x - e->x;
                float hdy = e->last_seen_y - e->y;
                float hdist = sse_distance(0, 0, hdx, hdy);

                if (hdist < 3.0f) {
                    // Reached last known position but no target - FLANK toward actual player pos
                    e->stalemate_timer++;

                    if (e->stalemate_timer > 30 && player->alive) {
                        // Push toward player's actual position
                        float next_x, next_y;
                        if (find_path(&game->level, e->x, e->y, player->x, player->y, &next_x, &next_y)) {
                            float speed = (e->type == ENEMY_RUSHER) ? PLAYER_RUN_SPEED : PLAYER_WALK_SPEED;
                            e->vx = (next_x - e->x) * speed;
                            e->vy = (next_y - e->y) * speed;
                            e->facing_angle = atan2f(player->y - e->y, player->x - e->x);
                            e->stalemate_timer = 0;
                        }
                    } else {
                        // Look around briefly
                        e->facing_angle += 0.15f;
                    }
                    e->alert_timer -= 1;
                } else {
                    float next_x, next_y;
                    if (find_path(&game->level, e->x, e->y, e->last_seen_x, e->last_seen_y, &next_x, &next_y)) {
                        float speed = PLAYER_WALK_SPEED * 0.9f;  // Faster hunt
                        e->vx = (next_x - e->x) * speed;
                        e->vy = (next_y - e->y) * speed;
                        e->facing_angle = atan2f(e->vy, e->vx);
                    }
                }
            }

            e->alert_timer--;

            // Found target again
            if (player_in_direct_view && player->alive) {
                e->state = STATE_COMBAT;
                e->target_id = game->player_id;
                e->stalemate_timer = 0;
            }
            // Don't give up - keep hunting aggressively if player is alive
            else if (e->alert_timer <= 0 && player->alive) {
                // Reset and keep hunting toward player's position
                e->alert_timer = 120;
                e->last_seen_x = player->x + randf_range(&game->rng_state, -5.0f, 5.0f);
                e->last_seen_y = player->y + randf_range(&game->rng_state, -5.0f, 5.0f);
            }
            else if (e->alert_timer <= 0) {
                e->state = STATE_PATROL;
                e->patrol_x = 0;
                e->patrol_y = 0;
                e->stalemate_timer = 0;
            }
            break;

        case STATE_COMBAT:
            if (!player->alive) {
                e->state = STATE_PATROL;
                break;
            }

            // Track target
            if (can_see_player) {
                e->facing_angle = atan2f(dy, dx);
                e->last_seen_x = player->x;
                e->last_seen_y = player->y;
            }

            {
                float ideal_range;
                switch (e->type) {
                    case ENEMY_SNIPER: ideal_range = 25.0f; break;
                    case ENEMY_RUSHER: ideal_range = 5.0f; break;
                    case ENEMY_HEAVY: ideal_range = 8.0f; break;
                    default: ideal_range = 12.0f; break;
                }

                if (can_see_player) {
                    // SSE: get normalized direction to player
                    float norm_dx = dx, norm_dy = dy;
                    sse_normalize(&norm_dx, &norm_dy);

                    if (dist_to_player > ideal_range + 2.0f) {
                        float next_x, next_y;
                        if (find_path(&game->level, e->x, e->y, player->x, player->y, &next_x, &next_y)) {
                            float speed = (e->type == ENEMY_RUSHER) ? PLAYER_RUN_SPEED : PLAYER_WALK_SPEED;
                            e->vx = (next_x - e->x) * speed;
                            e->vy = (next_y - e->y) * speed;
                        }
                    } else if (dist_to_player < ideal_range - 2.0f && e->type != ENEMY_RUSHER) {
                        // SSE: back away using normalized direction
                        e->vx = -norm_dx * PLAYER_WALK_SPEED * 0.5f;
                        e->vy = -norm_dy * PLAYER_WALK_SPEED * 0.5f;
                    } else {
                        // Strafe using perpendicular (SSE normalized)
                        e->vx = -norm_dy * PLAYER_WALK_SPEED * 0.3f;
                        e->vy = norm_dx * PLAYER_WALK_SPEED * 0.3f;
                    }

                    // Fire if in range and enough time tracking
                    if (dist_to_player < e->weapon.range && e->frames_target_visible > 10) {
                        if (e->fire_cooldown <= 0 && e->reload_timer <= 0 && e->weapon.mag_current > 0) {
                            spawn_bullet(game, entity_id, e->x, e->y, e->facing_angle, e->weapon.accuracy);
                            e->weapon.mag_current--;
                            e->fire_cooldown = e->weapon.fire_rate;
                        }
                    }
                } else {
                    // Lost sight during combat - hunt
                    e->state = STATE_HUNTING;
                    e->alert_timer = 180;
                }
            }

            // Reload check
            if (e->weapon.mag_current <= 0 && e->weapon.reserve > 0) {
                e->prev_state = STATE_COMBAT;
                e->state = STATE_RELOAD;
                e->reload_timer = e->weapon.reload_time;
            }
            break;

        case STATE_RELOAD:
            e->reload_timer--;
            e->vx *= 0.3f;
            e->vy *= 0.3f;

            if (e->reload_timer <= 0) {
                int to_load = e->weapon.mag_size;
                if (e->weapon.reserve < to_load) to_load = e->weapon.reserve;
                e->weapon.mag_current = to_load;
                e->weapon.reserve -= to_load;
                e->state = (e->prev_state == STATE_COMBAT && can_see_player) ? STATE_COMBAT : STATE_HUNTING;
            }
            break;

        default:
            e->state = STATE_PATROL;
            break;
    }

    e->fire_cooldown--;
}

// Smart player AI - uses stealth, cover, and tactical planning
static void update_player_ai(GameState* game) {
    Entity* player = &game->entities[game->player_id];
    if (!player->alive) return;

    // Scan for enemies and assess threats
    int enemies_aware = 0;       // Enemies that know we're here
    int enemies_visible = 0;     // Enemies we can see
    int enemies_unaware = 0;     // Enemies that don't know we're here
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

        // Can player see this enemy?
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

        // Is this enemy aware of player?
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

    // Check for nearby cover
    player->has_cover_nearby = find_nearby_cover(&game->level, player->x, player->y,
                                                  &player->cover_x, &player->cover_y);

    // Track rapid damage for healing decisions
    if (player->rapid_damage_timer <= 0) {
        // Reset damage tracking window
        player->health_at_damage_start = player->health;
        player->rapid_damage_timer = RAPID_DAMAGE_WINDOW;
    }
    player->rapid_damage_timer--;
    float damage_taken_recently = player->health_at_damage_start - player->health;

    // Check if we need healing (have medpens and not already healing)
    bool need_heal = false;
    if (player->medpens > 0 && player->state != STATE_HEALING) {
        // Emergency heal: rapid damage taken
        if (damage_taken_recently >= RAPID_DAMAGE_THRESHOLD && player->health < 200.0f) {
            need_heal = true;
        }
        // Low health heal: health below 100 and have cover or no enemies visible
        else if (player->health < 100.0f && (player->has_cover_nearby || enemies_visible == 0)) {
            need_heal = true;
        }
        // Critical heal: health below 50, heal even without cover
        else if (player->health < 50.0f) {
            need_heal = true;
        }
    }

    if (need_heal) {
        // Seek cover and heal
        player->state = STATE_HEALING;
        player->healing_timer = 0;  // Will start once in cover (or immediately if no cover)
        player->stalemate_timer = 0;
    }

    // REACT TO TAKING DAMAGE - this overrides normal behavior!
    bool taking_fire = (player->damage_react_timer > 0);
    if (taking_fire) {
        player->damage_react_timer--;

        // Turn toward the threat direction
        float threat_dx = player->last_damage_x - player->x;
        float threat_dy = player->last_damage_y - player->y;
        float threat_dist = sse_distance(0, 0, threat_dx, threat_dy);

        if (threat_dist > 0.1f) {
            // Quickly turn to face where shots are coming from
            float target_angle = atan2f(threat_dy, threat_dx);
            float angle_diff = target_angle - player->facing_angle;
            while (angle_diff > PI) angle_diff -= 2*PI;
            while (angle_diff < -PI) angle_diff += 2*PI;
            player->facing_angle += angle_diff * 0.3f;  // Fast turn

            // Update last seen position to damage source
            player->last_seen_x = player->last_damage_x;
            player->last_seen_y = player->last_damage_y;

            // Find the attacker if we can see them now
            for (int i = 0; i < game->entity_count; i++) {
                Entity* e = &game->entities[i];
                if (e->team == 0 || !e->alive) continue;

                // SSE distance squared for attacker proximity check
                float dist_sq = sse_distance_squared(e->x, e->y, player->last_damage_x, player->last_damage_y);
                if (dist_sq < 25.0f) {  // Within 5 units of damage source
                    // Found likely attacker
                    player->target_id = i;
                    player->state = STATE_COMBAT;
                    enemies_aware++;  // Treat as aware since they're shooting us
                    if (threat_dist < closest_aware_dist) {
                        closest_aware_dist = threat_dist;
                        closest_aware_id = i;
                    }
                    break;
                }
            }

            // If we're being sniped and can't see attacker, seek cover!
            if (player->state != STATE_COMBAT && player->has_cover_nearby) {
                player->state = STATE_HIDING;
                player->alert_timer = 90;  // Stay in cover longer when under fire
            }
        }
    }

    // Decision making based on situation
    float move_x = 0, move_y = 0;
    bool should_shoot = false;
    int target_id = -1;

    // State machine for player behavior
    switch (player->state) {
        case STATE_PATROL:
            // Exploring, looking for enemies
            // Don't run if there are unaware enemies visible (stay quiet)
            player->is_running = (enemies_unaware == 0 && enemies_aware == 0);
            player->is_crouching = (enemies_visible > 0);  // Crouch when enemies spotted

            // IMMEDIATE reaction to taking damage while patrolling
            if (taking_fire) {
                // We're under attack! Take cover immediately or fight
                if (player->has_cover_nearby) {
                    player->state = STATE_HIDING;
                    player->alert_timer = 60;
                } else {
                    player->state = STATE_COMBAT;
                }
                break;  // Exit patrol, handle in next frame
            }

            if (enemies_visible > 0 && enemies_aware == 0) {
                // Spotted unaware enemy - take cover and plan
                if (player->has_cover_nearby) {
                    player->state = STATE_HIDING;
                    player->target_id = closest_unaware_id;
                    player->alert_timer = 60;  // Time to plan
                } else {
                    // No cover - try to get first shot advantage
                    player->state = STATE_COMBAT;
                    player->target_id = closest_unaware_id;
                }
            } else if (enemies_aware > 0) {
                // We've been spotted! Take cover or fight
                if (player->has_cover_nearby && closest_aware_dist > 10.0f) {
                    player->state = STATE_HIDING;
                    player->target_id = closest_aware_id;
                } else {
                    player->state = STATE_COMBAT;
                    player->target_id = closest_aware_id;
                }
            } else {
                // Continue patrol
                if (player->patrol_x == 0 && player->patrol_y == 0) {
                    // Pick a new random destination in a room
                    for (int attempts = 0; attempts < 10; attempts++) {
                        int room_idx = randi_range(&game->rng_state, 0, game->level.room_count - 1);
                        Room* r = &game->level.rooms[room_idx];
                        float px = r->x + 1 + randf(&game->rng_state) * (r->width - 2);
                        float py = r->y + 1 + randf(&game->rng_state) * (r->height - 2);
                        // Make sure it's walkable
                        if (is_walkable(&game->level, (int)px, (int)py)) {
                            player->patrol_x = px;
                            player->patrol_y = py;
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
                    // Look around when reaching destination
                    player->facing_angle += randf_range(&game->rng_state, -0.3f, 0.3f);
                } else {
                    // Try to path to destination
                    float next_x, next_y;
                    if (find_path(&game->level, player->x, player->y, player->patrol_x, player->patrol_y, &next_x, &next_y)) {
                        move_x = next_x - player->x;
                        move_y = next_y - player->y;
                        float move_len = sse_distance(0, 0, move_x, move_y);
                        if (move_len > 0.1f) {
                            move_x /= move_len;
                            move_y /= move_len;
                        }
                        player->facing_angle = atan2f(move_y, move_x);
                        player->stalemate_timer = 0;
                    } else {
                        // Can't path - stuck! Pick new destination
                        player->stalemate_timer++;
                        if (player->stalemate_timer > 30) {
                            player->patrol_x = 0;
                            player->patrol_y = 0;
                        }
                    }
                }
            }

            // Try to pick up weapons from corpses while patrolling
            try_pickup_weapon(game, player);
            break;

        case STATE_HIDING:
            // Behind cover, planning attack or waiting for opportunity
            player->alert_timer--;
            player->stalemate_timer++;
            player->is_crouching = true;

            // Move toward cover if not there yet
            if (player->has_cover_nearby) {
                float cdx = player->cover_x - player->x;
                float cdy = player->cover_y - player->y;
                float cdist = sse_distance(0, 0, cdx, cdy);

                if (cdist > 1.0f) {
                    // SSE normalization
                    sse_normalize(&cdx, &cdy);
                    move_x = cdx;
                    move_y = cdy;
                }
            }

            // Peek to track enemy
            if (player->target_id >= 0 && player->target_id < game->entity_count) {
                Entity* target = &game->entities[player->target_id];
                if (target->alive) {
                    float tdx = target->x - player->x;
                    float tdy = target->y - player->y;

                    // SSE: get normalized direction to target
                    float norm_tdx = tdx, norm_tdy = tdy;
                    sse_normalize(&norm_tdx, &norm_tdy);

                    // Slowly turn toward target (peeking)
                    float target_angle = atan2f(tdy, tdx);
                    float angle_diff = target_angle - player->facing_angle;
                    while (angle_diff > PI) angle_diff -= 2*PI;
                    while (angle_diff < -PI) angle_diff += 2*PI;
                    player->facing_angle += angle_diff * 0.1f;

                    // Check if we can see target now
                    int vis = check_view_cone(&game->level, player->x, player->y, player->facing_angle,
                                              target->x, target->y, player->view_cone_angle, player->view_distance, true);

                    // Attack opportunity: target not looking at us
                    bool target_facing_away = false;
                    {
                        float to_player_x = player->x - target->x;
                        float to_player_y = player->y - target->y;
                        // SSE normalization
                        sse_normalize(&to_player_x, &to_player_y);
                        if (to_player_x != 0 || to_player_y != 0) {
                            // SSE dot product with facing direction
                            float facing_x = cosf(target->facing_angle);
                            float facing_y = sinf(target->facing_angle);
                            float dot = sse_dot2(facing_x, facing_y, to_player_x, to_player_y);
                            target_facing_away = (dot < 0.3f);  // Not looking at us
                        }
                    }

                    // Attack if: timer done, can see target, and good opportunity
                    if (player->alert_timer <= 0 && vis > 0 && (target_facing_away || target->state < STATE_ALERT)) {
                        player->state = STATE_COMBAT;
                        player->stalemate_timer = 0;
                        player->is_crouching = false;
                    }

                    // STALEMATE BREAKER: If hiding too long without seeing target, flank!
                    if (player->stalemate_timer > 90) {  // ~3 seconds
                        // Can't see them, they can't see us - time to flank
                        // Move perpendicular to target direction (using SSE normalized)
                        float perp_x = -norm_tdy;
                        float perp_y = norm_tdx;
                        // Pick direction randomly
                        if (randf(&game->rng_state) > 0.5f) {
                            perp_x = -perp_x;
                            perp_y = -perp_y;
                        }
                        // Set flank destination using normalized direction
                        player->patrol_x = player->x + perp_x * 8.0f + norm_tdx * 4.0f;
                        player->patrol_y = player->y + perp_y * 8.0f + norm_tdy * 4.0f;
                        player->state = STATE_COMBAT;  // Aggressive push
                        player->stalemate_timer = 0;
                        player->is_crouching = false;
                        player->is_running = true;  // Rush the flank
                    }
                } else {
                    // Target dead, go back to patrol
                    player->state = STATE_PATROL;
                    player->stalemate_timer = 0;
                    player->is_crouching = false;
                }
            } else {
                player->state = STATE_PATROL;
                player->stalemate_timer = 0;
                player->is_crouching = false;
            }

            // Abort hiding if taking fire - MUST react to damage!
            if (taking_fire) {
                // We're being hit! Either fight or run
                if (player->target_id >= 0) {
                    // We know who's attacking, fight back
                    player->state = STATE_COMBAT;
                    player->stalemate_timer = 0;
                    player->is_crouching = false;
                } else {
                    // Don't know where it's coming from, run to new cover
                    player->patrol_x = 0;
                    player->patrol_y = 0;
                    // Find cover away from damage direction
                    float flee_x = player->x - (player->last_damage_x - player->x);
                    float flee_y = player->y - (player->last_damage_y - player->y);
                    player->patrol_x = flee_x;
                    player->patrol_y = flee_y;
                    player->state = STATE_PATROL;
                    player->stalemate_timer = 0;
                    player->is_running = true;  // Run away!
                    player->is_crouching = false;
                }
            } else if (enemies_aware > 2 || (enemies_aware > 0 && closest_aware_dist < 8.0f)) {
                player->state = STATE_COMBAT;
                player->stalemate_timer = 0;
                player->is_crouching = false;
            }
            break;

        case STATE_COMBAT:
            // Active engagement
            player->is_crouching = false;

            // Find best target
            if (player->target_id >= 0 && player->target_id < game->entity_count) {
                Entity* target = &game->entities[player->target_id];
                if (!target->alive) {
                    player->target_id = -1;
                }
            }

            // Retarget if needed
            if (player->target_id < 0) {
                if (closest_aware_id >= 0) {
                    player->target_id = closest_aware_id;
                } else if (closest_unaware_id >= 0) {
                    player->target_id = closest_unaware_id;
                } else {
                    // No targets, go back to patrol
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

                // SSE: get normalized direction to target
                float norm_tdx = tdx, norm_tdy = tdy;
                sse_normalize(&norm_tdx, &norm_tdy);

                // Face target
                player->facing_angle = atan2f(tdy, tdx);

                // Check visibility
                int vis = check_view_cone(&game->level, player->x, player->y, player->facing_angle,
                                          target->x, target->y, player->view_cone_angle, player->view_distance, true);

                float ideal_range = player->weapon.range * 0.5f;

                if (vis > 0) {
                    player->frames_target_visible++;

                    // Check if we have cover from this target
                    bool in_cover = has_cover_from_direction(&game->level, player->x, player->y, target->x, target->y);

                    // Movement: maintain ideal range with strafing (using SSE normalized)
                    if (tdist > ideal_range + 5.0f) {
                        move_x = norm_tdx;
                        move_y = norm_tdy;
                    } else if (tdist < ideal_range - 3.0f && !in_cover) {
                        // Back up if too close and not in cover
                        move_x = -norm_tdx;
                        move_y = -norm_tdy;
                    } else if (in_cover) {
                        // If in cover, stay put and shoot
                        move_x = 0;
                        move_y = 0;
                        player->is_crouching = true;  // Crouch behind cover
                    } else {
                        // Strafe (perpendicular, SSE normalized)
                        float strafe_dir = (game->frame % 90 < 45) ? 1.0f : -1.0f;
                        move_x = -norm_tdy * strafe_dir * 0.7f;
                        move_y = norm_tdx * strafe_dir * 0.7f;
                    }

                    // Shoot if we have clear shot
                    if (tdist < player->weapon.range && player->frames_target_visible > 5) {
                        should_shoot = true;
                    }
                } else {
                    // Lost sight - move toward last known position AGGRESSIVELY
                    player->frames_target_visible = 0;
                    player->stalemate_timer++;

                    float ldx = player->last_seen_x - player->x;
                    float ldy = player->last_seen_y - player->y;
                    float ldist = sse_distance(0, 0, ldx, ldy);

                    // SSE: normalized direction to last seen
                    float norm_ldx = ldx, norm_ldy = ldy;
                    sse_normalize(&norm_ldx, &norm_ldy);

                    // Push toward target's actual position (already have norm_tdx/tdy)
                    float push_x = norm_tdx;
                    float push_y = norm_tdy;

                    if (ldist > 2.0f) {
                        // Mix between last seen and actual position for aggressive pursuit
                        move_x = sse_fmadd(norm_ldx, 0.5f, push_x * 0.5f);
                        move_y = sse_fmadd(norm_ldy, 0.5f, push_y * 0.5f);
                        player->is_running = true;  // Rush them!
                    } else if (player->stalemate_timer > 45) {
                        // Reached last known pos but can't see them - FLANK!
                        float perp_x = -norm_tdy;
                        float perp_y = norm_tdx;
                        if (game->frame % 60 < 30) {
                            perp_x = -perp_x;
                            perp_y = -perp_y;
                        }
                        // Move perpendicular + forward to flank
                        move_x = perp_x * 0.7f + push_x * 0.5f;
                        move_y = perp_y * 0.7f + push_y * 0.5f;
                        player->is_running = true;
                        player->stalemate_timer = 0;  // Reset after attempting flank
                    } else {
                        // Keep pushing toward target
                        move_x = push_x;
                        move_y = push_y;
                    }
                }
            }

            // Reset stalemate when we can see target
            if (enemies_visible > 0) {
                player->stalemate_timer = 0;
            }

            // Low health? Try to take cover
            if (player->health < 80.0f && player->has_cover_nearby && enemies_aware > 0) {
                player->state = STATE_HIDING;
                player->alert_timer = 30;  // Quick peek
            }

            // Need to reload? Take cover first if possible
            if (player->weapon.mag_current <= 2 && player->weapon.reserve > 0) {
                if (player->has_cover_nearby) {
                    player->state = STATE_HIDING;
                    player->alert_timer = player->weapon.reload_time + 20;
                }
                player->reload_timer = player->weapon.reload_time;
            }
            break;

        case STATE_RELOAD:
            // Reloading - try to stay still or in cover
            player->reload_timer--;
            move_x = 0;
            move_y = 0;

            if (player->reload_timer <= 0) {
                int to_load = player->weapon.mag_size;
                if (player->weapon.reserve < to_load) to_load = player->weapon.reserve;
                player->weapon.mag_current = to_load;
                player->weapon.reserve -= to_load;
                player->state = (enemies_aware > 0) ? STATE_COMBAT : STATE_PATROL;
            }
            break;

        case STATE_HEALING:
            // Using medpen - need to get to cover first, then apply
            player->is_crouching = true;
            player->is_running = false;

            // Move toward cover if not there
            if (player->has_cover_nearby) {
                float cdx = player->cover_x - player->x;
                float cdy = player->cover_y - player->y;
                float cdist = sse_distance(0, 0, cdx, cdy);

                if (cdist > 1.5f) {
                    // Rush to cover - SSE normalization
                    sse_normalize(&cdx, &cdy);
                    move_x = cdx;
                    move_y = cdy;
                    player->is_running = true;
                    player->is_crouching = false;
                } else {
                    // In cover - start/continue healing
                    move_x = 0;
                    move_y = 0;

                    if (player->healing_timer == 0) {
                        // Start using medpen
                        player->healing_timer = MEDPEN_USE_TIME;
                    }

                    player->healing_timer--;

                    if (player->healing_timer <= 0) {
                        // Healing complete!
                        player->medpens--;
                        player->health += MEDPEN_HEAL_AMOUNT;
                        if (player->health > 250.0f) player->health = 250.0f;  // Cap at max

                        // Reset damage tracking
                        player->health_at_damage_start = player->health;
                        player->rapid_damage_timer = RAPID_DAMAGE_WINDOW;

                        // Return to combat or patrol
                        player->state = (enemies_aware > 0) ? STATE_COMBAT : STATE_PATROL;
                        player->stalemate_timer = 0;
                    }
                }
            } else {
                // No cover available - heal in place (risky!)
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

            // Abort healing if taking more damage (can't heal under fire)
            if (taking_fire && player->healing_timer > 0) {
                player->healing_timer = 0;  // Interrupted!
                player->state = STATE_COMBAT;
            }
            break;

        default:
            player->state = STATE_PATROL;
            break;
    }

    // Execute shooting
    if (should_shoot && player->fire_cooldown <= 0 && player->reload_timer <= 0 && player->weapon.mag_current > 0) {
        spawn_bullet(game, game->player_id, player->x, player->y, player->facing_angle, player->weapon.accuracy);
        player->weapon.mag_current--;
        player->fire_cooldown = player->weapon.fire_rate;
    }

    // Apply movement
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

    // Pathfind
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

    // Handle reload completion
    if (player->reload_timer > 0 && player->state != STATE_RELOAD) {
        player->reload_timer--;
        if (player->reload_timer <= 0) {
            int to_load = player->weapon.mag_size;
            if (player->weapon.reserve < to_load) to_load = player->weapon.reserve;
            player->weapon.mag_current = to_load;
            player->weapon.reserve -= to_load;
        }
    }

    player->fire_cooldown--;
}

// ============================================================================
// PHYSICS UPDATE
// ============================================================================

static void update_physics(GameState* game) {
    // Update entity positions with collision
    for (int i = 0; i < game->entity_count; i++) {
        Entity* e = &game->entities[i];
        if (!e->alive) continue;

        // First, check if entity is currently stuck in a wall and push them out
        if (!is_walkable(&game->level, (int)e->x, (int)e->y)) {
            // Find nearest walkable tile
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

        // Try full movement first
        if (is_walkable(&game->level, (int)new_x, (int)new_y)) {
            e->x = new_x;
            e->y = new_y;
        } else {
            // Try X only
            if (is_walkable(&game->level, (int)new_x, (int)e->y)) {
                e->x = new_x;
            }
            // Try Y only
            if (is_walkable(&game->level, (int)e->x, (int)new_y)) {
                e->y = new_y;
            }
        }

        // Friction using SSE
        {
            __m128 vel = _mm_set_ps(0, 0, e->vy, e->vx);
            __m128 friction = _mm_set1_ps(0.9f);
            vel = _mm_mul_ps(vel, friction);
            ALIGN32 float vel_out[4];
            _mm_store_ps(vel_out, vel);
            e->vx = vel_out[0];
            e->vy = vel_out[1];
        }

        // Generate footstep sounds based on movement speed
        float move_speed = sse_distance(0, 0, e->vx, e->vy);
        if (move_speed > 0.05f) {
            float base_sound = e->is_running ? SOUND_RUN_BASE : SOUND_WALK_BASE;
            // Crouching reduces sound
            if (e->is_crouching) base_sound *= 0.3f;
            // Calculate sound radius with corridor amplification
            float sound_radius = calculate_sound_radius(&game->level, e->x, e->y, base_sound);
            // Only propagate sound periodically (every ~10 frames based on speed)
            e->steps_since_sound++;
            if (e->steps_since_sound > (int)(5.0f / (move_speed + 0.1f))) {
                propagate_sound(game, e->x, e->y, sound_radius);
                e->steps_since_sound = 0;
            }
        }
    }

    // Update bullets using AVX (batches of 8)
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

    // Bullet collision checks
    for (int bi = game->bullet_count - 1; bi >= 0; bi--) {
        Bullet* b = &game->bullets[bi];
        if (!b->active) continue;

        // Wall collision
        int tx = (int)b->x;
        int ty = (int)b->y;
        if (tx < 0 || tx >= LEVEL_WIDTH || ty < 0 || ty >= LEVEL_HEIGHT ||
            game->level.tiles[tx + ty * LEVEL_WIDTH] == TILE_WALL) {
            b->active = false;
            continue;
        }

        // Entity collision using SSE for distance
        for (int ei = 0; ei < game->entity_count; ei++) {
            Entity* e = &game->entities[ei];
            if (!e->alive || e->team == b->team || ei == b->owner_id) continue;

            // SSE distance squared calculation
            __m128 bpos = _mm_set_ps(0, 0, b->y, b->x);
            __m128 epos = _mm_set_ps(0, 0, e->y, e->x);
            __m128 diff = _mm_sub_ps(bpos, epos);
            __m128 sq = _mm_mul_ps(diff, diff);
            __m128 dist2_vec = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, 1));
            float dist2 = _mm_cvtss_f32(dist2_vec);

            if (dist2 < 1.0f) {  // Hit radius 1.0
                e->health -= b->damage;
                b->active = false;

                // Record where the damage came from (bullet trajectory)
                Entity* shooter = (b->owner_id >= 0 && b->owner_id < game->entity_count)
                                  ? &game->entities[b->owner_id] : NULL;
                if (shooter) {
                    e->last_damage_x = shooter->x;
                    e->last_damage_y = shooter->y;
                } else {
                    // Estimate from bullet velocity
                    e->last_damage_x = e->x - b->vx * 10.0f;
                    e->last_damage_y = e->y - b->vy * 10.0f;
                }
                e->damage_react_timer = 60;  // React for 2 seconds

                if (e->health <= 0) {
                    e->alive = false;
                    e->state = STATE_DEAD;
                }
                break;
            }
        }
    }

    // Compact bullet array (remove inactive)
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

// Sound propagation - alert nearby enemies when shots are fired
static void propagate_sound(GameState* game, float x, float y, float radius) {
    // Process in batches of 8
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

// ============================================================================
// RENDERING
// ============================================================================

static void render_game(GameState* game, Viewport* vp) {
    Entity* player = &game->entities[game->player_id];

    // Calculate visible game area (excluding UI: 3 lines header, 1 line footer)
    int game_area_height = vp->height - 4;
    int game_area_width = vp->width;

    // Update camera to follow player (center player in game area)
    float target_cam_x = player->x - game_area_width / 2.0f;
    float target_cam_y = player->y - game_area_height / 2.0f;

    // Smooth camera follow using SSE lerp
    game->camera_x = sse_lerp(game->camera_x, target_cam_x, 0.1f);
    game->camera_y = sse_lerp(game->camera_y, target_cam_y, 0.1f);

    // Clamp camera to level bounds using SSE clamp
    game->camera_x = sse_clamp(game->camera_x, 0, (float)(LEVEL_WIDTH - game_area_width));
    game->camera_y = sse_clamp(game->camera_y, 0, (float)(LEVEL_HEIGHT - game_area_height));

    int cam_x = (int)game->camera_x;
    int cam_y = (int)game->camera_y;

    clear_buffer(vp);

    // Render level tiles
    for (int y = 0; y < vp->height - 4; y++) {
        for (int x = 0; x < vp->width; x++) {
            int lx = cam_x + x;
            int ly = cam_y + y;

            if (lx >= 0 && lx < LEVEL_WIDTH && ly >= 0 && ly < LEVEL_HEIGHT) {
                int idx = lx + ly * LEVEL_WIDTH;
                switch (game->level.tiles[idx]) {
                    case TILE_WALL:
                        draw_pixel_char(vp, x, y + 3, '#');
                        break;
                    case TILE_COVER:
                        draw_pixel_char(vp, x, y + 3, '=');
                        break;
                    case TILE_FLOOR:
                        draw_pixel_char(vp, x, y + 3, '.');
                        break;
                }
            }
        }
    }

    // Render bullets
    for (int i = 0; i < game->bullet_count; i++) {
        Bullet* b = &game->bullets[i];
        if (!b->active) continue;

        int sx = (int)(b->x - cam_x);
        int sy = (int)(b->y - cam_y) + 3;

        if (sx >= 0 && sx < vp->width && sy >= 3 && sy < vp->height - 1) {
            draw_pixel_char(vp, sx, sy, b->team == 0 ? '*' : 'o');
        }
    }

    // Render entities
    for (int i = 0; i < game->entity_count; i++) {
        Entity* e = &game->entities[i];

        int sx = (int)(e->x - cam_x);
        int sy = (int)(e->y - cam_y) + 3;

        if (sx < 0 || sx >= vp->width || sy < 3 || sy >= vp->height - 1) continue;

        if (!e->alive) {
            draw_pixel_char(vp, sx, sy, 'x');
            continue;
        }

        char entity_char;
        if (e->team == 0) {
            entity_char = '@';  // Player
        } else {
            switch (e->type) {
                case ENEMY_GRUNT:   entity_char = 'g'; break;
                case ENEMY_SNIPER:  entity_char = 's'; break;
                case ENEMY_RUSHER:  entity_char = 'r'; break;
                case ENEMY_HEAVY:   entity_char = 'H'; break;
                default:            entity_char = '?'; break;
            }
            // Capitalize if alert/attacking
            if (e->state >= STATE_ALERT) {
                entity_char = entity_char - 32;  // To uppercase
            }
        }

        draw_pixel_char(vp, sx, sy, entity_char);

        // Draw facing direction indicator
        int fx = sx + (int)(cosf(e->facing_angle) * 1.5f);
        int fy = sy + (int)(sinf(e->facing_angle) * 1.5f);
        if (fx >= 0 && fx < vp->width && fy >= 3 && fy < vp->height - 1) {
            if (e->team == 0) {
                draw_pixel_char(vp, fx, fy, '+');
            }
        }
    }

    // UI - Top bar
    draw_box(vp, 0, 0, vp->width - 1, 2, "single");

    char status[256];
    int alive_enemies = 0;
    for (int i = 0; i < game->entity_count; i++) {
        if (game->entities[i].team != 0 && game->entities[i].alive) alive_enemies++;
    }

    snprintf(status, sizeof(status),
        " HP:%3.0f  STA:%3.0f  MAG:%2d/%2d  RES:%3d  MED:%d  Enemies:%2d ",
        player->health, player->stamina,
        player->weapon.mag_current, player->weapon.mag_size,
        player->weapon.reserve, player->medpens, alive_enemies);
    draw_string(vp, 1, 1, status);

    // Bottom status
    char state_str[32];
    switch (player->state) {
        case STATE_RELOAD: strcpy(state_str, "RELOADING"); break;
        case STATE_HEALING: strcpy(state_str, "HEALING"); break;
        default:
            if (player->is_running) strcpy(state_str, "RUNNING");
            else strcpy(state_str, "COMBAT");
            break;
    }

    char bottom[128];
    snprintf(bottom, sizeof(bottom), " [%s] g=grunt s=sniper r=rusher H=heavy  CAPS=alert ", state_str);
    draw_string(vp, 1, vp->height - 1, bottom);
}

// ============================================================================
// MAIN SIMULATION
// ============================================================================

void run_bullet_sim(int max_frames) {
    // Switch to alternate screen buffer (like vim/htop do)
    printf("\033[?1049h\033[H");
    fflush(stdout);

    Viewport* vp = create_viewport();

    GameState game;
    memset(&game, 0, sizeof(game));
    game.rng_state = (uint32_t)time(NULL);

    // Generate level
    generate_level(&game.level, &game.rng_state);

    // Spawn player in first room
    game.player_id = spawn_entity(&game, 0, game.level.spawn_x, game.level.spawn_y, 0);

    // Spawn enemies in other rooms
    for (int r = 1; r < game.level.room_count; r++) {
        Room* room = &game.level.rooms[r];

        // Spawn 1-3 enemies per room
        int enemy_count = randi_range(&game.rng_state, 1, 3);

        for (int e = 0; e < enemy_count; e++) {
            float ex = room->x + 1 + randf(&game.rng_state) * (room->width - 2);
            float ey = room->y + 1 + randf(&game.rng_state) * (room->height - 2);

            int enemy_type = randi_range(&game.rng_state, 0, 3);
            spawn_entity(&game, enemy_type, ex, ey, 1);
        }
    }

    // Main game loop - runs until player dies, all enemies dead, or max frames reached
    int prev_bullet_count = 0;

    for (game.frame = 0; max_frames < 0 || game.frame < max_frames; game.frame++) {
        // Update AI
        update_player_ai(&game);

        for (int i = 0; i < game.entity_count; i++) {
            if (game.entities[i].team != 0) {
                update_enemy_ai(&game, i);
            }
        }

        // Check for new shots (sound propagation)
        if (game.bullet_count > prev_bullet_count) {
            for (int i = prev_bullet_count; i < game.bullet_count; i++) {
                Bullet* b = &game.bullets[i];
                propagate_sound(&game, b->x, b->y, SOUND_RADIUS_SHOT);
            }
        }
        prev_bullet_count = game.bullet_count;

        // Update physics
        update_physics(&game);

        // Render
        render_game(&game, vp);
        render_buffer(vp);

        // Check end conditions
        Entity* player = &game.entities[game.player_id];
        if (!player->alive) {
            draw_string(vp, vp->width/2 - 6, vp->height/2, "PLAYER DIED!");
            render_buffer(vp);
            sleep_ms(2000);
            break;
        }

        int enemies_alive = 0;
        for (int i = 0; i < game.entity_count; i++) {
            if (game.entities[i].team != 0 && game.entities[i].alive) enemies_alive++;
        }

        if (enemies_alive == 0) {
            draw_string(vp, vp->width/2 - 8, vp->height/2, "ALL ENEMIES DOWN!");
            render_buffer(vp);
            sleep_ms(2000);
            break;
        }

        sleep_ms(1000 / FPS);
    }

    free_viewport(vp);

    // Switch back to normal screen buffer
    printf("\033[?1049l");
    fflush(stdout);
}

// Original raytrace function kept for compatibility
int trace_bullet_avx(const Ray* ray, const Sphere* spheres, int count, float* out_t) {
    int hit_mask_all = 0;

    __m256 ray_ox = _mm256_set1_ps(ray->origin.x);
    __m256 ray_oy = _mm256_set1_ps(ray->origin.y);
    __m256 ray_oz = _mm256_set1_ps(ray->origin.z);

    __m256 ray_dx = _mm256_set1_ps(ray->dir.x);
    __m256 ray_dy = _mm256_set1_ps(ray->dir.y);
    __m256 ray_dz = _mm256_set1_ps(ray->dir.z);

    for (int i = 0; i < count; i += 8) {
        float sx[8], sy[8], sz[8], sr[8];
        for(int k=0; k<8; ++k) {
            if (i+k < count) {
                sx[k] = spheres[i+k].x;
                sy[k] = spheres[i+k].y;
                sz[k] = spheres[i+k].z;
                sr[k] = spheres[i+k].r;
            } else {
                sx[k] = 0; sy[k] = 0; sz[k] = 0; sr[k] = -1.0f;
            }
        }

        __m256 sph_x = _mm256_loadu_ps(sx);
        __m256 sph_y = _mm256_loadu_ps(sy);
        __m256 sph_z = _mm256_loadu_ps(sz);
        __m256 sph_r = _mm256_loadu_ps(sr);

        __m256 lx = _mm256_sub_ps(sph_x, ray_ox);
        __m256 ly = _mm256_sub_ps(sph_y, ray_oy);
        __m256 lz = _mm256_sub_ps(sph_z, ray_oz);

        __m256 t_ca = _mm256_mul_ps(lx, ray_dx);
        t_ca = fmadd_ps(ly, ray_dy, t_ca);
        t_ca = fmadd_ps(lz, ray_dz, t_ca);

        __m256 l_dot_l = _mm256_mul_ps(lx, lx);
        l_dot_l = fmadd_ps(ly, ly, l_dot_l);
        l_dot_l = fmadd_ps(lz, lz, l_dot_l);

        __m256 d2 = fmsub_ps(t_ca, t_ca, l_dot_l);
        d2 = _mm256_sub_ps(_mm256_setzero_ps(), d2);
        __m256 r2 = _mm256_mul_ps(sph_r, sph_r);

        __m256 mask_miss = _mm256_cmp_ps(d2, r2, _CMP_GT_OQ);

        __m256 t_hc = sqrt_nr_ps(_mm256_sub_ps(r2, d2));

        __m256 t0 = _mm256_sub_ps(t_ca, t_hc);

        __m256 zero = _mm256_setzero_ps();
        __m256 mask_behind = _mm256_cmp_ps(t0, zero, _CMP_LT_OQ);

        __m256 valid = _mm256_andnot_ps(mask_miss, _mm256_andnot_ps(mask_behind, _mm256_set1_ps(1.0f)));

        int mask = _mm256_movemask_ps(valid);

        if (mask) {
            _mm256_storeu_ps(&out_t[i], t0);
            hit_mask_all |= (mask << i);
        }
    }
    return hit_mask_all;
}
