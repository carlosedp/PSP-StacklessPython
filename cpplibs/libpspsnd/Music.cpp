
/**
 * @file Music.cpp
 * @brief Implementation of class Music
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
// $Id: Music.cpp 22 2006-04-08 08:45:18Z fraca7 $

#include "Music.h"
#include "Exception.h"

#include <sstream>

using namespace PSPSND;
using namespace std;

// MikMod globals

extern UWORD md_mode;
extern UBYTE md_musicvolume;
extern UBYTE md_sndfxvolume;
extern UBYTE md_reverb;
extern UBYTE md_pansep;

extern char *_mm_errmsg[];
extern int _mm_errno;

Music::Music(const string& filename, bool loop, unsigned char maxchan)
{
    _mf = MikMod_LoadSong((char*)filename.c_str(), maxchan);

    if (!_mf)
    {
       stringstream s;

       s << "Could not load music file: " << _mm_errmsg[_mm_errno];

       throw SoundException(s.str());
    }

    _mf->loop = (int)loop;
}

Music::~Music()
{
    if (_mf)
       MikMod_FreeSong(_mf);
}

void Music::start()
{
    if (!_mf)
       return;

    Player_Start(_mf);
}

void Music::stop()
{
    if (!_mf)
       return;

    Player_Stop();
    MikMod_FreeSong(_mf);
    _mf = NULL;
}

void Music::setVolume(unsigned char volume)
{
    md_musicvolume = volume;
}

void Music::init()
{
    MikMod_RegisterAllLoaders();
    MikMod_RegisterAllDrivers();
    md_mode = DMODE_16BITS|DMODE_STEREO|DMODE_SOFT_SNDFX|DMODE_SOFT_MUSIC;
    md_musicvolume = 128;
    md_sndfxvolume = 128;
    md_reverb = 0;
    md_pansep = 128;
    MikMod_Init();
    MikMod_SetNumVoices(128, 32);

    MikMod_EnableOutput();
}

void Music::uninit()
{
    MikMod_Exit();
}

static const char* _rcsid_Music __attribute__((unused)) = "$Id: Music.cpp 22 2006-04-08 08:45:18Z fraca7 $";
