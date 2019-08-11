
// detect water regions and change our water movement factor, if we are in water
void ent_detect_water(ENTITY *ent, CCT *cct)
{
    if (!ent)
    {
        diag("\nERROR! No water region detection, entity doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! No water region detection, cct doesn't exist");
        return;
    }

    cct->water_reg_total = 0;
    cct->water_depth_factor = 1;

    if (cct->water_reg_total == 0)
    {
        cct->water_origin_diff_z = 0;
        cct->water_z_height = 0;
    }

    while (region_get(reg_water_str, cct->water_reg_total, &cct->water_reg_min, &cct->water_reg_max) != 0)
    {
        cct->water_reg_total += 1;

        if (ent->x < cct->water_reg_min.x || ent->x > cct->water_reg_max.x)
        {
            continue;
        }
        if (ent->y < cct->water_reg_min.y || ent->y > cct->water_reg_max.y)
        {
            continue;
        }
        if ((cct->water_reg_min.z - ent->z) > 0 || (ent->z - cct->water_reg_max.z) > 0)
        {
            continue;
        }

        cct->water_origin_diff_z = cct->water_reg_max.z - ent->z;
        cct->water_z_height = cct->water_reg_max.z;
        cct->water_depth_factor = clamp(1 - (cct->water_origin_diff_z / 25), 0.1, 1);
    }
}

// set water state for the given cct
void ent_set_water_state(CCT *cct)
{
    if (!cct)
    {
        diag("\nERROR! Can't set cct water state, cct doesn't exist");
        return;
    }

    // if we are in water
    if (cct->water_origin_diff_z > 0 && cct->water_origin_diff_z < (cct->bbox_z / 2))
    {
        cct->water_state = ON_WATER;
    }
    else
    {
        if (cct->water_origin_diff_z > (cct->bbox_z / 2))
        {
            cct->water_state = IN_WATER;
        }
        else
        {
            cct->water_state = OUT_OF_WATER;
        }
    }
}

