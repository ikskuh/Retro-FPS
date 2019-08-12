#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

typedef struct ANIMATOR
{
    VECTOR diff_vec;
    VECTOR ang_vec;

    var anim;
    var side_pan;
    var num_directions;
    var frames_per_scene;
} ANIMATOR;

// register and initializes new animator (8/4 directional sprites)
// and save it's pointer into given entity's OBJ_ANIM_STRUCT skill
ANIMATOR *register_animator(ENTITY *ent);

// returns pointer of the animator from given entity's OBJ_ANIM_STRUCT skill
ANIMATOR *get_animator(ENTITY *ent);

// removes animator from the memory
// pointer is taken from given entity's OBJ_ANIM_STRUCT skill
void delete_animator(ENTITY *ent);

// remove given animator from the memory
void delete_animator(ANIMATOR *animator);

#endif