
/**
 * @file Blitter.cpp
 * @brief Implementation of class Blitter
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
// $Id: Blitter.cpp 141 2008-03-02 11:59:44Z fraca7 $

#ifndef LINUX
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#endif

#include <cstdlib>
#include <cstring>

#include "../imaging/Blitter.h"

using namespace Imaging;

typedef struct
{
      unsigned short u, v;
      short x, y, z;
} Vertex;

ScreenBlitter::ScreenBlitter(Screen *scr,
                             u16 sx, u16 sy, u16 w, u16 h,
                             u16 dx, u16 dy,
                             bool blend,
                             double scaleX, double scaleY)
    : _scr(scr), _sx(sx), _sy(sy), _w(w), _h(h), _dx(dx), _dy(dy),
      _scaleX(scaleX), _scaleY(scaleY), _blend(blend)
{
}

void ScreenBlitter::visitScreen(Screen *scr)
{
    // Screen to Screen: not implemented!
}

void ScreenBlitter::visitImage(Image *img)
{
    // Image to screen

#ifndef LINUX
    sceKernelDcacheWritebackInvalidateAll();
    sceGuStart(GU_DIRECT, Screen::getList());

    if (_blend)
    {
       sceGuEnable(GU_TEXTURE_2D);

       sceGuTexImage(0, img->getTextureWidth(), img->getTextureHeight(), img->getTextureWidth(), (void*) img->getData());
       float u = 1.0f / ((float)img->getTextureWidth());
       float v = 1.0f / ((float)img->getTextureHeight());
       sceGuTexScale(u, v);

       u16 j = 0;
       while (j < _w) {
          Vertex* vertices = (Vertex*) sceGuGetMemory(2 * sizeof(Vertex));
          int sliceWidth = 64;
          if (j + sliceWidth > _w) sliceWidth = _w - j;
          vertices[0].u = _sx + j;
          vertices[0].v = _sy;
          vertices[0].x = _dx + (u16)(j * _scaleX);
          vertices[0].y = _dy;
          vertices[0].z = 0;
          vertices[1].u = _sx + j + sliceWidth;
          vertices[1].v = _sy + _h;
          vertices[1].x = _dx + (u16)((j + sliceWidth) * _scaleX);
          vertices[1].y = _dy + (u16)(_h * _scaleY);
          vertices[1].z = 0;
          sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
          j += sliceWidth;
       }

       sceGuDisable(GU_TEXTURE_2D);
    }
    else
    {
       u32* vram = Screen::getVramDrawBuffer();

       sceGuCopyImage(GU_PSM_8888, _sx, _sy, _w, _h, img->getTextureWidth(),
                      img->getData(), _dx, _dy, PSP_LINE_SIZE, vram);
    }

    sceGuFinish();
    sceGuSync(0,0);
#endif
}

ImageBlitter::ImageBlitter(Image *img,
                           u16 sx, u16 sy,
                           u16 w, u16 h,
                           u16 dx, u16 dy,
                           bool blend,
                           double scaleX, double scaleY)
    : _img(img), _sx(sx), _sy(sy), _w(w), _h(h), _dx(dx), _dy(dy),
      _scaleX(scaleX), _scaleY(scaleY), _blend(blend)
{
}

void ImageBlitter::visitScreen(Screen *scr)
{
#ifndef LINUX
    u32 *vram32;
    u16 *vram16;
    u8 *line;

    int bufferwidth;
    int pixelformat;
    int unknown = 0;

    int x, y;

    line = (u8*) malloc(SCREEN_WIDTH * 3);

    sceDisplayWaitVblankStart();
    sceDisplayGetFrameBuf((void**)&vram32,
                          &bufferwidth,
                          &pixelformat,
                          unknown);

    vram16 = (u16*) vram32;

    for (y = _sy; y < _sy + _h; y++)
    {
       if (y >= SCREEN_HEIGHT)
          break;

       if (y - _sy + _dy >= _img->getHeight())
          break;

       for (x = _sx; x < _sx + _w; x++)
       {
          u32 color = 0;

          if (x >= SCREEN_WIDTH)
             break;

          if (x - _sx + _dx >= _img->getWidth())
             break;

          switch (pixelformat)
          {
             case PSP_DISPLAY_PIXEL_FORMAT_565:
                color = vram16[x + y * bufferwidth];
                break;

             case PSP_DISPLAY_PIXEL_FORMAT_5551:
                color = vram16[x + y * bufferwidth];
                break;

             case PSP_DISPLAY_PIXEL_FORMAT_4444:
                color = vram16[x + y * bufferwidth];
                break;

             case PSP_DISPLAY_PIXEL_FORMAT_8888:
                color = vram32[x + y * bufferwidth];
                break;
          }

          _img->putPixel(color | 0xFF000000U, x - _sx + _dx, y - _sy + _dy);
       }
    }

    free(line);
#endif
}

void ImageBlitter::visitImage(Image *img)
{
    // Image to image
    u16 j;

    u32 *srcdec;
    u32 *dstdec;

    srcdec = img->getData() + _sy * img->getTextureWidth() + _sx;
    dstdec = _img->getData() + _dy * _img->getTextureWidth() + _dx;

    for (j = 0; j < _h; ++j)
    {
       if ((_sy + j >= img->getHeight()) || (_dy + j >= _img->getHeight()))
          break;

       if (_blend)
       {
          u16 i;

          for (i = 0; i < _w; ++i)
          {
             if ((_sx + i >= img->getWidth()) || (_dx + i >= _img->getWidth()))
                break;

             u32 a1, a2, r1, r2, g1, g2, b1, b2;

             u32 srccol = *(srcdec + j * img->getTextureWidth() + i);
             u32 dstcol = *(dstdec + j * _img->getTextureWidth() + i);

             a1 = srccol >> 24;
             b1 = (srccol >> 16) & 0xFF;
             g1 = (srccol >> 8) & 0xFF;
             r1 = srccol & 0xFF;

             a2 = dstcol >> 24;
             b2 = (dstcol >> 16) & 0xFF;
             g2 = (dstcol >> 8) & 0xFF;
             r2 = dstcol & 0xFF;

             *(dstdec + j * _img->getTextureWidth() + i) = ((a1 * a1 + (255 - a1) * a2) / 255 << 24) |
                (((a1 * b1 + (255 - a1) * b2) / 255) << 16) |
                (((a1 * g1 + (255 - a1) * g2) / 255) << 8) |
                ((a1 * r1 + (255 - a1) * r2) / 255);
          }
       }
       else
       {
          memcpy(dstdec + j * _img->getTextureWidth(),
                 srcdec + j * img->getTextureWidth(),
                 _w * sizeof(u32));
       }
    }
}

static const char* _rcsid_Blitter __attribute__((unused)) = "$Id: Blitter.cpp 141 2008-03-02 11:59:44Z fraca7 $";
