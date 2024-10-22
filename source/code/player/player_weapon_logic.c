
// find better weapon
void player_weapon_find_better(PLAYER *hero, var num)
{
    // play 'dry shoot sound'
    snd_play(weapon_dry_shoot_ogg, weapon_draw_volume, 0);

    var i = 0;
    for (i = num; i >= 0; i--)
    {
        if (hero->weapon[i].collected == 1 && weapon_id != i)
        {
            player_switch_weapon_by_id(hero, i);
            return;
        }
    }
}

// reset weapons input
void player_weapon_reset_input(PLAYER *hero)
{
    hero->wpn_slot_1 = 0;
    hero->wpn_slot_2 = 0;
    hero->wpn_slot_3 = 0;
    hero->wpn_slot_4 = 0;
    hero->wpn_slot_5 = 0;
    hero->wpn_slot_6 = 0;
    hero->wpn_slot_7 = 0;
    hero->wpn_slot_8 = 0;
    hero->wpn_slot_9 = 0;
    hero->wpn_shoot = 0;
}

// reset weapon by id
void player_weapon_reset_by_id(PLAYER *hero, var num)
{
    hero->weapon[weapon_id].animate = false;

    if (hero->weapon[num].ent)
    {
        set(hero->weapon[num].ent, INVISIBLE);
        hero->weapon[num].ent->frame = 0;
        hero->weapon[num].ent->OBJ_TIMER = 0;
        hero->weapon[num].ent->OBJ_STATE = PLAYER_WPN_WAIT;

        if (snd_playing(hero->weapon[num].ent->OBJ_SND_HANDLE))
        {
            snd_stop(hero->weapon[num].ent->OBJ_SND_HANDLE);
        }
    }
}

// reset all weapons
void player_weapons_reset_all(PLAYER *hero)
{
    var i = 0;
    for (i = 0; i < MAX_WEAPONS; i++)
    {
        player_weapon_reset_by_id(hero, i);
    }
}

// activate weapon by id
void player_weapon_activate_by_id(PLAYER *hero, var num)
{
    var i = 0;
    for (i = 0; i < MAX_WEAPONS; i++)
    {
        if (i != num)
        {
            continue;
        }

        if (hero->weapon[i].ent)
        {
            reset(hero->weapon[i].ent, INVISIBLE);
            hero->weapon[num].ent->OBJ_TIMER = 0;
            hero->weapon[num].ent->OBJ_STATE = PLAYER_WPN_DRAW;
        }
    }
}

// function used for all weapon sprites
void player_weapon_fnc()
{
    set(my, PASSABLE | NOFILTER | ZNEAR | INVISIBLE);
    vec_fill(&my->scale_x, 0.0625);
    my->ambient = 100;
    my->OBJ_TYPE = TYPE_PLAYER_WEAPON;
    my->OBJ_STATE = PLAYER_WPN_DRAW;

#ifndef FREE_VERSION
    my->material = mtl_z_write;
#endif
}

// setup basic weapon settings at start
void player_weapons_initialize(ENTITY *ent, PLAYER *hero)
{
    // reset weapon parameters
    weapon_id = PLAYER_PISTOL;
    weapon_fire_key_busy = false;
    weapon_in_use = false;
    hero->weapon_draw_counter = weapon_draw_time;
    hero->weapon_casing_spawn = true;

    // init all weapons here
    player_saw_init(hero);
    player_pistol_init(hero);
    player_shotgun_init(hero);
    player_sshotgun_init(hero);
    player_chaingun_init(hero);
    player_rocketlauncher_init(hero);
    player_lasergun_init(hero);
}

// update weapon's pos/angle
void player_weapon_view_pos(PLAYER *hero)
{
    var x_offset = 15;
    var z_offset = 16;

    VECTOR position;
    position.x = fcos(0.5 * camera->arc, x_offset) - (hero->cam.explo_vector.x * 0.15);
    position.y = -weapon_bob_y - (hero->cam.explo_vector.y * 0.15);
    position.z = 8 - fsin(0.5 * camera->arc, z_offset) * screen_size.y / screen_size.x - weapon_bob_z - weapon_z_land - (hero->weapon_draw_counter * 10);

    vec_set(&hero->weapon[weapon_id].ent->x, &position);
    vec_rotate(&hero->weapon[weapon_id].ent->x, &camera->pan);
    vec_add(&hero->weapon[weapon_id].ent->x, &camera->x);

    ANGLE temp_angle;
    vec_set(&temp_angle, vector(180, 0, 0));
    ang_add(&temp_angle, &camera->pan);
    vec_set(&hero->weapon[weapon_id].ent->pan, &temp_angle);
}

