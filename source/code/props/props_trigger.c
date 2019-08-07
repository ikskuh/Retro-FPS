
// trigger props with the same ID
void trigger_props()
{
    ENTITY *temp_ent = NULL;
    for (temp_ent = ent_next(NULL); temp_ent; temp_ent = ent_next(temp_ent))
    {
        // props
        if (temp_ent->OBJ_TYPE == TYPE_ELEVATOR || temp_ent->OBJ_TYPE == TYPE_PLATFORM || temp_ent->OBJ_TYPE == TYPE_DOOR || temp_ent->OBJ_TYPE == TYPE_SECRET_WALL)
        {
            // not the same ID OR not using trigger ? ignore
            if (temp_ent->ID != my->ID || !is(temp_ent, USE_TRIGGER))
            {
                continue;
            }

            // already triggered ? ignore
            if (temp_ent->OBJ_STATE != IDLE)
            {
                continue;
            }

            // if you were made to be triggered
            temp_ent->OBJ_ALLOW_MOVE += 1;
            temp_ent->OBJ_ALLOW_MOVE = cycle(temp_ent->OBJ_ALLOW_MOVE, 1, 3);
            temp_ent->OBJ_STATE = temp_ent->OBJ_ALLOW_MOVE;
        }
    }

    ent_delete_later(my);
}

// event function for trigger zone
void trigger_event_function()
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

        // trigger props with the same ID
        trigger_props();
    }
}

// simple trigger zone action
// uses: ID
action props_trigger_zone()
{
    vec_fill(&my->scale_x, 1);
    c_setminmax(my);
    set(my, POLYGON | TRANSLUCENT);

    my->group = PUSH_GROUP;
    my->push = PUSH_GROUP;

    my->OBJ_TYPE = TYPE_TRIGGER_ZONE;

    my->emask |= (ENABLE_PUSH);
    my->event = trigger_event_function;
}