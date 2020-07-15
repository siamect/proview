/*
  Test gdh functions.
*/

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "pwr_class.h"
#include "co_msg.h"
#include "co_dcli.h"
#include "rt_gdh.h"
#include "ra_cdhtest.h"
#include "co_cdh_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvolpwrtest01classes.h"


typedef struct {
  pwr_tOid oid1;
  pwr_tOid oid2;
  int sts;
} sObjidCompare;

void ra_cdhtest::ObjidCompare(void)
{
  sObjidCompare d[] = {
    {pwr_cNOid, pwr_cNOid, 0},
    {{11111,1}, {11111,1}, 0},
    {{11112,1}, {11111,1}, 1},
    {{11111,1}, {11112,1}, -1},
    {{11112,2}, {11111,1}, 1},
    {{11111,2}, {11112,1}, 1},
    {{11112,1}, {11111,2}, -1},
    {{11111,1}, {11112,2}, -1}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = cdh_ObjidCompare(d[i].oid1, d[i].oid2);
    if (sts != d[i].sts) {
      m_log->log('E', "ObjidCompare", i);
      return;
    }
  }

  m_log->log('S', "ObjidCompare", CDH__SUCCESS);
}

void ra_cdhtest::ObjidIsEqual(void)
{
  sObjidCompare d[] = {
    {pwr_cNOid, pwr_cNOid, 1},
    {{11111,1}, {11111,1}, 1},
    {{11112,1}, {11111,1}, 0},
    {{11111,1}, {11112,1}, 0},
    {{11112,2}, {11111,1}, 0},
    {{11111,2}, {11112,1}, 0},
    {{11112,1}, {11111,2}, 0},
    {{11111,1}, {11112,2}, 0}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = cdh_ObjidIsEqual(d[i].oid1, d[i].oid2);
    if (sts != d[i].sts) {
      m_log->log('E', "ObjidIsEqual", i);
      return;
    }
  }

  m_log->log('S', "ObjidIsEqual", CDH__SUCCESS);
}

void ra_cdhtest::ObjidIsNotEqual(void)
{
  sObjidCompare d[] = {
    {pwr_cNOid, pwr_cNOid, 0},
    {{11111,1}, {11111,1}, 0},
    {{11112,1}, {11111,1}, 1},
    {{11111,1}, {11112,1}, 1},
    {{11112,2}, {11111,1}, 1},
    {{11111,2}, {11112,1}, 1},
    {{11112,1}, {11111,2}, 1},
    {{11111,1}, {11112,2}, 1}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = cdh_ObjidIsNotEqual(d[i].oid1, d[i].oid2);
    if (sts != d[i].sts) {
      m_log->log('E', "ObjidIsNotEqual", i);
      return;
    }
  }

  m_log->log('S', "ObjidIsNotEqual", CDH__SUCCESS);
}

typedef struct {
  pwr_tOid oid;
  int sts;
} sObjidIsNull;

void ra_cdhtest::ObjidIsNull(void)
{
  sObjidIsNull d[] = {
    {pwr_cNOid, 1},
    {{11111,1}, 0},
    {{11112,1}, 0},
    {{0,1}, 0},
    {{11112,0}, 0},
    {{11111,0}, 0}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = cdh_ObjidIsNull(d[i].oid);
    if (sts != d[i].sts) {
      m_log->log('E', "ObjidIsNull", i);
      return;
    }
  }

  m_log->log('S', "ObjidIsNull", CDH__SUCCESS);
}

void ra_cdhtest::ObjidIsNotNull(void)
{
  sObjidIsNull d[] = {
    {pwr_cNOid, 0},
    {{11111,1}, 1},
    {{11112,1}, 1},
    {{0,1}, 1},
    {{11112,0}, 1},
    {{11111,0}, 1}
  };
  int sts;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    sts = cdh_ObjidIsNotNull(d[i].oid);
    if (sts != d[i].sts) {
      m_log->log('E', "ObjidIsNotNull", i);
      return;
    }
  }

  m_log->log('S', "ObjidIsNotNull", CDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tCid cid;
  int sts;
} sClassObjidToId;

void ra_cdhtest::ClassObjidToId(void)
{
  sClassObjidToId d[] = {
    {"pwrs:Class-$ClassDef", pwr_cClass_ClassDef},
    {"pwrs:Class-$PlantHier", pwr_cClass_PlantHier},
    {"pwrb:Class-And", pwr_cClass_and},
    {"BaseComponent:Class-BaseSensor", pwr_cClass_BaseSensor}
  };
  pwr_tCid cid;
  pwr_tOid oid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "ClassObjidToId, gdh_NameToObjid", i);
      return;
    }
    cid = cdh_ClassObjidToId(oid);
    if (cid != d[i].cid) {
      m_log->log('E', "ClassObjidToId", i);
      return;
    }
  }

  m_log->log('S', "ClassObjidToId", CDH__SUCCESS);
}

typedef struct {
  pwr_tCid cid;
  pwr_tOName name;
  int sts;
} sClassIdToObjid;

void ra_cdhtest::ClassIdToObjid(void)
{
  sClassIdToObjid d[] = {
    {pwr_cClass_ClassDef, "pwrs:Class-$ClassDef"},
    {pwr_cClass_PlantHier, "pwrs:Class-$PlantHier"},
    {pwr_cClass_and, "pwrb:Class-And"},
    {pwr_cClass_BaseSensor, "BaseComponent:Class-BaseSensor"}
  };
  pwr_tOid doid, oid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "ClassIdToObjid, gdh_NameToObjid", i);
      return;
    }
    doid = cdh_ClassIdToObjid(d[i].cid);
    if (cdh_ObjidIsNotEqual(oid, doid)) {
      m_log->log('E', "ClassIdToObjid", i);
      return;
    }
  }

  m_log->log('S', "ClassIdToObjid", CDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  pwr_tTypeId type;
  int sts;
} sTypeObjidToId;

