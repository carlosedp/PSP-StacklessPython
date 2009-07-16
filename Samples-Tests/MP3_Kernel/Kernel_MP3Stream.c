
#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <psppower.h>
#include <stdio.h>
#include <stdlib.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <psppower.h>
#include <pspdebug.h>
#include <psprtc.h>
#include <pspsdk.h>
#include <pspaudiocodec.h>
#include <pspaudio.h>
#include <string.h>
#include <malloc.h>
#include <pspmpeg.h>

int SetupCallbacks();

PSP_MODULE_INFO("MP3 decodeTest", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(0);

__attribute__ ((constructor))
void loaderInit(){
   pspKernelSetKernelPC();
   pspSdkInstallNoDeviceCheckPatch();
   pspSdkInstallNoPlainModuleCheckPatch();
   pspSdkInstallKernelLoadModulePatch();
}

SceCtrlData input;

static int bitrates[] = {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 };

unsigned long mp3_codec_buffer[65] __attribute__((aligned(64)));
short mp3_mix_buffer[1152 * 2] __attribute__((aligned(64)));

short mp3_output_buffer[4][1152 * 2] __attribute__((aligned(64)));
int mp3_output_index = 0;


SceUID mp3_handle;
u8* mp3_data_buffer;
u16 mp3_data_align;
u32 mp3_sample_per_frame;
u16 mp3_channel_mode;
u32 mp3_data_start;
u32 mp3_data_size;
u8 mp3_getEDRAM;
u32 mp3_channels;
u32 mp3_samplerate;


int main(void)
{
   SetupCallbacks();

   int result = pspSdkLoadStartModule("flash0:/kd/me_for_vsh.prx", PSP_MEMORY_PARTITION_KERNEL);

   result = pspSdkLoadStartModule("flash0:/kd/videocodec.prx", PSP_MEMORY_PARTITION_KERNEL);

   result = pspSdkLoadStartModule("flash0:/kd/audiocodec.prx", PSP_MEMORY_PARTITION_KERNEL);

   result = pspSdkLoadStartModule("flash0:/kd/mpegbase.prx", PSP_MEMORY_PARTITION_KERNEL);

   result = pspSdkLoadStartModule("flash0:/kd/mpeg_vsh.prx", PSP_MEMORY_PARTITION_USER);

   pspSdkFixupImports(result);

   sceMpegInit();

   mp3_handle = sceIoOpen("ms0:/Test.MP3", PSP_O_RDONLY, 0777);
   if (  ! mp3_handle )
      goto wait;

   mp3_channels = 2;
   mp3_samplerate = 44100; //this is mp3 file's samplerate, also can be 48000,....
   mp3_sample_per_frame = 1152;

   mp3_data_start = sceIoLseek32(mp3_handle, 0, PSP_SEEK_CUR);

   memset(mp3_codec_buffer, 0, sizeof(mp3_codec_buffer));

   if ( sceAudiocodecCheckNeedMem(mp3_codec_buffer, 0x1002) < 0 )
      goto wait;
   if ( sceAudiocodecGetEDRAM(mp3_codec_buffer, 0x1002) < 0 )
         goto wait;
   mp3_getEDRAM = 1;

   if ( sceAudiocodecInit(mp3_codec_buffer, 0x1002) < 0 ) {
      goto wait;
   }
   int audio_channel = sceAudioChReserve(0, mp3_sample_per_frame , PSP_AUDIO_FORMAT_STEREO);
   int eof = 0;
   while( !eof ) {
      int samplesdecoded;
      memset(mp3_mix_buffer, 0, mp3_sample_per_frame*2*2);
      unsigned char mp3_header_buf[4];
      if ( sceIoRead( mp3_handle, mp3_header_buf, 4 ) != 4 ) {
         eof = 1;
         continue;
      }
      int mp3_header = mp3_header_buf[0];
      mp3_header = (mp3_header<<8) | mp3_header_buf[1];
      mp3_header = (mp3_header<<8) | mp3_header_buf[2];
      mp3_header = (mp3_header<<8) | mp3_header_buf[3];

      int bitrate = (mp3_header & 0xf000) >> 12;
      int padding = (mp3_header & 0x200) >> 9;

      int frame_size = 144000*bitrates[bitrate]/mp3_samplerate + padding;

      if ( mp3_data_buffer )
         free(mp3_data_buffer);
      mp3_data_buffer = (u8*)memalign(64, frame_size);

      sceIoLseek32(mp3_handle, mp3_data_start, PSP_SEEK_SET); //seek back

      if ( sceIoRead( mp3_handle, mp3_data_buffer, frame_size ) != frame_size ) {
         eof = 1;
         continue;
      }

      mp3_data_start += frame_size;

      mp3_codec_buffer[6] = (unsigned long)mp3_data_buffer;
      mp3_codec_buffer[8] = (unsigned long)mp3_mix_buffer;

      mp3_codec_buffer[7] = mp3_codec_buffer[10] = frame_size;
      mp3_codec_buffer[9] = mp3_sample_per_frame * 4;

      int res = sceAudiocodecDecode(mp3_codec_buffer, 0x1002);
      if ( res < 0 ) {

         eof = 1;
         continue;
      }
      memcpy(mp3_output_buffer[mp3_output_index], mp3_mix_buffer, 1152*4);
      sceAudioOutputBlocking(audio_channel, PSP_AUDIO_VOLUME_MAX, mp3_output_buffer[mp3_output_index]);
      mp3_output_index = (mp3_output_index+1)%4;

      samplesdecoded = mp3_sample_per_frame;
   }

wait:

   if ( mp3_handle ) {
      sceIoClose(mp3_handle);
   }
   if ( mp3_data_buffer) {
      free(mp3_data_buffer);
   }
   if ( mp3_getEDRAM ) {
      sceAudiocodecReleaseEDRAM(mp3_codec_buffer);
   }

   sceCtrlReadBufferPositive(&input, 1);
   while(!(input.Buttons & PSP_CTRL_TRIANGLE))
   {
      sceKernelDelayThread(10000);   // wait 10 milliseconds
      sceCtrlReadBufferPositive(&input, 1);
   }

   sceKernelExitGame();
   return 0;
}


/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
   sceKernelExitGame();
   return 0;
}


/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
   int cbid;

   cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
   sceKernelRegisterExitCallback(cbid);

   sceKernelSleepThreadCB();

   return 0;
}


/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
   int thid = 0;

   thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
   if(thid >= 0)
   {
      sceKernelStartThread(thid, 0, 0);
   }

   return thid;
}
