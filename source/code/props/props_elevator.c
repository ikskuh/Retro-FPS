
// function to update given elevator
void elevator_update(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! Can't update this elevator, it doesn't exist");
        return;
    }

    // get props struct
    PROPS *props = get_props(ent);
    if (!props)
    {
        diag("\nERROR! Can't update this elevator, it's structure doesn't exist");
        return;
    }

    // state machine
    switch (ent->OBJ_STATE)
    {

    case (OPEN):
    {
        // save this state
        props->old_state = OPEN;

        // still can move ?
        if (vec_dist(&ent->x, &ent->OFFSET_X_) > 0.5)
        {
            // move to target position
            vec_diff(&props->diff, &ent->OFFSET_X_, &ent->x);
            var spd = props->movement_speed * time_step;
            if (vec_length(&props->diff) > spd)
            {
                vec_normalize(&props->diff, spd);
            }
            c_ignore(PUSH_GROUP, WATER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
            ent_move(ent, nullvector, &props->diff, IGNORE_WORLD | IGNORE_YOU | IGNORE_PUSH);

            // play start sound
            // armor skill used as a switch, to play sound once
            if (ent->OBJ_ARMOR == 0)
            {
                if (snd_playing(ent->OBJ_SND_HANDLE))
                {
                    snd_stop(ent->OBJ_SND_HANDLE);
                }
                ent_playsound(ent, elevator_start_ogg, props_elevator_snd_volume);
                ent->OBJ_SND_HANDLE = ent_playloop(ent, elevator_mid_ogg, props_elevator_snd_loop_volume);
                ent->OBJ_ARMOR = 1;
            }

            // when almost opened ?
            // and the gap is smaller than the biggest bbox ?
            props_almost_opened_check(ent);
        }
        else
        {
            // stoped sound
            if (ent->OBJ_ARMOR == 1)
            {
                if (snd_playing(ent->OBJ_SND_HANDLE))
                {
                    snd_stop(ent->OBJ_SND_HANDLE);
                }
                ent->OBJ_SND_HANDLE = ent_playsound(ent, elevator_stop_ogg, props_elevator_snd_volume);
                ent->OBJ_ARMOR = 0;
            }

            // set proper position
            vec_set(&ent->x, &ent->OFFSET_X_);

            // switch to delay
            ent->OBJ_STATE = DELAY;
            ent->OBJ_TIMER = props->delay_time;
        }

        break;
    }

    case (CLOSE):
    {
        // save this state
        props->old_state = CLOSE;

        if (vec_dist(&ent->x, &props->origin) > 0.5)
        {
            vec_diff(&props->diff, &props->origin, &ent->x);
            var spd = props->movement_speed * time_step;
            if (vec_length(&props->diff) > spd)
            {
                vec_normalize(&props->diff, spd);
            }
            c_ignore(PUSH_GROUP, WATER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
            ent_move(ent, nullvector, &props->diff, IGNORE_WORLD | IGNORE_YOU | IGNORE_PUSH);

            // play start sound
            // armor skill used as a switch, to play sound once
            if (ent->OBJ_ARMOR == 0)
            {
                if (snd_playing(ent->OBJ_SND_HANDLE))
                {
                    snd_stop(ent->OBJ_SND_HANDLE);
                }
                ent_playsound(ent, elevator_start_ogg, props_elevator_snd_volume);
                ent->OBJ_SND_HANDLE = ent_playloop(ent, elevator_mid_ogg, props_elevator_snd_loop_volume);
                ent->OBJ_ARMOR = 1;
            }

            // when almost closed ?
            // and the gap is smaller than the biggest bbox ?
            props_almost_closed_check(ent, props);
        }
        else
        {
            // stoped sound
            if (ent->OBJ_ARMOR == 1)
            {
                if (snd_playing(ent->OBJ_SND_HANDLE))
                {
                    snd_stop(ent->OBJ_SND_HANDLE);
                }
                ent->OBJ_SND_HANDLE = ent_playsound(ent, elevator_stop_ogg, props_elevator_snd_volume);
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

// event function for the elevator
void elevator_event_function()
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

        // if this elevator needs to be triggered by switch or trigger zone, don't interact with it
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
            my->OBJ_STATE = OPEN;
        }
    }
}

// simple elevator action
// uses: ID, OFFSET_X_, OFFSET_Y_, OFFSET_Z_, TOGGLEABLE, USE_SWITCH, USE_TRIGGER
action props_elevator()
{
    PROPS *props = register_props(my);
    props->movement_speed = 5;
    props->delay_time = 3;

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
    my->OBJ_TYPE = TYPE_ELEVATOR;
    my->OBJ_MOVE_CCT = true;

    // dirty hack
    props_offset_trim(my);

    my->emask |= (ENABLE_SHOOT);
    my->event = elevator_event_function;
}