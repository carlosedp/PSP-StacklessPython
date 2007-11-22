
/**
 * @file music.c
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: music.cpp 1434 2005-11-13 07:10:16Z fraca7 $

#include "music.h"

using namespace PSPSND;

extern PyObject* PySndError; // in pspsnd.cpp

static void music_dealloc(PyMusic *self)
{
    delete self->music;

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* music_new(PyTypeObject *type,
                           PyObject *args,
                           PyObject *kwargs)
{
    PyMusic *self;

    if (PyErr_CheckSignals())
       return NULL;

    self = (PyMusic*)type->tp_alloc(type, 0);
    if (self)
       self->music = NULL;

    return (PyObject*)self;
}

static int music_init(PyMusic *self,
                      PyObject *args,
                      PyObject *kwargs)
{
    char *filename;
    int maxchan = 128;
    int loop = 0;

    static char* kwids[] = { "filename", "maxchan", "loop", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|ii:__init__", kwids,
                                     &filename, &maxchan, &loop))
       return -1;

    if (PyErr_CheckSignals())
       return -1;

    try
    {
       self->music = new Music(filename, loop, maxchan);
    }
    catch (SoundException& e)
    {
       PyErr_SetString(PySndError, e.getMessage().c_str());
       return -1;
    }

    return 0;
}

static PyObject* music_start(PyMusic *self,
                             PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":start"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    self->music->start();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* music_stop(PyMusic *self,
                            PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":stop"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    self->music->stop();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef music_methods[] = {
   { "start", (PyCFunction)music_start, METH_VARARGS,
     "start()\n"
     "Starts playing the music." },
   { "stop", (PyCFunction)music_stop, METH_VARARGS,
     "stop()\n"
     "Stops playing the music." },

   { NULL }
};

static PyTypeObject PyMusicType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "pspsnd.Music",
   sizeof(PyMusic),
   0,
   (destructor)music_dealloc,
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
   "Music objects",
   0,
   0,
   0,
   0,
   0,
   0,
   music_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)music_init,
   0,
   music_new,
};

PyTypeObject* PPyMusicType = &PyMusicType;

#ifdef _GNUC
static const char* _rcsid_music __attribute__((unused)) = "$Id: music.cpp 1434 2005-11-13 07:10:16Z fraca7 $";
#endif
