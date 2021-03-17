
#include "simple_json.h"
#include "simple_logger.h"

#include "bu_camera.h"
#include "bu_player.h"
#include "bu_level.h"

static Player *player = { 0 };
static Uint8 player_count = 0;
static SJson *player_info = NULL;

void print_player_stats();

void player_free()
{
	if (!player) return;
	entity_free(player->ent);
	player->player_number = 0;
	memset(&player->stats, 0, sizeof(PlayerStats));
	memset(&player->state, 0, sizeof(PlayerState));
	memset(player, 0, sizeof(Player));
	sj_free(player_info);
	slog("Player freed");
}

void check_player_bounds(Entity *self)
{
	if (self->rect_collider.x < camera_get_player_bounds().x)
		camera_move(vector2d(-2 - self->velocity, 0));

	if (self->rect_collider.x + self->rect_collider.w >(camera_get_player_bounds().x + camera_get_player_bounds().w))
	{
		camera_move(vector2d(2 + self->velocity, 0));
		player->ent->rect_collider.x += 2 + self->velocity;
	}
	if (self->rect_collider.y < camera_get_player_bounds().y)
		camera_move(vector2d(0, -2 - self->velocity));

	if (self->rect_collider.y + self->rect_collider.h > (camera_get_player_bounds().y + camera_get_player_bounds().h))
		camera_move(vector2d(0, 2 + self->velocity));
}

void player_think(Entity* self)
{
	Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (!self)return;

	if (keys[SDL_SCANCODE_D])
	{
		player->state.IDLE = false;
		//slog("Moving left?");
		if (player->state.RUN)
			vector2d_add(self->position, self->position, vector2d(2 + self->velocity, 0));
		else
		{
			vector2d_add(self->position, self->position, vector2d(2, 0));
			player->state.WALK = true;
		}
			
	}
	if (keys[SDL_SCANCODE_A])
	{
		player->state.IDLE = false;
		//slog("Moving right?");
		if (player->state.RUN)
			vector2d_add(self->position, self->position, vector2d(-2 - self->velocity, 0));
		else
		{
			vector2d_add(self->position, self->position, vector2d(-2, 0));
			player->state.WALK = true;
		}
	}
	if (keys[SDL_SCANCODE_W])
	{
		player->state.IDLE = false;
		//slog("Moving up?");
		if (player->state.RUN)
			vector2d_add(self->position, self->position, vector2d(0, -2 - self->velocity));
		else
		{
			vector2d_add(self->position, self->position, vector2d(0, -2));
			player->state.WALK = true;
		}
	}
	if (keys[SDL_SCANCODE_S])
	{
		player->state.IDLE = false;
		//slog("Moving down?");
		if (player->state.RUN)
			vector2d_add(self->position, self->position, vector2d(0, 2 + self->velocity));
		else
		{
			vector2d_add(self->position, self->position, vector2d(0, 2));
			player->state.WALK = true;
		}
	}

	if (!keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_W])
		player->state.IDLE = true;

	if (keys[SDL_SCANCODE_LSHIFT])
	{
		//slog("Running?");
		player->state.WALK = false;
		player->state.RUN = true;
		self->velocity = 2.0;
	}
	else
	{
		player->state.RUN = false;
		self->velocity = 0.0;
	}
}

void player_update(Entity *self)
{
	if (self->position.x < 0)
	{
		self->position.x = 0;
	}
	if (self->position.y < 0)
	{
		self->position.y = 0;
	}

	//slog("xp: %f, yp: %f", self->position.x, self->position.y);
	//slog("xr: %f, yr: %f", self->rect_collider.x, self->rect_collider.y);

	if (self->position.x > get_level_dimension().x - self->rect_collider.w)
	{
		self->position.x = get_level_dimension().x - self->rect_collider.w;

	}
	if (self->position.y > get_level_dimension().y - self->rect_collider.h)
	{
		self->position.y = get_level_dimension().y - self->rect_collider.h;
	}

	self->rect_collider.x = self->position.x;
	self->rect_collider.y = self->position.y;
	check_player_bounds(self);

}

void player_load_json()
{
	player_info = sj_load("json/player.json");
	if (!player_info)
		slog("player info null");
}

