
#include "gfc_types.h"
#include "simple_logger.h"

#include "bu_FSM.h"
#include "bu_npc.h"


void set_state(FSM* fsm, void(*state)());
void update_state(FSM* fsm);

void free_FSM(FSM *fsm)
{
	if (!fsm)
		return;
	fsm->active_state = NULL;
	fsm->set_state = NULL;
	free(fsm);
	slog("FSM freed");
}

FSM *new_FSM()
{
	FSM* fsm = gfc_allocate_array(sizeof(FSM), 1);
	fsm->active_state = NULL;
	fsm->set_state = set_state;

	return fsm;
}

void set_state(NPC *npc, void (*state)())
{
	if (!npc)return;
	if (npc->fsm)
		npc->fsm->active_state = state;
}

void update_state(FSM *fsm)
{
	if (!fsm->active_state) return;
	slog("ACTIVE STATE");
		fsm->active_state;
}

/*eol@eof*/
