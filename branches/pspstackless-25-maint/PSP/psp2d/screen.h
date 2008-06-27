
/**
 * @file screen.h
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: screen.h 1468 2005-11-20 12:18:29Z fraca7 $

#ifndef _PYSCREEN_H
#define _PYSCREEN_H

#include <Python.h>
#include <imaging/Screen.h>

typedef struct
{
      PyObject_HEAD

      Imaging::Screen *scr;
} PyScreen;

extern PyTypeObject* PPyScreenType;

#endif /* _PYSCREEN_H */
