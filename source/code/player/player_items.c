
// visual sprites function
void item_visual_func()
{
    set(my, PASSABLE | NOFILTER | SHADOW | CAST);
    vec_fill(&my->scale_x, 0.5); // scaledown to 32x32
}

// set animations for the sprite
void item_set_anim(ENTITY *ent, var num)
{
    ent->ANIM_TOTAL_FRAMES = num;
    set(ent, ANIMATED);

#ifndef FREE_VERSION
    ent->material = mtl_anim_sprite;
    ent->skill41 = floatv(num);
    ent->skill42 = floatv(1);
#endif
}

// create visual sprite for the item
void item_create_sprite()
{
    // also add some ammo
    switch (my->ID)
    {
    case ITEM_HEALTH_SMALL:
        my->parent = ent_create(item_health_small_tga, &my->x, item_visual_func);
        item_set_anim(my->parent, 4);
        break;

    case ITEM_HEALTH_BIG:
        my->parent = ent_create(item_health_big_tga, &my->x, item_visual_func);
        my->parent->material = mtl_translucent;
        break;

    case ITEM_ARMOR_SMALL:
        my->parent = ent_create(item_armor_small_tga, &my->x, item_visual_func);
        item_set_anim(my->parent, 4);
        break;

    case ITEM_ARMOR_BIG:
        my->parent = ent_create(item_armor_big_tga, &my->x, item_visual_func);
        item_set_anim(my->parent, 2);
        break;

    case ITEM_AMMO_SMALL:
        my->parent = ent_create(item_ammo_small_tga, &my->x, item_visual_func);
        my->parent->material = mtl_translucent;
        break;

    case ITEM_AMMO_BIG:
        my->parent = ent_create(item_ammo_big_tga, &my->x, item_visual_func);
        my->parent->material = mtl_translucent;
        break;

    case ITEM_PISTOL_AMMO:
        my->parent = ent_create(item_pistol_ammo_tga, &my->x, item_visual_func);
        my->parent->material = mtl_translucent;
        break;

    case ITEM_ROCKET_BIG:
        my->parent = ent_create(item_rocket_big_tga, &my->x, item_visual_func);
        my->parent->material = mtl_translucent;
        break;

    case ITEM_ROCKET_SMALL:
        my->parent = ent_create(item_rocket_small_tga, &my->x, item_visual_func);
        my->parent->material = mtl_translucent;
        break;

    case ITEM_SHELLS_BIG:
        my->parent = ent_create(item_shells_big_tga, &my->x, item_visual_func);
        my->parent->material = mtl_translucent;
        break;

    case ITEM_SHELLS_SMALL:
        my->parent = ent_create(item_shells_small_tga, &my->x, item_visual_func);
        my->parent->material = mtl_translucent;
        break;

    case ITEM_SUIT:
        my->parent = ent_create(item_suit_tga, &my->x, item_visual_func);
        my->parent->material = mtl_translucent;
        break;

    case ITEM_KEY_RED:
        my->parent = ent_create(item_key_red_tga, &my->x, item_visual_func);
        item_set_anim(my->parent, 2);
        break;

    case ITEM_KEY_YELLOW:
        my->parent = ent_create(item_key_yellow_tga, &my->x, item_visual_func);
        item_set_anim(my->parent, 2);
        break;

    case ITEM_KEY_BLUE:
        my->parent = ent_create(item_key_blue_tga, &my->x, item_visual_func);
        item_set_anim(my->parent, 2);
        break;
    }
}

