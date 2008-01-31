
/**
 * @file BlitBatch.cpp
 * @brief Implementation of class BlitBatch
 */

/**********************************************************************

  Created: 20 Nov 2005

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
// $Id: BlitBatch.cpp 3 2006-03-12 15:13:39Z fraca7 $

#include <pspkernel.h>
#include <pspgu.h>

#include "BlitBatch.h"
#include "Screen.h"

using namespace PSP2D;
using namespace std;

typedef struct
{
      unsigned short u, v;
      short x, y, z;
} Vertex;

BlitBatch::BlitBatch()
{
}

BlitBatch::~BlitBatch()
{
    for (list<BatchedDrawable*>::iterator k = _drws.begin();
         k != _drws.end();
         ++k)
    {
       delete *k;
    }
}

void BlitBatch::add(BatchedDrawable *drw)
{
    _drws.push_back(drw);
}

void BlitBatch::blit()
{
    sceKernelDcacheWritebackInvalidateAll();
    sceGuStart(GU_DIRECT, Screen::getList());
    sceGuEnable(GU_TEXTURE_2D);

    for (list<BatchedDrawable*>::iterator k = _drws.begin();
         k != _drws.end();
         ++k)
    {
       Image *img;
       u16 sx, sy, w, h, dx, dy;

       if ((*k)->getData(img, sx, sy, w, h, dx, dy))
       {
          sceGuTexImage(0, img->getTextureWidth(), img->getTextureHeight(), img->getTextureWidth(), (void*) img->getData());
          float u = 1.0f / ((float)img->getTextureWidth());
          float v = 1.0f / ((float)img->getTextureHeight());
          sceGuTexScale(u, v);

          u16 j = 0;
          while (j < w) {
             Vertex* vertices = (Vertex*) sceGuGetMemory(2 * sizeof(Vertex));
             int sliceWidth = 64;
             if (j + sliceWidth > w) sliceWidth = w - j;
             vertices[0].u = sx + j;
             vertices[0].v = sy;
             vertices[0].x = dx + j;
             vertices[0].y = dy;
             vertices[0].z = 0;
             vertices[1].u = sx + j + sliceWidth;
             vertices[1].v = sy + h;
             vertices[1].x = dx + j + sliceWidth;
             vertices[1].y = dy + h;
             vertices[1].z = 0;
             sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
             j += sliceWidth;
          }
       }
    }

    sceGuDisable(GU_TEXTURE_2D);

    sceGuFinish();
    sceGuSync(0,0);
}

static const char* _rcsid_BlitBatch __attribute__((unused)) = "$Id: BlitBatch.cpp 3 2006-03-12 15:13:39Z fraca7 $";
