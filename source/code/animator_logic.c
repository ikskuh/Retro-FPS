
// cycle animations
void anim_cycle(ENTITY *ent, ANIMATOR *animator, var spd)
{
    animator->side_pan = ent->pan;

    animator->anim += spd * time_step;
    animator->anim %= 100;
    vec_diff(&animator->diff_vec, &camera->x, &ent->x);
    vec_to_angle(&animator->ang_vec, &animator->diff_vec);
    var part_to_animate = integer(((ang2(animator->ang_vec.x - animator->side_pan) + 360 / (animator->num_directions * 2)) % 360) / (360 / animator->num_directions));

    if (ent->parent)
    {
        ent->parent->frame = (animator->anim / 100 + part_to_animate) * animator->frames_per_scene + 1;
    }
}