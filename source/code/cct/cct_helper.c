
// handle interaction traces
void ent_interact(ENTITY *ent, CCT *cct)
{
    if (!ent)
    {
        diag("\nERROR! Interaction ent, doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! Interaction cct, doesn't exist!");
        return;
    }

    VECTOR start_pos;
    vec_set(&start_pos, &cct->origin);
    if (ent->OBJ_TYPE == TYPE_PLAYER)
    {
        vec_set(&start_pos, &camera->x);
    }
    // pressed interaction button ?
    if (cct->interact == true && cct->interact_switch == 0)
    {
        // make sure to note that this is an interaction trace
        ent->OBJ_C_INDICATOR = INTERACT;
        c_ignore(PUSH_GROUP, PLAYER_GROUP, PATHFIND_GROUP, ENEMY_GROUP, 0);
        ent_trace(ent, &start_pos, &cct->interact_front_pos, ACTIVATE_SHOOT);
        cct->interact_hit_front = false;

        if (HIT_TARGET)
        {
            if (you != NULL)
            {
                // we've detected a switch, elevator, platform or a door ?
                if (you->OBJ_TYPE == TYPE_SWITCH || you->OBJ_TYPE == TYPE_ELEVATOR || you->OBJ_TYPE == TYPE_PLATFORM || you->OBJ_TYPE == TYPE_DOOR)
                {
                    cct->interact_hit_front = true;
                }
            }
        }

        // if front trace failed ?
        if (cct->interact_hit_front == false)
        {
            // make sure to note that this is an interaction trace
            ent->OBJ_C_INDICATOR = INTERACT;
            c_ignore(PUSH_GROUP, PLAYER_GROUP, PATHFIND_GROUP, ENEMY_GROUP, 0);
            ent_trace(ent, &start_pos, &cct->interact_down_pos, ACTIVATE_SHOOT);
        }

        cct->interact_switch = 1;
    }
    else
    {
        cct->interact_switch = 0;
    }
}

// update interaction trace positions
void ent_interact_trace_pos(ENTITY *ent, CCT *cct)
{
    if (!ent)
    {
        diag("\nERROR! Interaction ent, doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! Interaction cct, doesn't exist!");
        return;
    }

    // front trace end position
    vec_set(&cct->interact_front_pos, vector(cct_interact_distance, 0, 0));

    if (ent->OBJ_TYPE == TYPE_PLAYER)
    {
        vec_rotate(&cct->interact_front_pos, &camera->pan);
        vec_add(&cct->interact_front_pos, &camera->x);
    }
    else
    {
        vec_rotate(&cct->interact_front_pos, vector(ent->pan, 0, 0));
        vec_add(&cct->interact_front_pos, &cct->origin);
    }

    // down trace end position
    vec_set(&cct->interact_down_pos, vector(0, 0, -cct_interact_distance));
    vec_rotate(&cct->interact_down_pos, vector(ent->pan, 0, 0));
    vec_add(&cct->interact_down_pos, &cct->origin);
}

// function used for stopping movement (velocity, speed, forces etc)
void ent_foot_stop_velocity(var is_x, ENTITY *ent, CCT *cct, VECTOR *hit_pos)
{
    if (!ent)
    {
        diag("\nERROR! Interaction ent, doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! Interaction cct, doesn't exist!");
        return;
    }

    // moving on X coordinates ?
    if (is_x == true)
    {
        if (hit_pos->x < ent->x)
        {
            cct->force.x = maxv(cct->force.x, 0);
            cct->velocity.x = maxv(cct->velocity.x, 0);
            cct->dist.x = maxv(cct->dist.x, 0);
            cct->push_force.x = maxv(cct->push_force.x, 0);
        }
        else
        {
            cct->force.x = minv(cct->force.x, 0);
            cct->velocity.x = minv(cct->velocity.x, 0);
            cct->dist.x = minv(cct->dist.x, 0);
            cct->push_force.x = minv(cct->push_force.x, 0);
        }
    }

    if (is_x == false)
    {
        if (hit_pos->y < ent->y)
        {
            cct->force.y = maxv(cct->force.y, 0);
            cct->velocity.y = maxv(cct->velocity.y, 0);
            cct->dist.y = maxv(cct->dist.y, 0);
            cct->push_force.y = maxv(cct->push_force.y, 0);
        }
        else
        {
            cct->force.y = minv(cct->force.y, 0);
            cct->velocity.y = minv(cct->velocity.y, 0);
            cct->dist.y = minv(cct->dist.y, 0);
            cct->push_force.y = minv(cct->push_force.y, 0);
        }
    }
}

// perform foot check for given entity
// it's used as a workaround for aweful ellipsoid hull that OBB uses in Acknex
void ent_foot_check(ENTITY *ent, CCT *cct)
{
    if (!ent)
    {
        diag("\nERROR! Interaction ent, doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! Interaction cct, doesn't exist!");
        return;
    }

    // x coordinates
    VECTOR check_vec;
    vec_set(&check_vec, vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset));
    check_vec.x += sign(cct->force.x) + (cct->foot_check_length * sign(cct->force.x));

    c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
    ent_trace(ent, vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset), &check_vec, IGNORE_YOU | USE_BOX);
    if (HIT_TARGET && abs(normal.z) < cct_slope_fac && (hit->z - (ent->z + ent->min_z)) > cct->foot_step_height)
    {
        ent_foot_stop_velocity(true, ent, cct, &hit->x);
    }

    // y coordinates
    vec_set(&check_vec, vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset));
    check_vec.y += sign(cct->force.y) + (cct->foot_check_length * sign(cct->force.y));

    c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
    ent_trace(ent, vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset), &check_vec, IGNORE_YOU | USE_BOX);
    if (HIT_TARGET && abs(normal.z) < cct_slope_fac && (hit->z - (ent->z + ent->min_z)) > cct->foot_step_height)
    {
        ent_foot_stop_velocity(false, ent, cct, &hit->x);
    }

    // different step height in air and on ground
    if (cct->is_grounded == true)
    {
        cct->foot_step_height = cct_def_foot_step_height;
    }
    else
    {
        cct->foot_step_height = 0;
    }
}

// checks if given cct is inside of the given entity
// returns true if true, else if not
var is_cct_in_rect(ENTITY *cct, ENTITY *rect, var scale)
{
	if (!cct)
	{
		diag("\nERROR! Can't perform cct_vs_rect check! CCT entity doesn't exist!");
		return;
	}

	if (!rect)
	{
		diag("\nERROR! Can't perform cct_vs_rect check! Rect entity doesn't exist!");
		return;
	}

	VECTOR pos;
	vec_diff(&pos, &cct->x, &rect->x);
	vec_rotateback(&pos, &rect->pan);
	vec_add(&pos, &rect->x);

	var delta_x = pos.x - maxv(rect->x + (rect->min_x * scale), minv(pos.x, rect->x + (rect->max_x * scale)));
	var delta_y = pos.y - maxv(rect->y + (rect->min_y * scale), minv(pos.y, rect->y + (rect->max_y * scale)));
	var rect_delta = (delta_x * delta_x) + (delta_y * delta_y);

	var cct_top = cct->z + (cct->max_z * scale);
	var cct_bottom = cct->z + (cct->min_z * scale);
	var cct_z_hit = false;

	if ((rect->z + rect->min_z) - cct_top > 0 || cct_bottom - (rect->z + rect->max_z) > 0)
	{
		cct_z_hit = true;
	}

	if (rect_delta < (cct->max_x * cct->max_x) && cct_z_hit == false)
	{
		return true;
	}

	return false;
}