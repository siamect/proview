"use strict";

enum Event {
  Ack = 1,
  Block = 2,
  Cancel = 3,
  CancelBlock = 4,
  Missing = 5,
  Reblock = 6,
  Return = 7,
  Unblock = 8,
  InfoSuccess = 32,
  Alarm = 64,
  MaintenanceAlarm = 128,
  SystemAlarm = 256,
  UserAlarm1 = 512,
  UserAlarm2 = 1024,
  UserAlarm3 = 2048,
  UserAlarm4 = 4096,
  Info = 8192
}

enum EventPrio {
  D = 64,
  C,
  B,
  A
}

enum EventStatus {
  NotRet = 1 << 0,
  NotAck = 1 << 1,
  Block = 1 << 2
}

class MhEventId {
  nix;
  idx;
  birthTime;
}

class MhEvent {
  eventTime;
  eventText;
  eventName;
  eventFlags;
  eventStatus;
  eventPrio;
  eventId;
  targetId;
  eventType;
  object;
  supObject;
  eventMoreText;
  syncIdx;
}

class Sub {
  sts;
  refid;
  type;
  elements;
  name;
  value;
}

class ObjectInfo {
  objid: PwrtObjid;
  cid;
  has_children;
  name;
  description;
  classname;
  fullname;
  param1;
}

class AttributeInfo {
  name;
  type;
  size;
  flags;
  elements;
  objid: PwrtObjid;
  full_name;
  classname;
}

class MenuButton {
  type;
  text;
  name;
  url;
}

class OpwindMenuInfo {
  title;
  text;
  enable_language;
  enable_login;
  enable_alarmlist;
  enable_eventlog;
  enable_navigator;
  disable_help;
  disable_proview;
  language;
  buttons = [];
}

class CrrInfo {
  type;
  objid: PwrtObjid;
  name;
  classname;
}

class GlowPieInfo {
  sector_num;
  min_val;
  max_val;
}

class GlowBarChartInfo {
  bars;
  barsegments;
  min_value;
  max_value;
}

class GlowTableInfo {
  columns;
  rows;
  column_size: Array = new Array(30);
}

class PendingData {
  func_cb: (id: number, val: number, sts: number = 0, res: number = 0) => void;
  data: object;

  constructor(func_cb, data) {
    this.func_cb = func_cb;
    this.data = data;
  }
}

enum GdhOp {
  GET_OP_SELF = 1,
  GET_OP_METHOD_PLC,
  GET_OP_METHOD_OBJECTGRAPH,
  GET_OP_METHOD_GRAPH,
  GET_OP_METHOD_HELPCLASS
}

