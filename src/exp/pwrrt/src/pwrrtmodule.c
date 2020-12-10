#include <Python.h>
#include <datetime.h>
#include "pwr.h"
#include "pwr_privilege.h"
#include "co_cdh.h"
#include "co_cnv.h"
#include "co_string.h"
#include "co_api_user.h"
#include "co_msg.h"
#include "co_time.h"
#include "co_string.h"
#include "rt_qcom.h"
#include "rt_gdh.h"
#include "rt_errh.h"
#include "rt_aproc.h"
#include "rt_sevcli.h"
#include "rt_ini_event.h"
#include "rt_gdh_msg.h"
#include "rt_sev_msg.h"
#include "rt_qcom_msg.h"
#include "rt_pwr_msg.h"
#include "co_user_msg.h"

static int pwrrt_initialized = 0;
static unsigned int pwrrt_priv = pwr_mPrv_RtRead;
static char pwrrt_user[80] = "";
static sevcli_tCtx pwrrt_scctx = 0;

/**
 * Doc for pwrrt module
 */
PyDoc_STRVAR(pwrrt_doc,"\
ProviewR Runtime Python API\n\n\
The runtime Python API contains a number of classes and functions\n\
to get information about the objects in the ProviewR runtime database.\n\
There are function to find objects and investigate their relationship\n\
to other objects and to examine the structure and content of the objects.\n\
There are also functions to set up subscriptions to continuously get new\n\
updates of attribute values.\n\n\
The API contains the classes\n\n\
Oid       object in the runtime database.\n\
Aref      attribute in the runtime database.\n\
Sub       subscription.\n\
Cid       class.\n\
Tid       type.\n\
ADef      attribute definition\n\n\
Type help(pwrrt.Oid), help(pwrrt.Aref) etc to get more information about\n\
these classes.");

/**
 * Doc for OidObject class
 */
PyDoc_STRVAR(oid_doc,"\
ProviewR runtime object\n\n\
The Oid object represents a ProviewR object.\n\
");

PyDoc_STRVAR(oid_name_doc,"\
name()\n--\n\n\
Get the name of the object.\n\
Returns the last segment of the object name, without the path.\n\n\
Returns\n\
-------\n\
Returns a string with the object name.\n\n\
Example\n\
-------\n\
o = pwrrt.object('H1-H2')\n\
print o.name()\n\
");

PyDoc_STRVAR(oid_fullName_doc,"\
fullName()\n--\n\n\
Get the full name of the object.\n\
Returns the full object name, including volume and path.\n\n\
Returns\n\
-------\n\
Returns a string with the full object name.\n\n\
Example\n\
-------\n\
o = pwrrt.object('H1-H2')\n\
print o.fullName()\n\
");

PyDoc_STRVAR(oid_next_doc,"\
next()\n--\n\n\
Get the next sibling of the object.\n\n\
Returns\n\
-------\n\
Returns an Oid object for the sibling. If no next sibling exist, a\n\
None object is returned.\n\n\
Example\n\
-------\n\
o = pwrrt.object('H1-H2')\n\
child = o.child()\n\
while (child):\n\
  print child.name()\n\
  child = child.next()\n\
");

PyDoc_STRVAR(oid_parent_doc,"\
parent()\n--\n\n\
Get the parent of the object.\n\n\
Returns\n\
-------\n\
Returns an Oid object for the parent.\n\n\
Example\n\
-------\n\
o = pwrrt.object('H1-H2')\n\
parent = o.parent()\n\
print parent.name()\n\
");

PyDoc_STRVAR(oid_child_doc,"\
child()\n--\n\n\
Get first child of the object.\n\n\
Returns\n\
-------\n\
Returns an Oid object for the first child.\n\n\
Example\n\
-------\n\
o = pwrrt.object('H1-H2')\n\
child = o.child()\n\
while child:\n\
  print child.name()\n\
  child = child.next()\n\
");

PyDoc_STRVAR(oid_children_doc,"\
children()\n--\n\n\
Get all children of the object.\n\n\
Returns\n\
-------\n\
Returns a tuple containg Oid objects for all children of the object.\n\n\
Example\n\
-------\n\
o = pwrrt.object('H1-H2')\n\
for child in o.children()\n\
  print child.name()\n\
");

PyDoc_STRVAR(oid_cid_doc,"\
cid()\n--\n\n\
Get object class.\n\n\
Returns\n\
-------\n\
Returns a Cid object for the class.\n\n\
Example\n\
-------\n\
o = pwrrt.object('H1-Dv1')\n\
c = o.cid()\n\
print c.fullName()\n\
");

PyDoc_STRVAR(oid_oidStr_doc,"\
oidStr()\n--\n\n\
Get object identity as a string.\n\n\
Arguments\n\
---------\n\
format Optional String\n\
  If not supplied oix is in decimal form.\n\
  'hex' oix in hexadecimal.\n\
  'file' for hex file format.\n\n\
Returns\n\
-------\n\
Returns a string with the object identity.\n\n\
Example\n\
-------\n\
o = pwrrt.object('H1-Dv1')\n\
print o.oidStr()\n\
'_O0.1.200.3:334'\n\
");

PyDoc_STRVAR(oid_attribute_doc,"\
attribute(attributeName)\n--\n\n\
Get an attribute within the object.\n\n\
Arguments\n\
---------\n\
attributeName String\n\
  Name of attribute.\n\n\
Returns\n\
-------\n\
Returns an Aref object for the attribute.\n\n\
Example\n\
-------\n\
o = pwrrt.object('H1-Dv1')\n\
a = o.attribute('ActualValue')\n\
print a.value()\n");

/*
 * Doc for ArefObject class
 */
PyDoc_STRVAR(aref_name_doc,"\
name()\n--\n\n\
Get the name of the attribute.\n\
Returns the last segment of the object and attribute name, without\n\
 the path.\n\n\
Returns\n\
-------\n\
Returns a string with the attribute name.\n\n\
Example\n\
-------\n\
a = pwrrt.attribute('H1-H2-Dv1.ActualValue')\n\
print a.name()\n\
");

PyDoc_STRVAR(aref_fullName_doc,"\
fullName()\n--\n\n\
Get the full name of the attribute.\n\
Returns the full attribute name, including volume and path.\n\n\
Returns\n\
-------\n\
Returns a string with the full attribute name.\n\n\
Example\n\
-------\n\
a = pwrrt.attribute('H1-H2-Dv1.ActualValue')\n\
print a.fullName()\n\
");

PyDoc_STRVAR(aref_arefStr_doc,"\
arefStr()\n--\n\n\
Get attribute reference as a string.\n\n\
Returns\n\
-------\n\
Returns a string with the attribute reference.\n\n\
Example\n\
-------\n\
o = pwrrt.attribute('H1-Dv1.ActualValue')\n\
print a.arefStr()\n\
'_A0.1.200.3:334(_T0.2:0.111.1)[88.4]'\n\
");

PyDoc_STRVAR(aref_tid_doc,"\
tid()\n--\n\n\
Get attribute type.\n\n\
Returns\n\
-------\n\
Returns a Tid object for the type.\n\n\
Example\n\
-------\n\
a = pwrrt.attribute('H1-Dv1.ActualValue')\n\
t = o.tid()\n\
print t.fullName()\n\
");

PyDoc_STRVAR(aref_value_doc,"\
value()\n--\n\n\
Get the value of an attribute.\n\n\
Returns\n\
-------\n\
Returns the value of the attribute.\n\n\
Example\n\
-------\n\
a = pwrrt.object('H1-Dv1.ActualValue')\n\
print a.value()\n\
");

PyDoc_STRVAR(aref_setValue_doc,"\
setValue(value, publicwrite)\n--\n\n\
Set the value of an attribute.\n\
Set is only permitted if a user with RtWrite or System privileges\n\
is logged in, or if the attribute is an attribute with public write\n\
permissions. In this case the public write argument should be set to 1.\n\n\
Arguments\n\
---------\n\
value         Arbitrary type\n\
  Value to set into attribute.\n\
publicwrite   Optional Integer\n\
  1 if value is set to an attribute with public write permissions.\n\n\
Example\n\
-------\n\
pwrrt.login('pwrp', 'somepassword')\n\
a = pwrrt.attribute('H1-H2-Dv1.ActualValue')\n\
a.setValue(1)\n\
");

PyDoc_STRVAR(aref_subscribe_doc,"\
subscribe()\n--\n\n\
Set up a subscription on an attribute.\n\n\
Returns\n\
-------\n\
Returns a Sub object for the subscription.\n\n\
Example\n\
-------\n\
a = pwrrt.object('H1-H2-Dv1.ActualValue')\n\
sub = a.subscribe()\n\
print sub.value()\n\
");

/*
 * Doc for CidObject class
 */

PyDoc_STRVAR(cid_name_doc,"\
name()\n--\n\n\
Get the name of the class.\n\n\
Returns\n\
-------\n\
Returns a string with the class name.\n\n\
Example\n\
-------\n\
c = pwrrt.Cid('$PlantHier')\n\
print c.name()\n\
'$PlantHier'\n\
");

PyDoc_STRVAR(cid_fullName_doc,"\
fullName()\n--\n\n\
Get the full name of the class object.\n\n\
Returns\n\
-------\n\
Returns a string with the class name.\n\n\
Example\n\
-------\n\
c = pwrrt.Cid('$PlantHier')\n\
print c.fullName()\n\
'pwrs:Class-$PlantHier'\n\
");

PyDoc_STRVAR(cid_object_doc,"\
object()\n--\n\n\
Get the first instance of the class.\n\
The object() method only gets whole objects and doesn't include\n\
attribute objects (see attrObject()).\n\n\
Returns\n\
-------\n\
Returns an Oid object for the first instance of the class.\n\n\
Example\n\
-------\n\
cid = pwrrt.Cid('$PlantHier')\n\
o = cid.object()\n\
while o:\n\
  print o.fullName()\n\
  o = cid.nextObject(o)\n\
");

PyDoc_STRVAR(cid_nextObject_doc,"\
nextObject(object)\n--\n\n\
Get the next instance of the class.\n\n\
Arguments\n\
---------\n\
object  Oid object\n\
  An Oid object for the previous instance.\n\n\
Returns\n\
-------\n\
Returns an Oid object for the next instance of the class.\n\n\
Example\n\
-------\n\
cid = pwrrt.Cid('$PlantHier')\n\
o = cid.object()\n\
while o:\n\
  print o.fullName()\n\
  o = cid.nextObject(o)\n\
");

PyDoc_STRVAR(cid_objects_doc,"\
objects()\n--\n\n\
Get all instances of the class.\n\n\
Returns\n\
-------\n\
Returns tuple of Oid object for all instance of the class.\n\n\
Example\n\
-------\n\
cid = pwrrt.Cid('$PlantHier')\n\
for o in cid.objects():\n\
  print o.fullName()\n\
");

PyDoc_STRVAR(cid_attrObject_doc,"\
attrObject()\n--\n\n\
Get the first instance of the class, including attribute objects.\n\n\
Returns\n\
-------\n\
Returns an Aref object for the first instance of the class.\n\n\
Example\n\
-------\n\
cid = pwrrt.Cid('Di')\n\
a = cid.attrObject()\n\
while a:\n\
  print a.fullName()\n\
  a = cid.nextAttrObject(a)\n\
");

PyDoc_STRVAR(cid_nextAttrObject_doc,"\
nextAttrObject(attribute)\n--\n\n\
Get the next instance of the class, including attribute objects.\n\n\
Arguments\n\
---------\n\
attribute  Aref object\n\
  An Aref object for the previous instance.\n\n\
Returns\n\
-------\n\
Returns an Aref object for the next instance of the class.\n\n\
Example\n\
-------\n\
cid = pwrrt.Cid('Di')\n\
a = cid.attrObject()\n\
while a:\n\
  print a.fullName()\n\
  a = cid.nextAttrObject(a)\n\
");

PyDoc_STRVAR(cid_attrObjects_doc,"\
attrObjects()\n--\n\n\
Get all instances of the class, including attribute objects.\n\n\
Returns\n\
-------\n\
Returns tuple of Aref object for all instance of the class.\n\n\
Example\n\
-------\n\
cid = pwrrt.Cid('Di')\n\
for a in cid.attrObjects():\n\
  print a.fullName()\n\
");

PyDoc_STRVAR(cid_attributes_doc,"\
attributes()\n--\n\n\
Get all attributes defined in the class.\n\n\
Returns\n\
-------\n\
Returns tuple of ADef objects for all the defined attributes in\n\
the class.\n\n\
Example\n\
-------\n\
cid = pwrrt.Cid('$PlantHier')\n\
for adef in cid.attributes():\n\
  print adef.name(), adef.offset(), adef.size()\n\
");

/*
 * Doc for TidObject class
 */

PyDoc_STRVAR(tid_name_doc,"\
name()\n--\n\n\
Get the name of the type.\n\n\
Returns\n\
-------\n\
Returns a string with the type name.\n\n\
Example\n\
-------\n\
a = pwrrt.attribute('H1-H2-Dv1.ActualValue')\n\
tid = a.tid()\n\
print tid.name()\n\
'$Boolean'\n\
");

PyDoc_STRVAR(tid_fullName_doc,"\
fullName()\n--\n\n\
Get the full name of the type object.\n\n\
Returns\n\
-------\n\
Returns a string with the full type name.\n\n\
Example\n\
-------\n\
a = pwrrt.attribute('H1-H2-Dv1.ActualValue')\n\
tid = a.tid()\n\
print tid.fullName()\n\
'pwrs:Type-$Boolean'\n\
");

/*
 * Doc for ADefObject class
 */
PyDoc_STRVAR(adef_name_doc,"\
name()\n--\n\n\
Get the name of the attribute.\n\n\
Returns\n\
-------\n\
Returns a string with the attribute name.\n\n\
Example\n\
-------\n\
c = pwrrt.Cid('$PlantHier')\n\
for adef in c.attributes():\n\
  print adef.name()\n\
'Description'\n\
'DefGraph'\n\
...\n\
");

PyDoc_STRVAR(adef_cid_doc,"\
cid()\n--\n\n\
Get the class identity for the attribute definition object.\n\n\
Returns\n\
-------\n\
Returns the class identity as an integer value.\n\
");

PyDoc_STRVAR(adef_offset_doc,"\
offset()\n--\n\n\
Get offset of the attribute.\n\n\
Returns\n\
-------\n\
Returns an integer value with the offset.\n\
");

PyDoc_STRVAR(adef_size_doc,"\
size()\n--\n\n\
Get size of the attribute.\n\n\
Returns\n\
-------\n\
Returns an integer value with the size.\n\
");

PyDoc_STRVAR(adef_flags_doc,"\
flags()\n--\n\n\
Get flags word of the attribute.\n\n\
Returns\n\
-------\n\
Returns the flags as an integer value.\n\
");

PyDoc_STRVAR(adef_elements_doc,"\
elements()\n--\n\n\
Get number of elements of the attribute.\n\n\
Returns\n\
-------\n\
Returns the number of elements.\n\
");

PyDoc_STRVAR(adef_typeref_doc,"\
offset()\n--\n\n\
Get TypeRef of the attribute.\n\n\
Returns\n\
-------\n\
Returns an integer value with the TypeRef.\n\
");

/*
 * Doc for SubObject class
 */
PyDoc_STRVAR(sub_doc,"\
ProviewR subscription. See pwrrt.subscribe()\n\
");

PyDoc_STRVAR(sub_value_doc,"\
value()\n--\n\n\
Get the value of the subscribed attribute.\n\n\
Returns\n\
-------\n\
Returns the subscription value.\n\
");

PyDoc_STRVAR(sub_setValue_doc,"\
setValue(value)\n--\n\n\
Set the value of the subscribed attribute.\n\n\
Arguments\n\
---------\n\
value   Arbitrary type\n\
  The value to set into the subscribed attribute. Only subscriptions\n\
  to local objects can be set.\n\
");

PyDoc_STRVAR(sub_close_doc,"\
close()\n--\n\n\
Close subscription.\n\
");

/*
 * Doc for ApplObject class
 */
PyDoc_STRVAR(appl_doc,"\
ProviewR runtime application. See pwrrt.application()\n\
");

PyDoc_STRVAR(appl_mainloop_doc,"\
mainloop()\n--\n\n\
Application mainloop. Calls the open callback function att start. Then\n\
the scan callback function is called cyclically with the specified scantime.\n\
Finally the close callback function is called as shutdown.\n\n\
Example\n\
-------\n\
def open_cb():\n\
  print 'Startup up'\n\
\n\
def close_cb():\n\
  print 'Closing down'\n\
\n\
def scan_cb():\n\
  print 'Scanning'\n\
\n\
a = pwrrt.Appl('MyAppl','Nodes-DemoNode-Apps-MyAppl', 61, 1,\n\
               open_cb, scan_cb, close_cb, None)\n\
a.mainloop()\n\
");

PyDoc_STRVAR(appl_setStatus_doc,"\
setStatus(status)\n--\n\n\
Set application status. The status argument is a string with one of the\n\
following values\n\
'none'           No status (0)\n\
'applerror'      Application error\n\
'applfatal'      Application fatal error\n\
'applwarning'    Application warinng\n\
'applinfo'       Application info\n\
'applstartup'    Application starting up\n\
'applrun'        Application running\n\
'applterm'       Application terminated\n\n\
Example\n\
-------\n\
def open_cb():\n\
  a.setStatus('applrun')\n\
\n\
def close_cb():\n\
  a.setStatus('applterm')\n\
\n\
def scan_cb():\n\
  a.setStatus('applrun')\n\
\n\
a = pwrrt.Appl('MyAppl','Nodes-DemoNode-Apps-MyAppl', 60, 1,\n\
               open_cb, scan_cb, close_cb, None)\n\
a.mainloop()\n\
");

PyDoc_STRVAR(appl_log_doc,"\
log(severity, text)\n--\n\n\
Log to the Proview logfile (System messages).\n\n\
Arguments\n\
---------\n\
severity  String\n\
  Message severity. Can be 'info', 'warning', 'error' or 'fatal'.\n\n\
text      String\n\
  Text to log.\n\n\
Example\n\
-------\n\
a.log('fatal', 'Application terminating.')\n\
quit()\n\
");


/*
 * Doc for static methods
 */
PyDoc_STRVAR(pwrrt_init_doc,"\
init(name)\n--\n\n\
Attach ProviewR runtime.\n\n\
Arguments\n\
---------\n\
name String\n\
  Name of application.\n\n\
Example\n\
-------\n\
pwrrt.init('my_py')\n");

