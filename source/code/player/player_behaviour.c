
// toggle auto run mode on and off
void player_toggle_run()
{
    if (player_allow_movement == false || player_health <= 0)
    {
        return;
    }
    if (level_is_loaded == false)
    {
        return;
    }

    player_always_run += 1;
    player_always_run %= 2;
}

// handles all voice sounds made by player
void player_sounds(ENTITY *ent, CCT *cct, PLAYER *hero)
{
    if (!ent)
    {
        diag("\nERROR! No sounds player, he doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! No sounds player, his cct doesn't exist!");
        return;
    }

    if (!hero)
    {
        diag("\nERROR! No sounds player, his structure doesn't exist!");
        return;
    }

    if (ent->OBJ_STATE == DEATH)
    {
        // smashed ?
        if (ent->OBJ_DEATH_TYPE == TYPE_SMASHED)
        {
            if (hero->death_snd_switch == 0)
            {
                if (snd_playing(ent->OBJ_SND_HANDLE))
                {
                    snd_stop(ent->OBJ_SND_HANDLE);
                }
                ent->OBJ_SND_HANDLE = snd_play(player_smashed_ogg, player_snd_volume, 0);

                hero->death_snd_switch = 1;
            }
        }
        else
        {
            if (hero->death_snd_switch == 0)
            {
                if (snd_playing(ent->OBJ_SND_HANDLE))
                {
                    snd_stop(ent->OBJ_SND_HANDLE);
                }
                var rnd = integer(random(2));
                if (rnd == 0)
                {
                    ent->OBJ_SND_HANDLE = snd_play(player_death_ogg, player_snd_volume, 0);
                }
                if (rnd == 1)
                {
                    ent->OBJ_SND_HANDLE = snd_play(player_x_death_ogg, player_snd_volume, 0);
                }

                hero->death_snd_switch = 1;
            }

            if (cct->water_state >= IN_WATER && hero->death_snd_switch == 1)
            {
                if (snd_playing(ent->OBJ_SND_HANDLE))
                {
                    snd_stop(ent->OBJ_SND_HANDLE);
                }
                ent->OBJ_SND_HANDLE = snd_play(player_death_to_water_ogg, player_snd_volume, 0);
                hero->death_snd_switch = -1;
            }
        }
    }
}

// handles all events that player receives
// such as EVENT_PUSH, EVENT_SCAN, EVENT_SHOOT etc
void player_event_function()
{
    if (event_type == EVENT_PUSH)
    {
        // smashed by props ?
        if (you->OBJ_TYPE == TYPE_ELEVATOR || you->OBJ_TYPE == TYPE_DOOR || you->OBJ_TYPE == TYPE_PLATFORM || you->OBJ_TYPE == TYPE_SECRET_WALL)
        {
            // get props structure
            PROPS *props = get_props(you);

            // get player's cct structure
            CCT *cct = get_cct(my);

            // push us away
            vec_set(&cct->push_force, vector(props->diff.x, props->diff.y, 0));

            // if we can perform check ?
            // means that door is almost closed (f.e.)
            if (you->OBJ_CHECK == true)
            {
                // perform check (if player is smashed or not)
                props_vs_npc_check(you, my, my->scale_x);
            }
        }
    }

    if (event_type == EVENT_SHOOT)
    {
    }

    if (event_type == EVENT_SCAN)
    {
    }
}

// handles all stuff related to after death life
void player_dead(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! No dead function for player, he doesn't exist!");
        return;
    }

    // reset lightrange
    ent->lightrange = 0;

    // no events from here on
    ent->emask &= ~(ENABLE_PUSH | ENABLE_SHOOT | ENABLE_SCAN);
    ent->event = NULL;
}

// handle all player's interactions
void player_interact(ENTITY *ent, CCT *cct)
{
    if (!ent)
    {
        diag("\nERROR! No interaction function for player, he doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! No interaction function for player, his cct doesn't exist!");
        return;
    }

    // when alive
    if (ent->OBJ_HEALTH > 0)
    {
        // handle interaction traces
        ent_interact(ent, cct);
    }
}

