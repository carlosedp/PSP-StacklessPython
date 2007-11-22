
/**
 * @file music.h
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: music.h 1468 2005-11-20 12:18:29Z fraca7 $

#ifndef _PYMUSIC_H
#define _PYMUSIC_H

#include <Python.h>
#include <libpspsnd/Music.h>
#include <libpspsnd/Exception.h>

typedef struct
{
      PyObject_HEAD

      PSPSND::Music *music;
} PyMusic;

extern PyTypeObject* PPyMusicType;

#endif /* _PYMUSIC_H */
