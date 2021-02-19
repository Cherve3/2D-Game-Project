
#include "simple_logger.h"

#include "bu_player.h"

void player_update(Entity *self)
{
	Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (!self)return;

	if (keys[SDL_SCANCODE_D])
	{
		//slog("Moving left?");
		vector2d_add(self->position, self->position, vector2d(1, 0));
	}
	if (keys[SDL_SCANCODE_A])
	{
		//slog("Moving right?");
		vector2d_add(self->position, self->position, vector2d(-1, 0));
	}
	if (keys[SDL_SCANCODE_W])
	{
		//slog("Moving up?");
		vector2d_add(self->position, self->position, vector2d(0, -1));
	}
	if (keys[SDL_SCANCODE_S])
	{
		//slog("Moving down?");
		vector2d_add(self->position, self->position, vector2d(0, 1));
	}
	
}

Entity *player_spawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("failed to create entity for the player");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = player_update;
	return ent;
}



/*eol@eof*/
