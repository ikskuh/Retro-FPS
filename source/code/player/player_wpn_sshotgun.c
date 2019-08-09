
// initialize all super shotgun parameters
void player_sshotgun_init(PLAYER *hero)
{
    hero->weapon[PLAYER_SSHOTGUN].collected = true;
    hero->weapon[PLAYER_SSHOTGUN].semiauto = true;

    hero->weapon[PLAYER_SSHOTGUN].ammo = 45;
    hero->weapon[PLAYER_SSHOTGUN].ammo_per_shoot = 2;
    hero->weapon[PLAYER_SSHOTGUN].fire_rate = 1.2;
    hero->weapon[PLAYER_SSHOTGUN].recoil_strength = 3.5;
    hero->weapon[PLAYER_SSHOTGUN].damage = 20;
    hero->weapon[PLAYER_SSHOTGUN].accuracy = 28;

    hero->weapon[PLAYER_SSHOTGUN].animate = false;
    hero->weapon[PLAYER_SSHOTGUN].anim_total_frames = 12;
    hero->weapon[PLAYER_SSHOTGUN].ent = ent_create(view_supershotgun_tga, &camera->x, player_weapon_fnc);
    hero->weapon[PLAYER_SSHOTGUN].fnc = player_sshotgun_shoot;
    hero->weapon[PLAYER_SSHOTGUN].snd = weapon_sshotgun_shoot_ogg;
}

// shotgun super shooting function
void player_sshotgun_shoot(PLAYER *hero)
{
    // create bullets here
}

// animate super shotgun
void player_sshotgun_animate(PLAYER *hero)
{
    // if not pistol, ignore
    if (weapon_id != PLAYER_SSHOTGUN)
    {
        return;
    }

    if (hero->weapon[PLAYER_SSHOTGUN].animate == true)
    {
        hero->weapon[PLAYER_SSHOTGUN].anim_speed += 0.7 * time_step;
        hero->weapon[PLAYER_SSHOTGUN].ent->frame = hero->weapon[PLAYER_SSHOTGUN].anim_speed + 1;
        if (hero->weapon[PLAYER_SSHOTGUN].anim_speed >= hero->weapon[PLAYER_SSHOTGUN].anim_total_frames)
        {
            hero->weapon[PLAYER_SSHOTGUN].animate = false;
            hero->weapon[PLAYER_SSHOTGUN].anim_speed %= hero->weapon[PLAYER_SSHOTGUN].anim_total_frames;
        }
    }
    else
    {
        // reset animation frame
        hero->weapon[PLAYER_SSHOTGUN].ent->frame = 0;
    }
}