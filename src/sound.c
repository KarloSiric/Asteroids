/*
* @Author: karlosiric
* @Date:   2025-05-11 19:30:00
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 18:16:06
*/

/* 
 * Sound management for the Asteroids game
 */

#include "sound.h"
#include "game.h"
#include <raylib.h>
#include <stdlib.h>

void InitSoundManager(SoundManager *soundManager)
{
    // Initialize the audio device
    InitAudioDevice();
    
    // Initialize volumes
    soundManager->musicVolume = 0.7f;
    soundManager->soundVolume = 1.0f;
    
    // Initialize sound loading states
    for (int i = 0; i < MAX_SOUNDS; i++) {
        soundManager->soundLoaded[i] = false;
    }
    
    soundManager->musicLoaded = false;
    
    // Load all game sounds
    LoadGameSounds(soundManager);
}

void LoadGameSounds(SoundManager *soundManager)
{
    // Load sound effects - using your specific file names
    
    // Shooting sounds - using alienshoot files
    if (FileExists("resources/sounds/alienshoot1.wav")) {
        soundManager->sounds[SOUND_SHOOT] = LoadSound("resources/sounds/alienshoot1.wav");
        soundManager->soundLoaded[SOUND_SHOOT] = true;
    }
    
    // Big explosion - using explosion_1.wav (presumably the largest one)
    if (FileExists("resources/sounds/explosion_1.wav")) {
        soundManager->sounds[SOUND_EXPLOSION_BIG] = LoadSound("resources/sounds/explosion_1.wav");
        soundManager->soundLoaded[SOUND_EXPLOSION_BIG] = true;
    }
    
    // Small explosion - using explosion_3.wav (medium sized one)
    if (FileExists("resources/sounds/explosion_3.wav")) {
        soundManager->sounds[SOUND_EXPLOSION_SMALL] = LoadSound("resources/sounds/explosion_3.wav");
        soundManager->soundLoaded[SOUND_EXPLOSION_SMALL] = true;
    }
    
    // Thrust sound - using engine.wav
    if (FileExists("resources/sounds/engine.wav")) {
        soundManager->sounds[SOUND_THRUST] = LoadSound("resources/sounds/engine.wav");
        soundManager->soundLoaded[SOUND_THRUST] = true;
    }
    
    // Menu selection sound
    if (FileExists("resources/sounds/menu_select.wav")) {
        soundManager->sounds[SOUND_MENU_SELECT] = LoadSound("resources/sounds/menu_select.wav");
        soundManager->soundLoaded[SOUND_MENU_SELECT] = true;
    }
    
    // Game over sound - you have this as MP3 so we'll use that
    if (FileExists("resources/sounds/game_over.mp3")) {
        soundManager->sounds[SOUND_GAME_OVER] = LoadSound("resources/sounds/game_over.mp3");
        soundManager->soundLoaded[SOUND_GAME_OVER] = true;
    }
    
    // Load music files
    if (FileExists("resources/music/menu_music.mp3")) {
        soundManager->menuMusic = LoadMusicStream("resources/music/menu_music.mp3");
        SetMusicVolume(soundManager->menuMusic, soundManager->musicVolume);
        soundManager->musicLoaded = true;
    }
    
    // For game music, we'll use menu_music2.mp3 if it exists
    if (FileExists("resources/music/menu_music2.mp3")) {
        soundManager->gameMusic = LoadMusicStream("resources/music/menu_music2.mp3");
        SetMusicVolume(soundManager->gameMusic, soundManager->musicVolume);
    } else if (soundManager->musicLoaded) {
        // Fallback to the same music for both menu and game if separate game music doesn't exist
        soundManager->gameMusic = soundManager->menuMusic;
    }
}

void UnloadGameSounds(SoundManager *soundManager)
{
    // Unload all sound effects that were loaded
    for (int i = 0; i < MAX_SOUNDS; i++) {
        if (soundManager->soundLoaded[i]) {
            UnloadSound(soundManager->sounds[i]);
            soundManager->soundLoaded[i] = false;
        }
    }
    
    // Unload music if it was loaded
    if (soundManager->musicLoaded) {
        // Only unload menuMusic if gameMusic isn't the same pointer
        UnloadMusicStream(soundManager->menuMusic);
        
        // Only unload gameMusic if it's different from menuMusic
        if (soundManager->gameMusic.ctxData != soundManager->menuMusic.ctxData) {
            UnloadMusicStream(soundManager->gameMusic);
        }
        
        soundManager->musicLoaded = false;
    }
    
    // Close the audio device
    CloseAudioDevice();
}

void PlayGameSound(SoundManager *soundManager, SoundType soundType)
{
    // Only play if the sound was loaded successfully and sound is enabled
    if (soundType < MAX_SOUNDS && soundManager->soundLoaded[soundType]) {
        PlaySound(soundManager->sounds[soundType]);
    }
}

