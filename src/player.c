/*
* @Author: karlosiric
* @Date:   2025-05-09 10:18:25
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 16:57:23
*/

/*
 * Main C program for controlling and managing the player sprite, its functionalities, how he behaves
 * works, shoots, spawns, moves and so forth. Important because it is used for rendering the player in the main game loop
 */

#include "player.h"
#include "bullet.h"
#include "raylib.h"
#include "utils.h"
#include <math.h>

// External globals for screen dimensions
extern int screenWidth;
extern int screenHeight;

// Control modes
#define CONTROL_KEYBOARD 0
#define CONTROL_MOUSE    1

void InitPlayer(Player *player)
{
    // Setting up initially
    player->position = (Vector2){ screenWidth / 2, screenHeight / 2};
    player->velocity = (Vector2){ 0, 0 };
    player->rotation = 0;
    player->rotationVelocity = 0;          // Add rotation velocity for smooth turning
    player->isThrusting = false;
    player->shootCooldown = 0;
    player->controlMode = CONTROL_KEYBOARD; // Default to keyboard controls
}

// Now we update the player
void UpdatePlayer(Player *player, Bullet bullets[])
{
    // Handle control mode switching
    if (IsKeyPressed(KEY_M)) {
        player->controlMode = (player->controlMode == CONTROL_KEYBOARD) ? 
                               CONTROL_MOUSE : CONTROL_KEYBOARD;
    }
    
    if (player->controlMode == CONTROL_KEYBOARD) {
        UpdatePlayerKeyboard(player, bullets);
    } else {
        UpdatePlayerMouse(player, bullets);
    }
    
    // Apply velocities to position (common for both control modes)
    player->position.x += player->velocity.x;
    player->position.y += player->velocity.y; 
    
    // Apply dampening (space drag)
    player->velocity.x *= SHIP_DRAG;
    player->velocity.y *= SHIP_DRAG;
    
    // Apply rotation velocity and dampening for smooth rotation
    player->rotation += player->rotationVelocity;
    player->rotationVelocity *= 0.9f; // Rotation dampening
    
    // Keep rotation in 0-360 range for clean math
    if (player->rotation > 360) player->rotation -= 360;
    if (player->rotation < 0) player->rotation += 360;
    
    // Wrap position around the screen
    WrapPosition(&player->position);
    
    // Handle shooting cooldown (common for both control modes)
    if (player->shootCooldown > 0) {
        player->shootCooldown--;
    }
}

void UpdatePlayerKeyboard(Player *player, Bullet bullets[])
{
    // Smoother rotation with acceleration
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        // Add rotation acceleration with a cap
        player->rotationVelocity = fmaxf(player->rotationVelocity - 0.3f, -ROTATION_SPEED);
    } 
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        // Add rotation acceleration with a cap
        player->rotationVelocity = fminf(player->rotationVelocity + 0.3f, ROTATION_SPEED);
    }
    else {
        // If no keys are pressed, apply more dampening to stop rotation more quickly
        player->rotationVelocity *= 0.85f;
    }
    
    // Handle thrusting with smoother acceleration
    player->isThrusting = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    if (player->isThrusting) {
        // Calculate the acceleration vector based on the ship's rotation
        float cosA = cos(player->rotation * DEG2RAD);
        float sinA = sin(player->rotation * DEG2RAD);
        
        // Apply acceleration with slightly increasing force for better control
        float thrustFactor = SHIP_ACCELERATION * (1.0f + 0.1f * (fabsf(player->velocity.x) + fabsf(player->velocity.y)) / 10.0f);
        thrustFactor = fminf(thrustFactor, SHIP_ACCELERATION * 1.5f); // Cap the boost
        
        player->velocity.x += cosA * thrustFactor;
        player->velocity.y += sinA * thrustFactor;
        
        // Cap maximum velocity for better control
        float currentSpeed = sqrtf(player->velocity.x * player->velocity.x + player->velocity.y * player->velocity.y);
        if (currentSpeed > 5.0f) {
            player->velocity.x = (player->velocity.x / currentSpeed) * 5.0f;
            player->velocity.y = (player->velocity.y / currentSpeed) * 5.0f;
        }
    }
    
    // Shooting with keyboard
    if ((IsKeyDown(KEY_SPACE) || IsKeyPressed(KEY_SPACE)) && player->shootCooldown == 0) {
        ShootBullets(bullets, player->position, player->rotation);
        player->shootCooldown = BULLET_COOLDOWN;
    }
}

