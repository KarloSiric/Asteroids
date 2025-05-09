#ifndef PLAYER_H
#define PLAYER_H 

#include <raylib.h>
#include "bullet.h"

// defining CONSTANTS
#define SHIP_SIZE              20
#define SHIP_ACCELERATION      0.1f
#define ROTATION_SPEED         3.0f

// Player ship structure
typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool isThrusting;

} Player;

// Function prototypes
void InitPlayer(Player *player);
void UpdatePlayer(Player *player, Bullet bullets[]);
void DrawPlayer(Player player);

#endif                        // PLAYER_H end config
