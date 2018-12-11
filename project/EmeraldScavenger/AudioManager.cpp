#include <utility>

//
// Created by Sergiy Isakov on 06.12.2018.
//

#include <SDL_filesystem.h>
#include <iostream>
#include "AudioManager.hpp"

AudioManager *AudioManager::sInstance = nullptr;

AudioManager::AudioManager() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        cout << "Mixer initialization error: " << Mix_GetError() << endl;
    }
}

AudioManager::~AudioManager() {
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

Mix_Music *AudioManager::getMusic(string fileName) {
    string fullPath = SDL_GetBasePath();
    fullPath.append(fileName);
    if (mMusic[fullPath] == nullptr) {
        mMusic[fullPath] = Mix_LoadMUS(fullPath.c_str());
        if (mMusic[fullPath] == nullptr)
            cout << "Music Loading Error: File: " << fileName.c_str() << " Error: " << Mix_GetError() << endl;
    }
    return mMusic[fullPath];
}

Mix_Chunk *AudioManager::getSFX(string fileName) {
    string fullPath = SDL_GetBasePath();
    fullPath.append(fileName);
    if (mSFX[fullPath] == nullptr) {
        mSFX[fullPath] = Mix_LoadWAV(fullPath.c_str());
        if (mSFX[fullPath] == nullptr)
            cout << "SFX Loading Error: File: " << fileName.c_str() << " Error: " << Mix_GetError() << endl;
    }
    return mSFX[fullPath];
}

void AudioManager::playMusic(string fileName, int volume, int loops) {
    resumeMusic();
    Mix_VolumeMusic(volume);
    Mix_Music* music = this->getMusic(move(fileName));
    Mix_PlayMusic(music, loops);
}

void AudioManager::pauseMusic() {
    if (Mix_PlayingMusic() != 0)Mix_PauseMusic();
}

void AudioManager::resumeMusic() {
    if (Mix_PausedMusic() != 0)Mix_ResumeMusic();
}

void AudioManager::playSFX(string fileName, int volume, int loops, int channel) {
    Mix_Chunk *chunk = this->getSFX(move(fileName));
    Mix_VolumeChunk(chunk, volume);
    Mix_PlayChannel(channel, chunk, loops);
}
