#ifndef __BU_NPC_H__
#define __BU_NPC_H__

#include "bu_entity.h"
#include "bu_items.h"

typedef enum {
	Friendly,
	Nerd,
	Jock,
	Preps,
	Goths,
	Mobster,
	Shop,
	Questgiver
}NPCType;

typedef enum {
	None,
	Brawler,
	Weapon,
	Thrower
}FightStyle;

typedef struct{
	TextWord *name;

	int level;
	int money;

	int life;
	int stamina;
	int stamina_max;
	int stamina_regen;

	int strength;
	int agility;
	int willpower;

	int punch;
	int kick;
	int weapon;
	int throwing;

}NPCStats;

typedef struct
{
	Bool IDLE;
	Bool WALK;
	Bool RUN;
	Bool ATTACK;
}NPCState;

typedef struct
{
	NPCType type;
	FightStyle fightStyle;
	Entity *ent;
	NPCStats stats;
	NPCState state;
	Bool isHostile;
	Item weapon;
}NPC;

/**
 * @brief Updates the npc
 * @param self A pointer to the entity of the npc being updated
 */
void npc_update(Entity *self);

/**
 * @brief Spawns a new npc entity
 * @param type The npc type value
 * @param position The initial position of the npc entity
 * @return A pointer to the npc entity
 */
void npc_spawn(NPCType type, FightStyle style, Vector2D position);

/**
* @brief Frees all the npc data
* @param npc A pointer to the npc struct data to free
*/
void npc_free_all();

void clear_npcs();

NPC *get_npc();

void print_npc();

#endif