PyDoc_STRVAR(pwrrt_volume_doc,"\
volume(volumeName)\n--\n\n\
Get volume\n\n\
Arguments\n\
---------\n\
volumeName Optional String\n\
  Name of the volume. If not supplied the first volume is returned.\n\n\
Returns\n\
-------\n\
Returns an Oid object for the volume object.\n\n\
Example\n\
-------\n\
vol = pwrrt.volume('VolPwrDemo')\n\
for o in vol.children():\n\
  print o.fullName()\n");

PyDoc_STRVAR(pwrrt_volumes_doc,"\
volumes()\n\
--\n\n\
Get all volumes.\n\n\
Returns\n\
-------\n\
Returns a tuple with Oid objects for allt the volumes.\n\n\
Example\n\
-------\n\
for v in pwrrt.volumes():\n\
  print v.name()\n");

PyDoc_STRVAR(pwrrt_object_doc,"\
object(objectName)\n--\n\n\
Get an object.\n\n\
Arguments\n\
---------\n\
objectName Optional String\n\
  Name of the object. If not supplied the first root object is returned.\n\n\
Returns\n\
-------\n\
Returns an Oid object for the object.\n\n\
Example\n\
-------\n\
o = pwrrt.object('Demo-Ge-Dynamics')\n\
print o.fullName()\n");

PyDoc_STRVAR(pwrrt_attribute_doc,"\
attribute(attributeName)\n--\n\n\
Get an attribute.\n\n\
Arguments\n\
---------\n\
attributeName String\n\
  Name of the attribute.\n\n\
Returns\n\
-------\n\
Returns an Aref object for the attribute.\n\n\
Example\n\
-------\n\
a = pwrrt.attribute('Demo-Ge-Dynamics-DigLowColor-Dv1.ActualValue')\n\
value = a.value()\n");

PyDoc_STRVAR(pwrrt_subscribe_doc,"\
subscribe(attributeName)\n--\n\n\
Set up an subscription\n\n\
Arguments\n\
---------\n\
attributeName String\n\
  Name of the attribute to subscribe to.\n\n\
Returns\n\
-------\n\
Returns a Sub object for the subscription.\n\n\
Example\n\
-------\n\
sub = pwrrt.subscribe('Demo-Ge-Dynamics-DigLowColor-Dv1.ActualValue')\n\
value = sub.value()\n");

PyDoc_STRVAR(pwrrt_application_doc,"\
application(name, applobject, anix, scantime, open_cb, scan_cb, close_cb,userobject)\n--\n\n\
Create an application\n\n\
-----------\n\
Arguments\n\
---------\n\
name        String\n\
  Application name.\n\
applobject  String\n\
  Application object in the node hierarchy.\n\
anix        Integer\n\
  Application index (errh_eAnix). A value between 61 and 110.\n\
scantime    Float\n\
  Scan time.\n\
open_cb     Function\n\
  Callback function called at startup.\n\
scan_cb     Function\n\
  Callback function called cyclically with the scantime.\n\
close_cb   Function\n\
  Callback function called at shutdown.\n\
userobject Object\n\
  An object that is supplied to the callback functions.\n\n\
Example\n\
-------\n\
def open_cb():\n\
  print 'Startup up'\n\
\n\
def close_cb():\n\
  print 'Closing down'\n\
\n\
def scan_cb():\n\
  print 'Scanning'\n\
\n\
a = pwrrt.application('MyAppl','Nodes-DemoNode-Apps-MyAppl', 61, 1,\n\
                      open_cb, scan_cb, close_cb, None)\n\
a.mainloop()\n\
");

PyDoc_STRVAR(pwrrt_login_doc,"\
login(username, password)\n\
--\n\
\n\
Login as a ProviewR user. This is usually done to gain write access\n\
to attributes in the database. The privileges RtWrite or System is\n\
required for write access.\n\n\
Arguments\n\
---------\n\
username\n\
  Name of a user defined in the ProviewR user database.\n\n\
password\n\
  Password for the user.\n\n\
Example\n\
-------\n\
pwrrt.login('pwrp', 'mypassword')\n");

PyDoc_STRVAR(pwrrt_logout_doc, "\
logout()\n\
--\n\
\n\
Logout from the previously logged in user.\n\
The privileges will return to RtRead.\n\n\
Example\n\
-------\n\
pwrrt.logout()\n");

PyDoc_STRVAR(pwrrt_getPriv_doc, "\
getPriv()\n\
--\n\
\n\
Returns the current privileges\n\n\
Returns\n\
-------\n\
Returns an integer with the current privileges.\n\n\
Example\n\
-------\n\
priv = pwrrt.getPriv()\n");

PyDoc_STRVAR(pwrrt_getUser_doc, "\
getUser()\n\
--\n\
\n\
Returns the current user\n\n\
Returns\n\
-------\n\
Returns a string with the user.\n\n\
Example\n\
-------\n\
user = pwrrt.getUser()\n");

PyDoc_STRVAR(pwrrt_getMsg_doc, "\
getMsg(sts)\n\
Convert a status to a string.\n\
Arguments\n\
---------\n\
sts Status\n\
  A proview status value.\n\n\
Returns\n\
-------\n\
Returns the corresponding string.\n\n\
Example\n\
-------\n\
msg = pwrrt.getMsg(sts)\n");

typedef struct {
  PyObject_HEAD
  pwr_tOid oid;
  pwr_tStatus sts;
} OidObject;

static PyObject *Oid_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static PyObject *Oid_str(PyObject *self);
static PyObject *Oid_richcompare(PyObject *self, PyObject *other, int op);
static int Oid_init(OidObject *self, PyObject *args, PyObject *kwds);

static PyObject *Oid_name(OidObject *self, PyObject *args);
static PyObject *Oid_fullName(OidObject *self, PyObject *args);
static PyObject *Oid_oidStr(PyObject *s, PyObject *args);
static PyObject *Oid_next(OidObject *self, PyObject *args);
static PyObject *Oid_parent(OidObject *self, PyObject *args);
static PyObject *Oid_child(OidObject *self, PyObject *args);
static PyObject *Oid_children(PyObject *s);
static PyObject *Oid_cid(OidObject *self, PyObject *args);
static PyObject *Oid_attribute(OidObject *self, PyObject *args);

static PyMethodDef Oid_methods[] = {
    { "next", (PyCFunction) Oid_next, METH_NOARGS, oid_next_doc },
    { "parent", (PyCFunction) Oid_parent, METH_NOARGS, oid_parent_doc },
    { "child", (PyCFunction) Oid_child, METH_NOARGS, oid_child_doc },
    { "children", (PyCFunction) Oid_children, METH_NOARGS, oid_children_doc },
    { "name", (PyCFunction) Oid_name, METH_VARARGS, oid_name_doc },
    { "fullName", (PyCFunction) Oid_fullName, METH_VARARGS, oid_fullName_doc },
    { "oidStr", (PyCFunction) Oid_oidStr, METH_VARARGS, oid_oidStr_doc },
    { "cid", (PyCFunction) Oid_cid, METH_NOARGS, oid_cid_doc },
    { "attribute", (PyCFunction) Oid_attribute, METH_VARARGS, oid_attribute_doc },
    { NULL }
};

static PyTypeObject OidType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Oid",         	       /* tp_name */
    sizeof(OidObject),         /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    Oid_str,                   /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    Oid_str,                   /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    oid_doc,  		       /* tp_doc */    
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    Oid_richcompare,           /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Oid_methods,               /* tp_methods */
    0,               	       /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Oid_init,        /* tp_init */
    0,                         /* tp_alloc */
    Oid_new,                   /* tp_new */
};

typedef struct {
  PyObject_HEAD
  pwr_tAttrRef aref;
  pwr_tStatus sts;
} ArefObject;

static PyObject *Aref_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static PyObject *Aref_str(PyObject *self);
static PyObject *Aref_richcompare(PyObject *self, PyObject *other, int op);
static int Aref_init(ArefObject *self, PyObject *args, PyObject *kwds);
static PyObject *Aref_name(ArefObject *self, PyObject *args);
static PyObject *Aref_fullName(ArefObject *self, PyObject *args);
static PyObject *Aref_arefStr(ArefObject *self, PyObject *args);
static PyObject *Aref_tid(ArefObject *self, PyObject *args);
static PyObject *Aref_value(ArefObject *self, PyObject *args);
static PyObject *Aref_setValue(ArefObject *self, PyObject *args);
static PyObject *Aref_subscribe(ArefObject *self, PyObject *args);

static PyMethodDef Aref_methods[] = {
    { "name", (PyCFunction) Aref_name, METH_VARARGS, aref_name_doc },
    { "fullName", (PyCFunction) Aref_fullName, METH_VARARGS, aref_fullName_doc },
    { "arefStr", (PyCFunction) Aref_arefStr, METH_VARARGS, aref_arefStr_doc },
    { "tid", (PyCFunction) Aref_tid, METH_NOARGS, aref_tid_doc },    
    { "value", (PyCFunction) Aref_value, METH_NOARGS, aref_value_doc },
    { "setValue", (PyCFunction) Aref_setValue, METH_VARARGS, aref_setValue_doc },
    { "subscribe", (PyCFunction) Aref_subscribe, METH_NOARGS, aref_subscribe_doc },
    { NULL }
};

static PyTypeObject ArefType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Aref",                    /* tp_name */
    sizeof(ArefObject),        /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    Aref_str,                  /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    Aref_str,                  /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "Proview runtime object",  /* tp_doc */    
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    Aref_richcompare,          /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Aref_methods,              /* tp_methods */
    0,               	       /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Aref_init,       /* tp_init */
    0,                         /* tp_alloc */
    Aref_new,                  /* tp_new */
};


typedef struct {
  PyObject_HEAD
  pwr_tCid cid;
} CidObject;

static PyObject *Cid_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static PyObject *Cid_str(PyObject *self);
static PyObject *Cid_richcompare(PyObject *self, PyObject *other, int op);
static int Cid_init(CidObject *self, PyObject *args, PyObject *kwds);
static PyObject *Cid_name(CidObject *self, PyObject *args);
static PyObject *Cid_fullName(CidObject *self, PyObject *args);
static PyObject *Cid_object(CidObject *self, PyObject *args);
static PyObject *Cid_nextObject(CidObject *self, PyObject *args);
static PyObject *Cid_objects(PyObject *s);
static PyObject *Cid_attrObject(CidObject *self, PyObject *args);
static PyObject *Cid_nextAttrObject(CidObject *self, PyObject *args);
static PyObject *Cid_attrObjects(PyObject *s);
static PyObject *Cid_attributes(PyObject *s);

static PyMethodDef Cid_methods[] = {
    { "name", (PyCFunction) Cid_name, METH_NOARGS, cid_name_doc },
    { "fullName", (PyCFunction) Cid_fullName, METH_NOARGS, cid_fullName_doc },
    { "object", (PyCFunction) Cid_object, METH_NOARGS, cid_object_doc },
    { "nextObject", (PyCFunction) Cid_nextObject, METH_VARARGS, cid_nextObject_doc },
    { "objects", (PyCFunction) Cid_objects, METH_VARARGS, cid_objects_doc },
    { "attrObject", (PyCFunction) Cid_attrObject, METH_NOARGS, cid_attrObject_doc },
    { "nextAttrObject", (PyCFunction) Cid_nextAttrObject, METH_VARARGS, cid_nextAttrObject_doc },
    { "attrObjects", (PyCFunction) Cid_attrObjects, METH_VARARGS, cid_attrObjects_doc },
    { "attributes", (PyCFunction) Cid_attributes, METH_VARARGS, cid_attributes_doc },
    { NULL }
};

static PyTypeObject CidType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Cid",         	       /* tp_name */
    sizeof(CidObject),         /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    Cid_str,                   /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    Cid_str,                   /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "Proview runtime class",   /* tp_doc */    
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    Cid_richcompare,           /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Cid_methods,               /* tp_methods */
    0,               	       /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Cid_init,        /* tp_init */
    0,                         /* tp_alloc */
    Cid_new,                   /* tp_new */
};

typedef struct {
  PyObject_HEAD
  pwr_tTid tid;
} TidObject;

static PyObject *Tid_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static PyObject *Tid_str(PyObject *self);
static PyObject *Tid_richcompare(PyObject *self, PyObject *other, int op);
static int Tid_init(TidObject *self, PyObject *args, PyObject *kwds);
static PyObject *Tid_name(TidObject *self, PyObject *args);
static PyObject *Tid_fullName(TidObject *self, PyObject *args);

static PyMethodDef Tid_methods[] = {
    { "name", (PyCFunction) Tid_name, METH_NOARGS, tid_name_doc },
    { "fullName", (PyCFunction) Tid_fullName, METH_NOARGS, tid_fullName_doc },
    { NULL }
};

static PyTypeObject TidType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Tid",         	       /* tp_name */
    sizeof(TidObject),         /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    Tid_str,                   /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    Tid_str,                   /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "Proview runtime class",   /* tp_doc */    
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    Tid_richcompare,           /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Tid_methods,               /* tp_methods */
    0,               	       /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Tid_init,        /* tp_init */
    0,                         /* tp_alloc */
    Tid_new,                   /* tp_new */
};

typedef struct {
  PyObject_HEAD
  pwr_tObjName name;
  pwr_tCid cid;
  pwr_tUInt32 offset;
  pwr_tUInt32 size;
  pwr_tUInt32 flags;
  pwr_tUInt32 elements;
  pwr_tUInt32 aix;
  pwr_tTypeId typeref;
} ADefObject;

static PyObject *ADef_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static PyObject *ADef_str(PyObject *self);
static int ADef_init(ADefObject *self, PyObject *args, PyObject *kwds);
static PyObject *ADef_name(ADefObject *self, PyObject *args);
static PyObject *ADef_cid(ADefObject *self, PyObject *args);
static PyObject *ADef_offset(ADefObject *self, PyObject *args);
static PyObject *ADef_size(ADefObject *self, PyObject *args);
static PyObject *ADef_flags(ADefObject *self, PyObject *args);
static PyObject *ADef_elements(ADefObject *self, PyObject *args);
static PyObject *ADef_typeref(ADefObject *self, PyObject *args);

static PyMethodDef ADef_methods[] = {
    { "name", (PyCFunction) ADef_name, METH_NOARGS, adef_name_doc },
    { "cid", (PyCFunction) ADef_cid, METH_NOARGS, adef_cid_doc },
    { "offset", (PyCFunction) ADef_offset, METH_NOARGS, adef_offset_doc },
    { "size", (PyCFunction) ADef_size, METH_NOARGS, adef_size_doc },
    { "flags", (PyCFunction) ADef_flags, METH_NOARGS, adef_flags_doc },
    { "elements", (PyCFunction) ADef_elements, METH_NOARGS, adef_elements_doc },
    { "typeref", (PyCFunction) ADef_typeref, METH_NOARGS, adef_typeref_doc },
    { NULL }
};

static PyTypeObject ADefType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "ADef",         	       /* tp_name */
    sizeof(ADefObject),         /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    ADef_str,                   /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    ADef_str,                   /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "Proview attribute definition", /* tp_doc */    
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0, 		               /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    ADef_methods,              /* tp_methods */
    0,               	       /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)ADef_init,       /* tp_init */
    0,                         /* tp_alloc */
    ADef_new,                  /* tp_new */
};

typedef struct {
  PyObject_HEAD
  pwr_eType type;
  unsigned int size;
  void *p;
  pwr_tRefId refid;
  pwr_tStatus sts;
} SubObject;

static PyObject *Sub_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static PyObject *Sub_str(PyObject *s);
static int Sub_init(SubObject *self, PyObject *args, PyObject *kwds);
static PyObject *Sub_value(SubObject *self, PyObject *args);
static PyObject *Sub_setValue(SubObject *self, PyObject *args);
static PyObject *Sub_close(SubObject *self, PyObject *args);

static PyMethodDef Sub_methods[] = {
    { "value", (PyCFunction) Sub_value, METH_NOARGS, sub_value_doc },
    { "setValue", (PyCFunction) Sub_setValue, METH_VARARGS, sub_setValue_doc },
    { "close", (PyCFunction) Sub_close, METH_NOARGS, sub_close_doc },
    { NULL }
};

static PyTypeObject SubType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Sub",                     /* tp_name */
    sizeof(SubObject),         /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    Sub_str,                   /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    Sub_str,                   /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    sub_doc,   		       /* tp_doc */    
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0, 		               /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Sub_methods,               /* tp_methods */
    0,               	       /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Sub_init,        /* tp_init */
    0,                         /* tp_alloc */
    Sub_new,                   /* tp_new */
};

typedef struct {
  PyObject_HEAD
  errh_eAnix anix;
  double scantime;
  double maxdelay;
  qcom_sQid qid;
  char name[80];
  pwr_tOid apploid;
  PyObject *open;
  PyObject *scan;
  PyObject *close;
  PyObject *ctx;
} ApplObject;

static PyObject *Appl_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static PyObject *Appl_str(PyObject *s);
static int Appl_init(ApplObject *self, PyObject *args, PyObject *kwds);
static PyObject *Appl_mainloop(ApplObject *self, PyObject *args);
static PyObject *Appl_setStatus(ApplObject *self, PyObject *args);
static PyObject *Appl_log(ApplObject *self, PyObject *args);

static PyMethodDef Appl_methods[] = {
    { "mainloop", (PyCFunction) Appl_mainloop, METH_NOARGS, appl_mainloop_doc },
    { "setStatus", (PyCFunction) Appl_setStatus, METH_VARARGS, appl_setStatus_doc },
    { "log", (PyCFunction) Appl_log, METH_VARARGS, appl_log_doc },
    { NULL }
};

static PyTypeObject ApplType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Appl",                     /* tp_name */
    sizeof(ApplObject),         /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    Appl_str,                   /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    Appl_str,                  /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    appl_doc,		       /* tp_doc */    
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0, 		               /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Appl_methods,              /* tp_methods */
    0,               	       /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Appl_init,       /* tp_init */
    0,                         /* tp_alloc */
    Appl_new,                  /* tp_new */
};


static void *set_error(pwr_tStatus sts) 
{
  char msg[120];
  msg_GetMsg(sts, msg, sizeof(msg));
  PyErr_SetString(PyExc_RuntimeError, msg);
  return NULL;
}

static int is_authorized(unsigned int access)
{
  return (pwrrt_priv & access) != 0;
}

/* 
 * Oid object functions
 */
static PyObject *
Oid_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  OidObject *self;

  self = (OidObject *)type->tp_alloc(type, 0);
  if (self != NULL) {
    self->oid = pwr_cNOid;
    self->sts = GDH__SUCCESS;
  }

  return (PyObject *)self;
}

