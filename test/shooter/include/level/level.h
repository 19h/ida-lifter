/*
 * Level System
 * Level generation, tile queries, A* pathfinding, and influence maps.
 */

#ifndef SHOOTER_LEVEL_H
#define SHOOTER_LEVEL_H

#include <stdbool.h>
#include <stdint.h>
#include "../types.h"

/* ==========================================================================
 * TILE QUERIES
 * ========================================================================== */

/* Check if a position is walkable (not a wall) */
bool is_walkable(const Level* level, int x, int y);

/* Check if a position is cover */
bool is_cover(const Level* level, int x, int y);

/* ==========================================================================
 * LEVEL GENERATION
 * ========================================================================== */

/* Generate a random level with rooms and corridors */
void generate_level(Level* level, uint32_t* rng);

/* ==========================================================================
 * VISION AND SOUND
 * ========================================================================== */

/*
 * Check if target is within view cone of observer
 * Returns: 0 = not visible, 1 = peripheral vision, 2 = direct vision
 */
int check_view_cone(
    const Level* level,
    float obs_x, float obs_y, float obs_angle,
    float tgt_x, float tgt_y,
    float cone_angle, float view_dist,
    bool check_los
);

/* Calculate corridor width at a position (for sound propagation) */
float get_corridor_width(const Level* level, float x, float y);

/* Calculate sound radius based on action and location */
float calculate_sound_radius(const Level* level, float x, float y, float base_radius);

/* ==========================================================================
 * COVER SYSTEM
 * ========================================================================== */

/* Find nearest cover position. Returns true if found. */
bool find_nearby_cover(const Level* level, float x, float y, float* cover_x, float* cover_y);

/* Check if position has cover from a threat direction */
bool has_cover_from_direction(const Level* level, float x, float y, float threat_x, float threat_y);

/* Find cover that provides protection from a specific threat position */
bool find_cover_from_threat(const Level* level, float x, float y, float threat_x, float threat_y,
                            float* cover_x, float* cover_y);

/* ==========================================================================
 * PATHFINDING (A* with tactical cost modifiers)
 * ========================================================================== */

/*
 * A* pathfinding with tactical cost modifiers from influence map.
 * Prefers paths through cover and avoids threatened areas.
 * Returns true if path exists, outputs next step position.
 */
bool find_path(
    const Level* level,
    float start_x, float start_y,
    float end_x, float end_y,
    float* out_next_x, float* out_next_y
);

/*
 * Tactical pathfinding that considers a specific threat position.
 * Uses influence map for threat avoidance.
 */
bool find_tactical_path(
    const Level* level,
    float start_x, float start_y,
    float end_x, float end_y,
    float threat_x, float threat_y,
    float* out_next_x, float* out_next_y
);

/* ==========================================================================
 * INFLUENCE MAPS
 * ========================================================================== */

/*
 * Update the influence map based on entity positions and facing directions.
 * Called periodically (every INFLUENCE_UPDATE_INTERVAL frames).
 */
void update_influence_map(Level* level, const GameState* game);

/* Query threat level at a position */
float get_threat_at(const Level* level, float x, float y);

/* Query cover value at a position */
float get_cover_value_at(const Level* level, float x, float y);

#endif /* SHOOTER_LEVEL_H */
