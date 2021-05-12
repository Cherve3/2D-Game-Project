#ifndef __BU_LEVEL_H__
#define __BU_LEVEL_H__

#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_types.h"

#include "gf2d_sprite.h"
#include "bu_entity.h"

#define space_f  0x0001
#define camera_f 0x0002
#define world_f  0x0003
#define player_f 0x0004
#define npc_f    0x0005
#define item_f   0x0006

typedef struct
{
	//Uint8   _inuse;
	char		*name;
	Sprite		*background;
	char        *audio;
	float		 width;
	float		 height;
	cpVect	     position;
	cpSpace		*space;
	cpShape		*border_top;
	cpShape		*border_bottom;
	cpShape		*border_left;
	cpShape		*border_right;
	Sprite		*sprite;
	Entity		*spawn_list;

}Level;

void level_free();

void level_init();

void level_load(char *name);

void level_draw();

Level *get_level();

Vector2D get_level_dimension();
#endif
