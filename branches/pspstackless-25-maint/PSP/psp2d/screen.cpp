
/**
 * @file screen.c
 */

/**********************************************************************

  Created: 30 Oct 2005

**********************************************************************/
// $Id: screen.cpp 1547 2005-12-09 12:25:07Z fraca7 $

#include "screen.h"
#include "image.h"
#include "color.h"
#include "font.h"

using namespace Imaging;

#define MIN(x, y) ((x) < (y) ? (x) : (y))

static void screen_dealloc(PyScreen *self)
{
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* screen_new(PyTypeObject *type,
                            PyObject *args,
                            PyObject *kwargs)
{
    PyScreen *self;

    if (PyErr_CheckSignals())
       return NULL;

    self = (PyScreen*)type->tp_alloc(type, 0);

    if (self)
       self->scr = NULL;

    return (PyObject*)self;
}

static int screen_init(PyScreen *self,
                      PyObject *args,
                      PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":__init__"))
       return -1;

    if (PyErr_CheckSignals())
       return -1;

    self->scr = Screen::getInstance();

    return 0;
}

static PyObject* screen_getwidth(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", 480);
}

static PyObject* screen_getheight(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", 272);
}

static PyObject* screen_getsize(PyImage *self, void *closure)
{
    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("ii", 480, 272);
}

static PyGetSetDef screen_getset[] = {
   { "width", (getter)screen_getwidth, NULL, "Width of the image", NULL },
   { "height", (getter)screen_getheight, NULL, "Height of the image", NULL },
   { "size", (getter)screen_getsize, NULL, "Size as a 2-tuple", NULL },

   { NULL }
};


static PyObject* screen_blit(PyScreen *self,
                             PyObject *args,
                             PyObject *kwargs)
{
    int sx = 0, sy = 0, w = -1, h = -1, dx = 0, dy = 0, blend = 0, dw = -1, dh = -1;
    double scaleX, scaleY;
    PyImage *img;

    static char* kwids[] = { "src", "sx", "sy", "w", "h", "dx", "dy", "blend", "dw", "dh", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
                                     "O|iiiiiiiii:blit", kwids,
                                     &img,
                                     &sx, &sy, &w, &h,
                                     &dx, &dy,
                                     &blend,
                                     &dw, &dh))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)img)->ob_type, PPyImageType))
    {
       PyErr_SetString(PyExc_TypeError, "Fifth argument must be an Image");
       return NULL;
    }

    if (PyErr_CheckSignals())
       return NULL;

    if (w == -1)
       w = (int)img->img->getWidth();

    if (h == -1)
       h = (int)img->img->getHeight();

    if (dw < 0)
       dw = w;

    if (dh < 0)
       dh = h;

    scaleX = (double)dw / w;
    scaleY = (double)dh / h;

    // Sanity checks

    if ((dx >= SCREEN_WIDTH) || (dy >= SCREEN_HEIGHT))
    {
       Py_INCREF(Py_None);
       return Py_None;
    }

    w = MIN(w, (int)img->img->getWidth() - sx);
    if (dx + scaleX * w >= SCREEN_WIDTH)
       w = (int)((SCREEN_WIDTH - dx) / scaleX);

    h = MIN(h, (int)img->img->getHeight() - sy);
    if (dy + scaleY * h >= SCREEN_HEIGHT)
       h = (int)((SCREEN_HEIGHT - dy) / scaleY);

    if ((w <= 0) || (h <= 0))
    {
       Py_INCREF(Py_None);
       return Py_None;
    }

    // OK

    self->scr->blit(img->img, sx, sy, w, h, dx, dy, blend, scaleX, scaleY);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* screen_clear(PyScreen *self,
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

    if (PyErr_CheckSignals())
       return NULL;

    self->scr->clear(color->color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* screen_fillRect(PyScreen *self,
                                 PyObject *args)
{
    PyColor *color;
    int x, y, w, h;

    if (!PyArg_ParseTuple(args, "iiiiO:fillRect", &x, &y,
                          &w, &h, &color))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)color)->ob_type, PPyColorType))
    {
       PyErr_SetString(PyExc_TypeError, "Fifth argument must be a Color.");
       return NULL;
    }

    if (PyErr_CheckSignals())
       return NULL;

    self->scr->fillRect(color->color, x, y, w, h);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* screen_swap(PyScreen *self,
                             PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":swap"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    self->scr->flip();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* screen_saveToFile(PyScreen *self,
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

    self->scr->saveToFile(filename, etype);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* screen_putPixel(PyScreen *self,
                                 PyObject *args)
{
    int x, y;
    PyColor *color;

    if (!PyArg_ParseTuple(args, "iiO:putPixel", &x, &y, &color))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (!PyType_IsSubtype(((PyObject*)color)->ob_type, PPyColorType))
    {
       PyErr_SetString(PyExc_TypeError, "Third argument must be a Color.");
       return NULL;
    }

    self->scr->putPixel(color->color, x, y);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* screen_getPixel(PyScreen *self,
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

    c = self->scr->getPixel(x, y);
    nargs = Py_BuildValue("iii", (int)(c & 0xFF), (int)((c >> 8) & 0xFF),
                          (int)((c >> 16) & 0xFF), (int)((c >> 24) & 0xFF));

    color = (PyColor*)PyType_GenericNew(PPyColorType, nargs, NULL);
    Py_DECREF(nargs);

    color->color = c;
    return (PyObject*)color;
}

static PyObject* screen_drawLine(PyScreen *self,
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

    self->scr->drawLine(x1, y1, x2, y2, color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* screen_drawText(PyScreen *self,
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

    self->scr->printText(x, y, text, color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef screen_methods[] = {
   { "blit", (PyCFunction)screen_blit, METH_VARARGS|METH_KEYWORDS, "" },
   { "clear", (PyCFunction)screen_clear, METH_VARARGS, "" },
   { "fillRect", (PyCFunction)screen_fillRect, METH_VARARGS, "" },
   { "swap", (PyCFunction)screen_swap, METH_VARARGS, "" },
   { "saveToFile", (PyCFunction)screen_saveToFile, METH_VARARGS, "" },
   { "putPixel", (PyCFunction)screen_putPixel, METH_VARARGS, "" },
   { "getPixel", (PyCFunction)screen_getPixel, METH_VARARGS, "" },
   { "drawLine", (PyCFunction)screen_drawLine, METH_VARARGS, "" },
   { "drawText", (PyCFunction)screen_drawText, METH_VARARGS, "" },

   { NULL }
};

static PyTypeObject PyScreenType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "pymaging.Screen",
   sizeof(PyScreen),
   0,
   (destructor)screen_dealloc,
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
   "Screen objects",
   0,
   0,
   0,
   0,
   0,
   0,
   screen_methods,
   0,
   screen_getset,
   0,
   0,
   0,
   0,
   0,
   (initproc)screen_init,
   0,
   screen_new,
};

PyTypeObject* PPyScreenType = &PyScreenType;

#ifdef _GNUC
static const char* _rcsid_screen __attribute__((unused)) = "$Id: screen.cpp 1547 2005-12-09 12:25:07Z fraca7 $";
#endif
