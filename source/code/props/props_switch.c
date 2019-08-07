
// enable all switches with the given ID
void switch_enable_by_id(var num)
{
    ENTITY *temp_ent = NULL;
    for (temp_ent = ent_next(NULL); temp_ent; temp_ent = ent_next(temp_ent))
    {
        if (temp_ent->OBJ_TYPE == TYPE_SWITCH)
        {
            // if it needs to be enabled only once, then ignore !
            if (!is(temp_ent, TOGGLEABLE))
            {
                continue;
            }

            // same ID? then disable it
            if (temp_ent->ID == num)
            {
                temp_ent->emask |= (ENABLE_SHOOT);
            }
        }
    }
}

// disable all switches with the given ID
void switch_disable_by_id(var num)
{
    ENTITY *temp_ent = NULL;
    for (temp_ent = ent_next(NULL); temp_ent; temp_ent = ent_next(temp_ent))
    {
        if (temp_ent->OBJ_TYPE == TYPE_SWITCH)
        {
            // same ID? then disable it
            if (temp_ent->ID == num)
            {
                temp_ent->emask &= ~ENABLE_SHOOT;
            }
        }
    }
}

// function used to trigger props with the same ID
void switch_trigger_props()
{
    ENTITY *temp_ent = NULL;
    for (temp_ent = ent_next(NULL); temp_ent; temp_ent = ent_next(temp_ent))
    {
        // props
        if (temp_ent->OBJ_TYPE == TYPE_ELEVATOR || temp_ent->OBJ_TYPE == TYPE_PLATFORM || temp_ent->OBJ_TYPE == TYPE_DOOR || temp_ent->OBJ_TYPE == TYPE_SECRET_WALL)
        {
            // not the same ID OR not using switch ? ignore
            if (temp_ent->ID != my->ID || !is(temp_ent, USE_SWITCH))
            {
                continue;
            }

            // already triggered ? ignore
            if (temp_ent->OBJ_STATE != IDLE)
            {
                continue;
            }
            // if you were made to be triggered
            if (is(temp_ent, TOGGLEABLE))
            {
                // toggle between open and close states
                temp_ent->OBJ_ALLOW_MOVE += 1;
                temp_ent->OBJ_ALLOW_MOVE = cycle(temp_ent->OBJ_ALLOW_MOVE, 1, 3);
                temp_ent->OBJ_STATE = temp_ent->OBJ_ALLOW_MOVE;
            }
            else
            {
                // other ways, open !
                temp_ent->OBJ_STATE = OPEN;
            }
        }
    }
}

// switch's event function
void switch_event_function()
{
    if (event_type == EVENT_SHOOT)
    {
        // only player can trigger switches ?
        if (you->OBJ_TYPE == TYPE_PLAYER)
        {
            // not interacting with us ? then ignore !
            if (you->OBJ_C_INDICATOR != INTERACT)
            {
                return;
            }

            // disable all switches with the same ID
            switch_disable_by_id(my->ID);

            // play switch sound
            if (snd_playing(my->OBJ_SND_HANDLE))
            {
                snd_stop(my->OBJ_SND_HANDLE);
            }
            my->OBJ_SND_HANDLE = ent_playsound(my, switch_ogg, props_switch_snd_volume);

            // wait for a while
            ent_delay(my, 1);
            wait_for(ent_delay);

            // trigger props with the same ID
            switch_trigger_props();
        }
    }
}
// switch to trigge other props
// uses: ID, TOGGLEABLE
// FLAG3: TOGGLEABLE 1
action props_switch()
{
    vec_fill(&my->scale_x, 1);
    c_setminmax(my);
    set(my, POLYGON);

    my->group = SWITCH_ITEM_GROUP;
    my->push = SWITCH_ITEM_GROUP;

    my->OBJ_TYPE = TYPE_SWITCH;

    my->emask |= (ENABLE_SHOOT);
    my->event = switch_event_function;
}