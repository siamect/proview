"use strict";

var MhC = {
  eEvent_Ack: 1,
  eEvent_Block: 2,
  eEvent_Cancel: 3,
  eEvent_CancelBlock: 4,
  eEvent_Missing: 5,
  eEvent_Reblock: 6,
  eEvent_Return: 7,
  eEvent_Unblock: 8,
  eEvent_InfoSuccess: 32,
  eEvent_Alarm: 64,
  eEvent_MaintenanceAlarm: 128,
  eEvent_SystemAlarm: 256,
  eEvent_UserAlarm1: 512,
  eEvent_UserAlarm2: 1024,
  eEvent_UserAlarm3: 2048,
  eEvent_UserAlarm4: 4096,
  eEvent_Info: 8192,

  eEventPrio_A: 67,
  eEventPrio_B: 66,
  eEventPrio_C: 65,
  eEventPrio_D: 64,

  mEventStatus_NotRet: 1 << 0,
  mEventStatus_NotAck: 1 << 1,
  mEventStatus_Block: 1 << 2
};

function MhEventId() {
  this.nix;
  this.idx;
  this.birthTime;
}

function MhEvent() {
  this.eventTime;
  this.eventText;
  this.eventName;
  this.eventFlags;
  this.eventStatus;
  this.eventId;
  this.targetId;
  this.eventType;
  this.object;
  this.supObject;
  this.eventMoreText;
  this.syncIdx;
}

/** Start Gdh **/
function Sub() {
  this.sts;
  this.refid;
  this.type;
  this.elements;
  this.name;
  this.value;
}

function ObjectInfo() {
  this.objid;
  this.cid;
  this.has_children;
  this.name;
  this.description;
  this.classname;
  this.full_name;
  this.param1;
}

function AttributeInfo() {
  this.name;
  this.type;
  this.size;
  this.flags;
  this.element;
  this.objid;
  this.full_name;
  this.classname;
}

function MenuButton() {
  this.type;
  this.text;
  this.name;
  this.url;
}

function OpwindMenuInfo() {
  this.title;
  this.text;
  this.enable_language;
  this.enable_login;
  this.enable_alarmlist;
  this.enable_eventlog;
  this.enable_navigator;
  this.disable_help;
  this.disable_proview;
  this.language;
  this.buttons = [];
}

function CrrInfo() {
  this.type;
  this.objid;
  this.name;
  this.classname;
}

function GlowPieInfo() {
  this.sector_num;
  this.min_val;
  this.max_val;
}

function GlowBarChartInfo() {
  this.bars;
  this.barsegments;
  this.min_value;
  this.max_value;
}

function GlowTableInfo() {
  this.columns;
  this.rows;
  this.column_size = new Array(Glow.TABLE_MAX_COL);
}

function PendingData(func_cb, data) {
  this.func_cb = func_cb;
  this.data = data;
}

var GdhOp = {
  GET_OP_SELF: 1,
  GET_OP_METHOD_PLC: 2,
  GET_OP_METHOD_OBJECTGRAPH: 3,
  GET_OP_METHOD_GRAPH: 4,
  GET_OP_METHOD_HELPCLASS: 5
};

