#ifndef MENU_H
#define MENU_H 

#include <raylib.h>
#include "game.h"

// MENU OPTIONS indices for the MAIN MENU
#define MENU_START                      0
#define MENU_OPTIONS                    1
#define MENU_CONTROLS                   2
#define MENU_EXIT                       3
#define MENU_MAIN_COUNT                 4

// menu indices for options menu
#define MENU_SOUND                      0
#define MENU_MUSIC                      1
#define MENU_FPS                        2
#define MENU_DIFFICULTY                 3
#define MENU_BACK                       4
#define MENU_OPTIONS_COUNT              5

// Making function prototypes for the fully functional menu
void DrawMainMenu(Game *game);
void DrawOptionsMenu(Game *game);
void DrawControlsMenu(Game *game);
void DrawPauseMenu(Game *game);
void UpdateMainMenu(Game *game);
void UpdateOptionsMenu(Game *game);
void UpdateControlsMenu(Game *game);
void UpdatePauseMenu(Game *game);

#endif                                   // end of the MENU_H header config
