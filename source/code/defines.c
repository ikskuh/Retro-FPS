
// delay functions
void delay(var time)
{
    if (time == 0)
    {
        time = 1;
    }
    var counter = abs(time);
    while (counter > 0)
    {
        if (level_is_loaded == false)
        {
            break;
        }
        counter -= time_frame / 16;
        wait(1);
    }
}

void ent_delay(ENTITY *ent, var time)
{
    if (!ent)
    {
        diag("\nERROR! Can't run delay function! Entity doesn't exist.");
        return;
    }
    if (time == 0)
    {
        time = 1;
    }
    var counter = abs(time);
    while (ent)
    {
        if (level_is_loaded == false)
        {
            break;
        }
        counter -= time_frame / 16;
        if (counter <= 0)
        {
            break;
        }
        wait(1);
    }
}

// math functions
var math_round(num)
{
    return (floor(num + 0.5));
}

var math_check_divide(value, divide)
{
    return (integer(divide * math_round(value / divide)));
}

// assert func
void _assert(int v, char const *msg)
{
    if (v)
    {
        return;
    } // assertion met
    error(msg);
    ExitProcess(1); // and fast exit
}

// safe remove entity (at the end of the frame)
void ent_delete_later(ENTITY *ent)
{
    ASSERT(ent != NULL);
    ent->OBJ_FLAGS |= FLAG_DELETE_LATER;
}

// safe remove entity
void ent_delete(ENTITY *ent)
{
    ptr_remove(ent);
    ent = NULL;
}

// alternative for c_move
var ent_move(ENTITY *ent, VECTOR *reldist, VECTOR *absdist, var mode)
{
    if (!ent)
    {
        diag("\nERROR! Can't move entity.. It doesn't exist!");
        return -1;
    }

    my = ent;
    var distance = c_move(ent, reldist, absdist, MOVE_FLAGS | mode);
    my = NULL;
    return distance;
}

// alternative for c_trace
var ent_trace(ENTITY *ent, VECTOR *from, VECTOR *to, var mode)
{
    if (!ent)
    {
        diag("\nERROR! Can't perform trace.. My entity doesn't exist!");
        return -1;
    }
    my = ent;
    var distance = c_trace(from, to, TRACE_FLAGS | mode);
    my = NULL;
    return distance;
}

// alternative for c_scan
var ent_scan(ENTITY *ent, VECTOR *pos, ANGLE *ang, VECTOR *sector, var mode)
{
    if (!ent)
    {
        diag("\nERROR! Can't perform scan.. My entity doesn't exist!");
        return -1;
    }
    my = ent;
    var distance = c_scan(pos, ang, sector, mode);
    my = NULL;
    return distance;
}

// rotate entity towards the camera
void ent_rotate_to_camera(ENTITY *ent)
{
    VECTOR temp_vec;
    vec_set(&temp_vec, &camera->x);
    vec_sub(&temp_vec, &ent->x);
    vec_to_angle(&ent->pan, &temp_vec);
    ent->tilt = 0;
    ent->roll = 0;
}

// place entity on the ground
// this function uses c_trace instead of ent_trace
// since it's called in action functions where my already exists !
void ent_place_on_ground(ENTITY *ent)
{
    if (!ent)
    {
        diag("\nERROR! Can't place entity on the ground! It doesn't exist..");
        return;
    }

    c_trace(&ent->x, vector(ent->x, ent->y, ent->z - 1024), TRACE_FLAGS | IGNORE_MODELS);
    if (HIT_TARGET)
    {
        ent->z = hit->z + ent->max_z;
    }
}

// used by animator for 8 directional sprites
var ang2(var p)
{
    p = ang(p);
    if (p < 0)
    {
        p += 360;
    }
    return p;
}