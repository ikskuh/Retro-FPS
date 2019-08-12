
// play saw hit sound
void saw_hit_solid_sfx()
{
    if (snd_playing(player_saw_hit_snd_handle))
    {
        snd_stop(player_saw_hit_snd_handle);
    }
    var rnd_snd = integer(random(3));
    if (rnd_snd == 0)
    {
        player_saw_hit_snd_handle = snd_play(weapon_saw_hit_01_ogg, player_saw_hit_snd_volume, 0);
    }
    if (rnd_snd == 1)
    {
        player_saw_hit_snd_handle = snd_play(weapon_saw_hit_02_ogg, player_saw_hit_snd_volume, 0);
    }
    if (rnd_snd == 2)
    {
        player_saw_hit_snd_handle = snd_play(weapon_saw_hit_03_ogg, player_saw_hit_snd_volume, 0);
    }
}

// play saw hit body sound
void saw_hit_body_sfx()
{
    if (snd_playing(player_saw_hit_snd_handle))
    {
        snd_stop(player_saw_hit_snd_handle);
    }
    var rnd_snd = integer(random(3));
    if (rnd_snd == 0)
    {
        player_saw_hit_snd_handle = snd_play(weapon_saw_hit_body_01_ogg, player_saw_hit_snd_volume, 0);
    }
    if (rnd_snd == 1)
    {
        player_saw_hit_snd_handle = snd_play(weapon_saw_hit_body_02_ogg, player_saw_hit_snd_volume, 0);
    }
    if (rnd_snd == 2)
    {
        player_saw_hit_snd_handle = snd_play(weapon_saw_hit_body_03_ogg, player_saw_hit_snd_volume, 0);
    }
}

// play saw impact sound
void saw_impact_fx(var is_alive, VECTOR *hit_vector, VECTOR *surf_angle)
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
        saw_hit_solid_sfx();
    }
    else
    {
        // add body hit impact sounds here
#ifndef PARTICLE_EFFECTS
        ent_create(blood_impact_tga, &impact_vec, bullet_blood_impact_sprite);
#else
        effect(blood_impact_particle, 8 + random(8), &impact_vec, nullvector);
#endif
        saw_hit_body_sfx();
    }
}

// stop saw sound
void player_saw_stop_snd(PLAYER *hero)
{
    if (snd_playing(hero->weapon[PLAYER_SAW].ent->OBJ_SND_HANDLE))
    {
        snd_stop(hero->weapon[PLAYER_SAW].ent->OBJ_SND_HANDLE);
    }
}

// initialize all saw parameters
void player_saw_init(PLAYER *hero)
{
    hero->weapon[PLAYER_SAW].collected = true;
    hero->weapon[PLAYER_SAW].semiauto = false;

    hero->weapon[PLAYER_SAW].ammo = 999;
    hero->weapon[PLAYER_SAW].ammo_per_shoot = 0;
    hero->weapon[PLAYER_SAW].fire_rate = 0.1;
    hero->weapon[PLAYER_SAW].recoil_strength = 0.25;
    hero->weapon[PLAYER_SAW].damage = 15;
    hero->weapon[PLAYER_SAW].accuracy = 1;

    hero->weapon[PLAYER_SAW].animate = false;
    hero->weapon[PLAYER_SAW].anim_total_frames = 4;
    hero->weapon[PLAYER_SAW].ent = ent_create(view_saw_tga, &camera->x, player_weapon_fnc);
    hero->weapon[PLAYER_SAW].fnc = player_saw_attack;
    hero->weapon[PLAYER_SAW].snd = weapon_saw_full_ogg;
}

// draw function for saw
void player_saw_draw(PLAYER *hero)
{
    // stop all saw sounds, if any there
    player_saw_stop_snd(hero);

    // if using saw ?
    if (weapon_id == PLAYER_SAW)
    {
        // play saw draw sound
        snd_play(weapon_saw_up_ogg, weapon_draw_volume, 0);
    }
}

