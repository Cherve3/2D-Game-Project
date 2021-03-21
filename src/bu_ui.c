
#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_text.h"

#include "bu_ui.h"
#include "bu_player.h"

typedef struct
{
	HUD hud;
	StartMenu start_menu;
	PlayerMenu player_menu;
}UI;

static UI *ui = { 0 };
static SJson *npc_dialog;

float life_ratio;
float stamina_ratio;

void hud_free()
{
	if (!ui)return;

	memset(ui->hud.base, 0, sizeof(Sprite));
	memset(ui->hud.health, 0, sizeof(Sprite));
	memset(ui->hud.stamina, 0, sizeof(Sprite));
	slog("freeing hud");
}

void start_menu_free()
{
	if (!ui)return;

	memset(ui->start_menu.background, 0, sizeof(Sprite));
	memset(ui->start_menu.quit_button, 0, sizeof(Sprite));
	memset(ui->start_menu.start_button, 0, sizeof(Sprite));
	slog("Freeing menu");
}

Sprite *create_ui_text(TextLine filename, TTF_Font *font, TextLine text, SDL_Color color)
{
	Sprite *sprite = NULL;
	SDL_Surface	*surface = NULL;

	surface = TTF_RenderText_Blended(font, text, color);

	if (!surface)
	{
		slog("Surface null");
	}
	sprite = gf2d_text_sprite_load(filename, surface, -1, -1, 0);

	if (!sprite)
		slog("Sprite null");
	return sprite;
}

void stat_menu_init()
{
	int i;
	TextLine text;

	snprintf(text, sizeof(TextLine), "Name:             %s", get_player()->stats.name);
	ui->player_menu.text_sprite[0] = create_ui_text("name_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Level:             %i", get_player()->stats.level);
	ui->player_menu.text_sprite[1] = create_ui_text("level_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Money:            %i", get_player()->stats.money);
	ui->player_menu.text_sprite[2] = create_ui_text("money_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Life:                %i /%i", get_player()->stats.life, get_player()->stats.life_max);
	ui->player_menu.text_sprite[3] = create_ui_text("life_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Stamina:          %i /%i", get_player()->stats.stamina, get_player()->stats.stamina_max);
	ui->player_menu.text_sprite[4] = create_ui_text("stamina_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Stamina regen: %i", get_player()->stats.level);
	ui->player_menu.text_sprite[5] = create_ui_text("regen_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Strength:        %i", get_player()->stats.level);
	ui->player_menu.text_sprite[6] = create_ui_text("strength_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Agility:           %i", get_player()->stats.level);
	ui->player_menu.text_sprite[7] = create_ui_text("agility_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Willpower:        %i", get_player()->stats.level);
	ui->player_menu.text_sprite[8] = create_ui_text("willpower_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Punch:             %i", get_player()->stats.level);
	ui->player_menu.text_sprite[9] = create_ui_text("punch_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Kick:               %i", get_player()->stats.level);
	ui->player_menu.text_sprite[10] = create_ui_text("kick_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Weapon:           %i", get_player()->stats.level);
	ui->player_menu.text_sprite[11] = create_ui_text("weapon_text", ui->player_menu.player_font, text, ui->player_menu.black);
	snprintf(text, sizeof(TextLine), "Throwing:        %i", get_player()->stats.level);
	ui->player_menu.text_sprite[12] = create_ui_text("throwing_text", ui->player_menu.player_font, text, ui->player_menu.black);
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

	//create_ui_text("statname",);

	if (!ui->hud.base) slog("ui base is NULL"); return;
	if (!ui->hud.health) slog("ui health is NULL"); return;
	if (!ui->hud.stamina) slog("ui stamina is NULL"); return;

	if (!ui->player_menu.stat_menu) slog("ui stamina is NULL"); return;
	
	slog("UI initialized");
	atexit(hud_free);
	atexit(start_menu_free);
}

void ui_update()
{
	life_ratio = (float)get_player()->stats.life / (float)get_player()->stats.life_max;
	stamina_ratio = (float)get_player()->stats.stamina / (float)get_player()->stats.stamina_max;

}

void ui_draw(Vector2D res)
{
	int i;
	int y_pos = -15;
	Vector2D scale_hud, scale_menu, scale_life, scale_stamina;
	scale_hud = vector2d(0.5, 0.5);
	scale_life = vector2d(life_ratio * 0.5, 0.5);
	scale_stamina = vector2d(stamina_ratio * 0.5, 0.5);
	scale_menu = vector2d(0.3, 0.3);

	gf2d_sprite_draw(ui->hud.base, vector2d(res.x * 0.01, 0), &scale_hud, NULL, NULL, NULL, NULL, NULL);
	gf2d_sprite_draw(ui->hud.health, vector2d(res.x * 0.014, res.y * 0.03), &scale_life, NULL, NULL, NULL, NULL, NULL);
	gf2d_sprite_draw(ui->hud.stamina, vector2d(res.x * 0.014, res.y * 0.084), &scale_stamina, NULL, NULL, NULL, NULL, NULL);
	
	if (get_player()->stats.toggle_stats){
		gf2d_sprite_draw(ui->player_menu.stat_menu, vector2d(res.x * 0.014, res.y * 0.1), &scale_menu, NULL, NULL, NULL, NULL, NULL);
		for (i = 0; i < ui->player_menu.text_count; i++){
			gf2d_sprite_draw(ui->player_menu.text_sprite[i], vector2d(res.x * 0.05, res.y *(0.2) + y_pos), &scale_menu, NULL, NULL, NULL, NULL, NULL);
			y_pos += 20;
		}
	}
}

/*eol@eof*/
