
/**
 * @file Font.cpp
 * @brief Implementation of class Font
 */

/**********************************************************************

  Created: 12 Nov 2005

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
// $Id: Font.cpp 50 2007-01-27 10:08:21Z fraca7 $

#include "Font.h"

using namespace PSP2D;
using namespace std;

Font::Font(const string& filename)
{
    Image *img = new Image(filename);
    unsigned int refcolor = img->getPixel(0, 0) & 0x00FFFFFFU;

    int x = 0;
    while (x < img->getWidth())
    {
       if ((img->getPixel(x, 0) & 0x00FFFFFFU) != refcolor)
       {
          int pos = x;

          while ((x < img->getWidth()) && ((img->getPixel(x, 0) & 0x00FFFFFFU) != refcolor))
          {
             ++x;
          }

          ST_LETTER *lt = new ST_LETTER;
          lt->width = x - pos;
          lt->letter = new Image(lt->width, img->getHeight() - 1);
          lt->letter->blit(img, pos, 1, lt->width, img->getHeight() - 1, 0, 0);

          _letters.push_back(lt);
       }

       ++x;
    }

    _height = img->getHeight() - 1;

    delete img;
}

Font::~Font()
{
    for (vector<ST_LETTER*>::iterator k = _letters.begin();
         k != _letters.end();
         ++k)
    {
       delete (*k)->letter;
       delete *k;
    }
}

u32 Font::getTextWidth(const string& text)
{
    u32 w = 0;

    for (string::const_iterator k = text.begin(); k != text.end(); ++k)
    {
       int offset = _getOffset((unsigned char)(*k));

       if (offset < 0)
          w += _letters[0]->width;
       else
          w += _letters[offset]->width;
    }

    return w;
}

void Font::drawText(Drawable *drw, const string& text, u16 x, u16 y)
{
    u16 cx = x;

    for (string::const_iterator k = text.begin(); k != text.end(); ++k)
    {
       int offset = _getOffset((unsigned char)(*k));

       if (offset < 0)
          cx += _letters[0]->width;
       else
       {
          drw->blit(_letters[offset]->letter, 0, 0, _letters[offset]->width, _height, cx, y, true);
          cx += _letters[offset]->width;
       }
    }
}

int Font::_getOffset(unsigned char ch)
{
    int offset;

    if (((ch >= 127) && (ch <= 159)) || (ch <= 32))
       offset = -1;
    else if (ch >= 160)
       offset = (int)ch - 160 + 126 - 33;
    else
       offset = (int)ch - 33;

    if (offset >= (int)_letters.size())
       offset = -1;

    return offset;
}

static const char* _rcsid_Font __attribute__((unused)) = "$Id: Font.cpp 50 2007-01-27 10:08:21Z fraca7 $";
