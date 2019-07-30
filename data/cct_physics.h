#ifndef _CCT_PHYSICS_H_
	#define _CCT_PHYSICS_H_
	
	#define SOLID 0
	#define STAIRS 1
	#define MOVING 2
	
	var cct_grav_trace_start_offset = 4;
	var cct_grav_trace_end_offset = 24;
	
	var cct_ground_z_offset = 2;
	
	typedef struct {
		
		VECTOR force;
		
		VECTOR surface_normal;
		VECTOR surface_speed;
		
		var bbox_x;
		var bbox_y;
		var bbox_z;
		
		var ground_info;
		
		var foot_height;
		var soil_height;
		
	} CCT_PHYSICS;
	
	CCT_PHYSICS *register_cct(ENTITY *ent);
	
	void init_cct(CCT_PHYSICS *cct);
	
	CCT_PHYSICS *get_cct(ENTITY *ent);
	
	void delete_cct(ENTITY *ent);
	
	void cct_gravity_trace(ENTITY *ent, CCT_PHYSICS *physics);
	
	#include "cct_physics.c"
#endif