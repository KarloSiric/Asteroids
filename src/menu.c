/*
* @Author: karlosiric
* @Date:   2025-05-09 19:03:42
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 18:17:47
*/

/*
 * Creating a menu.c helper for navigating through the menu and everything
 * For the proper game feel of having options of having everything basically
 * For allowing the user to configure everything how he wants to.
 */

#include "menu.h"
#include "game.h"
#include "utils.h"
#include "resolution.h"
#include "sound.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h> // Added for NULL

// External globals for screen dimensions
extern int screenWidth;
extern int screenHeight;

void DrawMenuTitle(const char *title)
{
    DrawText(title, screenWidth / 2 - MeasureText(title, 40) / 2, screenHeight / 6, 40, WHITE);
}

void DrawMenuOption(const char *text, int y, bool selected)
{
    // basic option selection, yellow for something being selected and font size 25
    // otherwise using default
    Color color = selected ? YELLOW : WHITE;
    int fontSize = selected ? 25: 20;

    if (selected)
    {   
        DrawText(">", screenWidth / 2 - MeasureText(text, fontSize) / 2 - 30,
                y, fontSize, YELLOW);
    }

    DrawText(text, screenWidth / 2 - MeasureText(text, fontSize) / 2, y, fontSize, color);

}

void DrawMainMenu(Game *game)
{
    // Game Logo Adding
    DrawMenuTitle("ASTEROIDS");

    // Menu Options
    int startY = screenHeight / 2 - 40;
    int spacing = 50;                    // for adding space in the menu
    DrawMenuOption("START GAME", startY, game->selectedOption == MENU_START);
    DrawMenuOption("OPTIONS", startY + spacing, game->selectedOption == MENU_OPTIONS);
    DrawMenuOption("CONTROLS", startY + spacing * 2, game->selectedOption == MENU_CONTROLS);
    DrawMenuOption("EXIT", startY + spacing * 3, game->selectedOption == MENU_EXIT);

    // Footer - FIXED positioning
    DrawText("© 2025 Karlo Siric", 
             screenWidth / 2 - MeasureText("© 2025 Karlo Siric", 15) / 2, 
             screenHeight - 30, 
             15, GRAY);

    // Showing high score if it exists - FIXED positioning
    if (game->highScore > 0)
    {
        DrawText(TextFormat("HIGH SCORE: %d", game->highScore), 
                 screenWidth / 2 - MeasureText(TextFormat("HIGH SCORE: %d", game->highScore), 20) / 2, 
                 screenHeight - 60, 
                 20, YELLOW);
    }
}

void UpdateMainMenu(Game *game)
{
    // Navigation bar
    bool menuChanged = false;
    
    if (IsKeyPressed(KEY_DOWN))
    {
        game->selectedOption = (game->selectedOption + 1) % MENU_MAIN_COUNT;
        menuChanged = true;
    }
    else if (IsKeyPressed(KEY_UP))
    {
        // FIX: Proper handling of wrap-around when going up in menu
        game->selectedOption = (game->selectedOption - 1 + MENU_MAIN_COUNT) % MENU_MAIN_COUNT;
        menuChanged = true;
    }

    // Play sound on menu navigation
    if (menuChanged && game->soundManager != NULL && game->settings.soundEnabled) {
        PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
    }

    // Selected
    if (IsKeyPressed(KEY_ENTER))
    {
        // Play selection sound
        if (game->soundManager != NULL && game->settings.soundEnabled) {
            PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
        }
        
        switch(game->selectedOption)
        {
            case MENU_START:
                game->state = GAMEPLAY;
                break;
            case MENU_OPTIONS:
                game->state = OPTIONS_MENU;            
                game->selectedOption = 0;               // need to reset the select for this to start from 0
                break;
            case MENU_CONTROLS:
                game->state = CONTROLS_MENU;
                game->selectedOption = 0;                // need to reset the select option
                break;
            case MENU_EXIT:
                // The proper Cleanup happens in the mai part of the main loop WindowShouldClose()
                CloseWindow();
                break;
        }
    }
}

