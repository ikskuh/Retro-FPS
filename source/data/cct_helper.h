#ifndef _CCT_HELPER_H_
	#define _CCT_HELPER_H_
	
	// Helper functions used for cct
	
	// handle interaction traces
	void ent_interact(ENTITY *ent, CCT *cct);
	
	// update interaction trace positions
	void ent_interact_trace_pos(ENTITY *ent, CCT *cct);

	// push given forces away from the normals with the given strength
	void ent_foot_push(VECTOR *force, VECTOR *surf_normal, var strength);
	
	// used to push entity away from the normals
	// in order to prevent it's auto climbing bullshit (caused by ellipsoid hull)
	void ent_foot_push_from_normals(ENTITY *ent, CCT *cct, VECTOR *hit_pos, VECTOR *surf_normal);
	
	// function used for stopping movement (velocity, speed, forces etc)
	void ent_foot_stop_velocity(var is_x, ENTITY *ent, CCT *cct, VECTOR *hit_pos);
	
	// perform foot check for given entity
	// it's used as a workaround for aweful ellipsoid hull that OBB uses in Acknex
	void ent_foot_check(ENTITY *ent, CCT *cct);
	
	// checks if given cct is inside of the given entity
	// returns true if true, else if not
	var is_cct_in_rect(ENTITY *cct, ENTITY *rect, var scale);

	#include "cct_helper.c"
#endif