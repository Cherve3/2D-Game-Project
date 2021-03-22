#ifndef __BU_TIMER_H__
#define __BU_TIMER_H__

#include "gfc_types.h"

typedef struct
{
	Uint32 timer;
	Uint32 old_time;
}Cooldown;

void time_init();

Uint32 get_start_time();

Uint32 get_current_time();

#endif
