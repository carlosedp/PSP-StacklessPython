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


#ifndef MUSICENGINE_H
#define MUSICENGINE_H


#include "mad.h" 
#include <pspkernel.h>

typedef struct  { 
	short left; 
	short right; 
} Sample; 



/*!
*
*	\fn	int Init()
*	\brief	This function initializes the PSP audio,
*			the MAD libraries and other info to enable
*			mp3 buffering.
*/
int Init(int channel);



/*!
*
*	\fn	int Load(const char* musicfile)
*	\brief	This function sets the path of the current
*			mp3. This path will be used in the callback
*			function to retrieve parts of the mp3.
*
*	@param	musicfile is the path to the song.
*/
int Load(const char* musicfile);



/*!
*
*	\fn	int Play()
*	\brief	This functions turns on the flag that the 
*			current mp3 file can be played.
*/
int Play();



/*!
*
*	\fn int Stop()
*	\brief	This function does not stop the thread. This
*			function sets the pause variable to true so
*			that the callbackfunction passes zero's
*			instead of musicparts. So the callbackfunction 
*			keeps on playing but plays zero's which means 
*			no sound. Also the file indicator is set to
*			zero.
*/
int Stop();




/*!
*
*	\fn int Pause()
*	\brief	This function pauses the music by turning the
*			variable pausesong to 1. This means that zero's
*			are passed to the callbackfunction so that no 
*			sound is played.
*			This function does not reset the file indicator.
*/
int Pause();



/*!
*
*	\fn int EndOfStream()
*	\brief	This function reads the flag that indicates
*			if a strem has reached its end.
*/
int EndOfStream();



/*!
*
*	\fn int fillFileBuffer()
*	\brief	This function is part of the callback function.
*			it reads the file for new parts to store into
*			the buffer.
*/
int fillFileBuffer();



/*!
*
*	\fn	void fillOutputBuffer(void* buffer, unsigned int samplesToWrite, void* userData)
*	\brief	This is the main callback function.
*
*	@param	buffer
*	@param	samplesToWrite
*	@param	userData
*
*/
void fillOutputBuffer(void* buffer, unsigned int samplesToWrite, void* userData);


/*!
*
*	\fn	void decode()
*	\brief	This function decodes a frame.
*
*/
void decode();



/*!
*
*	\fn	short convertSample(mad_fixed_t sample)
*	\brief	This function converts a sample.
*
*	@param	sample is an sample that needs converting.
*
*/	
short convertSample(mad_fixed_t sample);



/*!
*
*	\fn	void convertLeftSamples(Sample* first, Sample* last, const mad_fixed_t* src)
*	\brief	This function coverts the left part of the sample.
*
*	@param	first is the first sample
*	@param	last is the last sample
*	@param	src is the source?
*
*/
void convertLeftSamples(Sample* first, Sample* last, const mad_fixed_t* src);



/*!
*
*	\fn	void convertRightSamples(Sample* first, Sample* last, const mad_fixed_t* src)
*	\brief	This function coverts the right part of the sample.
*
*	@param	first is the first sample
*	@param	last is the last sample
*	@param	src is the source?
*
*/
void convertRightSamples(Sample* first, Sample* last, const mad_fixed_t* src);

/*!
*
*	\fn	void GetTimeString(char *dest)
*	\brief	This function returns a string into a reference containing the stream time.
*
*	@param	string reference
*
*/
void GetTimeString(char *dest);

void FreeTune();

void End();

/*!
*
* variables
*
*/

struct	mad_stream	stream; 
struct	mad_frame	frame; 
struct	mad_synth	synth;
mad_timer_t         timer;

int				pausesong;
int				started;
int             endofstream;
char			Song[356];
int				SongIsChanged;


SceUID			mutex;
SceUID			file; 
unsigned int	fileSize; 
unsigned int	filePos; 
unsigned char	fileBuffer[2048], *FBptr;
unsigned int	samplesRead; 



#endif
