
// update secret wall
void secret_wall_update(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! Can't update this secret wall, it doesn't exist");
        return;
    }

    // get props struct
    PROPS *props = get_props(ent);

    // open ?
    if (ent->OBJ_STATE == OPEN)
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
                ent->OBJ_SND_HANDLE = ent_playloop(ent, secret_wall_start_ogg, props_secret_wall_snd_volume);
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
                ent->OBJ_SND_HANDLE = ent_playsound(ent, secret_wall_stop_ogg, props_secret_wall_snd_volume);
                ent->OBJ_ARMOR = 0;
            }

            // set proper position
            vec_set(&ent->x, &ent->OFFSET_X_);

            // switch to DEAD (can't be used anymore)
            ent->OBJ_STATE = DEAD;
            ent->OBJ_CHECK = false;
        }
    }
}

// event function for secret wall
void secret_wall_event_function()
{
    if (event_type == EVENT_SHOOT)
    {
        // not in IDLE state ?
        if (my->OBJ_STATE != IDLE)
        {
            return;
        }

        // player is that you ?
        if (you->OBJ_TYPE != TYPE_PLAYER)
        {
            return;
        }

        // only once !
        my->emask &= ~ENABLE_SHOOT;

        // move this secret wall !
        my->OBJ_STATE = OPEN;
    }
}

// simple secret wall
// uses: OFFSET_X_, OFFSET_Y_, OFFSET_Z_
action props_secret_wall()
{
    PROPS *props = register_props(my);
    props->movement_speed = 2.5;

    vec_fill(&my->scale_x, 1);
    c_setminmax(my);
    set(my, POLYGON);

    my->group = OBSTACLE_GROUP;
    my->push = OBSTACLE_GROUP;

    my->OBJ_STATE = IDLE;
    my->OBJ_TYPE = TYPE_SECRET_WALL;
    my->OBJ_MOVE_CCT = TRUE;

    // dirty hack
    props_offset_trim(my);

#ifndef FREE_VERSION
    my->material = mtl_object;
#endif
    my->emask |= (ENABLE_SHOOT);
    my->event = secret_wall_event_function;
}

// event function for secret zone
void secret_zone_event_function()
{
    if (event_type == EVENT_PUSH)
    {
        // player is that you ?
        if (you->OBJ_TYPE != TYPE_PLAYER)
        {
            return;
        }

        // only once !
        my->emask &= ~ENABLE_PUSH;

        // plus 1 secret found on this level
        level_secrets_found++;

        // play secret found snd
        snd_play(secret_zone_found_ogg, props_secret_zone_snd_volume, 0);

        // show message

        // remove us
        ent_delete_later(my);
    }
}

// simple secret zone snd/message trigger
action props_secret_zone()
{
    // plus 1 secret created on this level
    level_secrets_total++;

    vec_fill(&my->scale_x, 1);
    c_setminmax(my);
    set(my, POLYGON | TRANSLUCENT);

    my->group = PUSH_GROUP;
    my->push = PUSH_GROUP;

    my->OBJ_TYPE = TYPE_SECRET_ZOME;

    my->emask |= (ENABLE_PUSH);
    my->event = secret_zone_event_function;
}