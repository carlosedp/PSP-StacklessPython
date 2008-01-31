
/**
 * @file Music.h
 */

/**********************************************************************

  Created: 13 Nov 2005

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
// $Id: Music.h 1481 2005-11-26 11:33:45Z fraca7 $

#ifndef _MUSIC_H
#define _MUSIC_H

#include <mikmod.h>

#include <string>

namespace PSPSND
{
    /**
     * Music
     */

    class Music
    {
      public:
       /**
        * Constructor
        *
        * @param  filename  -  A  music  file  (formats  supported  by
        * mikmodlib)
        * @param loop - Whether to loop
        */

       Music(const std::string& filename, bool loop = false, unsigned char maxchan = 128);

       ~Music();

       /**
        * Starts playing
        */

       void start(void);

       /**
        * Stops playing. After calling this, you can't use this object
        * any more.
        */

       void stop(void);

       /**
        * Sets the music volume
        */

       static void setVolume(unsigned char);

       /**
        * MikMod initialization. Call this before instantiating Music or Sound.
        */

       static void init(void);

       /**
        * Mikmod finalization.
        */

       static void uninit(void);

      protected:
       UNIMOD *_mf;
    };
};

#endif /* _MUSIC_H */
