
#include <math.h>

#include "chipmunk/chipmunk_private.h"

#include "simple_json.h"
#include "simple_logger.h"

#include "gfc_types.h"

#include "gf2d_sprite.h"

#include "bu_level.h"
#include "bu_money.h"
#include "bu_npc.h"

static NPC *npc = NULL;
static Uint32 npc_count = 0;
static SJson *npc_info = NULL;
static SJson *name_list = NULL;

void print_npc_stats(Uint32 num);
void generate_npc_stats(NPCStats *stats, NPCType type, FightStyle style);
void set_npc_state(NPC* npc, void (*state)());
void *patrol(NPC *npc);
void* idle(NPC* npc);

void npc_load_json()
{
	if (npc_info)
		slog("NPC info already loaded.");
	else
		npc_info = sj_load("json/npc.json");

	if (name_list)
		slog("NPC names already loaded.");
	else
		name_list = sj_load("json/names.json");

	if (!npc_info)
		slog("npc info null");
	
	if (!name_list)
		slog("name list null");
}

void npc_free_all()
{
	int i;
	if (!npc) return;
	for (i = 0; i < npc_count; i++){
		entity_free(npc[i].ent);
		npc[i].fightStyle = 0;
		npc[i].isHostile = 0;
		npc[i].type = 0;
		memset(&npc[i].stats, 0, sizeof(NPCStats));
	}
	
	memset(npc, 0, sizeof(NPC)*npc_count);
	npc_count = 0;
	sj_free(npc_info);
	sj_free(name_list);
	slog("NPCs freed");
}

void npc_free(NPC *self)
{
    self->fightStyle = 0;
	self->isHostile = 0;
	self->type = 0;
	memset(&self->stats, 0, sizeof(NPCStats));
}

void npc_animation(Entity *self)
{
	NPC *npc;
	if (!self) return;
	npc = (NPC*)self->data;
	//TODO: Replace with npc animations not player animations
	if (npc->state.IDLE){
		npc->ent->frameCount = 12;
		npc->ent->sprite = gf2d_sprite_load_all("images/player/player_idle.png", 60, 80, 12);
		npc->ent->rect_collider.w = 60;
		npc->ent->rect_collider.h = 80;
	}
	if (npc->state.WALK){
		npc->ent->frameCount = 12;
		npc->ent->sprite = gf2d_sprite_load_all("images/player/player_walk.png", 60, 80, 12);
		npc->ent->rect_collider.w = 60;
		npc->ent->rect_collider.h = 80;
	}
	if (npc->state.RUN){
		npc->ent->frameCount = 15;
		npc->ent->sprite = gf2d_sprite_load_all("images/player/player_run.png", 78, 81, 15);
		npc->ent->rect_collider.w = 78;
		npc->ent->rect_collider.h = 81;
	}
	if (npc->state.ATTACK)
	{
		npc->ent->frameCount = 6;
		npc->ent->sprite = gf2d_sprite_load_all("images/player/player_right_punch.png", 80, 80, 6);
		npc->ent->rect_collider.w = 80;
		npc->ent->rect_collider.h = 80;
	}
	/*
	if (npc->state.ATTACK)
	{
		npc->ent->frameCount = 6;
		npc->ent->sprite = gf2d_sprite_load_all("images/player/player_right_kick.png", 90, 80, 6);
		npc->ent->rect_collider.w = 90;
		npc->ent->rect_collider.h = 80;
	}*/
}

void npc_update(Entity *self)
{
	NPC *npc;
	int money;
	if (!self) return;
	//self->position.x += 2 * cos(30 * GFC_DEGTORAD) *.06;
	//self->position.y -= 2 * sin(30 * GFC_DEGTORAD) *.05;
	npc = (NPC*)self->data;
	entity_collision_check(self);
	money = npc->stats.money;

	self->position.x = self->body->p.x;
	self->position.y = self->body->p.y;
	self->rect_collider.x = self->position.x;
	self->rect_collider.y = self->position.y;

	npc_animation(self);
	
	update_state(npc->fsm);

	if (npc->stats.life == 0)
	{
		spawn_money(money, self->position);
		entity_free(self);
		npc_free(npc);
		
	}
}

void clear_npcs()
{
	int i;
	for (i = 0; i < npc_count; i++)
	{
		slog("NPC cleared: %s", npc[i].stats.name);
		npc_free(&npc[i]);
	}
	npc_count = 0;
}

