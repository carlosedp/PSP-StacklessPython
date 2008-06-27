// -*-c-*-

/**
 * @file MorphFilters.il
 */

/**********************************************************************

  Created: 02 Dec 2007

    Copyright (C) 2007 jerome@homeunix.net

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA

**********************************************************************/
// $Id: MorphFilters.il 141 2008-03-02 11:59:44Z fraca7 $

inline Imaging::StructuringElement::StructuringElement(bool *data, u16 size)
    : m_pData(new bool[size*size]),
      m_uSize(size)
{
    memcpy(m_pData, data, sizeof(bool) * size * size);
}

inline Imaging::StructuringElement::StructuringElement(u16 radius)
    : m_pData(new bool[(radius * 2 + 1) * (radius * 2 + 1)]),
      m_uSize(radius * 2 + 1)
{
    bool *pData = m_pData;

    for (int j = -radius; j < radius; ++j)
    {
       for (int i = -radius; i < radius; ++i)
       {
          if (j*j + i*i < radius*radius)
             *pData++ = true;
          else
             *pData++ = false;
       }
    }
}

inline Imaging::StructuringElement::StructuringElement(const Imaging::StructuringElement& e)
    : m_pData(new bool[e.m_uSize*e.m_uSize]),
      m_uSize(e.m_uSize)
{
    memcpy(m_pData, e.m_pData, sizeof(bool) * e.m_uSize * e.m_uSize);
}

inline Imaging::StructuringElement::~StructuringElement()
{
    delete[] m_pData;
}
