/*!
*
*		This file is created by R.N. den Hollander
*		a.k.a Ghoti.
*		This file is intended for use for the stackless
*		python project. If you want to use this for something
*		else that is ok but please let me know.
*
*		url: www.ghoti.nl
*
*
*/

/*!
*	This file is the code of the MP3 streaming
*	engine.
*
*/

#include "MusicEngine.h"
#include <pspaudiolib.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h> 
#include <pspdebug.h>

#define printf	pspDebugScreenPrintf


int Init(int channel) {
	pspAudioInit();											// init the audio psp

	mad_stream_init(&stream);								// create the mad variables, streams etc.
	mad_frame_init(&frame); 
	mad_synth_init(&synth); 
	mad_timer_reset(&timer);
	pspAudioSetChannelCallback(channel, fillOutputBuffer, 0);		// set the callback to the function.
	mutex = sceKernelCreateSema("myMutexName", 0, 1, 1, 0);	// create the mutex for threading secure.
	pausesong = 1;											// set the variable to pause so no sound is played.
	started = 0;											// the mp3 has not yet started.
	file = 0;												// set the file to zero
	fileSize = 1;
	filePos = 0;
	printf("MusicEngine is initialized.\n");

	return 1;
};



int Load(const char* musicfile) {
    endofstream = 0;                      // set the endofsong flag to 0.
	sceKernelWaitSema(mutex, 1, 0);		// wait for thread to be open.
	sprintf(Song, "%s", musicfile);		// Song is not used by another thread so we can change it
	SongIsChanged = 1;					// Mark status that the song is changed
	sceKernelSignalSema(mutex, 1);		// free the lock of this thread.
	printf("Music file %s loaded.\n", Song);

	return 1;
};


int Play() {
	if(pausesong == 1 && started == 0) {
		started = 1;
		pausesong = 0;
		printf("Play music true.\n");
		return 1;
	}
	printf("Play music false.\n");
	return 0;
};


int Stop() {
	pausesong = 1;
	started = 0;
	file = 0;
	printf("Music file stopped.");
	return 1;
};



int Pause() {
	pausesong = !pausesong;
	printf("Music file paused.");
	return 1;
};

int EndOfStream() {
    if (endofstream == 1) {
        return 1;
    } else {
        return 0;
    }
};

void GetTimeString(char *dest) {
    mad_timer_string(timer, dest, "%02lu:%02u:%02u", MAD_UNITS_HOURS, MAD_UNITS_MILLISECONDS, 0);
};

void End() {
    Stop();
    pspAudioEnd();
    FreeTune();
};

void FreeTune() {
    if (FBptr)
		free(FBptr);

    mad_synth_finish(&synth);
    mad_frame_finish(&frame);
    mad_stream_finish(&stream);
};

int fillFileBuffer() { 
	// wait for semaphore to be open
	sceKernelWaitSema(mutex, 1, 0);

	// Open the file if it's not open. 
	if (SongIsChanged == 1) {
		SongIsChanged = 0;
		if (file>0) sceIoClose(file); // If the file wasn't closed before (got to next song before reaching EOF), close it now 
		file = 0;
	}
	if (file <= 0) { 
		char   cd[1024]; 
		memset(cd, 0, sizeof(cd)); 
		getcwd(cd, sizeof(cd) - 1); 

		char   fileName[1024]; 
		memset(fileName, 0, sizeof(fileName)); 
		snprintf(fileName, sizeof(fileName) - 1, "%s/%s", cd, Song);

		file = sceIoOpen(fileName, PSP_O_RDONLY, 777); 
		if (file <= 0) 
		{ 
			pspDebugScreenPrintf("Failed (%s).\n", Song); 
			sceKernelSignalSema(mutex, 1);
			/*
			* do something when file is not loaded
			*/
			return 1; 
		} 

		// Get the size. 
		fileSize = sceIoLseek(file, 0, SEEK_END); 
		sceIoLseek(file, 0, SEEK_SET); 
	} 

	// Find out how much to keep and how much to fill. 
	const unsigned int   bytesToKeep   = stream.bufend - stream.next_frame; 
	unsigned int      bytesToFill   = sizeof(fileBuffer) - bytesToKeep; 
	//pspDebugScreenPrintf("bytesToFill = %u, bytesToKeep = %u.\n", bytesToFill, bytesToKeep); 

	// Want to keep any bytes? 
	if (bytesToKeep) 
	{ 
		// Copy the tail to the head. 
		memmove(fileBuffer, fileBuffer + sizeof(fileBuffer) - bytesToKeep, bytesToKeep); 
	} 

	// Read into the rest of the file buffer. 
	unsigned char* bufferPos = fileBuffer + bytesToKeep; 
	while (bytesToFill > 0) 
	{ 
		// Read some. 
		//pspDebugScreenPrintf("Reading %u bytes...\n", bytesToFill); 
		const unsigned int bytesRead = sceIoRead(file, bufferPos, bytesToFill); 

		// EOF? 
		if (bytesRead == 0) { 
			sceKernelSignalSema(mutex, 1);
			return 2; //handle stuff when end of file with return value !!!NOT HERE
		} 

		// Adjust where we're writing to. 
		bytesToFill -= bytesRead; 
		bufferPos += bytesRead; 
		filePos += bytesRead; 

		//pspDebugScreenPrintf("Read %u bytes from the file, %u left to fill.\n", bytesRead, bytesToFill); 
		//pspDebugScreenPrintf("%u%%.\n", filePos * 100 / fileSize); 
	} 
	// release the semaphore
	sceKernelSignalSema(mutex, 1);
	return 0;
} 



