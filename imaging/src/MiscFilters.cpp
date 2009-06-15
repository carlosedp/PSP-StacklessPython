
/**
 * @file MiscFilters.cpp
 * @brief Implementation of class MiscFilters
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
// $Id: MiscFilters.cpp 156 2008-03-16 12:07:15Z fraca7 $

#include "../imaging/MiscFilters.h"
#include "../imaging/Image.h"
#include "../imaging/Callback.h"

#include <math.h>
#include <cstdlib>
#include <cstring>

#include <stack>

using namespace Imaging;
using namespace std;

ScaleFilter::ScaleFilter(u16 w, u16 h)
    : _w(w), _h(h)
{
}

ScaleFilter::~ScaleFilter()
{
}

Image* ScaleFilter::apply(const Image *pImg, Callback *cb) const
{
    if ((_w > pImg->getWidth()) || (_h > pImg->getHeight()))
       return NULL;

    Image *pResult = new Image(_w, _h);

    for (int y = 0; y < _h; ++y)
    {
       for (int x = 0; x < _w; ++x)
       {
          unsigned int r = 0, g = 0, b = 0, a = 0, count = 0;

          for (int j = y * pImg->getHeight() / _h;
               j <= (y + 1) * pImg->getHeight() / _h;
               ++j)
          {
             if (j >= pImg->getHeight())
                break;

             for (int i = x * pImg->getWidth() / _w;
                  i <= (x + 1) * pImg->getWidth() / _w;
                  ++i)
             {
                if (i >= pImg->getWidth())
                   break;

                u32 px = pImg->getPixel(i, j);

                r += (px & 0xFF);
                g += ((px >> 8) & 0xFF);
                b += ((px >> 16) & 0xFF);
                a += ((px >> 24) & 0xFF);

                ++count;
             }
          }

          r /= count;
          g /= count;
          b /= count;
          a /= count;

          pResult->putPixel((a << 24) | (b << 16) | (g << 8) | r, x, y);
       }

       if (cb)
       {
          if (!cb->update(y, _h))
          {
             delete pResult;
             pResult = NULL;

             break;
          }
       }
    }

    if (cb && pResult)
    {
       cb->update(_h, _h);
    }

    return pResult;
}


//==========================================================================
//

SubFilter::SubFilter(u16 x, u16 y, u16 w, u16 h)
    : _x(x), _y(y), _w(w), _h(h)
{
}

SubFilter::~SubFilter()
{
}

Image* SubFilter::apply(const Image *pImg, Callback *cb) const
{
    if ((_x >= pImg->getWidth()) || (_y >= pImg->getHeight()))
       return NULL;

    u16 w = MIN(_w, pImg->getWidth() - 1 - _x);
    u16 h = MIN(_h, pImg->getHeight() - 1 - _y);

    Image *pResult = new Image(w, h);

    u32 *src = pImg->getData() + _y * pImg->getTextureWidth() + _x;
    u32 *dst = pResult->getData();

    for (u16 j = 0; j < h; ++j)
    {
       memcpy(dst, src, sizeof(u32) * w);

       src += pImg->getTextureWidth();
       dst += pResult->getTextureWidth();

       if (cb)
       {
          if (!cb->update(j, j))
          {
             delete pResult;
             pResult = NULL;

             break;
          }
       }
    }

    if (cb && pResult)
    {
       cb->update(h, h);
    }

    return pResult;
}

SobelFilter::~SobelFilter()
{
}

Image* SobelFilter::apply(const Image *pImg, Callback *cb) const
{
    u32 maxSquared = 0;

    int x, y;

    // First pass: compute max. squared gradient norm

    u32 *line0, *line1, *line2;

    line0 = pImg->getData();
    line1 = line0 + pImg->getTextureWidth();
    line2 = line1 + pImg->getTextureWidth();

    for (y = 1; y < pImg->getHeight() - 1; ++y)
    {
       for (x = 1; x < pImg->getWidth() - 1; ++x)
       {
          int gx = - (int)(*(line0 + x - 1) & 0xFF) + (*(line0 + x + 1) & 0xFF) +
             -2 * (*(line1 + x - 1) & 0xFF) + 2 * (*(line1 + x + 1) & 0xFF) +
             - (*(line2 + x - 1) & 0xFF) + (*(line2 + x + 1) & 0xFF);
          int gy = (int)(*(line0 + x - 1) & 0xFF) + 2 * (*(line0 + x) & 0xFF) + (*(line0 + x + 1) & 0xFF) +
             -(*(line2 + x - 1) & 0xFF) - 2 * (*(line2 + x) & 0xFF) - (*(line2 + x + 1) & 0xFF);

          u32 v = gx*gx + gy*gy;

          if (v > maxSquared)
             maxSquared = v;
       }

       line0 += pImg->getTextureWidth();
       line1 += pImg->getTextureWidth();
       line2 += pImg->getTextureWidth();

       if (cb)
       {
          if (!cb->update(y - 1, (pImg->getHeight() - 2) * 2))
          {
             return NULL;
          }
       }
    }

    // Now go

    Image *pResult = new Image(pImg->getWidth(), pImg->getHeight());

    line0 = pImg->getData();
    line1 = line0 + pImg->getTextureWidth();
    line2 = line1 + pImg->getTextureWidth();

    for (y = 1; y < pImg->getHeight() - 1; ++y)
    {
       for (x = 1; x < pImg->getWidth() - 1; ++x)
       {
          if (maxSquared)
          {
             int gx = - (int)(*(line0 + x - 1) & 0xFF) + (*(line0 + x + 1) & 0xFF) +
                -2 * (*(line1 + x - 1) & 0xFF) + 2 * (*(line1 + x + 1) & 0xFF) +
                - (*(line2 + x - 1) & 0xFF) + (*(line2 + x + 1) & 0xFF);
             int gy = (int)(*(line0 + x - 1) & 0xFF) + 2 * (*(line0 + x) & 0xFF) + (*(line0 + x + 1) & 0xFF) +
                -(*(line2 + x - 1) & 0xFF) - 2 * (*(line2 + x) & 0xFF) - (*(line2 + x + 1) & 0xFF);

             u32 v = (u32)(sqrt(1.0 * (gx*gx + gy*gy) * 255 * 255 / maxSquared));

             pResult->putPixel(v | (v << 8) | (v << 16) | 0xFF000000U, x, y);
          }
          else
          {
             pResult->putPixel(0, x, y);
          }
       }

       line0 += pImg->getTextureWidth();
       line1 += pImg->getTextureWidth();
       line2 += pImg->getTextureWidth();

       if (cb)
       {
          if (!cb->update(pImg->getHeight() - 2 + y - 1, (pImg->getHeight() - 2) * 2))
          {
             delete pResult;
             pResult = NULL;

             break;
          }
       }
    }

    if (pResult && cb)
    {
       cb->update((pImg->getHeight() - 2) * 2, (pImg->getHeight() - 2) * 2);
    }

    return pResult;
}

ConnectedFilter::ConnectedFilter(u16 x, u16 y)
    : _x(x),
      _y(y)
{
}

ConnectedFilter::~ConnectedFilter()
{
}

Image* ConnectedFilter::apply(const Image *pImg, Callback *cb) const
{
    Image *pResult = new Image(pImg->getWidth(), pImg->getHeight());
    pResult->clear(0);

    stack<int> stackX;
    stack<int> stackY;

    u32 oldcolour = pImg->getPixel(_x, _y) & 0x00FFFFFFU;

    stackX.push(_x);
    stackY.push(_y);

    while (stackX.size())
    {
       int xx, yy;

       xx = stackX.top();
       yy = stackY.top();

       stackX.pop();
       stackY.pop();

       int yr = yy;

       while ((yr >= 0) &&
              (((pImg->getPixel(xx, yr) & 0x00FFFFFFU) == oldcolour) && (!(pResult->getPixel(xx, yr) & 0x00FFFFFFU))))
       {
          --yr;
       }
       ++yr;

       bool sl = false, sr = false;

       while ((yr < pImg->getHeight()) &&
              ((pImg->getPixel(xx, yr) & 0x00FFFFFFU) == oldcolour) &&
              (!(pResult->getPixel(xx, yr) & 0x00FFFFFFU)))
       {
          pResult->putPixel(0xFFFFFFFFU, xx, yr);

          if (!sl &&
              (xx > 0) &&
              (((pImg->getPixel(xx - 1, yr) & 0x00FFFFFFU) == oldcolour) && (!(pResult->getPixel(xx - 1, yr) & 0x00FFFFFFU))))
          {
             stackX.push(xx - 1);
             stackY.push(yr);

             sl = true;
          }
          else if (sl &&
                   (((pImg->getPixel(xx - 1, yr) & 0x00FFFFFFU) != oldcolour) || (pResult->getPixel(xx - 1, yr) & 0x00FFFFFFU)))
          {
             sl = false;
          }

          if (!sr &&
              (xx < pImg->getWidth() - 1) &&
              (((pImg->getPixel(xx + 1, yr) & 0x00FFFFFFU) == oldcolour) && (!(pResult->getPixel(xx + 1, yr) & 0x00FFFFFFU))))
          {
             stackX.push(xx + 1);
             stackY.push(yr);

             sr = true;
          }
          else if (sr &&
                   (xx < pImg->getWidth() - 1) &&
                   (((pImg->getPixel(xx + 1, yr) & 0x00FFFFFFU) != oldcolour) || pResult->getPixel(xx + 1, yr) & 0x00FFFFFFU))
          {
             sr = false;
          }

          ++yr;
       }
    }

    return pResult;
}


//==========================================================================
//

DifferenceFilter::DifferenceFilter(Image *pImg)
    : m_pImg(pImg)
{
}

DifferenceFilter::~DifferenceFilter()
{
}

Image* DifferenceFilter::apply(const Image *pImg, Callback *cb) const
{
    Image *pResult = new Image(pImg->getWidth(), pImg->getHeight());

    u32 *src1 = m_pImg->getData();
    u32 *src2 = pImg->getData();

    u32 *dst = pResult->getData();

    for (int y = 0; y < pImg->getHeight(); ++y)
    {
       for (int x = 0; x < pImg->getWidth(); ++x)
       {
          int d = (int)(*src1 & 0xFF) - (int)(*src2 & 0xFF);

          if (d < 0)
             d = -d;

          *dst = 0xFF000000 | (d << 16) | (d << 8) | d;

          ++src1;
          ++src2;

          ++dst;
       }

       src1 += pImg->getTextureWidth() - pImg->getWidth();
       src2 += pImg->getTextureWidth() - pImg->getWidth();

       dst += pImg->getTextureWidth() - pImg->getWidth();
    }

    return pResult;
}

DifferenceInPlaceFilter::DifferenceInPlaceFilter(Image *pImg)
    : m_pImg(pImg)
{
}

DifferenceInPlaceFilter::~DifferenceInPlaceFilter()
{
}

void DifferenceInPlaceFilter::apply(Image *pImg, Callback *cb) const
{
    u32 *src1 = m_pImg->getData();
    u32 *src2 = pImg->getData();

    for (int y = 0; y < pImg->getHeight(); ++y)
    {
       for (int x = 0; x < pImg->getWidth(); ++x)
       {
          int d = (int)(*src1 & 0xFF) - (int)(*src2 & 0xFF);

          if (d < 0)
             d = -d;

          *src2 = 0xFF000000 | (d << 16) | (d << 8) | d;

          ++src1;
          ++src2;
       }

       src1 += pImg->getTextureWidth() - pImg->getWidth();
       src2 += pImg->getTextureWidth() - pImg->getWidth();
    }
}

static const char* _rcsid_MiscFilters __attribute__((unused)) = "$Id: MiscFilters.cpp 156 2008-03-16 12:07:15Z fraca7 $";
