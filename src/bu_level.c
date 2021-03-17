
#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_config.h"

#include "bu_camera.h"
#include "bu_level.h"
#include "bu_door.h"
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

	sj_free(level_json);
	
	memset(level, 0, sizeof(Level));
	slog("Level freed.");
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

void level_spawn_entities(SJson *spawnList)
{
	int i = 0, count = 0;
	SJson *item;
	TextWord *name;
	Vector2D position;
	NPCType type;
	FightStyle style;

	count = sj_array_get_count(spawnList);
	slog("count: %i", count);
	for (i = 0; i < get_max_entities(); i++)
	{
		if (!get_entities_list()[i]._inuse) continue;
		if (&get_entities_list()[i] == get_player()->ent) continue;
		entity_free(&get_entities_list()[i]);
	}
	item = NULL;
	for (i = 0; i < count; i++)
	{
		vector2d_clear(position);
		item = sj_array_get_nth(spawnList, i);
		if (!item)continue;
		if (i == 0)
		{
			int str_value = sj_value_as_vector2d(sj_object_get_value(item, "position"), &position);
			slog("player pos is %i", str_value);
			if (get_player()){
				get_player()->ent->position.x = position.x;
				get_player()->ent->position.y = position.y;
			}
			else
				player_spawn(position);
		}
		else
		{
			int pos_value = sj_value_as_vector2d(sj_object_get_value(item, "position"), &position);
			slog("npc pos is %i", pos_value);
			name = sj_get_string_value(sj_object_get_value(item, "name"));
			slog("NPC name: %s", name);
			if (strstr(name, "exit") != NULL)
			{
				slog("Door spawn");
				slog("position: %f, %f", position.x, position.y);
				door_spawn(position, NULL);
				
			}
			else
			{
				sj_get_integer_value(sj_object_get_value(item, "type"), &type);
				sj_get_integer_value(sj_object_get_value(item, "style"), &style);

				npc_spawn(type, style, position);
				/* TODO: spawning with the position does not work all the time
							temp fix is setting the position like below*/
				get_npc()[i - 1].ent->position.x = position.x;
				get_npc()[i - 1].ent->position.y = position.y;
				slog("NPC ent name: %s",get_npc()[i - 1].ent->name);
			}
		}
	}


	slog("NPC 0 location: %f, %f",get_npc()[0].ent->position.x, get_npc()[0].ent->position.y);
	slog("NPC 1 location: %f, %f", get_npc()[1].ent->position.x, get_npc()[1].ent->position.y);
	slog("NPC 2 location: %f, %f", get_npc()[2].ent->position.x, get_npc()[2].ent->position.y);
}

void level_load(char *name)
{
	SJson *level_info = sj_object_get_value(level_json, name);
	SJson *item;
	SJson *player_info;
	Vector2D position;
	
	if (spawn_list)
		spawn_list = NULL;
	spawn_list = sj_object_get_value(level_info, "spawn_list");

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
}

void level_transition()
{

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
	
	
	//gf2d_sprite_draw(level->sprite, vector2d(0, 0), &scale, NULL, NULL, NULL, NULL, NULL);
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
