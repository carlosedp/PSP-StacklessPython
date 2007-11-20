
/**
 * @file pspsnd.cpp
 * @brief Implementation of class pspsnd
 */

/**********************************************************************

  Created: 13 Nov 2005

    Copyright (C) 2005 Frank Buss, Jérôme Laheurte

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The names of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**********************************************************************/
// $Id: pspsnd.cpp 1481 2005-11-26 11:33:45Z fraca7 $

#include <Python.h>
#include <libpspsnd/Music.h>

#include "sound.h"
#include "music.h"

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

PyObject* PySndError;

static PyObject* pspsnd_setMusicVolume(PyObject *self,
                                       PyObject *args,
                                       PyObject *kwargs)
{
    int volume;

    if (!PyArg_ParseTuple(args, "i:setMusicVolume", &volume))
       return NULL;

    if ((volume < 0) || (volume > 255))
    {
       PyErr_SetString(PyExc_ValueError, "Bad volume value");
       return NULL;
    }

    if (PyErr_CheckSignals())
       return NULL;

    PSPSND::Music::setVolume((unsigned char)volume);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pspsnd_setSndFxVolume(PyObject *self,
                                       PyObject *args,
                                       PyObject *kwargs)
{
    int volume;

    if (!PyArg_ParseTuple(args, "i:setSndFxVolume", &volume))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if ((volume < 0) || (volume > 255))
    {
       PyErr_SetString(PyExc_ValueError, "Bad volume value");
       return NULL;
    }

    PSPSND::Sound::setVolume((unsigned char)volume);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef pspsnd_functions[] = {
   { "setMusicVolume", (PyCFunction)pspsnd_setMusicVolume, METH_VARARGS, "" },
   { "setSndFxVolume", (PyCFunction)pspsnd_setSndFxVolume, METH_VARARGS, "" },

   { NULL }
};

PyMODINIT_FUNC initpspsnd(void)
{
    PyObject *mdl;

    PSPSND::Music::init();

    if (PyType_Ready(PPyMusicType) < 0)
       return;

    if (PyType_Ready(PPySoundType) < 0)
       return;

    PySndError = PyErr_NewException("pspsnd.Error", NULL, NULL);
    if (!PySndError)
       return;

    mdl = Py_InitModule3("pspsnd", pspsnd_functions, "");
    if (!mdl)
       return;

    PyModule_AddObject(mdl, "Error", PySndError);

    Py_INCREF(PPyMusicType);
    PyModule_AddObject(mdl, "Music", (PyObject*)PPyMusicType);

    Py_INCREF(PPySoundType);
    PyModule_AddObject(mdl, "Sound", (PyObject*)PPySoundType);
}

static const char* _rcsid_pspsnd __attribute__((unused)) = "$Id: pspsnd.cpp 1481 2005-11-26 11:33:45Z fraca7 $";
