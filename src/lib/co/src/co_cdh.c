/* co_cdh.c -- class definition handler

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module contains the API-routines to the
   Class Definition Handler, CDH.  */

#ifdef OS_ELN
# include ctype
# include $vaxelnc
# include stdio
# include math
# include string
# include time
# include stdlib
# include float
# include stddef
# include errno
# include limits
#else
# include <stdio.h>
# include <math.h>
# include <string.h>
# include <time.h>
# include <stdlib.h>
# include <float.h>
# include <ctype.h>
# include <stddef.h>
# include <errno.h>
# include <limits.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "co_cdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"

#ifdef OS_ELN
struct  p1_ctx {
  int             p1$errno;                       /* UNIX-style error code */
  int             p1$vaxc$errno;                  /* VAX equivalent of errno above */
  int             p1$randx;                       /* random number seed */
  /* More members follows in the real struct */ 
};
#endif


/* Compare two object identities;

   return <0 if Object_1 < Object_2
   return  0 if Object_1 == Object_2
   return >0 if Object_1 > Object_2.  */

int
cdh_ObjidCompare (
  pwr_tObjid  Objid_1,
  pwr_tObjid  Objid_2
)
{

  if (Objid_1.vid == Objid_2.vid) {
    if (Objid_1.oix == Objid_2.oix)
      return 0;
    else if (Objid_1.oix < Objid_2.oix)
      return -1;
    else
      return 1;
  } else if (Objid_1.vid < Objid_2.vid)
    return -1;
  else
    return 1;

}


/* Test if two object identities are equal.

   return  1 if Object_1 == Object_2
   return  0 if Object_1 != Object_2.  */

int
cdh_ObjidIsEqual (
  pwr_tObjid  Objid_1,
  pwr_tObjid  Objid_2
)
{

  return (Objid_1.vid == Objid_2.vid) && (Objid_1.oix == Objid_2.oix);
}

/* Test if two object identities are different.

   return  1 if Object_1 != Object_2
   return  0 if Object_1 == Object_2.  */

int
cdh_ObjidIsNotEqual (
  pwr_tObjid  Objid_1,
  pwr_tObjid  Objid_2
)
{

  return (Objid_1.vid != Objid_2.vid) || (Objid_1.oix != Objid_2.oix);
}


/* Test if object identity is null.

   return  1 if Object_1 == pwr_cNObjid
   return  0 if Object_1 != pwr_cNObjid.  */

int
cdh_ObjidIsNull (
  pwr_tObjid  Objid
)
{
  return (Objid.vid == pwr_cNObjid.vid) && (Objid.oix == pwr_cNObjid.oix);
}


/* Test if object identity is not null.

   return  1 if Object_1 != pwr_cNObjid
   return  0 if Object_1 == pwr_cNObjid.  */

int
cdh_ObjidIsNotNull (
  pwr_tObjid  Objid
)
{
  return (Objid.vid != pwr_cNObjid.vid) || (Objid.oix != pwr_cNObjid.oix);
}


int
cdh_RefIdCompare (
  pwr_tRefId		Reference_1,
  pwr_tRefId		Reference_2
)
{
  return cdh_ObjidCompare(*(pwr_tObjid*) &Reference_1, *(pwr_tObjid*) &Reference_2);
}


int
cdh_RefIdIsEqual (
  pwr_tRefId		Reference_1,
  pwr_tRefId		Reference_2
)
{
  return cdh_ObjidIsEqual(*(pwr_tObjid*) &Reference_1, *(pwr_tObjid*) &Reference_2);
}


int
cdh_RefIdIsNotEqual (
  pwr_tRefId		Reference_1,
  pwr_tRefId		Reference_2
)
{
  return cdh_ObjidIsNotEqual(*(pwr_tObjid*) &Reference_1, *(pwr_tObjid*) &Reference_2);
}


int
cdh_RefIdIsNull (
  pwr_tRefId		Reference
)
{
  return cdh_ObjidIsNull(*(pwr_tObjid*) &Reference);
}


int
cdh_RefIdIsNotNull (
  pwr_tRefId		Reference
)
{
  return cdh_ObjidIsNotNull(*(pwr_tObjid*) &Reference);
}


int
cdh_SubidCompare (
  pwr_tSubid		Subscription_1,
  pwr_tSubid		Subscription_2
)
{
  return cdh_ObjidCompare(*(pwr_tObjid*) &Subscription_1, *(pwr_tObjid*) &Subscription_2);
}


int
cdh_SubidIsEqual (
  pwr_tSubid		Subscription_1,
  pwr_tSubid		Subscription_2
)
{
  return cdh_ObjidIsEqual(*(pwr_tObjid*) &Subscription_1, *(pwr_tObjid*) &Subscription_2);
}


int
cdh_SubidIsNotEqual (
  pwr_tSubid		Subscription_1,
  pwr_tSubid		Subscription_2
)
{
  return cdh_ObjidIsNotEqual(*(pwr_tObjid*) &Subscription_1, *(pwr_tObjid*) &Subscription_2);
}


int
cdh_SubidIsNull (
  pwr_tSubid		Subscription
)
{
  return cdh_ObjidIsNull(*(pwr_tObjid*) &Subscription);
}


int
cdh_SubidIsNotNull (
  pwr_tSubid		Subscription
)
{
  return cdh_ObjidIsNotNull(*(pwr_tObjid*) &Subscription);
}


int
cdh_DlidCompare (
  pwr_tDlid		DirectLink_1,
  pwr_tDlid		DirectLink_2
)
{
  return cdh_ObjidCompare(*(pwr_tObjid*) &DirectLink_1, *(pwr_tObjid*) &DirectLink_2);
}


int
cdh_DlidIsEqual (
  pwr_tDlid		DirectLink_1,
  pwr_tDlid		DirectLink_2
)
{
  return cdh_ObjidIsEqual(*(pwr_tObjid*) &DirectLink_1, *(pwr_tObjid*) &DirectLink_2);
}


int
cdh_DlidIsNotEqual (
  pwr_tDlid		DirectLink_1,
  pwr_tDlid		DirectLink_2
)
{
  return cdh_ObjidIsNotEqual(*(pwr_tObjid*) &DirectLink_1, *(pwr_tObjid*) &DirectLink_2);
}


int
cdh_DlidIsNull (
  pwr_tDlid		DirectLink
)
{
  return cdh_ObjidIsNull(*(pwr_tObjid*) &DirectLink);
}


int
cdh_DlidIsNotNull (
  pwr_tDlid		DirectLink
)
{
  return cdh_ObjidIsNotNull(*(pwr_tObjid*) &DirectLink);
}


/* Convert Objid to ClassId.  */

pwr_tClassId
cdh_ClassObjidToId (
  pwr_tObjid		Objid
)
{
  static cdh_uTypeId	cid;
  cdh_uObjid		oid;

  oid.pwr  = Objid;

  cid.c.bix = 0;
  cid.c.cix = oid.c.cix;
  cid.c.must_be_zero = 0;
  cid.c.vid_0 = oid.c.vid_0;
  cid.c.vid_1 = oid.c.vid_1;

  return (pwr_tClassId) cid.pwr;
}


/* Convert ClassId to Objid.  */

pwr_tObjid
cdh_ClassIdToObjid (
  pwr_tClassId		Class
)
{
  static cdh_uObjid	oid;
  cdh_uTypeId		cid;

  cid.pwr = Class;

  oid.c.aix = 0;
  oid.c.reserved = 0;
  oid.c.bix = 0;
  oid.c.cix = cid.c.cix;
  oid.c.must_be_two = 2;
  oid.c.vid_0 = cid.c.vid_0;
  oid.c.vid_1 = cid.c.vid_1;
  oid.c.vid_2 = 0;
  oid.c.vid_3 = 0;

  return oid.pwr;
}

