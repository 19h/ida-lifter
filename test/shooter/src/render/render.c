/*
 * Rendering System Implementation
 * Terminal-based game visualization with ANSI color support.
 */

#include "render/render.h"
#include "config.h"
#include "entity/entity.h"
#include "math/sse_math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

/* Get terminal dimensions from JavaScript */
EM_JS(int, get_term_cols, (), {
    return (typeof termCols !== 'undefined') ? termCols : 120;
});

EM_JS(int, get_term_rows, (), {
    return (typeof termRows !== 'undefined') ? termRows : 40;
});

#elif defined(_WIN32)
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

/* ==========================================================================
 * ANSI COLOR CODES
 * ========================================================================== */

#define ANSI_RESET       "\033[0m"
#define ANSI_BOLD        "\033[1m"
#define ANSI_DIM         "\033[2m"

/* Foreground colors */
#define FG_BLACK         "\033[30m"
#define FG_RED           "\033[31m"
#define FG_GREEN         "\033[32m"
#define FG_YELLOW        "\033[33m"
#define FG_BLUE          "\033[34m"
#define FG_MAGENTA       "\033[35m"
#define FG_CYAN          "\033[36m"
#define FG_WHITE         "\033[37m"
#define FG_BRIGHT_BLACK  "\033[90m"
#define FG_BRIGHT_RED    "\033[91m"
#define FG_BRIGHT_GREEN  "\033[92m"
#define FG_BRIGHT_YELLOW "\033[93m"
#define FG_BRIGHT_BLUE   "\033[94m"
#define FG_BRIGHT_MAGENTA "\033[95m"
#define FG_BRIGHT_CYAN   "\033[96m"
#define FG_BRIGHT_WHITE  "\033[97m"

/* Background colors */
#define BG_BLACK         "\033[40m"
#define BG_RED           "\033[41m"
#define BG_GREEN         "\033[42m"
#define BG_YELLOW        "\033[43m"
#define BG_BLUE          "\033[44m"
#define BG_MAGENTA       "\033[45m"
#define BG_CYAN          "\033[46m"
#define BG_WHITE         "\033[47m"
#define BG_BRIGHT_BLACK  "\033[100m"

/* Color indices for our color buffer */
#define COL_DEFAULT      0
#define COL_WALL         1
#define COL_FLOOR        2
#define COL_COVER        3
#define COL_WATER        4
#define COL_DOOR         5
#define COL_CARPET       6
#define COL_GRATE        7
#define COL_GRASS        8
#define COL_CONCRETE     9
#define COL_TERMINAL     10
#define COL_DEBRIS       11
#define COL_PLAYER       12
#define COL_ENEMY        13
#define COL_ENEMY_ALERT  14
#define COL_BULLET_P     15
#define COL_BULLET_E     16
#define COL_EXIT_ACTIVE  17
#define COL_EXIT_LOCKED  18
#define COL_DEAD         19
#define COL_PILLAR       20
#define COL_UI           21

/* Get ANSI color string for a color index */
static const char* get_color_code(int col) {
    switch (col) {
        case COL_WALL:        return FG_WHITE ANSI_BOLD;
        case COL_FLOOR:       return FG_BRIGHT_BLACK;
        case COL_COVER:       return FG_YELLOW;
        case COL_WATER:       return FG_BLUE ANSI_BOLD;
        case COL_DOOR:        return FG_BRIGHT_YELLOW ANSI_BOLD;
        case COL_CARPET:      return FG_MAGENTA;
        case COL_GRATE:       return FG_CYAN;
        case COL_GRASS:       return FG_GREEN;
        case COL_CONCRETE:    return FG_BRIGHT_BLACK;
        case COL_TERMINAL:    return FG_BRIGHT_CYAN ANSI_BOLD;
        case COL_DEBRIS:      return FG_YELLOW ANSI_DIM;
        case COL_PLAYER:      return FG_BRIGHT_GREEN ANSI_BOLD;
        case COL_ENEMY:       return FG_RED;
        case COL_ENEMY_ALERT: return FG_BRIGHT_RED ANSI_BOLD;
        case COL_BULLET_P:    return FG_BRIGHT_YELLOW ANSI_BOLD;
        case COL_BULLET_E:    return FG_BRIGHT_RED;
        case COL_EXIT_ACTIVE: return FG_BRIGHT_GREEN ANSI_BOLD;
        case COL_EXIT_LOCKED: return FG_BRIGHT_BLACK;
        case COL_DEAD:        return FG_BRIGHT_BLACK ANSI_DIM;
        case COL_PILLAR:      return FG_WHITE;
        case COL_UI:          return FG_WHITE;
        default:              return ANSI_RESET;
    }
}

