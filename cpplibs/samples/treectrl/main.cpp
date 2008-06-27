/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * Copyright (c) 2005 Jesper Svennevid
 */

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <pspgu.h>
#include <pspgum.h>

#include "TreeCtrl.h"
#include "TreeItem.h"

#include <libpsp2d/Controller.h>

using namespace PSP2D;
using namespace Imaging;

PSP_MODULE_INFO("TreeCtrl Sample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

static int exitRequest = 0;

int running()
{
	return !exitRequest;
}

int exitCallback(int arg1, int arg2, void *common)
{
	exitRequest = 1;
	return 0;
}

int callbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();

	return 0;
}

int setupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

int main(int argc, char* argv[])
{
	setupCallbacks();

	sceGuInit();

        TreeCtrl *tree = new TreeCtrl();
        tree->append(0, new StaticTextTreeItem("Item 1"));
        TreeItem *item = tree->append(0, new StaticTextTreeItem("Item 2"));
        tree->append(item, new StaticTextTreeItem("Item 3"));
        tree->append(item, new StaticTextTreeItem("Item 4"));

        while (true)
        {
           Controller ctrl;

           if (ctrl.circle())
              break;

           if (ctrl.right())
              tree->collapse(false);

           if (ctrl.left())
              tree->collapse(true);

           if (ctrl.down())
           {
              tree->selectNext();

              sceKernelDelayThreadCB(100000);
           }

           if (ctrl.up())
           {
              tree->selectPrev();

              sceKernelDelayThreadCB(100000);
           }
        }

        delete tree;

	sceGuTerm();

	sceKernelExitGame();
	return 0;
}
