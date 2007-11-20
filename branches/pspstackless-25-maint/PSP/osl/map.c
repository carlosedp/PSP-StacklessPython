
/**
 * @file map.c
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

#include "osl.h"
#include "map.h"
#include "image.h"

static void map_dealloc(PyMap *self)
{
    // There is no oslMapDelete!!!

    Py_XDECREF(self->pImg);

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* map_new(PyTypeObject *type,
                         PyObject *args,
                         PyObject *kwargs)
{
    PyMap *self;

    self = (PyMap*)type->tp_alloc(type, 0);

    if (self)
    {
       self->pMap = NULL;
       self->pImg = NULL;
    }

    return (PyObject*)self;
}

static int map_init(PyMap *self,
                    PyObject *args,
                    PyObject *kwargs)
{
    PyImage *img;
    PyObject *data;
    int tileX, tileY;
    unsigned short *dataptr, *ptr;
    int i;
    int width = 0, height;

    if (!PyArg_ParseTuple(args, "OOii:__init__",
                          &img, &data, &tileX, &tileY))
       return -1;

    if (!PyType_IsSubtype(((PyObject*)img)->ob_type, PPyImageType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument 1 must be an osl.Image");
       return -1;
    }

    if (!PyList_Check(data))
    {
       PyErr_SetString(PyExc_TypeError, "Argument 2 must be a list");
       return -1;
    }

    height = PyList_Size(data);

    for (i = 0; i < PyList_Size(data); ++i)
    {
       if (!PyList_Check(PyList_GetItem(data, i)))
       {
          PyErr_SetString(PyExc_TypeError, "List elements must be lists");
          return -1;
       }

       width = PyList_Size(PyList_GetItem(data, i));
    }

    for (i = 0;  i < PyList_Size(data); ++i)
    {
       int j;

       if (PyList_Size(PyList_GetItem(data, i)) != width)
       {
          PyErr_SetString(PyExc_TypeError, "Non uniform map width");
          return -1;
       }

       for (j = 0; j < PyList_Size(PyList_GetItem(data, i)); ++j)
       {
          if (!PyInt_Check(PyList_GetItem(PyList_GetItem(data, i), j)))
          {
             PyErr_SetString(PyExc_TypeError, "Array elements must be integers");
             return -1;
          }
       }
    }

    dataptr = (unsigned short*)malloc(sizeof(unsigned short) * width * height);
    ptr = dataptr;

    for (i = 0; i < PyList_Size(data); ++i)
    {
       int j;

       for (j = 0; j < PyList_Size(PyList_GetItem(data, i)); ++j)
       {
          *(ptr++) = (unsigned short)PyInt_AsLong(PyList_GetItem(PyList_GetItem(data, i), j));
       }
    }

    self->pMap = oslCreateMap(img->pImg, (void*)dataptr, tileX, tileY, width, height, OSL_MF_U16);

    if (!self->pMap)
    {
       PyErr_SetString(osl_Error, "Could not create map");
       return -1;
    }

    self->pImg = (PyObject*)img;
    Py_INCREF(self->pImg);

    return 0;
}

//==========================================================================
//

static PyObject* map_getscrollX(PyMap *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->pMap->scrollX);
}

static int map_setscrollX(PyMap *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete scrollX attribute");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "scrollX attribute must be an integer");
       return -1;
    }

    self->pMap->scrollX = PyInt_AsLong(value);

    return 0;
}

static PyObject* map_getscrollY(PyMap *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->pMap->scrollY);
}

static int map_setscrollY(PyMap *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete scrollY attribute");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "scrollY attribute must be an integer");
       return -1;
    }

    self->pMap->scrollY = PyInt_AsLong(value);

    return 0;
}

static PyGetSetDef map_getset[] = {
   { "scrollX", (getter)map_getscrollX, (setter)map_setscrollX, "", NULL },
   { "scrollY", (getter)map_getscrollY, (setter)map_setscrollY, "", NULL },

   { NULL }
};

//==========================================================================
//

static PyObject* map_draw(PyMap *self,
                          PyObject *args,
                          PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":draw"))
       return NULL;

    oslDrawMap(self->pMap);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* map_drawSimple(PyMap *self,
                                PyObject *args,
                                PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":drawSimple"))
       return NULL;

    oslDrawMapSimple(self->pMap);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef map_methods[] = {
   { "draw", (PyCFunction)map_draw, METH_VARARGS, "" },
   { "drawSimple", (PyCFunction)map_drawSimple, METH_VARARGS, "" },

   { NULL }
};

static PyTypeObject PyMapType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "osl.Map",
   sizeof(PyMap),
   0,
   (destructor)map_dealloc,
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
   "Map objects",
   0,
   0,
   0,
   0,
   0,
   0,
   map_methods,
   0,
   map_getset,
   0,
   0,
   0,
   0,
   0,
   (initproc)map_init,
   0,
   map_new,
};

PyTypeObject* PPyMapType = &PyMapType;

#ifdef _GNUC
static const char* _rcsid_map __attribute__((unused)) = "$Id$";
#endif
