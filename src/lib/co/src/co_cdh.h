#ifndef co_cdh_h
#define co_cdh_h

/* co_cdh.h -- class definition handler

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This include file contains definitions and function prototypes
   needed to use CDH.  */

#include <limits.h>

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#define cdh_cMaxVidGroup	 255
#define cdh_cMaxCix		4095
#define cdh_cMaxBix		   7
#define cdh_cMaxTix		2047
#define cdh_cMaxTyg		   1
#define cdh_cMaxOix	   ULONG_MAX

typedef enum {
  cdh_eVid3_local = 1,
  cdh_eVid3_subid = 128,
  cdh_eVid3_dlid  = 129,
  cdh_eVid3_qid   = 130
} cdh_eVId3;


#define cdh_CidToVid(cid) ((cid) >> 16)
#define cdh_TidToVid(tid) ((tid) >> 16)
#define cdh_cixToCid( Vid, Cix) (0 + (Vid << 16) +  (Cix << 3))
#define cdh_tixToTid( Vid, Tyg, Tix) (0 + (Vid << 16) + (1 << 15) + (Tyg << 11) +  Tix)
#define cdh_cixToOix( Cix, Bix, Aix) (0 + (1 << 31) + (Cix << 18) + (Bix << 15) + Aix)
#define cdh_tixToOix( Tyg, Tix) (0 + (1 << 31) + (1 << 30) + (Tyg << 26) + (Tix << 15))
#define cdh_oixToBix( Oix) ((pwr_eBix)((Oix >> 15) & 7))
#define cdh_oixToCix( Oix) ((Oix >> 18) & 0xfff)
#define cdh_oixToAix( Oix) (Oix & 0xfff)

typedef struct {
  unsigned int	oix		: 32;

#if (pwr_dHost_byteOrder == pwr_dBigEndian)

  unsigned int	vid_3		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_0		: 8;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

  unsigned int	vid_0		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_3		: 8;

#endif
} cdh_mOid;
typedef cdh_mOid cdh_mObjid;
    
typedef struct {
  unsigned int	rix		: 32;

#if (pwr_dHost_byteOrder == pwr_dBigEndian)

  unsigned int	vid_3		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_0		: 8;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

  unsigned int	vid_0		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_3		: 8;

#endif
} cdh_mRid;
typedef cdh_mRid cdh_mRefId;

typedef struct {
#if (pwr_dHost_byteOrder == pwr_dBigEndian)

  unsigned int	vid_3		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_0		: 8;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

  unsigned int	vid_0		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_3		: 8;

#endif
} cdh_mVid;
typedef cdh_mVid cdh_mVolumeId;

typedef struct {
#if (pwr_dHost_byteOrder == pwr_dBigEndian)

  unsigned int	nid_3		: 8;
  unsigned int	nid_2		: 8;
  unsigned int	nid_1		: 8;
  unsigned int	nid_0		: 8;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

  unsigned int	nid_0		: 8;
  unsigned int	nid_1		: 8;
  unsigned int	nid_2		: 8;
  unsigned int	nid_3		: 8;

#endif
} cdh_mNid;
typedef cdh_mNid cdh_mNodeId;

typedef struct {
#if (pwr_dHost_byteOrder == pwr_dBigEndian)

  unsigned int  must_be_two : 2;
  unsigned int	cix         : 12;
  unsigned int	bix         : 3;
  unsigned int	reserved    : 3;
  unsigned int	aix         : 12;

  unsigned int	vid_3       : 8;
  unsigned int	vid_2       : 8;
  unsigned int	vid_1       : 8;
  unsigned int	vid_0       : 8;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

  unsigned int	aix         : 12;
  unsigned int	reserved    : 3;
  unsigned int	bix         : 3;
  unsigned int	cix         : 12;
  unsigned int  must_be_two : 2;

  unsigned int	vid_0		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_3		: 8;

#endif
} cdh_mClassObjid;

typedef struct {
#if (pwr_dHost_byteOrder == pwr_dBigEndian)

  unsigned int	vid_1		: 8;
  unsigned int	vid_0		: 8;

  unsigned int  must_be_zero	: 1;
  unsigned int	cix		: 12;
  unsigned int	bix		: 3;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

  unsigned int	bix		: 3;
  unsigned int	cix		: 12;
  unsigned int  must_be_zero	: 1;

  unsigned int	vid_0		: 8;
  unsigned int	vid_1		: 8;

#endif
} cdh_mCid;
typedef cdh_mCid cdh_mClassId;

