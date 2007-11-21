
/**
 * @file transform.h
 */

/**********************************************************************

  Created: 06 Nov 2005

**********************************************************************/
// $Id: transform.h 1468 2005-11-20 12:18:29Z fraca7 $

#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <Python.h>
#include <psptypes.h>

#define TR_USER                 0
#define TR_PLUS                 1
#define TR_MULT                 2
#define TR_G2A                  3     // Gray level to alpha
#define TR_GRAY                 4
#define TR_BW                   5

typedef struct
{
      PyObject_HEAD

      u32 type;           // 0 == user-defined Python callback
      float param;
      PyObject *cb;
} PyTransform;

extern PyTypeObject* PPyTransformType;

#endif /* _TRANSFORM_H */
