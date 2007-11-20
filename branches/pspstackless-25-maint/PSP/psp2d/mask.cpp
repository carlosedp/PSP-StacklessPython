
/**
 * @file mask.c
 */

/**********************************************************************

  Created: 01 Nov 2005

**********************************************************************/
// $Id: mask.cpp 1432 2005-11-13 05:42:20Z fraca7 $

#include <malloc.h>
#include <math.h>

#include "mask.h"
#include "image.h"

using namespace PSP2D;

static void mask_dealloc(PyMask *self)
{
    delete self->msk;

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* mask_new(PyTypeObject *type,
                          PyObject *args,
                          PyObject *kwargs)
{
    PyMask *self;

    self = (PyMask*)type->tp_alloc(type, 0);

    if (self)
       self->msk = 0;

    return (PyObject*)self;
}

static int mask_init(PyMask *self,
                      PyObject *args,
                      PyObject *kwargs)
{
    PyImage *img;
    int x, y, w, h, t = 127;

    if (!PyArg_ParseTuple(args, "Oiiii|i", &img, &x, &y, &w, &h, &t))
       return -1;

#ifdef CHEKTYPE
    if (((PyObject*)img)->ob_type != PPyImageType)
    {
       PyErr_SetString(PyExc_TypeError, "First argument must be a Image.");
       return -1;
    }
#endif

    self->msk = new Mask(img->img, x, y, w, h, t);

    return 0;
}

static PyObject* mask_collide(PyMask *self,
                              PyObject *args,
                              PyObject *kwargs)
{
    PyMask *other;

    if (!PyArg_ParseTuple(args, "O", &other))
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)other)->ob_type, PPyMaskType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be a Mask.");
       return NULL;
    }

    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->msk->collide(other->msk));
}

static PyObject* mask_union(PyMask *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    PyMask *other;

    if (!PyArg_ParseTuple(args, "O", &other))
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)other)->ob_type, PPyMaskType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be a Mask.");
       return NULL;
    }

    if (PyErr_CheckSignals())
       return NULL;

    self->msk->set(other->msk);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* mask_isIn(PyMask *self,
                           PyObject *args,
                           PyObject *kwargs)
{
    int x, y;

    if (!PyArg_ParseTuple(args, "ii", &x, &y))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (self->msk->isSet(x, y))
    {
       Py_INCREF(Py_True);
       return Py_True;
    }

    Py_INCREF(Py_False);
    return Py_False;
}

static PyMethodDef mask_methods[] = {
   { "collide", (PyCFunction)mask_collide, METH_VARARGS, "" },
   { "union", (PyCFunction)mask_union, METH_VARARGS, "" },
   { "isIn", (PyCFunction)mask_isIn, METH_VARARGS, "" },

   { NULL }
};

static PyTypeObject PyMaskType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "psp2d.Mask",
   sizeof(PyMask),
   0,
   (destructor)mask_dealloc,
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
   "Mask objects",
   0,
   0,
   0,
   0,
   0,
   0,
   mask_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)mask_init,
   0,
   mask_new,
};

PyTypeObject* PPyMaskType = &PyMaskType;

#ifdef _GNUC
static const char* _rcsid_mask __attribute__((unused)) = "$Id: mask.cpp 1432 2005-11-13 05:42:20Z fraca7 $";
#endif
