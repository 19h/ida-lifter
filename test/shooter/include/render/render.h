/*
 * Rendering System
 * Terminal-based game visualization.
 */

#ifndef SHOOTER_RENDER_H
#define SHOOTER_RENDER_H

#include "../types.h"

/* Viewport structure with double buffering and color support */
struct Viewport {
    int width;
    int height;
    char** buffer;       /* Current frame character buffer */
    char** prev_buffer;  /* Previous frame for diff rendering */
    uint8_t** colors;    /* Color buffer (parallel to character buffer) */
    uint8_t** prev_colors; /* Previous colors for diff rendering */
};

/*
 * Create a new viewport for terminal rendering.
 */
Viewport* create_viewport(void);

/*
 * Free a viewport and its buffers.
 */
void free_viewport(Viewport* vp);

/*
 * Clear the viewport buffer.
 */
void clear_buffer(Viewport* vp);

/*
 * Draw a character at position (x, y).
 */
void draw_pixel_char(Viewport* vp, int x, int y, char c);

/*
 * Draw a colored character at position (x, y).
 */
void draw_pixel_colored(Viewport* vp, int x, int y, char c, uint8_t color);

/*
 * Draw a UTF-8 string as a single cell at position (x, y).
 */
void draw_pixel(Viewport* vp, int x, int y, const char* utf8);

/*
 * Draw a line from (x0, y0) to (x1, y1).
 */
void draw_line(Viewport* vp, int x0, int y0, int x1, int y1, const char* utf8);

/*
 * Draw a circle outline.
 */
void draw_circle(Viewport* vp, int cx, int cy, int r, const char* utf8);

/*
 * Draw a filled circle.
 */
void draw_filled_circle(Viewport* vp, int cx, int cy, int r, const char* utf8);

/*
 * Draw a string at position (x, y).
 */
void draw_string(Viewport* vp, int x, int y, const char* str);

/*
 * Draw a box from (x1, y1) to (x2, y2).
 * style: "single", "double", or "ascii"
 */
void draw_box(Viewport* vp, int x1, int y1, int x2, int y2, const char* style);

/*
 * Render the buffer to the terminal.
 */
void render_buffer(Viewport* vp);

/*
 * Sleep for the specified number of milliseconds.
 */
void sleep_ms(int ms);

/*
 * Render the game state to the viewport.
 */
void render_game(GameState* game, Viewport* vp);

#endif /* SHOOTER_RENDER_H */
