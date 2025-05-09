/*
* @Author: karlosiric
* @Date:   2025-05-08 22:09:52
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-09 17:19:57
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
    SetTargetFPS(100);

                                    // We initialize the Game itself
    Game game;
    initGame(&game);                // we pass the game structure pointer to the initialization of the game

    while(!WindowShouldClose())
    {
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
