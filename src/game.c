#include <math.h>

#include <SDL.h>

#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_private.h"

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "gf2d_windows.h"
#include "gfc_audio.h"

#include "bu_timer.h"
#include "bu_camera.h"
#include "bu_ui.h"
#include "bu_menu.h"
#include "bu_entity.h"
#include "bu_player.h"
#include "bu_npc.h"
#include "bu_level.h"
#include "bu_items.h"
#include "bu_combat.h"
#include "bu_hazards.h"

int main_menu_update(Window* win, List* updateList)
{
	int i, count;
	Element* e;
	if (!win)return 0;
	if (!updateList)return 0;
	count = gfc_list_get_count(updateList);
	for (i = 0; i < count; i++)
	{
		e = gfc_list_get_nth(updateList, i);
		if (!e)continue;
		slog("updated element index %i", e->index);
		switch (e->index)
		{
		case 51:
			slog("ok");
			break;
		case 52:
			slog("cancel");
			break;
		}
	}
	return 0;
}

void main_menu()
{
	Window* win;
	SJson* json = NULL;
	json = sj_load("config/testwindow.cfg");
	win = gf2f_window_load_from_json(json);
	if (win)
	{
		win->update = main_menu_update;
	}
	sj_free(json);
}


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int menu = 0;
	int start = 0;
	int done = 0;
	int charchoice = 0;
	int x, y;
	const Uint8 * keys = SDL_GetKeyboardState(NULL);

	SDL_Rect rect;
	Vector4D vec;

	Vector2D scale = vector2d(0.5, 0.5);
	Vector2D resolution = vector2d(1366 * 0.5, 786 * 0.5);
	
	//Mouse
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};

	//Item bat vector
	cpVect cp_vec;

	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	time_init();

    /*program initialization*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        NULL,
		NULL,
		resolution.x,
		resolution.y,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
	gfc_audio_init(256, 16,4,1,1,1);
	gf2d_windows_init(128);
	//color black
	vec.w = 255;
	vec.x = 255;
	vec.y = 255;
	vec.z = 255;
	//main_menu();
	start = open_menu(resolution);
	
	if (start == 1)
	{
		gf2d_sprite_init(1024);
		entity_manager_init(100);
		item_manager_init(50);
		level_init();

		cpFloat timeStep = 1.0 / 60.0;

		SDL_ShowCursor(SDL_DISABLE);

		// Camera
		camera_set_dimensions(vector2d(resolution.x, resolution.y));
		camera_set_position(vector2d(0, 0));
		camera_set_bounds(vector2d(1920, 1080));


		// Rectangle representation of player camera bounds
		rect.x = resolution.x / 6;
		rect.y = resolution.y / 6;
		rect.w = resolution.x / 1.5;
		rect.h = resolution.y / 1.5;

		mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);

		// Level
		level_load("locker_room");
		ui_init();
		attack_list_init();


		cp_vec.x = 500;
		cp_vec.y = 200;
		item_new("item_bat", weapon, cp_vec);
		//cp_vec.x = 500;
		//cp_vec.y = 500;
		//new_hazard("puddle", cp_vec);
		slog_sync();
		Vector2D start, end, position;

		/*main game loop*/
		while (!done)
		{
			SDL_PumpEvents();   // update SDL's internal event structures
			/*update things here*/
			SDL_GetMouseState(&mx, &my);
			mf += 0.1;
			if (mf >= 16.0)mf = 0;

			gf2d_windows_update_all();
			
			entity_manager_think_entities();
			entity_manager_update_entities();

			gf2d_graphics_clear_screen();// clears drawing buffers
			// all drawing should happen betweem clear_screen and next_frame
			//backgrounds drawn first

			level_draw();
			// Player bounds rect
			//gf2d_draw_rect(rect, vec);

			entity_draw_all();

			//UI elements last
			ui_update();
			ui_draw(resolution);

			cpSpaceStep(get_level()->space, timeStep);

			gf2d_windows_draw_all();
			gf2d_sprite_draw(
				mouse,
				vector2d(mx, my),
				NULL,
				NULL,
				NULL,
				NULL,
				&mouseColor,
				(int)mf);

			gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
			if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
			slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
		}

		level_free();
		npc_free();
		player_free();
		slog("---==== END ====---");
		slog_sync();
		return 0;
	}
	else if (start == -1)
	{
		while (!done)
		{
			slog("Editor Open");
			SDL_PumpEvents();
			if (keys[SDL_SCANCODE_ESCAPE])done = 1;
		}
	}
	else
	{
		SDL_DestroyRenderer(gf2d_graphics_get_renderer());
		SDL_DestroyTexture(menu);
		done = 1;
		SDL_Quit();
		return 0;
	}


}
  
/*eol@eof*/
