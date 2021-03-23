
#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_config.h"

#include "bu_camera.h"
#include "bu_level.h"
#include "bu_door.h"
#include "bu_items.h"
#include "bu_player.h"
#include "bu_npc.h"

static Level *level = { 0 };
static SJson *level_json = NULL;
static SJson *spawn_list = NULL;

void level_free()
{
	if (!level)return;
	slog("freeing level %s", level->name);
	free(level->spawn_list);
	gf2d_sprite_free(level->background);
	gf2d_sprite_free(level->sprite);
	
	/*TODO: Does not close cleanly freeing sjson files*/
	//sj_free(spawn_list);
	//sj_free(level_json);
	
	memset(level, 0, sizeof(Level));
	slog("Level freed");
}

void level_init()
{
	if (!level)
		level = (Level*)gfc_allocate_array(sizeof(Level), 1);

	level_json = sj_load("json/level.json");
	if (!level_json)
		slog("level json not found.");

	slog("Level initialized.");
}

void clear_entities()
{
	int i;
	for (i = 0; i < get_max_entities(); i++)
	{
		if (!get_entities_list()[i]._inuse) continue;
		if (&get_entities_list()[i] == get_player()->ent) continue;
		entity_free(&get_entities_list()[i]);
		clear_npcs();
	}
}

void level_spawn_entities(SJson *spawnList)
{
	int i = 0, count = 0;
	SJson *spawn_json;
	TextWord *name;
	TextWord **args = NULL;
	Vector2D position, dimension;
	ItemType item_type;
	NPCType npc_type;
	FightStyle style;

	item_type = consumable;
	npc_type = Friendly;
	style = None;
	args = gfc_allocate_array(sizeof(TextWord),2);

	count = sj_array_get_count(spawnList);
	slog("count: %i", count);

	clear_entities();
	slog("Entities cleared");

	spawn_json = NULL;
	for (i = 0; i < count; i++)
	{
		vector2d_clear(position);
		spawn_json = sj_array_get_nth(spawnList, i);
		if (!spawn_json)continue;
		if (i == 0)
		{
			sj_value_as_vector2d(sj_object_get_value(spawn_json, "position_1"), &position);

			if (get_player()){
				get_player()->ent->position.x = position.x;
				get_player()->ent->position.y = position.y;
			}
			else
				player_spawn(position);
		}
		else
		{
			sj_value_as_vector2d(sj_object_get_value(spawn_json, "position"), &position);
			name = sj_get_string_value(sj_object_get_value(spawn_json, "name"));
			
			if (strstr(name, "exit") != NULL)
			{
				sj_value_as_vector2d(sj_object_get_value(spawn_json, "dimension"), &dimension);
				args[0] = sj_get_string_value(sj_object_get_value(spawn_json, "name"));
				args[1] = sj_get_string_value(sj_object_get_value(spawn_json, "location"));
			
				door_spawn(position, dimension, args);
				slog("Door spawned.");
			}
			else if (strstr(name, "friendly") != NULL || strstr(name, "nerd") != NULL || 
				strstr(name, "jock") != NULL || strstr(name, "prep") != NULL ||
				strstr(name, "goth") != NULL || strstr(name, "mobster") != NULL ||
				strstr(name, "shop") != NULL || strstr(name, "questgiver") != NULL)
			{
				sj_get_integer_value(sj_object_get_value(spawn_json, "type"), &npc_type);
				sj_get_integer_value(sj_object_get_value(spawn_json, "style"), &style);
				npc_spawn(npc_type, style, position);
				slog("NPC spawned.");
			}
			else
			{
				sj_get_integer_value(sj_object_get_value(spawn_json, "type"), &item_type);
				item_new(name,item_type, position);
				slog("item spawned");
			}
		}
	}
}

void level_load(char *name)
{
	SJson *level_info = sj_object_get_value(level_json, name);
	SJson *item;
	SJson *player_info;
	Vector2D position;
	position.x = 0;
	position.y = 0;
	if (spawn_list)
		spawn_list = NULL;
	spawn_list = sj_object_get_value(level_info, "spawn_list");
	if (!spawn_list)
		slog("Spawn list does not exist.");

	slog("Loading level %s", name);

	if (!level_info)
		slog("Level does not exist.");

	level->name = name;
	level->background = gf2d_sprite_load_image( sj_get_string_value(sj_object_get_value(level_info, "background")) );

	if (!level->background)
		slog("Could not find background image");

	level->position.x = 0;
	level->position.y = 0;

	sj_get_float_value(sj_object_get_value(level_info,"width"),&level->width);
	sj_get_float_value(sj_object_get_value(level_info, "height"), &level->height);

	level_spawn_entities(spawn_list);
	slog("Entities spawned.");
	//print_entities();
	//print_npc();
}

void level_draw()
{
	Vector2D offset, drawPosition;
	int i;
	if (!level)
	{
		slog("cannot draw level, NULL pointer provided");
		return;
	}
	offset = camera_get_offset();
	//draw the background first
	drawPosition.x = level->position.x;
	drawPosition.y = level->position.y;
	gf2d_sprite_draw_image(level->background, vector2d(drawPosition.x + offset.x, drawPosition.y + offset.y));
}

Level *get_level()
{
	return level;
}

Vector2D get_level_dimension()
{
	return vector2d(level->width, level->height);
}



/*eol@eof*/
