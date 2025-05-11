/*
* @Author: karlosiric
* @Date:   2025-05-09 15:03:38
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 16:57:48
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

// External globals for screen dimensions
extern int screenWidth;
extern int screenHeight;

void InitBullets(Bullet *bullets)
{
    /* this technique is known as the object pooling where we don't use dynamic memory allocation 
     * but instead, we use only the existing amount of bullets or objects that we need
     * and instead of creating and destroying the existing ones, we just use and iterate over the 
     * ones that are already existing, so we use the active flag boolean that allows us to keep track
     * of everything that we will be using
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
        // Update active bullets
        if(bullets[i].active)
        {
            // Move the bullets
            bullets[i].position.x += bullets[i].velocity.x;
            bullets[i].position.y += bullets[i].velocity.y;

            // We don't wrap bullets around edges anymore - they disappear offscreen
            
            // If bullet goes off-screen, deactivate it
            if (bullets[i].position.x < 0 || 
                bullets[i].position.x > screenWidth ||
                bullets[i].position.y < 0 || 
                bullets[i].position.y > screenHeight)
            {
                bullets[i].active = false;
                continue;
            }

            // Update lifetime
            bullets[i].lifeTime--;
            
            // Fade bullets as they get older
            if (bullets[i].lifeTime < 40) {
                bullets[i].alpha = bullets[i].lifeTime / 40.0f;
            }
            
            // Deactivate expired bullets
            if (bullets[i].lifeTime <= 0)
            {
                bullets[i].active = false;
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
            // Create a color with adjusted alpha for fading effect
            Color bulletColor = bullets[i].color;
            bulletColor.a = (unsigned char)(bullets[i].alpha * 255.0f);
            
            // Draw the bullet
            DrawCircle(bullets[i].position.x, bullets[i].position.y, bullets[i].radius, bulletColor);
            
            // Draw a smaller inner circle for a more interesting visual
            Color innerColor = WHITE;
            innerColor.a = (unsigned char)(bullets[i].alpha * 255.0f);
            DrawCircle(bullets[i].position.x, bullets[i].position.y, bullets[i].radius * 0.5f, innerColor);
        }
    }
}

// We also need to program the shooting of the bullets
void ShootBullets(Bullet *bullets, Vector2 position, float rotation)
{
    // We'll shoot 3 bullets with a slight spread for a more interesting effect
    for (int spread = -1; spread <= 1; spread++)
    {
        // Find an inactive bullet to use
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if(!bullets[i].active)
            {
                // Get the actual rotation with spread
                float bulletRotation = rotation + spread * BULLET_SPREAD;
                
                // Calculate velocity based on spread-adjusted rotation
                float cosA = cos(bulletRotation * DEG2RAD);
                float sinA = sin(bulletRotation * DEG2RAD);
                
                bullets[i].position = position;
                bullets[i].velocity.x = cosA * BULLET_SPEED;
                bullets[i].velocity.y = sinA * BULLET_SPEED;
                bullets[i].radius = 3 + (float)abs(spread) * 0.5f; // Slightly different sizes
                bullets[i].lifeTime = BULLET_LIFETIME - abs(spread) * 10; // Center bullet lasts longer
                bullets[i].active = true;
                bullets[i].alpha = 1.0f;
                
                // Set different colors for visual interest
                if (spread == 0) {
                    bullets[i].color = (Color){ 255, 255, 255, 255 }; // White for center
                } else if (spread == -1) {
                    bullets[i].color = (Color){ 0, 200, 255, 255 };   // Blue-ish
                } else {
                    bullets[i].color = (Color){ 255, 200, 0, 255 };   // Yellow-ish
                }
                
                break; // We found an inactive bullet to use, so break the inner loop
            }
        }
    }
}
