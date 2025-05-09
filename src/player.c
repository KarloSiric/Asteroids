/*
* @Author: karlosiric
* @Date:   2025-05-09 10:18:25
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-09 17:59:19
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


void InitPlayer(Player *player)
{
    // Setting up initially
    player->position = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    player->velocity = (Vector2){ 0, 0 };
    player->rotation = 0;
    player->isThrusting = false;
    player->shootCooldown = 0;
}

// Now we update the player
void UpdatePlayer(Player *player, Bullet bullets[])
{
    // programming the rotational control
    if (IsKeyDown(KEY_LEFT)) 
    {
        player->rotation -= ROTATION_SPEED;
    }
    
    if (IsKeyDown(KEY_RIGHT))
    {
        player->rotation += ROTATION_SPEED;
    }

    player->isThrusting = IsKeyDown(KEY_UP);
    if (player->isThrusting)
    {
        // now we need to calculate the acceleration vector based on the ship's rotation
        float cosA = cos(player->rotation * DEG2RAD);
        float sinA = sin(player->rotation * DEG2RAD);
        player->velocity.x += cosA * SHIP_ACCELERATION;
        player->velocity.y += sinA * SHIP_ACCELERATION;
    } 

    // now we apply these velocities to the position
    player->position.x += player->velocity.x;
    player->position.y += player->velocity.y; 
    // now we can apply the space drag if we can call it that
    player->velocity.x *= SHIP_DRAG;
    player->velocity.y *= SHIP_DRAG;

    // we can wrap this around the position, so this doesnt allow it to go outside of the borders
    WrapPosition(&player->position);

    // Adding the shooting part (version 1.0 did not contain this piece of code)
    if (player->shootCooldown > 0)
    {
        player->shootCooldown--;
    }
    // Programming the shooting part
    if (IsKeyPressed(KEY_SPACE) && player->shootCooldown == 0)
    {
        ShootBullets(bullets, player->position, player->rotation);
        player->shootCooldown = BULLET_COOLDOWN;                     // included in the new version, not prevent in v1.0
    }
}

void DrawPlayer(Player player)
{
    Vector2 v1, v2, v3;
    float cosA = cos(player.rotation * DEG2RAD);
    float sinA = sin(player.rotation * DEG2RAD);
    
    // now we need to form three points that are going to be connected tgether to form the ship
    v1.x = player.position.x + cosA * SHIP_SIZE;
    v1.y = player.position.y + sinA * SHIP_SIZE;

    // now for the 2nd point
    v2.x = player.position.x + cos(player.rotation * DEG2RAD + 2.5f) * SHIP_SIZE * 0.7f;
    v2.y = player.position.y + sin(player.rotation * DEG2RAD + 2.5f) * SHIP_SIZE * 0.7f;

    // now the 3rd point
    v3.x = player.position.x + cos(player.rotation * DEG2RAD - 2.5f) * SHIP_SIZE * 0.7f;
    v3.y = player.position.y + sin(player.rotation * DEG2RAD - 2.5f) * SHIP_SIZE * 0.7f;

    DrawTriangleLines(v1, v2, v3, WHITE);

    // Now we can draw the exhaust or the so called thrust if possible
    if (player.isThrusting)
    {
        Vector2 thrustPos;
        thrustPos.x = player.position.x - cosA * SHIP_SIZE * 0.5f;
        thrustPos.y = player.position.y - sinA * SHIP_SIZE * 0.5f;

        DrawLineEx(thrustPos, (Vector2) { thrustPos.x - cosA * SHIP_SIZE * GetRandomValue(5, 10)/ 10.0f,
                                          thrustPos.y - sinA * SHIP_SIZE * GetRandomValue(5, 10) / 10.0f }, 
                                          3.0f, YELLOW);
    }
}