/* Convert Objid to TypeId.  */

pwr_tTypeId
cdh_TypeObjidToId (
  pwr_tObjid		Objid
)
{
  cdh_uObjid		oid;
  static cdh_uTypeId	tid;

  oid.pwr = Objid;

  if (oid.t.must_be_three == 2) {
    /* We have a class body.  */
    tid.c.bix = oid.c.bix;
    tid.c.cix = oid.c.cix;
    tid.c.must_be_zero = 0;
    tid.c.vid_0 = oid.c.vid_0;
    tid.c.vid_1 = oid.c.vid_1;
  } else if (oid.t.must_be_three == 3) {
    /* We have a type.  */
    tid.t.tix = oid.t.tix;
    tid.t.tyg = oid.t.tyg;
    tid.t.must_be_one = 1;
    tid.t.vid_0 = oid.t.vid_0;
    tid.t.vid_1 = oid.t.vid_1;
  } else {
    /* We have an error.  !!! To do !!!  */
    tid.t.tix = oid.t.tix;
    tid.t.tyg = oid.t.tyg;
    tid.t.must_be_one = 1;
    tid.t.vid_0 = oid.t.vid_0;
    tid.t.vid_1 = oid.t.vid_1;
  }

  return tid.pwr;
}


/* Convert TypeId to index.  */

int
cdh_TypeIdToIndex (
  pwr_tTypeId		Type
)
{
  cdh_uTypeId		tid;

  tid.pwr = Type;

  return tid.t.tix;
}

/* Convert TypeId to Objid.  */

pwr_tObjid
cdh_TypeIdToObjid (
  pwr_tTypeId		Type
)
{
  static cdh_uObjid	oid;
  cdh_uTypeId		tid;

  tid.pwr = Type;

  if (tid.t.must_be_one == 1) {
    oid.t.aix = 0;
    oid.t.reserved = 0;
    oid.t.tix = tid.t.tix;
    oid.t.tyg = tid.t.tyg;
    oid.t.must_be_three = 3;
    oid.t.vid_0 = tid.t.vid_0;
    oid.t.vid_1 = tid.t.vid_1;
    oid.t.vid_2 = 0;
    oid.t.vid_3 = 0;
  } else {
    oid.c.aix = 0;
    oid.c.reserved = 0;
    oid.c.bix = tid.c.bix;
    oid.c.cix = tid.c.cix;
    oid.c.must_be_two = 2;
    oid.c.vid_0 = tid.c.vid_0;
    oid.c.vid_1 = tid.c.vid_1;
    oid.c.vid_2 = 0;
    oid.c.vid_3 = 0;
  }

  return oid.pwr;
}

/* Converts an attribute given in internal binary format
   to a text string.
   */

pwr_tStatus
cdh_AttrValueToString (
  pwr_eType		Type,
  void			*Value,
  char			*String,
  int			MaxSize
)
{
  pwr_tStatus		sts = CDH__SUCCESS;
  char			timbuf[24];
  static char		sval[512];

  sval[0] = '\0';

  switch (Type) {
  case pwr_eType_Boolean:
    if (*(pwr_tBoolean *) Value)
      strcpy(sval, "1");
    else
      strcpy(sval, "0");
    break;
  case pwr_eType_Float32:
    sprintf(sval, "%g", *(pwr_tFloat32 *) Value);
    break;
  case pwr_eType_Float64:
    sprintf(sval, "%g", *(pwr_tFloat64 *) Value);
    break;
  case pwr_eType_Char:
    if (*(pwr_tChar *) Value == 0)
      *sval = '\0';
    else
      sprintf(sval, "%c", *(pwr_tChar *) Value);
    break;
  case pwr_eType_Int8:
    sprintf(sval, "%d", *(pwr_tInt8 *) Value);
    break;
  case pwr_eType_Int16:
    sprintf(sval, "%d", *(pwr_tInt16 *) Value);
    break;
  case pwr_eType_Int32:
    sprintf(sval, "%d", *(pwr_tInt32 *) Value);
    break;
  case pwr_eType_UInt8:
    sprintf(sval, "%u", *(pwr_tUInt8 *) Value);
    break;
  case pwr_eType_UInt16:
    sprintf(sval, "%u", *(pwr_tUInt16 *) Value);
    break;
  case pwr_eType_UInt32:
    sprintf(sval, "%u", *(pwr_tUInt32 *) Value);
    break;
  case pwr_eType_Time:
    if (ODD(time_AtoAscii(Value, time_eFormat_DateAndTime, timbuf, sizeof(timbuf)))) {
      strcpy(sval, timbuf);
    } else {
      sprintf(sval, "*** Bad time value ***");
      sts = CDH__INVTIME;
    }
    break;
  case pwr_eType_DeltaTime:
    if (ODD(time_DtoAscii(Value, 1, timbuf, sizeof(timbuf)))) {
      strcpy(sval, timbuf);
    } else {
      sprintf(sval, "*** Bad delta time value ***");
      sts = CDH__INVDELTATIME;
    }
    break;
  case pwr_eType_String:
    sprintf(sval,"%s", (char *) Value);
    break;
  default:
    sts = CDH__INVTYPE;
    break;
  }

  strncpy(String, sval, MaxSize);
  return sts;
}

/* Converts an attribute value given as a text string,
   to internal binary format.  */