typedef struct {
#if (pwr_dHost_byteOrder == pwr_dBigEndian)

  unsigned int	must_be_three	: 2;
  unsigned int	tyg		: 4;
  unsigned int	tix		: 11;
  unsigned int	reserved	: 3;
  unsigned int	aix		: 12;

  unsigned int	vid_3		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_0		: 8;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

  unsigned int	aix		: 12;
  unsigned int	reserved	: 3;
  unsigned int	tix		: 11;
  unsigned int	tyg		: 4;
  unsigned int	must_be_three	: 2;

  unsigned int	vid_0		: 8;
  unsigned int	vid_1		: 8;
  unsigned int	vid_2		: 8;
  unsigned int	vid_3		: 8;

#endif
} cdh_mTypeObjid;

typedef struct {
#if (pwr_dHost_byteOrder == pwr_dBigEndian)

  unsigned int	vid_1		: 8;
  unsigned int	vid_0		: 8;

  unsigned int	must_be_one	: 1;
  unsigned int	tyg		: 4;
  unsigned int	tix		: 11;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

  unsigned int	tix		: 11;
  unsigned int	tyg		: 4;
  unsigned int	must_be_one	: 1;

  unsigned int	vid_0		: 8;
  unsigned int	vid_1		: 8;

#endif
} cdh_mTid;
typedef cdh_mTid cdh_mTypeId;

typedef union {
  pwr_tOid		pwr;
  cdh_mObjid		o;
  cdh_mClassObjid	c;
  cdh_mTypeObjid	t;
} cdh_uOid;
typedef cdh_uOid cdh_uObjid;

typedef union {
  pwr_tRid pwr;
  cdh_mRid r;
} cdh_uRid;
typedef cdh_uRid cdh_uRefId;

typedef union {
  pwr_tCid pwr;
  cdh_mCid c;
  cdh_mTid t;
} cdh_uTid;
typedef cdh_uTid cdh_uTypeId;

typedef union {
  pwr_tVid pwr;
  cdh_mVid v;
} cdh_uVid;
typedef cdh_uVid cdh_uVolumeId;

typedef union {
  pwr_tNid pwr;
  cdh_mNid n;
} cdh_uNid;
typedef cdh_uNid cdh_uNodeId;

typedef enum {
  cdh_eId__ = 0,
  cdh_eId_objectIx,
  cdh_eId_objid,
  cdh_eId_classId,
  cdh_eId_volumeId,
  cdh_eId_typeId,
  cdh_eId_subid,
  cdh_eId_dlid,
  cdh_eId_aref,
  cdh_eId_
} cdh_eId;

typedef union {
  pwr_tOix     oix;
  pwr_tOid     oid;
  pwr_tCid     cid;
  pwr_tVid     vid;
  pwr_tTid     tid;
  pwr_tSubid   sid;
  pwr_tDlid    did;
  pwr_sAttrRef aref;
} cdh_uId;

typedef union {
  pwr_tUInt32		key;
  struct {
#if (pwr_dHost_byteOrder == pwr_dBigEndian)

    char		last;
    char		first;
    pwr_tUInt8		hash;
    pwr_tUInt8		len;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

    pwr_tUInt8		len;
    pwr_tUInt8		hash;
    char		first;
    char		last;

#endif
  } c;
} cdh_uPackName;

typedef struct {
  pwr_tObjName		orig;
  pwr_tObjName		norm;
  cdh_uPackName		pack;
} cdh_sObjName;

typedef struct {
  cdh_sObjName		name;
  pwr_tOid		poid;
} cdh_sFamily;

typedef union {
  struct {
#if (pwr_dHost_byteOrder == pwr_dBigEndian)

    pwr_tBit		fill		: 31;
    
    pwr_tBit		ascii_7		: 1;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

    pwr_tBit		ascii_7		: 1;
    
    pwr_tBit		fill		: 31;

#endif
  } b;
  pwr_tBitMask		m;

#define cdh_mParseName__		0
#define cdh_mParseName_ascii_7		pwr_Bit(0)
#define cdh_mParseName_			(~cdh_mParseName__)
} cdh_mParseName;

