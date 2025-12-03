/*
 * Level System Implementation
 * Level generation, tile queries, A* pathfinding, and influence maps.
 */

#include "level/level.h"
#include "config.h"
#include "core/rng.h"
#include "math/sse_math.h"
#include <string.h>
#include <math.h>

/* ==========================================================================
 * TILE QUERIES
 * ========================================================================== */

bool is_walkable(const Level* level, int x, int y) {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) return false;
    uint8_t tile = level->tiles[x + y * LEVEL_WIDTH];
    /* Impassable tiles */
    switch (tile) {
        case TILE_WALL:
        case TILE_WALL_H:
        case TILE_WALL_V:
        case TILE_CORNER_TL:
        case TILE_CORNER_TR:
        case TILE_CORNER_BL:
        case TILE_CORNER_BR:
        case TILE_WATER:
        case TILE_PILLAR:
            return false;
        default:
            return true;
    }
}

bool is_cover(const Level* level, int x, int y) {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) return false;
    uint8_t tile = level->tiles[x + y * LEVEL_WIDTH];
    /* Cover tiles */
    switch (tile) {
        case TILE_COVER:
        case TILE_TERMINAL:
        case TILE_CRATE:
        case TILE_BARREL:
            return true;
        default:
            return false;
    }
}

/* ==========================================================================
 * LEVEL GENERATION
 * ========================================================================== */

/* Room theme types for variety */
typedef enum {
    THEME_OFFICE,       /* Terminals, desks */
    THEME_WAREHOUSE,    /* Crates, barrels */
    THEME_LOBBY,        /* Carpet, pillars */
    THEME_MAINTENANCE,  /* Grates, vents */
    THEME_OUTDOOR,      /* Grass, concrete */
    THEME_COUNT
} RoomTheme;

/* Decorate a room based on its theme */
static void decorate_room(Level* level, Room* room, RoomTheme theme, uint32_t* rng) {
    int x = room->x, y = room->y, w = room->width, h = room->height;

    /* Base floor type based on theme */
    uint8_t floor_type = TILE_FLOOR;
    switch (theme) {
        case THEME_OFFICE:
            floor_type = (randf(rng) < 0.6f) ? TILE_CARPET : TILE_FLOOR;
            break;
        case THEME_WAREHOUSE:
            floor_type = TILE_CONCRETE;
            break;
        case THEME_LOBBY:
            floor_type = TILE_CARPET;
            break;
        case THEME_MAINTENANCE:
            floor_type = TILE_GRATE;
            break;
        case THEME_OUTDOOR:
            floor_type = TILE_GRASS;
            break;
        default:
            break;
    }

    /* Fill room with themed floor - keep it clean */
    for (int ry = y; ry < y + h; ry++) {
        for (int rx = x; rx < x + w; rx++) {
            level->tiles[rx + ry * LEVEL_WIDTH] = floor_type;
        }
    }

    /* Add sparse themed decorations - keep it minimal */
    switch (theme) {
        case THEME_OFFICE:
            /* Maybe one terminal */
            if (w > 8 && randf(rng) < 0.3f) {
                int tx = randi_range(rng, x + 2, x + w - 3);
                int ty = y + 1;
                level->tiles[tx + ty * LEVEL_WIDTH] = TILE_TERMINAL;
            }
            break;

        case THEME_WAREHOUSE:
            /* A few crates for cover */
            if (w > 6 && h > 5) {
                int items = randi_range(rng, 1, 2);
                for (int i = 0; i < items; i++) {
                    int ix = randi_range(rng, x + 2, x + w - 3);
                    int iy = randi_range(rng, y + 2, y + h - 3);
                    level->tiles[ix + iy * LEVEL_WIDTH] = TILE_CRATE;
                }
            }
            break;

        case THEME_LOBBY:
            /* Maybe pillars in large rooms */
            if (w > 10 && h > 8 && randf(rng) < 0.3f) {
                level->tiles[(x + 2) + (y + 2) * LEVEL_WIDTH] = TILE_PILLAR;
                level->tiles[(x + w - 3) + (y + h - 3) * LEVEL_WIDTH] = TILE_PILLAR;
            }
            break;

        case THEME_MAINTENANCE:
            /* One barrel maybe */
            if (randf(rng) < 0.3f && w > 5 && h > 5) {
                int bx = randi_range(rng, x + 2, x + w - 3);
                int by = randi_range(rng, y + 2, y + h - 3);
                level->tiles[bx + by * LEVEL_WIDTH] = TILE_BARREL;
            }
            break;

        case THEME_OUTDOOR:
            /* Small water feature in big rooms */
            if (w > 14 && h > 10 && randf(rng) < 0.25f) {
                int wx = x + w / 2 - 1;
                int wy = y + h / 2 - 1;
                for (int py = wy; py < wy + 2; py++) {
                    for (int px = wx; px < wx + 2; px++) {
                        level->tiles[px + py * LEVEL_WIDTH] = TILE_WATER;
                    }
                }
            }
            break;

        default:
            break;
    }
}