pwr_tStatus
cdh_StringToAttrValue (
  pwr_eType		Type,
  const char		*String,
  void			*Value
)
{
  pwr_tStatus		sts = CDH__SUCCESS;
  pwr_tBoolean		bval = 0;
  pwr_tInt8		i8val = 0;
  pwr_tInt16		i16val = 0;
  pwr_tInt32		i32val = 0;
  pwr_tUInt8		ui8val = 0;
  pwr_tUInt16		ui16val = 0;
  pwr_tUInt32		ui32val = 0;
  pwr_tFloat32		f32val = 0.0;
  pwr_tFloat64		f64val = 0.0;
  pwr_tClassId		cidval = pwr_cNClassId;
  pwr_tTypeId		tidval = pwr_cNTypeId;
  pwr_tObjectIx		oixval = pwr_cNObjectIx;
  pwr_tVolumeId		vidval = pwr_cNVolumeId;
  pwr_tTime		timeval;
  pwr_tDeltaTime	dtimeval;
  char			timbuf[24];
  unsigned int		timlen;
  char			*endp;

#ifdef OS_ELN
  struct p1_ctx *ctx_pointer = eln$locate_crtl_ctx();
  ctx_pointer->p1$errno = 0;
#else
  errno = 0;
#endif

  pwr_Assert(String != NULL);
  pwr_Assert(Value != NULL);




  switch (Type) {
  case pwr_eType_Boolean:
    if (*String != '\0') {
      i32val = strtoul(String, &endp, 0);
      if (errno == ERANGE || *endp != '\0' || (i32val != 0 && i32val != 1)) {
	sts = CDH__INVBOOL;
	break;
      }
    }
    bval = i32val;
    memcpy(Value, &bval, sizeof(bval));
    break;

  case pwr_eType_Float32:
    if (*String != '\0') {
      f64val = strtod(String, &endp);
      if (errno == ERANGE || *endp != '\0' || fabs(f64val) > FLT_MAX) {
	sts = CDH__INVFLOAT32;
	break;
      }
    }
    f32val = f64val;
    memcpy(Value, &f32val, sizeof(f32val));
    break;

  case pwr_eType_Float64:
    if (*String != '\0') {
      f64val = strtod(String, &endp);
      if (errno == ERANGE || *endp != '\0' || fabs(f64val) > DBL_MAX) { 
	sts = CDH__INVFLOAT64;
	break;
      }
    }
    memcpy(Value, &f64val, sizeof(f64val));
    break;

  case pwr_eType_Char:
    *((char *) Value) = *String;
    break;

  case pwr_eType_Int8:
    if (*String != '\0') {
      i32val = strtol(String, &endp, 0);
      if (errno == ERANGE || *endp != '\0' || i32val > SCHAR_MAX || 
	i32val < SCHAR_MIN
      ) {
	sts = CDH__INVINT8;
	break;
      }
    }
    i8val = i32val;
    memcpy(Value, &i8val, sizeof(i8val));
    break;

  case pwr_eType_Int16:
    if (*String != '\0') {
      i32val = strtol(String, &endp, 0);
      if (errno == ERANGE || *endp != '\0' || i32val > SHRT_MAX ||
	i32val < SHRT_MIN
      ) {
	sts = CDH__INVINT16;
	break;
      }
    }
    i16val = i32val;
    memcpy(Value, &i16val, sizeof(i16val));
    break;

  case pwr_eType_Int32:
    if (*String != '\0') {
      i32val = strtol(String, &endp, 0);
      if (errno == ERANGE || *endp != '\0' || i32val > LONG_MAX ||
	i32val < LONG_MIN
      ) {
	sts = CDH__INVINT32;
	break;
      }
    }
    memcpy(Value, &i32val, sizeof(i32val));
    break;

  case pwr_eType_UInt8:
    while (*String && isspace(*String)) String++;
    if (*String != '\0') {
      ui32val = strtoul(String, &endp, 0);
      if (errno == ERANGE || *endp != '\0' || ui32val > UCHAR_MAX 
	|| *String == '-') {
	sts = CDH__INVUINT8;
	break;
      }
    }
    ui8val = ui32val;
    memcpy(Value, &ui8val, sizeof(ui8val));
    break;

  case pwr_eType_UInt16:
    while (*String && isspace(*String)) String++;
    if (*String != '\0') {
      ui32val = strtoul(String, &endp, 0);
      if (errno == ERANGE || *endp != '\0' || ui32val > USHRT_MAX
	|| *String == '-') {
	sts = CDH__INVUINT16;
	break;
      }
    }
    ui16val = ui32val;
    memcpy(Value, &ui16val, sizeof(ui32val));
    break;

  case pwr_eType_UInt32:
  case pwr_eType_Mask:
  case pwr_eType_Enum:
    while (*String && isspace(*String)) String++;
    if (*String != '\0') {
      ui32val = strtoul(String, &endp, 0);
      if (errno == ERANGE || *endp != '\0' || ui32val > ULONG_MAX
	|| *String == '-') {
	sts = CDH__INVUINT32;
	break;
      }
    }
    memcpy(Value, &ui32val, sizeof(ui32val));
    break;

  case pwr_eType_String:
    strcpy(Value, String);
    break;

  case pwr_eType_Time:
    timlen = strlen(String);
    if (timlen > 23) {
      sts = CDH__INVTIME;
      break;
    }
    memcpy(timbuf, String, MIN(sizeof(timbuf), timlen));
    timbuf[MIN(sizeof(timbuf), timlen)] = '\0';
    sts = time_AsciiToA(timbuf, &timeval);
    if (EVEN(sts)) {
      sts = CDH__INVTIME;
      break;
    }
    memcpy(Value, &timeval, sizeof(timeval));
    break;

  case pwr_eType_DeltaTime:
    timlen = strlen(String);
    if (timlen > 23) {
      sts = CDH__INVDELTATIME;
      break;
    }
    memcpy(timbuf, String, MIN(sizeof(timbuf), timlen));
    timbuf[MIN(sizeof(timbuf), timlen)] = '\0';
    sts = time_AsciiToD(timbuf, &dtimeval);
    if (EVEN(sts)) {
      sts = CDH__INVDELTATIME;
      break;
    }
    memcpy(Value, &dtimeval, sizeof(dtimeval));
    break;

  case pwr_eType_ClassId:
    while (*String && isspace(*String)) String++;
    if (*String != '\0') {
      sts = cdh_StringToClassId(String, &cidval);
      if (EVEN(sts))
	break;
    }
    memcpy(Value, &cidval, sizeof(cidval));
    break;

  case pwr_eType_VolumeId:
    while (*String && isspace(*String)) String++;
    if (*String != '\0') {
      sts = cdh_StringToVolumeId(String, &vidval);
      if (EVEN(sts))
	break;
    }
    memcpy(Value, &vidval, sizeof(vidval));
    break;

  case pwr_eType_TypeId:
    while (*String && isspace(*String)) String++;
    if (*String != '\0') {
      sts = cdh_StringToTypeId(String, &tidval);
      if (EVEN(sts))
	break;
    }
    memcpy(Value, &tidval, sizeof(tidval));
    break;

  case pwr_eType_ObjectIx:
    while (*String && isspace(*String)) String++;
    if (*String != '\0') {
      sts = cdh_StringToObjectIx(String, &oixval);
      if (EVEN(sts))
	break;
    }
    memcpy(Value, &oixval, sizeof(oixval));
    break;

  default:
    sts = CDH__INVTYPE;
    break;
  }
  
  return sts;
}

pwr_tStatus
cdh_StringToClassId (
  const char		*s,
  pwr_tClassId		*cid
)
{
  unsigned int	vid_0;
  unsigned int	vid_1;
  unsigned int	cix;
  cdh_uTypeId		lcid;

  pwr_Assert(cid != NULL);

  lcid.pwr = pwr_cNClassId;

  if (*s == '_') s++;
  if (*s == 'C' || *s == 'c') s++;

  if (sscanf(s, "%d.%d:%d%*s", &vid_1, &vid_0, &cix) != 3)
    return CDH__INVCID;

  if (vid_1 > cdh_cMaxVidGroup || vid_0 > cdh_cMaxVidGroup || cix > cdh_cMaxCix)
    return CDH__INVCID;

  lcid.c.vid_1 = vid_1;
  lcid.c.vid_0 = vid_0;
  lcid.c.cix = cix;

  *cid = lcid.pwr;

  return CDH__SUCCESS;
}

pwr_tStatus
cdh_StringToObjid (
  const char		*s,
  pwr_tObjid		*oid
)
{
  unsigned int	vid_0;
  unsigned int	vid_1;
  unsigned int	vid_2;
  unsigned int	vid_3;
  unsigned int	oix;
  cdh_uObjid		loid;

  pwr_Assert(oid != NULL);

  loid.pwr = pwr_cNObjid;

  if (*s == '_') s++;
  if (*s == 'O' || *s == 'o') s++;
  if (*s == 'A' || *s == 'a') s++;

  if (sscanf(s, "%d.%d.%d.%d:%u%*s", &vid_3, &vid_2, &vid_1, &vid_0, &oix) != 5)
    return CDH__INVCID;

  if (
    vid_3 > cdh_cMaxVidGroup || vid_2 > cdh_cMaxVidGroup ||
    vid_1 > cdh_cMaxVidGroup || vid_0 > cdh_cMaxVidGroup
  ) {
    return CDH__INVCID;
  }

  loid.o.vid_3 = vid_3;
  loid.o.vid_2 = vid_2;
  loid.o.vid_1 = vid_1;
  loid.o.vid_0 = vid_0;
  loid.o.oix = oix;

  *oid = loid.pwr;

  return CDH__SUCCESS;
}

