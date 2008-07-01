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

#include "oggplayer.h"

/* Init OGG File */
static PyObject *pspogg_init(PyObject *self, PyObject *args) {
    int chan;
    if (!PyArg_ParseTuple(args, "i", &chan))
        return NULL;
    pspAudioInit();
    OGG_Init(chan);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Load OGG File */
static PyObject *pspogg_load(PyObject *self, PyObject *args) {
    const char *path;
    if (!PyArg_ParseTuple(args, "s", &path))
        return NULL;

	char fullpath[512];
  	getcwd(fullpath, 256);
	strcat(fullpath, "/");
	strcat(fullpath, path);

	FILE* soundFile = fopen(fullpath, "rb");
	if (!soundFile) {
        PyErr_SetString(PyExc_IOError, "Could not open file %s.");
        return NULL;
    }
    fclose(soundFile);
    printf("Loading %s\n", fullpath);
	//if (oggplaying == 0) {
	   OGG_Init(2);
	   printf("Init done!\n");
	   OGG_Load(fullpath);
	   printf("Load done...\n");
	   sceKernelDelayThread(10000);
	   printf("Delay Done...\n");
    //}

    Py_INCREF(Py_None);
    return Py_None;
}

/* Stop OGG Playback */
static PyObject *pspogg_stop(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

	//if (oggplaying == 1){
	   OGG_End();
//	   oggplaying = 0;
	//}
    Py_INCREF(Py_None);
    return Py_None;
}

/* Pause OGG Playback */
static PyObject *pspogg_pause(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

	//if (oggplaying == 1) {
	   OGG_Pause();
	//}
    Py_INCREF(Py_None);
    return Py_None;
}

/* Start OGG Playback */
static PyObject *pspogg_play(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;
	printf("Starting play...\n");
	//if (oggplaying == 0) {
       int a;
	   a = OGG_Play();
	//   oggplaying = 1;
	//}
    Py_INCREF(Py_None);
    return Py_None;
}

/* End of OGG Playback */
static PyObject *pspogg_endofstream(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

	//if (oggplaying == 1) {
        if (OGG_EndOfStream() == 1) {
            return Py_BuildValue("i", 1);
        } else {
            return Py_BuildValue("i", 0);
        }
    //}
    Py_INCREF(Py_None);
    return Py_None;
}

/* OGG Playback seconds */
static PyObject *pspogg_getsec(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

    return Py_BuildValue("i", OGG_GetSec());

}

/* OGG Playback minutes */
static PyObject *pspogg_getmin(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

    return Py_BuildValue("i", OGG_GetMin());

}

/* OGG Playback hours */
static PyObject *pspogg_gethour(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

    return Py_BuildValue("i", OGG_GetHour());

}

/* OGG Volume */
static PyObject *pspogg_volume(PyObject *self, PyObject *args) {
    int vol;
    
    if (!PyArg_ParseTuple(args, "i", &vol))
       return NULL;

    OGG_Volume(vol);

    Py_INCREF(Py_None);
    return Py_None;

}

/* OGG End */
static PyObject *pspogg_end(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

    OGG_End();

    Py_INCREF(Py_None);
    return Py_None;


}

static PyMethodDef pspoggMethods[] = {
    {"init",  pspogg_init, METH_VARARGS, "Initializes the OGGVorbis subsystem."},
    {"load",  pspogg_load, METH_VARARGS, "Loads an OGG file."},
    {"stop",  pspogg_stop, METH_VARARGS, "Stop OGG playback."},
    {"pause",  pspogg_pause, METH_VARARGS, "Pauses OGG playback, call again to unpause."},
    {"play",  pspogg_play, METH_VARARGS, "Play a loaded OGG file."},
    {"endofstream",  pspogg_endofstream, METH_VARARGS, "Returns 1 if stream has ended."},
    {"getsec",  pspogg_getsec, METH_VARARGS, "Returns the stream play time in seconds."},
    {"getmin",  pspogg_getmin, METH_VARARGS, "Returns the stream play time in minutes."},
    {"gethour",  pspogg_gethour, METH_VARARGS, "Returns the stream play time in hours."},
    {"volume",  pspogg_volume, METH_VARARGS, "Sets the OGG subsystem volume."},
    {"end",  pspogg_end, METH_VARARGS, "Stops playback and free up used memory."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initpspogg(void)
{
    //int oggplaying = 0;
    printf("Starting pspogg\n");
    (void) Py_InitModule("pspogg", pspoggMethods);
}
