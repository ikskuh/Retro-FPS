#include "props.h"
#include "defines.h"
#include "interaction.h"

// move platform
var platform_move(ENTITY *ent, PROPS *props){
	
	// play start sound
	// armor skill used as a switch, to play sound once
	if(ent->obj_armor == 0){
		
		if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
		ent_playsound(ent, platform_start_ogg, props_platform_snd_volume);
		ent->obj_snd_handle = ent_playloop(ent, platform_mid_ogg, props_platform_snd_loop_volume);
		ent->obj_armor = 1;
	}
	
	VECTOR node_pos_vec;
	if(path_getnode(ent, props->path_id, &node_pos_vec, NULL) == 0){
		
		props->dir *= -1;
		props->path_id += props->dir;
		path_getnode(ent, props->path_id, &node_pos_vec, NULL);
		
		// check if we need to kill player
		
		return false;
	}
	
	// perform smash check when close to any node !
	// because there might be edges and turns
	if(vec_dist(&ent->x, &node_pos_vec) <= biggest_bbox_z_side){
		
		// check for smashing !
		ent->obj_check = true;
	}
	else{
		
		// if too far, then don't check
		ent->obj_check = false;
	}
	
	props->dist = vec_dist(&ent->x, &node_pos_vec);
	vec_diff(&props->diff, &node_pos_vec, &ent->x);
	vec_normalize(&props->diff,  minv(props->movement_speed * time_step, props->dist));
	props->diff.z = 0;
	
	c_ignore(PUSH_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
	c_move(ent, nullvector, &props->diff, IGNORE_WORLD | IGNORE_YOU | IGNORE_PUSH | MOVE_FLAGS);
	
	// get next node
	if(props->dist < 1 * time_step){ props->path_id += props->dir; }
	
	return true;
}

// update this platform
void platform_update(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't update this platform, it doesn't exist");
		return;
	}
	
	if(interaction_was_triggered(ent))
	{
			
		// already moving ? ingore !
		if((ent->obj_state == IDLE) && !is(ent, use_switch) && !is(ent, use_trigger))
		{
			// toggle ?
			if(is(ent, toggleable)){
				
				// toggle open/close states
				ent->obj_allow_move += 1;
				ent->obj_allow_move = cycle(ent->obj_allow_move, 1, 3);
				ent->obj_state = ent->obj_allow_move;
				
			} // if once ? then only open
			else{ ent->obj_state = DELAY; }
		}
	}

	// get props struct
	PROPS *props = get_props(ent);
	
	// state machine
	switch(ent->obj_state){
		
		case(OPEN):{
			
			// save this state
			props->old_state = OPEN;
			
			// reached the end ?
			if(platform_move(ent, props) == false){
				
				// stoped sound
				if(ent->obj_armor == 1){
					
					if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
					ent->obj_snd_handle = ent_playsound(ent, door_01_close_ogg, props_door_snd_volume);
					ent->obj_armor = 0;
				}
				
				// set proper position
				vec_set(&ent->x, &ent->offset_x_);
				
				ent->obj_timer = props->delay_time;
				ent->obj_state = DELAY;
			}
			
			break;
		}
		
		case(CLOSE):{
			
			// save this state
			props->old_state = CLOSE;
			
			// reached the start ?
			if(platform_move(ent, props) == false){
				
				// stoped sound
				if(ent->obj_armor == 1){
					
					if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
					ent->obj_snd_handle = ent_playsound(ent, door_01_close_ogg, props_door_snd_volume);
					ent->obj_armor = 0;
				}
				
				// set proper position
				vec_set(&ent->x, &props->origin);
				
				if(is(ent, toggleable)){
					
					// switch to delay
					ent->obj_state = DELAY;
					ent->obj_timer = props->delay_time;
				}
				else{
					
					// switch to delay
					ent->obj_state = IDLE;
					
					// if we used switch, enable them all again
					if(is(ent, use_switch)){
						
						switch_enable_by_id(ent->id);
					}
				}
			}
			
			break;
		}
		
		case(DELAY):{
			
			// reset movement
			vec_fill(&props->diff, 0);
			
			// timer part
			ent->obj_timer -= time_frame / 16;
			if(ent->obj_timer <= 0){
				
				// reset check
				ent->obj_check = false;
				
				if(is(ent, toggleable)){
					
					ent->obj_state = IDLE;
					
					// if we used switch, enable them all again
					if(is(ent, use_switch)){
						
						switch_enable_by_id(ent->id);
					}
				}
				else{ ent->obj_state = CLOSE; }
			}
			
			break;
		}
	}	
}

// action for moving platform
// uses: id, toggleable, use_switch, use_trigger
action props_platform(){
	
	PROPS *props = register_props(my);
	
	props->movement_speed = 2.5;
	props->delay_time = 1;
	props->dir = 1;
	
	if(is(my, toggleable)){ props->delay_time = 0.5; }
	
	vec_fill(&my->scale_x, 1);
	c_setminmax(my);
	set(my, POLYGON);
	
	my->group = OBSTACLE_GROUP;
	my->push = OBSTACLE_GROUP;
	
	my->obj_state = IDLE;
	my->obj_type = TYPE_PLATFORM;
	my->obj_move_npc = true;
	
	// dirty hack
	props_offset_trim(my);
	
	interaction_enable(me);
	
	// find a path near by and attach us to it
	props->path_id = path_scan(me, &my->x, &my->pan, vector(360, 180, 128));
	if(props->path_id == 0){
		
		diag("\nERROR! Platform can't find any path!");
		return;
	}
	path_getnode(my, props->path_id, &my->x, NULL);
	
	// find total amount of nodes
	int i = 0;
	for(i = 0; i < 9; i++){
		
		props->total_nodes = i + 1;
		var res = path_nextnode(my, i + 1, 1);
		if(res == 0){ break; }
	}
	
	// save start and end positions
	path_getnode(my, 1, &props->origin, NULL);
	path_getnode(my, props->total_nodes, &my->offset_x_, NULL);
}