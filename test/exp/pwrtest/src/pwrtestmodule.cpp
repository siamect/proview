#include <Python.h>
#include <datetime.h>
#include "pwr.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_string.h"
#include "co_api_user.h"
#include "co_msg.h"
#include "co_time.h"
#include "co_string.h"
#include "co_tst_msg.h"
#include "co_tst_log.h"

/**
 * Doc for pwrtest module
 */
PyDoc_STRVAR(pwrtest_doc,"\
ProviewR Test Python API\n\n\
The test Python API contains a number of classes and functions\n\
for regression test.\n\n\
The API contains the classes\n\n\
Log       logg message to test logfile.\n\n\
Type help(pwrtest.Log) to get more information about\n\
these classes.");

/**
 * Doc for LogObject class
 */
PyDoc_STRVAR(log_doc,"\
ProviewR test logger\n\n\
The Log object represents the test logger.\n\
");

PyDoc_STRVAR(Log_log_doc,"\
log()\n--\n\n\
Log string to logfile.\n\n\
Example\n\
-------\n\
log = pwrtest.logger('myappl','$pwrp_log/myappl.log')\n\
log.log('E', 'Some error detected')\n\
");

PyDoc_STRVAR(Log_vlog_doc,"\
log()\n--\n\n\
Log string to logfile with format.\n\n\
Example\n\
-------\n\
log = pwrtest.logger('myappl','$pwrp_log/myappl.log')\n\
log.vlog('E', 'Error, %s is not equal %s', str1, str2)\n\
");

/*
 * Doc for static methods
 */
PyDoc_STRVAR(pwrtest_logger_doc,"\
logger(name, file)\n--\n\n\
Open logfile.\n\n\
Arguments\n\
---------\n\
name String\n\
  Name of application.\n\
file String\n\
  Name of logfile.\n\n\
Example\n\
-------\n\
pwrtest.logger('myappl', '$pwrp_log/myappl.log')\n");


typedef struct {
  PyObject_HEAD
  tst_log *tstlog;
  char name[80];
} LogObject;

static PyObject *Log_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static PyObject *Log_str(PyObject *self);
static int Log_init(LogObject *self, PyObject *args, PyObject *kwds);

static PyObject *Log_log(LogObject *self, PyObject *args);
static PyObject *Log_vlog(LogObject *self, PyObject *args);

static PyMethodDef Log_methods[] = {
    { "log", (PyCFunction) Log_log, METH_VARARGS, Log_log_doc },
    { "vlog", (PyCFunction) Log_vlog, METH_VARARGS, Log_vlog_doc },
    { NULL }
};

static PyTypeObject LogType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Log",         	       /* tp_name */
    sizeof(LogObject),         /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    Log_str,                   /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    Log_str,                   /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    log_doc,  		       /* tp_doc */    
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,		               /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Log_methods,               /* tp_methods */
    0,               	       /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Log_init,        /* tp_init */
    0,                         /* tp_alloc */
    Log_new,                   /* tp_new */
};

static void *set_error(pwr_tStatus sts) 
{
  char msg[120];
  msg_GetMsg(sts, msg, sizeof(msg));
  PyErr_SetString(PyExc_RuntimeError, msg);
  return NULL;
}

/* 
 * Log object functions
 */
static PyObject *
Log_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  LogObject *self;

  self = (LogObject *)type->tp_alloc(type, 0);
  if (self != NULL) {
    self->tstlog = 0;
  }

  return (PyObject *)self;
}

static PyObject *
Log_str(PyObject *self)
{
  return PyUnicode_FromFormat("%s", ((LogObject *)self)->name);
}

static int
Log_init(LogObject *self, PyObject *args, PyObject *kwds)
{
  char *name;
  char *filename;
  pwr_tStatus sts;

  if (! PyArg_ParseTuple(args, "ss", &name, &filename))
    return -1;

  strncpy(self->name, name, sizeof(self->name));

  self->tstlog = new tst_log(&sts, name, filename);
  if (EVEN(sts)) {
    set_error(sts);
    return -1;
  }

  return 0;
}

static PyObject *
Log_log(LogObject *self, PyObject *args)
{
  const char *str1;
  const char *str2 = 0;
  const char *severity;

  if ( !PyArg_ParseTuple(args, "ss|s", &severity, &str1, &str2))
    return NULL;

  if (str2 == 0)
    self->tstlog->log(severity[0], str1);
  else
    self->tstlog->log(severity[0], str1, str2);

  Py_RETURN_NONE;  
}

static PyObject *
Log_vlog(LogObject *self, PyObject *args)
{
  const char *format;
  const char *str1 = 0;
  const char *str2 = 0;
  const char *str3 = 0;
  const char *str4 = 0;
  const char *str5 = 0;
  const char *severity;

  if ( !PyArg_ParseTuple(args, "ss|sssss", &severity, &format,
			 &str1, &str2, &str3, &str4, &str5))
    return NULL;

  if (str1 == 0)
    self->tstlog->vlog(severity[0], format);
  else if (str2 == 0)
    self->tstlog->vlog(severity[0], format, str1);
  else if (str3 == 0)
    self->tstlog->vlog(severity[0], format, str1, str2);
  else if (str4 == 0)
    self->tstlog->vlog(severity[0], format, str1, str2, str3);
  else if (str5 == 0)
    self->tstlog->vlog(severity[0], format, str1, str2, str3, str4);
  else
    self->tstlog->vlog(severity[0], format, str1, str2, str3, str4, str5);

  Py_RETURN_NONE;  
}

/**
 * Static methods
 */


static PyObject *pwrtest_logger(PyObject *self, PyObject *args)
{
  PyObject *o = Log_new(&LogType, args, 0);
  Log_init((LogObject *)o, args, 0);
  return o;
}

static PyMethodDef PwrtestMethods[] = {
  {"logger", pwrtest_logger, METH_VARARGS, pwrtest_logger_doc},
  {NULL, NULL, 0, NULL}};

PyMODINIT_FUNC initpwrtest(void)
{
  PyObject *m;

  if (PyType_Ready(&LogType) < 0)
    return;

  m = Py_InitModule3("pwrtest", PwrtestMethods, pwrtest_doc);
  if (m == NULL)
    return;

  Py_INCREF(&LogType);
  PyModule_AddObject(m, "Log", (PyObject *)&LogType);

  PyModule_AddIntConstant(m, "FRAME_OPTIONS_CONDITION", 1);

  PyDateTime_IMPORT;
}
