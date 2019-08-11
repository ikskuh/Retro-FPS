
// explosion light flashing effect (out of the water)
void explosion_light_fnc()
{
    set(my, PASSABLE | INVISIBLE);
    my->lightrange = 128;
    my->OBJ_EXPLO_RANGE = 350;
    my->OBJ_EXPLO_DAMAGE = 100;
    explo_scan(my);

    while (my)
    {
        if (my->skill1 > 4)
        {
            break;
        }

        my->lightrange -= 25 * time_step;
        my->lightrange = clamp(my->lightrange, 16, 128);
        my->skill1 += time_step;
        wait(1);
    }
    my->lightrange = 0;
    wait(1);
    ent_delete_later(my);
}

// explosion light flashing effect (in the water)
void explosion_light_in_water_fnc()
{
    set(my, PASSABLE | INVISIBLE);
    my->lightrange = 128;
    my->OBJ_EXPLO_RANGE = explo_default_range;
    my->OBJ_EXPLO_DAMAGE = 50;
    explo_scan(my);

    while (my)
    {
        if (my->skill1 > 4)
        {
            break;
        }

        my->lightrange -= 25 * time_step;
        my->lightrange = clamp(my->lightrange, 16, 128);
        my->skill1 += time_step;
        wait(1);
    }
    my->lightrange = 0;
    wait(1);
    ent_delete_later(my);
}

// explosion's sfx function (when out of the water)
// MAX 4 explosion sounds at the same time !
void explosion_simple_snd_func()
{
    if (proc_status(explosion_simple_snd_func) > 4 || vec_dist(&my->x, &camera->x) > 512)
    {
        ptr_remove(my);
        return;
    }

    set(my, PASSABLE | INVISIBLE);

    var rnd = integer(random(3));
    if (rnd == 0)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, explo_01_ogg, explo_snd_volume);
    }
    if (rnd == 1)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, explo_02_ogg, explo_snd_volume);
    }
    if (rnd == 2)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, explo_03_ogg, explo_snd_volume);
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

// explosion sfx out of water
void explosion_snd_out_of_water(VECTOR *pos)
{
    VECTOR spawn_pos;
    vec_set(&spawn_pos, pos);
    ent_create(NULL, &spawn_pos, explosion_simple_snd_func);
}

// explosion's sfx function (when on water)
// MAX 4 explosion sounds at the same time !
void explosion_on_water_snd_func()
{
    if (proc_status(explosion_on_water_snd_func) > 4 || vec_dist(&my->x, &camera->x) > 512)
    {
        ptr_remove(my);
        return;
    }

    set(my, PASSABLE | INVISIBLE);

    var rnd = integer(random(3));
    if (rnd == 0)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, explo_water_01_ogg, explo_snd_volume);
    }
    if (rnd == 1)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, explo_water_02_ogg, explo_snd_volume);
    }
    if (rnd == 2)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, explo_water_03_ogg, explo_snd_volume);
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

// explosion sfx on water
void explosion_snd_on_water(VECTOR *pos)
{
    VECTOR spawn_pos;
    vec_set(&spawn_pos, pos);
    ent_create(NULL, &spawn_pos, explosion_on_water_snd_func);
}

// explosion's sfx function (when in water)
// MAX 4 explosion sounds at the same time !
void explosion_in_water_snd_func()
{
    if (proc_status(explosion_in_water_snd_func) > 4 || vec_dist(&my->x, &camera->x) > 512)
    {
        ptr_remove(my);
        return;
    }

    set(my, PASSABLE | INVISIBLE);

    var rnd = integer(random(3));
    if (rnd == 0)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, explo_underwater_01_ogg, explo_underwater_snd_volume);
    }
    if (rnd == 1)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, explo_underwater_02_ogg, explo_underwater_snd_volume);
    }
    if (rnd == 2)
    {
        my->OBJ_SND_HANDLE = ent_playsound(my, explo_underwater_03_ogg, explo_underwater_snd_volume);
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

// explosion sfx in water
void explosion_snd_in_water(VECTOR *pos)
{
    VECTOR spawn_pos;
    vec_set(&spawn_pos, pos);
    ent_create(NULL, &spawn_pos, explosion_in_water_snd_func);
}

// barrel sprite
void explosion_barrel_sprite()
{
    my->z += 48;
    vec_fill(&my->scale_x, 1);
    set(my, PASSABLE | NOFILTER);
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
        ent_rotate_to_camera(my);

        my->skill1 += time_step;
        my->frame = my->skill1 + 1;
        wait(1);
    }
    ent_delete_later(my);
}