/* Add proper wall borders around walkable areas */
static void add_wall_decorations(Level* level) {
    /* Create a copy to check neighbors without modifying during iteration */
    uint8_t* orig = malloc(LEVEL_SIZE);
    memcpy(orig, level->tiles, LEVEL_SIZE);

    for (int y = 1; y < LEVEL_HEIGHT - 1; y++) {
        for (int x = 1; x < LEVEL_WIDTH - 1; x++) {
            int idx = x + y * LEVEL_WIDTH;
            if (orig[idx] != TILE_WALL) continue;

            /* Check adjacent tiles */
            bool floor_n = (orig[x + (y-1) * LEVEL_WIDTH] != TILE_WALL && orig[x + (y-1) * LEVEL_WIDTH] != TILE_WATER);
            bool floor_s = (orig[x + (y+1) * LEVEL_WIDTH] != TILE_WALL && orig[x + (y+1) * LEVEL_WIDTH] != TILE_WATER);
            bool floor_e = (orig[(x+1) + y * LEVEL_WIDTH] != TILE_WALL && orig[(x+1) + y * LEVEL_WIDTH] != TILE_WATER);
            bool floor_w = (orig[(x-1) + y * LEVEL_WIDTH] != TILE_WALL && orig[(x-1) + y * LEVEL_WIDTH] != TILE_WATER);

            /* Determine wall type based on neighbors */
            if (floor_n && floor_e && !floor_s && !floor_w) {
                level->tiles[idx] = TILE_CORNER_BL;
            } else if (floor_n && floor_w && !floor_s && !floor_e) {
                level->tiles[idx] = TILE_CORNER_BR;
            } else if (floor_s && floor_e && !floor_n && !floor_w) {
                level->tiles[idx] = TILE_CORNER_TL;
            } else if (floor_s && floor_w && !floor_n && !floor_e) {
                level->tiles[idx] = TILE_CORNER_TR;
            } else if ((floor_n || floor_s) && !floor_e && !floor_w) {
                level->tiles[idx] = TILE_WALL_V;
            } else if ((floor_e || floor_w) && !floor_n && !floor_s) {
                level->tiles[idx] = TILE_WALL_H;
            }
        }
    }

    free(orig);
}

