
// function to update given elevator
void elevator_update(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't update this elevator, it doesn't exist");
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
				c_ignore(PUSH_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
				c_move(ent, nullvector, &props->diff, IGNORE_WORLD | IGNORE_YOU | IGNORE_PUSH | MOVE_FLAGS);
				
				// play start sound
				// armor skill used as a switch, to play sound once
				if(ent->obj_armor == 0){
					
					if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
					ent_playsound(ent, elevator_start_ogg, props_elevator_snd_volume);
					ent->obj_snd_handle = ent_playloop(ent, elevator_mid_ogg, props_elevator_snd_loop_volume);
					ent->obj_armor = 1;
				}
				
				// when almost opened ?
				// and the gap is smaller than the biggest bbox ?
				props_almost_opened_check(ent);
			}
			else{
				
				// stoped sound
				if(ent->obj_armor == 1){
					
					if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
					ent->obj_snd_handle = ent_playsound(ent, elevator_stop_ogg, props_elevator_snd_volume);
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
					ent_playsound(ent, elevator_start_ogg, props_elevator_snd_volume);
					ent->obj_snd_handle = ent_playloop(ent, elevator_mid_ogg, props_elevator_snd_loop_volume);
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
					ent->obj_snd_handle = ent_playsound(ent, elevator_stop_ogg, props_elevator_snd_volume);
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

// event function for the elevator
void elevator_event(){
	
	if(event_type == EVENT_SHOOT){
		
		// not interacting with us ? then ignore !
		if(you->obj_c_indicator != INTERACT){ return; }
		
		// already moving ? ingore !
		if(my->obj_state != IDLE){ return; }
		
		// if this elevator needs to be triggered by switch or trigger zone, don't interact with it
		if(is(my, use_switch) || is(my, use_trigger)){ return; }
		
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

// simple elevator action
// uses: id, offset_x_, offset_y_, offset_z_, toggleable, use_switch, use_trigger
action props_elevator(){
	
	PROPS *props = register_props(my);
	
	props->movement_speed = 5;
	props->delay_time = 3;
	
	if(is(my, toggleable)){ props->delay_time = 0.5; }
	
	vec_fill(&my->scale_x, 1);
	wait(1);
	c_setminmax(my);
	set(my, POLYGON);
	
	my->group = OBSTACLE_GROUP;
	my->push = OBSTACLE_GROUP;
	
	my->obj_state = IDLE;
	my->obj_type = TYPE_ELEVATOR;
	my->obj_move_npc = true;
	
	// dirty hack
	props_offset_trim(my);
	
	my->emask |= (ENABLE_SHOOT);
	my->event = elevator_event;
}