/* ==========================================================================
 * VIEWPORT MANAGEMENT
 * ========================================================================== */

Viewport* create_viewport(void) {
    Viewport* vp = malloc(sizeof(Viewport));
    if (!vp) return NULL;

#ifdef __EMSCRIPTEN__
    /* Get size from xterm.js via JavaScript */
    vp->width = get_term_cols();
    vp->height = get_term_rows();
#elif defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    vp->width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    vp->height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    vp->width = ws.ws_col;
    vp->height = ws.ws_row;
#endif

    /* Allocate character buffers */
    vp->buffer = malloc(vp->height * sizeof(char*));
    vp->prev_buffer = malloc(vp->height * sizeof(char*));
    /* Allocate color buffers */
    vp->colors = malloc(vp->height * sizeof(uint8_t*));
    vp->prev_colors = malloc(vp->height * sizeof(uint8_t*));

    for (int y = 0; y < vp->height; y++) {
        vp->buffer[y] = calloc(vp->width + 1, 1);
        vp->prev_buffer[y] = calloc(vp->width + 1, 1);
        vp->colors[y] = calloc(vp->width, sizeof(uint8_t));
        vp->prev_colors[y] = calloc(vp->width, sizeof(uint8_t));
        memset(vp->buffer[y], ' ', vp->width);
        memset(vp->prev_buffer[y], ' ', vp->width);
    }

    return vp;
}

void free_viewport(Viewport* vp) {
    if (!vp) return;
    for (int y = 0; y < vp->height; y++) {
        free(vp->buffer[y]);
        free(vp->prev_buffer[y]);
        free(vp->colors[y]);
        free(vp->prev_colors[y]);
    }
    free(vp->buffer);
    free(vp->prev_buffer);
    free(vp->colors);
    free(vp->prev_colors);
    free(vp);
}

void clear_buffer(Viewport* vp) {
    for (int y = 0; y < vp->height; y++) {
        memset(vp->buffer[y], ' ', vp->width);
        memset(vp->colors[y], COL_DEFAULT, vp->width);
    }
}

/* ==========================================================================
 * DRAWING PRIMITIVES
 * ========================================================================== */

void draw_pixel_char(Viewport* vp, int x, int y, char c) {
    if (x >= 0 && x < vp->width && y >= 0 && y < vp->height) {
        vp->buffer[y][x] = c;
        vp->colors[y][x] = COL_DEFAULT;
    }
}

void draw_pixel_colored(Viewport* vp, int x, int y, char c, uint8_t color) {
    if (x >= 0 && x < vp->width && y >= 0 && y < vp->height) {
        vp->buffer[y][x] = c;
        vp->colors[y][x] = color;
    }
}

void draw_pixel(Viewport* vp, int x, int y, const char* utf8) {
    if (x >= 0 && x < vp->width && y >= 0 && y < vp->height) {
        /* For simplicity, just use first character */
        vp->buffer[y][x] = utf8[0];
    }
}

void draw_line(Viewport* vp, int x0, int y0, int x1, int y1, const char* utf8) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        draw_pixel(vp, x0, y0, utf8);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

void draw_circle(Viewport* vp, int cx, int cy, int r, const char* utf8) {
    int x = r, y = 0;
    int err = 1 - r;

    while (x >= y) {
        draw_pixel(vp, cx + x, cy + y, utf8);
        draw_pixel(vp, cx - x, cy + y, utf8);
        draw_pixel(vp, cx + x, cy - y, utf8);
        draw_pixel(vp, cx - x, cy - y, utf8);
        draw_pixel(vp, cx + y, cy + x, utf8);
        draw_pixel(vp, cx - y, cy + x, utf8);
        draw_pixel(vp, cx + y, cy - x, utf8);
        draw_pixel(vp, cx - y, cy - x, utf8);

        y++;
        if (err < 0) {
            err += 2 * y + 1;
        } else {
            x--;
            err += 2 * (y - x) + 1;
        }
    }
}

