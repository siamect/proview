"use strict";

function PwrtStatus(sts) {
  this.sts = sts;

  this.evenSts = function () {
    return (sts % 2 === 0);
  };
  this.oddSts = function () {
    return (sts % 2 == 1);
  };
  this.getSts = function () {
    return sts;
  };
}

var Pwr = {
  eType_Boolean: 98305,
  eType_Float32: 98306,
  eType_Float64: 98307,
  eType_Char: 98308,
  eType_Int8: 98309,
  eType_Int16: 98310,
  eType_Int32: 98311,
  eType_UInt8: 98312,
  eType_UInt16: 98313,
  eType_UInt32: 98314,
  eType_Objid: 98315,
  eType_Buffer: 98316,
  eType_String: 98317,
  eType_Enum: 98318,
  eType_Struct: 98319,
  eType_Mask: 98320,
  eType_Array: 98321,
  eType_Time: 98322,
  eType_Text: 98323,
  eType_AttrRef: 98324,
  eType_UInt64: 98325,
  eType_Int64: 98326,
  eType_ClassId: 98327,
  eType_TypeId: 98328,
  eType_VolumeId: 98329,
  eType_ObjectIx: 98330,
  eType_RefId: 98331,
  eType_DeltaTime: 98332,
  eType_Status: 98333,
  eType_NetStatus: 98334,
  eType_CastId: 98335,
  eType_ProString: 98336,
  eType_DisableAttr: 98337,
  eType_DataRef: 98338,

  mPrv_RtRead: 1 << 0,
  mPrv_RtWrite: 1 << 1,
  mPrv_System: 1 << 2,
  mPrv_Maintenance: 1 << 3,
  mPrv_Process: 1 << 4,
  mPrv_Instrument: 1 << 5,
  mPrv_Operator1: 1 << 6,
  mPrv_Operator2: 1 << 7,
  mPrv_Operator3: 1 << 8,
  mPrv_Operator4: 1 << 9,
  mPrv_Operator5: 1 << 10,
  mPrv_Operator6: 1 << 11,
  mPrv_Operator7: 1 << 12,
  mPrv_Operator8: 1 << 13,
  mPrv_Operator9: 1 << 14,
  mPrv_Operator10: 1 << 15,
  mPrv_RtEventsAck: 1 << 18,
  mPrv_RtPlc: 1 << 19,
  mPrv_RtNavigator: 1 << 20,
  mPrv_DevRead: 1 << 21,
  mPrv_DevPlc: 1 << 22,
  mPrv_DevConfig: 1 << 23,
  mPrv_DevClass: 1 << 24,
  mPrv_RtEventsBlock: 1 << 25,
  mPrv_Administrator: 1 << 26,
  mPrv_SevRead: 1 << 27,
  mPrv_SevAdmin: 1 << 28,

  mAccess_RtRead: 1 << 0,
  mAccess_RtWrite: 1 << 1,
  mAccess_System: 1 << 2,
  mAccess_Maintenance: 1 << 3,
  mAccess_Process: 1 << 4,
  mAccess_Instrument: 1 << 5,
  mAccess_RtEventsBlock: 1 << 25,
  mAccess_RtEventsAck: 1 << 18,
  mAccess_RtPlc: 1 << 19,
  mAccess_RtNavigator: 1 << 20,
  mAccess_AllRt: 1 << 2 |
  1 << 3 |
  1 << 4 |
  1 << 5 |
  1 << 0 |
  1 << 1 |
  1 << 25 |
  1 << 18 |
  1 << 19 |
  1 << 20 |
  1 << 6 |
  1 << 7 |
  1 << 8 |
  1 << 9 |
  1 << 10 |
  1 << 11 |
  1 << 12 |
  1 << 13 |
  1 << 14 |
  1 << 15,
  mAccess_AllOperators: 1 << 6 |
  1 << 7 |
  1 << 8 |
  1 << 9 |
  1 << 10 |
  1 << 11 |
  1 << 12 |
  1 << 13 |
  1 << 14 |
  1 << 15,
  mAccess_AllPwr: ~0,

  mAdef_pointer: 1,
  mAdef_array: 2,
  mAdef_backup: 4,
  mAdef_changelog: 8,
  mAdef_state: 16,
  mAdef_const: 32,
  mAdef_rtvirtual: 64,
  mAdef_devbodyref: 128,
  mAdef_dynamic: 256,
  mAdef_publicwrite: 512,
  mAdef_noedit: 1024,
  mAdef_invisible: 2048,
  mAdef_refdirect: 4096,
  mAdef_noinvert: 8192,
  mAdef_noremove: 16384,
  mAdef_rtdbref: 32768,
  mAdef_private: 65536,
  mAdef_class: 131072,
  mAdef_superclass: 262144,
  mAdef_buffer: 524288,
  mAdef_nowbl: 1048576,
  mAdef_alwayswbl: 2097152,
  mAdef_disableattr: 4194304,
  mAdef_rthide: 8388608
};

var Pwrb = {
  mXttMethodsFlagsMask_IsConfigured: 1,

  mXttOpMethodsMask_OpenGraph: 1,
  mXttOpMethodsMask_OpenObjectGraph: 2,
  mXttOpMethodsMask_OpenTrend: 4,
  mXttOpMethodsMask_OpenHistory: 8,
  mXttOpMethodsMask_OpenFast: 16,
  mXttOpMethodsMask_Camera: 32,
  mXttOpMethodsMask_HistEvent: 64,
  mXttOpMethodsMask_BlockEvents: 128,
  mXttOpMethodsMask_Help: 256,
  mXttOpMethodsMask_Photo: 512,
  mXttOpMethodsMask_Note: 1024,
  mXttOpMethodsMask_ParentObjectGraph: 2048,

  mXttMntMethodsMask_OpenObject: 1,
  mXttMntMethodsMask_OpenTrace: 2,
  mXttMntMethodsMask_RtNavigator: 4,
  mXttMntMethodsMask_OpenCrossref: 8,
  mXttMntMethodsMask_HelpClass: 16,
  mXttMntMethodsMask_DataSheet: 32,
  mXttMntMethodsMask_CircuitDiagram: 64,
  mXttMntMethodsMask_Simulate: 1 << 31
};

function PwrtObjid(vid, oix) {
  this.oix = oix;
  this.vid = vid;
}

function PwrtAttrRef() {
  this.objid;
  this.offset;
  this.body;
  this.size;
  this.flags;
}

function CdhrNumber(value, sts) {
  this.value = value;
  this.sts = sts;

  this.evenSts = function () {
    return (sts % 2 === 0);
  };
  this.oddSts = function () {
    return (sts % 2 == 1);
  };
  this.getSts = function () {
    return sts;
  };
}

var CdhC = {
  cUserVolMin: (0 + (0 << 24) + (1 << 16) + (1 << 8) + 1),
  cUserVolMax: (0 + (0 << 24) + (254 << 16) + (254 << 8) + 254)
};

function UserdataCbReturn() {
  this.userdata;
  this.row;
}