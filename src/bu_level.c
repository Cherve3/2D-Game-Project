
#include "simple_logger.h"

#include "bu_camera.h"
#include "bu_level.h"

static Level *level = { 0 };
static Vector2D scale;

void level_free(Level *level)
{
	if (!level)return;

}

void level_load(char *name)
{
	slog("Loading level %s", name);
	if (!level)
		level = (Level*)gfc_allocate_array(sizeof(Level), 1);

	if (strcmp(name, "locker_room.png") == 0)
	{
		level->background = gf2d_sprite_load_image("images/backgrounds/locker_room.png");
		level->name = name;
	}
	if (strcmp(name, "school.png") == 0)
	{
		level->background = gf2d_sprite_load_image("images/backgrounds/school.png");
		level->name = name;
	}
	if (strcmp(name, "outside.png") == 0)
	{
		level->background = gf2d_sprite_load_image("images/backgrounds/outside.png");
		level->name = name;
	}
	if (strcmp(name, "shop.png") == 0)
	{
		level->background = gf2d_sprite_load_image("images/backgrounds/shop.png");
		level->name = name;
	}
	
	if (!level->background)
			slog("Could not find background image"); return;

	level->width = 1920;
	level->height = 1080;

	level->position.x = 0;
	level->position.y = 0;
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

/*eol@eof*/