void draw_filled_circle(Viewport* vp, int cx, int cy, int r, const char* utf8) {
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx*dx + dy*dy <= r*r) {
                draw_pixel(vp, cx + dx, cy + dy, utf8);
            }
        }
    }
}

void draw_string(Viewport* vp, int x, int y, const char* str) {
    if (y < 0 || y >= vp->height) return;
    int len = (int)strlen(str);
    for (int i = 0; i < len && x + i < vp->width; i++) {
        if (x + i >= 0) {
            vp->buffer[y][x + i] = str[i];
        }
    }
}

void draw_box(Viewport* vp, int x1, int y1, int x2, int y2, const char* style) {
    char tl, tr, bl, br, h, v;

    if (strcmp(style, "double") == 0) {
        tl = '+'; tr = '+'; bl = '+'; br = '+'; h = '='; v = '|';
    } else if (strcmp(style, "single") == 0) {
        tl = '+'; tr = '+'; bl = '+'; br = '+'; h = '-'; v = '|';
    } else {
        tl = '+'; tr = '+'; bl = '+'; br = '+'; h = '-'; v = '|';
    }

    /* Corners */
    draw_pixel_char(vp, x1, y1, tl);
    draw_pixel_char(vp, x2, y1, tr);
    draw_pixel_char(vp, x1, y2, bl);
    draw_pixel_char(vp, x2, y2, br);

    /* Horizontal edges */
    for (int x = x1 + 1; x < x2; x++) {
        draw_pixel_char(vp, x, y1, h);
        draw_pixel_char(vp, x, y2, h);
    }

    /* Vertical edges */
    for (int y = y1 + 1; y < y2; y++) {
        draw_pixel_char(vp, x1, y, v);
        draw_pixel_char(vp, x2, y, v);
    }
}

/* ==========================================================================
 * BUFFER RENDERING
 * ========================================================================== */

void render_buffer(Viewport* vp) {
    /* Use differential rendering with color support */
    for (int y = 0; y < vp->height; y++) {
        /* Check if this line needs updating (chars or colors changed) */
        bool line_changed = (memcmp(vp->buffer[y], vp->prev_buffer[y], vp->width) != 0) ||
                           (memcmp(vp->colors[y], vp->prev_colors[y], vp->width) != 0);

        if (line_changed) {
            /* Move to start of line */
            printf("\033[%d;1H", y + 1);

            /* Render each character with its color */
            int current_color = -1;
            for (int x = 0; x < vp->width; x++) {
                int col = vp->colors[y][x];
                if (col != current_color) {
                    printf("%s%s", ANSI_RESET, get_color_code(col));
                    current_color = col;
                }
                putchar(vp->buffer[y][x]);
            }
            printf("%s", ANSI_RESET);

            /* Update previous buffers */
            memcpy(vp->prev_buffer[y], vp->buffer[y], vp->width);
            memcpy(vp->prev_colors[y], vp->colors[y], vp->width);
        }
    }
    fflush(stdout);
}

