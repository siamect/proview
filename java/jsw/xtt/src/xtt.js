"use strict";

/** Pwr start **/

function PwrtStatus(sts) {
  this.sts = sts;

  this.evenSts = function () { return (sts % 2 === 0); };
  this.oddSts = function () { return (sts % 2 == 1); };
  this.getSts = function () { return sts; };
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

function CdhrNumber(value, sts) {
  this.value = value;
  this.sts = sts;

  this.evenSts = function () { return (sts % 2 === 0); };
  this.oddSts = function () { return (sts % 2 == 1); };
  this.getSts = function () { return sts; };
}

function UserdataCbReturn() {
  this.userdata;
  this.row;
}

/** Start Cli **/
function CliTable(command, qualifier) {
  this.command = command;
  this.qualifier = qualifier;
}

var CliC = {
  SUCCESS: 1,
  SYNTAX_ERROR: 2,
  UNKNOWN_COMMAND: 4,
  QUALNOTFOUND: 6,
  VERB_VECT_SIZE: 5,

  STATE_INIT: 0,
  STATE_VERB: 1,
  STATE_QUAL: 2,
  STATE_QUALVALUE: 3,
  STATE_SPACE: 4,
  STATE_EQUAL: 5,
  STATE_ERROR: 6,
  STATE_QUOTE_VERB: 7,
  STATE_QUOTE_QUALVALUE: 8,
  STATE_QUALVALUE_EXACT: 9,
  STATE_VERB_EXACT: 10,
  TAB: '	',
  SPACE: ' '
};

function Cli(cliTable) {
  this.verb = new Array(CliC.VERB_VECT_SIZE);
  this.verbCount = 0;
  this.qualifier = new Array(30);
  this.qualValue = new Array(30);
  this.qualifierCount = 0;
  this.status;
  this.cliTableIndex;
  this.cliQualifierIndex = new Array(30);
  this.configuredVerbs;
  this.cliTable = cliTable;

  /**
   * Return the status of the last operation as a string.
   * @return The status of the last operation.
   */
  this.getStsString = function () {
    switch (this.status) {
      case CliC.SUCCESS: return "Success";
      case CliC.SYNTAX_ERROR: return "Syntax error";
      case CliC.UNKNOWN_COMMAND: return "Unknown command verb";
      case CliC.QUALNOTFOUND: return "Unknown qualifier";
      default: return "Unknown command interpreter error";
    }
  };
  /**
   * Check if status of last operation is even. An error or warning
   * will result in an even status.
   * @return Returns true if status of last operation is even.
   */
  this.evenSts = function () {
    return (this.status % 2 === 0);
  };

  /**
   * Check if status of last operation is odd. A success or information
   * will result in an odd status.
   * @return Returns true if status of last operation is odd.
   */
  this.oddSts = function () {
    return (this.status % 2 !== 0);
  };

  /**
   * Parse a command line and detect verbs and qualifiers.
   * @param cmd Command line.
   */
  this.parse = function (cmd) {
    // Parse command string

    var state = CliC.STATE_INIT;
    var start_pos = 0;
    this.status = CliC.SUCCESS;
    this.verbCount = 0;
    this.qualifierCount = 0;
    var i;
    for (i = 0; i < cmd.length; i++) {
      var c = cmd.charAt(i);
      switch (state) {
        case CliC.STATE_INIT:
          if (c == CliC.SPACE || c == CliC.TAB)
            break;
          else {
            state = CliC.STATE_VERB;
            start_pos = i;
          }
          break;
        case CliC.STATE_SPACE:
          if (c == CliC.SPACE || c == CliC.TAB)
            break;
          if (c == '/') {
            state = CliC.STATE_QUAL;
            start_pos = i;
          } else if (c == '=') {
            if (this.qualifierCount === 0) {
              state = CliC.STATE_ERROR;
              this.status = CliC.SYNTAX_ERROR;
              break;
            }
            state = CliC.STATE_EQUAL;
          } else if (c == '"') {
            state = CliC.STATE_QUOTE_VERB;
            break;
          } else {
            state = CliC.STATE_VERB;
            start_pos = i;
          }
          break;
        case CliC.STATE_VERB:
          if (c == CliC.SPACE || c == CliC.TAB) {
            if (this.verbCount == CliC.VERB_VECT_SIZE) {
              state = CliC.STATE_ERROR;
              break;
            }
            if (this.verbCount === 0)
              this.verb[this.verbCount++] = cmd.substring(start_pos, i).toUpperCase();
            else
              this.verb[this.verbCount++] = cmd.substring(start_pos, i);
            state = CliC.STATE_SPACE;
          } else if (c == '/') {
            if (this.verbCount == CliC.VERB_VECT_SIZE) {
              state = CliC.STATE_ERROR;
              break;
            }
            this.verb[this.verbCount++] = cmd.substring(start_pos, i).toUpperCase();
            state = CliC.STATE_QUAL;
            start_pos = i;
          }
          break;
        case CliC.STATE_VERB_EXACT:
          if (c == '"') {
            if (this.verbCount == CliC.VERB_VECT_SIZE) {
              state = CliC.STATE_ERROR;
              break;
            }
            this.verb[this.verbCount++] = cmd.substring(start_pos, i);
            state = CliC.STATE_SPACE;
          }
          break;
        case CliC.STATE_QUAL:
          if (c == CliC.SPACE || c == CliC.TAB) {
            this.qualifier[this.qualifierCount++] = cmd.substring(start_pos, i).toUpperCase();
            state = CliC.STATE_SPACE;
          } else if (c == '=') {
            this.qualifier[this.qualifierCount++] = cmd.substring(start_pos, i).toUpperCase();
            state = CliC.STATE_EQUAL;
          } else if (c == '/') {
            this.qualifier[this.qualifierCount++] = cmd.substring(start_pos, i).toUpperCase();
            state = CliC.STATE_QUAL;
            start_pos = i;
          }
          break;
        case CliC.STATE_QUALVALUE:
          if (c == CliC.SPACE || c == CliC.TAB) {
            this.qualValue[this.qualifierCount - 1] = cmd.substring(start_pos, i);
            state = CliC.STATE_SPACE;
          } else if (c == '/') {
            this.qualValue[this.qualifierCount - 1] = cmd.substring(start_pos, i);
            state = CliC.STATE_QUAL;
            start_pos = i;
          }
          break;
        case CliC.STATE_QUALVALUE_EXACT:
          if (c == '"') {
            this.qualValue[this.qualifierCount - 1] = cmd.substring(start_pos, i);
            state = CliC.STATE_SPACE;
          }
          break;
        case CliC.STATE_QUOTE_VERB:
          state = CliC.STATE_VERB_EXACT;
          start_pos = i;
          break;
        case CliC.STATE_QUOTE_QUALVALUE:
          state = CliC.STATE_QUALVALUE_EXACT;
          start_pos = i;
          break;
        case CliC.STATE_EQUAL:
          if (c == CliC.SPACE || c == CliC.TAB)
            break;
          if (c == '"') {
            state = CliC.STATE_QUOTE_QUALVALUE;
          } else {
            state = CliC.STATE_QUALVALUE;
            start_pos = i;
          }
          break;

      }
      if (state == CliC.STATE_ERROR)
        break;
    }
    switch (state) {
      case CliC.STATE_INIT:
      case CliC.STATE_ERROR:
        return "";
      case CliC.STATE_VERB:
        if (this.verbCount == CliC.VERB_VECT_SIZE) {
          state = CliC.STATE_ERROR;
          break;
        }
        if (this.verbCount === 0)
          this.verb[this.verbCount++] = cmd.substring(start_pos, i).toUpperCase();
        else
          this.verb[this.verbCount++] = cmd.substring(start_pos, i);
        break;
      case CliC.STATE_VERB_EXACT:
        if (this.verbCount == CliC.VERB_VECT_SIZE) {
          state = CliC.STATE_ERROR;
          break;
        }
        this.verb[this.verbCount++] = cmd.substring(start_pos, i);
        break;
      case CliC.STATE_QUAL:
        this.qualifier[this.qualifierCount++] = cmd.substring(start_pos, i).toUpperCase();
        break;
      case CliC.STATE_QUALVALUE:
        this.qualValue[this.qualifierCount - 1] = cmd.substring(start_pos, i);
        break;
      case CliC.STATE_QUALVALUE_EXACT:
        this.qualValue[this.qualifierCount - 1] = cmd.substring(start_pos, i);
        break;
      case CliC.STATE_QUOTE_VERB:
      case CliC.STATE_QUOTE_QUALVALUE:
      case CliC.STATE_EQUAL:
        this.status = CliC.SYNTAX_ERROR;
        return "";
    }

    if (this.verbCount === 0) {
      this.status = CliC.SYNTAX_ERROR;
      return "";
    }

    // for ( i = 0; i < this.verbCount; i++)
    //   console.log("verb: \"" + this.verb[i] + "\"");
    // for ( i = 0; i < this.qualifierCount; i++)
    //  console.log("qual: \"" + this.qualifier[i] + "\" , \"" + this.qualValue[i] + "\"");

    // Identify verbs and qualifiers
    var found = false;
    for (i = 0; i < this.cliTable.length; i++) {
      if (this.verb[0].length > this.cliTable[i].command.length)
        continue;
      if (this.verb[0] == (this.cliTable[i].command.substring(0, this.verb[0].length))) {
        this.verb[0] = this.cliTable[i].command;
        found = true;
        break;
      }
    }
    if (!found) {
      this.status = CliC.UNKNOWN_COMMAND;
      return "";
    }
    this.cliTableIndex = i;

    this.configuredVerbs = 0;
    if (this.cliTable[this.cliTableIndex].qualifier !== null) {
      for (i = 0; i < this.cliTable[this.cliTableIndex].qualifier.length; i++) {
        if (this.cliTable[this.cliTableIndex].qualifier[i] === null)
          break;
        if (this.cliTable[this.cliTableIndex].qualifier[i] == ("cli_arg1"))
          this.configuredVerbs++;
        if (this.cliTable[this.cliTableIndex].qualifier[i] == ("cli_arg2"))
          this.configuredVerbs++;
        if (this.cliTable[this.cliTableIndex].qualifier[i] == ("cli_arg3"))
          this.configuredVerbs++;
        if (this.cliTable[this.cliTableIndex].qualifier[i] == ("cli_arg4"))
          this.configuredVerbs++;
        if (this.cliTable[this.cliTableIndex].qualifier[i] == ("cli_arg5"))
          this.configuredVerbs++;
      }

      for (var j = 0; j < this.qualifierCount; j++) {
        found = false;
        for (i = 0; i < this.cliTable[this.cliTableIndex].qualifier.length; i++) {
          if (this.cliTable[this.cliTableIndex].qualifier[i] === null)
            break;
          if (this.qualifier[j].length > this.cliTable[this.cliTableIndex].qualifier[i].length)
            continue;
          if (this.qualifier[j] == (this.cliTable[this.cliTableIndex].qualifier[i].substring(0, this.qualifier[j].length))) {
            this.cliQualifierIndex[j] = i;
            found = true;
            this.qualifier[j] = this.cliTable[this.cliTableIndex].qualifier[i];
          }
        }
        if (!found) {
          this.status = CliC.QUALNOTFOUND;
          return "";
        }
      }
    } else if (this.qualifierCount > 0) {
      this.status = CliC.QUALNOTFOUND;
      return "";
    }
    return this.verb[0];
  };

  /**
   * Check if a qualifier was present in the last parse operation.
   * @param qual Qualifier to check.
   * @return Returns true if the qualifier is present.
   */
  this.qualifierFound = function (qual) {
    if (qual == ("cli_arg1")) {
      if (this.verbCount < 2 || this.configuredVerbs < 1)
        return false;
      return true;
    }
    if (qual == ("cli_arg2")) {
      if (this.verbCount < 3 || this.configuredVerbs < 2)
        return false;
      return true;
    }
    if (qual == ("cli_arg3")) {
      if (this.verbCount < 4 || this.configuredVerbs < 3)
        return false;
      return true;
    }
    if (qual == ("cli_arg4")) {
      if (this.verbCount < 5 || this.configuredVerbs < 4)
        return false;
      return true;
    }
    for (var i = 0; i < this.qualifierCount; i++) {
      if (qual == (this.qualifier[i]))
        return true;
    }
    return false;
  };

  /**
   * Get the value of a qualifier in the last parse operation.
   * @param qual Qualifier to check.
   * @return Returns the value of the qualifier.
   */
  this.getQualValue = function (qual) {
    if (qual == ("cli_arg1")) {
      if (this.verbCount < 2 || this.configuredVerbs < 1)
        return "";
      return this.verb[1];
    }
    if (qual == ("cli_arg2")) {
      if (this.verbCount < 3 || this.configuredVerbs < 2)
        return "";
      return this.verb[2];
    }
    if (qual == ("cli_arg3")) {
      if (this.verbCount < 4 || this.configuredVerbs < 3)
        return this.verb[3];
    }
    if (qual == ("cli_arg4")) {
      if (this.verbCount < 5 || this.configuredVerbs < 4)
        return "";
      return this.verb[4];
    }
    for (var i = 0; i < this.qualifierCount; i++) {
      if (qual == (this.qualifier[i])) {
        if (this.qualValue[i] === null)
          return "";
        else
          return this.qualValue[i];
      }
    }
    return "";
  };
}
/** End Cli **/

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
  GET_OP_METHOD_PLC: 2
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
    GET_OPWIND_MENU: 64
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
      } else {
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
                      value = dv.getUint8(j);
                      j += 1;
                      break;
                    case Pwr.eType_Float32:
                      if (sub.elements <= 1) {
                        value = dv.getFloat32(j);
                        j += 4;
                      } else {
                        if (esize != sub.elements * 4)
                          console.log("Subscription size error", esize, sub.elements, eid);
                        value = new Array(sub.elements);
                        for (var k = 0; k < sub.elements; k++) {
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
                    case Pwr.eType_Mask:
                    case Pwr.eType_Enum:
                      value = dv.getInt32(j);
                      j += 4;
                      break;
                    case Pwr.eType_String:
                    case Pwr.eType_Time:
                    case Pwr.eType_DeltaTime:
                    case Pwr.eType_AttrRef:
                    case Pwr.eType_Objid:
                      var nsize = dv.getInt16(j);
                      j += 2;
                      var iarr = new Uint8Array(nsize);
                      for (var k = 0; k < nsize; k++) {
                        iarr[k] = dv.getUint8(j++);
                      }
                      value = String.fromCharCode.apply(null, iarr);
                      break;
                    default: break;
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
            case Msg.GET_OBJECT: {
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

                console.log("Opmenu", result.title, result.text, result.enable_language,
                  result.enable_login, result.enable_alarmlist, result.enable_eventlog,
                  result.enable_navigator, result.disable_help, result.disable_proview, result.language, bsize);
                console.log("Opmenu buttons size", bsize);
                for (var i = 0; i < bsize; i++) {
                  var button = new MenuButton();
                  button.type = dv.getUint32(j);
                  j += 4;
                  nsize = dv.getUint16(j);
                  j += 2;
                  console.log("text size", button.type, nsize);
                  iarr = new Uint8Array(nsize);
                  for (var k = 0; k < nsize; k++) {
                    console.log("Button.text", j);
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
                  console.log("Button", button.text, button.name, button.url);
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
    } else {
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
    console.log("Sending UnrefObjectInfo", this.next_id, size, k, refid);
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
  img: [null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null],
  pending: [null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null],
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
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAPElEQVQokWP4jwYYGBjgGKscNkXINLo4hgZsipFto44GbJi6NlDfSehhjUxjyGMVxQMYcJmEVwPB5ICEAdcbY6vf9TVAAAAAAElFTkSuQmCC']

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
    } else {
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
    } else {
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
      case 0: tsize = 8; break;
      case 1: tsize = 10; break;
      case 2: tsize = 12; break;
      case 3: tsize = 14; break;
      case 4: tsize = 14; break;
      case 5: tsize = 18; break;
      case 6: tsize = 18; break;
      case 7: tsize = 18; break;
      default: tsize = idx * 3;
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
    } else {
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
  }

  this.connect = function () {
    for (var i = 0; i < this.a.size(); i++)
      this.a.get(i).connect();
  }
  this.scan = function () {
    console.log("ctx scan", this.a.size());
    for (var i = 0; i < this.a.size(); i++)
      this.a.get(i).scan();
  }
  this.set_nodraw = function () {
    this.nodraw++;
  }
  this.reset_nodraw = function () {
    this.nodraw--;
  }

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
  }

  this.set_select = function (select) {
    for (var i = 0; i < this.a.size(); i++) {
      if (this.a.get(i) instanceof PlowNode)
        this.a.get(i).set_select(select);
    }
  }
  this.set_invert = function (invert) {
    for (var i = 0; i < this.a.size(); i++) {
      if (this.a.get(i) instanceof PlowNode)
        this.a.get(i).set_invert(invert);
    }
  }

  this.get_select = function () {
    return this.select_object;
  }
  this.insert = function (n, dest) {
    this.a.add(n);
  }
  this.insertNode = function (n, destination, destCode) {
    this.a.insertNode(n, destination, destCode);
  }
  this.insert_nc = function (nc) {
    this.a_nc.add(nc);
  }
  this.configure = function () {
    this.a.configure();
    this.a.set_borders();

    var height = this.a.a.length * 1.0 * this.zoom_factor;
    this.gdraw.canvas.height = height;
  }
  this.get_parent_object = function (o) {
    return this.a.get_parent_object(o);
  }
  this.get_next_object = function (o) {
    return this.a.get_next_object(o);
  }
  this.get_previous_object = function (o) {
    return this.a.get_previous_object(o);
  }
  this.close_node = function (o) {
    this.a.close_node(o);
  }
  this.is_visible = function (o) {
    if ((o.y_high * this.zoom_factor <= window.pageYOffset + window.innerHeight - this.gdraw.offset_top) &&
      (o.y_low * this.zoom_factor >= window.pageYOffset - this.gdraw.offset_top))
      return true;
    return false;
  }
  this.scroll = function (y, factor) {
    window.scrollTo(window.scrollX, y * this.zoom_factor - window.innerHeight * factor + this.gdraw.offset_top)
  }
}
/** End Plow **/

/** Start Xtt **/
function XttOpenChildrenData(node, open_next) {
  this.node = node;
  this.open_next = open_next;
}

function Xtt() {
  this.ctx = null;
  this.ncObject = null;
  this.scan_update;
  this.priv = 0;
  var self = this;

  this.init = function () {
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
        xtt.ctx.event_handler(Plow.eEvent_MB1ClickShift, x, y);
      else
        xtt.ctx.event_handler(Plow.eEvent_MB1Click, x, y);
    });
    document.addEventListener("keydown", function (event) {
      if (event.keyCode == 40) {
        self.ctx.event_handler(Plow.eEvent_Key_Down);
        event.preventDefault();
      } else if (event.keyCode == 39) {
        if (event.shiftKey)
          self.ctx.event_handler(Plow.eEvent_Key_ShiftRight);
        else
          self.ctx.event_handler(Plow.eEvent_Key_Right);
        event.preventDefault();
      } else if (event.keyCode == 37) {
        self.ctx.event_handler(Plow.eEvent_Key_Left);
        event.preventDefault();
      } else if (event.keyCode == 38) {
        self.ctx.event_handler(Plow.eEvent_Key_Up);
        event.preventDefault();
      } else if (event.keyCode == 82) {
        if (event.ctrlKey)
          self.ctx.event_handler(Plow.eEvent_Key_CtrlR);
        event.preventDefault();
      } else if (event.keyCode == 76) {
        if (event.ctrlKey)
          self.ctx.event_handler(Plow.eEvent_Key_CtrlL);
        event.preventDefault();
      } else if (event.keyCode == 71) {
        if (event.ctrlKey)
          self.ctx.event_handler(Plow.eEvent_Key_CtrlG);
        event.preventDefault();
      }
    });
    document.getElementById("toolitem1").addEventListener("click", function (event) {
    });
    document.getElementById("toolitem2").addEventListener("click", function (event) {
      console.log("toolitem1 event");
      var o = self.ctx.get_select();
      var newwindow = window.open("", "_blank");
      self.ctx.gdh.getObject(o.userdata.objid, GdhOp.GET_OP_METHOD_PLC, self.open_plc_cb, newwindow);
      console.log("toolitem2 event");
    });
    document.getElementById("toolitem3").addEventListener("click", function (event) {
      console.log("toolitem3 event");
    });
    document.getElementById("toolitem4").addEventListener("click", function (event) {
      console.log("toolitem4 event");
      var o = self.ctx.get_select();
      if (o == null)
        return;
      var item = o.userdata;
      item.open_attributes(self);
    });
    document.getElementById("toolitem5").addEventListener("click", function (event) {
      var o = self.ctx.get_select();
      self.ctx.gdh.crrSignal(o.userdata.objid, self.open_crr_cb, o);
      console.log("toolitem5 event");
    });
    document.getElementById("toolitem6").addEventListener("click", function (event) {
      console.log("toolitem6 event");
    });
    document.getElementById("toolitem7").addEventListener("click", function (event) {
      console.log("toolitem7 event");
      var o = self.ctx.get_select();
      if (o == null)
        return;
      if (o.userdata instanceof XttItemObject) {
        var newwindow = window.open("", "_blank");
        self.ctx.gdh.getObject(o.userdata.objid, GdhOp.GET_OP_SELF, self.open_helpclass_cb, newwindow);
      }
    });

    window.addEventListener("storage", function (event) {
      if (event.newValue == "")
        return;
      localStorage.setItem("XttMethodNavigator", "");
      self.display_object(event.newValue);
    });

  }

  this.is_authorized = function (access) {
    return (this.priv & access) ? true : false;
  }

  this.gdh_init_cb = function () {
    if (self.priv == null)
      self.ctx.gdh.login("", "", self.login_cb, self);

    var oid = new PwrtObjid(0, 0);
    self.ctx.gdh.getAllXttChildren(oid, self.open_children_cb, new XttOpenChildrenData(null, null));

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

  this.open_children_cb = function (id, data, sts, result) {
    self.ctx.set_nodraw();
    for (var i = 0; i < result.length; i++) {
      if (data.node == null) {
        result[i].full_name = result[i].name;
        new XttItemObject(self, result[i], null, Plow.DEST_AFTER);
      } else {
        result[i].full_name = data.node.userdata.full_name + "-" + result[i].name;
        new XttItemObject(self, result[i], data.node, Plow.DEST_INTOLAST);
      }
    }
    self.ctx.configure();

    if (data.open_next != null) {
      if (data.open_next.length == 0) {
        self.ctx.reset_nodraw();
        return;
      }
      var child = self.ctx.a.get_first_child(data.node);
      while (child != null) {
        if (child.userdata.name == data.open_next[0]) {
          if (data.open_next.length == 1) {
            child.set_select(true);
            child.set_invert(true);
            if (!self.ctx.is_visible(child))
              self.ctx.scroll(child.y_low, 0.50);
            window.focus(); // Doesn't work
          } else {
            data.open_next.splice(0, 1);
            child.userdata.open_children(self, data.open_next);
          }
          break;
        }
        child = self.ctx.a.get_next_sibling(child);
      }
    }

    self.ctx.reset_nodraw();
    self.ctx.draw();
  }

  this.open_attributes_cb = function (id, node, sts, result) {
    self.ctx.set_nodraw();
    for (var i = 0; i < result.length; i++) {
      result[i].objid = node.userdata.objid;
      result[i].full_name = node.userdata.full_name + "." + result[i].name;
      if ((result[i].flags & Pwr.mAdef_array) != 0)
        new XttItemAttrArray(self, result[i], node, Plow.DEST_INTOLAST);
      else if ((result[i].flags & Pwr.mAdef_class) != 0)
        new XttItemAttrObject(self, result[i], node, Plow.DEST_INTOLAST);
      else
        new XttItemAttr(self, result[i], node, Plow.DEST_INTOLAST);
    }

    self.ctx.configure();
    self.ctx.reset_nodraw();
    self.ctx.draw();
  }

  this.open_plc_cb = function (id, data, sts, result) {
    if ((sts & 1) != 0) {
      data.location.href = "flow.html?vid=" + result.objid.vid + "&oix=" + result.objid.oix;
      data.document.title = "Trace " + result.fullname;
    } else
      data.document.write("Error status " + sts);
  }

  this.open_objectgraph_cb = function (id, data, sts, result) {
    if ((sts & 1) != 0) {
      var graphname = "pwr_c_" + result.classname.toLowerCase();
      data.location.href = "ge.html?graph=" + graphname + "&instance=" + result.fullname;
      data.document.title = graphname + " " + result.fullname;
    } else
      data.document.write("Error status " + sts);
  }

  this.open_crr_cb = function (id, node, sts, crrdata) {
    if ((sts & 1) == 0) {
      return;
    }
    node.userdata.open_crossreferences(self, crrdata);
  }

  this.open_helpclass_cb = function (id, data, sts, result) {
    if ((sts & 1) != 0) {
      var url = location.protocol + "//" + location.host + "/pwr_doc/en_us/orm/pwrb_" + result.classname.toLowerCase() + ".html";
      data.location.href = url;
    } else
      data.document.write("Error status " + sts);
  }

  this.plow_event = function (event, object, x, y) {
    var item = null;
    if (object != null)
      item = object.get_userdata();

    switch (event) {
      case Plow.eEvent_ObjectDeleted:
        console.log("Object deleted cb");
        if (object.userdata instanceof XttItemAttr) {
          console.log("Object deleted cb XttItemAttr");
          object.userdata.scan_close(self);
        }
        break;
      case Plow.eEvent_MB1Click:
        if (object.in_icon(x, y)) {
          item.open_children(self, null);
        } else {
          if (object.select) {
            object.set_select(false);
            object.set_invert(false);
          } else {
            self.ctx.set_select(false);
            object.set_select(true);
            self.ctx.set_invert(false);
            object.set_invert(true);
            self.ctx.draw();
          }
        }
        break;
      case Plow.eEvent_MB1ClickShift:
        if (object.in_icon(x, y)) {
          item.open_attributes(self);
        }
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
        } else {
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
        var o = self.ctx.get_select();
        if (o != null) {
          item = o.userdata;
          item.open_children(self, null);
        }
        break;
      }
      case Plow.eEvent_Key_Left: {
        var o = self.ctx.get_select();
        if (o != null) {
          item = o.userdata;
          item.close(self);
        }
        break;
      }
      case Plow.eEvent_Key_ShiftRight: {
        var o = self.ctx.get_select();
        if (o != null) {
          item = o.userdata;
          item.open_attributes(self);
        }
        break;
      }
      case Plow.eEvent_Key_CtrlR: {
        var o = self.ctx.get_select();
        if (o != null)
          self.ctx.gdh.crrSignal(o.userdata.objid, self.open_crr_cb, o);
        break;
      }
      case Plow.eEvent_Key_CtrlL: {
        var o = self.ctx.get_select();
        if (o.userdata instanceof XttItemObject) {
          var newwindow = window.open("", "_blank");
          self.ctx.gdh.getObject(o.userdata.objid, GdhOp.GET_OP_METHOD_PLC, self.open_plc_cb, newwindow);
        } else if (o.userdata instanceof XttItemCrr) {
          var idx = o.userdata.name.lastIndexOf('-');
          var ostring = "";
          if (idx != -1) {
            ostring = "&obj=" + o.userdata.name.substring(idx + 1);
          }
          window.open("flow.html?vid=" + o.userdata.objid.vid + "&oix=" + o.userdata.objid.oix + ostring);
        }
        break;
      }
      case Plow.eEvent_Key_CtrlG: {
        var o = self.ctx.get_select();
        if (o.userdata instanceof XttItemObject) {
          var newwindow = window.open("", "_blank");
          console.log("CtrlG", newwindow);
          self.ctx.gdh.getObject(o.userdata.objid, GdhOp.GET_OP_METHOD_PLC, self.open_objectgraph_cb, newwindow);
        }
        break;
      }
    }
  }

  this.createNodeClasses = function () {
    var r1 = new PlowRect(this.ctx, 0, 0, 50, 1.0, Plow.COLOR_WHITE, Plow.COLOR_WHITE, true, false);
    var a1 = new PlowAnnot(this.ctx, 3, 0.9, 4, Plow.COLOR_BLACK, Plow.NEXT_RELATIVE_POSITION, 0);
    var a11 = new PlowAnnot(this.ctx, 7.5, 0.9, 4, Plow.COLOR_BLACK, Plow.RELATIVE_POSITION, 1);
    var a12 = new PlowAnnot(this.ctx, 12, 0.9, 4, Plow.COLOR_BLACK, Plow.RELATIVE_POSITION, 2);
    var p1 = new PlowAnnotPixmap(this.ctx, 0.4, 0.2, 0);

    this.ncObject = new PlowNodeClass(this.ctx);
    this.ncObject.insert(r1);
    this.ncObject.insert(a1);
    this.ncObject.insert(a11);
    this.ncObject.insert(a12);
    this.ncObject.insert(p1);
    this.ctx.insert_nc(this.ncObject);
  }

  this.methods_init = function () {
    localStorage.setItem("XttMethodNavigator", "");
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

  this.display_object = function (name) {
    var i = name.indexOf('.');
    if (i != -1)
      name = name.substring(0, i);
    var path = name.split('-');
    var idx = 0;

    this.collapse();
    for (var j = idx; j < this.ctx.a.size(); j++) {
      if (this.ctx.a.get(j).userdata.name == path[0]) {
        if (j == this.ctx.a.size() - 1) {
          var node = this.ctx.a.get(j);
          node.set_select(true);
          node.set_invert(true);
          if (!this.ctx.is_visible(node))
            this.ctx.scroll(node.y_low, 0.50);
          window.focus(); // Doesn't work
        } else {
          path.splice(0, 1);
          this.ctx.a.get(j).userdata.open_children(this, path);
        }
        break;
      }
    }
  }

  this.trace_cyclic = function () {
    self.ctx.gdh.getRefObjectInfoAll(self.trace_scan);
  };

  this.trace_scan = function (id, sts) {
    self.scan_update = false;
    for (var i = 0; i < self.ctx.a.size(); i++) {
      var item = self.ctx.a.get(i).userdata;
      if (item instanceof XttItemAttr) {
        item.scan(self);
      }
    }
    if (self.scan_update)
      self.ctx.draw();
    self.timer = setTimeout(self.trace_cyclic, 1000);
  };

  this.openValueInputDialog = function (item, text) {
    console.log("priv acc", this.priv, Pwr.mAccess_RtWrite | Pwr.mAccess_System);
    if (this.is_authorized(Pwr.mAccess_RtWrite | Pwr.mAccess_System)) {
      var value = prompt(text, "");
      if (value !== null)
        item.set_value(this, value);
    } else
      alert("Not authorized for this operation");
  };
}

function XttItemObject(xtt, object_info, destination, destCode) {
  this.objid = object_info.objid;
  this.cid = object_info.cid;
  this.name = object_info.name;
  this.full_name = object_info.full_name;
  this.has_children = object_info.has_children;
  this.node = new PlowNode(xtt.ctx, xtt.ncObject, 0);
  this.node.set_userdata(this);
  this.node.set_annotation(0, object_info.name);
  this.node.set_annotation(1, object_info.classname);
  this.node.set_annotation(2, object_info.description);
  this.node.set_annotation_pixmap(0, Bitmaps.map);
  xtt.ctx.insertNode(this.node, destination, destCode);
  if (object_info.has_children)
    this.node.set_annotation_pixmap(0, Bitmaps.map);
  else
    this.node.set_annotation_pixmap(0, Bitmaps.leaf);

  this.open_children = function (xtt, open_next) {
    if (this.node.node_open != 0)
      this.close(xtt);
    else if (!this.has_children)
      this.open_attributes(xtt);
    else {
      xtt.ctx.gdh.getAllXttChildren(this.objid, xtt.open_children_cb, new XttOpenChildrenData(this.node, open_next));
      this.node.node_open |= Plow.OPEN_CHILDREN;
      this.node.set_annotation_pixmap(0, Bitmaps.openmap);
    }
  }

  this.open_attributes = function (xtt) {
    if (this.node.node_open != 0)
      this.close(xtt);
    else {
      xtt.ctx.gdh.getAllClassAttributes(this.cid, this.objid, xtt.open_attributes_cb, this.node)

      this.node.node_open |= Plow.OPEN_ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    }
  }

  this.open_crossreferences = function (xtt, crrdata) {
    if (this.node.node_open != 0)
      this.close(xtt);
    else {
      for (var i = 0; i < crrdata.length; i++) {
        new XttItemCrr(xtt, crrdata[i], this.node, Plow.DEST_INTOLAST);
      }

      this.node.node_open |= Plow.OPEN_CROSSREFERENCES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    }
  }

  this.close = function (xtt) {
    if (this.node.node_open & Plow.OPEN_CHILDREN) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Plow.OPEN_CHILDREN;
      this.node.set_annotation_pixmap(0, Bitmaps.map);
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else if (this.node.node_open & Plow.OPEN_ATTRIBUTES) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Plow.OPEN_ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else if (this.node.node_open & Plow.OPEN_CROSSREFERENCES) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Plow.OPEN_CROSSREFERENCES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else {
      var parent = xtt.ctx.get_parent_object(this.node);
      if (parent != null) {
        parent.userdata.close(xtt);
        parent.set_select(true);
        parent.set_invert(true);
      }
    }
  }

}

function XttItemAttr(xtt, info, destination, destCode) {
  this.name = info.name;
  this.objid = info.objid;
  this.full_name = info.full_name;
  this.type = info.type;
  this.flags = info.flags;
  this.size = info.size;
  this.refid;
  this.firstScan = true;
  this.old_value;
  this.node = new PlowNode(xtt.ctx, xtt.ncObject, 0);
  this.node.set_userdata(this);
  this.node.set_annotation(0, this.name);
  this.node.set_annotation_pixmap(0, Bitmaps.attr);
  xtt.ctx.insertNode(this.node, destination, destCode);

  console.log("Attr refObjectInfo", this.full_name, info.type);
  this.refid = xtt.ctx.gdh.refObjectInfo(this.full_name, info.type, 1);

  this.open_children = function (xtt, open_next) {
    xtt.openValueInputDialog(this, "Enter value");
  };

  this.set_value = function (xtt, value) {
    var sts;

    switch (this.type) {
      case Pwr.eType_Float32:
        var inputValue = parseFloat(value.trim());
        sts = xtt.ctx.gdh.setObjectInfoFloat(this.full_name, inputValue);
        break;
      case Pwr.eType_Int8:
      case Pwr.eType_Int16:
      case Pwr.eType_Int32:
      case Pwr.eType_UInt8:
      case Pwr.eType_UInt16:
      case Pwr.eType_UInt32:
      case Pwr.eType_Status:
      case Pwr.eType_Mask:
      case Pwr.eType_Enum:
      case Pwr.eType_Boolean:
        var inputValue = parseInt(value.trim(), 10);
        sts = xtt.ctx.gdh.setObjectInfoInt(this.full_name, inputValue);
        break;
      case Pwr.eType_String:
      case Pwr.eType_Time:
      case Pwr.eType_DeltaTime:
      case Pwr.eType_AttrRef:
      case Pwr.eType_Objid:
        sts = xtt.ctx.gdh.setObjectInfoString(this.full_name, value);
        break;
      default:
        break;
    }
  };

  this.open_attributes = function (xtt) {
  }

  this.close = function (xtt) {
    var parent = xtt.ctx.get_parent_object(this.node);
    if (parent != null) {
      parent.userdata.close(xtt);
      parent.set_select(true);
      parent.set_invert(true);
    }
  }

  this.scan = function (xtt) {
    if (!this.refid)
      return;
    var value = xtt.ctx.gdh.getObjectRefInfo(this.refid);
    if (this.firstScan || value != this.old_value) {
      var value_str;

      switch (this.type) {
        case Pwr.eType_Float32:
          value_str = "" + value;
          break;
        case Pwr.eType_Boolean:
          value_str = value ? "1" : "0";
          break;
        case Pwr.eType_Int32:
        case Pwr.eType_Int16:
        case Pwr.eType_Int8:
        case Pwr.eType_UInt32:
        case Pwr.eType_UInt16:
        case Pwr.eType_UInt8:
          value_str = "" + value;
          break;
        case Pwr.eType_String:
        case Pwr.eType_AttrRef:
        case Pwr.eType_Objid:
          value_str = value;
          break;
        default:
          value_str = "" + value;
      }

      this.old_value = value;
      this.node.set_annotation(1, value_str);
      xtt.scan_update = true;
    }
    this.firstScan = false;
  }
  this.scan_close = function (xtt) {
    xtt.ctx.gdh.unrefObjectInfo(this.refid);
  };
}

function XttItemAttrArray(xtt, info, destination, destCode) {
  this.name = info.name;
  this.objid = info.objid;
  this.full_name = info.full_name;
  this.type = info.type;
  this.flags = info.flags;
  this.size = info.size;
  this.elements = info.elements;
  this.node = new PlowNode(xtt.ctx, xtt.ncObject, 0);
  this.node.set_userdata(this);
  this.node.set_annotation(0, this.name);
  this.node.set_annotation_pixmap(0, Bitmaps.attrarra);
  xtt.ctx.insertNode(this.node, destination, destCode);

  this.open_children = function (xtt, open_next) {
    this.open_attributes(xtt);
  }

  this.open_attributes = function (xtt) {
    var info = new AttributeInfo();

    info.objid = this.objid;
    info.type = this.type;
    info.flags = this.flags & ~Pwr.mAdef_array;
    info.size = this.size / this.elements;
    info.elements = 1;
    info.name = ""
    info.full_name = ""
    info.classname = ""

    xtt.ctx.set_nodraw();
    for (var i = 0; i < this.elements; i++) {
      info.name = this.name + "[" + i + "]";
      info.full_name = this.full_name + "[" + i + "]";
      console.log("AttrArray", info.name, info.type);
      if ((info.flags & Pwr.mAdef_array) != 0)
        new XttItemAttrArray(xtt, info, this.node, Plow.DEST_INTOLAST);
      else if ((info.flags & Pwr.mAdef_class) != 0)
        new XttItemAttrObject(xtt, info, this.node, Plow.DEST_INTOLAST);
      else
        new XttItemAttr(xtt, info, this.node, Plow.DEST_INTOLAST);
    }
    this.node.node_open |= Plow.OPEN_ATTRIBUTES;
    xtt.ctx.configure();
    xtt.ctx.reset_nodraw();
    xtt.ctx.draw();
  };

  this.close = function (xtt) {
    if (this.node.node_open & Plow.OPEN_ATTRIBUTES) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Plow.OPEN_ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else {
      var parent = xtt.ctx.get_parent_object(this.node);
      if (parent != null) {
        parent.userdata.close(xtt);
        parent.set_select(true);
        parent.set_invert(true);
      }
    }
  };

}

function XttItemAttrObject(xtt, info, destination, destCode) {
  this.name = info.name;
  this.classname = info.classname;
  this.objid = info.objid;
  this.full_name = info.full_name;
  this.cid = info.type;
  this.flags = info.flags;
  this.size = info.size;
  this.node = new PlowNode(xtt.ctx, xtt.ncObject, 0);
  this.node.set_userdata(this);
  this.node.set_annotation(0, this.name);
  this.node.set_annotation(1, this.classname);
  this.node.set_annotation_pixmap(0, Bitmaps.object);
  xtt.ctx.insertNode(this.node, destination, destCode);

  this.open_children = function (xtt, open_next) {
    this.open_attributes(xtt);
  }

  this.open_attributes = function (xtt) {
    if (this.node.node_open != 0)
      this.close(xtt);
    else {
      xtt.ctx.gdh.getAllClassAttributes(this.cid, this.objid, xtt.open_attributes_cb, this.node)

      this.node.node_open |= Plow.OPEN_ATTRIBUTES;
    }
  }

  this.close = function (xtt) {
    if (this.node.node_open & Plow.OPEN_ATTRIBUTES) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Plow.OPEN_ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else if (this.node.node_open & Plow.OPEN_CROSSREFERENCES) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Plow.OPEN_CROSSREFERENCES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else {
      var parent = xtt.ctx.get_parent_object(this.node);
      if (parent != null) {
        parent.userdata.close(xtt);
        parent.set_select(true);
        parent.set_invert(true);
      }
    }
  };
}

function XttItemCrr(xtt, info, destination, destCode) {
  this.name = info.name;
  this.classname = info.classname;
  this.objid = info.objid;
  this.type = info.type;
  this.node = new PlowNode(xtt.ctx, xtt.ncObject, 0);
  this.node.set_userdata(this);
  this.node.set_annotation(0, this.name);
  this.node.set_annotation(1, this.classname);

  switch (this.type) {
    case 0:
      this.node.set_annotation_pixmap(0, Bitmaps.crrread);
      break;
    case 1:
      this.node.set_annotation_pixmap(0, Bitmaps.crrwrite);
      break;
    case 2:
      this.node.set_annotation_pixmap(0, Bitmaps.crrwrite);
      // this.node.set_annotation_pixmap( 1, Bitmaps.crrread);
      break;
  }
  xtt.ctx.insertNode(this.node, destination, destCode);

  this.close = function (xtt) {
    var parent = xtt.ctx.get_parent_object(this.node);
    if (parent != null) {
      parent.userdata.close(xtt);
      parent.set_select(true);
      parent.set_invert(true);
    }
  }
}

var xtt = new Xtt();
xtt.init();

/** End Xtt **/
