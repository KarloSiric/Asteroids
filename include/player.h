#ifndef PLAYER_H
#define PLAYER_H 

#include <raylib.h>
#include "bullet.h"

// defining CONSTANTS
#define SHIP_SIZE              20                 
#define SHIP_ACCELERATION      0.15f              // Changed the ship acceleration (v1.0 -> 0.1f value) to 0.15f
#define ROTATION_SPEED         4.0f               // Changed the rot speed (v1.0 -> 3,0f) to 4.0f
#define SHIP_DRAG              0.96f              // Decided to add the necessary drag changes as well 
                                                  // version 1.0 has 0.98f drag value, been updated now
                                                  // This allowed the ship to be more responsive

// Player ship structure
typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool isThrusting;
    int shootCooldown;                             // included the new variable the shootCooldown (version 1.0 didn't have this)

} Player;

// Function prototypes
void InitPlayer(Player *player);
void UpdatePlayer(Player *player, Bullet bullets[]);
void DrawPlayer(Player player);

#endif                        // PLAYER_H end config
