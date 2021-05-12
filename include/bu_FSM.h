#ifndef __BU_FSM_H__
#define __BU_FSM_H__

typedef struct {
	void	(*active_state)();
	void	(*set_state)(void (*state));
}FSM;

void free_FSM(FSM* fsm);

FSM *new_FSM();

void update_state(FSM* fsm);

#endif
