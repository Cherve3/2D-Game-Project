#include <math.h>

#include <SDL.h>

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
    const Uint8 * keys;
    Sprite *sprite;

	SDL_Rect rect;
	Vector4D vec;

	Vector2D scale = vector2d(0.5, 0.5);
	Vector2D resolution = vector2d(1366 * 0.5, 786 * 0.5);
	
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};

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

    gf2d_sprite_init(512);
	entity_manager_init(100);
	item_manager_init(50);
	level_init();
	

    SDL_ShowCursor(SDL_DISABLE);

	// Camera
	camera_set_dimensions( vector2d(resolution.x, resolution.y) );
	camera_set_position( vector2d(0, 0) );
	camera_set_bounds(vector2d(1920, 1080));


	// Rectangle representation of player camera bounds
	rect.x = resolution.x / 6;
	rect.y = resolution.y / 6;
	rect.w = resolution.x / 1.5;
	rect.h = resolution.y / 1.5;
	//color black
	vec.w = 255;
	vec.x = 0;
	vec.y = 0;
	vec.z = 0;

    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	
	// Level
	level_load("locker_room");
	ui_init();
	item_new("item_bat", weapon, vector2d(500, 200));
	slog_sync();

	/*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

		entity_manager_think_entities();
		entity_manager_update_entities();

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first

		level_draw();
		// Player bounds rect
		//gf2d_draw_rect(rect, vec);
		
		gf2d_draw_rect(gfc_sdl_rect(
			get_player_collider().x+camera_get_offset().x,
			get_player_collider().y + camera_get_offset().y,
			get_player_collider().w,
			get_player_collider().h),
			vec);

		entity_draw_all();

		//UI elements last
		ui_update();
		ui_draw(resolution);

        gf2d_sprite_draw(
            mouse,
            vector2d(mx,my),
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
/*eol@eof*/
