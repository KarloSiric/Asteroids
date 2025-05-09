/* 
 * Adding stars as the background look and all to make the game look more nice
 */

#ifndef STARS_H
#define STARS_H 

#include <raylib.h>

#define MAX_STARS          100                 // number of stars 
#define STAR_LAYERS        3                   // using this to try to make a parallax effect in the game

// Star structure architecture
typedef struct Star {
    Vector2 position;                          // 2D vector for the position of the stars
    float brightness;                          // variable that controls the brightness of the stars 0.0f to 1.0f
    int size;                                  // 1-3 pixels size
    int layer;                                 // can be 0, 1 or 2 for different movement layers and speeds
    Color color;                               // wanted to make sure different stars have different colors
} Star;

// Function protoypes
void InitStars(Star stars[]);
void UpdateStars(Star stars[]);
void DrawStars(Star stars[]);


#endif                                         // ending STARS_H config header
