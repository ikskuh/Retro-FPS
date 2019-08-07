
// handle all player's states
void player_handle_state_machine(ENTITY *ent, CCT *cct)
{
    if (!ent)
    {
        diag("\nERROR! No state machine for player, he doesn't exist!");
        return;
    }

    if (!cct)
    {
        diag("\nERROR! No state machine for player, his cct doesn't exist!");
        return;
    }

    // if we are alive ?
    if (ent->OBJ_HEALTH > 0)
    {
        // we are in water ?
        if (cct->water_state >= IN_WATER)
        {
            // not inside of the water region ?
            if (cct->water_origin_diff_z < 25)
            {
                ent->OBJ_STATE = SWIM;
            }
            else
            {
                ent->OBJ_STATE = DIVE;
            }
        }
        else
        {
            // not in pain or shoot state ?
            if (ent->OBJ_STATE != PAIN && ent->OBJ_STATE != SHOOT)
            {
                // player jumped ?
                if (cct->is_grounded == false)
                {
                    // if in air for way too long, then switch to IN AIR state
                    if (cct->falling_timer > 0.5)
                    {
                        ent->OBJ_STATE = IN_AIR;
                    }
                    else
                    {
                        // jumped ?
                        if (cct->jump_allowed == false)
                        {
                            ent->OBJ_STATE = JUMPED;
                        }
                    }
                }
                else
                {
                    // on ground
                    if (cct->is_grounded == true)
                    {
                        // landed ?
                        if (cct->land_timer > 0.5)
                        {
                            ent->OBJ_STATE = LANDED;
                        }
                        else
                        {
                            // if we are moving
                            if (cct->is_moving == true)
                            {
                                // handle WALK and RUN states
                                // according to "always_run"
                                if (cct->always_run == true)
                                {
                                    if (cct->run == true)
                                    {
                                        ent->OBJ_STATE = WALK;
                                    }
                                    else
                                    {
                                        ent->OBJ_STATE = RUN;
                                    }
                                }
                                else
                                {
                                    if (cct->run == true)
                                    {
                                        ent->OBJ_STATE = RUN;
                                    }
                                    else
                                    {
                                        ent->OBJ_STATE = WALK;
                                    }
                                }
                            }
                            else
                            {
                                ent->OBJ_STATE = IDLE;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        ent->OBJ_STATE = DEATH;
    }
}