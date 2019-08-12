
// set's unused coordinates, to the origin ones
void props_offset_trim(ENTITY *ent)
{
	if (ent->OFFSET_X_ == 0)
	{
		ent->OFFSET_X_ = ent->x;
	}
	if (ent->OFFSET_Y_ == 0)
	{
		ent->OFFSET_Y_ = ent->y;
	}
	if (ent->OFFSET_Z_ == 0)
	{
		ent->OFFSET_Z_ = ent->z;
	}
}

// check if entity needs to be smashed or not (f.e. by door on close etc)
void props_vs_npc_check(ENTITY *prop, ENTITY *npc, var scale)
{
	if (!prop)
	{
		diag("\nERROR! Can't perform props vs npc check, props doesn't exist!");
		return;
	}

	if (!npc)
	{
		diag("\nERROR! Can't perform props vs npc check, npc doesn't exist!");
		return;
	}

	// we need to check for collision ?
	if (prop->OBJ_CHECK == 1)
	{
		// if props smashed npc, then npc should die !!!
		if (is_cct_in_rect(npc, prop, scale) == true)
		{
			npc->OBJ_HEALTH = -100;
			npc->OBJ_DEATH_TYPE = TYPE_SMASHED;
		}
		prop->OBJ_CHECK = 0;
	}
}

// check (for smashing cct) if props almost closed
void props_almost_closed_check(ENTITY *ent, PROPS *props)
{
	var x_distance = abs(ent->x - props->origin.x);
	var y_distance = abs(ent->y - props->origin.y);
	var z_distance = abs(ent->z - props->origin.z);

	if (x_distance <= biggest_bbox_x_side && y_distance <= biggest_bbox_x_side && z_distance <= biggest_bbox_z_side)
	{
		ent->OBJ_CHECK = true;
	}
}

// check (for smashing cct) if props almost opened
void props_almost_opened_check(ENTITY *ent)
{
	var x_distance = abs(ent->x - ent->OFFSET_X_);
	var y_distance = abs(ent->y - ent->OFFSET_Y_);
	var z_distance = abs(ent->z - ent->OFFSET_Z_);

	if (x_distance <= biggest_bbox_x_side && y_distance <= biggest_bbox_x_side && z_distance <= biggest_bbox_z_side)
	{
		ent->OBJ_CHECK = true;
	}
}

// simple gravity for some props
void props_gravity(ENTITY *ent, PROPS *props)
{
	if (!ent)
	{
		diag("\nERROR! No gravity for props! It doesn't exist.");
		return;
	}

	if (!props)
	{
		diag("\nERROR! No gravity for props! It's props strucutre doesn't exist.");
		return;
	}

	VECTOR from, to, target_vec;
	vec_set(&from, &ent->x);
	vec_set(&to, &from);
	to.z -= 64 + ent->min_z;

	if (props->speed.z <= 0)
	{
		c_ignore(PUSH_GROUP, WATER_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
		ent_trace(ent, &from, &to, IGNORE_PUSH | USE_BOX);
		vec_set(&target_vec, &to);
		if (HIT_TARGET)
		{
			vec_set(&target_vec, &target);
		}
		props->soil_height = target_vec.z - ent->min_z;
	}

	// additional height buffer when ground contact to avoid floating on slopes
	if (ent->z > props->soil_height + (5 + 20 * props->soil_contact) * time_step)
	{
		props->soil_contact = 0;
		props->speed.z = maxv(props->speed.z - cct_gravity * time_step, -cct_gravity_max);
	}
	else
	{
		c_ignore(PUSH_GROUP, WATER_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
		ent_move(ent, nullvector, vector(0, 0, props->soil_height - ent->z), IGNORE_YOU | GLIDE);
		props->soil_contact = 1;
		props->speed.z = 0;
	}

	// the maxv command makes sure you never move below the desired height
	if (props->speed.z)
	{
		c_ignore(PUSH_GROUP, WATER_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
		ent_move(ent, nullvector, vector(0, 0, maxv(props->speed.z * time_step, props->soil_height - ent->z)), IGNORE_YOU | GLIDE);
	}
}