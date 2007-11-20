
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspsdk.h>
#include <pspiofilemgr.h>
#include <psppower.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef WITH_PSP2D
#include <pspgu.h>
#endif

#ifdef WITH_OSLIB
#include <oslib/oslib.h>
#endif

#include "Python.h"

#define SYNCSTDOUT

PSP_MODULE_INFO("Python", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(0);

volatile int pspInterruptOccurred = 0;

static char* save_argv0;

/* Output handler */

static SceUID fd_stdout = 0;

static int my_stdout(const char *data, int len)
{
    if (!fd_stdout)
       fd_stdout = sceIoOpen("ms0:/pytrace.txt", PSP_O_WRONLY|PSP_O_CREAT
#ifdef SYNCSTDOUT
                             |PSP_O_APPEND
#else
                             |PSP_O_TRUNC
#endif
                             , 0777);

    if (fd_stdout)
    {
       int w = sceIoWrite(fd_stdout, data, len);
#ifdef SYNCSTDOUT
       sceIoClose(fd_stdout);
       fd_stdout = 0;
#endif
       return w;
    }
    else
       return len;
}

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
#ifdef WITH_OSL
    osl_quit = 1;
#else
    pspInterruptOccurred = 1;
#endif

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

    thid = sceKernelCreateThread("update_thread", CallbackThread,
                                 0x11, 0xFA0, 0, 0);
    if(thid >= 0)
    {
       sceKernelStartThread(thid, 0, 0);
    }

    return thid;
}

int main_thread(SceSize args, void *argp)
{
    FILE *fp;
    int argc = 1;
    char *argv[] = {save_argv0};

    /*
    char *path;
    int sz;
    */

    SetupCallbacks();

    if (pspSdkInetInit())
       return 1;

#ifdef WITH_PSP2D
    sceGuInit();
#endif

    /*

    path = strdup(save_argv0);
    sz = strlen(path);

    if (sz > 0)
    {
       if (path[sz - 1] == '/')
       {
          chdir(path);
       }
       else
       {
          int k = sz - 1;

          while ((k >= 0) && (path[k] != '/'))
          {
             --k;
          }

          if (k >= 0)
          {
             path[k + 1] = 0;

             chdir(path);
          }
       }
    }

    free(path);

    */

    PySys_ResetWarnOptions();
    Py_SetProgramName("python");

    Py_Initialize();

    PySys_SetArgv(argc, argv);

    fp = fopen("script.py", "r");
    if (fp)
    {
       PyRun_SimpleFile(fp, "script.py");
       fclose(fp);
    }
    else
    {
       printf("Error - could not open script.py\n");
    }

    Py_Finalize();

#ifdef WITH_PSP2D
    sceGuTerm();
#endif

    pspSdkInetTerm();

    if (fd_stdout)
       sceIoClose(fd_stdout);

    sceKernelExitGame();

    return 0;
}

int main(int argc, char *argv[])
{
#ifndef WITH_OSLIB
    SceUID thid;
#endif
    tzset();
    time_t now;

    save_argv0 = strdup(argv[0]);

#ifdef WITH_OSLIB
    oslInit(1);
#endif

    if(pspSdkLoadInetModules() < 0)
       return 1;

    pspDebugInstallStdoutHandler(my_stdout);
    pspDebugInstallStderrHandler(my_stdout);

    now = time(NULL);
    printf("Python-PSP started %s\n", ctime(&now));
    printf("Free memory: %d\n", sceKernelTotalFreeMemSize());
    printf("Search path: %s\n", Py_GetPath());
    printf("\n");

    scePowerSetClockFrequency(333, 333, 166);

#ifdef WITH_OSLIB
    main_thread(0, NULL);
#else
    thid = sceKernelCreateThread("python_thread", main_thread, 0x18, 0x10000,
                                 PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU, NULL);
    sceKernelStartThread(thid, 0, NULL);

    sceKernelExitDeleteThread(0);
#endif

    return 0;
}
