//
// Created by Sergiy Isakov on 06.12.2018.
//

#ifndef SIMPLERENDERENGINEPROJECT_ASSETMANAGER_H
#define SIMPLERENDERENGINEPROJECT_ASSETMANAGER_H

#include "SDL_mixer.h"
#include "map"

using namespace std;

class AssetManager {
    AssetManager();
    ~AssetManager();
    static AssetManager *sInstance;

    map<string, Mix_Music*> mMusic;
    map<string, Mix_Chunk*> mSFX;

public:
    static AssetManager* instance();
    static void release();


    Mix_Music* getMusic(string fileName);
    Mix_Chunk* getSFX(string fileName);
};


#endif //SIMPLERENDERENGINEPROJECT_ASSETMANAGER_H
