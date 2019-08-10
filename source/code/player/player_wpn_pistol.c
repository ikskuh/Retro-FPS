
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

    vec_set(&hero->weapon[PLAYER_PISTOL].casing_pos, vector(8, -1, -1));

    hero->weapon[PLAYER_PISTOL].animate = false;
    hero->weapon[PLAYER_PISTOL].anim_total_frames = 4;
    hero->weapon[PLAYER_PISTOL].anim_casing_frame = 2;
    hero->weapon[PLAYER_PISTOL].ent = ent_create(view_pistol_tga, &camera->x, player_weapon_fnc);
    hero->weapon[PLAYER_PISTOL].fnc = player_pistol_shoot;
    hero->weapon[PLAYER_PISTOL].snd = weapon_pistol_shoot_ogg;
}

// pistol shooting function
void player_pistol_shoot(PLAYER *hero)
{
    // create bullets here
    bullet_create(&camera->x, &camera->pan, PLAYER_GROUP, 500, hero->weapon[PLAYER_PISTOL].accuracy);
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
        hero->weapon[PLAYER_PISTOL].anim_frame += time_step;
        hero->weapon[PLAYER_PISTOL].ent->frame = hero->weapon[PLAYER_PISTOL].anim_frame + 1;
        if (hero->weapon[PLAYER_PISTOL].anim_frame >= hero->weapon[PLAYER_PISTOL].anim_total_frames)
        {
            hero->weapon[PLAYER_PISTOL].animate = false;
            hero->weapon[PLAYER_PISTOL].anim_frame %= hero->weapon[PLAYER_PISTOL].anim_total_frames;
        }
    }
    else
    {
        // reset animation frame
        hero->weapon[PLAYER_PISTOL].ent->frame = 0;
    }
}