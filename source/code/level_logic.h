#ifndef _LEVEL_LOGIC_H_
#define _LEVEL_LOGIC_H_

// This header includes everything related to level setup and loading

typedef struct LEVEL
{
    char name[128];
    char music[128];
    COLOR fog_color;

    ANGLE sun_angle;
    var sun_lightness;

    var clip_near;
    var clip_far;

    var fog_start;
    var fog_end;
} LEVEL;

// list of levels
LEVEL map[MAX_LEVELS];

// create new level object
void level_setup(var num, STRING *str, STRING *snd, COLOR *fog, ANGLE *sun, var lightness, var near, var far, var start, var end);

// initialize all levels
void init_levels();

// set level settings (such as fog, clipping etc)
void set_level_settings();

// reset some global level variables
void level_reset();

// restart current level
void level_restart();

// load level with the given id
void level_load(var num);

#endif