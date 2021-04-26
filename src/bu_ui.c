
#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_text.h"

#include "bu_ui.h"
#include "bu_shop.h"
#include "bu_player.h"
#include "bu_npc.h"
#include "bu_camera.h"

typedef struct
{
	HUD hud;
	StartMenu  start_menu;
	PlayerMenu player_menu;
	EquipMenu  equip_menu;
	ShopUI     shop_ui;
}UI;

static UI *ui = { 0 };
static SJson *npc_dialog;

float life_ratio;
float stamina_ratio;

void hud_free()
{
	slog("freeing hud");
	if (!ui)return;

	memset(ui->hud.base, 0, sizeof(Sprite));
	memset(ui->hud.health, 0, sizeof(Sprite));
	memset(ui->hud.stamina, 0, sizeof(Sprite));
	
}

void start_menu_free()
{
	if (!ui)return;

	memset(ui->start_menu.background, 0, sizeof(Sprite));
	memset(ui->start_menu.quit_button, 0, sizeof(Sprite));
	memset(ui->start_menu.start_button, 0, sizeof(Sprite));
	slog("Freeing menu");
}

Sprite *create_ui_text(TextLine filename, TTF_Font *font, TextLine text, SDL_Color color, Bool text_wrap)
{
	Sprite *sprite = NULL;
	SDL_Surface	*surface = NULL;

	if (text_wrap)
		surface = TTF_RenderText_Blended_Wrapped(font, text, color, 32);
	else
		surface = TTF_RenderText_Blended(font, text, color);

	if (!surface)
	{
		slog("Surface null");
		return NULL;
	}
	
	sprite = gf2d_text_sprite_load(filename, surface, -1, -1, 0);

	if (!sprite){
		slog("Sprite null");
		SDL_FreeSurface(surface);
		return NULL;
	}

	return sprite;
}

