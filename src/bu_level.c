
#include "simple_logger.h"

#include "bu_level.h"

static Level *level = { 0 };
static Vector2D scale;

void level_free(char *name)
{

}

void level_load(char *name)
{

	char filename[50];
	scale = vector2d(0.5, 0.5);

	slog("Loading level %s", name);
	if (!level)
		level = gfc_allocate_array(sizeof(Level), 1);

	slog("Level: %s", name);
	strcpy(filename, "images/backgrounds/");
	strcat(filename, name);
	slog("Filename: %s", filename);

	if (strcmp(name, "locker_room.png") == 0)
	{
		level->sprite = gf2d_sprite_load_image("images/backgrounds/locker_room.png");
		level->name = name;
		
		if (!level->sprite)
			slog("Could not find background image"); return;
	}
}

void level_draw()
{
	gf2d_sprite_draw(level->sprite, vector2d(0, 0), &scale, NULL, NULL, NULL, NULL, NULL);
}

Level *get_level()
{
	return level;
}

/*eol@eof*/
