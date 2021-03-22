
#include "bu_money.h"
#include "bu_entity.h"


void money_touch()
{

}

void money_think()
{

}

void spawn_money(int value, Vector2D position)
{
	Entity *money;

	money = entity_new();
	if (!money) return;

	money->name = "Money";
	money->position.x = position.x;
	money->position.y = position.y;
	money->rect_collider.x = position.x;
	money->rect_collider.y = position.y;
	money->rect_collider.w = 20;
	money->rect_collider.h = 20;
	money->think = money_think;
	money->onTouch = money_touch;

	money->sprite = gf2d_sprite_load_image("images/items/money.png");
	
}

/*eol@eof*/
