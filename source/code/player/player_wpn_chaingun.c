
// initialize all chaingun parameters
void player_chaingun_init(PLAYER *hero)
{
    hero->weapon[PLAYER_CHAINGUN].collected = false;
    hero->weapon[PLAYER_CHAINGUN].semiauto = false;

    hero->weapon[PLAYER_CHAINGUN].ammo = 145;
    hero->weapon[PLAYER_CHAINGUN].ammo_per_shoot = 1;
    hero->weapon[PLAYER_CHAINGUN].fire_rate = 0.11;
    hero->weapon[PLAYER_CHAINGUN].recoil_strength = 0.45;
    hero->weapon[PLAYER_CHAINGUN].damage = 5;
    hero->weapon[PLAYER_CHAINGUN].accuracy = 5;
    hero->weapon[PLAYER_CHAINGUN].projectile_speed = 250;

    vec_set(&hero->weapon[PLAYER_CHAINGUN].casing_pos, vector(7, -3, -2));

    hero->weapon[PLAYER_CHAINGUN].animate = false;
    hero->weapon[PLAYER_CHAINGUN].anim_total_frames = 4;
    hero->weapon[PLAYER_CHAINGUN].anim_casing_frame = 2;
    hero->weapon[PLAYER_CHAINGUN].ent = ent_create(view_chaingun_tga, &camera->x, player_weapon_fnc);
    hero->weapon[PLAYER_CHAINGUN].fnc = player_chaingun_shoot;
    hero->weapon[PLAYER_CHAINGUN].snd = weapon_chaingun_shoot_ogg;
}

// chaingun shooting function
void player_chaingun_shoot(PLAYER *hero)
{
    // create bullets here
    var spd = hero->weapon[PLAYER_CHAINGUN].projectile_speed;
    var dmg = hero->weapon[PLAYER_CHAINGUN].damage;
    var acc = hero->weapon[PLAYER_CHAINGUN].accuracy;
    bullet_create(&camera->x, &camera->pan, PLAYER_GROUP, dmg, spd, acc);
}

// animate chaingun
void player_chaingun_animate(PLAYER *hero)
{
    // if not chaingun, ignore
    if (weapon_id != PLAYER_CHAINGUN)
    {
        return;
    }

    if (hero->weapon[PLAYER_CHAINGUN].animate == true)
    {
        hero->weapon[PLAYER_CHAINGUN].anim_frame += 1.5 * time_step;
        hero->weapon[PLAYER_CHAINGUN].ent->frame = hero->weapon[PLAYER_CHAINGUN].anim_frame + 1;
        if (hero->weapon[PLAYER_CHAINGUN].anim_frame >= hero->weapon[PLAYER_CHAINGUN].anim_total_frames)
        {
            hero->weapon[PLAYER_CHAINGUN].animate = false;
            hero->weapon[PLAYER_CHAINGUN].anim_frame %= hero->weapon[PLAYER_CHAINGUN].anim_total_frames;
        }
    }
    else
    {
        // reset animation frame
        hero->weapon[PLAYER_CHAINGUN].ent->frame = 0;
    }
}