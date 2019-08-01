
// register and initializes new player's structure
// and save it's pointer into given entities obj_struct skill
void *register_player_struct(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't register player structure, given entity doesn't exist");
		return;
	}
	
	hero = sys_malloc(sizeof(PLAYER));
	init_player_struct(ent, hero);
	
	ent->obj_struct = hero;
	return hero;
}

// initialize all major variables for the given player's structure
// this function called inside of register_player_struct right after creating new one
void init_player_struct(ENTITY *ent, PLAYER *temp){
	
	if(!temp){
		
		diag("\nERROR! Can't init player structure, because it doesn't exist");
		return;
	}
	temp->cam = sys_malloc(sizeof(CAMERA));
	vec_set(&temp->cam->angle, &ent->pan);
	temp->cam->height = 10;
	temp->cam->fov = 90;
	temp->cam->third_person_dist = -128;
	temp->cam->bob_z_offset = 1.5;
	temp->cam->bob_speed_factor = 2;
	
	temp->always_run = false;
}

// returns pointer of the player structure from given entity's obj_struct skill
PLAYER *get_player_struct(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't get player structure, given entity doesn't exist");
		return;
	}
	
	PLAYER *temp = ent->obj_struct;
	return temp;
}

// removes player from the memory
// pointer is taken from given entity's obj_struct skill
void delete_player_struct(ENTITY *ent){
	
	PLAYER *temp = get_player_struct(ent);
	sys_free(temp->cam);
	sys_free(temp);
}

// remove given player from the memory
void delete_player_struct(PLAYER *temp){
	
	sys_free(temp->cam);
	sys_free(temp);
}