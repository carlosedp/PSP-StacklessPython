
/**
 * @file MorphFilters.cpp
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
// $Id: MorphFilters.cpp 141 2008-03-02 11:59:44Z fraca7 $

#include <string.h> // memcpy

#include "../imaging/Image.h"
#include "../imaging/MorphFilters.h"

using namespace Imaging;

ErosionFilter::ErosionFilter(const StructuringElement& se)
    : m_pSE(new StructuringElement(se))
{
}

ErosionFilter::~ErosionFilter()
{
    delete m_pSE;
}

Image* ErosionFilter::apply(const Image *pImg, Callback *cb) const
{
    int t, ir, jr, i, j;
    int r = m_pSE->m_uSize >> 1;

    Image *result = new Image(pImg->getWidth(), pImg->getHeight());

    u32* src = pImg->getData();
    u32* dst = result->getData();

    u8 minR, minG, minB, v;
    u32 color;

    for (int y = 0; y < pImg->getHeight(); ++y)
    {
       for (int x = 0; x < pImg->getWidth(); ++x, ++src, ++dst)
       {
          minR = minG = minB = 255;

          for (i = 0; i < m_pSE->m_uSize; ++i)
          {
             ir = i - r;
             t = y + ir;

             if (t < 0)
                continue;

             if (t >= pImg->getHeight())
                break;

             for (j = 0; j < m_pSE->m_uSize; ++j)
             {
                jr = j - r;
                t = x + jr;

                if (t < 0)
                   continue;

                if (t < pImg->getWidth())
                {
                   if (m_pSE->m_pData[i * m_pSE->m_uSize + j])
                   {
                      color = *(src + ir * pImg->getTextureWidth() + jr);

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

       src += pImg->getTextureWidth() - pImg->getWidth();
       dst += pImg->getTextureWidth() - pImg->getWidth();

       if (cb)
       {
          if (!cb->update(y, pImg->getHeight()))
          {
             delete result;
             result = NULL;
             break;
          }
       }
    }

    if (cb && result)
    {
       cb->update(pImg->getHeight(), pImg->getHeight());
    }

    return result;
}


//==========================================================================

DilationFilter::DilationFilter(const StructuringElement& se)
    : m_pSE(new StructuringElement(se))
{
}

DilationFilter::~DilationFilter()
{
    delete m_pSE;
}

Image* DilationFilter::apply(const Image *pImg, Callback *cb) const
{
    int t, ir, jr, i, j;
    int r = m_pSE->m_uSize >> 1;

    Image *result = new Image(pImg->getWidth(), pImg->getHeight());

    u32* src = pImg->getData();
    u32* dst = result->getData();

    u8 maxR, maxG, maxB, v;
    u32 color;

    for (int y = 0; y < pImg->getHeight(); ++y)
    {
       for (int x = 0; x < pImg->getWidth(); ++x, ++src, ++dst)
       {
          maxR = maxG = maxB = 0;

          for (i = 0; i < m_pSE->m_uSize; ++i)
          {
             ir = i - r;
             t = y + ir;

             if ( t < 0 )
                continue;

             if ( t >= pImg->getHeight() )
                break;

             for (j = 0; j < m_pSE->m_uSize; ++j)
             {
                jr = j - r;
                t = x + jr;

                if ( t < 0 )
                   continue;

                if (t < pImg->getWidth())
                {
                   if (m_pSE->m_pData[i * m_pSE->m_uSize + j])
                   {
                      color = *(src + ir * pImg->getTextureWidth() + jr);

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

       src += pImg->getTextureWidth() - pImg->getWidth();
       dst += pImg->getTextureWidth() - pImg->getWidth();

       if (cb)
       {
          if (!cb->update(y, pImg->getHeight()))
          {
             delete result;
             result = NULL;
             break;
          }
       }
    }

    if (cb && result)
    {
       cb->update(pImg->getHeight(), pImg->getHeight());
    }

    return result;
}

static const char* _rcsid_Morph __attribute__((unused)) = "$Id: MorphFilters.cpp 141 2008-03-02 11:59:44Z fraca7 $";