/* Describe the format of a namestring.

   Let us assume we have an object of class Ai.
   The object has an attribute called FilterAttribute.

   Object name:	Eobj
   Object id  : 1234567890
   Class name :	pwrb:Class-Ai
   Class id   : 0.2:34
   Volume name:	Avol
   Volume id  : 0.123.34.63
   Parents    : Bobj, Cobj, Dobj
   Attribute  : FilterAttribute
   Index      : 2
   Offset     : 60
   Size	      : 4
   Body name  :	pwrb:Class-Ai-RtBody
   Body id    : 0.2:0.34.1

   The name of this object can be written in different ways.
   The type cdh_mName is used to define the way an object is named.

   V P O B B A I E S  I   Form  Fallback        String
   o a b o o t n s e  d   
   l t j d d t d c p  T 
   u h e y y r e a a  y 
   m   c I N i x p r  p 
   e   t d a b   e a  e 
           m u   G t    
           e t   M o    
             e   S r    

   1 * * * * * * * 0  1   Id    *               _V0.123.34.63
   1 * * * * * * * 1  1   Id    *               _V0.123.34.63:
   1 * * * * * * * 0  0   Id    *                 0.123.34.63
   1 * * * * * * * 1  0   Id    *                 0.123.34.63:

   0 * 1 * * * * * *  1   Id    *               _O0.123.34.63:1234567890
   0 * 1 * * * * * *  0   Id    *                 0.123.34.63:1234567890

   0 * 0 1 * 1 0 * *  *   Id    *               _A0.123.34.63:1234567890(_T0.2:0.34.1)
   0 * 0 1 * 1 1 * *  *   Id    *               _A0.123.34.63:1234567890(_T0.2:0.34.1)[60.4]

   1 * * * * * * * *  *   Std   Export          _V0.123.34.63:
   0 0 0 * * 1 * * *  *   Std   Export          _O0.123.34.63:1234567890
   0 0 0 0 1 1 * * *  *   Std   Export          _A0.123.34.63:1234567890(pwrb:Class-Ai-RtBody)FilterAttribute[2]

   1 1 1 0 0 1 1 0 *  *   Std   Strict          Avol:Bobj-Cobj-Dobj-Eobj.FilterAttribute[2]
   0 1 1 0 0 1 1 0 *  *   Std   Strict               Bobj-Cobj-Dobj-Eobj.FilterAttribute[2]
   0 0 1 0 0 1 1 0 *  *   Std   Strict                              Eobj.FilterAttribute[2]
   0 0 0 0 0 1 1 0 0  *   Std   Strict                                   FilterAttribute[2]
   0 0 0 0 0 1 1 0 1  *   Std   Strict                                  .FilterAttribute[2]
   0 0 0 0 0 1 0 0 0  *   Std   Strict                                   FilterAttribute
   0 0 0 0 0 1 0 0 1  *   Std   Strict                                  .FilterAttribute
   1 1 1 0 0 1 0 0 *  *   Std   Strict          Avol:Bobj-Cobj-Dobj-Eobj.FilterAttribute
   1 1 1 0 0 0 0 0 0  *   Std   Strict          Avol:Bobj-Cobj-Dobj-Eobj
   1 1 1 0 0 0 0 0 1  *   Std   Strict          Avol:Bobj-Cobj-Dobj-Eobj-
   1 1 0 0 0 0 0 0 0  *   Std   Strict          Avol:Bobj-Cobj-Dobj
   1 1 0 0 0 0 0 0 1  *   Std   Strict          Avol:Bobj-Cobj-Dobj-
   1 0 0 0 0 0 0 0 0  *   Std   Strict          Avol
   1 0 0 0 0 0 0 0 1  *   Std   Strict          Avol:
   
   1 1 1 0 0 1 1 1 0  *   Std   Strict          Avol\:Bobj\-Cobj\-Dobj\-Eobj\.FilterAttribute[2]
   0 1 1 0 0 1 1 1 0  *   Std   Strict                Bobj\-Cobj\-Dobj\-Eobj\.FilterAttribute[2]
   0 0 1 0 0 1 1 1 0  *   Std   Strict                                  Eobj\.FilterAttribute[2]
   0 0 0 0 0 1 1 1 0  *   Std   Strict                                        FilterAttribute[2]
   1 1 1 0 0 1 0 1 0  *   Std   Strict          Avol\:Bobj\-Cobj\-Dobj\-Eobj\.FilterAttribute
   1 1 1 0 0 0 0 1 0  *   Std   Strict          Avol\:Bobj\-Cobj\-Dobj\-Eobj
   1 1 0 0 0 0 0 1 0  *   Std   Strict          Avol\:Bobj\-Cobj\-Dobj
   1 0 0 0 0 0 0 1 0  *   Std   Strict          Avol
   
   1 1 1 0 0 1 1 0 *  *   Root  Strict          //Avol/Bobj/Cobj/Dobj/Eobj.FilterAttribute[2]
   0 1 1 0 0 1 1 0 *  *   Root  Strict                /Bobj/Cobj/Dobj/Eobj.FilterAttribute[2]
   0 0 1 0 0 1 1 0 *  *   Root  Strict                                Eobj.FilterAttribute[2]
   0 0 0 0 0 1 1 0 0  *   Root  Strict                                     FilterAttribute[2]
   1 1 1 0 0 1 0 0 0  *   Root  Strict          //Avol/Bobj/Cobj/Dobj/Eobj.FilterAttribute
   1 1 1 0 0 0 0 0 0  *   Root  Strict          //Avol/Bobj/Cobj/Dobj/Eobj
   1 1 1 0 0 0 0 0 1  *   Root  Strict          //Avol/Bobj/Cobj/Dobj/Eobj/
   1 1 0 0 0 0 0 0 0  *   Root  Strict          //Avol/Bobj/Cobj/Dobj
   1 1 0 0 0 0 0 0 1  *   Root  Strict          //Avol/Bobj/Cobj/Dobj/
   1 0 0 0 0 0 0 0 0  *   Root  Strict          //Avol
   1 0 0 0 0 0 0 0 1  *   Root  Strict          //Avol/
   
   */

