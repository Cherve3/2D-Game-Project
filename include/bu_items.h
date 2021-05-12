#ifndef __BU_ITEMS_H__
#define __BU_ITEMS_H__

#include "chipmunk/chipmunk.h"

#include "bu_entity.h"

typedef enum
{
	consumable,
	weapon,
	equipment,
	hazard
}ItemType;

typedef struct{

	Bool _inuse;
	Bool picked_up;
	Bool store_item;
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

void item_new(TextWord *item_name, ItemType type, cpVect position);

void item_manager_init(Uint32 maxItems);

Uint32 get_max_items();

Uint32 get_item_count();

Item *get_item_list();

#endif