static PyObject *
Oid_str(PyObject *self)
{
  pwr_tOid oid;
  pwr_tOName name;
  pwr_tStatus sts;

  oid = ((OidObject *)self)->oid;

  if (oid.oix != 0) {
    sts = gdh_ObjidToName( oid, name, sizeof(name), cdh_mName_path | cdh_mName_object);
    if ( EVEN(sts))
      return set_error(sts);
  }
  else {
    sts = gdh_ObjidToName( oid, name, sizeof(name), cdh_mName_volume);
    if ( EVEN(sts))
      return set_error(sts);
    else
      name[strlen(name)-1] = 0;
  }

  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return PyUnicode_FromFormat("%s", utf8name);
}

static PyObject *
Oid_richcompare(PyObject *self, PyObject *other, int op)
{
  PyObject *result = NULL;

  if ( Py_TYPE(other) != &OidType) {
    if (Py_TYPE(other) == &PyBool_Type) {
      long oval = PyLong_AsLong(other);
      if ((ODD(((OidObject *)self)->sts) && oval) ||
	  (EVEN(((OidObject *)self)->sts) && !oval))
	result = Py_True;
      else
	result = Py_False;
    }
    else
      result = Py_NotImplemented;
  }
  else {
    switch ( op) {
    case Py_LT:
    case Py_LE:
    case Py_GT:
    case Py_GE:
      result = Py_NotImplemented;
      break;
    case Py_EQ:
      if ( ((OidObject *)self)->oid.vid == ((OidObject *)other)->oid.vid && 
	   ((OidObject *)self)->oid.oix == ((OidObject *)other)->oid.oix)
	result = Py_True;
      else
	result = Py_False;
      break;
    case Py_NE:
      if ( !(((OidObject *)self)->oid.vid == ((OidObject *)other)->oid.vid && 
	     ((OidObject *)self)->oid.oix == ((OidObject *)other)->oid.oix))
	result = Py_True;
      else
	result = Py_False;
      break;
    }
  }
  Py_XINCREF(result);
  return result;
}

static int
Oid_init(OidObject *self, PyObject *args, PyObject *kwds)
{
  char *name, *utf8name = 0;
  pwr_tOid oid;
  pwr_tStatus sts;

  if (! PyArg_ParseTuple(args, "|s", &utf8name))
    return -1;

  if ( utf8name) {
    name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);
    sts = gdh_NameToObjid(name, &oid);
    if ( EVEN(sts)) {
      self->sts = sts;
      set_error(sts);
      return -1;
    }
  }
  else {
    sts = gdh_GetRootList(&oid);
    if ( EVEN(sts)) {
      self->sts = sts;
      return -1;
    }
  }
  self->sts = sts;
  self->oid = oid;

  return 0;
}

static PyObject *
Oid_name(OidObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tOName name;
  pwr_tStatus sts;

  if (self->oid.oix != 0) {
    sts = gdh_ObjidToName( self->oid, name, sizeof(name), cdh_mName_object);
    if ( EVEN(sts))
      return set_error(sts);
  }
  else {
    sts = gdh_ObjidToName( self->oid, name, sizeof(name), cdh_mName_volume);
    if ( EVEN(sts))
      return set_error(sts);
    else
      name[strlen(name)-1] = 0;
  }

  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
Oid_fullName(OidObject *self, PyObject *args)
{
  pwr_tOName name;
  pwr_tStatus sts;

  if (EVEN(self->sts))
    return set_error(self->sts);

  sts = gdh_ObjidToName( self->oid, name, sizeof(name), cdh_mName_volumeStrict);
  if ( EVEN(sts))
    return set_error(sts);

  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
Oid_oidStr(PyObject *s, PyObject *args)
{
  if (EVEN(((OidObject *)s)->sts))
    return set_error(((OidObject *)s)->sts);

  char str[30];
  const char *name = 0;
  pwr_tOid oid = ((OidObject *)s)->oid;

  if ( !PyArg_ParseTuple(args, "|s", &name))
    return NULL;

  if (name == 0)
    sprintf(str, "_O%hhu.%hhu.%hhu.%hhu:%u", 
	    0xFF & (oid.vid >> 24),
	    0xFF & (oid.vid >> 16),
	    0xFF & (oid.vid >> 8),
	    0xFF & oid.vid,
	    oid.oix);
  else if ( strcmp(name, "hex") == 0)
    sprintf(str, "_O%hhu.%hhu.%hhu.%hhu:%x", 
	    0xFF & (oid.vid >> 24),
	    0xFF & (oid.vid >> 16),
	    0xFF & (oid.vid >> 8),
	    0xFF & oid.vid,
	    oid.oix);
  else if ( strcmp(name, "file") == 0)
    sprintf(str, "%03hhu_%03hhu_%03hhu_%hhu_%08x", 
	    0xFF & (oid.vid >> 24),
	    0xFF & (oid.vid >> 16),
	    0xFF & (oid.vid >> 8),
	    0xFF & oid.vid,
	    oid.oix);
  else
    return set_error(GDH__BADARG);

  return Py_BuildValue("s", str);  
}

static PyObject *
Oid_next(OidObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tOid soid;
  OidObject *sibling;
  pwr_tStatus sts;

  if ( self->oid.oix == 0) {
    sts = gdh_GetNextVolume( self->oid.vid, &soid.vid);
    if (EVEN(sts))
      Py_RETURN_NONE;
    soid.oix = 0;
  }
  else {
    sts = gdh_GetNextSibling( self->oid, &soid);
    if ( EVEN(sts))
      Py_RETURN_NONE;
  }

  sibling = (OidObject *)Oid_new(&OidType, 0, 0);
  if (sibling != NULL) {
    sibling->oid = soid;
  }

  return (PyObject *)sibling;
}

static PyObject *
Oid_parent(OidObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tOid poid;
  OidObject *parent;
  pwr_tStatus sts;

  sts = gdh_GetParent( self->oid, &poid);
  if ( EVEN(sts))
    Py_RETURN_NONE;

  parent = (OidObject *)Oid_new(&OidType, 0, 0);
  if (parent != NULL) {
    parent->oid = poid;
  }

  return (PyObject *)parent;
}

static PyObject *
Oid_child(OidObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tOid coid;
  OidObject *child;
  pwr_tStatus sts;

  sts = gdh_GetChild( self->oid, &coid);
  if ( EVEN(sts))
    Py_RETURN_NONE;

  child = (OidObject *)Oid_new(&OidType, 0, 0);
  if (child != NULL) {
    child->oid = coid;
  }

  return (PyObject *)child;
}

static PyObject *
Oid_children(PyObject *s)
{
  if (EVEN(((OidObject *)s)->sts))
    return set_error(((OidObject *)s)->sts);

  pwr_tOid coid;
  OidObject *child;
  pwr_tStatus sts;
  OidObject *self = (OidObject *)s;
  PyObject *result;
  int cnt = 0;

  for ( sts = gdh_GetChild( self->oid, &coid);
	ODD(sts);
	sts = gdh_GetNextSibling( coid, &coid))
    cnt++;
  
  result = PyTuple_New(cnt);
  cnt = 0;
  for ( sts = gdh_GetChild( self->oid, &coid);
	ODD(sts);
	sts = gdh_GetNextSibling( coid, &coid)) {
    child = (OidObject *)Oid_new(&OidType, 0, 0);
    if (child != NULL) {
      child->oid = coid;
      PyTuple_SetItem(result, cnt, (PyObject *)child);
      cnt++;
    }    
  }
  return result;
}

static PyObject *
Oid_cid(OidObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tCid cid;
  CidObject *cid_object;
  pwr_tStatus sts;

  sts = gdh_GetObjectClass( self->oid, &cid);
  if (EVEN(sts))
    return set_error(sts);

  cid_object = (CidObject *)Cid_new(&CidType, 0, 0);
  if (cid_object != NULL)
    cid_object->cid = cid;

  return (PyObject *)cid_object;
}

static PyObject *
Oid_attribute(OidObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tAttrRef aref, oaref;
  ArefObject *aref_object;
  pwr_tStatus sts;
  char *name, *utf8name;

  if (! PyArg_ParseTuple(args, "s", &utf8name))
    return NULL;

  name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);

  aref = cdh_ObjidToAref(self->oid);
  sts = gdh_ArefANameToAref(&aref, name, &oaref);
  if ( EVEN(sts))
    return set_error(sts);

  aref_object = (ArefObject *)Aref_new(&ArefType, 0, 0);
  if (aref_object != NULL)
    aref_object->aref = oaref;

  return (PyObject *)aref_object;
}

/* 
 * Aref object functions
 */
static PyObject *
Aref_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  ArefObject *self;

  self = (ArefObject *)type->tp_alloc(type, 0);
  if (self != NULL) {
    self->aref = pwr_cNAttrRef;
    self->sts = GDH__SUCCESS;
  }
  return (PyObject *)self;
}

static PyObject *
Aref_str(PyObject *self)
{
  pwr_tAttrRef *aref;
  pwr_tOName name;
  pwr_tStatus sts;

  aref = &((ArefObject *)self)->aref;

  sts = gdh_AttrrefToName(aref, name, sizeof(name), 
			  cdh_mName_path | cdh_mName_object | cdh_mName_attribute);
  if ( EVEN(sts))
    strcpy( name, "Unknown");

  /*
  return PyUnicode_FromFormat("_A%d.%d.%d.%d:%u:%u:%u %s", 
			      0xFF & (aref->Objid.vid >> 24), 
			      0xFF & (aref->Objid.vid >> 16), 
			      0xFF & (aref->Objid.vid >> 8), 
			      0xFF & aref->Objid.vid,
			      aref->Objid.oix, aref->Offset, aref->Size, name);
  */
  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return PyUnicode_FromFormat("%s", utf8name);
}

static PyObject *
Aref_richcompare(PyObject *self, PyObject *other, int op)
{
  PyObject *result = NULL;

  if ( Py_TYPE(other) != &ArefType) {
    result = Py_NotImplemented;
  }
  else {
    switch ( op) {
    case Py_LT:
    case Py_LE:
    case Py_GT:
    case Py_GE:
      result = Py_NotImplemented;
      break;
    case Py_EQ:
      if ( ((ArefObject *)self)->aref.Objid.vid == ((ArefObject *)other)->aref.Objid.vid && 
	   ((ArefObject *)self)->aref.Objid.oix == ((ArefObject *)other)->aref.Objid.oix &&
	   ((ArefObject *)self)->aref.Offset == ((ArefObject *)other)->aref.Offset &&
	   ((ArefObject *)self)->aref.Size == ((ArefObject *)other)->aref.Size)
	result = Py_True;
      else
	result = Py_False;
      break;
    case Py_NE:
      if ( !(((ArefObject *)self)->aref.Objid.vid == ((ArefObject *)other)->aref.Objid.vid && 
	     ((ArefObject *)self)->aref.Objid.oix == ((ArefObject *)other)->aref.Objid.oix &&
	     ((ArefObject *)self)->aref.Offset == ((ArefObject *)other)->aref.Offset &&
	     ((ArefObject *)self)->aref.Size == ((ArefObject *)other)->aref.Size))
	result = Py_True;
      else
	result = Py_False;
      break;
    }
  }
  Py_XINCREF(result);
  return result;
}

static int
Aref_init(ArefObject *self, PyObject *args, PyObject *kwds)
{
  char *name, *utf8name = 0;
  pwr_tStatus sts;

  if (! PyArg_ParseTuple(args, "|s", &utf8name))
    return -1;

  if ( utf8name) {
    name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);
    sts = gdh_NameToAttrref( pwr_cNOid, name, &self->aref);
    if ( EVEN(sts)) {
      set_error(sts);
      self->sts = sts;
      return -1;
    }
  }

  return 0;
}

static PyObject *
Aref_name(ArefObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tOName name;
  pwr_tStatus sts;

  sts = gdh_AttrrefToName( &self->aref, name, sizeof(name), cdh_mName_object | cdh_mName_attribute);
  if ( EVEN(sts))
    return set_error(sts);

  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
Aref_fullName(ArefObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tOName name;
  pwr_tStatus sts;

  sts = gdh_AttrrefToName( &self->aref, name, sizeof(name), cdh_mName_volumeStrict);
  if ( EVEN(sts))
    return set_error(sts);

  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
Aref_arefStr(ArefObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tAName name;

  cdh_ArefToString( name, sizeof(name), &self->aref, 1);

  return Py_BuildValue("s", name);
}

static PyObject *
Aref_tid(ArefObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tTid tid;
  TidObject *tid_object;
  pwr_tStatus sts;

  sts = gdh_GetAttrRefTid( &self->aref, &tid);
  if (EVEN(sts))
    return set_error(sts);

  tid_object = (TidObject *)Tid_new(&TidType, 0, 0);
  if (tid_object != NULL)
    tid_object->tid = tid;

  return (PyObject *)tid_object;
}

static PyObject *
Aref_value(ArefObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tStatus sts;
  char *buf;
  pwr_eType atype;
  unsigned int asize;

  sts = gdh_GetAttributeCharAttrref(&self->aref, &atype, &asize, 0, 0);
  if (EVEN(sts)) return NULL;

  buf = malloc(self->aref.Size);
  sts = gdh_GetObjectInfoAttrref( &self->aref, buf, self->aref.Size);
  if (EVEN(sts))
    return set_error(sts);

  switch ( atype) {
  case pwr_eType_Boolean: {
    pwr_tBoolean value = *(pwr_tBoolean *)buf;
    free(buf);
    return Py_BuildValue("i", value);
  }
  case pwr_eType_Int8: {
    pwr_tInt8 value = *(pwr_tInt8 *)buf;
    free(buf);
    return Py_BuildValue("b", value);
  }
  case pwr_eType_UInt8: {
    pwr_tUInt8 value = *(pwr_tUInt8 *)buf;
    free(buf);
    return Py_BuildValue("B", value);
  }
  case pwr_eType_Int16: {
    pwr_tInt16 value = *(pwr_tInt16 *)buf;
    free(buf);
    return Py_BuildValue("h", value);
  }
  case pwr_eType_UInt16: {
    pwr_tUInt16 value = *(pwr_tUInt16 *)buf;
    free(buf);
    return Py_BuildValue("H", value);
  }
  case pwr_eType_Int32:
  case pwr_eType_Enum: {
    pwr_tInt32 value = *(pwr_tInt32 *)buf;
    free(buf);
    return Py_BuildValue("i", value);
  }
  case pwr_eType_UInt32:
  case pwr_eType_Mask:
  case pwr_eType_Status:
  case pwr_eType_NetStatus: {
    pwr_tUInt32 value = *(pwr_tUInt32 *)buf;
    return Py_BuildValue("I", value);
  }
  case pwr_eType_Int64: {
    char format[2];
    pwr_tInt64 value = *(pwr_tInt64 *)buf;
    free(buf);
#if defined HW_X86_64
    strcpy( format, "l");
#else
    strcpy( format, "L");
#endif
    return Py_BuildValue(format, value);
  }
  case pwr_eType_UInt64: {
    char format[2];
    pwr_tUInt64 value = *(pwr_tUInt64 *)buf;
    free(buf);
#if defined HW_X86_64
    strcpy( format, "k");
#else
    strcpy( format, "K");
#endif
    return Py_BuildValue(format, value);
  }
  case pwr_eType_Float32: {
    pwr_tFloat32 value = *(pwr_tFloat32 *)buf;
    free(buf);
    return Py_BuildValue("f", value);
  }
  case pwr_eType_Float64: {
    pwr_tFloat64 value = *(pwr_tFloat64 *)buf;
    free(buf);
    return Py_BuildValue("d", value);
  }
  case pwr_eType_String:
  case pwr_eType_Text:
  case pwr_eType_ProString: {
    char *utf8buf = cnv_iso8859_to_utf8(buf, strlen(buf)+1);
    PyObject *ret = Py_BuildValue("s", utf8buf);
    free(buf);
    return ret;
  }
  case pwr_eType_Time: {
    char timstr[30];

    time_AtoAscii((pwr_tTime *)buf, time_eFormat_DateAndTime, timstr, sizeof(timstr));
    free(buf);
    return Py_BuildValue("s", timstr);
  }
  case pwr_eType_DeltaTime: {
    char timstr[30];

    time_DtoAscii((pwr_tDeltaTime *)buf, 1, timstr, sizeof(timstr));
    free(buf);
    return Py_BuildValue("s", timstr);
  }
  case pwr_eType_Objid: {    
    pwr_tOid value = *(pwr_tOid *)buf;
    free(buf);

    OidObject *oido = (OidObject *)Oid_new(&OidType, 0, 0);
    if ( oido != NULL)
      oido->oid = value;

    return (PyObject *)oido;
  }
  case pwr_eType_AttrRef: {    
    pwr_tAttrRef value = *(pwr_tAttrRef *)buf;
    free(buf);

    ArefObject *arefo = (ArefObject *)Aref_new(&ArefType, 0, 0);
    if ( arefo != NULL)
      arefo->aref = value;

    return (PyObject *)arefo;
  }
  default:
    return set_error(GDH__NYI);
  }
}

static PyObject *
Aref_setValue(ArefObject *self, PyObject *args)
{
  if (EVEN(self->sts))
    return set_error(self->sts);

  pwr_tStatus sts;
  char *buf;
  pwr_eType atype;
  unsigned int asize;
  int publicwrite = 0;
  unsigned int aflags;

  sts = gdh_GetAttributeCharAttrref(&self->aref, &atype, &asize, 0, 0);
  if (EVEN(sts)) return NULL;

  buf = malloc(self->aref.Size);

  switch ( atype) {
  case pwr_eType_Boolean: {
    unsigned int value;
    if ( !PyArg_ParseTuple(args, "I|I", &value, &publicwrite))
      goto error_return;
    if ( value == 1)
      *(pwr_tBoolean *)buf = 1;
    else if ( value == 0)
      *(pwr_tBoolean *)buf = 0;
    else
      goto error_return;

    break;
  }
  case pwr_eType_Int8: {
    if ( !PyArg_ParseTuple(args, "b|I", buf, &publicwrite))
      goto error_return;
    break;
  }
  case pwr_eType_UInt8: {
    if ( !PyArg_ParseTuple(args, "B|I", buf, &publicwrite))
      goto error_return;
    break;
  }
  case pwr_eType_Int16: {
    if ( !PyArg_ParseTuple(args, "h|I", buf, &publicwrite))
      goto error_return;
    break;
  }
  case pwr_eType_UInt16: {
    if ( !PyArg_ParseTuple(args, "H|I", buf, &publicwrite))
      goto error_return;
    break;
  }
  case pwr_eType_Int32:
  case pwr_eType_Enum: {
    if ( !PyArg_ParseTuple(args, "i|I", buf, &publicwrite))
      goto error_return;
    break;
  }
  case pwr_eType_UInt32:
  case pwr_eType_Mask:
  case pwr_eType_Status:
  case pwr_eType_NetStatus: {
    if ( !PyArg_ParseTuple(args, "I|I", buf, &publicwrite))
      goto error_return;
    break;
  }
  case pwr_eType_Int64: {
#if defined HW_X86_64
    char format[] = "l|I";
#else
    char format[] = "L|I";
#endif
    if ( !PyArg_ParseTuple(args, format, buf, &publicwrite))
      goto error_return;
    break;
  }
  case pwr_eType_UInt64: {
#if defined HW_X86_64
    char format[] = "k|I";
#else
    char format[] = "K|I";
#endif
    if ( !PyArg_ParseTuple(args, format, buf, &publicwrite))
      goto error_return;
    break;
  }
  case pwr_eType_Float32: {
    if ( !PyArg_ParseTuple(args, "f|I", buf, &publicwrite))
      goto error_return;
    break;
  }
  case pwr_eType_Float64: {
    if ( !PyArg_ParseTuple(args, "d|I", buf, &publicwrite))
      goto error_return;
    break;
  }
  case pwr_eType_String: {
    char *utf8value = 0, *value;
    if ( !PyArg_ParseTuple(args, "s|I", &utf8value, &publicwrite))
      goto error_return;

    value = cnv_utf8_to_iso8859(utf8value, strlen(utf8value)+1);
    strncpy( buf, value, self->aref.Size);
    buf[self->aref.Size-1] = 0;
    break;
  }
  case pwr_eType_Time: {
    char *value = 0;
    if ( !PyArg_ParseTuple(args, "s", &value))
      goto error_return;

    time_AsciiToA(value, (pwr_tTime *)buf);
    break;
  }
  case pwr_eType_DeltaTime: {
    char *value = 0;
    if ( !PyArg_ParseTuple(args, "s", &value))
      goto error_return;

    time_AsciiToD(value, (pwr_tDeltaTime *)buf);
    break;
  }
  case pwr_eType_Objid: {
    OidObject *o;
    pwr_tOid oid;

    if ( !PyArg_ParseTuple(args, "O", &o))
      goto error_return;

    oid = o->oid;
    memcpy(buf, &oid, sizeof(oid));
    break;
  }
  case pwr_eType_AttrRef: {
    ArefObject *a;
    pwr_tAttrRef aref;

    if ( !PyArg_ParseTuple(args, "O", &a))
      goto error_return;

    aref = a->aref;
    memcpy(buf, &aref, sizeof(aref));
    break;
  }
  default:
    set_error(GDH__NYI);    
    goto error_return;
  }

  if ( publicwrite) {
    sts = gdh_GetAttributeFlags( &self->aref, &aflags); 
    if ( EVEN(sts)) goto error_return;

    if ( !(aflags & pwr_mAdef_publicwrite)) {
      set_error(USER__NOTAUTHORIZED);    
      goto error_return;
    }
  }
  else {
    if ( !is_authorized(pwr_mAccess_RtWrite | pwr_mAccess_System)) {
      set_error(USER__NOTAUTHORIZED);
      goto error_return;
    }
  }

  sts = gdh_SetObjectInfoAttrref( &self->aref, buf, self->aref.Size);
  if (EVEN(sts))
    return set_error(sts);

  free(buf);
  Py_RETURN_NONE;

error_return:
  free(buf);
  return NULL;    
}

static PyObject *
Aref_subscribe(ArefObject *self, PyObject *args)
{
  pwr_tOName name;
  pwr_tStatus sts;
  SubObject *sub;

  sts = gdh_AttrrefToName( &self->aref, name, sizeof(name), cdh_mName_volumeStrict);
  if ( EVEN(sts))
    return set_error(sts);

  sub = (SubObject *)Sub_new(&SubType, 0, 0);
  if ( sub == NULL)
    return NULL;

  sts = gdh_GetAttributeCharAttrref(&self->aref, &sub->type, &sub->size, 0, 0);
  if (EVEN(sts)) 
    return set_error(sts);
  
  sts = gdh_RefObjectInfo(name, &sub->p, &sub->refid, sub->size);
  if ( EVEN(sts))
    return set_error(sts);

  return (PyObject *)sub;
}


/* 
 * Cid object functions
 */
static PyObject *
Cid_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  CidObject *self;

  self = (CidObject *)type->tp_alloc(type, 0);
  if (self != NULL) {
    self->cid = 0;
  }

  return (PyObject *)self;
}

static PyObject *
Cid_str(PyObject *self)
{
  pwr_tCid cid;
  pwr_tOName name;
  pwr_tStatus sts;

  cid = ((CidObject *)self)->cid;

  sts = gdh_ObjidToName( cdh_ClassIdToObjid(cid), name, sizeof(name), cdh_mName_volumeStrict);
  if ( EVEN(sts))
    strcpy( name, "Unknown");

  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return PyUnicode_FromFormat("%u %s", cid, utf8name);
}

static PyObject *
Cid_richcompare(PyObject *self, PyObject *other, int op)
{
  PyObject *result = NULL;

  if ( Py_TYPE(other) != &CidType) {
    result = Py_NotImplemented;
  }
  else {
    switch ( op) {
    case Py_LT:
    case Py_LE:
    case Py_GT:
    case Py_GE:
      result = Py_NotImplemented;
      break;
    case Py_EQ:
      if ( ((CidObject *)self)->cid == ((CidObject *)other)->cid)
	result = Py_True;
      else
	result = Py_False;
      break;
    case Py_NE:
      if ( ((CidObject *)self)->cid != ((CidObject *)other)->cid)
	result = Py_True;
      else
	result = Py_False;
      break;
    }
  }
  Py_XINCREF(result);
  return result;
}

static int
Cid_init(CidObject *self, PyObject *args, PyObject *kwds)
{
  char *name, *utf8name = 0;
  pwr_tCid cid;
  pwr_tStatus sts;

  if (! PyArg_ParseTuple(args, "|s", &utf8name))
    return -1;

  if ( utf8name) {
    name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);
    sts = gdh_ClassNameToId(name, &cid);
    if ( EVEN(sts)) {
      set_error(sts);
      return -1;
    }
  }
  else {
    cid = 0;
  }
  self->cid = cid;

  return 0;
}

