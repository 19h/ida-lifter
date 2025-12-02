#include "common.h"
#include <string.h>
#include <time.h>

int main() {
    printf("\033[2J"); // Clear screen
    printf("=== Decompiler Reference Testbed ===\n");
    printf("Running 4 Simulations (5-10 seconds each)...\n");
    sleep_ms(1000);

    // 1. Bullet Raytrace (5 seconds = 5 * FPS frames)
    run_bullet_sim(5 * FPS);

    // 2. Interplanetary Distance
    run_interplanetary_sim();

    // 3. Asteroid Trajectory
    run_asteroid_sim();

    // 4. Fluid Sim
    run_fluid_sim();

    printf("\nAll tests completed.\n");
    return 0;
}
