
#include "interaction.h"

// handle interaction traces
void ent_interact(ENTITY *ent, CCT *cct){
	
	VECTOR start_pos;
	vec_set(&start_pos, &cct->origin);
	if(ent->obj_type == TYPE_PLAYER){
		
		vec_set(&start_pos, &camera->x);
	}
	

	// pressed interaction button ?
	if(cct->interact == true && cct->interact_switch == 0){
		
		// make sure to note that this is an interaction trace
		ent->obj_c_indicator = INTERACT;
		c_ignore(PUSH_GROUP, PLAYER_GROUP, PATHFIND_GROUP, ENEMY_GROUP, 0);
		c_trace(&start_pos, &cct->interact_front_pos, TRACE_FLAGS);
		cct->interact_hit_front = false;
		
		if(HIT_TARGET){
			if(you != NULL){
				if(interaction_is_enabled(you)) {
					interaction_do(you, ent);
					cct->interact_hit_front = true;
				}
			}
		}
		
		// if front trace failed ?
		if(cct->interact_hit_front == false){
			
			// make sure to note that this is an interaction trace
			ent->obj_c_indicator = INTERACT;
			c_ignore(PUSH_GROUP, PLAYER_GROUP, PATHFIND_GROUP, ENEMY_GROUP, 0);
			c_trace(&start_pos, &cct->interact_down_pos, TRACE_FLAGS);
			if(HIT_TARGET) {
				if(you != NULL) {
					if(interaction_is_enabled(you)) {
						interaction_do(you, ent);
					}
				}
			}
		}
		
		cct->interact_switch = 1;
	}
	else{ cct->interact_switch = 0; }
}

// update interaction trace positions
void ent_interact_trace_pos(ENTITY *ent, CCT *cct){
	
	// front trace end position
	vec_set(&cct->interact_front_pos, vector(cct_interact_distance, 0, 0));
	
	if(ent->obj_type == TYPE_PLAYER){
		
		vec_rotate(&cct->interact_front_pos, &camera->pan);
		vec_add(&cct->interact_front_pos, &camera->x);
	}
	else{
		
		vec_rotate(&cct->interact_front_pos, vector(ent->pan, 0, 0));
		vec_add(&cct->interact_front_pos, &cct->origin);
	}
	
	// down trace end position
	vec_set(&cct->interact_down_pos, vector(0, 0, -cct_interact_distance));
	vec_rotate(&cct->interact_down_pos, vector(ent->pan, 0, 0));
	vec_add(&cct->interact_down_pos, &cct->origin);
}

// function used for stopping movement (velocity, speed, forces etc)
void ent_foot_stop_velocity(var is_x, ENTITY *ent, CCT *cct, VECTOR *hit_pos){
	
	if(is_x == true){
		
		if(hit_pos->x < ent->x){
			
			cct->force.x = maxv(cct->force.x, 0);
			cct->velocity.x = maxv(cct->velocity.x, 0);
			cct->dist.x = maxv(cct->dist.x, 0);
			cct->push_force.x = maxv(cct->push_force.x, 0);
		}
		else{
			
			cct->force.x = minv(cct->force.x, 0);
			cct->velocity.x = minv(cct->velocity.x, 0);
			cct->dist.x = minv(cct->dist.x, 0);
			cct->push_force.x = minv(cct->push_force.x, 0);
		}
	}
	
	if(is_x == false){
		
		if(hit_pos->y < ent->y){
			
			cct->force.y = maxv(cct->force.y, 0);
			cct->velocity.y = maxv(cct->velocity.y, 0);
			cct->dist.y = maxv(cct->dist.y, 0);
			cct->push_force.y = maxv(cct->push_force.y, 0);
		}
		else{
			
			cct->force.y = minv(cct->force.y, 0);
			cct->velocity.y = minv(cct->velocity.y, 0);
			cct->dist.y = minv(cct->dist.y, 0);
			cct->push_force.y = minv(cct->push_force.y, 0);
		}
	}
}

// performs foot check for given entity
// it's used as a workaround for aweful ellipsoid hull that OBB uses in Acknex
void ent_foot_check(ENTITY *ent, CCT *cct){
	
	if(!ent){
		
		diag("\nERROR! Can't perform foot check, entity doesn't exist!");
		return;
	}
	
	if(!cct){
		
		diag("\nERROR! Can't perform foot check, cct doesn't exist!");
		return;
	}
	
	VECTOR check_vec;
	
	// x coordinates
	vec_set(&check_vec, vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset));
	check_vec.x += sign(cct->force.x) + (cct->foot_check_length * sign(cct->force.x));
	
	c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
	c_trace(vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset), &check_vec, TRACE_FLAGS | IGNORE_YOU | USE_BOX);
	
	if(HIT_TARGET && abs(normal.z) < cct_slope_fac && (hit->z - (ent->z + ent->min_z)) > cct->foot_step_height){
		
		ent_foot_stop_velocity(true, ent, cct, &hit->x);
	}
	
	// y coordinates
	vec_set(&check_vec, vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset));
	check_vec.y += sign(cct->force.y) + (cct->foot_check_length * sign(cct->force.y));
	
	c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
	c_trace(vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset), &check_vec, TRACE_FLAGS | IGNORE_YOU | USE_BOX);
	
	if(HIT_TARGET && abs(normal.z) < cct_slope_fac && (hit->z - (ent->z + ent->min_z)) > cct->foot_step_height){
		
		ent_foot_stop_velocity(false, ent, cct, &hit->x);
	}
	
	// different step height in air and on ground
	if(cct->is_grounded == true){
		
		cct->foot_step_height = cct_def_foot_step_height;
	}
	else{
		
		cct->foot_step_height = 0;
	}
}