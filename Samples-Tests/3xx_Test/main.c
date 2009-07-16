#include <pspsdk.h>
#include <pspkernel.h>
#include <stdio.h>
#include <string.h>

PSP_MODULE_INFO("Test_3xx", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

#define printf pspDebugScreenPrintf

int main()
{
   pspDebugScreenInit();

   printf("Dumping...\n");

   printf("Done. Exiting in 2 seconds\n");

   sceKernelDelayThread(2*1000*1000);
   sceKernelExitGame();

   return 0;
}
