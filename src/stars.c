/*
* @Author: karlosiric
* @Date:   2025-05-09 18:12:58
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 16:41:02
*/

/* 
 * C program to implement the stars as the background look in the game
 * To add a bit of more stuff to the game itself
 */

#include "stars.h"
#include "raylib.h"
#include "utils.h"
#include <math.h>

// External globals for screen dimensions
extern int screenWidth;
extern int screenHeight;

void InitStars(Star *stars)
{
    for (int i = 0; i < MAX_STARS; i++)
    {
        // making random positions of the stars at first
        stars[i].position.x = GetRandomValue(0, screenWidth);
        stars[i].position.y = GetRandomValue(0, screenHeight);
        // adding stars brigthness levels
        stars[i].brightness = GetRandomValue(10, 100) / 100.0f;
        stars[i].size = GetRandomValue(1, 3);
        stars[i].layer = GetRandomValue(0, STAR_LAYERS - 1);

        // now adding slightly varying colors of the stars
        int colorVar = GetRandomValue(-20, 20);
        stars[i].color = (Color) {
            230 + colorVar,
            230 + colorVar,
            240,
            (unsigned char)(stars[i].brightness * 255)
        };
    }
}

// Adding function for Updating the stars themselves
void UpdateStars(Star *stars)
{
    // Adding a twinkle effect or something like that
    for (int i = 0; i < MAX_STARS; i++)
    {
        if (GetRandomValue(0, 100) < 5)
        {
            stars[i].brightness = GetRandomValue(10, 100) / 100.0f;
            stars[i].color.a = (unsigned char)(stars[i].brightness * 255);
        }
    }

    // Optional adding the paralax effect with layers
}

void DrawStars(Star *stars)
{
    for (int i = 0; i < MAX_STARS; i++)
    {
        if (stars[i].size == 1)
        {
            DrawPixelV(stars[i].position, stars[i].color);
        }
        else {
            DrawCircleV(stars[i].position, stars[i].size * 0.5f, stars[i].color);
        }
    }
}
