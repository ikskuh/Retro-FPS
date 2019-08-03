#ifndef _CCT_H_
	#define _CCT_H_
	
	// This is basic character controller (cct) header
	// it's used for registering, deleting and contains all cct parameters
	//
	// This header also includes such headers as
	// - cct_helper.h (everything needed for proper cct behaviour)
	// - cct_water_detection.h (everything needed to perform water detection)
	// - cct_movement.h (everything needed to perform all movement)
	
	// ground info defines
	#define SOLID 0
	#define STAIRS 1
	#define MOVING 2
	
	// water detection states
	#define OUT_OF_WATER 0
	#define ON_WATER 1
	#define IN_WATER 2
	#define HEAD_IN_WATER 3
	
	var cct_gravity = 4; // gravity strength
	var cct_gravity_max = 90; // gravity strength max
	var cct_gravity_fluid = 1; // gravity strength in fluid
	
	var cct_gnd_fric = 0.5; // ground friction
	var cct_air_fric = 0.1; // air friction
	var cct_water_fric = 0.75; // water friction
	var cct_grav_trace_start_offset = 4; // offset for start position of the gravity trace
	var cct_grav_trace_end_offset = 24; // length of the gravity trace
	
	var cct_def_foot_step_height = 8; // default footstep height
	var cct_ground_z_offset = 2; // offset from ground
	var cct_slope_fac = 0.5; // walkable slope angle
	
	var cct_reduce_input_in_air_factor = 0.2; // reduce input forces when we are in air
	var cct_underwater_no_air_damage = 10; // damage thaken underwater, when out of air
	var cct_fall_damage_limit = 9; // if falling limit is greater than this value - take damage
	var cct_fall_damage_const = 50; // used for falling damage calculations
	
	var cct_interact_distance = 64; // interaction distance (both front and downwards)
	
	var cct_allow_cheap_edge_climb = false; // if true, player will be able to climb edges :D
	
	// cct main structure
	typedef struct {
		
		VECTOR force;
		VECTOR abs_force;
		
		VECTOR dist;
		VECTOR abs_dist;
		
		VECTOR velocity;
		VECTOR push_force;
		
		VECTOR surface_normal;
		VECTOR surface_speed;
		
		VECTOR origin;
		VECTOR water_reg_min;
		VECTOR water_reg_max;
		
		VECTOR water_out_trace_mid;
		VECTOR water_out_trace_top;
		
		VECTOR interact_front_pos;
		VECTOR interact_down_pos;
		
		var bbox_x;
		var bbox_y;
		var bbox_z;
		
		var ground_info;
		
		var foot_height;
		var foot_check_length;
		var foot_step_height;
		var soil_height;
		var soil_allow_sliding;
		var is_grounded;
		var is_hit_ceiling;
		
		var jump_allowed;
		var jump_out_of_water;
		var jump_height;
		
		// input keys
		var forward;
		var backward;
		var straif_left;
		var straif_right;
		var jump;
		var run;
		var dive;
		var interact;
		
		var movement_speed;
		var friction;
		var falling_timer;
		var falling_damage_limit;
		var land_timer;
		var land_timer_limit;
		
		var always_run;
		
		var walk_speed_factor;
		var run_speed_factor;
		
		var water_state;
		var water_reg_total;
		var water_origin_diff_z;
		var water_z_height;
		var water_depth_factor;
		
		var water_mid_trace_result;
		var water_top_trace_result;
		var water_in_switch;
		var water_out_switch;
		
		var air_underwater;
		var air_underwater_timer;
		var air_underwater_timer_limit;
		var air_underwater_snd_gasp_switch;
		
		var swim_speed;
		var swim_z_force;
		var swim_z_speed;
		var swim_z_abs_dist;
		
		var water_z_distance;
		var water_z_speed;
		var water_z_is_moving;
		var water_z_snd_switch;
		
		var moving_distance;
		var moving_speed;
		var is_moving;
		
		var interact_switch;
		var interact_hit_front;		
	} CCT;
	
	// register and initializes new cct (character controller)
	// and save it's pointer into given entity's obj_cct_struct skill
	CCT *register_cct(ENTITY *ent);
	
	// initialize all major variables for the given cct
	// this function called inside of register_cct right after creating new cct
	void init_cct(CCT *cct);
	
	// returns pointer of the cct from given entity's obj_cct_struct skill
	CCT *get_cct(ENTITY *ent);
	
	// removes cct from the memory
	// pointer is taken from given entity's obj_cct_struct skill
	void delete_cct(ENTITY *ent);
	
	// remove given cct from the memory
	void delete_cct(CCT *cct);
	
	#include "cct_helper.h"
	#include "cct_water_detection.h"
	#include "cct_movement.h"
	#include "cct.c"
#endif