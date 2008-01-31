
/**
 * @file Timer.cpp
 * @brief Implementation of class Timer
 */

/**********************************************************************

  Created: 20 Nov 2005

    Copyright (C) 2005 Frank Buss, Jérôme Laheurte

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The names of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**********************************************************************/
// $Id: Timer.cpp 3 2006-03-12 15:13:39Z fraca7 $

#include "Timer.h"

#include <pspkernel.h>
#include <time.h>

using namespace PSP2D;

Timer::Timer(u32 tmo, TimerListener *listener)
    : _tmo(tmo),
      _listener(listener)
{
}

Timer::~Timer()
{
    delete _listener;
}

void Timer::run()
{
    u32 fired = (u32)clock() * 1000 / CLOCKS_PER_SEC;
    if (!_listener->fire())
       return;

    while (true)
    {
       u32 now = (u32)clock() * 1000 / CLOCKS_PER_SEC;

       if (now > fired + _tmo)
       {
          // Skipped...
          fired = now;
          if (!_listener->fire())
             break;
       }
       else
       {
          // Wait
          sceKernelDelayThreadCB((fired + _tmo - now) * 1000);
          fired = (u32)clock() * 1000 / CLOCKS_PER_SEC;
          if (!_listener->fire())
             break;
       }
    }
}

static const char* _rcsid_Timer __attribute__((unused)) = "$Id: Timer.cpp 3 2006-03-12 15:13:39Z fraca7 $";
