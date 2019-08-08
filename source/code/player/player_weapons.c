
// setup basic weapon settings at start
void player_weapons_initialize(ENTITY *ent, PLAYER *hero)
{
}

// main function to update all weapons
void player_weapons_update(ENTITY *ent, PLAYER *hero)
{
    // player is dead ?
    // terminate
    if (ent->OBJ_HEALTH <= 0)
    {
        return;
    }
}