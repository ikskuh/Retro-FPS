#ifndef _CCT_H_
#define _CCT_H_

// This is basic character controller (cct) header
// it's used for registering, deleting and contains all cct parameters
//
// Info about other cct related headers
// - cct_helper.h (everything needed for proper cct behaviour)
// - cct_water_detection.h (everything needed to perform water detection)
// - cct_movement.h (everything needed to perform all movement)

// cct main structure
typedef struct CCT
{
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

	VECTOR bbox_min_vec;

	var bbox_x;
	var bbox_y;
	var bbox_z;

	var pan_rotation;
	var ground_info;

	var foot_height;
	var foot_check_length;
	var foot_step_height;
	var soil_height;
	var soil_contact;
	var soil_allow_sliding;
	var height_to_ground;
	var is_grounded;
	var is_hit_ceiling;

	var jump_allowed;
	var jump_out_of_water;
	var jump_height;

	// input keys
	var forward;
	var backward;
	var strafe_left;
	var strafe_right;
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

#endif