void ra_cdhtest::TypeObjidToId(void)
{
  sTypeObjidToId d[] = {
    {"pwrs:Type-$Boolean", pwr_eType_Boolean},
    {"pwrs:Type-$Float32", pwr_eType_Float32},
    {"pwrs:Type-$Float64", pwr_eType_Float64},
    {"pwrs:Type-$Int8", pwr_eType_Int8},
    {"pwrs:Type-$Int16", pwr_eType_Int16},
    {"pwrs:Type-$Int32", pwr_eType_Int32},
    {"pwrs:Type-$Int64", pwr_eType_Int64},
    {"pwrs:Type-$UInt8", pwr_eType_UInt8},
    {"pwrs:Type-$UInt16", pwr_eType_UInt16},
    {"pwrs:Type-$UInt32", pwr_eType_UInt32},
    {"pwrs:Type-$UInt64", pwr_eType_UInt64},
    {"pwrs:Type-$Mask", pwr_eType_Mask},
    {"pwrs:Type-$Enum", pwr_eType_Enum},
    {"pwrs:Type-$Time", pwr_eType_Time},
    {"pwrs:Type-$DeltaTime", pwr_eType_DeltaTime},
    {"pwrs:Type-$Status", pwr_eType_Status},
    {"pwrs:Type-$Objid", pwr_eType_Objid},
    {"pwrs:Type-$AttrRef", pwr_eType_AttrRef},
    {"pwrs:Type-$VolumeId", pwr_eType_VolumeId},
    {"pwrs:Type-$String", pwr_eType_String}
  };
  pwr_tTypeId type;
  pwr_tOid oid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "TypeObjidToId, gdh_NameToObjid", i);
      return;
    }
    type = cdh_TypeObjidToId(oid);
    if (type != d[i].type) {
      m_log->log('E', "TypeObjidToId", i);
      return;
    }
  }

  m_log->log('S', "TypeObjidToId", CDH__SUCCESS);
}

typedef struct {
  pwr_tTypeId type;
  pwr_tOName name;
  int sts;
} sTypeIdToObjid;

void ra_cdhtest::TypeIdToObjid(void)
{
  sTypeIdToObjid d[] = {
    {pwr_eType_Boolean, "pwrs:Type-$Boolean"},
    {pwr_eType_Float32, "pwrs:Type-$Float32"},
    {pwr_eType_Float64, "pwrs:Type-$Float64"},
    {pwr_eType_Int8, "pwrs:Type-$Int8"},
    {pwr_eType_Int16, "pwrs:Type-$Int16"},
    {pwr_eType_Int32, "pwrs:Type-$Int32"},
    {pwr_eType_Int64, "pwrs:Type-$Int64"},
    {pwr_eType_UInt8, "pwrs:Type-$UInt8"},
    {pwr_eType_UInt16, "pwrs:Type-$UInt16"},
    {pwr_eType_UInt32, "pwrs:Type-$UInt32"},
    {pwr_eType_UInt64, "pwrs:Type-$UInt64"},
    {pwr_eType_Mask, "pwrs:Type-$Mask"},
    {pwr_eType_Enum, "pwrs:Type-$Enum"},
    {pwr_eType_Time, "pwrs:Type-$Time"},
    {pwr_eType_DeltaTime, "pwrs:Type-$DeltaTime"},
    {pwr_eType_Status, "pwrs:Type-$Status"},
    {pwr_eType_Objid, "pwrs:Type-$Objid"},
    {pwr_eType_AttrRef, "pwrs:Type-$AttrRef"},
    {pwr_eType_VolumeId, "pwrs:Type-$VolumeId"},
    {pwr_eType_String, "pwrs:Type-$String"}
  };
  pwr_tOid doid, oid;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = gdh_NameToObjid(d[i].name, &oid);
    if (EVEN(m_sts)) {
      m_log->log('E', "TypeIdToObjid, gdh_NameToObjid", i);
      return;
    }
    doid = cdh_TypeIdToObjid(d[i].type);
    if (cdh_ObjidIsNotEqual(oid, doid)) {
      m_log->log('E', "TypeIdToObjid", i);
      return;
    }
  }

  m_log->log('S', "TypeIdToObjid", CDH__SUCCESS);
}

typedef struct {
  pwr_tTypeId type;
  int tix;
} sTypeIdToIndex;

void ra_cdhtest::TypeIdToIndex(void)
{
  sTypeIdToIndex d[] = {
    {pwr_eType_Boolean, pwr_eTix_Boolean},
    {pwr_eType_Float32, pwr_eTix_Float32},
    {pwr_eType_Float64, pwr_eTix_Float64},
    {pwr_eType_Int32, pwr_eTix_Int32}
  };
  int tix;
  
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    tix = cdh_TypeIdToIndex(d[i].type);
    if (tix != d[i].tix) {
      m_log->log('E', "TypeIdToIndex", i);
      return;
    }
  }

  m_log->log('S', "TypeIdToIndex", CDH__SUCCESS);
}

typedef struct {
  pwr_tOid oid;
  pwr_tAttrRef aref;
} sObjidToAref;

void ra_cdhtest::ObjidToAref(void)
{
  sObjidToAref d[] = {
    {{13454,1},{{13454,1},0,0,0, pwr_mAttrRef_Object}},
    {{12345,6789},{{12345,6789},0,0,0, pwr_mAttrRef_Object}}
  };
  pwr_tAttrRef aref;

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    aref = cdh_ObjidToAref(d[i].oid);
    if (memcmp(&aref, &d[i].aref, sizeof(pwr_tAttrRef)) != 0) {
      m_log->log('E', "ObjidToAref", i);
      return;
    }
  }

  m_log->log('S', "ObjidToAref", CDH__SUCCESS);
}

typedef union {
  pwr_tBoolean b;
  pwr_tInt32 i8;
  pwr_tUInt32 ui8;
  pwr_tInt32 i16;
  pwr_tUInt32 ui16;
  pwr_tInt32 i32;
  pwr_tUInt32 ui32;
  pwr_tInt32 i64;
  pwr_tUInt32 ui64;
  pwr_tFloat32 f32;
  pwr_tFloat64 f64;
  char s40[40];
  pwr_tTime t;
  pwr_tDeltaTime dt;
  pwr_tOid oid;
  pwr_tAttrRef aref;
} uValueToString;

typedef struct {
  pwr_eType	type;
  uValueToString value;
  char		str[40];
} sAttrValueToString;

