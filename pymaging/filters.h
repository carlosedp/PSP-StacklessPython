
/**
 * @file filters.h
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

#ifndef _FILTERS_H
#define _FILTERS_H

#include <Python.h>

#include <imaging/Filter.h>
#include <imaging/MorphFilters.h>
#include <imaging/MiscFilters.h>

#include "image.h"

typedef struct
{
      PyObject_HEAD

      Imaging::Filter *pFilter;
} PyFilter;

typedef struct
{
      PyObject_HEAD

      Imaging::InPlaceFilter *pFilter;
} PyInPlaceFilter;


//==========================================================================

typedef struct
{
      PyObject_HEAD

      Imaging::StructuringElement *pSE;
} PyStructuringElement;

extern PyTypeObject* PPyStructuringElementType;

typedef struct
{
      PyObject_HEAD

      Imaging::Filter *pFilter;
      PyImage *pImg;
} PyDifferenceFilter;

typedef struct
{
      PyObject_HEAD

      Imaging::DifferenceInPlaceFilter *pFilter;
      PyImage *pImg;
} PyDifferenceInPlaceFilter;


//==========================================================================

extern PyTypeObject* PPyMonochromeFilterType;
extern PyTypeObject* PPyMonochromeInPlaceFilterType;
extern PyTypeObject* PPyGrayFilterType;
extern PyTypeObject* PPyGrayInPlaceFilterType;
extern PyTypeObject* PPyErosionFilterType;
extern PyTypeObject* PPyDilationFilterType;
extern PyTypeObject* PPyScaleFilterType;
extern PyTypeObject* PPySubFilterType;
extern PyTypeObject* PPySobelFilterType;
extern PyTypeObject* PPyConnectedFilterType;
extern PyTypeObject* PPyDifferenceFilterType;
extern PyTypeObject* PPyDifferenceInPlaceFilterType;

#endif /* _FILTERS_H */
