
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
    hero->weapon[PLAYER_SAW].damage = 25;
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
void player_saw_attack(PLAYER *hero)
{
    // trace here
}

// animate saw
void player_saw_animate(PLAYER *hero)
{
    // shooting ?
    if (weapon_fire_key_busy == true)
    {
        // for shooting, we need to play only last two frames
        hero->weapon[PLAYER_SAW].anim_speed += time_step;
        hero->weapon[PLAYER_SAW].anim_speed %= (hero->weapon[PLAYER_SAW].anim_total_frames / 2);
        hero->weapon[PLAYER_SAW].ent->frame = hero->weapon[PLAYER_SAW].anim_speed + 3;
    }
    else // weapon not busy ?
    {
        // idle ?
        if (hero->weapon[PLAYER_SAW].ent->OBJ_STATE == PLAYER_WPN_IDLE)
        {
            // for idle, we need to play only first two frames !
            hero->weapon[PLAYER_SAW].anim_speed += 0.75 * time_step;
            hero->weapon[PLAYER_SAW].anim_speed %= (hero->weapon[PLAYER_SAW].anim_total_frames / 2);
            hero->weapon[PLAYER_SAW].ent->frame = hero->weapon[PLAYER_SAW].anim_speed + 1;
        }
        else
        {
            // reset animation frames
            hero->weapon[PLAYER_SAW].ent->frame = 0;
        }
    }
}