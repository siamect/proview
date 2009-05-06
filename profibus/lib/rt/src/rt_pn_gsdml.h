/* 
 * Proview   $Id: rt_pb_gsd.h,v 1.8 2008-10-31 12:51:29 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass ave, Cambridge, MA 02139, USA.
 */

#ifndef rt_pn_gsdml_h
#define rt_pn_gsdml_h

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class pn_gsdml;
class gsdml_Valuelist;

typedef struct {
  char name[32];
  pwr_tCid cid;
} gsdml_sModuleClass;

typedef char 	gsdml_tString80[80];
typedef char 	gsdml_tString1024[1024];
typedef char 	gsdml_tString512[512];
typedef char 	gsdml_tString[80];
typedef char    *gsdml_tAllocatedString;
typedef char 	gsdml_tNormalizedString[80];
typedef char 	gsdml_tToken[80];
typedef char 	gsdml_tTokenList[80];
typedef struct {
  char str[160];
  gsdml_Valuelist *list;
} gsdml_tValueList;
typedef char 	gsdml_tEnum[80];
typedef char 	gsdml_tId[80];
typedef char 	gsdml_tIdT[256];
typedef struct {
  gsdml_tIdT ref;
  void *p;
} gsdml_tRefIdT;
typedef struct {
  char ref[80];
  void *p;
} gsdml_tRefId;
typedef int 	gsdml_tInteger;
typedef unsigned char 	gsdml_tUnsigned8;
typedef unsigned short 	gsdml_tUnsigned16;
typedef unsigned int 	gsdml_tUnsigned32;
typedef unsigned short 	gsdml_tUnsigned16hex;
typedef unsigned int 	gsdml_tUnsigned32hex;
typedef unsigned int 	gsdml_tBoolean;
typedef char 	gsdml_tSignedOrFloatValueList[160];

typedef enum {
  gsdml_eType_,
  gsdml_eType_Boolean,
  gsdml_eType_Integer,
  gsdml_eType_String,
  gsdml_eType_AllocatedString,
  gsdml_eType_NormalizedString,
  gsdml_eType_Token,
  gsdml_eType_Id,
  gsdml_eType_IdT,
  gsdml_eType_RefId,
  gsdml_eType_RefIdT,
  gsdml_eType_Unsigned8,
  gsdml_eType_Unsigned16,
  gsdml_eType_Unsigned32,
  gsdml_eType_Unsigned16hex,
  gsdml_eType_Unsigned32hex,
  gsdml_eType_ValueList,
  gsdml_eType_SignedOrFloatValueList,
  gsdml_eType_TokenList,
  gsdml_eType_Enum,
  gsdml_eType__
} gsdml_eType;

typedef enum {
  gsdml_eValueDataType_,
  gsdml_eValueDataType_Bit,
  gsdml_eValueDataType_BitArea,
  gsdml_eValueDataType_Integer8,
  gsdml_eValueDataType_Integer16,
  gsdml_eValueDataType_Integer32,
  gsdml_eValueDataType_Integer64,
  gsdml_eValueDataType_Unsigned8,
  gsdml_eValueDataType_Unsigned16,
  gsdml_eValueDataType_Unsigned32,
  gsdml_eValueDataType_Unsigned64,
  gsdml_eValueDataType_Float32,
  gsdml_eValueDataType_Float64,
  gsdml_eValueDataType_Date,
  gsdml_eValueDataType_TimeOfDayWithDate,
  gsdml_eValueDataType_TimeOfDayWithoutDate,
  gsdml_eValueDataType_TimeDiffWithDate,
  gsdml_eValueDataType_TimeDiffWithoutDate,
  gsdml_eValueDataType_NetworkTime,
  gsdml_eValueDataType_NetworkTimeDiff,
  gsdml_eValueDataType_VisibleString,
  gsdml_eValueDataType_OctetString,
  gsdml_eValueDataType__
} gsdml_eValueDataType;

