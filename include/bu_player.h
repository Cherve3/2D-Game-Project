#ifndef __BU_PLAYER_H__
#define __BU_PLAYER_H__

#include "bu_entity.h"

typedef struct{
	char* name;

	Uint8 level;
	Uint8 money;

	Uint8 life;
	Uint8 life_max;
	Uint8 stamina;
	Uint8 stamina_max;
	Uint8 stamina_regen;

	Uint8 strength;
	Uint8 strength_max;
	Uint8 agility;
	Uint8 agility_max;
	Uint8 willpower;
	Uint8 willpower_max;

	Uint8 punch;
	Uint8 punch_max;
	Uint8 kick;
	Uint8 kick_max;
	Uint8 weapon;
	Uint8 weapon_max;
	Uint8 throwing;
	Uint8 throwing_max;

}PlayerStats;

typedef struct
{
	Entity *ent;
	PlayerStats stats;
	Uint8	player_number;
}Player;

/**
 * @brief Spawns a player.
 * @param position The x and y posiiton of the player on the screen.
 */
Player *player_spawn(Vector2D position);

/**
* @brief Update the player.
* @param *self A pointer to the player entity. 
*/
void player_update(Entity *self);

/**
 * @brief Free the player from memory.
 * @param *player A pointer to the player struct.
 */
void player_free(Player *player);

#endif
