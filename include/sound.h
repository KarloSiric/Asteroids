#ifndef SOUND_H
#define SOUND_H

#include <raylib.h>

// Forward declaration for Game struct to avoid circular dependency
typedef struct Game Game;

// Sound Constants
#define MAX_SOUNDS 8

// Sound Types
typedef enum {
    SOUND_MENU_MUSIC,     // Background music for the menu
    SOUND_GAME_MUSIC,     // Background music during gameplay
    SOUND_SHOOT,          // Bullet shooting sound
    SOUND_EXPLOSION_BIG,  // Large asteroid explosion
    SOUND_EXPLOSION_SMALL,// Small asteroid explosion
    SOUND_THRUST,         // Player ship thrust sound
    SOUND_MENU_SELECT,    // Menu selection sound
    SOUND_GAME_OVER       // Game over sound
} SoundType;

// Sound structure
typedef struct SoundManager {
    Sound sounds[MAX_SOUNDS];
    Music menuMusic;
    Music gameMusic;
    bool soundLoaded[MAX_SOUNDS];
    bool musicLoaded;
    float musicVolume;
    float soundVolume;
} SoundManager;

// Function prototypes
void InitSoundManager(SoundManager *soundManager);
void LoadGameSounds(SoundManager *soundManager);
void UnloadGameSounds(SoundManager *soundManager);
void PlayGameSound(SoundManager *soundManager, SoundType soundType);
void UpdateGameMusic(SoundManager *soundManager, Game *game);
void SetGameSoundVolume(SoundManager *soundManager, float volume);   // RENAMED from SetSoundVolume
void SetGameMusicVolume(SoundManager *soundManager, float volume);   // RENAMED from SetMusicVolume
void ToggleSoundEnabled(SoundManager *soundManager, bool enabled);
void ToggleMusicEnabled(SoundManager *soundManager, bool enabled);
void PauseGameMusic(SoundManager *soundManager);
void ResumeGameMusic(SoundManager *soundManager);

#endif // SOUND_H
