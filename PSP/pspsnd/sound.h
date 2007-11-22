
/**
 * @file sound.h
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: sound.h 1468 2005-11-20 12:18:29Z fraca7 $

#ifndef _PYSOUND_H
#define _PYSOUND_H

#include <Python.h>
#include <libpspsnd/Sound.h>
#include <libpspsnd/Exception.h>

typedef struct
{
      PyObject_HEAD

      PSPSND::Sound *sound;
} PySound;

extern PyTypeObject* PPySoundType;

#endif /* _PYSOUND_H */
