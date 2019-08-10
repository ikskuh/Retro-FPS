
#include <acknex.h>
#include <default.c>
#include <windows.h>
#include <strio.c>

#define PRAGMA_POINTER

// if you use at least commercial version of Acknex
// then comment this in order to use shaders
//#define FREE_VERSION

#define PRAGMA_PATH "code"
#define PRAGMA_PATH "code\\cct"
#define PRAGMA_PATH "code\\fx"
#define PRAGMA_PATH "code\\player"
#define PRAGMA_PATH "code\\props"
#define PRAGMA_PATH "resources"
#define PRAGMA_PATH "resources\\models"
#define PRAGMA_PATH "resources\\sounds"
#define PRAGMA_PATH "resources\\sounds\\explo"
#define PRAGMA_PATH "resources\\sounds\\player"
#define PRAGMA_PATH "resources\\sounds\\props"
#define PRAGMA_PATH "resources\\sounds\\items"
#define PRAGMA_PATH "resources\\sounds\\weapons"
#define PRAGMA_PATH "resources\\sounds\\weapons\\casing"
#define PRAGMA_PATH "resources\\sprites"
#define PRAGMA_PATH "resources\\sprites\\fx"
#define PRAGMA_PATH "resources\\sprites\\items"
#define PRAGMA_PATH "resources\\sprites\\weapons"
#define PRAGMA_PATH "resources\\sprites\\weapons\\casing"

#include "includes.h"

// lock mouse inside of the game window
void mouse_lock()
{
    static var autolock_mouse_locked = false;
    if (!autolock_mouse_locked && window_focus)
    {
        autolock_mouse_locked = true;
        RECT rect;
        GetClientRect(hWnd, &rect);
        ClientToScreen(hWnd, &rect);
        ClientToScreen(hWnd, &rect.right);
        ClipCursor(&rect);
    }
    if (autolock_mouse_locked && !window_focus)
    {
        autolock_mouse_locked = false;
        ClipCursor(NULL);
    }
}

void set_video_settings()
{
    video_set(800, 600, 16, 2);
    video_aspect = 1.333;
}

void set_engine_settings()
{
    set_video_settings();
    reset(camera, SHOW);

    warn_level = 6;
    doppler_factor = 0;
    mouse_pointer = 0;

    max_entities = 10000;
    preload_mode = 3 + 4;

    d3d_near_models = 0.5;
    d3d_near_sprites = 0.5;
    d3d_near_decals = 0.99;

    fps_max = 60;
    fps_min = 30;
    time_smooth = 0.9;

    random_seed(0);
}

// main game loop
void on_frame_event()
{
    if (game_is_running == false)
    {
        return;
    }

    game_ticks += time_frame;
    mouse_lock();

    if (key_e)
    {
        fps_max = 30;
    }
    else
    {
        fps_max = 60;
    }

    if (game_state == GAME_LOADING)
    {
        game_state = GAME_RUNNING;
    }

    if (game_state == GAME_RUNNING)
    {
        if (level_is_loaded == true)
        {
            ENTITY *ent;

            // update all props
            FOR_ENT_OF_TYPE(ent, TYPE_SECRET_WALL)
            {
                secret_wall_update(ent);
            }

            FOR_ENT_OF_TYPE(ent, TYPE_ELEVATOR)
            {
                elevator_update(ent);
            }

            FOR_ENT_OF_TYPE(ent, TYPE_DOOR)
            {
                door_update(ent);
            }

            FOR_ENT_OF_TYPE(ent, TYPE_PLATFORM)
            {
                platform_update(ent);
            }

            // player ?
            FOR_ENT_OF_TYPE(ent, TYPE_PLAYER)
            {
                player_update(ent);
            }

            // bullets ?
            FOR_ENT_OF_TYPE(ent, TYPE_BULLET)
            {
                bullet_update(ent);
            }

            // casings/shells ?
            FOR_ENT_OF_TYPE(ent, TYPE_CASING)
            {
                casing_update(ent);
            }

            // cleanup all entities that should be deleted
            {
                ENTITY *ent = ent_next(NULL);
                while (ent)
                {
                    you = ent;
                    ent = ent_next(ent);
                    if (you->OBJ_FLAGS & FLAG_DELETE_LATER)
                    {
                        ent_delete(you);
                    }
                }
            }
        }
    }

    // cleanup all entities that should be deleted
    {
        ENTITY *ent = ent_next(NULL);
        while (ent)
        {
            you = ent;
            ent = ent_next(ent);
            if (is(you, FLAG1))
            {
                ptr_remove(you);
            }
        }
    }
}

void on_ent_remove_event(ENTITY *ent)
{
    if (ent->OBJ_TYPE == TYPE_PLAYER)
    {
        delete_cct(ent);
        delete_player_struct(ent);
    }

    if (ent->OBJ_TYPE == TYPE_ELEVATOR || ent->OBJ_TYPE == TYPE_DOOR || ent->OBJ_TYPE == TYPE_PLATFORM || ent->OBJ_TYPE == TYPE_SECRET_WALL)
    {
        delete_props(ent);
    }

    if (!ent->parent)
    {
        ptr_remove(ent->parent);
        ent->parent = NULL;
    }
}

// triggered periodically during a level_load call
// in time intervals of about 20 ms
void on_level_event()
{
}

// triggered on engine close
void on_close_event()
{
    music_stop();
    game_is_running = false;
}

void main()
{
    game_is_running = true;

    on_frame = on_frame_event;
    on_close = on_close_event;
    on_level = on_level_event;
    on_ent_remove = on_ent_remove_event;
    on_f4 = pipeline_update;
    on_f5 = pipeline_resolution;
    on_caps = player_toggle_run;
    on_r = level_restart;

    init_levels();
    set_engine_settings();

#ifndef FREE_VERSION
    pipeline_start();
#endif

    level_load(level_id);
}