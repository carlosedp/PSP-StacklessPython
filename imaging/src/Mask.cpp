
/**
 * @file Mask.cpp
 * @brief Implementation of class Mask
 */

/**********************************************************************

  Created: 13 Nov 2005

    Copyright (C) 2005 Jérôme Laheurte

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
// $Id: Mask.cpp 141 2008-03-02 11:59:44Z fraca7 $

#include <malloc.h>
#include <cstdlib>
#include <cstring>

#include "../imaging/Mask.h"

using namespace Imaging;

Mask::Mask(Image *img, u16 x, u16 y, u16 w, u16 h, unsigned char threshold,
           u16 xcoord, u16 ycoord)
    : _width(w), _height(h), _x(xcoord), _y(ycoord)
{
    _wcap = 1;
    while (_wcap < w)
       _wcap *= 2;

    _data = (u32*)memalign(16, sizeof(u32) * h * _wcap);
    memset(_data, 0, sizeof(u32) * h * _wcap);

    if (img)
    {
       int i, j;

       for (j = 0; j < h; ++j)
          for (i = 0; i < w; ++i)
             if ((img->getPixel(x + i, y + j) >> 24) >= threshold)
                set(i, j);
    }
}

Mask::~Mask()
{
    free(_data);
}

void Mask::move(u16 x, u16 y)
{
    _x = x;
    _y = y;
}

void Mask::set(Mask *msk)
{
    int i, j;

    // TODO: check dimensions

    for (j = 0; j < _height; ++j)
       for (i = 0; i < _width; ++i)
          _data[i + j * _wcap] |= msk->_data[i + j * _wcap];
}

u32 Mask::collide(Mask *msk)
{
    u32 count = 0;
    u16 startx, starty, endx, endy, i, j;

    startx = MIN(_x, msk->_x);
    starty = MIN(_y, msk->_y);
    endx = MAX(_x + _width, msk->_x + msk->_width);
    endy = MAX(_y + _height, msk->_y + msk->_height);

    for (i = startx; i < endx; ++i)
    {
       if ((i < _x) || (i >= _x + _width) || (i < msk->_x) || (i >= msk->_x + msk->_width))
          continue;

       for (j = starty; j < endy; ++j)
       {
          if ((j < _y) || (j >= _y + _height) || (j < msk->_y) || (j >= msk->_y + msk->_height))
             continue;

          u32 u = _data[i - _x + (j - _y) * _wcap] & msk->_data[i - msk->_x + (j - msk->_y) * msk->_wcap];

          if (u)
          {
             int k;

             for (k = 0; k < 32; ++k)
                if ((u >> k) & 1)
                   ++count;
          }
       }
    }

    return count;
}

static const char* _rcsid_Mask __attribute__((unused)) = "$Id: Mask.cpp 141 2008-03-02 11:59:44Z fraca7 $";
