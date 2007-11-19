
/**
 * @file font.c
 */

/**********************************************************************

  Created: 24 Feb 2007

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

#include "font.h"
#include "osl.h"

static PyFont *osl_py_font = NULL;

//==========================================================================
// Init & free

static void font_dealloc(PyFont *self)
{
    if (self->pFont)
       oslDeleteFont(self->pFont);

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* font_new(PyTypeObject *type,
                          PyObject *args,
                          PyObject *kwargs)
{
    PyFont *self;

    self = (PyFont*)type->tp_alloc(type, 0);

    if (self)
       self->pFont = NULL;

    return (PyObject*)self;
}

static int font_init(PyFont *self,
                     PyObject *args,
                     PyObject *kwargs)
{
    char *filename;

    if (!PyArg_ParseTuple(args, "s:__init__", &filename))
       return -1;

    self->pFont = oslLoadFontFile(filename);

    if (!self->pFont)
    {
       PyErr_SetString(osl_Error, "Could not load font file");
       return -1;
    }

    return 0;
}

//==========================================================================
// Methods

static PyObject* font_set(PyFont *self,
                          PyObject *args,
                          PyObject *kwargs)
{
    Py_XDECREF(osl_py_font);

    oslSetFont(self->pFont);
    osl_py_font = self;
    Py_INCREF(self);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef font_methods[] = {
   { "set", (PyCFunction)font_set, METH_VARARGS, "" },

   { NULL }
};

static PyTypeObject PyFontType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "osl.Font",
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
static const char* _rcsid_font __attribute__((unused)) = "$Id$";
#endif
