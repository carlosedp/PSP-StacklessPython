
/**
 * @file font.c
 */

/**********************************************************************

  Created: 05 Nov 2005

**********************************************************************/
// $Id: font.cpp 1428 2005-11-12 17:22:44Z fraca7 $

#include <png.h>
#include <malloc.h>

#include "font.h"
#include "image.h"

#ifndef LINUX
#include "screen.h"
#endif

using namespace Imaging;

static void font_dealloc(PyFont *self)
{
    delete self->fnt;

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* font_new(PyTypeObject *type,
                          PyObject *args,
                          PyObject *kwargs)
{
    PyFont *self;

    if (PyErr_CheckSignals())
       return NULL;

    self = (PyFont*)type->tp_alloc(type, 0);

    if (self)
       self->fnt = NULL;

    return (PyObject*)self;
}

static int font_init(PyFont *self,
                     PyObject *args,
                     PyObject *kwargs)
{
    char *filename;

    if (!PyArg_ParseTuple(args, "s:__init__", &filename))
       return -1;

    if (PyErr_CheckSignals())
       return -1;

    try
    {
       self->fnt = new Font(filename);
    }
    catch (ImageIOException)
    {
       PyErr_SetString(PyExc_IOError, "Could not open file");
       return -1;
    }
    catch (ImageException)
    {
       PyErr_SetString(PyExc_RuntimeError, "While loading font");
       return -1;
    }

    return 0;
}

static PyObject* font_textWidth(PyFont *self,
                                PyObject *args)
{
    char *text;

    if (!PyArg_ParseTuple(args, "s:textWidth", &text))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->fnt->getTextWidth(text));
}

static PyObject* font_textHeight(PyFont *self,
                                 PyObject *args)
{
    char *text;

    if (!PyArg_ParseTuple(args, "s:textHeight", &text))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->fnt->getTextHeight(text));
}

static PyObject* font_drawText(PyFont *self,
                               PyObject *args)
{
    PyObject *drw;
    int x, y;
    char *text;

    if (!PyArg_ParseTuple(args, "Oiis:drawText", &drw, &x, &y, &text))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (drw->ob_type == PPyImageType)
    {
       PyImage *img = (PyImage*)drw;

       self->fnt->drawText(img->img, text, x, y);
    }
#ifndef LINUX
    else if (drw->ob_type == PPyScreenType)
    {
       PyScreen *scr = (PyScreen*)drw;

       self->fnt->drawText(scr->scr, text, x, y);
    }
#endif
    else
    {
       PyErr_SetString(PyExc_TypeError, "First argument must be an Image or a Screen");
       return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef font_methods[] = {
   { "textWidth", (PyCFunction)font_textWidth, METH_VARARGS, "" },
   { "textHeight", (PyCFunction)font_textHeight, METH_VARARGS, "" },
   { "drawText", (PyCFunction)font_drawText, METH_VARARGS, "" },

   { NULL }
};

static PyTypeObject PyFontType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "Font",
   sizeof(PyFont),
   0,
   (destructor)font_dealloc,
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
   "Font objects",
   0,
   0,
   0,
   0,
   0,
   0,
   font_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)font_init,
   0,
   font_new,
};

PyTypeObject* PPyFontType = &PyFontType;

#ifdef _GNUC
static const char* _rcsid_font __attribute__((unused)) = "$Id: font.cpp 1428 2005-11-12 17:22:44Z fraca7 $";
#endif
