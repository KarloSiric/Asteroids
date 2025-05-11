#ifndef BULLET_H
#define BULLET_H 

#include <raylib.h>

// Defining constants

#define MAX_BULLETS      100
#define BULLET_SPEED     10                         // Bullet speed
#define BULLET_COOLDOWN  8                          // 8 frames cooldown between shots (was 0)
#define BULLET_LIFETIME  120                        // How long bullets live for
#define BULLET_SPREAD    2.0f                       // Slight spread when shooting (in degrees)


// Bullets structure
typedef struct Bullet {
    Vector2 position;
    Vector2 velocity;
    float radius;
    bool active;
    float lifeTime;
    Color color;                                    // Added color for visual variety
    float alpha;                                    // Added alpha for fading effect
} Bullet;

// Functions prototypes
void InitBullets(Bullet bullets[]);
void UpdateBullets(Bullet bullets[]);
void DrawBullets(Bullet bullets[]);
void ShootBullets(Bullet bullets[], Vector2 position, float rotation);

#endif                 // BULLET_H end config
