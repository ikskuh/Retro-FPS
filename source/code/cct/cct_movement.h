#ifndef _CCT_MOVEMENT_H_
	#define _CCT_MOVEMENT_H_
	
	// Functions needed to perform all movement
	
	// perform gravity trace for the given entity with the given cct
	void ent_gravity_trace(ENTITY *ent, CCT *cct);
	
	// calculate the damage from a fall and return that value:
	function ent_fall_damage(CCT *cct);

	// handle all stuff related to landing
	// taking damage from landing etc
	void ent_landed(ENTITY *ent, CCT *cct);

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
#endif