void ra_cdhtest::AttrValueToString(void)
{
  char str[40];
  sAttrValueToString d[] = {
    {pwr_eType_Boolean,	{.b=1}, 		"1"},
    {pwr_eType_Char,	{.i8=65}, 		"A"},
    {pwr_eType_Int8,	{.i8=120},     		"120"},
    {pwr_eType_UInt8,	{.i8=222}, 		"222"},
    {pwr_eType_Int16,	{.i16=222}, 		"222"},
    {pwr_eType_UInt16,	{.i16=222}, 		"222"},
    {pwr_eType_Int32,	{.i32=222}, 		"222"},
    {pwr_eType_UInt32,	{.i32=222}, 		"222"},
    {pwr_eType_Int64,	{.i64=222}, 		"222"},
    {pwr_eType_UInt64,	{.i64=222}, 		"222"},
    {pwr_eType_Mask,	{.i32=0xffff}, 		"65535"},
    {pwr_eType_Enum,	{.i32=222}, 		"222"},
    {pwr_eType_Float32,	{.f32=222.22}, 		"222.22"},
    {pwr_eType_Float64,	{.f64=666.66}, 		"666.66"},
    {pwr_eType_Time,	{.t={1586956789,500000000}}, "15-APR-2020 15:19:49.50"},
    {pwr_eType_DeltaTime,{.dt={54223,500000000}}, "15:03:43.50"},
    {pwr_eType_String,	{.s40={'S','o','m','e',' ','s','t','r','i','n','g',0,}}, "Some string"},
    {pwr_eType_ProString,	{.s40={'S','o','m','e',' ','s','t','r','i','n','g',0,}}, "***********"},
    {pwr_eType_Text,	{.s40={'S','o','m','e',' ','t','e','x','t',0,}}, "Some text"}
  };
    

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_AttrValueToString(d[i].type, &d[i].value, str, sizeof(str));
    if (EVEN(m_sts)) {
      m_log->log('E', "AttrValueToString", d[i].str, m_sts);
      return;
    }
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "AttrValueToString", str);
      return;
    }
  }

  m_log->log('S', "AttrValueToString", CDH__SUCCESS);
}

typedef struct {
  pwr_eType	type;
  uValueToString value;
  int 		size;
  char		str[40];
} sStringToAttrValue;

void ra_cdhtest::StringToAttrValue(void)
{
  char value[100];
  sStringToAttrValue d[] = {
    {pwr_eType_Boolean,	{.b=1}, 	sizeof(pwr_tBoolean),	"1"},
    {pwr_eType_Char,	{.i8=65}, 	sizeof(pwr_tChar),	"A"},
    {pwr_eType_Int8,	{.i8=120}, 	sizeof(pwr_tInt8),	"120"},
    {pwr_eType_UInt8,	{.i8=222}, 	sizeof(pwr_tUInt8),	"222"},
    {pwr_eType_Int16,	{.i16=222}, 	sizeof(pwr_tInt16),	"222"},
    {pwr_eType_UInt16,	{.i16=222}, 	sizeof(pwr_tUInt16),	"222"},
    {pwr_eType_Int32,	{.i32=222}, 	sizeof(pwr_tInt32),	"222"},
    {pwr_eType_UInt32,	{.i32=222}, 	sizeof(pwr_tUInt32),	"222"},
    {pwr_eType_Int64,	{.i64=222}, 	sizeof(pwr_tInt64),	"222"},
    {pwr_eType_UInt64,	{.i64=222}, 	sizeof(pwr_tUInt64),	"222"},
    {pwr_eType_Mask,	{.i32=0xffff}, 	sizeof(pwr_tMask),	"65535"},
    {pwr_eType_Enum,	{.i32=222}, 	sizeof(pwr_tEnum),	"222"},
    {pwr_eType_Float32,	{.f32=222.22}, 	sizeof(pwr_tFloat32),	"222.22"},
    {pwr_eType_Float64,	{.f64=666.66}, 	sizeof(pwr_tFloat64),	"666.66"},
    {pwr_eType_Time,	{.t={1586956789,500000000}}, sizeof(pwr_tTime), "15-APR-2020 15:19:49.50"},
    {pwr_eType_DeltaTime,{.dt={54223,500000000}}, sizeof(pwr_tDeltaTime), "15:03:43.50"},
    {pwr_eType_String,	{.s40={'S','o','m','e',' ','s','t','r','i','n','g',0,}}, 40,	"Some string"},
    {pwr_eType_Text,	{.s40={'S','o','m','e',' ','t','e','x','t',0,}}, 40,	"Some text"}
  };
    

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToAttrValue(d[i].type, d[i].str, value);
    if (EVEN(m_sts)) {
      m_log->log('E', "StringToAttrValue", d[i].str, m_sts);
      return;
    }
    switch (d[i].type) {
    case pwr_eType_Float32:
      if (fabs(d[i].value.f32 - *(pwr_tFloat32 *)&value) > 200*FLT_EPSILON) {
	m_log->log('E', "StringToAttrValue", d[i].str);
	return;
      }
      break;
    case pwr_eType_Float64:
      if (fabs(d[i].value.f64 - *(pwr_tFloat64 *)&value) > 600*FLT_EPSILON) {
	m_log->log('E', "StringToAttrValue", d[i].str);
	return;
      }
      break;
    case pwr_eType_String:
    case pwr_eType_ProString:
    case pwr_eType_Text:
      if (strcmp(d[i].value.s40, value) != 0) {
	m_log->log('E', "StringToAttrValue", d[i].str);
	return;
      }
      break;
    default:
      if (memcmp(&d[i].value, value, d[i].size) != 0) {
	m_log->log('E', "StringToAttrValue", d[i].str);
	return;
      }
    }
  }

  m_log->log('S', "StringToAttrValue", CDH__SUCCESS);
}

typedef struct {
  pwr_tMask	mask;
  int		noofbits;
  char		str[40];
} sMaskToBinaryString;

