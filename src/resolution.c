/*
* @Author: karlosiric
* @Date:   2025-05-11 14:53:57
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 16:46:04
*/

#include "resolution.h"
#include "game.h"
#include "raylib.h"
#include "utils.h"
#include <stdio.h>

// External globals for screen dimensions that will be updated
extern int screenWidth;
extern int screenHeight;

void InitResolutions(Game *game)
{
    game->resolutions[0] = (Resolution) { 800, 600, "800x600" };
    game->resolutions[1] = (Resolution) { 1024, 768, "1024x768" };
    game->resolutions[2] = (Resolution) { 1280, 720, "1280x720 (HD)" };
    game->resolutions[3] = (Resolution) { 1920, 1080, "1920x1080 (FHD)" };

    game->currentResolution = 2; // Default to 1280x720

    // Store the default screen dimensions
    game->defaultScreenWidth = GetScreenWidth();
    game->defaultScreenHeight = GetScreenHeight();
}

void ChangeResolution(Game *game, int newResolutionIndex)
{
    if (newResolutionIndex < 0 || newResolutionIndex >= MAX_RESOLUTIONS) {
        return; // Invalid resolution index
    }

    // If we're currently in fullscreen, exit fullscreen first
    if (IsWindowFullscreen()) {
        ToggleFullscreen();
        game->settings.fullscreen = false;
    }

    // Update resolution details
    Resolution newRes = game->resolutions[newResolutionIndex];
    
    // Change window size
    SetWindowSize(newRes.width, newRes.height);

    // Center the window on the monitor
    int displayWidth = GetMonitorWidth(GetCurrentMonitor());
    int displayHeight = GetMonitorHeight(GetCurrentMonitor());
    SetWindowPosition(
        (displayWidth - newRes.width) / 2, 
        (displayHeight - newRes.height) / 2
    );

    // Update screen dimension globals
    screenWidth = newRes.width;
    screenHeight = newRes.height;

    // Update current resolution index
    game->currentResolution = newResolutionIndex;
    
    // If the game was in fullscreen, toggle it back 
    if (game->settings.fullscreen) {
        ToggleFullscreenMode(game);
    }

    // Handle any additional adjustments needed
    HandleResolutionChange(game);
}

void ToggleFullscreenMode(Game *game)
{
    int monitor = GetCurrentMonitor();
    
    if (!IsWindowFullscreen()) {
        // Save current window dimensions before going fullscreen
        game->defaultScreenWidth = screenWidth;
        game->defaultScreenHeight = screenHeight;
        
        // Get monitor dimensions for proper fullscreen
        int monitorWidth = GetMonitorWidth(monitor);
        int monitorHeight = GetMonitorHeight(monitor);
        
        // Set to the monitor's resolution before toggling fullscreen
        // This is critical to ensure the game fills the entire screen
        SetWindowSize(monitorWidth, monitorHeight);
        
        // Toggle fullscreen
        ToggleFullscreen();
        
        // Update screen dimensions to monitor size
        screenWidth = monitorWidth;
        screenHeight = monitorHeight;
        
        // Update fullscreen flag
        game->settings.fullscreen = true;
    } else {
        // Exit fullscreen first
        ToggleFullscreen();
        
        // Get the current resolution from the selected resolution index
        Resolution currentRes = game->resolutions[game->currentResolution];
        
        // Restore window size to the selected resolution
        SetWindowSize(currentRes.width, currentRes.height);
        
        // Update screen dimensions
        screenWidth = currentRes.width;
        screenHeight = currentRes.height;
        
        // Center window
        int displayWidth = GetMonitorWidth(monitor);
        int displayHeight = GetMonitorHeight(monitor);
        SetWindowPosition(
            (displayWidth - currentRes.width) / 2, 
            (displayHeight - currentRes.height) / 2
        );
        
        // Update fullscreen flag
        game->settings.fullscreen = false;
    }
    
    // Apply necessary adjustments for the new screen size
    HandleResolutionChange(game);
}

void HandleResolutionChange(Game *game)
{
    // Adjust game elements based on new resolution if needed
    
    // Reinitialize stars to fill the new screen dimensions
    InitStars(game->stars);
    
    // Reset player to center of new screen
    game->player.position.x = screenWidth / 2;
    game->player.position.y = screenHeight / 2;
}