typedef union {
  pwr_tBitMask		m;
  struct {
#if (pwr_dHost_byteOrder == pwr_dBigEndian)

    pwr_tBit		fallback	: 8;

    pwr_tBit		form		: 8;

    pwr_tBit		fill		: 5;
    pwr_tBit		parent		: 1;
    pwr_tBit		idString	: 1;
    pwr_tBit		separator	: 1;

    pwr_tBit		escapeGMS	: 1;
    pwr_tBit		index		: 1;
    pwr_tBit		attribute	: 1;
    pwr_tBit		bodyName	: 1;
    pwr_tBit		bodyId		: 1;
    pwr_tBit		object		: 1;
    pwr_tBit		path		: 1;
    pwr_tBit		volume		: 1;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

    pwr_tBit		volume		: 1;
    pwr_tBit		path		: 1;
    pwr_tBit		object		: 1;
    pwr_tBit		bodyId		: 1;
    pwr_tBit		bodyName	: 1;
    pwr_tBit		attribute	: 1;
    pwr_tBit		index		: 1;
    pwr_tBit		escapeGMS	: 1;

    pwr_tBit		separator	: 1;
    pwr_tBit		idString	: 1;
    pwr_tBit		parent		: 1;
    pwr_tBit		fill		: 5;

    pwr_tBit		form		: 8;

    pwr_tBit		fallback	: 8;

#endif
  } b;
  struct {
#if (pwr_dHost_byteOrder == pwr_dBigEndian)

    pwr_tUInt8		fallback;
    pwr_tUInt8		form;
    pwr_tUInt16		bits;

#elif (pwr_dHost_byteOrder == pwr_dLittleEndian)

    pwr_tUInt16		bits;
    pwr_tUInt8		form;
    pwr_tUInt8		fallback;

#endif
  } e;

#define cdh_mNName 0 
#define	cdh_mName__			0
#define	cdh_mName_volume		pwr_Bit(0)
#define	cdh_mName_path			pwr_Bit(1)
#define	cdh_mName_object		pwr_Bit(2)
#define	cdh_mName_bodyId		pwr_Bit(3)
#define	cdh_mName_bodyName		pwr_Bit(4)
#define	cdh_mName_attribute		pwr_Bit(5)
#define	cdh_mName_index			pwr_Bit(6)
#define cdh_mName_escapeGMS		pwr_Bit(7)
#define cdh_mName_separator		pwr_Bit(8)
#define cdh_mName_idString		pwr_Bit(9)
#define cdh_mName_parent		pwr_Bit(10)
#define	cdh_mName_			(~cdh_mName__)

#define cdh_mName_eForm_std		1
#define cdh_mName_eForm_root		2
#define cdh_mName_eForm_id		3

# define cdh_mName_form_std		pwr_SetByte(2, cdh_mName_eForm_std)
# define cdh_mName_form_root		pwr_SetByte(2, cdh_mName_eForm_root)
# define cdh_mName_form_id		pwr_SetByte(2, cdh_mName_eForm_id)

#define  cdh_mName_eFallback_bestTry	1
#define  cdh_mName_eFallback_strict	2
#define  cdh_mName_eFallback_export	3
#define  cdh_mName_eFallback_volumeDump	4

#define  cdh_mName_fallback_bestTry	pwr_SetByte(3, cdh_mName_eFallback_bestTry)
#define  cdh_mName_fallback_strict	pwr_SetByte(3, cdh_mName_eFallback_strict)
#define  cdh_mName_fallback_export	pwr_SetByte(3, cdh_mName_eFallback_export)
#define  cdh_mName_fallback_volumeDump	pwr_SetByte(3, cdh_mName_eFallback_volumeDump)

#define cdh_mName_pathBestTry		(cdh_mName_path | cdh_mName_object |cdh_mName_attribute |\
					 cdh_mName_index | cdh_mName_form_std | cdh_mName_Fallback_bestTry)