void ra_cdhtest::MaskToBinaryString(void)
{
  sMaskToBinaryString d[] = {
    {0x0001,	16, "0000000000000001"},
    {0x000f,	16, "0000000000001111"},
    {0x00f0,	16, "0000000011110000"},
    {0x0f00,	16, "0000111100000000"},
    {0xf000,	16, "1111000000000000"},
    {0x8000,	16, "1000000000000000"},
    {0x00000001,	32, "00000000000000000000000000000001"},
    {0x0000000f,	32, "00000000000000000000000000001111"},
    {0x000000f0,	32, "00000000000000000000000011110000"},
    {0x00000f00,	32, "00000000000000000000111100000000"},
    {0x0000f000,	32, "00000000000000001111000000000000"},
    {0x00008000,	32, "00000000000000001000000000000000"},
    {0x000f0000,	32, "00000000000011110000000000000000"},
    {0x00f00000,	32, "00000000111100000000000000000000"},
    {0x0f000000,	32, "00001111000000000000000000000000"},
    {0xf0000000,	32, "11110000000000000000000000000000"},
    {0x80000000,	32, "10000000000000000000000000000000"},
    {0x27a4f68c,	32, "00100111101001001111011010001100"}
  };
  char str[40];
    

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    cdh_MaskToBinaryString(d[i].mask, d[i].noofbits, str);
    if (strcmp(d[i].str, str) != 0) {
      m_log->log('E', "MaskToBinaryString", str);
      return;
    }
  }

  m_log->log('S', "MaskToBinaryString", CDH__SUCCESS);
}

typedef struct {
  char		str[40];
  pwr_tCid	cid;
  pwr_tStatus	sts;
} sStringToClassId;

void ra_cdhtest::StringToClassId(void)
{
  sStringToClassId d[] = {
    {"0.1:1", 65544, CDH__SUCCESS},
    {"0.1:10", (0 << 24) + (1 << 16) + (10 << 3), CDH__SUCCESS},
    {"1.200:123", (1 << 24) + (200 << 16) + (123 << 3), CDH__SUCCESS},
    {"_C0.1:1", 65544, CDH__SUCCESS},
    {"_C0.1:10", (0 << 24) + (1 << 16) + (10 << 3), CDH__SUCCESS},
    {"_C1.200:123", (1 << 24) + (200 << 16) + (123 << 3), CDH__SUCCESS},
    {"_C255.255:444", ((unsigned int)255 << 24) + (255 << 16) + (444 << 3), CDH__SUCCESS},
    {"_C1:10", 0, CDH__INVCID},
    {"_C256.1:10", 0, CDH__INVCID},
    {"_C1.1:4096", 0, CDH__INVCID},
    {"1:10", 0, CDH__INVCID},
    {"256.1:10", 0, CDH__INVCID},
    {"1.1:4096", 0, CDH__INVCID}
  };
  pwr_tCid cid;
    

  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToClassId(d[i].str, &cid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "StringToClassId wrong sts", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (cid != d[i].cid) {
	m_log->log('E', "StringToClassId", i);
	return;
      }
    }
  }

  m_log->log('S', "StringToClassId", CDH__SUCCESS);
}

typedef struct {
  char		str[40];
  pwr_tOid	oid;
  pwr_tStatus	sts;
} sStringToObjid;

void ra_cdhtest::StringToObjid(void)
{
  sStringToObjid d[] = {
    {"0.1.1.1:1", {1,(0<<24)+(1<<16)+(1<<8)+1}, CDH__SUCCESS},
    {"_O0.1.1.1:1", {1,(0<<24)+(1<<16)+(1<<8)+1}, CDH__SUCCESS},
    {"_o0.1.1.1:1", {1,(0<<24)+(1<<16)+(1<<8)+1}, CDH__SUCCESS},
    {"200.44.55.66:77777", {77777,((unsigned int)200<<24)+(44<<16)+(55<<8)+66}, CDH__SUCCESS},
    {"_O200.44.55.66:77777", {77777,((unsigned int)200<<24)+(44<<16)+(55<<8)+66}, CDH__SUCCESS},
    {"1.1.1:1", pwr_cNOid, CDH__INVOID},
    {"_O1.1.1:1", pwr_cNOid, CDH__INVOID},
    {"1.1.1.1:", pwr_cNOid, CDH__INVOID},
    {"_O1.1.1.1", pwr_cNOid, CDH__INVOID},
    {"_O256.1.1.1:1", pwr_cNOid, CDH__INVOID},
    {"_O1.256.1.1:1", pwr_cNOid, CDH__INVOID},
    {"_O1.1.256.1:1", pwr_cNOid, CDH__INVOID},
    {"_O1.1.1.256:1", pwr_cNOid, CDH__INVOID}
  };
  pwr_tOid oid;
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToObjid(d[i].str, &oid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "StringToObjid wrong sts", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (cdh_ObjidIsNotEqual(oid, d[i].oid)) {
	m_log->log('E', "StringToObjid", i);
	return;
      }
    }
  }

  m_log->log('S', "StringToObjid", CDH__SUCCESS);
}

typedef struct {
  char		str[40];
  pwr_tAttrRef	aref;
  pwr_tStatus	sts;
} sStringToAref;

void ra_cdhtest::StringToAref(void)
{
  sStringToAref d[] = {
    {"_A0.1.1.1:1(_T3.7:0.1.1)[120.4]", {{1,(0<<24)+(1<<16)+(1<<8)+1}, (3<<24)+(7<<16)+(1<<3)+(1),120,4,0}, CDH__SUCCESS},
    {"_a0.1.1.1:1(_T3.7:0.1.1)[120.4]", {{1,(0<<24)+(1<<16)+(1<<8)+1}, (3<<24)+(7<<16)+(1<<3)+(1),120,4,0}, CDH__SUCCESS},
    {"0.1.1.1:1(_T3.7:0.1.1)[120.4]", {{1,(0<<24)+(1<<16)+(1<<8)+1}, (3<<24)+(7<<16)+(1<<3)+(1),120,4,0}, CDH__SUCCESS},
    {"0.1.1.1:1(_T3.7:0.3.1)[120.4]", {{1,(0<<24)+(1<<16)+(1<<8)+1}, (3<<24)+(7<<16)+(3<<3)+(1),120,4,0}, CDH__SUCCESS},
    {"0.1.1.1:1(_T3.7:1.1.1)[120.4]", {{1,(0<<24)+(1<<16)+(1<<8)+1}, (3<<24)+(7<<16)+(1<<15)+(1<<11)+1,120,4,0}, CDH__SUCCESS},
    {"0.1.1.1:1(_T3.7:1.3.1)[120.4]", {pwr_cNOid,0,0,0}, CDH__INVTID},
    {"0.1.1.1:1(_T3.7:1.1.2048)[120.4]", {pwr_cNOid,0,0,0}, CDH__INVTID},
    {"0.1.1.1:1(_T3.7:1.1.3)[120.4]", {{1,(0<<24)+(1<<16)+(1<<8)+1}, (3<<24)+(7<<16)+(1<<15)+(1<<11)+3,120,4,0}, CDH__SUCCESS}
  };
  pwr_tAttrRef aref;
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToAref(d[i].str, &aref);
    if (m_sts != d[i].sts) {
      m_log->log('E', "StringToAref wrong sts", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (memcmp(&aref, &d[i].aref, sizeof(pwr_tAttrRef)) != 0) {
	m_log->log('E', "StringToAref", i);
	return;
      }
    }
  }

  m_log->log('S', "StringToAref", CDH__SUCCESS);
}

