
#include "simple_logger.h"

#include "bu_entity.h"

#include "bu_door.h"
#include "bu_collision.h"
#include "bu_player.h"
#include "bu_level.h"

void door_touch(Entity *self, Entity *player)
{
	if (!self) return;

	slog("Level load");
	level_load("main_entrance");
	entity_free(self);
}

void door_think(Entity *self)
{
	if (!self) return;
	entity_collision_check(self);
}

Entity *door_spawn(Vector2D position, SJson *args)
{
	Entity *door;
	
	door = entity_new();
	if (!door) return NULL;
	door->name = "door";
	door->sprite = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);
	door->position.x = position.x;
	door->position.y = position.y;
	door->rect_collider.x = position.x;
	door->rect_collider.y = position.y;
	door->rect_collider.h = 50;
	door->rect_collider.w = 50;
	door->onTouch = door_touch;
	door->think = door_think;
	slog("Door spawn at position: %f, %f", position.x, position.y);
}

/*eol@eof*/
