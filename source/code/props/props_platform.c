
// move platform
var platform_move(ENTITY *ent, PROPS *props)
{
    if (!ent)
    {
        diag("\nERROR! Can't move platform, it doesn't exist");
        return;
    }

    if (!props)
    {
        diag("\nERROR! Can't move platform, it's structure doesn't exist");
        return;
    }

    // play start sound
    // armor skill used as a switch, to play sound once
    if (ent->OBJ_ARMOR == 0)
    {
        if (snd_playing(ent->OBJ_SND_HANDLE))
        {
            snd_stop(ent->OBJ_SND_HANDLE);
        }
        ent_playsound(ent, platform_start_ogg, props_platform_snd_volume);
        ent->OBJ_SND_HANDLE = ent_playloop(ent, platform_mid_ogg, props_platform_snd_loop_volume);
        ent->OBJ_ARMOR = 1;
    }

    VECTOR node_pos_vec;
    if (path_getnode(ent, props->path_id, &node_pos_vec, NULL) == 0)
    {
        props->dir *= -1;
        props->path_id += props->dir;
        path_getnode(ent, props->path_id, &node_pos_vec, NULL);
        return false;
    }

    // perform smash check when close to any node !
    // because there might be edges and turns
    if (vec_dist(&ent->x, &node_pos_vec) <= biggest_bbox_z_side)
    {
        // check for smashing !
        ent->OBJ_CHECK = true;
    }
    else
    {
        // if too far, then don't check
        ent->OBJ_CHECK = false;
    }

    props->dist = vec_dist(&ent->x, &node_pos_vec);
    vec_diff(&props->diff, &node_pos_vec, &ent->x);
    vec_normalize(&props->diff, minv(props->movement_speed * time_step, props->dist));
    props->diff.z = 0;

    c_ignore(PUSH_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
    ent_move(ent, nullvector, &props->diff, IGNORE_WORLD | IGNORE_YOU | IGNORE_PUSH);

    // get next node
    if (props->dist < 1 * time_step)
    {
        props->path_id += props->dir;
    }

    return true;
}

// update this platform
void platform_update(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! Can't update this platform, it doesn't exist");
        return;
    }

    // get props struct
    PROPS *props = get_props(ent);

    // state machine
    switch (ent->OBJ_STATE)
    {

    case (OPEN):
    {
        // save this state
        props->old_state = OPEN;

        // reached the end ?
        if (platform_move(ent, props) == false)
        {
            // stoped sound
            if (ent->OBJ_ARMOR == 1)
            {
                if (snd_playing(ent->OBJ_SND_HANDLE))
                {
                    snd_stop(ent->OBJ_SND_HANDLE);
                }
                ent->OBJ_SND_HANDLE = ent_playsound(ent, door_01_close_ogg, props_door_snd_volume);
                ent->OBJ_ARMOR = 0;
            }

            // set proper position
            vec_set(&ent->x, &ent->OFFSET_X_);

            ent->OBJ_TIMER = props->delay_time;
            ent->OBJ_STATE = DELAY;
        }

        break;
    }

    case (CLOSE):
    {
        // save this state
        props->old_state = CLOSE;

        // reached the start ?
        if (platform_move(ent, props) == false)
        {
            // stoped sound
            if (ent->OBJ_ARMOR == 1)
            {
                if (snd_playing(ent->OBJ_SND_HANDLE))
                {
                    snd_stop(ent->OBJ_SND_HANDLE);
                }
                ent->OBJ_SND_HANDLE = ent_playsound(ent, door_01_close_ogg, props_door_snd_volume);
                ent->OBJ_ARMOR = 0;
            }

            // set proper position
            vec_set(&ent->x, &props->origin);

            if (is(ent, TOGGLEABLE))
            {
                // switch to delay
                ent->OBJ_STATE = DELAY;
                ent->OBJ_TIMER = props->delay_time;
            }
            else
            {
                // switch to delay
                ent->OBJ_STATE = IDLE;

                // if we used switch, enable them all again
                if (is(ent, USE_SWITCH))
                {
                    switch_enable_by_id(ent->ID);
                }
            }
        }

        break;
    }

    case (DELAY):
    {
        // reset movement
        vec_fill(&props->diff, 0);

        // timer part
        ent->OBJ_TIMER -= time_frame / 16;
        if (ent->OBJ_TIMER <= 0)
        {
            // reset check
            ent->OBJ_CHECK = false;

            if (is(ent, TOGGLEABLE))
            {
                ent->OBJ_STATE = IDLE;

                // if we used switch, enable them all again
                if (is(ent, USE_SWITCH))
                {
                    switch_enable_by_id(ent->ID);
                }
            }
            else
            {
                ent->OBJ_STATE = CLOSE;
            }
        }

        break;
    }
    }
}

// event function for platform
void platform_event_function()
{
    if (event_type == EVENT_SHOOT)
    {
        // not interacting with us ? then ignore !
        if (you->OBJ_C_INDICATOR != INTERACT)
        {
            return;
        }

        // already moving ? ingore !
        if (my->OBJ_STATE != IDLE)
        {
            return;
        }

        // if this platform needs to be triggered by switch or trigger zone, don't interact with it
        if (is(my, USE_SWITCH) || is(my, USE_TRIGGER))
        {
            return;
        }

        // toggle ?
        if (is(my, TOGGLEABLE))
        {
            // toggle open/close states
            my->OBJ_ALLOW_MOVE += 1;
            my->OBJ_ALLOW_MOVE = cycle(my->OBJ_ALLOW_MOVE, 1, 3);
            my->OBJ_STATE = my->OBJ_ALLOW_MOVE;

        } // if once ? then only open
        else
        {
            my->OBJ_STATE = DELAY;
        }
    }
}

// action for moving platform
// uses: ID, TOGGLEABLE, USE_SWITCH, USE_TRIGGER
action props_platform()
{
    PROPS *props = register_props(my);
    props->movement_speed = 2.5;
    props->delay_time = 1;
    props->dir = 1;
    if (is(my, TOGGLEABLE))
    {
        props->delay_time = 0.5;
    }

    vec_fill(&my->scale_x, 1);
    c_setminmax(my);
    set(my, POLYGON);

    my->group = OBSTACLE_GROUP;
    my->push = OBSTACLE_GROUP;

    my->OBJ_STATE = IDLE;
    my->OBJ_TYPE = TYPE_PLATFORM;
    my->OBJ_MOVE_CCT = true;

    // dirty hack
    props_offset_trim(my);

    my->emask |= (ENABLE_SHOOT);
    my->event = platform_event_function;

    // find a path near by and attach us to it
    props->path_id = path_scan(me, &my->x, &my->pan, vector(360, 180, 128));
    if (props->path_id == 0)
    {
        diag("\nERROR! Platform can't find any path!");
        return;
    }
    path_getnode(my, props->path_id, &my->x, NULL);

    // find total amount of nodes
    int i = 0;
    for (i = 0; i < 9; i++)
    {
        props->total_nodes = i + 1;
        var res = path_nextnode(my, i + 1, 1);
        if (res == 0)
        {
            break;
        }
    }

    // save start and end positions
    path_getnode(my, 1, &props->origin, NULL);
    path_getnode(my, props->total_nodes, &my->OFFSET_X_, NULL);
}