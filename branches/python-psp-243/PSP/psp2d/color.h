
/**
 * @file color.h
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: color.h 1468 2005-11-20 12:18:29Z fraca7 $

#ifndef _COLOR_H
#define _COLOR_H

#include <Python.h>
#include <psptypes.h>

typedef struct
{
      PyObject_HEAD

      u32 color;
} PyColor;

extern PyTypeObject* PPyColorType;

#endif /* _COLOR_H */