enum Msg {
  SET_OBJECT_INFO_BOOLEAN = 1,
  SET_OBJECT_INFO_FLOAT,
  SET_OBJECT_INFO_INT,
  SET_OBJECT_INFO_STRING,
  GET_OBJECT_INFO_BOOLEAN,
  GET_OBJECT_INFO_FLOAT,
  GET_OBJECT_INFO_INT,
  GET_OBJECT_INFO_STRING,
  TOGGLE_OBJECT_INFO,
  REF_OBJECT_INFO,
  GET_OBJECT_REF_INFO_BOOLEAN,
  GET_OBJECT_REF_INFO_FLOAT,
  GET_OBJECT_REF_INFO_INT,
  GET_OBJECT_REF_INFO_STRING,
  UNREF_OBJECT_INFO,
  NAME_TO_OBJID,
  OBJID_TO_NAME,
  GET_ROOT_LIST,
  GET_NEXT_OBJECT,
  GET_CHILD,
  GET_NEXT_SIBLING,
  GET_OBJECT_CLASS,
  GET_CLASS_LIST,
  CLASS_ID_TO_OBJID,
  GET_OBJECT_REF_INFO_ALL,
  REF_OBJECT_INFO_LIST,
  POLL,
  STATISTICS,
  CHECK_USER,
  GET_NODE_OBJECT,
  LOG_STRING,
  UNREF_OBJECT_INFO_ALL,
  CREATE_INSTANCE_FILE,
  GET_ATTRIBUTE_CHAR,
  GET_CLASS_ATTRIBUTE,
  GET_ALL_CLASS_ATTRIBUTES,
  GET_ALL_SIBLINGS,
  GET_ALL_XTT_SIBLINGS,
  GET_ALL_XTT_CHILDREN,
  REF_OBJECT_INFO_VECTOR,
  GET_SUBSCRIPTIONS,
  CRR_SIGNAL,
  CRR_OBJECT,
  GET_PARENT,
  GET_OBJECT_INFO_OBJID,
  GET_OBJECT_REF_INFO_BOOLEAN_ARRAY,
  GET_OBJECT_REF_INFO_FLOAT_ARRAY,
  GET_OBJECT_REF_INFO_INT_ARRAY,
  GET_OBJECT_REF_INFO_STRING_ARRAY,
  GET_MSG,
  GET_MSG_TEXT,
  NAME_TO_ATTRREF,
  ATTRREF_TO_NAME,
  GET_ATTRREF_TID,
  GET_SUPER_CLASS,
  GET_ALL_CLASS_ATTRIBUTES_STRING,
  GET_OBJECT_INFO_FLOAT_ARRAY,
  GET_OBJECT_INFO_INT_ARRAY,
  GET_CIRCBUFF_INFO,
  UPDATE_CIRCBUFF_INFO,
  GET_ATTRIBUTE_FLAGS,
  CLASSNAME_TO_ID,
  GET_OBJECT,
  GET_OPWIND_MENU,
  GET_OBJECT_FROM_NAME,
  MH_SYNC,
  MH_ACK,
  GET_OBJECT_FROM_AREF
}

class DataViewHelper {
  dv: DataView;
  offset = 0;

  constructor(data: ArrayBuffer) {
    this.dv = new DataView(data);
  }

  getUint8() {
    this.offset += 1;
    return this.dv.getUint8(this.offset - 1);
  }

  getUint16() {
    this.offset += 2;
    return this.dv.getUint8(this.offset - 2);
  }

  getInt32() {
    this.offset += 4;
    return this.dv.getInt32(this.offset - 4);
  }

  getUint32() {
    this.offset += 4;
    return this.dv.getUint32(this.offset - 4);
  }

  getFloat32() {
    this.offset += 4;
    return this.dv.getFloat32(this.offset - 4);
  }

  getString() {
    let csize = this.getUint16();
    let iarr = new Uint8Array(csize);
    for (let k = 0; k < csize; k++) {
      iarr[k] = this.getUint8();
    }
    return String.fromCharCode.apply(null, iarr);
  }
}

class Gdh {
  sub: Array<Sub> = [];
  static PORT = 4448;
  ws: WebSocket = null;
  next_id = 1234;
  subscriptionCount = 1;
  listSent = false;
  pending = [];

  constructor(open_cb, close_cb = null) {
    if (window.location.hostname === "") {
      this.ws = new WebSocket("ws:127.0.0.1:" + String(Gdh.PORT));
    } else {
      this.ws = new WebSocket("ws://" + window.location.hostname + ":" + String(Gdh.PORT));
    }
    this.ws.binaryType = "arraybuffer";

    this.ws.onopen = function (e) {
      if (open_cb !== null) {
        open_cb();
      }
    };

    this.ws.onclose = function () {
      if (close_cb !== null) {
        close_cb();
      }
    };

    this.ws.onmessage = function (e) {
      let dv = new DataViewHelper(e.data);
      let type = dv.getUint8();
      let id = dv.getUint32();

      let resolve = this.pending[id];
      if (type === Msg.GET_OBJECT_REF_INFO_ALL) {
        resolve(new DataViewHelper(e.data));
        delete this.pending[id];
      } else {
        let sts = dv.getUint32();
        let value = this.unpackType(dv, sts, type);
        resolve({
          "sts": sts,
          "value": value
        });
        delete this.pending[id];
      }
    };
  }