pwr_tStatus
cdh_StringToTypeId (
  const char		*s,
  pwr_tTypeId		*tid
)
{
  unsigned int	vid_0;
  unsigned int	vid_1;
  unsigned int	bit;
  unsigned int	tyg;
  unsigned int	tix;
  cdh_uTypeId		ltid;

  pwr_Assert(tid != NULL);

  ltid.pwr = pwr_cNTypeId;

  if (*s == '_') s++;
  if (*s == 'T' || *s == 't') s++;

  if (sscanf(s, "%d.%d:%d.%d.%d%*s", &vid_1, &vid_0, &bit, &tyg, &tix) != 5)
    return CDH__INVTID;

  if (vid_0 > cdh_cMaxVidGroup || vid_1 > cdh_cMaxVidGroup || bit > 1)
    return CDH__INVTID;

  if (bit == 0) { /*  Class Type.  */
    if (tyg > cdh_cMaxCix || tix > cdh_cMaxBix)
      return CDH__INVTID;

    ltid.c.vid_1	= vid_1;
    ltid.c.vid_0	= vid_0;
    ltid.c.must_be_zero = 0;
    ltid.c.cix		= tyg;
    ltid.c.bix		= tix;

  } else {	  /*  Type.  */
    if (tyg > cdh_cMaxTyg || tix > cdh_cMaxTix)
      return CDH__INVTID;

    ltid.t.vid_1	= vid_1;
    ltid.t.vid_0	= vid_0;
    ltid.t.must_be_one	= 1;
    ltid.t.tyg		= tyg;
    ltid.t.tix		= tix;

  }

  *tid = ltid.pwr;
  return CDH__SUCCESS;
}

pwr_tStatus
cdh_StringToVolumeId (
  const char		*s,
  pwr_tVolumeId		*vid
)
{
  unsigned int	vid_0;
  unsigned int	vid_1;
  unsigned int	vid_2;
  unsigned int	vid_3;
  cdh_uVolumeId	lvid;

  pwr_Assert(vid != NULL);

  lvid.pwr = pwr_cNVolumeId;

  if (*s == '_') s++;
  if (*s == 'V' || *s == 'v') s++;
  if (*s == 'O' || *s == 'o') s++;
  if (*s == 'A' || *s == 'a') s++;

  if (sscanf(s, "%d.%d.%d.%d%*s", &vid_3, &vid_2, &vid_1, &vid_0) != 4)
    return CDH__INVVID;

  if (vid_3 > cdh_cMaxVidGroup || vid_2 > cdh_cMaxVidGroup ||
    vid_1 > cdh_cMaxVidGroup || vid_0 > cdh_cMaxVidGroup)
    return CDH__INVVID;

  lvid.v.vid_3 = vid_3;
  lvid.v.vid_2 = vid_2;
  lvid.v.vid_1 = vid_1;
  lvid.v.vid_0 = vid_0;

  *vid = lvid.pwr;

  return CDH__SUCCESS;
}

pwr_tStatus
cdh_StringToObjectIx (
  const char		*s,
  pwr_tObjectIx		*oix
)
{
  pwr_tObjectIx		loix = pwr_cNObjectIx;

  pwr_Assert(oix != NULL);

  if (*s == '_') s++;
  if (*s == 'X' || *s == 'x') s++;

  loix = strtoul(s, NULL, 0);
  if (errno == ERANGE || loix > cdh_cMaxOix)
    return CDH__INVOIX;

  *oix = loix;

  return CDH__SUCCESS;
}

pwr_tStatus
cdh_StringToSubid (
  const char		*s,
  pwr_tSubid		*sid
)
{
  unsigned int	vid_0;
  unsigned int	vid_1;
  unsigned int	vid_2;
  unsigned int	vid_3;
  unsigned int	six;
  cdh_uRefId		lrid;

  pwr_Assert(sid != NULL);

  lrid.pwr = pwr_cNSubid;

  if (*s == '_') s++;
  if (*s == 'S' || *s == 's') s++;

  if (sscanf(s, "%d.%d.%d.%d:%d%*s", &vid_3, &vid_2, &vid_1, &vid_0, &six) != 5)
    return CDH__INVSID;

  if (
    vid_3 != cdh_eVid3_subid || vid_2 > cdh_cMaxVidGroup ||
    vid_1 > cdh_cMaxVidGroup || vid_0 > cdh_cMaxVidGroup
  ) {
    return CDH__INVSID;
  }

  lrid.r.vid_3	= vid_3;
  lrid.r.vid_2	= vid_2;
  lrid.r.vid_1	= vid_1;
  lrid.r.vid_0	= vid_0;
  lrid.r.rix	= six;

  *sid = lrid.pwr;

  return CDH__SUCCESS;
}

pwr_tStatus
cdh_StringToDlid (
  const char		*s,
  pwr_tDlid		*did
)
{
  unsigned int	vid_0;
  unsigned int	vid_1;
  unsigned int	vid_2;
  unsigned int	vid_3;
  unsigned int	dix;
  cdh_uRefId		lrid;

  pwr_Assert(did != NULL);

  lrid.pwr = pwr_cNDlid;

  if (*s == '_') s++;
  if (*s == 'D' || *s == 'd') s++;

  if (sscanf(s, "%d.%d.%d.%d:%d%*s", &vid_3, &vid_2, &vid_1, &vid_0, &dix) != 5)
    return CDH__INVDID;

  if (
    vid_3 != cdh_eVid3_dlid || vid_2 > cdh_cMaxVidGroup ||
    vid_1 > cdh_cMaxVidGroup || vid_0 > cdh_cMaxVidGroup
  ) {
    return CDH__INVDID;
  }

  lrid.r.vid_3 = vid_3;
  lrid.r.vid_2 = vid_2;
  lrid.r.vid_1 = vid_1;
  lrid.r.vid_0 = vid_0;
  lrid.r.rix = dix;

  *did = lrid.pwr;

  return CDH__SUCCESS;
}

/*  Converts a class identifier, 'cid' to a string.
    The output string will be in the format:

    0.1:34  where 0.1 is the volume identifier and
	    34 is the class index within that volume.

    If 's' is non null the resultant string will be catenated
    to 's', otherwise the resultant string will be returned.

    If 'prefix' is not zero, a '_C' prefix will be included in
    the resultant string.  */

char *
cdh_ClassIdToString (
  char			*s,
  pwr_tClassId		cid,
  int			prefix
)
{
  cdh_uTypeId		lcid;
  static char		ls[sizeof("_C255.255:4095??")];

  lcid.pwr = cid;

  if (	/*  Check validity of class identifier. */
    lcid.c.bix != 0 ||
    lcid.c.must_be_zero != 0
  ) {
    sprintf(ls, "%s%u.%u:?%u?", (prefix ? "_C" : ""),
      lcid.c.vid_1, lcid.c.vid_0, lcid.c.cix);
  } else {
    sprintf(ls, "%s%u.%u:%u", (prefix ? "_C" : ""),
      lcid.c.vid_1, lcid.c.vid_0, lcid.c.cix);
  }

  if (s != NULL)
    return strcat(s, ls);
  else
    return ls;
  
}

/*  Converts a type identifier, 'tid' to a string.
    The output string will be in the format:

    0.1:0.34.1	where 0.1 is the volume identifier and
		0.34.1 is the RtBody of class 34 in that
		volume.

    or

    0.1:1.0.3	where 0.1 is the volume identifier and
		1.0.3 is the simple type with index 3 in
		that volume.

    If 's' is non null the resultant string will be catenated
    to 's', otherwise the resultant string will be returned.

    If 'prefix' is not zero, a '_T' prefix will be included in
    the resultant string.  */

char *
cdh_TypeIdToString (
  char			*s,
  pwr_tTypeId		tid,
  int			prefix
)
{
  cdh_uTypeId		ltid;
  static char		lts[sizeof("_T255.255:1.15.2047")];
  static char		lcs[sizeof("_T255.255:0.4095.7")];
  char			*ls;

  ltid.pwr = tid;

  if (ltid.t.must_be_one == 0) {	/*  This is a class TypeId.  */
    sprintf(lcs, "%s%u.%u:0.%u.%u", (prefix ? "_T" : ""),
      ltid.c.vid_1, ltid.c.vid_0, ltid.c.cix, ltid.c.bix);
    ls = lcs;
  } else {  /*	This i s a type TypeId.  */
    sprintf(lts, "%s%u.%u:1.%u.%u", (prefix ? "_T" : ""),
      ltid.t.vid_1, ltid.t.vid_0, ltid.t.tyg, ltid.t.tix);
    ls = lts;
  }

  if (s != NULL)
    return strcat(s, ls);
  else
    return ls;
  
}

