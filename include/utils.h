#ifndef UTILS_H
#define UTILS_H 

#include <raylib.h>
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "asteroids.h"

// Defining constants
#define SCREEN_WIDTH      1280
#define SCREEN_HEIGHT     920

// Global screen dimensions that can be modified at runtime
extern int screenWidth;
extern int screenHeight;

// Function Prototypes
bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2);
void checkCollisions(Player *player, Asteroid asteroids[], Bullet bullets[], int *score, GameState *gameState);
void WrapPosition(Vector2 *position);


#endif             // UTILS_H end config