void npc_spawn(NPCType type, FightStyle style, cpSpace* space, cpVect position)
{	
	char ent_name[16];
	cpShape *shape;
	cpShapeFilter filter;
	slog("Spawning NPC...");
	if (!npc)
		npc = (NPC*)gfc_allocate_array(sizeof(NPC), 10);

	if (!name_list)
		npc_load_json();

	npc[npc_count].ent = entity_new();
	if (!npc[npc_count].ent)
	{
		slog("failed to create npc entity");
		return;
	}
	
	npc[npc_count].ent->name = "NPC";
	npc[npc_count].ent->sprite = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);
	npc[npc_count].ent->frameRate = 0.1;
	npc[npc_count].ent->frameCount = 16;
	npc[npc_count].ent->update = npc_update;
	npc[npc_count].ent->_inuse = 1;
	npc[npc_count].ent->data = (void*)&npc[npc_count];

	npc[npc_count].ent->position.x = position.x;
	npc[npc_count].ent->position.y = position.y;
	npc[npc_count].ent->rect_collider.x = position.x;
	npc[npc_count].ent->rect_collider.y = position.y;
	npc[npc_count].ent->rect_collider.w = 60;
	npc[npc_count].ent->rect_collider.h = 80;
	npc[npc_count].ent->body = cpBodyNew(1.0, INFINITY);
	cpSpaceAddBody(space, npc[npc_count].ent->body);
	cpBodySetPosition(npc[npc_count].ent->body, npc[npc_count].ent->position);
	slog("NPC location: %f, %f", npc[npc_count].ent->position.x, npc[npc_count].ent->position.y);

	npc[npc_count].fightStyle	= style;
	npc[npc_count].type			= type;
	generate_npc_stats(&npc[npc_count].stats, type, style);
	npc[npc_count].isHostile	= false;
	npc[npc_count].state.ATTACK = false;
	npc[npc_count].state.IDLE	= true;
	npc[npc_count].state.RUN	= false;
	npc[npc_count].state.WALK	= false;
	
	npc[npc_count].base		= gf2d_sprite_load_image("images/ui/base_bar_npc.png");
	npc[npc_count].health	= gf2d_sprite_load_image("images/ui/health_bar.png");

	if (type != Friendly || type != Shop)
	{
		npc[npc_count].isHostile = true;
		npc[npc_count].fsm = new_FSM();
		npc[npc_count].fsm->set_state = set_npc_state;
	}

	filter.categories = npc_f;
	filter.mask = space_f || world_f || player_f;

	shape = cpBoxShapeNew2(npc[npc_count].ent->body, cpBBNew(-0.1, -1.0, 50.0, 70.0), 0.2);
	cpShapeSetFilter(shape, filter);
	cpShapeSetFriction(shape, 1.0);
	cpSpaceAddShape(space, shape);
	npc[npc_count].ent->shape = shape;

	npc_count++;
}

void set_npc_state(NPC *npc, void (*state)())
{
	npc->fsm->set_state(state);
}

void* idle(NPC* npc)
{
	int i;
	if (!npc) return;
	npc->state.IDLE = true;
	npc->state.WALK = false;
	npc->state.RUN = false;
	npc->ent->body->v.x = 0.0;
	npc->ent->body->v.y = 0.0;
	npc->fsm->set_state(npc, patrol(npc));
}


void* patrol(NPC* npc)
{
	cpVect destination;
	cpVect interpolate;
	if (!npc) return;
	npc->state.IDLE = false;
	npc->state.WALK = true;
	destination.x = (int)(gfc_random(1920) * 1000);
	destination.y = (int)(gfc_random(1080) * 1000);
	interpolate = cpvlerpconst(npc->ent->body->p, destination, cpvdist(npc->ent->body->p, destination));
	slog("X: %f, Y: %f", destination.x, destination.y);
	slog("nX: %f, nY: %f", npc->ent->body->p.x, npc->ent->body->p.y);

	if (npc->ent->body->p.x != destination.x && npc->ent->body->p.y != destination.y)
		cpBodySetVelocity(npc->ent->body, interpolate);
	else
		npc->fsm->set_state(npc, idle(npc));
}

void generate_npc_stats(NPCStats *stats, NPCType type, FightStyle style)
{
	SJson *npc_stats = NULL;
	SJson *names = NULL;

	int random = 0;

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
			slog("Spawning prep");
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


	names = sj_object_get_value(name_list, "names");

	random = (int)(gfc_random(150) * 100);

	stats->name = sj_get_string_value(sj_array_get_nth(names, random));
	sj_get_integer_value(sj_object_get_value(npc_stats, "level"),			&stats->level);
	sj_get_integer_value(sj_object_get_value(npc_stats, "money"),			&stats->money);
	sj_get_integer_value(sj_object_get_value(npc_stats, "life"),			&stats->life);
	sj_get_integer_value(sj_object_get_value(npc_stats, "life_max"),		&stats->life_max);
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
	slog("", stats->life);
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

	print_npc_stats(npc_count);
}

void print_npc_stats(Uint32 num)
{
	
	slog("NPC stats:");
	slog("           Level: %i", npc[num].stats.level);
	slog("           Money: %i", npc[num].stats.money);

	slog("            Life: %i", npc[num].stats.life);
	slog("         Stamina: %i", npc[num].stats.stamina);
	slog("     Stamina Max: %i", npc[num].stats.stamina_max);
	slog("   Stamina Regen: %i", npc[num].stats.stamina_regen);

	slog("        Strength: %i", npc[num].stats.strength);
	slog("       Willpower: %i", npc[num].stats.willpower);
	slog("         Agility: %i", npc[num].stats.agility);
	slog("           Punch: %i", npc[num].stats.punch);
	slog("            Kick: %i", npc[num].stats.kick);
	slog("          Weapon: %i", npc[num].stats.weapon);
	slog("        Throwing: %i", npc[num].stats.throwing);
}

NPC *get_npc()
{
	return npc;
}

Uint32 get_npc_count()
{
	return npc_count;
}

void print_npc()
{
	int i;
	for (i = 0; i < npc_count; i++)
	{
		slog("NPC: %s", npc[i].stats.name);
		slog("    Location:  %f, %f", npc[i].ent->position.x, npc[i].ent->position.y);
		slog("    Dimension: %f, %f", npc[i].ent->rect_collider.w, npc[i].ent->rect_collider.h);
		slog("    Inuse: %i\n", npc[i].ent->_inuse);
	}
}

/*eol@eof*/
