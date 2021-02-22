#include "simple_logger.h"

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
	if (!ent)
	{
		slog("cannot draw NULL entity");
		return;
	}
	if (ent->draw)
	{
		ent->draw(ent);
	}
	if (ent->sprite == NULL)
	{
		return;
	}
	gf2d_sprite_draw(
		ent->sprite,
		ent->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)ent->frame);
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

/*eol@eof*/
