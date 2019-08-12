
// barrel's visual sprite function
void barrel_visual_func()
{
    set(my, PASSABLE | NOFILTER | SHADOW | CAST);

#ifndef FREE_VERSION
    my->material = mtl_anim_sprite;
    my->skill41 = floatv(2);
    my->skill42 = floatv(1);
#endif
}

// barrel's event function
void barrel_event_function()
{
    if (event_type == EVENT_SHOOT)
    {
        // you will take damage according to the weapon type
        my->OBJ_HEALTH -= you->OBJ_TAKE_DAMAGE;
    }

    if (event_type == EVENT_SCAN)
    {
        if (my->OBJ_HEALTH > 0)
        {
            // check for walls, handle pushing and damage !
            explo_check_walls(my, &you->x, &my->x, you->OBJ_EXPLO_DAMAGE);
        }
    }
}

// update barrels
void barrel_update(ENTITY *ent)
{
    // get props structure
    PROPS *props = get_props(ent);

    // apply gravity
    props_gravity(ent, props);

    // push us away (if needed)
    ent->VEL_X += ent->DIR_X;
    ent->VEL_Y += ent->DIR_Y;

    ent->VEL_X -= sign(ent->VEL_X) * minv(0.6 * abs(ent->VEL_X) * time_step, abs(ent->VEL_X));
    ent->VEL_Y -= sign(ent->VEL_Y) * minv(0.6 * abs(ent->VEL_Y) * time_step, abs(ent->VEL_Y));

    if (ent->VEL_X != 0 || ent->VEL_Y != 0)
    {
        c_move(ent, nullvector, vector(ent->VEL_X * time_step, ent->VEL_Y * time_step, 0), MOVE_FLAGS);
    }

    ent->DIR_X = 0;
    ent->DIR_Y = 0;

    if (ent->parent)
    {
        vec_set(&ent->parent->x, &ent->x);

        ent->parent->skill1 += 0.25 * time_step;
        ent->parent->skill1 %= 2;
        ent->parent->frame = ent->parent->skill1;
        ent->parent->skill43 = floatd(floor(ent->parent->frame), 2);
        ent->parent->skill44 = floatd(0, 1);

        // rotate towards the camera
        ent_rotate_to_camera(ent->parent);
    }

    // if dead ?
    // then count down till explosion
    if (ent->OBJ_HEALTH <= 0)
    {
        ent->OBJ_TIMER -= time_frame / 16;

        // we can add burning fx here
    }

    if (ent->OBJ_TIMER <= 0)
    {
        // explosion effect here !
        explosion_create(&ent->x, true);
        ent_delete_later(ent->parent);
        ent_delete_later(ent);
    }
}

// main barrels action
action props_barrel()
{
    PROPS *props = register_props(my);
    vec_set(&my->scale_x, vector(0.75, 0.75, 1));
    c_setminmax(my);
    set(my, POLYGON | INVISIBLE);

    my->group = OBSTACLE_GROUP;
    my->push = OBSTACLE_GROUP;
    my->OBJ_HEALTH = 45;
    my->OBJ_TIMER = 1; // 1 second delay before explosion
    my->OBJ_TYPE = TYPE_BARREL;

    // create non passable bbox !
    my->parent = ent_create(props_barrel_tga, &my->x, barrel_visual_func);

    my->emask |= (ENABLE_SHOOT | ENABLE_SCAN);
    my->event = barrel_event_function;
}