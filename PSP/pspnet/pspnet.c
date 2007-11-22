
#include <Python.h>

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pspwlan.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
//#include <pspnet_adhoc.h>
#include <psputility_netparam.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <errno.h>

static PyObject* net_error = NULL;


//==========================================================================
// AdHoc object

/*

typedef struct
{
      PyObject_HEAD

      int pdpid;
} PyPSP_AdHocObject;

static void PyPSP_AdHoc_dealloc(PyPSP_AdHocObject *self)
{
    sceNetAdhocPdpDelete(self->pdpid, 0);

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* PyPSP_AdHoc_new(PyTypeObject *type,
                                 PyObject *args,
                                 PyObject *kwargs)
{
    PyPSP_AdHocObject *self;

    self = (PyPSP_AdHocObject*)type->tp_alloc(type, 0);
    if (self)
       self->pdpid = -1;

    return (PyObject*)self;
}

static int PyPSP_AdHoc_init(PyPSP_AdHocObject *self,
                            PyObject *args,
                            PyObject *kwargs)
{
    int port;
    char ethaddr[7];

    if (!PyArg_ParseTuple(args, "i:__init__", &port))
       return -1;

    // Get our own MAC address
    sceWlanGetEtherAddr(ethaddr);

    // Create PDP
    self->pdpid = sceNetAdhocPdpCreate(ethaddr, port, 0x400, 0);
    if (self->pdpid < 0)
    {
       PyErr_SetString(net_error, "Could not create PDP");
       return -1;
    }

    return 0;
}

static PyObject* PyPSP_AdHoc_send(PyPSP_AdHocObject *self,
                                  PyObject *args,
                                  PyObject *kwargs)
{
    PyObject *macaddr;
    int port;
    void *data;
    int datalen;
    unsigned char dstmac[7];

    int k, nb, sent;

    if (!PyArg_ParseTuple(args, "Ois#:send", &macaddr, &port, &data, &datalen))
       return NULL;

    if (!PySequence_Check(macaddr))
    {
       PyErr_SetString(PyExc_TypeError, "MAC address must be a sequence of 6 integers");
       return NULL;
    }

    if (PySequence_Length(macaddr) != 6)
    {
       PyErr_SetString(PyExc_TypeError, "MAC address must be a sequence of 6 integers");
       return NULL;
    }

    for (k = 0; k < PySequence_Length(macaddr); ++k)
    {
       PyObject *n = PySequence_GetItem(macaddr, k);

       if (!PyInt_Check(n))
       {
          PyErr_SetString(PyExc_TypeError, "MAC address must be a sequence of 6 integers");
          Py_DECREF(n);
          return NULL;
       }

       nb = PyInt_AsLong(n);
       Py_DECREF(n);

       if ((nb < 0) || (nb > 255))
       {
          PyErr_SetString(PyExc_ValueError, "MAC components must be in the range 0-255");
          return NULL;
       }

       dstmac[k] = (unsigned char)nb;
    }

    dstmac[6] = 0;

    sent = sceNetAdhocPdpSend(self->pdpid, (char*)dstmac, port, data, datalen, 0, 0);

    if (sent < 0)
    {
       PyErr_SetString(net_error, "Error while sending data on PDP");
       return NULL;
    }

    return Py_BuildValue("i", sent);
}

static PyObject* PyPSP_AdHoc_recv(PyPSP_AdHocObject *self,
                                  PyObject *args,
                                  PyObject *kwargs)
{
    unsigned char srcmacaddr[7];
    int port;
    char data[4096];

    int recved;

    if (!PyArg_ParseTuple(args, ":recv"))
       return NULL;

    recved = sceNetAdhocPdpRecv(self->pdpid, (char*)srcmacaddr, &port, data, (void*)sizeof(data), 0, 0);

    if (recved < 0)
    {
       PyErr_SetString(net_error, "Error while receiving PDP data");
       return NULL;
    }

    return Py_BuildValue("(iiiiii)s#",
                         (int)srcmacaddr[0],
                         (int)srcmacaddr[1],
                         (int)srcmacaddr[2],
                         (int)srcmacaddr[3],
                         (int)srcmacaddr[4],
                         (int)srcmacaddr[5],
                         data, recved);
}

// Methods table

static PyMethodDef PyPSP_AdHoc_methods[] = {
   { "send", (PyCFunction)PyPSP_AdHoc_send, METH_VARARGS, "" },
   { "recv", (PyCFunction)PyPSP_AdHoc_recv, METH_VARARGS, "" },

   { NULL }
};

// Type structure

static PyTypeObject PyPSP_AdHocType = {
   PyObject_HEAD_INIT(NULL)
   0,
   "pspnet.AdHoc",
   sizeof(PyPSP_AdHocObject),
   0,
   (destructor)PyPSP_AdHoc_dealloc,
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
   "AdHoc objects",
   0,
   0,
   0,
   0,
   0,
   0,
   PyPSP_AdHoc_methods,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   (initproc)PyPSP_AdHoc_init,
   0,
   PyPSP_AdHoc_new,
};

*/


//==========================================================================
// Module functions

