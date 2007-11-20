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
#include "screen.h"

using namespace PSP2D;

extern PyObject* Py2DError;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

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

    if (!PyArg_ParseTuple(args, "O|O", &a, &b))
       return -1;

    if (a->ob_type == PPyImageType)
    {
       PyImage *other = (PyImage*)a;

       try
       {
          self->img = new Image(other->img);
       }
       catch (ImageException& e)
       {
          PyErr_SetString(Py2DError, e.getMessage().c_str());
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
          self->img = new Image(filename);
       }
       catch (ImageIOException)
       {
          PyErr_SetString(PyExc_IOError, "Could not open file");
          return -1;
       }
       catch (ImageException& e)
       {
          PyErr_SetString(Py2DError, e.getMessage().c_str());
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
          self->img = new Image(w, h);
       }
       catch (ImageException& e)
       {
          PyErr_SetString(Py2DError, e.getMessage().c_str());
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

static PyGetSetDef image_getset[] = {
   { "width", (getter)image_getwidth, NULL, "Width of the image", NULL },
   { "height", (getter)image_getheight, NULL, "Height of the image", NULL },

   { NULL }
};

static PyObject* image_clear(PyImage *self,
                             PyObject *args,
                             PyObject *kwargs)
{
    PyColor *color;

    if (!PyArg_ParseTuple(args, "O:clear", &color))
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)color)->ob_type, PPyColorType))
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be a Color.");
       return NULL;
    }

    if (PyErr_CheckSignals())
       return NULL;

    self->img->clear(color->color);

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

    static char* kwids[] = { "src", "sx", "sy", "w", "h", "dx", "dy", "blend", "dw", "dh", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
                                     "O|iiiiiiiii", kwids,
                                     &src,
                                     &sx, &sy, &w, &h,
                                     &dx, &dy,
                                     &blend,
                                     &dw, &dh))
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)src)->ob_type, PPyImageType))
    {
       if (!PyType_IsSubtype(((PyObject*)src)->ob_type, PPyScreenType))
       {
          PyErr_SetString(PyExc_TypeError, "First argument must be an Image or a Screen");
          return NULL;
       }

       dtype = 1;
    }
    else
    {
       dtype = 0;
    }

    if (PyErr_CheckSignals())
       return NULL;

    switch (dtype)
    {
       case 0:
          if (w == -1)
             w = src->img->getWidth();

          if (h == -1)
             h = src->img->getHeight();

          break;

       case 1:
          if (w == -1)
             w = SCREEN_WIDTH;

          if (h == -1)
             h = SCREEN_HEIGHT;

          break;
    }

    // Sanity checks

    if ((dx >= (int)self->img->getWidth()) || (dy >= (int)self->img->getHeight()))
    {
       Py_INCREF(Py_None);
       return Py_None;
    }

    w = MIN(w, (int)self->img->getWidth() - dx);
    h = MIN(h, (int)self->img->getHeight() - dy);

    switch (dtype)
    {
       case 0:
          w = MIN(w, (int)src->img->getWidth() - sx);
          h = MIN(h, (int)src->img->getHeight() - sy);

          break;

       case 1:
          w = MIN(w, SCREEN_WIDTH - sx);
          h = MIN(h, SCREEN_HEIGHT - sy);

          break;
    }

    if ((w <= 0) || (h <= 0))
    {
       Py_INCREF(Py_None);
       return Py_None;
    }

    switch (dtype)
    {
       case 0:
          self->img->blit(src->img, sx, sy, w, h, dx, dy, blend, dw, dh);
          break;

       case 1:
          self->img->blit(((PyScreen*)src)->scr, sx, sy, w, h, dx, dy, blend, dw, dh);
          break;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_fillRect(PyImage *self,
                                PyObject *args,
                                PyObject *kwargs)
{
    int x, y, w, h;
    PyColor *color;

    if (!PyArg_ParseTuple(args, "iiiiO:fillRect", &x, &y, &w, &h,
                          &color))
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)color)->ob_type, PPyColorType))
    {
       PyErr_SetString(PyExc_TypeError, "Fifth argument must be a Color.");
       return NULL;
    }

    if (PyErr_CheckSignals())
       return NULL;

    self->img->fillRect(color->color, x, y, w, h);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_saveToFile(PyImage *self,
                                  PyObject *args,
                                  PyObject *kwargs)
{
    char *filename;
    int type = (int)IMG_PNG;
    ImageType etype = IMG_PNG;

    if (!PyArg_ParseTuple(args, "s|i:saveToFile", &filename, &type))
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

    self->img->saveToFile(filename, etype);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_putPixel(PyImage *self,
                                PyObject *args,
                                PyObject *kwargs)
{
    int x, y;
    PyColor *color;

    if (!PyArg_ParseTuple(args, "iiO", &x, &y, &color))
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)color)->ob_type, PPyColorType))
    {
       PyErr_SetString(PyExc_TypeError, "Third argument must be a Color.");
       return NULL;
    }

    self->img->putPixel(color->color, x, y);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_getPixel(PyImage *self,
                                PyObject *args,
                                PyObject *kwargs)
{
    int x, y;
    PyColor *color;
    PyObject *nargs;
    u32 c;

    if (!PyArg_ParseTuple(args, "ii", &x, &y))
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
                                PyObject *args,
                                PyObject *kwargs)
{
    int x1, y1, x2, y2;
    PyColor *colorobj = NULL;
    u32 color = 0xFFFFFFFFU;

    if (!PyArg_ParseTuple(args, "iiii|O:drawLine", &x1, &y1, &x2, &y2, &colorobj))
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

    self->img->drawLine(x1, y1, x2, y2, color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* image_drawText(PyImage *self,
                                PyObject *args,
                                PyObject *kwargs)
{
    int x, y;
    char *text;
    PyColor *colorobj = NULL;
    u32 color = 0xFFFFFFFFU;

    if (!PyArg_ParseTuple(args, "iis|O:drawText", &x, &y, &text, &colorobj))
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

    self->img->printText(x, y, text, color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef image_methods[] = {
   { "clear", (PyCFunction)image_clear, METH_VARARGS, "" },
   { "blit", (PyCFunction)image_blit, METH_VARARGS|METH_KEYWORDS, "" },
   { "fillRect", (PyCFunction)image_fillRect, METH_VARARGS, "" },
   { "saveToFile", (PyCFunction)image_saveToFile, METH_VARARGS, "" },
   { "putPixel", (PyCFunction)image_putPixel, METH_VARARGS, "" },
   { "getPixel", (PyCFunction)image_getPixel, METH_VARARGS, "" },
   { "drawLine", (PyCFunction)image_drawLine, METH_VARARGS, "" },
   { "drawText", (PyCFunction)image_drawText, METH_VARARGS, "" },

   { NULL }
};

static PyTypeObject PyImageType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "psp2d.Image",
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
