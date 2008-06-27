
/**
 * @file filters.cpp
 */

/**********************************************************************

  Created: 02 Dec 2007

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
// $Id: filters.cpp 157 2008-03-16 12:12:25Z fraca7 $

#include "filters.h"
#include "image.h"

#include <imaging/ColourFilters.h>
#include <imaging/Callback.h>

using namespace Imaging;

class PythonCallback : public Callback
{
   public:
      PythonCallback(PyObject *pCallback)
          : m_pState(PyThreadState_Get()),
            m_pCallback(pCallback)
      {
          Py_XINCREF(m_pCallback);

          PyEval_SaveThread();
      };

      virtual ~PythonCallback()
      {
          PyEval_RestoreThread(m_pState);

          Py_XDECREF(m_pCallback);
      };

      bool update(int v, int t)
      {
          if (m_pCallback)
          {
             PyObject *ret;
             bool result = false;

             PyEval_RestoreThread(m_pState);

             ret = PyObject_CallFunction(m_pCallback, "ii", v, t);
             result = PyObject_IsTrue(ret);
             Py_XDECREF(ret);

             if (!ret)
             {
                PyErr_Print();
             }

             PyEval_SaveThread();

             return result;
          }

          return true;
      };

   private:
      PyThreadState* m_pState;
      PyObject* m_pCallback;
};


//==========================================================================
// StructuringElement

static void se_dealloc(PyStructuringElement *self)
{
    delete self->pSE;

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* se_new(PyTypeObject *type,
                            PyObject *args,
                            PyObject *kwargs)
{
    PyStructuringElement *self;

    if (PyErr_CheckSignals())
       return NULL;

    self = (PyStructuringElement*)type->tp_alloc(type, 0);

    if (self)
       self->pSE = 0;

    return (PyObject*)self;
}

static int se_init(PyStructuringElement *self,
                        PyObject *args,
                        PyObject *kwargs)
{
    PyObject *pParam;

    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, "O:__init__", &pParam))
       return -1;

    if (PyInt_Check(pParam))
    {
       self->pSE = new StructuringElement(PyInt_AsLong(pParam));
    }
    else if (PyList_Check(pParam))
    {
       int k = 1, sz = PyList_Size(pParam);

       while (k*k < sz)
          ++k;

       if (k*k != sz)
       {
          PyErr_SetString(PyExc_TypeError, "List size must be a square.");
          return -1;
       }

       bool* t = new bool[sz];

       for (int i = 0; i < sz; ++i)
       {
          t[i] = (bool)PyObject_IsTrue(PyList_GetItem(pParam, i));
       }

       self->pSE = new StructuringElement(t, k);

       delete[] t;
    }
    else
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be a list or an integer");

       return -1;
    }

    return 0;
}

static PyMethodDef se_methods[] = {
   { NULL }
};

static PyTypeObject PyStructuringElementType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "StructuringElement",
   sizeof(PyStructuringElement),
   0,
   (destructor)se_dealloc,
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
   "StructuringElement objects",
   0,
   0,
   0,
   0,
   0,
   0,
   se_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)se_init,
   0,
   se_new,
};

PyTypeObject* PPyStructuringElementType = &PyStructuringElementType;


//==========================================================================
// Filters: common methods

static void filter_dealloc(PyFilter *self)
{
    delete self->pFilter;

    self->ob_type->tp_free((PyObject*)self);
}

static void ipfilter_dealloc(PyInPlaceFilter *self)
{
    delete self->pFilter;

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* filter_new(PyTypeObject *type,
                            PyObject *args,
                            PyObject *kwargs)
{
    PyFilter *self;

    if (PyErr_CheckSignals())
       return NULL;

    self = (PyFilter*)type->tp_alloc(type, 0);

    if (self)
       self->pFilter = 0;

    return (PyObject*)self;
}

static PyObject* ipfilter_new(PyTypeObject *type,
                              PyObject *args,
                              PyObject *kwargs)
{
    PyInPlaceFilter *self;

    if (PyErr_CheckSignals())
       return NULL;

    self = (PyInPlaceFilter*)type->tp_alloc(type, 0);

    if (self)
       self->pFilter = 0;

    return (PyObject*)self;
}

static PyObject* filter_apply(PyFilter *self,
                              PyObject *args)
{
    PyImage *pImg, *pRet;
    Image *pResult;
    PyObject *cb = NULL;
    PythonCallback *pyCb;

    if (!PyArg_ParseTuple(args, "O|O:apply", &pImg, &cb))
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)pImg)->ob_type, PPyImageType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be an Image.");
       return NULL;
    }

    if (cb)
    {
       if (!PyCallable_Check(cb))
       {
          PyErr_SetString(PyExc_TypeError, "Callback must be a callable");
          return NULL;
       }
    }

    Py_INCREF((PyObject*)pImg);

    Py_INCREF(self);
    pyCb = new PythonCallback(cb);
    pResult = self->pFilter->apply(pImg->img, pyCb);
    delete pyCb;
    Py_DECREF(self);

    Py_DECREF((PyObject*)pImg);

    if (pResult)
    {
       pRet = (PyImage*)PyType_GenericNew(PPyImageType, NULL, NULL);
       pRet->img = pResult;
    }
    else
    {
       PyErr_SetString(PyExc_RuntimeError, "Error while apply()ing"); // FIXME: specific error
       pRet = NULL;
    }

    return (PyObject*)pRet;
}

static PyObject* ipfilter_apply(PyInPlaceFilter *self,
                                PyObject *args)
{
    PyImage *pImg;
    PyObject *cb = NULL;
    PythonCallback *pyCb = NULL;

    if (!PyArg_ParseTuple(args, "O|O:apply", &pImg, &cb))
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)pImg)->ob_type, PPyImageType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be an Image.");
       return NULL;
    }

    if (cb)
    {
       if (!PyCallable_Check(cb))
       {
          PyErr_SetString(PyExc_TypeError, "Callback must be a callable");
          return NULL;
       }
    }

    Py_INCREF((PyObject*)pImg);

    Py_INCREF(self);
    pyCb = new PythonCallback(cb);
    self->pFilter->apply(pImg->img, pyCb);
    delete pyCb;
    Py_DECREF(self);

    Py_DECREF((PyObject*)pImg);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef filter_methods[] = {
   { "apply", (PyCFunction)filter_apply, METH_VARARGS, "" },

   { NULL }
};

static PyMethodDef ipfilter_methods[] = {
   { "apply", (PyCFunction)ipfilter_apply, METH_VARARGS, "" },

   { NULL }
};


//==========================================================================
// Filters: specific

static int mfilter_init(PyFilter *self,
                        PyObject *args,
                        PyObject *kwargs)
{
    int t = 128;

    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, "i:__init__", &t))
       return -1;

    if ((t < 0) || (t > 255))
    {
       PyErr_SetString(PyExc_ValueError, "Threshold must be in the range [0, 255]");
       return -1;
    }

    self->pFilter = new MonochromeFilter((unsigned char)t);

    return 0;
}

static PyTypeObject PyMonochromeFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "MonochromeFilter",
   sizeof(PyFilter),
   0,
   (destructor)filter_dealloc,
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
   "MonochromeFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   filter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)mfilter_init,
   0,
   filter_new,
};

PyTypeObject* PPyMonochromeFilterType = &PyMonochromeFilterType;


//==========================================================================

static int mipfilter_init(PyInPlaceFilter *self,
                          PyObject *args,
                          PyObject *kwargs)
{
    int t = 128;

    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, "|i:__init__", &t))
       return -1;

    if ((t < 0) || (t > 255))
    {
       PyErr_SetString(PyExc_ValueError, "Threshold must be in the range [0, 255]");
       return -1;
    }

    self->pFilter = new MonochromeInPlaceFilter((unsigned char)t);

    return 0;
}

static PyTypeObject PyMonochromeInPlaceFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "MonochromeInPlaceFilter",
   sizeof(PyInPlaceFilter),
   0,
   (destructor)ipfilter_dealloc,
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
   "MonochromeInPlaceFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   ipfilter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)mipfilter_init,
   0,
   ipfilter_new,
};

PyTypeObject* PPyMonochromeInPlaceFilterType = &PyMonochromeInPlaceFilterType;


//==========================================================================

static int gfilter_init(PyFilter *self,
                        PyObject *args,
                        PyObject *kwargs)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, ":__init__"))
       return -1;

    self->pFilter = new GrayFilter();

    return 0;
}

static PyTypeObject PyGrayFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "GrayFilter",
   sizeof(PyFilter),
   0,
   (destructor)filter_dealloc,
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
   "GrayFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   filter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)gfilter_init,
   0,
   filter_new,
};

PyTypeObject* PPyGrayFilterType = &PyGrayFilterType;


//==========================================================================

static int gipfilter_init(PyInPlaceFilter *self,
                          PyObject *args,
                          PyObject *kwargs)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, ":__init__"))
       return -1;

    self->pFilter = new GrayInPlaceFilter();

    return 0;
}

static PyTypeObject PyGrayInPlaceFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "GrayInPlaceFilter",
   sizeof(PyInPlaceFilter),
   0,
   (destructor)ipfilter_dealloc,
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
   "GrayInPlaceFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   ipfilter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)gipfilter_init,
   0,
   ipfilter_new,
};

PyTypeObject* PPyGrayInPlaceFilterType = &PyGrayInPlaceFilterType;


//==========================================================================

static int efilter_init(PyFilter *self,
                        PyObject *args,
                        PyObject *kwargs)
{
    PyStructuringElement *pParam;

    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, "O:__init__", &pParam))
       return -1;

    if (!PyType_IsSubtype(((PyObject*)pParam)->ob_type, PPyStructuringElementType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be a StructuringElement");
       return -1;
    }

    self->pFilter = new ErosionFilter(*pParam->pSE);

    return 0;
}

static PyTypeObject PyErosionFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "ErosionFilter",
   sizeof(PyFilter),
   0,
   (destructor)filter_dealloc,
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
   "ErosionFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   filter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)efilter_init,
   0,
   filter_new,
};

PyTypeObject* PPyErosionFilterType = &PyErosionFilterType;


//==========================================================================

static int dfilter_init(PyFilter *self,
                        PyObject *args,
                        PyObject *kwargs)
{
    PyStructuringElement *pParam;

    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, "O:__init__", &pParam))
       return -1;

    if (!PyType_IsSubtype(((PyObject*)pParam)->ob_type, PPyStructuringElementType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be a StructuringElement");
       return -1;
    }

    self->pFilter = new DilationFilter(*pParam->pSE);

    return 0;
}

static PyTypeObject PyDilationFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "DilationFilter",
   sizeof(PyFilter),
   0,
   (destructor)filter_dealloc,
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
   "DilationFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   filter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)dfilter_init,
   0,
   filter_new,
};

PyTypeObject* PPyDilationFilterType = &PyDilationFilterType;

//==========================================================================
//

static int sfilter_init(PyFilter *self,
                        PyObject *args,
                        PyObject *kwargs)
{
    int w, h;

    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, "ii:__init__", &w, &h))
       return -1;

    self->pFilter = new ScaleFilter(w, h);

    return 0;
}

static PyTypeObject PyScaleFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "ScaleFilter",
   sizeof(PyFilter),
   0,
   (destructor)filter_dealloc,
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
   "ScaleFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   filter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)sfilter_init,
   0,
   filter_new,
};

PyTypeObject* PPyScaleFilterType = &PyScaleFilterType;

//==========================================================================
//

static int subfilter_init(PyFilter *self,
                          PyObject *args,
                          PyObject *kwargs)
{
    int x, y, w, h;

    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, "iiii:__init__", &x, &y, &w, &h))
       return -1;

    self->pFilter = new SubFilter(x, y, w, h);

    return 0;
}

static PyTypeObject PySubFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "SubFilter",
   sizeof(PyFilter),
   0,
   (destructor)filter_dealloc,
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
   "SubFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   filter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)subfilter_init,
   0,
   filter_new,
};

PyTypeObject* PPySubFilterType = &PySubFilterType;

//==========================================================================
//

static int sobelfilter_init(PyFilter *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, ":__init__"))
       return -1;

    self->pFilter = new SobelFilter();

    return 0;
}

static PyTypeObject PySobelFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "SobelFilter",
   sizeof(PyFilter),
   0,
   (destructor)filter_dealloc,
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
   "SobelFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   filter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)sobelfilter_init,
   0,
   filter_new,
};

PyTypeObject* PPySobelFilterType = &PySobelFilterType;

//==========================================================================
//

static int connectedfilter_init(PyFilter *self,
                                PyObject *args,
                                PyObject *kwargs)
{
    int x, y;

    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, "ii:__init__", &x, &y))
       return -1;

    self->pFilter = new ConnectedFilter(x, y);

    return 0;
}

static PyTypeObject PyConnectedFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "ConnectedFilter",
   sizeof(PyFilter),
   0,
   (destructor)filter_dealloc,
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
   "ConnectedFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   filter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)connectedfilter_init,
   0,
   filter_new,
};

PyTypeObject* PPyConnectedFilterType = &PyConnectedFilterType;

//==========================================================================
//

static void differencefilter_dealloc(PyDifferenceFilter *self)
{
    delete self->pFilter;

    Py_DECREF(self->pImg);

    self->ob_type->tp_free((PyObject*)self);
}

static int differencefilter_init(PyDifferenceFilter *self,
                                 PyObject *args,
                                 PyObject *kwargs)
{
    PyImage *pImg;

    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, "O:__init__", &pImg))
       return -1;

    if (!PyType_IsSubtype(((PyObject*)pImg)->ob_type, PPyImageType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be a PyImage");
       return -1;
    }

    self->pFilter = new DifferenceFilter(pImg->img);
    self->pImg = pImg;

    Py_INCREF(pImg);

    return 0;
}

static PyTypeObject PyDifferenceFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "DifferenceFilter",
   sizeof(PyDifferenceFilter),
   0,
   (destructor)differencefilter_dealloc,
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
   "DifferenceFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   filter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)differencefilter_init,
   0,
   filter_new,
};

PyTypeObject* PPyDifferenceFilterType = &PyDifferenceFilterType;

//==========================================================================
//

static void differenceipfilter_dealloc(PyDifferenceInPlaceFilter *self)
{
    delete self->pFilter;

    Py_DECREF(self->pImg);

    self->ob_type->tp_free((PyObject*)self);
}

static int differenceipfilter_init(PyDifferenceInPlaceFilter *self,
                                   PyObject *args,
                                   PyObject *kwargs)
{
    PyImage *pImg;

    if (PyErr_CheckSignals())
       return -1;

    if (!PyArg_ParseTuple(args, "O:__init__", &pImg))
       return -1;

    if (!PyType_IsSubtype(((PyObject*)pImg)->ob_type, PPyImageType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be a PyImage");
       return -1;
    }

    self->pFilter = new DifferenceInPlaceFilter(pImg->img);
    self->pImg = pImg;

    Py_INCREF(pImg);

    return 0;
}

static PyTypeObject PyDifferenceInPlaceFilterType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "DifferenceInPlaceFilter",
   sizeof(PyDifferenceInPlaceFilter),
   0,
   (destructor)differenceipfilter_dealloc,
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
   "DifferenceInPlaceFilter objects",
   0,
   0,
   0,
   0,
   0,
   0,
   ipfilter_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)differenceipfilter_init,
   0,
   ipfilter_new,
};

PyTypeObject* PPyDifferenceInPlaceFilterType = &PyDifferenceInPlaceFilterType;

#ifdef _GNUC
static const char* _rcsid_filters __attribute__((unused)) = "$Id: filters.cpp 157 2008-03-16 12:12:25Z fraca7 $";
#endif
