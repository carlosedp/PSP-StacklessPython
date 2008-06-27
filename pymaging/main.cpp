
/**
 * @file main.cpp
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: main.cpp 157 2008-03-16 12:12:25Z fraca7 $

#include <Python.h>

#include "color.h"
#include "image.h"
#include "mask.h"
#include "font.h"
#include "transform.h"
#include "filters.h"

#include <imaging/Drawable.h>

using namespace Imaging;

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

PyObject* PymagingError;

static PyMethodDef pymaging_functions[] = {
   { NULL }
};

PyMODINIT_FUNC initpymaging(void)
{
    PyObject *mdl;

    if (PyType_Ready(PPyColorType) < 0)
       return;

    if (PyType_Ready(PPyImageType) < 0)
       return;

    if (PyType_Ready(PPyMaskType) < 0)
       return;

    if (PyType_Ready(PPyFontType) < 0)
       return;

    if (PyType_Ready(PPyTransformType) < 0)
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

    if (PyType_Ready(PPyDifferenceInPlaceFilterType) < 0)
       return;

    PymagingError = PyErr_NewException(IMAGING_MODULE_NAME "." "Error", NULL, NULL);
    if (!PymagingError)
       return;

    mdl = Py_InitModule3(IMAGING_MODULE_NAME, pymaging_functions, "2D programming for the PSP");
    if (!mdl)
       return;

    Py_INCREF(PPyColorType);
    PyModule_AddObject(mdl, "Color", (PyObject*)PPyColorType);

    Py_INCREF(PPyImageType);
    PyModule_AddObject(mdl, "Image", (PyObject*)PPyImageType);

    Py_INCREF(PPyMaskType);
    PyModule_AddObject(mdl, "Mask", (PyObject*)PPyMaskType);

    Py_INCREF(PPyFontType);
    PyModule_AddObject(mdl, "Font", (PyObject*)PPyFontType);

    Py_INCREF(PPyTransformType);
    PyModule_AddObject(mdl, "Transform", (PyObject*)PPyTransformType);

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

    Py_INCREF(PPyDifferenceInPlaceFilterType);
    PyModule_AddObject(mdl, "DifferenceInPlaceFilter", (PyObject*)PPyDifferenceInPlaceFilterType);

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
static const char* _rcsid_pymaging __attribute__((unused)) = "$Id: main.cpp 157 2008-03-16 12:12:25Z fraca7 $";
#endif
