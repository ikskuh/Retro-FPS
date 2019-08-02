#ifndef _PLAYER_STATE_MACHINE_H_
	#define _PLAYER_STATE_MACHINE_H_
	
	// state machine states
	#define IDLE 0
	#define WALK 1
	#define RUN 2
	#define JUMPED 3
	#define IN_AIR 4
	#define LANDED 5
	#define PAIN 6
	#define SHOOT 7
	#define DEATH 8
	#define SWIM 9
	#define DIVE 10
	
	// handle all player's states
	void player_handle_state_machine(ENTITY *ent, CCT *cct);
	
	#include "player_state_machine.c"
#endif