// handle all weapon animations
void player_weapon_animate(PLAYER *hero)
{
    switch (weapon_id)
    {
    case PLAYER_SAW:
        player_saw_animate(hero);
        break;

    case PLAYER_PISTOL:
        player_pistol_animate(hero);
        break;

    case PLAYER_SHOTGUN:
        player_shotgun_animate(hero);
        break;

    case PLAYER_SSHOTGUN:
        player_sshotgun_animate(hero);
        break;

    case PLAYER_CHAINGUN:
        player_chaingun_animate(hero);
        break;

    case PLAYER_ROCKETLAUNCHER:
        player_rocketlauncher_animate(hero);
        break;

    case PLAYER_LASERGUN:
        player_lasergun_animate(hero);
        break;
    }

    // casing part
    // no shells for saw, rocketlauncher and lasergun
    if (weapon_id != PLAYER_SAW && weapon_id != PLAYER_ROCKETLAUNCHER && weapon_id != PLAYER_LASERGUN)
    {
        if (hero->weapon[weapon_id].anim_frame >= hero->weapon[weapon_id].anim_casing_frame)
        {
            if (hero->weapon_casing_spawn == true)
            {
                VECTOR casing_pos;
                vec_set(&casing_pos, &hero->weapon[weapon_id].casing_pos);
                vec_rotate(&casing_pos, &camera->pan);
                vec_add(&casing_pos, &camera->x);

                if (weapon_id == PLAYER_SHOTGUN || weapon_id == PLAYER_SSHOTGUN)
                {
                    ent_create(bbox_casing_mdl, &casing_pos, shell_func);

                    if (weapon_id == PLAYER_SSHOTGUN)
                    {
                        // one more shell, since it's double barreled shoutgun
                        ent_create(bbox_casing_mdl, &casing_pos, shell_func);
                    }
                }
                else
                {
                    ent_create(bbox_casing_mdl, &casing_pos, casing_func);
                }

                hero->weapon_casing_spawn = false;
            }
        }
    }
}

// switch to weapon by id
void player_switch_weapon_by_id(PLAYER *hero, var num)
{
    if (hero->weapon[num].collected == 1 && weapon_id != num)
    {
        // reset current weapon
        player_weapon_reset_by_id(hero, weapon_id);

        // reset variables
        hero->weapon_draw_counter = 0;
        weapon_in_use = false;
        weapon_do_recoil = false;

        hero->weapon[num].ent->OBJ_STATE = PLAYER_WPN_HIDE;
        weapon_id = num;
    }
}

// switch weapons
void player_switch_weapons(PLAYER *hero)
{
    if (hero->wpn_slot_1 && weapon_in_use == true)
    {
        player_switch_weapon_by_id(hero, PLAYER_SAW);
    }

    if (hero->wpn_slot_2 && weapon_in_use == true)
    {
        player_switch_weapon_by_id(hero, PLAYER_PISTOL);
    }

    if (hero->wpn_slot_3 && weapon_in_use == true)
    {
        player_switch_weapon_by_id(hero, PLAYER_SHOTGUN);
    }

    if (hero->wpn_slot_4 && weapon_in_use == true)
    {
        player_switch_weapon_by_id(hero, PLAYER_SSHOTGUN);
    }

    if (hero->wpn_slot_5 && weapon_in_use == true)
    {
        player_switch_weapon_by_id(hero, PLAYER_CHAINGUN);
    }

    if (hero->wpn_slot_6 && weapon_in_use == true)
    {
        player_switch_weapon_by_id(hero, PLAYER_ROCKETLAUNCHER);
    }

    if (hero->wpn_slot_7 && weapon_in_use == true)
    {
        player_switch_weapon_by_id(hero, PLAYER_LASERGUN);
    }
}

