#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <pspaudio.h>
#include <pspaudiolib.h>
#include "oggplayer.h"

#define FALSE 0
#define TRUE !FALSE
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define INPUT_BUFFER_SIZE	(5*8192)
#define OUTPUT_BUFFER_SIZE	2048

OggVorbis_File vf;
int eof = 0;
int current_section;
char **oggComments;
vorbis_info *vi;
static int isPlaying;
static int myChannel;
int fd = 0;

static int eos;

unsigned long ret;

#define MAXVOLUME	0x8000
int ogg_volume = 100;

static void OGGCallback(void *_buf2, unsigned int numSamples, void *pdata)
{
    short *_buf = (short *)_buf2;
    static short tempmixbuf[PSP_NUM_AUDIO_SAMPLES * 2 * 2] __attribute__ ((aligned(64)));
    static unsigned long tempmixleft = 0;
    if (isPlaying == TRUE) {
	if (vi->rate == 44100) {
	while (tempmixleft < numSamples) {
	    unsigned long bytesRequired = (numSamples - tempmixleft) * 4;	// 2channels, 16bit = 4 bytes per sample
	    ret = ov_read(&vf, (char *) &tempmixbuf[tempmixleft * 2], bytesRequired, &current_section);
	    if (ret == 0) {	//EOF
			eos = 1;
			if (ov_pcm_seek_page(&vf, 0) != 0) {
				OGG_End();
			}
			return;
	    } else if (ret < 0) {
		return;
	    }
	    tempmixleft += ret / 4;
		}
	}
	if (tempmixleft >= numSamples) {
	    int count, count2;
	    short *_buf2;
	    for (count = 0; count < numSamples; count++) {
		count2 = count + count;
		_buf2 = _buf + count2;
		*(_buf2) = tempmixbuf[count2];
		*(_buf2 + 1) = tempmixbuf[count2 + 1];
	    }
	    tempmixleft -= numSamples;
	    for (count = 0; count < tempmixleft; count++)
		tempmixbuf[count] = tempmixbuf[numSamples + count];
	}

    } else {
	int count;
	for (count = 0; count < numSamples * 2; count++)
	    *(_buf + count) = 0;
    }
}

size_t ogg_callback_read(void *ptr, size_t size, size_t nmemb, void *datasource)
{
    return sceIoRead(*(int *) datasource, ptr, size * nmemb);
}
int ogg_callback_seek(void *datasource, ogg_int64_t offset, int whence)
{
    return sceIoLseek32(*(int *) datasource, (unsigned int) offset, whence);
}
long ogg_callback_tell(void *datasource)
{
    return sceIoLseek32(*(int *) datasource, 0, SEEK_CUR);
}
int ogg_callback_close(void *datasource)
{
    return sceIoClose(*(int *) datasource);
}

int OGG_Load(char *filename)
{
    eos = 0;
    isPlaying = 0;
    ov_callbacks ogg_callbacks;

    ogg_callbacks.read_func = ogg_callback_read;
    ogg_callbacks.seek_func = ogg_callback_seek;
    ogg_callbacks.close_func = ogg_callback_close;
    ogg_callbacks.tell_func = ogg_callback_tell;

    if ((fd = sceIoOpen(filename, PSP_O_RDONLY, 0777)) <= 0) {
	return 0;
    }
    if (ov_open_callbacks(&fd, &vf, NULL, 0, ogg_callbacks) < 0) {
	return 0;
    } else {
	oggComments = ov_comment(&vf, -1)->user_comments;
	vi = ov_info(&vf, -1);
    }
    return 1;
}

void OGG_Init(int channel)
{
    myChannel = channel;
    isPlaying = FALSE;
    pspAudioSetChannelCallback(myChannel, OGGCallback,0);
}

int OGG_Play()
{
    if (isPlaying)
	return FALSE;
    isPlaying = TRUE;
    return TRUE;
}

int OGG_Stop()
{
    isPlaying = FALSE;

    return TRUE;
}

void OGG_Pause()
{
    isPlaying = !isPlaying;
}

void OGG_FreeTune()
{
    ov_clear(&vf);
    if (fd)
	sceIoClose(fd);
}

void OGG_End()
{
    OGG_Stop();
    pspAudioSetChannelCallback(myChannel, 0,0);
    OGG_FreeTune();
}

int OGG_GetSec()
{
  unsigned int time = (unsigned int) ov_time_tell(&vf);
#define F_MULT 1000
#define S_MULT 60
#define M_MULT 60
#define H_MULT 60
    unsigned int timeS, timeM, timeH, timeF;
    timeF = time % F_MULT;
    time -= timeF;
    time /= F_MULT;
    timeS = time % S_MULT;
    time -= timeS;
    time /= S_MULT;
    timeM = time % M_MULT;
    time -= timeM;
    time /= M_MULT;
    timeH = time;
    return timeS;
}

int OGG_GetMin()
{
  unsigned int time = (unsigned int) ov_time_tell(&vf);
#define F_MULT 1000
#define S_MULT 60
#define M_MULT 60
#define H_MULT 60
    unsigned int timeS, timeM, timeH, timeF;
    timeF = time % F_MULT;
    time -= timeF;
    time /= F_MULT;
    timeS = time % S_MULT;
    time -= timeS;
    time /= S_MULT;
    timeM = time % M_MULT;
    time -= timeM;
    time /= M_MULT;
    timeH = time;
    return timeM;
}

int OGG_GetHour()
{
  unsigned int time = (unsigned int) ov_time_tell(&vf);
#define F_MULT 1000
#define S_MULT 60
#define M_MULT 60
#define H_MULT 60
    unsigned int timeS, timeM, timeH, timeF;
    timeF = time % F_MULT;
    time -= timeF;
    time /= F_MULT;
    timeS = time % S_MULT;
    time -= timeS;
    time /= S_MULT;
    timeM = time % M_MULT;
    time -= timeM;
    time /= M_MULT;
    timeH = time;
    return timeH;
}

void OGG_Volume(int volume)
{
	ogg_volume = volume;
	int vol = MAXVOLUME*ogg_volume/100;
	pspAudioSetVolume(myChannel,vol,vol);
}

int OGG_EndOfStream()
{
    if (eos == 1)
	return 1;
    return 0;
}
