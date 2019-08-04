#include "props.h"
#include "interaction.h"

// function to update given door
void door_update(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't update this door, it doesn't exist");
		return;
	}
	
	// get props struct
	PROPS *props = get_props(ent);
	
	if(interaction_was_triggered(ent) && ent->obj_state == IDLE)
	{
		bool enable_interaction = true;
		// if this door needs to be triggered by switch or trigger zone, don't interact with it
		if(!is(ent, use_switch) && !is(ent, use_trigger))
		{
			// requires a key ?
			if(is(ent, red_key) || is(ent, yellow_key) || is(ent, blue_key)){
				
				ENTITY * source = interaction_get_source(ent);

				// interaction from player ?
				if(source->obj_type == TYPE_PLAYER){
					
					// no keys ?
					if(door_player_has_key(ent) == false){
												
						// play no keycard sound here !
						if(snd_playing(source->obj_snd_handle)){ snd_stop(source->obj_snd_handle); }
						source->obj_snd_handle = snd_play(player_no_use_ogg, player_snd_volume, 0);

						enable_interaction = false;
					}
					else{
						
						// reset our key
						if(is(ent, red_key)){ reset(ent, red_key); }
						if(is(ent, yellow_key)){ reset(ent, yellow_key); }
						if(is(ent, blue_key)){ reset(ent, blue_key); }
					}
				}
				else{
					
					// ignore npcs
					return;
				}			
			}
			
			if(enable_interaction)
			{
				// toggle ?
				if(is(ent, toggleable)){
					
					// toggle open/close states
					ent->obj_allow_move += 1;
					ent->obj_allow_move = cycle(ent->obj_allow_move, 1, 3);
					ent->obj_state = ent->obj_allow_move;
					
				} // if once ? then only open
				else{ ent->obj_state = OPEN; }
			}
		}
	}

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
var door_player_has_key(ENTITY * ent){
	
	if(is(ent, red_key) && player_has_red_key == false){ return false; }
	if(is(ent, yellow_key) && player_has_yellow_key == false){ return false; }
	if(is(ent, blue_key) && player_has_blue_key == false){ return false; }
	
	return true;
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

	interaction_enable(my);
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