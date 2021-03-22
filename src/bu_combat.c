
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
	//slog("punch");

	draw_attack(punch.collider);

	if (attack_collision(punch.collider))
		slog("Collided");
	
	
}

void kick(Player *player)
{
	slog("Kick");
}

void throw_weapon(Player *player)
{
	slog("Throw weapon");
}

void weapon_attack_left(Player *player)
{
	slog("weapon attack left");
}

void weapon_attack_right(Player *player)
{
	slog("weapon attack right");
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
