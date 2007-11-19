
/**
 * @file controller.h
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: controller.h 1468 2005-11-20 12:18:29Z fraca7 $

#ifndef _PYCONTROLLER_H
#define _PYCONTROLLER_H

#include <Python.h>
#include <libpsp2d/Controller.h>

typedef struct
{
      PyObject_HEAD

      PSP2D::Controller *ctrl;
} PyController;

extern PyTypeObject* PPyControllerType;

#endif /* _PYCONTROLLER_H */
