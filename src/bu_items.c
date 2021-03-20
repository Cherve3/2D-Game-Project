#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_config.h"

#include "bu_items.h"
#include "bu_entity.h"

typedef struct
{
	Item *item_list;
	Uint32 item_count;

}ItemManager;

static ItemManager items = { 0 };

static SJson *item_json = NULL;

void item_free(Item self)
{

}

void load_item_json()
{
	if (!item_json)
		item_json = sj_load("json/item.json");
	else
		slog("Item info already initialized");

	if (!item_json)
		slog("item json not found.");
}

void item_manager_close()
{
	int i;

	sj_free(item_json);

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

	atexit(item_manager_close);
	slog("Item System Initialized");
}

void print_item(Item *item)
{
	slog("name:        %s", item->name);
	slog("description: %s", item->description);
	slog("type:        %i", item->type);
	slog("armor:       %i", item->armor);
	slog("damage:      %i", item->damage);
	slog("cost:        %i", item->cost);
	slog("filepath:    %s", item->ent->sprite->filepath);
	slog("pos x:       %f", item->ent->rect_collider.x);
	slog("pos y:       %f", item->ent->rect_collider.y);
	slog("width:       %f", item->ent->rect_collider.w);
	slog("height:      %f", item->ent->rect_collider.h);
}

void item_create(TextWord *item_name, ItemType type, Item *item)
{
	SJson *item_info;
	SJson *info;
	Vector2D dimension;


	switch (type)
	{
		case consumable:
			item_info = sj_object_get_value(item_json, "consumable");
			if (gfc_word_cmp(item_name, "candy") == 0)
				info = sj_object_get_value(item_info, "candy");
			if (gfc_word_cmp(item_name, "sandwich") == 0)
				info = sj_object_get_value(item_info, "sandwich");
			break;
		case weapon:
			item_info = sj_object_get_value(item_json, "weapon");
			if (gfc_word_cmp(item_name, "bat") == 0)
				info = sj_object_get_value(item_info, "bat");
			if (gfc_word_cmp(item_name, "chain") == 0)
				info = sj_object_get_value(item_info, "chain");
			if (gfc_word_cmp(item_name, "stick") == 0)
				info = sj_object_get_value(item_info, "stick");
			break;
		case equipment:
			item_info = sj_object_get_value(item_json, "equipment");
			if (gfc_word_cmp(item_name, "helmet") == 0)
				info = sj_object_get_value(item_info, "helmet");
			if (gfc_word_cmp(item_name, "t_shirt") == 0)
				info = sj_object_get_value(item_info, "t_shirt");
			if (gfc_word_cmp(item_name, "pants") == 0)
				info = sj_object_get_value(item_info, "pants");
			if (gfc_word_cmp(item_name, "runners") == 0)
				info = sj_object_get_value(item_info, "runners");
			break;
	}

	if (!item_info)
		slog("item info is null");
	if (!info)
		slog("info is null");

	item->_inuse = 1;
	item->type = type;
	item->name = sj_get_string_value(sj_object_get_value(info, "name"));
	item->description = sj_get_string_value(sj_object_get_value(info, "description"));
	sj_get_integer_value(sj_object_get_value(info, "armor"), &item->armor);
	sj_get_integer_value(sj_object_get_value(info, "damage"), &item->damage);
	sj_get_integer_value(sj_object_get_value(info, "cost"), &item->cost);
	item->ent->sprite = gf2d_sprite_load_image(sj_get_string_value(sj_object_get_value(info, "sprite")));
	sj_value_as_vector2d(sj_object_get_value(info, "dimensions"), &dimension);
	slog("dimensions: %f, %f", dimension.x, dimension.y);
	item->ent->rect_collider.w = dimension.x;
	item->ent->rect_collider.h = dimension.y;
	slog("dimensions: %f, %f", item->ent->rect_collider.x, item->ent->rect_collider.y);
}

void item_new(TextWord *item_name, ItemType type, Vector2D position)
{
	int i;
	if (items.item_list == NULL)
	{
		slog("Entity system does not exist.");
		return NULL;
	}
	for (i = 0; i < items.item_count; i++)
	{
		if (items.item_list[i]._inuse) continue;

		memset(&items.item_list[i], 0, sizeof(Item));

		items.item_list[i].ent = entity_new();
		if (!items.item_list[i].ent)
		{
			slog("Entity not created for item.");
			return;
		}
		items.item_list[i].ent->position = position;
		items.item_list[i].ent->name = "Item_1";
		items.item_list[i].ent->rect_collider.x = position.x;
		items.item_list[i].ent->rect_collider.y = position.y;

		item_create(item_name, type, &items.item_list[i]);
		print_item(&items.item_list[i]);
		return;
	}
	slog("no free items available.");
	return;
}

/*eol@eof*/
