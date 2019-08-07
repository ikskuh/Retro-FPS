
// register player's structure
PLAYER *register_player_struct(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! Can't register player's structure! Entity doesn't exist!");
        return NULL;
    }

    if (ent->OBJ_STRUCT != 0)
    {
        diag("\nERROR! Can't register player's structure twice! It already exists!");
        return get_player_struct(ent);
    }

    PLAYER *hero = sys_malloc(sizeof(PLAYER));
    initialize_player_struct(ent, hero);
    ent->OBJ_STRUCT = hero;
    return hero;
}

// initialize player's structure to default values
var initialize_player_struct(ENTITY *ent, PLAYER *hero)
{
    vec_set(&hero->cam.position, &ent->x);
    vec_set(&camera->x, &ent->x);
    vec_set(&hero->cam.angle, &ent->pan);
    vec_set(&camera->pan, vector(ent->pan, 0, 0));
    ent->pan = 0;
    hero->cam.height = 10;
    hero->cam.fov = 90;
    hero->cam.third_person_dist = -128;
    hero->cam.bob_z_offset = 1.5;
    hero->cam.bob_speed_factor = 2;
}

// get player's structure from the given entity
PLAYER *get_player_struct(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! Can't get player's structure! Entity doesn't exist.");
        return NULL;
    }

    if (ent->OBJ_STRUCT == 0)
    {
        diag("\nERROR! Can't get player's structure! It doesn't exist.");
        return NULL;
    }

    PLAYER *hero = ent->OBJ_STRUCT;
    return hero;
}

// free player structure from memory from given entity
var delete_player_struct(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! Can't free player's structure! Entity doesn't exist");
        return false;
    }

    PLAYER *hero = get_player_struct(ent);

    if (!hero)
    {
        diag("\nERROR! Can't free player's structure! It already doesn't exist.");
        return false;
    }

    delete_player_struct(hero);
    return true;
}

// free given player structure from the memory
void delete_player_struct(PLAYER *hero)
{
    sys_free(hero);
}