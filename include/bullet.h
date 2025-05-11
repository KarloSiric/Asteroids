#ifndef BULLET_H
#define BULLET_H 

#include <raylib.h>

// Defining constants

#define MAX_BULLETS      100
#define BULLET_SPEED     10                         // eventually improved the shooting as well -> version 1.0 had 5 pixels speed, bumped to 8
#define BULLET_COOLDOWN  0                          // new constant added for bullet cd between each shot in 15 (frames)



// Bullets structure
typedef struct Bullet {
    Vector2 position;
    Vector2 velocity;
    float radius;
    bool active;
    float lifeTime;
} Bullet;

// Functions prototypes
void InitBullets(Bullet bullets[]);
void UpdateBullets(Bullet bullets[]);
void DrawBullets(Bullet bullets[]);
void ShootBullets(Bullet bullets[], Vector2 position, float rotation);

#endif                 // BULLET_H end config
