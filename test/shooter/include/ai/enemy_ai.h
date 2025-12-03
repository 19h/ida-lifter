/*
 * Enemy AI System
 * AI behavior for non-player entities.
 */

#ifndef SHOOTER_ENEMY_AI_H
#define SHOOTER_ENEMY_AI_H

#include "../types.h"

/*
 * Update AI behavior for an enemy entity.
 * Handles state transitions, movement, and combat decisions.
 */
void update_enemy_ai(GameState* game, int entity_id);

#endif /* SHOOTER_ENEMY_AI_H */
