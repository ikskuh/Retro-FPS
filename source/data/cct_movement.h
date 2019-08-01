#ifndef _CCT_MOVEMENT_H_
	#define _CCT_MOVEMENT_H_
	
	// Functions needed to perform all movement
	
	// handle stuff related movement speed
	// f.e. don't allow cheating by holding forward and left-right keys
	void ent_handle_movement_speed(CCT *cct, var spd);
	
	// handle all gravity movement when out of the water
	void ent_vertical_movement_on_foot(ENTITY *ent, CCT *cct);
	
	// same as above, but in water
	void ent_vertical_movement_in_water(ENTITY *ent, CCT *cct);
	
	// handle all horizontal (XY) movement when out of the water
	void ent_horizontal_movement_on_foot(ENTITY *ent, CCT *cct);
	
	// same as above, but in water
	void ent_horizontal_movement_in_water(ENTITY *ent, CCT *cct);
	
	// handle all the movement
	void ent_movement(ENTITY *ent, CCT *cct);
	
	// slide on steep slopes, if we are allowed to
	void ent_slide_on_slopes(ENTITY *ent, CCT *cct);
	
	// stop all movement for the given cct
	void ent_stop_movement(CCT *cct);
	
	// stop all movement from on foot state
	void ent_stop_movement_on_foot(CCT *cct);

	// stop all movement from on water state
	void ent_stop_movement_in_water(CCT *cct);
	
	#include "cct_movement.c"
#endif