// find surface to hop onto
void ent_detect_water_edge(ENTITY *ent, CCT *cct)
{
    if (!ent)
    {
        diag("\nERROR! Can't detect any edges (when in water), entity doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! Can't detect any edges (when in water), cct doesn't exist");
        return;
    }

    // only if we are in water !
    // otherwise, player will be able to climb edges :D
    if (cct->water_state < IN_WATER && cct_allow_cheap_edge_climb == false)
    {
        return;
    }

    var trace_length = cct->bbox_x;
    vec_set(&cct->water_out_trace_mid, vector(cct->bbox_x + trace_length, 0, 0));
    vec_rotate(&cct->water_out_trace_mid, vector(cct->pan_rotation, 0, 0));
    vec_add(&cct->water_out_trace_mid, &ent->x);
    vec_set(&cct->water_out_trace_top, &cct->water_out_trace_mid);
    cct->water_out_trace_top.z += cct->bbox_z;

    c_ignore(WATER_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
    ent_trace(ent, &ent->x, &cct->water_out_trace_mid, IGNORE_PUSH);
    cct->water_mid_trace_result = false;
    if (HIT_TARGET)
    {
        cct->water_mid_trace_result = true;
    }

    c_ignore(WATER_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
    ent_trace(ent, vector(ent->x, ent->y, cct->water_out_trace_top.z), &cct->water_out_trace_top, IGNORE_PUSH);
    cct->water_top_trace_result = false;
    if (HIT_TARGET)
    {
        cct->water_top_trace_result = true;
    }

    // logic is pretty simple, if top trace doesn't hit anything, while the middle one does
    // then it means that we can try to hop onto it
    cct->jump_out_of_water = false;
    if (cct->water_mid_trace_result == true && cct->water_top_trace_result == false)
    {
        cct->jump_out_of_water = true;
    }

    // and for npcs, we need to force them to jump up
    if (ent->OBJ_TYPE != TYPE_PLAYER)
    {
        cct->jump = true;
    }
}

// detect if entity is inside of the water region, or not
void ent_detect_water_state(ENTITY *ent, CCT *cct)
{
    if (!ent)
    {
        diag("\nERROR! Can't detect cct water state, entity doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! Can't detect cct water state, cct doesn't exist");
        return;
    }

    // perform only if wasn't smashed !
    if (ent->OBJ_DEATH_TYPE == TYPE_SMASHED)
    {
        return;
    }

    // detect water region
    ent_detect_water(ent, cct);

    // detect surface to hop onto
    ent_detect_water_edge(ent, cct);

    // if player ?
    if (ent->OBJ_TYPE == TYPE_PLAYER)
    {
        // had to make this dirty trick with camera
        // to avoid 'no fog' thing when just entered water
        VECTOR temp_camera;
        vec_set(&temp_camera, vector(camera->x, camera->y, camera->z - 0.5));

        // camera is inside of the water ?
        // then our head is in water !
        if (region_check(reg_water_str, &temp_camera, &temp_camera))
        {
            cct->water_state = HEAD_IN_WATER;
        }
        else
        {
            // handle all other states as usual
            ent_set_water_state(cct);
        }
    }
    else
    {
        // handle all other states as usual
        ent_set_water_state(cct);
    }

    // handle water sounds
    ent_water_interaction_sounds(ent, cct);

    // handle underwater air (breathing), only if we are alive
    if (ent->OBJ_HEALTH > 0)
    {
        // handle breathing underwater
        ent_air_underwater(ent, cct);
    }
}

// handle sounds when player get in or out of the water
void ent_water_interaction_sounds(ENTITY *ent, CCT *cct)
{
    if (!ent)
    {
        diag("\nERROR! No water interaction sounds, entity doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! No water interaction sounds, cct doesn't exist");
        return;
    }

    // handle surfacing sounds
    ent_water_surface_sound(ent, cct);

    // if we are swimming
    if (cct->water_state >= IN_WATER)
    {
        // if npc ?
        if (ent->OBJ_TYPE != TYPE_PLAYER)
        {
            // stop all voice sounds when in water
            if (snd_playing(ent->OBJ_SND_HANDLE))
            {
                snd_stop(ent->OBJ_SND_HANDLE);
            }
        }

        // we jumped into the water?
        if (cct->falling_timer > 1 && cct->water_in_switch == false)
        {
            // play water splash sound
            if (ent->OBJ_TYPE == TYPE_PLAYER)
            {
                snd_play(water_in_ogg, player_snd_volume, 0);
            }
            else
            {
                ent_playsound(ent, water_in_ogg, cct_snd_volume);
            }

            // create impact on water place
            water_cct_impact(vector(ent->x, ent->y, cct->water_z_height + 0.1), 0.5);

            // add some underwater bubbles
            bubbles_spawn(&cct->origin, 64, vector(16, 16, 32), cct->water_z_height);

            cct->water_in_switch = true;
        }

        // we walked into the water slowly
        // so play in water slowly sound here
        if (cct->falling_timer < 1 && cct->water_in_switch == false)
        {
            if (ent->OBJ_TYPE == TYPE_PLAYER)
            {
                snd_play(water_in_slow_ogg, player_snd_volume, 0);
            }
            else
            {
                ent_playsound(ent, water_in_slow_ogg, cct_snd_volume);
            }

            cct->water_in_switch = true;
        }
    }
    else
    {
        // we are out of the water and standing on surface ?
        if (cct->water_out_switch == false && cct->is_grounded == true)
        {
            // play water splash sound
            if (ent->OBJ_TYPE == TYPE_PLAYER)
            {
                snd_play(water_out_ogg, player_snd_volume, 0);
            }
            else
            {
                ent_playsound(ent, water_out_ogg, cct_snd_volume);
            }

            cct->water_out_switch = true;
        }
    }
}

// handle water sounds, when entity is surfacing
void ent_water_surface_sound(ENTITY *ent, CCT *cct)
{
    if (!ent)
    {
        diag("\nERROR! No water surface sounds, entity doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! No water surface sounds, cct doesn't exist");
        return;
    }

    if (cct->water_z_is_moving == true && cct->water_state == HEAD_IN_WATER)
    {
        if (cct->water_z_snd_switch == 0 && cct->swim_z_abs_dist > 0)
        {
            var rnd = integer(random(2));
            if (ent->OBJ_TYPE == TYPE_PLAYER)
            {
                if (rnd == 0)
                {
                    snd_play(player_swim_01_ogg, player_snd_volume, 0);
                }
                if (rnd == 1)
                {
                    snd_play(player_swim_02_ogg, player_snd_volume, 0);
                }
            }
            else
            {
                // play npc's swimming up sounds here, if needed
            }

            cct->water_z_snd_switch = 1;
        }

        if (cct->swim_z_abs_dist < 0 && cct->water_z_snd_switch == 1)
        {
            cct->water_z_snd_switch = 0;
        }
    }
    else
    {
        cct->water_z_snd_switch = 0;
    }
}

// handle breathing underwater, and if out of air, then take damage
void ent_air_underwater(ENTITY *ent, CCT *cct)
{
    if (!ent)
    {
        diag("\nERROR! No air handling underwater, entity doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! No air handling underwater, cct doesn't exist");
        return;
    }

    // if our head is under water ?
    if (cct->water_state == HEAD_IN_WATER)
    {
        // decrease air
        cct->air_underwater = clamp(cct->air_underwater -= 0.5 * time_step, 0, 100);

        // if no air, take damage
        if (cct->air_underwater <= 0)
        {
            cct->air_underwater_timer += time_frame / 16;

            if (cct->air_underwater_timer >= cct->air_underwater_timer_limit)
            {
                // play drown sound effects for player
                if (ent->OBJ_TYPE == TYPE_PLAYER)
                {
                    if (snd_playing(ent->OBJ_SND_HANDLE))
                    {
                        snd_stop(ent->OBJ_SND_HANDLE);
                    }
                    var rnd = integer(random(2));
                    if (rnd == 0)
                    {
                        ent->OBJ_SND_HANDLE = snd_play(player_drown_01_ogg, player_snd_volume, 0);
                    }
                    if (rnd == 1)
                    {
                        ent->OBJ_SND_HANDLE = snd_play(player_drown_02_ogg, player_snd_volume, 0);
                    }
                }

                // add some underwater bubbles
                VECTOR bubble_pos;
                vec_set(&bubble_pos, vector(8, 0, cct->bbox_z / 2));
                vec_rotate(&bubble_pos, vector(cct->pan_rotation, 0, 0));
                vec_add(&bubble_pos, &cct->origin);
                bubbles_spawn(&bubble_pos, 16, vector(2, 2, 2), cct->water_z_height);

                ent->OBJ_PAIN_TYPE = TYPE_OUT_OF_AIR;
                ent->OBJ_HEALTH -= cct_underwater_no_air_damage + random(5);
                cct->air_underwater_timer %= cct->air_underwater_timer_limit;
            }
        }

        // allow to play gasp sounds
        if (cct->air_underwater < 75 && cct->air_underwater_snd_gasp_switch == 1)
        {
            // out of water!
            cct->air_underwater_snd_gasp_switch = 0;
        }
    }
    else
    {
        // but still in water ?
        if (cct->water_state == IN_WATER)
        {
            // create water ripple effect
            if ((total_frames % 10) == 1 && cct->is_moving == true)
            {
                water_create_ripple_effect(vector(ent->x, ent->y, cct->water_z_height + 0.1), 0.5);
            }
            else
            {
                if ((total_frames % 50) == 1)
                {
                    water_create_ripple_effect(vector(ent->x, ent->y, cct->water_z_height + 0.1), 0.5);
                }
            }
        }

        // increase air when out of water
        cct->air_underwater = clamp(cct->air_underwater += 2 * time_step, 0, 100);

        // play gasp sounds here, if this is player
        if (ent->OBJ_TYPE == TYPE_PLAYER)
        {
            if (cct->air_underwater < 75 && cct->air_underwater_snd_gasp_switch == 0)
            {
                if (snd_playing(ent->OBJ_SND_HANDLE))
                {
                    snd_stop(ent->OBJ_SND_HANDLE);
                }

                if (cct->air_underwater > 25)
                {
                    ent->OBJ_SND_HANDLE = snd_play(player_gasp_01_ogg, player_snd_volume, 0);
                }
                else
                {
                    ent->OBJ_SND_HANDLE = snd_play(player_gasp_02_ogg, player_snd_volume, 0);
                }

                cct->air_underwater_snd_gasp_switch = 1;
            }
        }
    }
}