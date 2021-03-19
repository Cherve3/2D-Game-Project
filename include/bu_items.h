#ifndef __BU_ITEMS_H__
#define __BU_ITEMS_H__

#include "bu_entity.h"

typedef enum
{
	consumable,
	weapon,
	equipment,
}ItemType;

typedef struct{
	ItemType type;
	char *name;
	char *description;

	Entity *ent;

	Uint8 armor;
	Uint8 damage;

	Uint8 cost;
}Item;

void item_drop();

void item_free();

void item_spawn();

void item_manager_init(Uint32 maxItems);

#endif
