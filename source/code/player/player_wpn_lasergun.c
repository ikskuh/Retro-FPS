
// initialize all lasergun parameters
void player_lasergun_init(PLAYER *hero)
{
    hero->weapon[PLAYER_LASERGUN].collected = true;
    hero->weapon[PLAYER_LASERGUN].semiauto = true;

    hero->weapon[PLAYER_LASERGUN].ammo = 45;
    hero->weapon[PLAYER_LASERGUN].ammo_per_shoot = 1;
    hero->weapon[PLAYER_LASERGUN].fire_rate = 0.65;
    hero->weapon[PLAYER_LASERGUN].recoil_strength = 0.25;
    hero->weapon[PLAYER_LASERGUN].damage = 99;
    hero->weapon[PLAYER_LASERGUN].accuracy = 1;

    hero->weapon[PLAYER_LASERGUN].animate = false;
    hero->weapon[PLAYER_LASERGUN].anim_total_frames = 4;
    hero->weapon[PLAYER_LASERGUN].ent = ent_create(view_lasergun_tga, &camera->x, player_weapon_fnc);
    hero->weapon[PLAYER_LASERGUN].fnc = player_lasergun_shoot;
    hero->weapon[PLAYER_LASERGUN].snd = weapon_lasergun_shoot_ogg;
}

// lasergun shooting function
void player_lasergun_shoot(PLAYER *hero)
{
    // shoot lasers here
}

// animate lasergun
void player_lasergun_animate(PLAYER *hero)
{
    // if not lasergun, ignore
    if (weapon_id != PLAYER_LASERGUN)
    {
        return;
    }

    if (hero->weapon[PLAYER_LASERGUN].animate == true)
    {
        hero->weapon[PLAYER_LASERGUN].anim_speed += time_step;
        hero->weapon[PLAYER_LASERGUN].ent->frame = hero->weapon[PLAYER_LASERGUN].anim_speed + 1;
        if (hero->weapon[PLAYER_LASERGUN].anim_speed >= hero->weapon[PLAYER_LASERGUN].anim_total_frames)
        {
            hero->weapon[PLAYER_LASERGUN].animate = false;
            hero->weapon[PLAYER_LASERGUN].anim_speed %= hero->weapon[PLAYER_LASERGUN].anim_total_frames;
        }
    }
    else
    {
        // reset animation frame
        hero->weapon[PLAYER_LASERGUN].ent->frame = 0;
    }
}