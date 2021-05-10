#ifndef __BU_DOOR_H__
#define __BU_DOOR_H__

#include "chipmunk/chipmunk.h"

#include "gfc_vector.h"
#include "simple_json.h"

#include "bu_entity.h"

void door_spawn(cpVect position, Vector2D dimension, TextWord **args);

#endif
