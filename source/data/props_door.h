#ifndef _PROPS_DOOR_H_
	#define _PROPS_DOOR_H_
	
	// NOTE - props_door_a and props_door_b differ only by sound effects
	
	// red_key OR yellow_key OR blue_key
	// if any set, then door will require the key with the set color to be opened!
	
	// use_once (set by default)
	// if set, then door will go through whole states (open, delay, close) by itself
	// if not set, then each interaction will open/close the door
	
	// use_switch_id
	// if set, door can be triggered only by switch (with the same ID)
	
	// use trigger
	// if set, door can be triggered only by trigger zone (with the same ID)
	
	// function to update given door
	void door_update(ENTITY *ent);
	
	// check if player has needed key
	var door_player_has_key();

	// event function for the door
	void door_event();
	
	#include "props_door.c"
#endif