void sleep_ms(int ms) {
#ifdef __EMSCRIPTEN__
    /* emscripten_sleep requires ASYNCIFY to yield properly */
    emscripten_sleep((unsigned int)ms);
#elif defined(_WIN32)
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

/* ==========================================================================
 * GAME RENDERING
 * ========================================================================== */

void render_game(GameState* game, Viewport* vp) {
    Entity* player = &game->entities[game->player_id];

    /* Calculate visible game area (excluding UI: 3 lines header, 1 line footer) */
    int game_area_height = vp->height - 4;
    int game_area_width = vp->width;

    /* Update camera to follow player (center player in game area) */
    float target_cam_x = player->x - game_area_width / 2.0f;
    float target_cam_y = player->y - game_area_height / 2.0f;

    /* Smooth camera follow using SSE lerp */
    game->camera_x = sse_lerp(game->camera_x, target_cam_x, 0.1f);
    game->camera_y = sse_lerp(game->camera_y, target_cam_y, 0.1f);

    /* Clamp camera to level bounds using SSE clamp */
    float max_cam_x = (float)(LEVEL_WIDTH - game_area_width);
    float max_cam_y = (float)(LEVEL_HEIGHT - game_area_height);
    if (max_cam_x < 0) max_cam_x = 0;  /* Level smaller than viewport */
    if (max_cam_y < 0) max_cam_y = 0;
    game->camera_x = sse_clamp(game->camera_x, 0, max_cam_x);
    game->camera_y = sse_clamp(game->camera_y, 0, max_cam_y);

    int cam_x = (int)game->camera_x;
    int cam_y = (int)game->camera_y;

    clear_buffer(vp);

    /* Render level tiles with colors and varied symbols */
    for (int y = 0; y < vp->height - 4; y++) {
        for (int x = 0; x < vp->width; x++) {
            int lx = cam_x + x;
            int ly = cam_y + y;

            if (lx >= 0 && lx < LEVEL_WIDTH && ly >= 0 && ly < LEVEL_HEIGHT) {
                int idx = lx + ly * LEVEL_WIDTH;
                uint8_t tile = game->level.tiles[idx];
                char ch;
                uint8_t col;

                switch (tile) {
                    /* Walls - simple, clean look */
                    case TILE_WALL:
                    case TILE_WALL_H:
                    case TILE_WALL_V:
                    case TILE_CORNER_TL:
                    case TILE_CORNER_TR:
                    case TILE_CORNER_BL:
                    case TILE_CORNER_BR:
                        ch = '#';
                        col = COL_WALL;
                        break;
                    case TILE_PILLAR:
                        ch = 'O';
                        col = COL_PILLAR;
                        break;

                    /* Floors - subtle, clean */
                    case TILE_FLOOR:
                    case TILE_CONCRETE:
                    case TILE_DEBRIS:
                        ch = ' ';  /* Empty space for clean look */
                        col = COL_DEFAULT;
                        break;
                    case TILE_CARPET:
                        ch = '.';
                        col = COL_CARPET;
                        break;
                    case TILE_GRATE:
                    case TILE_VENT:
                        ch = '%';
                        col = COL_GRATE;
                        break;
                    case TILE_GRASS:
                        ch = '.';
                        col = COL_GRASS;
                        break;

                    /* Cover objects - stand out */
                    case TILE_COVER:
                        ch = '=';
                        col = COL_COVER;
                        break;
                    case TILE_CRATE:
                        ch = '#';
                        col = COL_COVER;
                        break;
                    case TILE_BARREL:
                        ch = 'o';
                        col = COL_COVER;
                        break;
                    case TILE_TERMINAL:
                        ch = '$';
                        col = COL_TERMINAL;
                        break;

                    /* Special tiles */
                    case TILE_DOOR:
                        ch = '+';
                        col = COL_DOOR;
                        break;
                    case TILE_WATER:
                        ch = '~';
                        col = COL_WATER;
                        break;

                    default:
                        ch = ' ';
                        col = COL_DEFAULT;
                        break;
                }
                draw_pixel_colored(vp, x, y + 3, ch, col);
            }
        }
    }

    /* Render exit marker if objective is REACH_EXIT */
    if (game->objective.type == OBJECTIVE_REACH_EXIT) {
        int ex = (int)(game->objective.exit_x - cam_x);
        int ey = (int)(game->objective.exit_y - cam_y) + 3;

        /* Count enemies to determine if exit is active */
        int enemies_alive = 0;
        for (int i = 0; i < game->entity_count; i++) {
            if (game->entities[i].team != 0 && game->entities[i].alive) enemies_alive++;
        }

        if (ex >= 0 && ex < vp->width && ey >= 3 && ey < vp->height - 1) {
            /* Show different marker based on whether exit is active */
            if (enemies_alive == 0) {
                draw_pixel_colored(vp, ex, ey, 'E', COL_EXIT_ACTIVE);
            } else {
                draw_pixel_colored(vp, ex, ey, 'e', COL_EXIT_LOCKED);
            }
        }
    }

    /* Render bullets with colors */
    for (int i = 0; i < game->bullet_count; i++) {
        Bullet* b = &game->bullets[i];
        if (!b->active) continue;

        int sx = (int)(b->x - cam_x);
        int sy = (int)(b->y - cam_y) + 3;

        if (sx >= 0 && sx < vp->width && sy >= 3 && sy < vp->height - 1) {
            if (b->team == 0) {
                draw_pixel_colored(vp, sx, sy, '*', COL_BULLET_P);
            } else {
                draw_pixel_colored(vp, sx, sy, 'o', COL_BULLET_E);
            }
        }
    }

    /* Render entities with colors */
    for (int i = 0; i < game->entity_count; i++) {
        Entity* e = &game->entities[i];

        int sx = (int)(e->x - cam_x);
        int sy = (int)(e->y - cam_y) + 3;

        if (sx < 0 || sx >= vp->width || sy < 3 || sy >= vp->height - 1) continue;

        if (!e->alive) {
            draw_pixel_colored(vp, sx, sy, 'x', COL_DEAD);
            continue;
        }

        char entity_char;
        uint8_t entity_color;

        if (e->team == 0) {
            entity_char = '@';
            entity_color = COL_PLAYER;
        } else {
            switch (e->type) {
                case ENEMY_GRUNT:   entity_char = 'g'; break;
                case ENEMY_SNIPER:  entity_char = 's'; break;
                case ENEMY_RUSHER:  entity_char = 'r'; break;
                case ENEMY_HEAVY:   entity_char = 'H'; break;
                default:            entity_char = '?'; break;
            }
            /* Capitalize and change color if alert/attacking */
            if (e->state >= STATE_ALERT) {
                entity_char = entity_char - 32;  /* To uppercase */
                entity_color = COL_ENEMY_ALERT;
            } else {
                entity_color = COL_ENEMY;
            }
        }

        draw_pixel_colored(vp, sx, sy, entity_char, entity_color);

        /* Draw facing direction indicator for player */
        if (e->team == 0) {
            int fx = sx + (int)(cosf(e->facing_angle) * 1.5f);
            int fy = sy + (int)(sinf(e->facing_angle) * 1.5f);
            if (fx >= 0 && fx < vp->width && fy >= 3 && fy < vp->height - 1) {
                draw_pixel_colored(vp, fx, fy, '+', COL_PLAYER);
            }
        }
    }

    /* UI - Top bar */
    draw_box(vp, 0, 0, vp->width - 1, 2, "single");

    char status[256];
    int alive_enemies = 0;
    for (int i = 0; i < game->entity_count; i++) {
        if (game->entities[i].team != 0 && game->entities[i].alive) alive_enemies++;
    }

    WeaponStats wstats = weapon_get_stats(&player->weapon);
    snprintf(status, sizeof(status),
        " HP:%3.0f  STA:%3.0f  MAG:%2d/%2d  RES:%3d  MED:%d  Enemies:%2d ",
        player->health, player->stamina,
        player->weapon.mag_current, wstats.mag_size,
        player->weapon.reserve, player->medpens, alive_enemies);
    draw_string(vp, 1, 1, status);

    /* Objective status */
    char obj_str[64];
    if (game->objective.type == OBJECTIVE_REACH_EXIT) {
        if (alive_enemies > 0) {
            snprintf(obj_str, sizeof(obj_str), "Kill all enemies, then reach EXIT");
        } else {
            snprintf(obj_str, sizeof(obj_str), "Reach the EXIT (E)!");
        }
    } else {
        snprintf(obj_str, sizeof(obj_str), "Eliminate all enemies");
    }
    draw_string(vp, vp->width - (int)strlen(obj_str) - 2, 1, obj_str);

    /* Bottom status */
    char state_str[32];
    switch (player->state) {
        case STATE_RELOAD: strcpy(state_str, "RELOADING"); break;
        case STATE_HEALING: strcpy(state_str, "HEALING"); break;
        case STATE_RETREATING: strcpy(state_str, "RETREATING"); break;
        case STATE_HIDING: strcpy(state_str, "IN COVER"); break;
        case STATE_COMBAT: strcpy(state_str, "COMBAT"); break;
        case STATE_PATROL: strcpy(state_str, "PATROL"); break;
        default:
            if (player->is_running) strcpy(state_str, "RUNNING");
            else strcpy(state_str, "IDLE");
            break;
    }

    char bottom[128];
    snprintf(bottom, sizeof(bottom), " [%s] @=you  e=exit(locked) E=EXIT(open)  CAPS=alert ", state_str);
    draw_string(vp, 1, vp->height - 1, bottom);
}
