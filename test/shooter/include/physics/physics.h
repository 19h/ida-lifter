/*
 * Physics System
 * Entity movement, bullet updates, and collision detection.
 */

#ifndef SHOOTER_PHYSICS_H
#define SHOOTER_PHYSICS_H

#include "../types.h"

/*
 * Update all physics simulation for one frame.
 * Handles entity movement, bullet movement, and collisions.
 */
void update_physics(GameState* game);

#endif /* SHOOTER_PHYSICS_H */
