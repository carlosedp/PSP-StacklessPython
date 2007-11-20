
/**
 * @file sound.c
 */

/**********************************************************************

  Created: 17 Feb 2007

    Copyright (C) 2007 fraca7@free.fr

    This library  is free software; you can  redistribute it and/or
    modify  it under  the terms  of the  GNU Lesser  General Public
    License as  published by  the Free Software  Foundation; either
    version  2.1 of  the License,  or  (at your  option) any  later
    version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY;  without even the implied warranty of
    MERCHANTABILITY or  FITNESS FOR A PARTICULAR  PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You  should have  received a  copy  of the  GNU Lesser  General
    Public License  along with this  library; if not, write  to the
    Free  Software Foundation,  Inc., 59  Temple Place,  Suite 330,
    Boston, MA 02111-1307 USA

**********************************************************************/
// $Id$

#include "osl.h"
#include "sound.h"

static void sound_dealloc(PySound *self)
{
    if (self->pSound)
       oslDeleteSound(self->pSound);

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* sound_new(PyTypeObject *type,
                           PyObject *args,
                           PyObject *kwargs)
{
    PySound *self;

    self = (PySound*)type->tp_alloc(type, 0);

    if (self)
       self->pSound = NULL;

    return (PyObject*)self;
}

static int sound_init(PySound *self,
                      PyObject *args,
                      PyObject *kwargs)
{
    char *filename;
    int fmt = OSL_FMT_NONE;

    if (!PyArg_ParseTuple(args, "s|i:__init__", &filename, &fmt))
       return -1;

    self->pSound = oslLoadSoundFile(filename, fmt);

    if (!self->pSound)
    {
       PyErr_SetString(osl_Error, "Could not load sound file");
       return -1;
    }

    return 0;
}

//==========================================================================
//

static PyObject* sound_play(PySound *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    int voice = 0;

    if (!PyArg_ParseTuple(args, "|i:play", &voice))
       return NULL;

    oslPlaySound(self->pSound, voice);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* sound_stop(PySound *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":stop"))
       return NULL;

    oslStopSound(self->pSound);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* sound_pause(PySound *self,
                             PyObject *args,
                             PyObject *kwargs)
{
    int pause = 1;

    if (!PyArg_ParseTuple(args, "|i:pause", &pause))
       return NULL;

    oslPauseSound(self->pSound, pause);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* sound_loop(PySound *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    int loop = 1;

    if (!PyArg_ParseTuple(args, "|i:loop", &loop))
       return NULL;

    oslSetSoundLoop(self->pSound, loop);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* sound_getChannel(PySound *self,
                                  PyObject *args,
                                  PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":getChannel"))
       return NULL;

    return Py_BuildValue("i", oslGetSoundChannel(self->pSound));
}

static PyMethodDef sound_methods[] = {
   { "play", (PyCFunction)sound_play, METH_VARARGS, "" },
   { "stop", (PyCFunction)sound_stop, METH_VARARGS, "" },
   { "pause", (PyCFunction)sound_pause, METH_VARARGS, "" },
   { "loop", (PyCFunction)sound_loop, METH_VARARGS, "" },
   { "getChannel", (PyCFunction)sound_getChannel, METH_VARARGS, "" },

   { NULL }
};

static PyTypeObject PySoundType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "osl.Sound",
   sizeof(PySound),
   0,
   (destructor)sound_dealloc,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
   "Sound objects",
   0,
   0,
   0,
   0,
   0,
   0,
   sound_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)sound_init,
   0,
   sound_new,
};

PyTypeObject* PPySoundType = &PySoundType;

#ifdef _GNUC
static const char* _rcsid_sound __attribute__((unused)) = "$Id$";
#endif
