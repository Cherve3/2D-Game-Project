#ifndef __BU_COMBAT_H__
#define __BU_COMBAT_H__

#include "simple_logger.h"


#include "bu_player.h"
#include "bu_npc.h"

void attack_list_init();

void punch(Player *player);

void kick(Player *player);

void throw_weapon(Player *player);

void weapon_attack_left(Player *player);

void weapon_attack_right(Player *player);

void draw_attacks();

#endif