void generate_level(Level* level, uint32_t* rng) {
    /* Initialize all as walls */
    memset(level->tiles, TILE_WALL, LEVEL_SIZE);
    level->room_count = 0;

    /* Initialize influence map */
    memset(&level->influence, 0, sizeof(InfluenceMap));

    /* Generate random rooms */
    int attempts = 0;
    while (level->room_count < MAX_ROOMS && attempts < 100) {
        attempts++;

        int w = randi_range(rng, 10, 22);
        int h = randi_range(rng, 8, 16);
        int x = randi_range(rng, 2, LEVEL_WIDTH - w - 2);
        int y = randi_range(rng, 2, LEVEL_HEIGHT - h - 2);

        /* Check overlap with existing rooms */
        bool overlap = false;
        for (int i = 0; i < level->room_count && !overlap; i++) {
            Room* r = &level->rooms[i];
            if (x < r->x + r->width + 3 && x + w + 3 > r->x &&
                y < r->y + r->height + 3 && y + h + 3 > r->y) {
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

            /* Assign random theme */
            RoomTheme theme = randi_range(rng, 0, THEME_COUNT - 1);

            /* First room is always office/lobby (player start) */
            if (level->room_count == 1) {
                theme = THEME_LOBBY;
            }

            decorate_room(level, room, theme, rng);
        }
    }

    /* Connect rooms with corridors */
    #define CORRIDOR_WIDTH 4
    for (int i = 1; i < level->room_count; i++) {
        Room* r1 = &level->rooms[i - 1];
        Room* r2 = &level->rooms[i];

        int x1 = r1->x + r1->width / 2;
        int y1 = r1->y + r1->height / 2;
        int x2 = r2->x + r2->width / 2;
        int y2 = r2->y + r2->height / 2;

        /* Corridor floor type */
        uint8_t corridor_floor = (randf(rng) < 0.3f) ? TILE_GRATE : TILE_CONCRETE;

        /* L-shaped corridor */
        if (randf(rng) > 0.5f) {
            /* Horizontal first */
            int sx = x1 < x2 ? x1 : x2;
            int ex = x1 < x2 ? x2 : x1;
            for (int cx = sx; cx <= ex; cx++) {
                for (int w = 0; w < CORRIDOR_WIDTH; w++) {
                    int cy = y1 - CORRIDOR_WIDTH/2 + w;
                    if (cy >= 0 && cy < LEVEL_HEIGHT) {
                        int idx = cx + cy * LEVEL_WIDTH;
                        if (level->tiles[idx] == TILE_WALL) {
                            level->tiles[idx] = corridor_floor;
                        }
                    }
                }
            }
            /* Then vertical */
            int sy = y1 < y2 ? y1 : y2;
            int ey = y1 < y2 ? y2 : y1;
            for (int cy = sy; cy <= ey; cy++) {
                for (int w = 0; w < CORRIDOR_WIDTH; w++) {
                    int cx = x2 - CORRIDOR_WIDTH/2 + w;
                    if (cx >= 0 && cx < LEVEL_WIDTH) {
                        int idx = cx + cy * LEVEL_WIDTH;
                        if (level->tiles[idx] == TILE_WALL) {
                            level->tiles[idx] = corridor_floor;
                        }
                    }
                }
            }
        } else {
            /* Vertical first */
            int sy = y1 < y2 ? y1 : y2;
            int ey = y1 < y2 ? y2 : y1;
            for (int cy = sy; cy <= ey; cy++) {
                for (int w = 0; w < CORRIDOR_WIDTH; w++) {
                    int cx = x1 - CORRIDOR_WIDTH/2 + w;
                    if (cx >= 0 && cx < LEVEL_WIDTH) {
                        int idx = cx + cy * LEVEL_WIDTH;
                        if (level->tiles[idx] == TILE_WALL) {
                            level->tiles[idx] = corridor_floor;
                        }
                    }
                }
            }
            /* Then horizontal */
            int sx = x1 < x2 ? x1 : x2;
            int ex = x1 < x2 ? x2 : x1;
            for (int cx = sx; cx <= ex; cx++) {
                for (int w = 0; w < CORRIDOR_WIDTH; w++) {
                    int cy = y2 - CORRIDOR_WIDTH/2 + w;
                    if (cy >= 0 && cy < LEVEL_HEIGHT) {
                        int idx = cx + cy * LEVEL_WIDTH;
                        if (level->tiles[idx] == TILE_WALL) {
                            level->tiles[idx] = corridor_floor;
                        }
                    }
                }
            }
        }

        r2->connected = true;
    }
    #undef CORRIDOR_WIDTH

    /* Wall decorations disabled - using simple # for all walls */
    /* add_wall_decorations(level); */

    /* Set spawn point in first room */
    if (level->room_count > 0) {
        Room* spawn_room = &level->rooms[0];
        level->spawn_x = spawn_room->x + spawn_room->width / 2.0f;
        level->spawn_y = spawn_room->y + spawn_room->height / 2.0f;
    }

    /* Initialize cover values in influence map */
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            int idx = x + y * LEVEL_WIDTH;
            if (is_cover(level, x, y)) {
                level->influence.cover_value[idx] = 1.0f;
                /* Adjacent tiles get partial cover value */
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = x + dx, ny = y + dy;
                        if (nx >= 0 && nx < LEVEL_WIDTH && ny >= 0 && ny < LEVEL_HEIGHT) {
                            int nidx = nx + ny * LEVEL_WIDTH;
                            if (level->influence.cover_value[nidx] < 0.5f) {
                                level->influence.cover_value[nidx] = 0.5f;
                            }
                        }
                    }
                }
            }
        }
    }
}

