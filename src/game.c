#include <SDL.h>

#include <math.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"

#include "bu_camera.h"
#include "bu_ui.h"
#include "bu_entity.h"
#include "bu_player.h"
#include "bu_npc.h"
#include "bu_level.h"

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
    gf2d_sprite_init(1024);
	entity_manager_init(100);
	ui_init();
    SDL_ShowCursor(SDL_DISABLE);

	// Camera
	camera_set_dimensions( vector2d(resolution.x, resolution.y) );
	camera_set_position( vector2d(0, 0) );
	camera_set_bounds(vector2d(1920, 1080));

	rect.x = resolution.x / 6;
	rect.y = resolution.y / 6;
	rect.w = resolution.x / 1.5;
	rect.h = resolution.y / 1.5;
	vec.w = 255;
	vec.x = 0;
	vec.y = 0;
	vec.z = 0;

	// Level
	level_load("shop.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	
	// Entity spawns
	player_spawn(vector2d((resolution.x)-600, (resolution.y)-250));
	npc_spawn(0, 0, vector2d(200, (786 * 0.5) - 200));
	npc_spawn(1, 0, vector2d(100, (786 * 0.5) - 200));

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

		entity_manager_update_entities();

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
		
		if (keys[SDL_SCANCODE_UP])
			camera_move(vector2d(0, -1));
		if (keys[SDL_SCANCODE_DOWN])
			camera_move(vector2d(0, 1));
		if (keys[SDL_SCANCODE_LEFT])
			camera_move(vector2d(-1, 0));
		if (keys[SDL_SCANCODE_RIGHT])
			camera_move(vector2d(1, 0));


		level_draw();
		gf2d_draw_rect(rect, vec);

		entity_draw_all();
		ui_draw(resolution);


        //UI elements last
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
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
