
/**
 * @file Image.h
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
// $Id: Image.h 1547 2005-12-09 12:25:07Z fraca7 $

#ifndef _IMAGE_H
#define _IMAGE_H

#include <string>
#include <list>

#include <imaging/Drawable.h>
#include <imaging/Exception.h>
#include <imaging/Callback.h>

#define MAX(x, y) ((x) < (y) ? (y) : (x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/**
 * Simple Rectangle class.
 */

class Rectangle
{
   public:
    /**
     * Constructor.
     *
     * @param x1 Left coordinate
     * @param y1 Top coordinate
     * @param x2 Right coordinate
     * @param y2 Bottom coordinate
     */

    Rectangle(int x1, int y1, int x2, int y2)
       : _x1(x1), _y1(y1), _x2(x2), _y2(y2)
    {
    }

    /**
     * @return true if the specified point lies in the Rectangle
     */

    bool isIn(int x, int y) const
    {
       return ((x >= _x1) && (y >= _y1) && (x <= _x2) && (y <= _y2));
    }

    /**
     * @return true if the specified Rectangle intersects this one.
     */

    bool intersects(const Rectangle& r) const
    {
       if (_y2 < r._y1)
          return false;
       if (_y1 > r._y2)
          return false;
       if (_x2 < r._x1)
          return false;
       if (_x1 > r._x2)
          return false;
       return true;
    }

    /**
     * Merges this rectangle with another.
     */

    void union_(const Rectangle& r)
    {
       _x1 = MIN(_x1, r._x1);
       _y1 = MIN(_y1, r._y1);
       _x2 = MAX(_x2, r._x2);
       _y2 = MAX(_y2, r._y2);
    }

    u16 _x1, _y1, _x2, _y2;
};

namespace Imaging
{
    /**
     * Generic exception that can be throwed by the Image class.
     */

    class ImageException : public Exception
    {
      public:
       /**
        * @param msg An informative error message
        */

       ImageException(const std::string& msg) : Exception(msg) {};
    };

    /**
     * Some I/O error
     */

    class ImageIOException : public ImageException
    {
      public:
       ImageIOException(const std::string& msg) : ImageException(msg) {};
    };

    /**
     * Error from libpng
     */

    class ImagePNGException : public ImageException
    {
      public:
       ImagePNGException(const std::string& msg) : ImageException(msg) {};
    };

    /**
     * Error from libjpeg
     */

    class ImageJPEGException : public ImageException
    {
      public:
       ImageJPEGException(const std::string& msg) : ImageException(msg) {};
    };

    /**
     * Image class. This represents an in-memory image.
     */

    class Image : public Drawable
    {
      public:
       /**
        * Constructor. Loads the image from a PNG or JPEG file.
        */

       Image(const std::string& filename);

       /**
        * Constructor. Creates an empty image.
        */

       Image(u16 width, u16 height);

       /**
        * Constructor. Copies another image.
        */

       Image(const Image& img);

       virtual ~Image();

       /**
        * Returns the width of the image in pixels.
        */

       u16 getWidth() const;

       /**
        * Returns the height of the image in pixels.
        */

       u16 getHeight() const;

       /**
        * Returns the  width used to store this  image (smallest power
        * of 2 that is >= to the actual width).
        */

       u16 getTextureWidth() const;

       /**
        * Returns the  height used to store this  image (smallest power
        * of 2 that is >= to the actual height).
        */

       u16 getTextureHeight() const;

       /**
        * Returns  a  pointer  to  the  actual bytes.  Should  not  be
        * public. Will be fixed.
        */

       u32* getData() const;

       /**
        * Given an origin, this  will return the bounding rectangle of
        * the 4-connected area computed from the origin (colour-wise).
        */

       void boundingRectangle(u16 x, u16 y, u16& x1, u16& y1, u16& x2, u16& y2);

       /**
        * Returns a list of disjoint bounding rectangles
        */

       std::list<Rectangle> boundingRectangles();

       /**
        * Visitor pattern. See DrawableVisitor.
        */

       void accept(DrawableVisitor*);

#ifdef LINUX
       /**
        * From RGB data. The buffer size is expected to be 3 * w * h.
        */

       void fromData(int, int, unsigned char*);
#endif

       // These will inherit documentation from Drawable

       void blit(Drawable* drw, u16 sx, u16 sy, u16 w, u16 h, u16 dx, u16 dy, bool blend = false,
                 double scaleX = 1.0, double scaleY = 1.0);
       void clear(u32 color);
       void putPixel(u32 color, u16 x, u16 y);
       u32 getPixel(u16 x, u16 y) const;
       void drawLine(u16 x0, u16 y0, u16 x1, u16 y1, u32 color);
       void fillRect(u32 color, u16 x, u16 y, u16 w, u16 h);
       void printText(u16 x, u16 y, const std::string& text, u32 color);

       void saveToFile(const std::string& filename, ImageType type = IMG_PNG);

      protected:
       u16 _width, _height, _textureWidth, _textureHeight;
       u32 *_data;

       void _loadFromPNG(const std::string&);
       void _loadFromJPEG(const std::string&);

       void _saveToPNG(const std::string&);
       void _saveToJPEG(const std::string&);
    };
};

// Inline methods
#include <imaging/Image.il>

#endif /* _IMAGE_H */