static PyObject *
Cid_name(CidObject *self, PyObject *args)
{
  pwr_tOName name;
  pwr_tStatus sts;

  sts = gdh_ObjidToName( cdh_ClassIdToObjid(self->cid), name, sizeof(name), cdh_mName_object);
  if ( EVEN(sts))
    strcpy( name, "");

  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
Cid_fullName(CidObject *self, PyObject *args)
{
  pwr_tOName name;
  pwr_tStatus sts;

  sts = gdh_ObjidToName( cdh_ClassIdToObjid(self->cid), name, sizeof(name), cdh_mName_volumeStrict);
  if ( EVEN(sts))
    strcpy( name, "");

  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
Cid_object(CidObject *self, PyObject *args)
{
  OidObject *instance;
  pwr_tStatus sts;
  pwr_tOid oid;

  sts = gdh_GetClassList( self->cid, &oid);
  if ( EVEN(sts))
    Py_RETURN_NONE;

  instance = (OidObject *)Oid_new(&OidType, 0, 0);
  if (instance != NULL) {
    instance->oid = oid;
  }

  return (PyObject *)instance;
}

static PyObject *
Cid_nextObject(CidObject *self, PyObject *args)
{
  pwr_tStatus sts;
  OidObject *o, *next;
  pwr_tOid next_oid;

  if ( !PyArg_ParseTuple(args, "O", &o))
    return NULL;

  sts = gdh_GetNextObject( o->oid, &next_oid);
  if ( EVEN(sts))
    Py_RETURN_NONE;

  next = (OidObject *)Oid_new(&OidType, 0, 0);
  if (next != NULL) {
    next->oid = next_oid;
  }

  return (PyObject *)next;
}

static PyObject *
Cid_objects(PyObject *s)
{
  pwr_tOid oid;
  OidObject *object;
  pwr_tStatus sts;
  CidObject *self = (CidObject *)s;
  PyObject *result;
  int cnt = 0;

  for ( sts = gdh_GetClassList( self->cid, &oid);
	ODD(sts);
	sts = gdh_GetNextObject( oid, &oid))
    cnt++;
  
  result = PyTuple_New(cnt);
  cnt = 0;
  for ( sts = gdh_GetClassList( self->cid, &oid);
	ODD(sts);
	sts = gdh_GetNextObject( oid, &oid)) {
    object = (OidObject *)Oid_new(&OidType, 0, 0);
    if (object != NULL) {
      object->oid = oid;
      PyTuple_SetItem(result, cnt, (PyObject *)object);
      cnt++;
    }    
  }
  return result;
}

static PyObject *
Cid_attrObject(CidObject *self, PyObject *args)
{
  ArefObject *instance;
  pwr_tStatus sts;
  pwr_tAttrRef aref;

  sts = gdh_GetClassListAttrRef( self->cid, &aref);
  if ( EVEN(sts))
    Py_RETURN_NONE;

  instance = (ArefObject *)Aref_new(&ArefType, 0, 0);
  if (instance != NULL) {
    instance->aref = aref;
  }

  return (PyObject *)instance;
}

static PyObject *
Cid_nextAttrObject(CidObject *self, PyObject *args)
{
  pwr_tStatus sts;
  ArefObject *a, *next;
  pwr_tAttrRef next_aref;

  if ( !PyArg_ParseTuple(args, "O", &a))
    return NULL;

  sts = gdh_GetNextAttrRef( self->cid, &a->aref, &next_aref);
  if ( EVEN(sts))
    Py_RETURN_NONE;

  next = (ArefObject *)Aref_new(&ArefType, 0, 0);
  if (next != NULL) {
    next->aref = next_aref;
  }

  return (PyObject *)next;
}

static PyObject *
Cid_attrObjects(PyObject *s)
{
  pwr_tAttrRef aref;
  ArefObject *object;
  pwr_tStatus sts;
  CidObject *self = (CidObject *)s;
  PyObject *result;
  int cnt = 0;

  for ( sts = gdh_GetClassListAttrRef( self->cid, &aref);
	ODD(sts);
	sts = gdh_GetNextAttrRef( self->cid, &aref, &aref))
    cnt++;
  
  result = PyTuple_New(cnt);
  cnt = 0;
  for ( sts = gdh_GetClassListAttrRef( self->cid, &aref);
	ODD(sts);
	sts = gdh_GetNextAttrRef( self->cid, &aref, &aref)) {
    object = (ArefObject *)Aref_new(&ArefType, 0, 0);
    if (object != NULL) {
      object->aref = aref;
      PyTuple_SetItem(result, cnt, (PyObject *)object);
      cnt++;
    }    
  }
  return result;
}

static PyObject *
Cid_attributes(PyObject *s)
{
  pwr_tStatus sts;
  CidObject *self = (CidObject *)s;
  gdh_sAttrDef *bd;
  int rows;
  PyObject *result;
  int i;
  int cnt;
  ADefObject *adef;

  sts = gdh_GetObjectBodyDef( self->cid, &bd, &rows, pwr_cNOid);
  if (EVEN(sts))
    return set_error(sts);
  
  result = PyTuple_New(rows);

  cnt = 0;
  for ( i = 0; i < rows; i++) {
    adef = (ADefObject *)ADef_new(&ADefType, 0, 0);
    if (adef != NULL) {
      strcpy(adef->name, bd[i].attrName);
      adef->cid = bd[i].attrClass;
      adef->offset = bd[i].attr->Param.Info.Offset;
      adef->size = bd[i].attr->Param.Info.Size;
      adef->flags = bd[i].attr->Param.Info.Flags;
      adef->elements = bd[i].attr->Param.Info.Elements;
      adef->aix = bd[i].attr->Param.Info.ParamIndex;
      adef->typeref = bd[i].attr->Param.TypeRef;
      PyTuple_SetItem(result, cnt, (PyObject *)adef);
      cnt++;
    }    
  }
  free((char *)bd);
  return result;
}

/* 
 * Tid object functions
 */
static PyObject *
Tid_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  TidObject *self;

  self = (TidObject *)type->tp_alloc(type, 0);
  if (self != NULL) {
    self->tid = 0;
  }

  return (PyObject *)self;
}

static PyObject *
Tid_str(PyObject *self)
{
  pwr_tTid tid;
  pwr_tOName name;
  pwr_tStatus sts;

  tid = ((TidObject *)self)->tid;

  if ( cdh_tidIsCid(tid))
    sts = gdh_ObjidToName( cdh_ClassIdToObjid(tid), name, sizeof(name), cdh_mName_volumeStrict);
  else
    sts = gdh_ObjidToName( cdh_TypeIdToObjid(tid), name, sizeof(name), cdh_mName_volumeStrict);
  if ( EVEN(sts))
    strcpy( name, "Unknown");

  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return PyUnicode_FromFormat("%u %s", tid, utf8name);
}

static PyObject *
Tid_richcompare(PyObject *self, PyObject *other, int op)
{
  PyObject *result = NULL;

  if ( Py_TYPE(other) != &TidType) {
    result = Py_NotImplemented;
  }
  else {
    switch ( op) {
    case Py_LT:
    case Py_LE:
    case Py_GT:
    case Py_GE:
      result = Py_NotImplemented;
      break;
    case Py_EQ:
      if ( ((TidObject *)self)->tid == ((TidObject *)other)->tid)
	result = Py_True;
      else
	result = Py_False;
      break;
    case Py_NE:
      if ( ((TidObject *)self)->tid != ((TidObject *)other)->tid)
	result = Py_True;
      else
	result = Py_False;
      break;
    }
  }
  Py_XINCREF(result);
  return result;
}

static int
Tid_init(TidObject *self, PyObject *args, PyObject *kwds)
{
  return 0;
}

static PyObject *
Tid_name(TidObject *self, PyObject *args)
{
  pwr_tOName name;
  pwr_tStatus sts;

  if ( cdh_tidIsCid(self->tid))
    sts = gdh_ObjidToName( cdh_ClassIdToObjid(self->tid), name, sizeof(name), cdh_mName_object);
  else
    sts = gdh_ObjidToName( cdh_TypeIdToObjid(self->tid), name, sizeof(name), cdh_mName_object);
  if ( EVEN(sts))
    strcpy( name, "");

  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
Tid_fullName(TidObject *self, PyObject *args)
{
  pwr_tOName name;
  pwr_tStatus sts;

  if ( cdh_tidIsCid(self->tid))
    sts = gdh_ObjidToName( cdh_ClassIdToObjid(self->tid), name, sizeof(name), cdh_mName_volumeStrict);
  else
    sts = gdh_ObjidToName( cdh_TypeIdToObjid(self->tid), name, sizeof(name), cdh_mName_volumeStrict);
  if ( EVEN(sts))
    strcpy( name, "");

  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);
}

/* 
 * ADef object functions
 */
static PyObject *
ADef_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  ADefObject *self;

  self = (ADefObject *)type->tp_alloc(type, 0);
  if (self != NULL) {
    strcpy(self->name, "");
    self->cid = 0;
    self->offset = 0;
    self->size = 0;
    self->flags = 0;
    self->elements = 0;
    self->aix = 0;
    self->typeref = 0;
  }

  return (PyObject *)self;
}

static PyObject *
ADef_str(PyObject *self)
{
  char *utf8name = cnv_iso8859_to_utf8(((ADefObject *)self)->name, 
				       strlen(((ADefObject *)self)->name)+1);
  return PyUnicode_FromFormat("%s", utf8name);
}

static int
ADef_init(ADefObject *self, PyObject *args, PyObject *kwds)
{
  return 0;
}

static PyObject *
ADef_name(ADefObject *self, PyObject *args)
{
  char *utf8name = cnv_iso8859_to_utf8(self->name, strlen(self->name)+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
ADef_cid(ADefObject *self, PyObject *args)
{
  return Py_BuildValue("i", self->cid);
}

static PyObject *
ADef_offset(ADefObject *self, PyObject *args)
{
  return Py_BuildValue("i", self->offset);
}

static PyObject *
ADef_size(ADefObject *self, PyObject *args)
{
  return Py_BuildValue("i", self->size);
}

static PyObject *
ADef_flags(ADefObject *self, PyObject *args)
{
  return Py_BuildValue("i", self->flags);
}

static PyObject *
ADef_elements(ADefObject *self, PyObject *args)
{
  return Py_BuildValue("i", self->elements);
}

static PyObject *
ADef_typeref(ADefObject *self, PyObject *args)
{
  return Py_BuildValue("i", self->typeref);
}

/* 
 * Sub object functions
 */
static PyObject *
Sub_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  SubObject *self;

  self = (SubObject *)type->tp_alloc(type, 0);
  if (self != NULL) {
    self->type = 0;
    self->size = 0;
    self->p = 0;
    self->refid = pwr_cNRefId;
  }

  return (PyObject *)self;
}

static PyObject *
Sub_str(PyObject *s)
{
  SubObject *self = (SubObject *)s;
  return PyUnicode_FromFormat("%d.%d.%d.%d:%u", 
			      0xFF & (self->refid.nid >> 24), 
			      0xFF & (self->refid.nid >> 16), 
			      0xFF & (self->refid.nid >> 8), 
			      0xFF & self->refid.nid, 
			      self->refid.rix);
}

static int
Sub_init(SubObject *self, PyObject *args, PyObject *kwds)
{
  pwr_tStatus sts;
  char *name, *utf8name;
  pwr_eType atype;
  unsigned int asize;

  if (! PyArg_ParseTuple(args, "s", &utf8name))
    return -1;

  name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);
  sts = gdh_GetAttributeCharacteristics(name, &atype, &asize, 0, 0);
  if (EVEN(sts)) {
    self->sts = sts;
    self->p = 0;
    return -1;
  }

  sts = gdh_RefObjectInfo(name, &self->p, &self->refid, asize);
  if ( EVEN(sts)) {
    self->sts = sts;
    self->p = 0;
    return -1;
  }
  
  self->type = atype;
  self->size = asize;
  return 0;
}