#define cdh_mName_volumeBestTry		(cdh_mName_volume | cdh_mName_path | cdh_mName_object |cdh_mName_attribute |\
                                         cdh_mName_index | cdh_mName_form_std | cdh_mName_fallback_bestTry)
#define cdh_mName_pathStrict		(cdh_mName_path | cdh_mName_object |cdh_mName_attribute |\
                                         cdh_mName_index | cdh_mName_form_std | cdh_mName_fallback_strict)
#define cdh_mName_volumeStrict		(cdh_mName_volume | cdh_mName_path | cdh_mName_object |cdh_mName_attribute |\
                                         cdh_mName_index | cdh_mName_form_std | cdh_mName_fallback_strict)
} cdh_mName;

typedef struct {
  pwr_tOid		 poid;		/* Parent objid, or NOBJID */
  cdh_mParseName parseFlags;

  cdh_mName		 flags;
  void			 *ohp;
  cdh_eId		 eId;
  cdh_uId		 uId;
  pwr_tTid		 bid;
  pwr_tUInt32    index;
  pwr_tUInt32    offset;
  pwr_tUInt32    size;
  pwr_tUInt32    nObject;
  pwr_tUInt32    nBody;
  cdh_sFamily    volume;
  cdh_sFamily    object[20];
  cdh_sFamily    body[10];
  cdh_sFamily    attribute;
} cdh_sParseName;

/*  Function prototypes to exported functions.  */

int
cdh_ObjidCompare (
  pwr_tOid Object_1,
  pwr_tOid Object_2
);

int
cdh_ObjidIsEqual (
  pwr_tOid Object_1,
  pwr_tOid Object_2
);

int
cdh_ObjidIsNotEqual (
  pwr_tOid Object_1,
  pwr_tOid Object_2
);

int
cdh_ObjidIsNull (
  pwr_tOid Object
);

int
cdh_ObjidIsNotNull (
  pwr_tOid Object
);

int
cdh_SubidCompare (
  pwr_tSubid Subscription_1,
  pwr_tSubid Subscription_2
);

int
cdh_SubidIsEqual (
  pwr_tSubid Subscription_1,
  pwr_tSubid Subscription_2
);

int
cdh_SubidIsNotEqual (
  pwr_tSubid Subscription_1,
  pwr_tSubid Subscription_2
);

int
cdh_SubidIsNull (
  pwr_tSubid Subscription
);

int
cdh_SubidIsNotNull (
  pwr_tSubid Subscription
);

int
cdh_RefIdCompare (
  pwr_tRefId Reference_1,
  pwr_tRefId Reference_2
);

int
cdh_RefIdIsEqual (
  pwr_tRefId Reference_1,
  pwr_tRefId Reference_2
);

