#ifndef __BU_NPC_H__
#define __BU_NPC_H__

#include "bu_entity.h"

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
	Brawler,
	Weapon,
	Thrower
}FightStyle;

typedef struct{
	char* name;

	Uint8 level;
	Uint8 money;

	Uint8 life;
	Uint8 stamina;
	Uint8 stamina_max;
	Uint8 stamina_regen;

	Uint8 strength;
	Uint8 agility;
	Uint8 willpower;

	Uint8 punch;
	Uint8 kick;
	Uint8 weapon;
	Uint8 throwing;

}NPCStats;

typedef struct
{
	NPCType type;
	FightStyle fightStyle;
	Entity *ent;
	NPCStats stats;
	Bool isHostile;
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
void npc_free(NPC *npc);

#endif
