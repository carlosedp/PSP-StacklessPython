
/**
 * @file image.h
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: image.h 1468 2005-11-20 12:18:29Z fraca7 $

#ifndef _PYIMAGE_H
#define _PYIMAGE_H

#include <Python.h>
#include <imaging/Image.h>

typedef struct
{
      PyObject_HEAD

      Imaging::Image *img;
} PyImage;

extern PyTypeObject* PPyImageType;

#endif /* _PYIMAGE_H */
