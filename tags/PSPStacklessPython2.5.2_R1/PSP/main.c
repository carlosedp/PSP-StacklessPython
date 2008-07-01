#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspsdk.h>
#include <pspiofilemgr.h>
#include <psppower.h>
#include <psputility_netmodules.h>
#include <psputility_sysparam.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef WITH_SSL
/* Include OpenSSL header files */
#include "openssl/rsa.h"
#include "openssl/crypto.h"
#include "openssl/x509.h"
#include "openssl/pem.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/rand.h"
#endif

#ifdef WITH_PSP2D
#include <pspgu.h>
#endif

#ifdef WITH_OSLIB
#include <oslib/oslib.h>
#endif

#include "Python.h"

#define VERS    2
#define REVS    52
PSP_MODULE_INFO("StacklessPython", 0, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU);
//PSP_HEAP_SIZE_KB(18*1024); /* Use 18Mb */
PSP_HEAP_SIZE_KB(-1024);    /* Leave 1024k to OS, rest to app */
PSP_MAIN_THREAD_STACK_SIZE_KB(64);

volatile int pspInterruptOccurred = 0;

/*!
*	Exit callback function
*/
int exit_callback(int arg1, int arg2, void *common) {
#ifdef WITH_OSL
    osl_quit = 1;
#else
    pspInterruptOccurred = 1;
#endif
    fclose(stdout);
    fclose(stderr);
    sceKernelExitGame();
    return 0;
}

/*!
*	Callback thread function
*/
int CallbackThread(SceSize args, void *argp) {
    int cbid;

    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);

    sceKernelSleepThreadCB();

    return 0;
}

/*!
*	Sets up the callback thread and
*	returns its thread id function
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
*	Inits timezone based on PSP settings
*/


void initTimezone() {
    int tzOffset = 0;
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_TIMEZONE, &tzOffset);
    int tzOffsetAbs = tzOffset < 0 ? -tzOffset : tzOffset;
    int hours = tzOffsetAbs / 60;
    int minutes = tzOffsetAbs - hours * 60;
    int pspDaylight = 0;
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_DAYLIGHTSAVINGS, &pspDaylight);
    static char tz[18];
    sprintf(tz, "GMT%s%02i:%02i%s", tzOffset < 0 ? "+" : "-", hours, minutes, pspDaylight ? " DST" : "");
    setenv("TZ", tz, 1);
    tzset();
}

int main(int argc, char *argv[]) {
    SetupCallbacks();
    stdout = fopen("pytrace.txt", "a+");
    stderr = stdout;
    setenv("PYTHONPATH", "python.zip", 1);

    initTimezone();
    time_t now;
    sceKernelLibcTime(&now);

    if (sceUtilityLoadNetModule(1) < 0)
        return 1;
    if(sceUtilityLoadNetModule(3) < 0)
        return 1;
    if(pspSdkInetInit() != 0)
        return 1;

#ifdef WITH_PSP2D
    sceGuInit();
#endif
#ifdef WITH_OSLIB
    oslInit(1);
#endif

#ifdef WITH_SSL
    /* Start the random number generator */
    time_t start_time;
    start_time = time(NULL);
    RAND_seed((unsigned char *)&start_time, sizeof(time_t));
#endif

    char path[1024];
    printf("-------------- Python-PSP started %s\n", ctime(&now));
    printf("Free memory: %d\n", sceKernelTotalFreeMemSize());
    printf("PyPSP running in: %s\n", getcwd(path, sizeof(path)));
    printf("Search path: %s\n", Py_GetPath());
    printf("\n");

    //scePowerSetClockFrequency(333, 333, 166);
    FILE *fp;
    PySys_ResetWarnOptions();
    Py_SetProgramName("python");

    Py_Initialize();
    PySys_SetArgv(argc, argv);
    
    fp = fopen("script.py", "r");
    if (fp) {
        PyRun_SimpleFile(fp, "script.py");
        fclose(fp);
    } else {
        printf("Error - could not open script.py\n");
    }

    Py_Finalize();

#ifdef WITH_PSP2D
    sceGuTerm();
#endif
    //pspSdkInetTerm();
#ifdef WITH_OSL
    osl_quit = 1;
#else
    pspInterruptOccurred = 1;
#endif
    printf("--------------------------------------------------------------------------------\n");
    fclose(stdout);
    fclose(stderr);
    sceKernelExitGame();
    return 0;
}
