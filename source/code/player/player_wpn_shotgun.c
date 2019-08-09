
// initialize all shotgun parameters
void player_shotgun_init(PLAYER *hero)
{
    hero->weapon[PLAYER_SHOTGUN].collected = true;
    hero->weapon[PLAYER_SHOTGUN].semiauto = true;

    hero->weapon[PLAYER_SHOTGUN].ammo = 45;
    hero->weapon[PLAYER_SHOTGUN].ammo_per_shoot = 1;
    hero->weapon[PLAYER_SHOTGUN].fire_rate = 0.85;
    hero->weapon[PLAYER_SHOTGUN].recoil_strength = 2;
    hero->weapon[PLAYER_SHOTGUN].damage = 10;
    hero->weapon[PLAYER_SHOTGUN].accuracy = 24;

    hero->weapon[PLAYER_SHOTGUN].animate = false;
    hero->weapon[PLAYER_SHOTGUN].anim_total_frames = 8;
    hero->weapon[PLAYER_SHOTGUN].ent = ent_create(view_shotgun_tga, &camera->x, player_weapon_fnc);
    hero->weapon[PLAYER_SHOTGUN].fnc = player_shotgun_shoot;
    hero->weapon[PLAYER_SHOTGUN].snd = weapon_shotgun_shoot_ogg;
}

// shotgun shooting function
void player_shotgun_shoot(PLAYER *hero)
{
    // create bullets here
}

// animate shotgun
void player_shotgun_animate(PLAYER *hero)
{
    // if not pistol, ignore
    if (weapon_id != PLAYER_SHOTGUN)
    {
        return;
    }

    if (hero->weapon[PLAYER_SHOTGUN].animate == true)
    {
        hero->weapon[PLAYER_SHOTGUN].anim_speed += 0.75 * time_step;
        hero->weapon[PLAYER_SHOTGUN].ent->frame = hero->weapon[PLAYER_SHOTGUN].anim_speed + 1;
        if (hero->weapon[PLAYER_SHOTGUN].anim_speed >= hero->weapon[PLAYER_SHOTGUN].anim_total_frames)
        {
            hero->weapon[PLAYER_SHOTGUN].animate = false;
            hero->weapon[PLAYER_SHOTGUN].anim_speed %= hero->weapon[PLAYER_SHOTGUN].anim_total_frames;
        }
    }
    else
    {
        // reset animation frame
        hero->weapon[PLAYER_SHOTGUN].ent->frame = 0;
    }
}