void UpdatePlayerMouse(Player *player, Bullet bullets[])
{
    // Get mouse position
    Vector2 mousePos = GetMousePosition();
    
    // Calculate direction to mouse from player
    Vector2 direction = {
        mousePos.x - player->position.x,
        mousePos.y - player->position.y
    };
    
    // Calculate angle to mouse (in degrees)
    float targetAngle = atan2f(direction.y, direction.x) * RAD2DEG;
    if (targetAngle < 0) targetAngle += 360.0f;
    
    // Smoothly rotate toward mouse pointer
    float angleDiff = targetAngle - player->rotation;
    
    // Handle angle wrapping
    if (angleDiff > 180) angleDiff -= 360;
    if (angleDiff < -180) angleDiff += 360;
    
    // Set rotation velocity based on how far we need to turn
    player->rotationVelocity = angleDiff * 0.1f;
    
    // Cap rotation speed
    if (player->rotationVelocity > ROTATION_SPEED)
        player->rotationVelocity = ROTATION_SPEED;
    if (player->rotationVelocity < -ROTATION_SPEED)
        player->rotationVelocity = -ROTATION_SPEED;
    
    // Right mouse button for thrust
    player->isThrusting = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    if (player->isThrusting) {
        float cosA = cos(player->rotation * DEG2RAD);
        float sinA = sin(player->rotation * DEG2RAD);
        player->velocity.x += cosA * SHIP_ACCELERATION;
        player->velocity.y += sinA * SHIP_ACCELERATION;
        
        // Cap maximum velocity for better control
        float currentSpeed = sqrtf(player->velocity.x * player->velocity.x + player->velocity.y * player->velocity.y);
        if (currentSpeed > 5.0f) {
            player->velocity.x = (player->velocity.x / currentSpeed) * 5.0f;
            player->velocity.y = (player->velocity.y / currentSpeed) * 5.0f;
        }
    }
    
    // Left mouse button for shooting
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && player->shootCooldown == 0) {
        ShootBullets(bullets, player->position, player->rotation);
        player->shootCooldown = BULLET_COOLDOWN;
    }
}

void DrawPlayer(Player player)
{
    Vector2 v1, v2, v3;
    float cosA = cos(player.rotation * DEG2RAD);
    float sinA = sin(player.rotation * DEG2RAD);
    
    // Draw the ship triangle
    v1.x = player.position.x + cosA * SHIP_SIZE;
    v1.y = player.position.y + sinA * SHIP_SIZE;

    v2.x = player.position.x + cos(player.rotation * DEG2RAD + 2.5f) * SHIP_SIZE * 0.7f;
    v2.y = player.position.y + sin(player.rotation * DEG2RAD + 2.5f) * SHIP_SIZE * 0.7f;

    v3.x = player.position.x + cos(player.rotation * DEG2RAD - 2.5f) * SHIP_SIZE * 0.7f;
    v3.y = player.position.y + sin(player.rotation * DEG2RAD - 2.5f) * SHIP_SIZE * 0.7f;

    DrawTriangleLines(v1, v2, v3, WHITE);

    // Draw the thrust flame with animated size for visual feedback
    if (player.isThrusting)
    {
        Vector2 thrustPos;
        thrustPos.x = player.position.x - cosA * SHIP_SIZE * 0.5f;
        thrustPos.y = player.position.y - sinA * SHIP_SIZE * 0.5f;

        // Animated flame length
        float flameLength = SHIP_SIZE * GetRandomValue(5, 15) / 10.0f;
        
        DrawLineEx(thrustPos, 
                  (Vector2) { 
                      thrustPos.x - cosA * flameLength,
                      thrustPos.y - sinA * flameLength
                  }, 
                  3.0f, YELLOW);
                  
        // Add a second, shorter flame line for visual effect
        DrawLineEx(thrustPos, 
                  (Vector2) { 
                      thrustPos.x - cosA * flameLength * 0.7f + sinA * 3.0f,
                      thrustPos.y - sinA * flameLength * 0.7f - cosA * 3.0f
                  }, 
                  2.0f, RED);
    }
    
    // Indicate control mode with a small indicator
    DrawText(player.controlMode == CONTROL_KEYBOARD ? "K" : "M", 
             player.position.x - 5, 
             player.position.y - SHIP_SIZE - 10, 
             10, GRAY);
}
