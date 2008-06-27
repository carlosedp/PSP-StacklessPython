
/**
 * @file Blitter.h
 */

/**********************************************************************

  Created: 08 Nov 2005

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
// $Id: Blitter.h 1547 2005-12-09 12:25:07Z fraca7 $

#ifndef _BLITTER_H
#define _BLITTER_H

#include <imaging/Screen.h>
#include <imaging/Image.h>

namespace Imaging
{
    /**
     * Visitor pattern. This is used to identify the exact type of the
     * source drawable.
     */

    class DrawableVisitor
    {
      public:
       virtual ~DrawableVisitor() {};

       /**
        * Called by Screen::accept
        */

       virtual void visitScreen(Screen*) = 0;

       /**
        * Called by Image::accept
        */

       virtual void visitImage(Image*) = 0;
    };

    /**
     * Blits a drawable to  the screen. Actually, the only implemented
     * way is Image to Screen.  Screen to Screen doesn't make sense to
     * me.
     */

    class ScreenBlitter : public DrawableVisitor
    {
      public:
       /**
        * Constructor.
        *
        * @param scr: The Screen instance to blit to
        * @param sx: Source X coordinate
        * @param sy: Source Y coordinate
        * @param w: Width of rectangle to blit
        * @param h: Height of rectangle to blit
        * @param dx: Destination X coordinate
        * @param dy: Destination Y coordinate
        * @param blend: Wether to take the source alpha channel into account
        * @param scaleX: Scale factor in X
        * @param scaleY: Scale factor in Y
        */

       ScreenBlitter(Screen *scr,
                     u16 sx, u16 sy, u16 w, u16 h,
                     u16 dx, u16 dy,
                     bool blend = false,
                     double scaleX = 1.0, double scaleY = 1.0);

       /**
        * Called by Screen::accept
        */

       void visitScreen(Screen*);

       /**
        * Called by Image::accept
        */

       void visitImage(Image*);

      protected:
       Screen *_scr;
       u16 _sx, _sy, _w, _h, _dx, _dy;
       double _scaleX, _scaleY;
       bool _blend;
    };

    /**
     * Blits a drawable  to an image. Blitting the  screen to an image
     * isn't implemented yet.
     */

    class ImageBlitter : public DrawableVisitor
    {
      public:
       /**
        * Constructor.
        *
        * @param img: The Image instance to blit to
        * @param sx: Source X coordinate
        * @param sy: Source Y coordinate
        * @param w: Width of rectangle to blit
        * @param h: Height of rectangle to blit
        * @param dx: Destination X coordinate
        * @param dy: Destination Y coordinate
        * @param blend: Wether to take the source alpha channel into account
        * @param scaleX: Scale factor in X
        * @param scaleY: Scale factor in Y
        */

       ImageBlitter(Image *img,
                    u16 sx, u16 sy, u16 w, u16 h,
                    u16 dx, u16 dy,
                    bool blend = false,
                    double scaleX = 1.0000, double scaleY = 1.0);

       /**
        * Called by Screen::accept
        */

       void visitScreen(Screen*);

       /**
        * Called by Image::accept
        */

       void visitImage(Image*);

      protected:
       Image *_img;
       u16 _sx, _sy, _w, _h, _dx, _dy;
       double _scaleX, _scaleY;
       bool _blend;
    };
};

#endif /* _BLITTER_H */
