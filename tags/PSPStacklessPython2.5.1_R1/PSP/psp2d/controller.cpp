
/**
 * @file controller.c
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: controller.cpp 1439 2005-11-13 08:47:24Z fraca7 $

#include "controller.h"

using namespace PSP2D;

static void controller_dealloc(PyController *self)
{
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* controller_new(PyTypeObject *type,
                                PyObject *args,
                                PyObject *kwargs)
{
    PyController *self;

    if (PyErr_CheckSignals())
       return NULL;

    self = (PyController*)type->tp_alloc(type, 0);

    if (self)
       self->ctrl = NULL;

    return (PyObject*)self;
}

static int controller_init(PyController *self,
                           PyObject *args,
                           PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":__init__"))
       return -1;

    if (PyErr_CheckSignals())
       return -1;

    self->ctrl = new Controller();

    return 0;
}

static int controller_cmp(PyController *a, PyController *b)
{
    if ((a->ob_type != PPyControllerType) || (b->ob_type != PPyControllerType))
       return 1;

    if (*(a->ctrl) == *(b->ctrl))
       return 0;

    return 1;
}

static PyObject* controller_getanalogx(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->ctrl->analogX());
}

static PyObject* controller_getanalogy(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->ctrl->analogY());
}

static PyObject* controller_getsquare(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->square());
}

static PyObject* controller_gettriangle(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->triangle());
}

static PyObject* controller_getcircle(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->circle());
}

static PyObject* controller_getcross(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->cross());
}

static PyObject* controller_getup(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->up());
}

static PyObject* controller_getdown(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->down());
}

static PyObject* controller_getleft(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->left());
}

static PyObject* controller_getright(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->right());
}

static PyObject* controller_getstart(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->start());
}

static PyObject* controller_getselect(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->select());
}

static PyObject* controller_getl(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->l());
}

static PyObject* controller_getr(PyController *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", (int)self->ctrl->r());
}

static PyGetSetDef controller_getset[] = {
   { "analogX", (getter)controller_getanalogx, NULL, "Analog pad X", NULL },
   { "analogY", (getter)controller_getanalogy, NULL, "Analog pad Y", NULL },
   { "square", (getter)controller_getsquare, NULL, "Square button.", NULL },
   { "triangle", (getter)controller_gettriangle, NULL, "Triangle button.", NULL },
   { "circle", (getter)controller_getcircle, NULL, "Circle button.", NULL },
   { "cross", (getter)controller_getcross, NULL, "Cross button.", NULL },
   { "up", (getter)controller_getup, NULL, "Up button.", NULL },
   { "down", (getter)controller_getdown, NULL, "Down button.", NULL },
   { "left", (getter)controller_getleft, NULL, "Left button.", NULL },
   { "right", (getter)controller_getright, NULL, "Right button.", NULL },
   { "start", (getter)controller_getstart, NULL, "Start button.", NULL },
   { "select", (getter)controller_getselect, NULL, "Select button.", NULL },
   { "l", (getter)controller_getl, NULL, "Left trigger.", NULL },
   { "r", (getter)controller_getr, NULL, "Right trigger.", NULL },

   { NULL }
};

static PyMethodDef controller_methods[] = {
   { NULL }
};

static PyTypeObject PyControllerType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "psp2d.Controller",
   sizeof(PyController),
   0,
   (destructor)controller_dealloc,
   0,
   0,
   0,
   (cmpfunc)controller_cmp,
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
   "Controller objects",
   0,
   0,
   0,
   0,
   0,
   0,
   controller_methods,
   0,
   controller_getset,
   0,
   0,
   0,
   0,
   0,
   (initproc)controller_init,
   0,
   controller_new,
};

PyTypeObject* PPyControllerType = &PyControllerType;

#ifdef _GNUC
static const char* _rcsid_controller __attribute__((unused)) = "$Id: controller.cpp 1439 2005-11-13 08:47:24Z fraca7 $";
#endif
