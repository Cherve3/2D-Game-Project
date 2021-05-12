
#include "SDL.h"

#include "simple_logger.h"
#include "gfc_audio.h"

#include "bu_menu.h"

int open_menu(Vector2D resolution)
{
	int start = 0;
	int done;
	int x, y;
	SDL_Event event;
	SDL_Texture* menu = NULL;
	char* model_name = NULL;
	SDL_Texture* loading = NULL;
	SDL_Texture* load_icon = NULL;
	SDL_Rect start_rect;
	SDL_Rect editor_rect;
	SDL_Rect quit_rect;
	SDL_Rect load_rect;
	Sound* menu_music = NULL;

	// Main Menu
	slog("initializing menu");
	menu = IMG_LoadTexture(gf2d_graphics_get_renderer(), "images/ui/mainmenu.png");
	if (!menu) { slog("Main menu texture not created."); SDL_Quit(); exit(0); }

	SDL_RenderClear(gf2d_graphics_get_renderer());
	SDL_RenderCopy(gf2d_graphics_get_renderer(), menu, NULL, NULL);

	start_rect.h = 70;
	start_rect.w = 270;
	start_rect.x = resolution.x / 3.3;//250
	start_rect.y = resolution.y / 3;//230

	editor_rect.h = 70;
	editor_rect.w = 270;
	editor_rect.x = resolution.x / 3.3;// 250
	editor_rect.y = resolution.y / 1.8;//350

	quit_rect.h = 70;
	quit_rect.w = 270;
	quit_rect.x = resolution.x / 3.3;// 250
	quit_rect.y = resolution.y / 1.35;//350

	menu_music = gfc_sound_load("audio/507747__magmi-soundtracks__hybrid-action-music-01.wav", 1, 0);
	gfc_sound_play(menu_music, -1, 0.3, -1, -1);
	slog("Main Menu loaded");

	while (!start)
	{
		SDL_RenderPresent(gf2d_graphics_get_renderer());
		if (SDL_PollEvent(&event))
		{/*
			if (event.type == SDL_MOUSEMOTION)
			{
				x = event.motion.x;
				y = event.motion.y;

				if ((x > start_rect.x) && (x < start_rect.x + start_rect.w) &&
					(y > start_rect.y) && (y < start_rect.y + start_rect.h))
				{
					slog("Hovering over start");
				}

				if ((x > editor_rect.x) && (x < editor_rect.x + editor_rect.w) &&
					(y > editor_rect.y) && (y < editor_rect.y + editor_rect.h))
				{
					slog("Hovering over editor");
				}

				if ((x > quit_rect.x) && (x < quit_rect.x + quit_rect.w) &&
					(y > quit_rect.y) && (y < quit_rect.y + quit_rect.h))
				{
					slog("Hovering over Quit");
				}
			}*/
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				x = event.button.x;
				y = event.button.y;

				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if ((x > start_rect.x) && (x < start_rect.x + start_rect.w) &&
						(y > start_rect.y) && (y < start_rect.y + start_rect.h))
					{
						slog("Pressed Start button");
						start = 1;
						gfc_sound_clear_all();
						return start;
					}

					if ((x > editor_rect.x) && (x < editor_rect.x + editor_rect.w) &&
						(y > editor_rect.y) && (y < editor_rect.y + editor_rect.h))
					{
						slog("Pressed Editor button");
						start = -1;
						gfc_sound_clear_all();
						return start;
					}

					if ((x > quit_rect.x) && (x < quit_rect.x + quit_rect.w) &&
						(y > quit_rect.y) && (y < quit_rect.y + quit_rect.h))
					{
						slog("Pressed Quit button");
						gfc_sound_clear_all();
						SDL_DestroyRenderer(gf2d_graphics_get_renderer());
						SDL_DestroyTexture(menu);
						SDL_Quit();
						return 0;
					}
				}
			}
		}
	}
}

/*eol@eof*/
