
#include "simple_logger.h"

#include "bu_money.h"
#include "bu_player.h"
#include "bu_level.h"
#include "bu_entity.h"

typedef struct
{
	int value;
}Money;

void money_touch(Entity *self, Entity *player)
{
	Money *money;
	const Uint8 *keys;
	if (!self) return;
	keys = SDL_GetKeyboardState(NULL);
	money = (Money*)self->data;
	if (player == get_player()->ent)
	{
		if (keys[SDL_SCANCODE_E])
		{
			get_player()->stats.money = money->value;
			entity_free(self);
			slog("picking up money");
		}
	}
}

void money_think(Entity *self)
{
	if (!self) return;
	entity_collision_check(self);
}

void spawn_money(int value, cpVect position)
{
	Entity *ent;
	cpShape* shape;
	cpShapeFilter filter;

	Money *money = gfc_allocate_array(sizeof(Money),1);
	ent = entity_new();
	if (!money) return;
	money->value = value;
	ent->name = "Money";
	ent->position.x = position.x;
	ent->position.y = position.y;
	ent->rect_collider.x = position.x;
	ent->rect_collider.y = position.y;
	ent->rect_collider.w = 20;
	ent->rect_collider.h = 20;
	ent->think = money_think;
	ent->onTouch = money_touch;
	ent->body = cpBodyNew(1.0, INFINITY);
	cpSpaceAddBody(get_level()->space, ent->body);
	cpBodySetPosition(ent->body, ent->position);
	filter.categories = item_f;
	filter.mask = space_f || world_f;
	shape = cpBoxShapeNew2(ent->body, cpBBNew(-0.1, -1.0, 50.0, 70.0), 0.2);
	cpShapeSetFilter(shape, filter);
	cpShape* p_shape = cpSpaceAddShape(get_level()->space, shape);
	ent->data = (void*)money;
	ent->sprite = gf2d_sprite_load_image("images/items/money.png");
	
}

/*eol@eof*/