void DrawOptionsMenu(Game *game)
{
    DrawMenuTitle("OPTIONS");

    int startY = screenHeight / 2 - 60;
    int spacing = 50;

    char soundText[20];
    char musicText[20];
    char fpsText[20];
    char difficultyText[30];                                                        // Increased size to be safe
    char fullscreenText[30];                                                        // added a buffer to hold FULLSCREEN NEW!!
    char resolutionText[40];                                                        // Buffer for resolution text

    sprintf(soundText, "SOUND: %s", game->settings.soundEnabled ? "ON" : "OFF");
    sprintf(musicText, "MUSIC: %s", game->settings.musicEnabled ? "ON" : "OFF");
    sprintf(fpsText, "SHOW FPS: %s", game->settings.showFPS ? "ON" : "OFF");
    sprintf(fullscreenText, "FULLSCREEN: %s", game->settings.fullscreen ? "ON" : "OFF");
    sprintf(resolutionText, "RESOLUTION: %s", game->resolutions[game->currentResolution].name);

    // setting difficulty switch case
    switch(game->settings.difficulty)
    {
        case 0:
            sprintf(difficultyText, "DIFFICULTY: EASY");
            break;
        case 1:
            sprintf(difficultyText, "DIFFICULTY: NORMAL");
            break;
        case 2:
            sprintf(difficultyText, "DIFFICULTY: HARD");
            break;
    }

    DrawMenuOption(soundText, startY, game->selectedOption == MENU_SOUND);
    DrawMenuOption(musicText, startY + spacing, game->selectedOption == MENU_MUSIC);
    DrawMenuOption(fpsText, startY + spacing * 2, game->selectedOption == MENU_FPS);
    DrawMenuOption(difficultyText, startY +  spacing * 3, game->selectedOption == MENU_DIFFICULTY);
    DrawMenuOption(resolutionText, startY + spacing * 4, game->selectedOption == MENU_RESOLUTION);
    DrawMenuOption(fullscreenText, startY + spacing * 5, game->selectedOption == MENU_FULLSCREEN); 
    DrawMenuOption("BACK", startY + spacing * 6, game->selectedOption == MENU_BACK);

    // Instructions in the menu
    DrawText("<- -> to change settings", screenWidth / 2 - MeasureText("<- -> to change settings", 15) / 2, 
             screenHeight - 30, 15, GRAY);
}

void UpdateOptionsMenu(Game *game)
{
    // Navigation
    bool menuChanged = false;
    
    if (IsKeyPressed(KEY_DOWN))
    {
        game->selectedOption = (game->selectedOption + 1) % MENU_OPTIONS_COUNT;
        menuChanged = true;
    }
    else if (IsKeyPressed(KEY_UP))
    {
        game->selectedOption = (game->selectedOption - 1 + MENU_OPTIONS_COUNT) % MENU_OPTIONS_COUNT;
        menuChanged = true;
    }

    // Play sound on menu navigation
    if (menuChanged && game->soundManager != NULL && game->settings.soundEnabled) {
        PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
    }

    // Change settings with left/right keybinds
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT))
    {
        // Play selection sound
        if (game->soundManager != NULL && game->settings.soundEnabled) {
            PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
        }
        
        // toggle or cycle values
        switch(game->selectedOption)
        {
            case MENU_SOUND:
                game->settings.soundEnabled = !game->settings.soundEnabled;
                // Update sound manager
                if (game->soundManager != NULL) {
                    ToggleSoundEnabled(game->soundManager, game->settings.soundEnabled);
                }
                break;
            case MENU_MUSIC:
                game->settings.musicEnabled = !game->settings.musicEnabled;
                // Update music manager
                if (game->soundManager != NULL) {
                    ToggleMusicEnabled(game->soundManager, game->settings.musicEnabled);
                }
                break;
            case MENU_FPS:
                game->settings.showFPS = !game->settings.showFPS;
                break;
            case MENU_RESOLUTION:
                if (IsKeyPressed(KEY_RIGHT))
                {
                    // Cycle to next resolution
                    int newRes = (game->currentResolution + 1) % MAX_RESOLUTIONS;
                    ChangeResolution(game, newRes);
                }
                else
                {
                    // Cycle to previous resolution
                    int newRes = (game->currentResolution - 1 + MAX_RESOLUTIONS) % MAX_RESOLUTIONS;
                    ChangeResolution(game, newRes);
                }
                break;
            case MENU_FULLSCREEN:
                ToggleFullscreenMode(game);
                break;
            case MENU_DIFFICULTY:
                if (IsKeyPressed(KEY_RIGHT))
                {
                    game->settings.difficulty = (game->settings.difficulty + 1) % 3;
                }
                else {
                    game->settings.difficulty = (game->settings.difficulty - 1 + 3) % 3;
                }
                break;
        }
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        // Play selection sound
        if (game->soundManager != NULL && game->settings.soundEnabled) {
            PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
        }
        
        if (game->selectedOption == MENU_BACK)
        {
            game->state = MAIN_MENU;
            game->selectedOption = MENU_OPTIONS;
        }
    }

    // Going back using ESC keys
    if (IsKeyPressed(KEY_ESCAPE))
    {
        // Play selection sound
        if (game->soundManager != NULL && game->settings.soundEnabled) {
            PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
        }
        
        game->state = MAIN_MENU;
        game->selectedOption = MENU_OPTIONS;
    }
}