/*  Converts a object index, 'oix' to a string.
    The output string will be in the format:

    1234

    If 's' is non null the resultant string will be catenated
    to 's', otherwise the resultant string will be returned.

    If 'prefix' is not zero, a '_X' prefix will be included in
    the resultant string.  */

char *
cdh_ObjectIxToString (
  char			*s,
  pwr_tObjectIx		oix,
  int			prefix
)
{
  static char		ls[sizeof("_X4294967295")];

  sprintf(ls, "%s%u", (prefix ? "_X" : ""), oix);

  if (s != NULL)
    return strcat(s, ls);
  else
    return ls;
}

/*  Converts an attribute reference , 'aref' to a string.
    The output string will be in the format:

    0.1.2.3:1234(_T0.34.1)[34.60]

    If 's' is non null the resultant string will be catenated
    to 's', otherwise the resultant string will be returned.

    If 'prefix' is not zero, a '_A' prefix will be included in
    the resultant string.  */

char *
cdh_ArefToString (
  char			*s,
  pwr_sAttrRef		*aref,
  int			prefix
)
{
  static char		ls[sizeof("_A255.255.255.255:4294967295(_T255.255:1.15.2047)[1234567890.1234567890]")];
  char			tmp[sizeof("[0123456789.01234567890]")];

  sprintf(ls, "%s", (prefix ? "_A" : ""));

  cdh_ObjidToString(ls, aref->Objid, 0);

  if (aref->Body != pwr_cNTypeId) {
    strcat(ls, "(");
    cdh_TypeIdToString(ls, aref->Body, 1);
    strcat(ls, ")");
    if (aref->Offset > 0 || aref->Size > 0) {
      sprintf(tmp, "[%d.%d]", aref->Offset, aref->Size);
      strcat(ls, tmp);
    }
  }

  if (s != NULL)
    return strcat(s, ls);
  else
    return ls;
}

/*  Converts a volume identifier, 'vid' to a string.
    The output string will be in the format:

    0.1.2.3

    If 's' is non null the resultant string will be catenated
    to 's', otherwise the resultant string will be returned.

    If 'prefix' is not zero, a '_V' prefix will be included in
    the resultant string.

    If 'suffix' is not zero, a  ':' suffix will be included in
    the resultant string.  */

char *
cdh_NodeIdToString (
  char			*s,
  pwr_tNodeId		nid,
  int			prefix,
  int			suffix
)
{
  cdh_uVolumeId		lvid;
  static char		ls[sizeof("_N255.255.255.255:")];

  lvid.pwr = (pwr_tVolumeId) nid;

  sprintf(ls, "%s%u.%u.%u.%u%s", (prefix ? "_N" : ""), lvid.v.vid_3,
    lvid.v.vid_2, lvid.v.vid_1, lvid.v.vid_0, (suffix ? ":" : ""));

  if (s != NULL)
    return strcat(s, ls);
  else
    return ls;
  
}

/*  Converts a object identifier, 'oid' to a string.
    The output string will be in the format:

    0.1.2.3:1234

    If 's' is non null the resultant string will be catenated
    to 's', otherwise the resultant string will be returned.

    If 'prefix' is not zero, a '_O' prefix will be included in
    the resultant string.  */

char *
cdh_ObjidToString (
  char			*s,
  pwr_tObjid		oid,
  int			prefix
)
{
  cdh_uObjid		loid;
  static char		ls[sizeof("_O255.255.255.255:4294967295")];

  loid.pwr = oid;

  sprintf(ls, "%s%u.%u.%u.%u:%u", (prefix ? "_O" : ""), loid.o.vid_3,
    loid.o.vid_2, loid.o.vid_1, loid.o.vid_0, loid.o.oix);

  if (s != NULL)
    return strcat(s, ls);
  else
    return ls;
  
}

/*  Converts a volume identifier, 'vid' to a string.
    The output string will be in the format:

    0.1.2.3

    If 's' is non null the resultant string will be catenated
    to 's', otherwise the resultant string will be returned.

    If 'prefix' is not zero, a '_V' prefix will be included in
    the resultant string.

    If 'suffix' is not zero, a  ':' suffix will be included in
    the resultant string.  */

char *
cdh_VolumeIdToString (
  char			*s,
  pwr_tVolumeId		vid,
  int			prefix,
  int			suffix
)
{
  cdh_uVolumeId	lvid;
  static char		ls[sizeof("_V255.255.255.255:")];

  lvid.pwr = vid;

  sprintf(ls, "%s%u.%u.%u.%u%s", (prefix ? "_V" : ""), lvid.v.vid_3,
    lvid.v.vid_2, lvid.v.vid_1, lvid.v.vid_0, (suffix ? ":" : ""));

  if (s != NULL)
    return strcat(s, ls);
  else
    return ls;
  
}

/*  Converts a subscription identifier, 'sid' to a string.
    The output string will be in the format:

    0.1.2.3:1234

    If 's' is non null the resultant string will be catenated
    to 's', otherwise the resultant string will be returned.

    If 'prefix' is not zero, a '_S' prefix will be included in
    the resultant string.  */

char *
cdh_SubidToString (
  char			*s,
  pwr_tSubid		sid,
  int			prefix
)
{
  cdh_uRefId		lrid;
  static char		ls[sizeof("_S255.255.255.255:4294967295")];

  lrid.pwr = sid;

  sprintf(ls, "%s%u.%u.%u.%u:%u", (prefix ? "_S" : ""), lrid.r.vid_3,
    lrid.r.vid_2, lrid.r.vid_1, lrid.r.vid_0, lrid.r.rix);

  if (s != NULL)
    return strcat(s, ls);
  else
    return ls;
  
}

/*  Converts a direct link identifier, 'did' to a string.
    The output string will be in the format:

    0.1.2.3:1234

    If 's' is non null the resultant string will be catenated
    to 's', otherwise the resultant string will be returned.

    If 'prefix' is not zero, a '_D' prefix will be included in
    the resultant string.  */

char *
cdh_DlidToString (
  char			*s,
  pwr_tDlid		did,
  int			prefix
)
{
  cdh_uRefId		lrid;
  static char		ls[sizeof("_D255.255.255.255:4294967295")];

  lrid.pwr = did;

  sprintf(ls, "%s%u.%u.%u.%u:%u", (prefix ? "_D" : ""), lrid.r.vid_3,
    lrid.r.vid_2, lrid.r.vid_1, lrid.r.vid_0, lrid.r.rix);

  if (s != NULL)
    return strcat(s, ls);
  else
    return ls;
  
}

cdh_sFamily *
cdh_Family (
  cdh_sFamily		*f,
  const char		*name,
  pwr_tObjid		poid
)
{

  cdh_ObjName(&f->name, name);
  f->poid = poid;

  return f;
}

cdh_sObjName *
cdh_ObjName (
  cdh_sObjName		*on,
  const char		*name
)
{

  strncpy(on->orig, name, sizeof(pwr_tObjName));
  on->orig[sizeof(pwr_tObjName) - 1] = '\0';
  cdh_ToUpper(on->norm, on->orig);
  on->pack.key = cdh_PackName(on->norm);

  return on;
}

