//
// Created by Sergiy Isakov on 06.12.2018.
//

#include <SDL_filesystem.h>
#include <iostream>
#include "AssetManager.hpp"

AssetManager *AssetManager::sInstance = nullptr;

AssetManager::AssetManager() {

}

AssetManager::~AssetManager() {

}

AssetManager *AssetManager::instance() {
    if (sInstance == nullptr)
        sInstance = new AssetManager();
    return sInstance;
}

void AssetManager::release() {
    delete sInstance;
    sInstance = nullptr;
}

Mix_Music *AssetManager::getMusic(string fileName) {
    string fullPath = SDL_GetBasePath();
    fullPath.append(fileName);
    if (mMusic[fullPath] == nullptr) {
        mMusic[fullPath] = Mix_LoadMUS(fullPath.c_str());
        if (mMusic[fullPath] == nullptr)
            cout << "Music Loading Error: File: " << fileName.c_str() << " Error: " << Mix_GetError() << endl;
    }
    return mMusic[fullPath];
}

Mix_Chunk *AssetManager::getSFX(string fileName) {
    string fullPath = SDL_GetBasePath();
    fullPath.append(fileName);
    if (mSFX[fullPath] == nullptr) {
        mSFX[fullPath] = Mix_LoadWAV(fullPath.c_str());
        if (mSFX[fullPath] == nullptr)
            cout << "SFX Loading Error: File: " << fileName.c_str() << " Error: " << Mix_GetError() << endl;
    }
    return mSFX[fullPath];
}
