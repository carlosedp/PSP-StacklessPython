
/**
 * @file psp2d.c
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: psp2d.cpp 1484 2005-11-26 12:25:25Z fraca7 $

#include <Python.h>
#include <pspdisplay.h>

#include "color.h"
#include "image.h"
#include "controller.h"
#include "screen.h"
#include "mask.h"
#include "font.h"
#include "transform.h"
#include "blitbatch.h"
#include "timer.h"

#include <libpsp2d/Drawable.h>

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

PyObject* Py2DError;

static PyMethodDef psp2d_functions[] = {
   { NULL }
};

PyMODINIT_FUNC initpsp2d(void)
{
    PyObject *mdl;

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    (void)PSP2D::Screen::getInstance();

    if (PyType_Ready(PPyColorType) < 0)
       return;

    if (PyType_Ready(PPyImageType) < 0)
       return;

    if (PyType_Ready(PPyControllerType) < 0)
       return;

    if (PyType_Ready(PPyScreenType) < 0)
       return;

    if (PyType_Ready(PPyMaskType) < 0)
       return;

    if (PyType_Ready(PPyFontType) < 0)
       return;

    if (PyType_Ready(PPyTransformType) < 0)
       return;

    if (PyType_Ready(PPyBlitBatchType) < 0)
       return;

    if (PyType_Ready(PPyTimerType) < 0)
       return;

    Py2DError = PyErr_NewException("psp2d.Error", NULL, NULL);
    if (!Py2DError)
       return;

    mdl = Py_InitModule3("psp2d", psp2d_functions, "2D programming for the PSP");
    if (!mdl)
       return;

    Py_INCREF(PPyColorType);
    PyModule_AddObject(mdl, "Color", (PyObject*)PPyColorType);

    Py_INCREF(PPyImageType);
    PyModule_AddObject(mdl, "Image", (PyObject*)PPyImageType);

    Py_INCREF(PPyControllerType);
    PyModule_AddObject(mdl, "Controller", (PyObject*)PPyControllerType);

    Py_INCREF(PPyScreenType);
    PyModule_AddObject(mdl, "Screen", (PyObject*)PPyScreenType);

    Py_INCREF(PPyMaskType);
    PyModule_AddObject(mdl, "Mask", (PyObject*)PPyMaskType);

    Py_INCREF(PPyFontType);
    PyModule_AddObject(mdl, "Font", (PyObject*)PPyFontType);

    Py_INCREF(PPyTransformType);
    PyModule_AddObject(mdl, "Transform", (PyObject*)PPyTransformType);

    Py_INCREF(PPyBlitBatchType);
    PyModule_AddObject(mdl, "BlitBatch", (PyObject*)PPyBlitBatchType);

    Py_INCREF(PPyTimerType);
    PyModule_AddObject(mdl, "Timer", (PyObject*)PPyTimerType);

    PyModule_AddIntConstant(mdl, "TR_USER", TR_USER);
    PyModule_AddIntConstant(mdl, "TR_PLUS", TR_PLUS);
    PyModule_AddIntConstant(mdl, "TR_MULT", TR_MULT);
    PyModule_AddIntConstant(mdl, "TR_G2A", TR_G2A);
    PyModule_AddIntConstant(mdl, "TR_GRAY", TR_GRAY);
    PyModule_AddIntConstant(mdl, "TR_BW", TR_BW);

    PyModule_AddIntConstant(mdl, "IMG_PNG", (int)PSP2D::IMG_PNG);
    PyModule_AddIntConstant(mdl, "IMG_JPEG", (int)PSP2D::IMG_JPEG);
}

#ifdef _GNUC
static const char* _rcsid_psp2d __attribute__((unused)) = "$Id: psp2d.cpp 1484 2005-11-26 12:25:25Z fraca7 $";
#endif
