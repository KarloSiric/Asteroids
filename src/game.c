/*
* @Author: karlosiric
* @Date:   2025-05-09 09:11:58
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-09 17:19:18
*/

/*
 * Game.c is a c program responsible for generating and drawing the game and all of that so
 */

// Includes
#include <raylib.h>
#include <raymath.h>
#include "asteroids.h"
#include "bullet.h"
#include "player.h"
#include "utils.h"
#include "game.h"


// We are passing the pointer to the game structure so we need to use -> in this case
void initGame(Game *game) 
{
    // dereferencing the pointer and using -> syntax in this case
    game->state = MENU;                     // we initially set this to the MENU part of the game
    game->score = 0;                        // we then set the score to be equal to 0 for its own sake of the game

                                            // We initialize the player now
    InitPlayer(&game->player);

    InitAsteroid(game->asteroids);          // initialize the asteroids

    InitBullets(game->bullets);             // initialize the bullets

    for (int i = 0; i < 5; i++)
    {
        SpawnAsteroids(game->asteroids);
    }
}

void UpdateGame(Game *game)
{
    if((game->state == MENU) && IsKeyPressed(KEY_ENTER)) 
    {
        game->state = GAMEPLAY;
    }
    else if ((game->state == GAME_OVER) && IsKeyPressed(KEY_ENTER))
    {
        // now we reset the game itself
        ResetGame(game);
        game->state = GAMEPLAY;
    }

    if(game->state == GAMEPLAY)
    {
        // then we update the player and the astroids the bullets and all
        /* 
            Here we are passing the pointer to the player because, the function expects a pointer
            If we did game->player we would be passing a value not a memory address and be careful
            Here we need to get the address of the player and pass it to the function itself
            becauase it is expecting a address or a pointer.

            Bullets dont contain that because bullets are an array which are already passed as a pointer
            to the function. SO C treats it as a pointer to the first element in the Array

            So in the end we want to actually modify or Player structure where it is in memory
            and not change the the values and make copies, this way functions directly affect the original
            Player structure that is somewhere going to be stored in the memory and this way we allow the ship
            to move, rotate, fire bullets and many other.
        */
        UpdatePlayer(&game->player, game->bullets); 

        UpdateAsteroid(game->asteroids);
        UpdateBullets(game->bullets);

        // We check the collisions
        checkCollisions(&game->player, game->asteroids, game->bullets, &game->score, &game->state);

    }
}

// Now we need to actually draw the game
void DrawGame(Game *game) 
{
    // we first draw the menu selections
    if (game->state == MENU) 
    {
        // We draw that menu
        DrawText("ASTEROIDS", SCREEN_WIDTH / 2 - MeasureText("ASTEROIDS", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, WHITE);
        DrawText("Press ENTER to Start", SCREEN_WIDTH / 2 - MeasureText("Press ENTER to Start", 20), SCREEN_HEIGHT / 2 + 20, 40, WHITE);
    }
    else if (game->state == GAMEPLAY)
    {
        DrawAsteroids(game->asteroids);
        DrawBullets(game->bullets);
        DrawPlayer(game->player);

        // For drawing the score on the screen
        DrawText(TextFormat("SCORE: %d", game->score), 10, 10, 20, WHITE);
    }
    else if (game->state == GAME_OVER)
    {
        // Draw game over text
        DrawText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, WHITE);
        DrawText(TextFormat("FINAL SCORE: %d", game->score), SCREEN_WIDTH / 2 - MeasureText(TextFormat("FINAL SCORE: %d", game->score), 20) / 2, SCREEN_HEIGHT / 2, 20, WHITE);
        DrawText("Press ENTER to play again", SCREEN_WIDTH / 2 - MeasureText("Press ENTER to play again", 20) / 2, SCREEN_HEIGHT / 2 + 40, 20, WHITE);
    }
}

// Implementing the reset game feature
void ResetGame(Game *game)
{
    // We reset the player
    InitPlayer(&game->player);

    InitAsteroid(game->asteroids);
    InitBullets(game->bullets);

    // now we spawn those initial asteroids once again
    for (int i = 0; i < 5; i++)
    {
        SpawnAsteroids(game->asteroids);
    }
    // reset the score finally
    game->score = 0; 
}
