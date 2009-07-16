// Hello World - My First App for the PSP

/*
          This program was created by (Your Name Here) on (Date Here)
          It is a simple "Hello World" Application.
*/
#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspaudio.h>
#include <pspaudiolib.h>
#include <psppower.h>
#include <time.h>
#include "mp3player.h"


PSP_MODULE_INFO("Hello World", 0, 1, 1);

#define printf pspDebugScreenPrintf

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
          pspAudioEnd();
          sceKernelExitGame();
          return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
          int cbid;

          cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
          sceKernelRegisterExitCallback(cbid);

          sceKernelSleepThreadCB();

          return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
          int thid = 0;

          thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
          if(thid >= 0) {
                    sceKernelStartThread(thid, 0, 0);
          }

          return thid;
}

int main() {
    pspDebugScreenInit();
    SetupCallbacks();
    scePowerSetClockFrequency(333, 333, 166);
    
    pspAudioInit();
    SceCtrlData pad;
    int i;

    tzset();
    printf("Hello World\n");
    time_t now;
    now = time(NULL);
    printf("Started %s\n", ctime(&now));

    MP3_Init(1);
    MP3_Load("test.mp3");
    MP3_Play();
    while(1) {
            sceCtrlReadBufferPositive(&pad, 1);
            if(pad.Buttons & PSP_CTRL_CROSS) {
                      break;
            } else if(pad.Buttons & PSP_CTRL_CIRCLE) {
                      MP3_Pause();
                      for(i=0; i<10; i++) {
                                sceDisplayWaitVblankStart();
                      }
            }

            if (MP3_EndOfStream() == 1) {
                      MP3_Stop();
            }
    }
    MP3_Stop();
    MP3_FreeTune();
    sceKernelSleepThread();
    return 0;
    }



