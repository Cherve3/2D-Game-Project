
#ifndef __BU_UI_H__
#define __BU_UI_H__

#include "SDL_ttf.h"

#include "gf2d_sprite.h"

typedef struct
{
	Sprite *health;
	Sprite *stamina;
	Sprite *base;
}HUD;

typedef struct
{
	Sprite   *background;
	Sprite   *start_button;
	Sprite   *quit_button;
	TTF_Font *start_font;
}StartMenu;

typedef struct
{
	Sprite     *stat_menu;
	Sprite     *inventory_menu;
	Sprite    **text_sprite;
	TTF_Font   *player_font;
	TextLine   **stat_text;
	int			text_count;
	SDL_Color   black;
}PlayerMenu;

typedef struct
{
	Sprite  *equip_menu;
	Sprite **equip_items;
	int      equip_count;
}EquipMenu;

typedef struct
{
	Sprite    *text_box;
	Sprite   **shop_text; 
	TTF_Font  *shop_font;
	SDL_Color  black;
}ShopUI;

void start_menu_init();

void start_menu_free();

void ui_init();

void hud_free();

void ui_update();

void shop_ui_init();

void ui_draw(Vector2D);

#endif
