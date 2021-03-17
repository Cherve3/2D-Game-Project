#ifndef __BU_COLLISION_H__
#define __BU_COLLISION_H__

#include "bu_shapes.h"

Bool player_bounds_collision(Rect player, Rect bounds);

Bool entity_collision(Rect ent, Rect other);

#endif
