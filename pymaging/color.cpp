
/**
 * @file color.c
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: color.cpp 1473 2005-11-20 14:45:59Z fraca7 $

#include "color.h"

static void color_dealloc(PyColor *self)
{
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* color_new(PyTypeObject *type,
                           PyObject *args,
                           PyObject *kwargs)
{
    PyColor *self;

    if (PyErr_CheckSignals())
       return NULL;

    self = (PyColor*)type->tp_alloc(type, 0);
    if (self)
       self->color = 0;

    return (PyObject*)self;
}

static int color_init(PyColor *self,
                      PyObject *args,
                      PyObject *kwargs)
{
    int r, g, b, a = 255;

    if (!PyArg_ParseTuple(args, "iii|i:__init__", &r, &g, &b, &a))
       return -1;

    if (PyErr_CheckSignals())
       return -1;

    self->color = ((u32)a << 24) | ((u32)b << 16) | ((u32)g << 8) | (u32)r;

    return 0;
}

static PyObject* color_getr(PyColor *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->color & 0xFF);
}

static int color_setr(PyColor *self, PyObject *value, void *closure)
{
    int v;

    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Cannot delete red component");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "Red component must be an integer.");
       return -1;
    }

    v = PyInt_AsLong(value);

    if ((v < 0) || (v > 255))
    {
       PyErr_SetString(PyExc_ValueError, "Red component must be between 0 and 255");
       return -1;
    }

    self->color = (self->color & 0xFFFFFF00U) | (u32)v;

    return 0;
}

static PyObject* color_getg(PyColor *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (self->color >> 8) & 0xFF);
}

static int color_setg(PyColor *self, PyObject *value, void *closure)
{
    int v;

    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Cannot delete green component");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "Green component must be an integer.");
       return -1;
    }

    v = PyInt_AsLong(value);

    if ((v < 0) || (v > 255))
    {
       PyErr_SetString(PyExc_ValueError, "Green component must be between 0 and 255");
       return -1;
    }

    self->color = (self->color & 0xFFFF00FFU) | ((u32)v << 8);

    return 0;
}

static PyObject* color_getb(PyColor *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (self->color >> 16) & 0xFF);
}

static int color_setb(PyColor *self, PyObject *value, void *closure)
{
    int v;

    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Cannot delete blue component");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "Blue component must be an integer.");
       return -1;
    }

    v = PyInt_AsLong(value);

    if ((v < 0) || (v > 255))
    {
       PyErr_SetString(PyExc_ValueError, "Blue component must be between 0 and 255");
       return -1;
    }

    self->color = (self->color & 0xFF00FFFFU) | ((u32)v << 16);

    return 0;
}

static PyObject* color_geta(PyColor *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (self->color >> 24) & 0xFF);
}

static int color_seta(PyColor *self, PyObject *value, void *closure)
{
    int v;

    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Cannot delete alpha component");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "Alpha component must be an integer.");
       return -1;
    }

    v = PyInt_AsLong(value);

    if ((v < 0) || (v > 255))
    {
       PyErr_SetString(PyExc_ValueError, "Alpha component must be between 0 and 255");
       return -1;
    }

    self->color = (self->color & 0x00FFFFFFU) | ((u32)v << 24);

    return 0;
}

static PyGetSetDef color_getset[] = {
   { "red", (getter)color_getr, (setter)color_setr, "Red component", NULL },
   { "green", (getter)color_getg, (setter)color_setg, "Green component", NULL },
   { "blue", (getter)color_getb, (setter)color_setb, "Blue component", NULL },
   { "alpha", (getter)color_geta, (setter)color_seta, "Alpha component", NULL },

   { NULL }
};

static PyMethodDef color_methods[] = {
   { NULL }
};

static PyTypeObject PyColorType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "Color",
   sizeof(PyColor),
   0,
   (destructor)color_dealloc,
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
   "Color objects",
   0,
   0,
   0,
   0,
   0,
   0,
   color_methods,
   0,
   color_getset,
   0,
   0,
   0,
   0,
   0,
   (initproc)color_init,
   0,
   color_new,
};

PyTypeObject* PPyColorType = &PyColorType;

#ifdef _GNUC
static const char* _rcsid_color __attribute__((unused)) = "$Id: color.cpp 1473 2005-11-20 14:45:59Z fraca7 $";
#endif