static PyObject *
Sub_value(SubObject *self, PyObject *args)
{
  if ( self->p == 0)
    return set_error(self->sts);

  switch ( self->type) {
  case pwr_eType_Boolean: {
    pwr_tBoolean value = *(pwr_tBoolean *)self->p;
    return Py_BuildValue("i", value);
  }
  case pwr_eType_Int8: {
    pwr_tInt8 value = *(pwr_tInt8 *)self->p;
    return Py_BuildValue("b", value);
  }
  case pwr_eType_UInt8: {
    pwr_tUInt8 value = *(pwr_tUInt8 *)self->p;
    return Py_BuildValue("B", value);
  }
  case pwr_eType_Int16: {
    pwr_tInt16 value = *(pwr_tInt16 *)self->p;
    return Py_BuildValue("h", value);
  }
  case pwr_eType_UInt16: {
    pwr_tUInt16 value = *(pwr_tUInt16 *)self->p;
    return Py_BuildValue("H", value);
  }
  case pwr_eType_Int32: {
    pwr_tInt32 value = *(pwr_tInt32 *)self->p;
    return Py_BuildValue("i", value);
  }
  case pwr_eType_UInt32: {
    pwr_tUInt32 value = *(pwr_tUInt32 *)self->p;
    return Py_BuildValue("I", value);
  }
  case pwr_eType_Int64: {
    char format[2];
    pwr_tInt64 value = *(pwr_tInt64 *)self->p;
#if defined HW_X86_64
    strcpy( format, "l");
#else
    strcpy( format, "L");
#endif
    return Py_BuildValue(format, value);
  }
  case pwr_eType_UInt64: {
    char format[2];
    pwr_tUInt64 value = *(pwr_tUInt64 *)self->p;
#if defined HW_X86_64
    strcpy( format, "k");
#else
    strcpy( format, "K");
#endif
    return Py_BuildValue(format, value);
  }
  case pwr_eType_Float32: {
    pwr_tFloat32 value = *(pwr_tFloat32 *)self->p;
    return Py_BuildValue("f", value);
  }
  case pwr_eType_Float64: {
    pwr_tFloat64 value = *(pwr_tFloat64 *)self->p;
    return Py_BuildValue("d", value);
  }
  case pwr_eType_String:
    return Py_BuildValue("s", self->p);
  case pwr_eType_Time: {
    char timstr[30];

    time_AtoAscii((pwr_tTime *)self->p, time_eFormat_DateAndTime, timstr, sizeof(timstr));
    return Py_BuildValue("s", timstr);
  }
  case pwr_eType_DeltaTime: {
    char timstr[30];

    time_DtoAscii((pwr_tDeltaTime *)self->p, 1, timstr, sizeof(timstr));
    return Py_BuildValue("s", timstr);
  }
  case pwr_eType_Objid: {    
    pwr_tOid value = *(pwr_tOid *)self->p;

    OidObject *oido = (OidObject *)Oid_new(&OidType, 0, 0);
    if ( oido != NULL)
      oido->oid = value;

    return (PyObject *)oido;
  }
  case pwr_eType_AttrRef: {    
    pwr_tAttrRef value = *(pwr_tAttrRef *)self->p;

    ArefObject *arefo = (ArefObject *)Aref_new(&ArefType, 0, 0);
    if ( arefo != NULL)
      arefo->aref = value;

    return (PyObject *)arefo;
  }
  default:
    return set_error(GDH__BADOBJTYPE);
  }
  Py_RETURN_NONE;
}

static PyObject *
Sub_setValue(SubObject *self, PyObject *args)
{
  char *buf;

  if ( self->p == 0)
    return NULL;

  buf = (char *)malloc(self->size);

  switch ( self->type) {
  case pwr_eType_Boolean: {
    unsigned int value;
    if ( !PyArg_ParseTuple(args, "I", &value))
      goto error_return;
    if ( value == 1)
      *(pwr_tBoolean *)buf = 1;
    else if ( value == 0)
      *(pwr_tBoolean *)buf = 0;
    else
      goto error_return;

    break;
  }
  case pwr_eType_Int8: {
    if ( !PyArg_ParseTuple(args, "b", buf))
      goto error_return;
    break;
  }
  case pwr_eType_UInt8: {
    if ( !PyArg_ParseTuple(args, "B", buf))
      goto error_return;
    break;
  }
  case pwr_eType_Int16: {
    if ( !PyArg_ParseTuple(args, "h", buf))
      goto error_return;
    break;
  }
  case pwr_eType_UInt16: {
    if ( !PyArg_ParseTuple(args, "H", buf))
      goto error_return;
    break;
  }
  case pwr_eType_Int32:
  case pwr_eType_Enum: {
    if ( !PyArg_ParseTuple(args, "i", buf))
      goto error_return;
    break;
  }
  case pwr_eType_UInt32:
  case pwr_eType_Status:
  case pwr_eType_NetStatus:
  case pwr_eType_Mask: {
    if ( !PyArg_ParseTuple(args, "I", buf))
      goto error_return;
    break;
  }
  case pwr_eType_Int64: {
#if defined HW_X86_64
    char format[] = "l";
#else
    char format[] = "L";
#endif
    if ( !PyArg_ParseTuple(args, format, buf))
      goto error_return;
    break;
  }
  case pwr_eType_UInt64: {
#if defined HW_X86_64
    char format[] = "k";
#else
    char format[] = "K";
#endif
    if ( !PyArg_ParseTuple(args, format, buf))
      goto error_return;
    break;
  }
  case pwr_eType_Float32: {
    if ( !PyArg_ParseTuple(args, "f", buf))
      goto error_return;
    break;
  }
  case pwr_eType_Float64: {
    if ( !PyArg_ParseTuple(args, "d", buf))
      goto error_return;
    break;
  }
  case pwr_eType_String: {
    char *value, *utf8value = 0;
    if ( !PyArg_ParseTuple(args, "s", &utf8value))
      goto error_return;

    value = cnv_utf8_to_iso8859(utf8value, strlen(utf8value)+1);
    strncpy( buf, value, self->size);
    buf[self->size-1] = 0;
    break;
  }
  case pwr_eType_Time: {
    char *value = 0;
    if ( !PyArg_ParseTuple(args, "s", &value))
      goto error_return;

    time_AsciiToA(value, (pwr_tTime *)buf);
    break;
  }
  case pwr_eType_DeltaTime: {
    char *value = 0;
    if ( !PyArg_ParseTuple(args, "s", &value))
      goto error_return;

    time_AsciiToD(value, (pwr_tDeltaTime *)buf);
    break;
  }
  case pwr_eType_Objid: {
    OidObject *o;
    pwr_tOid oid;

    if ( !PyArg_ParseTuple(args, "O", &o))
      goto error_return;

    oid = o->oid;
    memcpy(buf, &oid, sizeof(oid));
    break;
  }
  case pwr_eType_AttrRef: {
    ArefObject *a;
    pwr_tAttrRef aref;

    if ( !PyArg_ParseTuple(args, "O", &a))
      goto error_return;

    aref = a->aref;
    memcpy(buf, &aref, sizeof(aref));
    break;
  }
  default:
    set_error(GDH__NYI);
    goto error_return;
  }

  memcpy(self->p, buf, self->size);
  free(buf);
  Py_RETURN_NONE;

error_return:
  free(buf);
  return NULL;    
}

static PyObject *
Sub_close(SubObject *self, PyObject *args)
{
  if ( self->p) {
    gdh_UnrefObjectInfo(self->refid);
    self->p = 0;
  }
  Py_RETURN_NONE;
}

/* 
 * Appl object functions
 */
static PyObject *
Appl_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  ApplObject *self;

  self = (ApplObject *)type->tp_alloc(type, 0);
  if (self != NULL) {
    self->anix = 0;
    self->scantime = 0;
    self->maxdelay = 5;
    self->qid = qcom_cNQid;
    strcpy( self->name, "");
    self->open = 0;
    self->scan = 0;
    self->close = 0;
    self->ctx = 0;
  }

  return (PyObject *)self;
}

static PyObject *
Appl_str(PyObject *s)
{
  ApplObject *self = (ApplObject *)s;
  return PyUnicode_FromFormat("%s", self->name);
}

static int
Appl_init(ApplObject *self, PyObject *args, PyObject *kwds)
{
  pwr_tStatus sts;
  char name[80], *utf8name;
  char *applobj, *utf8applobj;
  int anix;
  float scantime;
  float maxdelay = 0;
  PyObject *open, *scan, *close;
  PyObject *ctx = 0;
  pwr_tAName aname;
  qcom_sQid qini;
  qcom_sQattr qAttr;

  if (! PyArg_ParseTuple(args, "ssIfOOO|Of", &utf8name, &utf8applobj, &anix, &scantime, &open, &scan, &close,
			 &ctx, &maxdelay))
    return -1;

  strncpy(name, cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1), sizeof(name));
  applobj = cnv_utf8_to_iso8859(utf8applobj, strlen(utf8applobj)+1);
  if ( !PyCallable_Check(open)) {
    set_error(GDH__BADARG);
    return -1;
  }
  if ( !PyCallable_Check(scan)) {
    set_error(GDH__BADARG);
    return -1;
  }
  if ( !PyCallable_Check(close)) {
    set_error(GDH__BADARG);
    return -1;
  }

  if (!pwrrt_initialized) {
    sts = gdh_Init(name);
    if ( EVEN(sts)) {
      set_error(sts);
      return -1;
    }
  }

  sts = gdh_NameToObjid(applobj, &self->apploid);
  if ( EVEN(sts)) {
    set_error(sts);
    return -1;
  }

  strncpy( self->name, name, sizeof(self->name));
  self->anix = anix;
  self->scantime = scantime;
  Py_XINCREF(open);
  Py_XINCREF(close);
  Py_XINCREF(scan);
  self->open = open;
  self->close = close;
  self->scan = scan;
  if ( ctx) {
    Py_XINCREF(ctx);
    self->ctx = ctx;
  }
  if ( maxdelay != 0)
    self->maxdelay = maxdelay;

  // Init error and status logger with a unique application index per node.
  errh_Init( self->name, self->anix);
  if (self->anix < errh_eAnix_plc1)
    errh_SetStatus( PWR__APPLSTARTUP);
  else
    errh_SetStatus( PWR__SRVSTARTUP);

  // Create a queue to receive stop and restart events
  if (!qcom_Init(&sts, 0, self->name)) {
    errh_Fatal("qcom_Init, %m", sts); 
    if (self->anix < errh_eAnix_plc1)
      errh_SetStatus( PWR__SRVTERM);
    else
      errh_SetStatus( PWR__APPLTERM);
    set_error(sts);
    return -1;
  } 

  qAttr.type = qcom_eQtype_private;
  qAttr.quota = 100;
  if (!qcom_CreateQ(&sts, &self->qid, &qAttr, "events")) {
    errh_Fatal("qcom_CreateQ, %m", sts);
    if (self->anix < errh_eAnix_plc1)
      errh_SetStatus( PWR__APPLTERM);
    else
      errh_SetStatus( PWR__SRVTERM);
    set_error(sts);
    return -1;
  } 

  qini = qcom_cQini;
  if (!qcom_Bind(&sts, &self->qid, &qini)) {
    errh_Fatal("qcom_Bind(Qini), %m", sts);
    if (self->anix < errh_eAnix_plc1)
      errh_SetStatus( PWR__SRVTERM);
    else
      errh_SetStatus( PWR__APPLTERM);
    set_error(sts);
    return -1;
  }

  aproc_RegisterObject( self->apploid);

  strncpy(aname, applobj, sizeof(aname));
  strcat(aname, ".Anix");

  sts = gdh_SetObjectInfo(aname, &self->anix, sizeof(self->anix));

  return 0;
}

static PyObject *
Appl_mainloop(ApplObject *self, PyObject *args)
{
  PyObject *arglist;
  pwr_tStatus sts;
  int tmo;
  char mp[2000];
  qcom_sGet get;
  int swap = 0;
  int first_scan = 1;

  arglist = Py_BuildValue("()", self->ctx);

  PyEval_CallObject(self->open, arglist);

  aproc_TimeStamp( self->scantime, self->maxdelay);
  if (self->anix < errh_eAnix_plc1)
    errh_SetStatus( PWR__SRUN);
  else
    errh_SetStatus( PWR__ARUN);

  first_scan = 1;
  for (;;) {
    if ( first_scan) {
      tmo = (int) (self->scantime * 1000 - 1);
    }

    get.maxSize = sizeof(mp);
    get.data = mp;
    qcom_Get( &sts, &self->qid, &get, tmo);
    if (sts == QCOM__TMO || sts == QCOM__QEMPTY) {
      if ( !swap) {
	aproc_TimeStamp( self->scantime, self->maxdelay);
	PyEval_CallObject(self->scan, arglist);
      }
    } 
    else {
      ini_mEvent  new_event;
      qcom_sEvent *ep = (qcom_sEvent*) get.data;

      new_event.m  = ep->mask;
      if (new_event.b.oldPlcStop && !swap) {
	if (self->anix < errh_eAnix_plc1)
	  errh_SetStatus( PWR__SRVRESTART);
	else
	  errh_SetStatus( PWR__APPLRESTART);
        swap = 1;
	PyEval_CallObject(self->close, arglist);
      } else if (new_event.b.swapDone && swap) {
        swap = 0;
	PyEval_CallObject(self->open, arglist);
	if (self->anix < errh_eAnix_plc1)
	  errh_SetStatus( PWR__SRUN);
	else
	  errh_SetStatus( PWR__ARUN);
      } else if (new_event.b.terminate) {
	exit(0);
      }
    }
    first_scan = 0;
  }


  Py_DECREF(arglist);

  Py_RETURN_NONE;
}

static PyObject *
Appl_setStatus(ApplObject *self, PyObject *args)
{
  pwr_tStatus sts;
  char *stsstr;

  if ( !PyArg_ParseTuple(args, "s", &stsstr))
    return NULL;

  if ( strcmp(stsstr, "none") == 0)
    sts = 0;
  else if ( strcmp(stsstr, "applerror") == 0)
    sts = PWR__APPLERROR;
  else if ( strcmp(stsstr, "applfatal") == 0)
    sts = PWR__APPLFATAL;
  else if ( strcmp(stsstr, "applwarning") == 0)
    sts = PWR__APPLWARNING;
  else if ( strcmp(stsstr, "applinfo") == 0)
    sts = PWR__APPLINFO;
  else if ( strcmp(stsstr, "applstartup") == 0)
    sts = PWR__APPLSTARTUP;
  else if ( strcmp(stsstr, "applrun") == 0)
    sts = PWR__ARUN;
  else if ( strcmp(stsstr, "applstartup") == 0)
    sts = PWR__APPLSTARTUP;
  else if ( strcmp(stsstr, "applterm") == 0)
    sts = PWR__APPLTERM;
  else if ( strcmp(stsstr, "srverror") == 0)
    sts = PWR__SRVERROR;
  else if ( strcmp(stsstr, "srvfatal") == 0)
    sts = PWR__SRVFATAL;
  else if ( strcmp(stsstr, "srvwarning") == 0)
    sts = PWR__SRVWARNING;
  else if ( strcmp(stsstr, "srvinfo") == 0)
    sts = PWR__SRVINFO;
  else if ( strcmp(stsstr, "srvrun") == 0)
    sts = PWR__SRUN;
  else if ( strcmp(stsstr, "srvrestart") == 0)
    sts = PWR__SRVRESTART;
  else if ( strcmp(stsstr, "srvterm") == 0)
    sts = PWR__SRVTERM;
  else
    return set_error(GDH__BADARG);

  errh_SetStatus( sts);

  Py_RETURN_NONE;
}

static PyObject *
Appl_log(ApplObject *self, PyObject *args)
{
  char *severity;
  char *text;

  if ( !PyArg_ParseTuple(args, "ss", &severity, &text))
    return NULL;

  if ( strcmp(severity, "info") == 0)
    errh_Info(text);
  else if ( strcmp(severity, "warning") == 0)
    errh_Warning(text);
  else if ( strcmp(severity, "error") == 0)
    errh_Error(text);
  else if ( strcmp(severity, "fatal") == 0)
    errh_Fatal(text);
  else
    return set_error(GDH__BADARG);

  Py_RETURN_NONE;
}

/**
 * Static methods
 */


static PyObject *pwrrt_init(PyObject *self, PyObject *args)
{	     
  const char *name;
  pwr_tStatus sts;

  if (pwrrt_initialized)
    Py_RETURN_NONE;

  if ( !PyArg_ParseTuple(args, "s", &name))
    return NULL;

  sts = gdh_Init(name);
  if ( EVEN(sts))
    return set_error(sts);
  pwrrt_initialized = 1;
  Py_RETURN_NONE;
}

static PyObject *pwrrt_volume(PyObject *self, PyObject *args)
{
  pwr_tOid oid;
  OidObject *o;
  pwr_tStatus sts;
  char *name, *utf8name = 0;
  pwr_tVid vid;

  if ( !PyArg_ParseTuple(args, "|s", &utf8name))
    return NULL;

  if ( utf8name) {
    pwr_tObjName vname;
    int found;

    name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);

    found = 0;
    for ( sts = gdh_GetVolumeList(&vid);
	  ODD(sts);
	  sts = gdh_GetNextVolume(vid, &vid)) {
      sts = gdh_VolumeIdToName(vid, vname, sizeof(vname));
      if (EVEN(sts))
	return set_error(sts);

      if ( str_NoCaseStrcmp( name, vname) == 0) {
	found = 1;
        break;
      }
    }
    if ( !found)
      return set_error(GDH__NOSUCHVOL);

    oid.vid = vid;
    oid.oix = 0;
  }
  else {
    /* No args */
    sts = gdh_GetRootVolume( &vid);
    if (EVEN(sts))
      return set_error(GDH__NOSUCHVOL);
    oid.vid = vid;
    oid.oix = 0;
  }

  o = (OidObject *)Oid_new(&OidType, 0, 0);
  if (o != NULL)
    o->oid = oid;

  return (PyObject *)o;
}

static PyObject *pwrrt_volumes(PyObject *self, PyObject *args)
{
  OidObject *vol;
  pwr_tStatus sts;
  PyObject *result;
  pwr_tVid vid;
  int cnt = 0;

  for ( sts = gdh_GetVolumeList(&vid);
	ODD(sts);
	sts = gdh_GetNextVolume(vid, &vid))
    cnt++;
  
  result = PyTuple_New(cnt);
  cnt = 0;
  for ( sts = gdh_GetVolumeList(&vid);
	ODD(sts);
	sts = gdh_GetNextVolume(vid, &vid)) {
    vol = (OidObject *)Oid_new(&OidType, 0, 0);
    if (vol != NULL) {
      vol->oid.vid = vid;
      vol->oid.oix = 0;
      PyTuple_SetItem(result, cnt, (PyObject *)vol);
      cnt++;
    }    
  }
  return result;
}

