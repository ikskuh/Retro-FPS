#ifndef _PROPS_ELEVATOR_H
	#define _PROPS_ELEVATOR_H
	
	// toggleable
	// if set, then each interaction will (toggle) move plaftorm up and down
	// if not set, then elevator will go through whole states (open, delay, close) by itself
	
	// use_switch
	// if set, elevator can be triggered only by switch (with the same ID)
	
	// use trigger
	// if set, elevator can be triggered only by trigger zone (with the same ID)
	
	// function to update given elevator
	void elevator_update(ENTITY *ent);
#endif