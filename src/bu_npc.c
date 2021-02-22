
#include <math.h>

#include "simple_json.h"
#include "simple_logger.h"

#include "gfc_types.h"

#include "bu_npc.h"

static NPC *npc = NULL;
static Uint8 npc_count = 0;
static SJson *npc_info = NULL;

void print_npc_stats();
void generate_npc_stats(NPCStats *stats, NPCType type, FightStyle style);

void npc_load_json()
{
	npc_info = sj_load("json/npc.json");
	if (!npc_info)
		slog("npc info null");
}

void npc_update(Entity *self)
{
	//self->position.x += 2 * cos(30 * GFC_DEGTORAD) *.06;
	//self->position.y -= 2 * sin(30 * GFC_DEGTORAD) *.05;
}

void npc_spawn(NPCType type, FightStyle style, Vector2D position)
{	
	if (!npc)
		npc = (NPC*)gfc_allocate_array(sizeof(NPC), 10);


	npc[npc_count].ent = entity_new();
	if (!npc[npc_count].ent)
	{
		slog("failed to create npc entity");
		slog_sync();
		return NULL;
	}

	npc[npc_count].ent->sprite = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);
	npc[npc_count].ent->frameRate = 0.1;
	npc[npc_count].ent->frameCount = 16;
	npc[npc_count].ent->update = npc_update;

	vector2d_copy(npc->ent->position, position);
	
	npc[npc_count].fightStyle = style;
	npc[npc_count].type = type;
	generate_npc_stats(&npc[npc_count].stats, type, style);
	npc[npc_count].isHostile = false;

	if (style != Friendly)
		npc[npc_count].isHostile = true;
	
	npc_count++;
}

void generate_npc_stats(NPCStats *stats, NPCType type, FightStyle style)
{
	SJson *npc_stats = NULL;

	if (!npc_info)
		npc_load_json();
	
	switch (type)
	{
		case Friendly:
			npc_stats = sj_object_get_value(npc_info, "friendly");
			slog("Spawning friendly");
			break;
		case Nerd:
			npc_stats = sj_object_get_value(npc_info, "nerd");
			slog("Spawning nerd");
			break;
		case Jock:
			npc_stats = sj_object_get_value(npc_info, "jock");
			slog("Spawning jock");
			break;
		case Preps:
			npc_stats = sj_object_get_value(npc_info, "preps");
			slog("Spawning preps");
			break;
		case Goths:
			npc_stats = sj_object_get_value(npc_info, "goth");
			slog("Spawning goth");
			break;
		case Mobster:
			npc_stats = sj_object_get_value(npc_info, "mobster");
			slog("Spawning mobster");
			break;
		case Shop:
			npc_stats = sj_object_get_value(npc_info, "shop");
			slog("Spawning shop");
			break;
		case Questgiver:
			npc_stats = sj_object_get_value(npc_info, "questgiver");
			slog("Spawning questgiver");
			break;
	}			

	sj_get_integer_value(sj_object_get_value(npc_stats, "level"),			&stats->level);
//	sj_get_integer_value(sj_object_get_value(npc_stats, "money"),			&stats->money);
	sj_get_integer_value(sj_object_get_value(npc_stats, "life"),			&stats->life);
	sj_get_integer_value(sj_object_get_value(npc_stats, "stamina"),			&stats->stamina);
	sj_get_integer_value(sj_object_get_value(npc_stats, "stamina_max"),		&stats->stamina_max);
	sj_get_integer_value(sj_object_get_value(npc_stats, "stamina_regen"),	&stats->stamina_regen);
	sj_get_integer_value(sj_object_get_value(npc_stats, "stamina_max"),		&stats->stamina_max);
	sj_get_integer_value(sj_object_get_value(npc_stats, "strength"),		&stats->strength);
	sj_get_integer_value(sj_object_get_value(npc_stats, "agility"),			&stats->agility);
	sj_get_integer_value(sj_object_get_value(npc_stats, "willpower"),		&stats->willpower);
	sj_get_integer_value(sj_object_get_value(npc_stats, "punch"),			&stats->punch);
	sj_get_integer_value(sj_object_get_value(npc_stats, "kick"),			&stats->kick);
	sj_get_integer_value(sj_object_get_value(npc_stats, "weapon"),			&stats->weapon);
	sj_get_integer_value(sj_object_get_value(npc_stats, "throwing"),		&stats->throwing);

	if (!npc_stats)
	{
		slog("Failed to load npc json data %s", sj_get_error());
		return;
	}
	if (style == Brawler)
	{
		stats->kick += 10;
		stats->punch += 10;
	}
	if (style == Weapon)
	{
		stats->weapon += 10;
		stats->throwing += 5;
	}
	if (style == Thrower)
	{
		stats->throwing += 10;
	}

	//print_npc_stats();
}

void print_npc_stats()
{
	slog("NPC stats:");
	slog("           Level: %i", npc[0].stats.level);
	slog("           Money: %s", npc[0].stats.money);

	slog("            Life: %i", npc[0].stats.life);
	slog("         Stamina: %i", npc[0].stats.stamina);
	slog("     Stamina Max: %i", npc[0].stats.stamina_max);
	slog("   Stamina Regen: %i", npc[0].stats.stamina_regen);

	slog("        Strength: %i", npc[0].stats.strength);
	slog("       Willpower: %i", npc[0].stats.willpower);
	slog("         Agility: %i", npc[0].stats.agility);
	slog("           Punch: %i", npc[0].stats.punch);
	slog("            Kick: %i", npc[0].stats.kick);
	slog("            Kick: %i", npc[0].stats.weapon);
	slog("        Throwing: %i", npc[0].stats.throwing);
}
/*eol@eof*/
