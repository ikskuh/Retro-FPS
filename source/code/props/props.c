
// register and initializes new props (f.e. doors, platforms, elevators)
// and save it's pointer into given entity's obj_struct skill
PROPS *register_props(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't register PROPS, given entity doesn't exist");
		return;
	}
	
	PROPS *props = sys_malloc(sizeof(PROPS));
	init_props(ent, props);
	
	ent->obj_struct = props;
	return props;
}

// initialize all major variables for the given props
// this function called inside of register_props right after creating new props
void init_props(ENTITY *ent, PROPS *props){
	
	if(!ent){
		
		diag("\nERROR! Can't init PROPS, given entity doesn't exist");
		return;
	}
	
	if(!props){
		
		diag("\nERROR! Can't init PROPS, because it doesn't exist");
		return;
	}
	
	vec_set(&props->origin, &ent->x);
	props->movement_speed = 5; // default speed
	props->delay_time = 3; // 3 seconds, default time
}

// returns pointer of the props from given entity's obj_struct skill
PROPS *get_props(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't get PROPS, given entity doesn't exist");
		return;
	}
	
	PROPS *props = ent->obj_struct;
	return props;
}

// removes props from the memory
// pointer is taken from given entity's obj_struct skill
void delete_props(ENTITY *ent){
	
	PROPS *props = get_props(ent);
	sys_free(props);
}

// remove given props from the memory
void delete_props(PROPS *props){
	
	sys_free(props);
}
