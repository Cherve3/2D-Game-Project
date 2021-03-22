#ifndef __BU_PLAYER_H__
#define __BU_PLAYER_H__

#include "bu_entity.h"
#include "bu_items.h"

typedef struct{
	char* name;

	int level;
	int money;

	int life;
	int life_max;
	int stamina;
	int stamina_max;
	int stamina_regen;

	int strength;
	int strength_max;
	int agility;
	int agility_max;
	int willpower;
	int willpower_max;

	int punch;
	int punch_max;
	int kick;
	int kick_max;
	int weapon;
	int weapon_max;
	int throwing;
	int throwing_max;

	Bool toggle_stats;
	Bool toggle_inventory;
	Bool pickup_item;
	Bool can_carry;
	Bool throw_item;

}PlayerStats;

typedef struct
{
	Bool IDLE;
	Bool WALK;
	Bool RUN;
	Bool ATTACK;
}PlayerState;

typedef struct
{
	Item head;
	Item torso;
	Item legs;
	Item feet;
}Equipment;

typedef struct
{
	Entity *ent;
	PlayerStats stats;
	PlayerState state;
	Uint8	player_number;
	Equipment equip;
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
 */
void player_free();

Rect get_player_collider();

Player *get_player();

void set_player_time(Uint32 time);

Uint32 get_player_time();

#endif