typedef struct {
  char		str[40];
  pwr_tTypeId	type;
  pwr_tStatus	sts;
} sStringToTypeId;

void ra_cdhtest::StringToTypeId(void)
{
  // vid.vid:1.cix.bix  or vid.vid:0.tyg.tix
  // vid: 0-255, cix:0-4095, bix:0-7, tyg:0-1, tix:0-247
  sStringToTypeId d[] = {
    {"3.7:0.1.1", (3<<24)+(7<<16)+(1<<3)+(1), CDH__SUCCESS},
    {"3.7:0.3.1", (3<<24)+(7<<16)+(3<<3)+(1), CDH__SUCCESS},
    {"_T3.7:0.1.1", (3<<24)+(7<<16)+(1<<3)+(1), CDH__SUCCESS},
    {"_T3.7:0.3.1", (3<<24)+(7<<16)+(3<<3)+(1), CDH__SUCCESS},
    {"_T3.7:0.4096.1", 0, CDH__INVTID},
    {"_T3.7:0.1", 0, CDH__INVTID},
    {"_T3.7:0.3.8", 0, CDH__INVTID},
    {"_T256.7:0.3.1", 0, CDH__INVTID},
    {"_T3.256:0.3.1", 0, CDH__INVTID},
    {"_T3.7:0.4096.1", 0, CDH__INVTID},
    {"_T3.7:1.1.1", (3<<24)+(7<<16)+(1<<15)+(1<<11)+1, CDH__SUCCESS},
    {"_T3.7:1.3.1", 0, CDH__INVTID},
    {"_T3.7:1.1.2048", 0, CDH__INVTID},
    {"_T3.7:1.1.3", (3<<24)+(7<<16)+(1<<15)+(1<<11)+3, CDH__SUCCESS}
  };
  pwr_tTypeId type;
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToTypeId(d[i].str, &type);
    if (m_sts != d[i].sts) {
      m_log->log('E', "StringToTypeId wrong sts", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (type != d[i].type) {
	m_log->log('E', "StringToTypeId", i);
	return;
      }
    }
  }

  m_log->log('S', "StringToTypeId", CDH__SUCCESS);
}

typedef struct {
  char		str[40];
  pwr_tVid	vid;
  pwr_tStatus	sts;
} sStringToVolumeId;

void ra_cdhtest::StringToVolumeId(void)
{
  sStringToVolumeId d[] = {
    {"3.7.22.55", (3<<24)+(7<<16)+(22<<8)+55, CDH__SUCCESS},
    {"_V3.7.22.55", (3<<24)+(7<<16)+(22<<8)+55, CDH__SUCCESS},
    {"_v3.7.22.55", (3<<24)+(7<<16)+(22<<8)+55, CDH__SUCCESS},
    {"_O3.7.22.55", (3<<24)+(7<<16)+(22<<8)+55, CDH__SUCCESS},
    {"_A3.7.22.55", (3<<24)+(7<<16)+(22<<8)+55, CDH__SUCCESS},
    {"_V256.7.22.55", 0, CDH__INVVID},
    {"_V3.256.22.55", 0, CDH__INVVID},
    {"_V3.7.256.55", 0, CDH__INVVID},
    {"_V3.7.22.256", 0, CDH__INVVID}
  };
  pwr_tVid vid;
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToVolumeId(d[i].str, &vid);
    if (m_sts != d[i].sts) {
      m_log->log('E', "StringToVolumeId wrong sts", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (vid != d[i].vid) {
	m_log->log('E', "StringToVolumeId", i);
	return;
      }
    }
  }

  m_log->log('S', "StringToVolumeId", CDH__SUCCESS);
}

typedef struct {
  char		str[40];
  pwr_tOix	oix;
  pwr_tStatus	sts;
} sStringToObjectIx;

void ra_cdhtest::StringToObjectIx(void)
{
  sStringToObjectIx d[] = {
    {"_X1", 1, CDH__SUCCESS},
    {"_x1", 1, CDH__SUCCESS},
    {"1", 1, CDH__SUCCESS},
    {"_X123456789", 123456789, CDH__SUCCESS}
  };
  pwr_tOix oix;
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    m_sts = cdh_StringToObjectIx(d[i].str, &oix);
    if (m_sts != d[i].sts) {
      m_log->log('E', "StringToObjectIx wrong sts", m_sts);
      return;
    }
    if (ODD(m_sts)) {
      if (oix != d[i].oix) {
	m_log->log('E', "StringToObjectIx", i);
	return;
      }
    }
  }

  m_log->log('S', "StringToObjectIx", CDH__SUCCESS);
}

typedef struct {
  pwr_tCid	cid;
  int		prefix;
  char		str[40];
} sClassIdToString;

void ra_cdhtest::ClassIdToString(void)
{
  sClassIdToString d[] = {
    {65544, 0, "0.1:1"},
    {(0 << 24) + (1 << 16) + (10 << 3), 0, "0.1:10"},
    {(1 << 24) + (200 << 16) + (123 << 3), 0, "1.200:123"},
    {65544, 1, "_C0.1:1"},
    {(0 << 24) + (1 << 16) + (10 << 3), 1, "_C0.1:10"},
    {(1 << 24) + (200 << 16) + (123 << 3), 1, "_C1.200:123"},
    {((unsigned int)255 << 24) + (255 << 16) + (444 << 3), 1, "_C255.255:444"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    cdh_ClassIdToString(str, sizeof(str), d[i].cid, d[i].prefix);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "ClassIdToString", i);
      return;
    }
  }

  m_log->log('S', "ClassIdToString", CDH__SUCCESS);
}

