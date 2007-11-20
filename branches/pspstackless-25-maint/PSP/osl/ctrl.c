
/**
 * @file ctrl.c
 */

/**********************************************************************

  Created: 17 Feb 2007

    Copyright (C) 2007 fraca7@free.fr

    This library  is free software; you can  redistribute it and/or
    modify  it under  the terms  of the  GNU Lesser  General Public
    License as  published by  the Free Software  Foundation; either
    version  2.1 of  the License,  or  (at your  option) any  later
    version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY;  without even the implied warranty of
    MERCHANTABILITY or  FITNESS FOR A PARTICULAR  PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You  should have  received a  copy  of the  GNU Lesser  General
    Public License  along with this  library; if not, write  to the
    Free  Software Foundation,  Inc., 59  Temple Place,  Suite 330,
    Boston, MA 02111-1307 USA

**********************************************************************/
// $Id$

#include "ctrl.h"

static void ctrl_dealloc(PyController *self)
{
    if (self->pCtrl)
       free(self->pCtrl);

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* ctrl_new(PyTypeObject *type,
                          PyObject *args,
                          PyObject *kwargs)
{
    PyController *self;

    self = (PyController*)type->tp_alloc(type, 0);

    if (self)
       self->pCtrl = NULL;

    return (PyObject*)self;
}

static int ctrl_init(PyController *self,
                     PyObject *args,
                     PyObject *kwargs)
{
    OSL_CONTROLLER *ctrl;

    if (!PyArg_ParseTuple(args, ":__init__"))
       return -1;

    ctrl = oslReadKeys();
    self->pCtrl = (OSL_CONTROLLER*)malloc(sizeof(*ctrl));
    memcpy(self->pCtrl, ctrl, sizeof(*ctrl));

    return 0;
}

//==========================================================================
//

#define GETKEY(type, name) \
static PyObject* ctrl_get##type##_##name(PyController *self, void *closure) \
{ \
    if (PyErr_CheckSignals()) \
       return NULL; \
    return Py_BuildValue("i", (int)self->pCtrl->type.name); \
}

#define GETATTR(name) \
static PyObject* ctrl_get##name(PyController *self, void *closure) \
{ \
    if (PyErr_CheckSignals()) \
       return NULL; \
    return Py_BuildValue("i", (int)self->pCtrl->name); \
}

GETKEY(held, select);
GETKEY(held, start);
GETKEY(held, up);
GETKEY(held, right);
GETKEY(held, down);
GETKEY(held, left);
GETKEY(held, L);
GETKEY(held, R);
GETKEY(held, triangle);
GETKEY(held, circle);
GETKEY(held, cross);
GETKEY(held, square);
GETKEY(held, home);
GETKEY(held, hold);
GETKEY(held, note);

GETKEY(pressed, select);
GETKEY(pressed, start);
GETKEY(pressed, up);
GETKEY(pressed, right);
GETKEY(pressed, down);
GETKEY(pressed, left);
GETKEY(pressed, L);
GETKEY(pressed, R);
GETKEY(pressed, triangle);
GETKEY(pressed, circle);
GETKEY(pressed, cross);
GETKEY(pressed, square);
GETKEY(pressed, home);
GETKEY(pressed, hold);
GETKEY(pressed, note);

GETATTR(autoRepeatInit);
GETATTR(autoRepeatInterval);
GETATTR(autoRepeatMask);
GETATTR(autoRepeatCounter);
GETATTR(analogX);
GETATTR(analogY);

#define DECLKEY(type, name) { #type "_" #name, (getter)ctrl_get##type##_##name, NULL, "", NULL }
#define DECLATTR(name) { #name, (getter)ctrl_get##name, NULL, "", NULL }

static PyGetSetDef ctrl_getset[] = {
   DECLKEY(held, select),
   DECLKEY(held, start),
   DECLKEY(held, up),
   DECLKEY(held, right),
   DECLKEY(held, down),
   DECLKEY(held, left),
   DECLKEY(held, L),
   DECLKEY(held, R),
   DECLKEY(held, triangle),
   DECLKEY(held, circle),
   DECLKEY(held, cross),
   DECLKEY(held, square),
   DECLKEY(held, home),
   DECLKEY(held, hold),
   DECLKEY(held, note),

   DECLKEY(pressed, select),
   DECLKEY(pressed, start),
   DECLKEY(pressed, up),
   DECLKEY(pressed, right),
   DECLKEY(pressed, down),
   DECLKEY(pressed, left),
   DECLKEY(pressed, L),
   DECLKEY(pressed, R),
   DECLKEY(pressed, triangle),
   DECLKEY(pressed, circle),
   DECLKEY(pressed, cross),
   DECLKEY(pressed, square),
   DECLKEY(pressed, home),
   DECLKEY(pressed, hold),
   DECLKEY(pressed, note),

   DECLATTR(autoRepeatInit),
   DECLATTR(autoRepeatInterval),
   DECLATTR(autoRepeatMask),
   DECLATTR(autoRepeatCounter),
   DECLATTR(analogX),
   DECLATTR(analogY),

   { NULL }
};

static PyMethodDef ctrl_methods[] = {
   { NULL }
};

static PyTypeObject PyControllerType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "osl.Controller",
   sizeof(PyController),
   0,
   (destructor)ctrl_dealloc,
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
   "Controller objects",
   0,
   0,
   0,
   0,
   0,
   0,
   ctrl_methods,
   0,
   ctrl_getset,
   0,
   0,
   0,
   0,
   0,
   (initproc)ctrl_init,
   0,
   ctrl_new,
};

PyTypeObject* PPyControllerType = &PyControllerType;

#ifdef _GNUC
static const char* _rcsid_ctrl __attribute__((unused)) = "$Id$";
#endif
