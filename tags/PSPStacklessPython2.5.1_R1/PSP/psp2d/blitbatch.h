
/**
 * @file blitbatch.h
 */

/**********************************************************************

  Created: 20 Nov 2005

**********************************************************************/
// $Id: blitbatch.h 1468 2005-11-20 12:18:29Z fraca7 $

#ifndef _PYBLITBATCH_H
#define _PYBLITBATCH_H

#include <Python.h>
#include <libpsp2d/BlitBatch.h>

typedef struct
{
      PyObject_HEAD

      PSP2D::BlitBatch *batch;
} PyBlitBatch;

extern PyTypeObject* PPyBlitBatchType;

#endif /* _PYBLITBATCH_H */