typedef struct {
  pwr_tTypeId	type;
  int		prefix;
  char		str[40];
} sTypeIdToString;

void ra_cdhtest::TypeIdToString(void)
{
  sTypeIdToString d[] = {
    {(3<<24)+(7<<16)+(1<<3)+(1), 0, "3.7:0.1.1"},
    {(3<<24)+(7<<16)+(3<<3)+(1), 0, "3.7:0.3.1"},
    {(3<<24)+(7<<16)+(1<<3)+(1), 1, "_T3.7:0.1.1"},
    {(3<<24)+(7<<16)+(3<<3)+(1), 1, "_T3.7:0.3.1"},
    {(3<<24)+(7<<16)+(1<<15)+(1<<11)+1, 1, "_T3.7:1.1.1"},
    {(3<<24)+(7<<16)+(1<<15)+(1<<11)+3, 1, "_T3.7:1.1.3"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    cdh_TypeIdToString(str, sizeof(str), d[i].type, d[i].prefix);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "TypeIdToString", i);
      return;
    }
  }

  m_log->log('S', "TypeIdToString", CDH__SUCCESS);
}

typedef struct {
  pwr_tOix	oix;
  int		prefix;
  char		str[40];
} sObjectIxToString;

void ra_cdhtest::ObjectIxToString(void)
{
  sObjectIxToString d[] = {
    {1, 1, "_X1"},
    {1, 0, "1"},
    {123456789, 1, "_X123456789"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    cdh_ObjectIxToString(str, sizeof(str), d[i].oix, d[i].prefix);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "ObjectIxToString", i);
      return;
    }
  }

  m_log->log('S', "ObjectIxToString", CDH__SUCCESS);
}

typedef struct {
  pwr_tAttrRef	aref;
  int		prefix;
  char		str[40];
} sArefToString;

void ra_cdhtest::ArefToString(void)
{
  sArefToString d[] = {
    {{{1,(0<<24)+(1<<16)+(1<<8)+1}, (3<<24)+(7<<16)+(1<<3)+(1),120,4,0}, 1, "_A0.1.1.1:1(_T3.7:0.1.1)[120.4]"},
    {{{1,(0<<24)+(1<<16)+(1<<8)+1}, (3<<24)+(7<<16)+(1<<3)+(1),120,4,0}, 0, "0.1.1.1:1(_T3.7:0.1.1)[120.4]"},
    {{{1,(0<<24)+(1<<16)+(1<<8)+1}, (3<<24)+(7<<16)+(3<<3)+(1),120,4,0}, 0, "0.1.1.1:1(_T3.7:0.3.1)[120.4]"},
    {{{1,(0<<24)+(1<<16)+(1<<8)+1}, (3<<24)+(7<<16)+(1<<15)+(1<<11)+1,120,4,0}, 0, "0.1.1.1:1(_T3.7:1.1.1)[120.4]"},
    {{{1,(0<<24)+(1<<16)+(1<<8)+1}, (3<<24)+(7<<16)+(1<<15)+(1<<11)+3,120,4,0}, 0, "0.1.1.1:1(_T3.7:1.1.3)[120.4]"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    cdh_ArefToString(str, sizeof(str), &d[i].aref, d[i].prefix);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "ArefToString", i);
      return;
    }
  }
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    strcpy(str, cdh_AttrRefToString(&d[i].aref, d[i].prefix));
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "ArefToString, AttrRefToString", i);
      return;
    }
  }

  m_log->log('S', "ArefToString", CDH__SUCCESS);
}

typedef struct {
  pwr_tNodeId	nid;
  int		prefix;
  int		suffix;
  char		str[40];
} sNodeIdToString;

void ra_cdhtest::NodeIdToString(void)
{
  sNodeIdToString d[] = {
    {pwr_cNNid, 0, 0, "0.0.0.0"},
    {pwr_cNNid, 1, 0, "_N0.0.0.0"},
    {pwr_cNNid, 1, 1, "_N0.0.0.0:"},
    {(3<<24)+(7<<16)+(22<<8)+55, 0, 0, "3.7.22.55"},
    {(3<<24)+(7<<16)+(22<<8)+55, 0, 1, "3.7.22.55:"},
    {(3<<24)+(7<<16)+(22<<8)+55, 1, 0, "_N3.7.22.55"},
    {(3<<24)+(7<<16)+(22<<8)+55, 1, 1, "_N3.7.22.55:"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    strcpy(str, "");
    cdh_NodeIdToString(str, d[i].nid, d[i].prefix, d[i].suffix);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "NodeIdToString", i);
      return;
    }
  }

  m_log->log('S', "NodeIdToString", CDH__SUCCESS);
}

typedef struct {
  pwr_tOid	oid;
  int		prefix;
  char		str[40];
} sOidToString;

void ra_cdhtest::OidToString(void)
{
  sOidToString d[] = {
    {pwr_cNOid, 0, "0.0.0.0:0"},
    {pwr_cNOid, 1, "_O0.0.0.0:0"},
    {{1,(0<<24)+(1<<16)+(1<<8)+1}, 0, "0.1.1.1:1"},
    {{1,(0<<24)+(1<<16)+(1<<8)+1}, 1, "_O0.1.1.1:1"},
    {{77777,((unsigned int)200<<24)+(44<<16)+(55<<8)+66}, 0, "200.44.55.66:77777"},
    {{77777,((unsigned int)200<<24)+(44<<16)+(55<<8)+66}, 1, "_O200.44.55.66:77777"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    cdh_OidToString(str, sizeof(str), d[i].oid, d[i].prefix);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "OidToString", i);
      return;
    }
  }
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    strcpy(str, cdh_ObjidToString(d[i].oid, d[i].prefix));
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "OidToString, ObjidToString", i);
      return;
    }
  }

  m_log->log('S', "OidToString", CDH__SUCCESS);
}

void ra_cdhtest::OidToFnString(void)
{
  sOidToString d[] = {
    {pwr_cNOid, 0, "000_000_000_000_00000000"},
    {{1,(0<<24)+(1<<16)+(1<<8)+1}, 0, "000_001_001_001_00000001"},
    {{0x77777,((unsigned int)200<<24)+(44<<16)+(55<<8)+66}, 0, "200_044_055_066_00077777"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    strcpy(str, "");
    cdh_ObjidToFnString(str, d[i].oid);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "OidToFnString, ObjidToString", i);
      return;
    }
  }

  m_log->log('S', "OidToFnString", CDH__SUCCESS);
}

