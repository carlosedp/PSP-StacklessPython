
/**
 * @file ColourFilters.h
 */

/**********************************************************************

  Created: 02 Dec 2007

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

#ifndef _COLOURFILTERS_H
#define _COLOURFILTERS_H

#include <imaging/Image.h>
#include <imaging/Filter.h>

namespace Imaging
{
    /**
     * Gray-level filters. Alpha channel is kept.
     */

    class GrayInPlaceFilter : public InPlaceFilter
    {
      public:
       void apply(Image*, Callback* = 0) const;
    };

    class GrayFilter : public Filter
    {
      public:
       Image* apply(const Image*, Callback* = 0) const;
    };

    /**
     * Monochrome filters. Alpha channel is kept.
     */

    class MonochromeInPlaceFilter : public InPlaceFilter
    {
      public:
       /**
        * @param threshold The threshold
        */

       MonochromeInPlaceFilter(unsigned char threshold = 128)
          : _threshold(threshold)
       {
       };

       void apply(Image*, Callback* = 0) const;

      protected:
       unsigned char _threshold;
    };

    class MonochromeFilter : public Filter
    {
      public:
       /**
        * @param threshold The threshold
        */

       MonochromeFilter(unsigned char threshold = 128)
          : _threshold(threshold)
       {
       };

       Image* apply(const Image *pImg, Callback *cb = 0) const;

      protected:
       unsigned char _threshold;
    };
};

#endif /* _COLOURFILTERS_H */
