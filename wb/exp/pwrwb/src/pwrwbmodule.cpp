#include <Python.h>
#include "pwr.h"
#include "pwr_privilege.h"
#include "pwr_names.h"
#include "pwr_version.h"
#include "co_cdh.h"
#include "co_cnv.h"
#include "co_api_user.h"
#include "co_msg.h"
#include "co_time.h"
#include "wb_ldh.h"
#include "wb_lfu.h"
#include "wb_env.h"
#include "wb_adrep.h"
#include "wb_session.h"
#include "wb_build.h"
#include "wb_nav.h"
#include "wb_ldh_msg.h"
#include "wb_foe_msg.h"
#include "co_user_msg.h"

static unsigned int pwrwb_priv = pwr_mPrv_RtRead;
static char pwrwb_user[80] = "";
wb_session *pwrwb_ses = 0;
wb_env *pwrwb_env = 0;

/**
 * Doc for pwrwb module
 */
PyDoc_STRVAR(pwrwb_doc,"\
ProviewR Workbench Python API\n\n\
The workbench Python API contains a number of classes and functions\n\
to get information about the objects in the ProviewR development database.\n\
There are function to find objects and investigate their relationship\n\
to other objects and to examine the structure and content of the objects.\n\
There are also functions to build and execute commands.\n\n\
The API contains the classes\n\n\
Vid       volume in the development database.\n\
Oid       object in the development database.\n\
Aref      attribute in the development database.\n\
Cid       class.\n\
Tid       type.\n\
ADef      attribute definition.\n\n\
Type help(pwrwb.Oid), help(pwrwb.Aref) etc to get more information about\n\
these classes.");

/**
 * Doc for Vid class
 */
PyDoc_STRVAR(vid_doc,"\
ProviewR development volume\n\n\
The Vid object represents a ProviewR volume.\n\
");

PyDoc_STRVAR(vid_name_doc,"\
name()\n--\n\n\
Get the name of the volume.\n\
Returns volume name.\n\n\
Returns\n\
-------\n\
Returns a string with the volume name.\n\n\
Example\n\
-------\n\
v = pwrwb.volume('VolDemo')\n\
print v.name()\n\
");

PyDoc_STRVAR(vid_fullName_doc,"\
fullName()\n--\n\n\
Same as name().\n\
");

PyDoc_STRVAR(vid_next_doc,"\
next()\n--\n\n\
Get the next volume.\n\n\
Returns\n\
-------\n\
Returns an vid object for the volume. If no next volume exist, a\n\
None object is returned.\n\n\
Example\n\
-------\n\
v = pwrwb.volume('VolDemo')\n\
while (v):\n\
  print v.name()\n\
  v = v.next()\n\
");

PyDoc_STRVAR(vid_root_doc,"\
root()\n--\n\n\
Get first root object of the volume.\n\n\
Returns\n\
-------\n\
Returns an Oid object for the root object.\n\n\
Example\n\
-------\n\
v = pwrwb.object('VolDemo')\n\
o = v.child()\n\
while o:\n\
  print o.name()\n\
  o = o.next()\n\
");

PyDoc_STRVAR(vid_roots_doc,"\
roots()\n--\n\n\
Get all root objects of the volume.\n\n\
Returns\n\
-------\n\
Returns a tuple containg Oid objects for all root objects of the volume.\n\n\
Example\n\
-------\n\
v = pwrwb.volume('VolDemo')\n\
for o in v.children()\n\
  print o.name()\n\
");

PyDoc_STRVAR(vid_cid_doc,"\
cid()\n--\n\n\
Get volume class.\n\n\
Returns\n\
-------\n\
Returns a Cid object for the class.\n\n\
Example\n\
-------\n\
v = pwrwb.volume('VolDemo')\n\
c = v.cid()\n\
print c.fullName()\n\
");

PyDoc_STRVAR(vid_vidStr_doc,"\
vid()\n--\n\n\
Get volume identity as a string.\n\n\
Returns\n\
-------\n\
Returns a string with the volume identity.\n\n\
Example\n\
-------\n\
v = pwrwb.volume('VolDemo')\n\
print v.vidStr()\n\
'_V0.1.200.3'\n\
");

/**
 * Doc for Oid class
 */
