
/**
 * @file psp2d.c
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: psp2d.cpp 1484 2005-11-26 12:25:25Z fraca7 $

#include <Python.h>

#ifndef LINUX
#include <pspdisplay.h>
#endif

#include "color.h"
#include "image.h"
#include "mask.h"
#include "font.h"
#include "transform.h"
#include "timer.h"
#include "filters.h"

#ifndef LINUX
#include "controller.h"
#include "screen.h"
#endif

#include <imaging/Drawable.h>

using namespace PSP2D;
using namespace Imaging;

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

PyObject* Py2DError;

#ifndef LINUX

PyScreen* TheScreen;

static PyObject* GetScreen(PyObject *self,
                           PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":Screen"))
       return NULL;

    Py_INCREF((PyObject*)TheScreen);
    return (PyObject*)TheScreen;
}

#endif

static PyMethodDef psp2d_functions[] = {
#ifndef LINUX
   { "Screen", (PyCFunction)GetScreen, METH_VARARGS, "" },
#endif

   { NULL }
};

PyMODINIT_FUNC initpsp2d(void)
{
    PyObject *mdl;

#ifndef LINUX
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    (void)Screen::getInstance();
#endif

    if (PyType_Ready(PPyColorType) < 0)
       return;

    if (PyType_Ready(PPyImageType) < 0)
       return;

#ifndef LINUX
    if (PyType_Ready(PPyControllerType) < 0)
       return;

    if (PyType_Ready(PPyScreenType) < 0)
       return;
#endif

    if (PyType_Ready(PPyMaskType) < 0)
       return;

    if (PyType_Ready(PPyFontType) < 0)
       return;

    if (PyType_Ready(PPyTransformType) < 0)
       return;

    if (PyType_Ready(PPyTimerType) < 0)
       return;

    if (PyType_Ready(PPyMonochromeFilterType) < 0)
       return;

    if (PyType_Ready(PPyMonochromeInPlaceFilterType) < 0)
       return;

    if (PyType_Ready(PPyGrayFilterType) < 0)
       return;

    if (PyType_Ready(PPyGrayInPlaceFilterType) < 0)
       return;

    if (PyType_Ready(PPyStructuringElementType) < 0)
       return;

    if (PyType_Ready(PPyErosionFilterType) < 0)
       return;

    if (PyType_Ready(PPyDilationFilterType) < 0)
       return;

    if (PyType_Ready(PPyScaleFilterType) < 0)
       return;

    if (PyType_Ready(PPySubFilterType) < 0)
       return;

    if (PyType_Ready(PPySobelFilterType) < 0)
       return;

    if (PyType_Ready(PPyConnectedFilterType) < 0)
       return;

    if (PyType_Ready(PPyDifferenceFilterType) < 0)
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

#ifndef LINUX

    Py_INCREF(PPyControllerType);
    PyModule_AddObject(mdl, "Controller", (PyObject*)PPyControllerType);

    /* Special case for Screen: Singleton, with an access function for
     * backwards compatibility */

    Py_INCREF(PPyScreenType);
    TheScreen = (PyScreen*)PyType_GenericNew(PPyScreenType, NULL, NULL);
    TheScreen->scr = Screen::getInstance();
    PyModule_AddObject(mdl, "TheScreen", (PyObject*)TheScreen);

#endif

    Py_INCREF(PPyMaskType);
    PyModule_AddObject(mdl, "Mask", (PyObject*)PPyMaskType);

    Py_INCREF(PPyFontType);
    PyModule_AddObject(mdl, "Font", (PyObject*)PPyFontType);

    Py_INCREF(PPyTransformType);
    PyModule_AddObject(mdl, "Transform", (PyObject*)PPyTransformType);

    Py_INCREF(PPyTimerType);
    PyModule_AddObject(mdl, "Timer", (PyObject*)PPyTimerType);

    Py_INCREF(PPyMonochromeFilterType);
    PyModule_AddObject(mdl, "MonochromeFilter", (PyObject*)PPyMonochromeFilterType);

    Py_INCREF(PPyMonochromeInPlaceFilterType);
    PyModule_AddObject(mdl, "MonochromeInPlaceFilter", (PyObject*)PPyMonochromeInPlaceFilterType);

    Py_INCREF(PPyGrayFilterType);
    PyModule_AddObject(mdl, "GrayFilter", (PyObject*)PPyGrayFilterType);

    Py_INCREF(PPyGrayInPlaceFilterType);
    PyModule_AddObject(mdl, "GrayInPlaceFilter", (PyObject*)PPyGrayInPlaceFilterType);

    Py_INCREF(PPyStructuringElementType);
    PyModule_AddObject(mdl, "StructuringElement", (PyObject*)PPyStructuringElementType);

    Py_INCREF(PPyErosionFilterType);
    PyModule_AddObject(mdl, "ErosionFilter", (PyObject*)PPyErosionFilterType);

    Py_INCREF(PPyDilationFilterType);
    PyModule_AddObject(mdl, "DilationFilter", (PyObject*)PPyDilationFilterType);

    Py_INCREF(PPyScaleFilterType);
    PyModule_AddObject(mdl, "ScaleFilter", (PyObject*)PPyScaleFilterType);

    Py_INCREF(PPySubFilterType);
    PyModule_AddObject(mdl, "SubFilter", (PyObject*)PPySubFilterType);

    Py_INCREF(PPySobelFilterType);
    PyModule_AddObject(mdl, "SobelFilter", (PyObject*)PPySobelFilterType);

    Py_INCREF(PPyConnectedFilterType);
    PyModule_AddObject(mdl, "ConnectedFilter", (PyObject*)PPyConnectedFilterType);

    Py_INCREF(PPyDifferenceFilterType);
    PyModule_AddObject(mdl, "DifferenceFilter", (PyObject*)PPyDifferenceFilterType);

    PyModule_AddIntConstant(mdl, "TR_USER", TR_USER);
    PyModule_AddIntConstant(mdl, "TR_PLUS", TR_PLUS);
    PyModule_AddIntConstant(mdl, "TR_MULT", TR_MULT);
    PyModule_AddIntConstant(mdl, "TR_G2A", TR_G2A);
    PyModule_AddIntConstant(mdl, "TR_GRAY", TR_GRAY);
    PyModule_AddIntConstant(mdl, "TR_BW", TR_BW);

    PyModule_AddIntConstant(mdl, "IMG_PNG", (int)IMG_PNG);
    PyModule_AddIntConstant(mdl, "IMG_JPEG", (int)IMG_JPEG);
}

#ifdef _GNUC
static const char* _rcsid_psp2d __attribute__((unused)) = "$Id: psp2d.cpp 1484 2005-11-26 12:25:25Z fraca7 $";
#endif
