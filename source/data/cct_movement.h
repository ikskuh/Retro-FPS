#ifndef _CCT_MOVEMENT_H_
	#define _CCT_MOVEMENT_H_
	
	// Functions needed to perform all movement
	
	// handle all gravity movement when out of the water
	void ent_vertical_movement(ENTITY *ent, CCT *cct);
	
	// handle all horizontal (XY) movement when out of the water
	void ent_horizontal_movement(ENTITY *ent, CCT *cct);
	
	// handle all the movement
	void ent_movement(ENTITY *ent, CCT *cct);
	
	// slide on steep slopes, if we are allowed to
	void ent_slide_on_slopes(ENTITY *ent, CCT *cct);
	
	// stop all movement for the given cct
	void ent_stop_movement(CCT *cct);
	
	#include "cct_movement.c"
#endif