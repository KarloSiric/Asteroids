/*
* @Author: karlosiric
* @Date:   2025-05-09 15:18:26
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 16:47:04
*/

/*
 * Utilities part of the game that is responsible for checking the collisions
 * between the player bullets and the asteroids, player ship and asteroids and so forth
 */

#include "utils.h"
#include "asteroids.h"
#include "bullet.h"
#include "game.h"
#include "player.h"
#include <raylib.h>
#include <math.h>

// External reference to global screen dimensions defined in main.c
extern int screenWidth;
extern int screenHeight;

bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2)
{
    float dx = center2.x - center1.x;  
    float dy = center2.y - center1.y;
    float distance = sqrtf(dx*dx + dy*dy);

    return distance <= radius1 + radius2;
}

void WrapPosition(Vector2 *position)
{
    if (position->x > screenWidth)
    {
        position->x = 0;
    }
    else if (position->x < 0)
    {
        position->x = screenWidth;
    }
    
    if (position->y > screenHeight)
    {
        position->y = 0;
    }
    else if (position->y < 0)
    {
        position->y = screenHeight;
    }
}

/* Function for checking collisions between bullets, asteroids, player and updating the score nad gameState if needed */
void checkCollisions(Player *player, Asteroid *asteroids, Bullet *bullets, int *score, GameState *gameState)
{
    // let's check the bullet and asteroid collisions
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if(bullets[i].active)
        {
            for (int j = 0; j < MAX_ASTEROIDS; j++)
            {
                if (asteroids[j].active)
                {
                    // now we check here if the collision occured really
                    if (CheckCollisionCircles(bullets[i].position, bullets[i].radius,
                                              asteroids[j].position, asteroids[j].radius))
                    {
                        // if it did occur the asteroid has been hit it seems!
                        bullets[i].active = false;
                        asteroids[j].active = false;
                        *score += 100;

                        if (asteroids[j].radius > 20)
                        {
                            SplitAsteroid(asteroids, j);
                        }

                        break;
                    }
                }
            }
        }
    }

    // now we check the collisions between ship and asteroid
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        // if the asteroid is acctive
        if (asteroids[i].active)
        {
            // if we the collision happened
            if (CheckCollisionCircles(player->position, (float) SHIP_SIZE / 2, 
                                      asteroids[i].position, asteroids[i].radius))
            {
                // Player has been HIT!
                *gameState = GAME_OVER;
                break;
            }
        }
    }
}