void stat_menu_init()
{
	TextLine text;

	snprintf(text, sizeof(TextLine), "Name:             %s", get_player()->stats.name);
	if (!ui->player_menu.stat_text[0] || strcmp(ui->player_menu.stat_text[0], text) != 0)
	{
		ui->player_menu.stat_text[0] = text;
		ui->player_menu.text_sprite[0] = create_ui_text("name_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}

	snprintf(text, sizeof(TextLine), "Level:             %i", get_player()->stats.level);
	if (!ui->player_menu.stat_text[1] || strcmp(ui->player_menu.stat_text[1], text) != 0)
	{
		ui->player_menu.stat_text[1] = text;
		ui->player_menu.text_sprite[1] = create_ui_text("level_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
	snprintf(text, sizeof(TextLine), "Money:            %i", get_player()->stats.money);
	if (!ui->player_menu.stat_text[2] || strcmp(ui->player_menu.stat_text[2], text) != 0)
	{
		ui->player_menu.stat_text[2] = text;
		ui->player_menu.text_sprite[2] = create_ui_text("money_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
	snprintf(text, sizeof(TextLine), "Life:                %i /%i", get_player()->stats.life, get_player()->stats.life_max);
	int cmp = strcmp(ui->player_menu.stat_text[3], text);
	

	if (!ui->player_menu.stat_text[3] || strcmp(ui->player_menu.stat_text[3], &text) != 0)
	{
		if (ui->player_menu.stat_text[3]){
			slog("%s = %s", ui->player_menu.stat_text[3], text);
			slog("strcmp %i", gfc_line_cmp(ui->player_menu.stat_text[3], text));
		}
		ui->player_menu.text_sprite[3] = NULL;
		ui->player_menu.stat_text[3] = text;
		ui->player_menu.text_sprite[3] = create_ui_text("life_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
	snprintf(text, sizeof(TextLine), "Stamina:          %i /%i", get_player()->stats.stamina, get_player()->stats.stamina_max);
	if (!ui->player_menu.stat_text[4] || strcmp(ui->player_menu.stat_text[4], text) != 0)
	{
		ui->player_menu.stat_text[4] = text;
		ui->player_menu.text_sprite[4] = create_ui_text("stamina_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
	snprintf(text, sizeof(TextLine), "Stamina regen: %i", get_player()->stats.level);
	if (!ui->player_menu.stat_text[5] || strcmp(ui->player_menu.stat_text[5], text) != 0)
	{
		ui->player_menu.stat_text[5] = text;
		ui->player_menu.text_sprite[5] = create_ui_text("regen_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
	snprintf(text, sizeof(TextLine), "Strength:        %i", get_player()->stats.level);
	if (!ui->player_menu.stat_text[6] || strcmp(ui->player_menu.stat_text[6], text) != 0)
	{
		ui->player_menu.stat_text[6] = text;
		ui->player_menu.text_sprite[6] = create_ui_text("strength_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
	snprintf(text, sizeof(TextLine), "Agility:           %i", get_player()->stats.level);
	if (!ui->player_menu.stat_text[7] || strcmp(ui->player_menu.stat_text[7], text) != 0)
	{
		ui->player_menu.stat_text[7] = text;
		ui->player_menu.text_sprite[7] = create_ui_text("agility_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
	snprintf(text, sizeof(TextLine), "Willpower:        %i", get_player()->stats.level);
	if (!ui->player_menu.stat_text[8] || strcmp(ui->player_menu.stat_text[8], text) != 0)
	{
		ui->player_menu.stat_text[8] = text;
		ui->player_menu.text_sprite[8] = create_ui_text("willpower_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
	snprintf(text, sizeof(TextLine), "Punch:             %i", get_player()->stats.level);
	if (!ui->player_menu.stat_text[9] || strcmp(ui->player_menu.stat_text[9], text) != 0)
	{
		ui->player_menu.stat_text[9] = text;
		ui->player_menu.text_sprite[9] = create_ui_text("punch_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
	snprintf(text, sizeof(TextLine), "Kick:               %i", get_player()->stats.level);
	if (!ui->player_menu.stat_text[10] || strcmp(ui->player_menu.stat_text[10], text) != 0)
	{
		ui->player_menu.stat_text[10] = text;
		ui->player_menu.text_sprite[10] = create_ui_text("kick_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
	snprintf(text, sizeof(TextLine), "Weapon:           %i", get_player()->stats.level);
	if (!ui->player_menu.stat_text[11] || strcmp(ui->player_menu.stat_text[11], text) != 0)
	{
		ui->player_menu.stat_text[11] = text;
		ui->player_menu.text_sprite[11] = create_ui_text("weapon_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
	snprintf(text, sizeof(TextLine), "Throwing:        %i", get_player()->stats.level);
	if (!ui->player_menu.stat_text[12] || strcmp(ui->player_menu.stat_text[12], text) != 0)
	{
		ui->player_menu.stat_text[12] = text;
		ui->player_menu.text_sprite[12] = create_ui_text("throwing_text", ui->player_menu.player_font, text, ui->player_menu.black, NULL);
	}
}

void equip_menu_init()
{
	ui->equip_menu.equip_count = 4;
	ui->equip_menu.equip_menu = gf2d_sprite_load_image("images/ui/equip_info.png");
	ui->equip_menu.equip_items = (Sprite*)gfc_allocate_array(sizeof(Sprite), ui->equip_menu.equip_count);

}

void shop_ui_init()
{
	ui->shop_ui.shop_text = gfc_allocate_array(sizeof(Sprite), 1);
	ui->shop_ui.black = ui->player_menu.black;
	ui->shop_ui.text_box = gf2d_sprite_load_image("images/ui/text_box.png");
	ui->shop_ui.shop_font = ui->player_menu.player_font;
	ui->shop_ui.shop_text[0] = create_ui_text("shop_text", ui->shop_ui.shop_font, "Do you want to buy this item?", ui->shop_ui.black, NULL);
	ui->shop_ui.shop_text[1] = create_ui_text("shop_text", ui->shop_ui.shop_font, "You bought the item.", ui->shop_ui.black, 1);
}

void ui_init()
{
	slog("Initializing UI...");

	life_ratio = 0;
	stamina_ratio = 0;

	if (!ui)
		ui = (UI*)gfc_allocate_array(sizeof(UI), 1);
	else{
		slog("UI already initialized");
		return;
	}
	ui->player_menu.text_count = 13;
	ui->player_menu.text_sprite = (Sprite*)gfc_allocate_array(sizeof(Sprite), ui->player_menu.text_count);
	ui->player_menu.stat_text = (TextLine*)gfc_allocate_array(sizeof(TextLine), ui->player_menu.text_count);
	
	if (TTF_Init() < 0) {
		SDL_GetError();
	}

	// Fonts
	slog("Loading fonts...");
	ui->start_menu.start_font = TTF_OpenFont("fonts/Whypo-rrey.ttf", 32);
	ui->player_menu.player_font = TTF_OpenFont("fonts/KiriFont.ttf", 32);
	
	if (!ui->start_menu.start_font)
	{
		slog("Menu font is NULL");
		slog(SDL_GetError());
	}
	if (!ui->player_menu.player_font)
	{
		slog("Player font is NULL");
		slog(SDL_GetError());
	}
	slog("Fonts loaded.");
	// HUD
	ui->hud.base = gf2d_sprite_load_image("images/ui/base_bars.png");
	ui->hud.health = gf2d_sprite_load_image("images/ui/health_bar.png");
	ui->hud.stamina = gf2d_sprite_load_image("images/ui/stamina_bar.png");

	// Player menus
	ui->player_menu.stat_menu = gf2d_sprite_load_image("images/ui/char_info.png");
	ui->player_menu.black.a = 255;
	ui->player_menu.black.r = 0;
	ui->player_menu.black.g = 0;
	ui->player_menu.black.b = 0;

	// Player stats text sprites
	stat_menu_init();
	equip_menu_init();
	shop_ui_init();

	if (!ui->hud.base) slog("ui base is NULL"); return;
	if (!ui->hud.health) slog("ui health is NULL"); return;
	if (!ui->hud.stamina) slog("ui stamina is NULL"); return;

	if (!ui->player_menu.stat_menu) slog("ui stamina is NULL"); return;
	
	slog("UI initialized");
	atexit(hud_free);
	//atexit(start_menu_free);
}

void ui_update()
{
	life_ratio = (float)get_player()->stats.life / (float)get_player()->stats.life_max;
	stamina_ratio = (float)get_player()->stats.stamina / (float)get_player()->stats.stamina_max;
	//stat_menu_init();
}

void ui_draw(Vector2D res)
{
	int i;
	int y_pos = -15;
	float npc_life_ratio;
	TextLine npc_name;
	TextWord npc_index;
	Vector2D scale_hud, scale_menu, scale_life, scale_stamina, scale_npc_health, scale_npc_ui;
	scale_hud = vector2d(0.5, 0.5);
	scale_life = vector2d(life_ratio * 0.5, 0.5);
	scale_stamina = vector2d(stamina_ratio * 0.5, 0.5);
	scale_menu = vector2d(0.3, 0.3);

	// Draw Hud
	gf2d_sprite_draw(ui->hud.base, vector2d(res.x * 0.01, 0), &scale_hud, NULL, NULL, NULL, NULL, NULL);
	gf2d_sprite_draw(ui->hud.health, vector2d(res.x * 0.014, res.y * 0.03), &scale_life, NULL, NULL, NULL, NULL, NULL);
	gf2d_sprite_draw(ui->hud.stamina, vector2d(res.x * 0.014, res.y * 0.084), &scale_stamina, NULL, NULL, NULL, NULL, NULL);
	
	// Draw stats
	if (get_player()->stats.toggle_stats){
		gf2d_sprite_draw(ui->player_menu.stat_menu, vector2d(res.x * 0.014, res.y * 0.1), &scale_menu, NULL, NULL, NULL, NULL, NULL);
		for (i = 0; i < ui->player_menu.text_count; i++)
		{
			gf2d_sprite_draw(ui->player_menu.text_sprite[i], vector2d(res.x * 0.05, res.y *(0.2) + y_pos), &scale_menu, NULL, NULL, NULL, NULL, NULL);
			y_pos += 20;
		}
	}

    // Draw NPC stats
	for (i = 0; i < get_npc_count(); i++)
	{
		if (get_npc()[i].ent->_inuse)
		{
			
			if (get_npc()[i].stats.life < get_npc()[i].stats.life_max)
			{
				npc_life_ratio		= (float)get_npc()[i].stats.life / (float)get_npc()[i].stats.life_max;
				scale_npc_health	= vector2d(npc_life_ratio * 0.3, 0.5);
				scale_npc_ui		= vector2d(0.3, 0.5);
				
				strcpy(npc_name, "npc_name_");
				itoa(i, npc_index, 10);
				strcat(npc_name, npc_index);
				
				get_npc()[i].name = create_ui_text(npc_name, ui->player_menu.player_font, get_npc()[i].stats.name, ui->shop_ui.black, NULL);
				gf2d_sprite_draw(get_npc()[i].name, vector2d(get_npc()[i].ent->position.x + camera_get_offset().x, get_npc()[i].ent->position.y - 33 + camera_get_offset().y), &scale_hud, NULL, NULL, NULL, NULL, NULL);
				gf2d_sprite_draw(get_npc()[i].base, vector2d(get_npc()[i].ent->position.x + camera_get_offset().x, get_npc()[i].ent->position.y - 12 + camera_get_offset().y), &scale_npc_ui, NULL, NULL, NULL, NULL, NULL);
				gf2d_sprite_draw(get_npc()[i].health, vector2d(get_npc()[i].ent->position.x + camera_get_offset().x, get_npc()[i].ent->position.y - 9 + camera_get_offset().y), &scale_npc_health, NULL, NULL, NULL, NULL, NULL);
			}
		}
	}

	// Draw inventory
	if (get_player()->stats.toggle_inventory){
		gf2d_sprite_draw(ui->equip_menu.equip_menu, vector2d(res.x * 0.4, res.y * 0.1), &scale_menu, NULL, NULL, NULL, NULL, NULL);
		for (i = 0; i < ui->equip_menu.equip_count; i++)
		{
			if (!ui->equip_menu.equip_items[i])
				gf2d_sprite_draw(ui->equip_menu.equip_items[i], vector2d(res.x * 0.4, res.y * 0.1), &scale_menu, NULL, NULL, NULL, NULL, NULL);
		}
	}
	// Draw shop
	if (get_shop_open())
	{
		gf2d_sprite_draw(ui->shop_ui.text_box, vector2d(res.x * 0.2, res.y * 0.7), NULL, NULL, NULL, NULL, NULL, NULL);
		gf2d_sprite_draw(ui->shop_ui.shop_text[0], vector2d(res.x * 0.3, res.y * 0.8), &scale_menu, NULL, NULL, NULL, NULL, NULL);
		//buy_item();
	}


}

/*eol@eof*/
