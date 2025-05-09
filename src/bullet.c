/*
* @Author: karlosiric
* @Date:   2025-05-09 15:03:38
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-09 15:18:08
*/

/* 
 * Game program for the bullet architecture and behavior
 */

#include "bullet.h"
#include "utils.h"
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void InitBullets(Bullet *bullets)
{
    /* this echnique is known as the object pooling where we dont use dynamic memory allocation 
     * but instead, we use only the existing amount of bullets or objects that we need
     * and instead of creating and destroying the existing ones, we just use and iteretae over the 
     * ones that ae already existing, so we use the active flag boolean that allows us to keep track
     * of everything that we will be using, this is better becauase we allow memory leaks this way
     * and this allows us to not have to deal with malloc and so forth.
     */
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].active = false;
    }
}

void UpdateBullets(Bullet *bullets)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        // we take the bullets that are active
        if(bullets[i].active)
        {
            // now we need to move the bullets
            bullets[i].position.x += bullets[i].velocity.x;             // Simple physics so changing the position over time is just applying velocity to it
            bullets[i].position.y += bullets[i].velocity.y;

            // We wrap the around edges as well like asteroids
            WrapPosition(&bullets[i].position);

            // we need to deal with the bullet lifetime
            bullets[i].lifeTime--;
            // in case taht the lifetime of a bullet is less or equal to 0 then we need to remove the bullet
            if (bullets[i].lifeTime <= 0)
            {
                bullets[i].active = false;                              // we make it not active, because we delete, since we only render active "objects"
            }
        }
    }
}

// Now we need to do the drawing part of all of this
void DrawBullets(Bullet *bullets)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            DrawCircle(bullets[i].position.x, bullets[i].position.y, bullets[i].radius, WHITE);
        }
    }
}

// We also need to program the shooting of the bullets

void ShootBullets(Bullet *bullets, Vector2 position, float rotation)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        // if it is not active we need to make it active so that's why
        if(!bullets[i].active)
        {
            bullets[i].position = position;
            bullets[i].velocity.x = cos(rotation * DEG2RAD) * BULLET_SPEED;
            bullets[i].velocity.y = sin(rotation * DEG2RAD) * BULLET_SPEED;
            bullets[i].radius = 3;
            bullets[i].lifeTime = 120;                                     // active life bullet time
            bullets[i].active = true;                                      // we activate the bullet that we have just made
        }
    }
}