void generate_player_stats(PlayerStats *stats)
{
	if (!player_info)
		player_load_json();
	
	stats->name = sj_get_string_value(sj_object_get_value(player_info, "name"));
	sj_get_integer_value(sj_object_get_value(player_info, "money"), &stats->money);
	sj_get_integer_value(sj_object_get_value(player_info, "level"), &stats->level);
	sj_get_integer_value(sj_object_get_value(player_info, "life"), &stats->life);
	sj_get_integer_value(sj_object_get_value(player_info, "life_max"), &stats->life_max);
	sj_get_integer_value(sj_object_get_value(player_info, "stamina"), &stats->stamina);
	sj_get_integer_value(sj_object_get_value(player_info, "stamina_max"), &stats->stamina_max);
	sj_get_integer_value(sj_object_get_value(player_info, "stamina_regen"), &stats->stamina_regen);
	sj_get_integer_value(sj_object_get_value(player_info, "stamina_max"), &stats->stamina_max);
	sj_get_integer_value(sj_object_get_value(player_info, "strength"), &stats->strength);
	sj_get_integer_value(sj_object_get_value(player_info, "strength_max"), &stats->strength_max);
	sj_get_integer_value(sj_object_get_value(player_info, "agility"), &stats->agility);
	sj_get_integer_value(sj_object_get_value(player_info, "agility_max"), &stats->agility_max);
	sj_get_integer_value(sj_object_get_value(player_info, "willpower"), &stats->willpower);
	sj_get_integer_value(sj_object_get_value(player_info, "willpower_max"), &stats->willpower_max);
	sj_get_integer_value(sj_object_get_value(player_info, "punch"), &stats->punch);
	sj_get_integer_value(sj_object_get_value(player_info, "punch_max"), &stats->punch_max);
	sj_get_integer_value(sj_object_get_value(player_info, "kick"), &stats->kick);
	sj_get_integer_value(sj_object_get_value(player_info, "kick_max"), &stats->kick_max);
	sj_get_integer_value(sj_object_get_value(player_info, "weapon"), &stats->weapon);
	sj_get_integer_value(sj_object_get_value(player_info, "weapon_max"), &stats->weapon_max);
	sj_get_integer_value(sj_object_get_value(player_info, "throwing"), &stats->throwing);
	sj_get_integer_value(sj_object_get_value(player_info, "throwing_max"), &stats->throwing_max);

	//print_player_stats();
}

void print_player_stats()
{
	slog("Player stats:");
	slog("     Player name: %s", player[0].stats.name);
	slog("           Level: %i", player[0].stats.level);
	slog("           Money: %s", player[0].stats.money);

	slog("            Life: %i", player[0].stats.life);
	slog("        Life Max: %i", player[0].stats.life_max);
	slog("         Stamina: %i", player[0].stats.stamina);
	slog("     Stamina Max: %i", player[0].stats.stamina_max);
	slog("   Stamina Regen: %i", player[0].stats.stamina_regen);

	slog("        Strength: %i", player[0].stats.strength);
	slog("    Strength Max: %i", player[0].stats.strength_max);
	slog("       Willpower: %i", player[0].stats.willpower);
	slog("   Willpower Max: %i", player[0].stats.willpower_max);
	slog("         Agility: %i", player[0].stats.agility);
	slog("     Agility Max: %i", player[0].stats.agility_max);
	slog("           Punch: %i", player[0].stats.punch);
	slog("       Punch Max: %i", player[0].stats.punch_max);
	slog("            Kick: %i", player[0].stats.kick);
	slog("        Kick Max: %i", player[0].stats.kick_max);
	slog("          Weapon: %i", player[0].stats.weapon);
	slog("      Weapon Max: %i", player[0].stats.weapon_max);
	slog("        Throwing: %i", player[0].stats.throwing);
	slog("    Throwing Max: %i", player[0].stats.throwing_max);
}

Rect get_player_collider()
{
	return player->ent->rect_collider;
}

Player *get_player()
{
	return player;
}

Player *player_spawn(Vector2D position)
{
	if (!player)
		player = (Player*)gfc_allocate_array(sizeof(Player), 2);

	if (player && player_count < 2)
	{
		player_count++;
		player->ent = entity_new();
		if (!player->ent)
		{
			slog("failed to create entity for the player");
			return NULL;
		}
		player->ent->name = "Player 1";
		player->ent->sprite = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);
		vector2d_copy(player->ent->position, position);
		player->ent->frameRate = 0.1;
		player->ent->frameCount = 16;
		slog("position player: %f, %f", position.x, position.y);
		player->ent->rect_collider.x = position.x;
		player->ent->rect_collider.y = position.y;
		slog("player collider: %f, %f", player->ent->rect_collider.x, player->ent->rect_collider.y);
		player->ent->rect_collider.w = 100;
		player->ent->rect_collider.h = 100;
		player->ent->update = player_update;
		player->ent->think = player_think;
		player->player_number = player_count;
		generate_player_stats(&player->stats);
		slog("Player %i spawning...", player->player_number);
	}
	else
		slog("Warning: There are already two players active"); return;

	return player;
}

/*eol@eof*/