PyDoc_STRVAR(oid_doc,"\
ProviewR development object\n\n\
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
o = pwrwb.object('H1-H2')\n\
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
o = pwrwb.object('H1-H2')\n\
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
o = pwrwb.object('H1-H2')\n\
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
o = pwrwb.object('H1-H2')\n\
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
o = pwrwb.object('H1-H2')\n\
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
o = pwrwb.object('H1-H2')\n\
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
o = pwrwb.object('H1-Dv1')\n\
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
o = pwrwb.object('H1-Dv1')\n\
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
o = pwrwb.object('H1-Dv1')\n\
a = o.attribute('ActualValue')\n\
print a.value()\n");

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
a = pwrwb.attribute('H1-H2-Dv1.ActualValue')\n\
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
a = pwrwb.attribute('H1-H2-Dv1.ActualValue')\n\
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
o = pwrwb.attribute('H1-Dv1.ActualValue')\n\
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
a = pwrwb.attribute('H1-Dv1.ActualValue')\n\
t = o.tid()\n\
print t.fullName()\n\
");

PyDoc_STRVAR(aref_value_doc,"\
value()\n--\n\n\
Get the value of an attribute.\n\n\
Returns\n\
-------\n\
Returns the value of the attribute.\n\
Return types for different attributes types are\n\
pwr_tEnum      Integer.\n\
pwr_tMask      Integer.\n\
pwr_tStatus    Integer.\n\
pwr_tObjid     OidObject.\n\
pwr_tAttrRef   ArefObject.\n\
pwr_tTime      A string with format '17-JAN-2019 10:51:10.58'. Can be\n\
               converted from a datetime with format\n\
               '%d-%b-%Y %H:%M:%S.%f'.\n\n\
Example\n\
-------\n\
a = pwrwb.object('H1-Dv1.ActualValue')\n\
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
  The value types for different attributes types should be\n\
  pwr_tEnum      Integer.\n\
  pwr_tMask      Integer.\n\
  pwr_tStatus    Integer.\n\
  pwr_tObjid     OidObject.\n\
  pwr_tAttrRef   ArefObject.\n\
  pwr_tTime      A string with format '17-JAN-2019 10:51:10.58'. Can be\n\
                 converted from a datetime with format\n\
                 '%d-%b-%Y %H:%M:%S.%f'.\n\n\
publicwrite   Optional Integer\n\
  1 if value is set to an attribute with public write permissions.\n\n\
Example\n\
-------\n\
pwrwb.login('pwrp', 'somepassword')\n\
a = pwrwb.attribute('H1-H2-Dv1.ActualValue')\n\
a.setValue(1)\n\
");

PyDoc_STRVAR(cid_name_doc,"\
name()\n--\n\n\
Get the name of the class.\n\n\
Returns\n\
-------\n\
Returns a string with the class name.\n\n\
Example\n\
-------\n\
c = pwrwb.Cid('$PlantHier')\n\
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
c = pwrwb.Cid('$PlantHier')\n\
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
cid = pwrwb.Cid('$PlantHier')\n\
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
cid = pwrwb.Cid('$PlantHier')\n\
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
cid = pwrwb.Cid('$PlantHier')\n\
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
cid = pwrwb.Cid('Di')\n\
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
cid = pwrwb.Cid('Di')\n\
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
cid = pwrwb.Cid('Di')\n\
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
cid = pwrwb.Cid('$PlantHier')\n\
for adef in cid.attributes():\n\
  print adef.name(), adef.offset(), adef.size()\n\
");

PyDoc_STRVAR(tid_name_doc,"\
name()\n--\n\n\
Get the name of the type.\n\n\
Returns\n\
-------\n\
Returns a string with the type name.\n\n\
Example\n\
-------\n\
a = pwrwb.attribute('H1-H2-Dv1.ActualValue')\n\
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
a = pwrwb.attribute('H1-H2-Dv1.ActualValue')\n\
tid = a.tid()\n\
print tid.fullName()\n\
'pwrs:Type-$Boolean'\n\
");

PyDoc_STRVAR(adef_name_doc,"\
name()\n--\n\n\
Get the name of the attribute.\n\n\
Returns\n\
-------\n\
Returns a string with the attribute name.\n\n\
Example\n\
-------\n\
c = pwrwb.Cid('$PlantHier')\n\
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
 * Doc for static function
 */

PyDoc_STRVAR(pwrwb_version_doc,"\
version()\n--\n\n\
Get version\n\n\
Returns\n\
-------\n\
The current ProviewR version.\n\n\
Example\n\
-------\n\
version = pwrwb.version()\n\
print version\n\
'V5.6.0'\n\
");

PyDoc_STRVAR(pwrwb_open_doc,"\
open(database)\n--\n\n\
Open database and open session to root volume in the database.\n\
If no database argument is supplied, the directory volume is opened.\n\n\
Arguments\n\
---------\n\
database Optional String\n\
  Name of the database. If not supplied the directory volume is opened.\n\n\
Example\n\
-------\n\
pwrwb.open('VolDemo')\n\
");

PyDoc_STRVAR(pwrwb_openWb_doc,"\
openWb(database)\n--\n\n\
Open database.\n\
If no database argument is supplied, the directory volume is opened.\n\n\
Arguments\n\
---------\n\
database Optional String\n\
  Name of the database. If not supplied the directory volume is opened.\n\n\
Example\n\
-------\n\
pwrwb.openWb('VolDemo')\n\
");

PyDoc_STRVAR(pwrwb_openSession_doc,"\
openSession(volume, access)\n--\n\n\
Open session.\n\
If no database argument is supplied, the directory volume is opened.\n\n\
Arguments\n\
---------\n\
volume VidObject\n\
  A VidObject for the volume to open a session to.\n\
access Optional String\n\
  'w' for write and 'r' for read access. Default read access.\n\n\
Example\n\
-------\n\
v = pwrwb.volume('VolDemo')\n\
pwrwb.openSession(v, 'w')\n\
");

PyDoc_STRVAR(pwrwb_close_doc,"\
close()\n--\n\n\
Close session and database.\n\n\
Example\n\
-------\n\
pwrwb.close()\n\
");

PyDoc_STRVAR(pwrwb_closeWb_doc,"\
closeWb()\n--\n\n\
Close database.\n\n\
Example\n\
-------\n\
pwrwb.closeWb()\n\
");

PyDoc_STRVAR(pwrwb_closeSession_doc,"\
closeSession()\n--\n\n\
Close the current session.\n\n\
Example\n\
-------\n\
pwrwb.closeSession()\n\
");

PyDoc_STRVAR(pwrwb_saveSession_doc,"\
saveSession()\n--\n\n\
Save the current session.\n\n\
Example\n\
-------\n\
pwrwb.saveSession()\n\
");

PyDoc_STRVAR(pwrwb_revertSession_doc,"\
revertSession()\n--\n\n\
Revert the current session.\n\n\
Example\n\
-------\n\
pwrwb.revertSession()\n\
");

PyDoc_STRVAR(pwrwb_getSessionVolume_doc,"\
getSessionVolume()\n--\n\n\
Get the volume of the current session.\n\n\
Returns\n\
-------\n\
Returns a Vid object for the volume.\n\n\
Example\n\
-------\n\
v = pwrwb.getSessionVolume()\n\
print v\n\
'VolDemo'\n\
");

PyDoc_STRVAR(pwrwb_sessionIsEmpty_doc,"\
sessionIsEmpty()\n--\n\n\
Returns 1 if no modifications is done in this session since last save,\n\
else 0.\n\n\
Returns\n\
-------\n\
Returns 1 if session is empty, else 0.\n\
");

PyDoc_STRVAR(pwrwb_volume_doc,"\
volume(volumeName)\n--\n\n\
Get volume\n\n\
Arguments\n\
---------\n\
volumeName Optional String\n\
  Name of the volume. If not supplied the first volume is returned.\n\n\
Returns\n\
-------\n\
Returns an Vid object for the volume object.\n\n\
Example\n\
-------\n\
vol = pwrwb.volume('VolPwrDemo')\n\
for o in vol.roots():\n\
  print o.fullName()\n");

PyDoc_STRVAR(pwrwb_volumes_doc,"\
volumes()\n\
--\n\n\
Get all volumes loaded into the database.\n\n\
Returns\n\
-------\n\
Returns a tuple with Vid objects for all the volumes.\n\n\
Example\n\
-------\n\
for v in pwrwb.volumes():\n\
  print v.name()\n");

PyDoc_STRVAR(pwrwb_object_doc,"\
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
o = pwrwb.object('Demo-Ge-Dynamics')\n\
print o.fullName()\n\
'VolDemo:Demo-Ge-Dynamics'\n");

PyDoc_STRVAR(pwrwb_attribute_doc,"\
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
a = pwrwb.attribute('H1-H2-Dv1.ActualValue')\n\
print a.fullName()\n\
'VolDemo:H1-H2-Dv1.ActualValue'\n");

PyDoc_STRVAR(pwrwb_login_doc,"\
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
pwrwb.login('pwrp', 'mypassword')\n");

PyDoc_STRVAR(pwrwb_logout_doc, "\
logout()\n\
--\n\
\n\
Logout from the previously logged in user.\n\
The privileges will return to RtRead.\n\n\
Example\n\
-------\n\
pwrwb.logout()\n");

PyDoc_STRVAR(pwrwb_getPriv_doc, "\
getPriv()\n\
--\n\
\n\
Returns the current privileges\n\n\
Returns\n\
-------\n\
Returns an integer with the current privileges.\n\n\
Example\n\
-------\n\
priv = pwrwb.getPriv()\n");

PyDoc_STRVAR(pwrwb_getUser_doc, "\
getUser()\n\
--\n\
\n\
Returns the current user\n\n\
Returns\n\
-------\n\
Returns a string with the user.\n\n\
Example\n\
-------\n\
user = pwrwb.getUser()\n");

PyDoc_STRVAR(pwrwb_build_doc, "\
build(nodename)\n\
--\n\
\n\
Build a node.\n\n\
Arguments\n\
---------\n\
nodename String\n\
  Name of the node to build.\n\n\
Example\n\
-------\n\
pwrwb.build('pwrdemo')\n");

PyDoc_STRVAR(pwrwb_command_doc, "\
command(cmd)\n\
--\n\
\n\
Execute a wb command.\n\n\
Arguments\n\
---------\n\
cmd String\n\
  Command to execute.\n\n\
Example\n\
-------\n\
pwrwb.command('compile /all')\n");


typedef struct {
  PyObject_HEAD
  pwr_tVid vid;
} VidObject;

static PyObject *Vid_str(PyObject *self);
static PyObject *Vid_richcompare(PyObject *self, PyObject *other, int op);
static int Vid_init(PyObject *self, PyObject *args, PyObject *kwds);
static PyObject *Vid_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

static PyObject *Vid_next(PyObject *self, PyObject *args);
static PyObject *Vid_root(PyObject *self, PyObject *args);
static PyObject *Vid_roots(PyObject *self, PyObject *args);
static PyObject *Vid_name(PyObject *self, PyObject *args);
static PyObject *Vid_fullName(PyObject *self, PyObject *args);
static PyObject *Vid_vidStr(PyObject *self, PyObject *args);
static PyObject *Vid_cid(PyObject *self, PyObject *args);

static PyMethodDef Vid_methods[] = {
    { "next", (PyCFunction) Vid_next, METH_NOARGS, vid_next_doc },
    { "root", (PyCFunction) Vid_root, METH_NOARGS, vid_root_doc },
    { "roots", (PyCFunction) Vid_roots, METH_NOARGS, vid_roots_doc },
    { "name", (PyCFunction) Vid_name, METH_VARARGS, vid_name_doc },
    { "fullName", (PyCFunction) Vid_fullName, METH_VARARGS, vid_fullName_doc },
    { "vidStr", (PyCFunction) Vid_vidStr, METH_NOARGS, vid_vidStr_doc },
    { "cid", (PyCFunction) Vid_cid, METH_NOARGS, vid_cid_doc },
    { NULL }
};

static PyTypeObject VidType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Vid",         	       /* tp_name */
    sizeof(VidObject),         /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    Vid_str,                   /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    Vid_str,                   /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    vid_doc,  		       /* tp_doc */    
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    Vid_richcompare,           /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Vid_methods,               /* tp_methods */
    0,               	       /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Vid_init,        /* tp_init */
    0,                         /* tp_alloc */
    Vid_new,                   /* tp_new */
};

typedef struct {
  PyObject_HEAD
  wb_orep *orep;
  wb_object o;
} OidObject;

static PyObject *Oid_str(PyObject *self);
static void Oid_dealloc(PyObject *self);
static PyObject *Oid_richcompare(PyObject *self, PyObject *other, int op);
static int Oid_init(PyObject *self, PyObject *args, PyObject *kwds);
static PyObject *Oid_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

static PyObject *Oid_next(PyObject *self, PyObject *args);
static PyObject *Oid_parent(PyObject *self, PyObject *args);
static PyObject *Oid_child(PyObject *self, PyObject *args);
static PyObject *Oid_children(PyObject *self, PyObject *args);
static PyObject *Oid_name(PyObject *self, PyObject *args);
static PyObject *Oid_fullName(PyObject *self, PyObject *args);
static PyObject *Oid_oidStr(PyObject *self, PyObject *args);
static PyObject *Oid_cid(PyObject *self, PyObject *args);
static PyObject *Oid_attribute(PyObject *self, PyObject *args);

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
    Oid_dealloc,               /* tp_dealloc */
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
  wb_attribute attr;
} ArefObject;

static PyObject *Aref_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static PyObject *Aref_str(PyObject *self);
static PyObject *Aref_richcompare(PyObject *self, PyObject *other, int op);
static int Aref_init(PyObject *self, PyObject *args, PyObject *kwds);
static void Aref_dealloc(PyObject *self);

static PyObject *Aref_name(PyObject *s, PyObject *args);
static PyObject *Aref_fullName(PyObject *s, PyObject *args);
static PyObject *Aref_arefStr(PyObject *self, PyObject *args);
static PyObject *Aref_tid(PyObject *s, PyObject *args);
static PyObject *Aref_value(PyObject *s, PyObject *args);
static PyObject *Aref_setValue(PyObject *s, PyObject *args);

static PyMethodDef Aref_methods[] = {
    { "name", (PyCFunction) Aref_name, METH_VARARGS, aref_name_doc },
    { "fullName", (PyCFunction) Aref_fullName, METH_VARARGS, aref_fullName_doc },
    { "arefStr", (PyCFunction) Aref_arefStr, METH_VARARGS, aref_arefStr_doc },
    { "tid", (PyCFunction) Aref_tid, METH_NOARGS, aref_tid_doc },    
    { "value", (PyCFunction) Aref_value, METH_NOARGS, aref_value_doc },
    { "setValue", (PyCFunction) Aref_setValue, METH_VARARGS, aref_setValue_doc },
    { NULL }
};

static PyTypeObject ArefType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Aref",                    /* tp_name */
    sizeof(ArefObject),        /* tp_basicsize */
    0,                         /* tp_itemsize */
    Aref_dealloc,              /* tp_dealloc */
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
    "Proview development attribute",  /* tp_doc */    
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
static int Cid_init(PyObject *s, PyObject *args, PyObject *kwds);

static PyObject *Cid_name(PyObject *s, PyObject *args);
static PyObject *Cid_fullName(PyObject *s, PyObject *args);
static PyObject *Cid_object(PyObject *s, PyObject *args);
static PyObject *Cid_nextObject(PyObject *s, PyObject *args);
static PyObject *Cid_objects(PyObject *s);
static PyObject *Cid_attrObject(PyObject *s, PyObject *args);
static PyObject *Cid_nextAttrObject(PyObject *s, PyObject *args);
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
    "Class of a development object", /* tp_doc */    
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
static int Tid_init(PyObject *self, PyObject *args, PyObject *kwds);

static PyObject *Tid_name(PyObject *s, PyObject *args);
static PyObject *Tid_fullName(PyObject *s, PyObject *args);

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
    "Attribute type",          /* tp_doc */    
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
static int ADef_init(PyObject *self, PyObject *args, PyObject *kwds);

static PyObject *ADef_name(PyObject *s, PyObject *args);
static PyObject *ADef_cid(PyObject *s, PyObject *args);
static PyObject *ADef_offset(PyObject *s, PyObject *args);
static PyObject *ADef_size(PyObject *s, PyObject *args);
static PyObject *ADef_flags(PyObject *s, PyObject *args);
static PyObject *ADef_elements(PyObject *s, PyObject *args);
static PyObject *ADef_typeref(PyObject *s, PyObject *args);

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


static PyObject *set_error(pwr_tStatus sts) 
{
  char msg[120];
  msg_GetMsg(sts, msg, sizeof(msg));
  PyErr_SetString(PyExc_RuntimeError, msg);
  return NULL;
}

static int is_authorized(unsigned int access)
{
  return (pwrwb_priv & access) != 0;
}

/* 
 * Vid object functions
 */
static PyObject *
Vid_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  VidObject *self;

  self = (VidObject *)type->tp_alloc(type, 0);
  if (self != NULL) {
    self->vid = 0;
  }

  return (PyObject *)self;
}

static PyObject *
Vid_str(PyObject *self)
{
  wb_volume v = pwrwb_env->volume(((VidObject *)self)->vid);
  return PyUnicode_FromFormat("%s", v.name());  
}

static PyObject *
Vid_richcompare(PyObject *self, PyObject *other, int op)
{
  PyObject *result = NULL;

  if ( Py_TYPE(other) != &VidType) {
    result = Py_NotImplemented;
  }
  else {
    pwr_tVid self_vid = ((VidObject *)self)->vid;
    pwr_tVid other_vid = ((VidObject *)other)->vid;

    switch ( op) {
    case Py_LT:
    case Py_LE:
    case Py_GT:
    case Py_GE:
      result = Py_NotImplemented;
      break;
    case Py_EQ:
      if ( self_vid == other_vid)
	result = Py_True;
      else
	result = Py_False;
      break;
    case Py_NE:
      if ( self_vid != other_vid)
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
Vid_init(PyObject *s, PyObject *args, PyObject *kwds)
{
  char *name, *utf8name = 0;
  VidObject *self = (VidObject *)s;

  if (! PyArg_ParseTuple(args, "|s", &utf8name))
    return -1;


  wb_volume v;
  if (utf8name) {
    name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);
    v = pwrwb_env->volume(name);
  }
  else
    v = pwrwb_env->volume();
  if (!v) {
    set_error(v.sts());
    return -1;
  }

  self->vid = v.vid();
  return 0;
}

static PyObject *
Vid_name(PyObject *s, PyObject *args)
{
  wb_volume v = pwrwb_env->volume(((VidObject *)s)->vid);
  char *utf8name = cnv_iso8859_to_utf8(v.name(), strlen(v.name())+1);
  return Py_BuildValue("s", utf8name);  
}

static PyObject *
Vid_fullName(PyObject *s, PyObject *args)
{
  wb_volume v = pwrwb_env->volume(((VidObject *)s)->vid);
  char *utf8name = cnv_iso8859_to_utf8(v.name(), strlen(v.name())+1);
  return Py_BuildValue("s", utf8name);  
}

static PyObject *
Vid_next(PyObject *s, PyObject *args)
{
  wb_volume v = pwrwb_env->volume(((VidObject *)s)->vid);
  wb_volume vnext = v.next();
  if ( !vnext)
    Py_RETURN_NONE;

  VidObject *next = (VidObject *)Vid_new(&VidType, 0, 0);
  if (next != NULL) {
    next->vid = vnext.vid();
  }       
  return (PyObject *)next;
}

static PyObject *
Vid_root(PyObject *s, PyObject *args)
{
  OidObject *child;
  wb_volume v = pwrwb_env->volume(((VidObject *)s)->vid);
  wb_object co = v.object();
  if ( !co)
    Py_RETURN_NONE;

  child = (OidObject *)Oid_new(&OidType, 0, 0);
  if (child != NULL)
    child->o = co;

  return (PyObject *)child;
}

static PyObject *
Vid_roots(PyObject *s, PyObject *args)
{
  OidObject *child;
  PyObject *result;
  int cnt = 0;

  wb_volume v = pwrwb_env->volume(((VidObject *)s)->vid);
  wb_object o = v.object();
  while( o) {
    cnt++;
    o = o.after();
  }  
  result = PyTuple_New(cnt);
  cnt = 0;
  for ( o = v.object(); o; o = o.after()) {
    child = (OidObject *)Oid_new(&OidType, 0, 0);
    if (child != NULL) {
      child->o = o;
      PyTuple_SetItem(result, cnt, (PyObject *)child);
      cnt++;
    }    
  }
  return result;
}

static PyObject *
Vid_cid(PyObject *s, PyObject *args)
{
  CidObject *cid_object;

  wb_volume v = pwrwb_env->volume(((VidObject *)s)->vid);
  cid_object = (CidObject *)Cid_new(&CidType, 0, 0);
  if (cid_object != NULL)
    cid_object->cid = v.cid();

  return (PyObject *)cid_object;
}

static PyObject *
Vid_vidStr(PyObject *s, PyObject *args)
{
  char str[20];

  sprintf(str, "_V%hhu.%hhu.%hhu.%hhu", 
	  0xFF & (((VidObject *)s)->vid >> 24),
	  0xFF & (((VidObject *)s)->vid >> 16),
	  0xFF & (((VidObject *)s)->vid >> 8),
	  0xFF & (((VidObject *)s)->vid));
  return Py_BuildValue("s", str);  
}


/* 
 * Oid object functions
 */
static PyObject *
Oid_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  OidObject *self;

  self = (OidObject *)type->tp_alloc(type, 0);

  return (PyObject *)self;
}

static void
Oid_dealloc(PyObject *self)
{
  if ( ((OidObject *)self)->o.m_orep)
    ((OidObject *)self)->o.m_orep->unref();
  PyObject_Del(self);
}

static PyObject *
Oid_str(PyObject *self)
{
  pwr_tOName name;

  strcpy(name, ((OidObject *)self)->o.longName().name(cdh_mName_path | cdh_mName_object));
  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return PyUnicode_FromFormat("%s", utf8name);  
}

static PyObject *
Oid_richcompare(PyObject *self, PyObject *other, int op)
{
  PyObject *result = NULL;

  if ( Py_TYPE(other) != &OidType) {
    result = Py_NotImplemented;
  }
  else {
    pwr_tOid self_oid = ((OidObject *)self)->o.oid();
    pwr_tOid other_oid = ((OidObject *)other)->o.oid();

    switch ( op) {
    case Py_LT:
    case Py_LE:
    case Py_GT:
    case Py_GE:
      result = Py_NotImplemented;
      break;
    case Py_EQ:
      if ( self_oid.vid == other_oid.vid && self_oid.oix == other_oid.oix)
	result = Py_True;
      else
	result = Py_False;
      break;
    case Py_NE:
      if ( !(self_oid.vid == other_oid.vid && self_oid.oix == other_oid.oix))
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
Oid_init(PyObject *s, PyObject *args, PyObject *kwds)
{
  char *name, *utf8name = 0;
  OidObject *self = (OidObject *)s;

  if (!pwrwb_ses) {
    set_error(LDH__NOSUCHSESS);
    return -1;
  }

  if (! PyArg_ParseTuple(args, "|s", &utf8name))
    return -1;

  if (utf8name) {
    name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);
    self->o = pwrwb_ses->object(name);
  }
  else
    self->o = pwrwb_ses->object();
  if (!self->o) {
    set_error(self->o.sts());
    return -1;
  }

  return 0;
}

static PyObject *
Oid_name(PyObject *s, PyObject *args)
{
  if (!((OidObject *)s)->o)
    return set_error(((OidObject *)s)->o.sts());

  char *utf8name = cnv_iso8859_to_utf8(((OidObject *)s)->o.name(), strlen(((OidObject *)s)->o.name())+1);
  return Py_BuildValue("s", utf8name);  
}

static PyObject *
Oid_fullName(PyObject *s, PyObject *args)
{
  pwr_tOName name;

  if (!((OidObject *)s)->o)
    return set_error(((OidObject *)s)->o.sts());

  strcpy(name, ((OidObject *)s)->o.longName().name(cdh_mName_volumeStrict));
  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);  
}

static PyObject *
Oid_oidStr(PyObject *s, PyObject *args)
{
  if (!((OidObject *)s)->o)
    return set_error(((OidObject *)s)->o.sts());

  char str[30];
  const char *name = 0;
  pwr_tOid oid = ((OidObject *)s)->o.oid();

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
    return set_error(LDH__SYNTAX);

  return Py_BuildValue("s", str);  
}

static PyObject *
Oid_next(PyObject *s, PyObject *args)
{
  if (!((OidObject *)s)->o)
    return set_error(((OidObject *)s)->o.sts());

  OidObject *next;
  wb_object next_o = ((OidObject *)s)->o.after();
  if ( !next_o)
    Py_RETURN_NONE;

  next = (OidObject *)Oid_new(&OidType, 0, 0);
  if (next != NULL) {
    next->o = next_o;
  }       
  return (PyObject *)next;
}

static PyObject *
Oid_parent(PyObject *s, PyObject *args)
{
  if (!((OidObject *)s)->o)
    return set_error(((OidObject *)s)->o.sts());

  OidObject *parent;
  wb_object parent_o = ((OidObject *)s)->o.parent();
  if ( !parent_o)
    Py_RETURN_NONE;

  parent = (OidObject *)Oid_new(&OidType, 0, 0);
  if (parent != NULL) {
    parent->o = parent_o;
  }       
  return (PyObject *)parent;
}

static PyObject *
Oid_child(PyObject *s, PyObject *args)
{
  if (!((OidObject *)s)->o)
    return set_error(((OidObject *)s)->o.sts());

  OidObject *child;
  wb_object child_o = ((OidObject *)s)->o.first();
  if ( !child_o)
    Py_RETURN_NONE;

  child = (OidObject *)Oid_new(&OidType, 0, 0);
  if (child != NULL)
    child->o = child_o;

  return (PyObject *)child;
}

static PyObject *
Oid_children(PyObject *s, PyObject *args)
{
  if (!((OidObject *)s)->o)
    return set_error(((OidObject *)s)->o.sts());

  OidObject *child;
  wb_object child_o;
  PyObject *result;
  int cnt = 0;

  for ( child_o = ((OidObject *)s)->o.first();
	child_o;
	child_o = child_o.after())
    cnt++;

  result = PyTuple_New(cnt);
  cnt = 0;
  for ( child_o = ((OidObject *)s)->o.first();
	child_o;
	child_o = child_o.after()) {
    child = (OidObject *)Oid_new(&OidType, 0, 0);
    if (child != NULL) {
      child->o = child_o;
      PyTuple_SetItem(result, cnt, (PyObject *)child);
      cnt++;
    }    
  }
  return result;
}

static PyObject *
Oid_cid(PyObject *s, PyObject *args)
{
  if (!((OidObject *)s)->o)
    return set_error(((OidObject *)s)->o.sts());

  pwr_tCid cid;
  CidObject *cid_object;

  cid = ((OidObject *)s)->o.cid();
  cid_object = (CidObject *)Cid_new(&CidType, 0, 0);
  if (cid_object != NULL)
    cid_object->cid = cid;

  return (PyObject *)cid_object;
}

static PyObject *
Oid_attribute(PyObject *s, PyObject *args)
{
  if (!((OidObject *)s)->o)
    return set_error(((OidObject *)s)->o.sts());

  OidObject *self = (OidObject *)s;
  char *name, *utf8name;
  ArefObject *aref_object;

  if (! PyArg_ParseTuple(args, "s", &utf8name))
    return NULL;

  name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);

  wb_attrname aname = wb_attrname(name);
  wb_attribute a = pwrwb_ses->attribute(self->o, aname);
  if (!a)
    return set_error(a.sts());

  aref_object = (ArefObject *)Aref_new(&ArefType, 0, 0);
  if (aref_object != NULL)
    aref_object->attr = a;

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

  return (PyObject *)self;
}

static void
Aref_dealloc(PyObject *self)
{
  wb_orep *orep = ((ArefObject *)self)->attr;
  if ( orep)
    orep->unref();
  if ( ((ArefObject *)self)->attr.adrep())
    ((ArefObject *)self)->attr.adrep()->unref();

  PyObject_Del(self);
}

static PyObject *
Aref_str(PyObject *self)
{
  pwr_tAName name;

  if (!((ArefObject *)self)->attr)
    return set_error(((ArefObject *)self)->attr.sts());

  strcpy(name, ((ArefObject *)self)->attr.longName().name(cdh_mName_volumeStrict));
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
      if ( ((ArefObject *)self)->attr == ((ArefObject *)other)->attr)
	result = Py_True;
      else
	result = Py_False;
      break;
    case Py_NE:
      if ( !(((ArefObject *)self)->attr == ((ArefObject *)other)->attr))
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
Aref_init(PyObject *s, PyObject *args, PyObject *kwds)
{
  char *name, *utf8name = 0;

  if (!pwrwb_ses) {
    set_error(LDH__NOSUCHSESS);
    return -1;
  }

  if (! PyArg_ParseTuple(args, "|s", &utf8name))
    return -1;

  if (utf8name) {
    name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);
    wb_name wname(name);
    ((ArefObject *)s)->attr = pwrwb_ses->attribute(wname);
  }
  return 0;
}

static PyObject *
Aref_name(PyObject *s, PyObject *args)
{
  pwr_tAName name;

  if (!((ArefObject *)s)->attr)
    return set_error(((ArefObject *)s)->attr.sts());

  strcpy(name, ((ArefObject *)s)->attr.longName().name(cdh_mName_object | cdh_mName_attribute));
  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);  
}

static PyObject *
Aref_fullName(PyObject *s, PyObject *args)
{
  pwr_tAName name;

  if (!((ArefObject *)s)->attr)
    return set_error(((ArefObject *)s)->attr.sts());

  strcpy(name, ((ArefObject *)s)->attr.longName().name(cdh_mName_volumeStrict));
  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);  
}

static PyObject *
Aref_arefStr(PyObject *s, PyObject *args)
{
  if (!((ArefObject *)s)->attr)
    return set_error(((ArefObject *)s)->attr.sts());

  pwr_tAName name;
  pwr_tAttrRef aref = ((ArefObject *)s)->attr.aref();

  cdh_ArefToString( name, sizeof(name), &aref, 1);

  return Py_BuildValue("s", name);
}

static PyObject *
Aref_tid(PyObject *s, PyObject *args)
{
  if (!((ArefObject *)s)->attr)
    return set_error(((ArefObject *)s)->attr.sts());

  pwr_tTid tid;
  TidObject *tid_object;

  tid = ((ArefObject *)s)->attr.tid();
  tid_object = (TidObject *)Tid_new(&TidType, 0, 0);
  if (tid_object != NULL)
    tid_object->tid = tid;

  return (PyObject *)tid_object;
}

static PyObject *
Aref_value(PyObject *s, PyObject *args)
{
  if (!((ArefObject *)s)->attr)
    return set_error(((ArefObject *)s)->attr.sts());

  ArefObject *self = (ArefObject *)s;
  char *buf;
  pwr_eType atype;

  buf = (char *)malloc(self->attr.size());

  self->attr.value(buf);
  atype = self->attr.type();

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
  case pwr_eType_Status:
  case pwr_eType_NetStatus:
  case pwr_eType_Mask: {
    pwr_tUInt32 value = *(pwr_tUInt32 *)buf;
    free(buf);
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
  case pwr_eType_String: {
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
    wb_object o = pwrwb_ses->object(value);
    if ( !o)
      Py_RETURN_NONE;

    OidObject *oido = (OidObject *)Oid_new(&OidType, 0, 0);
    if ( oido != NULL)
      oido->o = o;

    return (PyObject *)oido;
  }
  case pwr_eType_AttrRef: {    
    pwr_tAttrRef value = *(pwr_tAttrRef *)buf;
    free(buf);
    wb_attribute a = pwrwb_ses->attribute(&value);
    if ( !a)
      Py_RETURN_NONE;

    ArefObject *arefo = (ArefObject *)Aref_new(&ArefType, 0, 0);
    if ( arefo != NULL)
      arefo->attr = a;

    return (PyObject *)arefo;
  }
  default:
    free(buf);
    return NULL;
  }
}

static PyObject *
Aref_setValue(PyObject *s, PyObject *args)
{
  if (!((ArefObject *)s)->attr)
    return set_error(((ArefObject *)s)->attr.sts());

  ArefObject *self = (ArefObject *)s;
  char *buf;
  pwr_eType atype;

  if ( !is_authorized(pwr_mAccess_RtWrite | pwr_mAccess_System))
    return set_error(USER__NOTAUTHORIZED);

  atype = self->attr.type();
  buf = (char *)malloc(self->attr.size());

  switch ( atype) {
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
    strncpy( buf, value, self->attr.size());
    buf[self->attr.size()-1] = 0;
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

    oid = o->o.oid();
    memcpy(buf, &oid, sizeof(oid));
    break;
  }
  case pwr_eType_AttrRef: {
    ArefObject *a;
    pwr_tAttrRef aref;

    if ( !PyArg_ParseTuple(args, "O", &a))
      goto error_return;

    aref = a->attr.aref();
    memcpy(buf, &aref, sizeof(aref));
    break;
  }
  default: ;
  }

  pwrwb_ses->writeAttribute(self->attr, buf, self->attr.size());
  free(buf);
  if (pwrwb_ses->evenSts())
    return set_error(pwrwb_ses->sts());

  Py_RETURN_NONE;

error_return:
  free(buf);
  return NULL;    
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
  pwr_tOName name;

  if (!pwrwb_ses)
    return set_error(LDH__NOSUCHSESS);

  wb_cdef cdef = pwrwb_ses->cdef(((CidObject *)self)->cid);
  if (!cdef)
    return set_error(cdef.sts());

  strcpy(name, cdef.longName().name(cdh_mName_volumeStrict));
  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return PyUnicode_FromFormat("%s", utf8name);
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
Cid_init(PyObject *s, PyObject *args, PyObject *kwds)
{
  CidObject *self = (CidObject *)s;
  char *name, *utf8name = 0;

  if (! PyArg_ParseTuple(args, "|s", &utf8name))
    return -1;

  if (utf8name) {
    name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);
    wb_name n = wb_name(name);
    wb_cdef cdef = pwrwb_ses->cdef(n);
    if (!cdef) {
      set_error(cdef.sts());
      return -1;
    }
    self->cid = cdef.cid();
  }
  else
    self->cid = 0;

  return 0;
}

static PyObject *
Cid_name(PyObject *s, PyObject *args)
{
  CidObject *self = (CidObject *)s;

  wb_cdef cdef = pwrwb_ses->cdef(self->cid);
  if (!cdef)
    return set_error(cdef.sts());

  char *utf8name = cnv_iso8859_to_utf8(cdef.name(), strlen(cdef.name())+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
Cid_fullName(PyObject *s, PyObject *args)
{
  CidObject *self = (CidObject *)s;
  pwr_tOName name;

  wb_cdef cdef = pwrwb_ses->cdef(self->cid);
  if (!cdef)
    return set_error(cdef.sts());

  strcpy(name, cdef.longName().name(cdh_mName_volumeStrict));
  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
Cid_object(PyObject *s, PyObject *args)
{
  wb_object o = pwrwb_ses->object(((CidObject *)s)->cid);
  if ( !o)
    Py_RETURN_NONE;

  OidObject *instance = (OidObject *)Oid_new(&OidType, 0, 0);
  if (instance != NULL) {
    instance->o = o;
  }
  return (PyObject *)instance;
}

static PyObject *
Cid_nextObject(PyObject *s, PyObject *args)
{
  OidObject *o, *next;

  if ( !PyArg_ParseTuple(args, "O", &o))
    return NULL;

  wb_object no = o->o.next();
  if ( !no)
    Py_RETURN_NONE;

  next = (OidObject *)Oid_new(&OidType, 0, 0);
  if (next != NULL)
    next->o = no;

  return (PyObject *)next;
}

static PyObject *
Cid_objects(PyObject *s)
{
  PyObject *result;
  OidObject *object;
  int cnt = 0;
  wb_object o;

  for ( o= pwrwb_ses->object(((CidObject *)s)->cid); o; o = o.next())
    cnt++;

  result = PyTuple_New(cnt);
  cnt = 0;
  for ( o= pwrwb_ses->object(((CidObject *)s)->cid); o; o = o.next()) {
    object = (OidObject *)Oid_new(&OidType, 0, 0);
    if (object != NULL) {
      object->o = o;
      PyTuple_SetItem(result, cnt, (PyObject *)object);
    }
    cnt++;
  }
  return result;
}

static PyObject *
Cid_attrObject(PyObject *s, PyObject *args)
{
  CidObject *self = (CidObject *)s;
  pwr_tAttrRef aref;
  ArefObject *instance;

  pwrwb_ses->aref(self->cid, &aref);
  if (pwrwb_ses->evenSts())
    Py_RETURN_NONE;

  instance = (ArefObject *)Aref_new(&ArefType, 0, 0);
  if (instance != NULL) {
    wb_attribute a = pwrwb_ses->attribute(&aref);
    instance->attr = a;
  }
  return (PyObject *)instance;
}

static PyObject *
Cid_nextAttrObject(PyObject *s, PyObject *args)
{
  CidObject *self = (CidObject *)s;
  pwr_tAttrRef aref, oaref;
  ArefObject *a, *instance;

  if ( !PyArg_ParseTuple(args, "O", &a))
    return NULL;

  if (!a->attr)
    return set_error(a->attr.sts());

  aref = a->attr.aref();
  pwrwb_ses->nextAref(self->cid, &aref, &oaref);
  if (pwrwb_ses->evenSts())
    Py_RETURN_NONE;

  instance = (ArefObject *)Aref_new(&ArefType, 0, 0);
  if (instance != NULL) {
    wb_attribute oa = pwrwb_ses->attribute(&oaref);
    instance->attr = oa;
  }
  return (PyObject *)instance;
}

static PyObject *
Cid_attrObjects(PyObject *s)
{
  pwr_tAttrRef aref;
  ArefObject *object;
  CidObject *self = (CidObject *)s;
  PyObject *result;
  int cnt = 0;

  pwrwb_ses->aref(self->cid, &aref);
  while (pwrwb_ses->oddSts()) {
    cnt++;
    pwrwb_ses->nextAref(self->cid, &aref, &aref);
  }
  
  result = PyTuple_New(cnt);
  cnt = 0;
  pwrwb_ses->aref(self->cid, &aref);
  while (pwrwb_ses->oddSts()) {
    object = (ArefObject *)Aref_new(&ArefType, 0, 0);
    if (object != NULL) {
      wb_attribute oa = pwrwb_ses->attribute(&aref);
      object->attr = oa;
      PyTuple_SetItem(result, cnt, (PyObject *)object);
      cnt++;
    }    
    pwrwb_ses->nextAref(self->cid, &aref, &aref);
  }
  return result;
}

static PyObject *
Cid_attributes(PyObject *s)
{
  pwr_tStatus sts;
  CidObject *self = (CidObject *)s;
  ldh_sParDef *sys_bd;
  int sys_rows;
  ldh_sParDef *rt_bd;
  int rt_rows;
  ldh_sParDef *dev_bd;
  int dev_rows;
  PyObject *result;
  int i;
  int cnt;
  ADefObject *adef;

  sts = ldh_GetObjectBodyDef( pwrwb_ses, self->cid, "SysBody", 0, &sys_bd, &sys_rows);
  if (EVEN(sts))
    sys_rows = 0;

  sts = ldh_GetObjectBodyDef( pwrwb_ses, self->cid, "RtBody", 0, &rt_bd, &rt_rows);
  if (EVEN(sts))
    rt_rows = 0;

  sts = ldh_GetObjectBodyDef( pwrwb_ses, self->cid, "DevBody", 0, &dev_bd, &dev_rows);
  if (EVEN(sts))
    dev_rows = 0;

  result = PyTuple_New(sys_rows + rt_rows + dev_rows);

  cnt = 0;
  for ( i = 0; i < sys_rows; i++) {
    adef = (ADefObject *)ADef_new(&ADefType, 0, 0);
    if (adef != NULL) {
      strcpy(adef->name, sys_bd[i].ParName);
      adef->cid = sys_bd[i].ParClass;
      adef->offset = sys_bd[i].Par->Param.Info.Offset;
      adef->size = sys_bd[i].Par->Param.Info.Size;
      adef->flags = sys_bd[i].Par->Param.Info.Flags;
      adef->elements = sys_bd[i].Par->Param.Info.Elements;
      adef->aix = sys_bd[i].Par->Param.Info.ParamIndex;
      adef->typeref = sys_bd[i].Par->Param.TypeRef;
      PyTuple_SetItem(result, cnt, (PyObject *)adef);
      cnt++;
    }    
  }
  for ( i = 0; i < rt_rows; i++) {
    adef = (ADefObject *)ADef_new(&ADefType, 0, 0);
    if (adef != NULL) {
      strcpy(adef->name, rt_bd[i].ParName);
      adef->cid = rt_bd[i].ParClass;
      adef->offset = rt_bd[i].Par->Param.Info.Offset;
      adef->size = rt_bd[i].Par->Param.Info.Size;
      adef->flags = rt_bd[i].Par->Param.Info.Flags;
      adef->elements = rt_bd[i].Par->Param.Info.Elements;
      adef->aix = rt_bd[i].Par->Param.Info.ParamIndex;
      adef->typeref = rt_bd[i].Par->Param.TypeRef;
      PyTuple_SetItem(result, cnt, (PyObject *)adef);
      cnt++;
    }    
  }
  for ( i = 0; i < dev_rows; i++) {
    adef = (ADefObject *)ADef_new(&ADefType, 0, 0);
    if (adef != NULL) {
      strcpy(adef->name, dev_bd[i].ParName);
      adef->cid = dev_bd[i].ParClass;
      adef->offset = dev_bd[i].Par->Param.Info.Offset;
      adef->size = dev_bd[i].Par->Param.Info.Size;
      adef->flags = dev_bd[i].Par->Param.Info.Flags;
      adef->elements = dev_bd[i].Par->Param.Info.Elements;
      adef->aix = dev_bd[i].Par->Param.Info.ParamIndex;
      adef->typeref = dev_bd[i].Par->Param.TypeRef;
      PyTuple_SetItem(result, cnt, (PyObject *)adef);
      cnt++;
    }    
  }
  if ( sys_rows)
    free((char *)sys_bd);
  if ( rt_rows)
    free((char *)rt_bd);
  if ( dev_rows)
    free((char *)dev_bd);
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

  tid = ((TidObject *)self)->tid;

  if ( cdh_tidIsCid(tid)) {
    wb_object o = pwrwb_ses->object(cdh_ClassIdToObjid(tid));
    strncpy(name, o.longName().name(cdh_mName_volumeStrict), sizeof(name));
  }
  else {
    wb_object o = pwrwb_ses->object(cdh_TypeIdToObjid(tid));
    strncpy(name, o.longName().name(cdh_mName_volumeStrict), sizeof(name));
  }
  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return PyUnicode_FromFormat("%s", utf8name);  
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
Tid_init(PyObject *self, PyObject *args, PyObject *kwds)
{
  return 0;
}

static PyObject *
Tid_name(PyObject *s, PyObject *args)
{
  pwr_tTid tid;
  pwr_tObjName name;

  tid = ((TidObject *)s)->tid;

  if ( cdh_tidIsCid(tid)) {
    wb_object o = pwrwb_ses->object(cdh_ClassIdToObjid(tid));
    strncpy(name, o.longName().name(cdh_mName_object), sizeof(name));
  }
  else {
    wb_object o = pwrwb_ses->object(cdh_TypeIdToObjid(tid));
    strncpy(name, o.longName().name(cdh_mName_object), sizeof(name));
  }
  char *utf8name = cnv_iso8859_to_utf8(name, strlen(name)+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
Tid_fullName(PyObject *s, PyObject *args)
{
  pwr_tTid tid;
  pwr_tOName name;

  tid = ((TidObject *)s)->tid;

  if ( cdh_tidIsCid(tid)) {
    wb_object o = pwrwb_ses->object(cdh_ClassIdToObjid(tid));
    strncpy(name, o.longName().name(cdh_mName_volumeStrict), sizeof(name));
  }
  else {
    wb_object o = pwrwb_ses->object(cdh_TypeIdToObjid(tid));
    strncpy(name, o.longName().name(cdh_mName_volumeStrict), sizeof(name));
  }
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
  char *utf8name = cnv_iso8859_to_utf8(((ADefObject *)self)->name, strlen(((ADefObject *)self)->name)+1);
  return PyUnicode_FromFormat("%s", utf8name);
}

static int
ADef_init(PyObject *self, PyObject *args, PyObject *kwds)
{
  return 0;
}

static PyObject *
ADef_name(PyObject *s, PyObject *args)
{
  ADefObject *self = (ADefObject *)s;

  char *utf8name = cnv_iso8859_to_utf8(self->name, strlen(self->name)+1);
  return Py_BuildValue("s", utf8name);
}

static PyObject *
ADef_cid(PyObject *s, PyObject *args)
{
  ADefObject *self = (ADefObject *)s;
  return Py_BuildValue("i", self->cid);
}

static PyObject *
ADef_offset(PyObject *s, PyObject *args)
{
  ADefObject *self = (ADefObject *)s;
  return Py_BuildValue("i", self->offset);
}

static PyObject *
ADef_size(PyObject *s, PyObject *args)
{
  ADefObject *self = (ADefObject *)s;
  return Py_BuildValue("i", self->size);
}

static PyObject *
ADef_flags(PyObject *s, PyObject *args)
{
  ADefObject *self = (ADefObject *)s;
  return Py_BuildValue("i", self->flags);
}

static PyObject *
ADef_elements(PyObject *s, PyObject *args)
{
  ADefObject *self = (ADefObject *)s;
  return Py_BuildValue("i", self->elements);
}

static PyObject *
ADef_typeref(PyObject *s, PyObject *args)
{
  ADefObject *self = (ADefObject *)s;
  return Py_BuildValue("i", self->typeref);
}

/**
 * Static methods
 */


static PyObject *pwrwb_version(PyObject *self, PyObject *args)
{
  return Py_BuildValue("s", pwrv_cPwrVersionStr);
}

static PyObject *pwrwb_open(PyObject *self, PyObject *args)
{
  pwr_tStatus sts;
  char *name, *utf8name = 0;
  char dbname[80];
  unsigned int options = 0;

  if ( !PyArg_ParseTuple(args, "|s", &utf8name))
    return NULL;

  if ( utf8name == 0)
    strcpy(dbname, "directory");
  else {
    name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);
    strncpy(dbname, name, sizeof(dbname));
  }
  sts = ldh_OpenWB((ldh_tWorkbench *)&pwrwb_env, dbname, options);
  if ( EVEN(sts))
    return set_error(sts);

  wb_volume v;
  if ( !utf8name)
    v = pwrwb_env->volume("Directory");
  else {
    int found = 0;
    for ( v = pwrwb_env->volume(); v; v = v.next()) {
      if ( v.cid() == pwr_eClass_RootVolume && 
	   (v.type() == ldh_eVolRep_Db || v.type() == ldh_eVolRep_Mem ||
	    v.type() == ldh_eVolRep_Ced)) {
	found = 1;
	break;
      }
    }
    if ( !found)
      return set_error(LDH__NOSUCHVOL);
  }

  ldh_tVolume volctx;
  ldh_eAccess access = ldh_eAccess_ReadWrite;

  sts = ldh_AttachVolume(pwrwb_env, v.vid(), &volctx);
  if ( EVEN(sts))
    return set_error(sts);

  sts = ldh_OpenSession((ldh_tSession *)&pwrwb_ses, volctx, access, ldh_eUtility_Pwr);
  if ( EVEN(sts))
    return set_error(sts);

  Py_RETURN_NONE;
}

static PyObject *pwrwb_openWb(PyObject *self, PyObject *args)
{
  pwr_tStatus sts;
  char *name, *utf8name = 0;
  char dbname[80];
  unsigned int options = 0;

  if ( !PyArg_ParseTuple(args, "|s", &utf8name))
    return NULL;

  if ( utf8name == 0)
    strcpy(dbname, "directory");
  else {
    name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);
    strncpy(dbname, name, sizeof(dbname));
  }

  sts = ldh_OpenWB((ldh_tWorkbench *)&pwrwb_env, dbname, options);
  if ( EVEN(sts))
    return set_error(sts);
  Py_RETURN_NONE;
}

static PyObject *pwrwb_openSession(PyObject *self, PyObject *args)
{
  pwr_tStatus sts;
  VidObject *v;
  ldh_tVolume volctx;
  const char *access_str = 0;
  ldh_eAccess access = ldh_eAccess_ReadOnly;

  if ( !PyArg_ParseTuple(args, "O|s", &v, &access_str))
    return NULL;

  if ( access_str) {
    if ( strcmp(access_str, "w") == 0)
      access = ldh_eAccess_ReadWrite;
  }

  sts = ldh_AttachVolume(pwrwb_env, v->vid, &volctx);
  if ( EVEN(sts))
    return set_error(sts);

  sts = ldh_OpenSession((ldh_tSession *)&pwrwb_ses, volctx, access, ldh_eUtility_Pwr);
  if ( EVEN(sts))
    return set_error(sts);

  Py_RETURN_NONE;
}

static PyObject *pwrwb_saveSession(PyObject *self, PyObject *args)
{
  if (!pwrwb_ses)
    return set_error(LDH__NOSUCHSESS);

  pwrwb_ses->commit();
  if (pwrwb_ses->evenSts())
    return set_error(pwrwb_ses->sts());

  Py_RETURN_NONE;
}

static PyObject *pwrwb_revertSession(PyObject *self, PyObject *args)
{
  if (!pwrwb_ses)
    return set_error(LDH__NOSUCHSESS);

  pwrwb_ses->abort();
  if (pwrwb_ses->evenSts())
    return set_error(pwrwb_ses->sts());

  Py_RETURN_NONE;
}

static PyObject *pwrwb_closeWb(PyObject *self, PyObject *args)
{
  if (!pwrwb_env)
    return set_error(LDH__NOSUCHWB);

  delete pwrwb_env;
  pwrwb_env = 0;
  Py_RETURN_NONE;
}

static PyObject *pwrwb_closeSession(PyObject *self, PyObject *args)
{
  if (!pwrwb_ses)
    return set_error(LDH__NOSUCHSESS);

  delete pwrwb_ses;
  pwrwb_ses = 0;
  Py_RETURN_NONE;
}

static PyObject *pwrwb_close(PyObject *self, PyObject *args)
{
  if (pwrwb_ses) {
    delete pwrwb_ses;
    pwrwb_ses = 0;
  }

  if (pwrwb_env) {
    delete pwrwb_env;
    pwrwb_env = 0;
  }
  else
    return set_error(LDH__NOSUCHWB);

  Py_RETURN_NONE;
}

static PyObject *pwrwb_volume(PyObject *self, PyObject *args)
{
  VidObject *o;
  char *name, *utf8name = 0;
  pwr_tVid vid;

  if ( !pwrwb_env)
    return set_error(LDH__NOSUCHWB);

  if ( !PyArg_ParseTuple(args, "|s", &utf8name))
    return NULL;

  if ( utf8name) {
    name = cnv_utf8_to_iso8859(utf8name, strlen(utf8name)+1);
    wb_volume v = pwrwb_env->volume(name);
    if (!v)
      return set_error(v.sts());
    vid = v.vid();
  }
  else {
    /* No args */
    wb_volume v = pwrwb_env->volume();
    vid = v.vid();
  }

  o = (VidObject *)Vid_new(&VidType, 0, 0);
  if (o != NULL) {
    o->vid = vid;
  }

  return (PyObject *)o;
}

static PyObject *pwrwb_volumes(PyObject *self, PyObject *args)
{
  VidObject *vol;
  PyObject *result;
  int cnt = 0;
  wb_volume v;

  for ( v = pwrwb_env->volume(); v; v = v.next())
    cnt++;

  result = PyTuple_New(cnt);
  cnt = 0;
  for ( v = pwrwb_env->volume(); v; v = v.next()) {
    vol = (VidObject *)Vid_new(&VidType, 0, 0);
    if (vol != NULL) {
      vol->vid = v.vid();
      PyTuple_SetItem(result, cnt, (PyObject *)vol);
      cnt++;
    }    
  }
  return result;
}

static PyObject *pwrwb_object(PyObject *self, PyObject *args)
{
  PyObject *o = Oid_new(&OidType, args, 0);
  Oid_init(o, args, 0);
  if (!((OidObject *)o)->o)
    return NULL;
  return o;
}

static PyObject *pwrwb_attribute(PyObject *self, PyObject *args)
{
  PyObject *a = Aref_new(&ArefType, args, 0);
  Aref_init(a, args, 0);
  if (((ArefObject *)a)->attr.evenSts())
    return set_error(((ArefObject *)a)->attr.sts());
  return a;
}

static PyObject *pwrwb_login(PyObject *self, PyObject *args)
{
  pwr_tStatus sts;
  const char *utf8user;
  const char *utf8passwd;
  char user[80];
  char passwd[80];
  unsigned int priv;
  char systemgroup[80];
  char systemname[80];

  sts = lfu_ReadSysObjectFile( systemname, systemgroup);
  if ( EVEN(sts))
    return set_error(sts);

  if ( !PyArg_ParseTuple(args, "ss", &utf8user, &utf8passwd))
    return NULL;

  strncpy(user, cnv_utf8_to_iso8859(utf8user, strlen(utf8user)+1), sizeof(user));
  strncpy(passwd, cnv_utf8_to_iso8859(utf8passwd, strlen(utf8passwd)+1), sizeof(passwd));
  sts = user_CheckUser( systemgroup, user, user_PwCrypt((char *)passwd), &priv);
  if ( EVEN(sts))
    return set_error(sts);

  strcpy( pwrwb_user, user);
  pwrwb_priv = priv;

  Py_RETURN_NONE;
}

static PyObject *pwrwb_logout(PyObject *self, PyObject *args)
{
  strcpy( pwrwb_user, "");
  pwrwb_priv = pwr_mPrv_RtRead;

  Py_RETURN_NONE;
}

static PyObject *pwrwb_getPriv(PyObject *self, PyObject *args)
{
  return Py_BuildValue("I", pwrwb_priv);
}

static PyObject *pwrwb_getUser(PyObject *self, PyObject *args)
{
  char *utf8user = cnv_iso8859_to_utf8(pwrwb_user, strlen(pwrwb_user)+1);
  return Py_BuildValue("s", utf8user);
}

static PyObject *pwrwb_getSessionVolume(PyObject *self, PyObject *args)
{
  VidObject *o;

  if (!pwrwb_ses)
    return set_error(LDH__NOSUCHSESS);

  o = (VidObject *)Vid_new(&VidType, 0, 0);
  if (o != NULL) {
    o->vid = pwrwb_ses->vid();
  }

  return (PyObject *)o;
}

static PyObject *pwrwb_sessionIsEmpty(PyObject *self, PyObject *args)
{
  if (!pwrwb_ses)
    return set_error(LDH__NOSUCHSESS);

  return Py_BuildValue("I", pwrwb_ses->isEmpty() ? 1: 0);
}

static PyObject *pwrwb_build(PyObject *self, PyObject *args)
{
  char *nodename, *utf8nodename;
  char *options = 0;
  void *volumelist;
  int	volumecount;
  pwr_tStatus sts;

  if ( !pwrwb_env)
    return set_error(LDH__NOSUCHWB);

  if ( !is_authorized(pwr_mAccess_RtWrite | pwr_mAccess_System))
    return set_error(USER__NOTAUTHORIZED);

  if ( !PyArg_ParseTuple(args, "s|s", &utf8nodename, &options))
    return NULL;

  nodename = cnv_utf8_to_iso8859(utf8nodename, strlen(utf8nodename)+1);

  sts = lfu_volumelist_load( pwr_cNameBootList, (lfu_t_volumelist **) &volumelist,
			     &volumecount);
  if ( sts == FOE__NOFILE)
    return set_error(sts);

  WNav *wnav = new WNav(0, "", "", pwrwb_ses, 0, wnav_eWindowType_No, &sts);

  wb_build build( *pwrwb_ses, wnav);
  if ( options) {
    if ( strstr(options, "force") != 0)
      build.opt.force = ODD( dcli_get_qualifier( "/FORCE", 0, 0));
    if ( strstr(options, "debug") != 0)
      build.opt.debug = ODD( dcli_get_qualifier( "/DEBUG", 0, 0));
    if ( strstr(options, "cross") != 0)
      build.opt.crossref = ODD( dcli_get_qualifier( "/CROSSREFERENCE", 0, 0));
    if ( strstr(options, "manual") != 0)
      build.opt.manual = ODD( dcli_get_qualifier( "/MANUAL", 0, 0));
  }  
  build.node( nodename, bld_eNodeType_Node, volumelist, volumecount);
  delete wnav;
  free( (char *) volumelist);

  if ( build.evenSts())
    return set_error(build.sts());

  Py_RETURN_NONE;
}

static PyObject *pwrwb_command(PyObject *self, PyObject *args)
{
  pwr_tStatus sts;
  char *cmd, *utf8cmd;

  if ( !pwrwb_env)
    return set_error(LDH__NOSUCHWB);

  if ( !PyArg_ParseTuple(args, "s", &utf8cmd))
    return NULL;

  cmd = cnv_utf8_to_iso8859(utf8cmd, strlen(utf8cmd)+1);

  WNav *wnav = new WNav(0, "", "", pwrwb_ses, 0, wnav_eWindowType_No, &sts);
  wnav->priv = pwrwb_priv;
  wnav->command(cmd);
  sts = wnav->get_command_sts();
  delete wnav;

  if (EVEN(sts))
    return set_error(sts);
  
  Py_RETURN_NONE;
}


static PyMethodDef PwrwbMethods[] = {
  {"version", pwrwb_version, METH_NOARGS, pwrwb_version_doc},
  {"open", pwrwb_open, METH_VARARGS, pwrwb_open_doc},
  {"openWb", pwrwb_openWb, METH_VARARGS, pwrwb_openWb_doc},
  {"openSession", pwrwb_openSession, METH_VARARGS, pwrwb_openSession_doc},
  {"close", pwrwb_close, METH_VARARGS, pwrwb_close_doc},
  {"closeWb", pwrwb_closeWb, METH_NOARGS, pwrwb_closeWb_doc},
  {"closeSession", pwrwb_closeSession, METH_NOARGS, pwrwb_closeSession_doc},
  {"saveSession", pwrwb_saveSession, METH_NOARGS, pwrwb_saveSession_doc},
  {"revertSession", pwrwb_revertSession, METH_NOARGS, pwrwb_revertSession_doc},
  {"getSessionVolume", pwrwb_getSessionVolume, METH_NOARGS, pwrwb_getSessionVolume_doc},
  {"sessionIsEmpty", pwrwb_sessionIsEmpty, METH_NOARGS, pwrwb_sessionIsEmpty_doc},
  {"volume", pwrwb_volume, METH_VARARGS, pwrwb_volume_doc},
  {"volumes", pwrwb_volumes, METH_NOARGS, pwrwb_volumes_doc},
  {"object", pwrwb_object, METH_VARARGS, pwrwb_object_doc},
  {"attribute", pwrwb_attribute, METH_VARARGS, pwrwb_attribute_doc},
  {"login", pwrwb_login, METH_VARARGS, pwrwb_login_doc},
  {"logout", pwrwb_logout, METH_NOARGS, pwrwb_logout_doc},
  {"getPriv", pwrwb_getPriv, METH_NOARGS, pwrwb_getPriv_doc},
  {"getUser", pwrwb_getUser, METH_NOARGS, pwrwb_getUser_doc},
  {"build", pwrwb_build, METH_VARARGS, pwrwb_build_doc},
  {"command", pwrwb_command, METH_VARARGS, pwrwb_command_doc},
  {NULL, NULL, 0, NULL}};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "pwrwb",
    pwrwb_doc,
    -1,
    PwrwbMethods
};

PyMODINIT_FUNC PyInit_pwrwb(void)
{
  PyObject *m;

  if (PyType_Ready(&VidType) < 0 ||
      PyType_Ready(&OidType) < 0 ||
      PyType_Ready(&ArefType) < 0 ||
      PyType_Ready(&CidType) < 0 ||
      PyType_Ready(&TidType) < 0 ||
      PyType_Ready(&ADefType) < 0)
    return NULL;

  m = PyModule_Create(&moduledef);
  if (m == NULL)
    return m;

  Py_INCREF(&OidType);
  PyModule_AddObject(m, "Vid", (PyObject *)&VidType);
  Py_INCREF(&VidType);
  PyModule_AddObject(m, "Oid", (PyObject *)&OidType);
  Py_INCREF(&ArefType);
  PyModule_AddObject(m, "Aref", (PyObject *)&ArefType);
  Py_INCREF(&CidType);
  PyModule_AddObject(m, "Cid", (PyObject *)&CidType);
  Py_INCREF(&TidType);
  PyModule_AddObject(m, "Tid", (PyObject *)&TidType);
  Py_INCREF(&ADefType);
  PyModule_AddObject(m, "ADef", (PyObject *)&ADefType);

  return m;
}