function Gdh() {
  var Msg = {
    SET_OBJECT_INFO_BOOLEAN: 1,
    SET_OBJECT_INFO_FLOAT: 2,
    SET_OBJECT_INFO_INT: 3,
    SET_OBJECT_INFO_STRING: 4,
    GET_OBJECT_INFO_BOOLEAN: 5,
    GET_OBJECT_INFO_FLOAT: 6,
    GET_OBJECT_INFO_INT: 7,
    GET_OBJECT_INFO_STRING: 8,
    TOGGLE_OBJECT_INFO: 9,
    REF_OBJECT_INFO: 10,
    GET_OBJECT_REF_INFO_BOOLEAN: 11,
    GET_OBJECT_REF_INFO_FLOAT: 12,
    GET_OBJECT_REF_INFO_INT: 13,
    GET_OBJECT_REF_INFO_STRING: 14,
    UNREF_OBJECT_INFO: 15,
    NAME_TO_OBJID: 16,
    OBJID_TO_NAME: 17,
    GET_ROOT_LIST: 18,
    GET_NEXT_OBJECT: 19,
    GET_CHILD: 20,
    GET_NEXT_SIBLING: 21,
    GET_OBJECT_CLASS: 22,
    GET_CLASS_LIST: 23,
    CLASS_ID_TO_OBJID: 24,
    GET_OBJECT_REF_INFO_ALL: 25,
    REF_OBJECT_INFO_LIST: 26,
    POLL: 27,
    STATISTICS: 28,
    CHECK_USER: 29,
    GET_NODE_OBJECT: 30,
    LOG_STRING: 31,
    UNREF_OBJECT_INFO_ALL: 32,
    CREATE_INSTANCE_FILE: 33,
    GET_ATTRIBUTE_CHAR: 34,
    GET_CLASS_ATTRIBUTE: 35,
    GET_ALL_CLASS_ATTRIBUTES: 36,
    GET_ALL_SIBLINGS: 37,
    GET_ALL_XTT_SIBLINGS: 38,
    GET_ALL_XTT_CHILDREN: 39,
    REF_OBJECT_INFO_VECTOR: 40,
    GET_SUBSCRIPTIONS: 41,
    CRR_SIGNAL: 42,
    CRR_OBJECT: 43,
    GET_PARENT: 44,
    GET_OBJECT_INFO_OBJID: 45,
    GET_OBJECT_REF_INFO_BOOLEAN_ARRAY: 46,
    GET_OBJECT_REF_INFO_FLOAT_ARRAY: 47,
    GET_OBJECT_REF_INFO_INT_ARRAY: 48,
    GET_OBJECT_REF_INFO_STRING_ARRAY: 49,
    GET_MSG: 50,
    GET_MSG_TEXT: 51,
    NAME_TO_ATTRREF: 52,
    ATTRREF_TO_NAME: 53,
    GET_ATTRREF_TID: 54,
    GET_SUPER_CLASS: 55,
    GET_ALL_CLASS_ATTRIBUTES_STRING: 56,
    GET_OBJECT_INFO_FLOAT_ARRAY: 57,
    GET_OBJECT_INFO_INT_ARRAY: 58,
    GET_CIRCBUFF_INFO: 59,
    UPDATE_CIRCBUFF_INFO: 60,
    GET_ATTRIBUTE_FLAGS: 61,
    CLASSNAME_TO_ID: 62,
    GET_OBJECT: 63,
    GET_OPWIND_MENU: 64,
    GET_OBJECT_FROM_NAME: 65,
    MH_SYNC: 66,
    MH_ACK: 67,
    GET_OBJECT_FROM_AREF: 68
  };

  this.debug = false;
  this.pending = [];
  this.sub = [];
  this.PORT = 4448;
  this.ws = null;
  this.open_cb = null;
  this.close_cb = null;
  this.return_cb = null;
  this.next_id = 1234;
  this.subscriptionCount = 1;
  this.listSend = false;

  this.init = function () {
    if (window.location.hostname === "")
      this.ws = new WebSocket("ws:127.0.0.1:4448");
    else
      this.ws = new WebSocket("ws://" + window.location.hostname + ":4448");
    this.ws.binaryType = "arraybuffer";
    this.ws.gdh = this;

    this.ws.onopen = function (e) {
      if (this.gdh.open_cb !== null)
        this.gdh.open_cb();
    };

    this.ws.onclose = function () {
      if (this.debug) console.log("Socket closed");
      if (this.gdh.close_cb !== null)
        this.gdh.close_cb();
    };

    this.ws.onmessage = function (e) {
      if (typeof e.data == "string") {
        console.log("String message received", e, e.data);
      }
      else {
        if (e.data instanceof ArrayBuffer) {
          var dv = new DataView(e.data);
          var type = dv.getUint8(0);
          var id = dv.getUint32(1);
          var sts = dv.getUint32(5);

          switch (type) {
            case Msg.GET_OBJECT_INFO_BOOLEAN: {
              if (this.gdh.debug) console.log("GetObjectInfoBoolean received");
              var value = dv.getUint8(9);
              var func_cb = this.gdh.pending[id].func_cb;
              func_cb(id, sts, value);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OBJECT_INFO_INT: {
              if (this.gdh.debug) console.log("GetObjectInfoInt received");
              var value = dv.getUint32(9);
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, value);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OBJECT_INFO_FLOAT: {
              if (this.gdh.debug) console.log("GetObjectInfoFloat received");
              var value = dv.getFloat32(9);
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, value);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OBJECT_INFO_FLOAT_ARRAY: {
              if (this.gdh.debug) console.log("GetObjectInfoFloatArray received");
              var asize = dv.getInt32(9);
              var value = new Array(asize);
              k = 13;
              for (var i = 0; i < asize; i++) {
                value[i] = dv.getFloat32(k);
                k += 4;
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, value);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.SET_OBJECT_INFO_BOOLEAN: {
              if (this.gdh.debug) console.log("SetObjectInfoBoolean received", id, sts);
              break;
            }
            case Msg.SET_OBJECT_INFO_INT: {
              if (this.gdh.debug) console.log("SetObjectInfoInt received", id, sts);
              break;
            }
            case Msg.SET_OBJECT_INFO_FLOAT: {
              if (this.gdh.debug) console.log("SetObjectInfoFloat received", id, sts);
              break;
            }
            case Msg.SET_OBJECT_INFO_STRING: {
              if (this.gdh.debug) console.log("SetObjectInfoString received", id, sts);
              break;
            }
            case Msg.TOGGLE_OBJECT_INFO: {
              if (this.gdh.debug) console.log("ToggleObjectInfo received", id, sts);
              break;
            }
            case Msg.REF_OBJECT_INFO: {
              if (this.gdh.debug) console.log("RefObjectInfo received", id, sts);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.UNREF_OBJECT_INFO: {
              if (this.gdh.debug) console.log("UnrefObjectInfo received", id, sts);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.REF_OBJECT_INFO_LIST: {
              if (this.gdh.debug) console.log("RefObjectInfoList received", id, sts);
              var func_cb = this.gdh.pending[id].func_cb;
              func_cb(id, sts);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OBJECT_REF_INFO_ALL: {
              var j = 9;
              var size = dv.getUint32(j);
              if (this.gdh.debug) console.log("GetObjectRefInfoAll received", id, size);
              j += 4;
              for (var i = 0; i < size; i++) {
                var eid = dv.getUint32(j);
                j += 4;
                var esize = dv.getUint32(j);
                j += 4;
                var sub = this.gdh.sub[eid];
                if (typeof sub == 'undefined')
                  j += esize;
                else {
                  var value;
                  switch (sub.type) {
                    case Pwr.eType_Boolean:
                      if (sub.elements <= 1) {
                        value = dv.getUint8(j);
                        j += 1;
                      }
                      else {
                        var elements = esize;
                        if (elements != sub.elements)
                          console.log("Subscription size error", elements, sub.elements, eid);
                        value = new Array(elements);
                        for (var k = 0; k < elements; k++) {
                          value[k] = dv.getUint8(j);
                          j += 1;
                        }
                      }
                      break;
                    case Pwr.eType_Float32:
                      if (sub.elements <= 1) {
                        value = dv.getFloat32(j);
                        j += 4;
                      }
                      else {
                        var elements = esize / 4;
                        if (elements != sub.elements)
                          console.log("Subscription size error", elements, sub.elements, eid);
                        value = new Array(elements);
                        for (var k = 0; k < elements; k++) {
                          value[k] = dv.getFloat32(j);
                          j += 4;
                        }
                      }
                      break;
                    case Pwr.eType_Int8:
                    case Pwr.eType_Int16:
                    case Pwr.eType_Int32:
                    case Pwr.eType_UInt8:
                    case Pwr.eType_UInt16:
                    case Pwr.eType_UInt32:
                    case Pwr.eType_Status:
                    case Pwr.eType_NetStatus:
                    case Pwr.eType_Mask:
                    case Pwr.eType_Enum:
                    case GraphIfc.eType_Bit:
                      if (sub.elements <= 1) {
                        value = dv.getInt32(j);
                        j += 4;
                      }
                      else {
                        var elements = esize / 4;
                        if (elements != sub.elements)
                          console.log("Subscription size error", elements, sub.elements, eid);
                        value = new Array(elements);
                        for (var k = 0; k < elements; k++) {
                          value[k] = dv.getInt32(j);
                          j += 4;
                        }
                      }
                      break;
                    case Pwr.eType_String:
                    case Pwr.eType_Time:
                    case Pwr.eType_DeltaTime:
                    case Pwr.eType_AttrRef:
                    case Pwr.eType_Objid:
                      if (sub.elements <= 1) {
                        var nsize = dv.getInt16(j);
                        j += 2;
                        var iarr = new Uint8Array(nsize);
                        for (var k = 0; k < nsize; k++) {
                          iarr[k] = dv.getUint8(j++);
                        }
                        value = String.fromCharCode.apply(null, iarr);
                      }
                      else {
                        var elements = sub.elements;
                        if (elements != sub.elements)
                          console.log("Subscription size error", elements, sub.elements, eid);
                        value = new Array(elements);
                        for (var l = 0; l < elements; l++) {
                          var nsize = dv.getInt16(j);
                          j += 2;
                          var iarr = new Uint8Array(nsize);
                          for (var k = 0; k < nsize; k++) {
                            iarr[k] = dv.getUint8(j++);
                          }
                          value[l] = String.fromCharCode.apply(null, iarr);
                        }
                      }
                      break;
                    default:
                      break;
                  }
                  this.gdh.sub[eid].value = value;
                }
              }
              if (typeof this.gdh.pending[id] == 'undefined') {
                console.log("** GetObjectRefInfoAll received removed", id);
                break;
              }
              var func_cb = this.gdh.pending[id].func_cb;
              func_cb(id, sts);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_ALL_XTT_CHILDREN: {
              var result = [];
              var j = 9;
              var size = dv.getUint32(j);
              if (this.gdh.debug) console.log("GetAllXttChildren received", id, size);
              console.log("GetAllXttChildren received", sts, id, size);
              j += 4;
              for (var i = 0; i < size; i++) {
                var info = new ObjectInfo();
                info.objid = new PwrtObjid();
                info.objid.vid = dv.getUint32(j);
                j += 4;
                info.objid.oix = dv.getUint32(j);
                j += 4;
                info.cid = dv.getUint32(j);
                j += 4;
                info.has_children = dv.getUint16(j) !== 0;
                j += 2;
                var nsize = dv.getUint16(j);
                j += 2;
                var iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.name = String.fromCharCode.apply(null, iarr);
                //j += nsize;
                var dsize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(dsize);
                for (var k = 0; k < dsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.description = String.fromCharCode.apply(null, iarr);
                var csize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(csize);
                for (var k = 0; k < csize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.classname = String.fromCharCode.apply(null, iarr);
                result.push(info);
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, result);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_ALL_CLASS_ATTRIBUTES: {
              var result = [];
              var j = 9;
              var size = dv.getUint32(j);
              if (this.gdh.debug) console.log("GetAllClassAttributes received", id, size);
              j += 4;
              for (var i = 0; i < size; i++) {
                var info = new AttributeInfo();
                info.type = dv.getUint32(j);
                j += 4;
                info.flags = dv.getUint32(j);
                j += 4;
                info.size = dv.getUint16(j);
                j += 2;
                info.elements = dv.getUint16(j);
                j += 2;
                var nsize = dv.getUint16(j);
                j += 2;
                var iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.name = String.fromCharCode.apply(null, iarr);

                nsize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.classname = String.fromCharCode.apply(null, iarr);
                result.push(info);
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, result);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OBJECT:
            case Msg.GET_OBJECT_FROM_AREF:
            case Msg.GET_OBJECT_FROM_NAME: {
              if (this.gdh.debug) console.log("GetObject received", id, sts);
              var info = null;
              if ((sts & 1) !== 0) {
                var j = 9;
                info = new ObjectInfo();
                info.objid = new PwrtObjid();
                info.objid.vid = dv.getUint32(j);
                j += 4;
                info.objid.oix = dv.getUint32(j);
                j += 4;
                info.cid = dv.getUint32(j);
                j += 4;
                info.has_children = dv.getUint16(j) !== 0;
                j += 2;

                var nsize = dv.getUint16(j);
                j += 2;
                var iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.name = String.fromCharCode.apply(null, iarr);

                nsize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.fullname = String.fromCharCode.apply(null, iarr);

                var csize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(csize);
                for (var k = 0; k < csize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.classname = String.fromCharCode.apply(null, iarr);

                var dsize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(dsize);
                for (var k = 0; k < dsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.description = String.fromCharCode.apply(null, iarr);

                var p1size = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(p1size);
                for (var k = 0; k < p1size; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.param1 = String.fromCharCode.apply(null, iarr);
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, info);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.CRR_SIGNAL: {
              var crrtext = null;
              if ((sts & 1) !== 0) {
                var j = 9;
                var result = [];
                var size = dv.getUint16(j);
                j += 2;
                if (this.gdh.debug) console.log("CrrSignal received", id, size);
                for (var i = 0; i < size; i++) {
                  var info = new CrrInfo();
                  info.type = dv.getUint16(j);
                  j += 2;
                  info.objid = new PwrtObjid();
                  info.objid.vid = dv.getUint32(j);
                  j += 4;
                  info.objid.oix = dv.getUint32(j);
                  j += 4;
                  var nsize = dv.getUint16(j);
                  j += 2;
                  var iarr = new Uint8Array(nsize);
                  for (var k = 0; k < nsize; k++) {
                    iarr[k] = dv.getUint8(j++);
                  }
                  info.name = String.fromCharCode.apply(null, iarr);
                  var csize = dv.getUint16(j);
                  j += 2;
                  iarr = new Uint8Array(csize);
                  for (var k = 0; k < csize; k++) {
                    iarr[k] = dv.getUint8(j++);
                  }
                  info.classname = String.fromCharCode.apply(null, iarr);
                  result.push(info);
                }
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, result);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OPWIND_MENU: {
              var result = new OpwindMenuInfo();
              var j = 9;
              if (this.gdh.debug) console.log("GetOpwindMenu received", id, size);
              console.log("GetOpwindMenu received", sts, id);

              if (sts & 1) {

                var nsize = dv.getUint16(j);
                j += 2;
                var iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                result.title = String.fromCharCode.apply(null, iarr);

                nsize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                result.text = String.fromCharCode.apply(null, iarr);

                result.enable_language = dv.getUint32(j);
                j += 4;
                result.enable_login = dv.getUint32(j);
                j += 4;
                result.enable_alarmlist = dv.getUint32(j);
                j += 4;
                result.enable_eventlog = dv.getUint32(j);
                j += 4;
                result.enable_navigator = dv.getUint32(j);
                j += 4;
                result.disable_help = dv.getUint32(j);
                j += 4;
                result.disable_proview = dv.getUint32(j);
                j += 4;
                result.language = dv.getUint32(j);
                j += 4;

                var bsize = dv.getUint16(j);
                j += 2;

                for (var i = 0; i < bsize; i++) {
                  var button = new MenuButton();
                  button.type = dv.getUint32(j);
                  j += 4;
                  nsize = dv.getUint16(j);
                  j += 2;
                  iarr = new Uint8Array(nsize);
                  for (var k = 0; k < nsize; k++) {
                    iarr[k] = dv.getUint8(j++);
                  }
                  button.text = String.fromCharCode.apply(null, iarr);
                  nsize = dv.getUint16(j);
                  j += 2;
                  iarr = new Uint8Array(nsize);
                  for (var k = 0; k < nsize; k++) {
                    iarr[k] = dv.getUint8(j++);
                  }
                  button.name = String.fromCharCode.apply(null, iarr);
                  nsize = dv.getUint16(j);
                  j += 2;
                  iarr = new Uint8Array(nsize);
                  for (var k = 0; k < nsize; k++) {
                    iarr[k] = dv.getUint8(j++);
                  }
                  button.url = String.fromCharCode.apply(null, iarr);
                  result.buttons.push(button);
                }
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, result);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.CHECK_USER: {
              var j = 9;
              if (this.gdh.debug) console.log("Check user received", id, size);
              console.log("Check user received", sts, id);

              var priv = 0;
              if (sts & 1) {
                priv = dv.getUint32(j);
                j += 4;
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, priv);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_MSG: {
              if (sts & 1) {
                var j = 9;
                var nsize = dv.getUint16(j);
                j += 2;
                var iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                var msg = String.fromCharCode.apply(null, iarr);
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, msg);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.MH_SYNC: {
              var result = [];
              var j = 9;
              var size = dv.getUint32(j);
              if (this.gdh.debug) console.log("MhSync received", id, size);
              j += 4;
              for (var i = 0; i < size; i++) {
                var e = new MhEvent();

                var len = dv.getUint16(j);
                j += 2;
                var iarr = new Uint8Array(len);
                for (var k = 0; k < len; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                e.eventTime = String.fromCharCode.apply(null, iarr);

                len = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(len);
                for (var k = 0; k < len; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                e.eventText = String.fromCharCode.apply(null, iarr);

                len = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(len);
                for (var k = 0; k < len; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                e.eventName = String.fromCharCode.apply(null, iarr);

                e.eventFlags = dv.getUint32(j);
                j += 4;
                e.eventStatus = dv.getUint32(j);
                j += 4;
                e.eventPrio = dv.getUint32(j);
                j += 4;

                e.eventId = new MhEventId();
                e.eventId.nix = dv.getUint32(j);
                j += 4;
                e.eventId.idx = dv.getUint32(j);
                j += 4;
                len = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(len);
                for (var k = 0; k < len; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                e.eventId.birthTime = String.fromCharCode.apply(null, iarr);

                e.targetId = new MhEventId();
                e.targetId.nix = dv.getUint32(j);
                j += 4;
                e.targetId.idx = dv.getUint32(j);
                j += 4;
                len = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(len);
                for (var k = 0; k < len; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                e.targetId.birthTime = String.fromCharCode.apply(null, iarr);

                e.eventType = dv.getUint32(j);
                j += 4;
                var objid = new PwrtObjid(0, 0);
                objid.vid = dv.getUint32(j);
                j += 4;
                objid.oix = dv.getUint32(j);
                j += 4;
                e.object = new PwrtAttrRef();
                e.object.objid = objid;
                e.object.offset = dv.getUint32(j);
                j += 4;
                e.object.body = dv.getUint32(j);
                j += 4;
                e.object.size = dv.getUint32(j);
                j += 4;
                e.object.flags = dv.getUint32(j);
                j += 4;
                var supObjid = new PwrtObjid(0, 0);
                supObjid.vid = dv.getUint32(j);
                j += 4;
                supObjid.oix = dv.getUint32(j);
                j += 4;
                e.supObject = new PwrtAttrRef();
                e.supObject.objid = supObjid;
                e.supObject.offset = dv.getUint32(j);
                j += 4;
                e.supObject.body = dv.getUint32(j);
                j += 4;
                e.supObject.size = dv.getUint32(j);
                j += 4;
                e.supObject.flags = dv.getUint32(j);
                j += 4;

                len = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(len);
                for (var k = 0; k < len; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                e.eventMoreText = String.fromCharCode.apply(null, iarr);

                e.syncIdx = dv.getUint32(j);
                j += 4;
                result.push(e);
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, result);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.MH_ACK: {
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts);
              delete this.gdh.pending[id];
              break;
            }
            default:
              console.log("Unknown message type");
          }
        }
      }
    };
  };

  this.getObjectInfoBoolean = function (name, return_cb) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length + 6);
    buf[0] = Msg.GET_OBJECT_INFO_BOOLEAN;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    for (var i = 0; i < name.length; i++) {
      buf[i + 6] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, null);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectInfoInt = function (name, return_cb, data) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length + 6);
    buf[0] = Msg.GET_OBJECT_INFO_INT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    for (var i = 0; i < name.length; i++) {
      buf[i + 6] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectInfoFloat = function (name, return_cb, data) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length + 6);
    buf[0] = Msg.GET_OBJECT_INFO_FLOAT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    for (var i = 0; i < name.length; i++) {
      buf[i + 6] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectInfoFloatArray = function (name, asize, return_cb, data) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length + 10);
    buf[0] = Msg.GET_OBJECT_INFO_FLOAT_ARRAY;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = asize & 0xFF;
    buf[7] = (asize >> 8) & 0xFF;
    buf[8] = (asize >> 16) & 0xFF;
    buf[9] = (asize >> 24) & 0xFF;
    for (var i = 0; i < name.length; i++) {
      buf[i + 10] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    this.ws.send(buf);
    this.next_id++;
  };

  this.refObjectInfo = function (name, type, elements) {
    var sub = new Sub();
    sub.name = name;
    sub.refid = this.subscriptionCount;
    sub.type = type;
    sub.elements = elements;
    this.sub[this.subscriptionCount] = sub;
    this.subscriptionCount++;
    if (!this.listSent) {
      return sub.refid;
    }
    else {
      var size = 0;
      var len = 0;

      size = 12 + sub.name.length;

      var buf = new Uint8Array(size + 10);
      buf[0] = Msg.REF_OBJECT_INFO;
      buf[2] = this.next_id & 0xFF;
      buf[3] = (this.next_id >> 8) & 0xFF;
      buf[4] = (this.next_id >> 16) & 0xFF;
      buf[5] = (this.next_id >> 24) & 0xFF;
      if (this.debug) console.log("RefObjectInfo: ", sub.refid);
      var k = 6;
      buf[k++] = sub.refid & 0xFF;
      buf[k++] = (sub.refid >> 8) & 0xFF;
      buf[k++] = (sub.refid >> 16) & 0xFF;
      buf[k++] = (sub.refid >> 24) & 0xFF;
      buf[k++] = sub.elements & 0xFF;
      buf[k++] = (sub.elements >> 8) & 0xFF;
      buf[k++] = (sub.elements >> 16) & 0xFF;
      buf[k++] = (sub.elements >> 24) & 0xFF;
      buf[k++] = sub.name.length & 0xFF;
      buf[k++] = (sub.name.length >> 8) & 0xFF;
      buf[k++] = (sub.name.length >> 16) & 0xFF;
      buf[k++] = (sub.name.length >> 24) & 0xFF;

      for (var j = 0; j < sub.name.length; j++) {
        buf[k++] = sub.name.charCodeAt(j);
      }

      this.pending[this.next_id] = new PendingData(this.refObjectInfoReply, null);
      if (this.debug) console.log("Sending RefObjectInfo", this.next_id, size, k);
      this.ws.send(buf);

      this.next_id++;

      return sub.refid;
    }
  };
  this.refObjectInfoReply = function (id, sts) {
    if (this.debug) console.log("refObjectInfoReply", id, sts);
  };
  this.unrefObjectInfo = function (refid) {
    var size = 0;
    var len = 0;

    size = 4;

    var buf = new Uint8Array(size + 10);
    buf[0] = Msg.UNREF_OBJECT_INFO;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    if (this.debug) console.log("UnrefObjectInfo: ", refid);
    var k = 6;
    buf[k++] = refid & 0xFF;
    buf[k++] = (refid >> 8) & 0xFF;
    buf[k++] = (refid >> 16) & 0xFF;
    buf[k++] = (refid >> 24) & 0xFF;

    this.pending[this.next_id] = new PendingData(this.unrefObjectInfoReply, null);
    if (this.debug) console.log("Sending UnrefObjectInfo", this.next_id, size, k, refid);
    this.ws.send(buf);

    this.next_id++;
    delete this.sub[refid];
  };
  this.refObjectInfoList = function (return_cb) {
    var size = 0;
    var len = 0;

    this.return_cb = return_cb;

    for (var i in this.sub) {
      size += 12 + this.sub[i].name.length;
      len++;
    }
    var buf = new Uint8Array(size + 10);
    buf[0] = Msg.REF_OBJECT_INFO_LIST;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = len & 0xFF;
    buf[7] = (len >> 8) & 0xFF;
    buf[8] = (len >> 16) & 0xFF;
    buf[9] = (len >> 24) & 0xFF;
    var k = 10;
    for (var i in this.sub) {
      if (i === 0)
        continue;
      if (this.debug) console.log("RefObjectInfoList: ", this.sub[i].refid);
      buf[k++] = this.sub[i].refid & 0xFF;
      buf[k++] = (this.sub[i].refid >> 8) & 0xFF;
      buf[k++] = (this.sub[i].refid >> 16) & 0xFF;
      buf[k++] = (this.sub[i].refid >> 24) & 0xFF;
      buf[k++] = this.sub[i].elements & 0xFF;
      buf[k++] = (this.sub[i].elements >> 8) & 0xFF;
      buf[k++] = (this.sub[i].elements >> 16) & 0xFF;
      buf[k++] = (this.sub[i].elements >> 24) & 0xFF;
      buf[k++] = this.sub[i].name.length & 0xFF;
      buf[k++] = (this.sub[i].name.length >> 8) & 0xFF;
      buf[k++] = (this.sub[i].name.length >> 16) & 0xFF;
      buf[k++] = (this.sub[i].name.length >> 24) & 0xFF;

      for (var j = 0; j < this.sub[i].name.length; j++) {
        buf[k++] = this.sub[i].name.charCodeAt(j);
      }
    }
    this.pending[this.next_id] = new PendingData(return_cb, null);
    if (this.debug) console.log("Sending RefObjectInfoList", this.next_id, size, k, this.next_id);
    this.ws.send(buf);

    this.next_id++;
    this.listSent = true;
  };
  this.refObjectInfoListReply = function (id, sts) {
    if (this.debug) console.log("refObjectInfoListReply", id, sts);
  };
  this.getRefObjectInfoAll = function (return_cb) {
    var buf = new Uint8Array(6);
    buf[0] = Msg.GET_OBJECT_REF_INFO_ALL;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, null);
    if (this.debug) console.log("Sending getRefObjectInfoAll", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };
  this.getRefObjectInfoAllReply = function (id, sts) {
    if (this.debug) console.log("getRefObjectInfoAllReply", id, sts);
  };
  this.getObjectRefInfo = function (id) {
    if (this.debug) console.log("getObjectRefInfo", id, this.sub[id].value);
    return this.sub[id].value;
  };
  this.setObjectInfoBoolean = function (name, value) {
    var buf = new Uint8Array(12 + name.length);
    buf[0] = Msg.SET_OBJECT_INFO_BOOLEAN;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = value & 0xFF;
    buf[7] = (value >> 8) & 0xFF;
    buf[8] = (value >> 16) & 0xFF;
    buf[9] = (value >> 24) & 0xFF;
    buf[10] = name.length & 0xFF;
    buf[11] = (name.length >> 8) & 0xFF;
    var k = 12;
    for (var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    this.ws.send(buf);
    if (this.debug) console.log("Sending setObjectInfoBoolean", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus(1);
  };

  this.setObjectInfoInt = function (name, value) {
    var buf = new Uint8Array(12 + name.length);
    buf[0] = Msg.SET_OBJECT_INFO_INT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = value & 0xFF;
    buf[7] = (value >> 8) & 0xFF;
    buf[8] = (value >> 16) & 0xFF;
    buf[9] = (value >> 24) & 0xFF;
    buf[10] = name.length & 0xFF;
    buf[11] = (name.length >> 8) & 0xFF;
    var k = 12;
    for (var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if (this.debug) console.log("Sending setObjectInfoInt", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus(1);
  };

  this.setObjectInfoFloat = function (name, value) {
    var buf = new Uint8Array(12 + name.length);
    buf[0] = Msg.SET_OBJECT_INFO_FLOAT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    var fbuf = new ArrayBuffer(4);
    var fa = new Float32Array(fbuf);
    fa[0] = value;
    var ba = new Uint8Array(fbuf);
    buf[6] = ba[0];
    buf[7] = ba[1];
    buf[8] = ba[2];
    buf[9] = ba[3];
    buf[10] = name.length & 0xFF;
    buf[11] = (name.length >> 8) & 0xFF;
    var k = 12;
    for (var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if (this.debug) console.log("Sending setObjectInfoFloat", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus(1);
  };

  this.setObjectInfoString = function (name, value) {
    var i;
    var buf = new Uint8Array(10 + value.length + name.length);
    buf[0] = Msg.SET_OBJECT_INFO_STRING;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = value.length & 0xFF;
    buf[7] = (value.length >> 8) & 0xFF;
    var k = 8;
    for (i = 0; i < value.length; i++)
      buf[k++] = value.charCodeAt(i);
    buf[k++] = name.length & 0xFF;
    buf[k++] = (name.length >> 8) & 0xFF;
    for (i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if (this.debug) console.log("Sending setObjectInfoString", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus(1);
  };

  this.toggleObjectInfo = function (name) {
    var buf = new Uint8Array(8 + name.length);
    buf[0] = Msg.TOGGLE_OBJECT_INFO;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = name.length & 0xFF;
    buf[7] = (name.length >> 8) & 0xFF;
    var k = 8;
    for (var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if (this.debug) console.log("Sending toggleObjectInfoBoolean", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus(1);
  };

  this.getAllXttChildren = function (oid, return_cb, data) {
    var buf = new Uint8Array(14);
    buf[0] = Msg.GET_ALL_XTT_CHILDREN;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = oid.vid & 0xFF;
    buf[7] = (oid.vid >> 8) & 0xFF;
    buf[8] = (oid.vid >> 16) & 0xFF;
    buf[9] = (oid.vid >> 24) & 0xFF;
    buf[10] = oid.oix & 0xFF;
    buf[11] = (oid.oix >> 8) & 0xFF;
    buf[12] = (oid.oix >> 16) & 0xFF;
    buf[13] = (oid.oix >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending getAllXttChildren", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getAllClassAttributes = function (cid, oid, return_cb, data) {
    var buf = new Uint8Array(18);
    buf[0] = Msg.GET_ALL_CLASS_ATTRIBUTES;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = cid & 0xFF;
    buf[7] = (cid >> 8) & 0xFF;
    buf[8] = (cid >> 16) & 0xFF;
    buf[9] = (cid >> 24) & 0xFF;
    buf[10] = oid.vid & 0xFF;
    buf[11] = (oid.vid >> 8) & 0xFF;
    buf[12] = (oid.vid >> 16) & 0xFF;
    buf[13] = (oid.vid >> 24) & 0xFF;
    buf[14] = oid.oix & 0xFF;
    buf[15] = (oid.oix >> 8) & 0xFF;
    buf[16] = (oid.oix >> 16) & 0xFF;
    buf[17] = (oid.oix >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending getAllClassAttributes", this.next_id, cid, oid.vid, oid.oix);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObject = function (oid, op, return_cb, data) {
    var buf = new Uint8Array(16);
    buf[0] = Msg.GET_OBJECT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = op & 0xFF;
    buf[7] = (op >> 8) & 0xFF;
    buf[8] = oid.vid & 0xFF;
    buf[9] = (oid.vid >> 8) & 0xFF;
    buf[10] = (oid.vid >> 16) & 0xFF;
    buf[11] = (oid.vid >> 24) & 0xFF;
    buf[12] = oid.oix & 0xFF;
    buf[13] = (oid.oix >> 8) & 0xFF;
    buf[14] = (oid.oix >> 16) & 0xFF;
    buf[15] = (oid.oix >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending getObject", this.next_id, oid.vid, oid.oix);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectFromAref = function (aref, op, return_cb, data) {
    var buf = new Uint8Array(32);
    buf[0] = Msg.GET_OBJECT_FROM_AREF;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = op & 0xFF;
    buf[7] = (op >> 8) & 0xFF;
    buf[8] = aref.objid.vid & 0xFF;
    buf[9] = (aref.objid.vid >> 8) & 0xFF;
    buf[10] = (aref.objid.vid >> 16) & 0xFF;
    buf[11] = (aref.objid.vid >> 24) & 0xFF;
    buf[12] = aref.objid.oix & 0xFF;
    buf[13] = (aref.objid.oix >> 8) & 0xFF;
    buf[14] = (aref.objid.oix >> 16) & 0xFF;
    buf[15] = (aref.objid.oix >> 24) & 0xFF;
    buf[16] = aref.offset & 0xFF;
    buf[17] = (aref.offset >> 8) & 0xFF;
    buf[18] = (aref.offset >> 16) & 0xFF;
    buf[19] = (aref.offset >> 24) & 0xFF;
    buf[20] = aref.body & 0xFF;
    buf[21] = (aref.body >> 8) & 0xFF;
    buf[22] = (aref.body >> 16) & 0xFF;
    buf[23] = (aref.body >> 24) & 0xFF;
    buf[24] = aref.size & 0xFF;
    buf[25] = (aref.size >> 8) & 0xFF;
    buf[26] = (aref.size >> 16) & 0xFF;
    buf[27] = (aref.size >> 24) & 0xFF;
    buf[28] = aref.flags & 0xFF;
    buf[29] = (aref.flags >> 8) & 0xFF;
    buf[30] = (aref.flags >> 16) & 0xFF;
    buf[31] = (aref.flags >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending getObject", this.next_id, oid.vid, oid.oix);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectFromName = function (name, op, return_cb, data) {
    var buf = new Uint8Array(10 + name.length);
    buf[0] = Msg.GET_OBJECT_FROM_NAME;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = op & 0xFF;
    buf[7] = (op >> 8) & 0xFF;
    buf[8] = name.length & 0xFF;
    buf[9] = (name.length >> 8) & 0xFF;
    var k = 10;
    for (var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending getObjectFromName", this.next_id, name);
    this.ws.send(buf);
    this.next_id++;
  };

  this.crrSignal = function (oid, return_cb, data) {
    var buf = new Uint8Array(14);
    buf[0] = Msg.CRR_SIGNAL;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = oid.vid & 0xFF;
    buf[7] = (oid.vid >> 8) & 0xFF;
    buf[8] = (oid.vid >> 16) & 0xFF;
    buf[9] = (oid.vid >> 24) & 0xFF;
    buf[10] = oid.oix & 0xFF;
    buf[11] = (oid.oix >> 8) & 0xFF;
    buf[12] = (oid.oix >> 16) & 0xFF;
    buf[13] = (oid.oix >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending crrObject", this.next_id, oid.vid, oid.oix);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getOpwindMenu = function (name, return_cb, data) {
    var len = name.length;

    var buf = new Uint8Array(8 + name.length);
    buf[0] = Msg.GET_OPWIND_MENU;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = name.length & 0xFF;
    buf[7] = (name.length >> 8) & 0xFF;
    var k = 8;
    for (var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending getOpwindMenu", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };

  this.login = function (user, passwd, return_cb, data) {

    var buf = new Uint8Array(6 + 2 + user.length + 2 + passwd.length);
    buf[0] = Msg.CHECK_USER;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    var k = 6;
    buf[k] = user.length & 0xFF;
    buf[k + 1] = (user.length >> 8) & 0xFF;
    k += 2;
    for (var i = 0; i < user.length; i++) {
      buf[k++] = user.charCodeAt(i);
    }
    buf[k] = passwd.length & 0xFF;
    buf[k + 1] = (passwd.length >> 8) & 0xFF;
    k += 2;
    for (var i = 0; i < passwd.length; i++) {
      buf[k++] = passwd.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending login", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getMsg = function (value, return_cb, data) {
    var buf = new Uint8Array(10);
    buf[0] = Msg.GET_MSG;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = value & 0xFF;
    buf[7] = (value >> 8) & 0xFF;
    buf[8] = (value >> 16) & 0xFF;
    buf[9] = (value >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, data);
    this.ws.send(buf);
    if (this.debug) console.log("Sending getMsg", this.next_id, value);
    this.next_id++;
  };

  this.mhSync = function (sync, return_cb, data) {
    var buf = new Uint8Array(10);
    buf[0] = Msg.MH_SYNC;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = sync & 0xFF;
    buf[7] = (sync >> 8) & 0xFF;
    buf[8] = (sync >> 16) & 0xFF;
    buf[9] = (sync >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending mhSync", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };

  this.mhAcknowledge = function (event_id, return_cb, data) {
    var buf = new Uint8Array(16 + event_id.birthTime.length);
    buf[0] = Msg.MH_ACK;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = event_id.nix & 0xFF;
    buf[7] = (event_id.nix >> 8) & 0xFF;
    buf[8] = (event_id.nix >> 16) & 0xFF;
    buf[9] = (event_id.nix >> 24) & 0xFF;
    buf[10] = event_id.idx & 0xFF;
    buf[11] = (event_id.idx >> 8) & 0xFF;
    buf[12] = (event_id.idx >> 16) & 0xFF;
    buf[13] = (event_id.idx >> 24) & 0xFF;
    var k = 14;
    buf[k] = event_id.birthTime.length & 0xFF;
    buf[k + 1] = (event_id.birthTime.length >> 8) & 0xFF;
    k += 2;
    for (var i = 0; i < event_id.birthTime.length; i++) {
      buf[k++] = event_id.birthTime.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending mhAcknowledge", this.next_id);
    console.log("Sending mhAcknowledge", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };
}

/** End Gdh **/


/** Start Plow **/
var Bitmaps = {
  leaf: 0,
  map: 2,
  openmap: 4,
  object: 6,
  attrenum: 8,
  attrarra: 10,
  attrarel: 12,
  attr: 14,
  crrwrite: 16,
  crrread: 18,
  ack: 20,
  alarm: 22,
  eventacked: 24,
  eventalarm: 26,
  eventreturn: 28,
  info: 30,
  system: 32,
  maintenance: 34,
  blockl: 36,
  blockr: 38,
  img: [null, null, null, null, null, null, null, null, null, null,
    null, null, null, null, null, null, null, null, null, null,
    null, null, null, null, null, null, null, null, null, null,
    null, null, null, null, null, null, null, null],
  pending: [null, null, null, null, null, null, null, null, null, null,
    null, null, null, null, null, null, null, null, null, null,
    null, null, null, null, null, null, null, null, null, null,
    null, null, null, null, null, null, null, null],
  images: [
    // leaf
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAASElEQVQokWP4jwMwMDBgYBR5XBpwGYZVIzYNGDZB+QyEFOBiM+CyCacGBI0hgEGjsxkYGCiwkSI/4tKMz0DqxCM2A4hOOcQCAObFEQyI2PpKAAAAAElFTkSuQmCC',
    // leaf inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAQUlEQVQokWNgYGD4jw1jA2hqsGvAZRhWjdg0oIsh8QkqwMXGbhMuDXAxdAFsNDobyifTRor8SFGoUhSPFKUcYjEAMsMz2y6w8kgAAAAASUVORK5CYII=',
    // map
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAKElEQVQokWP4DwUMDAwYGB9gwKUQm0FoGL/JOGwb1TgINZKFSbYOCgD1JxQJG0vK9AAAAABJRU5ErkJggg==',
    // map inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAK0lEQVQokWNgYGD4z8DA8B8bgMlhxbgUEgIMBE3Ggkc1Dk6N5AAGUm2DYQAkYTDe0vu7CAAAAABJRU5ErkJggg==',
    // openmap
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAQklEQVQokaXOwQ4AIAgCUP7/p+nUlpMSzY2bDwWHA5IEkFJCtaiKxE7dvsue8HZNJEPneoAuSq+OYAf9wy4K0Mk5C+d++RWimsw3AAAAAElFTkSuQmCC',
    // openmap inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAPUlEQVQokaXRQQoAQAgCQP//6brKZmSt0M2hIACI4yBURqiKXQp0ThuhGwmt7Vy00XvqCa7QN1wjhtYLCCYyCkvDVnkJOQAAAABJRU5ErkJggg==',
    // object
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAH0lEQVQokWP4TyJgYGBg+E8iZiDFdHrZMKqBGA2kYAD8gaJsjwzf9wAAAABJRU5ErkJggg==',
    //  object inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAIklEQVQokWNgYGD4TxL+TyJg+P//P9GmwzXQ3oZRDdSOBwAGOSrkrXppgQAAAABJRU5ErkJggg==',
    // attrenum
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAPklEQVQokWP4////fwYGBgxMCDBgU4jNICyYsOlYbENoJNIW7BqJsYm2NuJyBVE2EqWRfjbiUoQ3oAgpwgUANLqccvbgec0AAAAASUVORK5CYII=',
    // attrenum inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAPElEQVQokWNgYGD4jw0wMDDgx9gUEgMYiDIdDaNoJBZg1UiUTTS1EZcriLKRKI30sxGXIgKBhF8RrqgBAOTOqGZ5aiCnAAAAAElFTkSuQmCC',
    // attrarra
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAMElEQVQokWP4////fwYGBgxMCDBgU4jNICwYv+nY5InWOGojXW0khLEahtc6PDYCAB9hxkjBPICvAAAAAElFTkSuQmCC',
    // attrarray inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAALUlEQVQokWNgYGD4jw0wMDDgx9gUEgMYCJmOTZ5ojaM20tVGQgCHK/AnLVwAAPonfpBwU5f4AAAAAElFTkSuQmCC',
    // attrarel
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAIklEQVQokWP4////fwYGBpwYHTDANGADeDWM2jAEbSAFAADB26JsIjYj1AAAAABJRU5ErkJggg==',
    // attrarel inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAIElEQVQokWNgYGD4jw8wMDCgYwZcEvg1jNowBG0gBQMAQN8q5COZl1cAAAAASUVORK5CYII=',
    // attr
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAIUlEQVQokWP4TyJgGIQaGBgY/pOIibcEroEkTLIfhoEGADzs8B5gcUg/AAAAAElFTkSuQmCC',
    // attr inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAJUlEQVQokWNgYGD4TyIedBr+kwgY/v//T7TpcA0k2TAIQ4nmGgDFzt0jExR7hgAAAABJRU5ErkJggg==',
    // crrwrite
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAN0lEQVQokaWNQQoAMAjD8v9P633WVWhBkJIotQQoYPYbrHYrqC9D+MG54OBMuMC54GApuPBed9OxiMNLGke1JwAAAABJRU5ErkJggg==',
    // crrwrite inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAANElEQVQokWNgYGD4jw3DABY57IqxsQlqwGELSYop1ECEYgo0EKmYQg1EKMbUQEAxRAMpAABRMgoFjbTzXgAAAABJRU5ErkJggg==',
    // crrread
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAPElEQVQokWNgYGD4j4yRATY5BmyKkGks4qgS2BSj2UYFDdgAdW2gvpOwhDW6ItwaCGI8JuHWgMOtWD0PACufaaWhXDFDAAAAAElFTkSuQmCC',
    // crrread inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAPElEQVQokWP4jwYYGBjgGKscNkXINLo4hgZsipFto44GbJi6NlDfSehhjUxjyGMVxQMYcJmEVwPB5ICEAdcbY6vf9TVAAAAAAElFTkSuQmCC',
    // ack
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAS0lEQVQokZ2Q2w4AIAhCz///ND11M3RVmxuDUBRAtw8QHRyC4SSJSDjDht1Yhxdudks+bFNxYsX9G6rz2qVHxqRspGi2Wpoji/dqaLh22DbO2VuXAAAAAElFTkSuQmCC',
    // ack inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAASklEQVQokWP4////fwYGBqIwVC2cgSKBTQzKxhDA0ICmGau12BRCMLJp2BQgy8H9gM9ELGJkasAXvFg9TQjgDCVc8YBXAwFMmgYASkT1C9E5Ya0AAAAASUVORK5CYII=',
    // alarm
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAARklEQVQokZ2QQQoAMAjD+v9Pu9PA1ThQQRBtoFUBJSkk0SlwOwKykCAEcn+BK8hih/aAe++y7IDuhWgXfcKHCuBBfX6ASR3Vn8ZINQzCrQAAAABJRU5ErkJggg==',
    // alarm inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAQ0lEQVQokWNgYGD4j45hAJscA8UakBXi0ITddDy2YJqOrBiLJjI1oLsdj1/I0IAnCLFpwtSALYQwNKB7FJ2NooEUDAAtGwcI+Svs4gAAAABJRU5ErkJggg==',
    // eventacked
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAL0lEQVQokWP4TyJgGCkaGBgQyhhwSeASw9CAYho2A3CZiE0xVg34FOPUgA+QrAEA1FYi+tWeG/cAAAAASUVORK5CYII=',
    // eventacked inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAMElEQVQokWNgYGD4TyIeGRr+//+PXQOaBC4xVElkBdgMwHASTBEOxdj9gEcxHYIVAC5kqlZXl5JMAAAAAElFTkSuQmCC',
    // eventalarm
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAJ0lEQVQokWP4jwMwMDBgF8elGIZHNeDSANOEVRyXBpwGUcUGqvoBAK+H8xt0qXFWAAAAAElFTkSuQmCC',
    // eventalarm inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAJ0lEQVQokWNgYGD4jw3///8fqzgDLsUwMKoBlwaSg5UAHoiIw2cDAFMz2iY65DAoAAAAAElFTkSuQmC',
    // eventreturn
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAASUlEQVQokZ2RUQoAIAjFdv9L11dhskeUEIQ2mcYIAXjeisA+EahFA9ad/siAA0wqSUsnu87wDZh3VEodO6Rr1c51rc8fd9OoMQHLk7dXak3qLwAAAABJRU5ErkJggg==',
    // eventreturn inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAR0lEQVQokZ2S2woAIAhD9/8/vV6tjgMNhLxMN1SSTGYb46JkfS2gJglQ/ncRAZ5JTCXQYsFRwxpAvFtKoeMvmoqDv1jc5DQONycV+bfOetgAAAAASUVORK5CYII=',
    // info
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAOUlEQVQokWNgYGD4///////E0AwMDP8hPDQAk8QhjlMCuzg2G3AB6tlAez8MglAiCZNiw/////8DACmmtFrq9aGNAAAAAElFTkSuQmCC',
    // info inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAN0lEQVQokWP4////fwYGhv/E0FA2w390jCSJIc6ASwKnBmw24MLUs4H2fhgEoUQKIMkGBgaG/wDZFBj2pVi3HgAAAABJRU5ErkJggg==',
    // system
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAN0lEQVQokWNgYGD4TyJm+E8sgGugig3YxHFqwGcIQQ3obAwN6G7GawMuhVg1EFKMUwM+QJ+IAwDD1Gyi1EZc6gAAAABJRU5ErkJggg==',
    // system inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAO0lEQVQokWP4TyJg+P///38GBgaiMFwDVWzAJo5TA15DCGnAYKNrwHAzPhtwKcSqgZBinBqIigeaRhwAPuZgrlZXy70AAAAASUVORK5CYII=',
    // maintenance
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAP0lEQVQokZWPQQoAMAzC8v9Pu8soDOxaBS82QkWNgPKTJ/BYsLekAOxfquwHWyfws6ED7OgtfNk9bAuTSGBJOufqnHIsmYkzAAAAAElFTkSuQmCC',
    // maintenance inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAQUlEQVQokZWPQQ4AMAQE5/+f1kuTCkuROGAmADCVPsJsBfdCsX0u3N7sJFfXsAqW8BMaID+9gE3SJayEFo7CFwY7GtAw3ouVj50AAAAASUVORK5CYII=',
    // blockl
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAQElEQVQokZ2QQQ4AMAjC+v9Pu9MWl5AJ42a0IlKBgCIadgGgQ8imqo9D3zDBvJoyR+zwlWHS9SUHaOf4QOywtQCQGucn0g2dEQAAAABJRU5ErkJggg==',
    // blockl inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAOklEQVQokWNgYGD4Tyz+////fwZSFBOtAaoQQsM46JLY+HAbUEwgrJmwyZTZQJYfSAolmsQDWTENwwByoOYaWbAqegAAAABJRU5ErkJggg==',
    // blockr
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAPElEQVQokWP4jwMwMDBgF8enAZsmvBqw2cSARQBDIQqbGEUoaggpQBcj3Qay/IAN4AwlfBrIigeiNeACADpU5yeYXn+XAAAAAElFTkSuQmCC',
    // blockr inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAPElEQVQokWNgYGD4jw3///8fqzgDPg04NOG3AUMTugA2hWhqiFKEzCeoAF2MRBvI8gNJoUT1eCBaAy4MAMhm5hr5iTaWAAAAAElFTkSuQmCC'
  ]
};


var Plow = {
  DRAWOFFSET: 2,
  DEST_INTOLAST: 0,
  DEST_INTOFIRST: 1,
  DEST_AFTER: 2,
  DEST_BEFORE: 3,
  TREE_INDENTATION: 1.0,
  OPEN_ATTRIBUTES: 1,
  OPEN_CHILDREN: 2,
  OPEN_CROSSREFERENCES: 4,
  OPEN_ALL: 7,
  COLOR_BLACK: 1,
  COLOR_RED: 2,
  COLOR_GRAY: 3,
  COLOR_DARKGRAY: 4,
  COLOR_LIGHTGRAY: 5,
  COLOR_WHITE: 6,
  COLOR_YELLOW: 7,
  COLOR_GREEN: 8,
  COLOR_LIGHTBLUE: 9,
  COLOR_BLUE: 10,
  COLOR_VIOLET: 11,
  eEvent_MB1Click: 0,
  eEvent_MB1ClickShift: 1,
  eEvent_Key_Up: 2,
  eEvent_Key_Down: 3,
  eEvent_Key_Right: 4,
  eEvent_Key_Left: 5,
  eEvent_Key_ShiftRight: 6,
  eEvent_Key_CtrlR: 7,
  eEvent_Key_CtrlL: 8,
  eEvent_Key_CtrlG: 9,
  eEvent_ObjectDeleted: 10,
  RELATIVE_POSITION: 1,
  NEXT_RELATIVE_POSITION: 2
}

function PlowGeometry() {
  this.ll_x = 0;
  this.ll_y = 0;
  this.ur_x = 0;
  this.ur_y = 0;
}

function PlowNodeClass(ctx) {

  this.a = new PlowArray(ctx);
  this.ctx = ctx;
  this.nc_name = "";
  this.group = 0;
  this.node_open = 0;

  this.draw = function (g, p, node, highlight) {
    this.a.draw(g, p, node, highlight);
  }

  this.insert = function (elem) {
    this.a.add(elem);
  }
}

function PlowArray(ctx) {
  this.a = [];
  this.ctx = ctx;

  this.add = function (elem) {
    this.a.push(elem);
  }
  this.insertNode = function (elem, destination, code) {
    var idx = this.find(elem);
    if (idx != -1)
      return;

    if (destination == null) {
      switch (code) {
        case Plow.DEST_INTOLAST:
        case Plow.DEST_AFTER:
          this.a.push(elem);
          elem.level = 0;
          break;
        default:
          elem.level = 0;
          this.a.unshift(elem);
      }
    }
    else {
      var dest_idx = this.find(destination);
      if (dest_idx == -1)
        return;

      switch (code) {
        case Plow.DEST_INTOFIRST:
          if (dest_idx == this.a.length - 1)
            this.a.push(elem);
          else
            this.a.splice(dest_idx + 1, 0, elem);
          elem.level = destination.level + 1;
          break;
        case Plow.DEST_INTOLAST: {
          if (dest_idx == this.a.length - 1)
            this.a.push(elem);
          else {
            idx = this.a.length;
            for (var i = dest_idx + 1; i < this.a.length; i++) {
              if (this.a[i].level <= destination.level) {
                idx = i;
                break;
              }
            }
            if (idx == this.a.length)
              this.a.push(elem);
            else
              this.a.splice(idx, 0, elem);
          }
          elem.level = destination.level + 1;
          break;
        }
        case Plow.DEST_AFTER: {
          if (dest_idx == this.a.length - 1)
            this.a.push(elem);
          else {
            var i;
            for (i = idx; i < this.a.length; i++) {
              if (this.a[i].level < destination.level)
                break;
            }
            this.a.splice(i, 0, elem);
          }
          elem.level = destination.level;
          break;
        }
        case Plow.DEST_BEFORE:
          if (idx > 0)
            idx--;
          this.a.splice(idx, 0, elem);
          elem.level = destination.level;
          break;
      }
    }
  }

  this.remove = function (elem) {
    var idx = this.find(elem);
    if (idx == -1)
      return;
    this.ctx.event_cb(Plow.eEvent_ObjectDeleted, this.a[idx + 1], 0, 0);
    this.a.splice(idx, 1);
  }

  this.size = function () {
    return this.a.length;
  }
  this.get = function (idx) {
    return this.a[idx];
  }
  this.draw = function (g, p, node, highlight) {
    for (var i = 0; i < this.a.length; i++)
      this.a[i].draw(g, p, node, highlight);
  }
  this.set_borders = function (node) {
    for (var i = 0; i < this.a.length; i++)
      this.a[i].set_borders(node);
  }
  this.configure = function () {
    for (var i = 0; i < this.a.length; i++) {
      this.a[i].pos.x = this.a[i].level * 1.0;
      this.a[i].pos.y = i * 1.0;
    }
  }
  this.close_node = function (node) {
    var idx = this.find(node);
    if (idx == -1)
      return;
    var level = node.level;
    var i;
    for (i = idx + 1; i < this.a.length; i++) {
      if (this.a[i].level <= level)
        break;
    }
    var next_idx = i;
    if (next_idx == idx + 1)
      return;
    for (i = idx + 1; i < next_idx; i++) {
      // Event backcall
      if (ctx.select_object == this.a[idx + 1])
        ctx.select_object = null;
      this.ctx.event_cb(Plow.eEvent_ObjectDeleted, this.a[idx + 1], 0, 0);
      this.a.splice(idx + 1, 1);
    }
  }
  this.get_parent_object = function (node) {
    var idx = this.find(node);
    if (idx == -1)
      return null;

    for (var i = idx; i >= 0; i--) {
      if (this.a[i].level < node.level)
        return this.a[i];
    }
    return null;
  }

  this.get_first_child = function (node) {
    var idx = this.find(node);
    if (idx == -1)
      return null;

    if (this.a.length == idx - 1)
      return null;

    if (this.a[idx + 1].level > node.level)
      return this.a[idx + 1];
    return null;
  }

  this.get_next_sibling = function (node) {
    var found = false;
    for (var i = 0; i < this.a.length; i++) {
      if (this.a[i] == node) {
        found = true;
        continue;
      }
      if (found) {
        if (this.a[i].level == node.level)
          return this.a[i];
        if (this.a[i].level < node.level)
          return null;
      }
    }
    return null;
  }

  this.get_next_object = function (node) {
    for (var i = 0; i < this.a.length; i++) {
      if (this.a[i] == node) {
        if (i == this.a.length - 1)
          return null;
        return this.a[i + 1];
      }
    }
    return null;
  }
  this.get_previous_object = function (node) {
    for (var i = 0; i < this.a.length; i++) {
      if (this.a[i] == node) {
        if (i == 0)
          return null;
        return this.a[i - 1];
      }
    }
    return null;
  }
  this.get_first_object = function () {
    if (this.a.length == 0)
      return null;
    return this.a[0];
  }
  this.get_last_object = function () {
    if (this.a.length == 0)
      return null;
    return this.a[this.a.length - 1];
  }

  this.find = function (elem) {
    for (var i = 0; i < this.a.length; i++) {
      if (this.a[i] == elem)
        return i;
    }
    return -1;
  }
}

function PlowNode(ctx, nc, level) {
  this.ctx = ctx;
  this.userdata = null;
  this.OFFSET = 2;
  this.x_right = 0.0;
  this.x_left = 0.0;
  this.y_high = 0.0;
  this.y_low = 0.0;
  this.nc = nc;
  this.pos = new PlowPoint(ctx, 0, 0);
  this.n_name = "";
  this.annotv = [];
  this.annotsize = [];
  this.pixmapv = [];
  this.trace_object = "";
  this.trace_attribute = "";
  this.trace_attr_type = 0;
  this.highlight = false;
  this.select = false;
  this.invert = false;
  this.level = level;
  this.node_open = false;
  this.fill_color = 0;
  this.p = 0;
  this.old_value = 0;
  this.first_scan = true;
  this.relative_position = 0;

  this.set_annotation = function (number, text) {
    if (number >= 10)
      return;
    this.annotv[number] = text;
  }
  this.set_annotation_pixmap = function (number, pixmap) {
    if (number >= 10)
      return;
    this.pixmapv[number] = pixmap;
  }
  this.draw_object = function () {
    this.draw(this.ctx.gdraw.gctx, null, null, false);
  }
  this.draw = function (g, p, node, highlight) {
    var x = this.x_left * this.ctx.zoom_factor;
    var y = this.y_low * this.ctx.zoom_factor - 1;
    var width = (this.x_right - this.x_left) * this.ctx.zoom_factor;
    var height = (this.y_high - this.y_low) * this.ctx.zoom_factor + 2;
    g.fillStyle = "white";
    if (this.select)
      g.fillStyle = "lightblue";
    g.fillRect(x, y, width, height);

    this.nc.draw(g, this.pos, this, this.highlight);
  }

  this.connect = function () {
    if (this.trace_object == "" || this.trace_attribute == "")
      return;

    var n = this.trace_attribute.indexOf('#');
    if (n != -1)
      this.trace_attribute = this.trace_attribute.substring(0, n);

    var o = this.trace_object + "." + this.trace_attribute;
    this.p = ctx.gdh.refObjectInfo(o, this.trace_attr_type);
    console.log("connecting", o, this.p);
  }
  this.scan = function () {
    if (this.p == 0)
      return;
    var v1 = ctx.gdh.getRefObjectInfo(this.p);
    var evaluate = true;
    if (this.first_scan)
      this.first_scan = false;
    else if (v1 == this.old_value)
      return;

    if (v1)
      this.highlight = true;
    else
      this.highlight = false;

    this.old_value = v1;
    this.draw_object();
  }

  this.set_borders = function () {
    this.x_left = 1e37;
    this.x_right = -1e37;
    this.y_low = 1e37;
    this.y_high = -1e37;
    nc.a.set_borders(this);
  }

  this.event_handler = function (event, x, y) {
    if ((x - this.ctx.offset_x) / this.ctx.zoom_factor >= this.x_left &&
      (x - this.ctx.offset_x) / this.ctx.zoom_factor <= this.x_right &&
      (y - this.ctx.offset_y) / this.ctx.zoom_factor >= this.y_low &&
      (y - this.ctx.offset_y) / this.ctx.zoom_factor <= this.y_high) {
      ctx.event_object = this;
      return 1;
    }
    return 0;
  }

  this.set_select = function (select) {
    if (select) {
      this.select = true;
      this.ctx.select_object = this;
    }
    else {
      this.select = false;
      if (this.ctx.select_object == this)
        this.ctx.select_object = null;
    }
    if (select != this.select)
      this.draw_object();
  }
  this.set_invert = function (invert) {
    this.invert = invert;
    this.draw_object();
  }
  this.set_userdata = function (userdata) {
    this.userdata = userdata;
  }
  this.get_userdata = function () {
    return this.userdata;
  }
  this.in_icon = function (x, y) {
    if (x >= this.x_left * this.ctx.zoom_factor &&
      x <= (this.x_left + 1.75) * this.ctx.zoom_factor)
      return true;
    return false;
  }
  this.measure = function () {
    var geom = new PlowGeometry();
    geom.ll_x = this.x_left * this.ctx.zoom_factor;
    geom.ll_y = this.y_low * this.ctx.zoom_factor;
    geom.ur_x = this.x_right * this.ctx.zoom_factor;
    geom.ur_y = this.y_high * this.ctx.zoom_factor;

    return geom;
  };
}


function PlowPoint(ctx, x, y) {
  this.x = x;
  this.y = y;
  this.ctx = ctx;
}

function PlowAnnot(ctx, x, y, text_size, text_color, annot_type, number) {
  this.RELATIVE_OFFSET = 1;
  this.p = new PlowPoint(ctx, x, y);
  this.draw_type = text_color;
  this.text_size = text_size;
  this.annot_type = annot_type;
  this.number = number;
  this.ctx = ctx;

  this.draw = function (g, p0, node, highlight) {
    if (node == null)
      return;
    if (node.annotv[this.number] == null)
      return;

    var tsize = 0;
    var idx = this.ctx.zoom_factor / this.ctx.base_zoom_factor * (this.text_size + 4) - 4;
    if (idx < 0) return;

    switch (idx) {
      case 0:
        tsize = 8;
        break;
      case 1:
        tsize = 10;
        break;
      case 2:
        tsize = 12;
        break;
      case 3:
        tsize = 14;
        break;
      case 4:
        tsize = 14;
        break;
      case 5:
        tsize = 18;
        break;
      case 6:
        tsize = 18;
        break;
      case 7:
        tsize = 18;
        break;
      default:
        tsize = idx * 3;
    }


    g.font = tsize + "px Arial";
    if (node.invert)
      g.fillStyle = "white";
    else
      g.fillStyle = "black";
    if (highlight)
      g.fillStyle = "red";
    g.lineWidth = 0.5;

    var x = (this.p.x + p0.x) * this.ctx.zoom_factor;
    var y = (this.p.y + p0.y) * this.ctx.zoom_factor - tsize / 4;

    if ((this.annot_type & Plow.RELATIVE_POSITION) != 0) {
      var rel_x = (p0.x + node.relative_position + this.RELATIVE_OFFSET) * this.ctx.zoom_factor;
      if (x < rel_x)
        x = rel_x;
    }

    var tokens = node.annotv[this.number].split('\n');
    for (var i = 0; i < tokens.length; i++) {
      g.fillText(tokens[i], x, y);
      y += tsize * 1.4;
    }
    if ((this.annot_type & Plow.NEXT_RELATIVE_POSITION) != 0 || (this.annot_type & Plow.RELATIVE_POSITION) != 0) {
      node.relative_position = (x + g.measureText(node.annotv[this.number]).width) / this.ctx.zoom_factor - p0.x;
    }
  }
  this.set_borders = function (node) {
  }
}

function PlowAnnotPixmap(ctx, x, y, number) {
  this.p = new PlowPoint(ctx, x, y);
  this.number = number;
  this.ctx = ctx;

  this.draw = function (gctx, p0, node, highlight) {
    var x = (this.p.x + p0.x) * this.ctx.zoom_factor;
    var y = (this.p.y + p0.y) * this.ctx.zoom_factor;

    var img;
    var bix = node.pixmapv[this.number];
    if (typeof bix === 'undefined' || bix === null)
      return;
    if (node.invert)
      bix++;

    img = Bitmaps.img[bix];
    if (img == null) {
      var img = new Image();
      img.src = Bitmaps.images[bix];
      Bitmaps.img[bix] = img;
      Bitmaps.pending[bix] = [];
      Bitmaps.pending[bix].push(new PlowPoint(this.ctx, x, y));

      img.onload = function () {
        for (var i = 0; i < Bitmaps.pending[bix].length; i++)
          gctx.drawImage(img, Bitmaps.pending[bix][i].x, Bitmaps.pending[bix][i].y);
        Bitmaps.pending[bix] = null;
      }
    }
    else {
      if (!img.complete)
        Bitmaps.pending[bix].push(new PlowPoint(this.ctx, x, y));
      else
        gctx.drawImage(img, x, y);
    }

  }
  this.set_borders = function (node) {
  }
}

function PlowRect(ctx, x, y, width, height, fill_color, border_color, fill, fix_color) {
  this.ll = new PlowPoint(ctx, x, y);
  this.ur = new PlowPoint(ctx, x + width, y + height);
  this.border_color = border_color;
  this.fill_color = fill_color;
  this.fill = fill;
  this.fix_color = fix_color;
  this.ctx = ctx;

  this.draw = function (g, p, node, highlight) {

    var x = (this.ll.x + p.x) * this.ctx.zoom_factor;
    var y = (this.ll.y + p.y) * this.ctx.zoom_factor;
    var width = (this.ur.x - this.ll.x) * this.ctx.zoom_factor;
    var height = (this.ur.y - this.ll.y) * this.ctx.zoom_factor;

    g.lineWidth = 1;
    switch (this.border_color) {
      case Plow.COLOR_GRAY:
        g.strokeStyle = "grey";
        break;
      case Plow.COLOR_RED:
        g.strokeStyle = "red";
        break;
      case Plow.COLOR_WHITE:
        g.strokeStyle = "white";
        break;
      default:
        g.strokeStyle = "black";
    }
    if (highlight)
      g.strokeStyle = "red";
    g.strokeRect(x, y, width, height);

    if (this.fill) {
      switch (this.fill_color) {
        case Plow.COLOR_GRAY:
          g.fillStyle = "grey";
          break;
        case Plow.COLOR_RED:
          g.fillStyle = "red";
          break;
        case Plow.COLOR_YELLOW:
          g.fillStyle = "yellow";
          break;
        case Plow.COLOR_GREEN:
          g.fillStyle = "lightgreen";
          break;
        case Plow.COLOR_WHITE:
          if (node.invert)
            g.fillStyle = "black";
          else
            g.fillStyle = "white";
          break;
        default:
          if (node.invert)
            g.fillStyle = "white";
          else
            g.fillStyle = "black";
      }
      g.fillRect(x, y, width, height);
    }
  }
  this.set_borders = function (node) {
    if (this.ll.x + node.pos.x < node.x_left)
      node.x_left = this.ll.x + node.pos.x;
    if (this.ur.x + node.pos.x > node.x_right)
      node.x_right = this.ur.x + node.pos.x;
    if (this.ll.y + node.pos.y < node.y_low)
      node.y_low = this.ll.y + node.pos.y;
    if (this.ur.y + node.pos.y > node.y_high)
      node.y_high = this.ur.y + node.pos.y;
  }
}

function GDraw(ctx) {
  this.ctx = ctx;
  this.canvas = document.querySelector("canvas");
  this.gctx = this.canvas.getContext("2d");
  this.offset_top = this.canvas.offsetTop;
}

function PlowCtx() {
  this.gdh = 0;
  this.debug = false;
  this.nodraw = 0;
  this.zoom_factor = 20.0;
  this.base_zoom_factor = 20.0;
  this.offset_x = 0;
  this.offset_y = 0;
  this.x_right = 0.0;
  this.x_left = 0.0;
  this.y_high = 0.0;
  this.y_low = 0.0;
  this.a = new PlowArray(this);
  this.a_nc = new PlowArray(this);
  this.name = "Claes context";
  this.gdraw = new GDraw(this);
  this.selct_object = null;
  this.event_cb = null;
  this.event_object = null;

  this.draw = function () {
    if (this.nodraw > 0)
      return;
    this.gdraw.gctx.fillStyle = "white";
    this.gdraw.gctx.fillRect(0, 0, this.gdraw.canvas.width, this.gdraw.canvas.height);
    this.a.draw(this.gdraw.gctx, null, null, false);
  };

  this.connect = function () {
    for (var i = 0; i < this.a.size(); i++)
      this.a.get(i).connect();
  };
  this.scan = function () {
    console.log("ctx scan", this.a.size());
    for (var i = 0; i < this.a.size(); i++)
      this.a.get(i).scan();
  };
  this.set_nodraw = function () {
    this.nodraw++;
  };
  this.reset_nodraw = function () {
    this.nodraw--;
  };
  this.size = function () {
    return this.a.size()
  };
  this.event_handler = function (event, x, y) {
    var sts = 0;
    switch (event) {
      case Plow.eEvent_MB1Click:
      case Plow.eEvent_MB1ClickShift:
        for (var i = 0; i < this.a.size(); i++) {
          if (this.a.get(i) instanceof PlowNode) {
            sts = this.a.get(i).event_handler(event, x, y);
            if (sts == 1)
              break;
          }
        }
        if (sts == 1) {
          this.event_cb(event, this.event_object, x, y);
          this.draw();
        }
        break;
      case Plow.eEvent_Key_Up:
      case Plow.eEvent_Key_Down:
      case Plow.eEvent_Key_Left:
      case Plow.eEvent_Key_Right:
      case Plow.eEvent_Key_ShiftRight:
      case Plow.eEvent_Key_CtrlR:
      case Plow.eEvent_Key_CtrlL:
      case Plow.eEvent_Key_CtrlG:
        this.event_cb(event, null, 0, 0);
        break;
    }
  };

  this.set_select = function (select) {
    for (var i = 0; i < this.a.size(); i++) {
      if (this.a.get(i) instanceof PlowNode)
        this.a.get(i).set_select(select);
    }
  };
  this.set_invert = function (invert) {
    for (var i = 0; i < this.a.size(); i++) {
      if (this.a.get(i) instanceof PlowNode)
        this.a.get(i).set_invert(invert);
    }
  };

  this.get_select = function () {
    return this.select_object;
  };
  this.insert = function (n, dest) {
    this.a.add(n);
  };
  this.insertNode = function (n, destination, destCode) {
    this.a.insertNode(n, destination, destCode);
  };
  this.remove = function (n) {
    if (this.select_object == n)
      this.select_object = null;
    this.a.remove(n);
  };
  this.insert_nc = function (nc) {
    this.a_nc.add(nc);
  };
  this.configure = function () {
    this.a.configure();
    this.a.set_borders();

    var height = this.a.a.length * 1.0 * this.zoom_factor;
    this.gdraw.canvas.height = height;
  };
  this.get_parent_object = function (o) {
    return this.a.get_parent_object(o);
  };
  this.get_next_object = function (o) {
    return this.a.get_next_object(o);
  };
  this.get_last_object = function () {
    return this.a.get_last_object();
  };
  this.get_first_object = function () {
    return this.a.get_first_object();
  };
  this.get_previous_object = function (o) {
    return this.a.get_previous_object(o);
  };
  this.close_node = function (o) {
    this.a.close_node(o);
  };
  this.is_visible = function (o) {
    if ((o.y_high * this.zoom_factor <= window.pageYOffset + window.innerHeight - this.gdraw.offset_top) &&
      (o.y_low * this.zoom_factor >= window.pageYOffset - this.gdraw.offset_top))
      return true;
    return false;
  };
  this.scroll = function (y, factor) {
    window.scrollTo(window.scrollX, y * this.zoom_factor - window.innerHeight * factor + this.gdraw.offset_top)
  };
}

/** End Plow **/

/** Start Ev **/
function EvOpenChildrenData(node, open_next) {
  this.node = node;
  this.open_next = open_next;
}

var EvC = {
  eType_AlarmList: 0,
  eType_EventList: 1,
  eType_BlockList: 2
};


function Ev() {
  this.ctx = null;
  this.ncAlarmA = null;
  this.ncAlarmB = null;
  this.ncAlarm = null;
  this.ncInfo = null;
  this.ncSuccess = null;
  this.scan_update;
  this.priv = 0;
  this.mhSyncIdx = 0;
  this.type = EvC.eType_AlarmList;
  this.ala = null;
  this.maxEve = 30;
  this.maxAla = 30;
  var self = this;


  this.init = function () {
    this.type = this.get_type();
    switch (this.type) {
      case EvC.eType_EventList:
        document.title = "Event List";
        break;
      default:
        ;
    }

    this.priv = sessionStorage.getItem("pwr_privilege");
    console.log("pwr_privilege", this.priv);

    this.methods_init();
    this.ctx = new PlowCtx();
    this.ctx.event_cb = this.plow_event;
    this.createNodeClasses();

    this.ctx.gdh = new Gdh();
    this.ctx.gdh.open_cb = this.gdh_init_cb;
    this.ctx.gdh.init()

    this.ctx.gdraw.canvas.addEventListener("click", function (event) {
      var y = event.pageY - self.ctx.gdraw.offset_top;
      var x = event.pageX;
      if (event.shiftKey)
        ev.ctx.event_handler(Plow.eEvent_MB1ClickShift, x, y);
      else
        ev.ctx.event_handler(Plow.eEvent_MB1Click, x, y);
    });
    document.addEventListener("keydown", function (event) {
      if (event.keyCode == 40) {
        self.ctx.event_handler(Plow.eEvent_Key_Down);
        event.preventDefault();
      }
      else if (event.keyCode == 39) {
        if (event.shiftKey)
          self.ctx.event_handler(Plow.eEvent_Key_ShiftRight);
        else
          self.ctx.event_handler(Plow.eEvent_Key_Right);
        event.preventDefault();
      }
      else if (event.keyCode == 37) {
        self.ctx.event_handler(Plow.eEvent_Key_Left);
        event.preventDefault();
      }
      else if (event.keyCode == 38) {
        self.ctx.event_handler(Plow.eEvent_Key_Up);
        event.preventDefault();
      }
      else if (event.keyCode == 82) {
        if (event.ctrlKey)
          self.ctx.event_handler(Plow.eEvent_Key_CtrlR);
        event.preventDefault();
      }
      else if (event.keyCode == 76) {
        if (event.ctrlKey)
          self.ctx.event_handler(Plow.eEvent_Key_CtrlL);
        event.preventDefault();
      }
      else if (event.keyCode == 71) {
        if (event.ctrlKey)
          self.ctx.event_handler(Plow.eEvent_Key_CtrlG);
        event.preventDefault();
      }
    });
    // Ack
    document.getElementById("toolitem1").addEventListener("click", function (event) {
      console.log("Ack");
      self.ack();
    });
    // Navigator sup object
    document.getElementById("toolitem2").addEventListener("click", function (event) {
      var o = self.ctx.get_select();
      if (o === null || typeof o === 'undefined')
        return;
      console.log("toolitem2", o.userdata.e.supObject.vid,
        o.userdata.e.supObject.oix);
      self.ctx.gdh.getObjectFromAref(o.userdata.e.supObject, GdhOp.GET_OP_SELF, self.open_navigator_cb, null);
      console.log("toolitem2 event");
    });
    // Trace sup object
    document.getElementById("toolitem3").addEventListener("click", function (event) {
      console.log("toolitem5 event");
      var o = self.ctx.get_select();
      if (o === null)
        return;
      var newwindow = window.open("", "_blank");
      self.ctx.gdh.getObjectFromAref(o.userdata.e.supObject, GdhOp.GET_OP_METHOD_PLC, self.open_plc_cb, newwindow);
    });
    // Graph event name
    document.getElementById("toolitem4").addEventListener("click", function (event) {
      var o = self.ctx.get_select();
      if (o.userdata instanceof EvItemAlarm) {
        var newwindow = window.open("", "_blank");
        self.ctx.gdh.getObjectFromName(o.userdata.e.eventName, GdhOp.GET_OP_METHOD_GRAPH, self.open_graph_cb, newwindow);
      }
    });
    // Object raph event name
    document.getElementById("toolitem5").addEventListener("click", function (event) {
      var o = self.ctx.get_select();
      if (o.userdata instanceof EvItemAlarm) {
        var newwindow = window.open("", "_blank");
        self.ctx.gdh.getObjectFromName(o.userdata.e.eventName, GdhOp.GET_OP_METHOD_OBJECTGRAPH, self.open_graph_cb, newwindow);
      }
    });
    // Navigator event name
    document.getElementById("toolitem6").addEventListener("click", function (event) {
      var o = self.ctx.get_select();
      console.log("storage", localStorage.getItem("XttMethodNavigator"));
      localStorage.setItem("XttMethodNavigator", o.userdata.e.eventName);
      console.log("storage", localStorage.getItem("XttMethodNavigator"));
      console.log("toolitem6 event window", window.opener);
      window.opener.focus();
    });
    // Trace event name
    document.getElementById("toolitem7").addEventListener("click", function (event) {
      var o = self.ctx.get_select();
      if (o === null)
        return;
      var newwindow = window.open("", "_blank");
      self.ctx.gdh.getObjectFromName(o.userdata.e.eventName, GdhOp.GET_OP_METHOD_PLC, self.open_plc_cb, newwindow);
      console.log("toolitem7 event");
    });
    // History event name
    document.getElementById("toolitem8").addEventListener("click", function (event) {
      console.log("toolitem8 event");
    });
    // Object event name
    document.getElementById("toolitem9").addEventListener("click", function (event) {
      console.log("toolitem9 event");
      var o = self.ctx.get_select();
      if (o === null)
        return;
      var item = o.userdata;
      item.open_attributes(self);
    });
    // Crossref event name
    document.getElementById("toolitem10").addEventListener("click", function (event) {
      var o = self.ctx.get_select();
      if (o === null)
        return;
      self.ctx.gdh.crrSignal(o.userdata.e.eventName, self.open_crr_cb, o);
      console.log("toolitem10 event");
    });
    // Help event name
    document.getElementById("toolitem11").addEventListener("click", function (event) {
      console.log("toolitem11 event");
    });
    // Class help event name
    document.getElementById("toolitem12").addEventListener("click", function (event) {
      console.log("toolitem12 event");
      var o = self.ctx.get_select();
      if (o == null)
        return;
      if (o.userdata instanceof EvItemAlarm) {
        var newwindow = window.open("", "_blank");
        self.ctx.gdh.getObjectFromName(o.userdata.e.eventName, GdhOp.GET_OP_METHOD_HELPCLASS, self.open_helpclass_cb, newwindow);
      }
    });

  }

  this.is_authorized = function (access) {
    return (this.priv & access) ? true : false;
  }

  this.gdh_init_cb = function () {
    if (self.priv == null)
      self.ctx.gdh.login("", "", self.login_cb, self);

    //self.ctx.gdh.mhSync( self.mhSyncIdx, self.sync_cb, self);

    self.ctx.gdh.listSent = true;
    self.trace_cyclic();
  }

  this.login_cb = function (id, data, sts, result) {
    console.log("Login:", sts, result);
    if (sts & 1)
      self.priv = result;
    else
      self.priv = 0;
  };

  this.sync_cb = function (id, data, sts, result) {
    if (!(sts & 1))
      return;

    if (result.length === 0)
      return;

    if (self.type == EvC.eType_AlarmList) {

      self.ctx.set_nodraw();
      for (var i = result.length - 1; i >= 0; i--) {
        var e = result[i];
        switch (e.eventType) {
          case MhC.eEvent_Alarm:
          case MhC.eEvent_MaintenanceAlarm:
          case MhC.eEvent_SystemAlarm:
          case MhC.eEvent_UserAlarm1:
          case MhC.eEvent_UserAlarm2:
          case MhC.eEvent_UserAlarm3:
          case MhC.eEvent_UserAlarm4:
          case MhC.eEvent_Info:
          case MhC.eEvent_InfoSuccess:
            if (self.ctx.size() >= self.maxEve) {
              var o = self.ctx.get_last_object();
              self.ctx.remove(o);
            }

            new EvItemAlarm(self, e, null, Plow.DEST_BEFORE);
            break;
          case MhC.eEvent_Ack:
            var item = self.find_event(e.targetId);
            if (item !== null) {
              if (item.e.eventStatus & MhC.mEventStatus_NotRet) {
                item.e.eventStatus &= ~MhC.mEventStatus_NotAck;
                item.update();
              }
              else
                self.ctx.remove(item.node);
            }
            break;
          case MhC.eEvent_Return:
            var item = self.find_event(e.targetId);
            if (item !== null) {
              if (item.e.eventStatus & MhC.mEventStatus_NotAck) {
                item.e.eventStatus &= ~MhC.mEventStatus_NotRet;
                item.update();
              }
              else
                self.ctx.remove(item.node);
            }
            break;
        }
        if (self.mhSyncIdx < result[i].syncIdx)
          self.mhSyncIdx = result[i].syncIdx;
      }
      self.ctx.configure();
      self.ctx.reset_nodraw();
      self.ctx.draw();
    }
    else if (self.type == EvC.eType_EventList) {
      self.ctx.set_nodraw();
      for (var i = result.length - 1; i >= 0; i--) {
        var e = result[i];
        if (self.ctx.size() >= self.maxEve) {
          var o = self.ctx.get_last_object();
          self.ctx.remove(o);
        }

        new EvItemAlarm(self, e, null, Plow.DEST_BEFORE);
        if (self.mhSyncIdx < result[i].syncIdx)
          self.mhSyncIdx = result[i].syncIdx;
      }
      self.ctx.configure();
      self.ctx.reset_nodraw();
      self.ctx.draw();
    }
  };

  this.find_event = function (event_id) {
    for (var o = this.ctx.get_first_object();
         o !== null;
         o = this.ctx.get_next_object(o)) {
      var item = o.get_userdata();
      if (item.e.eventId.nix == event_id.nix &&
        item.e.eventId.idx == event_id.idx)
        return item;
    }
    return null;
  }

  this.is_authorized = function (access) {
    return (this.priv & access) ? true : false;
  }

  this.ack = function () {
    if (!this.is_authorized(Pwr.mAccess_RtEventsAck)) {
      window.alert("Not authorzed for this operation");
      return;
    }

    // Ack last
    var node = this.ctx.get_first_object();
    if (node === null)
      return;

    var item = node.get_userdata();
    console.log("Ack", item.e.eventText);
    this.ctx.gdh.mhAcknowledge(item.e.eventId, this.ack_cb, this);

    if (item.e.eventStatus & MhC.mEventStatus_NotRet) {
      item.e.eventStatus &= ~MhC.mEventStatus_NotAck;
      item.update();
    }
    else
      this.ctx.remove(item.node);
    this.ctx.configure();
    this.ctx.draw();
  }

  this.ack_cb = function (id, data, sts) {
    console.log("ack sts", sts);
  }

  this.open_objectgraph_cb = function (id, data, sts, result) {
    if ((sts & 1) != 0) {
      data.location.href = "ge.html?graph=" + result.param1 + "&instance=" + result.fullname;
      data.document.title = result.fullname;
    }
    else
      data.document.write("Error status " + sts);
  }

  this.open_graph_cb = function (id, data, sts, result) {
    if ((sts & 1) != 0) {
      var idx = result.param1.indexOf('.');
      if (idx != -1)
        result.param1 = result.param1.substring(0, idx);

      var instancestr = "";
      if (result.fullname !== "")
        instancestr = "&instance=" + result.fullname;

      data.location.href = "ge.html?graph=" + result.param1 + instancestr;
      data.document.title = result.param1;
    }
    else
      data.document.write("Error status " + sts);
  }

  this.open_plc_cb = function (id, data, sts, result) {
    if ((sts & 1) != 0) {
      var param1;
      if (result.param1 !== "")
        param1 = "&obj=" + result.param1;
      else
        param1 = "";
      console.log("flow.html?vid=" + result.objid.vid + "&oix=" + result.objid.oix + param1);
      data.location.href = "flow.html?vid=" + result.objid.vid + "&oix=" + result.objid.oix + param1;
      data.document.title = "Trace " + result.fullname;
    }
    else
      data.document.write("Error status " + sts);
  }

  this.open_navigator_cb = function (id, data, sts, result) {
    console.log("Open navigator", sts);
    if ((sts & 1) != 0) {
      localStorage.setItem("XttMethodNavigator", result.fullname);
      console.log("storage", localStorage.getItem("XttMethodNavigator"));
    }
    else
      console.log("Error status " + sts);
  }

  this.open_objectgraph_cb = function (id, data, sts, result) {
    if ((sts & 1) != 0) {
      var classname = result.classname.toLowerCase();
      if (classname.substring(0, 1) == "$")
        classname = classname.substring(1);
      var graphname = "pwr_c_" + classname;
      data.location.href = "ge.html?graph=" + graphname + "&instance=" + result.fullname;
      data.document.title = graphname + " " + result.fullname;
    }
    else
      data.document.write("Error status " + sts);
  }

  this.open_helpclass_cb = function (id, data, sts, result) {
    if ((sts & 1) != 0) {
      console.log("open_helpclass", result.param1);
      var url = location.protocol + "//" + location.host + result.param1;
      data.location.href = url;
    }
    else
      data.document.write("Error status " + sts);
  }

  this.methods_init = function () {
    localStorage.setItem("EvMethodNavigator", "");
  }

  this.collapse = function () {
    this.ctx.set_nodraw();
    for (var i = 0; i < this.ctx.a.size(); i++) {
      var node = this.ctx.a.get(i);
      if (node.level == 0)
        node.userdata.close(this);
    }
    this.ctx.reset_nodraw();
    this.ctx.draw();
  }

  this.trace_cyclic = function () {
    self.ctx.gdh.mhSync(self.mhSyncIdx, self.sync_cb, self);

    self.timer = setTimeout(self.trace_cyclic, 1000);
  };

  this.trace_scan = function (id, sts) {
    self.scan_update = false;
    if (self.scan_update)
      self.ctx.draw();
  };

  this.createNodeClasses = function () {
    var r1 = new PlowRect(this.ctx, 0, 0, 50, 1.0, Plow.COLOR_WHITE, Plow.COLOR_WHITE, true, false);
    var r2a = new PlowRect(this.ctx, 0.1, 0.15, 0.7, 0.7, Plow.COLOR_RED, Plow.COLOR_BLACK, true, false);
    var r2b = new PlowRect(this.ctx, 0.1, 0.15, 0.7, 0.7, Plow.COLOR_YELLOW, Plow.COLOR_BLACK, true, false);
    var r2info = new PlowRect(this.ctx, 0.1, 0.15, 0.7, 0.7, Plow.COLOR_WHITE, Plow.COLOR_BLACK, true, false);
    var r2success = new PlowRect(this.ctx, 0.1, 0.15, 0.7, 0.7, Plow.COLOR_GREEN, Plow.COLOR_BLACK, true, false);
    var a1 = new PlowAnnot(this.ctx, 1, 0.9, 4, Plow.COLOR_BLACK, 0, 0);
    var p1 = new PlowAnnotPixmap(this.ctx, 1.8, 0.2, 0);
    var p2 = new PlowAnnotPixmap(this.ctx, 2.4, 0.2, 1);
    var p3 = new PlowAnnotPixmap(this.ctx, 3.2, 0.2, 2);
    var p4 = new PlowAnnotPixmap(this.ctx, 4.0, 0.2, 3);
    var a2 = new PlowAnnot(this.ctx, 4.8, 0.9, 4, Plow.COLOR_BLACK, 0, 1);
    var a3 = new PlowAnnot(this.ctx, 11.5, 0.9, 4, Plow.COLOR_BLACK, Plow.NEXT_RELATIVE_POSITION, 2);
    var a4 = new PlowAnnot(this.ctx, 22.5, 0.9, 4, Plow.COLOR_BLACK, Plow.RELATIVE_POSITION, 3);

    // A alarm with red square
    this.ncAlarmA = new PlowNodeClass(this.ctx);
    this.ncAlarmA.insert(r1);
    this.ncAlarmA.insert(r2a);
    this.ncAlarmA.insert(a1);
    this.ncAlarmA.insert(p1);
    this.ncAlarmA.insert(p2);
    this.ncAlarmA.insert(p3);
    this.ncAlarmA.insert(p4);
    this.ncAlarmA.insert(a2);
    this.ncAlarmA.insert(a3);
    this.ncAlarmA.insert(a4);
    this.ctx.insert_nc(this.ncAlarmA);

    // A alarm with yellow square
    this.ncAlarmB = new PlowNodeClass(this.ctx);
    this.ncAlarmB.insert(r1);
    this.ncAlarmB.insert(r2b);
    this.ncAlarmB.insert(a1);
    this.ncAlarmB.insert(p1);
    this.ncAlarmB.insert(p2);
    this.ncAlarmB.insert(p3);
    this.ncAlarmB.insert(p4);
    this.ncAlarmB.insert(a2);
    this.ncAlarmB.insert(a3);
    this.ncAlarmB.insert(a4);
    this.ctx.insert_nc(this.ncAlarmB);

    // D and C alarm with no square
    this.ncAlarm = new PlowNodeClass(this.ctx);
    this.ncAlarm.insert(r1);
    this.ncAlarm.insert(a1);
    this.ncAlarm.insert(p1);
    this.ncAlarm.insert(p2);
    this.ncAlarm.insert(p3);
    this.ncAlarm.insert(p4);
    this.ncAlarm.insert(a2);
    this.ncAlarm.insert(a3);
    this.ncAlarm.insert(a4);
    this.ctx.insert_nc(this.ncAlarm);

    // Info with white square
    this.ncInfo = new PlowNodeClass(this.ctx);
    this.ncInfo.insert(r1);
    this.ncInfo.insert(r2info);
    this.ncInfo.insert(a1);
    this.ncInfo.insert(p1);
    this.ncInfo.insert(p2);
    this.ncInfo.insert(p3);
    this.ncInfo.insert(p4);
    this.ncInfo.insert(a2);
    this.ncInfo.insert(a3);
    this.ncInfo.insert(a4);
    this.ctx.insert_nc(this.ncInfo);

    // InfoSuccess with green square
    this.ncSuccess = new PlowNodeClass(this.ctx);
    this.ncSuccess.insert(r1);
    this.ncSuccess.insert(r2success);
    this.ncSuccess.insert(a1);
    this.ncSuccess.insert(p1);
    this.ncSuccess.insert(p2);
    this.ncSuccess.insert(p3);
    this.ncSuccess.insert(p4);
    this.ncSuccess.insert(a2);
    this.ncSuccess.insert(a3);
    this.ncSuccess.insert(a4);
    this.ctx.insert_nc(this.ncSuccess);
  };

  this.plow_event = function (event, object, x, y) {

    var item = null;
    if (object != null)
      item = object.get_userdata();

    switch (event) {
      case Plow.eEvent_ObjectDeleted:
        //if ( object.userdata instanceof EvItemAttr) {
        //	object.userdata.scan_close( self);
        //}
        break;
      case Plow.eEvent_MB1Click:
        if (item != null && item.e.eventMoreText !== "" &&
          x >= object.measure().ll_x + 70 && x <= object.measure().ll_x + 90) {
          window.alert(item.e.eventMoreText);
        }
        else if (object.select) {
          object.set_select(false);
          object.set_invert(false);
        }
        else {
          self.ctx.set_select(false);
          object.set_select(true);
          self.ctx.set_invert(false);
          object.set_invert(true);
          self.ctx.draw();
        }
        break;
      case Plow.eEvent_MB1ClickShift:
        break;
      case Plow.eEvent_Key_Down: {
        var o = self.ctx.get_select();
        if (o != null) {
          var next = self.ctx.get_next_object(o);
          if (next != null) {
            o.set_select(false);
            o.set_invert(false);
            next.set_select(true);
            next.set_invert(true);
            if (!self.ctx.is_visible(next))
              self.ctx.scroll(next.y_low, 0.10);
          }
        }
        else {
          o = self.ctx.a.a[0];
          o.set_select(true);
          o.set_invert(true);
        }

        break;
      }
      case Plow.eEvent_Key_Up: {
        var o = self.ctx.get_select();
        if (o != null) {
          var next = self.ctx.get_previous_object(o);
          if (next != null) {
            o.set_select(false);
            o.set_invert(false);
            next.set_select(true);
            next.set_invert(true);
            o.draw(self.ctx.gdraw.gctx, null, null, 0);
            next.draw(self.ctx.gdraw.gctx, null, null, 0);
            if (!self.ctx.is_visible(next))
              self.ctx.scroll(next.y_low, 0.90);
          }
        }

        break;
      }
      case Plow.eEvent_Key_Right: {
        break;
      }
      case Plow.eEvent_Key_Left: {
        break;
      }
      case Plow.eEvent_Key_ShiftRight: {
        break;
      }
      case Plow.eEvent_Key_CtrlR: {
        break;
      }
      case Plow.eEvent_Key_CtrlL: {
        var o = self.ctx.get_select();
        if (o.userdata instanceof EvItemAlarm) {
          var newwindow = window.open("", "_blank");
          self.ctx.gdh.getObject(o.userdata.objid, GdhOp.GET_OP_METHOD_PLC, self.open_plc_cb, newwindow);
        }
        break;
      }
      case Plow.eEvent_Key_CtrlG: {
        var o = self.ctx.get_select();
        if (o.userdata instanceof EvItemAlarm) {
          var newwindow = window.open("", "_blank");
          self.ctx.gdh.getObject(o.userdata.objid, GdhOp.GET_OP_METHOD_PLC, self.open_objectgraph_cb, newwindow);
        }
        break;
      }
    }
  };

  // Query 'list' = alarm/event/block. Alarm is default.
  this.get_type = function () {
    var query = window.location.search.substring(1);
    var type;

    if (query === "")
      return EvC.eType_AlarmList;

    var vars = query.split('&');
    var typestr = vars[0].substring(5);
    if (typestr == "event")
      type = EvC.eType_EventList;
    else if (typestr == "block")
      type = EvC.eType_BlockList;
    else
      type = EvC.eType_AlarmList;

    return type;
  };
}

function EvItemAlarm(ev, e,
                     destination, destCode) {

  this.node;
  this.e = e;

  var nodeclass = ev.ncAlarm;
  switch (this.e.eventType) {
    case MhC.eEvent_Alarm:
    case MhC.eEvent_MaintenanceAlarm:
    case MhC.eEvent_SystemAlarm:
    case MhC.eEvent_UserAlarm1:
    case MhC.eEvent_UserAlarm2:
    case MhC.eEvent_UserAlarm3:
    case MhC.eEvent_UserAlarm4:
    case MhC.eEvent_Block:
    case MhC.eEvent_Reblock:
    case MhC.eEvent_CancelBlock:
    case MhC.eEvent_Unblock:
      switch (this.e.eventPrio) {
        case MhC.eEventPrio_A:
          nodeclass = ev.ncAlarmA;
          break;
        case MhC.eEventPrio_B:
          nodeclass = ev.ncAlarmB;
          break;
        case MhC.eEventPrio_C:
          nodeclass = ev.ncAlarm;
          break;
        case MhC.eEventPrio_D:
          nodeclass = ev.ncAlarm;
          break;
      }
      break;
    case MhC.eEvent_Info:
      nodeclass = ev.ncInfo;
      break;
    case MhC.eEvent_InfoSuccess:
      nodeclass = ev.ncSuccess;
      break;
  }
  this.node = new PlowNode(ev.ctx, nodeclass, 0);
  ev.ctx.insertNode(this.node, destination, destCode);
  this.node.set_userdata(this);

  switch (this.e.eventType) {
    case MhC.eEvent_Alarm:
    case MhC.eEvent_MaintenanceAlarm:
    case MhC.eEvent_SystemAlarm:
    case MhC.eEvent_UserAlarm1:
    case MhC.eEvent_UserAlarm2:
    case MhC.eEvent_UserAlarm3:
    case MhC.eEvent_UserAlarm4:
    case MhC.eEvent_Block:
    case MhC.eEvent_Reblock:
    case MhC.eEvent_CancelBlock:
    case MhC.eEvent_Unblock:
      switch (this.e.eventPrio) {
        case MhC.eEventPrio_A:
          this.node.set_annotation(0, "A");
          break;
        case MhC.eEventPrio_B:
          this.node.set_annotation(0, "B");
          break;
        case MhC.eEventPrio_C:
          this.node.set_annotation(0, "C");
          break;
        case MhC.eEventPrio_D:
          this.node.set_annotation(0, "D");
          break;
      }
      break;
  }
  switch (this.e.eventType) {
    case MhC.eEvent_MaintenanceAlarm:
      this.node.set_annotation_pixmap(3, Bitmaps.maintenance);
      break;
    case MhC.eEvent_SystemAlarm:
      this.node.set_annotation_pixmap(3, Bitmaps.system);
      break;
    case MhC.eEvent_Block:
      this.e.eventText = "Blocked";
      break;
    case MhC.eEvent_Reblock:
      this.e.eventText = "Reblocked";
      break;
    case MhC.eEvent_CancelBlock:
      this.e.eventText = "Block canceled";
      break;
    case MhC.eEvent_Unblock:
      this.e.eventText = "Unblocked";
      break;
  }
  this.node.set_annotation(1, this.e.eventTime.substring(0, 17));
  this.node.set_annotation(2, this.e.eventText);
  this.node.set_annotation(3, this.e.eventName);

  switch (ev.type) {
    case EvC.eType_EventList:
      switch (this.e.eventType) {
        case MhC.eEvent_Alarm:
        case MhC.eEvent_MaintenanceAlarm:
        case MhC.eEvent_SystemAlarm:
        case MhC.eEvent_UserAlarm1:
        case MhC.eEvent_UserAlarm2:
        case MhC.eEvent_UserAlarm3:
        case MhC.eEvent_UserAlarm4:
        case MhC.eEvent_Info:
        case MhC.eEvent_InfoSuccess:
          this.node.set_annotation_pixmap(0, Bitmaps.eventalarm);
          break;
        case MhC.eEvent_Ack:
          this.node.set_annotation_pixmap(0, Bitmaps.eventacked);
          break;
        case MhC.eEvent_Return:
          this.node.set_annotation_pixmap(0, Bitmaps.eventreturn);
          break;
        case MhC.eEvent_Block:
        case MhC.eEvent_Reblock:
          this.node.set_annotation_pixmap(0, Bitmaps.blockl);
          break;
        case MhC.eEvent_CancelBlock:
        case MhC.eEvent_Unblock:
          this.node.set_annotation_pixmap(0, Bitmaps.blockr);
          break;
      }
      break;
    case EvC.eType_AlarmList:
      if (this.e.eventStatus & MhC.mEventStatus_NotRet)
        this.node.set_annotation_pixmap(1, Bitmaps.alarm);
      if (this.e.eventStatus & MhC.mEventStatus_NotAck)
        this.node.set_annotation_pixmap(0, Bitmaps.ack);
      if (this.e.eventMoreText !== "")
        this.node.set_annotation_pixmap(2, Bitmaps.info);
      break;
  }

  this.update = function () {
    if (this.e.eventStatus & MhC.mEventStatus_NotRet)
      this.node.set_annotation_pixmap(1, Bitmaps.alarm);
    else
      this.node.set_annotation_pixmap(1, null);

    if (this.e.eventStatus & MhC.mEventStatus_NotAck)
      this.node.set_annotation_pixmap(0, Bitmaps.ack);
    else
      this.node.set_annotation_pixmap(0, null);
  }
}


var ev = new Ev();
ev.init();

/** End Ev **/