  unpackType(dv, sts, type) {
    switch (type) {
      case Msg.GET_OBJECT_INFO_BOOLEAN:
        return dv.getUint8();
      case Msg.GET_OBJECT_INFO_INT:
        return dv.getUint32();
      case Msg.GET_OBJECT_INFO_FLOAT:
        return dv.getFloat32();
      case Msg.GET_OBJECT_INFO_FLOAT_ARRAY:
        let asize = dv.getInt32();
        let value = new Array(asize);
        for (let i = 0; i < asize; i++) {
          value[i] = dv.getFloat32();
        }
        return value;
      case Msg.GET_ALL_XTT_CHILDREN:
        let result = [];
        let size = dv.getUint32();
        for (let i = 0; i < size; i++) {
          let info = new ObjectInfo();
          let vid = dv.getUint32();
          let oix = dv.getUint32();
          info.objid = new PwrtObjid(vid, oix);
          info.cid = dv.getUint32();
          info.has_children = dv.getUint16() !== 0;
          info.name = dv.getString();
          info.description = dv.getString();
          info.classname = dv.getString();
          result.push(info);
        }
        return result;
      case Msg.GET_ALL_CLASS_ATTRIBUTES:
        let result = [];
        let size = dv.getUint32();
        for (let i = 0; i < size; i++) {
          let info = new AttributeInfo();
          info.type = dv.getUint32();
          info.flags = dv.getUint32();
          info.size = dv.getUint16();
          info.elements = dv.getUint16();
          info.name = dv.getString();
          info.classname = dv.getString();
          result.push(info);
        }
        return result;
      case Msg.GET_OBJECT:
      case Msg.GET_OBJECT_FROM_AREF:
      case Msg.GET_OBJECT_FROM_NAME:
        let info = null;
        if (odd(sts)) {
          info = new ObjectInfo();
          let vid = dv.getUint32();
          let oix = dv.getUint32();
          info.objid = new PwrtObjid(vid, oix);
          info.cid = dv.getUint32();
          info.has_children = dv.getUint16() !== 0;
          info.name = dv.getString();
          info.fullname = dv.getString();
          info.classname = dv.getString();
          info.description = dv.getString();
          info.param1 = dv.getString();
        }
        return info;
      case Msg.CRR_SIGNAL:
        let crrtext = null;
        let result = [];
        if (odd(sts)) {
          let size = dv.getUint16();
          for (let i = 0; i < size; i++) {
            let info = new CrrInfo();
            info.type = dv.getUint16();
            let vid = dv.getUint32();
            let oix = dv.getUint32();
            info.objid = new PwrtObjid(vid, oix);
            info.name = dv.getString();
            info.classname = dv.getString();
            result.push(info);
          }
        }
        return result;
      case Msg.GET_OPWIND_MENU:
        let result = new OpwindMenuInfo();

        if (odd(sts)) {
          result.title = dv.getString();
          result.text = dv.getString();

          result.enable_language = dv.getUint32();
          result.enable_login = dv.getUint32();
          result.enable_alarmlist = dv.getUint32();
          result.enable_eventlog = dv.getUint32();
          result.enable_navigator = dv.getUint32();
          result.disable_help = dv.getUint32();
          result.disable_proview = dv.getUint32();
          result.language = dv.getUint32();

          let bsize = dv.getUint16();

          for (let i = 0; i < bsize; i++) {
            let button = new MenuButton();
            button.type = dv.getUint32();
            button.text = dv.getString();
            button.name = dv.getString();
            button.url = dv.getString();
            result.buttons.push(button);
          }
        }
        return result;
      case Msg.CHECK_USER:
        let priv = 0;
        if (odd(sts)) {
          priv = dv.getUint32();
        }
        return priv;
      case Msg.GET_MSG:
        let msg = "";
        if (odd(sts)) {
          msg = dv.getString();
        }
        return msg;
      case Msg.MH_SYNC:
        let result = [];
        let size = dv.getUint32();
        for (let i = 0; i < size; i++) {
          let e = new MhEvent();
          e.eventTime = dv.getString();
          e.eventText = dv.getString();
          e.eventName = dv.getString();
          e.eventFlags = dv.getUint32();
          e.eventStatus = dv.getUint32();
          e.eventPrio = dv.getUint32();

          e.eventId = new MhEventId();
          e.eventId.nix = dv.getUint32();
          e.eventId.idx = dv.getUint32();
          e.eventId.birthTime = dv.getString();

          e.targetId = new MhEventId();
          e.targetId.nix = dv.getUint32();
          e.targetId.idx = dv.getUint32();
          e.targetId.birthTime = dv.getString();

          e.eventType = dv.getUint32();
          let objid = new PwrtObjid(0, 0);
          objid.vid = dv.getUint32();
          objid.oix = dv.getUint32();
          e.object = new PwrtAttrRef();
          e.object.objid = objid;
          e.object.offset = dv.getUint32();
          e.object.body = dv.getUint32();
          e.object.size = dv.getUint32();
          e.object.flags = dv.getUint32();
          let supObjid = new PwrtObjid(0, 0);
          supObjid.vid = dv.getUint32();
          supObjid.oix = dv.getUint32();
          e.supObject = new PwrtAttrRef();
          e.supObject.objid = supObjid;
          e.supObject.offset = dv.getUint32();
          e.supObject.body = dv.getUint32();
          e.supObject.size = dv.getUint32();
          e.supObject.flags = dv.getUint32();

          e.eventMoreText = dv.getString();

          e.syncIdx = dv.getUint32();
          result.push(e);
        }
        return result;
      default:
        return null;
    }
  }