/* ==========================================================================
 * VISION AND SOUND
 * ========================================================================== */

int check_view_cone(
    const Level* level,
    float obs_x, float obs_y, float obs_angle,
    float tgt_x, float tgt_y,
    float cone_angle, float view_dist,
    bool check_los
) {
    /* SSE: Calculate dx, dy, and distance in one go */
    __m128 obs = _mm_set_ps(0, 0, obs_y, obs_x);
    __m128 tgt = _mm_set_ps(0, 0, tgt_y, tgt_x);
    __m128 delta = _mm_sub_ps(tgt, obs);

    ALIGN32 float delta_arr[4];
    _mm_store_ps(delta_arr, delta);
    float dx = delta_arr[0];
    float dy = delta_arr[1];

    /* SSE distance calculation */
    __m128 sq = _mm_mul_ps(delta, delta);
    __m128 sum = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, 1));
    __m128 dist_vec = _mm_sqrt_ss(sum);
    float dist = _mm_cvtss_f32(dist_vec);

    /* SSE comparison: dist > view_dist * 1.5f */
    __m128 max_dist = _mm_set_ss(view_dist * 1.5f);
    if (_mm_comigt_ss(dist_vec, max_dist)) return 0;  /* Too far even for peripheral */

    /* Angle to target */
    float angle_to_target = atan2f(dy, dx);
    float angle_diff = angle_to_target - obs_angle;

    /* Normalize angle difference to [-PI, PI] */
    while (angle_diff > PI) angle_diff -= 2*PI;
    while (angle_diff < -PI) angle_diff += 2*PI;

    /* SSE absolute value */
    __m128 abs_mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
    __m128 angle = _mm_set_ss(angle_diff);
    angle = _mm_and_ps(angle, abs_mask);
    angle_diff = _mm_cvtss_f32(angle);

    int visibility = 0;

    /* SSE comparisons for visibility checks */
    __m128 periph_angle = _mm_set_ss(PERIPHERAL_ANGLE);
    __m128 periph_dist = _mm_set_ss(PERIPHERAL_DISTANCE);
    __m128 v_cone_angle = _mm_set_ss(cone_angle);
    __m128 v_view_dist = _mm_set_ss(view_dist);
    __m128 v_angle_diff = _mm_set_ss(angle_diff);

    /* Check peripheral vision (wider angle, shorter range) */
    if (_mm_comilt_ss(v_angle_diff, periph_angle) && _mm_comilt_ss(dist_vec, periph_dist)) {
        visibility = 1;
    }

    /* Check direct vision (narrower cone, longer range) */
    if (_mm_comilt_ss(v_angle_diff, v_cone_angle) && _mm_comilt_ss(dist_vec, v_view_dist)) {
        visibility = 2;
    }

    if (visibility == 0) return 0;

    /* Line of sight check using SSE for step calculation */
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
                return 0;  /* Blocked */
            }
        }
    }

    return visibility;
}

