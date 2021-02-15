#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

Entity *player_spawn(Vector2D position);

void player_update(Entity *self);

#endif
