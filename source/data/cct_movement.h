#ifndef _CCT_MOVEMENT_H_
	#define _CCT_MOVEMENT_H_
	
	void ent_vertical_movement(ENTITY *ent, CCT_PHYSICS *physics);
	
	void ent_horizontal_movement(ENTITY *ent, CCT_PHYSICS *physics);
	
	void ent_movement(ENTITY *ent, CCT_PHYSICS *physics);
	
	void ent_slide_on_slopes(ENTITY *ent, CCT_PHYSICS *physics);
	
	void ent_stop_movement(CCT_PHYSICS *physics);
	
	#include "cct_movement.c"
#endif