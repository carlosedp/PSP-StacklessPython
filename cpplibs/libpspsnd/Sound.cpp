
/**
 * @file Sound.cpp
 * @brief Implementation of class Sound
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
// $Id: Sound.cpp 24 2006-04-12 15:14:36Z fraca7 $

#include "Sound.h"
#include "Exception.h"

#include <sstream>

using namespace PSPSND;
using namespace std;

extern char *_mm_errmsg[];
extern int _mm_errno;

Sound::Sound(const string& filename)
{
    _sf = WAV_LoadFN((char*)filename.c_str()); // ugh...

    if (!_sf)
    {
       stringstream s;

       s << "Could not load sound file: " << _mm_errmsg[_mm_errno];

       throw SoundException(s.str());
    }
}

Sound::~Sound()
{
    WAV_Free(_sf);
}

void Sound::start()
{
    int voice;

    voice = MikMod_PlaySample(_sf, 0, 0);
    Voice_SetPanning(voice, 127);
}

void Sound::setVolume(unsigned char volume)
{
    md_sndfxvolume = volume;
}

static const char* _rcsid_Sound __attribute__((unused)) = "$Id: Sound.cpp 24 2006-04-12 15:14:36Z fraca7 $";