// player's update function
void player_update(ENTITY *ent)
{
    PLAYER *hero = get_player_struct(ent);
    if (!hero)
    {
        diag("\nERROR! No update function for player, his structure doesn't exist!");
        return;
    }

    CCT *cct = get_cct(ent);
    if (!cct)
    {
        diag("\nERROR! No update function for player, his cct doesn't exist!");
        return;
    }

    // for testing
    if (key_q)
    {
        ent->OBJ_HEALTH = -1;
    }
    DEBUG_VAR(ent->OBJ_HEALTH, 10);
    DEBUG_VAR(cct->air_underwater, 30);
    DEBUG_VAR(cct->falling_timer, 50);
    DEBUG_VAR(cct->soil_contact, 70);
    DEBUG_VAR(cct->is_grounded, 90);

    // update our health, armor etc
    // used globally - f.e. for gui
    player_health = ent->OBJ_HEALTH;
    player_armor = ent->OBJ_ARMOR;
    player_allow_movement = ent->OBJ_ALLOW_MOVE;
    cct->always_run = player_always_run;

    // perform gravity trace
    ent_gravity_trace(ent, cct);

    // shake camera, if we are on moving props
    if (cct->ground_info == MOVING)
    {
        hero->cam.explo_power = 0.15;
    }

    // alive?
    if (ent->OBJ_HEALTH > 0)
    {
        // allowed to move ?
        if (ent->OBJ_ALLOW_MOVE == 1)
        {
            // set input keys and update every frame
            cct->forward = key_w;
            cct->backward = key_s;
            cct->strafe_left = key_a;
            cct->strafe_right = key_d;
            cct->run = key_shift;
            cct->jump = key_space;
            cct->dive = (key_ctrl || key_c);
            cct->interact = (key_e || mouse_right);
            cct->pan_rotation = hero->cam.angle.pan;
        }
        else
        {
            // stop all movement, cause we aren't allowed to move !
            ent_stop_movement(cct);
        }
    }
    else
    {
        // handle all stuff related to death
        // f.e. disable lightrange, stop sounds etc
        player_dead(ent);

        // count down
        ent->OBJ_TIMER -= time_frame / 16;

        // allow to reset level, as soon as movement has stoped and camera has lowered
        if (mouse_left && ent->OBJ_TIMER <= 0)
        {
            // make sure that it's intentional
            ent->OBJ_STATE = -1;
        }
    }

    // handle movement
    ent_movement(ent, cct);

    // handle state machine
    player_handle_state_machine(ent, cct);

    // handle sound effects
    player_sounds(ent, cct, hero);

    // save our fake origin position
    vec_set(&cct->origin, vector(ent->x, ent->y, ent->z + 16));

    // attach and update camera
    camera_update(ent, cct, hero);

    // update weapons
    player_weapons_update(ent, hero);

    // interact with the world
    player_interact(ent, cct);
}

// main player's action
action player_controller()
{
    PLAYER *hero = register_player_struct(my);
    CCT *cct = register_cct(my);

    set(my, TRANSLUCENT);
    c_setminmax(my);
    my->max_z = cct->bbox_z;
    my->min_z = -0.1;

    my->OBJ_HEALTH = 100;
    my->OBJ_ARMOR = 0;
    my->OBJ_TYPE = TYPE_PLAYER;
    my->OBJ_ALLOW_MOVE = true;
    my->OBJ_TIMER = 1; // 1 second delay, before retry

    my->group = PLAYER_GROUP;
    my->push = PLAYER_GROUP;

    my->emask |= (ENABLE_PUSH | ENABLE_SHOOT | ENABLE_SCAN);
    my->event = player_event_function;

    // init weapons
    player_weapons_initialize(my, hero);
}