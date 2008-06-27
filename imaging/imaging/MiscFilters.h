
/**
 * @file MiscFilters.h
 */

/**********************************************************************

  Created: 09 Dec 2007

    Copyright (C) 2007 Frank Buss, Jérôme Laheurte

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

#ifndef _MISCFILTERS_H
#define _MISCFILTERS_H

#include <imaging/ltypes.h>
#include <imaging/Filter.h>

namespace Imaging
{
    class Image;
    class Callback;

    /**
     * This filter scales down an image, with linear smoothing.
     */

    class ScaleFilter : public Filter
    {
      public:
       /**
        * Constructor.
        *
        * @param w Width of resulting Image
        * @param h Height of resulting Image
        */

       ScaleFilter(u16 w, u16 h);
       virtual ~ScaleFilter();

       Image* apply(const Image*, Callback* = 0) const;

      protected:
       u16 _w, _h;
    };

    /**
     * This filter extracts a subimage.
     */

    class SubFilter : public Filter
    {
      public:
       /**
        * Constructor.
        *
        * @param x Left coordinate of cropped Image
        * @param y Top coordinate of cropped Image
        * @param w Width of cropped Image
        * @param h Height of cropped Image
        */

       SubFilter(u16 x, u16 y, u16 w, u16 h);
       virtual ~SubFilter();

       Image* apply(const Image*, Callback* = 0) const;

      protected:
       u16 _x, _y, _w, _h;
    };

    /**
     * Normalized  norm  of X  and  Y  Sobel  operator. The  image  is
     * supposed to be gray-level (only the R channel is used).
     */

    class SobelFilter : public Filter
    {
      public:
       virtual ~SobelFilter();

       Image* apply(const Image*, Callback* = 0) const;
    };

    /**
     * Returns the  connected component  to which belongs  a specified
     * point, as B/W image (white for points which belong).
     */

    class ConnectedFilter : public Filter
    {
      public:
       /**
        * Constructor.
        *
        * @param x X coordinate of the point
        * @param y Y coordinate of the point
        */

       ConnectedFilter(u16 x, u16 y);
       virtual ~ConnectedFilter();

       Image* apply(const Image*, Callback* = 0) const;

      protected:
       u16 _x, _y;
    };

    /**
     * Absolute difference. The image is supposed to be gray.
     */

    class DifferenceFilter : public Filter
    {
      public:
       /**
        * Constructor.
        *
        * @param pImg Image to substract
        */

       DifferenceFilter(Image* pImg);
       virtual ~DifferenceFilter();

       Image* apply(const Image*, Callback* = 0) const;

      protected:
       Image *m_pImg;
    };

    /**
     * Absolute difference. The image is supposed to be gray.
     */

    class DifferenceInPlaceFilter : public InPlaceFilter
    {
      public:
       /**
        * Constructor.
        *
        * @param pImg Image to substract
        */

       DifferenceInPlaceFilter(Image* pImg);
       virtual ~DifferenceInPlaceFilter();

       void apply(Image*, Callback* = 0) const;

      protected:
       Image *m_pImg;
    };
};

#endif /* _MISCFILTERS_H */
