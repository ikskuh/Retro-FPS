
// initialize all pistol parameters
void player_pistol_init(PLAYER *hero)
{
    hero->weapon[PLAYER_PISTOL].collected = true;
    hero->weapon[PLAYER_PISTOL].semiauto = true;

    hero->weapon[PLAYER_PISTOL].ammo = 45;
    hero->weapon[PLAYER_PISTOL].ammo_per_shoot = 1;
    hero->weapon[PLAYER_PISTOL].fire_rate = 0.15;
    hero->weapon[PLAYER_PISTOL].recoil_strength = 0.75;
    hero->weapon[PLAYER_PISTOL].damage = 15;
    hero->weapon[PLAYER_PISTOL].accuracy = 3;

    hero->weapon[PLAYER_PISTOL].animate = false;
    hero->weapon[PLAYER_PISTOL].anim_total_frames = 4;
    hero->weapon[PLAYER_PISTOL].ent = ent_create(view_pistol_tga, &camera->x, player_weapon_fnc);
    hero->weapon[PLAYER_PISTOL].fnc = player_pistol_shoot;
    hero->weapon[PLAYER_PISTOL].snd = weapon_pistol_shoot_ogg;
}

// pistol shooting function
void player_pistol_shoot(PLAYER *hero)
{
    // create bullets here
}

// animate pistol
void player_pistol_animate(PLAYER *hero)
{
    // if not pistol, ignore
    if (weapon_id != PLAYER_PISTOL)
    {
        return;
    }

    if (hero->weapon[PLAYER_PISTOL].animate == true)
    {
        hero->weapon[PLAYER_PISTOL].anim_speed += time_step;
        hero->weapon[PLAYER_PISTOL].ent->frame = hero->weapon[PLAYER_PISTOL].anim_speed + 1;
        if (hero->weapon[PLAYER_PISTOL].anim_speed >= hero->weapon[PLAYER_PISTOL].anim_total_frames)
        {
            hero->weapon[PLAYER_PISTOL].animate = false;
            hero->weapon[PLAYER_PISTOL].anim_speed %= hero->weapon[PLAYER_PISTOL].anim_total_frames;
        }
    }
    else
    {
        // reset animation frame
        hero->weapon[PLAYER_PISTOL].ent->frame = 0;
    }
}