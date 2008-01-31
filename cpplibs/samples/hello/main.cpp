/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * Copyright (c) 2005 Jesper Svennevid
 */

#include <pspkernel.h>

PSP_MODULE_INFO("cpplibs demo", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#include <libpsp2d/Screen.h>
#include <libpsp2d/Image.h>
#include <libpsp2d/Controller.h>

#include <libpspsnd/Music.h>

#include <math.h>

#include <sstream>

#include "menu.h"

using namespace PSP2D;
using namespace PSPSND;

using namespace std;

int SetupCallbacks();

int main(int argc, char* argv[])
{
    SetupCallbacks();

    string msg("Hello, world!");

    Screen *screen = Screen::getInstance();

    Image background("background.png");
    Font font("font.png");

    // Uncomment  this and copy  stranglehold.xm to  your PSP  to have
    // music

    /*
    Music::init();
    Music music("stranglehold.xm", true);
    music.start();
    */

    vector<int> widths;
    int totalw = 0;

    for (string::iterator k = msg.begin();
         k != msg.end();
         ++k)
    {
       stringstream s;
       s << *k;

       widths.push_back(font.getTextWidth(s.str()));
       totalw += font.getTextWidth(s.str());
    }

    u16 x, y;

    y = (272 - font.getTextHeight(msg)) / 2;
    x = (480 - totalw) / 2;

    float angle = 0.0;

    Menu menu(10, 10);

    FloatMenuItem *amplitude = new FloatMenuItem("Amplitude", 20.0, 0.5);
    FloatMenuItem *step = new FloatMenuItem("Step", 0.2, 0.02);
    FloatMenuItem *slope = new FloatMenuItem("Slope", 0.2, 0.02);

    menu.addItem(amplitude);
    menu.addItem(step);
    menu.addItem(slope);

    menu.realize(&font);

    while (true)
    {
       screen->blit(&background, 0, 0, 480, 272, 0, 0);
       menu.draw(&font, screen);

       int dx = 0, idx = 0;
       for (string::iterator k = msg.begin();
            k != msg.end();
            ++k, ++idx)
       {
          stringstream s;
          s << *k;

          font.drawText(screen,
                        s.str(),
                        x + dx,
                        (int)(y + amplitude->getValue() * sinf(angle + slope->getValue() * idx)));

          dx += widths[idx];
       }

       screen->flip();

       angle += step->getValue();

       Controller pad;
       if (pad.circle())
          break;

       menu.update(&pad);
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
