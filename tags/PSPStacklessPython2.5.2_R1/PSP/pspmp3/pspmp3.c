#include <Python.h>

#include <pspaudio.h>
#include <pspaudiolib.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "MusicEngine.h"

/* Init MP3 File */
static PyObject *pspmp3_init(PyObject *self, PyObject *args) {
    int chan;
    if (!PyArg_ParseTuple(args, "i", &chan))
        return NULL;
    Init(chan);
    //printf("Init done!\n");

    Py_INCREF(Py_None);
    return Py_None;
}


/* Load MP3 File */
static PyObject *pspmp3_load(PyObject *self, PyObject *args) {
    const char *path;
    if (!PyArg_ParseTuple(args, "s", &path))
        return NULL;

	FILE* soundFile = fopen(path, "rb");
	if (!soundFile) {
        PyErr_SetString(PyExc_IOError, "Could not open file.");
        return NULL;
    }
    fclose(soundFile);
    //printf("Loading %s\n", path);
    Load(path);
    //printf("Load done...\n");
    sceKernelDelayThread(10000);
    //printf("Delay Done...\n");

    Py_INCREF(Py_None);
    return Py_None;
}

/* Stop MP3 Playback */
static PyObject *pspmp3_stop(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

    Stop();

    Py_INCREF(Py_None);
    return Py_None;
}

/* Pause MP3 Playback */
static PyObject *pspmp3_pause(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

    Pause();

    Py_INCREF(Py_None);
    return Py_None;
}

/* Start MP3 Playback */
static PyObject *pspmp3_play(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;
	//printf("Starting play...\n");

    Play();
    //printf("Ended play...\n");
    Py_INCREF(Py_None);
    return Py_None;
}

/* End of MP3 Playback */
static PyObject *pspmp3_endofstream(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

    if (EndOfStream() == 1) {
        return Py_BuildValue("i", 1);
    } else {
        return Py_BuildValue("i", 0);
    }

    Py_INCREF(Py_None);
    return Py_None;
}

/* Get MP3 File Time */
static PyObject *pspmp3_gettime(PyObject *self, PyObject *args) {

    if (!PyArg_ParseTuple(args, ""))
        return NULL;

    char time[13];
    GetTimeString(time);
	
    return Py_BuildValue("s", time);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Release MP3 File */
static PyObject *pspmp3_freetune(PyObject *self, PyObject *args) {

    if (!PyArg_ParseTuple(args, ""))
        return NULL;

    FreeTune();

    Py_INCREF(Py_None);
    return Py_None;
}

/* End MP3 File */
static PyObject *pspmp3_end(PyObject *self, PyObject *args) {

    if (!PyArg_ParseTuple(args, ""))
        return NULL;

    End();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef pspmp3Methods[] = {
    {"init",  pspmp3_init, METH_VARARGS, "Initializes the MP3 subsystem."},
    {"load",  pspmp3_load, METH_VARARGS, "Loads a MP3 file."},
    {"stop",  pspmp3_stop, METH_VARARGS, "Stop MP3 playback."},
    {"pause",  pspmp3_pause, METH_VARARGS, "Pauses MP3 playback, call again to unpause."},
    {"play",  pspmp3_play, METH_VARARGS, "Play a loaded MP3 file."},
    {"endofstream",  pspmp3_endofstream, METH_VARARGS, "Returns 1 if stream has ended."},
    {"gettime",  pspmp3_gettime, METH_VARARGS, "Returns the stream play time in format HH:MM:SS."},
    {"freetune",  pspmp3_freetune, METH_VARARGS, "Free up used memory and releases MP3 subsystem."},
    {"end",  pspmp3_end, METH_VARARGS, "Stops playback and calls freetune."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initpspmp3(void)
{
    //printf("Starting pspmp3\n");
    (void) Py_InitModule("pspmp3", pspmp3Methods);
}
