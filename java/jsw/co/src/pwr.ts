"use strict";

function even(sts) {
  return sts % 2 === 0;
}

function odd(sts) {
  return sts % 2 === 1;
}

function clamp(val, min, max) {
  return val > max ? max : (val < min ? min : val);
}

enum Type {
  Bit = (1 << 15) + 1,
  Boolean = 98305,
  Float32,
  Float64,
  Char,
  Int8,
  Int16,
  Int32,
  UInt8,
  UInt16,
  UInt32,
  Objid,
  Buffer,
  String,
  Enum,
  Struct,
  Mask,
  Array,
  Time,
  Text,
  AttrRef,
  UInt64,
  Int64,
  ClassId,
  TypeId,
  VolumeId,
  ObjectIx,
  RefId,
  DeltaTime,
  Status,
  NetStatus,
  CastId,
  ProString,
  DisableAttr,
  DataRef
}

enum Prv {
  RtRead = 1 << 0,
  RtWrite = 1 << 1,
  System = 1 << 2,
  Maintenance = 1 << 3,
  Process = 1 << 4,
  Instrument = 1 << 5,
  Operator1 = 1 << 6,
  Operator2 = 1 << 7,
  Operator3 = 1 << 8,
  Operator4 = 1 << 9,
  Operator5 = 1 << 10,
  Operator6 = 1 << 11,
  Operator7 = 1 << 12,
  Operator8 = 1 << 13,
  Operator9 = 1 << 14,
  Operator10 = 1 << 15,
  RtEventsAck = 1 << 18,
  RtPlc = 1 << 19,
  RtNavigator = 1 << 20,
  DevRead = 1 << 21,
  DevPlc = 1 << 22,
  DevConfig = 1 << 23,
  DevClass = 1 << 24,
  RtEventsBlock = 1 << 25,
  Administrator = 1 << 26,
  SevRead = 1 << 27,
  SevAdmin = 1 << 28
}

enum Access {
  RtRead = 1 << 0,
  RtWrite = 1 << 1,
  System = 1 << 2,
  Maintenance = 1 << 3,
  Process = 1 << 4,
  Instrument = 1 << 5,
  RtEventsBlock = 1 << 25,
  RtEventsAck = 1 << 18,
  RtPlc = 1 << 19,
  RtNavigator = 1 << 20,
  AllRt = 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 0 | 1 << 1 | 1 << 25 |
      1 << 18 | 1 << 19 | 1 << 20 | 1 << 6 | 1 << 7 | 1 << 8 | 1 << 9 | 1 << 10 |
      1 << 11 | 1 << 12 | 1 << 13 | 1 << 14 | 1 << 15,
  AllOperators = 1 << 6 | 1 << 7 | 1 << 8 | 1 << 9 | 1 << 10 | 1 << 11 |
      1 << 12 | 1 << 13 | 1 << 14 | 1 << 15,
  AllPwr = ~0
}

enum Adef {
  Pointer = 1,
  Array = 2,
  Backup = 4,
  Changelog = 8,
  State = 16,
  Const = 32,
  Rtvirtual = 64,
  Devbodyref = 128,
  Dynamic = 256,
  Publicwrite = 512,
  Noedit = 1024,
  Invisible = 2048,
  Refdirect = 4096,
  Noinvert = 8192,
  Noremove = 16384,
  Rtdbref = 32768,
  Private = 65536,
  Class = 131072,
  Superclass = 262144,
  Buffer = 524288,
  Nowbl = 1048576,
  Alwayswbl = 2097152,
  Disableattr = 4194304,
  Rthide = 8388608
}

enum XttMethodsFlagsMask {
  IsConfigured = 1
}

enum XttOpMethodsMask {
  OpenGraph = 1,
  OpenObjectGraph = 2,
  OpenTrend = 4,
  OpenHistory = 8,
  OpenFast = 16,
  Camera = 32,
  HistEvent = 64,
  BlockEvents = 128,
  Help = 256,
  Photo = 512,
  Note = 1024,
  ParentObjectGraph = 2048
}

enum XttMntMethodsMask {
  OpenObject = 1,
  OpenTrace = 2,
  RtNavigator = 4,
  OpenCrossref = 8,
  HelpClass = 16,
  DataSheet = 32,
  CircuitDiagram = 64,
  Simulate = 1 << 31
}

class PwrtObjid {
  vid: number;
  oix: number;

  constructor(vid, oix) {
    this.oix = oix;
    this.vid = vid;
  }
}

class PwrtAttrRef {
  objid;
  offset;
  body;
  size;
  flags;
}

class CdhrNumber {
  value: number;
  sts: number;

  constructor(value, sts) {
    this.value = value;
    this.sts = sts;
  }
}

enum UserVol {
  Min = ((0 << 24) + (1 << 16) + (1 << 8) + 1),
  Max = ((0 << 24) + (254 << 16) + (254 << 8) + 254)
}

class UserdataCbReturn {
  userdata;
  row;
}

class Point {
  x = 0;
  y = 0;

  constructor() {
  }

  constructor(x: number, y: number) {
    this.x = x;
    this.y = y;
  }
}

class Rect {
  x = 0;
  y = 0;
  width = 0;
  height = 0;

  constructor() {
  }

  constructor(x: number, y: number, width: number, height: number) {
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
  }
}