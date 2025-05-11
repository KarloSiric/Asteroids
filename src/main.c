/*
* @Author: karlosiric
* @Date:   2025-05-08 22:09:52
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 14:34:19
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


// defining necessary things

int main(void)
{
   // We initialize the window first
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids game in C using Raylib");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(240);
    SetExitKey(0);                                                               // disabling the exit key by default

                                                                                 // We initialize the Game itself
    Game game;
    initGame(&game);                                                             // we pass the game structure pointer to the initialization of the game

    while(!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
            game.settings.fullscreen = !game.settings.fullscreen;                 // so here we are setting the fullscreen NEW
        }
        
        UpdateGame(&game);
        
        // Begin Drawing
        BeginDrawing();
            ClearBackground(BLACK);
            DrawGame(&game);
        // End Drawing
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
