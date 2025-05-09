/*
* @Author: karlosiric
* @Date:   2025-05-09 19:03:42
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-09 21:30:36
*/

/*
 * Creating a menu.c helper for navigating through the menu and everything
 * For the proper game feel of having options of having everything basically
 * For allowing the user to configure everything how he wants to.
 */

#include "menu.h"
#include "game.h"
#include "utils.h"
#include <raylib.h>
#include <stdio.h>

void DrawMenuTitle(const char *title)
{
    DrawText(title, SCREEN_WIDTH / 2 - MeasureText(title, 40) / 2, SCREEN_HEIGHT / 6, 40, WHITE);
}

void DrawMenuOption(const char *text, int y, bool selected)
{
    // basic option selection, yellow for something being selected and font size 25
    // otherwise using default
    Color color = selected ? YELLOW : WHITE;
    int fontSize = selected ? 25: 20;

    if (selected)
    {   
        DrawText(">", SCREEN_WIDTH / 2 - MeasureText(text, fontSize) / 2 - 30,
                y, fontSize, YELLOW);
    }

    DrawText(text, SCREEN_WIDTH / 2 - MeasureText(text, fontSize) / 2, y, fontSize, color);

}

void DrawMainMenu(Game *game)
{
    // Game Logo Adding
    DrawMenuTitle("ASTEROIDS");

    // Menu Options
    int startY = SCREEN_HEIGHT / 2 - 40;
    int spacing = 50;                    // for adding space in the menu
    DrawMenuOption("START GAME", startY, game->selectedOption == MENU_START);
    DrawMenuOption("OPTIONS", startY + spacing, game->selectedOption == MENU_OPTIONS);
    DrawMenuOption("CONTROLS", startY + spacing * 2, game->selectedOption == MENU_CONTROLS);
    DrawMenuOption("EXIT", startY + spacing * 3, game->selectedOption == MENU_EXIT);

    // Footer - FIXED positioning
    DrawText("© 2025 Karlo Siric", 
             SCREEN_WIDTH / 2 - MeasureText("© 2025 Karlo Siric", 15) / 2, 
             SCREEN_HEIGHT - 30, 
             15, GRAY);

    // Showing high score if it exists - FIXED positioning
    if (game->highScore > 0)
    {
        DrawText(TextFormat("HIGH SCORE: %d", game->highScore), 
                 SCREEN_WIDTH / 2 - MeasureText(TextFormat("HIGH SCORE: %d", game->highScore), 20) / 2, 
                 SCREEN_HEIGHT - 60, 
                 20, YELLOW);
    }
}

void UpdateMainMenu(Game *game)
{
    // Navigation bar
    if (IsKeyPressed(KEY_DOWN))
    {
        game->selectedOption = (game->selectedOption + 1) % MENU_MAIN_COUNT;
        // play sound here
    }
    else if (IsKeyPressed(KEY_UP))
    {
        // FIX: Proper handling of wrap-around when going up in menu
        game->selectedOption = (game->selectedOption - 1 + MENU_MAIN_COUNT) % MENU_MAIN_COUNT;
    }


    // Selected
    if (IsKeyPressed(KEY_ENTER))
    {
        switch(game->selectedOption)
        {
            case MENU_START:
                game->state = GAMEPLAY;
                // play the sounds needed
                break;
            case MENU_OPTIONS:
                game->state = OPTIONS_MENU;            
                game->selectedOption = 0;               // need to reset the select for this to start from 0
                // play menu sound
                break;
            case MENU_CONTROLS:
                game->state = CONTROLS_MENU;
                game->selectedOption = 0;                // need to reset the select option
                // play the sound menu
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

    int startY = SCREEN_HEIGHT / 2 - 60;
    int spacing = 50;

    char soundText[20];
    char musicText[20];
    char fpsText[20];
    char difficultyText[30]; // Increased size to be safe

    sprintf(soundText, "SOUND: %s", game->settings.soundEnabled ? "ON" : "OFF");
    sprintf(musicText, "MUSIC: %s", game->settings.musicEnabled ? "ON" : "OFF");
    sprintf(fpsText, "SHOW FPS: %s", game->settings.showFPS ? "ON" : "OFF");

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
    DrawMenuOption("BACK", startY + spacing * 4, game->selectedOption == MENU_BACK);

    // Instructions in the menu
    DrawText("<- -> to change settings", SCREEN_WIDTH / 2 - MeasureText("<- -> to change settings", 15) / 2, 
             SCREEN_HEIGHT - 30, 15, GRAY);
}

void UpdateOptionsMenu(Game *game)
{
    // Navigation
    if (IsKeyPressed(KEY_DOWN))
    {
        game->selectedOption = (game->selectedOption + 1) % MENU_OPTIONS_COUNT;
        // play the sound here
    }
    else if (IsKeyPressed(KEY_UP))
    {
        game->selectedOption = (game->selectedOption - 1 + MENU_OPTIONS_COUNT) % MENU_OPTIONS_COUNT;
    }

    // Change settings with left/right keybinds
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT))
    {
        // toggle or cycle values
        switch(game->selectedOption)
        {
            case MENU_SOUND:
                game->settings.soundEnabled = !game->settings.soundEnabled;
                break;
            case MENU_MUSIC:
                game->settings.musicEnabled = !game->settings.musicEnabled;
                break;
            case MENU_FPS:
                game->settings.showFPS = !game->settings.showFPS;
                break;
            case MENU_DIFFICULTY:
                if (IsKeyPressed(KEY_RIGHT))
                {
                    game->settings.difficulty = (game->settings.difficulty + 1) % 3;
                }
                else {
                    game->settings.difficulty = (game->settings.difficulty - 1 + 3) % 3;
                }
                break; // FIX: Added missing break statement
        }
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        if (game->selectedOption == MENU_BACK)
        {
            game->state = MAIN_MENU;
            game->selectedOption = MENU_OPTIONS;
        }
    }

    // Going back using ESC keys
    if (IsKeyPressed(KEY_ESCAPE))
    {
        game->state = MAIN_MENU;
        game->selectedOption = MENU_OPTIONS;
    }
}

