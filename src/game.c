#include <SDL.h>

#include <math.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "bu_entity.h"
#include "bu_player.h"
#include "bu_npc.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;

	Player *player;
	NPC *npc;

	Vector2D scale = vector2d(0.5, 0.5);
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1366/2,
        786/2,
        1366/2,
        786/2,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
	entity_manager_init(100);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
	

    sprite = gf2d_sprite_load_image("images/locker_room.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

	player = player_spawn(vector2d((1366 * 0.5)-200, (786 * 0.5)-200));
	if (!player)slog("player null");slog_sync();

	npc_spawn(0, 0, vector2d(200, (786 * 0.5) - 200));

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

		gf2d_sprite_draw(sprite, vector2d(0, 0), &scale, NULL, NULL, NULL, NULL, NULL);

		entity_draw_all();

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
