
#include "simple_logger.h"

#include "bu_ui.h"

static UI *ui = { 0 };

void hud_free()
{
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
}

void ui_init()
{
	if (!ui)
		ui = (UI*)gfc_allocate_array(sizeof(UI), 1);
	
	ui->hud.base = gf2d_sprite_load_image("images/base_bars.png");
	ui->hud.health = gf2d_sprite_load_image("images/health_bar.png");
	ui->hud.stamina = gf2d_sprite_load_image("images/stamina_bar.png");
	
	if (!ui->hud.base) slog("ui base is NULL"); return;
	if (!ui->hud.health) slog("ui health is NULL"); return;
	if (!ui->hud.stamina) slog("ui stamina is NULL"); return;

	slog("UI initialized");
	atexit(hud_free);
}

void ui_draw(Vector2D res)
{
	Vector2D scale;
	scale = vector2d(0.5, 0.5);
	gf2d_sprite_draw(ui->hud.base, vector2d(res.x * 0.01, 0), &scale,NULL,NULL,NULL,NULL,NULL);
	gf2d_sprite_draw(ui->hud.health, vector2d(res.x * 0.014, res.y * 0.03), &scale, NULL, NULL, NULL, NULL, NULL);
	gf2d_sprite_draw(ui->hud.stamina, vector2d(res.x * 0.014, res.y * 0.084), &scale, NULL, NULL, NULL, NULL, NULL);
}

/*eol@eof*/
