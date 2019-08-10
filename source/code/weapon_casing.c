
// play impact sound
// each casing plays impact sound only twice !
// ignore if more than 4 casings were createds
void casing_impact_snd(ENTITY *ent, var is_shell)
{
    if (weapon_casing_snd_counter > 4 && vec_dist(&ent->x, &camera->x) > 128)
    {
        return;
    }

    // stop previous impact sound
    snd_stop(ent->OBJ_SND_HANDLE);

    var rnd = integer(random(3));
    if (is_shell == false)
    {
        if (rnd == 0)
        {
            ent->OBJ_SND_HANDLE = ent_playsound(ent, casing_hit_01_ogg, weapon_casing_snd_volume);
        }
        if (rnd == 1)
        {
            ent->OBJ_SND_HANDLE = ent_playsound(ent, casing_hit_02_ogg, weapon_casing_snd_volume);
        }
        if (rnd == 2)
        {
            ent->OBJ_SND_HANDLE = ent_playsound(ent, casing_hit_03_ogg, weapon_casing_snd_volume);
        }
    }
    else
    {
        if (rnd == 0)
        {
            ent->OBJ_SND_HANDLE = ent_playsound(ent, shell_hit_01_ogg, weapon_casing_snd_volume);
        }
        if (rnd == 1)
        {
            ent->OBJ_SND_HANDLE = ent_playsound(ent, shell_hit_02_ogg, weapon_casing_snd_volume);
        }
        if (rnd == 2)
        {
            ent->OBJ_SND_HANDLE = ent_playsound(ent, shell_hit_03_ogg, weapon_casing_snd_volume);
        }
    }
}

// function to update casing
void casing_update(ENTITY *ent)
{
    // if can't play impact sounds..
    if (ent->CASING_HIT_COUNT > 3 && ent->OBJ_CHECK != 1)
    {
        weapon_casing_snd_counter--;
        ent->OBJ_CHECK = 1;
    }

    // we still can bounce ?
    if (ent->CASING_HIT_COUNT < 5)
    {
        ent->CASING_VEL_Z -= time_step;
        c_ignore(PUSH_GROUP, WATER_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, ENEMY_GROUP, 0);
        ent->CASING_DIST = ent_move(ent, nullvector, &ent->CASING_VEL_X, IGNORE_PASSABLE | IGNORE_FLAG2);

        if (HIT_TARGET)
        {
            // play impact sounds
            // only if not in water
            if (ent->OBJ_HEALTH != -1 && ent->CASING_HIT_COUNT <= 2)
            {
                casing_impact_snd(ent, ent->CASING_IS_SHELL);
            }

            // change the visual sprite
            if (ent->parent && ent->parent->skill1 == 0)
            {
                if (ent->CASING_IS_SHELL == true)
                {
                    ent_morph(ent->parent, shell_floor_tga);
                }
                else
                {
                    ent_morph(ent->parent, casing_floor_tga);
                }
                ent->parent->skill1 = -1;
            }

            // hit a wall?
            if (normal.z == 0)
            {
                // one more hit to die
                ent->CASING_HIT_COUNT -= 1;

                // move us away from the wall
                ent->CASING_VEL_X = -ent->CASING_VEL_X;
                ent->CASING_VEL_Y = -ent->CASING_VEL_Y;
            }

            // slow down
            ent->CASING_VEL_X = ent->CASING_VEL_X * 0.9;
            ent->CASING_VEL_Y = ent->CASING_VEL_Y * 0.9;
            ent->CASING_HIT_COUNT += 1;

            // bounce Z
            if (ent->CASING_HIT_COUNT < 3)
            {
                ent->CASING_VEL_Z *= -(0.5 + random(0.25));
                if (abs(ent->CASING_VEL_Z) < 1)
                {
                    vec_set(&ent->CASING_VEL_X, nullvector);
                }
            }
            else
            {
                // reset XY movement, slow down Z bouncing
                ent->CASING_VEL_X = 0;
                ent->CASING_VEL_Y = 0;
                ent->CASING_VEL_Z *= -(0.5 + random(0.25));
            }
        }

        // we get in water ?
        if (region_check(reg_water_str, &ent->x, &ent->x) && ent->OBJ_HEALTH != -1)
        {
            // play got in water sound for casing
            VECTOR place_pos;
            vec_fill(&place_pos, 0);
            water_get_pos_on_surface(ent, &place_pos);
            water_small_impact_snd(ent);
            water_create_small_impact(&place_pos, 0.15);
            ent->OBJ_HEALTH = -1;
        }
    }

    if (ent->parent)
    {
        vec_set(&ent->parent->x, &ent->x);

        // if we are flying ?
        // animate flying casing/shell
        if (ent->parent->skill1 == 0)
        {
            ent->parent->skill2 += 2 * time_step;
            ent->parent->skill2 %= 8;
            ent->parent->frame = ent->parent->skill2 + 1;
        }
        else
        {
            // set random frame
            // when we are on ground
            if (ent->parent->skill3 == 0)
            {
                ent->parent->skill2 = random(3);
                ent->parent->frame = ent->parent->skill2 + 1;
                ent->parent->skill3 = 1;
            }
        }

        // fade out slowly
        ent->parent->alpha -= 0.15 * time_step;
        if (ent->parent->alpha <= 0)
        {
            ent_delete_later(ent->parent);
            ent_delete_later(ent);
        }
    }
}

// init casing/shell
void casing_shell_init(var is_shell)
{
    weapon_casing_snd_counter++;
    vec_fill(&my->scale_x, 0.05);
    c_setminmax(my);
    set(my, INVISIBLE);
    my->OBJ_TYPE = TYPE_CASING;

    vec_set(&my->CASING_VEL_X, vector((4 - random(8)) * time_step, -(5 + random(3)) * time_step, (4 + random(2)) * time_step));
    vec_rotate(&my->CASING_VEL_X, &camera->pan);

    my->group = PUSH_GROUP;
    my->push = PUSH_GROUP;

    if (is_shell == true)
    {
        my->CASING_IS_SHELL = is_shell;
        my->parent = ent_create(shell_fly_tga, &my->x, NULL);
    }
    else
    {
        my->parent = ent_create(casing_fly_tga, &my->x, NULL);
    }

    vec_fill(&my->parent->scale_x, 0.075);
    set(my->parent, PASSABLE | NOFILTER | TRANSLUCENT);
    my->parent->alpha = 100;

    // already in water ?
    if (region_check(reg_water_str, &my->x, &my->x))
    {
        my->OBJ_HEALTH = -1;
    }
}

// function to setup casing
void casing_func()
{
    casing_shell_init(false);
}

// function to setup shells
void shell_func()
{
    casing_shell_init(true);
}