int
cdh_RefIdIsNotEqual (
  pwr_tRefId Reference_1,
  pwr_tRefId Reference_2
);

int
cdh_RefIdIsNull (
  pwr_tRefId Reference
);

int
cdh_RefIdIsNotNull (
  pwr_tRefId Reference
);

int
cdh_DlidCompare (
  pwr_tDlid DirectLink_1,
  pwr_tDlid DirectLink_2
);

int
cdh_DlidIsEqual (
  pwr_tDlid DirectLink_1,
  pwr_tDlid DirectLink_2
);

int
cdh_DlidIsNotEqual (
  pwr_tDlid DirectLink_1,
  pwr_tDlid DirectLink_2
);

int
cdh_DlidIsNull (
  pwr_tDlid DirectLink
);

int
cdh_DlidIsNotNull (
  pwr_tDlid DirectLink
);

pwr_tCid
cdh_ClassObjidToId (
  pwr_tOid Object
);

pwr_tOid
cdh_ClassIdToObjid (
  pwr_tCid Class
);

pwr_tTid
cdh_TypeObjidToId (
  pwr_tOid Object
);

int
cdh_TypeIdToIndex (
  pwr_tTid Type
);

pwr_tOid
cdh_TypeIdToObjid (
  pwr_tTid Type
);

pwr_tStatus
cdh_AttrValueToString (
  pwr_eType Type,
  void      *Value,
  char      *String,
  int       MaxSize
);

pwr_tStatus
cdh_StringToAttrValue (
  pwr_eType		Type,
  const char		*String,
  void			*Value
);

pwr_tStatus
cdh_StringToClassId (
  const char		*s,
  pwr_tCid		*cid
);

pwr_tStatus
cdh_StringToTypeId (
  const char		*s,
  pwr_tTid		*tid
);

pwr_tStatus
cdh_StringToVolumeId (
  const char		*s,
  pwr_tVid		*tid
);

pwr_tStatus
cdh_StringToObjectIx (
  const char		*s,
  pwr_tOix		*oix
);

pwr_tStatus
cdh_StringToObjid (
  const char    *s,
  pwr_tOid		*oid
);

pwr_tStatus
cdh_StringToSubid (
  const char		*s,
  pwr_tSubid		*sid
);

pwr_tStatus
cdh_StringToDlid (
  const char    *s,
  pwr_tDlid		*did
);

char *
cdh_ClassIdToString (
  char			*s,
  pwr_tCid  cid,
  int			prefix
);

char *
cdh_ObjectIxToString (
  char			*s,
  pwr_tOix		oix,
  int			prefix
);

char *
cdh_ObjidToString (
  char			*s,
  pwr_tOid		oid,
  int			prefix
);

char *
cdh_ArefToString (
  char			*s,
  pwr_sAttrRef  *aref,
  int			prefix
);

char *
cdh_NodeIdToString (
  char     *s,
  pwr_tNid nid,
  int      prefix,
  int      suffix
);

char *
cdh_TypeIdToString(char *s, pwr_tTid tid, int prefix);

char *
cdh_VolumeIdToString (
    char     *s,
    pwr_tVid vid,
    int      prefix,
    int      suffix
    );

char *
cdh_SubidToString (
  char			*s,
  pwr_tSubid    sid,
  int			prefix
);

char *
cdh_DlidToString (
  char			*s,
  pwr_tDlid		did,
  int			prefix
);

cdh_sFamily *
cdh_Family (
  cdh_sFamily		*f,
  const char		*name,
  pwr_tOid		    poid
);

cdh_sObjName *
cdh_ObjName (
  cdh_sObjName		*on,
  const char		*name
);

pwr_tUInt32
cdh_PackName (
  const char		*name
);

cdh_sParseName *
cdh_ParseName (
  pwr_tStatus		*sts,
  cdh_sParseName	*pn,
  pwr_tOid		    poid,
  const char		*name,
  pwr_tUInt32		flags
);

char *
cdh_Low (
  const char		*s
);

char *
cdh_ToLower (
  char			*t,
  const char		*s
);

char *
cdh_ToUpper (
  char			*t,
  const char		*s
);

int
cdh_NoCaseStrcmp (
  const char		*s,
  const char		*t
);

#ifdef __cplusplus
}
#endif

#endif
