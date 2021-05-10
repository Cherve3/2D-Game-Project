
#include "chipmunk/chipmunk_private.h"

#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_config.h"
#include "gf2d_draw.h"

#include "bu_camera.h"
#include "bu_level.h"
#include "bu_door.h"
#include "bu_items.h"
#include "bu_player.h"
#include "bu_npc.h"
#include "bu_shop.h"

static const char *LEVEL_JSON = "json/level.json";

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
	cpVect   gravity = { 0 };
	cpBody  *staticBody = NULL;
	cpShape *ground;

	//sj_enable_debug();
	if (!level)
		level = (Level*)gfc_allocate_array(sizeof(Level), 1);

	if (!level_json)
		level_json = sj_load(LEVEL_JSON);

	//slog("Error json: %s", sj_get_error());
	if (!level_json){
		slog("level json not found.");
		return;
	}

	level->space = cpSpaceNew();
	gravity = cpv(0, 50);
	//cpSpaceSetGravity(level->space, gravity);
	//cpSpaceSetIterations(level->space, 10);
	cpSpaceSetCollisionSlop(level->space, 0.1);
	cpSpaceUseSpatialHash(level->space, 200.0, 10);
	staticBody = cpSpaceGetStaticBody(level->space);

	// Horizontal ceiling
	staticBody = cpSpaceGetStaticBody(level->space);
	ground = cpSegmentShapeNew(staticBody, cpv(0.0, 10), cpv(1920.0, 10.0), 0);
	cpShapeSetFriction(ground, 1.0);
	cpSpaceAddShape(level->space, ground);
	//Vertical wall left
	ground = cpSegmentShapeNew(staticBody, cpv(10.0, 0.0), cpv(10.0, 1080.0), 0);
	cpShapeSetFriction(ground, 1.0);
	cpSpaceAddShape(level->space, ground);
	// Horizontal floor
	ground = cpSegmentShapeNew(staticBody, cpv(0.0, 390.0), cpv(1920.0, 390.0), 0);
	cpShapeSetFriction(ground, 1.0);
	cpSpaceAddShape(level->space, ground);
	// Vertical right
	ground = cpSegmentShapeNew(staticBody, cpv(650.0, 0.0), cpv(650.0, 1080.0), 0);
	cpShapeSetFriction(ground, 1.0);
	cpSpaceAddShape(level->space, ground);


	level->border_top = cpSegmentShapeNew(staticBody, cpv(0, 0), cpv(1920, 0), 0);
	cpShapeSetFriction(level->border_top, 1);
	cpSpaceAddShape(level->space, level->border_top);

	level->border_right = cpSegmentShapeNew(staticBody, cpv(1920, 0), cpv(1920, 1080), 0);
	cpShapeSetFriction(level->border_right, 1);
	cpSpaceAddShape(level->space, level->border_right);

	level->border_bottom = cpSegmentShapeNew(staticBody, cpv(0, 1080), cpv(1920, 1080), 0);
	cpShapeSetFriction(level->border_bottom, 1);
	cpSpaceAddShape(level->space, level->border_bottom);

	level->border_left = cpSegmentShapeNew(staticBody, cpv(0, 0), cpv(0, 1080), 0);
	cpShapeSetFriction(level->border_left, 1);
	cpSpaceAddShape(level->space, level->border_left);

	

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
	Vector2D   position;
	cpVect    cp_position;
	Vector2D dimension;
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

	shop_init();

	spawn_json = NULL;
	for (i = 0; i < count; i++)
	{
		vector2d_clear(position);
		spawn_json = sj_array_get_nth(spawnList, i);
		if (!spawn_json)continue;
		if (i == 0)
		{
			sj_value_as_vector2d(sj_object_get_value(spawn_json, "position_1"), &position);
			slog("Position::: %f, %f", position.x, position.y);
			if (get_player()){
				get_player()->ent->position.x = position.x;
				get_player()->ent->position.y = position.y;
			}
			else
			{
				cp_position.x = position.x;
				cp_position.y = position.y;
				player_spawn(level->space, cp_position);
			}
		}
		else
		{
			sj_value_as_vector2d(sj_object_get_value(spawn_json, "position"), &position);
			name = sj_get_string_value(sj_object_get_value(spawn_json, "name"));
			cp_position.x = position.x;
			cp_position.y = position.y;

			if (strstr(name, "exit") != NULL)
			{
				sj_value_as_vector2d(sj_object_get_value(spawn_json, "dimension"), &dimension);
				args[0] = sj_get_string_value(sj_object_get_value(spawn_json, "name"));
				args[1] = sj_get_string_value(sj_object_get_value(spawn_json, "location"));
			
				door_spawn(cp_position, dimension, args);
				slog("Door spawned.");
			}
			else if (strstr(name, "friendly") != NULL || strstr(name, "nerd") != NULL || 
				strstr(name, "jock") != NULL || strstr(name, "prep") != NULL ||
				strstr(name, "goth") != NULL || strstr(name, "mobster") != NULL ||
				strstr(name, "shop") != NULL || strstr(name, "questgiver") != NULL)
			{
				sj_get_integer_value(sj_object_get_value(spawn_json, "type"), &npc_type);
				sj_get_integer_value(sj_object_get_value(spawn_json, "style"), &style);
				npc_spawn(npc_type, style, get_level()->space, cp_position);
				slog("NPC spawned.");
			}
			else
			{
				sj_get_integer_value(sj_object_get_value(spawn_json, "type"), &item_type);
				item_new(name,item_type, cp_position);
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
	cpVect position;
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
	//cpFloat timeStep = 1.0 / 60.0;
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

	Vector2D start, end;
	start.x = 0.0+camera_get_offset().x;
	start.y = 10.0 + camera_get_offset().y;
	end.x = 1920.0 + camera_get_offset().x;
	end.y = 10.0 + camera_get_offset().y;
	gf2d_draw_line(start, end, vector4d(255, 255, 255, 255));
	start.x = 10.0 + camera_get_offset().x;
	start.y = 0.0 + camera_get_offset().y;
	end.x = 10.0 + camera_get_offset().x;
	end.y = 1080.0 + camera_get_offset().y;
	gf2d_draw_line(start, end, vector4d(255, 255, 255, 255));
	start.x = 0.0 + camera_get_offset().x;
	start.y = 390.0 + camera_get_offset().y;
	end.x = 1920.0 + camera_get_offset().x;
	end.y = 390.0 + camera_get_offset().y;
	gf2d_draw_line(start, end, vector4d(255, 255, 255, 255));
	start.x = 650.0 + camera_get_offset().x;
	start.y = 0.0 + camera_get_offset().y;
	end.x = 650.0 + camera_get_offset().x;
	end.y = 1080.0 + camera_get_offset().y;
	gf2d_draw_line(start, end, vector4d(255, 255, 255, 255));
	//printf(
	//	"LEVEL ballBody is at (%5.2f, %5.2f). It's velocity is (%5.2f, %5.2f)\n",
	//	pos.x, pos.y, vel.x, vel.y
	//);
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