static PyObject *pwrrt_object(PyObject *self, PyObject *args)
{
  PyObject *o = Oid_new(&OidType, args, 0);
  Oid_init((OidObject *)o, args, 0);
  if (EVEN(((OidObject *)o)->sts))
    return NULL;
  return o;
}

static PyObject *pwrrt_attribute(PyObject *self, PyObject *args)
{
  PyObject *a = Aref_new(&ArefType, args, 0);
  Aref_init((ArefObject *)a, args, 0);
  if (EVEN(((ArefObject *)a)->sts))
    return NULL;
  return a;
}

static PyObject *pwrrt_subscribe(PyObject *self, PyObject *args)
{
  PyObject *s = Sub_new(&SubType, args, 0);
  Sub_init((SubObject *)s, args, 0);
  return s;
}

static PyObject *pwrrt_application(PyObject *self, PyObject *args)
{
  PyObject *a = Appl_new(&ApplType, args, 0);
  Appl_init((ApplObject *)a, args, 0);
  return a;
}

static PyObject *pwrrt_login(PyObject *self, PyObject *args)
{
  pwr_tStatus sts;
  char *utf8user;
  char *utf8passwd;
  char user[80], passwd[80];
  unsigned int priv;
  const char systemgroup[80];

  sts = gdh_GetObjectInfo ( "pwrNode-System.SystemGroup", (pwr_tAddress)&systemgroup, 
		sizeof(systemgroup));
  if ( EVEN(sts))
    return set_error(sts);

  if ( !PyArg_ParseTuple(args, "ss", &utf8user, &utf8passwd))
    return NULL;

  strncpy(user, cnv_utf8_to_iso8859(utf8user, strlen(utf8user)+1), sizeof(user));
  strncpy(passwd, cnv_utf8_to_iso8859(utf8passwd, strlen(utf8passwd)+1), sizeof(passwd));

  sts = user_CheckUser( systemgroup, user, user_PwCrypt((char *)passwd), &priv);
  if ( EVEN(sts))
    return set_error(sts);

  strcpy( pwrrt_user, user);
  pwrrt_priv = priv;

  Py_RETURN_NONE;
}

static PyObject *pwrrt_logout(PyObject *self, PyObject *args)
{
  strcpy( pwrrt_user, "");
  pwrrt_priv = pwr_mPrv_RtRead;

  Py_RETURN_NONE;
}

static PyObject *pwrrt_getPriv(PyObject *self, PyObject *args)
{
  return Py_BuildValue("I", pwrrt_priv);
}

static PyObject *pwrrt_getUser(PyObject *self, PyObject *args)
{
  char *utf8user = cnv_iso8859_to_utf8(pwrrt_user, strlen(pwrrt_user)+1);
  return Py_BuildValue("s", utf8user);
}

static PyObject *pwrrt_getMsg(PyObject *self, PyObject *args)
{
  int sts;
  char msg[120];

  if ( !PyArg_ParseTuple(args, "i", &sts))
    return NULL;

  msg_GetMsg(sts, msg, sizeof(msg));

  char *utf8msg = cnv_iso8859_to_utf8(msg, strlen(msg)+1);
  return Py_BuildValue("s", utf8msg);
}


static PyObject *pwrrt_getSevItemList(PyObject *self, PyObject *args)
{
  char *server;
  char *filter = 0, *utf8filter = 0;
  pwr_tStatus sts;
  sevcli_sHistItem *itemlist;
  unsigned int itemcnt;
  int cnt;
  int i, j;
  sevcli_sHistItem *lp;
  PyObject *ituple, *result;

  if ( !pwrrt_scctx) {
    sevcli_init( &sts, &pwrrt_scctx);
    if ( EVEN(sts))
      return set_error(sts);
  }

  if ( !PyArg_ParseTuple(args, "s|si", &server, &utf8filter))
    return NULL;

  sevcli_set_servernode( &sts, pwrrt_scctx, server);
  if ( EVEN(sts))
    return set_error(sts);

  sevcli_get_itemlist( &sts, pwrrt_scctx, &itemlist, &itemcnt);
  if (EVEN(sts))
    return set_error(sts);

  if ( utf8filter && strcmp(utf8filter,"") == 0)
    utf8filter = 0;

  cnt = 0;
  if ( utf8filter) {
    filter = cnv_utf8_to_iso8859(utf8filter, strlen(utf8filter)+1);

    lp = itemlist;
    for ( i = 0; i < itemcnt; i++) {
      if ( str_NoCaseStrncmp(filter, lp->oname, strlen(filter)) == 0)
	cnt++;
      lp = (sevcli_sHistItem *)&lp->attr[lp->attrnum];
    }
  }
  else
    cnt = itemcnt;

  result = PyTuple_New(cnt);

  cnt = 0;
  lp = itemlist;
  for ( i = 0; i < itemcnt; i++) {
    if ( !filter || str_NoCaseStrncmp(filter, lp->oname, strlen(filter)) == 0) {

      ituple = PyTuple_New(6+lp->attrnum);
      PyTuple_SetItem(ituple, 0, PyUnicode_FromString(cnv_iso8859_to_utf8(lp->oname, strlen(lp->oname)+1)));
      PyTuple_SetItem(ituple, 1, PyUnicode_FromString(cdh_ObjidToString(lp->oid, 1)));
      PyTuple_SetItem(ituple, 2, PyUnicode_FromString(cnv_iso8859_to_utf8(lp->description, sizeof(lp->description)+1)));
      PyTuple_SetItem(ituple, 3, PyLong_FromUnsignedLong(lp->deadband));
      PyTuple_SetItem(ituple, 4, PyLong_FromUnsignedLong(lp->options));
      PyTuple_SetItem(ituple, 5, PyFloat_FromDouble(lp->scantime));
      for ( j = 0; j < lp->attrnum; j++)
	PyTuple_SetItem(ituple, j+6, PyUnicode_FromString(cnv_iso8859_to_utf8(lp->attr[j].aname, sizeof(lp->attr[j])+1)));
      
      PyTuple_SetItem(result, cnt, ituple);
      cnt++;
    }
    lp = (sevcli_sHistItem *)&lp->attr[lp->attrnum];
  }

  free(itemlist);
  return result;
}

static PyObject *pwrrt_getSevEventsItemList(PyObject *self, PyObject *args)
{
  char *server;
  char *filter = 0;
  pwr_tStatus sts;
  sevcli_sEventsItem *itemlist;
  unsigned int itemcnt;
  int cnt;
  int i;
  PyObject *ituple, *result;

  if ( !pwrrt_scctx) {
    sevcli_init( &sts, &pwrrt_scctx);
    if ( EVEN(sts))
      return set_error(sts);
  }

  if ( !PyArg_ParseTuple(args, "s|si", &server, &filter))
    return NULL;

  sevcli_set_servernode( &sts, pwrrt_scctx, server);
  if ( EVEN(sts))
    return set_error(sts);

  sevcli_get_eventsitemlist( &sts, pwrrt_scctx, &itemlist, &itemcnt);
  if (EVEN(sts))
    return set_error(sts);

  if ( filter && strcmp(filter,"") == 0)
    filter = 0;

  cnt = 0;
  if ( filter) {
    for ( i = 0; i < itemcnt; i++) {
      if ( str_NoCaseStrncmp(filter, itemlist[i].oname, strlen(filter)) == 0)
	cnt++;
    }
  }
  else
    cnt = itemcnt;

  result = PyTuple_New(cnt);

  cnt = 0;
  for ( i = 0; i < itemcnt; i++) {
    if ( !filter || str_NoCaseStrncmp(filter, itemlist[i].oname, strlen(filter)) == 0) {

      ituple = PyTuple_New(4);
      PyTuple_SetItem(ituple, 0, PyUnicode_FromString(cnv_iso8859_to_utf8(itemlist[i].oname, strlen(itemlist[i].oname)+1)));
      PyTuple_SetItem(ituple, 1, PyUnicode_FromString(cdh_ObjidToString(itemlist[i].oid, 1)));
      PyTuple_SetItem(ituple, 2, PyUnicode_FromString(cnv_iso8859_to_utf8(itemlist[i].description, strlen(itemlist[i].description)+1)));
      PyTuple_SetItem(ituple, 3, PyLong_FromUnsignedLong(itemlist[i].options));
      
      PyTuple_SetItem(result, cnt, ituple);
      cnt++;
    }
  }

  free(itemlist);
  return result;
}

static PyObject *pwrrt_getSevItemData(PyObject *self, PyObject *args)
{
  pwr_tStatus sts;
  char *server, *oidstr, *aname, *fromstr, *tostr;
  pwr_tTime from, to;
  pwr_tDeltaTime fromdelta;
  pwr_tOid oid;
  pwr_tTime *tbuf;
  void *vbuf;
  int maxrows = 0;
  int rows;
  pwr_eType vtype;
  unsigned int vsize;
  PyObject *vtuple, *ttuple, *result;
  char timstr[30];
  int i;
  PyObject *date;
  time_t sec;
  struct tm ts;
  char *time_format = 0;
  int time_string = 0;

  if ( !pwrrt_scctx) {
    sevcli_init( &sts, &pwrrt_scctx);
    if ( EVEN(sts))
      return set_error(sts);
  }

  if ( !PyArg_ParseTuple(args, "sssss|Is", &server, &oidstr, &aname, &fromstr, &tostr, &maxrows,
			 &time_format))
    return NULL;

  if ( time_format) {
    if ( strcmp(time_format, "string") == 0)
      time_string = 1;
  }
  if ( maxrows == 0)
    maxrows = 1000;

  sevcli_set_servernode( &sts, pwrrt_scctx, server);
  if ( EVEN(sts))
    return set_error(sts);

  if ( strncmp("_O", oidstr, 2) == 0) 
    sts = cdh_StringToObjid( oidstr, &oid);
  else
    sts = gdh_NameToObjid( oidstr, &oid);
  if ( EVEN(sts))
    return set_error(sts);

  if ( strcmp(tostr, "now") == 0) {
    /* fromstr is a deltatime */
    sts = time_AsciiToD(fromstr, &fromdelta);
    if ( EVEN(sts))
      return set_error(sts);

    time_GetTime(&to);
    time_Asub(&from, &to, &fromdelta);
  }
  else {
    sts = time_AsciiToA(fromstr, &from);
    if ( EVEN(sts))
      return set_error(sts);

    sts = time_AsciiToA(tostr, &to);
    if ( EVEN(sts))
      return set_error(sts);
  }

  sevcli_get_itemdata( &sts, pwrrt_scctx, oid, aname, from, to, maxrows, &tbuf, &vbuf,
  		       &rows, &vtype, &vsize);
  if ( sts == SEV__NOPOINTS)
    Py_RETURN_NONE;
  else if (EVEN(sts))
    return set_error(sts);

  result = PyTuple_New(3);
  vtuple = PyTuple_New(rows);
  ttuple = PyTuple_New(rows);


  for ( i = 0; i < rows; i++) {
    switch (vtype) {
    case pwr_eType_Float32:
      PyTuple_SetItem(vtuple, i, PyFloat_FromDouble((double)((pwr_tFloat32 *)vbuf)[i]));
      break;
    case pwr_eType_Int32:
      PyTuple_SetItem(vtuple, i, PyLong_FromLong((long)((pwr_tInt32 *)vbuf)[i]));
      break;
    case pwr_eType_Boolean:
    case pwr_eType_UInt32:
      PyTuple_SetItem(vtuple, i, PyLong_FromUnsignedLong((unsigned long)((pwr_tUInt32 *)vbuf)[i]));
      break;
    default:;
    }

    if ( time_string) {
      /* Time string */
      time_AtoAscii( &tbuf[i], time_eFormat_DateAndTime, timstr, sizeof(timstr));
      PyTuple_SetItem(ttuple, i, PyBytes_FromString(timstr));
    }
    else {
      /* Time datetime object */
      sec = (time_t)tbuf[i].tv_sec;
      localtime_r(&sec, &ts);
      date = PyDateTime_FromDateAndTime(ts.tm_year+1900, ts.tm_mon+1, ts.tm_mday,
					ts.tm_hour, ts.tm_min, ts.tm_sec, 
					(int)tbuf[i].tv_nsec/1000);
      PyTuple_SetItem(ttuple, i, date);
    }      
  }

  free(tbuf);
  free(vbuf);

  PyTuple_SetItem(result, 0, PyLong_FromLong((long)rows));
  PyTuple_SetItem(result, 1, vtuple);
  PyTuple_SetItem(result, 2, ttuple);

  return result;
}