// main function to update all weapons
void player_weapons_update(ENTITY *ent, PLAYER *hero)
{
    // player is dead ?
    // terminate
    if (ent->OBJ_HEALTH <= 0)
    {
        player_weapons_reset_all(hero);
        return;
    }

    // supershotgun uses shotgun shells
    hero->weapon[PLAYER_SSHOTGUN].ammo = hero->weapon[PLAYER_SHOTGUN].ammo;

    // update ammo/damage
    player_ammo = hero->weapon[weapon_id].ammo;

    // state machine
    // drawing ?
    if (hero->weapon[weapon_id].ent->OBJ_STATE == PLAYER_WPN_DRAW)
    {
        // run once
        if (hero->weapon_snd_switch != PLAYER_WPN_DRAW)
        {
            // make us visible
            reset(hero->weapon[weapon_id].ent, INVISIBLE);

            // saw draw sound (stop or play if needed)
            player_saw_draw(hero);

            // generic draw sound for all weapons (except saw)
            if (weapon_id != PLAYER_SAW)
            {
                snd_play(weapon_draw_ogg, weapon_draw_volume, 0);
            }
            hero->weapon_snd_switch = PLAYER_WPN_DRAW;
        }

        hero->weapon_draw_counter -= time_frame / 8;
        if (hero->weapon_draw_counter <= 0.01)
        {
            weapon_in_use = true;
            hero->weapon_draw_counter = 0;
            hero->weapon[weapon_id].ent->OBJ_STATE = PLAYER_WPN_IDLE;
        }
    }

    // idle ?
    if (hero->weapon[weapon_id].ent->OBJ_STATE == PLAYER_WPN_IDLE)
    {
        // pressed fire button
        if (hero->wpn_shoot)
        {
            // we have ammo left ?
            if (hero->weapon[weapon_id].ammo >= hero->weapon[weapon_id].ammo_per_shoot)
            {
                // fully automatic weapon ?
                if (hero->weapon[weapon_id].semiauto == false)
                {
                    weapon_do_recoil = true;
                    weapon_fire_key_busy = true;
                    hero->weapon[weapon_id].ent->OBJ_TIMER = 0;
                    hero->weapon[weapon_id].ent->OBJ_STATE = PLAYER_WPN_SHOOT;
                }
                else
                {
                    // if this is semiautomatic weapon, wait for it to become 'not busy'
                    // means, till player releases the shoot button
                    if (weapon_fire_key_busy == false)
                    {
                        weapon_do_recoil = true;
                        weapon_fire_key_busy = true;
                        hero->weapon[weapon_id].ent->OBJ_TIMER = 0;
                        hero->weapon[weapon_id].ent->OBJ_STATE = PLAYER_WPN_SHOOT;
                    }
                }
            }
            else
            {
                // switch for to better weapon
                player_weapon_find_better(hero, weapon_id);
            }
        }
        else
        {
            weapon_fire_key_busy = false;
        }

        // run once
        if (hero->weapon_snd_switch != PLAYER_WPN_IDLE)
        {
            // saw idling (play sound if needed etc)
            player_saw_idle(hero);

            hero->weapon_snd_switch = PLAYER_WPN_IDLE;
        }
    }

    // shooting delay ?
    if (hero->weapon[weapon_id].ent->OBJ_STATE == PLAYER_WPN_SHOOT)
    {
        // not holding down the fire button ?
        // then realese the switch
        if (hero->wpn_shoot == 0 && hero->weapon[weapon_id].semiauto == true)
        {
            weapon_fire_key_busy = false;
        }

        if (hero->weapon_snd_switch != PLAYER_WPN_SHOOT)
        {
            // allow to spawn casings
            hero->weapon_casing_spawn = true;

            // saw ?
            player_saw_snd_attack(hero);

            // other weapons
            if (weapon_id != PLAYER_SAW)
            {
                // light flashing
                ent->lightrange = 192;

                // decrease ammo
                // for supershotgun, we should take shotgun's ammo
                if (weapon_id == PLAYER_SSHOTGUN)
                {
                    hero->weapon[PLAYER_SHOTGUN].ammo -= hero->weapon[weapon_id].ammo_per_shoot;
                }
                else
                {
                    hero->weapon[weapon_id].ammo -= hero->weapon[weapon_id].ammo_per_shoot;
                }

                // play shooting sound
                if (hero->weapon[weapon_id].snd)
                {
                    hero->weapon[weapon_id].ent->OBJ_SND_HANDLE = snd_play(hero->weapon[weapon_id].snd, weapon_shoot_volume, 0);
                }
            }

            // animate
            hero->weapon[weapon_id].animate = true;

            // reset frames (but not for saw, since it's looping)
            if (weapon_id != PLAYER_SAW)
            {
                hero->weapon[weapon_id].anim_frame = 0;
            }

            // call shooting function here
            ent->OBJ_C_INDICATOR = SHOOT;
            weapon_function = hero->weapon[weapon_id].fnc;
            if (weapon_function != NULL)
            {
                weapon_function(hero);
            }

            hero->weapon_snd_switch = PLAYER_WPN_SHOOT;
        }

        hero->weapon[weapon_id].ent->OBJ_TIMER += time_frame / 16;
        while (hero->weapon[weapon_id].ent->OBJ_TIMER >= hero->weapon[weapon_id].fire_rate)
        {
            weapon_do_recoil = false;
            hero->weapon[weapon_id].ent->OBJ_STATE = PLAYER_WPN_IDLE;
            hero->weapon[weapon_id].ent->OBJ_TIMER -= hero->weapon[weapon_id].fire_rate;
        }
    }

    // hide ?
    if (hero->weapon[weapon_id].ent->OBJ_STATE == PLAYER_WPN_HIDE)
    {
        if (hero->weapon_snd_switch != PLAYER_WPN_HIDE)
        {
            snd_play(weapon_holster_ogg, weapon_draw_volume, 0);
            hero->weapon_snd_switch = PLAYER_WPN_HIDE;
        }

        hero->weapon_draw_counter += time_frame / 8;
        if (hero->weapon_draw_counter >= weapon_draw_time)
        {
            // activate the one we are using now
            player_weapon_activate_by_id(hero, weapon_id);

            hero->weapon[weapon_id].ent->OBJ_STATE = PLAYER_WPN_DRAW;
            hero->weapon_draw_counter = weapon_draw_time;
        }
    }

    // switch weapons
    player_switch_weapons(hero);

    // handle all weapon animations
    player_weapon_animate(hero);

    // update weapon's position on screen
    player_weapon_view_pos(hero);

    // light flashing on shoots
    ent->lightrange -= 50 * time_step;
    ent->lightrange = clamp(ent->lightrange, 0, 192);
}