typedef struct {
  pwr_tVolumeId	vid;
  int		prefix;
  int		suffix;
  char		str[40];
} sVolumeIdToString;

void ra_cdhtest::VolumeIdToString(void)
{
  sVolumeIdToString d[] = {
    {pwr_cNVid, 0, 0, "0.0.0.0"},
    {pwr_cNVid, 1, 0, "_V0.0.0.0"},
    {pwr_cNVid, 1, 1, "_V0.0.0.0:"},
    {(3<<24)+(7<<16)+(22<<8)+55, 0, 0, "3.7.22.55"},
    {(3<<24)+(7<<16)+(22<<8)+55, 0, 1, "3.7.22.55:"},
    {(3<<24)+(7<<16)+(22<<8)+55, 1, 0, "_V3.7.22.55"},
    {(3<<24)+(7<<16)+(22<<8)+55, 1, 1, "_V3.7.22.55:"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    strcpy(str, "");
    cdh_VolumeIdToString(str, sizeof(str), d[i].vid, d[i].prefix, d[i].suffix);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "VolumeIdToString", i);
      return;
    }
  }

  m_log->log('S', "VolumeIdToString", CDH__SUCCESS);
}

void ra_cdhtest::VolumeIdToFnString(void)
{
  sVolumeIdToString d[] = {
    {pwr_cNVid, 0, 0, "000_000_000_000"},
    {(3<<24)+(7<<16)+(22<<8)+55, 0, 0, "003_007_022_055"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    strcpy(str, "");
    cdh_VolumeIdToFnString(str, sizeof(str), d[i].vid);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "VolumeIdToFnString", i);
      return;
    }
  }

  m_log->log('S', "VolumeIdToFnString", CDH__SUCCESS);
}

typedef struct {
  pwr_tSubid	oid;
  int		prefix;
  char		str[40];
} sSubidToString;

void ra_cdhtest::SubidToString(void)
{
  sSubidToString d[] = {
    {pwr_cNSubid, 0, "0.0.0.0:0"},
    {pwr_cNSubid, 1, "_S0.0.0.0:0"},
    {{1,(0<<24)+(1<<16)+(1<<8)+1}, 0, "0.1.1.1:1"},
    {{1,(0<<24)+(1<<16)+(1<<8)+1}, 1, "_S0.1.1.1:1"},
    {{77777,((unsigned int)200<<24)+(44<<16)+(55<<8)+66}, 0, "200.44.55.66:77777"},
    {{77777,((unsigned int)200<<24)+(44<<16)+(55<<8)+66}, 1, "_S200.44.55.66:77777"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    cdh_SubidToString(str, sizeof(str), d[i].oid, d[i].prefix);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "SubidToString", i);
      return;
    }
  }

  m_log->log('S', "SubidToString", CDH__SUCCESS);
}

void ra_cdhtest::DlidToString(void)
{
  sSubidToString d[] = {
    {pwr_cNDlid, 0, "0.0.0.0:0"},
    {pwr_cNDlid, 1, "_D0.0.0.0:0"},
    {{1,(0<<24)+(1<<16)+(1<<8)+1}, 0, "0.1.1.1:1"},
    {{1,(0<<24)+(1<<16)+(1<<8)+1}, 1, "_D0.1.1.1:1"},
    {{77777,((unsigned int)200<<24)+(44<<16)+(55<<8)+66}, 0, "200.44.55.66:77777"},
    {{77777,((unsigned int)200<<24)+(44<<16)+(55<<8)+66}, 1, "_D200.44.55.66:77777"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    cdh_DlidToString(str, sizeof(str), d[i].oid, d[i].prefix);
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "DlidToString", i);
      return;
    }
  }

  m_log->log('S', "DlidToString", CDH__SUCCESS);
}

typedef struct {
  char		str[40];
  char		lowstr[40];
} sLow;

void ra_cdhtest::Low(void)
{
  sLow d[] = {
    {"AbCdEfGhIjKl", "abcdefghijkl"},
    {"MNOPRSTUVXYZ", "mnoprstuvxyz"},
    {"≈ƒ÷Â‰ˆ", "Â‰ˆÂ‰ˆ"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    strcpy(str, cdh_Low(d[i].str));
    if (strcmp(str, d[i].lowstr) != 0) {
      m_log->log('E', "Low", i);
      return;
    }
  }

  m_log->log('S', "Low", CDH__SUCCESS);
}

typedef struct {
  pwr_mOpSys	opsys;
  char		str[40];
} sOpSysToStr;

void ra_cdhtest::OpSysToStr(void)
{
  sOpSysToStr d[] = {
    {pwr_mOpSys_X86_LINUX, "x86_linux"},
    {pwr_mOpSys_X86_64_LINUX, "x86_64_linux"},
    {pwr_mOpSys_X86_64_MACOS, "x86_64_macos"},
    {pwr_mOpSys_ARM_LINUX, "arm_linux"},
    {pwr_mOpSys_ARM64_LINUX, "arm64_linux"},
    {pwr_mOpSys_X86_64_FREEBSD, "x86_64_freebsd"},
    {pwr_mOpSys_X86_64_OPENBSD, "x86_64_openbsd"},
    {pwr_mOpSys_X86_CYGWIN, "x86_cygwin"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    strcpy(str, cdh_OpSysToStr(d[i].opsys));
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "OpSysToStr", i);
      return;
    }
  }

  m_log->log('S', "OpSysToStr", CDH__SUCCESS);
}

void ra_cdhtest::OpSysToDirStr(void)
{
  sOpSysToStr d[] = {
    {pwr_mOpSys_X86_LINUX, "os_linux/hw_x86"},
    {pwr_mOpSys_X86_64_LINUX, "os_linux/hw_x86_64"},
    {pwr_mOpSys_X86_64_MACOS, "os_macos/hw_x86_64"},
    {pwr_mOpSys_ARM_LINUX, "os_linux/hw_arm"},
    {pwr_mOpSys_ARM64_LINUX, "os_linux/hw_arm64"},
    {pwr_mOpSys_X86_64_FREEBSD, "os_freebsd/hw_x86_64"},
    {pwr_mOpSys_X86_64_OPENBSD, "os_openbsd/hw_x86_64"},
    {pwr_mOpSys_X86_CYGWIN, "os_cygwin/hw_x86"}
  };
  char str[40];
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    strcpy(str, cdh_OpSysToDirStr(d[i].opsys));
    if (strcmp(str, d[i].str) != 0) {
      m_log->log('E', "OpSysToDirStr", i);
      return;
    }
  }

  m_log->log('S', "OpSysToDirStr", CDH__SUCCESS);
}

