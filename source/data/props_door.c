#include "props.h

// function to update given door
void door_update(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't update this door, it doesn't exist");
		return;
	}
	
	// get props struct
	PROPS *props = get_props(ent);
	
	// state machine
	switch(ent->obj_state){
		
		case(OPEN):{
			
			// save this state
			props->old_state = OPEN;
			
			// still can move ?
			if(vec_dist(&ent->x, &ent->offset_x_) > 0.5){
				
				// move to target position
				vec_diff(&props->diff, &ent->offset_x_, &ent->x);
				var spd = props->movement_speed * time_step;
				if(vec_length(&props->diff) > spd){
					
					vec_normalize(&props->diff, spd);
				}
				vec_add(&ent->x, &props->diff);
				
				// play start sound
				// armor skill used as a switch, to play sound once
				if(ent->obj_armor == 0){
					
					if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
					ent->obj_snd_handle = ent_playsound(ent, door_01_open_ogg, props_door_snd_volume);
					ent->obj_armor = 1;
				}
			}
			else{
				
				// stoped sound
				if(ent->obj_armor == 1){
					
					if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
					ent->obj_snd_handle = ent_playsound(ent, door_01_close_ogg, props_door_snd_volume);
					ent->obj_armor = 0;
				}
				
				// set proper position
				vec_set(&ent->x, &ent->offset_x_);
				
				// switch to delay
				ent->obj_state = DELAY;
				ent->obj_timer = props->delay_time;
			}
			
			break;
		}
		
		case(CLOSE):{
			
			// save this state
			props->old_state = CLOSE;
			
			if(vec_dist(&ent->x, &props->origin) > 0.5){
				
				vec_diff(&props->diff, &props->origin, &ent->x);
				var spd = props->movement_speed * time_step;
				if(vec_length(&props->diff) > spd){
					
					vec_normalize(&props->diff, spd);
				}
				c_ignore(PUSH_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
				c_move(ent, nullvector, &props->diff, IGNORE_WORLD | IGNORE_YOU | IGNORE_PUSH | MOVE_FLAGS);
				
				// play start sound
				// armor skill used as a switch, to play sound once
				if(ent->obj_armor == 0){
					
					if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
					ent->obj_snd_handle = ent_playsound(ent, door_01_open_ogg, props_door_snd_volume);
					ent->obj_armor = 1;
				}
				
				// when almost closed ?
				// and the gap is smaller than the biggest bbox ?
				props_almost_closed_check(ent, props);
			}
			else{
				
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

// check if player has needed key
var door_player_has_key(){
	
	if(is(my, red_key) && player_has_red_key == false){ return false; }
	if(is(my, yellow_key) && player_has_yellow_key == false){ return false; }
	if(is(my, blue_key) && player_has_blue_key == false){ return false; }
	
	return true;
}

// event function for the door
void door_event(){
	
	if(event_type == EVENT_SHOOT){
		
		// not interacting with us ? then ignore !
		if(you->obj_c_indicator != INTERACT){ return; }
		
		// already moving ? ingore !
		if(my->obj_state != IDLE){ return; }
		
		// if this door needs to be triggered by switch or trigger zone, don't interact with it
		if(is(my, use_switch) || is(my, use_trigger)){ return; }
		
		// requires a key ?
		if(is(my, red_key) || is(my, yellow_key) || is(my, blue_key)){
			
			// interaction from player ?
			if(you->obj_type == TYPE_PLAYER){
				
				// no keys ?
				if(door_player_has_key() == false){
					
					// don't allow to interact
					my->emask &= ~ENABLE_SHOOT;
					
					// play no keycard sound here !
					if(snd_playing(you->obj_snd_handle)){ snd_stop(you->obj_snd_handle); }
					you->obj_snd_handle = snd_play(player_no_use_ogg, player_snd_volume, 0);
					
					// show message, that this door requires a key
					
					// 1/2 second delay
					var counter = 0.5;
					while(my){
						
						counter -= time_frame / 16;
						if(counter <= 0){ break; }
						wait(1);
					}
					
					my->emask |= (ENABLE_SHOOT);
					
					return;	
				}
				else{
					
					// reset our key
					if(is(my, red_key)){ reset(my, red_key); }
					if(is(my, yellow_key)){ reset(my, yellow_key); }
					if(is(my, blue_key)){ reset(my, blue_key); }
				}
			}
			else{
				
				// ignore npcs
				return;
			}			
		}
		
		// toggle ?
		if(is(my, toggleable)){
			
			// toggle open/close states
			my->obj_allow_move += 1;
			my->obj_allow_move = cycle(my->obj_allow_move, 1, 3);
			my->obj_state = my->obj_allow_move;
			
		} // if once ? then only open
		else{ my->obj_state = OPEN; }
	}
}

void props_door_init(ENTITY * ent, int sound_fx)
{
	PROPS *props = register_props(my);
	
	props->movement_speed = 5;
	props->delay_time = 5;
	
	if(is(my, toggleable)){ props->delay_time = 0.5; }
	
	vec_fill(&my->scale_x, 1);
	wait(1);
	c_setminmax(my);
	set(my, POLYGON);
	
	my->group = OBSTACLE_GROUP;
	my->push = OBSTACLE_GROUP;
	
	my->obj_state = IDLE;
	my->obj_type = TYPE_DOOR;
	my->obj_move_npc = true;
	my->obj_pain_type = sound_fx; // default snd fx
	
	// dirty hack
	props_offset_trim(my);
	
	my->emask |= (ENABLE_SHOOT);
	my->event = door_event;
}

// simple door action
// uses: id, offset_x_, offset_y_, offset_z_, red_key, yellow_key, blue_key, toggleable, use_switch, use_trigger
action props_door_a(){
	
	props_door_init(me, 0);
}

// simple door action
// uses: id, offset_x_, offset_y_, offset_z_, red_key, yellow_key, blue_key, toggleable, use_switch, use_trigger
action props_door_b(){
	
	props_door_init(me, 1);
}