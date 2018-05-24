//
//  EncoderLame.cpp
//  EncoderToMp3
//
//  Created by Apple on 2018/5/24.
//  Copyright © 2018年 zhoujiawen. All rights reserved.
//

#include "EncoderLame.hpp"
//初始化代码 (pcm数据必须是双声道。否则转换会失败。 使用 lame_set_in_samplerate 设置参数必须与pcm数据采样率一致)
int EncoderLame::Init(const char *pcmFilePath, const char *mp3FilePath, int sampleRate, int channels, int bitRate){
    int ret = -1;
        pcmFile = fopen(pcmFilePath,"rb");
        if (pcmFile) {
            mp3File = fopen(mp3FilePath, "wb");
            if (mp3File) {
                lameClient = lame_init();
                lame_set_in_samplerate(lameClient, sampleRate);
                lame_set_out_samplerate(lameClient, sampleRate);
                lame_set_num_channels(lameClient, channels);//声道
                lame_set_brate(lameClient, bitRate);// 压缩的比特率为128K
                lame_init_params(lameClient);
                ret = 0;
            }
        }
    return ret;
}
//编码
void EncoderLame::Encode(){
        /*
         16 比特(bit)等于1个short [-32768,32768] 共有65536个可能取值
         */
        int bufferSize = 1024 * 256;
        short *buffer = new short[bufferSize / 2];//
        short *leftBuffer = new short[bufferSize / 4];//左声道bufferSize / 4 = 65536长度
        short *rightBuffer = new short[bufferSize / 4];//右声道bufferSize / 4 = 65536长度
        unsigned char *mp3_buffer = new unsigned char[bufferSize];
        size_t readBufferSize = 0;//size_t长度达到设备的最大存储长度
        while ((readBufferSize = fread(buffer, 2, bufferSize / 2, pcmFile))>0) {
            for(int i = 0;i<readBufferSize;i++){
                if(i%2 == 0){
                    leftBuffer[i / 2] = buffer[i];
                }else{
                    rightBuffer[i / 2] = buffer[i];
                }
            }
            size_t wrotSize = lame_encode_buffer(lameClient, (short int *)leftBuffer, (short int *)rightBuffer, (int)(readBufferSize / 2), mp3_buffer, bufferSize);
            fwrite(mp3_buffer, 1, wrotSize, mp3File);
        }
        delete[] buffer;
        delete[] leftBuffer;
        delete[] rightBuffer;
        delete[] mp3_buffer;
}
//销毁
void EncoderLame::Destory(){
        if (pcmFile) {
            fclose(pcmFile);
        }
        if (mp3File) {
            fclose(mp3File);
        }
        lame_close(lameClient);
}
