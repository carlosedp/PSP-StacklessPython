#include <pspsdk.h>
#include <pspkernel.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <psputility_sysparam.h>

PSP_MODULE_INFO("Test_3xx_Date", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1); /* 18MB */

#define printf pspDebugScreenPrintf
#define time sceKernelLibcTime

void initTimezone()
{
   int tzOffset = 0;
   sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_TIMEZONE, &tzOffset);
   int tzOffsetAbs = tzOffset < 0 ? -tzOffset : tzOffset;
   int hours = tzOffsetAbs / 60;
   int minutes = tzOffsetAbs - hours * 60;
   static char tz[10];
   sprintf(tz, "GMT%s%02i:%02i", tzOffset < 0 ? "+" : "-", hours, minutes);
   setenv("TZ", tz, 1);
   tzset();
}

int main()
{
    pspDebugScreenInit();
    initTimezone();
    time_t now;

    time(&now);

    printf("Starting %s\n", ctime(&now));
    printf("Done. Exiting in 1 seconds\n");

    sceKernelDelayThread(5*1000*1000);
    sceKernelExitGame();

    return 0;
}
