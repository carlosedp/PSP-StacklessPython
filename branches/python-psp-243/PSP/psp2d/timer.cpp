
/**
 * @file timer.cpp
 * @brief Implementation of class timer
 */

/**********************************************************************

  Created: 20 Nov 2005

**********************************************************************/
// $Id: timer.cpp 1471 2005-11-20 13:15:21Z fraca7 $

#include "timer.h"

using namespace PSP2D;

class PyListener : public PSP2D::TimerListener
{
   public:
      PyListener(PyTimer*);
      ~PyListener();

      bool fire(void);

      void saveState(void);
      void restoreState(void);

   protected:
      PyTimer* _self;
      PyObject* _args;

      PyThreadState* _state;
};

PyListener::PyListener(PyTimer *self)
    : _self(self),
      _state(NULL)
{
    Py_INCREF(_self);

    _args = PyTuple_New(0);
}

PyListener::~PyListener()
{
    Py_DECREF(_self);
    Py_DECREF(_args);
}

bool PyListener::fire()
{
    restoreState();

    PyObject *cb = PyObject_GetAttrString((PyObject*)_self, "fire");

    if (!PyCallable_Check(cb))
    {
       PyErr_SetString(PyExc_TypeError, "fire must be callable");
       PyErr_Print();

       saveState();

       return false;
    }

    Py_INCREF(cb);
    PyObject *ret = PyObject_Call(cb, _args, NULL);

    if (!ret)
    {
       PyErr_Print();
       Py_DECREF(cb);

       saveState();

       return false;
    }

    if (PyObject_IsTrue(ret))
    {
       Py_DECREF(cb);
       Py_DECREF(ret);

       saveState();

       return true;
    }

    Py_DECREF(cb);
    Py_DECREF(ret);

    saveState();

    return false;
}

void PyListener::saveState()
{
    _state = PyEval_SaveThread();
}

void PyListener::restoreState()
{
    PyEval_RestoreThread(_state);
}

static void timer_dealloc(PyTimer *self)
{
    // 'lst' is owned by the Timer

    delete self->tmr;

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* timer_new(PyTypeObject *type,
                           PyObject *args,
                           PyObject *kwargs)
{
    PyTimer *self;

    self = (PyTimer*)type->tp_alloc(type, 0);

    if (self)
    {
       self->tmr = NULL;
       self->lst = NULL;
    }

    return (PyObject*)self;
}

static int timer_init(PyTimer *self,
                      PyObject *args,
                      PyObject *kwargs)
{
    int tmo;

    if (!PyArg_ParseTuple(args, "i", &tmo))
       return -1;

    self->lst = new PyListener(self);
    self->tmr = new Timer((u32)tmo, self->lst);

    return 0;
}

static PyObject* timer_fire(PyTimer *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

    PyErr_SetString(PyExc_NotImplementedError, "Not implemented");
    return NULL;
}

static PyObject* timer_run(PyTimer *self,
                           PyObject *args,
                           PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ""))
       return NULL;

    self->lst->saveState();
    self->tmr->run();
    self->lst->restoreState();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef timer_methods[] = {
   { "fire", (PyCFunction)timer_fire, METH_VARARGS, "" },
   { "run", (PyCFunction)timer_run, METH_VARARGS, "" },

   { NULL }
};

static PyTypeObject PyTimerType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "psp2d.Timer",
   sizeof(PyTimer),
   0,
   (destructor)timer_dealloc,
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
   "Timer objects",
   0,
   0,
   0,
   0,
   0,
   0,
   timer_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)timer_init,
   0,
   timer_new,
};

PyTypeObject* PPyTimerType = &PyTimerType;

static const char* _rcsid_timer __attribute__((unused)) = "$Id: timer.cpp 1471 2005-11-20 13:15:21Z fraca7 $";
