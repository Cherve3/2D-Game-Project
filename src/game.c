#include <math.h>

#include <SDL.h>

#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_private.h"

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"

#include "bu_timer.h"
#include "bu_camera.h"
#include "bu_ui.h"
#include "bu_entity.h"
#include "bu_player.h"
#include "bu_npc.h"
#include "bu_level.h"
#include "bu_items.h"
#include "bu_combat.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
	int start = 0;
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

	//Menu variables
	SDL_Event event;
	SDL_Texture *menu = NULL;
	char* model_name = NULL;
	SDL_Texture *loading = NULL;
	SDL_Texture *load_icon = NULL;
	SDL_Rect start_rect;
	SDL_Rect editor_rect;
	SDL_Rect quit_rect;
	SDL_Rect load_rect;

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

	//color black
	vec.w = 255;
	vec.x = 255;
	vec.y = 255;
	vec.z = 255;
	
	// Main Menu
	slog("initializing menu");
	menu = IMG_LoadTexture(gf2d_graphics_get_renderer(), "images/ui/mainmenu.png");
	if (!menu){ slog("Main menu texture not created."); SDL_Quit(); exit(0); }

	SDL_RenderClear(gf2d_graphics_get_renderer());
	SDL_RenderCopy(gf2d_graphics_get_renderer(), menu, NULL, NULL);

	start_rect.h = 70;
	start_rect.w = 270;
	start_rect.x = resolution.x/3.3;//250
	start_rect.y = resolution.y/3 ;//230

	editor_rect.h = 70;
	editor_rect.w = 270;
	editor_rect.x = resolution.x / 3.3;// 250
	editor_rect.y = resolution.y / 1.8;//350

	quit_rect.h = 70;
	quit_rect.w = 270;
	quit_rect.x = resolution.x / 3.3;// 250
	quit_rect.y = resolution.y / 1.35;//350

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

					}

					if ((x > editor_rect.x) && (x < editor_rect.x + editor_rect.w) &&
						(y > editor_rect.y) && (y < editor_rect.y + editor_rect.h))
					{
						slog("Pressed Editor button");
						start = -1;

					}

					if ((x > quit_rect.x) && (x < quit_rect.x + quit_rect.w) &&
						(y > quit_rect.y) && (y < quit_rect.y + quit_rect.h))
					{
						slog("Pressed Quit button");
						SDL_DestroyRenderer(gf2d_graphics_get_renderer());
						SDL_DestroyTexture(menu);
						done = 1;
						SDL_Quit();
						return 0;
					}
				}
			}
		}
	}
	
	if (start == 1)
	{
		gf2d_sprite_init(512);
		entity_manager_init(100);
		item_manager_init(50);
		level_init();

		/*Testing Chipmunk*/
		//Ball
		cpFloat radius = 5;
		cpFloat mass = 1;
		cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);
		cpBody* ballBody = cpSpaceAddBody(get_level()->space, cpBodyNew(mass, moment));
		cpBodySetPosition(ballBody, cpv(200, 20));
		cpShape* ballShape = cpSpaceAddShape(get_level()->space, cpCircleShapeNew(ballBody, radius, cpvzero));
		cpShapeSetFriction(ballShape, 0.7);

		cpBody* boxBody = cpSpaceAddBody(get_level()->space, cpBodyNew(mass, moment));
		cpBodySetPosition(boxBody, cpv(300, 20));
		cpShape* boxShape = cpSpaceAddShape(get_level()->space, cpBoxShapeNew2(boxBody, cpBBNew(-10.0,-10.0,10.0,10.0), 0.0));
		//cpSpaceAddShape(space, cpBoxShapeNew(body, 50, 50, 0.0));
		///shape->e = 0.0f; shape->u = 0.7f;
		cpShapeSetFriction(boxShape, 0.9);
		

		cpFloat timeStep = 1.0 / 60.0;
		/*Testing Chipmunk*/

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

		cp_vec.x = 500;
		cp_vec.y = 200;
		item_new("item_bat", weapon, cp_vec);
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

			//Chipmunk test
			cpVect pos = cpBodyGetPosition(ballBody);
			cpVect vel = cpBodyGetVelocity(ballBody);
			Vector2D position, start, end;
			position.x = pos.x;
			position.y = pos.y;
			gf2d_draw_circle(position, 5, vec);
			SDL_Rect rect;
			pos = cpBodyGetPosition(boxBody);
			rect.x = pos.x;
			rect.y = pos.y;
			rect.w = 10;
			rect.h = 10;

			gf2d_draw_rect(rect, vec);
			printf(
				"ballBody is at (%5.2f, %5.2f). It's velocity is (%5.2f, %5.2f)\n",
				pos.x, pos.y, vel.x, vel.y
			);

			cpSpaceStep(get_level()->space, timeStep);
			//Chipmunk test


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
			//slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
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

		// Clean up our objects and exit!
		//cpShapeFree(ballShape);
		//cpBodyFree(ballBody);
		//cpShapeFree(ground);
		//cpSpaceFree(space);

		return 0;
	}


}
  
/*eol@eof*/
