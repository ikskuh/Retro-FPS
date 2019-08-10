
// water splash function
void water_splash_effect()
{
    wait(1);
    VECTOR temp_vec;
    vec_fill(&temp_vec, 0);
    c_setminmax(my);
    my->z += my->max_z;
    set(my, PASSABLE | NOFILTER | BRIGHT);
    my->ambient = 100;

#ifndef FREE_VERSION
    my->material = mtl_z_write;
#endif

    while (my)
    {
        if (my->frame > 16)
        {
            break;
        }

        // rotate towards the camera
        vec_set(&temp_vec, &camera->x);
        vec_sub(&temp_vec, &my->x);
        vec_to_angle(&my->pan, &temp_vec);
        my->tilt = 0;
        my->roll = 0;

        my->skill1 += 1.25 * time_step;
        my->frame = my->skill1 + 1;
        wait(1);
    }
    ent_delete_later(my);
}

// water ripple function
void water_ripple_effect()
{
    my->z += 0.05;
    my->tilt = 90;
    set(my, PASSABLE | NOFILTER | BRIGHT);
    my->ambient = 100;

#ifndef FREE_VERSION
    my->material = mtl_z_write;
#endif

    while (my)
    {
        if (my->frame > 16)
        {
            break;
        }
        my->skill1 += time_step;
        my->frame = my->skill1 + 1;
        wait(1);
    }
    ent_delete_later(my);
}

// get impact pos on water plane
var water_get_pos_on_surface(ENTITY *ent, VECTOR *pos)
{
    if (!ent)
    {
        diag("\nERROR! Can't get pos on water surface! Entity doesn't exist.");
        return;
    }

    ent_trace(ent, vector(ent->x, ent->y, ent->z + 32), &ent->x, IGNORE_PASSABLE | IGNORE_WORLD);
    if (HIT_TARGET)
    {
        if (you)
        {
            if (you->OBJ_TYPE == TYPE_WATER_PLANE)
            {
                vec_set(pos, &hit->x);
                return true;
            }
        }
    }
    return false;
}

// create ripple effect on water
void water_create_ripple_effect(VECTOR *pos, var scale)
{
    // create effects
    VECTOR spawn_pos;
    vec_set(&spawn_pos, pos);

    var i = 0;
    for (i = 0; i < 4; i++)
    {
        you = ent_create(water_ripple_tga, &spawn_pos, water_ripple_effect);
        vec_fill(&you->scale_x, scale);
        wait(-0.25);
    }
}

// create small water impact effect
void water_create_small_impact(VECTOR *pos, var scale)
{
    // save position vector
    VECTOR spawn_pos;
    vec_set(&spawn_pos, pos);

    // create effects
    water_create_ripple_effect(&spawn_pos, scale);

    you = ent_create(water_splash_small_tga, &spawn_pos, water_splash_effect);
    vec_fill(&you->scale_x, scale);
}

// create medium water impact effect
void water_create_medium_impact(VECTOR *pos, var scale)
{
    // save position vector
    VECTOR spawn_pos;
    vec_set(&spawn_pos, pos);

    // create effects
    water_create_ripple_effect(&spawn_pos, scale);

    you = ent_create(water_splash_medium_tga, &spawn_pos, water_splash_effect);
    vec_fill(&you->scale_x, scale);
}

// create big water impact effect
void water_create_big_impact(VECTOR *pos, var scale)
{
    // save position vector
    VECTOR spawn_pos;
    vec_set(&spawn_pos, pos);

    // create effects
    water_create_ripple_effect(&spawn_pos, scale);

    you = ent_create(water_splash_big_tga, &spawn_pos, water_splash_effect);
    vec_fill(&you->scale_x, scale);
}

// water hard impact sound
void water_hard_impact_snd(ENTITY *ent)
{
    var rnd = integer(random(3));
    if (rnd == 0)
    {
        ent->OBJ_SND_HANDLE = ent_playsound(ent, water_hard_impact_01_ogg, water_hard_impact_volume);
    }
    if (rnd == 1)
    {
        ent->OBJ_SND_HANDLE = ent_playsound(ent, water_hard_impact_02_ogg, water_hard_impact_volume);
    }
    if (rnd == 2)
    {
        ent->OBJ_SND_HANDLE = ent_playsound(ent, water_hard_impact_03_ogg, water_hard_impact_volume);
    }
}

// water small impact sound
void water_small_impact_snd(ENTITY *ent)
{
    var rnd = integer(random(3));
    if (rnd == 0)
    {
        ent->OBJ_SND_HANDLE = ent_playsound(ent, water_small_impact_01_ogg, water_small_impact_volume);
    }
    if (rnd == 1)
    {
        ent->OBJ_SND_HANDLE = ent_playsound(ent, water_small_impact_02_ogg, water_small_impact_volume);
    }
    if (rnd == 2)
    {
        ent->OBJ_SND_HANDLE = ent_playsound(ent, water_small_impact_03_ogg, water_small_impact_volume);
    }
}