static PyObject *pwrrt_getSevItemsDataFrame(PyObject *self, PyObject *args)
{
  pwr_tStatus sts;
  char *server, *fromstr, *tostr;
  pwr_tOName *oidvect;
  pwr_tOName *anamevect;
  pwr_tOName cond_oid;
  pwr_tOName cond_aname;
  int *isobjectvect;
  int oidcnt, anamecnt, isobjectcnt;
  pwr_tTime from, to;
  pwr_tDeltaTime fromdelta;
  float tdiff;
  pwr_tOid oid;
  pwr_tTime *ttbuf;
  PyObject *oidobj, *anameobj, *isobjectobj;  
  pwr_tFloat32 *vbuf;
  int maxrows = 0;
  int options = 0;
  int rows;
  pwr_eType *vtype;
  pwr_eType vtcond;
  unsigned int vsize;
  PyObject *vtuple, *result;
  int i, j, k;
  PyObject *date;
  time_t sec;
  struct tm ts;
  char *time_format = 0;
  int valcnt;
  pwr_tDeltaTime dt;
  pwr_tFloat32 **vvect;
  float *tbuf;  
  
  if ( !pwrrt_scctx) {
    sevcli_init( &sts, &pwrrt_scctx);
    if ( EVEN(sts))
      return set_error(sts);
  }

  if ( !PyArg_ParseTuple(args, "sOOOssf|IsI", &server, &oidobj, &anameobj, &isobjectobj, 
			 &fromstr, &tostr, &tdiff, &maxrows, &time_format, &options))
    return NULL;

  if ( time_format) {
    if ( strcmp(time_format, "string") == 0)
      ;
  }
  if ( maxrows == 0)
    maxrows = 1000;

  sevcli_set_servernode( &sts, pwrrt_scctx, server);
  if ( EVEN(sts))
    return set_error(sts);

  if ( strcmp(tostr, "now") == 0) {
    /* fromstr is a deltatime */
    sts = time_AsciiToD(fromstr, &fromdelta);
    if ( EVEN(sts))
      return set_error(sts);

    time_GetTime(&to);
    time_Asub(&from, &to, &fromdelta);
  }
  else {
    sts = time_AsciiToA(fromstr, &from);
    if ( EVEN(sts))
      return set_error(sts);

    sts = time_AsciiToA(tostr, &to);
    if ( EVEN(sts))
      return set_error(sts);
  }

  if ( PyTuple_Check(oidobj))
    oidcnt = PyTuple_Size(oidobj);
  else if ( PyList_Check(oidobj))
    oidcnt = PyList_Size(oidobj);
  else
    return set_error(GDH__ARGCOUNT);

  if ( PyTuple_Check(anameobj))
    anamecnt = PyTuple_Size(anameobj);
  else if ( PyList_Check(anameobj))
    anamecnt = PyList_Size(anameobj);
  else
    return set_error(GDH__ARGCOUNT);

  if ( PyTuple_Check(isobjectobj))
    isobjectcnt = PyTuple_Size(isobjectobj);
  else if ( PyList_Check(isobjectobj))
    isobjectcnt = PyList_Size(isobjectobj);
  else
    return set_error(GDH__ARGCOUNT);

  if ( oidcnt <= 0 || anamecnt <= 0 || isobjectcnt <= 0 || 
       oidcnt != anamecnt || oidcnt != isobjectcnt)
    return set_error(GDH__ARGCOUNT);

  oidvect = (pwr_tOName *)malloc(oidcnt * sizeof(pwr_tOName));
  vtype = (pwr_eType *)malloc(oidcnt * sizeof(pwr_eType));
  anamevect = (pwr_tOName *)malloc(anamecnt * sizeof(pwr_tOName));
  isobjectvect = (int *)malloc(isobjectcnt * sizeof(int));

  for ( i = 0; i < oidcnt; i++) {
    
    PyObject *pystr, *tstr;

    if ( PyTuple_Check(oidobj))
      pystr = PyTuple_GetItem(oidobj, i);
    else
      pystr = PyList_GetItem(oidobj, i);

    tstr = PyUnicode_AsEncodedString(pystr, "UTF-8", "strict");
    if (tstr != NULL) {
      strcpy(oidvect[i], PyBytes_AS_STRING(tstr));
      Py_DECREF(tstr);
    } else {
      strcpy(oidvect[i], "");
    }

    if ( PyTuple_Check(anameobj))
      pystr = PyTuple_GetItem(anameobj, i);
    else
      pystr = PyList_GetItem(anameobj, i);

    tstr = PyUnicode_AsEncodedString(pystr, "UTF-8", "strict");
    if (tstr != NULL) {
      strcpy(anamevect[i], PyBytes_AS_STRING(tstr));
      Py_DECREF(tstr);
    } else {
      strcpy(anamevect[i], "");
    }

    if ( PyTuple_Check(isobjectobj))
      pystr = PyTuple_GetItem(isobjectobj, i);
    else
      pystr = PyList_GetItem(isobjectobj, i);
    
    isobjectvect[i] = PyLong_AsLong(pystr);
  }

  if (options & 1) {
    /* First objid is condition */
    strcpy(cond_oid, oidvect[0]);
    strcpy(cond_aname, anamevect[0]);

    for (i = 0; i < oidcnt - 1; i++) {
      strcpy(oidvect[i], oidvect[i+1]);
      strcpy(anamevect[i], anamevect[i+1]);
      isobjectvect[i] = isobjectvect[i+1];
    }
    oidcnt -= 1;
    anamecnt -= 1;
    isobjectcnt -= 1;

    if (oidcnt <= 0)
      return set_error(GDH__ARGCOUNT);

    /* Get condition data */
    if ( strncmp("_O", cond_oid, 2) == 0) 
      sts = cdh_StringToObjid(cond_oid, &oid);
    else
      sts = gdh_NameToObjid(cond_oid, &oid);
    if ( EVEN(sts))
      return set_error(sts);

    sevcli_get_itemdata( &sts, pwrrt_scctx, oid, cond_aname, from, to, maxrows, &ttbuf, 
			 (void **)&vbuf, &rows, &vtcond, &vsize);
    if ( sts == SEV__NOPOINTS)
	Py_RETURN_NONE;
      else if (EVEN(sts))
	return set_error(sts);

    //for (i = 0; i < rows; i++) {
    //  char timstr[40];
    //  time_AtoAscii(&ttbuf[i], time_eFormat_DateAndTime, timstr, sizeof(timstr));
    //  printf("%d %s %d\n", i, timstr, ((pwr_tBoolean *)vbuf)[i]);      
    //}

    /* Find longest continuous interval where condition is high */
    pwr_tBoolean last_value = 0;
    pwr_tFloat32 max_time = 0.0f;
    int last_idx = 0;
    int start_idx = 0;
    int end_idx = 0;
    for (i = 0; i < rows; i++) {
      if (((pwr_tBoolean *)vbuf)[i] == 1 && last_value == 0) {
	last_value = 1;
	last_idx = i;
      }
      else if ((((pwr_tBoolean *)vbuf)[i] == 0 && last_value == 1) ||
	       (((pwr_tBoolean *)vbuf)[i] == 1 && last_value == 1 && i == rows - 1)) {
	last_value = 0;
	time_Adiff(&dt, &ttbuf[i], &ttbuf[last_idx]);
	if (time_DToFloat(0, &dt) > max_time) {
	  max_time = time_DToFloat(0, &dt);
	  start_idx = last_idx;
	  end_idx = i;
	}
      }
    }
    if (max_time > 0.0f) {
      from = ttbuf[start_idx];
      to = ttbuf[end_idx];
    }
    free(ttbuf);
    free(vbuf);
  }

  time_Adiff(&dt, &to, &from);
  valcnt = (int)time_DToFloat(NULL, &dt)/tdiff;  

  //for ( i = 0; i < oidcnt; i++)
  //  printf("arg %s.%s\n", oidvect[i], anamevect[i]);

  vvect = calloc(oidcnt, sizeof(float *));

  for ( i = 0; i < oidcnt; i++) {
    if ( strncmp("_O", oidvect[i], 2) == 0) 
      sts = cdh_StringToObjid( oidvect[i], &oid);
    else
      sts = gdh_NameToObjid( oidvect[i], &oid);
    if ( EVEN(sts))
      return set_error(sts);

    if (!isobjectvect[i]) {
      sevcli_get_itemdata( &sts, pwrrt_scctx, oid, anamevect[i], from, to, maxrows, &ttbuf, 
			   (void **)&vbuf, &rows, &vtype[i], &vsize);
      if ( sts == SEV__NOPOINTS)
	Py_RETURN_NONE;
      else if (EVEN(sts))
	return set_error(sts);

      /* Create data rows for pandas with interpolation */
      tbuf = malloc(rows * sizeof(float));

      for ( j = 0; j < rows; j++) {
	time_Adiff(&dt, &ttbuf[j], &from);
	time_DToFloat(&tbuf[j], &dt);
      }

      vvect[i] = malloc(valcnt * sizeof(float));
      k = 0;
      switch (vtype[i]) {
      case pwr_eType_Float32:
	for ( j = 0; j < valcnt; j++) {
	  if ( j*tdiff < tbuf[0])
	    vvect[i][j] = ((pwr_tFloat32 *)vbuf)[0];
	  else if ( j*tdiff >= tbuf[rows - 1])
	    vvect[i][j] = ((pwr_tFloat32 *)vbuf)[rows-1]; 
	  else {
	    while( tbuf[k] <= j*tdiff && k < rows - 1)
	      k++;
	    vvect[i][j] = ((pwr_tFloat32 *)vbuf)[k-1] + 
	      (((pwr_tFloat32 *)vbuf)[k] - ((pwr_tFloat32 *)vbuf)[k-1])/(tbuf[k] - tbuf[k-1]) * (j*tdiff - tbuf[k-1]);
	  }
	}
	break;
      case pwr_eType_Int32:
      case pwr_eType_UInt32:
      case pwr_eType_Boolean:
	for ( j = 0; j < valcnt; j++) {
	  if ( j*tdiff < tbuf[0])
	    vvect[i][j] = (float)((pwr_tInt32 *)vbuf)[0];
	  else if ( j*tdiff >= tbuf[rows - 1])
	    vvect[i][j] = (float)((pwr_tInt32 *)vbuf)[rows-1]; 
	  else {
	    while( tbuf[k] <= j*tdiff && k < rows - 1)
	      k++;
	    vvect[i][j] = (float)((pwr_tInt32 *)vbuf)[k-1] + 
	      (((pwr_tInt32 *)vbuf)[k] - ((pwr_tInt32 *)vbuf)[k-1])/(tbuf[k] - tbuf[k-1]) * (j*tdiff - tbuf[k-1]);
	  }
	}
	break;
      default: ;
	for ( j = 0; j < valcnt; j++)
	  vvect[i][j] = 0;
      }
      free(ttbuf);
      free(tbuf);
      free(vbuf);
    }
    else {
      sevcli_sHistAttr *attrbuf;
      int attrnum;
      int l, m;
      float *vp1, *vp2;

      sevcli_get_objectitemdata( &sts, pwrrt_scctx, oid, "", from, to, maxrows, &ttbuf, 
				 (void **)&vbuf, &rows, &attrbuf, &attrnum);
      if ( sts == SEV__NOPOINTS)
	Py_RETURN_NONE;
      else if (EVEN(sts))
	return set_error(sts);


      /* Create data rows for panda with interpolation */
      tbuf = malloc(rows * sizeof(float));

      for ( j = 0; j < rows; j++) {
	time_Adiff(&dt, &ttbuf[j], &from);
	time_DToFloat(&tbuf[j], &dt);
      }

      int linesize = 0;
      for (j = 0; j < attrnum; j++)
	linesize += attrbuf[j].size;

      for ( l = i; l < oidcnt; l++) {
	if ( !streq(oidvect[l], oidvect[i]))
	  break;

	for (m = 0; m < attrnum; m++) {
	  if ( streq(anamevect[l], attrbuf[m].aname)) {
	    int lineoffs = 0;
	    for (j = 0; j < m; j++)
	      lineoffs += attrbuf[j].size;
	  
	    vvect[l] = malloc(valcnt * sizeof(float));
	    k = 0;
	    for ( j = 0; j < valcnt; j++) {
	      if ( j*tdiff < tbuf[0]) {
		vp1 = (float *)(((char*)vbuf) + lineoffs);
		switch (attrbuf[m].type) {
		case pwr_eType_Float32:
		  vvect[l][j] = (float)*(pwr_tFloat32 *)vp1;
		  break;
		case pwr_eType_Int32:
		  vvect[l][j] = (float)*(pwr_tInt32 *)vp1;
		  break;
		case pwr_eType_UInt32:
		case pwr_eType_Boolean:
		  vvect[l][j] = (float)*(pwr_tUInt32 *)vp1;
		  break;
		default:
		  vvect[l][j] = 0;
		}
	      }
	      else if ( j*tdiff >=tbuf[rows - 1]) {
		vp1 = (float *)(((char*)vbuf) + (rows - 1)*linesize + lineoffs);
		switch (attrbuf[m].type) {
		case pwr_eType_Float32:
		  vvect[l][j] = (float)*(pwr_tFloat32 *)vp1;
		  break;
		case pwr_eType_Int32:
		  vvect[l][j] = (float)*(pwr_tInt32 *)vp1;
		  break;
		case pwr_eType_UInt32:
		case pwr_eType_Boolean:
		  vvect[l][j] = (float)*(pwr_tUInt32 *)vp1;
		  break;
		default:
		  vvect[l][j] = 0;
		}
	      }
	      else {
		while( tbuf[k] <= j*tdiff && k < rows)
		  k++;
		vp1 = (float *)(((char *)vbuf) + (k - 1)*linesize + lineoffs);
		vp2 = (float *)(((char *)vp1) + linesize);
		switch (attrbuf[m].type) {
		case pwr_eType_Float32:
		  vvect[l][j] = (float)(*(pwr_tFloat32 *)vp1 + (*(pwr_tFloat32 *)vp2 - 
		      *(pwr_tFloat32 *)vp1)/(tbuf[k] - tbuf[k-1]) * (j*tdiff - tbuf[k-1]));
		  break;
		case pwr_eType_Int32:
		  vvect[l][j] = (float)(*(pwr_tInt32 *)vp1 + (*(pwr_tInt32 *)vp2 - 
		      *(pwr_tInt32 *)vp1)/(tbuf[k] - tbuf[k-1]) * (j*tdiff - tbuf[k-1]));
		  break;
		case pwr_eType_UInt32:
		case pwr_eType_Boolean:
		  vvect[l][j] = (float)(*(pwr_tUInt32 *)vp1 + (*(pwr_tUInt32 *)vp2 - 
		      *(pwr_tUInt32 *)vp1)/(tbuf[k] - tbuf[k-1]) * (j*tdiff - tbuf[k-1]));
		  break;
		default:
		  vvect[l][j] = 0;
		}
	      }
	    }
	    break;
	  }
	}
      }
      free(vbuf);
      free(ttbuf);
      free(tbuf);
      free(attrbuf);
      i = l;
    }
  }

  free(oidvect);
  free(vtype);
  free(anamevect);

  result = PyList_New(valcnt);
  for ( i = 0; i < valcnt; i++) {
    vtuple = PyTuple_New(oidcnt+1);

    /* Time datetime object */
    pwr_tTime t;

    time_FloatToD(&dt, i*tdiff);
    time_Aadd(&t, &from, &dt);
    sec = (time_t)t.tv_sec;
    localtime_r(&sec, &ts);
    date = PyDateTime_FromDateAndTime(ts.tm_year+1900, ts.tm_mon+1, ts.tm_mday,
				      ts.tm_hour, ts.tm_min, ts.tm_sec, 
				      (int)t.tv_nsec/1000);
    PyTuple_SetItem(vtuple, 0, date);

    for ( j = 0; j < oidcnt; j++) {
      PyTuple_SetItem(vtuple, j+1, PyFloat_FromDouble((double)vvect[j][i]));
    }
    PyList_SetItem(result, i, vtuple);
  }

  free(vvect);
  
  return result;
}

static PyObject *pwrrt_getSevItemsDataFrameD(PyObject *self, PyObject *args)
{
  pwr_tStatus sts;
  char *server, *fromstr, *tostr;
  pwr_tOName *oidvect;
  pwr_tOName *anamevect;
  int *isobjectvect;
  int oidcnt, anamecnt, isobjectcnt;
  pwr_tTime from, to;
  pwr_tDeltaTime fromdelta;
  float tdiff;
  pwr_tOid oid;
  pwr_tTime *ttbuf, *tt0buf = 0;
  PyObject *oidobj, *anameobj, *isobjectobj;  
  pwr_tFloat32 *vbuf;
  int maxrows = 0;
  int rows;
  pwr_eType vtype;
  unsigned int vsize;
  PyObject *vtuple, *result;
  int i, j, k;
  PyObject *date;
  time_t sec;
  struct tm ts;
  char *time_format = 0;
  int valcnt = 0;
  pwr_tDeltaTime dt;
  pwr_tFloat32 **vvect;
  float *tbuf, *t0buf = 0;
  
  if ( !pwrrt_scctx) {
    sevcli_init( &sts, &pwrrt_scctx);
    if ( EVEN(sts))
      return set_error(sts);
  }

  if ( !PyArg_ParseTuple(args, "sOOOssf|Is", &server, &oidobj, &anameobj, &isobjectobj, 
			 &fromstr, &tostr, &tdiff, &maxrows, &time_format))
    return NULL;

  if ( time_format) {
    if ( strcmp(time_format, "string") == 0)
      ;
  }
  if ( maxrows == 0)
    maxrows = 1000;

  sevcli_set_servernode( &sts, pwrrt_scctx, server);
  if ( EVEN(sts))
    return set_error(sts);

  if ( strcmp(tostr, "now") == 0) {
    /* fromstr is a deltatime */
    sts = time_AsciiToD(fromstr, &fromdelta);
    if ( EVEN(sts))
      return set_error(sts);

    time_GetTime(&to);
    time_Asub(&from, &to, &fromdelta);
  }
  else {
    sts = time_AsciiToA(fromstr, &from);
    if ( EVEN(sts))
      return set_error(sts);

    sts = time_AsciiToA(tostr, &to);
    if ( EVEN(sts))
      return set_error(sts);
  }
  time_Adiff(&dt, &to, &from);

  if ( PyTuple_Check(oidobj))
    oidcnt = PyTuple_Size(oidobj);
  else if ( PyList_Check(oidobj))
    oidcnt = PyList_Size(oidobj);
  else
    return set_error(GDH__ARGCOUNT);

  if ( PyTuple_Check(anameobj))
    anamecnt = PyTuple_Size(anameobj);
  else if ( PyList_Check(anameobj))
    anamecnt = PyList_Size(anameobj);
  else
    return set_error(GDH__ARGCOUNT);

  if ( PyTuple_Check(isobjectobj))
    isobjectcnt = PyTuple_Size(isobjectobj);
  else if ( PyList_Check(isobjectobj))
    isobjectcnt = PyList_Size(isobjectobj);
  else
    return set_error(GDH__ARGCOUNT);

  if ( oidcnt <= 0 || anamecnt <= 0 || isobjectcnt <= 0 || 
       oidcnt != anamecnt || oidcnt != isobjectcnt)
    return set_error(GDH__ARGCOUNT);

  oidvect = (pwr_tOName *)malloc(oidcnt * sizeof(pwr_tOName));
  anamevect = (pwr_tOName *)malloc(anamecnt * sizeof(pwr_tOName));
  isobjectvect = (int *)malloc(isobjectcnt * sizeof(int));

  for ( i = 0; i < oidcnt; i++) {    
    PyObject *pystr, *tstr;

    if ( PyTuple_Check(oidobj))
      pystr = PyTuple_GetItem(oidobj, i);
    else
      pystr = PyList_GetItem(oidobj, i);

    tstr = PyUnicode_AsEncodedString(pystr, "UTF-8", "strict");
    if (tstr != NULL) {
      strcpy(oidvect[i], PyBytes_AS_STRING(tstr));
      Py_DECREF(tstr);
    } else {
      strcpy(oidvect[i], "");
    }

    if ( PyTuple_Check(anameobj))
      pystr = PyTuple_GetItem(anameobj, i);
    else
      pystr = PyList_GetItem(anameobj, i);

    tstr = PyUnicode_AsEncodedString(pystr, "UTF-8", "strict");
    if (tstr != NULL) {
      strcpy(anamevect[i], PyBytes_AS_STRING(tstr));
      Py_DECREF(tstr);
    } else {
      strcpy(anamevect[i], "");
    }

    if ( PyTuple_Check(isobjectobj))
      pystr = PyTuple_GetItem(isobjectobj, i);
    else
      pystr = PyList_GetItem(isobjectobj, i);
    
    isobjectvect[i] = PyLong_AsLong(pystr);
  }

  for ( i = 0; i < oidcnt; i++)
    printf("arg %s.%s\n", oidvect[i], anamevect[i]);

  vvect = calloc(oidcnt, sizeof(float *));

  for ( i = 0; i < oidcnt; i++) {
    if ( strncmp("_O", oidvect[i], 2) == 0) 
      sts = cdh_StringToObjid( oidvect[i], &oid);
    else
      sts = gdh_NameToObjid( oidvect[i], &oid);
    if ( EVEN(sts))
      return set_error(sts);

    if (!isobjectvect[i]) {
      sevcli_get_itemdata( &sts, pwrrt_scctx, oid, anamevect[i], from, to, maxrows, &ttbuf, 
			   (void **)&vbuf, &rows, &vtype, &vsize);
      if ( sts == SEV__NOPOINTS)
	Py_RETURN_NONE;
      else if (EVEN(sts))
	return set_error(sts);

      /* Create data rows for panda with interpolation */
      tbuf = malloc(rows * sizeof(float));
      
      for ( j = 0; j < rows; j++) {
	time_Adiff(&dt, &ttbuf[j], &from);
	time_DToFloat(&tbuf[j], &dt);
      }

      if ( i == 0) {
	valcnt = rows;
	tt0buf = ttbuf;
	t0buf = tbuf;
      }

      vvect[i] = malloc(valcnt * sizeof(float));
      k = 0;
      for ( j = 0; j < valcnt; j++) {
	if ( i == 0)
	  vvect[i][j] = vbuf[j];
	else {

	  if ( t0buf[j] < tbuf[0]) {
	    vvect[i][j] = vbuf[0];
	  }
	  else if ( t0buf[j] > tbuf[rows - 1]) {
	    vvect[i][j] = vbuf[rows-1]; 
	  }
	  else {
	    while( tbuf[k] <= t0buf[j])
	      k++;
	    if ( fabs(t0buf[j] - tbuf[k-1]) < FLT_EPSILON)
	      vvect[i][j] = vbuf[k-1];
	    else
	      vvect[i][j] = vbuf[k-1] + (vbuf[k] - vbuf[k-1])/(tbuf[k] - tbuf[k-1]) * (t0buf[j] - tbuf[k-1]);
	  }
	}
      }
      free(vbuf);
      if ( i != 0) {
	free(tbuf);
	free(ttbuf);
      }
    } 
    else {
      sevcli_sHistAttr *attrbuf;
      int attrnum;
      int l, m;
      float *vp1, *vp2;

      sevcli_get_objectitemdata( &sts, pwrrt_scctx, oid, "", from, to, maxrows, &ttbuf, 
				 (void **)&vbuf, &rows, &attrbuf, &attrnum);
      if ( sts == SEV__NOPOINTS)
	Py_RETURN_NONE;
      else if (EVEN(sts))
	return set_error(sts);

      /* Create data rows for panda with interpolation */
      tbuf = malloc(rows * sizeof(float));

      for ( j = 0; j < rows; j++) {
	time_Adiff(&dt, &ttbuf[j], &from);
	time_DToFloat(&tbuf[j], &dt);
      }

      if ( i == 0) {
	valcnt = rows;
	tt0buf = ttbuf;
	t0buf = tbuf;
      }

      int linesize = 0;
      for (j = 0; j < attrnum; j++)
	linesize += attrbuf[j].size;

      for ( l = i; l < oidcnt; l++) {
	if ( !streq(oidvect[l], oidvect[i]))
	  break;

	for (m = 0; m < attrnum; m++) {
	  if ( streq(anamevect[l], attrbuf[m].aname)) {
	    int lineoffs = 0;
	    for (j = 0; j < m; j++)
	      lineoffs += attrbuf[j].size;
	  
	    vvect[l] = malloc(valcnt * sizeof(float));
	    k = 0;
	    for ( j = 0; j < valcnt; j++) {
	      if ( i == 0) {
		vp1 = (float *)(((char*)vbuf) + j * linesize + lineoffs);
		vvect[l][j] = *vp1;
	      }
	      else {
		if ( t0buf[j] < tbuf[0]) {
		  vp1 = (float *)(((char *)vbuf) + lineoffs);
		  vvect[l][j] = *vp1;
		}
		else if ( t0buf[j] > tbuf[rows - 1]) {
		  vp1 = (float *)(((char*)vbuf) + (rows - 1)*linesize + lineoffs);
		  vvect[l][j] = *vp1;
		}
		else {
		  while( tbuf[k] <= t0buf[j])
		    k++;
		  if (fabs(t0buf[j] - tbuf[k-1]) < FLT_EPSILON) {
		    vp1 = (float *)(((char *)vbuf) + (k - 1)*linesize + lineoffs);
		    vvect[l][j] = *vp1;
		  }
		  else {
		    vp1 = (float *)(((char *)vbuf) + (k - 1)*linesize + lineoffs);
		    vp2 = (float *)(((char *)vp1) + linesize);
		    vvect[l][j] = *vp1 + (*vp2 - *vp1)/(tbuf[k] - tbuf[k-1]) * (t0buf[j] - tbuf[k-1]);
		  }
		}
	      }
	    }
	    break;
	  }
	}
      }
      free(vbuf);
      free(attrbuf);
      if ( i != 0) {
	free(ttbuf);
	free(tbuf);
      }
      i = l - 1;
    }
  }

  free(t0buf);
  free(oidvect);
  free(anamevect);

  result = PyList_New(valcnt);
  for ( i = 0; i < valcnt; i++) {
    vtuple = PyTuple_New(oidcnt+1);

    /* Time datetime object */
    pwr_tTime t = tt0buf[i];
    sec = (time_t)t.tv_sec;
    localtime_r(&sec, &ts);
    date = PyDateTime_FromDateAndTime(ts.tm_year+1900, ts.tm_mon+1, ts.tm_mday,
				      ts.tm_hour, ts.tm_min, ts.tm_sec, 
				      (int)t.tv_nsec/1000);
    PyTuple_SetItem(vtuple, 0, date);

    for ( j = 0; j < oidcnt; j++) {
      PyTuple_SetItem(vtuple, j+1, PyFloat_FromDouble((double)vvect[j][i]));
    }
    PyList_SetItem(result, i, vtuple);
  }

  free(vvect);  
  free(tt0buf);
  
  return result;
}

