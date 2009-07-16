#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>

#include <pspaudiolib.h>
#include <pspdebug.h>
#include <pspkernel.h>
#include <pspmoduleinfo.h>
#include <psppower.h>
#include <pspctrl.h>

#include "mad.h"

PSP_MODULE_INFO("Test", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

static int exitCallback(int arg1, int arg2, void* common)
{
 // Exit.
 sceKernelExitGame();
 return 0;
}

static int powerCallback(int unknown, int powerInfo, void* common)
{
 return 0;
}

static int callbackThread(SceSize args, void *argp)
{
 // Register the exit callback.
 const SceUID exitCallbackID = sceKernelCreateCallback("exitCallback", exitCallback, NULL);
 sceKernelRegisterExitCallback(exitCallbackID);

 // Register the power callback.
 const SceUID powerCallbackID = sceKernelCreateCallback("powerCallback", powerCallback, NULL);
 scePowerRegisterCallback(0, powerCallbackID);

 // Sleep and handle callbacks.
 sceKernelSleepThreadCB();
 return 0;
}

static int setUpCallbackThread(void)
{
 const int thid = sceKernelCreateThread("callbackThread", callbackThread, 0x11, 0xFA0, 0, 0);
 if (thid >= 0)
    sceKernelStartThread(thid, 0, 0);
 return thid;
}

struct Sample
{
 short left;
 short right;
};

static SceUID         file         = 0;
static unsigned int      fileSize      = 1;
static unsigned int      filePos         = 0;
static mad_stream      stream;
static mad_frame      frame;
static mad_synth      synth;
static unsigned char   fileBuffer[2048];
static unsigned int      samplesRead;

static void fillFileBuffer()
{
 // Open the file if it's not open.
 if (file <= 0)
 {
    char   cd[1024];
    memset(cd, 0, sizeof(cd));
    getcwd(cd, sizeof(cd) - 1);

    char   fileName[1024];
    memset(fileName, 0, sizeof(fileName));
    snprintf(fileName, sizeof(fileName) - 1, "%s/%s", cd, "test.mp3");

    pspDebugScreenPrintf("Opening %s... ", fileName);
    file = sceIoOpen(fileName, PSP_O_RDONLY, 777);
    if (file <= 0)
    {
       pspDebugScreenPrintf("Failed (%d).\n", file);
       return;
    }
    else
    {
       pspDebugScreenPrintf("OK (%d).\n", file);
    }

    // Get the size.
    fileSize = sceIoLseek(file, 0, SEEK_END);
    sceIoLseek(file, 0, SEEK_SET);
 }

 // Find out how much to keep and how much to fill.
 const unsigned int   bytesToKeep   = stream.bufend - stream.next_frame;
 unsigned int      bytesToFill   = sizeof(fileBuffer) - bytesToKeep;
 pspDebugScreenPrintf("bytesToFill = %u, bytesToKeep = %u.\n", bytesToFill, bytesToKeep);

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
    pspDebugScreenPrintf("Reading %u bytes...\n", bytesToFill);
    const unsigned int bytesRead = sceIoRead(file, bufferPos, bytesToFill);

    // EOF?
    if (bytesRead == 0)
    {
       pspDebugScreenPrintf("End of file.\n");
       sceIoLseek(file, 0, SEEK_SET);
       filePos = 0;
       continue;
    }

    // Adjust where we're writing to.
    bytesToFill -= bytesRead;
    bufferPos += bytesRead;
    filePos += bytesRead;

    pspDebugScreenPrintf("Read %u bytes from the file, %u left to fill.\n", bytesRead, bytesToFill);
    pspDebugScreenPrintf("%u%%.\n", filePos * 100 / fileSize);
 }
}

static void decode()
{
 // While we need to fill the buffer...
 while (
    (mad_frame_decode(&frame, &stream) == -1) &&
    ((stream.error == MAD_ERROR_BUFLEN) || (stream.error == MAD_ERROR_BUFPTR))
    )
 {
    // Fill up the remainder of the file buffer.
    fillFileBuffer();

    // Give new buffer to the stream.
    mad_stream_buffer(&stream, fileBuffer, sizeof(fileBuffer));
 }

 // Synth the frame.
 mad_synth_frame(&synth, &frame);
}

static inline short convertSample(mad_fixed_t sample)
{
 /* round */
 sample += (1L << (MAD_F_FRACBITS - 16));

 /* clip */
 if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
 else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

 /* quantize */
 return sample >> (MAD_F_FRACBITS + 1 - 16);
}

static void convertLeftSamples(Sample* first, Sample* last, const mad_fixed_t* src)
{
 for (Sample* dst = first; dst != last; ++dst)
 {
    dst->left = convertSample(*src++);
 }
}

static void convertRightSamples(Sample* first, Sample* last, const mad_fixed_t* src)
{
 for (Sample* dst = first; dst != last; ++dst)
 {
    dst->right = convertSample(*src++);
 }
}

static void fillOutputBuffer(void* buffer, unsigned int samplesToWrite, void* userData)
{
 // Where are we writing to?
 Sample* destination = static_cast<Sample*> (buffer);

 // While we've got samples to write...
 while (samplesToWrite > 0)
 {
    // Enough samples available?
    const unsigned int samplesAvailable = synth.pcm.length - samplesRead;
    if (samplesAvailable > samplesToWrite)
    {
       // Write samplesToWrite samples.
       convertLeftSamples(destination, destination + samplesToWrite, &synth.pcm.samples[0][samplesRead]);
       convertRightSamples(destination, destination + samplesToWrite, &synth.pcm.samples[1][samplesRead]);

       // We're still using the same PCM data.
       samplesRead += samplesToWrite;

       // Done.
       samplesToWrite = 0;
    }
    else
    {
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

int main(int argc, char *argv[])
{
   // Set up the callback thread.
   setUpCallbackThread();

   // Initialise the debug screen.
   pspDebugScreenInit();

   // Set up MAD.
   pspDebugScreenPrintf("Setting up MAD... ");
   mad_stream_init(&stream);
   mad_frame_init(&frame);
   mad_synth_init(&synth);
   pspDebugScreenPrintf("OK.\n");

   // Initialise the audio system.
   pspAudioInit();

   // Set the channel callback.
   pspDebugScreenPrintf("Decoding...\n");
   pspAudioSetChannelCallback(0, fillOutputBuffer, 0);

   // Wait for a button press.
   SceCtrlData pad;
   memset(&pad, 0, sizeof(pad));
   while (pad.Buttons != 0)
   {
      sceCtrlReadBufferPositive(&pad, 1);
   }
   while (pad.Buttons == 0)
   {
      sceCtrlReadBufferPositive(&pad, 1);
   }
   while (pad.Buttons != 0)
   {
      sceCtrlReadBufferPositive(&pad, 1);
   }

   // Shut down audio.
   pspAudioEnd();

   // Shut down MAD.
   mad_synth_finish(&synth);
   mad_frame_finish(&frame);
   mad_stream_finish(&stream);

   // Quit.
   sceKernelExitGame();
   return 0;
}
