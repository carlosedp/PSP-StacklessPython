
/**
 * @file pspos.c
 */

/**********************************************************************

  Created: 04 Oct 2005

**********************************************************************/
// $Id: pspos.c 1817 2006-03-05 16:49:09Z fraca7 $

#include "Python.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include <pspiofilemgr.h>
#include <pspkernel.h>
#include <psppower.h>
#include <psputility.h>

#include "structseq.h"

#define MKCON(name) PyModule_AddIntConstant(mdl, # name, name)

extern char* formatPSPError(int); // in ../psperror.c

static PyObject* PyPSP_open(PyObject *self,
                            PyObject *args)
{
    char *path = NULL;
    int flags;
    int mode = 0777;
    int fd;


	if (!PyArg_ParseTuple(args, "eti|i",
	                      Py_FileSystemDefaultEncoding, &path,
	                      &flags, &mode))
		return NULL;

	Py_BEGIN_ALLOW_THREADS
	fd = open(path, flags, mode);
	Py_END_ALLOW_THREADS
	if (fd < 0)
        PyErr_SetString(PyExc_OSError, formatPSPError(fd));
        return NULL;
	PyMem_Free(path);
	return PyInt_FromLong((long)fd);

/*
    if (!PyArg_ParseTuple(args, "si|i:open", &path, &flags, &mode))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;
    Py_BEGIN_ALLOW_THREADS
    fd = open(path, flags,mode);
    Py_END_ALLOW_THREADS
    if (fd < 0)
    {
       PyErr_SetString(PyExc_OSError, formatPSPError(fd));
       return NULL;
    }
    return PyInt_FromLong((long)fd);
*/
}

static int closefile(FILE *fp)
{
    return fclose(fp);
}

static PyObject* PyPSP_fdopen(PyObject *self,
                              PyObject *args)
{
    int fd;
    char *mode = "r";
    FILE *fp;

    if (!PyArg_ParseTuple(args, "i|s:fdopen", &fd, &mode))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;
    Py_BEGIN_ALLOW_THREADS
    fp = fdopen(fd, mode);
    Py_END_ALLOW_THREADS
    
    return PyFile_FromFile(fp, "", mode, closefile); // FIXME: name ?
}

