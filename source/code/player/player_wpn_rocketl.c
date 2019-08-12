
// initialize all rocketlauncher parameters
void player_rocketlauncher_init(PLAYER *hero)
{
    hero->weapon[PLAYER_ROCKETLAUNCHER].collected = false;
    hero->weapon[PLAYER_ROCKETLAUNCHER].semiauto = false;

    hero->weapon[PLAYER_ROCKETLAUNCHER].ammo = 45;
    hero->weapon[PLAYER_ROCKETLAUNCHER].ammo_per_shoot = 1;
    hero->weapon[PLAYER_ROCKETLAUNCHER].fire_rate = 0.8;
    hero->weapon[PLAYER_ROCKETLAUNCHER].recoil_strength = 1.5;
    hero->weapon[PLAYER_ROCKETLAUNCHER].damage = 15;
    hero->weapon[PLAYER_ROCKETLAUNCHER].accuracy = 3;

    hero->weapon[PLAYER_ROCKETLAUNCHER].animate = false;
    hero->weapon[PLAYER_ROCKETLAUNCHER].anim_total_frames = 4;
    hero->weapon[PLAYER_ROCKETLAUNCHER].ent = ent_create(view_rocketlauncher_tga, &camera->x, player_weapon_fnc);
    hero->weapon[PLAYER_ROCKETLAUNCHER].fnc = player_rocketlauncher_shoot;
    hero->weapon[PLAYER_ROCKETLAUNCHER].snd = weapon_rocketlauncher_shoot_ogg;
}

// rocketlauncher shooting function
void player_rocketlauncher_shoot(PLAYER *hero)
{
    // create rockets here
}

// animate rocketlauncher
void player_rocketlauncher_animate(PLAYER *hero)
{
    // if not rocketlauncher, ignore
    if (weapon_id != PLAYER_ROCKETLAUNCHER)
    {
        return;
    }

    if (hero->weapon[PLAYER_ROCKETLAUNCHER].animate == true)
    {
        hero->weapon[PLAYER_ROCKETLAUNCHER].anim_frame += time_step;
        hero->weapon[PLAYER_ROCKETLAUNCHER].ent->frame = hero->weapon[PLAYER_ROCKETLAUNCHER].anim_frame + 1;
        if (hero->weapon[PLAYER_ROCKETLAUNCHER].anim_frame >= hero->weapon[PLAYER_ROCKETLAUNCHER].anim_total_frames)
        {
            hero->weapon[PLAYER_ROCKETLAUNCHER].animate = false;
            hero->weapon[PLAYER_ROCKETLAUNCHER].anim_frame %= hero->weapon[PLAYER_ROCKETLAUNCHER].anim_total_frames;
        }
    }
    else
    {
        // reset animation frame
        hero->weapon[PLAYER_ROCKETLAUNCHER].ent->frame = 0;
    }
}