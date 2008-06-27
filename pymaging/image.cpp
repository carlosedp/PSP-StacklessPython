/**
 * @file image.c
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: image.cpp 1547 2005-12-09 12:25:07Z fraca7 $

#include <png.h>
#include <malloc.h>

#include "image.h"
#include "color.h"
#include "font.h"

#include "scoped.h"

#ifndef LINUX
#include "screen.h"
#endif

using namespace Imaging;
using namespace std;

extern PyObject* PymagingError;

static void image_dealloc(PyImage *self)
{
    delete self->img;

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* image_new(PyTypeObject *type,
                           PyObject *args,
                           PyObject *kwargs)
{
    PyImage *self;

    if (PyErr_CheckSignals())
       return NULL;

    self = (PyImage*)type->tp_alloc(type, 0);

    if (self)
       self->img = NULL;

    return (PyObject*)self;
}

static int image_init(PyImage *self,
                      PyObject *args,
                      PyObject *kwargs)
{
    PyObject *a, *b = NULL;

    if (!PyArg_ParseTuple(args, "O|O:__init__", &a, &b))
       return -1;

    if (PyErr_CheckSignals())
       return -1;

    if (a->ob_type == PPyImageType)
    {
       PyImage *other = (PyImage*)a;

       try
       {
	 ScopedThreadSaver s;

          self->img = new Image(*other->img);
       }
       catch (ImageException& e)
       {
          PyErr_SetString(PymagingError, e.getMessage().c_str());
          return -1;
       }
    }
    // If only 1 argument was given and it is not an Image, assume it is a filename.
    else if (!b)
    {
       if (!PyString_Check(a))
       {
          PyErr_SetString(PyExc_TypeError, "Argument must be a string");
          return -1;
       }

       char *filename = PyString_AsString(a);

       try
       {
	 ScopedThreadSaver s;

          self->img = new Image(filename);
       }
       catch (ImageIOException)
       {
          PyErr_SetString(PyExc_IOError, "Could not open file");
          return -1;
       }
       catch (ImageException& e)
       {
          PyErr_SetString(PymagingError, e.getMessage().c_str());
          return -1;
       }
    }
    else
    {
       // Create empty image
       int w, h;

       if (!PyInt_Check(a) || !PyInt_Check(b))
       {
          PyErr_SetString(PyExc_TypeError, "Arguments must be integers");
          return -1;
       }

       w = PyInt_AsLong(a);
       h = PyInt_AsLong(b);

       try
       {
	 ScopedThreadSaver s;

          self->img = new Image(w, h);
       }
       catch (ImageException& e)
       {
          PyErr_SetString(PymagingError, e.getMessage().c_str());
          return -1;
       }
    }

    return 0;
}

static PyObject* image_getwidth(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->img->getWidth());
}

static PyObject* image_getheight(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", self->img->getHeight());
}

static PyObject* image_getsize(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("ii", self->img->getWidth(), self->img->getHeight());
}

static PyGetSetDef image_getset[] = {
   { "width", (getter)image_getwidth, NULL, "Width of the image", NULL },
   { "height", (getter)image_getheight, NULL, "Height of the image", NULL },
   { "size", (getter)image_getsize, NULL, "Size as a 2-tuple", NULL },

   { NULL }
};

static PyObject* image_clear(PyImage *self,
                             PyObject *args)
{
    PyColor *color;

    if (!PyArg_ParseTuple(args, "O:clear", &color))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)color)->ob_type, PPyColorType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be a Color.");
       return NULL;
    }

    Py_INCREF(self);
    Py_BEGIN_ALLOW_THREADS
       self->img->clear(color->color);
    Py_END_ALLOW_THREADS
    Py_DECREF(self);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_blit(PyImage *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    PyImage *src;
    int sx = 0, sy = 0, w = -1, h = -1, dx = 0, dy = 0, dw = -1, dh = -1;
    int blend = 0;
    int dtype = 0; // 0: image, 1: screen
    double scaleX, scaleY;

    static char* kwids[] = { "src", "sx", "sy", "w", "h", "dx", "dy", "blend", "dw", "dh", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
                                     "O|iiiiiiiii:blit", kwids,
                                     &src,
                                     &sx, &sy, &w, &h,
                                     &dx, &dy,
                                     &blend,
                                     &dw, &dh))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)src)->ob_type, PPyImageType))
    {
#ifdef LINUX
       PyErr_SetString(PyExc_NotImplementedError, "No screen under Linux.");
       return NULL;
#else
       if (!PyType_IsSubtype(((PyObject*)src)->ob_type, PPyScreenType))
       {
          PyErr_SetString(PyExc_TypeError, "First argument must be an Image or a Screen");
          return NULL;
       }

       dtype = 1;
#endif
    }
    else
    {
       dtype = 0;
    }

    switch (dtype)
    {
       case 0:
          if (w == -1)
             w = src->img->getWidth();

          if (h == -1)
             h = src->img->getHeight();

          break;

#ifndef LINUX
       case 1:
          if (w == -1)
             w = SCREEN_WIDTH;

          if (h == -1)
             h = SCREEN_HEIGHT;

          break;
#endif
    }

    // Sanity checks

    if (dw < 0)
       dw = w;

    if (dh < 0)
       dh = h;

    scaleX = (double)dw / w;
    scaleY = (double)dh / h;

    if ((dx >= (int)self->img->getWidth()) || (dy >= (int)self->img->getHeight()))
    {
       Py_INCREF(Py_None);
       return Py_None;
    }

    w = MIN(w, (int)self->img->getWidth() - dx);
    h = MIN(h, (int)self->img->getHeight() - dy);

    {
       int targetW = 0, targetH = 0;

       if (dtype == 0)
       {
          targetW = (int)self->img->getWidth();
          targetH = (int)self->img->getHeight();
       }

#ifndef LINUX
       else
       {
          targetW = SCREEN_WIDTH;
          targetH = SCREEN_HEIGHT;
       }
#endif

       if (dx + scaleX * w >= targetW)
          w = (int)((targetW - dx) / scaleX);

       if (dy + scaleY * h >= targetH)
          h = (int)((targetH - dy) / scaleY);
    }

    if ((w <= 0) || (h <= 0))
    {
       Py_INCREF(Py_None);
       return Py_None;
    }

    Py_INCREF(self);

    switch (dtype)
    {
       case 0:
	 Py_BEGIN_ALLOW_THREADS
             self->img->blit(src->img, sx, sy, w, h, dx, dy, blend, scaleX, scaleY);
	 Py_END_ALLOW_THREADS
	 break;

#ifndef LINUX
       case 1:
	 Py_BEGIN_ALLOW_THREADS
             self->img->blit(((PyScreen*)src)->scr, sx, sy, w, h, dx, dy, blend, scaleX, scaleY);
	 Py_END_ALLOW_THREADS
         break;
#endif
    }

    Py_DECREF(self);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_fillRect(PyImage *self,
                                PyObject *args)
{
    int x, y, w, h;
    PyColor *color;

    if (!PyArg_ParseTuple(args, "iiiiO:fillRect", &x, &y, &w, &h,
                          &color))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)color)->ob_type, PPyColorType))
    {
       PyErr_SetString(PyExc_TypeError, "Fifth argument must be a Color.");
       return NULL;
    }

    Py_INCREF(self);
    Py_BEGIN_ALLOW_THREADS
       self->img->fillRect(color->color, x, y, w, h);
    Py_END_ALLOW_THREADS
    Py_DECREF(self);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_saveToFile(PyImage *self,
                                  PyObject *args)
{
    char *filename;
    int type = (int)IMG_PNG;
    ImageType etype = IMG_PNG;

    if (!PyArg_ParseTuple(args, "s|i:saveToFile", &filename, &type))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    switch (type)
    {
       case (int)IMG_PNG:
          etype = IMG_PNG;
          break;
       case (int)IMG_JPEG:
          etype = IMG_JPEG;
          break;
    }

    Py_INCREF(self);
    Py_BEGIN_ALLOW_THREADS
       self->img->saveToFile(filename, etype);
    Py_END_ALLOW_THREADS
    Py_DECREF(self);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_putPixel(PyImage *self,
                                PyObject *args)
{
    int x, y;
    PyColor *color = NULL;
    u32 col = 0xFFFFFFFFU;

    if (!PyArg_ParseTuple(args, "ii|O:putPixel", &x, &y, &color))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (color)
    {
       if (!PyType_IsSubtype(((PyObject*)color)->ob_type, PPyColorType))
       {
          PyErr_SetString(PyExc_TypeError, "Third argument must be a Color.");
          return NULL;
       }

       col = color->color;
    }

    self->img->putPixel(col, x, y);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_getPixel(PyImage *self,
                                PyObject *args)
{
    int x, y;
    PyColor *color;
    PyObject *nargs;
    u32 c;

    if (!PyArg_ParseTuple(args, "ii:getPixel", &x, &y))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    c = self->img->getPixel(x, y);
    nargs = Py_BuildValue("iii", (int)(c & 0xFF), (int)((c >> 8) & 0xFF),
                          (int)((c >> 16) & 0xFF), (int)((c >> 24) & 0xFF));

    color = (PyColor*)PyType_GenericNew(PPyColorType, nargs, NULL);
    Py_DECREF(nargs);

    color->color = c;
    return (PyObject*)color;
}

static PyObject* image_drawLine(PyImage *self,
                                PyObject *args)
{
    int x1, y1, x2, y2;
    PyColor *colorobj = NULL;
    u32 color = 0xFFFFFFFFU;

    if (!PyArg_ParseTuple(args, "iiii|O:drawLine", &x1, &y1, &x2, &y2, &colorobj))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (colorobj)
    {
       if (!PyType_IsSubtype(((PyObject*)colorobj)->ob_type, PPyColorType))
       {
          PyErr_SetString(PyExc_TypeError, "Fifth argument must be a Color.");
          return NULL;
       }

       color = colorobj->color;
    }

    Py_INCREF(self);
    Py_BEGIN_ALLOW_THREADS
       self->img->drawLine(x1, y1, x2, y2, color);
    Py_END_ALLOW_THREADS
    Py_DECREF(self);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_drawText(PyImage *self,
                                PyObject *args)
{
    int x, y;
    char *text;
    PyColor *colorobj = NULL;
    u32 color = 0xFFFFFFFFU;

    if (!PyArg_ParseTuple(args, "iis|O:drawText", &x, &y, &text, &colorobj))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (colorobj)
    {
       if (!PyType_IsSubtype(((PyObject*)colorobj)->ob_type, PPyColorType))
       {
          PyErr_SetString(PyExc_TypeError, "Fifth argument must be a Color.");
          return NULL;
       }

       color = colorobj->color;
    }

    Py_INCREF(self);
    Py_BEGIN_ALLOW_THREADS
       self->img->printText(x, y, text, color);
    Py_END_ALLOW_THREADS
    Py_DECREF(self);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_boundingRectangle(PyImage *self,
                                         PyObject *args)
{
    u16 x1, y1, x2, y2;
    int x, y;

    if (!PyArg_ParseTuple(args, "ii:boundingRectangle", &x, &y))
       return NULL;

    Py_INCREF(self);
    Py_BEGIN_ALLOW_THREADS
       self->img->boundingRectangle(x, y, x1, y1, x2, y2);
    Py_END_ALLOW_THREADS
    Py_DECREF(self);

    return Py_BuildValue("iiii", (int)x1, (int)y1, (int)x2, (int)y2);
}

static PyObject* image_boundingRectangles(PyImage *self,
                                          PyObject *args)
{
    list<Rectangle> ls;
    PyObject *ret;

    if (!PyArg_ParseTuple(args, ":boundingRectangles"))
       return NULL;

    Py_INCREF(self);
    Py_BEGIN_ALLOW_THREADS
       ls = self->img->boundingRectangles();
    Py_END_ALLOW_THREADS
    Py_DECREF(self);

    ret = PyList_New(0);

    for (list<Rectangle>::iterator k = ls.begin();
         k != ls.end();
         ++k)
    {
       PyList_Append(ret, Py_BuildValue("iiii", (int)k->_x1, (int)k->_y1, (int)k->_x2, (int)k->_y2));
    }

    return ret;
}

#ifdef LINUX

static PyObject* image_fromData(PyImage *self,
                                PyObject *args)
{
    unsigned char *data;
    int w, h, sz;

    if (!PyArg_ParseTuple(args, "iis#:fromData", &w, &h, &data, &sz))
       return NULL;

    if (sz != 3 * w * h)
    {
       PyErr_SetString(PyExc_ValueError, "Wrong buffer size");
       return NULL;
    }

    Py_INCREF(self);
    Py_BEGIN_ALLOW_THREADS
       self->img->fromData(w, h, data);
    Py_END_ALLOW_THREADS
    Py_DECREF(self);

    Py_INCREF(Py_None);
    return Py_None;
}

#endif

static PyMethodDef image_methods[] = {
   { "clear", (PyCFunction)image_clear, METH_VARARGS, "" },
   { "blit", (PyCFunction)image_blit, METH_VARARGS|METH_KEYWORDS, "" },
   { "fillRect", (PyCFunction)image_fillRect, METH_VARARGS, "" },
   { "saveToFile", (PyCFunction)image_saveToFile, METH_VARARGS, "" },
   { "putPixel", (PyCFunction)image_putPixel, METH_VARARGS, "" },
   { "getPixel", (PyCFunction)image_getPixel, METH_VARARGS, "" },
   { "drawLine", (PyCFunction)image_drawLine, METH_VARARGS, "" },
   { "drawText", (PyCFunction)image_drawText, METH_VARARGS, "" },
   { "boundingRectangle", (PyCFunction)image_boundingRectangle, METH_VARARGS, "" },
   { "boundingRectangles", (PyCFunction)image_boundingRectangles, METH_VARARGS, "" },

#ifdef LINUX
   { "fromData", (PyCFunction)image_fromData, METH_VARARGS, "" },
#endif

   { NULL }
};

static PyTypeObject PyImageType = {
   PyObject_HEAD_INIT(NULL)
   0,
   IMAGING_MODULE_NAME "." "Image",
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
static const char* _rcsid_image __attribute__((unused)) = "$Id: image.cpp 1547 2005-12-09 12:25:07Z fraca7 $";
#endif
