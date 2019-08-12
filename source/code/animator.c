
// register and initializes new animator (8/4 directional sprites)
// and save it's pointer into given entity's OBJ_ANIM_STRUCT skill
ANIMATOR *register_animator(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! Can't register animator! Entity doesn't exist!");
        return NULL;
    }

    if (ent->OBJ_STRUCT != 0)
    {
        diag("\nERROR! ANIMATOR already exists for given entity!");
        return get_animator(ent);
    }

    ANIMATOR *animator = sys_malloc(sizeof(ANIMATOR));
    ent->OBJ_ANIM_STRUCT = animator;
    return animator;
}

// returns pointer of the animator from given entity's OBJ_ANIM_STRUCT skill
ANIMATOR *get_animator(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! Can't get animator! Entity doesn't exist.");
        return NULL;
    }

    if (ent->OBJ_ANIM_STRUCT == 0)
    {
        diag("\nERROR! Can't get animator! It doesn't exist.");
        return NULL;
    }

    ANIMATOR *animator = ent->OBJ_ANIM_STRUCT;
    return animator;
}

// removes animator from the memory
// pointer is taken from given entity's OBJ_ANIM_STRUCT skill
void delete_animator(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! Can't free animator! Entity doesn't exist");
        return false;
    }

    ANIMATOR *animator = get_animator(ent);

    if (!animator)
    {
        diag("\nERROR! Can't free animator! It already doesn't exist.");
        return false;
    }

    delete_animator(animator);
    return true;
}

// remove given animator from the memory
void delete_animator(ANIMATOR *animator)
{
    sys_free(animator);
}