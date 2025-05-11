#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <raylib.h>
#include "game.h" 

// Core resolution functions
void InitResolutions(Game *game);
void ChangeResolution(Game *game, int newResolutionIndex);
void ToggleFullscreenMode(Game *game);
void HandleResolutionChange(Game *game);

// Get current screen dimensions
int GetCurrentScreenWidth(void);
int GetCurrentScreenHeight(void);

// Helper drawing function
void DrawTextCentered(const char* text, int posY, int fontSize, Color color);

#endif // RESOLUTION_H
