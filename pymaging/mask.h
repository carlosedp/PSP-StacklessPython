
/**
 * @file mask.h
 */

/**********************************************************************

  Created: 01 Nov 2005

**********************************************************************/
// $Id: mask.h 1468 2005-11-20 12:18:29Z fraca7 $

#ifndef _PYMASK_H
#define _PYMASK_H

#include <Python.h>
#include <imaging/Mask.h>

typedef struct
{
      PyObject_HEAD

      Imaging::Mask *msk;
} PyMask;

extern PyTypeObject* PPyMaskType;

#endif /* _PYMASK_H */
