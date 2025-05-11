/*
* @Author: karlosiric
* @Date:   2025-05-09 12:46:09
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-09 23:33:37
*/


/* 
 * C program that manages the asteroids in the game, their spawning, movement, shape and etc
 */

#include <raylib.h>
#include "asteroids.h"
#include "utils.h"
#include <math.h>

void InitAsteroid(Asteroid *asteroids)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        asteroids[i].active = false;
    }
}
void UpdateAsteroid(Asteroid *asteroids)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (asteroids[i].active)
        {
            // Then we move the asteroids
            asteroids[i].position.x += asteroids[i].velocity.x;
            asteroids[i].position.y += asteroids[i].velocity.y;

            // Now we can rotate the asteroids
            asteroids[i].rotation += asteroids[i].rotationSpeed;

            // Now we wrap their position
            WrapPosition(&asteroids[i].position);
        }
    }

    // Spawn new asteroids ocassionally
    if (GetRandomValue(0, 100) < 1)
    {
        SpawnAsteroids(asteroids);
    }
}

void DrawAsteroids(Asteroid *asteroids)
{
    // we need to draw some interesting asteroid shape
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (asteroids[i].active)
        {
            // we make it a irregular polygon of 8 sides
            int points = 8;
            Vector2 prev = { 0 };
            Vector2 current = { 0 };

            for (int j = 0; j <= points; j++)
            {
                // we divide the circles into equal segments
                float angle = j * (2.0f * PI / points) + asteroids[i].rotation;
                float radius = asteroids[i].radius * (0.8f + 0.2f * sinf(angle * 5));

                current.x = asteroids[i].position.x + radius * cosf(angle);
                current.y = asteroids[i].position.y + radius * sinf(angle);

                if (j > 0) 
                {
                    DrawLineV(prev, current, WHITE);
                }
                prev = current;
            }
        }
    }
}

void SpawnAsteroids(Asteroid *asteroids)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (!asteroids[i].active)
        {
            // randomly choose from one of the window edges
            float edge = GetRandomValue(0, 3);
            // if the edge is 0 then we get the following;
            if (edge == 0)
            {
                // this will make an asteroid that will spawn from the top
                asteroids[i].position = (Vector2) { GetRandomValue(0, SCREEN_WIDTH), 0 };
            }
            else if (edge == 1) // Right
            {
                asteroids[i].position = (Vector2) { SCREEN_WIDTH, GetRandomValue(0, SCREEN_HEIGHT) };
            }
            else if (edge == 2) // BOTTOM
            {
                asteroids[i].position = (Vector2) { GetRandomValue(0, SCREEN_WIDTH), SCREEN_HEIGHT };
            }
            else // Left
            {
                asteroids[i].position = (Vector2) { 0, GetRandomValue(0, SCREEN_HEIGHT) };
            }

            // random velocity we need to do this first
            float angle = GetRandomValue(0, 360) * DEG2RAD;
            asteroids[i].velocity.x = cos(angle) * ASTEROID_SPEED;
            asteroids[i].velocity.y = sin(angle) * ASTEROID_SPEED;

            // Now we do the size and rotational part, we need to program that as well
            asteroids[i].radius = GetRandomValue(20, 40);
            asteroids[i].rotation = GetRandomValue(0, 360) * DEG2RAD;
            asteroids[i].rotationSpeed = ((float) GetRandomValue(-10, 10) / 100.0f);

            asteroids[i].active = true;
            break;
        }
    }
}

// Now we need to implement the functionality of the SPlitting of the asteroid
void SplitAsteroid(Asteroid *asteroids, int index)
{
    Vector2 position = asteroids[index].position;                                                  // we get the position of the asteroid
    float radius = asteroids[index].radius / 2;                                                    // here we are splitting the radius

    // we need to split only if the radius is big enough
    if (radius >= 10)                                                                              // we only make fragments if that radius is larger than 10 pixels
    {
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < MAX_ASTEROIDS; j++)
            {
                if (!asteroids[j].active)
                {
                    asteroids[j].position = position;                                               // here we are setting the position of the fragmented asteroid to the original position of the asteroid
                    float angle = GetRandomValue(0, 360) * DEG2RAD;                                 // we need to make a new angle for this fragment to move in
                    asteroids[j].velocity.x = cos(angle) * ASTEROID_SPEED * 1.5f;                   // we need to make sure that fragments move faster than big asteroids
                    asteroids[j].velocity.y = sin(angle) * ASTEROID_SPEED * 1.5f;                   // factor of 1.5 is to make sure it moves faster than regular
                    asteroids[j].radius = radius;
                    asteroids[j].rotation = GetRandomValue(0, 360) * DEG2RAD;
                    asteroids[j].rotationSpeed = ((float) GetRandomValue(-15, 15) / 100.0f);        // it is from -15 to 15 because they spin faster
                    asteroids[j].active = true;
                    break;
                }
            }
        }
    }
}