  Uint8ArrayHelper(tag) {
    let obj = {
      buf: [tag, 0],
      pack16Bit: function(value) {
        this.buf.push(value & 0xFF);
        this.buf.push((value >> 8) & 0xFF);
      },
      pack32Bit: function(value) {
        this.buf.push(value & 0xFF);
        this.buf.push((value >> 8) & 0xFF);
        this.buf.push((value >> 16) & 0xFF);
        this.buf.push((value >> 24) & 0xFF);
      },
      packString: function(string) {
        this.pack16Bit(string.length);
        for (let i = 0; i < string.length; i++) {
          this.buf.push(string.charCodeAt(i));
        }
      }
    };
    obj.pack32Bit(this.next_id);
    return obj;
  }

  getObjectInfoBoolean(name) {
    let helper = this.Uint8ArrayHelper(Msg.GET_OBJECT_INFO_BOOLEAN);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  getObjectInfoInt(name) {
    let helper = this.Uint8ArrayHelper(Msg.GET_OBJECT_INFO_INT);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  getObjectInfoIntArray(name, asize) {
    let helper = this.Uint8ArrayHelper(Msg.GET_OBJECT_INFO_INT_ARRAY);
    helper.pack32Bit(asize);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  getObjectInfoFloat(name) {
    let helper = this.Uint8ArrayHelper(Msg.GET_OBJECT_INFO_FLOAT);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  getObjectInfoFloatArray(name, asize) {
    let helper = this.Uint8ArrayHelper(Msg.GET_OBJECT_INFO_FLOAT_ARRAY);
    helper.pack32Bit(asize);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  refObjectInfo(name, type, elements) {
    let sub = new Sub();
    sub.name = name;
    sub.refid = this.subscriptionCount;
    sub.type = type;
    sub.elements = elements;
    this.sub[this.subscriptionCount] = sub;
    this.subscriptionCount++;
    if (this.listSent) {
      let helper = this.Uint8ArrayHelper(Msg.REF_OBJECT_INFO);
      helper.pack32Bit(sub.refid);
      helper.pack32Bit(sub.elements);
      helper.packString(sub.name);

      this.ws.send(helper.buf);
    }
    return sub.refid;
  }

  unrefObjectInfo(refid) {
    let helper = this.Uint8ArrayHelper(Msg.UNREF_OBJECT_INFO);
    helper.pack32Bit(refid);
    delete this.sub[refid];
    return this.sendRequest(helper);
  }

  refObjectInfoList() {
    let size = 0;
    let len = 0;

    for (let i in this.sub) {
      size += 12 + this.sub[i].name.length;
      len++;
    }
    let helper = this.Uint8ArrayHelper(Msg.REF_OBJECT_INFO_LIST);
    helper.pack32Bit(len);
    this.sub.slice(1).forEach(function (s) {
      helper.pack32Bit(s.refid);
      helper.pack32Bit(s.elements);
      helper.packString(s.name);
    });

    this.listSent = true;
    return this.sendRequest(helper);
  }

  getRefObjectInfoAll(return_cb) {
    let helper = this.Uint8ArrayHelper(Msg.GET_OBJECT_REF_INFO_ALL);
    this.sendRequest(helper).then(function (dv) {
      let type = dv.getUint8();
      let id = dv.getUint32();
      let sts = dv.getUint32();
      let size = dv.getUint32();
      for (let i = 0; i < size; i++) {
        let eid = dv.getUint32();
        let esize = dv.getUint32();
        let sub = this.gdh.sub[eid];
        if (typeof sub !== 'undefined') {
          let value;
          switch (sub.type) {
            case Type.Boolean:
              if (sub.elements <= 1) {
                value = dv.getUint8();
              } else {
                let elements = esize;
                if (elements !== sub.elements) {
                  console.log("Subscription size error", elements,
                      sub.elements, eid);
                }
                value = new Array(elements);
                for (let k = 0; k < elements; k++) {
                  value[k] = dv.getUint8();
                }
              }
              break;
            case Type.Float32:
              if (sub.elements <= 1) {
                value = dv.getFloat32();
              } else {
                let elements = esize / 4;
                if (elements !== sub.elements) {
                  console.log("Subscription size error", elements,
                      sub.elements, eid);
                }
                value = new Array(elements);
                for (let k = 0; k < elements; k++) {
                  value[k] = dv.getFloat32();
                }
              }
              break;
            case Type.Int8:
            case Type.Int16:
            case Type.Int32:
            case Type.UInt8:
            case Type.UInt16:
            case Type.UInt32:
            case Type.Status:
            case Type.NetStatus:
            case Type.Mask:
            case Type.Enum:
            case Type.Bit:
              if (sub.elements <= 1) {
                value = dv.getInt32();
              } else {
                let elements = esize / 4;
                if (elements !== sub.elements) {
                  console.log("Subscription size error", elements,
                      sub.elements, eid);
                }
                value = new Array(elements);
                for (let k = 0; k < elements; k++) {
                  value[k] = dv.getInt32();
                }
              }
              break;
            case Type.String:
            case Type.Time:
            case Type.DeltaTime:
            case Type.AttrRef:
            case Type.Objid:
              if (sub.elements <= 1) {
                value = dv.getString();
              } else {
                let elements = sub.elements;
                if (elements !== sub.elements) {
                  console.log("Subscription size error", elements,
                      sub.elements, eid);
                }
                value = new Array(elements);
                for (let l = 0; l < elements; l++) {
                  value[l] = dv.getString();
                }
              }
              break;
            default:
              break;
          }
          this.gdh.sub[eid].value = value;
        }
      }
      if (typeof this.gdh.pending[id] === 'undefined') {
        console.log("** GetObjectRefInfoAll received removed", id);
        return;
      }
      return_cb();
    });
  }

  getObjectRefInfo(id) {
    return this.sub[id].value;
  }

  setObjectInfoBoolean(name, value) {
    let helper = this.Uint8ArrayHelper(Msg.SET_OBJECT_INFO_BOOLEAN);
    helper.pack32Bit(value);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  setObjectInfoInt(name, value) {
    let helper = this.Uint8ArrayHelper(Msg.SET_OBJECT_INFO_INT);
    helper.pack32Bit(value);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  setObjectInfoFloat(name, value) {
    let helper = this.Uint8ArrayHelper(Msg.SET_OBJECT_INFO_FLOAT);
    let fbuf = new ArrayBuffer(4);
    let fa = new Float32Array(fbuf);
    fa[0] = value;
    let ba = new Uint8Array(fbuf);
    helper.buf.push(ba[0]);
    helper.buf.push(ba[1]);
    helper.buf.push(ba[2]);
    helper.buf.push(ba[3]);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  setObjectInfoString(name, value) {
    let helper = this.Uint8ArrayHelper(Msg.SET_OBJECT_INFO_STRING);
    helper.packString(value);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  setObjectInfo(name, value, type) {
    if (type === Type.Boolean) {
      return this.setObjectInfoBoolean(name, value);
    } else if (type === Type.Float32 || type === Type.Float64) {
      return this.setObjectInfoFloat(name, value);
    } else if (type === Type.String) {
      return this.setObjectInfoString(name, value);
    } else {
      return this.setObjectInfoInt(name, value);
    }
  }

  toggleObjectInfo(name) {
    let helper = this.Uint8ArrayHelper(Msg.TOGGLE_OBJECT_INFO);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  getAllXttChildren(oid) {
    let helper = this.Uint8ArrayHelper(Msg.GET_ALL_XTT_CHILDREN);
    helper.pack32Bit(oid.vid);
    helper.pack32Bit(oid.oix);
    return this.sendRequest(helper);
  }

  getAllClassAttributes(cid, oid) {
    let helper = this.Uint8ArrayHelper(Msg.GET_ALL_CLASS_ATTRIBUTES);
    helper.pack32Bit(cid);
    helper.pack32Bit(oid.vid);
    helper.pack32Bit(oid.oix);
    return this.sendRequest(helper);
  }

  getObject(oid, op) {
    let helper = this.Uint8ArrayHelper(Msg.GET_OBJECT);
    helper.pack16Bit(op);
    helper.pack32Bit(oid.vid);
    helper.pack32Bit(oid.oix);
    return this.sendRequest(helper);
  }

  getObjectFromAref(aref, op) {
    let helper = this.Uint8ArrayHelper(Msg.GET_OBJECT_FROM_AREF);
    helper.pack16Bit(op);
    helper.pack32Bit(aref.objid.vid);
    helper.pack32Bit(aref.objid.oix);
    helper.pack32Bit(aref.offset);
    helper.pack32Bit(aref.body);
    helper.pack32Bit(aref.size);
    helper.pack32Bit(aref.flags);
    return this.sendRequest(helper);
  }

  getObjectFromName(name, op) {
    let helper = this.Uint8ArrayHelper(Msg.GET_OBJECT_FROM_NAME);
    helper.pack16Bit(op);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  crrSignal(oid) {
    let helper = this.Uint8ArrayHelper(Msg.CRR_SIGNAL);
    helper.pack32Bit(oid.vid);
    helper.pack32Bit(oid.oix);
    return this.sendRequest(helper);
  }

  getOpwindMenu(name) {
    let helper = this.Uint8ArrayHelper(Msg.GET_OPWIND_MENU);
    helper.packString(name);
    return this.sendRequest(helper);
  }

  login(user, passwd) {
    let helper = this.Uint8ArrayHelper(Msg.CHECK_USER);
    helper.packString(user);
    helper.packString(passwd);
    return this.sendRequest(helper);
  }

  getMsg(value) {
    let helper = this.Uint8ArrayHelper(Msg.GET_MSG);
    helper.pack32Bit(value);
    return this.sendRequest(helper);
  }

  mhSync(sync) {
    let helper = this.Uint8ArrayHelper(Msg.MH_SYNC);
    helper.pack32Bit(sync);
    return this.sendRequest(helper);
  }

  mhAcknowledge(event_id) {
    let helper = this.Uint8ArrayHelper(Msg.MH_ACK);
    helper.pack32Bit(event_id.nix);
    helper.pack32Bit(event_id.idx);
    helper.packString(event_id.birthTime);
    return this.sendRequest(helper);
  }

  sendRequest(data) {
    return new Promise((resolve, reject) => {
      this.pending[this.next_id] = resolve;
      this.next_id++;
      this.ws.send(Uint8Array.from(data.buf));
    });
  }
}