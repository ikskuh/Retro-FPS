
// check if there is anothing between two positions, if not, take damage !
// using c_trace here, cause it's gonna be called within event functions
void explo_check_walls(ENTITY *ent, VECTOR *from, VECTOR *to, var dmg)
{
    // save input
    VECTOR you_pos;
    VECTOR my_pos;
    vec_set(&you_pos, from);
    vec_set(&my_pos, to);
    var temp_damage = dmg;

    wait(1); // because in called in event function !

    // if we are close enough ?
    if (vec_dist(&you_pos, &my_pos) > explo_default_range)
    {
        return;
    }

    c_ignore(PUSH_GROUP, WATER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, PLAYER_GROUP, ENEMY_GROUP, SHOOTABLE_GROUP, 0);
    if (c_trace(&from->x, &to->x, TRACE_FLAGS))
    {
        return;
    }

    // calculate damage
    // 128 quants (by default), because it's the closes range
    var damage = (1 - (vec_dist(from, to) / explo_default_range)) * temp_damage;
    ent->OBJ_HEALTH -= damage + 20; // to make sure, that you will die, if very close

    // if your health is too low, then you need to spawn gibs
    if (ent->OBJ_HEALTH <= -20)
    {
        ent->OBJ_DEATH_TYPE = TYPE_SMASHED;
    }

    // push away
    VECTOR push_vec;
    vec_diff(&push_vec, to, from);
    vec_normalize(&push_vec, damage * 0.25);

    // if player ?
    if (ent->OBJ_TYPE == TYPE_PLAYER)
    {
        CCT *cct = get_cct(ent);
        vec_set(&cct->push_force, &push_vec);
    }

    // if another barrel ?
    if (ent->OBJ_TYPE == TYPE_BARREL)
    {
        vec_set(&ent->DIR_X, &push_vec);
    }

    return;
}

// explosion scan for entities around
// we use c_scan here, because it's used in while with wait
// so there is gonna be valid ME pointer !
void explo_scan(ENTITY *ent)
{
    c_scan(&ent->x, &ent->pan, vector(360, 0, ent->OBJ_EXPLO_RANGE), IGNORE_ME | IGNORE_PASSABLE | SCAN_ENTS | SCAN_LIMIT);
}