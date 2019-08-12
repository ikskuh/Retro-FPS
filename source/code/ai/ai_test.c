
void ai_event_function()
{
    if (event_type == EVENT_SHOOT)
    {
        // you will take damage according to the weapon type
        my->OBJ_HEALTH -= you->OBJ_TAKE_DAMAGE;
        my->OBJ_PAIN_TYPE = TYPE_SHOOT;
    }
}

action ai()
{
    vec_fill(&my->scale_x, 1);
    c_setminmax(my);
    set(my, POLYGON | TRANSLUCENT);

    my->group = ENEMY_GROUP;
    my->push = ENEMY_GROUP;

    my->OBJ_HEALTH = 100;
    my->OBJ_TYPE = TYPE_NPC;

    my->emask |= (ENABLE_SHOOT);
    my->event = ai_event_function;

    while(my)
    {
        if(my->OBJ_HEALTH <= 0)
        {
            break;
        }
        wait(1);
    }
    ent_delete_later(my);
}