static PyObject *pwrrt_getSevEvents(PyObject *self, PyObject *args)
{
  pwr_tStatus sts;
  char *server, *oidstr, *fromstr, *tostr;
  unsigned int eventtypemask, eventpriomask;
  pwr_tTime from, to;
  pwr_tDeltaTime fromdelta;
  pwr_tOid oid;
  int maxrows = 0;
  sevcli_sEvents *list;
  unsigned int listcnt;
  PyObject *item_tuple, *eventid_tuple, *result;
  char timstr[30];
  int i;
  PyObject *date;
  time_t sec;
  struct tm ts;
  char *time_format = 0;
  int time_string = 0;
  char supobject[80];
  char eventtext[80];
  char eventname[200];
  char *utf8eventtext, *utf8eventname;

  if ( !pwrrt_scctx) {
    sevcli_init( &sts, &pwrrt_scctx);
    if ( EVEN(sts))
      return set_error(sts);
  }

  if ( !PyArg_ParseTuple(args, "ssssIIss|Is", &server, &oidstr, &fromstr, &tostr, &eventtypemask,
			 &eventpriomask, &utf8eventtext, &utf8eventname, &maxrows,
			 &time_format))
    return NULL;

  strncpy(eventtext, cnv_utf8_to_iso8859(utf8eventtext, 
     strlen(utf8eventtext)+1), sizeof(eventtext));
  strncpy(eventname, cnv_utf8_to_iso8859(utf8eventname,
     strlen(utf8eventname)+1), sizeof(eventname));


  if ( time_format) {
    if ( strcmp(time_format, "string") == 0)
      time_string = 1;
  }
  if ( maxrows == 0)
    maxrows = 1000;

  sevcli_set_servernode( &sts, pwrrt_scctx, server);
  if ( EVEN(sts))
    return set_error(sts);

  if ( strncmp("_O", oidstr, 2) == 0) 
    sts = cdh_StringToObjid( oidstr, &oid);
  else
    sts = gdh_NameToObjid( oidstr, &oid);
  if ( EVEN(sts))
    return set_error(sts);

  if ( strcmp(tostr, "now") == 0) {
    /* fromstr is a deltatime */
    sts = time_AsciiToD(fromstr, &fromdelta);
    if ( EVEN(sts))
      return set_error(sts);

    time_GetTime(&to);
    time_Asub(&from, &to, &fromdelta);
  }
  else {
    sts = time_AsciiToA(fromstr, &from);
    if ( EVEN(sts))
      return set_error(sts);

    sts = time_AsciiToA(tostr, &to);
    if ( EVEN(sts))
      return set_error(sts);
  }

  sevcli_get_events( &sts, pwrrt_scctx, oid, from, to, eventtypemask, eventpriomask,
		     eventtext, eventname, maxrows, &list, &listcnt);
  if ( sts == SEV__NOPOINTS)
    Py_RETURN_NONE;
  else if (EVEN(sts))
    return set_error(sts);

  result = PyTuple_New(listcnt);

  for ( i = 0; i < listcnt; i++) {
    item_tuple = PyTuple_New(8);
    eventid_tuple = PyTuple_New(2);
    if ( time_string) {
      /* Time string */
      time_AtoAscii( &list[i].Time, time_eFormat_DateAndTime, timstr, sizeof(timstr));
      PyTuple_SetItem(item_tuple, 0, PyBytes_FromString(timstr));
    }
    else {
      /* Time datetime object */
      sec = (time_t)list[i].Time.tv_sec;
      localtime_r(&sec, &ts);
      date = PyDateTime_FromDateAndTime(ts.tm_year+1900, ts.tm_mon+1, ts.tm_mday,
					ts.tm_hour, ts.tm_min, ts.tm_sec, 
					(int)list[i].Time.tv_nsec/1000);
      PyTuple_SetItem(item_tuple, 0, date);
    }      
    PyTuple_SetItem(item_tuple, 1, PyLong_FromLong((long)list[i].EventType));
    PyTuple_SetItem(item_tuple, 2, PyLong_FromLong((long)list[i].EventPrio));
    PyTuple_SetItem(item_tuple, 3, PyUnicode_FromString(cnv_iso8859_to_utf8(list[i].EventText, strlen(list[i].EventText)+1)));
    PyTuple_SetItem(item_tuple, 4, PyUnicode_FromString(cnv_iso8859_to_utf8(list[i].EventName, strlen(list[i].EventName)+1)));
    strcpy(supobject, cdh_ObjidToString(list[i].SupObjectOid, 1));
    if (list[i].SupObjectOffset > 0)
      sprintf(&supobject[strlen(supobject)], "/%d:%d", list[i].SupObjectOffset, list[i].SupObjectSize);
    PyTuple_SetItem(item_tuple, 5, PyUnicode_FromString(supobject));
    PyTuple_SetItem(eventid_tuple, 0, PyLong_FromLong((long)list[i].EventId.Nix));
    PyTuple_SetItem(eventid_tuple, 1, PyLong_FromLong((long)list[i].EventId.Idx));
    PyTuple_SetItem(item_tuple, 6, eventid_tuple);
    PyTuple_SetItem(item_tuple, 7, PyLong_FromLong((long)list[i].EventStatus));
    
    PyTuple_SetItem(result, i, item_tuple);
  }

  free(list);

  return result;
}

static PyObject *pwrrt_getSevEventsDataFrame(PyObject *self, PyObject *args)
{
  pwr_tStatus sts;
  char *server, *oidstr, *fromstr, *tostr, *eventtext, *eventname;
  unsigned int eventtypemask, eventpriomask;
  pwr_tTime from, to;
  pwr_tDeltaTime fromdelta;
  pwr_tOid oid;
  int maxrows = 0;
  sevcli_sEvents *list;
  unsigned int listcnt;
  PyObject *item_tuple, *eventid_tuple, *result;
  char timstr[30];
  int i;
  PyObject *date;
  time_t sec;
  struct tm ts;
  char *time_format = 0;
  int time_string = 0;
  char supobject[80];

  if ( !pwrrt_scctx) {
    sevcli_init( &sts, &pwrrt_scctx);
    if ( EVEN(sts))
      return set_error(sts);
  }

  if ( !PyArg_ParseTuple(args, "ssssIIss|Is", &server, &oidstr, &fromstr, &tostr, &eventtypemask,
			 &eventpriomask, &eventtext, &eventname, &maxrows,
			 &time_format))
    return NULL;

  if ( time_format) {
    if ( strcmp(time_format, "string") == 0)
      time_string = 1;
  }
  if ( maxrows == 0)
    maxrows = 1000;

  sevcli_set_servernode( &sts, pwrrt_scctx, server);
  if ( EVEN(sts))
    return set_error(sts);

  if ( strncmp("_O", oidstr, 2) == 0) 
    sts = cdh_StringToObjid( oidstr, &oid);
  else
    sts = gdh_NameToObjid( oidstr, &oid);
  if ( EVEN(sts))
    return set_error(sts);

  if ( strcmp(tostr, "now") == 0) {
    /* fromstr is a deltatime */
    sts = time_AsciiToD(fromstr, &fromdelta);
    if ( EVEN(sts))
      return set_error(sts);

    time_GetTime(&to);
    time_Asub(&from, &to, &fromdelta);
  }
  else {
    sts = time_AsciiToA(fromstr, &from);
    if ( EVEN(sts))
      return set_error(sts);

    sts = time_AsciiToA(tostr, &to);
    if ( EVEN(sts))
      return set_error(sts);
  }

  sevcli_get_events( &sts, pwrrt_scctx, oid, from, to, eventtypemask, eventpriomask,
		     eventtext, eventname, maxrows, &list, &listcnt);
  if ( sts == SEV__NOPOINTS)
    Py_RETURN_NONE;
  else if (EVEN(sts))
    return set_error(sts);

  result = PyList_New(listcnt);

  for ( i = 0; i < listcnt; i++) {
    item_tuple = PyTuple_New(8);
    eventid_tuple = PyTuple_New(2);
    if ( time_string) {
      /* Time string */
      time_AtoAscii( &list[i].Time, time_eFormat_DateAndTime, timstr, sizeof(timstr));
      PyTuple_SetItem(item_tuple, 0, PyBytes_FromString(timstr));
    }
    else {
      /* Time datetime object */
      sec = (time_t)list[i].Time.tv_sec;
      localtime_r(&sec, &ts);
      date = PyDateTime_FromDateAndTime(ts.tm_year+1900, ts.tm_mon+1, ts.tm_mday,
					ts.tm_hour, ts.tm_min, ts.tm_sec, 
					(int)list[i].Time.tv_nsec/1000);
      PyTuple_SetItem(item_tuple, 0, date);
    }      
    PyTuple_SetItem(item_tuple, 1, PyLong_FromLong((long)list[i].EventType));
    PyTuple_SetItem(item_tuple, 2, PyLong_FromLong((long)list[i].EventPrio));
    PyTuple_SetItem(item_tuple, 3, PyUnicode_FromString(cnv_iso8859_to_utf8(list[i].EventText, strlen(list[i].EventText)+1)));
    PyTuple_SetItem(item_tuple, 4, PyUnicode_FromString(cnv_iso8859_to_utf8(list[i].EventName, strlen(list[i].EventName)+1)));
    strcpy(supobject, cdh_ObjidToString(list[i].SupObjectOid, 1));
    if (list[i].SupObjectOffset > 0)
      sprintf(&supobject[strlen(supobject)], "#%d:%d", list[i].SupObjectOffset, list[i].SupObjectSize);
    PyTuple_SetItem(item_tuple, 5, PyUnicode_FromString(supobject));
    PyTuple_SetItem(eventid_tuple, 0, PyLong_FromLong((long)list[i].EventId.Nix));
    PyTuple_SetItem(eventid_tuple, 1, PyLong_FromLong((long)list[i].EventId.Idx));
    PyTuple_SetItem(item_tuple, 6, eventid_tuple);
    PyTuple_SetItem(item_tuple, 7, PyLong_FromLong((long)list[i].EventStatus));
    
    PyList_SetItem(result, i, item_tuple);
  }

#if 0
  result = PyList_New(7);
  time_tuple = PyTuple_New(listcnt);
  eventtype_tuple = PyTuple_New(listcnt);
  eventprio_tuple = PyTuple_New(listcnt);
  eventtext_tuple = PyTuple_New(listcnt);
  eventname_tuple = PyTuple_New(listcnt);
  supobject_tuple = PyTuple_New(listcnt);
  eventid_tuple = PyTuple_New(listcnt);

  for ( i = 0; i < listcnt; i++) {
    if ( time_string) {
      /* Time string */
      time_AtoAscii( &list[i].Time, time_eFormat_DateAndTime, timstr, sizeof(timstr));
      PyTuple_SetItem(time_tuple, i, PyBytes_FromString(timstr));
    }
    else {
      /* Time datetime object */
      sec = (time_t)list[i].Time.tv_sec;
      localtime_r(&sec, &ts);
      date = PyDateTime_FromDateAndTime(ts.tm_year+1900, ts.tm_mon+1, ts.tm_mday,
					ts.tm_hour, ts.tm_min, ts.tm_sec, 
					(int)list[i].Time.tv_nsec/1000);
      PyTuple_SetItem(time_tuple, i, date);
    }      
    PyTuple_SetItem(eventtype_tuple, i, PyLong_FromLong((long)list[i].EventType));
    PyTuple_SetItem(eventprio_tuple, i, PyLong_FromLong((long)list[i].EventPrio));
    PyTuple_SetItem(eventtext_tuple, i, PyBytes_FromString(list[i].EventText));
    PyTuple_SetItem(eventname_tuple, i, PyBytes_FromString(list[i].EventName));
    strcpy(supobject, cdh_ObjidToString(list[i].SupObjectOid, 1));
    if (list[i].SupObjectOffset > 0)
      sprintf(&supobject[strlen(supobject)], "#%d:%d", list[i].SupObjectOffset, list[i].SupObjectSize);
    PyTuple_SetItem(supobject_tuple, i, PyBytes_FromString(supobject));
    PyTuple_SetItem(eventid_tuple, i, PyLong_FromLong((long)list[i].EventId.Idx));
    
  }
  PyList_SetItem(result, 0, time_tuple);
  PyList_SetItem(result, 1, eventtype_tuple);
  PyList_SetItem(result, 2, eventprio_tuple);
  PyList_SetItem(result, 3, eventtext_tuple);
  PyList_SetItem(result, 4, eventname_tuple);
  PyList_SetItem(result, 5, supobject_tuple);
  PyList_SetItem(result, 6, eventid_tuple);
#endif

  free(list);

  return result;
}

static PyMethodDef PwrrtMethods[] = {
  {"init", pwrrt_init, METH_VARARGS, pwrrt_init_doc},
  {"volume", pwrrt_volume, METH_VARARGS, pwrrt_volume_doc},
  {"volumes", pwrrt_volumes, METH_NOARGS, pwrrt_volumes_doc},
  {"object", pwrrt_object, METH_VARARGS, pwrrt_object_doc},
  {"attribute", pwrrt_attribute, METH_VARARGS, pwrrt_attribute_doc},
  {"subscribe", pwrrt_subscribe, METH_VARARGS, pwrrt_subscribe_doc},
  {"application", pwrrt_application, METH_VARARGS, pwrrt_application_doc},
  {"login", pwrrt_login, METH_VARARGS, pwrrt_login_doc},
  {"logout", pwrrt_logout, METH_NOARGS, pwrrt_logout_doc},
  {"getPriv", pwrrt_getPriv, METH_NOARGS, pwrrt_getPriv_doc},
  {"getUser", pwrrt_getUser, METH_NOARGS, pwrrt_getUser_doc},
  {"getMsg", pwrrt_getMsg, METH_VARARGS, pwrrt_getMsg_doc},
  {"getSevItemList", pwrrt_getSevItemList, METH_VARARGS, "Get history item list"},
  {"getSevEventsItemList", pwrrt_getSevEventsItemList, METH_VARARGS, "Get events history item list"},
  {"getSevItemData", pwrrt_getSevItemData, METH_VARARGS, "Get history data"},
  {"getSevItemsDataFrame", pwrrt_getSevItemsDataFrame, METH_VARARGS, "Get history data frame"},
  {"getSevItemsDataFrameD", pwrrt_getSevItemsDataFrameD, METH_VARARGS, "Get history data frame discrete"},
  {"getSevEvents", pwrrt_getSevEvents, METH_VARARGS, "Get history events"},
  {"getSevEventsDataFrame", pwrrt_getSevEventsDataFrame, METH_VARARGS, "Get history events data frame"},
  {NULL, NULL, 0, NULL}};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "pwrrt",
    pwrrt_doc,
    -1,
    PwrrtMethods
};

PyMODINIT_FUNC PyInit_pwrrt(void)
{
  PyObject *m;

  if (PyType_Ready(&OidType) < 0 ||
      PyType_Ready(&ArefType) < 0 ||
      PyType_Ready(&CidType) < 0 ||
      PyType_Ready(&TidType) < 0 ||
      PyType_Ready(&ADefType) < 0 ||
      PyType_Ready(&SubType) < 0 ||
      PyType_Ready(&ApplType) < 0)
    return NULL;

  m = PyModule_Create(&moduledef);
  if (m == NULL)
    return m;

  Py_INCREF(&OidType);
  PyModule_AddObject(m, "Oid", (PyObject *)&OidType);
  Py_INCREF(&ArefType);
  PyModule_AddObject(m, "Aref", (PyObject *)&ArefType);
  Py_INCREF(&CidType);
  PyModule_AddObject(m, "Cid", (PyObject *)&CidType);
  Py_INCREF(&TidType);
  PyModule_AddObject(m, "Tid", (PyObject *)&TidType);
  Py_INCREF(&ADefType);
  PyModule_AddObject(m, "ADef", (PyObject *)&ADefType);
  Py_INCREF(&SubType);
  PyModule_AddObject(m, "Sub", (PyObject *)&SubType);
  Py_INCREF(&ApplType);
  PyModule_AddObject(m, "Appl", (PyObject *)&ApplType);

  PyModule_AddIntConstant(m, "FRAME_OPTIONS_CONDITION", 1);

  PyDateTime_IMPORT;

  return m;
}
