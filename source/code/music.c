
// start level's theme, if any available
void music_start()
{
    music_stop();

    if (str_len(map[level_id].music) > 0)
    {
        music_handle = media_loop(map[level_id].music, NULL, music_volume);
    }
}

// stop level's theme music
void music_stop()
{
    if (media_playing(music_handle))
    {
        media_stop(music_handle);
    }
}

// continue playing level's theme music
void music_continue()
{
    media_start(music_handle);
}

// pause level's theme music
void music_pause()
{
    if (media_playing(music_handle))
    {
        media_pause(music_handle);
    }
}