void DrawControlsMenu(Game *game)
{
    (void)game;  // Silence the unused parameter warning
    
    DrawMenuTitle("CONTROLS");

    int startY = screenHeight / 2 - 150; // Start higher to fit more controls
    int spacing = 30; // Reduced spacing to fit more items
    int currentY = startY;

    // Keyboard controls section
    DrawText("KEYBOARD CONTROLS:", 
            screenWidth / 2 - MeasureText("KEYBOARD CONTROLS:", 22) / 2,
            currentY, 22, YELLOW);
    currentY += spacing + 10;

    DrawText("UP / W - Thrust",
            screenWidth / 2 - MeasureText("UP / W - Thrust", 20) / 2,
            currentY, 20, WHITE);
    currentY += spacing;

    DrawText("LEFT / A - Rotate Left", 
            screenWidth / 2 - MeasureText("LEFT / A - Rotate Left", 20) / 2,
            currentY, 20, WHITE);
    currentY += spacing;

    DrawText("RIGHT / D - Rotate Right", 
            screenWidth / 2 - MeasureText("RIGHT / D - Rotate Right", 20) / 2,
            currentY, 20, WHITE);
    currentY += spacing;

    DrawText("SPACE - Fire", 
            screenWidth / 2 - MeasureText("SPACE - Fire", 20) / 2, 
            currentY, 20, WHITE);
    currentY += spacing + 20;

    // Mouse controls section
    DrawText("MOUSE CONTROLS:", 
            screenWidth / 2 - MeasureText("MOUSE CONTROLS:", 22) / 2,
            currentY, 22, YELLOW);
    currentY += spacing + 10;

    DrawText("MOUSE POSITION - Aim Ship", 
            screenWidth / 2 - MeasureText("MOUSE POSITION - Aim Ship", 20) / 2,
            currentY, 20, WHITE);
    currentY += spacing;

    DrawText("LEFT CLICK - Fire", 
            screenWidth / 2 - MeasureText("LEFT CLICK - Fire", 20) / 2,
            currentY, 20, WHITE);
    currentY += spacing;

    DrawText("RIGHT CLICK - Thrust", 
            screenWidth / 2 - MeasureText("RIGHT CLICK - Thrust", 20) / 2,
            currentY, 20, WHITE);
    currentY += spacing;

    DrawText("M - Switch Control Mode", 
            screenWidth / 2 - MeasureText("M - Switch Control Mode", 20) / 2,
            currentY, 20, WHITE);
    currentY += spacing + 20;

    // General controls
    DrawText("P - Pause Game", 
            screenWidth / 2 - MeasureText("P - Pause Game", 20) / 2, 
            currentY, 20, WHITE);
    currentY += spacing;

    DrawText("ESC - Return to Menu", 
            screenWidth / 2 - MeasureText("ESC - Return to Menu", 20) / 2, 
            currentY, 20, WHITE);
    currentY += spacing + 10;

    // back button
    DrawMenuOption("BACK", currentY, true);

    // Instructions for the menu
    DrawText("Press ENTER or ESC to return",
        screenWidth / 2 - MeasureText("Press ENTER or ESC to return", 15) / 2,
        screenHeight - 30,
        15, GRAY);
}

