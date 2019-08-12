
// initialize all shotgun parameters
void player_shotgun_init(PLAYER *hero)
{
    hero->weapon[PLAYER_SHOTGUN].collected = false;
    hero->weapon[PLAYER_SHOTGUN].semiauto = true;

    hero->weapon[PLAYER_SHOTGUN].ammo = 45;
    hero->weapon[PLAYER_SHOTGUN].ammo_per_shoot = 1;
    hero->weapon[PLAYER_SHOTGUN].fire_rate = 0.85;
    hero->weapon[PLAYER_SHOTGUN].recoil_strength = 2;
    hero->weapon[PLAYER_SHOTGUN].damage = 20;
    hero->weapon[PLAYER_SHOTGUN].accuracy = 12;
    hero->weapon[PLAYER_SHOTGUN].projectile_speed = 150;

    vec_set(&hero->weapon[PLAYER_SHOTGUN].casing_pos, vector(8, 0, -3));

    hero->weapon[PLAYER_SHOTGUN].animate = false;
    hero->weapon[PLAYER_SHOTGUN].anim_total_frames = 8;
    hero->weapon[PLAYER_SHOTGUN].anim_casing_frame = 6;
    hero->weapon[PLAYER_SHOTGUN].ent = ent_create(view_shotgun_tga, &camera->x, player_weapon_fnc);
    hero->weapon[PLAYER_SHOTGUN].fnc = player_shotgun_shoot;
    hero->weapon[PLAYER_SHOTGUN].snd = weapon_shotgun_shoot_ogg;
}

// shotgun shooting function
void player_shotgun_shoot(PLAYER *hero)
{
    // create bullets here
    var i = 0;
    var spd = hero->weapon[PLAYER_SHOTGUN].projectile_speed;
    var dmg = hero->weapon[PLAYER_SHOTGUN].damage;
    var acc = hero->weapon[PLAYER_SHOTGUN].accuracy;
    for (i = 0; i < 8; i++)
    {
        bullet_create(&camera->x, &camera->pan, PLAYER_GROUP, dmg, spd, acc);
    }
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
        hero->weapon[PLAYER_SHOTGUN].anim_frame += 0.75 * time_step;
        hero->weapon[PLAYER_SHOTGUN].ent->frame = hero->weapon[PLAYER_SHOTGUN].anim_frame + 1;
        if (hero->weapon[PLAYER_SHOTGUN].anim_frame >= hero->weapon[PLAYER_SHOTGUN].anim_total_frames)
        {
            hero->weapon[PLAYER_SHOTGUN].animate = false;
            hero->weapon[PLAYER_SHOTGUN].anim_frame %= hero->weapon[PLAYER_SHOTGUN].anim_total_frames;
        }
    }
    else
    {
        // reset animation frame
        hero->weapon[PLAYER_SHOTGUN].ent->frame = 0;
    }
}