#ifndef _CCT_HELPER_H_
	#define _CCT_HELPER_H_
	
	// Helper functions used for cct
	
	// handle interaction traces
	void ent_interact(ENTITY *ent, CCT *cct);
	
	// update interaction trace positions
	void ent_interact_trace_pos(ENTITY *ent, CCT *cct);

	// function used for stopping movement (velocity, speed, forces etc)
	void ent_foot_stop_velocity(var is_x, ENTITY *ent, CCT *cct, VECTOR *hit_pos);
	
	// perform foot check for given entity
	// it's used as a workaround for aweful ellipsoid hull that OBB uses in Acknex
	void ent_foot_check(ENTITY *ent, CCT *cct);
	
	#include "cct_helper.c"
#endif