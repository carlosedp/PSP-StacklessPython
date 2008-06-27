
/**
 * @file MorphFilters.h
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

#ifndef _MORPH_H
#define _MORPH_H

#include <imaging/ltypes.h>
#include <imaging/Filter.h>

namespace Imaging
{
    /**
     * Structuring element
     */

    class StructuringElement
    {
      public:
       /**
        * Builds  a  structuring  element   from  a  square  array  of
        * bools.  There must  be size*size  elements. This  copies the
        * array.
        */

       StructuringElement(bool* data, u16 size);

       /**
        * Builds a circular structuring element from a radius.
        */

       StructuringElement(u16 radius);

       /**
        * Copy constructor.
        */

       StructuringElement(const StructuringElement&);

       /**
        */

       ~StructuringElement();

      protected:
       bool* m_pData;
       u16 m_uSize;

       friend class ErosionFilter;
       friend class DilationFilter;
    };

    /**
     * Erosion
     */

    class ErosionFilter : public Filter
    {
      public:
       /**
        * Constructor.
        */

       ErosionFilter(const StructuringElement&);

       virtual ~ErosionFilter();

       Image* apply(const Image*, Callback* = 0) const;

      protected:
       StructuringElement *m_pSE;
    };

    /**
     * Dilation
     */

    class DilationFilter : public Filter
    {
      public:
       /**
        * Constructor.
        */

       DilationFilter(const StructuringElement&);

       virtual ~DilationFilter();

       Image* apply(const Image*, Callback* = 0) const;

      protected:
       StructuringElement *m_pSE;
    };
};

// Inline methods
#include <imaging/MorphFilters.il>

#endif /* _MORPH_H */