static PyObject *PyPSP_close(PyObject *self, PyObject *args)
{
	int fd, res;
	if (!PyArg_ParseTuple(args, "i:close", &fd))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = close(fd);
	Py_END_ALLOW_THREADS
	if (res < 0)
		return PyErr_SetFromErrno(PyExc_OSError);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *PyPSP_access(PyObject *self, PyObject *args)
{
	char *path;
	int mode;
	int res;
	if (!PyArg_ParseTuple(args, "eti:access",
			      Py_FileSystemDefaultEncoding, &path, &mode))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = access(path, mode);
	Py_END_ALLOW_THREADS
	PyMem_Free(path);
	return PyBool_FromLong(res == 0);
}

static PyObject* PyPSP_unlink(PyObject *self,
                              PyObject *args)
{
    char *path;

    if (!PyArg_ParseTuple(args, "s:unlink", &path))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (unlink(path))
    {
       PyErr_SetFromErrno(PyExc_OSError);
       return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_rename(PyObject *self,
                              PyObject *args)
{
    char *src, *dst;

    if (!PyArg_ParseTuple(args, "ss:rename", &src, &dst))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (rename(src, dst))
    {
       PyErr_SetFromErrno(PyExc_OSError);
       return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_system(PyObject *self,
                              PyObject *args)
{
    char *cmd;

    if (!PyArg_ParseTuple(args, "s:system", &cmd))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    return PyInt_FromLong(system(cmd));
}

static PyObject* PyPSP_rmdir(PyObject *self,
                             PyObject *args)
{
    char *path;

    if (!PyArg_ParseTuple(args, "s:rmdir", &path))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (rmdir(path))
    {
       PyErr_SetFromErrno(PyExc_OSError);
       return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_chdir(PyObject *self,
                             PyObject *args)
{
    char *path;

    if (!PyArg_ParseTuple(args, "s:chdir", &path))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (chdir(path))
    {
       PyErr_SetFromErrno(PyExc_OSError);
       return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_getcwd(PyObject *self,
                              PyObject *args)
{
    char *cwd = NULL;
    int sz = 128;
    PyObject *ret;

    if (!PyArg_ParseTuple(args, ":getcwd"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    for (;;)
    {
       char *n;

       sz *= 2;
       n = (char*)realloc(cwd, sz);

       if (!n)
       {
          if (cwd) free(cwd);
          PyErr_SetString(PyExc_MemoryError, "");
          return NULL;
       }

       cwd = n;

       if (!getcwd(cwd, sz))
       {
          if (errno == ERANGE)
          {
             continue;
          }

          PyErr_SetFromErrno(PyExc_OSError);
          if (cwd) free(cwd);
       }

       break;
    }

    ret = PyString_FromString(cwd);
    free(cwd);
    return ret;
}

static PyObject* PyPSP_mkdir(PyObject *self,
                             PyObject *args)
{
    char *path;
    int mode = 0, err;

    if (!PyArg_ParseTuple(args, "s|i:mkdir", &path, &mode))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    //err = sceIoMkdir(path, mode);
    err = mkdir(path, mode);
    if (err)
    {
       PyErr_SetString(PyExc_OSError, formatPSPError(err));
       return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_listdir(PyObject *self,
                               PyObject *args)
{
    char *path;
    int fd;

    if (!PyArg_ParseTuple(args, "s:listdir", &path))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    fd = sceIoDopen(path);
    if (fd > 0)
    {
       SceIoDirent dr;

       PyObject *ret;
       int r;

       // Without this memset, the PSP reboots...
       memset(&dr, 0, sizeof(dr));

       ret = PyList_New(0);

       for (;;)
       {
          r = sceIoDread(fd, (SceIoDirent*)&dr);

          if (r < 0)
          {
             PyErr_SetString(PyExc_OSError, formatPSPError(r));
             Py_DECREF(ret);
             sceIoDclose(fd);
             return NULL;
          }

          if (r == 0)
             break;

          if (strcmp(dr.d_name, ".") && strcmp(dr.d_name, ".."))
          {
             char *name = dr.d_name;

             while (*name)
                *name++ = tolower(*name);

             PyList_Append(ret, PyString_FromString(dr.d_name)); // FIXME: reference count ?
          }
       }

       sceIoDclose(fd);
       return ret;
    }
    else
    {
       PyErr_SetString(PyExc_OSError, formatPSPError(fd));
       return NULL;
    }
}

PyDoc_STRVAR(stat_result__doc__,
"stat_result: Result from stat or lstat.\n\n\
This object may be accessed either as a tuple of\n\
  (mode,ino,dev,nlink,uid,gid,size,atime,mtime,ctime)\n\
or via the attributes st_mode, st_ino, st_dev, st_nlink, st_uid, and so on.\n\
\n\
See os.stat for more information.");

static PyStructSequence_Field stat_result_fields[] = {
   { "st_mode",  "protection bits" },
   { "st_ino",   "inode" },
   { "st_dev",   "device" },
   { "st_nlink", "number of hard links" },
   { "st_uid",   "user ID of owner" },
   { "st_gid",   "group ID of owner" },
   { "st_size",  "total size, in bytes" },
   { "st_atime", "time of last access" },
   { "st_mtime", "time of last modification" },
   { "st_ctime", "time of last change" },

   { 0 }
};

static PyStructSequence_Desc stat_result_desc = {
   "pspos.stat_result",
   stat_result__doc__,
   stat_result_fields,
   10
};

static PyTypeObject StatResultType;

static PyObject* PyPSP_stat(PyObject *self,
                            PyObject *args)
{
    char *path;
    PyObject *ret;
    struct stat st;
    int err;

    if (!PyArg_ParseTuple(args, "s:stat", &path))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    err = stat(path, &st);
    if (err)
    {
       PyErr_SetString(PyExc_OSError, formatPSPError(err));
       return NULL;
    }

    ret = PyStructSequence_New(&StatResultType);
    PyStructSequence_SET_ITEM(ret,  0, PyInt_FromLong(st.st_mode));
    PyStructSequence_SET_ITEM(ret,  1, PyInt_FromLong(st.st_ino));
    PyStructSequence_SET_ITEM(ret,  2, PyInt_FromLong(st.st_rdev));
    PyStructSequence_SET_ITEM(ret,  3, PyInt_FromLong(st.st_nlink));
    PyStructSequence_SET_ITEM(ret,  4, PyInt_FromLong(st.st_uid));
    PyStructSequence_SET_ITEM(ret,  5, PyInt_FromLong(st.st_gid));
    PyStructSequence_SET_ITEM(ret,  6, PyInt_FromLong(st.st_size));
    PyStructSequence_SET_ITEM(ret,  7, PyInt_FromLong((long)st.st_atime));
    PyStructSequence_SET_ITEM(ret,  8, PyInt_FromLong((long)st.st_mtime));
    PyStructSequence_SET_ITEM(ret,  9, PyInt_FromLong((long)st.st_ctime));

    return ret;
}

static PyObject* PyPSP_chmod(PyObject *self,
                             PyObject *args)
{
    char *path;
    int mode;

    if (!PyArg_ParseTuple(args, "si:chmod", &path, &mode))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    // FIXME

    /*
    if (chmod(path, mode))
    {
       PyErr_SetString(PyExc_OSError, "FIXME");
       return NULL;
    }
    */

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_utime(PyObject *self,
                             PyObject *args)
{
    // TODO

    if (PyErr_CheckSignals())
       return NULL;

    PyErr_SetString(PyExc_NotImplementedError, "Not implemented");
    return NULL;
}

static PyObject* PyPSP_getenv(PyObject *self,
                              PyObject *args)
{
    char *name;

    if (!PyArg_ParseTuple(args, "s:getenv", &name))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    // No such thing here...

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_putenv(PyObject *self,
                              PyObject *args)
{
    char *name, *val;

    if (!PyArg_ParseTuple(args, "ss:putenv", &name, &val))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    // No such thing here...

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_delenv(PyObject *self,
                              PyObject *args)
{
    char *name;

    if (!PyArg_ParseTuple(args, "s:delenv", &name))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    // No such thing here...

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_getenvdict(PyObject *self,
                                  PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":getenvdict"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    return PyDict_New();
}


//==========================================================================
// Memory

static PyObject* PyPSP_freemem(PyObject *self,
                               PyObject *args)
{
    SceSize mem;

    if (!PyArg_ParseTuple(args, ":freemem"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    mem = sceKernelTotalFreeMemSize();

    return Py_BuildValue("i", (int)mem);
}


//============================================================================
// Battery

static PyObject* PyPSP_battery(PyObject *self,
                               PyObject *args)
{
    int plugged, present, charging, lifep, lifet, temp, volt;

    if (!PyArg_ParseTuple(args, ":battery"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    plugged = scePowerIsPowerOnline();
    present = scePowerIsBatteryExist();
    charging = scePowerIsBatteryCharging();
    lifep = scePowerGetBatteryLifePercent();
    lifet = scePowerGetBatteryLifeTime();
    temp = scePowerGetBatteryTemp();
    volt = scePowerGetBatteryVolt();

    return Py_BuildValue("iiiiiii", plugged, present, charging, lifep,
                         lifet, temp, volt);
}


//==========================================================================
// CPU speed control

void setClockBus(int cpufreq, int busfreq) {
    scePowerSetClockFrequency(cpufreq < 19 ? 19 : cpufreq, cpufreq, busfreq);
    return;
}


static PyObject* PyPSP_setclocks(PyObject *self,
                                PyObject *args)
{
    int cpufreq, busfreq;

    if (!PyArg_ParseTuple(args, "ii:setclocks", &cpufreq, &busfreq))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if ((cpufreq < 1) || (cpufreq > 333))
    {
       PyErr_Format(PyExc_OSError, "Bad CPU frequency: %d", cpufreq);
       return NULL;
    }
    if ((busfreq < 1) || (busfreq > 167))
    {
       PyErr_Format(PyExc_OSError, "Bad BUS frequency: %d", busfreq);
       return NULL;
    }
    setClockBus(cpufreq, busfreq);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_getclocks(PyObject *self,
                                PyObject *args)
{

    if (!PyArg_ParseTuple(args, ":getclocks"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("ii", scePowerGetCpuClockFrequency(),
                               scePowerGetBusClockFrequency());
}

static PyObject* PyPSP_setclock(PyObject *self,
                                PyObject *args)
{
    int freq;
    if (!PyArg_ParseTuple(args, "i:setclock", &freq))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if ((freq < 1) || (freq > 333))
    {
       PyErr_Format(PyExc_OSError, "Bad CPU frequency: %d", freq);
       return NULL;
    }
    scePowerSetCpuClockFrequency(freq);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_getclock(PyObject *self,
                                PyObject *args)
{

    if (!PyArg_ParseTuple(args, ":getclock"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", scePowerGetCpuClockFrequency());
}

static PyObject* PyPSP_setbus(PyObject *self,
                              PyObject *args)
{
    int freq;

    if (!PyArg_ParseTuple(args, "i:setbus", &freq))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if ((freq < 1) || (freq > 167))
    {
       PyErr_Format(PyExc_OSError, "Bad bus frequency: %d", freq);
       return NULL;
    }
    
    scePowerSetBusClockFrequency(freq);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PyPSP_getbus(PyObject *self,
                              PyObject *args)
{

    if (!PyArg_ParseTuple(args, ":getbus"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    return Py_BuildValue("i", scePowerGetBusClockFrequency());
}

static PyObject* PyPSP_powertick(PyObject *self,
                                 PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":powertick"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    scePowerTick(0);

    Py_INCREF(Py_None);
    return Py_None;
}


//==========================================================================
// Generic system parameters

static PyObject* PyPSP_getsystemparam(PyObject *self, PyObject *args)
{
    int id;
    PyObject *ret;

    if (!PyArg_ParseTuple(args, "i:getsystemparam", &id))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if (id == PSP_SYSTEMPARAM_ID_STRING_NICKNAME)
    {
       char sValue[256];

       if (sceUtilityGetSystemParamString(id, sValue, sizeof(sValue)) == PSP_SYSTEMPARAM_RETVAL_FAIL)
       {
          PyErr_SetString(PyExc_OSError, "Could not get system parameter");
          return NULL;
       }

       ret = Py_BuildValue("s", sValue);
    }
    else
    {
       int iValue;

       if (sceUtilityGetSystemParamInt(id, &iValue) == PSP_SYSTEMPARAM_RETVAL_FAIL)
       {
          PyErr_SetString(PyExc_OSError, "Could not get system parameter");
          return NULL;
       }

       ret = Py_BuildValue("i", iValue);
    }

    return ret;
}

/*
static PyObject* PyPSP_setsystemparam(PyObject *self, PyObject *args)
{
    int id;
    PyObject *val;

    if (!PyArg_ParseTuple(args, "iO:setsystemparam", &id, &val))
       return NULL;

    if (PyInt_Check(val))
    {
       int iValue = PyInt_AsLong(val);

       if (sceUtilitySetSystemParamInt(id, iValue) == PSP_SYSTEMPARAM_RETVAL_FAIL)
       {
          PyErr_SetString(PyExc_OSError, "Could not set integer system parameter");
          return NULL;
       }
    }
    else if (PyString_Check(val))
    {
       char *sValue = PyString_AsString(val);

       if (sceUtilitySetSystemParamString(id, sValue) == PSP_SYSTEMPARAM_RETVAL_FAIL)
       {
          PyErr_SetString(PyExc_OSError, "Could not set integer system parameter");
          return NULL;
       }
    }
    else
    {
       PyErr_SetString(PyExc_TypeError, "Bad type, must be integer or string");
       return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}
*/


//==========================================================================
// Specific system parameters

static PyObject* PyPSP_getnickname(PyObject *self, PyObject *args)
{
    char nickname[256];

    if (!PyArg_ParseTuple(args, ":getnickname"))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    if(sceUtilityGetSystemParamString(PSP_SYSTEMPARAM_ID_STRING_NICKNAME,
                                      nickname,
                                      256) == PSP_SYSTEMPARAM_RETVAL_FAIL)
    {
       PyErr_SetString(PyExc_OSError, "Could not get nickname");
       return NULL;
    }

    return Py_BuildValue("s", nickname);
}

/*
static PyObject* PyPSP_setnickname(PyObject *self, PyObject *args)
{
    char *nickname;

    if (!PyArg_ParseTuple(args, "s:setnickname", &nickname))
       return NULL;

    if (sceUtilitySetSystemParamString(PSP_SYSTEMPARAM_ID_STRING_NICKNAME,
                                       nickname) == PSP_SYSTEMPARAM_RETVAL_FAIL)
    {
       PyErr_SetString(PyExc_OSError, "Could not set nickname");
       return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}
*/


//==========================================================================
//

typedef struct
{
      void *buffer;
      void *next;
} _LINK;

static PyObject* PyPSP_realmem(PyObject *self, PyObject *args)
{
    int size = 4096, total = 0;
    _LINK *first = NULL, *current = NULL, *lnk;

    if (!PyArg_ParseTuple(args, "|i:realmem", &size))
       return NULL;

    if (PyErr_CheckSignals())
       return NULL;

    while (1)
    {
       lnk = (_LINK*)malloc(sizeof(_LINK));
       if (!lnk)
          break;

       total += sizeof(_LINK);

       lnk->buffer = malloc(size);
       if (!lnk->buffer)
       {
          free(lnk);
          break;
       }

       total += size;
       lnk->next = NULL;

       if (current)
       {
          current->next = (void*)lnk;
          current = lnk;
       }
       else
       {
          current = first = lnk;
       }
    }

    lnk = first;
    while (lnk)
    {
       free(lnk->buffer);
       current = lnk->next;
       free(lnk);
       lnk = current;
    }

    return Py_BuildValue("i", total);
}


//==========================================================================
//

static PyMethodDef pspos_functions[] = {
   { "open", PyPSP_open, METH_VARARGS, "" },
   { "close", PyPSP_close, METH_VARARGS, "" },
   { "access", PyPSP_access, METH_VARARGS, "" },
   { "fdopen", PyPSP_fdopen, METH_VARARGS, "" },
   { "unlink", PyPSP_unlink, METH_VARARGS, "" },
   { "remove", PyPSP_unlink, METH_VARARGS, "" },
   { "rename", PyPSP_rename, METH_VARARGS, "" },
   { "system", PyPSP_system, METH_VARARGS, "" },
   { "rmdir", PyPSP_rmdir, METH_VARARGS, "" },
   { "chdir", PyPSP_chdir, METH_VARARGS, "" },
   { "getcwd", PyPSP_getcwd, METH_VARARGS, "" },
   { "mkdir", PyPSP_mkdir, METH_VARARGS, "" },
   { "listdir", PyPSP_listdir, METH_VARARGS, "" },
   { "stat", PyPSP_stat, METH_VARARGS, "" },
   { "lstat", PyPSP_stat, METH_VARARGS, "" },
   { "chmod", PyPSP_chmod, METH_VARARGS, "" },
   { "utime", PyPSP_utime, METH_VARARGS, "" },
   { "getenv", PyPSP_getenv, METH_VARARGS, "" },
   { "putenv", PyPSP_putenv, METH_VARARGS, "" },
   { "delenv", PyPSP_delenv, METH_VARARGS, "" },
   { "getenvdict", PyPSP_getenvdict, METH_VARARGS, "" },

   { "freemem", PyPSP_freemem, METH_VARARGS, "" },
   { "battery", PyPSP_battery, METH_VARARGS, "" },
   { "setclocks", PyPSP_setclocks, METH_VARARGS, "" },
   { "getclocks", PyPSP_getclocks, METH_VARARGS, "" },
   { "setclock", PyPSP_setclock, METH_VARARGS, "" },
   { "getclock", PyPSP_getclock, METH_VARARGS, "" },
   { "setbus", PyPSP_setbus, METH_VARARGS, "" },
   { "getbus", PyPSP_getbus, METH_VARARGS, "" },
   { "powertick", PyPSP_powertick, METH_VARARGS, "" },

   { "getsystemparam", PyPSP_getsystemparam, METH_VARARGS, "" },
   //{ "setsystemparam", PyPSP_setsystemparam, METH_VARARGS, "" },

   { "getnickname", PyPSP_getnickname, METH_VARARGS, "" },
   //{ "setnickname", PyPSP_setnickname, METH_VARARGS, "" },

   { "realmem", PyPSP_realmem, METH_VARARGS, "" },

   { NULL }
};

void initpspos(void)
{
    PyObject *mdl;

    mdl = Py_InitModule3("pspos", pspos_functions, "");
    if (!mdl)
       return;

    PyStructSequence_InitType(&StatResultType, &stat_result_desc);
    Py_INCREF(&StatResultType);
    PyModule_AddObject(mdl, "stat_result", (PyObject*)&StatResultType);

    MKCON(O_RDONLY);
    MKCON(O_WRONLY);
    MKCON(O_RDWR);
    MKCON(O_CREAT);
    MKCON(O_EXCL);

    MKCON(PSP_SYSTEMPARAM_ID_STRING_NICKNAME);
    MKCON(PSP_SYSTEMPARAM_ID_INT_ADHOC_CHANNEL);
    MKCON(PSP_SYSTEMPARAM_ID_INT_WLAN_POWERSAVE);
    MKCON(PSP_SYSTEMPARAM_ID_INT_DATE_FORMAT);
    MKCON(PSP_SYSTEMPARAM_ID_INT_TIME_FORMAT);
    MKCON(PSP_SYSTEMPARAM_ID_INT_TIMEZONE);
    MKCON(PSP_SYSTEMPARAM_ID_INT_DAYLIGHTSAVINGS);
    MKCON(PSP_SYSTEMPARAM_ID_INT_LANGUAGE);

    // For ADHOC_CHANNEL
    MKCON(PSP_SYSTEMPARAM_ADHOC_CHANNEL_AUTOMATIC);
    MKCON(PSP_SYSTEMPARAM_ADHOC_CHANNEL_1);
    MKCON(PSP_SYSTEMPARAM_ADHOC_CHANNEL_6);
    MKCON(PSP_SYSTEMPARAM_ADHOC_CHANNEL_11);

    // For WLAN_POWERSAVE
    MKCON(PSP_SYSTEMPARAM_WLAN_POWERSAVE_ON);
    MKCON(PSP_SYSTEMPARAM_WLAN_POWERSAVE_OFF);

    // For DATE_FORMAT
    MKCON(PSP_SYSTEMPARAM_DATE_FORMAT_YYYYMMDD);
    MKCON(PSP_SYSTEMPARAM_DATE_FORMAT_MMDDYYYY);
    MKCON(PSP_SYSTEMPARAM_DATE_FORMAT_DDMMYYYY);

    // For TIME_FORMAT
    MKCON(PSP_SYSTEMPARAM_TIME_FORMAT_12HR);
    MKCON(PSP_SYSTEMPARAM_TIME_FORMAT_24HR);

    // For DAYLIGHT_SAVINGS
    MKCON(PSP_SYSTEMPARAM_DAYLIGHTSAVINGS_STD);
    MKCON(PSP_SYSTEMPARAM_DAYLIGHTSAVINGS_SAVING);

    // For LANGUAGE
    MKCON(PSP_SYSTEMPARAM_LANGUAGE_JAPANESE);
    MKCON(PSP_SYSTEMPARAM_LANGUAGE_ENGLISH);
    MKCON(PSP_SYSTEMPARAM_LANGUAGE_FRENCH);
    MKCON(PSP_SYSTEMPARAM_LANGUAGE_SPANISH);
    MKCON(PSP_SYSTEMPARAM_LANGUAGE_GERMAN);
    MKCON(PSP_SYSTEMPARAM_LANGUAGE_ITALIAN);
    MKCON(PSP_SYSTEMPARAM_LANGUAGE_DUTCH);
    MKCON(PSP_SYSTEMPARAM_LANGUAGE_PORTUGUESE);
    MKCON(PSP_SYSTEMPARAM_LANGUAGE_KOREAN);

    Py_INCREF(PyExc_OSError);
    PyModule_AddObject(mdl, "error", PyExc_OSError);
}
