
#include "gf2d_draw.h"
#include "gfc_audio.h"

#include "bu_shapes.h"
#include "bu_combat.h"
#include "bu_collision.h"
#include "bu_camera.h"

typedef struct
{
	Rect    collider;
	int     damage;
	float   duration;
	Sound **audio;
}Attack;

static List* attack_list = { 0 };

void draw_attack(Rect attack);

Attack *new_attack()
{
	Attack *attack = gfc_allocate_array(sizeof(Attack), 1);;
	attack->audio = gfc_allocate_array(sizeof(Sound), 2);

	return attack;
}

void attack_list_init()
{
	Attack *punch        = new_attack();
	Attack *kick         = new_attack();
	Attack *weapon_left  = new_attack();
	Attack *weapon_right = new_attack();
	attack_list = gfc_list_new();
	
	punch->damage = 0;
	punch->audio[0] = gfc_sound_load("audio/174435__drminky__punch-miss-whoosh.wav", 1.0, 1);
	punch->audio[1] = gfc_sound_load("audio/566435__merrick079__punch2.wav", 1.0, 1);
	kick->damage = 0;
	kick->audio[0] = gfc_sound_load("audio/174435__drminky__punch-miss-whoosh.wav", 1.0, 1);
	kick->audio[1] = gfc_sound_load("audio/455917__bolkmar__kick-hit.wav", 1.0, 1);
	
	gfc_list_append(attack_list, punch);
	gfc_list_append(attack_list, kick);
}

void punch(Player *player)
{
	Attack* punch = (Attack*)gfc_list_get_nth(attack_list,0);

	slog("PUNCH: %i", punch->damage);
	punch->collider.x = player->ent->position.x + player->ent->rect_collider.w + camera_get_offset().x + camera_get_position().x;;
	punch->collider.y = player->ent->position.y + 10 + +camera_get_offset().y + camera_get_position().y;
	punch->collider.w = player->ent->rect_collider.w;
	punch->collider.h = player->ent->rect_collider.h;

	punch->damage = 1 + (player->stats.punch * player->stats.strength) * 0.5;
	punch->duration = 1.0;
	
	draw_attack(punch->collider);

	if (attack_collision(punch->collider, punch->damage))
	{
		gfc_sound_play(punch->audio[1], 0, 0.3, 1, -1);
		slog("Punch Collided");
	}
	else
		gfc_sound_play(punch->audio[0], 0, 1, 1, -1);
	
}

void kick(Player *player)
{
	Attack *kick = (Attack*)gfc_list_get_nth(attack_list, 1);;
	kick->collider.x = player->ent->position.x + player->ent->rect_collider.w + camera_get_offset().x + camera_get_position().x;
	kick->collider.y = player->ent->position.y + 10 + +camera_get_offset().y + camera_get_position().y;
	kick->collider.h = 50;
	kick->collider.w = 50;

	kick->damage = 2 + (player->stats.kick * player->stats.strength) * 0.5;
	kick->duration = 1.0;

	draw_attack(kick->collider);

	if (attack_collision(kick->collider, kick->damage))
	{
		slog("Kick Collided");
		gfc_sound_play(kick->audio[1], 0, 0.3, 1, -1);
	}
	else
		gfc_sound_play(kick->audio[0], 0, 1, 1, -1);
}

void throw_weapon(Player *player)
{
	if (!player->stats.can_carry)
		slog("Throw weapon");
}

void weapon_attack_left(Player *player)
{
	Attack weapon_left;
	weapon_left.collider.x = player->ent->position.x + player->ent->rect_collider.w + camera_get_offset().x + camera_get_position().x;
	weapon_left.collider.y = player->ent->position.y + 10 + +camera_get_offset().y + camera_get_position().y;
	weapon_left.collider.h = 70;
	weapon_left.collider.w = 70;

	weapon_left.damage = 1 + (player->stats.throwing * player->stats.strength) * 0.5;
	weapon_left.duration = 1.0;

	draw_attack(weapon_left.collider);

	if (attack_collision(weapon_left.collider, weapon_left.damage))
		slog("Weapon attack 1 Collided");

}

void weapon_attack_right(Player *player)
{
	Attack weapon_right;
	weapon_right.collider.x = player->ent->position.x + player->ent->rect_collider.w + camera_get_offset().x + camera_get_position().x;
	weapon_right.collider.y = player->ent->position.y + 10 + +camera_get_offset().y + camera_get_position().y;
	weapon_right.collider.h = 100;
	weapon_right.collider.w = 100;

	weapon_right.damage = 2 + (player->stats.throwing * player->stats.strength) * 0.5;
	weapon_right.duration = 1.0;

	draw_attack(weapon_right.collider);

	if (attack_collision(weapon_right.collider, weapon_right.damage))
		slog("Weapon attack 2 Collided");
}

void draw_attack(Rect attack)
{
	Vector4D vec;
	vec.w = 255;
	vec.x = 0;
	vec.y = 0;
	vec.z = 0;

	gf2d_draw_rect(gfc_sdl_rect(
		attack.x,
		attack.y,
		attack.w,
		attack.h),
		vec);
}
/*eol@eof*/
