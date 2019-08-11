
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

    if (event_type == EVENT_FRAME)
    {
        // push us away (if needed)
        my->VEL_X += my->DIR_X;
        my->VEL_Y += my->DIR_Y;

        my->VEL_X -= sign(my->VEL_X) * minv(0.6 * abs(my->VEL_X) * time_step, abs(my->VEL_X));
        my->VEL_Y -= sign(my->VEL_Y) * minv(0.6 * abs(my->VEL_Y) * time_step, abs(my->VEL_Y));

        if (my->VEL_X != 0 || my->VEL_Y != 0)
        {
            c_move(me, nullvector, vector(my->VEL_X * time_step, my->VEL_Y * time_step, 0), MOVE_FLAGS);
        }

        my->DIR_X = 0;
        my->DIR_Y = 0;

        if (my->parent)
        {
            vec_set(&my->parent->x, &my->x);

            my->parent->skill1 += 0.25 * time_step;
            my->parent->skill1 %= 2;
            my->parent->frame = my->parent->skill1;
            my->parent->skill43 = floatd(floor(my->parent->frame), 2);
            my->parent->skill44 = floatd(0, 1);

            // rotate towards the camera
            ent_rotate_to_camera(my->parent);
        }

        if (my->OBJ_HEALTH <= 0)
        {
            // explosion effect here !
            explosion_create(&my->x, true);
            ent_delete_later(my->parent);
            ent_delete_later(my);
        }
    }
}

// main barrels action
action props_barrel()
{
    vec_set(&my->scale_x, vector(0.75, 0.75, 1));
    c_setminmax(my);
    props_place_on_ground(my);
    set(my, POLYGON | INVISIBLE);

    my->group = OBSTACLE_GROUP;
    my->push = OBSTACLE_GROUP;
    my->OBJ_HEALTH = 45;
    my->OBJ_TYPE = TYPE_BARREL;

    // create non passable bbox !
    my->parent = ent_create(props_barrel_tga, &my->x, barrel_visual_func);

    my->emask |= (ENABLE_SHOOT | ENABLE_SCAN | ENABLE_FRAME);
    my->event = barrel_event_function;
}