#ifndef _PLAYER_BEHAVIOUR_H_
	#define _PLAYER_BEHAVIOUR_H_
	
	// This header includes everything related to player's action
	
	// toggle auto run mode on and off
	void player_toggle_run();
	
	// handles all voice sounds made by player
	void player_sounds(ENTITY * me, CCT *cct);
	
	// handles all events that player receives
	// such as EVENT_PUSH, EVENT_SCAN, EVENT_SHOOT etc
	void player_event();
	
	// handles all stuff related to after death life
	void player_dead();
	
	// handle all player's interactions
	void player_interact(ENTITY * me, CCT *cct);

  // update all player code
	void player_update(ENTITY * ent);
	
	#include "player_behaviour.c"
#endif