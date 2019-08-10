// function to update given door
void door_update(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! Can't update this door, it doesn't exist");
        return;
    }

    // get props struct
    PROPS *props = get_props(ent);
    if (!props)
    {
        diag("\nERROR! Can't update this door, it's structure doesn't exist");
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
            vec_add(&ent->x, &props->diff);

            // play start sound
            // armor skill used as a switch, to play sound once
            if (ent->OBJ_ARMOR == 0)
            {
                if (snd_playing(ent->OBJ_SND_HANDLE))
                {
                    snd_stop(ent->OBJ_SND_HANDLE);
                }
                ent->OBJ_SND_HANDLE = ent_playsound(ent, door_01_open_ogg, props_door_snd_volume);
                ent->OBJ_ARMOR = 1;
            }
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
                ent->OBJ_SND_HANDLE = ent_playsound(ent, door_01_close_ogg, props_door_snd_volume);
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
                ent->OBJ_SND_HANDLE = ent_playsound(ent, door_01_open_ogg, props_door_snd_volume);
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

// check if player has needed key
var door_player_has_key()
{
    if (is(my, RED_KEY) && player_has_red_key == false)
    {
        return false;
    }
    if (is(my, YELLOW_KEY) && player_has_yellow_key == false)
    {
        return false;
    }
    if (is(my, BLUE_KEY) && player_has_blue_key == false)
    {
        return false;
    }

    return true;
}

// event function for the door
void door_event_function()
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

        // if this door needs to be triggered by switch or trigger zone, don't interact with it
        if (is(my, USE_SWITCH) || is(my, USE_TRIGGER))
        {
            return;
        }

        // requires a key ?
        if (is(my, RED_KEY) || is(my, YELLOW_KEY) || is(my, BLUE_KEY))
        {
            // interaction from player ?
            if (you->OBJ_TYPE == TYPE_PLAYER)
            {
                // no keys ?
                if (door_player_has_key() == false)
                {
                    // don't allow to interact
                    my->emask &= ~ENABLE_SHOOT;

                    // play no keycard sound here !
                    if (snd_playing(you->OBJ_SND_HANDLE))
                    {
                        snd_stop(you->OBJ_SND_HANDLE);
                    }
                    you->OBJ_SND_HANDLE = snd_play(player_no_use_ogg, player_snd_volume, 0);

                    // show message, that this door requires a key

                    // wait for a while
                    ent_delay(my, 1);
                    wait_for(ent_delay);

                    my->emask |= (ENABLE_SHOOT);

                    return;
                }
                else
                {
                    // reset our key
                    if (is(my, RED_KEY))
                    {
                        reset(my, RED_KEY);
                    }
                    if (is(my, YELLOW_KEY))
                    {
                        reset(my, YELLOW_KEY);
                    }
                    if (is(my, BLUE_KEY))
                    {
                        reset(my, BLUE_KEY);
                    }
                }
            }
            else
            {
                // ignore npcs
                return;
            }
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

// initialize door settings
void door_setup(var snd)
{
    PROPS *props = register_props(my);
    props->movement_speed = 5;
    props->delay_time = 5;
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
    my->OBJ_TYPE = TYPE_DOOR;
    my->OBJ_MOVE_CCT = true;

    var sound_type = snd;
    if (sound_type < 0)
    {
        sound_type = 0;
    }
    if (sound_type > 1)
    {
        sound_type = 1;
    }
    my->OBJ_PAIN_TYPE = sound_type;

    // dirty hack
    props_offset_trim(my);

    my->emask |= (ENABLE_SHOOT);
    my->event = door_event_function;
}

// simple door action
// uses: ID, OFFSET_X_, OFFSET_Y_, OFFSET_Z_, RED_KEY, YELLOW_KEY, BLUE_KEY, TOGGLEABLE, USE_SWITCH, USE_TRIGGER
action props_door()
{
    door_setup(0);
}

// simple door action
// uses: ID, OFFSET_X_, OFFSET_Y_, OFFSET_Z_, RED_KEY, YELLOW_KEY, BLUE_KEY, TOGGLEABLE, USE_SWITCH, USE_TRIGGER
action props_door_alter()
{
    door_setup(1); // all same as above door, but uses alternative sound
}