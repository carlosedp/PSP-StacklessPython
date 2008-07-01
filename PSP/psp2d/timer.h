
/**
 * @file timer.h
 */

/**********************************************************************

  Created: 20 Nov 2005

**********************************************************************/
// $Id: timer.h 1471 2005-11-20 13:15:21Z fraca7 $

#ifndef _PYTIMER_H
#define _PYTIMER_H

#include <Python.h>
#include <libpsp2d/Timer.h>

class PyListener;

typedef struct
{
      PyObject_HEAD

      PSP2D::Timer *tmr;
      PyListener *lst;
} PyTimer;

extern PyTypeObject* PPyTimerType;

#endif /* _PYTIMER_H */
