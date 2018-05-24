//
//  EncoderLame.hpp
//  EncoderToMp3
//
//  Created by Apple on 2018/5/24.
//  Copyright © 2018年 zhoujiawen. All rights reserved.
//

#ifndef EncoderLame_hpp
#define EncoderLame_hpp
#include <stdio.h>
#include "lame.h"
class EncoderLame {
    private:
        FILE *pcmFile;
        FILE *mp3File;
        lame_t lameClient;
    public:
        int Init(const char *pcmFilePath, const char *mp3FilePath, int sampleRate, int channels, int bitRate);
        void Encode();
        void Destory();
};
#endif /* EncoderLame_hpp */