void DrawControlsMenu(Game *game)
{
    DrawMenuTitle("CONTROLS");

    int startY = SCREEN_HEIGHT / 2 - 100;
    int spacing = 40;

    DrawText("UP / W - Thrust",
            SCREEN_WIDTH / 2 - MeasureText("UP / W - Thrust", 20) / 2,
                startY, 20, WHITE);

    DrawText("LEFT / A - Rotate Left", 
            SCREEN_WIDTH / 2 - MeasureText("LEFT / A - Rotate Left", 20) / 2,
                startY + spacing, 20, WHITE);

    // FIX: Corrected MeasureText parameter formatting
    DrawText("RIGHT / D - Rotate Right", 
            SCREEN_WIDTH / 2 - MeasureText("RIGHT / D - Rotate Right", 20) / 2,
                startY + spacing * 2, 20, WHITE);

    DrawText("SPACE - Fire", 
            SCREEN_WIDTH / 2 - MeasureText("SPACE - Fire", 20) / 2, 
                startY + spacing * 3, 20, WHITE);

    DrawText("P - Pause Game", 
            SCREEN_WIDTH / 2 - MeasureText("P - Pause Game", 20) / 2, 
                startY + spacing * 4, 20, WHITE);

    DrawText("ESC - Return to Menu", 
            SCREEN_WIDTH / 2 - MeasureText("ESC - Return to Menu", 20) / 2, 
                startY + spacing * 5, 20, WHITE);

    // back button
    DrawMenuOption("BACK", startY + spacing * 7, true);

    // Instructions for the menu
    DrawText("Press ENTER or ESC to return",
        SCREEN_WIDTH / 2 - MeasureText("Press ENTER or ESC to return", 15) / 2,
        SCREEN_HEIGHT - 30,
        15, GRAY);
}

void UpdateControlsMenu(Game *game)
{
    // Only need to handle back action
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
    {
        game->state = MAIN_MENU;
        game->selectedOption = MENU_CONTROLS;           // usually is always selected at main menu controls
    }
}

void DrawPauseMenu(Game *game)
{
    // Semi-transparent overlay
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color) { 0, 0, 0, 150 });

    // Draw the title
    const char *titleText = "PAUSED";
    int titleFontSize = 40;
    int titleWidth = MeasureText(titleText, titleFontSize);
    int titleX = SCREEN_WIDTH/2 - titleWidth/2;
    int titleY = SCREEN_HEIGHT/6;
    DrawText(titleText, titleX, titleY, titleFontSize, WHITE);

    // Menu options
    int startY = SCREEN_HEIGHT/2 - 40;
    int spacing = 50;
    
    // Draw RESUME option
    const char *resumeText = "RESUME";
    int optionFontSize = game->selectedOption == 0 ? 25 : 20;
    Color optionColor = game->selectedOption == 0 ? YELLOW : WHITE;
    int resumeWidth = MeasureText(resumeText, optionFontSize);
    int resumeX = SCREEN_WIDTH/2 - resumeWidth/2;
    
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
    int returnX = SCREEN_WIDTH/2 - returnWidth/2;
    
    // Draw selector if this option is selected
    if (game->selectedOption == 1) {
        DrawText(">", returnX - 30, startY + spacing, optionFontSize, YELLOW);
    }
    
    DrawText(returnText, returnX, startY + spacing, optionFontSize, optionColor);
    
    // Draw score text
    const char *scoreText = TextFormat("SCORE: %d", game->score);
    int scoreFontSize = 20;
    int scoreWidth = MeasureText(scoreText, scoreFontSize);
    int scoreX = SCREEN_WIDTH/2 - scoreWidth/2;
    int scoreY = SCREEN_HEIGHT - 60;
    
    DrawText(scoreText, scoreX, scoreY, scoreFontSize, YELLOW);
}

void UpdatePauseMenu(Game *game)
{
    // We only need two options
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_UP))
    {
        game->selectedOption = !game->selectedOption;               // toggle between 0 and 1
    }

    if (IsKeyPressed(KEY_ENTER))
    {
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
        game->state = GAMEPLAY;
    }
}
