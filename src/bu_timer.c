
#include "bu_timer.h"

static Uint32 start_time = 0;
static Uint32 current_time = 0;

void time_init()
{
	start_time = SDL_GetTicks();
}

Uint32 get_start_time()
{
	return start_time;
}

Uint32 get_current_time()
{
	current_time = SDL_GetTicks();
	return current_time;
}
/*eol@eof*/
