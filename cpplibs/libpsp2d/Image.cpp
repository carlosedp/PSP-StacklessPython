
/**
 * @file Image.cpp
 * @brief Implementation of class Image
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
// $Id: Image.cpp 106 2007-09-08 11:06:26Z fraca7 $

#include <malloc.h>

#include <algorithm>

#include <png.h>

extern "C"
{
#include <jpeglib.h>
#include <jerror.h>
}

#include "Image.h"
#include "Blitter.h"

using namespace PSP2D;
using namespace std;

extern u8 msx[];

static u16 getNextPower2(u16 width)
{
    int k;
    u16 r = 1;

    for (k = 0; k < 16; ++k)
    {
       if (r >= width)
          return (r < 16) ? 16 : r;

       r *= 2;
    }

    return 0;
}

static void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
{
}

Image::Image(const string& filename)
{
    string png(".png"), jpg(".jpg"), jpeg(".jpeg");

    if (search(filename.begin(), filename.end(),
               png.begin(), png.end()) != filename.end())
    {
       _loadFromPNG(filename);
    }
    else if (search(filename.begin(), filename.end(),
                    jpg.begin(), jpg.end()) != filename.end())
    {
       _loadFromJPEG(filename);
    }
    else if (search(filename.begin(), filename.end(),
                    jpeg.begin(), jpeg.end()) != filename.end())
    {
       _loadFromJPEG(filename);
    }
    else
    {
       throw ImageException("Unknown file type");
    }
}

void Image::_loadFromPNG(const string& filename)
{
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int bit_depth, color_type, interlace_type, y;
    png_uint_32 pw, ph;
    //u32* line;
    FILE *fp;
    int passes;
    u8** rows;

    if ((fp = fopen(filename.c_str(), "rb")) == NULL)
       throw ImageIOException("Could not open file");

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
       fclose(fp);

       throw ImagePNGException("Could not create PNG struct");
    }

    png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, user_warning_fn);

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
       fclose(fp);
       png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);

       throw ImagePNGException("Could not create PNG info struct");
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, sig_read);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &pw, &ph, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);

    _width = (u16)pw;
    _height = (u16)ph;

    _textureWidth = getNextPower2(_width);
    _textureHeight = getNextPower2(_height);

    png_set_strip_16(png_ptr);
    png_set_packing(png_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
       png_set_palette_to_rgb(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
       png_set_gray_1_2_4_to_8(png_ptr);

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
       png_set_tRNS_to_alpha(png_ptr);

    if (interlace_type == PNG_INTERLACE_ADAM7)
    {
       passes = png_set_interlace_handling(png_ptr);
       png_read_update_info(png_ptr, info_ptr);
    }
    else
    {
       passes = 1;
    }

    png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
    _data = (u32*) memalign(16, _textureWidth * _textureHeight * sizeof(u32));

    if (!_data)
    {
       fclose(fp);
       png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);

       throw ImageException("Memory error");
    }

    rows = (u8**)malloc(sizeof(u8*) * _height);
    for (y = 0;  y < _height; ++y)
    {
       rows[y] = (u8*)(_data + y * _textureWidth);
    }

    for (y = 0; y < passes; ++y)
    {
       png_read_rows(png_ptr, rows, NULL, _height);
    }

    free(rows);

    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
    fclose(fp);
}

void Image::_loadFromJPEG(const string& filename)
{
    struct jpeg_decompress_struct dinfo;
    struct jpeg_error_mgr jerr;

    dinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&dinfo);

    FILE* inFile = fopen(filename.c_str(), "rb");
    if (!inFile)
    {
       jpeg_destroy_decompress(&dinfo);

       throw ImageIOException("Could not open file");
    }

    jpeg_stdio_src(&dinfo, inFile);
    jpeg_read_header(&dinfo, TRUE);

    int width = dinfo.image_width;
    int height = dinfo.image_height;

    _width = width;
    _height = height;
    _textureWidth = getNextPower2(width);
    _textureHeight = getNextPower2(height);

    jpeg_start_decompress(&dinfo);

    _data = (u32*) memalign(16, _textureWidth * _textureHeight * sizeof(u32));
    if (!_data) {
       jpeg_destroy_decompress(&dinfo);

       fclose(inFile);

       throw ImageException("Memory error");
    }

    u8* line = (u8*) malloc(width * 3);
    if (!line) {
       jpeg_destroy_decompress(&dinfo);

       fclose(inFile);
       free(_data);

       throw ImageException("Memory error");
    }

    if (dinfo.jpeg_color_space == JCS_GRAYSCALE)
    {
       while (dinfo.output_scanline < dinfo.output_height)
       {
          int y = dinfo.output_scanline;
          jpeg_read_scanlines(&dinfo, &line, 1);

          for (int x = 0; x < width; x++) {
             u32 c = line[x];
             _data[x + _textureWidth * y] = c | (c << 8) | (c << 16) | 0xff000000;;
          }
       }
    }
    else
    {
       while (dinfo.output_scanline < dinfo.output_height)
       {
          int y = dinfo.output_scanline;
          jpeg_read_scanlines(&dinfo, &line, 1);
          u8* linePointer = line;

          for (int x = 0; x < width; x++)
          {
             u32 c = *(linePointer++);
             c |= (*(linePointer++)) << 8;
             c |= (*(linePointer++)) << 16;
             _data[x + _textureWidth * y] = c | 0xff000000;
          }
       }
    }

    jpeg_finish_decompress(&dinfo);
    jpeg_destroy_decompress(&dinfo);

    fclose(inFile);
    free(line);
}

Image::Image(u16 width, u16 height)
    : _width(width),
      _height(height),
      _data(NULL)
{
    _textureWidth = getNextPower2(_width);
    _textureHeight = getNextPower2(_height);

    _data = (u32*) memalign(16, _textureWidth * _textureHeight * sizeof(u32));

    if (!_data)
       throw ImageException("Memory error");
}

Image::Image(Image *img)
    : _width(img->_width),
      _height(img->_height),
      _textureWidth(img->_textureWidth),
      _textureHeight(img->_textureHeight)
{
    _data = (u32*)memalign(16, _textureWidth * _textureHeight * sizeof(u32));

    if (!_data)
       throw ImageException("Memory error");

    int j;
    for (j = 0; j < _height; ++j)
       memcpy(_data + j * _textureWidth,
              img->_data + j * img->_textureWidth,
              _width * sizeof(u32));
}

Image::~Image()
{
    free(_data);
}

u16 Image::getWidth()
{
    return _width;
}

u16 Image::getHeight()
{
    return _height;
}

u16 Image::getTextureWidth()
{
    return _textureWidth;
}

u16 Image::getTextureHeight()
{
    return _textureHeight;
}

u32* Image::getData()
{
    return _data;
}

void Image::accept(DrawableVisitor *v)
{
    v->visitImage(this);
}

void Image::blit(Drawable *drw, u16 sx, u16 sy, u16 w, u16 h,
                 u16 dx, u16 dy, bool blend, double scaleX, double scaleY)
{
    ImageBlitter blt(this, sx, sy, w, h, dx, dy, blend, scaleX, scaleY);

    drw->accept(&blt);
}

void Image::clear(u32 color)
{
    u16 i, j;

    for (j = 0; j < _height; ++j)
    {
       for (i = 0; i < _width; ++i)
       {
          *(_data + j * _textureWidth + i) = color;
       }
    }
}

void Image::drawLine(u16 x0, u16 y0, u16 x1, u16 y1, u32 color)
{
    int dy;
    int dx;
    int stepx, stepy;

    x0 = (x0 >= _width) ? _width - 1 : x0;
    x1 = (x1 >= _width) ? _width - 1 : x1;

    y0 = (y0 >= _height) ? _height - 1 : y0;
    y1 = (y1 >= _height) ? _height - 1 : y1;

    dy = y1 - y0;
    dx = x1 - x0;

    if (dy < 0) { dy = -dy;  stepy = -_textureWidth; } else { stepy = _textureWidth; }
    if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
    dy <<= 1;
    dx <<= 1;

    y0 *= _textureWidth;
    y1 *= _textureWidth;
    _data[x0+y0] = color;
    if (dx > dy) {
       int fraction = dy - (dx >> 1);
       while (x0 != x1) {
          if (fraction >= 0) {
             y0 += stepy;
             fraction -= dx;
          }
          x0 += stepx;
          fraction += dy;
          _data[x0+y0] = color;
       }
    } else {
       int fraction = dx - (dy >> 1);
       while (y0 != y1) {
          if (fraction >= 0) {
             x0 += stepx;
             fraction -= dy;
          }
          y0 += stepy;
          fraction += dx;
          _data[x0+y0] = color;
       }
    }
}

void Image::fillRect(u32 color, u16 x, u16 y, u16 w, u16 h)
{
    u16 nx, ny;
    u32 *dst;

    x = (x >= _width) ? _width - 1 : x;

    w = (x + w > _width) ? _width - x : w;
    h = (y + h > _height) ? _height - y : h;

    dst = _data + x + y * _textureWidth;

    for (ny = 0; ny < h; ny++, dst += _textureWidth) {
       for (nx = 0; nx < w; nx++) *(dst + nx) = color;
    }
}

void Image::printText(u16 x, u16 y, const string& text, u32 color)
{
    int c, i, j, l;
    u8 *font;
    u32 *data_ptr;
    u32 *data;

    for (c = 0; c < (int)text.size(); c++) {
       if ((x + 8 > _width) || (y + 8 > _height))
          break;

       char ch = text[c];
       data = _data + x + y * _textureWidth;

       font = &msx[ (int)ch * 8];
       for (i = l = 0; i < 8; i++, l += 8, font++) {
          data_ptr  = data;
          for (j = 0; j < 8; j++) {
             if ((*font & (128 >> j))) *data_ptr = color;
             data_ptr++;
          }
          data += _textureWidth;
       }
       x += 8;
    }
}

void Image::saveToFile(const string& filename, ImageType type)
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

void Image::_saveToPNG(const string& filename)
{
    png_structp png_ptr;
    png_infop info_ptr;
    FILE* fp;
    int i, x, y;
    u8* line;
	
    if ((fp = fopen(filename.c_str(), "wb")) == NULL)
       return;

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
    png_set_IHDR(png_ptr, info_ptr, _width, _height, 8,
                 PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);

    line = (u8*) malloc(_width * 4);

    for (y = 0; y < _height; y++) {
       for (i = 0, x = 0; x < _width; x++) {
          u32 color = _data[x + y * _textureWidth];
          u8 r = color & 0xff;
          u8 g = (color >> 8) & 0xff;
          u8 b = (color >> 16) & 0xff;
          u8 a = (color >> 24) & 0xff;
          line[i++] = r;
          line[i++] = g;
          line[i++] = b;
          line[i++] = a;
       }
       png_write_row(png_ptr, line);
    }

    free(line);
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    fclose(fp);
}

void Image::_saveToJPEG(const string& filename)
{
    FILE* outFile = fopen(filename.c_str(), "wb");

    if (!outFile)
    {
       throw ImageIOException("Could not open file");
    }

    struct jpeg_error_mgr jerr;
    struct jpeg_compress_struct cinfo;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outFile);

    cinfo.image_width = _width;
    cinfo.image_height = _height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 100, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    u8* row = (u8*) malloc(_width * 3);
    if (!row)
    {
       fclose(outFile);

       throw ImageException("Memory error");
    }

    for (int y = 0; y < _height; y++)
    {
       u8* rowPointer = row;
       for (int x = 0; x < _width; x++)
       {
          u32 c = _data[x + cinfo.next_scanline * _textureWidth];
          *(rowPointer++) = c & 0xff;
          *(rowPointer++) = (c >> 8) & 0xff;
          *(rowPointer++) = (c >> 16) & 0xff;
       }

       jpeg_write_scanlines(&cinfo, &row, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(outFile);
    free(row);
}

Image* Image::erode(bool *kernel, u16 size, Callback *cb)
{
    int t, ir, jr, i, j;
    int r = size >> 1;

    Image *result = new Image(_width, _height);

    u32* src = _data;
    u32* dst = result->_data;

    u8 minR, minG, minB, v;
    u32 color;

    for (int y = 0; y < _height; ++y)
    {
       for (int x = 0; x < _width; ++x, ++src, ++dst)
       {
          minR = minG = minB = 255;

          for (i = 0; i < size; ++i)
          {
             ir = i - r;
             t = y + ir;

             if (t < 0)
                continue;

             if (t >= _height)
                break;

             for (j = 0; j < size; ++j)
             {
                jr = j - r;
                t = x + jr;

                if (t < 0)
                   continue;

                if (t < _width)
                {
                   if (kernel[i * size + j])
                   {
                      color = *(src + ir * _textureWidth + jr);

                      v = (u8)(color & 0xFF);
                      if (v < minR)
                         minR = v;

                      v = (u8)((color >> 8) & 0xFF);
                      if (v < minG)
                         minG = v;

                      v = (u8)((color >> 16) & 0xFF);
                      if (v < minB)
                         minB = v;
                   }
                }
             }
          }

          *dst = (u32)minR | ((u32)minG << 8) | ((u32)minB << 16) | 0xFF000000U;
       }

       src += _textureWidth - _width;
       dst += _textureWidth - _width;

       if (cb)
       {
          if (!cb->update(y, _height))
          {
             delete result;
             result = NULL;
             break;
          }
       }

    }

    if (cb && result)
    {
       cb->update(_height, _height);
    }

    return result;
}

Image* Image::dilate(bool *kernel, u16 size, Callback *cb)
{
    int t, ir, jr, i, j;
    int r = size >> 1;

    Image *result = new Image(_width, _height);

    u32* src = _data;
    u32* dst = result->_data;

    u8 maxR, maxG, maxB, v;
    u32 color;

    for (int y = 0; y < _height; ++y)
    {
       for (int x = 0; x < _width; ++x, ++src, ++dst)
       {
          maxR = maxG = maxB = 0;

          for (i = 0; i < size; ++i)
          {
             ir = i - r;
             t = y + ir;

             if ( t < 0 )
                continue;

             if ( t >= _height )
                break;

             for (j = 0; j < size; ++j)
             {
                jr = j - r;
                t = x + jr;

                if ( t < 0 )
                   continue;

                if (t < _width)
                {
                   if (kernel[i * size + j])
                   {
                      color = *(src + ir * _textureWidth + jr);

                      v = (u8)(color & 0xFF);
                      if ( v > maxR )
                         maxR = v;

                      v = (u8)((color >> 8) & 0xFF);
                      if ( v > maxG )
                         maxG = v;

                      v = (u8)((color >> 16) & 0xFF);
                      if ( v > maxB )
                         maxB = v;
                   }
                }
             }
          }

          *dst = (u32)maxR | ((u32)maxG << 8) | ((u32)maxB << 16) | 0xFF000000U;
       }

       src += _textureWidth - _width;
       dst += _textureWidth - _width;

       if (cb)
       {
          if (!cb->update(y, _height))
          {
             delete result;
             result = NULL;
             break;
          }
       }
    }

    if (cb && result)
    {
       cb->update(_height, _height);
    }

    return result;
}

static const char* _rcsid_Image __attribute__((unused)) = "$Id: Image.cpp 106 2007-09-08 11:06:26Z fraca7 $";