static PyObject* connect_to_apctl(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int err, config = 1;
    int stateLast = -1;
    int timeout = -1;
    PyObject *callback = NULL, *ret;

    time_t started;

    static char* kwids[] = { "config", "callback", "timeout", NULL };

    time(&started);

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|iOi:connect_to_apctl", kwids,
                                     &config, &callback, &timeout))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (callback)
    {
       if (!PyCallable_Check(callback))
       {
          PyErr_SetString(PyExc_TypeError, "callback must be callable");
          return NULL;
       }
    }

    Py_BEGIN_ALLOW_THREADS
       err = sceNetApctlConnect(config);
    Py_END_ALLOW_THREADS

    if (err != 0)
    {
       PyErr_Format(net_error, "sceNetApctlConnect returns %08x\n", err);
       return NULL;
    }

    while (1)
    {
       int state;

       if (PyErr_CheckSignals())
          return NULL;

       Py_BEGIN_ALLOW_THREADS
          err = sceNetApctlGetState(&state);
       Py_END_ALLOW_THREADS

       if (err != 0)
       {
          PyErr_Format(net_error, "sceNetApctlGetState returns %08x\n", err);
          return NULL;
       }

       if (state > stateLast)
       {
          if (callback)
          {
             ret = PyObject_CallFunction(callback, "i", state);
             if (!ret)
                return NULL;
             Py_XDECREF(ret);
          }

          stateLast = state;
       }

       if (state == 4)
          break;  // connected with static IP

       // wait a little before polling again
       Py_BEGIN_ALLOW_THREADS
          sceKernelDelayThread(50 * 1000); // 50ms
       Py_END_ALLOW_THREADS

       if (timeout > 0)
       {
          time_t now;

          time(&now);
          if ((int)(now - started) >= timeout)
          {
             PyErr_SetString(net_error, "Timeout while trying to connect");
             return NULL;
          }
       }
    }

    if (callback)
    {
       ret = PyObject_CallFunction(callback, "i", -1);
       if (!ret)
          return NULL;
       Py_XDECREF(ret);
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* get_apctl_state(PyObject *self, PyObject *args)
{
    int state, ret;

    if (!PyArg_ParseTuple(args, ":get_apctl_state"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    Py_BEGIN_ALLOW_THREADS
       ret = sceNetApctlGetState(&state);
    Py_END_ALLOW_THREADS

    if (ret)
    {
       PyErr_Format(net_error, "sceNetApctlGetState returned 0x%08x", ret);
       return NULL;
    }

    return Py_BuildValue("i", state);
}

static PyObject* disconnect_apctl(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":disconnect_apctl"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    Py_BEGIN_ALLOW_THREADS
       sceNetApctlDisconnect();
    Py_END_ALLOW_THREADS

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* get_ip(PyObject *self, PyObject *args)
{
    char ipaddr[32];

    if (!PyArg_ParseTuple(args, ":get_ip"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (sceNetApctlGetInfo(8, ipaddr) != 0)
    {
       PyErr_SetString(net_error, "Could not get IP");
       return NULL;
    }

    return Py_BuildValue("s", ipaddr);
}

static PyObject* enum_configs(PyObject *self, PyObject *args)
{
    int index = 1;
    PyObject *ret;
    netData name, ip;

    if (!PyArg_ParseTuple(args, ":enum_configs"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    ret = PyList_New(0);

    for (index = 1; index < 100; index++)
    {
       if (sceUtilityCheckNetParam(index))
          break;

       sceUtilityGetNetParam(index, PSP_NETPARAM_NAME, &name);
       sceUtilityGetNetParam(index, PSP_NETPARAM_IP, &ip);

       PyList_Append(ret, Py_BuildValue("iss", index, name.asString, ip.asString));

       index++;
    }

    return ret;
}

static PyObject* wlanIsPowered(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":wlanIsPowered"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", sceWlanDevIsPowerOn());
}

static PyObject* wlanEtherAddr(PyObject *self, PyObject *args)
{
    unsigned char buffer[8];

    if (!PyArg_ParseTuple(args, ":wlanEtherAddr"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (sceWlanGetEtherAddr(buffer) < 0)
    {
       PyErr_SetString(PyExc_RuntimeError, "Could not get ethernet address");
       return NULL;
    }

    buffer[7] = 0;

    return Py_BuildValue("s", buffer);
}

static PyObject* wlanSwitchState(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":wlanSwitchState"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", sceWlanGetSwitchState());
}

static PyMethodDef methods[] = {
   { "connectToAPCTL", (PyCFunction)connect_to_apctl, METH_VARARGS|METH_KEYWORDS, "" },
   { "disconnectAPCTL", (PyCFunction)disconnect_apctl, METH_VARARGS, "" },
   { "getAPCTLState", (PyCFunction)get_apctl_state, METH_VARARGS, "" },
   { "getIP", (PyCFunction)get_ip, METH_VARARGS, "" },
   { "enumConfigs", (PyCFunction)enum_configs, METH_VARARGS, "" },

   { "wlanIsPowered", (PyCFunction)wlanIsPowered, METH_VARARGS, "" },
   { "wlanEtherAddr", (PyCFunction)wlanEtherAddr, METH_VARARGS, "" },
   { "wlanSwitchState", (PyCFunction)wlanSwitchState, METH_VARARGS, "" },

   { NULL }
};

PyMODINIT_FUNC initpspnet(void)
{
    PyObject *mdl;

    /*
    if (PyType_Ready(&PyPSP_AdHocType) < 0)
        return;
    */

    net_error = PyErr_NewException("pspnet.Error", NULL, NULL);
    if (!net_error)
       return;

    mdl = Py_InitModule3("pspnet", methods, "PSP-specific net functions");
    if (!mdl)
       return;

    /*
    Py_INCREF((PyObject*)&PyPSP_AdHocType);
    PyModule_AddObject(mdl, "AdHoc", (PyObject*)&PyPSP_AdHocType);
    */

    PyModule_AddObject(mdl, "Error", net_error);
}

#ifdef GNUC
static const char* _rcsid_pspnet __attribute__((unused)) = "$Id$";
#endif
