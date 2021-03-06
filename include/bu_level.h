#ifndef __BU_LEVEL_H__
#define __BU_LEVEL_H__

#include "gf2d_sprite.h"

typedef struct
{
	//Uint8   _inuse;
	char   *name;
	Sprite *background;
	float width;
	float height;
	Vector2D position;
	Sprite *sprite;

}Level;

void level_free(char *name);

void level_load(char *name);

void level_draw();

Level *get_level();
#endif
