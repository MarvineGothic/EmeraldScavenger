#include <utility>

//
// Created by Sergiy Isakov on 06.12.2018.
//

#include <iostream>
#include "AudioManager.hpp"

AudioManager *AudioManager::sInstance = nullptr;

AudioManager::AudioManager() {
    assetManager = AssetManager::instance();
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        cout << "Mixer initialization error: " << Mix_GetError() << endl;
    }
}

AudioManager::~AudioManager() {
    assetManager = nullptr;
    Mix_Quit();
}

AudioManager *AudioManager::instance() {
    if (sInstance == nullptr)
        sInstance = new AudioManager();
    return sInstance;
}

void AudioManager::release() {
    delete sInstance;
    sInstance = nullptr;
}

void AudioManager::playMusic(string fileName, int volume, int loops) {
    resumeMusic();
    Mix_VolumeMusic(volume);
    Mix_Music* music = assetManager->getMusic(move(fileName));
    Mix_PlayMusic(music, loops);
}

void AudioManager::pauseMusic() {
    if (Mix_PlayingMusic() != 0)Mix_PauseMusic();
}

void AudioManager::resumeMusic() {
    if (Mix_PausedMusic() != 0)Mix_ResumeMusic();
}

void AudioManager::playSFX(string fileName, int volume, int loops, int channel) {
    Mix_Chunk *chunk = assetManager->getSFX(move(fileName));
    Mix_VolumeChunk(chunk, volume);
    Mix_PlayChannel(channel, chunk, loops);
}
