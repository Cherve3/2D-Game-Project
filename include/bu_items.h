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

	Bool _inuse;
	Bool picked_up;
	ItemType type;
	char *name;
	char *description;

	Entity *ent;

	int armor;
	int damage;

	int cost;
}Item;

void item_drop();

void item_free();

void item_new(TextWord *item_name, ItemType type, Vector2D position);

void item_manager_init(Uint32 maxItems);

#endif
