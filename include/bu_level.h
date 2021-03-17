#ifndef __BU_LEVEL_H__
#define __BU_LEVEL_H__

#include "gf2d_sprite.h"
#include "bu_entity.h"

typedef struct
{
	//Uint8   _inuse;
	char		*name;
	Sprite		*background;
	float		 width;
	float		 height;
	Vector2D	 position;
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
