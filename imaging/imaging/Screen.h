
/**
 * @file Screen.h
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
// $Id: Screen.h 1547 2005-12-09 12:25:07Z fraca7 $

#ifndef _SCREEN_H
#define _SCREEN_H

#include <imaging/Drawable.h>

#define PSP_LINE_SIZE 512
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

#define FRAMEBUFFER_SIZE (PSP_LINE_SIZE*SCREEN_HEIGHT*4)

namespace Imaging
{
    /**
     * This class represents the physical screen of the PSP. This is a
     * Singleton.
     */

    class Screen : public Drawable
    {
      public:
       // Inherit documentation from Drawable

       void blit(Drawable* drw, u16 sx, u16 sy, u16 w, u16 h, u16 dx, u16 dy, bool blend = false,
                 double scaleX = 1.000, double scaleY = 1.0);
       void clear(u32 color);
       void putPixel(u32 color, u16 x, u16 y);
       u32 getPixel(u16 x, u16 y) const;
       void drawLine(u16 x0, u16 y0, u16 x1, u16 y1, u32 color);
       void fillRect(u32 color, u16 x, u16 y, u16 w, u16 h);
       void printText(u16 x, u16 y, const std::string& text, u32 color);

       void saveToFile(const std::string& filename, ImageType type = IMG_PNG);

       /**
        * Swaps the back and front buffers.
        */

       void flip();

       /**
        * Part  of the Singleton  pattern, get  the instance  for this
        * class
        */

       static Screen* getInstance();

       /**
        * Initializes  the  graphics   system.  This  must  be  called
        * once.  It is  called by  the constructor  if needed,  so you
        * shouldn't have to call it youserves.
        */

       static void init(void);

       /**
        * This should be protected.
        */

       static u32* getList(void);

       /**
        * Part of the Visitor pattern. See DrawableVisitor.
        */

       void accept(DrawableVisitor*);

       /**
        * I'm not sure this should be public.
        */

       static u32* getVramDrawBuffer(void);

      protected:
       static int _current;
       static bool _initialized;

       void _saveToPNG(const std::string&);
       void _saveToJPEG(const std::string&);

      private:
       Screen();
       virtual ~Screen();

       static Screen* _instance;
    };
};

// Inline methods
#include <imaging/Screen.il>

#endif /* _SCREEN_H */
