#ifndef __BU_DOOR_H__
#define __BU_DOOR_H__

#include "gfc_vector.h"
#include "simple_json.h"

#include "bu_entity.h"

Entity *door_spawn(Vector2D position, SJson *args);

#endif
