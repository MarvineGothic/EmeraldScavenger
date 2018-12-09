//
// Created by Sergiy Isakov on 06.12.2018.
//

#ifndef SIMPLERENDERENGINEPROJECT_AUDIOMANAGER_H
#define SIMPLERENDERENGINEPROJECT_AUDIOMANAGER_H


#include "AssetManager.hpp"

class AudioManager {
    AudioManager();

    ~AudioManager();

    static AudioManager *sInstance;
    AssetManager *assetManager;
public:
    static AudioManager *instance();

    static void release();

    void playMusic(string fileName, int volume = MIX_MAX_VOLUME, int loops = -1);

    void pauseMusic();

    void resumeMusic();

    void playSFX(string fileName, int volume = MIX_MAX_VOLUME, int loops = 0, int channel = 0);
};


#endif //SIMPLERENDERENGINEPROJECT_AUDIOMANAGER_H
