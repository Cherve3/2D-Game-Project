
#include "simple_logger.h"

#include "bu_entity.h"

#include "bu_door.h"
#include "bu_collision.h"
#include "bu_player.h"
#include "bu_level.h"

void door_touch(Entity *self, Entity *player)
{
	if (!self) return;
	if (player == get_player()->ent)
	{
		level_load(self->data);
		entity_free(self);
	}
}

void door_think(Entity *self)
{
	if (!self) return;
	entity_collision_check(self);
}

Entity *door_spawn(Vector2D position, Vector2D dimension, TextWord **args)
{
	Entity *door;
	TextLine *name;
	
	door = entity_new();
	if (!door) return NULL;
	slog("name: %s and location: %s", args[0], args[1]);
	door->name = args[0];
	door->sprite = gf2d_sprite_load_all("images/door.png", 128, 128, 16);
	door->_inuse = 1;
	door->position.x = position.x;
	door->position.y = position.y;
	door->rect_collider.x = position.x;
	door->rect_collider.y = position.y;
	door->rect_collider.w = dimension.x;
	door->rect_collider.h = dimension.y;
	door->onTouch = door_touch;
	door->think = door_think;
	door->data = args[1];
	slog("Door spawn at position: %f, %f dimension: %f, %f and exits to %s", position.x, position.y, dimension.x, dimension.y, args);
}

/*eol@eof*/