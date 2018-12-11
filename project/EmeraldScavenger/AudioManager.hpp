//
// Created by Sergiy Isakov on 06.12.2018.
//

#ifndef SIMPLERENDERENGINEPROJECT_AUDIOMANAGER_H
#define SIMPLERENDERENGINEPROJECT_AUDIOMANAGER_H

#include "SDL_mixer.h"
#include "map"

using namespace std;

class AudioManager {
    AudioManager();

    ~AudioManager();

    static AudioManager *sInstance;
    //AssetManager *assetManager;

    map<string, Mix_Music *> mMusic;
    map<string, Mix_Chunk *> mSFX;

public:
    static AudioManager *instance();

    static void release();

    Mix_Music *getMusic(string fileName);

    Mix_Chunk *getSFX(string fileName);

    void playMusic(string fileName, int volume = MIX_MAX_VOLUME, int loops = -1);

    void pauseMusic();

    void resumeMusic();

    void playSFX(string fileName, int volume = MIX_MAX_VOLUME, int loops = 0, int channel = 0);
};


#endif //SIMPLERENDERENGINEPROJECT_AUDIOMANAGER_H