// idle function for saw
void player_saw_idle(PLAYER *hero)
{
    // if not saw ?
    // then stop previous sounds and terminate
    if (weapon_id != PLAYER_SAW)
    {
        // stop all saw sounds, if any there
        player_saw_stop_snd(hero);

        // reset switch for attack sound
        hero->weapon[PLAYER_SAW].ent->OBJ_CHECK = 0;

        return;
    }

    // if using saw ?
    if (weapon_fire_key_busy == false)
    {
        // stop all saw sounds, if any there
        player_saw_stop_snd(hero);

        // reset switch for attack sound
        hero->weapon[PLAYER_SAW].ent->OBJ_CHECK = 0;

        // play idle sound
        hero->weapon[PLAYER_SAW].ent->OBJ_SND_HANDLE = snd_loop(weapon_saw_idle_ogg, weapon_idle_volume, 0);
    }
}

// function for playing saw attack sound
void player_saw_snd_attack(PLAYER *hero)
{
    // not saw ?
    // then ignore
    if (weapon_id != PLAYER_SAW)
    {
        return;
    }

    // shooting ?
    if (weapon_fire_key_busy == true)
    {
        // run this block once
        if (hero->weapon[PLAYER_SAW].ent->OBJ_CHECK == 0)
        {
            // stop all previous sounds, if any there
            player_saw_stop_snd(hero);

            // start looping attack sound
            hero->weapon[PLAYER_SAW].ent->OBJ_SND_HANDLE = snd_loop(weapon_saw_full_ogg, weapon_shoot_volume, 0);
            hero->weapon[PLAYER_SAW].ent->OBJ_CHECK = 1;
        }
    }
}

// saw attack function
void player_saw_func()
{
    set(my, PASSABLE | INVISIBLE);
    my->OBJ_TYPE = TYPE_SAW_TRACE;
}

// update saw tracing object
void player_saw_trace_update(ENTITY *ent)
{
    c_ignore(PUSH_GROUP, WATER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, PLAYER_GROUP, SHOOTABLE_GROUP, 0);
    ent_trace(ent, &ent->x, &ent->DIR_X, ACTIVATE_SHOOT | SCAN_TEXTURE | USE_POLYGON);

    if (HIT_TARGET)
    {
        if (you)
        {
            if (you->OBJ_TYPE == TYPE_NPC)
            {
                saw_impact_fx(true, &hit->x, &normal);
            }
            else
            {
                saw_impact_fx(false, &hit->x, &normal);
            }
        }
        else
        {
            saw_impact_fx(false, &hit->x, &normal);
        }
    }

    ent_delete_later(ent);
}

// create tracing object here
void player_saw_attack(PLAYER *hero)
{
    var accur = hero->weapon[PLAYER_SAW].accuracy;
    var damage = hero->weapon[PLAYER_SAW].damage;

    you = ent_create(NULL, &camera->x, player_saw_func);
    vec_set(&you->DIR_X, vector(16, (accur - random(accur * 2)), (accur - random(accur * 2))));
    vec_rotate(&you->DIR_X, &camera->pan);
    vec_add(&you->DIR_X, &camera->x);
    you->OBJ_TAKE_DAMAGE = damage;
}

// animate saw
void player_saw_animate(PLAYER *hero)
{
    // shooting ?
    if (weapon_fire_key_busy == true && weapon_in_use == true)
    {
        // for shooting, we need to play only last two frames
        hero->weapon[PLAYER_SAW].anim_frame += time_step;
        hero->weapon[PLAYER_SAW].anim_frame %= (hero->weapon[PLAYER_SAW].anim_total_frames / 2);
        hero->weapon[PLAYER_SAW].ent->frame = hero->weapon[PLAYER_SAW].anim_frame + 3;
    }
    else // weapon not busy ?
    {
        // idle ?
        if (hero->weapon[PLAYER_SAW].ent->OBJ_STATE == PLAYER_WPN_IDLE)
        {
            // for idle, we need to play only first two frames !
            hero->weapon[PLAYER_SAW].anim_frame += 0.75 * time_step;
            hero->weapon[PLAYER_SAW].anim_frame %= (hero->weapon[PLAYER_SAW].anim_total_frames / 2);
            hero->weapon[PLAYER_SAW].ent->frame = hero->weapon[PLAYER_SAW].anim_frame + 1;
        }
        else
        {
            // reset animation frames
            hero->weapon[PLAYER_SAW].ent->frame = 0;
        }
    }
}