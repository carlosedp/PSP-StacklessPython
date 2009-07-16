/*!
*
*	This is the main file of the MP3 streaming
*	engine.
*/

#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspgu.h>
#include <stdio.h>
#include <stdlib.h>
#include <psputils.h>
#include <dirent.h> 
#include <psprtc.h>
#include <time.h>
#include <psppower.h>
#include "MusicEngine.h"

#define printf   pspDebugScreenPrintf

PSP_MODULE_INFO("Mp3Streaming", 0x0000, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);



/*!
*
*	Exit callback function
*
*/
int exit_callback(int arg1, int arg2, void *common) {
		  sceKernelExitGame();
		  return 0;
}


/*!
*
*	Callback thread function
*
*/
int CallbackThread(SceSize args, void *argp) {
		  int cbid;

		  cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
		  sceKernelRegisterExitCallback(cbid);

		  sceKernelSleepThreadCB();

		  return 0;
}


/*!
*
*	Sets up the callback thread and 
*	returns its thread id function
*
*/
int SetupCallbacks(void) {
		  int thid = 0;

		  thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);   //als er foutenzijn dan  kan het hieraanliggen
		  if(thid >= 0) {
					sceKernelStartThread(thid, 0, 0);
		  }

		  return thid;
} 



/*!
*
*	Main function
*
*/
int main (void)
{
	scePowerSetClockFrequency(333, 333, 166);	// set the speed
	pspDebugScreenInit();						// init the debug screen functions
	SetupCallbacks();							// setup the callbacks
	//MusicEngine musicengine;
	Init(1);
	Load("INXS_-_Pretty_Vegas.mp3");
	Play();
	char t[13];
	int tmp = 0;
	while(tmp<10) {
		sceKernelDelayThread(1*1000*1000);
		tmp++;
    	GetTimeString(t);
    	printf("Time %d: %s\n", tmp, t);

	};

	Pause();
	sceKernelDelayThread(4*1000*1000);
	Pause();

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

	End();
	sceKernelExitGame();
	return 0;
}