cdh_sParseName *
cdh_ParseName (
  pwr_tStatus		*sts,
  cdh_sParseName	*pn,
  pwr_tObjid		poid,
  const char		*name,
  pwr_tUInt32		flags
)
{
  pwr_tStatus		lsts = 1;
  const char		*inp;
  char			*outp;
  char			*outcp;
  pwr_tUInt32		len;
  pwr_tUInt32		seglen = 0;
  pwr_tUInt32		number = 0;
  unsigned char		c;
  unsigned char		cc;
  char			*segp;
  char			*segcp;
  pwr_tInt32		state;
  cdh_sParseName	ParseName;
  char			origName[256];
  char			normName[256];

  /* Conversion table.
     ! : illegal character
     * : terminates state
     ^ : lowercase, subtract 32 if cdh_mParseName_UpperCase
     + : eat
     - : new segment
     ~ : end of string
     */

  static const char	*cvttab[] = {
  /* 0: init */
"\
~+++++++++++++++++++++++++++++++\
+*******************************\
********************************\
********************************\
++++++++++++++++++++++++++++++++\
********************************\
********************************\
********************************\
",
  /* 1: id() before '(' */
"\
~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!*!!!!-.!0123456789:!!!!!\
!A!CD!!!!!!!!!!O!!!ST!V!X!!!!!!_\
!^!^^!!!!!!!!!!^!!!^^!^!^!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
",
  /* 2: id(bname)attribute[index],  (bvol:Class-b-b-b) before ':' */
"\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!$!!!!!!!!!!!0123456789*!!!!!\
!ABCDEFGHIJKLMNOPQRSTUVWXYZ!!!!_\
!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
",
  /* 3: id(bname)attribute[index],  (bvol:Class-b-b-b) before ')'  */
"\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!$!!!!*!!!-!!0123456789!!!!!!\
!ABCDEFGHIJKLMNOPQRSTUVWXYZ!!!!_\
!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
",
  /* 4: id(bname)attribute[index],  attribute before '['  */
"\
~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!$!!!!!!!!!!!0123456789!!!!!!\
!ABCDEFGHIJKLMNOPQRSTUVWXYZ*!!!_\
!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
¿¡¬√ƒ≈∆«»… ÀÃÕŒœ!—“”‘’÷◊ÿŸ⁄€‹›!!\
^^^^^^^^^^^^^^^^!^^^^^^^^^^^^^!!\
",
  /* 5: id(bname)attribute[index], index before ']'  */
"\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!0123456789!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
",
  /* 6: id(bname)attribute[index], after  ']'  */
"\
~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
",
  /* 7: id(bid)[offset.size], bid before ')'  */
"\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!*!!!!.!0123456789:!!!!!\
!!!!!!!!!!!!!!!!!!!!T!!!!!!!!!!_\
!!!!!!!!!!!!!!!!!!!!^!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
",
  /* 8: id(bid)[offset.size], offset before '.'  */
"\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!*!0123456789!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
",
  /* 9: id(bid)[offset.size], size before ']'  */
"\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!0123456789!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
",
  /* 10: id(bid)[offset.size], after  ']'  */
"\
~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
",
  /* 11: name.attribute[index], name before ':'  */
"\
~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!$!!!!!!!!**!0123456789*!!!!!\
!ABCDEFGHIJKLMNOPQRSTUVWXYZ!!!!_\
!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
¿¡¬√ƒ≈∆«»… ÀÃÕŒœ!—“”‘’÷◊ÿŸ⁄€‹›!!\
^^^^^^^^^^^^^^^^!^^^^^^^^^^^^^!!\
",
  /* 12: name.attribute[index], name before '.'  */
"\
~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!$!!!!!!!!-*!0123456789!!!!!!\
!ABCDEFGHIJKLMNOPQRSTUVWXYZ!!!!_\
!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
¿¡¬√ƒ≈∆«»… ÀÃÕŒœ!—“”‘’÷◊ÿŸ⁄€‹›!!\
^^^^^^^^^^^^^^^^!^^^^^^^^^^^^^!!\
",
  /* 13: name.attribute[index], attribute before '['  */
"\
~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!$!!!!!!!!!!!0123456789!!!!!!\
!ABCDEFGHIJKLMNOPQRSTUVWXYZ*!!!_\
!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
¿¡¬√ƒ≈∆«»… ÀÃÕŒœ!—“”‘’÷◊ÿŸ⁄€‹›!!\
^^^^^^^^^^^^^^^^!^^^^^^^^^^^^^!!\
",
  /* 14: name.attribute[index], index before ']'  */
"\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!0123456789!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
",
  /* 15: name.attribute[index], after  ']'  */
"\
~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
",
"\
~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!$!!!!!!!!-.!0123456789:!!!!!\
!ABCDEFGHIJKLMNOPQRSTUVWXYZ[!]!_\
!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
¿¡¬√ƒ≈∆«»… ÀÃÕŒœ!—“”‘’÷◊ÿŸ⁄€‹›!!\
^^^^^^^^^^^^^^^^!^^^^^^^^^^^^^!!"};

  /* Conversion table.
     ! : do not convert
     any other copy as is.  */

  static const char	ascii7tab[] = {"\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
AAAAAAACEEEEIIII!NOOOOOOOUUUUY!!\
aaaaaaaceeeeiiii!nooooooouuuuy!!\
"};

  len = strlen(name);
  if (len >= sizeof(origName)) {
    *sts = CDH__NAMELEN;
    return NULL;
  }

  if (pn == NULL)
    pn = &ParseName;

  memset(pn, 0, sizeof(*pn));

  if (cdh_ObjidIsNotNull(poid)) {
    pn->poid = poid;
    pn->flags.b.parent = 1;
  }
  pn->parseFlags.m = flags;

  inp = name;
  outp = normName;
  outcp = origName;
  *outp = '\0';
  *outcp = '\0';
  segp = outp;
  segcp = outcp;
  seglen = 0;
  
  state = 0;

  while (inp <= name + len) {
    cc = c = *inp;
    if (cvttab[state][c] == '!') {
      printf("State: %d, %c\n", state, c);
      lsts = CDH__INVCHAR;
      goto error;
    }
    else if (cvttab[state][c] == '^')
      c -= 32;
    else
      c = cvttab[state][c];

    if (pn->parseFlags.b.ascii_7 && ascii7tab[c] != '!') {
      c = ascii7tab[c];
      cc = ascii7tab[cc];
    }

    switch (state) {
    case 0:	/* 0: init */
      if (c == '*') {
	if (*inp == '_') {
	  state++;
	} else if (*inp == '.') {
	  ++inp;
	  state = 13;
	} else if (pn->flags.b.parent) {
	  state = 12;
	} else {
	  state = 11;
	}
      } else if (c == '~') {
	lsts = CDH__NONAME;
	goto error;
      } else if (c == '+') {
	++inp;
      } 
      break;
    case 1:	/* 1: id before '(' */
      if (c == '~') {
	if (outp == segp) {
	  lsts = CDH__INVID;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	pn->flags.b.idString = 1;
	switch (segp[1]) {
	case 'O':
	  lsts = cdh_StringToObjid(segp, &pn->uId.oid);
	  pn->eId = cdh_eId_objid;
	  break;
	case 'C':
	  lsts = cdh_StringToClassId(segp, &pn->uId.cid);
	  pn->eId = cdh_eId_classId;
	  break;
	case 'V':
	  lsts = cdh_StringToVolumeId(segp, &pn->uId.vid);
	  pn->eId = cdh_eId_volumeId;
	  break;
	case 'T':
	  lsts = cdh_StringToTypeId(segp, &pn->uId.tid);
	  pn->eId = cdh_eId_typeId;
	  break;
	case 'A':
	  lsts = cdh_StringToObjid(segp, &pn->uId.aref.Objid);
	  pn->eId = cdh_eId_aref;
	  break;
	case 'S':
	  lsts = cdh_StringToSubid(segp, &pn->uId.sid);
	  pn->eId = cdh_eId_subid;
	  break;
	case 'D':
	  lsts = cdh_StringToDlid(segp, &pn->uId.did);
	  pn->eId = cdh_eId_dlid;
	  break;
	case 'X':
	  lsts = cdh_StringToObjectIx(segp, &pn->uId.oix);
	  pn->eId = cdh_eId_objectIx;
	  break;
	default:
	  lsts = CDH__INVIDCHAR;
	  goto error;
	  break;
	}
	if(EVEN(lsts)) goto error;
	segp = outp;
	segcp = outcp;
	seglen = 0;
      } else if (c == '*') {
	if (outp == segp) {
	  lsts = CDH__INVSYNTAX;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	pn->flags.b.idString = 1;
	switch (segp[1]) {
	case 'A':
	  lsts = cdh_StringToObjid(segp, &pn->uId.aref.Objid);
	  pn->eId = cdh_eId_aref;
	  if (*(inp+1) == '_')
	    state = 7;
	  else
	    state = 2;
	  break;
	default:
	  lsts = CDH__INVIDCHAR;
	  goto error;
	  break;
	}
	if(EVEN(lsts)) {
	  goto error;
	}
	segp = outp;
	segcp = outcp;
	seglen = 0;
      } else if (c == '-') {
	if (outp == segp) {
	  lsts = CDH__INVSYNTAX;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	pn->flags.b.parent = 1;
	switch (segp[1]) {
	case 'A':
	case 'O':
	  lsts = cdh_StringToObjid(segp, &pn->poid);
	  state = 12;
	  break;
	default:
	  lsts = CDH__INVIDCHAR;
	  goto error;
	  break;
	}
	if(EVEN(lsts)) goto error;
	segp = outp;
	segcp = outcp;
	seglen = 0;
      } else {
	*outp++ = c;
	*outcp++ = cc;
	seglen++;
      }
      inp++;
      break;
    case 2:	/* 2: id(bname)attribute[index],  (bvol:Class-b-b-b) before ':' */
      if (c == '*') {
	if (outp == segp) {
	  lsts = CDH__INVTYPENAME;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	if (seglen >= sizeof(pwr_tObjName)) {
	  lsts = CDH__SEGLEN;
	  goto error;
	}
	strcpy(pn->body[pn->nBody].name.orig, segcp);
	strcpy(pn->body[pn->nBody].name.norm, segp);
	pn->body[pn->nBody].name.pack.key = cdh_PackName(segp);
	pn->nBody++;
	segp = outp;
	segcp = outcp;
	seglen = 0;
	state++;
      } else {
	*outp++ = c;
	*outcp++ = cc;
	seglen++;
      }
      inp++;
      break;
    case 3:	/* 3: id(bname)attribute[index],  (bvol:Class-b-b-b) before ')'  */
      if (c == '*') {
	if (outp == segp) {
	  lsts = CDH__INVTYPENAME;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	pn->flags.b.bodyName = 1;
	if (seglen >= sizeof(pwr_tObjName)) {
	  lsts = CDH__SEGLEN;
	  goto error;
	}
	strcpy(pn->body[pn->nBody].name.orig, segcp);
	strcpy(pn->body[pn->nBody].name.norm, segp);
	pn->body[pn->nBody].name.pack.key = cdh_PackName(segp);
	pn->nBody++;
	segp = outp;
	segcp = outcp;
	seglen = 0;
	state++;
      } else if (c == '-') {
	if (outp == segp) {
	  lsts = CDH__INVTYPENAME;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	if (seglen >= sizeof(pwr_tObjName)) {
	  lsts = CDH__SEGLEN;
	  goto error;
	}
	strcpy(pn->body[pn->nBody].name.orig, segcp);
	strcpy(pn->body[pn->nBody].name.norm, segp);
	pn->body[pn->nBody].name.pack.key = cdh_PackName(segp);
	pn->nBody++;
	segp = outp;
	segcp = outcp;
	seglen = 0;
      } else {
	*outp++ = c;
	*outcp++ = cc;
	seglen++;
      }
      inp++;
      break;
    case 4:	/* 4: id(bname)attribute[index],  attribute before '['  */
      if (c == '~') {
	if (outp != segp) {
	*outp++ = '\0';
	*outcp++ = '\0';
	pn->flags.b.attribute = 1;
	if (seglen >= sizeof(pwr_tObjName)) {
	  lsts = CDH__SEGLEN;
	  goto error;
	}
	strcpy(pn->attribute.name.orig, segcp);
	strcpy(pn->attribute.name.norm, segp);
	pn->attribute.name.pack.key = cdh_PackName(segp);
	segp = outp;
	segcp = outcp;
	seglen = 0;
	}
      } else if (c == '*') {
	if (outp == segp) {
	  lsts = CDH__INVATTRNAME;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	pn->flags.b.attribute = 1;
	if (seglen >= sizeof(pwr_tObjName)) {
	  lsts = CDH__SEGLEN;
	  goto error;
	}
	strcpy(pn->attribute.name.orig, segcp);
	strcpy(pn->attribute.name.norm, segp);
	pn->attribute.name.pack.key = cdh_PackName(segp);
	segp = outp;
	segcp = outcp;
	seglen = 0;
	state++;
      } else {
	*outp++ = c;
	*outcp++ = cc;
	seglen++;
      }
      inp++;
      break;
    case 7:	/* 7: id(bid)[offset.size], bid before ')'  */
      if (c == '*') {
	if (outp == segp) {
	  lsts = CDH__INVBID;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	lsts = cdh_StringToTypeId(segp, &pn->bid);
	if (EVEN(lsts)) goto error;	  
	pn->uId.aref.Body = pn->bid;
	pn->flags.b.bodyId = 1;
	segp = outp;
	segcp = outcp;
	seglen = 0;
	if (*(inp+1) == '[') {
	  inp++;
	  state = 8;
	} else {
	  state = 10;
	}
      } else {
	*outp++ = c;
	*outcp++ = cc;
	seglen++;
      }
      inp++;
      break;
    case 11:	/* 11: name.attribute[index], name before ':'  */
      if (c == '~') {
	if (outp == segp) {
	  lsts = CDH__INVCHAR;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	pn->flags.b.object = 1;
	if (seglen >= sizeof(pwr_tObjName)) {
	  lsts = CDH__SEGLEN;
	  goto error;
	}
	strcpy(pn->object[pn->nObject].name.orig, segcp);
	strcpy(pn->object[pn->nObject].name.norm, segp);
	pn->object[pn->nObject].name.pack.key = cdh_PackName(segp);
	pn->nObject++;
	segp = outp;
	segcp = outcp;
	seglen = 0;
      } else if (c == '*') {
	if (outp == segp) {
	  lsts = CDH__INVCHAR;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	if (*inp == ':') {
	  if (seglen >= sizeof(pwr_tObjName)) {
	    lsts = CDH__SEGLEN;
	    goto error;
	  }
	  strcpy(pn->volume.name.orig, segcp);
	  strcpy(pn->volume.name.norm, segp);
	  pn->volume.name.pack.key = cdh_PackName(segp);
	  pn->flags.b.volume = 1;
	  state = 12;
	} else if (*inp == '-') {
	  pn->flags.b.object = 1;
	  if (seglen >= sizeof(pwr_tObjName)) {
	    lsts = CDH__SEGLEN;
	    goto error;
	  }
	  strcpy(pn->object[pn->nObject].name.orig, segcp);
	  strcpy(pn->object[pn->nObject].name.norm, segp);
	  pn->object[pn->nObject].name.pack.key = cdh_PackName(segp);
	  pn->nObject++;
	  state = 12;
	} else if (*inp == '.') {
	  pn->flags.b.object = 1;
	  if (seglen >= sizeof(pwr_tObjName)) {
	    lsts = CDH__SEGLEN;
	    goto error;
	  }
	  strcpy(pn->object[pn->nObject].name.orig, segcp);
	  strcpy(pn->object[pn->nObject].name.norm, segp);
	  pn->object[pn->nObject].name.pack.key = cdh_PackName(segp);
	  pn->nObject++;
	  state = 13;
	}
	segp = outp;
	segcp = outcp;
	seglen = 0;
      } else {
	*outp++ = c;
	*outcp++ = cc;
	seglen++;
      }
      inp++;
      break;
    case 12:	/* 12: name.attribute[index], name before '.'  */
      if (c == '~') {
	if (outp != segp) {
	  *outp++ = '\0';
	  *outcp++ = '\0';
	  pn->flags.b.object = 1;
	  if (seglen >= sizeof(pwr_tObjName)) {
	    lsts = CDH__SEGLEN;
	    goto error;
	  }
	  strcpy(pn->object[pn->nObject].name.orig, segcp);
	  strcpy(pn->object[pn->nObject].name.norm, segp);
	  pn->object[pn->nObject].name.pack.key = cdh_PackName(segp);
	  pn->nObject++;
	  segp = outp;
	  segcp = outcp;
	  seglen = 0;
	}
      } else if (c == '*') {
	if (outp == segp) {
	  lsts = CDH__INVCHAR;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	pn->flags.b.object = 1;
	  if (seglen >= sizeof(pwr_tObjName)) {
	    lsts = CDH__SEGLEN;
	    goto error;
	  }
	strcpy(pn->object[pn->nObject].name.orig, segcp);
	strcpy(pn->object[pn->nObject].name.norm, segp);
	pn->object[pn->nObject].name.pack.key = cdh_PackName(segp);
	pn->nObject++;
	segp = outp;
	segcp = outcp;
	seglen = 0;
	state = 13;
      } else if (c == '-') {
	if (outp == segp) {
	  lsts = CDH__INVCHAR;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	pn->flags.b.object = 1;
	  if (seglen >= sizeof(pwr_tObjName)) {
	    lsts = CDH__SEGLEN;
	    goto error;
	  }
	strcpy(pn->object[pn->nObject].name.orig, segcp);
	strcpy(pn->object[pn->nObject].name.norm, segp);
	pn->object[pn->nObject].name.pack.key = cdh_PackName(segp);
	pn->nObject++;
	segp = outp;
	segcp = outcp;
	seglen = 0;
      } else {
	*outp++ = c;
	*outcp++ = cc;
	seglen++;
      }
      inp++;
      break;
    case 13:	/* 13: name.attribute[index], attribute before '['  */
      if (c == '~') {
	if (outp == segp) {
	  lsts = CDH__INVATTRNAME;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	pn->flags.b.attribute = 1;
	  if (seglen >= sizeof(pwr_tObjName)) {
	    lsts = CDH__SEGLEN;
	    goto error;
	  }
	strcpy(pn->attribute.name.orig, segcp);
	strcpy(pn->attribute.name.norm, segp);
	pn->attribute.name.pack.key = cdh_PackName(segp);
	segp = outp;
	segcp = outcp;
	seglen = 0;
      } else if (c == '*') {
	if (outp == segp) {
	  lsts = CDH__INVATTRNAME;
	  goto error;
	}
	*outp++ = '\0';
	*outcp++ = '\0';
	pn->flags.b.attribute = 1;
	  if (seglen >= sizeof(pwr_tObjName)) {
	    lsts = CDH__SEGLEN;
	    goto error;
	  }
	strcpy(pn->attribute.name.orig, segcp);
	strcpy(pn->attribute.name.norm, segp);
	pn->attribute.name.pack.key = cdh_PackName(segp);
	segp = outp;
	segcp = outcp;
	seglen = 0;
	state++;
      } else {
	*outp++ = c;
	*outcp++ = cc;
	seglen++;
      }
      inp++;
      break;
    case 5:	/* 5: id(bname)attribute[index], index before ']'  */
    case 8:	/* 8: id(bid)[offset.size], offset before '.'  */
    case 9:	/* 9: id(bid)[offset.size], size before ']'  */
    case 14:	/* 14: name.attribute[index], index before ']'  */
      if (c == '*') {
	switch (state) {
	case 5:
	case 14:
	  pn->index = number;
	  pn->flags.b.index = 1;
	  break;
	case 8:
	  pn->offset = number;
	  pn->uId.aref.Offset = pn->offset;
	  break;
	case 9:
	  pn->size = number;
	  pn->uId.aref.Size = pn->size;
	  pn->flags.b.index = 1;
	  break;
	}
	number = 0;
	state++;
      } else {
	number = number * 10 + (c - '0');
	if (number > 65535) {
	  lsts = CDH__INVATTRINDEX;
	  goto error;
	}
      }
      inp++;
      break;
    case 6:	/* 6:  id(bname)attribute[index], after  ']'  */
    case 10:	/* 10: id(bid)[offset.size], after  ']'  */
    case 15:	/* 15: name.attribute[index], after  ']'  */
      state = -1;
      inp++;
      break;
    }
  }

  if (pn->nObject > 1) pn->flags.b.path = 1;
  *sts = lsts;
  return pn;

error:
  *sts = lsts;
  return NULL;
}

pwr_tUInt32
cdh_PackName (
  const char		*name
)
{
  cdh_uPackName		pack;
  unsigned int		len = strlen(name);
  const char		*s = name;
  unsigned char		hash = 0;

  while (*s)
    hash = (hash*3) ^ *s++;
  
  pack.c.len = len;
  pack.c.hash = hash;
  pack.c.first = name[0];
  pack.c.last = name[len-1];
  
  return pack.key;  
}

char *
cdh_Low (
  const char		*s
)
{
  static char buf[500];

  return cdh_ToLower(buf, s);
}

char *
cdh_ToLower (
  char			*t,
  const char		*s
)
{
  char			*rs = t;
  static const char	cvttab[] = "\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!\
!abcdefghijklmnopqrstuvwxyz!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
^^^^^^^^^^^^^^^^!^^^^^^^^^^^^^!!\
‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔ!ÒÚÛÙıˆ˜¯˘˙˚¸˝!!";

  if (t == NULL) return NULL;
  if (s == NULL) s = t;

  while (*s)
    if (cvttab[(unsigned char)*s] == '^')
      *t++ = (*s++) + 32;
    else
      *t++ = *s++; 

  *t = *s;  /* Copy the null byte.  */

  return rs;
}

char *
cdh_ToUpper (
  char			*t,
  const char		*s
)
{
  char			*rs = t;
  static const char     cvttab[] = "\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!ABCDEFGHIJKLMNOPQRSTUVWXYZ!!!!!\
!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
¿¡¬√ƒ≈∆«»… ÀÃÕŒœ!—“”‘’÷◊ÿŸ⁄€‹›!!\
^^^^^^^^^^^^^^^^!^^^^^^^^^^^^^!!";

  if (t == NULL) return NULL;
  if (s == NULL) s = t;

  while (*s)
    if (cvttab[(unsigned char)*s] == '^')
      *t++ = (*s++) - 32;
    else
      *t++ = *s++; 

  *t = *s;  /* Copy the null byte.  */

  return rs;
}

/* Compare two strings not regarding their casing.

   This routine works only on alphabetic characters.
   It works on the standard ascii a-z and on the 
   DEC multinational extensions.  

   The function exploits the fact that only bit 5 changes
   when you change the case of a character.

   The function returns the uppercase offset between
   the two first differing characters.  */

int
cdh_NoCaseStrcmp (
  const char		*s,
  const char		*t
)
{

  while (*s && *t && !(((*s) ^ (*t)) & ~(1<<5)))
    s++, t++; 

  return ((*s) & ~(1<<5)) - ((*t) & ~(1<<5));
}
