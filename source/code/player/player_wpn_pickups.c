
// visual weapons sprite (pickable)
void wpn_visual_func()
{
    set(my, PASSABLE | NOFILTER | SHADOW | CAST);
    vec_fill(&my->scale_x, 0.5); // scaledown to 32x32

#ifndef FREE_VERSION
    my->material = mtl_translucent;
#endif
}

// add ammo to the given weapon
void wpn_add_ammo(PLAYER *hero, var num, var am, var mx)
{
    hero->weapon[num].ammo += am;
    hero->weapon[num].ammo = clamp(hero->weapon[num].ammo, 0, mx);
}

// weapon pickup event function
void wpn_event_function()
{
    if (event_type == EVENT_PUSH)
    {
        // player is that you ?
        if (you->OBJ_TYPE != TYPE_PLAYER)
        {
            return;
        }

        // only once !
        my->emask &= ~ENABLE_PUSH;

        // get player's structure
        PLAYER *hero = get_player_struct(you);

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_weapon_ogg, wpn_pickup_snd_volume, 0);

        // maybe show some text showing that we've got a pistol ?

        // we don't have this weapon ?
        // then collect it
        if (hero->weapon[my->ID].collected == false)
        {
            hero->weapon[my->ID].collected = true;
        }

        // also add some ammo
        switch (my->ID)
        {
        case PLAYER_PISTOL:
            wpn_add_ammo(hero, my->ID, wpn_pistol_add_ammo, wpn_pistol_max_ammo);
            break;

        case PLAYER_SHOTGUN:
            wpn_add_ammo(hero, my->ID, wpn_shotgun_add_ammo, wpn_shotgun_max_ammo);
            break;

        case PLAYER_SSHOTGUN:
            // add ammo to shotgun, so both weapons use same ammo
            wpn_add_ammo(hero, PLAYER_SHOTGUN, wpn_shotgun_add_ammo, wpn_shotgun_max_ammo);
            break;

        case PLAYER_CHAINGUN:
            wpn_add_ammo(hero, my->ID, wpn_chaingun_add_ammo, wpn_chaingun_max_ammo);
            break;

        case PLAYER_ROCKETLAUNCHER:
            wpn_add_ammo(hero, my->ID, wpn_rocketl_add_ammo, wpn_rocketl_max_ammo);
            break;

        case PLAYER_LASERGUN:
            wpn_add_ammo(hero, my->ID, wpn_lasergun_add_ammo, wpn_lasergun_max_ammo);
            break;
        }
        my->OBJ_HEALTH = -1;
    }

    if (event_type == EVENT_FRAME)
    {
        if (my->OBJ_HEALTH <= 0)
        {
            ent_delete_later(my->parent);
            ent_delete_later(my);
        }

        if (my->parent)
        {
            vec_set(&my->parent->x, vector(my->x, my->y, my->z + 8));
        }
    }
}

// init pickable weapon
void wpn_init(var num)
{
    vec_fill(&my->scale_x, 0.5);
    c_setminmax(my);
    ent_place_on_ground(my);
    set(my, POLYGON | TRANSLUCENT | SHADOW | CAST);
    my->alpha = 0;

    my->group = PUSH_GROUP;
    my->push = PUSH_GROUP;

    if (num < 0 || num > MAX_WEAPONS)
    {
        my->ID = PLAYER_PISTOL;
    }
    my->ID = num;
    my->OBJ_HEALTH = 100;
    my->OBJ_TYPE = TYPE_WORLD_PISTOL;

    my->emask |= (ENABLE_PUSH | ENABLE_FRAME);
    my->event = wpn_event_function;

    // depending on our type, create visual sprite
    switch (my->ID)
    {
    case PLAYER_SAW:
        my->parent = ent_create(world_saw_tga, &my->x, wpn_visual_func);
        break;

    case PLAYER_PISTOL:
        my->parent = ent_create(world_pistol_tga, &my->x, wpn_visual_func);
        break;

    case PLAYER_SHOTGUN:
        my->parent = ent_create(world_shotgun_tga, &my->x, wpn_visual_func);
        break;

    case PLAYER_SSHOTGUN:
        my->parent = ent_create(world_supershotgun_tga, &my->x, wpn_visual_func);
        break;

    case PLAYER_CHAINGUN:
        my->parent = ent_create(world_chaingun_tga, &my->x, wpn_visual_func);
        break;

    case PLAYER_ROCKETLAUNCHER:
        my->parent = ent_create(world_rocketlauncher_tga, &my->x, wpn_visual_func);
        break;

    case PLAYER_LASERGUN:
        my->parent = ent_create(world_lasergun_tga, &my->x, wpn_visual_func);
        break;
    }
}

// pickable weapons
// but we already have saw by default !
action wpn_saw()
{
    wpn_init(PLAYER_SAW);
}

action wpn_pistol()
{
    wpn_init(PLAYER_PISTOL);
}

action wpn_shotgun()
{
    wpn_init(PLAYER_SHOTGUN);
}

action wpn_supershotgun()
{
    wpn_init(PLAYER_SSHOTGUN);
}

action wpn_chaingun()
{
    wpn_init(PLAYER_CHAINGUN);
}

action wpn_rocketlauncher()
{
    wpn_init(PLAYER_ROCKETLAUNCHER);
}

action wpn_lasergun()
{
    wpn_init(PLAYER_LASERGUN);
}