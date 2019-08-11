
// impact sprite animation
void bullet_impact_sprite()
{
    VECTOR temp_vec;
    vec_fill(&temp_vec, 0);
    vec_fill(&my->scale_x, 0.25 + random(0.25));

    set(my, PASSABLE | NOFILTER);
    my->ambient = 100;

#ifndef FREE_VERSION
    my->material = mtl_z_write;
#endif

    while (my)
    {
        if (my->frame > 4)
        {
            break;
        }

        // rotate towards the camera
        ent_rotate_to_camera(my);

        my->skill1 += time_step;
        my->frame = my->skill1 + 1;
        wait(1);
    }
    ent_delete_later(my);
}

// blood impact sprite animation
void bullet_blood_impact_sprite()
{
    VECTOR temp_vec;
    vec_fill(&temp_vec, 0);
    vec_fill(&my->scale_x, 0.25 + random(0.25));

    set(my, PASSABLE | NOFILTER);
    my->ambient = 100;

#ifndef FREE_VERSION
    my->material = mtl_z_write;
#endif

    while (my)
    {
        if (my->frame > 4)
        {
            break;
        }

        // rotate towards the camera
        ent_rotate_to_camera(my);

        my->skill1 += time_step;
        my->frame = my->skill1 + 1;
        wait(1);
    }
    ent_delete_later(my);
}

// ricochet sounds
// MAX 4 bullet impact sounds !
void bullet_ric_snd()
{
    if (proc_status(bullet_ric_snd) > 4 || vec_dist(&my->x, &camera->x) > 256)
    {
        ptr_remove(my);
        return;
    }

    set(my, PASSABLE | INVISIBLE);

    var rnd = integer(random(3));
    if (rnd == 0)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, bullet_ric_01_ogg, weapon_ric_volume);
    }
    if (rnd == 1)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, bullet_ric_02_ogg, weapon_ric_volume);
    }
    if (rnd == 2)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, bullet_ric_03_ogg, weapon_ric_volume);
    }

    while (my)
    {
        if (!snd_playing(my->OBJ_SND_HANDLE))
        {
            break;
        }
        wait(1);
    }
    ent_delete_later(my);
}

// bullet impact water sounds
// MAX 4 bullet impact sounds !
void bullet_water_impact_snd()
{
    if (proc_status(bullet_water_impact_snd) > 4 || vec_dist(&my->x, &camera->x) > 256)
    {
        ptr_remove(my);
        return;
    }

    set(my, PASSABLE | INVISIBLE);

    water_hard_impact_snd(my);

    while (my)
    {
        if (!snd_playing(my->OBJ_SND_HANDLE))
        {
            break;
        }
        wait(1);
    }
    ent_delete_later(my);
}

// handle bullet impacts
void bullet_impact_fx(ENTITY *ent, var is_alive, VECTOR *hit_vector, VECTOR *surf_angle)
{
    VECTOR temp_tec;
    VECTOR impact_vec;
    vec_fill(&impact_vec, 0);
    vec_set(&impact_vec, vector(4, 0, 0));

    vec_to_angle(&temp_tec, &surf_angle->x);
    vec_rotate(&impact_vec, &temp_tec);
    vec_add(&impact_vec, &hit_vector->x);

    // we hit something static here
    if (is_alive == false)
    {

#ifndef PARTICLE_EFFECTS
        ent_create(bullet_impact_tga, &impact_vec, bullet_impact_sprite);
#else
        effect(bullet_impact_particle, 8 + random(8), &impact_vec, nullvector);
#endif
        // only if not in water
        if (ent->OBJ_ARMOR != -1)
        {
            ent_create(NULL, &hit_vector->x, bullet_ric_snd);
        }
    }
    else
    {
        // add body hit impact sounds here

#ifndef PARTICLE_EFFECTS
        ent_create(blood_impact_tga, &impact_vec, bullet_blood_impact_sprite);
#else
        effect(blood_impact_particle, 8 + random(8), &impact_vec, nullvector);
#endif
    }
}

