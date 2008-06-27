
/**
 * @file test.cpp
 * @brief Implementation of class test
 */

/**********************************************************************

  Created: 02 Mar 2008

    Copyright (C) 2008 Frank Buss, Jérôme Laheurte

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
// $Id: test.cpp 156 2008-03-16 12:07:15Z fraca7 $

#include <sys/time.h>

#include <imaging/Image.h>
#include <imaging/MorphFilters.h>
#include <imaging/MiscFilters.h>
#include <imaging/ColourFilters.h>

using namespace Imaging;

#define NBTRIES 10

#define BENCHFILTER(klass, ...) { \
  Image img("test.png"); \
  Filter *flt = new klass ( __VA_ARGS__ ); \
  int total = 0; \
  struct timeval t0, t1; \
  for (int i = 0; i < NBTRIES; ++i) { \
    Image *pResult; \
    gettimeofday(&t0, NULL); \
    pResult = flt->apply(&img); \
    gettimeofday(&t1, NULL); \
    total += (t1.tv_sec * 1000 + t1.tv_usec / 1000) - (t0.tv_sec * 1000 + t0.tv_usec / 1000); \
    if (i == 0) \
    { \
      pResult->saveToFile("result/" #klass ".png"); \
    } \
    delete pResult; \
  } \
  printf(#klass ": %d ms\n", total / NBTRIES); \
}

#define BENCHIPFILTER(klass, ...) { \
  Image img("test.png"); \
  InPlaceFilter *flt = new klass ( __VA_ARGS__ ); \
  int total = 0; \
  struct timeval t0, t1; \
  for (int i = 0; i < NBTRIES; ++i) { \
    Image p(img); \
    gettimeofday(&t0, NULL); \
    flt->apply(&p); \
    gettimeofday(&t1, NULL); \
    total += (t1.tv_sec * 1000 + t1.tv_usec / 1000) - (t0.tv_sec * 1000 + t0.tv_usec / 1000); \
    if (i == 0) \
    { \
      p.saveToFile("result/" #klass ".png"); \
    } \
  } \
  printf(#klass ": %d ms\n", total / NBTRIES); \
}

int main(int argc, char *argv[])
{
    Image img("test.png");

    BENCHFILTER(ErosionFilter, StructuringElement(3));
    BENCHFILTER(DilationFilter, StructuringElement(3));

    BENCHFILTER(GrayFilter);
    BENCHIPFILTER(GrayInPlaceFilter);
    BENCHFILTER(MonochromeFilter, 128);
    BENCHIPFILTER(MonochromeInPlaceFilter, 128);

    BENCHFILTER(ScaleFilter, 200, 200);
    BENCHFILTER(SobelFilter);
    BENCHFILTER(DifferenceFilter, &img);
    BENCHIPFILTER(DifferenceInPlaceFilter, &img);

    return 0;
}

static const char* _rcsid_test __attribute__((unused)) = "$Id: test.cpp 156 2008-03-16 12:07:15Z fraca7 $";
