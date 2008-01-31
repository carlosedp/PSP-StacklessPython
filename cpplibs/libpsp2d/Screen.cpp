
/**
 * @file Screen.cpp
 * @brief Implementation of class Screen
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
// $Id: Screen.cpp 99 2007-08-18 13:01:27Z fraca7 $

#include <pspgu.h>
#include <pspdisplay.h>

#include <png.h>

#include "framebuffer.h"

#include "Screen.h"
#include "Blitter.h"

using namespace PSP2D;
using namespace std;

static unsigned int __attribute__((aligned(16))) list[262144];

Screen* Screen::_instance = NULL;
int Screen::_current = 0;
bool Screen::_initialized = false;

extern u8 msx[];

typedef struct
{
      short x, y, z;
} Vertex;

Screen::Screen()
{
    init();
}

Screen::~Screen()
{
}

void Screen::blit(Drawable* drw, u16 sx, u16 sy, u16 w, u16 h,
                  u16 dx, u16 dy, bool blend,
                  double scaleX, double scaleY)
{
    ScreenBlitter blt(this, sx, sy, w, h, dx, dy, blend, scaleX, scaleY);

    drw->accept(&blt);
}

void Screen::clear(u32 color)
{
    sceGuStart(GU_DIRECT, list);
    sceGuClearColor(color);
    sceGuClearDepth(0);
    sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
    sceGuFinish();
    sceGuSync(0, 0);
}

void Screen::fillRect(u32 color, u16 x0, u16 y0, u16 w, u16 h)
{
    sceGuStart(GU_DIRECT, getList());

    Vertex *pv = (Vertex*)sceGuGetMemory(4 * sizeof(Vertex));

    memset(pv, 0, sizeof(Vertex) * 4);

    pv[0].x = x0;
    pv[0].y = y0;

    pv[1].x = x0 + w;
    pv[1].y = y0;

    pv[2].x = x0 + w;
    pv[2].y = y0 + h;

    pv[3].x = x0;
    pv[3].y = y0 + h;

    sceGuColor(color);
    sceGuDrawArray(GU_TRIANGLE_FAN, GU_VERTEX_16BIT|GU_TRANSFORM_2D, 4, NULL, pv);

    sceGuFinish();
    sceGuSync(0, 0);
}

void Screen::drawLine(u16 x0, u16 y0, u16 x1, u16 y1, u32 color)
{
    sceGuStart(GU_DIRECT, getList());

    Vertex *pv = (Vertex*)sceGuGetMemory(sizeof(Vertex) * 2);

    pv[0].x = x0;
    pv[0].y = y0;
    pv[0].z = 0;

    pv[1].x = x1;
    pv[1].y = y1;
    pv[1].z = 0;

    sceGuColor(color);
    sceGuDrawArray(GU_LINES, GU_VERTEX_16BIT|GU_TRANSFORM_2D, 2, NULL, pv);

    sceGuFinish();
    sceGuSync(0, 0);
}

void Screen::printText(u16 x, u16 y, const string& text, u32 color)
{
    int c, i, j, l;
    u8 *font;
    u32 *vram_ptr;
    u32 *vram;

    for (c = 0; c < (int)text.size(); c++) {
       if ((x + 8 > SCREEN_WIDTH) || (y + 8 > SCREEN_HEIGHT))
          break;

       char ch = text[c];
       vram = getVramDrawBuffer() + x + y * PSP_LINE_SIZE;

       font = &msx[ (int)ch * 8];
       for (i = l = 0; i < 8; i++, l += 8, font++) {
          vram_ptr  = vram;
          for (j = 0; j < 8; j++) {
             if ((*font & (128 >> j))) *vram_ptr = color;
             vram_ptr++;
          }
          vram += PSP_LINE_SIZE;
       }
       x += 8;
    }
}

void Screen::flip()
{
    sceGuSwapBuffers();
    _current ^= 1;
}

Screen* Screen::getInstance()
{
    if (!_instance)
       _instance = new Screen();

    return _instance;
}

void Screen::init()
{
    if (!_initialized)
    {
       _current = 0;

       sceGuInit();

       sceGuStart(GU_DIRECT, list);
       sceGuDrawBuffer(GU_PSM_8888, (void*)FRAMEBUFFER_SIZE, PSP_LINE_SIZE);
       sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, (void*)0, PSP_LINE_SIZE);
       sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
       sceGuDepthBuffer((void*) (FRAMEBUFFER_SIZE*2), PSP_LINE_SIZE);
       sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
       sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
       sceGuDepthRange(0xc350, 0x2710);
       sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
       sceGuEnable(GU_SCISSOR_TEST);

       sceGuAlphaFunc(GU_GREATER, 0, 0xff);
       sceGuEnable(GU_ALPHA_TEST);

       sceGuDepthFunc(GU_GEQUAL);
       sceGuEnable(GU_DEPTH_TEST);
       sceGuFrontFace(GU_CW);
       sceGuShadeModel(GU_SMOOTH);
       sceGuEnable(GU_CULL_FACE);

       sceGuEnable(GU_CLIP_PLANES);

       sceGuTexMode(GU_PSM_8888, 0, 0, 0);
       sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
       sceGuTexFilter(GU_NEAREST, GU_NEAREST);
       sceGuAmbientColor(0xffffffff);

       sceGuEnable(GU_BLEND);
       sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

       sceGuFinish();
       sceGuSync(0, 0);

       sceDisplayWaitVblankStart();
       sceGuDisplay(GU_TRUE);
    }

    _initialized = true;
}

u32* Screen::getList()
{
    return (u32*)list;
}

void Screen::accept(DrawableVisitor *v)
{
    v->visitScreen(this);
}

u32* Screen::getVramDrawBuffer()
{
    u32* vram = (u32*)g_vram_base;
    if (_current == 0)
       vram += FRAMEBUFFER_SIZE / sizeof(u32);
    return vram;
}

void Screen::saveToFile(const string& filename, ImageType type)
{
    switch (type)
    {
       case IMG_PNG:
          _saveToPNG(filename);
          break;
       case IMG_JPEG:
          _saveToJPEG(filename);
          break;
    }
}

void Screen::_saveToPNG(const string& filename)
{
    u32* vram32;
    u16* vram16;
    int bufferwidth;
    int pixelformat;
    int unknown = 0;
    int i, x, y;
    png_structp png_ptr;
    png_infop info_ptr;
    FILE* fp;
    u8* line;

    fp = fopen(filename.c_str(), "wb");
    if (!fp)
    {
       return;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
       fclose(fp);
       return;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
       png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
       fclose(fp);
       return;
    }

    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, SCREEN_WIDTH, SCREEN_HEIGHT,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);
    line = (u8*) malloc(SCREEN_WIDTH * 3);
    sceDisplayWaitVblankStart();  // if framebuf was set with PSP_DISPLAY_SETBUF_NEXTFRAME, wait until it is changed
    sceDisplayGetFrameBuf((void**)&vram32, &bufferwidth, &pixelformat, unknown);
    vram16 = (u16*) vram32;
    for (y = 0; y < SCREEN_HEIGHT; y++) {
       for (i = 0, x = 0; x < SCREEN_WIDTH; x++) {
          u32 color = 0;
          u8 r = 0, g = 0, b = 0;
          switch (pixelformat) {
             case PSP_DISPLAY_PIXEL_FORMAT_565:
                color = vram16[x + y * bufferwidth];
                r = (color & 0x1f) << 3;
                g = ((color >> 5) & 0x3f) << 2 ;
                b = ((color >> 11) & 0x1f) << 3 ;
                break;
             case PSP_DISPLAY_PIXEL_FORMAT_5551:
                color = vram16[x + y * bufferwidth];
                r = (color & 0x1f) << 3;
                g = ((color >> 5) & 0x1f) << 3 ;
                b = ((color >> 10) & 0x1f) << 3 ;
                break;
             case PSP_DISPLAY_PIXEL_FORMAT_4444:
                color = vram16[x + y * bufferwidth];
                r = (color & 0xf) << 4;
                g = ((color >> 4) & 0xf) << 4 ;
                b = ((color >> 8) & 0xf) << 4 ;
                break;
             case PSP_DISPLAY_PIXEL_FORMAT_8888:
                color = vram32[x + y * bufferwidth];
                r = color & 0xff;
                g = (color >> 8) & 0xff;
                b = (color >> 16) & 0xff;
                break;
          }
          line[i++] = r;
          line[i++] = g;
          line[i++] = b;
       }
       png_write_row(png_ptr, line);
    }
    free(line);
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    fclose(fp);
}

void Screen::_saveToJPEG(const string& filename)
{
    // TODO
}

static const char* _rcsid_Screen __attribute__((unused)) = "$Id: Screen.cpp 99 2007-08-18 13:01:27Z fraca7 $";