void decode() { 
	// While we need to fill the buffer... 
	while ( 
	(mad_frame_decode(&frame, &stream) == -1) && 
	((stream.error == MAD_ERROR_BUFLEN) || (stream.error == MAD_ERROR_BUFPTR)) 
	) 
	{ 
		// Fill up the remainder of the file buffer. 
		int tmp;
		tmp = fillFileBuffer(); 
		if (tmp==2) {
            endofstream = 1;
			//MusicPlayerNextSong();
			/*!
			*
			*	Put here the function that does something when the file has ended.
			*
			*/
		}

		// Give new buffer to the stream. 
		mad_stream_buffer(&stream, fileBuffer, sizeof(fileBuffer)); 
	}
    // Add to the timer the stream duration
    mad_timer_add(&timer, frame.header.duration);
	// Synth the frame. 
	mad_synth_frame(&synth, &frame); 
} 



short convertSample(mad_fixed_t sample) { 
	// round  
	sample += (1L << (MAD_F_FRACBITS - 16)); 

	// clip 
	if (sample >= MAD_F_ONE) 
	sample = MAD_F_ONE - 1; 
	else if (sample < -MAD_F_ONE) 
	sample = -MAD_F_ONE; 

	// quantize
	return sample >> (MAD_F_FRACBITS + 1 - 16); 
} 



void convertLeftSamples(Sample* first, Sample* last, const mad_fixed_t* src) { 
	Sample* dst;
	for (dst = first; dst != last; ++dst)  
		dst->left = convertSample(*src++); 
} 




void convertRightSamples(Sample* first, Sample* last, const mad_fixed_t* src) { 
	Sample* dst;
	for (dst = first; dst != last; ++dst) 
		dst->right = convertSample(*src++); 
} 




void fillOutputBuffer(void* buffer, unsigned int samplesToWrite, void* userData) { 

	// Where are we writing to? 
	Sample* destination = (Sample*)buffer;
	unsigned int i = 0; 
	//printf("filloutputbuffer wordt gedraaid.\n");
	if (pausesong == 1) { 
		for (i=0; i<samplesToWrite; ++i) { 
			// fill the buffer with no sound
			destination[i].left	= 0; 
			destination[i].right = 0; 
		} 
		return; 
	}

	// While we've got samples to write... 
	while (samplesToWrite > 0) 	{ 
		// Enough samples available? 
		const unsigned int samplesAvailable = synth.pcm.length - samplesRead; 

		if (samplesAvailable > samplesToWrite) { 
			// Write samplesToWrite samples. 
			convertLeftSamples(destination, destination + samplesToWrite, &synth.pcm.samples[0][samplesRead]); 
			convertRightSamples(destination, destination + samplesToWrite, &synth.pcm.samples[1][samplesRead]); 

			// We're still using the same PCM data. 
			samplesRead += samplesToWrite; 

			// Done. 
			samplesToWrite = 0; 
		} 
		else { 
			// Write samplesAvailable samples. 
			convertLeftSamples(destination, destination + samplesAvailable, &synth.pcm.samples[0][samplesRead]); 
			convertRightSamples(destination, destination + samplesAvailable, &synth.pcm.samples[1][samplesRead]); 

			// We need more PCM data. 
			samplesRead = 0; 
			decode(); 

			// We've still got more to write. 
			destination += samplesAvailable; 
			samplesToWrite -= samplesAvailable; 
		} 
	} 
} 
