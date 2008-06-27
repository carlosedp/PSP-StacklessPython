
/**
 * @file Drawable.h
 */

/**********************************************************************

  Created: 07 Nov 2005

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
// $Id: Drawable.h 1547 2005-12-09 12:25:07Z fraca7 $

#ifndef _DRAWABLE_H
#define _DRAWABLE_H

#include <imaging/ltypes.h>

#include <string>

namespace Imaging
{
    class DrawableVisitor;

    /**
     * Image types
     */

    typedef enum
    {
       IMG_PNG,
       IMG_JPEG
    } ImageType;

    /**
     * Abstract base  class for stuff  you can draw to  (actually only
     * Screen and Image).
     */

    class Drawable
    {
      public:
       virtual ~Drawable();

       /**
        * Copies a  rectangle from another drawable.  The operation is
        * actually performed in a visitor; see DrawableVisitor.
        *
        * @param drw - The source drawable
        * @param sx - left position of source rectangle
        * @param sy - top position of source rectangle
        * @param w - width of source rectangle
        * @param h - height of source rectangle
        * @param dx - left position of destination rectangle
        * @param dy - top position of destination rectangle
        * @param blend - wether to take alpha channel into account
        * @param scaleX - Scale factor in X
        * @param scaleY - Scale factor in Y
        */

       virtual void blit(Drawable* drw,
                         u16 sx, u16 sy, u16 w, u16 h,
                         u16 dx, u16 dy,
                         bool blend = false,
                         double scaleX = 1.0, double scaleY = 1.0) = 0;

       /**
        * Fills the drawable with a solid color.
        */

       virtual void clear(u32 color) = 0;

       /**
        * Sets a pixel to a specified color.
        */

       virtual void putPixel(u32 color, u16 x, u16 y) = 0;

       /**
        * Gets the color of a pixel.
        */

       virtual u32 getPixel(u16 x, u16 y) const = 0;

       /**
        * Draws a line.
        */

       virtual void drawLine(u16 x0, u16 y0, u16 x1, u16 y1, u32 color) = 0;

       /**
        * Fills a rectangle.
        */

       virtual void fillRect(u32 color, u16 x, u16 y, u16 w, u16 h) = 0;

       /**
        * Prints some text
        */

       virtual void printText(u16 x, u16 y, const std::string& text, u32 color) = 0;

       /**
        * Saves this drawable to a PNG or JPEG file.
        */

       virtual void saveToFile(const std::string& filename, ImageType type = IMG_PNG) = 0;

       /**
        * Part of the Visitor pattern. See DrawableVisitor.
        */

       virtual void accept(DrawableVisitor*) = 0;
    };
};

#endif /* _DRAWABLE_H */