// pickup item
void item_pickup(ENTITY *ent, var num)
{
    switch (my->ID)
    {
    case ITEM_HEALTH_SMALL:
        if (ent->OBJ_HEALTH >= item_health_small_max)
        {
            my->emask |= (ENABLE_PUSH);
            return;
        }
        ent->OBJ_HEALTH += item_health_small_add;
        ent->OBJ_HEALTH = clamp(ent->OBJ_HEALTH, 0, item_health_small_max);
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_small_item_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_HEALTH_BIG:
        if (ent->OBJ_HEALTH >= item_health_big_max)
        {
            my->emask |= (ENABLE_PUSH);
            return;
        }
        ent->OBJ_HEALTH += item_health_big_add;
        ent->OBJ_HEALTH = clamp(ent->OBJ_HEALTH, 0, item_health_big_max);
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_item_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_ARMOR_SMALL:
        if (ent->OBJ_ARMOR >= item_armor_small_max)
        {
            my->emask |= (ENABLE_PUSH);
            return;
        }
        ent->OBJ_ARMOR += item_armor_small_add;
        ent->OBJ_ARMOR = clamp(ent->OBJ_ARMOR, 0, item_armor_small_max);
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_small_item_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_ARMOR_BIG:
        if (ent->OBJ_ARMOR >= item_armor_big_max)
        {
            my->emask |= (ENABLE_PUSH);
            return;
        }
        ent->OBJ_ARMOR += item_armor_big_add;
        ent->OBJ_ARMOR = clamp(ent->OBJ_ARMOR, 0, item_armor_big_max);
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_item_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_AMMO_SMALL:
        // get player's structure
        PLAYER *hero = get_player_struct(you);

        // already full ammo ?
        if (hero->weapon[PLAYER_PISTOL].ammo >= wpn_pistol_max_ammo &&
            hero->weapon[PLAYER_SHOTGUN].ammo >= wpn_shotgun_max_ammo &&
            hero->weapon[PLAYER_CHAINGUN].ammo >= wpn_chaingun_max_ammo &&
            hero->weapon[PLAYER_ROCKETLAUNCHER].ammo >= wpn_rocketl_max_ammo &&
            hero->weapon[PLAYER_LASERGUN].ammo >= wpn_lasergun_max_ammo)
        {
            my->emask |= (ENABLE_PUSH);
            return;
        }

        // add ammo to all weapons
        wpn_add_ammo(hero, PLAYER_PISTOL, item_small_pistol_add, wpn_pistol_max_ammo);
        wpn_add_ammo(hero, PLAYER_SHOTGUN, item_small_shotgun_add, wpn_shotgun_max_ammo);
        wpn_add_ammo(hero, PLAYER_CHAINGUN, item_small_chaingun_add, wpn_chaingun_max_ammo);
        wpn_add_ammo(hero, PLAYER_ROCKETLAUNCHER, item_small_rocketl_add, wpn_rocketl_max_ammo);
        wpn_add_ammo(hero, PLAYER_ROCKETLAUNCHER, item_small_lasergun_add, wpn_lasergun_max_ammo);

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_weapon_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_AMMO_BIG:
        // get player's structure
        PLAYER *hero = get_player_struct(you);

        // already full ammo ?
        if (hero->weapon[PLAYER_PISTOL].ammo >= wpn_pistol_max_ammo &&
            hero->weapon[PLAYER_SHOTGUN].ammo >= wpn_shotgun_max_ammo &&
            hero->weapon[PLAYER_CHAINGUN].ammo >= wpn_chaingun_max_ammo &&
            hero->weapon[PLAYER_ROCKETLAUNCHER].ammo >= wpn_rocketl_max_ammo &&
            hero->weapon[PLAYER_LASERGUN].ammo >= wpn_lasergun_max_ammo)
        {
            my->emask |= (ENABLE_PUSH);
            return;
        }

        // add ammo to all weapons
        wpn_add_ammo(hero, PLAYER_PISTOL, item_big_pistol_add, wpn_pistol_max_ammo);
        wpn_add_ammo(hero, PLAYER_SHOTGUN, item_big_shotgun_add, wpn_shotgun_max_ammo);
        wpn_add_ammo(hero, PLAYER_CHAINGUN, item_big_chaingun_add, wpn_chaingun_max_ammo);
        wpn_add_ammo(hero, PLAYER_ROCKETLAUNCHER, item_big_rocketl_add, wpn_rocketl_max_ammo);
        wpn_add_ammo(hero, PLAYER_LASERGUN, item_big_lasergun_add, wpn_lasergun_max_ammo);

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_weapon_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_PISTOL_AMMO:
        // get player's structure
        PLAYER *hero = get_player_struct(you);
        if (hero->weapon[PLAYER_PISTOL].ammo >= wpn_pistol_max_ammo)
        {
            my->emask |= (ENABLE_PUSH);
            return;
        }

        // add ammo to all weapons
        wpn_add_ammo(hero, PLAYER_PISTOL, item_small_pistol_add, wpn_pistol_max_ammo);

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_weapon_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_ROCKET_BIG:
        // get player's structure
        PLAYER *hero = get_player_struct(you);
        if (hero->weapon[PLAYER_ROCKETLAUNCHER].ammo >= wpn_rocketl_max_ammo)
        {
            my->emask |= (ENABLE_PUSH);
            return;
        }

        // add ammo to all weapons
        wpn_add_ammo(hero, PLAYER_ROCKETLAUNCHER, item_big_rocketl_add, wpn_rocketl_max_ammo);

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_weapon_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_ROCKET_SMALL:
        // get player's structure
        PLAYER *hero = get_player_struct(you);
        if (hero->weapon[PLAYER_ROCKETLAUNCHER].ammo >= wpn_rocketl_max_ammo)
        {
            my->emask |= (ENABLE_PUSH);
            return;
        }

        // add ammo to all weapons
        wpn_add_ammo(hero, PLAYER_ROCKETLAUNCHER, item_small_rocketl_add, wpn_rocketl_max_ammo);

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_weapon_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_SHELLS_BIG:
        // get player's structure
        PLAYER *hero = get_player_struct(you);
        if (hero->weapon[PLAYER_SHOTGUN].ammo >= wpn_shotgun_max_ammo)
        {
            my->emask |= (ENABLE_PUSH);
            return;
        }

        // add ammo to all weapons
        wpn_add_ammo(hero, PLAYER_SHOTGUN, item_big_shotgun_add, wpn_shotgun_max_ammo);

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_weapon_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_SHELLS_SMALL:
        // get player's structure
        PLAYER *hero = get_player_struct(you);
        if (hero->weapon[PLAYER_SHOTGUN].ammo >= wpn_shotgun_max_ammo)
        {
            my->emask |= (ENABLE_PUSH);
            return;
        }

        // add ammo to all weapons
        wpn_add_ammo(hero, PLAYER_SHOTGUN, item_small_shotgun_add, wpn_shotgun_max_ammo);

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_weapon_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_SUIT:
        if (player_has_suit == false)
        {
            player_has_suit = true;
        }
        else
        {
            player_suit_counter = 0;
        }

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_item_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_KEY_RED:
        player_has_red_key = true;

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_key_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_KEY_YELLOW:
        player_has_yellow_key = true;

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_key_ogg, item_pickup_snd_volume, 0);
        break;

    case ITEM_KEY_BLUE:
        player_has_blue_key = true;

        // play pickup snd
        if (snd_playing(player_pick_snd_handle))
        {
            snd_stop(player_pick_snd_handle);
        }
        player_pick_snd_handle = snd_play(pickup_key_ogg, item_pickup_snd_volume, 0);
        break;
    }
    my->OBJ_HEALTH = -1;
}

