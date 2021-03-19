#include "simple_logger.h"
#include "simple_json.h"

#include "bu_items.h"
#include "bu_entity.h"

typedef struct
{
	Item *item_list;
	Uint32 item_count;

}ItemManager;

static ItemManager items = { 0 };

static SJson *item_info = NULL;

void item_free(Item self)
{

}

void load_item_json()
{
	if (!item_info)
		item_info = sj_load("json/item.json");
	else
		slog("Item info already initialized");

	if (!item_info)
		slog("item json not found.");
}

void item_close()
{
	int i;

	sj_free(item_info);

	if (items.item_list != NULL)
	{
		for (i = 0; i < items.item_count; i++)
		{
			entity_free(&items.item_list[i].ent);
		}
		free(items.item_list);
	}
	memset(&items, 0, sizeof(ItemManager));
	slog("Item System Closed");
}

void item_manager_init(Uint32 maxItems)
{
	if (items.item_list != NULL)
	{
		slog("WARNING: Item system already initialized");
		return;
	}

	items.item_list = gfc_allocate_array(sizeof(Item), maxItems);
	if (!items.item_list)
		slog("failed to allocate item list");

	load_item_json();

	items.item_count = maxItems;

	atexit(item_close);
	slog("Item System Initialized");
}

void item_spawn()
{

}

/*eol@eof*/
