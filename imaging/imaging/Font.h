
/**
 * @file Font.h
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
// $Id: Font.h 1481 2005-11-26 11:33:45Z fraca7 $

#ifndef _FONT_H
#define _FONT_H

#include <imaging/Image.h>
#include <imaging/Drawable.h>

#include <string>
#include <vector>

namespace Imaging
{
    /**
     * Helper struct used by Font
     */

    typedef struct
    {
          Image *letter;
          unsigned int width;
    } ST_LETTER;

    /**
     * SFont-like variable width font
     */

    class Font
    {
      public:
       /**
        * Constructor
        *
        * @param filename - A PNG file
        */

       Font(const std::string& filename);

       ~Font();

       /**
        * Computes a text's width, in pixels.
        */

       u32 getTextWidth(const std::string& text);

       /**
        * Computes a  text's height,  in pixels. This  actually always
        * return the same value.
        */

       u16 getTextHeight(const std::string& text);

       /**
        * Draws some text from this font to a Drawable.
        *
        * @param drw - Drawable to draw to
        * @param text - Text to draw (encoded in ISO-8859-1)
        * @param x - X coordinate to draw to
        * @param y - Y coordinate to draw to
        */

       void drawText(Drawable *drw, const std::string& text, u16 x, u16 y);

      protected:
       std::vector<ST_LETTER*> _letters;
       u16 _height;

       int _getOffset(unsigned char);
    };
};

// Inline methods
#include <imaging/Font.il>

#endif /* _FONT_H */