typedef enum {
  gsdml_eTag_,
  gsdml_eTag_xml,
  gsdml_eTag_ISO15745Profile,
  gsdml_eTag_ProfileHeader,
  gsdml_eTag_ProfileIdentification,
  gsdml_eTag_ProfileRevision,
  gsdml_eTag_ProfileName,
  gsdml_eTag_ProfileSource,
  gsdml_eTag_ProfileClassID,
  gsdml_eTag_ISO15745Reference,
  gsdml_eTag_ISO15745Part,
  gsdml_eTag_ISO15745Edition,
  gsdml_eTag_ProfileTechnology,
  gsdml_eTag_ProfileBody,
  gsdml_eTag_DeviceIdentity,
  gsdml_eTag_InfoText,
  gsdml_eTag_VendorName,
  gsdml_eTag_DeviceFunction,
  gsdml_eTag_Family,
  gsdml_eTag_ApplicationProcess,
  gsdml_eTag_DeviceAccessPointList,
  gsdml_eTag_DeviceAccessPointItem,
  gsdml_eTag_ModuleInfo,
  gsdml_eTag_Name,
  gsdml_eTag_OrderNumber,
  gsdml_eTag_HardwareRelease,
  gsdml_eTag_SoftwareRelease,
  gsdml_eTag_SubslotList,
  gsdml_eTag_SubslotItem,
  gsdml_eTag_IOConfigData,
  gsdml_eTag_UseableModules,
  gsdml_eTag_ModuleItemRef,
  gsdml_eTag_VirtualSubmoduleList,
  gsdml_eTag_VirtualSubmoduleItem,
  gsdml_eTag_IOData,
  gsdml_eTag_Input,
  gsdml_eTag_Output,
  gsdml_eTag_DataItem,
  gsdml_eTag_BitDataItem,
  gsdml_eTag_RecordDataList,
  gsdml_eTag_ParameterRecordDataItem,
  gsdml_eTag_Const,
  gsdml_eTag_Ref,
  gsdml_eTag_F_ParameterRecordDataItem,
  gsdml_eTag_F_Check_iPar,
  gsdml_eTag_F_SIL,
  gsdml_eTag_F_CRC_Length,
  gsdml_eTag_F_Block_ID,
  gsdml_eTag_F_Par_Version,
  gsdml_eTag_F_Source_Add,
  gsdml_eTag_F_Dest_Add,
  gsdml_eTag_F_WD_Time,
  gsdml_eTag_F_Par_CRC,
  gsdml_eTag_F_iPar_CRC,
  gsdml_eTag_Graphics,
  gsdml_eTag_GraphicItemRef,
  gsdml_eTag_IsochroneMode,
  gsdml_eTag_SystemDefinedSubmoduleList,
  gsdml_eTag_InterfaceSubmoduleItem,
  gsdml_eTag_General,
  gsdml_eTag_DCP_FlashOnceSignalUnit,
  gsdml_eTag_RT_Class3Properties,
  gsdml_eTag_SynchronisationMode,
  gsdml_eTag_ApplicationRelations,
  gsdml_eTag_TimingProperties,
  gsdml_eTag_RT_Class3TimingProperties,
  gsdml_eTag_MediaRedundancy,
  gsdml_eTag_PortSubmoduleItem,
  gsdml_eTag_UseableSubmodules,
  gsdml_eTag_SubmoduleItemRef,
  gsdml_eTag_SlotList,
  gsdml_eTag_SlotItem,
  gsdml_eTag_SlotGroups,
  gsdml_eTag_SlotGroup,
  gsdml_eTag_ModuleList,
  gsdml_eTag_ModuleItem,
  gsdml_eTag_SubmoduleList,
  gsdml_eTag_ValueList,
  gsdml_eTag_ValueItem,
  gsdml_eTag_Help,
  gsdml_eTag_Assignments,
  gsdml_eTag_Assign,
  gsdml_eTag_ChannelDiagList,
  gsdml_eTag_ChannelDiagItem,
  gsdml_eTag_ProfileChannelDiagItem,
  gsdml_eTag_ExtChannelDiagList,
  gsdml_eTag_ExtChannelDiagItem,
  gsdml_eTag_ExtChannelAddValue,
  gsdml_eTag_UnitDiagTypeList,
  gsdml_eTag_UnitDiagTypeItem,
  gsdml_eTag_GraphicsList,
  gsdml_eTag_GraphicItem,
  gsdml_eTag_Embedded,
  gsdml_eTag_CategoryList,
  gsdml_eTag_CategoryItem,
  gsdml_eTag_ExternalTextList,
  gsdml_eTag_PrimaryLanguage,
  gsdml_eTag_Language,
  gsdml_eTag_Text,
  gsdml_eTag__
} gsdml_eTag;


#define VL_END 0xffffffff

class gsdml_ValuelistValue {
 public:
  gsdml_ValuelistValue( unsigned int v) : value1(v), is_range(false) {}
  gsdml_ValuelistValue( unsigned int v1, unsigned int v2) : value1(v1), value2(v2), is_range(true) {}
  unsigned int value1;
  unsigned int value2;
  bool is_range;
};

class gsdml_Valuelist {
 public:
  gsdml_Valuelist( char *str);
  vector<gsdml_ValuelistValue> value;
  unsigned int status;

  void sort();
  int parse( char *str);
  bool in_list( unsigned int);
  unsigned int sts() { return status;}
};

class gsdml_ValuelistIterator {
 public:
  gsdml_ValuelistIterator( gsdml_Valuelist *vl) : valuelist(vl), current_value(0),
						  initiated(false) {}
  gsdml_Valuelist *valuelist;
  unsigned int next();
  unsigned int begin();
  unsigned int end() { return VL_END;}
  unsigned int current_value;
  unsigned int current_idx;
  bool initiated;
};

#define VLS_END 0xefffffff

class gsdml_SValuelistValue {
 public:
  gsdml_SValuelistValue( int v) : value1(v), is_range(false) {}
  gsdml_SValuelistValue( int v1, int v2) : value1(v1), value2(v2), is_range(true) {}
  int value1;
  int value2;
  bool is_range;
};

class gsdml_SValuelist {
 public:
  gsdml_SValuelist( char *str);
  vector<gsdml_SValuelistValue> value;
  unsigned int status;

  void sort();
  int parse( char *str);
  bool in_list( int);
  unsigned int sts() { return status;}
};

class gsdml_SValuelistIterator {
 public:
  gsdml_SValuelistIterator( gsdml_SValuelist *vl) : valuelist(vl), current_value(0),
						  initiated(false) {}
  gsdml_SValuelist *valuelist;
  int next();
  int begin();
  int end() { return VL_END;}
  int current_value;
  unsigned int current_idx;
  bool initiated;
};

class gsdml_FValuelistValue {
 public:
  gsdml_FValuelistValue( double v) : value1(v), is_range(false) {}
  gsdml_FValuelistValue( double v1, double v2) : value1(v1), value2(v2), is_range(true) {}
  double value1;
  double value2;
  bool is_range;
};

class gsdml_FValuelist {
 public:
  gsdml_FValuelist( char *str);
  vector<gsdml_FValuelistValue> value;
  unsigned int status;

  void sort();
  int parse( char *str);
  bool in_list( double val);
  unsigned int sts() { return status;}
};


typedef struct {
  gsdml_tString80 Version;
  gsdml_tString80 Encoding;
} gsdml_sXml;

