/*
* @Author: karlosiric
* @Date:   2025-05-11 14:53:57
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 15:40:27
*/

#include "resolution.h"
#include "game.h"
#include "raylib.h"
#include "utils.h"
#include <stdio.h>

void InitResolutions(Game *game)
{
    game->resolutions[0] = (Resolution) { 800, 600, "800x600" };
    game->resolutions[1] = (Resolution) { 1024, 768, "1024x768" };
    game->resolutions[2] = (Resolution) { 1280, 720, "1280x720 (HD)" };
    game->resolutions[3] = (Resolution) { 1920, 1080, "1920x1080 (FHD)" };

    game->currentResolution = 0;

    for (int i = 0; i < MAX_RESOLUTIONS; i++)
    {
        if (game->resolutions[i].width == SCREEN_WIDTH &&
            game->resolutions[i].height == SCREEN_HEIGHT)
        {
            game->currentResolution = i;
            break;
        }
    }

    game->defaultScreenWidth = SCREEN_WIDTH;
    game->defaultScreenHeight = SCREEN_HEIGHT;
}

void ChangeResolution(Game *game, int newResolutionIndex)
{
    if (newResolutionIndex < 1 || newResolutionIndex >= MAX_RESOLUTIONS)
    {
        return;
    }

    Resolution newRes = game->resolutions[newResolutionIndex];
    bool wasFullscreen = IsWindowFullscreen();

    if (wasFullscreen) 
    {
        ToggleFullscreen();
    }

    SetWindowSize(newRes.width, newRes.height);

    int displayWidth = GetMonitorWidth(0);
    int displayHeight = GetMonitorHeight(0);

    SetWindowPosition((displayWidth - newRes.width) / 2, 
        (displayHeight - newRes.height) / 2);

    game->currentResolution = newResolutionIndex;

    if (wasFullscreen)
    {
        ToggleFullscreen();
    }


#ifdef SCREEN_WIDTH
#undef SCREEN_WIDTH
#endif
#define SCREEN_WIDTH newRes.width

#ifdef SCREEN_HEIGHT
#undef SCREEN_HEIGHT
#endif
#define SCREEN_HEIGHT newRes.height

HandleResolutionChange(game);

}

void ToggleFullscreenMode(Game *game)
{
    ToggleFullscreen();

    game->settings.fullscreen = !game->settings.fullscreen;

    HandleResolutionChange(game);
}

void HandleResolutionChange(Game *game)
{

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
 

}