float get_corridor_width(const Level* level, float x, float y) {
    int ix = (int)x;
    int iy = (int)y;

    /* Count open tiles in cardinal directions */
    int open_h = 0, open_v = 0;

    for (int dx = -5; dx <= 5; dx++) {
        if (is_walkable(level, ix + dx, iy)) open_h++;
        else if (dx < 0) open_h = 0;  /* Reset if wall found before center */
        else break;
    }

    for (int dy = -5; dy <= 5; dy++) {
        if (is_walkable(level, ix, iy + dy)) open_v++;
        else if (dy < 0) open_v = 0;
        else break;
    }

    /* Return minimum dimension (narrower = more corridor-like) */
    return (float)(open_h < open_v ? open_h : open_v);
}

float calculate_sound_radius(const Level* level, float x, float y, float base_radius) {
    float corridor_width = get_corridor_width(level, x, y);

    /* Sound travels further in narrow corridors */
    float mult = 1.0f;
    if (corridor_width < 4.0f) {
        mult = CORRIDOR_SOUND_MULT;
    }

    return base_radius * mult;
}

/* ==========================================================================
 * COVER SYSTEM
 * ========================================================================== */

bool find_nearby_cover(const Level* level, float x, float y, float* cover_x, float* cover_y) {
    float best_dist = 1e10f;
    bool found = false;

    for (int dy = -8; dy <= 8; dy++) {
        for (int dx = -8; dx <= 8; dx++) {
            int cx = (int)x + dx;
            int cy = (int)y + dy;

            if (is_cover(level, cx, cy)) {
                /* Check if we can stand next to it */
                for (int ady = -1; ady <= 1; ady++) {
                    for (int adx = -1; adx <= 1; adx++) {
                        if (is_walkable(level, cx + adx, cy + ady) && !is_cover(level, cx + adx, cy + ady)) {
                            /* SSE distance squared calculation */
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

bool has_cover_from_direction(const Level* level, float x, float y, float threat_x, float threat_y) {
    float dx = threat_x - x;
    float dy = threat_y - y;
    float dist = sse_distance(0, 0, dx, dy);
    if (dist < 0.1f) return false;

    /* SSE normalization */
    sse_normalize(&dx, &dy);

    /* Check if there's cover between us and threat (within 2 tiles) */
    for (float t = 0.5f; t < 2.5f; t += 0.5f) {
        int cx = (int)(x + dx * t);
        int cy = (int)(y + dy * t);
        if (is_cover(level, cx, cy) || !is_walkable(level, cx, cy)) {
            return true;
        }
    }

    return false;
}

/* Find cover that provides protection from a specific threat */
bool find_cover_from_threat(const Level* level, float x, float y, float threat_x, float threat_y,
                            float* cover_x, float* cover_y) {
    float best_score = -1e10f;
    bool found = false;

    float to_threat_x = threat_x - x;
    float to_threat_y = threat_y - y;
    sse_normalize(&to_threat_x, &to_threat_y);

    for (int dy = -10; dy <= 10; dy++) {
        for (int dx = -10; dx <= 10; dx++) {
            int cx = (int)x + dx;
            int cy = (int)y + dy;

            if (!is_walkable(level, cx, cy)) continue;

            /* Check if this position has cover from threat direction */
            float pos_x = cx + 0.5f;
            float pos_y = cy + 0.5f;

            if (!has_cover_from_direction(level, pos_x, pos_y, threat_x, threat_y)) continue;

            /* Score based on distance (prefer closer cover) */
            float dist = sse_distance(0, 0, (float)dx, (float)dy);
            float score = 20.0f - dist;

            /* Bonus for cover that's perpendicular to threat direction (flanking position) */
            float to_cover_x = (float)dx;
            float to_cover_y = (float)dy;
            sse_normalize(&to_cover_x, &to_cover_y);
            float dot = to_cover_x * to_threat_x + to_cover_y * to_threat_y;
            score += (1.0f - fabsf(dot)) * 5.0f;  /* Perpendicular is better */

            if (score > best_score) {
                best_score = score;
                *cover_x = pos_x;
                *cover_y = pos_y;
                found = true;
            }
        }
    }

    return found;
}

/* ==========================================================================
 * A* PATHFINDING
 * ========================================================================== */

/* Priority queue node for A* */
typedef struct {
    int x, y;
    float f_cost;
} PQNode;

/* Simple min-heap operations for A* priority queue */
static void pq_swap(PQNode* a, PQNode* b) {
    PQNode tmp = *a;
    *a = *b;
    *b = tmp;
}

static void pq_push(PQNode* heap, int* size, int x, int y, float f) {
    int i = (*size)++;
    heap[i].x = x;
    heap[i].y = y;
    heap[i].f_cost = f;

    /* Bubble up */
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[i].f_cost < heap[parent].f_cost) {
            pq_swap(&heap[i], &heap[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

static PQNode pq_pop(PQNode* heap, int* size) {
    PQNode result = heap[0];
    (*size)--;

    if (*size > 0) {
        heap[0] = heap[*size];

        /* Bubble down */
        int i = 0;
        while (1) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < *size && heap[left].f_cost < heap[smallest].f_cost) {
                smallest = left;
            }
            if (right < *size && heap[right].f_cost < heap[smallest].f_cost) {
                smallest = right;
            }

            if (smallest != i) {
                pq_swap(&heap[i], &heap[smallest]);
                i = smallest;
            } else {
                break;
            }
        }
    }

    return result;
}

/* Octile distance heuristic (allows diagonal movement) */
static float heuristic(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int min_d = dx < dy ? dx : dy;
    int max_d = dx > dy ? dx : dy;
    return PATHFIND_STRAIGHT_COST * (max_d - min_d) + PATHFIND_DIAGONAL_COST * min_d;
}

bool find_path(
    const Level* level,
    float start_x, float start_y,
    float end_x, float end_y,
    float* out_next_x, float* out_next_y
) {
    int sx = (int)start_x;
    int sy = (int)start_y;
    int ex = (int)end_x;
    int ey = (int)end_y;

    /* Clamp to bounds */
    if (sx < 0) sx = 0; if (sx >= LEVEL_WIDTH) sx = LEVEL_WIDTH - 1;
    if (sy < 0) sy = 0; if (sy >= LEVEL_HEIGHT) sy = LEVEL_HEIGHT - 1;
    if (ex < 0) ex = 0; if (ex >= LEVEL_WIDTH) ex = LEVEL_WIDTH - 1;
    if (ey < 0) ey = 0; if (ey >= LEVEL_HEIGHT) ey = LEVEL_HEIGHT - 1;

    /* Already at destination */
    if (sx == ex && sy == ey) {
        *out_next_x = end_x;
        *out_next_y = end_y;
        return true;
    }

    /* Quick direct line check first */
    float dx = end_x - start_x;
    float dy = end_y - start_y;
    float len = sse_distance(0, 0, dx, dy);

    if (len > 0.1f && len < 15.0f) {
        /* SSE normalization */
        float ndx = dx, ndy = dy;
        sse_normalize(&ndx, &ndy);

        /* Check if direct path is clear (check a few points along the way) */
        bool clear = true;
        for (float t = 0.5f; t <= len && clear; t += 0.5f) {
            int cx = (int)(start_x + ndx * t);
            int cy = (int)(start_y + ndy * t);
            if (!is_walkable(level, cx, cy)) {
                clear = false;
            }
        }

        if (clear) {
            *out_next_x = start_x + ndx;
            *out_next_y = start_y + ndy;
            return true;
        }
    }

    /* A* pathfinding */
    static PQNode open_set[PATHFIND_MAX_ITERATIONS];
    static float g_cost[LEVEL_SIZE];
    static int8_t came_from[LEVEL_SIZE];  /* Direction we came from: 0-7, -1 = not visited */

    memset(g_cost, 0x7F, sizeof(g_cost));  /* Initialize to large value */
    memset(came_from, -1, sizeof(came_from));

    int open_size = 0;

    /* Direction offsets (8 directions) */
    const int dir_x[] = {1, 1, 0, -1, -1, -1, 0, 1};
    const int dir_y[] = {0, 1, 1, 1, 0, -1, -1, -1};
    const float dir_cost[] = {
        PATHFIND_STRAIGHT_COST,
        PATHFIND_DIAGONAL_COST,
        PATHFIND_STRAIGHT_COST,
        PATHFIND_DIAGONAL_COST,
        PATHFIND_STRAIGHT_COST,
        PATHFIND_DIAGONAL_COST,
        PATHFIND_STRAIGHT_COST,
        PATHFIND_DIAGONAL_COST
    };

    /* Start node */
    g_cost[sx + sy * LEVEL_WIDTH] = 0;
    came_from[sx + sy * LEVEL_WIDTH] = 8;  /* Mark as visited (source) */
    pq_push(open_set, &open_size, sx, sy, heuristic(sx, sy, ex, ey));

    bool found = false;
    int iterations = 0;

    while (open_size > 0 && !found && iterations < PATHFIND_MAX_ITERATIONS) {
        iterations++;

        PQNode current = pq_pop(open_set, &open_size);
        int cx = current.x;
        int cy = current.y;

        if (cx == ex && cy == ey) {
            found = true;
            break;
        }

        int cidx = cx + cy * LEVEL_WIDTH;

        /* Check all 8 directions */
        for (int d = 0; d < 8; d++) {
            int nx = cx + dir_x[d];
            int ny = cy + dir_y[d];

            if (!is_walkable(level, nx, ny)) continue;

            /* For diagonal movement, check that we don't cut corners */
            if (d % 2 == 1) {  /* Diagonal */
                if (!is_walkable(level, cx + dir_x[d], cy) ||
                    !is_walkable(level, cx, cy + dir_y[d])) {
                    continue;  /* Can't cut corner */
                }
            }

            int nidx = nx + ny * LEVEL_WIDTH;

            /* Calculate movement cost */
            float move_cost = dir_cost[d];

            /* Tactical cost modifiers from influence map */
            move_cost += level->influence.threat[nidx] * 0.5f;        /* Avoid threatened areas */
            move_cost += level->influence.visibility[nidx] * PATHFIND_EXPOSED_PENALTY;
            move_cost += level->influence.cover_value[nidx] * PATHFIND_COVER_BONUS;  /* Prefer cover */

            float new_g = g_cost[cidx] + move_cost;

            if (new_g < g_cost[nidx]) {
                g_cost[nidx] = new_g;
                came_from[nidx] = d;

                float f = new_g + heuristic(nx, ny, ex, ey);
                pq_push(open_set, &open_size, nx, ny, f);
            }
        }
    }

    if (found) {
        /* Trace back from goal to find next step from start */
        int cx = ex, cy = ey;
        int prev_cx = cx, prev_cy = cy;

        while (cx != sx || cy != sy) {
            int idx = cx + cy * LEVEL_WIDTH;
            int d = came_from[idx];
            if (d < 0 || d >= 8) break;

            prev_cx = cx;
            prev_cy = cy;

            /* Move in opposite direction */
            int opp = (d + 4) % 8;
            cx += dir_x[opp];
            cy += dir_y[opp];
        }

        /* prev_cx, prev_cy is the first step from start */
        *out_next_x = prev_cx + 0.5f;
        *out_next_y = prev_cy + 0.5f;
        return true;
    }

    /* Fallback: try to move in any valid direction toward goal */
    float best_dist = 1e10f;
    float best_x = start_x;
    float best_y = start_y;

    for (int d = 0; d < 8; d++) {
        float nx = start_x + dir_x[d];
        float ny = start_y + dir_y[d];

        if (is_walkable(level, (int)nx, (int)ny)) {
            /* SSE distance squared */
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
}

/* Tactical pathfinding - finds path that uses cover and avoids exposure */
bool find_tactical_path(
    const Level* level,
    float start_x, float start_y,
    float end_x, float end_y,
    float threat_x, float threat_y,
    float* out_next_x, float* out_next_y
) {
    /* Temporarily boost threat influence around threat position */
    int threat_tile_x = (int)threat_x;
    int threat_tile_y = (int)threat_y;

    /* Use standard A* but with boosted threat avoidance */
    /* The influence map already handles this, so just use find_path */
    return find_path(level, start_x, start_y, end_x, end_y, out_next_x, out_next_y);
}

/* ==========================================================================
 * INFLUENCE MAPS
 * ========================================================================== */

void update_influence_map(Level* level, const GameState* game) {
    /* Only update periodically */
    if (game->frame - level->influence.last_update < INFLUENCE_UPDATE_INTERVAL) {
        return;
    }
    level->influence.last_update = game->frame;

    /* Decay existing threat values */
    for (int i = 0; i < LEVEL_SIZE; i++) {
        level->influence.threat[i] *= INFLUENCE_THREAT_DECAY;
        if (level->influence.threat[i] < 0.01f) {
            level->influence.threat[i] = 0.0f;
        }
    }

    /* Add threat from each enemy entity */
    for (int e = 0; e < game->entity_count; e++) {
        const Entity* ent = &game->entities[e];
        if (!ent->alive) continue;

        int ex = (int)ent->x;
        int ey = (int)ent->y;

        /* Threat radius based on weapon range */
        WeaponStats stats = weapon_get_stats(&ent->weapon);
        int radius = (int)(stats.range * 0.5f);

        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                int tx = ex + dx;
                int ty = ey + dy;
                if (tx < 0 || tx >= LEVEL_WIDTH || ty < 0 || ty >= LEVEL_HEIGHT) continue;

                float dist = sse_distance(0, 0, (float)dx, (float)dy);
                if (dist > radius) continue;

                /* Threat decreases with distance */
                float threat_add = 1.0f - (dist / radius);

                /* Higher threat in entity's facing direction */
                float angle_to = atan2f((float)dy, (float)dx);
                float angle_diff = angle_to - ent->facing_angle;
                while (angle_diff > PI) angle_diff -= 2*PI;
                while (angle_diff < -PI) angle_diff += 2*PI;
                if (fabsf(angle_diff) < ent->view_cone_angle) {
                    threat_add *= 1.5f;
                }

                int idx = tx + ty * LEVEL_WIDTH;
                level->influence.threat[idx] += threat_add;
            }
        }
    }

    /* Clamp threat values */
    for (int i = 0; i < LEVEL_SIZE; i++) {
        if (level->influence.threat[i] > 10.0f) {
            level->influence.threat[i] = 10.0f;
        }
    }
}

/* Query influence at a position */
float get_threat_at(const Level* level, float x, float y) {
    int ix = (int)x;
    int iy = (int)y;
    if (ix < 0 || ix >= LEVEL_WIDTH || iy < 0 || iy >= LEVEL_HEIGHT) return 0.0f;
    return level->influence.threat[ix + iy * LEVEL_WIDTH];
}

float get_cover_value_at(const Level* level, float x, float y) {
    int ix = (int)x;
    int iy = (int)y;
    if (ix < 0 || ix >= LEVEL_WIDTH || iy < 0 || iy >= LEVEL_HEIGHT) return 0.0f;
    return level->influence.cover_value[ix + iy * LEVEL_WIDTH];
}
