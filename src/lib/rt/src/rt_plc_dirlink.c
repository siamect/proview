/* rt_plc_dirlink.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   This module contains all routines to handle direct linking
   for the PLC-program.  */

#ifdef	OS_ELN
#include $vaxelnc
#include $kernelmsg
#include $kerneldef
#else
#include <stdio.h>
#include <string.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_cdh.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_hash_msg.h"
#include "rt_ini_msg.h"
#include "rt_plc_msg.h"
#include "rt_plc.h"
#include "rt_plc_io.h"

#if 0
/*
 * The dummy array is used as direct link pointer when the direct call
 * fails by some reason. If the size of the direct linked object is greater
 * than the fixed object, then a xDLp object will be allocated to that size.
 */

static char xDLbuf[512];
static char xDLsize = 0;
static char *xDLp = NULL;

#define GUARD_DL( _PTR, _SIZE )\
  if (_SIZE < sizeof( xDLbuf ))\
    *_PTR = &xDL;\
  else if ((xDLp != NULL) && (_SIZE < xDLsize)))\
    *_PTR = xDLp;\
  else\
  {\
    xDLsize = _SIZE;\
    xDlp = calloc(1, xDLsize);\
    *_PTR = xDLp;\
  }
#else
#define GUARD_DL( _PTR, _SIZE ) /* Do nothing */
#endif

/* Setup Direct links or subscriptions

      local_object I	TRUE if object is local
             class I	Class of object to directlink
             objid I	Object to directlink
 	        la I	Direct link array from the plc-code
 	   aname I	Attribute name string 

*/

static pwr_tStatus
plc_RefObjidAttr (
  pwr_tBoolean  local_object,
  pwr_tClassId  class,
  pwr_sAttrRef  ar,
  plc_t_rtdbref *la,
  char		*aname)
{
  pwr_tStatus	sts;
  pwr_tDlid	dlid;
  pwr_tSubid	subid;
  pwr_sAttrRef	car;

  if ( strcmp( aname, "") != 0) {
    sts = gdh_ClassAttrToAttrref( class, aname, &car);
    if (EVEN(sts)) return sts;

    ar.Offset += car.Offset;
    ar.Size = car.Size;
    ar.Flags.m = car.Flags.m;
  }
  if (ar.Size < la->Size)
    ar.Size = la->Size;

  if (local_object) {
    sts = gdh_DLRefObjectInfoAttrref( &ar, la->Pointer, &dlid );
    if (EVEN(sts)) return sts;
  } else {
    sts = gdh_SubRefObjectInfoAttrref( &ar, &subid );
    if (EVEN(sts)) return sts;

    sts = gdh_SubAssociateBuffer( subid, la->Pointer, ar.Size );
    if (EVEN(sts)) return sts;
  }

  return PLC__SUCCESS;
}

/* Get the value of an attribute in an object
   The objid must be a local object

    If 'class' is zero use objid */

