/*
 * Shooter Game Entry Point
 * 2D tactical shooter with AVX/SSE SIMD optimizations.
 */

#include "game/game.h"
#include <stdlib.h>

int main(int argc, char** argv) {
    int frames = -1;  /* Run until game ends naturally */
    if (argc > 1) {
        frames = atoi(argv[1]);
    }
    run_bullet_sim(frames);
    return 0;
}
