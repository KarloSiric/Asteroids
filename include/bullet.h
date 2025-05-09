#ifndef BULLET_H
#define BULLET_H 

#include <raylib.h>

// Defining constants

#define MAX_BULLETS      30
#define BULLET_SPEED     5

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