void UpdateControlsMenu(Game *game)
{
    // Only need to handle back action
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
    {
        // Play selection sound
        if (game->soundManager != NULL && game->settings.soundEnabled) {
            PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
        }
        
        game->state = MAIN_MENU;
        game->selectedOption = MENU_CONTROLS;           // usually is always selected at main menu controls
    }
}

void DrawPauseMenu(Game *game)
{
    // Semi-transparent overlay
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color) { 0, 0, 0, 150 });

    // Draw the title
    const char *titleText = "PAUSED";
    int titleFontSize = 40;
    int titleWidth = MeasureText(titleText, titleFontSize);
    int titleX = screenWidth/2 - titleWidth/2;
    int titleY = screenHeight/6;
    DrawText(titleText, titleX, titleY, titleFontSize, WHITE);

    // Menu options
    int startY = screenHeight/2 - 40;
    int spacing = 50;
    
    // Draw RESUME option
    const char *resumeText = "RESUME";
    int optionFontSize = game->selectedOption == 0 ? 25 : 20;
    Color optionColor = game->selectedOption == 0 ? YELLOW : WHITE;
    int resumeWidth = MeasureText(resumeText, optionFontSize);
    int resumeX = screenWidth/2 - resumeWidth/2;
    
    // Draw selector if this option is selected
    if (game->selectedOption == 0) {
        DrawText(">", resumeX - 30, startY, optionFontSize, YELLOW);
    }
    
    DrawText(resumeText, resumeX, startY, optionFontSize, optionColor);
    
    // Draw RETURN TO MENU option
    const char *returnText = "RETURN TO MENU";
    optionFontSize = game->selectedOption == 1 ? 25 : 20;
    optionColor = game->selectedOption == 1 ? YELLOW : WHITE;
    int returnWidth = MeasureText(returnText, optionFontSize);
    int returnX = screenWidth/2 - returnWidth/2;
    
    // Draw selector if this option is selected
    if (game->selectedOption == 1) {
        DrawText(">", returnX - 30, startY + spacing, optionFontSize, YELLOW);
    }
    
    DrawText(returnText, returnX, startY + spacing, optionFontSize, optionColor);
    
    // Draw score text
    const char *scoreText = TextFormat("SCORE: %d", game->score);
    int scoreFontSize = 20;
    int scoreWidth = MeasureText(scoreText, scoreFontSize);
    int scoreX = screenWidth/2 - scoreWidth/2;
    int scoreY = screenHeight - 60;
    
    DrawText(scoreText, scoreX, scoreY, scoreFontSize, YELLOW);
}

void UpdatePauseMenu(Game *game)
{
    // We only need two options
    bool menuChanged = false;
    
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_UP))
    {
        game->selectedOption = !game->selectedOption;               // toggle between 0 and 1
        menuChanged = true;
    }

    // Play sound on menu navigation
    if (menuChanged && game->soundManager != NULL && game->settings.soundEnabled) {
        PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        // Play selection sound
        if (game->soundManager != NULL && game->settings.soundEnabled) {
            PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
        }
        
        if (game->selectedOption == 0)
        {
            // Resume
            game->state = GAMEPLAY;
        }
        else 
        {
            game->state = MAIN_MENU;
            game->selectedOption = 0;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))
    {
        // Play selection sound
        if (game->soundManager != NULL && game->settings.soundEnabled) {
            PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
        }
        
        game->state = GAMEPLAY;
    }
}
