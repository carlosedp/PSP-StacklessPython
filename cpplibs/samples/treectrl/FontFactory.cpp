
/**
 * @file FontFactory.cpp
 * @brief Implementation of class FontFactory
 */

/**********************************************************************

  Created: 13 May 2008

    Copyright (C) 2008 Frank Buss, Jérôme Laheurte

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
// $Id$

#include "FontFactory.h"

using namespace std;
using namespace Imaging;

FontFactory* FontFactory::m_pInstance = 0;

FontFactory::FontFactory()
{
}

FontFactory* FontFactory::getInstance()
{
    if (!m_pInstance)
       m_pInstance = new FontFactory();

    return m_pInstance;
}

Font* FontFactory::getFont(const string& name)
{
    if (m_Map.find(name) == m_Map.end())
    {
       string filename(name + ".png");

       m_Map.insert(pair<string, Font*>(name, new Font(filename)));
    }

    return m_Map[name];
}

static const char* _rcsid_FontFactory __attribute__((unused)) = "$Id$";