typedef struct {
  pwr_tObjName name;
  pwr_tObjName objname;
} sStringToObjectName;

void ra_cdhtest::StringToObjectName(void)
{
  sStringToObjectName d[] = {
    {"ABC!\"#§%&/()=?+0123456789,.;:D", "ABC____________0123456789____D"},
    {"h1-h2-h3-h4", "h1_h2_h3_h4"},
    {"&h1-h2-h3-h4", "Oh1_h2_h3_h4"},
    {"≈k÷ster-", "≈k÷ster_"}
  };
  pwr_tObjName objname;
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    cdh_StringToObjectName(objname, d[i].name);
    if (strcmp(objname, d[i].objname) != 0) {
      m_log->log('E', "StringToObjectName", i);
      return;
    }
  }

  m_log->log('S', "StringToObjectName", CDH__SUCCESS);
}

typedef struct {
  pwr_tOName name;
  int segments;
  pwr_tOName outname;
} sCutNameSegments;

void ra_cdhtest::CutNameSegments(void)
{
  sCutNameSegments d[] = {
    {"h1-h2-h3-h4-h5", 0, ""},
    {"h1-h2-h3-h4-h5", 1, "h5"},
    {"h1-h2-h3-h4-h5", 2, "h4-h5"},
    {"h1-h2-h3-h4-h5", 3, "h3-h4-h5"},
    {"h1-h2-h3-h4-h5", 4, "h2-h3-h4-h5"},
    {"h1-h2-h3-h4-h5", 5, "h1-h2-h3-h4-h5"},
    {"h1-h2-h3-h4-h5", 6, "h1-h2-h3-h4-h5"},
  };
  pwr_tOName outname;
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    cdh_CutNameSegments(outname, d[i].name, d[i].segments);
    if (strcmp(outname, d[i].outname) != 0) {
      m_log->vlog('E', "CutNameSegments, %s != %s %d", outname, d[i].outname, i);
      return;
    }
  }

  m_log->log('S', "CutNameSegments", CDH__SUCCESS);
}

typedef struct {
  pwr_sParInfo parinfo;
  pwr_tUInt32 size;
} sAttrSize;

void ra_cdhtest::AttrSize(void)
{
  sAttrSize d[] = {
    {{"",pwr_eType_Int32,0,4,0,1,1}, 4},
    {{"",pwr_eType_Int32,0,4,PWR_MASK_POINTER,1,1}, 8},
    {{"",pwr_eType_Int32,0,4,PWR_MASK_POINTER,100,1}, 800},
    {{"",pwr_eType_Int32,0,400,PWR_MASK_POINTER|PWR_MASK_PRIVATE,100,1}, 400}
  };
  pwr_tUInt32 size;
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    size = cdh_AttrSize(&d[i].parinfo);
    if (size != d[i].size) {
      m_log->log('E', "AttrSize", i);
      return;
    }
  }

  m_log->log('S', "AttrSize", CDH__SUCCESS);
}

void ra_cdhtest::AttrElemSize(void)
{
  sAttrSize d[] = {
    {{"",pwr_eType_Int32,0,4,0,1,1}, 4},
    {{"",pwr_eType_Int32,0,4,PWR_MASK_POINTER,1,1}, 8},
    {{"",pwr_eType_Int32,0,4,PWR_MASK_POINTER,100,1}, 8},
    {{"",pwr_eType_Int32,0,400,PWR_MASK_POINTER|PWR_MASK_PRIVATE,100,1}, 4}
  };
  pwr_tUInt32 size;
    
  for (unsigned int i = 0; i < sizeof(d)/sizeof(d[0]); i++) {
    size = cdh_AttrElemSize(&d[i].parinfo);
    if (size != d[i].size) {
      m_log->log('E', "AttrElemSize", i);
      return;
    }
  }

  m_log->log('S', "AttrElemSize", CDH__SUCCESS);
}

// Constructor
ra_cdhtest::ra_cdhtest()
{
  m_log = new tst_log(&m_sts, "co-Cdh", "$pwrp_log/cdh.tlog");
  if (EVEN(m_sts))
    printf("** Unable to open log file");
}

// Destructor
ra_cdhtest::~ra_cdhtest()
{
  delete m_log;
}

int main()
{
  ra_cdhtest cdh;
  pwr_tStatus sts;

  setenv("TZ", "Europe/Stockholm", 1);
  tzset();

  sts = gdh_Init("ra_cdhtest");
  if (EVEN(sts)) {
    cdh.log()->log('S', "cdh gdh_Init", sts);  
    exit(0);
  }

  cdh.ObjidCompare();
  cdh.ObjidIsEqual();
  cdh.ObjidIsNotEqual();
  cdh.ObjidIsNull();
  cdh.ObjidIsNotNull();
  cdh.ClassObjidToId();
  cdh.ClassIdToObjid();
  cdh.TypeObjidToId();
  cdh.TypeIdToObjid();
  cdh.TypeIdToIndex();
  cdh.ObjidToAref();
  cdh.AttrValueToString();
  cdh.StringToAttrValue();
  cdh.MaskToBinaryString();
  cdh.StringToClassId();
  cdh.StringToObjid();
  cdh.StringToAref();
  cdh.StringToVolumeId();
  cdh.StringToObjectIx();
  cdh.ClassIdToString();
  cdh.TypeIdToString();
  cdh.ObjectIxToString();
  cdh.ArefToString();
  cdh.NodeIdToString();
  cdh.OidToString();
  cdh.OidToFnString();
  cdh.VolumeIdToString();
  cdh.VolumeIdToFnString();
  cdh.SubidToString();
  cdh.DlidToString();
  cdh.Low();
  cdh.OpSysToStr();
  cdh.OpSysToDirStr();
  cdh.StringToObjectName();
  cdh.CutNameSegments();
  cdh.AttrSize();
  cdh.AttrElemSize();
}


