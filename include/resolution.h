#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <raylib.h>
#include "game.h" 

void InitResolutions(Game *game);
void ChangeResolution(Game *game, int newResolutionIndex);
void ToggleFullscreenMode(Game *game);
void HandleResolutionChange(Game *game);

#endif
