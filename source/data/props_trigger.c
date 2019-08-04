
// trigger props with the same id
void trigger_props(){
	
	ENTITY *temp_ent = NULL;			
	for(temp_ent = ent_next(NULL); temp_ent; temp_ent = ent_next(temp_ent)){
		
		// props
		if(temp_ent->obj_type == TYPE_ELEVATOR || temp_ent->obj_type == TYPE_PLATFORM || temp_ent->obj_type == TYPE_DOOR || temp_ent->obj_type == TYPE_SECRET_WALL){
			
			// not the same id OR not using trigger ? ignore
			if(temp_ent->id != my->id || !is(temp_ent, use_trigger)){ continue; }
			
			// already triggered ? ignore
			if(temp_ent->obj_state != IDLE){ continue; }
			
			// if you were made to be triggered
			temp_ent->obj_allow_move += 1;
			temp_ent->obj_allow_move = cycle(temp_ent->obj_allow_move, 1, 3);
			temp_ent->obj_state = temp_ent->obj_allow_move;
		}
	}
	
	wait(1);
	ent_delete(my);
}

// event function for trigger zone
void trigger_event(){
	
	if(event_type == EVENT_PUSH){
		
		// player is that you ?
		if(you->obj_type != TYPE_PLAYER){ return; }
		
		// only once !
		my->emask &= ~ENABLE_PUSH;
		
		// trigger props with the same id
		trigger_props();
	}
}

// simple trigger zone action
// uses: id
action props_trigger_zone(){
	
	vec_fill(&my->scale_x, 1);
	wait(1);
	c_setminmax(my);
	set(my, POLYGON | TRANSLUCENT);
	
	my->group = PUSH_GROUP;
	my->push = PUSH_GROUP;
	
	my->obj_type = TYPE_TRIGGER_ZONE;
	
	my->emask |= (ENABLE_PUSH);
	my->event = trigger_event;
}