class gsdml_Xml {
 public:
  gsdml_Xml( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sXml Body;
  pn_gsdml *gsdml;
};

typedef struct {
  gsdml_tString80 ProfileIdentification;
  gsdml_tString80 ProfileRevision;
  gsdml_tString80 ProfileName;
  gsdml_tString80 ProfileSource;
  gsdml_tString80 ProfileClassID;
  gsdml_tInteger ISO15745Part;
  gsdml_tInteger ISO15745Edition;
  gsdml_tString80 ProfileTechnology;
} gsdml_sProfileHeader;

class gsdml_ProfileHeader {
 public:
  gsdml_ProfileHeader( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sProfileHeader Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16hex VendorID;
  gsdml_tUnsigned16hex DeviceID;
  gsdml_tRefIdT InfoText;
  gsdml_tToken VendorName;
} gsdml_sDeviceIdentity;
    
class gsdml_DeviceIdentity {
 public:
  gsdml_DeviceIdentity( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sDeviceIdentity Body;
  pn_gsdml *gsdml;
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tString80 MainFamily;
  gsdml_tString80 ProductFamily;
} gsdml_sDeviceFunction;
    
class gsdml_DeviceFunction {
 public:
  gsdml_DeviceFunction( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sDeviceFunction Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

typedef struct {
  gsdml_tRefIdT CategoryRef;
  gsdml_tRefIdT SubCategory1Ref;
  gsdml_tRefIdT Name;
  gsdml_tRefIdT InfoText;
  gsdml_tToken VendorName;
  gsdml_tToken OrderNumber;
  gsdml_tToken HardwareRelease;
  gsdml_tToken SoftwareRelease;
  gsdml_tString MainFamily;
  gsdml_tString ProductFamily;
} gsdml_sModuleInfo;

class gsdml_ModuleInfo {
 public:
  gsdml_ModuleInfo( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sModuleInfo Body;
  pn_gsdml *gsdml;
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 SubslotNumber;
  gsdml_tRefId TextId;
} gsdml_sSubslotItem;

class gsdml_SubslotItem {
 public:
  gsdml_SubslotItem( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sSubslotItem Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

class gsdml_SubslotList {
 public:
  gsdml_SubslotList( pn_gsdml *g) : gsdml(g) {}
  ~gsdml_SubslotList();
  vector<gsdml_SubslotItem *> SubslotItem;
  pn_gsdml *gsdml;
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 MaxInputLength;
  gsdml_tUnsigned16 MaxOutputLength;
  gsdml_tUnsigned16 MaxDataLength;
} gsdml_sIOConfigData;

class gsdml_IOConfigData {
 public:
  gsdml_IOConfigData( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sIOConfigData Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

typedef struct {
  gsdml_tRefId ModuleItemTarget;
  gsdml_tValueList AllowedInSlots;
  gsdml_tValueList UsedInSlots;
  gsdml_tValueList FixedInSlots;
} gsdml_sModuleItemRef;

class gsdml_ModuleItemRef {
 public:
  gsdml_ModuleItemRef( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sModuleItemRef Body;
  pn_gsdml *gsdml;
  ~gsdml_ModuleItemRef();
  void build();
  void print( int ind);
};

class gsdml_UseableModules {
 public:
  gsdml_UseableModules( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_ModuleItemRef *> ModuleItemRef;
  pn_gsdml *gsdml;
  ~gsdml_UseableModules();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned8 BitOffset;
  gsdml_tRefIdT TextId;
} gsdml_sBitDataItem;

class gsdml_BitDataItem {
 public:
  gsdml_BitDataItem( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sBitDataItem Body;
  pn_gsdml *gsdml;
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tEnum DataType;
  gsdml_tUnsigned16 Length;
  gsdml_tBoolean UseAsBits;
  gsdml_tRefIdT TextId;
} gsdml_sDataItem;

class gsdml_DataItem {
 public:
  gsdml_DataItem( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sDataItem Body;
  vector<gsdml_BitDataItem *> BitDataItem;
  ~gsdml_DataItem();
  pn_gsdml *gsdml;
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tEnum Consistency;
} gsdml_sInput;

class gsdml_Input {
 public:
  gsdml_Input( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sInput Body;
  vector<gsdml_DataItem *> DataItem;
  ~gsdml_Input();
  pn_gsdml *gsdml;
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tEnum Consistency;
} gsdml_sOutput;

class gsdml_Output {
 public:
  gsdml_Output( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sOutput Body;
  vector<gsdml_DataItem *> DataItem;
  pn_gsdml *gsdml;
  ~gsdml_Output();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned8 IOPS_Length;
  gsdml_tUnsigned8 IOCS_Length;
  gsdml_tUnsigned8 F_IO_StructureDescVersion;
  gsdml_tUnsigned32 F_IO_StructureDescCRC;
} gsdml_sIOData;

class gsdml_IOData {
 public:
  gsdml_IOData( pn_gsdml *g) : Input(0), Output(0), gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sIOData Body;
  gsdml_Input *Input;
  gsdml_Output *Output;
  pn_gsdml *gsdml;
  ~gsdml_IOData();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tRefId ValueItemTarget;
  gsdml_tUnsigned32 ByteOffset;
  gsdml_tInteger BitOffset;
  gsdml_tInteger BitLength;
  gsdml_tEnum DataType;
  gsdml_tString DefaultValue;
  gsdml_tSignedOrFloatValueList AllowedValues;
  gsdml_tBoolean Changeable;
  gsdml_tBoolean Visible;
  gsdml_tRefIdT TextId;
  gsdml_tUnsigned16 Length;
} gsdml_sRef;

class gsdml_Ref {
 public:
  gsdml_Ref( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sRef Body;
  pn_gsdml *gsdml;
  void print( int ind);
};
  
typedef struct {
  gsdml_tUnsigned32 ByteOffset;
  gsdml_tString1024 Data;
} gsdml_sConst;

class gsdml_Const {
 public:
  gsdml_Const( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sConst Body;
  pn_gsdml *gsdml;
  void print( int ind);
};
  
typedef struct {
  gsdml_tUnsigned16 Index;
  gsdml_tUnsigned32 Length;
  gsdml_tUnsigned16 TransferSequence;
  gsdml_tRefIdT Name;
} gsdml_sParameterRecordDataItem;

class gsdml_ParameterRecordDataItem {
 public:
  gsdml_ParameterRecordDataItem( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sParameterRecordDataItem Body;
  vector<gsdml_Const *> Const;
  vector<gsdml_Ref *> Ref;
  pn_gsdml *gsdml;
  ~gsdml_ParameterRecordDataItem();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 F_ParamDescCRC;
  gsdml_tUnsigned16 Index;
  gsdml_tUnsigned16 TransferSequence;
  gsdml_tEnum    F_Check_iPar_DefaultValue;
  gsdml_tString  F_Check_iPar_AllowedValues;
  gsdml_tBoolean F_Check_iPar_Visible;
  gsdml_tBoolean F_Check_iPar_Changeable;
  gsdml_tEnum    F_SIL_DefaultValue;
  gsdml_tString  F_SIL_AllowedValues;
  gsdml_tBoolean F_SIL_Visible;
  gsdml_tBoolean F_SIL_Changeable;
  gsdml_tEnum    F_CRC_Length_DefaultValue;
  gsdml_tString  F_CRC_Length_AllowedValues;
  gsdml_tBoolean F_CRC_Length_Visible;
  gsdml_tBoolean F_CRC_Length_Changeable;
  gsdml_tInteger    F_Block_ID_DefaultValue;
  gsdml_tValueList  F_Block_ID_AllowedValues;
  gsdml_tBoolean F_Block_ID_Visible;
  gsdml_tBoolean F_Block_ID_Changeable;
  gsdml_tInteger    F_Par_Version_DefaultValue;
  gsdml_tValueList  F_Par_Version_AllowedValues;
  gsdml_tBoolean F_Par_Version_Visible;
  gsdml_tBoolean F_Par_Version_Changeable;
  gsdml_tUnsigned16    F_Source_Add_DefaultValue;
  gsdml_tValueList  F_Source_Add_AllowedValues;
  gsdml_tBoolean F_Source_Add_Visible;
  gsdml_tBoolean F_Source_Add_Changeable;
  gsdml_tUnsigned16    F_Dest_Add_DefaultValue;
  gsdml_tValueList  F_Dest_Add_AllowedValues;
  gsdml_tBoolean F_Dest_Add_Visible;
  gsdml_tBoolean F_Dest_Add_Changeable;
  gsdml_tUnsigned16    F_WD_Time_DefaultValue;
  gsdml_tValueList  F_WD_Time_AllowedValues;
  gsdml_tBoolean F_WD_Time_Visible;
  gsdml_tBoolean F_WD_Time_Changeable;
  gsdml_tUnsigned16    F_Par_CRC_DefaultValue;
  gsdml_tValueList  F_Par_CRC_AllowedValues;
  gsdml_tBoolean F_Par_CRC_Visible;
  gsdml_tBoolean F_Par_CRC_Changeable;
  gsdml_tUnsigned32    F_iPar_CRC_DefaultValue;
  gsdml_tValueList  F_iPar_CRC_AllowedValues;
  gsdml_tBoolean F_iPar_CRC_Visible;
  gsdml_tBoolean F_iPar_CRC_Changeable;
} gsdml_sF_ParameterRecordDataItem;

class gsdml_F_ParameterRecordDataItem {
 public:
  gsdml_F_ParameterRecordDataItem( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sF_ParameterRecordDataItem Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

class gsdml_RecordDataList {
 public:
  gsdml_RecordDataList( pn_gsdml *g) : F_ParameterRecordDataItem(0), gsdml(g) {}
  vector<gsdml_ParameterRecordDataItem *> ParameterRecordDataItem;
  gsdml_F_ParameterRecordDataItem *F_ParameterRecordDataItem;
  pn_gsdml *gsdml;
  ~gsdml_RecordDataList();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tEnum Type;
  gsdml_tRefId GraphicItemTarget;
} gsdml_sGraphicItemRef;

class gsdml_GraphicItemRef {
 public:
  gsdml_GraphicItemRef( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sGraphicItemRef Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

class gsdml_Graphics {
 public:
  gsdml_Graphics( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_GraphicItemRef *> GraphicItemRef;
  pn_gsdml *gsdml;
  ~gsdml_Graphics();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 T_DC_Base;
  gsdml_tUnsigned16 T_DC_Min;
  gsdml_tUnsigned16 T_DC_Max;
  gsdml_tUnsigned32 T_IO_Base;
  gsdml_tUnsigned32 T_IO_InputMin;
  gsdml_tUnsigned32 T_IO_OutputMin;
  gsdml_tBoolean IsochroneModeRequired;
} gsdml_sIsochroneMode;

class gsdml_IsochroneMode {
 public:
  gsdml_IsochroneMode( pn_gsdml *g) : gsdml(g) { memset( &Body, 0, sizeof(Body));}
  gsdml_sIsochroneMode Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

typedef struct {
  gsdml_tId ID;
  gsdml_tUnsigned32hex SubmoduleIdentNumber;
  gsdml_tUnsigned32 API;
  gsdml_tValueList FixedInSubslots;
  gsdml_tBoolean PROFIsafeSupported;
  gsdml_tValueList Writeable_IM_Records;
  gsdml_tUnsigned32 Max_iParameterSize;
  gsdml_tUnsigned16 SubsysModuleDirIndex;
} gsdml_sVirtualSubmoduleItem;

class gsdml_VirtualSubmoduleItem {
 public:
  gsdml_VirtualSubmoduleItem( pn_gsdml *g) : IOData(0), RecordDataList(0), ModuleInfo(0), 
    Graphics(0), IsochroneMode(0), gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sVirtualSubmoduleItem Body;
  gsdml_IOData *IOData;
  gsdml_RecordDataList *RecordDataList;
  gsdml_ModuleInfo *ModuleInfo;
  gsdml_Graphics *Graphics;
  gsdml_IsochroneMode *IsochroneMode;
  pn_gsdml *gsdml;
  ~gsdml_VirtualSubmoduleItem();
  void build();
  void print( int ind);
};

class gsdml_VirtualSubmoduleList {
 public:
  gsdml_VirtualSubmoduleList( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_VirtualSubmoduleItem *> VirtualSubmoduleItem;
  pn_gsdml *gsdml;
  ~gsdml_VirtualSubmoduleList();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tRefIdT TextId;
} gsdml_sDCP_FlashOnceSignalUnit;

class gsdml_DCP_FlashOnceSignalUnit {
 public:
  gsdml_DCP_FlashOnceSignalUnit( pn_gsdml *g) : gsdml(g) 
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sDCP_FlashOnceSignalUnit Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

class gsdml_General {
 public:
  gsdml_General( pn_gsdml *g) : DCP_FlashOnceSignalUnit(0), gsdml(g) {}
  gsdml_DCP_FlashOnceSignalUnit *DCP_FlashOnceSignalUnit;
  pn_gsdml *gsdml;
  ~gsdml_General();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 MaxBridgeDelay;
  gsdml_tUnsigned16 MaxNumberIR_FrameData;
} gsdml_sRT_Class3Properties;

class gsdml_RT_Class3Properties {
 public:
  gsdml_RT_Class3Properties( pn_gsdml *g) : gsdml(g) 
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sRT_Class3Properties Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

typedef struct {
  gsdml_tEnum SupportedRole;
  gsdml_tUnsigned16 MaxLocalJitter;
  gsdml_tUnsigned16 T_PLL_MAX;
  gsdml_tTokenList SupportedSyncProtocols;
} gsdml_sSynchronisationMode;

class gsdml_SynchronisationMode {
 public:
  gsdml_SynchronisationMode( pn_gsdml *g) : gsdml(g) 
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sSynchronisationMode Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

typedef struct {
  gsdml_tValueList SendClock;
  gsdml_tValueList ReductionRatio;
  gsdml_tValueList ReductionRatioPow2;
  gsdml_tValueList ReductionRatioNonPow2;
} gsdml_sTimingProperties;

class gsdml_TimingProperties {
 public:
  gsdml_TimingProperties( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sTimingProperties Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

typedef struct {
  gsdml_tValueList SendClock;
  gsdml_tValueList ReductionRatio;
  gsdml_tValueList ReductionRatioPow2;
  gsdml_tValueList ReductionRatioNonPow2;
} gsdml_sRT_Class3TimingProperties;

class gsdml_RT_Class3TimingProperties {
 public:
  gsdml_RT_Class3TimingProperties( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sRT_Class3TimingProperties Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 NumberOfAdditionalInputCR;
  gsdml_tUnsigned16 NumberOfAdditionalOutputCR;
  gsdml_tUnsigned16 NumberOfAdditionalMulticastProviderCR;
  gsdml_tUnsigned16 NumberOfMulticastConsumerCR;
  gsdml_tUnsigned16 PullModuleAlarmSupported;
} gsdml_sInterfaceSubmoduleItem_ApplicationRelations;

class gsdml_InterfaceSubmoduleItem_ApplicationRelations {
 public:
  gsdml_InterfaceSubmoduleItem_ApplicationRelations( pn_gsdml *g) : TimingProperties(0), RT_Class3TimingProperties(0), gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sInterfaceSubmoduleItem_ApplicationRelations Body;
  gsdml_TimingProperties *TimingProperties;
  gsdml_RT_Class3TimingProperties *RT_Class3TimingProperties;
  pn_gsdml *gsdml;
  ~gsdml_InterfaceSubmoduleItem_ApplicationRelations();
  void print( int ind);
};

typedef struct {
  gsdml_tBoolean RT_MediaRedundancySupported;
  gsdml_tTokenList SupportedRole;
} gsdml_sMediaRedundancy;

class gsdml_MediaRedundancy {
 public:
  gsdml_MediaRedundancy( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sMediaRedundancy Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 SubslotNumber;
  gsdml_tRefId TextId;
  gsdml_tEnum SupportedRT_Class;
  gsdml_tTokenList SupportedRT_Classes;
  gsdml_tBoolean IsochroneModeSupported;
  gsdml_tTokenList IsochroneModeInRT_Classes;
  gsdml_tUnsigned32hex SubmoduleIdentNumber;
  gsdml_tTokenList SupportedProtocols;
  gsdml_tTokenList SupportedMibs;
  gsdml_tBoolean NetworkComponentDiagnosisSupported;
  gsdml_tBoolean DCP_HelloSupported;
  gsdml_tBoolean PTP_BoundarySupported;
  gsdml_tBoolean DCP_BoundarySupported;
} gsdml_sInterfaceSubmoduleItem;

class gsdml_InterfaceSubmoduleItem {
 public:
  gsdml_InterfaceSubmoduleItem( pn_gsdml *g) : General(0), RecordDataList(0), RT_Class3Properties(0), SynchronisationMode(0),
    ApplicationRelations(0), MediaRedundancy(0), gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sInterfaceSubmoduleItem Body;
  gsdml_General *General;
  gsdml_RecordDataList *RecordDataList;
  gsdml_RT_Class3Properties *RT_Class3Properties;
  gsdml_SynchronisationMode *SynchronisationMode;
  gsdml_InterfaceSubmoduleItem_ApplicationRelations *ApplicationRelations;
  gsdml_MediaRedundancy *MediaRedundancy;
  pn_gsdml *gsdml;
  ~gsdml_InterfaceSubmoduleItem();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 SubslotNumber;
  gsdml_tRefId TextId;
  gsdml_tEnum MAUType;
  gsdml_tValueList MAUTypes;
  gsdml_tValueList FiberOpticTypes;
  gsdml_tUnsigned16 MaxPortTxDelay;
  gsdml_tUnsigned16 MaxPortRxDelay;
  gsdml_tUnsigned32 SubmoduleIdentNumber;
  gsdml_tBoolean PortDeactivationSupported;
  gsdml_tEnum LinkStateDiagnosisCapability;
  gsdml_tBoolean PowerBudgetControlSupported;
  gsdml_tBoolean SupportsRingportConfig;
  gsdml_tBoolean IsDefaultRingport;
} gsdml_sPortSubmoduleItem;

class gsdml_PortSubmoduleItem {
 public:
  gsdml_PortSubmoduleItem( pn_gsdml *g) : RecordDataList(0), gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sPortSubmoduleItem Body;
  gsdml_RecordDataList *RecordDataList;
  pn_gsdml *gsdml;
  ~gsdml_PortSubmoduleItem();
  void build();
  void print( int ind);
};

class gsdml_SystemDefinedSubmoduleList {
 public:
  gsdml_SystemDefinedSubmoduleList( pn_gsdml *g) : gsdml(g) {}
  gsdml_InterfaceSubmoduleItem *InterfaceSubmoduleItem;
  vector<gsdml_PortSubmoduleItem *> PortSubmoduleItem;
  pn_gsdml *gsdml;
  ~gsdml_SystemDefinedSubmoduleList();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 AR_BlockVersion;
  gsdml_tUnsigned16 IOCR_BlockVersion;
  gsdml_tUnsigned16 AlarmCR_BlockVersion;
  gsdml_tUnsigned16 SubmoduleDataBlockVersion;;
} gsdml_sDeviceAccessPointItem_ApplicationRelations;

class gsdml_DeviceAccessPointItem_ApplicationRelations {
 public:
  gsdml_DeviceAccessPointItem_ApplicationRelations( pn_gsdml *g) : TimingProperties(0), gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sDeviceAccessPointItem_ApplicationRelations Body;
  gsdml_TimingProperties *TimingProperties;
  pn_gsdml *gsdml;
  ~gsdml_DeviceAccessPointItem_ApplicationRelations();
  void print( int ind);
};

typedef struct {
  gsdml_tRefId SubmoduleItemTarget;
  gsdml_tValueList AllowedInSubslots;
  gsdml_tValueList UsedInSubslots;
  gsdml_tValueList FixedInSubslots;
} gsdml_sSubmoduleItemRef;

class gsdml_SubmoduleItemRef {
 public:
  gsdml_SubmoduleItemRef( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sSubmoduleItemRef Body;
  pn_gsdml *gsdml;
  void build();
  void print( int ind);
};

class gsdml_UseableSubmodules {
 public:
  gsdml_UseableSubmodules( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_SubmoduleItemRef *> SubmoduleItemRef;
  pn_gsdml *gsdml;
  ~gsdml_UseableSubmodules();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 SlotNumber;
  gsdml_tRefId TextId;
} gsdml_sSlotItem;

class gsdml_SlotItem {
 public:
  gsdml_SlotItem( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sSlotItem Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

class gsdml_SlotList {
 public:
  gsdml_SlotList( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_SlotItem *> SlotItem;
  pn_gsdml *gsdml;
  ~gsdml_SlotList();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tValueList SlotList;
  gsdml_tRefId Name;
  gsdml_tRefId InfoText;
} gsdml_sSlotGroup;

class gsdml_SlotGroup {
 public:
  gsdml_SlotGroup( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sSlotGroup Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

class gsdml_SlotGroups {
 public:
  gsdml_SlotGroups( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_SlotGroup *> SlotGroup;
  pn_gsdml *gsdml;
  ~gsdml_SlotGroups();
  void print( int ind);
};

typedef struct {
  gsdml_tId ID;
  gsdml_tValueList PhysicalSlots;
  gsdml_tUnsigned32hex ModuleIdentNumber;
  gsdml_tUnsigned16 MinDeviceInterval;
  gsdml_tNormalizedString ImplementationType;
  gsdml_tString DNS_CompatibleName;
  gsdml_tBoolean ExtendedAddressAssignmentSupported;
  gsdml_tTokenList AddressAssignment;
  gsdml_tValueList AllowedInSlots;
  gsdml_tValueList FixedInSlots;
  gsdml_tUnsigned16 ObjectUUID_LocalIndex;
  gsdml_tString RequiredSchemaVersion;
  gsdml_tBoolean MultipleWriteSupported;
  gsdml_tBoolean IOXS_Required;
  gsdml_tValueList PhysicalSubslots;
  gsdml_tUnsigned16 RemoteApplicationTimeout;
  gsdml_tUnsigned32 MaxSupportedRecordSize;
  gsdml_tUnsigned32 PowerOnToCommReady;
  gsdml_tBoolean ParameterizationSpeedSupported;
  gsdml_tBoolean NameOfStationNotTransferable;
} gsdml_sDeviceAccessPointItem;

class gsdml_DeviceAccessPointItem {
 public:
  gsdml_DeviceAccessPointItem( pn_gsdml *g) : ModuleInfo(0), SubslotList(0), IOConfigData(0), UseableModules(0),
    VirtualSubmoduleList(0), SystemDefinedSubmoduleList(0), Graphics(0), ApplicationRelations(0),
    UseableSubmodules(0), SlotList(0), SlotGroups(0), gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sDeviceAccessPointItem Body;
  gsdml_ModuleInfo *ModuleInfo;
  gsdml_SubslotList *SubslotList;
  gsdml_IOConfigData *IOConfigData;
  gsdml_UseableModules *UseableModules;
  gsdml_VirtualSubmoduleList *VirtualSubmoduleList;
  gsdml_SystemDefinedSubmoduleList *SystemDefinedSubmoduleList;
  gsdml_Graphics *Graphics;
  gsdml_DeviceAccessPointItem_ApplicationRelations *ApplicationRelations;
  gsdml_UseableSubmodules *UseableSubmodules;
  gsdml_SlotList *SlotList;
  gsdml_SlotGroups *SlotGroups;
  pn_gsdml *gsdml;
  ~gsdml_DeviceAccessPointItem();
  void build();
  void print( int ind);
};
  
class gsdml_DeviceAccessPointList {
 public:
  gsdml_DeviceAccessPointList( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_DeviceAccessPointItem *> DeviceAccessPointItem;
  pn_gsdml *gsdml;
  ~gsdml_DeviceAccessPointList();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tId ID;
  gsdml_tUnsigned32hex ModuleIdentNumber;
  gsdml_tString RequiredSchemaVersion;
  gsdml_tValueList PhysicalSubslots;
} gsdml_sModuleItem;

class gsdml_ModuleItem {
 public:
  gsdml_ModuleItem( pn_gsdml *g) : ModuleInfo(0), SubslotList(0), VirtualSubmoduleList(0), 
    SystemDefinedSubmoduleList(0), UseableSubmodules(0), Graphics(0), gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sModuleItem Body;
  gsdml_ModuleInfo *ModuleInfo;
  gsdml_SubslotList *SubslotList;
  gsdml_VirtualSubmoduleList *VirtualSubmoduleList;
  gsdml_SystemDefinedSubmoduleList *SystemDefinedSubmoduleList;
  gsdml_UseableSubmodules *UseableSubmodules;
  gsdml_Graphics *Graphics;
  pn_gsdml *gsdml;
  ~gsdml_ModuleItem();
  void build();
  void print( int ind);
};

class gsdml_ModuleList {
 public:
  gsdml_ModuleList( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_ModuleItem *> ModuleItem;
  pn_gsdml *gsdml;
  ~gsdml_ModuleList();
  void build();
  void print( int ind);
};

class gsdml_SubmoduleList {
 public:
  gsdml_SubmoduleList( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_VirtualSubmoduleItem *> SubmoduleItem;
  pn_gsdml *gsdml;
  ~gsdml_SubmoduleList();
  void print( int ind);
};

typedef struct {
  gsdml_tString Content;
   gsdml_tRefIdT TextId;
} gsdml_sAssign;

class gsdml_Assign {
 public:
  gsdml_Assign( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sAssign Body;
  pn_gsdml *gsdml;
  void build();
  void print( int ind);
};

class gsdml_Assignments {
 public:
  gsdml_Assignments( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_Assign *> Assign;
  pn_gsdml *gsdml;
  ~gsdml_Assignments();
  void print( int ind);
};

typedef struct {
  gsdml_tId ID;
  gsdml_tRefIdT Help;
} gsdml_sValueItem;

class gsdml_ValueItem {
 public:
  gsdml_ValueItem( pn_gsdml *g) : Assignments(0), gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sValueItem Body;
  gsdml_Assignments *Assignments;
  pn_gsdml *gsdml;
  ~gsdml_ValueItem();
  void build();
  void print( int ind);
};

class gsdml_ValueList {
 public:
  gsdml_ValueList( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_ValueItem *> ValueItem;
  pn_gsdml *gsdml;
  ~gsdml_ValueList();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned8 Id;
  gsdml_tEnum DataType;
  gsdml_tUnsigned16 Length;
} gsdml_sExtChannelAddValue_DataItem;

class gsdml_ExtChannelAddValue_DataItem {
 public:
  gsdml_ExtChannelAddValue_DataItem( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sExtChannelAddValue_DataItem Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

class gsdml_ExtChannelAddValue {
 public:
  gsdml_ExtChannelAddValue( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_ExtChannelAddValue_DataItem *> DataItem;
  pn_gsdml *gsdml;
  ~gsdml_ExtChannelAddValue();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 ErrorType;
  gsdml_tTokenList MaintenanceAlarmState;
  gsdml_tUnsigned32 API;
  gsdml_tRefId Name;
  gsdml_tRefId Help;
} gsdml_sExtChannelDiagItem;

class gsdml_ExtChannelDiagItem {
 public:
  gsdml_ExtChannelDiagItem( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sExtChannelDiagItem Body;
  vector<gsdml_ExtChannelAddValue *> ExtChannelAddValue;
  pn_gsdml *gsdml;
  ~gsdml_ExtChannelDiagItem();
  void print( int ind);
};

class gsdml_ExtChannelDiagList {
 public:
  gsdml_ExtChannelDiagList( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_ExtChannelDiagItem *> ExtChannelDiagItem;
  pn_gsdml *gsdml;
  ~gsdml_ExtChannelDiagList();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 ErrorType;
  gsdml_tTokenList MaintenanceAlarmState;
  gsdml_tUnsigned32 API;
  gsdml_tRefId Name;
  gsdml_tRefId Help;
} gsdml_sChannelDiagItem;

class gsdml_ChannelDiagItem {
 public:
  gsdml_ChannelDiagItem( pn_gsdml *g) : ExtChannelDiagList(0), gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sChannelDiagItem Body;
  gsdml_ExtChannelDiagList *ExtChannelDiagList;
  pn_gsdml *gsdml;
  ~gsdml_ChannelDiagItem();
  void build();
  void print( int ind);
};

class gsdml_ChannelDiagList {
 public:
  gsdml_ChannelDiagList( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_ChannelDiagItem *> ChannelDiagItem;
  pn_gsdml *gsdml;
  ~gsdml_ChannelDiagList();
  void build();
  void print( int ind);
};

typedef struct {
  gsdml_tUnsigned16 UserStructureIdentifier;
  gsdml_tUnsigned32 API;
} gsdml_sUnitDiagTypeItem;

class gsdml_UnitDiagTypeItem {
 public:
  gsdml_UnitDiagTypeItem( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sUnitDiagTypeItem Body;
  vector<gsdml_Ref *> Ref;
  pn_gsdml *gsdml;
  ~gsdml_UnitDiagTypeItem();
  void print( int ind);
};

class gsdml_UnitDiagTypeList {
 public:
  gsdml_UnitDiagTypeList( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_UnitDiagTypeItem *> UnitDiagTypeItem;
  pn_gsdml *gsdml;
  ~gsdml_UnitDiagTypeList();
  void print( int ind);
};

typedef struct {
  gsdml_tId ID;
  gsdml_tString GraphicFile;
  gsdml_tString Embedded;
} gsdml_sGraphicItem;

class gsdml_GraphicItem {
 public:
  gsdml_GraphicItem( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sGraphicItem Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

class gsdml_GraphicsList {
 public:
  gsdml_GraphicsList( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_GraphicItem *> GraphicItem;
  pn_gsdml *gsdml;
  ~gsdml_GraphicsList();
  void print( int ind);
};

typedef struct {
  gsdml_tId ID;
  gsdml_tRefIdT TextId;
  gsdml_tRefId InfoText;
} gsdml_sCategoryItem;

class gsdml_CategoryItem {
 public:
  gsdml_CategoryItem( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sCategoryItem Body;
  pn_gsdml *gsdml;
  void print( int ind);
};

class gsdml_CategoryList {
 public:
  gsdml_CategoryList( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_CategoryItem *> CategoryItem;
  pn_gsdml *gsdml;
  ~gsdml_CategoryList();
  void print( int ind);
};

typedef struct {
  gsdml_tIdT TextId;
  gsdml_tAllocatedString Value;
} gsdml_sText;

class gsdml_Text {
 public:
  gsdml_Text( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sText Body;
  pn_gsdml *gsdml;
  ~gsdml_Text();
  void print( int ind);
};

class gsdml_PrimaryLanguage {
 public:
  gsdml_PrimaryLanguage( pn_gsdml *g) : gsdml(g) {}
  vector<gsdml_Text *> Text;
  pn_gsdml *gsdml;
  ~gsdml_PrimaryLanguage();
  void print( int ind);
};

typedef struct {
  gsdml_tEnum xml_lang;
} gsdml_sLanguage;

class gsdml_Language {
 public:
  gsdml_Language( pn_gsdml *g) : gsdml(g)
    { memset( &Body, 0, sizeof(Body));}
  gsdml_sLanguage Body;
  vector<gsdml_Text *> Text;
  pn_gsdml *gsdml;
  ~gsdml_Language();
  void print( int ind);
};

class gsdml_ExternalTextList {
 public:
  gsdml_ExternalTextList( pn_gsdml *g) : gsdml(g) {}
  gsdml_PrimaryLanguage *PrimaryLanguage;
  vector<gsdml_Language *> Language;
  pn_gsdml *gsdml;
  ~gsdml_ExternalTextList();
  void print( int ind);
};

class gsdml_ApplicationProcess {
 public:
  gsdml_ApplicationProcess( pn_gsdml *g) : DeviceAccessPointList(0), ModuleList(0), SubmoduleList(0),
    ValueList(0), ChannelDiagList(0), UnitDiagTypeList(0), GraphicsList(0), CategoryList(0), gsdml(g) {}
  gsdml_DeviceAccessPointList *DeviceAccessPointList;
  gsdml_ModuleList *ModuleList;
  gsdml_SubmoduleList *SubmoduleList;
  gsdml_ValueList *ValueList;
  gsdml_ChannelDiagList *ChannelDiagList;
  gsdml_UnitDiagTypeList *UnitDiagTypeList;
  gsdml_GraphicsList *GraphicsList;
  gsdml_CategoryList *CategoryList;
  gsdml_ExternalTextList *ExternalTextList;
  pn_gsdml *gsdml;
  ~gsdml_ApplicationProcess();
  void build();
  void print( int ind);
};

class pn_gsdml {
 public:
  pn_gsdml();
  ~pn_gsdml();
    
  ifstream fp;
  int logglevel;
  bool first_token;
  unsigned int state;
  int line_cnt;
  int char_cnt;
  bool in_comment;
  char c;
  char c_f;
  char c_ff;
  void *c_sts;
  void *c_f_sts;
  void *c_ff_sts;
  char current_tag[256];
  char current_attribute_name[256];
  char current_attribute_value[4096];
  char current_tag_value[256];
  unsigned int current_tag_idx;
  unsigned int current_attribute_name_idx;
  unsigned int current_attribute_value_idx;
  unsigned int current_tag_value_idx;
  int suppress_msg;
  gsdml_eTag tag_stack[100];
  unsigned int tag_stack_cnt;
  void *object_stack[100];
  gsdml_eTag object_stack_id[100];
  unsigned int object_stack_cnt;
  void *current_body;
  unsigned int current_body_size;
  char current_lang[10];
  int byte_order;
  gsdml_sModuleClass *module_classlist;
  pwr_tFileName gsdmlfile;
  gsdml_Xml *xml;
  gsdml_ProfileHeader *ProfileHeader;
  gsdml_DeviceIdentity *DeviceIdentity;
  gsdml_DeviceFunction *DeviceFunction;
  gsdml_ApplicationProcess *ApplicationProcess;
 
  int read( const char *filename);
  int tag( const char *name);
  int metatag( const char *name);
  int tag_end( const char *name);
  int metatag_end( const char *name);
  int tag_value( const char *value);
  int tag_attribute( const char *name, const char *value);
  int tag_stack_push( gsdml_eTag id);
  int tag_stack_pull( gsdml_eTag id);
  int object_stack_push( void *o, gsdml_eTag id);
  int object_stack_pull( gsdml_eTag id);
  void set_language( const char *lang);
  static int ostring_to_data( unsigned char **data, const char *str, int size, int *rsize);
  static int data_to_ostring( unsigned char *data, int size, char *str, int strsize);
  static int string_to_value_datatype( char *str, gsdml_eValueDataType *type);
  int datavalue_to_string( gsdml_eValueDataType datatype, void *value,
			   unsigned int size, char *str, unsigned int strsize);
  int string_to_datavalue( gsdml_eValueDataType datatype, void *value,
			   unsigned int size, char *str);
  void set_classes( gsdml_sModuleClass *mclist) { module_classlist = mclist;}
  int set_par_record_default( unsigned char *data, int size, 
			      gsdml_ParameterRecordDataItem *par_record);

  void *next_token();
  bool is_space( const char c);
  int find_tag( const char *name, int *idx);
  int find_tag( gsdml_eTag id, int *idx);
  int find_tag_attribute( const char *name, gsdml_eTag id, gsdml_eTag pid, int *idx);
  void error_message_line( const char *msg);
  void error_message( const char *format, const char *value);
  gsdml_eTag get_tag_stack();
  gsdml_eTag get_tag_stack( int p); 
  void *get_object_stack( gsdml_eTag id);
  void *get_object_stack( int p, gsdml_eTag id); 
  int string_to_value( gsdml_eType type, unsigned int size, const char *str, void *buf);
  int set_default_values( gsdml_eTag id, void *data, unsigned int size);
  void *object_factory( gsdml_eTag id);
  void *find_value_ref( char *ref);
  void *find_module_ref( char *ref);
  void *find_submodule_ref( char *ref);
  void *find_category_ref( char *ref);
  void *find_text_ref( char *ref);
  void *find_graphic_ref( char *ref);
  void gsdml_print();
  void build();
};

#endif
