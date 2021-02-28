
#ifndef __BU_UI_H__
#define __BU_UI_H__

#include "gf2d_sprite.h"

typedef struct
{
	Sprite *health;
	Sprite *stamina;
	Sprite *base;
}HUD;

typedef struct
{
	Sprite *background;
	Sprite *start_button;
	Sprite *quit_button;
}StartMenu;

typedef struct
{
	HUD hud;
	StartMenu start_menu;
}UI;

void start_menu_init();

void start_menu_free();

void ui_init();

void hud_free();

void start_menu_free();

void ui_draw(Vector2D);

#endif
