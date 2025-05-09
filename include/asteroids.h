#ifndef ASTEROIDS_H
#define ASTEROIDS_H 

#include <raylib.h>

// Defining constants
#define MAX_ASTEROIDS       20
#define ASTEROID_SPEED      2

// Asteroids structure code
typedef struct Asteroid {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float rotationSpeed;
    float radius;
    bool active;
} Asteroid;

// Function prototypes

void InitAsteroid(Asteroid asteroids[]);
void UpdateAsteroid(Asteroid asteroids[]);
void DrawAsteroids(Asteroid asteroids[]);
void SpawnAsteroids(Asteroid asteroids[]);
void SplitAsteroid(Asteroid asteroids[], int index);

#endif
