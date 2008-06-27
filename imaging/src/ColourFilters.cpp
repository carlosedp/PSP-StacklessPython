
/**
 * @file ColourFilters.cpp
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
// $Id: ColourFilters.cpp 153 2008-03-16 10:29:17Z fraca7 $

#include "../imaging/Image.h"
#include "../imaging/ColourFilters.h"
#include "../imaging/Callback.h"

using namespace Imaging;

void GrayInPlaceFilter::apply(Image *pImg, Callback *cb) const
{
    u32 *data = pImg->getData();

    for (int j = 0; j < pImg->getHeight(); ++j)
    {
       for (int i = 0; i < pImg->getWidth(); ++i)
       {
          unsigned char v;

          v = (unsigned char)((((*data >> 16) & 0xFF) +
                               ((*data >> 8) & 0xFF) +
                               (*data & 0xFF)) / 3);

          *data = (*data & 0xFF000000) + ((u32)v << 16) + ((u32)v << 8) + (u32)v;

          ++data;
       }

       data += pImg->getTextureWidth() - pImg->getWidth();

       if (cb)
       {
          if (!cb->update(j, pImg->getHeight()))
          {
             return;
          }
       }
    }

    if (cb)
    {
       cb->update(pImg->getHeight(), pImg->getHeight());
    }
}

Image* GrayFilter::apply(const Image *pImg, Callback *cb) const
{
    Image *pResult = new Image(pImg->getWidth(), pImg->getHeight());

    u32 *data = pImg->getData();
    u32 *dstd = pResult->getData();

    for (int j = 0; j < pImg->getHeight(); ++j)
    {
       for (int i = 0; i < pImg->getWidth(); ++i)
       {
          unsigned char v;

          v = (unsigned char)((((*data >> 16) & 0xFF) +
                               ((*data >> 8) & 0xFF) +
                               (*data & 0xFF)) / 3);

          *dstd = (*data & 0xFF000000) + ((u32)v << 16) + ((u32)v << 8) + (u32)v;

          ++data;
          ++dstd;
       }

       data += pImg->getTextureWidth() - pImg->getWidth();
       dstd += pImg->getTextureWidth() - pImg->getWidth();

       if (cb)
       {
          if (!cb->update(j, pImg->getHeight()))
          {
             return NULL;
          }
       }
    }

    if (cb)
    {
       cb->update(pImg->getHeight(), pImg->getHeight());
    }

    return pResult;
}


//==========================================================================

void MonochromeInPlaceFilter::apply(Image *pImg, Callback *cb) const
{
    u32 *data = pImg->getData();

    for (int j = 0; j < pImg->getHeight(); ++j)
    {
       for (int i = 0; i < pImg->getWidth(); ++i)
       {
          unsigned char v;

          v = (unsigned char)((((*data >> 16) & 0xFF) +
                               ((*data >> 8) & 0xFF) +
                               (*data & 0xFF)) / 3);

          v = (v < _threshold) ? 0 : 255;

          *data = (*data & 0xFF000000) + ((u32)v << 16) + ((u32)v << 8) + (u32)v;

          ++data;
       }

       data += pImg->getTextureWidth() - pImg->getWidth();

       if (cb)
       {
          if (!cb->update(j, pImg->getHeight()))
          {
             return;
          }
       }
    }

    if (cb)
    {
       cb->update(pImg->getHeight(), pImg->getHeight());
    }
}

Image* MonochromeFilter::apply(const Image *pImg, Callback *cb) const
{
    Image *pResult = new Image(pImg->getWidth(), pImg->getHeight());

    u32 *data = pImg->getData();
    u32 *dstd = pResult->getData();

    for (int j = 0; j < pImg->getHeight(); ++j)
    {
       for (int i = 0; i < pImg->getWidth(); ++i)
       {
          unsigned char v;

          v = (unsigned char)((((*data >> 16) & 0xFF) +
                               ((*data >> 8) & 0xFF) +
                               (*data & 0xFF)) / 3);

          v = (v < _threshold) ? 0 : 255;

          *dstd = (*data & 0xFF000000) + ((u32)v << 16) + ((u32)v << 8) + (u32)v;

          ++data;
          ++dstd;
       }

       data += pImg->getTextureWidth() - pImg->getWidth();
       dstd += pImg->getTextureWidth() - pImg->getWidth();

       if (cb)
       {
          if (!cb->update(j, pImg->getHeight()))
          {
             return NULL;
          }
       }
    }

    if (cb)
    {
       cb->update(pImg->getHeight(), pImg->getHeight());
    }

    return pResult;
}

static const char* _rcsid_ColourFilters __attribute__((unused)) = "$Id: ColourFilters.cpp 153 2008-03-16 10:29:17Z fraca7 $";