void UpdateGameMusic(SoundManager *soundManager, Game *game)
{
    // Only update music if it was loaded successfully
    if (!soundManager->musicLoaded) return;
    
    // Update music stream, required to play music
    UpdateMusicStream(soundManager->menuMusic);
    if (soundManager->gameMusic.ctxData != soundManager->menuMusic.ctxData) {
        UpdateMusicStream(soundManager->gameMusic);
    }
    
    static GameState previousState = 0;
    // Remove unused variable
    previousState = game->state;
    
    // Switch between menu and game music based on game state
    if (game->state == GAMEPLAY) {
        // If in gameplay, stop menu music and play game music if it's not already playing
        if (IsMusicStreamPlaying(soundManager->menuMusic)) {
            StopMusicStream(soundManager->menuMusic);
        }
        
        if (!IsMusicStreamPlaying(soundManager->gameMusic) && game->settings.musicEnabled) {
            PlayMusicStream(soundManager->gameMusic);
        }
    } else if (game->state != PAUSED) { // Don't change music when paused
        // If in menu, stop game music and play menu music if it's not already playing
        if (IsMusicStreamPlaying(soundManager->gameMusic)) {
            StopMusicStream(soundManager->gameMusic);
        }
        
        if (!IsMusicStreamPlaying(soundManager->menuMusic) && game->settings.musicEnabled) {
            PlayMusicStream(soundManager->menuMusic);
        }
    }
}

void SetGameSoundVolume(SoundManager *soundManager, float volume)
{
    // Clamp volume between 0.0 and 1.0
    soundManager->soundVolume = volume < 0.0f ? 0.0f : (volume > 1.0f ? 1.0f : volume);
    
    // Apply volume to all loaded sounds
    for (int i = 0; i < MAX_SOUNDS; i++) {
        if (soundManager->soundLoaded[i]) {
            SetSoundVolume(soundManager->sounds[i], soundManager->soundVolume);
        }
    }
}

void SetGameMusicVolume(SoundManager *soundManager, float volume)
{
    // Clamp volume between 0.0 and 1.0
    soundManager->musicVolume = volume < 0.0f ? 0.0f : (volume > 1.0f ? 1.0f : volume);
    
    if (soundManager->musicLoaded) {
        SetMusicVolume(soundManager->menuMusic, soundManager->musicVolume);
        if (soundManager->gameMusic.ctxData != soundManager->menuMusic.ctxData) {
            SetMusicVolume(soundManager->gameMusic, soundManager->musicVolume);
        }
    }
}

void ToggleSoundEnabled(SoundManager *soundManager, bool enabled)
{
    if (enabled) {
        SetGameSoundVolume(soundManager, soundManager->soundVolume);
    } else {
        // Keep the soundManager->soundVolume value but set actual sound output to 0
        for (int i = 0; i < MAX_SOUNDS; i++) {
            if (soundManager->soundLoaded[i]) {
                SetSoundVolume(soundManager->sounds[i], 0.0f);
            }
        }
    }
}

void ToggleMusicEnabled(SoundManager *soundManager, bool enabled)
{
    if (!soundManager->musicLoaded) return;
    
    if (enabled) {
        SetMusicVolume(soundManager->menuMusic, soundManager->musicVolume);
        if (soundManager->gameMusic.ctxData != soundManager->menuMusic.ctxData) {
            SetMusicVolume(soundManager->gameMusic, soundManager->musicVolume);
        }
        // Resume the appropriate music based on current state (handled in UpdateGameMusic)
    } else {
        // Pause all music
        if (IsMusicStreamPlaying(soundManager->menuMusic)) {
            PauseMusicStream(soundManager->menuMusic);
        }
        
        if (soundManager->gameMusic.ctxData != soundManager->menuMusic.ctxData && 
            IsMusicStreamPlaying(soundManager->gameMusic)) {
            PauseMusicStream(soundManager->gameMusic);
        }
    }
}

void PauseGameMusic(SoundManager *soundManager)
{
    if (soundManager->musicLoaded) {
        if (IsMusicStreamPlaying(soundManager->menuMusic)) {
            PauseMusicStream(soundManager->menuMusic);
        }
        
        if (soundManager->gameMusic.ctxData != soundManager->menuMusic.ctxData && 
            IsMusicStreamPlaying(soundManager->gameMusic)) {
            PauseMusicStream(soundManager->gameMusic);
        }
    }
}

void ResumeGameMusic(SoundManager *soundManager)
{
    if (!soundManager->musicLoaded) return;
    
    // We don't directly resume music here, UpdateGameMusic will handle it
    // based on the current game state
}