static pwr_tStatus
plc_GetObjectAttrValue (
  pwr_tClassId	class,
  pwr_sAttrRef	attrref,
  char		*aname,
  void		*buffer,
  pwr_tInt32	size
)
{
  pwr_tStatus	sts;
  pwr_sAttrRef	ar;
  char		*p;

  if (class == 0) {
    sts = gdh_GetObjectClass( attrref.Objid, &class);
    if (EVEN(sts)) return sts;
  }

  sts = gdh_ClassAttrToAttrref( class, aname, &ar );
  if (EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer(attrref.Objid, (pwr_tAddress *)&p );
  if (EVEN(sts)) return sts;

  memcpy( buffer, p + attrref.Offset + ar.Offset, size);

  return PLC__SUCCESS;
}

/* Return a pointer to an object or parameter in
   rtdb. This pointer is used to help the PLC code speed
   up it's access of objects and parameters in rtdb

   If the object type is a type that stores its actual values
   in an I/O-copy object, then if the name that is passed to 
   plc_DirectLink ends with ".ActualValue", then we want to 
   return the pointer to an element in an I/O-copy object

   On the other hand, if the name doesn't end with ".ActualValue",
   then we return a pointer to start of the single rtdb object.  */

pwr_tStatus
plc_rtdbref (
  plc_t_rtdbref (*LinkArray)[], 
  plc_sThread *tp
)
{
  pwr_tStatus	sts;
  pwr_tInt32	Index;
  char		aname[255];
  plc_t_rtdbref *la;
  pwr_tClassId	class;
  pwr_tBoolean	local_object;

  for (la = *LinkArray; la->Pointer != NULL; ++la) {
    /* NULL pointer terminates the list */
    *la->Pointer = NULL;

    sts = gdh_GetObjectLocation(la->AttrRef.Objid, &local_object);
    if (EVEN(sts)) {
      local_object = FALSE;
    }

    if (la->UseCode == UC_NORMAL) {
      /* If we got here, we can link directly to the object without having
	 to append an index to the entered name.  */

      sts = plc_RefObjidAttr(local_object, la->ObjType, la->AttrRef, la, "");
      if (EVEN(sts)) {
	errh_Error("plc_RefObjidAttr object %s, %m", cdh_ObjidToString(NULL, la->AttrRef.Objid, 0), sts);
	GUARD_DL( la->Pointer, la->Size );
 	continue;
      }
    } else {
      /* We're direct linking to a parameter in a Area-object. 
         These parameters are stored in a vector. We have to find
         out the index in the vector before we can direct link. The
         index is stored in the object itself.  */

      if (local_object) {
        sts = plc_GetObjectAttrValue(la->ObjType, la->AttrRef, ".ValueIndex", &Index, sizeof(pwr_tInt32));
        if (EVEN(sts)) {
	  errh_Error("plc_GetObjectAttrValue object %s.ValueIndex\n%m", cdh_ObjidToString(NULL, la->AttrRef.Objid, 0), sts);
	  GUARD_DL( la->Pointer, la->Size );
	  continue;
        }
      }


      /* Get the address to the correct element (indicated by Index) in the
	correct object, and store the value in Pointer.  */

      strcpy(aname, ".ActualValue" );
      class = la->ObjType;
      switch (la->ObjType) {
      case pwr_cClass_Di:
	if (la->UseCode == UC_READ && local_object)
	  *la->Pointer = &tp->copy.di_a.p->Value[Index];
	break;

      case pwr_cClass_Ai:
	if (la->UseCode == UC_READ && local_object)
	  *la->Pointer = &tp->copy.ai_a.p->Value[Index];
	break;

      case pwr_cClass_Ii:
	if (la->UseCode == UC_READ && local_object)
	  *la->Pointer = &tp->copy.ii_a.p->Value[Index];
	break;

      case pwr_cClass_Do:
	if (la->UseCode == UC_READ && local_object)
	  *la->Pointer = &tp->copy.do_a.p->Value[Index];
	break;

      case pwr_cClass_Ao:
	if (la->UseCode == UC_READ && local_object)
	  *la->Pointer = &tp->copy.ao_a.p->Value[Index];
	break;

      case pwr_cClass_Io:
	if (la->UseCode == UC_READ && local_object)
	  *la->Pointer = &tp->copy.io_a.p->Value[Index];
	break;

      case pwr_cClass_Dv:
	if (la->UseCode == UC_READ && local_object)
	  *la->Pointer = &tp->copy.dv_a.p->Value[Index];
	break;

      case pwr_cClass_Av:
	if (la->UseCode == UC_READ && local_object)
	  *la->Pointer = &tp->copy.av_a.p->Value[Index];
	break;

      case pwr_cClass_Iv:
	if (la->UseCode == UC_READ && local_object)
	  *la->Pointer = &tp->copy.iv_a.p->Value[Index];
	break;

      /* Special: UC_READ => AbsValue, UC_READ2 => RawValue */
      case pwr_cClass_Co:
	if (local_object) {
	  if (la->UseCode == UC_READ) {
	    *la->Pointer = &tp->copy.ca_a.p->Value[Index];
	  } else if (la->UseCode == UC_READ2) {
	    *la->Pointer = &tp->copy.co_a.p->Value[Index];
	  } else if (la->UseCode == UC_WRITE) {
	    strcpy(aname, ".AbsValue" );
	  } else {
	    strcpy(aname, ".RawValue" );
	  }
	} else {
	  if (la->UseCode == UC_READ)
	    strcpy(aname, ".AbsValue" );
	  else
	    strcpy(aname, ".RawValue" );
	}
	break;
      }

      if (*la->Pointer != NULL)
	continue;

      sts = plc_RefObjidAttr(local_object, class, la->AttrRef, la, aname);
      if (EVEN(sts)) {
        errh_Error("plc_RefObjidAttr(%s), %m", cdh_ObjidToString(NULL, la->AttrRef.Objid, 0), sts);
	GUARD_DL( la->Pointer, la->Size );
 	continue;
      }
    }
  }

  return PLC__SUCCESS;
}

/* Get a pointer to an object in rtdb. I the object doesn't exist,
   then we create the object in order to be able to get the pointer.
   A direct link is made to the object.  */

void 
plc_GetPointerToObject (
  pwr_tAddress	*Pointer,
  char		*Name,
  pwr_tSubid	*Subid,
  pwr_tObjid	*Objid,
  pwr_tInt32	Type,
  pwr_tInt32	Size
)
{
  pwr_tStatus	sts;

  /* Only handle object if BaseValue object > 0 bytes */
  if (Size == 0) return;

  sts = gdh_NameToObjid(Name, Objid);
  if (ODD(sts)) {
    sts = gdh_RefObjectInfo(Name, Pointer, Subid, Size);
    if (EVEN(sts)) 
      errh_Error("gdh_RefObjectInfo(%s), %m", Name, sts);
    return;
  }

  /* The object either doesn't exist, or we have an error.  */
  if (sts != GDH__NOSUCHOBJ && sts != HASH__NOTFOUND) {
    errh_Error("Getting pointer to object (%s), %m", Name, sts);
    return;
  }

  /* Create the object with the same size as the 
     basevalue object for this object type. However, if the XXBaseValueSize
     equals 0, then there aren't any objects of this type used on this 
     node, and therefore we don't create or link to a copy object. No
     use creating objects if they won't be used.  */

  sts = gdh_CreateObject( Name, Type, Size, Objid, pwr_cNObjid, 0, pwr_cNObjid);
  if (EVEN(sts)) {
    errh_Error("Creating object (%s), %m", Name, sts);
  } else {
   /* Make a direct link to the object */
    sts = gdh_RefObjectInfo(Name, Pointer, Subid, Size);
    if (EVEN(sts)) 
      errh_Error("Direct link to object (%s), %m", Name, sts);
  }
}





