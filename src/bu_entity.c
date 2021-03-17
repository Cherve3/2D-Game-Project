#include "simple_logger.h"

#include "bu_collision.h"
#include "bu_camera.h"
#include "bu_entity.h"

typedef struct
{
	Entity *entity_list;
	Uint32  max_entities;

}EntityManager;

static EntityManager entity_manager = { 0 };

void entity_manager_init(Uint32 max_entities)
{
	if (max_entities == 0)
	{
		slog("cannot allocate 0 entities!");
		return;
	}
	if (entity_manager.entity_list != NULL)
	{
		entity_manager_free();
	}
	entity_manager.entity_list = (Entity *)gfc_allocate_array(sizeof(Entity), max_entities);
	if (entity_manager.entity_list == NULL)
	{
		slog("failed to allocate entity list");
		return;
	}
	entity_manager.max_entities = max_entities;
	atexit(entity_manager_free);
	slog("Entity System Initialized");
}

void entity_manager_free()
{
	if (entity_manager.entity_list != NULL)
	{
		free(entity_manager.entity_list);
	}
	memset(&entity_manager, 0, sizeof(EntityManager));
	slog("entity system closed");
}

void entity_think(Entity *self)
{
	if (!self)return;
	if (self->think)self->think(self);
}

void entity_manager_think_entities()
{
	int i;
	if (entity_manager.entity_list == NULL)
	{
		slog("entity system does not exist");
		return;
	}
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse == 0)continue;
		entity_think(&entity_manager.entity_list[i]);
	}
}

void entity_update(Entity *self)
{
	if (!self)return;

	self->frame += self->frameRate;
	if (self->frame >= self->frameCount)self->frame = 0;
	if (self->update)self->update(self);
}

void entity_manager_update_entities()
{
	int i;
	if (entity_manager.entity_list == NULL)
	{
		slog("entity system does not exist");
		return;
	}
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse == 0)continue;
		entity_update(&entity_manager.entity_list[i]);
	}
}

Entity *entity_new()
{
	int i;
	if (entity_manager.entity_list == NULL)
	{
		slog("Entity system does not exist.");
		return NULL;
	}
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse) continue;

		memset(&entity_manager.entity_list[i], 0, sizeof(Entity));
		entity_manager.entity_list[i]._inuse = 1;
		return &entity_manager.entity_list[i];
	}
	slog("no free entities available.");
	return NULL;
}

void entity_free(Entity *ent)
{
	if (!ent)
	{
		slog("cannot free a NULL entity");
		return;
	}
	gf2d_sprite_free(ent->sprite);
	ent->sprite = NULL;
	ent->_inuse = 0;
}

void entity_draw(Entity *ent)
{
	Vector2D drawPosition, offset;
	if (!ent)
	{
		slog("cannot draw NULL entity");
		return;
	}
	if (ent->draw)
	{
		ent->draw(ent);
	}
	else
	{
		if (ent->sprite == NULL)
		{
			return;
		}
		offset = camera_get_offset();
		/*
		if (!camera_rect_on_screen(gfc_sdl_rect(ent->position.x, ent->position.y, ent->sprite->frame_w, ent->sprite->frame_h)));
		{
			slog("Entity %s not on screen", ent->sprite->filepath);
			return;
		}*/
		drawPosition.x = ent->position.x + offset.x;
		drawPosition.y = ent->position.y + offset.y;
//		slog("Position: %f,%f", drawPosition.x, drawPosition.y);
		gf2d_sprite_draw(
			ent->sprite,
			drawPosition,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			(Uint32)ent->frame);
	}
}

void entity_draw_all()
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (!entity_manager.entity_list[i]._inuse) continue;
		entity_draw(&entity_manager.entity_list[i]);
	}
}

void entity_touch(Entity *self, Entity *other)
{
	if (!self)return;
	if (!self->onTouch)return; // No touch function
	self->onTouch(self, other);
}

void entity_collision_check(Entity *self)
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (!entity_manager.entity_list[i]._inuse) continue;
		if (&entity_manager.entity_list[i] == self) continue;
		if (entity_collision(self->rect_collider, entity_manager.entity_list[i].rect_collider))
		{
			slog("Entity: %s collided with Entity: %s", self->name, entity_manager.entity_list[i].name);
			entity_touch(self, &entity_manager.entity_list[i]);
		}
	}
}

Entity *get_entities_list()
{
	return entity_manager.entity_list;
}

Uint32 get_max_entities()
{
	return entity_manager.max_entities;
}
/*eol@eof*/
