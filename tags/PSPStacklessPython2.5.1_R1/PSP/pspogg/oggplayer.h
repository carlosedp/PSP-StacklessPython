#ifndef _OGGPLAYER_H_
#define _OGGPLAYER_H_

#include <tremor/ivorbiscodec.h>
#include <tremor/ivorbisfile.h>
#include "codec.h"

#ifdef __cplusplus
extern "C" {
#endif

// private functions
    void OGG_Init(int channel);
    int OGG_Play();
    void OGG_Pause();
    int OGG_Stop();
    void OGG_End();
    int OGG_Load(char *filename);
    void OGG_Tick();
    void OGG_Close();
    void OGG_FreeTune();
    int OGG_GetSec();
	int OGG_GetMin();
	int OGG_GetHour();
	void OGG_Volume(int volume);
    int OGG_EndOfStream();

#ifdef __cplusplus
}
#endif
#endif