// main event function for all items
void item_event_function()
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

        // pickup function
        item_pickup(you, my->ID);
    }

    if (event_type == EVENT_FRAME)
    {
        if (my->parent)
        {
            vec_set(&my->parent->x, vector(my->x, my->y, my->z + 8));
            ent_rotate_to_camera(my->parent);

            if (is(my->parent, ANIMATED))
            {
                my->parent->ANIM_FRAMES += 0.25 * time_step;
                my->parent->ANIM_FRAMES %= my->parent->ANIM_TOTAL_FRAMES;
                my->parent->frame = my->parent->ANIM_FRAMES;
                my->parent->skill43 = floatd(floor(my->parent->frame), my->parent->ANIM_TOTAL_FRAMES);
                my->parent->skill44 = floatd(0, 1);
            }
        }

        if (my->OBJ_HEALTH <= 0)
        {
            ent_delete_later(my->parent);
            ent_delete_later(my);
        }
    }
}

// function for all items
void item_init(var num)
{
    vec_fill(&my->scale_x, 0.5);
    c_setminmax(my);
    ent_place_on_ground(my);
    set(my, POLYGON | TRANSLUCENT | SHADOW | CAST);
    my->alpha = 0;

    my->group = PUSH_GROUP;
    my->push = PUSH_GROUP;

    my->ID = num;
    my->OBJ_HEALTH = 100;
    my->OBJ_TYPE = TYPE_PICKABLE_ITEM;

    item_create_sprite();

    my->emask |= (ENABLE_PUSH | ENABLE_FRAME);
    my->event = item_event_function;
}

// items (used in WED)
action item_health_small()
{
    item_init(ITEM_HEALTH_SMALL);
}

action item_health_big()
{
    item_init(ITEM_HEALTH_BIG);
}

action item_armor_small()
{
    item_init(ITEM_ARMOR_SMALL);
}

action item_armor_big()
{
    item_init(ITEM_ARMOR_BIG);
}

action item_ammo_small()
{
    item_init(ITEM_AMMO_SMALL);
}

action item_ammo_big()
{
    item_init(ITEM_AMMO_BIG);
}

action item_pistol_ammo()
{
    item_init(ITEM_PISTOL_AMMO);
}

action item_rockets_big()
{
    item_init(ITEM_ROCKET_BIG);
}

action item_rockets_small()
{
    item_init(ITEM_ROCKET_SMALL);
}

action item_shells_big()
{
    item_init(ITEM_SHELLS_BIG);
}

action item_shells_small()
{
    item_init(ITEM_SHELLS_SMALL);
}

action item_suit()
{
    item_init(ITEM_SUIT);
}

action item_key_red()
{
    item_init(ITEM_KEY_RED);
}

action item_key_yellow()
{
    item_init(ITEM_KEY_YELLOW);
}

action item_key_blue()
{
    item_init(ITEM_KEY_BLUE);
}