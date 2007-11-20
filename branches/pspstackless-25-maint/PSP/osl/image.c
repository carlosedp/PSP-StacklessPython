
/**
 * @file image.c
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
#include "image.h"

//==========================================================================
// Init & free

typedef struct
{
      int bDelete;
      OSL_IMAGE *pImg;
      void *next;
} FREEIMG;

static FREEIMG *pCurrent = NULL;

static void image_dealloc(PyImage *self)
{
    if (self->pImg)
    {
       if (self->location == OSL_IN_VRAM)
       {
          FREEIMG *pCur = pCurrent;

          while (pCur)
          {
             if (pCur->pImg == self->pImg)
             {
                pCur->bDelete = 1;
                break;
             }

             pCur = (FREEIMG*)(pCur->next);
          }

          if (pCur)
          {
             while (pCurrent && pCurrent->bDelete) // Mmhh, short-circuit
             {
                FREEIMG *pTmp;

                oslDeleteImage(pCurrent->pImg);

                pTmp = (FREEIMG*)(pCurrent->next);
                free(pCurrent);
                pCurrent = pTmp;
             }
          }
          else
          {
             PyErr_SetString(PyExc_RuntimeError, "Internal error: VRAM image not registered");
             PyErr_Print();
          }
       }
       else
       {
          oslDeleteImage(self->pImg);
       }
    }

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* image_new(PyTypeObject *type,
                           PyObject *args,
                           PyObject *kwargs)
{
    PyImage *self;

    self = (PyImage*)type->tp_alloc(type, 0);

    if (self)
       self->pImg = NULL;

    return (PyObject*)self;
}

static int image_init(PyImage *self,
                      PyObject *args,
                      PyObject *kwargs)
{
    PyObject *initial;
    int location = OSL_IN_VRAM;
    int pf = OSL_PF_8888;

    if (!PyArg_ParseTuple(args, "O|ii:__init__", &initial, &location, &pf))
       return -1;

    self->location = location;

    if (PyString_Check(initial))
    {
       // Load from file

       char *filename = PyString_AsString(initial);

       self->pImg = oslLoadImageFile(filename, location, pf);

       if (!self->pImg)
       {
          PyErr_SetString(osl_Error, "Could not load image.");
          return -1;
       }
    }
    else if (PyTuple_Check(initial))
    {
       int w, h;

       if (PyTuple_Size(initial) != 2)
       {
          PyErr_SetString(PyExc_TypeError, "Image dimension must be a 2-tuple");
          return -1;
       }

       if (!PyInt_Check(PyTuple_GetItem(initial, 0)))
       {
          PyErr_SetString(PyExc_TypeError, "Image width must be an integer");
          return -1;
       }

       if (!PyInt_Check(PyTuple_GetItem(initial, 1)))
       {
          PyErr_SetString(PyExc_TypeError, "Image height must be an integer");
          return -1;
       }

       w = PyInt_AsLong(PyTuple_GetItem(initial, 0));
       h = PyInt_AsLong(PyTuple_GetItem(initial, 1));

       self->pImg = oslCreateImage(w, h, location, pf);

       if (!self->pImg)
       {
          PyErr_SetString(osl_Error, "Could not create image");
          return -1;
       }
    }
    else
    {
       PyErr_SetString(PyExc_TypeError, "First argument must be a filename or a 2-tuple");
       return -1;
    }

    if (location == OSL_IN_VRAM)
    {
       FREEIMG *pFree = (FREEIMG*)malloc(sizeof(FREEIMG));

       pFree->bDelete = 0;
       pFree->pImg = self->pImg;
       pFree->next = (void*)pCurrent;
       pCurrent = pFree;
    }

    return 0;
}

//==========================================================================
// Public properties

static PyObject* image_getx(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->pImg->x);
}

static int image_setx(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete x attribute");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "x attribute must be an integer");
       return -1;
    }

    self->pImg->x = PyInt_AsLong(value);

    return 0;
}

static PyObject* image_gety(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->pImg->y);
}

static int image_sety(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete y attribute");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "y attribute must be an integer");
       return -1;
    }

    self->pImg->y = PyInt_AsLong(value);

    return 0;
}

static PyObject* image_getstretchX(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->pImg->stretchX);
}

static int image_setstretchX(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete stretchX attribute");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "stretchX attribute must be an integer");
       return -1;
    }

    self->pImg->stretchX = PyInt_AsLong(value);

    return 0;
}

static PyObject* image_getstretchY(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->pImg->stretchY);
}

static int image_setstretchY(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete stretchY attribute");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "stretchY attribute must be an integer");
       return -1;
    }

    self->pImg->stretchY = PyInt_AsLong(value);

    return 0;
}

static PyObject* image_getoffsetX0(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("f", self->pImg->offsetX0);
}

static int image_setoffsetX0(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete offsetX0 attribute");
       return -1;
    }

    if (!PyFloat_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "offsetX0 attribute must be a float");
       return -1;
    }

    self->pImg->offsetX0 = PyFloat_AsDouble(value);

    return 0;
}

static PyObject* image_getoffsetY0(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("f", self->pImg->offsetY0);
}

static int image_setoffsetY0(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete offsetY0 attribute");
       return -1;
    }

    if (!PyFloat_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "offsetY0 attribute must be a float");
       return -1;
    }

    self->pImg->offsetY0 = PyFloat_AsDouble(value);

    return 0;
}

static PyObject* image_getoffsetX1(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("f", self->pImg->offsetX1);
}

static int image_setoffsetX1(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete offsetX1 attribute");
       return -1;
    }

    if (!PyFloat_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "offsetX1 attribute must be a float");
       return -1;
    }

    self->pImg->offsetX1 = PyFloat_AsDouble(value);

    return 0;
}

static PyObject* image_getoffsetY1(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("f", self->pImg->offsetY1);
}

static int image_setoffsetY1(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete offsetY1 attribute");
       return -1;
    }

    if (!PyFloat_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "offsetY1 attribute must be a float");
       return -1;
    }

    self->pImg->offsetY1 = PyFloat_AsDouble(value);

    return 0;
}

static PyObject* image_getcenterX(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->pImg->centerX);
}

static int image_setcenterX(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete centerX attribute");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "centerX attribute must be an integer");
       return -1;
    }

    self->pImg->centerX = PyInt_AsLong(value);

    return 0;
}

static PyObject* image_getcenterY(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->pImg->centerY);
}

static int image_setcenterY(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete centerY attribute");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "centerY attribute must be an integer");
       return -1;
    }

    self->pImg->centerY = PyInt_AsLong(value);

    return 0;
}

static PyObject* image_getangle(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("f", self->pImg->angle);
}

static int image_setangle(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete angle attribute");
       return -1;
    }

    if (!PyFloat_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "angle attribute must be a float");
       return -1;
    }

    self->pImg->angle = PyFloat_AsDouble(value);

    return 0;
}

static PyObject* image_getautoStrip(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->pImg->autoStrip);
}

static int image_setautoStrip(PyImage *self, PyObject *value, void *closure)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!value)
    {
       PyErr_SetString(PyExc_TypeError, "Can not delete autoStrip attribute");
       return -1;
    }

    if (!PyInt_Check(value))
    {
       PyErr_SetString(PyExc_TypeError, "autoStrip attribute must be an integer");
       return -1;
    }

    self->pImg->autoStrip = PyInt_AsLong(value);

    return 0;
}

// Read-only

static PyObject* image_getsizeX(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->pImg->sizeX);
}

static PyObject* image_getsizeY(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->pImg->sizeY);
}

static PyGetSetDef image_getset[] = {
   { "x", (getter)image_getx, (setter)image_setx, "", NULL },
   { "y", (getter)image_gety, (setter)image_sety, "", NULL },
   { "stretchX", (getter)image_getstretchX, (setter)image_setstretchX, "", NULL },
   { "stretchY", (getter)image_getstretchY, (setter)image_setstretchY, "", NULL },

   { "offsetX0", (getter)image_getoffsetX0, (setter)image_setoffsetX0, "", NULL },
   { "offsetY0", (getter)image_getoffsetY0, (setter)image_setoffsetY0, "", NULL },
   { "offsetX1", (getter)image_getoffsetX1, (setter)image_setoffsetX1, "", NULL },
   { "offsetY1", (getter)image_getoffsetY1, (setter)image_setoffsetY1, "", NULL },

   { "centerX", (getter)image_getcenterX, (setter)image_setcenterX, "", NULL },
   { "centerY", (getter)image_getcenterY, (setter)image_setcenterY, "", NULL },
   { "angle", (getter)image_getangle, (setter)image_setangle, "", NULL },

   { "autoStrip", (getter)image_getautoStrip, (setter)image_setautoStrip, "", NULL },

   // TODO: palette

   { "sizeX", (getter)image_getsizeX, NULL, "", NULL },
   { "sizeY", (getter)image_getsizeY, NULL, "", NULL },

   { NULL }
};

//==========================================================================
// Methods

static PyObject* image_copy(PyImage *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    PyImage *other;

    if (!PyArg_ParseTuple(args, "O:copy", &other))
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)other)->ob_type, PPyImageType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be an osl.Image");
       return NULL;
    }

    oslCopyImage(self->pImg, other->pImg);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_uncache(PyImage *self,
                               PyObject *args,
                               PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":uncache"))
       return NULL;

    oslUncacheImage(self->pImg);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_draw(PyImage *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":draw"))
       return NULL;

    oslDrawImage(self->pImg);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_correctHalfBorder(PyImage *self,
                                         PyObject *args,
                                         PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":correctHalfBorder"))
       return NULL;

    oslCorrectImageHalfBorder(self->pImg);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_lock(PyImage *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":lock"))
       return NULL;

    oslLockImage(self->pImg);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_unlock(PyImage *self,
                              PyObject *args,
                              PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":unlock"))
       return NULL;

    oslUnlockImage(self->pImg);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_tile(PyImage *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    int x0, y0, x1, y1;
    PyImage *ret;
    OSL_IMAGE *pImg;

    if (!PyArg_ParseTuple(args, "iiii:tile",
                          &x0, &y0, &x1, &y1))
       return NULL;

    pImg = oslCreateImageTile(self->pImg, x0, y0, x1, y1);

    if (!pImg)
    {
       PyErr_SetString(osl_Error, "Could not create tile");
       return NULL;
    }

    ret = (PyImage*)PyType_GenericNew(PPyImageType, NULL, NULL);
    ret->pImg = pImg;

    return (PyObject*)ret;
}

static PyObject* image_swizzle(PyImage *self,
                               PyObject *args,
                               PyObject *kwargs)
{
    PyImage *other;

    if (!PyArg_ParseTuple(args, "O:swizzle", &other))
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)other)->ob_type, PPyImageType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be an osl.Image");
       return NULL;
    }

    oslSwizzleImage(other->pImg, self->pImg);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_clear(PyImage *self,
                             PyObject *args,
                             PyObject *kwargs)
{
    int color;

    if (!PyArg_ParseTuple(args, "i:clear", &color))
       return NULL;

    oslClearImage(self->pImg, color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_write(PyImage *self,
                             PyObject *args,
                             PyObject *kwargs)
{
    char *filename;
    int flags = 0;

    if (!PyArg_ParseTuple(args, "s|i:write", &filename, &flags))
       return NULL;

    oslWriteImageFile(self->pImg, filename, flags);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef image_methods[] = {
   { "copy", (PyCFunction)image_copy, METH_VARARGS, "" },
   { "uncache", (PyCFunction)image_uncache, METH_VARARGS, "" },
   { "draw", (PyCFunction)image_draw, METH_VARARGS, "" },
   { "correctHalfBorder", (PyCFunction)image_correctHalfBorder, METH_VARARGS, "" },
   { "lock", (PyCFunction)image_lock, METH_VARARGS, "" },
   { "unlock", (PyCFunction)image_unlock, METH_VARARGS, "" },
   { "tile", (PyCFunction)image_tile, METH_VARARGS, "" },
   { "swizzle", (PyCFunction)image_swizzle, METH_VARARGS, "" },
   { "clear", (PyCFunction)image_clear, METH_VARARGS, "" },
   { "write", (PyCFunction)image_write, METH_VARARGS, "" },

   { NULL }
};

//==========================================================================
//

static PyTypeObject PyImageType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "osl.Image",
   sizeof(PyImage),
   0,
   (destructor)image_dealloc,
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
   "Image objects",
   0,
   0,
   0,
   0,
   0,
   0,
   image_methods,
   0,
   image_getset,
   0,
   0,
   0,
   0,
   0,
   (initproc)image_init,
   0,
   image_new,
};

PyTypeObject* PPyImageType = &PyImageType;

#ifdef _GNUC
static const char* _rcsid_image __attribute__((unused)) = "$Id$";
#endif
