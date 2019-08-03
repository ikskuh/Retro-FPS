
// enable all switches with the given id
void switch_enable_by_id(var num){
	
	ENTITY *temp_ent = NULL;			
	for(temp_ent = ent_next(NULL); temp_ent; temp_ent = ent_next(temp_ent)){
		
		if(temp_ent->obj_type == TYPE_SWITCH){
			
			// if it needs to be enabled only once, then ignore !
			if(is(temp_ent, use_once)){ continue; }
			
			// same id? then disable it
			if(temp_ent->id == num){
				
				temp_ent->emask |= (ENABLE_SHOOT);
			}
		}
	}
}

// disable all switches with the given id
void switch_disable_by_id(var num){

	ENTITY *temp_ent = NULL;			
	for(temp_ent = ent_next(NULL); temp_ent; temp_ent = ent_next(temp_ent)){
		
		if(temp_ent->obj_type == TYPE_SWITCH){
			
			// same id? then disable it
			if(temp_ent->id == num){
				
				temp_ent->emask &= ~ENABLE_SHOOT;
			}
		}
	}	
}

// function used to trigger props with the same id
void switch_trigger_props(){
	
	ENTITY *temp_ent = NULL;			
	for(temp_ent = ent_next(NULL); temp_ent; temp_ent = ent_next(temp_ent)){
		
		// props
		if(temp_ent->obj_type == TYPE_ELEVATOR || temp_ent->obj_type == TYPE_PLATFORM || temp_ent->obj_type == TYPE_DOOR || temp_ent->obj_type == TYPE_SECRET){
			
			// not the same id OR not using switch ? ignore
			if(temp_ent->id != my->id || !is(temp_ent, use_switch_id)){ continue; }
			
			// already triggered ? ignore
			if(temp_ent->obj_state != IDLE){ continue; }
			
			// if you were made to be triggered
			if(!is(temp_ent, use_once)){
				
				// toggle between open and close states
				temp_ent->obj_allow_move += 1;
				temp_ent->obj_allow_move = cycle(temp_ent->obj_allow_move, 1, 3);
				temp_ent->obj_state = temp_ent->obj_allow_move;
			}
			else{
				
				// other ways, open !
				temp_ent->obj_state = OPEN;
			}
		}
	}
}

// switch's event function
void switch_event(){
	
	if(event_type == EVENT_SHOOT){
		
		// only player can trigger switches ?
		if(you->obj_type == TYPE_PLAYER){
			
			// not interacting with us ? then ignore !
			if(you->obj_c_indicator != INTERACT){ return; }
			
			// disable all switches with the same id
			switch_disable_by_id(my->id);
			
			// play switch sound
			if(snd_playing(my->obj_snd_handle)){ snd_stop(my->obj_snd_handle); }
			my->obj_snd_handle = ent_playsound(my, switch_ogg, props_switch_snd_volume);
			
			// 1 second delay
			var counter = 1;
			while(my){
				
				counter -= time_frame / 16;
				if(counter <= 0){ break; }
				wait(1);
			}
			
			// trigger props with the same id
			switch_trigger_props();
		}
	}	
}

// switch to trigger other props
// uses: id, use_once
action props_switch(){
	
	vec_fill(&my->scale_x, 1);
	c_setminmax(my);
	set(my, POLYGON);
	
	my->group = SWITCH_ITEM_GROUP;
	my->push = SWITCH_ITEM_GROUP;

	my->obj_type = TYPE_SWITCH;
	
	my->emask |= (ENABLE_SHOOT);
	my->event = switch_event;
}