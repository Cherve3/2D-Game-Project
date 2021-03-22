
#include "bu_shapes.h"

#include "bu_combat.h"
#include "bu_collision.h"
#include "gf2d_draw.h"
#include "bu_camera.h"

typedef struct
{
	Rect collider;
	int damage;
	float duration;

}Attack;

void draw_attack(Rect attack);

void punch(Player *player)
{
	Attack punch;
	punch.collider.x = player->ent->position.x + player->ent->rect_collider.w + camera_get_offset().x +camera_get_position().x;
	punch.collider.y = player->ent->position.y + 10 + +camera_get_offset().y + camera_get_position().y;
	punch.collider.h = 50;
	punch.collider.w = 50;
	
	punch.damage = 1 + (player->stats.punch * player->stats.strength) * 0.5;
	punch.duration = 1.0;

	draw_attack(punch.collider);

	if (attack_collision(punch.collider))
		slog("Punch Collided");
	
	
}

void kick(Player *player)
{
	Attack kick;
	kick.collider.x = player->ent->position.x + player->ent->rect_collider.w + camera_get_offset().x + camera_get_position().x;
	kick.collider.y = player->ent->position.y + 10 + +camera_get_offset().y + camera_get_position().y;
	kick.collider.h = 50;
	kick.collider.w = 50;

	kick.damage = 1 + (player->stats.kick * player->stats.strength) * 0.5;
	kick.duration = 1.0;

	draw_attack(kick.collider);

	if (attack_collision(kick.collider))
		slog("Kick Collided");
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

	if (attack_collision(weapon_left.collider))
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

	if (attack_collision(weapon_right.collider))
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
