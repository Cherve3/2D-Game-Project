#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_config.h"

#include "bu_timer.h"
#include "bu_level.h"
#include "bu_items.h"
#include "bu_player.h"
#include "bu_shop.h"

typedef struct
{
	Item *item_list;
	Uint32 item_count;
	Uint32 max_items;

}ItemManager;

static ItemManager items = { 0 };

static SJson *item_json = NULL;
static SJson* hazard_json = NULL;

void item_free(Item *self)
{
	if (!self)return;
	self->_inuse = 0;
	self->armor = 0;
	self->cost = 0;
	self->damage = 0;
	self->description = NULL;
	self->name = NULL;
	self->type = 0;
}

void load_item_json()
{
	if (!item_json)
		item_json = sj_load("json/item.json");
	else
		slog("Item info already initialized");

	if (!item_json)
		slog("item json not found.");

	if (!hazard_json)
		hazard_json = sj_load("json/hazards.json");
	else
		slog("Hazard info already initialized");

	if (!hazard_json)
		slog("hazard json not found.");
}

void item_manager_close()
{
	int i;

	slog("Item system closing...");
	sj_free(item_json);

	if (items.item_list != NULL)
	{
		for (i = 0; i < items.item_count; i++)
		{
			item_free(&items.item_list[i]);
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

	items.item_list = (Item*)gfc_allocate_array(sizeof(Item), maxItems);
	items.max_items = maxItems;
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

void item_touch(Entity *self, Entity *player)
{
	Item *item;
	const Uint8 *keys;
	keys = SDL_GetKeyboardState(NULL);
	if (!self) return;
	item = (Item*)self->data;
	if (player != get_player()->ent) return;

	if (keys[SDL_SCANCODE_Q] && get_player()->stats.can_carry && item->store_item == false)
	{
		if (get_current_time() - get_player_time() > 500)
		{
			get_player()->stats.can_carry = false;
			get_player()->stats.throw_item = true;
			item->picked_up = true;
			set_player_time(get_current_time());
			slog("Item pick up");
		}
	}
	else if (keys[SDL_SCANCODE_Q] && !get_player()->stats.can_carry && item->store_item == false)
	{
		if (get_current_time() - get_player_time() > 500)
		{
			get_player()->stats.can_carry = true;
			get_player()->stats.throw_item = false;
			item->picked_up = false;
			set_player_time(get_current_time());
			slog("Item dropped");
		}
	}
	
	if (keys[SDL_SCANCODE_E] && item->store_item == true)
	{
		
		if (get_current_time() - get_player_time() > 500)
		{
			slog("item buying");
			open_shop();
			set_player_time(get_current_time());
		}
	}
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && get_shop_open())
		buy_item(item, player);
}

void item_think(Entity *self)
{
	Item *item;
	if (!self) return;
	item = (Item*)self->data;
	
	if (item->picked_up && !get_player()->stats.throw_item)
	{
		item->picked_up = false;
		get_player()->stats.can_carry  = true;
		get_player()->stats.throw_item = false;
		//slog("item position: %f", item->ent->position.x);
		item->ent->position.x += 10 * (5 * SDL_cos(0));
		item->ent->rect_collider.x = cpBodyGetPosition(item->ent->body).x;
		item->ent->rect_collider.y = cpBodyGetPosition(item->ent->body).y;
	}

	entity_collision_check(self);
}

void item_update(Entity *self)
{
	Item *item;
	cpVect vec;
	if (!self) return;
	item = (Item*)self->data;
	if (item->picked_up){
		vec.x = get_player()->ent->position.x + 50;
		vec.y = get_player()->ent->position.y + 20;
		cpBodySetPosition(item->ent->body, vec);
		item->ent->rect_collider.x = cpBodyGetPosition(item->ent->body).x;
		item->ent->rect_collider.y = cpBodyGetPosition(item->ent->body).y;
	}
}

void item_create(TextWord *item_name, ItemType type, Item *item)
{
	SJson* item_info = NULL;
	SJson *info = NULL;
	Vector2D dimension;

	switch (type)
	{
		case consumable:
			item_info = sj_object_get_value(item_json, "consumable");
			item->store_item = true;
			if (gfc_word_cmp(item_name, "item_candy") == 0)
			{
				info = sj_object_get_value(item_info, "item_candy");
				item->ent->name = "item_candy";
			}
			if (gfc_word_cmp(item_name, "item_sandwich") == 0)
			{
				info = sj_object_get_value(item_info, "item_sandwich");
				item->ent->name = "item_sandwich";
			}
			if (gfc_word_cmp(item_name, "book_punch") == 0)
			{
				info = sj_object_get_value(item_info, "book_punch");
				item->ent->name = "book_punch";
			}
			if (gfc_word_cmp(item_name, "book_kick") == 0)
			{
				info = sj_object_get_value(item_info, "book_kick");
				item->ent->name = "book_kick";
			}
			if (gfc_word_cmp(item_name, "book_weapon") == 0)
			{
				info = sj_object_get_value(item_info, "book_weapon");
				item->ent->name = "book_weapon";
			}
			if (gfc_word_cmp(item_name, "book_throw") == 0)
			{
				info = sj_object_get_value(item_info, "book_throw");
				item->ent->name = "book_throw";
			}
			break;
		case weapon:
			item_info = sj_object_get_value(item_json, "weapon");
			item->store_item = false;
			if (gfc_word_cmp(item_name, "item_bat") == 0)
			{
				info = sj_object_get_value(item_info, "item_bat");
				item->ent->name = "item_bat";
			}
			if (gfc_word_cmp(item_name, "item_chain") == 0)
			{
				info = sj_object_get_value(item_info, "item_chain");
				item->ent->name = "item_chain";
			}
			if (gfc_word_cmp(item_name, "item_stick") == 0)
			{
				info = sj_object_get_value(item_info, "item_stick");
				item->ent->name = "item_stick";
			}
			break;
		case equipment:
			item_info = sj_object_get_value(item_json, "equipment");
			item->store_item = true;
			if (gfc_word_cmp(item_name, "item_helmet") == 0)
			{
				info = sj_object_get_value(item_info, "item_helmet");
				item->ent->name = "item_helmet";
			}
			if (gfc_word_cmp(item_name, "item_t_shirt") == 0)
			{
				info = sj_object_get_value(item_info, "item_t_shirt");
				item->ent->name = "item_t_shirt";
			}
			if (gfc_word_cmp(item_name, "item_pants") == 0)
			{
				info = sj_object_get_value(item_info, "item_pants");
				item->ent->name = "item_pants";
			}
			if (gfc_word_cmp(item_name, "item_runners") == 0)
			{
				info = sj_object_get_value(item_info, "item_runners");
				item->ent->name = "item_runners";
			}
		case hazard:
			item->store_item = false;
			if (gfc_word_cmp(item_name, "fire_hydrant") == 0)
			{
				info = sj_object_get_value(hazard_json, "fire_hydrant");
				item->ent->name = "fire_hydrant";
			}
			if (gfc_word_cmp(item_name, "water_spray") == 0)
			{
				info = sj_object_get_value(hazard_json, "water_spray");
				item->ent->name = "water_spray";
			}
			if (gfc_word_cmp(item_name, "puddle") == 0)
			{
				info = sj_object_get_value(hazard_json, "puddle");
				item->ent->name = "puddle";
			}
			if (gfc_word_cmp(item_name, "caution") == 0)
			{
				info = sj_object_get_value(hazard_json, "caution");
				item->ent->name = "caution";
			}
			if (gfc_word_cmp(item_name, "car") == 0)
			{
				info = sj_object_get_value(hazard_json, "car");
				item->ent->name = "car";
			}
			break;
	}

	if (!item_info)
		slog("item info is null");
	if (!info)
		slog("info is null");

	item->_inuse = true;
	item->picked_up = false;
	item->type = type;
	item->name = sj_get_string_value(sj_object_get_value(info, "name"));
	item->description = sj_get_string_value(sj_object_get_value(info, "description"));
	sj_get_integer_value(sj_object_get_value(info, "armor"), &item->armor);
	sj_get_integer_value(sj_object_get_value(info, "damage"), &item->damage);
	sj_get_integer_value(sj_object_get_value(info, "cost"), &item->cost);
	sj_value_as_vector2d(sj_object_get_value(info, "dimensions"), &dimension);
	slog("dimensions: %f, %f", dimension.x, dimension.y);
	item->ent->rect_collider.w = dimension.x;
	item->ent->rect_collider.h = dimension.y;

	if (gfc_word_cmp(item_name, "water_spray") == 0)
	{
		item->ent->rect_collider.w = 46.0;
		item->ent->rect_collider.h = 50.0;
		item->ent->sprite = gf2d_sprite_load_all(sj_get_string_value(sj_object_get_value(info, "sprite")), 46, 50, 5);
	}
	else if (gfc_word_cmp(item_name, "car") == 0)
	{
		item->ent->rect_collider.w = 97.0;
		item->ent->rect_collider.h = 145.0;
		item->ent->sprite = gf2d_sprite_load_all(sj_get_string_value(sj_object_get_value(info, "sprite")), 97, 145, 1);
	
	}
	else
	{
		item->ent->sprite = gf2d_sprite_load_image(sj_get_string_value(sj_object_get_value(info, "sprite")));
	}
}

void item_new(TextWord *item_name, ItemType type, cpVect position)
{
	int i;
	cpShape* shape;
	cpShapeFilter filter;

	if (items.item_list == NULL)
	{
		slog("Item system does not exist.");
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
		items.item_list[i].ent->body = cpBodyNew(1.0, INFINITY);
		cpSpaceAddBody(get_level()->space, items.item_list[i].ent->body);
		cpBodySetPosition(items.item_list[i].ent->body, position);
		shape = cpBoxShapeNew2(items.item_list[i].ent->body, cpBBNew(-0.1, -1.0, 50.0, 70.0), 0.2);
		filter.categories = item_f;
		filter.mask = space_f || world_f;
		cpShapeSetFilter(shape, filter);
		cpShape* p_shape = cpSpaceAddShape(get_level()->space, shape);


		items.item_list[i].ent->position = cpBodyGetPosition(items.item_list[i].ent->body);
		items.item_list[i].ent->name = "Item_1";
		items.item_list[i].ent->rect_collider.x = cpBodyGetPosition(items.item_list[i].ent->body).x;
		items.item_list[i].ent->rect_collider.y = cpBodyGetPosition(items.item_list[i].ent->body).y;
		items.item_list[i].ent->onTouch = item_touch;
		items.item_list[i].ent->think = item_think;
		items.item_list[i].ent->update = item_update;

		item_create(item_name, type, &items.item_list[i]);


		items.item_list[i].ent->data = (void*)&items.item_list[i];
		//print_item(&items.item_list[i]);
		return;
	}
	slog("no free items available.");
	return;
}

Uint32 get_max_items()
{
	return items.max_items;
}

Uint32 get_item_count()
{
	return items.item_count;
}

Item *get_item_list()
{
	return items.item_list;
}

/*eol@eof*/
