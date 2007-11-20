
/**
 * @file osl.c
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

#include <oslib/oslib.h>

#include "osl.h"
#include "image.h"
#include "ctrl.h"
#include "sound.h"
#include "map.h"
#include "font.h"

PyObject* osl_Error = NULL;

//==========================================================================
// Functions

static PyObject* osl_mustQuit(PyObject *self,
                              PyObject *args,
                              PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":mustQuit"))
       return NULL;

    return Py_BuildValue("i", osl_quit);
}

static PyObject* osl_doQuit(PyObject *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":doQuit"))
       return NULL;

    oslQuit();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_initGfx(PyObject *self,
                             PyObject *args,
                             PyObject *kwargs)
{
    int pf = OSL_PF_8888;
    int db = 1;

    if (!PyArg_ParseTuple(args, "|ii:initGfx", &pf, &db))
       return NULL;

    oslInitGfx(pf, db);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_startDrawing(PyObject *self,
                                  PyObject *args,
                                  PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":startDrawing"))
       return NULL;

    oslStartDrawing();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_syncDrawing(PyObject *self,
                                 PyObject *args,
                                 PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":syncDrawing"))
       return NULL;

    oslSyncDrawing();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_endDrawing(PyObject *self,
                                PyObject *args,
                                PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":endDrawing"))
       return NULL;

    oslEndDrawing();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_waitVSync(PyObject *self,
                               PyObject *args,
                               PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":waitVSync"))
       return NULL;

    oslWaitVSync();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_swapBuffers(PyObject *self,
                                 PyObject *args,
                                 PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":swapBuffers"))
       return NULL;

    oslSwapBuffers();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_endGfx(PyObject *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":endGfx"))
       return NULL;

    oslEndGfx();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_syncFrame(PyObject *self,
                               PyObject *args,
                               PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":syncFrame"))
       return NULL;

    oslSyncFrame();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_syncFrameEx(PyObject *self,
                                 PyObject *args,
                                 PyObject *kwargs)
{
    int skip, maxskip, vsync;

    if (!PyArg_ParseTuple(args, "iii:syncFrameEx", &skip, &maxskip, &vsync))
       return NULL;

    oslSyncFrameEx(skip, maxskip, vsync);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_setTransparentColor(PyObject *self,
                                         PyObject *args,
                                         PyObject *kwargs)
{
    unsigned long color;

    if (!PyArg_ParseTuple(args, "k:setTransparentColor", &color))
       return NULL;

    oslSetTransparentColor(color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_disableTransparentColor(PyObject *self,
                                             PyObject *args,
                                             PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":disableTransparentColor"))
       return NULL;

    oslDisableTransparentColor();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_drawLine(PyObject *self,
                              PyObject *args,
                              PyObject *kwargs)
{
    int x1, y1, x2, y2;
    unsigned long color;

    if (!PyArg_ParseTuple(args, "iiiik:drawLine", &x1, &y1, &x2, &y2, &color))
       return NULL;

    oslDrawLine(x1, y1, x2, y2, color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_drawRect(PyObject *self,
                              PyObject *args,
                              PyObject *kwargs)
{
    int x1, y1, x2, y2;
    unsigned long color;

    if (!PyArg_ParseTuple(args, "iiiik:drawRect", &x1, &y1, &x2, &y2, &color))
       return NULL;

    oslDrawRect(x1, y1, x2, y2, color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_drawFillRect(PyObject *self,
                                  PyObject *args,
                                  PyObject *kwargs)
{
    int x1, y1, x2, y2;
    unsigned long color;

    if (!PyArg_ParseTuple(args, "iiiik:drawFillRect", &x1, &y1, &x2, &y2, &color))
       return NULL;

    oslDrawFillRect(x1, y1, x2, y2, color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_drawGradientRect(PyObject *self,
                                      PyObject *args,
                                      PyObject *kwargs)
{
    int x1, y1, x2, y2;
    unsigned long c1, c2, c3, c4;

    if (!PyArg_ParseTuple(args, "iiiikkkk:drawGradientRect",
                          &x1, &y1, &x2, &y2, &c1, &c2, &c3, &c4))
       return NULL;

    oslDrawGradientRect(x1, y1, x2, y2, c1, c2, c3, c4);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_clearScreen(PyObject *self,
                                 PyObject *args,
                                 PyObject *kwargs)
{
    int color;

    if (!PyArg_ParseTuple(args, "i:clearScreen", &color))
       return NULL;

    oslClearScreen(color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_setScreenClipping(PyObject *self,
                                       PyObject *args,
                                       PyObject *kwargs)
{
    int x1, y1, x2, y2;

    if (!PyArg_ParseTuple(args, "iiii:setScreenClipping",
                          &x1, &y1, &x2, &y2))
       return NULL;

    oslSetScreenClipping(x1, y1, x2, y2);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_setAlpha(PyObject *self,
                              PyObject *args,
                              PyObject *kwargs)
{
    int effect, coeff;

    if (!PyArg_ParseTuple(args, "ii:setAlpha", &effect, &coeff))
       return NULL;

    oslSetAlpha(effect, coeff);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_systemMessage(PyObject *self,
                                   PyObject *args,
                                   PyObject *kwargs)
{
    char *msg;

    if (!PyArg_ParseTuple(args, "s:systemMessage", &msg))
       return NULL;

    // We  strdup() here since  other threads  could destroy  the last
    // reference to the string

    msg = strdup(msg);

    Py_BEGIN_ALLOW_THREADS
       oslSystemMessage(msg);
    Py_END_ALLOW_THREADS

    free(msg);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_messageBox(PyObject *self,
                                PyObject *args,
                                PyObject *kwargs)
{
    char *text, *title;
    int ret;

    int b1, a1;
    int b2 = 0, a2 = 0;
    int b3 = 0, a3 = 0;

    if (!PyArg_ParseTuple(args, "ssii|iiii:messageBox", &text, &title,
                          &b1, &a1, &b2, &a2, &b3, &a3))
       return NULL;

    text = strdup(text);
    title = strdup(title);

    Py_BEGIN_ALLOW_THREADS
       ret = oslMessageBox(text, title, oslMake3Buttons(b1, a1, b2, a2, b3, a3));
    Py_END_ALLOW_THREADS

    free(text);
    free(title);

    return Py_BuildValue("i", ret);
}

static PyObject* osl_waitKey(PyObject *self,
                             PyObject *args,
                             PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":waitKey"))
       return NULL;

    return Py_BuildValue("i", oslWaitKey());
}

static PyObject* osl_kbhit(PyObject *self,
                           PyObject *args,
                           PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":kbhit"))
       return NULL;

    return Py_BuildValue("i", oslKbhit());
}

static PyObject* osl_flushKey(PyObject *self,
                              PyObject *args,
                              PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":flushKey"))
       return NULL;

    oslFlushKey();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* currentDrawBuffer = NULL;

static PyObject* osl_setDrawBuffer(PyObject *self,
                                   PyObject *args,
                                   PyObject *kwargs)
{
    PyObject* buffer;

    if (!PyArg_ParseTuple(args, "O:setDrawBuffer"), &buffer)
       return NULL;

    if (PyInt_Check(buffer))
    {
       oslSetDrawBuffer((OSL_IMAGE*)PyInt_AsLong(buffer));
    }
    else if (PyType_IsSubtype(buffer->ob_type, PPyImageType))
    {
       if (currentDrawBuffer)
       {
          Py_DECREF(currentDrawBuffer);
       }

       oslSetDrawBuffer(((PyImage*)buffer)->pImg);

       Py_INCREF(buffer);
       currentDrawBuffer = buffer;
    }
    else
    {
       PyErr_SetString(PyExc_TypeError, "Argument must be an osl.Image or predefined constant");
       return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_initConsole(PyObject *self,
                                 PyObject *args,
                                 PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":initConsole"))
       return NULL;

    oslInitConsole();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_print(PyObject *self,
                           PyObject *args,
                           PyObject *kwargs)
{
    char *text;

    if (!PyArg_ParseTuple(args, "s:print", &text))
       return NULL;

    oslPrintf("%s", text);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_cls(PyObject *self,
                         PyObject *args,
                         PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":cls"))
       return NULL;

    oslCls();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_moveTo(PyObject *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    int x, y;

    if (!PyArg_ParseTuple(args, "ii:moveTo", &x, &y))
       return NULL;

    oslMoveTo(x, y);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_printxy(PyObject *self,
                             PyObject *args,
                             PyObject *kwargs)
{
    int x, y;
    char *text;

    if (!PyArg_ParseTuple(args, "iis:printxy", &x, &y, &text))
       return NULL;

    oslPrintf_xy(x, y, "%s", text);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_setTextColor(PyObject *self,
                                  PyObject *args,
                                  PyObject *kwargs)
{
    unsigned long color;

    if (!PyArg_ParseTuple(args, "k:setTextColor", &color))
       return NULL;

    oslSetTextColor(color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_setBkColor(PyObject *self,
                                PyObject *args,
                                PyObject *kwargs)
{
    unsigned long color;;

    if (!PyArg_ParseTuple(args, "k:setBkColor", &color))
       return NULL;

    oslSetBkColor(color);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_setBilinearFilter(PyObject *self,
                                       PyObject *args,
                                       PyObject *kwargs)
{
    int enable = 1;

    if (!PyArg_ParseTuple(args, "|i:setBilinearFilter", &enable))
       return NULL;

    oslSetBilinearFilter(enable);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_initAudio(PyObject *self,
                               PyObject *args,
                               PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":initAudio"))
       return NULL;

    oslInitAudio();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_audioVSync(PyObject *self,
                                PyObject *args,
                                PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":audioVSync"))
       return NULL;

    oslAudioVSync();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_setKeyAutorepeat(PyObject *self,
                                      PyObject *args,
                                      PyObject *kwargs)
{
    int keys, init, interval;

    if (!PyArg_ParseTuple(args, "iii:setKeyAutorepeat",
                          &keys, &init, &interval))
       return NULL;

    oslSetKeyAutorepeat(keys, init, interval);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_setKeyAutorepeatMask(PyObject *self,
                                          PyObject *args,
                                          PyObject *kwargs)
{
    int keys;

    if (!PyArg_ParseTuple(args, "i:setKeyAutorepeatMask",
                          &keys))
       return NULL;

    oslSetKeyAutorepeatMask(keys);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_setKeyAutorepeatInit(PyObject *self,
                                          PyObject *args,
                                          PyObject *kwargs)
{
    int init;

    if (!PyArg_ParseTuple(args, "i:setKeyAutorepeatInit",
                          &init))
       return NULL;

    oslSetKeyAutorepeatInit(init);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_setKeyAutorepeatInterval(PyObject *self,
                                              PyObject *args,
                                              PyObject *kwargs)
{
    int interval;

    if (!PyArg_ParseTuple(args, "i:setKeyAutorepeatInterval",
                          &interval))
       return NULL;

    oslSetKeyAutorepeatInterval(interval);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_flushDataCache(PyObject *self,
                                    PyObject *args,
                                    PyObject *kwargs)
{
    if (!PyArg_ParseTuple(args, ":flushDataCache"))
       return NULL;

    oslFlushDataCache();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_setChannelVolume(PyObject *self,
                                      PyObject *args,
                                      PyObject *kwargs)
{
    int channel, left, right;

    if (!PyArg_ParseTuple(args, "iii:setChannelVolume",
                          &channel, &left, &right))
       return NULL;

    oslSetChannelVolume(channel, left, right);

    Py_INCREF(Py_None);
    return Py_None;
}

//==========================================================================
// text.h

static PyObject* osl_drawString(PyObject *self,
                                PyObject *args,
                                PyObject *kwargs)
{
    char *string;
    int x, y;

    if (!PyArg_ParseTuple(args, "iis:drawString", &x, &y, &string))
       return NULL;

    oslDrawString(x, y, string);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* osl_drawTextBox(PyObject *self,
                                 PyObject *args,
                                 PyObject *kwargs)
{
    int x0, y0, x1, y1, format;
    char *string;

    if (!PyArg_ParseTuple(args, "iiiisi:drawTextBox",
                          &x0, &y0, &x1, &y1, &string, &format))
       return NULL;

    oslDrawTextBox(x0, y0, x1, y1, string, format);

    Py_INCREF(Py_None);
    return Py_None;
}

//==========================================================================
// Macros

static PyObject* osl_RGB(PyObject *self,
                         PyObject *args,
                         PyObject *kwargs)
{
    int r, g, b;

    if (!PyArg_ParseTuple(args, "iii:RGB", &r, &g, &b))
       return NULL;

    return Py_BuildValue("i", RGB(r, g, b));
}

static PyObject* osl_RGB12(PyObject *self,
                           PyObject *args,
                           PyObject *kwargs)
{
    int r, g, b;

    if (!PyArg_ParseTuple(args, "iii:RGB12", &r, &g, &b))
       return NULL;

    return Py_BuildValue("i", RGB12(r, g, b));
}

static PyObject* osl_RGB15(PyObject *self,
                           PyObject *args,
                           PyObject *kwargs)
{
    int r, g, b;

    if (!PyArg_ParseTuple(args, "iii:RGB15", &r, &g, &b))
       return NULL;

    return Py_BuildValue("i", RGB15(r, g, b));
}

static PyObject* osl_RGB16(PyObject *self,
                           PyObject *args,
                           PyObject *kwargs)
{
    int r, g, b;

    if (!PyArg_ParseTuple(args, "iii:RGB16", &r, &g, &b))
       return NULL;

    return Py_BuildValue("i", RGB16(r, g, b));
}

static PyObject* osl_RGBA(PyObject *self,
                          PyObject *args,
                          PyObject *kwargs)
{
    int r, g, b, a;

    if (!PyArg_ParseTuple(args, "iiii:RGBA", &r, &g, &b, &a))
       return NULL;

    return Py_BuildValue("i", RGBA(r, g, b, a));
}

static PyObject* osl_RGBA12(PyObject *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    int r, g, b, a;

    if (!PyArg_ParseTuple(args, "iiii:RGBA12", &r, &g, &b, &a))
       return NULL;

    return Py_BuildValue("i", RGBA12(r, g, b, a));
}

static PyObject* osl_RGBA15(PyObject *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    int r, g, b, a;

    if (!PyArg_ParseTuple(args, "iiii:RGBA15", &r, &g, &b, &a))
       return NULL;

    return Py_BuildValue("i", RGBA15(r, g, b, a));
}

//==========================================================================
// Module

#define DECLFUNC(name, doc) { #name, (PyCFunction)osl_##name, METH_VARARGS, doc }

static PyMethodDef functions[] = {
   DECLFUNC(mustQuit, ""),
   DECLFUNC(doQuit, ""),

   DECLFUNC(initGfx, ""),
   DECLFUNC(startDrawing, ""),
   DECLFUNC(syncDrawing, ""),
   DECLFUNC(endDrawing, ""),
   DECLFUNC(waitVSync, ""),
   DECLFUNC(swapBuffers, ""),
   DECLFUNC(endGfx, ""),

   DECLFUNC(syncFrame, ""),
   DECLFUNC(syncFrameEx, ""),
   DECLFUNC(setTransparentColor, ""),
   DECLFUNC(disableTransparentColor, ""),

   DECLFUNC(drawLine, ""),
   DECLFUNC(drawRect, ""),
   DECLFUNC(drawFillRect, ""),
   DECLFUNC(drawGradientRect, ""),

   DECLFUNC(clearScreen, ""),
   DECLFUNC(setScreenClipping, ""),

   DECLFUNC(setAlpha, ""),

   DECLFUNC(systemMessage, ""),
   DECLFUNC(messageBox, ""),

   DECLFUNC(waitKey, ""),
   DECLFUNC(kbhit, ""),
   DECLFUNC(flushKey, ""),

   DECLFUNC(initConsole, ""),
   DECLFUNC(print, ""),
   DECLFUNC(cls, ""),
   DECLFUNC(moveTo, ""),
   DECLFUNC(printxy, ""),
   DECLFUNC(setTextColor, ""),
   DECLFUNC(setBkColor, ""),

   DECLFUNC(setDrawBuffer, ""),
   DECLFUNC(setBilinearFilter, ""),

   DECLFUNC(initAudio, ""),
   DECLFUNC(audioVSync, ""),

   DECLFUNC(setKeyAutorepeat, ""),
   DECLFUNC(setKeyAutorepeatMask, ""),
   DECLFUNC(setKeyAutorepeatInit, ""),
   DECLFUNC(setKeyAutorepeatInterval, ""),

   DECLFUNC(flushDataCache, ""),

   DECLFUNC(setChannelVolume, ""),

   DECLFUNC(drawString, ""),
   DECLFUNC(drawTextBox, ""),

   DECLFUNC(RGB, ""),
   DECLFUNC(RGB12, ""),
   DECLFUNC(RGB15, ""),
   DECLFUNC(RGB16, ""),
   DECLFUNC(RGBA, ""),
   DECLFUNC(RGBA12, ""),
   DECLFUNC(RGBA15, ""),

   { NULL }
};

#define ADDINT(name) PyModule_AddIntConstant(mdl, #name, (int)OSL_##name)

void initosl(void)
{
    PyObject *mdl;

    if (PyType_Ready(PPyImageType) < 0)
       return;

    if (PyType_Ready(PPyControllerType) < 0)
       return;

    if (PyType_Ready(PPySoundType) < 0)
       return;

    if (PyType_Ready(PPyMapType) < 0)
       return;

    if (PyType_Ready(PPyFontType) < 0)
       return;

    mdl = Py_InitModule3("osl", functions, "");
    if (!mdl)
       return;

    ADDINT(PF_8888);
    ADDINT(PF_5650);
    ADDINT(PF_5551);
    ADDINT(PF_4444);
    ADDINT(PF_8BIT);
    ADDINT(PF_4BIT);

    ADDINT(FX_NONE);
    ADDINT(FX_FLAT);
    ADDINT(FX_RGBA);
    ADDINT(FX_ALPHA);
    ADDINT(FX_ADD);
    ADDINT(FX_SUB);
    ADDINT(FX_COLOR);

    ADDINT(IN_VRAM);
    ADDINT(IN_RAM);

    ADDINT(DEFAULT_BUFFER);
    ADDINT(SECONDARY_BUFFER);

    ADDINT(MB_OK);
    ADDINT(MB_CANCEL);
    ADDINT(MB_YES);
    ADDINT(MB_NO);
    ADDINT(MB_QUIT);

    ADDINT(KEY_SELECT);
    ADDINT(KEY_START);
    ADDINT(KEY_UP);
    ADDINT(KEY_RIGHT);
    ADDINT(KEY_DOWN);
    ADDINT(KEY_LEFT);
    ADDINT(KEY_L);
    ADDINT(KEY_R);
    ADDINT(KEY_TRIANGLE);
    ADDINT(KEY_CIRCLE);
    ADDINT(KEY_CROSS);
    ADDINT(KEY_SQUARE);
    ADDINT(KEY_HOLD);
    ADDINT(KEY_HOME);
    ADDINT(KEY_NOTE);

    ADDINT(FMT_STREAM);
    ADDINT(FMT_NONE);

    osl_Error = PyErr_NewException("osl.Error", NULL, NULL);
    PyModule_AddObject(mdl, "Error", osl_Error);

    Py_INCREF(PPyImageType);
    PyModule_AddObject(mdl, "Image", (PyObject*)PPyImageType);

    Py_INCREF(PPyControllerType);
    PyModule_AddObject(mdl, "Controller", (PyObject*)PPyControllerType);

    Py_INCREF(PPySoundType);
    PyModule_AddObject(mdl, "Sound", (PyObject*)PPySoundType);

    Py_INCREF(PPyMapType);
    PyModule_AddObject(mdl, "Map", (PyObject*)PPyMapType);

    Py_INCREF(PPyFontType);
    PyModule_AddObject(mdl, "Font", (PyObject*)PPyFontType);
}

#ifdef _GNUC
static const char* _rcsid_osl __attribute__((unused)) = "$Id$";
#endif