// simple explosion sprite
void explosion_sprite()
{
    vec_fill(&my->scale_x, 1);
    set(my, PASSABLE | NOFILTER);
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
        ent_rotate_to_camera(my);

        my->skill1 += time_step;
        my->frame = my->skill1 + 1;
        wait(1);
    }
    ent_delete_later(my);
}

// underwater explosion sprite
void explosion_underwater_sprite()
{
    vec_fill(&my->scale_x, 0.5);
    set(my, PASSABLE | NOFILTER);
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
        ent_rotate_to_camera(my);

        my->skill1 += time_step;
        my->frame = my->skill1 + 1;
        wait(1);
    }
    ent_delete_later(my);
}

// explosion sprite (for quake like particle effect)
void explosion_particle_sprite()
{
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

// function for creating explosions
void explosion_create(VECTOR *pos, var is_barrel)
{
    VECTOR spawn_pos;
    vec_set(&spawn_pos, pos);

    VECTOR reg_min;
    VECTOR reg_max;
    var water_region_count = 0;
    var water_plane_height = 0;
    var camera_in_water = false;

    // check if explosion is in water, near the water plane or out of the water !
    if (region_check(reg_water_str, vector(spawn_pos.x, spawn_pos.y, spawn_pos.z - 16), vector(spawn_pos.x, spawn_pos.y, spawn_pos.z + 16)))
    {
        // if camera is underwater ?
        if (region_check(reg_water_str, &camera->x, &camera->x))
        {
            camera_in_water = true;
        }

        // find the region we are in right now
        while (region_get(reg_water_str, water_region_count, &reg_min, &reg_max) != 0)
        {
            water_region_count += 1;

            if (spawn_pos.x < reg_min.x || spawn_pos.x > reg_max.x)
            {
                continue;
            }
            if (spawn_pos.y < reg_min.y || spawn_pos.y > reg_max.y)
            {
                continue;
            }
            if ((spawn_pos.z + 16) < reg_min.z || (spawn_pos.z - 16) > reg_max.z)
            {
                continue;
            }

            water_plane_height = reg_max.z;
        }

        // if our center above the water, then damage is gonna be the same
        // as if we were not in water at all
        if ((water_plane_height - spawn_pos.z) < 16)
        {
            // light flashing effect (above the water)
            // with c_scan for damage !
            ent_create(NULL, &spawn_pos, explosion_light_fnc);
        }
        else
        {
            // light flashing effect (from inside of the water)
            // with c_scan for damage !
            // damage from the underwater explosions is smaller !
            ent_create(NULL, &spawn_pos, explosion_light_in_water_fnc);
        }

        // check if we are near the water plane, or deep in the water
        if ((water_plane_height - spawn_pos.z) < 32)
        {
            // and camera is in water ?
            if (camera_in_water == true)
            {
                // play underwater explosion sfx
                explosion_snd_in_water(&spawn_pos);
            }
            else
            {
                // play on water explosion sfx
                explosion_snd_on_water(&spawn_pos);
            }
            water_create_big_impact(vector(spawn_pos.x, spawn_pos.y, water_plane_height), 1.25);
            effect(explosion_underwater_particle, 512, vector(spawn_pos.x, spawn_pos.y, water_plane_height - 2), vector(water_plane_height, 0, 0));
        }
        else
        {
            // we are deep in the water
            explosion_snd_in_water(&spawn_pos);
            effect(explosion_underwater_particle, 512, &spawn_pos, vector(water_plane_height, 0, 0));

#ifndef PARTICLE_EFFECTS
            ent_create(explo_underwater_tga, &spawn_pos, explosion_underwater_sprite);
#else
            ent_create(explo_underwater_particle_sprite_tga, &spawn_pos, explosion_particle_sprite);
#endif
        }
    }
    else // out of water !
    {
        // light flashing effect (above the water)
        // with c_scan for damage !
        ent_create(NULL, &spawn_pos, explosion_light_fnc);

        explosion_snd_out_of_water(&spawn_pos);

#ifndef PARTICLE_EFFECTS

        // if this is a barrel ?
        if (is_barrel == true)
        {
            ent_create(explo_barrel_tga, &spawn_pos, explosion_barrel_sprite);
        }
        else
        {
            ent_create(explo_big_tga, &spawn_pos, explosion_sprite);
        }
#else
        effect(explosion_particle, 512, &spawn_pos, nullvector);
        ent_create(explo_particle_sprite_tga, &spawn_pos, explosion_particle_sprite);
#endif
    }
}