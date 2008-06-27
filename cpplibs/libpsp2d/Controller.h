
/**
 * @file Controller.h
 */

/**********************************************************************

  Created: 11 Nov 2005

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
// $Id: Controller.h 1481 2005-11-26 11:33:45Z fraca7 $

#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#ifndef LINUX
#include <pspctrl.h>
#endif

namespace PSP2D
{
    /**
     * This class abstracts the state of the PSP buttons and pads at a
     * given time.
     */
    class Controller
    {
      public:
       /**
        * Constructor.
        *
        * Note that the pad's state is read upon instantiation.
        */
       Controller();

       ~Controller();

       /**
        * Analog pad in X direction.  Returns a value between -127 and
        * 128. Note that  values less  than 6 mean  the analog  pad is
        * probably in its dead zone, you should ignore it.
        */

       int analogX();

       /**
        * Analog pad in Y direction.  Returns a value between -127 and
        * 128. Note that  values less  than 6 mean  the analog  pad is
        * probably in its dead zone, you should ignore it.
        */

       int analogY();

       /**
        * State of the square button.
        */

       bool square();

       /**
        * State of the triangle button.
        */

       bool triangle();

       /**
        * State of the cross button.
        */

       bool cross();

       /**
        * State of the circle button.
        */

       bool circle();

       /**
        * State of the up button of the digital pad.
        */

       bool up();

       /**
        * State of the down button of the digital pad.
        */

       bool down();

       /**
        * State of the left button of the digital pad.
        */

       bool left();

       /**
        * State of the right button of the digital pad.
        */

       bool right();

       /**
        * State of the left trigger.
        */

       bool l();

       /**
        * State of the right trigger.
        */

       bool r();

       /**
        * State of the start button.
        */

       bool start();

       /**
        * State of the select button.
        */

       bool select();

       /**
        * Comparison
        */

       bool operator==(const Controller&);

       /**
        * Affectation
        */

       Controller& operator=(const Controller&);

      protected:
       SceCtrlData _data;

       static bool _initialized;
    };
};

// Inline methods
#include <libpsp2d/Controller.il>

#endif /* _CONTROLLER_H */