// bullet function
void bullet_func()
{
    set(my, PASSABLE | INVISIBLE);
    my->OBJ_TYPE = TYPE_BULLET;
    my->OBJ_TIMER = 16;

    // already in water ?
    if (region_check(reg_water_str, &my->x, &my->x))
    {
        my->OBJ_HEALTH = -1;
    }
}

// create bullets
void bullet_create(VECTOR *pos, VECTOR *angle, var owner_group, var owner_damage, var bullet_speed, var accur)
{
    you = ent_create(NULL, pos, bullet_func);
    vec_set(&you->ORIGIN_X, pos);
    vec_set(&you->DIR_X, vector(bullet_speed, (accur - random(accur * 2)), (accur - random(accur * 2))));
    vec_rotate(&you->DIR_X, angle);
    you->OWNER_GROUP = owner_group;
    you->OBJ_TAKE_DAMAGE = owner_damage;
}

// function to update given bullet
void bullet_update(ENTITY *ent)
{
    // simply flying ?
    if (ent->OBJ_STATE == PROJ_FLY)
    {
        vec_set(&ent->VEL_X, &ent->DIR_X);
        vec_scale(&ent->VEL_X, time_step);
        vec_add(&ent->VEL_X, &ent->ORIGIN_X);

        c_ignore(PUSH_GROUP, WATER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, ent->OWNER_GROUP, SHOOTABLE_GROUP, 0);
        ent_trace(ent, &ent->ORIGIN_X, &ent->VEL_X, ACTIVATE_SHOOT | SCAN_TEXTURE | USE_POLYGON);

        if (HIT_TARGET)
        {
            vec_set(&ent->DIR_X, nullvector);
            vec_set(&ent->VEL_X, &hit->x);
            vec_set(&ent->DIR_X, &normal);

            if (you)
            {
                if (you->OBJ_TYPE == TYPE_PLAYER || you->OBJ_TYPE == TYPE_NPC)
                {
                    ent->OBJ_STATE = PROJ_HIT_ALIVE;
                }
                else
                {
                    ent->OBJ_STATE = PROJ_HIT_STATIC;
                }
            }
            else
            {
                ent->OBJ_STATE = PROJ_HIT_STATIC;
            }
        }

        vec_set(&ent->ORIGIN_X, &ent->VEL_X);

        ent->OBJ_TIMER -= time_step;
        if (ent->OBJ_TIMER <= 0)
        {
            ent->OBJ_STATE = 3; // death state
        }
    }

    // hit something alive ?
    if (ent->OBJ_STATE == PROJ_HIT_ALIVE)
    {
        bullet_impact_fx(ent, true, &ent->VEL_X, &ent->DIR_X);
        ent->OBJ_STATE = PROJ_DELETE;
    }

    // hit something static ?
    if (ent->OBJ_STATE == PROJ_HIT_STATIC)
    {
        // check if hit position was out of the water region
        if (region_check(reg_water_str, &ent->VEL_X, &ent->VEL_X))
        {
            // don't create ricochete sounds
            ent->OBJ_ARMOR = -1;
        }

        // was shoot while in water ?
        if (ent->OBJ_HEALTH != -1)
        {
            c_ignore(PUSH_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, ent->OWNER_GROUP, SHOOTABLE_GROUP, 0);
            ent_trace(ent, &ent->x, &ent->VEL_X, SCAN_TEXTURE | USE_POLYGON);

            if (HIT_TARGET)
            {
                if (you)
                {
                    if (you->OBJ_TYPE == TYPE_WATER_PLANE)
                    {
                        water_create_medium_impact(&hit->x, 0.25);
                        ent_create(NULL, &hit->x, bullet_water_impact_snd);
                        ent->OBJ_HEALTH = -2;
                    }
                }
            }
        }

        // we can spawn normal fx ?
        if (ent->OBJ_HEALTH != -2)
        {
            bullet_impact_fx(ent, false, &ent->VEL_X, &ent->DIR_X);
        }

        ent->OBJ_STATE = PROJ_DELETE;
    }

    // delete ?
    if (ent->OBJ_STATE == PROJ_DELETE)
    {
        ent_delete_later(ent);
    }
}