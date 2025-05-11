/*
* @Author: karlosiric
* @Date:   2025-05-08 22:09:52
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 18:17:53
*/

/* 
 * Main entry point in the Asteroids Raylib Game in C langauge
 */

#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>
#include "utils.h"
#include "game.h"
#include "resolution.h"
#include "sound.h"

// Global screen dimensions
int screenWidth = SCREEN_WIDTH;
int screenHeight = SCREEN_HEIGHT;

// defining necessary things

int main(void)
{
    // Initialize global screen dimensions
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;
   
    // Set a custom trace log level to ignore non-important trace logs
    // Helps avoid spamming the console with "Viewport changed" messages when resizing
    SetTraceLogLevel(LOG_WARNING);
    
    // We initialize the window first
    InitWindow(screenWidth, screenHeight, "Asteroids game in C using Raylib");
    
    // Enable vsync
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);  // Changed from 240 to 60 for better sound timing
    
    // Disable default exit key (escape)
    SetExitKey(0);

    // Initialize the sound system
    SoundManager soundManager;
    InitSoundManager(&soundManager);
    
    // Initialize the Game itself
    Game game;
    game.soundManager = &soundManager;  // Link the sound manager to the game
    initGame(&game);

    while(!WindowShouldClose())
    {
        // We handle the F11 key for fullscreen toggle
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreenMode(&game);
        }
        
        UpdateGame(&game);
        
        // Begin Drawing
        BeginDrawing();
            ClearBackground(BLACK);
            DrawGame(&game);
        // End Drawing
        EndDrawing();
    }
    
    // Unload game sounds before closing
    UnloadGameSounds(&soundManager);
    
    CloseWindow();
    return 0;
}
