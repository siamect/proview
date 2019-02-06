"use strict";

/** Start Pwr **/

function PwrtStatus( sts)
{
  this.sts = sts;
  
  this.evenSts = function() { return (sts % 2 === 0);};
  this.oddSts = function() { return (sts % 2 == 1);};
  this.getSts = function() { return sts;};
}

var Pwr = {
  eType_Boolean 	: 98305,
  eType_Float32        	: 98306,
  eType_Float64		: 98307,
  eType_Char		: 98308,
  eType_Int8		: 98309,
  eType_Int16		: 98310,
  eType_Int32		: 98311,
  eType_UInt8		: 98312,
  eType_UInt16		: 98313,
  eType_UInt32		: 98314,
  eType_Objid		: 98315,
  eType_Buffer		: 98316,
  eType_String		: 98317,
  eType_Enum		: 98318,
  eType_Struct		: 98319,
  eType_Mask		: 98320,
  eType_Array		: 98321,
  eType_Time		: 98322,
  eType_Text		: 98323,
  eType_AttrRef		: 98324,
  eType_UInt64		: 98325,
  eType_Int64		: 98326,
  eType_ClassId		: 98327,
  eType_TypeId		: 98328,
  eType_VolumeId 	: 98329,
  eType_ObjectIx 	: 98330,
  eType_RefId		: 98331,
  eType_DeltaTime 	: 98332,
  eType_Status		: 98333,
  eType_NetStatus 	: 98334,
  eType_CastId		: 98335,
  eType_ProString 	: 98336,
  eType_DisableAttr 	: 98337,
  eType_DataRef		: 98338,

  mPrv_RtRead		: 1 << 0,
  mPrv_RtWrite		: 1 << 1,
  mPrv_System		: 1 << 2,
  mPrv_Maintenance	: 1 << 3,
  mPrv_Process		: 1 << 4,
  mPrv_Instrument	: 1 << 5,
  mPrv_Operator1	: 1 << 6,
  mPrv_Operator2	: 1 << 7,
  mPrv_Operator3	: 1 << 8,
  mPrv_Operator4	: 1 << 9,
  mPrv_Operator5	: 1 << 10,
  mPrv_Operator6	: 1 << 11,
  mPrv_Operator7	: 1 << 12,
  mPrv_Operator8	: 1 << 13,
  mPrv_Operator9	: 1 << 14,
  mPrv_Operator10	: 1 << 15,
  mPrv_RtEventsAck     	: 1 << 18,
  mPrv_RtPlc		: 1 << 19,
  mPrv_RtNavigator     	: 1 << 20,
  mPrv_DevRead		: 1 << 21,
  mPrv_DevPlc		: 1 << 22,
  mPrv_DevConfig	: 1 << 23,
  mPrv_DevClass		: 1 << 24,
  mPrv_RtEventsBlock   	: 1 << 25,
  mPrv_Administrator   	: 1 << 26,
  mPrv_SevRead		: 1 << 27,
  mPrv_SevAdmin		: 1 << 28,

  mAccess_RtRead       	: 1 << 0,
  mAccess_RtWrite      	: 1 << 1,
  mAccess_System       	: 1 << 2,
  mAccess_Maintenance  	: 1 << 3,
  mAccess_Process      	: 1 << 4,
  mAccess_Instrument    : 1 << 5,
  mAccess_RtEventsBlock : 1 << 25,
  mAccess_RtEventsAck 	: 1 << 18,
  mAccess_RtPlc 	: 1 << 19,
  mAccess_RtNavigator 	: 1 << 20,
  mAccess_AllRt		: 1 << 2 |
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
  mAccess_AllOperators	: 1 << 6 |
						  1 << 7 |
						  1 << 8 |
						  1 << 9 |
						  1 << 10 |
						  1 << 11 |
						  1 << 12 |
						  1 << 13 |
						  1 << 14 |
						  1 << 15,
  mAccess_AllPwr	: ~0,

  mAdef_pointer		: 1,
  mAdef_array	        : 2,
  mAdef_backup	        : 4,
  mAdef_changelog	: 8,
  mAdef_state	        : 16,
  mAdef_const	        : 32,
  mAdef_rtvirtual	: 64,
  mAdef_devbodyref	: 128,
  mAdef_dynamic	        : 256,
  mAdef_publicwrite	: 512,
  mAdef_noedit	        : 1024,
  mAdef_invisible	: 2048,
  mAdef_refdirect	: 4096,
  mAdef_noinvert       	: 8192,
  mAdef_noremove       	: 16384,
  mAdef_rtdbref	        : 32768,
  mAdef_private	        : 65536,
  mAdef_class	        : 131072,
  mAdef_superclass      : 262144,
  mAdef_buffer	        : 524288,
  mAdef_nowbl       	: 1048576,
  mAdef_alwayswbl       : 2097152,
  mAdef_disableattr     : 4194304,
  mAdef_rthide	        : 8388608
};

var Pwrb = {
  mXttMethodsFlagsMask_IsConfigured : 1,

  mXttOpMethodsMask_OpenGraph      : 1,
  mXttOpMethodsMask_OpenObjectGraph : 2,
  mXttOpMethodsMask_OpenTrend      : 4,
  mXttOpMethodsMask_OpenHistory    : 8,
  mXttOpMethodsMask_OpenFast       : 16,
  mXttOpMethodsMask_Camera         : 32,
  mXttOpMethodsMask_HistEvent      : 64,
  mXttOpMethodsMask_BlockEvents    : 128,
  mXttOpMethodsMask_Help           : 256,
  mXttOpMethodsMask_Photo          : 512,
  mXttOpMethodsMask_Note           : 1024,
  mXttOpMethodsMask_ParentObjectGraph : 2048,

  mXttMntMethodsMask_OpenObject    : 1,
  mXttMntMethodsMask_OpenTrace     : 2,
  mXttMntMethodsMask_RtNavigator   : 4,
  mXttMntMethodsMask_OpenCrossref  : 8,
  mXttMntMethodsMask_HelpClass     : 16,
  mXttMntMethodsMask_DataSheet     : 32,
  mXttMntMethodsMask_CircuitDiagram : 64,
  mXttMntMethodsMask_Simulate      : 1 << 31
};

function PwrtObjid( vid, oix) {
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

function CdhrNumber( value, sts)
{
  this.value = value;
  this.sts = sts;
  
  this.evenSts = function() { return (sts % 2 === 0);};
  this.oddSts = function() { return (sts % 2 == 1);};
  this.getSts = function() { return sts;};
}

var CdhC = {
  cUserVolMin : (0 + (0 << 24) + (1 << 16) + (1 << 8) + 1),
  cUserVolMax : (0 + (0 << 24) + (254 << 16) + (254 << 8) + 254)
};

function UserdataCbReturn() {
  this.userdata;
  this.row;
}
/** End Pwr **/

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

function PendingData( func_cb, data) {
  this.func_cb = func_cb;
  this.data = data;
}

var GdhOp  = {
  GET_OP_SELF 			: 1,
  GET_OP_METHOD_PLC       	: 2,
  GET_OP_METHOD_OBJECTGRAPH  	: 3,
  GET_OP_METHOD_GRAPH  		: 4,
  GET_OP_METHOD_HELPCLASS      	: 5
};

function Gdh() {
  var Msg = {
    SET_OBJECT_INFO_BOOLEAN 	: 1,
    SET_OBJECT_INFO_FLOAT 	: 2,
    SET_OBJECT_INFO_INT        	: 3,
    SET_OBJECT_INFO_STRING 	: 4,
    GET_OBJECT_INFO_BOOLEAN 	: 5,
    GET_OBJECT_INFO_FLOAT 	: 6,
    GET_OBJECT_INFO_INT        	: 7,
    GET_OBJECT_INFO_STRING 	: 8,
    TOGGLE_OBJECT_INFO 		: 9,
    REF_OBJECT_INFO 		: 10,
    GET_OBJECT_REF_INFO_BOOLEAN	: 11,
    GET_OBJECT_REF_INFO_FLOAT 	: 12,
    GET_OBJECT_REF_INFO_INT 	: 13,
    GET_OBJECT_REF_INFO_STRING 	: 14,
    UNREF_OBJECT_INFO 		: 15,
    NAME_TO_OBJID 		: 16,
    OBJID_TO_NAME 		: 17,
    GET_ROOT_LIST 		: 18,
    GET_NEXT_OBJECT 		: 19,
    GET_CHILD 			: 20,
    GET_NEXT_SIBLING 		: 21,
    GET_OBJECT_CLASS 		: 22,
    GET_CLASS_LIST 		: 23,
    CLASS_ID_TO_OBJID 		: 24,
    GET_OBJECT_REF_INFO_ALL 	: 25,
    REF_OBJECT_INFO_LIST       	: 26,
    POLL 		       	: 27,
    STATISTICS 			: 28,
    CHECK_USER 			: 29,
    GET_NODE_OBJECT 		: 30,
    LOG_STRING 			: 31,
    UNREF_OBJECT_INFO_ALL 	: 32,
    CREATE_INSTANCE_FILE       	: 33,
    GET_ATTRIBUTE_CHAR 		: 34,
    GET_CLASS_ATTRIBUTE        	: 35,
    GET_ALL_CLASS_ATTRIBUTES 	: 36,
    GET_ALL_SIBLINGS 		: 37,
    GET_ALL_XTT_SIBLINGS       	: 38,
    GET_ALL_XTT_CHILDREN       	: 39,
    REF_OBJECT_INFO_VECTOR 	: 40,
    GET_SUBSCRIPTIONS 		: 41,
    CRR_SIGNAL 			: 42,
    CRR_OBJECT 			: 43,
    GET_PARENT 			: 44,
    GET_OBJECT_INFO_OBJID 	: 45,
    GET_OBJECT_REF_INFO_BOOLEAN_ARRAY : 46,
    GET_OBJECT_REF_INFO_FLOAT_ARRAY : 47,
    GET_OBJECT_REF_INFO_INT_ARRAY : 48,
    GET_OBJECT_REF_INFO_STRING_ARRAY : 49,
    GET_MSG 			: 50,
    GET_MSG_TEXT 	       	: 51,
    NAME_TO_ATTRREF 		: 52,
    ATTRREF_TO_NAME 		: 53,
    GET_ATTRREF_TID 		: 54,
    GET_SUPER_CLASS 		: 55,
    GET_ALL_CLASS_ATTRIBUTES_STRING : 56,
    GET_OBJECT_INFO_FLOAT_ARRAY	: 57,
    GET_OBJECT_INFO_INT_ARRAY 	: 58,
    GET_CIRCBUFF_INFO 		: 59,
    UPDATE_CIRCBUFF_INFO       	: 60,
    GET_ATTRIBUTE_FLAGS        	: 61,
    CLASSNAME_TO_ID 		: 62,
    GET_OBJECT	 		: 63,
    GET_OPWIND_MENU   		: 64,
    GET_OBJECT_FROM_NAME       	: 65,
    MH_SYNC			: 66,
    MH_ACK			: 67,
    GET_OBJECT_FROM_AREF       	: 68
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

  this.init = function() {
    if ( window.location.hostname === "")
      this.ws = new WebSocket( "ws:127.0.0.1:4448");
    else
      this.ws = new WebSocket( "ws://" + window.location.hostname + ":4448");
    this.ws.binaryType = "arraybuffer";
    this.ws.gdh = this;

    this.ws.onopen = function( e) {
      if ( this.gdh.open_cb !== null)
        this.gdh.open_cb();
    };

    this.ws.onclose = function() {
      if ( this.debug) console.log( "Socket closed");
      if ( this.gdh.close_cb !== null)
	this.gdh.close_cb();
    };

    this.ws.onmessage = function(e) {
      if ( typeof e.data == "string") {
	console.log("String message received", e, e.data);
      }
      else {
	if ( e.data instanceof ArrayBuffer) {
	  var dv = new DataView(e.data);
	  var type = dv.getUint8(0);
	  var id = dv.getUint32(1);
	  var sts = dv.getUint32(5);
	  
	  switch( type) {
	  case Msg.GET_OBJECT_INFO_BOOLEAN: {
	    if ( this.gdh.debug) console.log("GetObjectInfoBoolean received");
	    var value = dv.getUint8(9);
	    var func_cb = this.gdh.pending[id].func_cb;
	    func_cb( id, sts, value);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.GET_OBJECT_INFO_INT: {
	    if ( this.gdh.debug) console.log("GetObjectInfoInt received");
	    var value = dv.getUint32(9);
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts, value);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.GET_OBJECT_INFO_FLOAT: {
	    if ( this.gdh.debug) console.log("GetObjectInfoFloat received");
	    var value = dv.getFloat32(9);
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts, value);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.GET_OBJECT_INFO_FLOAT_ARRAY: {
	    if ( this.gdh.debug) console.log("GetObjectInfoFloatArray received");
	    var asize = dv.getInt32(9);
	    var value = new Array(asize);
	    k = 13;
	    for ( var i = 0; i < asize; i++) {
	      value[i] = dv.getFloat32(k);
	      k += 4;
	    }
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts, value);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.SET_OBJECT_INFO_BOOLEAN: {
	    if ( this.gdh.debug) console.log("SetObjectInfoBoolean received", id, sts);
	    break;
	  }
	  case Msg.SET_OBJECT_INFO_INT: {
	    if ( this.gdh.debug) console.log("SetObjectInfoInt received", id, sts);
	    break;
	  }
	  case Msg.SET_OBJECT_INFO_FLOAT: {
	    if ( this.gdh.debug) console.log("SetObjectInfoFloat received", id, sts);
	    break;
	  }
	  case Msg.SET_OBJECT_INFO_STRING: {
	    if ( this.gdh.debug) console.log("SetObjectInfoString received", id, sts);
	    break;
	  }
	  case Msg.TOGGLE_OBJECT_INFO: {
	    if ( this.gdh.debug) console.log("ToggleObjectInfo received", id, sts);
	    break;
	  }
	  case Msg.REF_OBJECT_INFO: {
	    if ( this.gdh.debug) console.log("RefObjectInfo received", id, sts);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.UNREF_OBJECT_INFO: {
	    if ( this.gdh.debug) console.log("UnrefObjectInfo received", id, sts);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.REF_OBJECT_INFO_LIST: {
	    if ( this.gdh.debug) console.log("RefObjectInfoList received", id, sts);
	    var func_cb = this.gdh.pending[id].func_cb;
	    func_cb( id, sts);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.GET_OBJECT_REF_INFO_ALL: {
	    var j = 9;
	    var size = dv.getUint32(j);
	    if ( this.gdh.debug) console.log("GetObjectRefInfoAll received", id, size);
	    j += 4;
	    for ( var i = 0; i < size; i++) {
	      var eid = dv.getUint32(j);
	      j += 4;
	      var esize = dv.getUint32(j);
	      j += 4;
	      var sub = this.gdh.sub[eid];
	      if ( typeof sub == 'undefined')
		j += esize;
	      else {
		var value;
		switch ( sub.type) {
		case Pwr.eType_Boolean:
		  if ( sub.elements <= 1) {
		    value = dv.getUint8(j);
		    j += 1;
		  }
		  else {
		    var elements = esize;
		    if ( elements != sub.elements)
		      console.log("Subscription size error", elements, sub.elements, eid);
		    value = new Array(elements);
		    for ( var k = 0; k < elements; k++) {
		      value[k] = dv.getUint8(j);
		      j += 1;
		    }
		  }
		  break;
		case Pwr.eType_Float32:
		  if ( sub.elements <= 1) {
		    value = dv.getFloat32(j);
		    j += 4;
		  }
		  else {
		    var elements = esize / 4;
		    if ( elements != sub.elements)
		      console.log("Subscription size error", elements, sub.elements, eid);
		    value = new Array(elements);
		    for ( var k = 0; k < elements; k++) {
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
		  if ( sub.elements <= 1) {
		    value = dv.getInt32(j);
		    j += 4;
		  }
		  else {
		    var elements = esize / 4;
		    if ( elements != sub.elements)
		      console.log("Subscription size error", elements, sub.elements, eid);
		    value = new Array(elements);
		    for ( var k = 0; k < elements; k++) {
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
		  if ( sub.elements <= 1) {
		    var nsize = dv.getInt16(j);		
		    j += 2;
		    var iarr = new Uint8Array( nsize);
		    for ( var k = 0; k < nsize; k++) {
		      iarr[k] = dv.getUint8(j++);
		    }
		    value = String.fromCharCode.apply( null, iarr);
		  }
		  else {
		    var elements = sub.elements;
		    if ( elements != sub.elements)
		      console.log("Subscription size error", elements, sub.elements, eid);
		    value = new Array(elements);
		    for ( var l = 0; l < elements; l++) {
		      var nsize = dv.getInt16(j);
		      j += 2;
		      var iarr = new Uint8Array( nsize);
		      for ( var k = 0; k < nsize; k++) {
			iarr[k] = dv.getUint8(j++);
		      }
		      value[l] = String.fromCharCode.apply( null, iarr);
		    }
		  }
		  break;
		default: break;
		}
		this.gdh.sub[eid].value = value;
	      }
	    }
	    if ( typeof this.gdh.pending[id] == 'undefined') {
	      console.log( "** GetObjectRefInfoAll received removed", id);
	      break;
	    }
	    var func_cb = this.gdh.pending[id].func_cb;
	    func_cb( id, sts);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.GET_ALL_XTT_CHILDREN: {
	    var result = [];
	    var j = 9;
	    var size = dv.getUint32(j);
	    if ( this.gdh.debug) console.log("GetAllXttChildren received", id, size);
	    console.log("GetAllXttChildren received", sts, id, size);
	    j += 4;
	    for ( var i = 0; i < size; i++) {
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
	      var iarr = new Uint8Array( nsize);
	      for ( var k = 0; k < nsize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      info.name = String.fromCharCode.apply( null, iarr);
	      //j += nsize;
	      var dsize = dv.getUint16(j);
 	      j += 2;
	      iarr = new Uint8Array( dsize);
	      for ( var k = 0; k < dsize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      info.description = String.fromCharCode.apply( null, iarr);
	      var csize = dv.getUint16(j);
 	      j += 2;
	      iarr = new Uint8Array( csize);
	      for ( var k = 0; k < csize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      info.classname = String.fromCharCode.apply( null, iarr);
	      result.push(info);
	    }
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts, result);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.GET_ALL_CLASS_ATTRIBUTES: {
	    var result = [];
	    var j = 9;
	    var size = dv.getUint32(j);
	    if ( this.gdh.debug) console.log("GetAllClassAttributes received", id, size);
	    j += 4;
	    for ( var i = 0; i < size; i++) {
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
	      var iarr = new Uint8Array( nsize);
	      for ( var k = 0; k < nsize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      info.name = String.fromCharCode.apply( null, iarr);

	      nsize = dv.getUint16(j);
	      j += 2;
	      iarr = new Uint8Array( nsize);
	      for ( var k = 0; k < nsize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      info.classname = String.fromCharCode.apply( null, iarr);
	      result.push(info);
	    }
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts, result);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.GET_OBJECT:
	  case Msg.GET_OBJECT_FROM_AREF:
	  case Msg.GET_OBJECT_FROM_NAME: {
	    if ( this.gdh.debug) console.log("GetObject received", id, sts);
	    var info = null;
	    if ( (sts & 1) !== 0) {
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
	      var iarr = new Uint8Array( nsize);
	      for ( var k = 0; k < nsize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      info.name = String.fromCharCode.apply( null, iarr);

	      nsize = dv.getUint16(j);
	      j += 2;
	      iarr = new Uint8Array( nsize);
	      for ( var k = 0; k < nsize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      info.fullname = String.fromCharCode.apply( null, iarr);

	      var csize = dv.getUint16(j);
	      j += 2;
	      iarr = new Uint8Array( csize);
	      for ( var k = 0; k < csize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      info.classname = String.fromCharCode.apply( null, iarr);
     
	      var dsize = dv.getUint16(j);
	      j += 2;
	      iarr = new Uint8Array( dsize);
	      for ( var k = 0; k < dsize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      info.description = String.fromCharCode.apply( null, iarr);

	      var p1size = dv.getUint16(j);
	      j += 2;
	      iarr = new Uint8Array( p1size);
	      for ( var k = 0; k < p1size; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      info.param1 = String.fromCharCode.apply( null, iarr);
	    }	      
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts, info);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.CRR_SIGNAL: {
	    var crrtext = null;
	    if ( (sts & 1) !== 0) {
	      var j = 9;
	      var result = [];
	      var size = dv.getUint16(j);
	      j += 2;
	      if ( this.gdh.debug) console.log("CrrSignal received", id, size);
	      for ( var i = 0; i < size; i++) {
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
		var iarr = new Uint8Array( nsize);
		for ( var k = 0; k < nsize; k++) {
		  iarr[k] = dv.getUint8(j++);
		}
		info.name = String.fromCharCode.apply( null, iarr);
		var csize = dv.getUint16(j);
		j += 2;
		iarr = new Uint8Array( csize);
		for ( var k = 0; k < csize; k++) {
		  iarr[k] = dv.getUint8(j++);
		}
		info.classname = String.fromCharCode.apply( null, iarr);
		result.push(info);
	      }
	    }
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts, result);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.GET_OPWIND_MENU: {
	    var result = new OpwindMenuInfo();
	    var j = 9;
	    if ( this.gdh.debug) console.log("GetOpwindMenu received", id, size);
	    console.log("GetOpwindMenu received", sts, id);

	    if ( sts & 1) {

	      var nsize = dv.getUint16(j);
	      j += 2;
	      var iarr = new Uint8Array( nsize);
	      for ( var k = 0; k < nsize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      result.title = String.fromCharCode.apply( null, iarr);

	      nsize = dv.getUint16(j);
	      j += 2;
	      iarr = new Uint8Array( nsize);
	      for ( var k = 0; k < nsize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      result.text = String.fromCharCode.apply( null, iarr);

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

	      for ( var i = 0; i < bsize; i++) {
		var button = new MenuButton();
		button.type = dv.getUint32(j);
		j += 4;
		nsize = dv.getUint16(j);
		j += 2;
		iarr = new Uint8Array( nsize);
		for ( var k = 0; k < nsize; k++) {
		  iarr[k] = dv.getUint8(j++);
		}
		button.text = String.fromCharCode.apply( null, iarr);
		nsize = dv.getUint16(j);
		j += 2;
		iarr = new Uint8Array( nsize);
		for ( var k = 0; k < nsize; k++) {
		  iarr[k] = dv.getUint8(j++);
		}
		button.name = String.fromCharCode.apply( null, iarr);
		nsize = dv.getUint16(j);
		j += 2;
		iarr = new Uint8Array( nsize);
		for ( var k = 0; k < nsize; k++) {
		  iarr[k] = dv.getUint8(j++);
		}
		button.url = String.fromCharCode.apply( null, iarr);
		result.buttons.push(button);
	      }
	    }
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts, result);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.CHECK_USER: {
	    var j = 9;
	    if ( this.gdh.debug) console.log("Check user received", id, size);
	    console.log("Check user received", sts, id);

	    var priv = 0;
	    if ( sts & 1) {
	      priv = dv.getUint32(j);
	      j += 4;
	    }
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts, priv);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.GET_MSG: {
	    if ( sts & 1) {
	      var j = 9;
	      var nsize = dv.getUint16(j);
	      j += 2;
	      var iarr = new Uint8Array( nsize);
	      for ( var k = 0; k < nsize; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      var msg = String.fromCharCode.apply( null, iarr);
	    }
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts, msg);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.MH_SYNC: {
	    var result = [];
	    var j = 9;
	    var size = dv.getUint32(j);
	    if ( this.gdh.debug) console.log("MhSync received", id, size);
	    j += 4;
	    for ( var i = 0; i < size; i++) {
	      var e = new MhEvent();

	      var len = dv.getUint16(j);
 	      j += 2;
	      var iarr = new Uint8Array( len);
	      for ( var k = 0; k < len; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      e.eventTime = String.fromCharCode.apply( null, iarr);

	      len = dv.getUint16(j);
 	      j += 2;
	      iarr = new Uint8Array( len);
	      for ( var k = 0; k < len; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      e.eventText = String.fromCharCode.apply( null, iarr);

	      len = dv.getUint16(j);
 	      j += 2;
	      iarr = new Uint8Array( len);
	      for ( var k = 0; k < len; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      e.eventName = String.fromCharCode.apply( null, iarr);

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
	      iarr = new Uint8Array( len);
	      for ( var k = 0; k < len; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      e.eventId.birthTime = String.fromCharCode.apply( null, iarr);

	      e.targetId = new MhEventId();
	      e.targetId.nix = dv.getUint32(j);
	      j += 4;
	      e.targetId.idx = dv.getUint32(j);
	      j += 4;
	      len = dv.getUint16(j);
 	      j += 2;
	      iarr = new Uint8Array( len);
	      for ( var k = 0; k < len; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      e.targetId.birthTime = String.fromCharCode.apply( null, iarr);

	      e.eventType = dv.getUint32(j);
	      j += 4;
	      var objid = new PwrtObjid(0,0);
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
	      var supObjid = new PwrtObjid(0,0);
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
	      iarr = new Uint8Array( len);
	      for ( var k = 0; k < len; k++) {
		iarr[k] = dv.getUint8(j++);
	      }
	      e.eventMoreText = String.fromCharCode.apply( null, iarr);

	      e.syncIdx = dv.getUint32(j);	      
	      j += 4;
	      result.push(e);
	    }
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts, result);
	    delete this.gdh.pending[id];
	    break;
	  }
	  case Msg.MH_ACK: {
	    var pending_data = this.gdh.pending[id];
	    pending_data.func_cb( id, pending_data.data, sts);
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

  this.getObjectInfoBoolean = function( name, return_cb) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length+6);
    buf[0] = Msg.GET_OBJECT_INFO_BOOLEAN;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    for ( var i = 0; i < name.length; i++) {
      buf[i+6] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectInfoInt = function( name, return_cb, data) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length+6);
    buf[0] = Msg.GET_OBJECT_INFO_INT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    for ( var i = 0; i < name.length; i++) {
      buf[i+6] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData( return_cb, data);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectInfoFloat = function( name, return_cb, data) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length+6);
    buf[0] = Msg.GET_OBJECT_INFO_FLOAT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    for ( var i = 0; i < name.length; i++) {
      buf[i+6] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData( return_cb, data);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectInfoFloatArray = function( name, asize, return_cb, data) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length+10);
    buf[0] = Msg.GET_OBJECT_INFO_FLOAT_ARRAY;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = asize & 0xFF;
    buf[7] = (asize >> 8) & 0xFF;
    buf[8] = (asize >> 16) & 0xFF;
    buf[9] = (asize >> 24) & 0xFF;
    for ( var i = 0; i < name.length; i++) {
      buf[i+10] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData( return_cb, data);
    this.ws.send(buf);
    this.next_id++;
  };

  this.refObjectInfo = function( name, type, elements) {
    var sub = new Sub();
    sub.name = name;
    sub.refid = this.subscriptionCount;
    sub.type = type;
    sub.elements = elements;
    this.sub[this.subscriptionCount] = sub;
    this.subscriptionCount++;
    if ( !this.listSent) {
      return sub.refid;
    }
    else {
      var size = 0;
      var len = 0;

      size = 12 + sub.name.length;

      var buf = new Uint8Array(size+10);
      buf[0] = Msg.REF_OBJECT_INFO;
      buf[2] = this.next_id & 0xFF;
      buf[3] = (this.next_id >> 8) & 0xFF;
      buf[4] = (this.next_id >> 16) & 0xFF;
      buf[5] = (this.next_id >> 24) & 0xFF;
      if ( this.debug) console.log("RefObjectInfo: ", sub.refid);
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

      for ( var j = 0; j < sub.name.length; j++) {
	buf[k++] = sub.name.charCodeAt(j);
      }

      this.pending[this.next_id] = new PendingData( this.refObjectInfoReply, null);
      if ( this.debug) console.log( "Sending RefObjectInfo", this.next_id, size, k);
      this.ws.send(buf);

      this.next_id++;

      return sub.refid;
    }
  };
  this.refObjectInfoReply = function( id, sts) {
    if ( this.debug) console.log( "refObjectInfoReply", id, sts);
  };
  this.unrefObjectInfo = function( refid) {
    var size = 0;
    var len = 0;

    size = 4;

    var buf = new Uint8Array(size+10);
    buf[0] = Msg.UNREF_OBJECT_INFO;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    if ( this.debug) console.log("UnrefObjectInfo: ", refid);
    var k = 6;
    buf[k++] = refid & 0xFF;
    buf[k++] = (refid >> 8) & 0xFF;
    buf[k++] = (refid >> 16) & 0xFF;
    buf[k++] = (refid >> 24) & 0xFF;

    this.pending[this.next_id] = new PendingData( this.unrefObjectInfoReply, null);
    if ( this.debug) console.log( "Sending UnrefObjectInfo", this.next_id, size, k, refid);
    this.ws.send(buf);

    this.next_id++;
    delete this.sub[refid];
  };
  this.refObjectInfoList = function( return_cb) {
    var size = 0;
    var len = 0;

    this.return_cb = return_cb;

    for( var i in this.sub) {
      size += 12 + this.sub[i].name.length;
      len++;
    }
    var buf = new Uint8Array(size+10);
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
    for ( var i in this.sub) {
      if ( i === 0)
	continue;
      if ( this.debug) console.log("RefObjectInfoList: ", this.sub[i].refid);
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

      for ( var j = 0; j < this.sub[i].name.length; j++) {
	buf[k++] = this.sub[i].name.charCodeAt(j);
      }
    }
    this.pending[this.next_id] = new PendingData( return_cb, null);
    if ( this.debug) console.log( "Sending RefObjectInfoList", this.next_id, size, k, this.next_id);
    this.ws.send(buf);

    this.next_id++;
    this.listSent = true;
  };
  this.refObjectInfoListReply = function( id, sts) {
    if ( this.debug) console.log( "refObjectInfoListReply", id, sts);
  };
  this.getRefObjectInfoAll = function( return_cb) {
    var buf = new Uint8Array(6);
    buf[0] = Msg.GET_OBJECT_REF_INFO_ALL;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData( return_cb, null);
    if ( this.debug) console.log( "Sending getRefObjectInfoAll", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };
  this.getRefObjectInfoAllReply = function( id, sts) {
    if ( this.debug) console.log( "getRefObjectInfoAllReply", id, sts);
  };
  this.getObjectRefInfo = function( id) {
    if ( this.debug)  console.log("getObjectRefInfo", id, this.sub[id].value);
    return this.sub[id].value;
  };
  this.setObjectInfoBoolean = function( name, value) {
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
    for ( var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    this.ws.send(buf);
    if ( this.debug) console.log("Sending setObjectInfoBoolean", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus( 1);
  };

  this.setObjectInfoInt = function( name, value) {
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
    for ( var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if ( this.debug) console.log("Sending setObjectInfoInt", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus( 1);
  };

  this.setObjectInfoFloat = function( name, value) {
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
    for ( var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if ( this.debug) console.log("Sending setObjectInfoFloat", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus( 1);
  };

  this.setObjectInfoString = function( name, value) {
    var i;
    var buf = new Uint8Array( 10 + value.length + name.length);
    buf[0] = Msg.SET_OBJECT_INFO_STRING;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = value.length & 0xFF;
    buf[7] = (value.length >> 8) & 0xFF;
    var k = 8;
    for ( i = 0; i < value.length; i++)
      buf[k++] = value.charCodeAt(i);
    buf[k++] = name.length & 0xFF;
    buf[k++] = (name.length >> 8) & 0xFF;
    for ( i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if ( this.debug) console.log("Sending setObjectInfoString", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus( 1);
  };

  this.toggleObjectInfo = function( name) {
    var buf = new Uint8Array(8 + name.length);
    buf[0] = Msg.TOGGLE_OBJECT_INFO;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = name.length & 0xFF;
    buf[7] = (name.length >> 8) & 0xFF;
    var k = 8;
    for ( var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if ( this.debug) console.log("Sending toggleObjectInfoBoolean", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus( 1);
  };

  this.getAllXttChildren = function( oid, return_cb, data) {
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
    this.pending[this.next_id] = new PendingData( return_cb, data);
    if ( this.debug) console.log( "Sending getAllXttChildren", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getAllClassAttributes = function( cid, oid, return_cb, data) {
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
    this.pending[this.next_id] = new PendingData( return_cb, data);
    if ( this.debug) console.log( "Sending getAllClassAttributes", this.next_id, cid, oid.vid, oid.oix);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObject = function( oid, op, return_cb, data) {
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
    this.pending[this.next_id] = new PendingData( return_cb, data);
    if ( this.debug) console.log( "Sending getObject", this.next_id, oid.vid, oid.oix);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectFromAref = function( aref, op, return_cb, data) {
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
    this.pending[this.next_id] = new PendingData( return_cb, data);
    if ( this.debug) console.log( "Sending getObject", this.next_id, oid.vid, oid.oix);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectFromName = function( name, op, return_cb, data) {
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
    for ( var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData( return_cb, data);
    if ( this.debug) console.log( "Sending getObjectFromName", this.next_id, name);
    this.ws.send(buf);
    this.next_id++;
  };

  this.crrSignal = function( oid, return_cb, data) {
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
    this.pending[this.next_id] = new PendingData( return_cb, data);
    if ( this.debug) console.log( "Sending crrObject", this.next_id, oid.vid, oid.oix);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getOpwindMenu = function( name, return_cb, data) {
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
    for ( var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData( return_cb, data);
    if ( this.debug) console.log( "Sending getOpwindMenu", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };

  this.login = function( user, passwd, return_cb, data) {

    var buf = new Uint8Array(6 + 2 + user.length + 2 + passwd.length);
    buf[0] = Msg.CHECK_USER;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    var k = 6;
    buf[k] = user.length & 0xFF;
    buf[k+1] = (user.length >> 8) & 0xFF;
    k += 2;
    for ( var i = 0; i < user.length; i++) {
      buf[k++] = user.charCodeAt(i);
    }
    buf[k] = passwd.length & 0xFF;
    buf[k+1] = (passwd.length >> 8) & 0xFF;
    k += 2;
    for ( var i = 0; i < passwd.length; i++) {
      buf[k++] = passwd.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData( return_cb, data);
    if ( this.debug) console.log( "Sending login", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getMsg = function( value, return_cb, data) {
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
    this.pending[this.next_id] = new PendingData( return_cb, data);
    this.ws.send(buf);
    if ( this.debug) console.log("Sending getMsg", this.next_id, value);
    this.next_id++;
  };

  this.mhSync = function( sync, return_cb, data) {
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
    this.pending[this.next_id] = new PendingData( return_cb, data);
    if ( this.debug) console.log( "Sending mhSync", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };

  this.mhAcknowledge = function( event_id, return_cb, data) {
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
    buf[k+1] = (event_id.birthTime.length >> 8) & 0xFF;
    k += 2;
    for ( var i = 0; i < event_id.birthTime.length; i++) {
      buf[k++] = event_id.birthTime.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData( return_cb, data);
    if ( this.debug) console.log( "Sending mhAcknowledge", this.next_id);
    console.log( "Sending mhAcknowledge", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };
}
/** End Gdh **/

/** Start Flow **/

var Flow = {
    DRAWOFFSET                          : 2,

    eConType_Straight                   : 0,
    eConType_Fixed                      : 1,
    eConType_AllFixed                   : 2,
    eConType_Routed                     : 3,
    eConType_StepDiv                    : 4,
    eConType_StepConv                   : 5,
    eConType_TransDiv                   : 6,
    eConType_TransConv                  : 7,
    eConType_StraightOneArrow           : 8,
    eConType_Reference                  : 9,

    eDrawType_Line                      : 0,
    eDrawType_LineRed                   : 1,
    eDrawType_LineGray                  : 2,
    eDrawType_LineErase                 : 3,
    eDrawType_LineDashed                : 4,
    eDrawType_LineDashedRed             : 5,
    eDrawType_TextHelvetica             : 6,
    eDrawType_TextHelveticaBold         : 7,
    eDrawType_TextHelveticaErase        : 8,
    eDrawType_TextHelveticaEraseBold    : 9,
    eDrawType_   			: 10,
    eDrawType_Green   			: 11,
    eDrawType_Yellow   			: 12,
    eDrawType_DarkGray   	       	: 13,
    eDrawType_Inherit   	       	: 9999,

    mDisplayLevel_1		        : 1 << 0,
    mDisplayLevel_2		        : 1 << 2,
    mDisplayLevel_3		        : 1 << 3,
    mDisplayLevel_4		        : 1 << 4,
    mDisplayLevel_5		        : 1 << 5,
    mDisplayLevel_6		        : 1 << 6,

    eNodeGroup_Common                   : 0,
    eNodeGroup_Document                 : 1,
    eNodeGroup_Trace                    : 2,

    eTraceType_Boolean                  : 0,
    eTraceType_Int32                    : 1,
    eTraceType_Float32                  : 2,
    eTraceType_User                     : 3,

    eSave_Ctx			        : 1,
    eSave_Array			        : 2,
    eSave_NodeClass			: 3,
    eSave_ConClass			: 4,
    eSave_Rect			        : 5,
    eSave_Line			        : 6,
    eSave_Point			        : 7,
    eSave_Arc			        : 8,
    eSave_Text			        : 9,
    eSave_Node			        : 10,
    eSave_Con			        : 11,
    eSave_ConPoint			: 12,
    eSave_Annot			        : 13,
    eSave_Arrow			        : 14,
    eSave_Pixmap			: 15,
    eSave_AnnotPixmap		        : 16,
    eSave_Radiobutton		        : 17,
    eSave_Frame			        : 18,
    eSave_AnnotPixmapButton    	        : 19,
    eSave_Triangle	       	        : 20,
    eSave_End			        : 99,
    eSave_Ctx_zoom_factor		: 100,
    eSave_Ctx_base_zoom_factor	        : 101,
    eSave_Ctx_offset_x		        : 102,
    eSave_Ctx_offset_y		        : 103,
    eSave_Ctx_nav_zoom_factor	        : 104,
    eSave_Ctx_print_zoom_factor	        : 105,
    eSave_Ctx_nav_offset_x		: 106,
    eSave_Ctx_nav_offset_y		: 107,
    eSave_Ctx_x_right		        : 108,
    eSave_Ctx_x_left		        : 109,
    eSave_Ctx_y_high	                : 110,
    eSave_Ctx_y_low			: 111,
    eSave_Ctx_window_width		: 112,
    eSave_Ctx_window_height		: 113,
    eSave_Ctx_nav_window_width	        : 114,
    eSave_Ctx_nav_window_height	        : 115,
    eSave_Ctx_nav_rect_ll_x		: 116,
    eSave_Ctx_nav_rect_ll_y		: 117,
    eSave_Ctx_nav_rect_ur_x		: 118,
    eSave_Ctx_nav_rect_ur_y		: 119,
    eSave_Ctx_nav_rect_hot		: 120,
    eSave_Ctx_name			: 121,
    eSave_Ctx_user_highlight	        : 122,
    eSave_Ctx_a_nc			: 123,
    eSave_Ctx_a_cc			: 124,
    eSave_Ctx_a			        : 125,
    eSave_Ctx_grid_size_x		: 126,
    eSave_Ctx_grid_size_y		: 127,
    eSave_Ctx_grid_on		        : 128,
    eSave_Ctx_draw_delta		: 129,
    eSave_Ctx_refcon_width		: 130,
    eSave_Ctx_refcon_height		: 131,
    eSave_Ctx_refcon_textsize	        : 132,
    eSave_Ctx_refcon_linewidth          : 133,
    eSave_Ctx_user_version              : 134,
    eSave_Array_a			: 200,
    eSave_NodeClass_nc_name		: 300,
    eSave_NodeClass_a		        : 301,
    eSave_NodeClass_group		: 302,
    eSave_NodeClass_no_con_obstacle    	: 303,
    eSave_ConClass_cc_name		: 400,
    eSave_ConClass_con_type		: 401,
    eSave_ConClass_corner		: 402,
    eSave_ConClass_draw_type	        : 403,
    eSave_ConClass_line_width	        : 404,
    eSave_ConClass_arrow_width          : 405,
    eSave_ConClass_arrow_length	        : 406,
    eSave_ConClass_round_corner_amount  : 407,
    eSave_ConClass_group		: 408,
    eSave_Rect_draw_type		: 500,
    eSave_Rect_line_width		: 501,
    eSave_Rect_ll			: 502,
    eSave_Rect_ur			: 503,
    eSave_Rect_display_level	        : 504,
    eSave_Rect_fill		        : 505,
    eSave_Line_draw_type		: 600,
    eSave_Line_line_width		: 601,
    eSave_Line_p1			: 602,
    eSave_Line_p2			: 603,
    eSave_Point_x			: 700,
    eSave_Point_y			: 701,
    eSave_Arc_angel1		        : 800,
    eSave_Arc_angel2		        : 801,
    eSave_Arc_draw_type		        : 802,
    eSave_Arc_line_width		: 803,
    eSave_Arc_ll			: 804,
    eSave_Arc_ur			: 805,
    eSave_Text_text_size		: 900,
    eSave_Text_draw_type		: 901,
    eSave_Text_text			: 902,
    eSave_Text_p			: 903,
    eSave_Node_nc			: 1000,
    eSave_Node_pos			: 1001,
    eSave_Node_n_name		        : 1002,
    eSave_Node_annotsize		: 1003,
    eSave_Node_annotv		        : 1004,
    eSave_Node_refcon_cnt		: 1005,
    eSave_Node_x_right		        : 1006,
    eSave_Node_x_left		        : 1007,
    eSave_Node_y_high		        : 1008,
    eSave_Node_y_low		        : 1009,
    eSave_Node_trace_object		: 1010,
    eSave_Node_trace_attribute	        : 1011,
    eSave_Node_trace_attr_type	        : 1012,
    eSave_Node_obst_x_right		: 1013,
    eSave_Node_obst_x_left		: 1014,
    eSave_Node_obst_y_high		: 1015,
    eSave_Node_obst_y_low		: 1016,
    eSave_Node_trace_inverted		: 1017,
    eSave_Con_x_right		        : 1100,
    eSave_Con_x_left		        : 1101,
    eSave_Con_y_high		        : 1102,
    eSave_Con_y_low			: 1103,
    eSave_Con_cc			: 1104,
    eSave_Con_dest_node		        : 1105,
    eSave_Con_source_node		: 1106,
    eSave_Con_dest_conpoint		: 1107,
    eSave_Con_source_conpoint	        : 1108,
    eSave_Con_dest_direction	        : 1109,
    eSave_Con_source_direction          : 1110,
    eSave_Con_line_a		        : 1111,
    eSave_Con_arc_a			: 1112,
    eSave_Con_arrow_a		        : 1113,
    eSave_Con_ref_a			: 1114,
    eSave_Con_p_num			: 1115,
    eSave_Con_l_num			: 1116,
    eSave_Con_a_num			: 1117,
    eSave_Con_arrow_num		        : 1118,
    eSave_Con_ref_num	                : 1119,
    eSave_Con_point_x		        : 1120,
    eSave_Con_point_y		        : 1121,
    eSave_Con_source_ref_cnt            : 1122,
    eSave_Con_dest_ref_cnt		: 1123,
    eSave_Con_c_name		        : 1124,
    eSave_Con_trace_object		: 1125,
    eSave_Con_trace_attribute	        : 1126,
    eSave_Con_trace_attr_type	        : 1127,
    eSave_Con_temporary_ref		: 1128,
    eSave_ConPoint_number		: 1200,
    eSave_ConPoint_direction	        : 1201,
    eSave_ConPoint_p		        : 1202,
    eSave_ConPoint_trace_attribute	: 1203,
    eSave_ConPoint_trace_attr_type	: 1204,
    eSave_Annot_number		        : 1300,
    eSave_Annot_draw_type		: 1301,
    eSave_Annot_text_size		: 1302,
    eSave_Annot_p			: 1303,
    eSave_Annot_annot_type		: 1304,
    eSave_Annot_display_level	        : 1305,
    eSave_Arrow_arrow_width		: 1400,
    eSave_Arrow_arrow_length	        : 1401,
    eSave_Arrow_draw_type		: 1402,
    eSave_Arrow_line_width		: 1403,
    eSave_Arrow_p_dest		        : 1404,
    eSave_Arrow_p1			: 1405,
    eSave_Arrow_p2			: 1406,
    eSave_Triangle_rect_part	       	: 2000
}

function GDraw( ctx) {
  this.ctx = ctx;
  this.canvas = document.querySelector("canvas");
  this.gctx = this.canvas.getContext("2d");
  this.offset_top = this.canvas.offsetTop;
  this.offset_left = this.canvas.offsetTop;
  console.log("offset_top", this.offset_top, "offset_left", this.offset_left);

  this.rect = function( x, y, width, height) {
    this.gctx.strokeRect( x, y, width, height);
  }
  this.line = function( x1, y1, x2, y2) {
    this.gctx.beginPath();
    this.gctx.moveTo( x1, y1);
    this.gctx.lineTo( x2, y2);
    this.gctx.stroke();
  }
}

function FlowArray( ctx) {
  this.a = [];
  this.ctx = ctx;

  this.add = function( elem) {
    this.a.push(elem);
  }
  this.size = function() {
    return this.a.length;
  }
  this.get = function( idx) {
    return this.a[idx];
  }
  this.open = function( lines, row) {
    var end = false;
    var i;
    for ( i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);

      if ( this.ctx.debug) console.log( "array : " + lines[i]);

      switch ( key) {
      case Flow.eSave_Array:
	break;
      case Flow.eSave_NodeClass:
	var nc = new FlowNodeClass( ctx);
	i = nc.open( lines, i+1);
	this.a.push( nc);
	break;
      case Flow.eSave_ConClass:
	var cc = new FlowConClass( ctx);
	i = cc.open( lines, i+1);
	this.a.push( cc);
	break;
      case Flow.eSave_Node:
	var n = new FlowNode( ctx);
	i = n.open( lines, i+1);
	this.a.push( n);
	break;
      case Flow.eSave_Con:
	var c = new FlowCon( ctx);
	i = c.open( lines, i+1);
	this.a.push( c);
	break;
      case Flow.eSave_Rect:
	var rect = new FlowRect( ctx);
	i = rect.open( lines, i+1);
	this.a.push( rect);
	break;
      case Flow.eSave_Line:
	var l = new FlowLine( ctx);
	i = l.open( lines, i+1);
	this.a.push( l);
	break;
      case Flow.eSave_Triangle:
	var t = new FlowTriangle( ctx);
	i = t.open( lines, i+1);
	this.a.push( t);
	break;
      case Flow.eSave_Arc:
	var arc = new FlowArc( ctx);
	i = arc.open( lines, i+1);
	this.a.push( arc);
	break;
      case Flow.eSave_Text:
	var text = new FlowText( ctx);
	i = text.open( lines, i+1);
	this.a.push( text);
	break;
      case Flow.eSave_ConPoint:
	var cp = new FlowConPoint( ctx);
	i = cp.open( lines, i+1);
	this.a.push( cp);
	break;
      case Flow.eSave_Annot:
	var annot = new FlowAnnot( ctx);
	i = annot.open( lines, i+1);
	this.a.push( annot);
	break;
      case Flow.eSave_Arrow:
	var arrow = new FlowArrow( ctx);
	i = arrow.open( lines, i+1);
	this.a.push( arrow);
	break;
      case Flow.eSave_Point:
	var point = new FlowPoint( ctx);
	i = point.open( lines, i+1);
	this.a.push( point);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:	  
	console.log( "Syntax error in FlowArray", key);
	break;
      }
      if ( end)
	break;
    }
    return i;
  };

  this.draw = function( g, p, node, highlight) {
    for ( var i = 0; i < this.a.length; i++)
      this.a[i].draw( g, p, node, highlight);
  }

  this.search_by_name = function( name) {
    var uname = name.toUpperCase();
    for ( var i = 0; i < this.a.length; i++) {
      if ( this.a[i] instanceof FlowNode) {
	console.log( "Search", this.a[i].n_name, name);
	if ( this.a[i].n_name.toUpperCase() == uname)
	  return this.a[i];
      }
    }
    return null;
  }
}

function FlowNodeClass( ctx) {

  this.a = new FlowArray( ctx);
  this.ctx = ctx;
  this.nc_name = "";
  this.group = 0;

  this.draw = function( g, p, node, highlight) {
    this.a.draw( g, p, node, highlight);
  }

  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);
      
      if ( this.ctx.debug) console.log( "nodeclass : " + lines[i]);
      
      switch ( key) {
      case Flow.eSave_NodeClass_nc_name:
	this.nc_name = tokens[1];
	break;
      case Flow.eSave_NodeClass_a:
	i = this.a.open( lines, i+1);
	break;
      case Flow.eSave_NodeClass_group:
	this.group  = parseInt( tokens[1]);
	break;
      case Flow.eSave_NodeClass_no_con_obstacle:
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:	  
	console.log( "Syntax error in FlowNodeClass");
	break;
      }
      if ( end)
	break;
    }
    return i;
  }

  this.event_handler = function( x, y) {
    return 0;
  }
}

function FlowConClass( ctx) {
  this.cc_name = "";
  this.con_type = 0;
  this.corner = 0;
  this.draw_type = 0;
  this.line_width = 0;
  this.arrow_width = 0.0;
  this.arrow_length = 0.0;
  this.round_corner_amount = 0.0;
  this.group = 0;
  this.ctx = ctx;

  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);
      
      if ( this.ctx.debug) console.log( "conclass : " + lines[i]);
      
      switch ( key) {
      case Flow.eSave_ConClass:
	break;
      case Flow.eSave_ConClass_cc_name:
	this.cc_name = tokens[1];
	break;
      case Flow.eSave_ConClass_con_type:
	this.con_type = parseInt( tokens[1]);
	break;
      case Flow.eSave_ConClass_corner:
	this.corner = parseInt( tokens[1]);
	break;
      case Flow.eSave_ConClass_draw_type:
	this.draw_type = parseInt( tokens[1]);
	break;
      case Flow.eSave_ConClass_line_width:
	this.line_width = parseInt( tokens[1]);
	break;
      case Flow.eSave_ConClass_arrow_width:
	this.arrow_width = parseFloat( tokens[1]);
	break;
      case Flow.eSave_ConClass_arrow_length:
	this.arrow_length = parseFloat( tokens[1]);
	break;
      case Flow.eSave_ConClass_round_corner_amount:
	this.round_corner_amount = parseFloat( tokens[1]);
	break;
      case Flow.eSave_ConClass_group:
	this.group = parseInt( tokens[1]);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:	  
	console.log( "Syntax error in FlowConClass");
	break;
      }
      if ( end)
	break;
    }
    return i;
  }

}

function FlowPoint( ctx) {
  this.x = 0.0;
  this.y = 0.0;
  this.ctx = ctx;
  
  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);
      
      if ( this.ctx.debug) console.log( "point : " + lines[i]);
      
      switch ( key) {
      case Flow.eSave_Point:
	break;
      case Flow.eSave_Point_x:
	this.x = parseFloat( tokens[1]);
	break;
      case Flow.eSave_Point_y:
	this.y = parseFloat( tokens[1]);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:
	System.out.println("Syntax error in FlowPoint");
	break;
      }
	if ( end)
	  break;
    }
    return i;
  }
}

function FlowLine( ctx) {
  this.p1 = new FlowPoint(ctx);
  this.p2 = new FlowPoint(ctx);
  this.draw_type = 0;
  this.line_width = 0;
  this.ctx = ctx;

  this.draw = function( g, p, node, highlight) {
    var x1 = (this.p1.x + p.x) * this.ctx.zoom_factor;
    var y1 = (this.p1.y + p.y) * this.ctx.zoom_factor;
    var x2 = (this.p2.x + p.x) * this.ctx.zoom_factor;
    var y2 = (this.p2.y + p.y) * this.ctx.zoom_factor;

    g.lineWidth = this.ctx.zoom_factor / this.ctx.base_zoom_factor * this.line_width;
    if ( g.lineWidth < 1)
      g.lineWidth = 1;

    switch ( this.draw_type) {
    case Flow.eDrawType_LineDashed:
    case Flow.eDrawType_LineDashedRed:
      g.setLineDash([8,8]);
      break;
    }

    switch ( this.draw_type) {
    case Flow.eDrawType_LineGray:
      g.strokeStyle = "lightgray";
      break;
    case Flow.eDrawType_LineRed: 
    case Flow.eDrawType_LineDashedRed:
      g.strokeStyle = "red";
      break;
    case Flow.eDrawType_LineErase:
      return;
    default:
      g.strokeStyle = "black";
    }
    if ( highlight)
      g.strokeStyle = "red";
    g.beginPath();
    g.moveTo( x1, y1);
    g.lineTo( x2, y2);
    g.stroke();

    switch ( this.draw_type) {
    case Flow.eDrawType_LineDashed:
    case Flow.eDrawType_LineDashedRed:
      g.setLineDash([]);
      break;
    }
  }
  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);
      
      if ( this.ctx.debug) console.log( "line : " + lines[i]);

      switch ( key) {
      case Flow.eSave_Line:
	break;
      case Flow.eSave_Line_draw_type:
	this.draw_type = parseInt(tokens[1]);
	break;
      case Flow.eSave_Line_line_width:
	this.line_width = parseInt(tokens[1]);
	break;
      case Flow.eSave_Line_p1:
	i = this.p1.open( lines, i+1);
	break;
      case Flow.eSave_Line_p2:
	i = this.p2.open( lines, i+1);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in FlowLine");
	break;
      }
      if ( end)
	break;
    }
    return i;
  }
}

function FlowRect( ctx) {
  this.ll = new FlowPoint(ctx);
  this.ur = new FlowPoint(ctx);
  this.draw_type = 0;
  this.line_width = 0;
  this.display_level = 0;
  this.fill = 0;
  this.ctx = ctx;

  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);
      
      if ( this.ctx.debug) console.log( "rect : " + lines[i]);

      switch ( key) {
      case Flow.eSave_Rect:
	break;
      case Flow.eSave_Rect_draw_type:
	this.draw_type = parseInt(tokens[1]);
	break;
      case Flow.eSave_Rect_line_width:
	this.line_width = parseInt(tokens[1]);
	break;
      case Flow.eSave_Rect_display_level:
	this.display_level = parseInt(tokens[1]);
	break;
      case Flow.eSave_Rect_fill:
	this.fill = parseInt(tokens[1]);
	break;
      case Flow.eSave_Rect_ll:
	i = this.ll.open( lines, i+1);
	break;
      case Flow.eSave_Rect_ur:
	i = this.ur.open( lines, i+1);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in FlowRect");
	break;
      }
      if ( end)
	break;
    }
    return i;
  }
  this.draw = function( g, p, node, highlight) {
    if ( (this.display_level & this.ctx.display_level) == 0)
      return;

    var x = (this.ll.x + p.x) * this.ctx.zoom_factor;
    var y = (this.ll.y + p.y) * this.ctx.zoom_factor;
    var width = (this.ur.x - this.ll.x) * this.ctx.zoom_factor;
    var height = (this.ur.y - this.ll.y) * this.ctx.zoom_factor;

    g.lineWidth = this.ctx.zoom_factor / this.ctx.base_zoom_factor * this.line_width;
    if ( g.lineWidth < 1)
      g.lineWidth = 1;
    switch ( this.draw_type) {
    case Flow.eDrawType_LineGray:
      g.strokeStyle = "grey";
      break;
    case Flow.eDrawType_LineRed: 
    case Flow.eDrawType_LineDashedRed:
      g.strokeStyle = "red";
      break;
    case Flow.eDrawType_LineErase:
      return;
    default:
      g.strokeStyle = "black";
    }
    if ( highlight)
      g.strokeStyle = "red";
    g.strokeRect( x, y, width, height);
  }
}

function FlowArc( ctx) {
  this.ll = new FlowPoint(ctx);
  this.ur = new FlowPoint(ctx);
  this.angel1 = 0.0;
  this.angel2 = 0.0;
  this.draw_type = 0;
  this.line_width = 0;
  this.ctx = ctx;

  this.draw = function( g, p, node, highlight) {
    var r = (this.ur.x - this.ll.x) / 2 * this.ctx.zoom_factor;
    var x = (this.ll.x + p.x) * this.ctx.zoom_factor + r; 
    var y = (this.ll.y + p.y) * this.ctx.zoom_factor + r; 
    if ( this.angel1 == 90 || this.angel1 == 270)
      var a1 = (this.angel1 + 90)/ 180 * Math.PI;
    else
      var a1 = (this.angel1 - 90)/ 180 * Math.PI;
    var a2 = a1 + this.angel2 / 180 * Math.PI;
    g.lineWidth = this.ctx.zoom_factor / this.ctx.base_zoom_factor * this.line_width;
    if ( g.lineWidth < 1)
      g.lineWidth = 1;

    g.strokeStyle = "black";
    if ( highlight)
      g.strokeStyle = "red";

    g.beginPath()
    g.arc( x, y, r, a1, a2, false);
    g.stroke();
  }
  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);
      
      if ( this.ctx.debug) console.log( "arc : " + lines[i]);

      switch ( key) {
      case Flow.eSave_Arc:
	break;
      case Flow.eSave_Arc_draw_type:
	this.draw_type = parseInt(tokens[1]);
	break;
      case Flow.eSave_Arc_line_width:
	this.line_width = parseInt(tokens[1]);
	break;
      case Flow.eSave_Arc_angel1:
	this.angel1 = parseFloat(tokens[1]);
	break;
      case Flow.eSave_Arc_angel2:
	this.angel2 = parseFloat(tokens[1]);
	break;
      case Flow.eSave_Arc_ll:
	i = this.ll.open( lines, i+1);
	break;
      case Flow.eSave_Arc_ur:
	i = this.ur.open( lines, i+1);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in FlowArc");
	break;
      }
      if ( end)
	break;
    }
    return i;
  }
}

function FlowText( ctx) {
  this.p = new FlowPoint(ctx);
  this.draw_type = 0;
  this.text_size = 0;
  this.text = "";
  this.ctx = ctx;

  this.draw = function( g, p0, node, highlight) {
    var tsize = 0;
    var idx = this.ctx.zoom_factor / this.ctx.base_zoom_factor * (this.text_size +4) - 4;
    if ( idx < 0) return;

    var x = (this.p.x + p0.x) * this.ctx.zoom_factor;
    var y = (this.p.y + p0.y) * this.ctx.zoom_factor;

    switch( idx) {
    case 0: tsize = 8; break;
    case 1: tsize = 10; break;
    case 2: tsize = 12; break;
    case 3: tsize = 14; break;
    case 4: tsize = 14; break;
    case 5: tsize = 8; break;
    case 6: tsize = 18; break;
    case 7: tsize = 18; break;
    default: tsize = 3 * idx;
    }
    g.font = tsize + "px Arial";
    g.lineWidth = 0.5;

    g.fillStyle = "black";
    if ( highlight)
      g.fillStyle = "red";
    g.fillText( this.text, x, y);
  }
  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);
      
      if ( this.ctx.debug) console.log( "text : " + lines[i]);

      switch ( key) {
      case Flow.eSave_Text:
	break;
      case Flow.eSave_Text_text_size:
	this.text_size = parseInt(tokens[1]);
	break;
      case Flow.eSave_Text_draw_type:
	this.draw_type = parseInt(tokens[1]);
	break;
      case Flow.eSave_Text_text:
	this.text = tokens[1];
	break;
      case Flow.eSave_Text_p:
	i = this.p.open( lines, i+1);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in FlowText");
	break;
      }
      if ( end)
	break;
    }
    return i;
  }
}

function FlowArrow( ctx) {
  this.p1 = new FlowPoint(ctx);
  this.p2 = new FlowPoint(ctx);
  this.p_dest = new FlowPoint(ctx);
  this.arrow_width = 0.0;
  this.arrow_length = 0.0;
  this.draw_type = 0;
  this.line_width = 0;
  this.ctx = ctx;

  this.draw = function( g, p, node, highlight) {
    var x1 = (this.p1.x + p.x) * this.ctx.zoom_factor;
    var x2 = (this.p2.x + p.x) * this.ctx.zoom_factor;
    var x3 = (this.p_dest.x + p.x) * this.ctx.zoom_factor;
    var y1 = (this.p1.y + p.y) * this.ctx.zoom_factor;
    var y2 = (this.p2.y + p.y) * this.ctx.zoom_factor;
    var y3 = (this.p_dest.y + p.y) * this.ctx.zoom_factor;

    switch ( this.draw_type) {
    case Flow.eDrawType_LineGray:
      g.fillStyle = "lightgray";
      break;
    case Flow.eDrawType_LineRed: 
    case Flow.eDrawType_LineDashedRed:
      g.fillStyle = "red";
      break;
    case Flow.eDrawType_LineErase:
      return;
    default:
      g.fillStyle = "black";
    }
    if ( highlight)
      g.fillStyle = "red";
    g.beginPath()
    g.moveTo( x1, y1);
    g.lineTo( x2, y2);
    g.lineTo( x3, y3);
    g.lineTo( x1, y1);
    g.fill();
  }

  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);
      
      if ( this.ctx.debug) console.log( "arrow : " + lines[i]);

      switch ( key) {
      case Flow.eSave_Arrow:
	break;
      case Flow.eSave_Arrow_arrow_width:
	this.arrow_width = parseFloat( tokens[1]);
	break;
      case Flow.eSave_Arrow_arrow_length:
	this.arrow_length = parseFloat( tokens[1]);
	break;
      case Flow.eSave_Arrow_draw_type:
	this.draw_type = parseInt( tokens[1]);
	break;
      case Flow.eSave_Arrow_line_width:
	this.line_width = parseInt( tokens[1]);
	break;
      case Flow.eSave_Arrow_p_dest:
	i = this.p_dest.open( lines, i+1);
	break;
      case Flow.eSave_Arrow_p1:
	i = this.p1.open( lines, i+1);
	break;
      case Flow.eSave_Arrow_p2:
	i = this.p2.open( lines, i+1);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in FlowArrow");
	break;
      }
      if ( end)
	break;
    }
  }
}

function FlowTriangle( ctx) {
  this.prototype = new FlowRect(ctx);

  this.draw = function( g, p, node, highlight) {
    var x1 = (this.prototype.ll.x + p.x) * this.prototype.ctx.zoom_factor;
    var y1 = (this.prototype.ll.y + p.y) * this.prototype.ctx.zoom_factor;
    var x2 = (this.prototype.ur.x + p.x) * this.prototype.ctx.zoom_factor;
    var y2 = (this.prototype.ur.y + p.y) * this.prototype.ctx.zoom_factor;

    g.lineWidth = this.prototype.ctx.zoom_factor / this.prototype.ctx.base_zoom_factor * 
      this.prototype.line_width;
    if ( g.lineWidth < 1)
      g.lineWidth = 1;

    var dtype = this.prototype.draw_type;
    if ( dtype == Flow.eDrawType_Inherit && node != null)
      dtype = node.fill_color;

    if ( this.prototype.fill == 1) {
      switch ( dtype) {
      case Flow.eDrawType_LineRed:
	g.fillStyle = "red";
	break;
      case Flow.eDrawType_Green: 
	g.fillStyle = "green";
	break;
      case Flow.eDrawType_Yellow:
	g.fillStyle = "yellow";
	break;
      default:
	g.fillStyle = "gray";
      }

      g.beginPath();
      g.moveTo( x1, y2);
      g.lineTo( (x1 + x2) / 2, y1);
      g.lineTo( x2, y2);
      g.lineTo( x1, y2);
      g.fill();
    }
    else {
      g.strokeStyle = "black";
      g.beginPath();
      g.moveTo( x1, y2);
      g.lineTo( (x1 + x2) / 2, y1);
      g.lineTo( x2, y2);
      g.lineTo( x1, y2);
      g.stroke();
    }
  }
  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);
      
      if ( this.prototype.ctx.debug) console.log( "triangle : " + lines[i]);

      switch ( key) {
      case Flow.eSave_Triangle_rect_part:
	i = this.prototype.open( lines, i+1);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in FlowTriangle");
	break;
      }
      if ( end)
	break;
    }
    return i;
  }
}

function FlowConPoint( ctx) {
  this.p = new FlowPoint( ctx);
  this.number = 0;
  this.direction = 0;
  this.trace_attribute = "";
  this.trace_attr_type = 0;
  this.ctx = ctx;

  this.draw = function( g, p, node, highlight) {
  }
  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);
      
      if ( ctx.debug) console.log( "conpoint : " + lines[i]);

      switch ( key) {
      case Flow.eSave_ConPoint:
	break;
      case Flow.eSave_ConPoint_number:
	this.number = parseInt(tokens[1]);
	break;
      case Flow.eSave_ConPoint_direction:
	this.direction = parseInt(tokens[1]);
	break;
      case Flow.eSave_ConPoint_p:
	i = this.p.open( lines, i+1);
	break;
      case Flow.eSave_ConPoint_trace_attribute:
	this.trace_attribute = tokens[1];
	break;
      case Flow.eSave_ConPoint_trace_attr_type:
	this.trace_attr_type = parseInt(tokens[1]);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in FlowConPoint");
	break;
      }
      if ( end)
	break;
    }
    return i;
  }
}

function FlowAnnot( ctx) {
  this.p = new FlowPoint(ctx);
  this.draw_type = 0;
  this.text_size = 0;
  this.display_level = 0;
  this.annot_type = 0;
  this.number = 0;
  this.ctx = ctx;

  this.draw = function( g, p0, node, highlight) {
    if ( node == null)
      return;
    if ( node.annotv[this.number] == null)
      return;
    if ( (this.display_level & this.ctx.display_level) == 0)
      return;

    var tsize = 0;
    var idx = this.ctx.zoom_factor / this.ctx.base_zoom_factor * (this.text_size +4) - 4;
    if ( idx < 0) return;

    switch( idx) {
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


    switch ( this.draw_type) {
      case Flow.eDrawType_TextHelveticaBold:
        g.font = "bold " + tsize + "px Arial";
        break;
      default:
        g.font =  tsize + "px Arial";
    }
    g.fillStyle = "black";
    if ( highlight)
      g.fillStyle = "red";
    g.lineWidth = 0.5;

    var x = (this.p.x + p0.x) * this.ctx.zoom_factor;
    var y = (this.p.y + p0.y) * this.ctx.zoom_factor - tsize/4;

    var tokens = node.annotv[this.number].split('\n');
    for ( var i = 0; i < tokens.length; i++) {
      g.fillText( tokens[i], x, y);
      y += tsize * 1.4;
    }
  }
  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);
      
      if ( this.ctx.debug) console.log( "annot : " + lines[i]);

      switch ( key) {
      case Flow.eSave_Annot:
	break;
      case Flow.eSave_Annot_number:
	this.number = parseInt( tokens[1]);
	break;
      case Flow.eSave_Annot_draw_type:
	this.draw_type = parseInt( tokens[1]);
	break;
      case Flow.eSave_Annot_text_size:
	this.text_size = parseInt( tokens[1]);
	break;
      case Flow.eSave_Annot_display_level:
	this.display_level = parseInt( tokens[1]);
	break;
      case Flow.eSave_Annot_p:
	i = this.p.open( lines, i+1);
	break;
      case Flow.eSave_Annot_annot_type:
	this.annot_type = parseInt( tokens[1]);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in FlowAnnot");
	break;
      }
      if ( end)
	break;
    }
    return i;
  }
}

function FlowCon(ctx) {
  this.ctx = ctx;
  this.x_right = 0.0;
  this.x_left = 0.0;
  this.y_high = 0.0;
  this.y_low = 0.0;
  this.cc = null;
  this.p_num = 0;
  this.l_num = 0;
  this.a_num = 0;
  this.arrow_num = 0;
  this.ref_num = 0;
  this.point_x = new FlowArray(ctx);
  this.point_y = new FlowArray(ctx);
  this.line_a = new FlowArray(ctx);
  this.arc_a = new FlowArray(ctx);
  this.arrow_a = new FlowArray(ctx);
  this.ref_a = new FlowArray(ctx);
  this.c_name = "";
  this.trace_object = "";
  this.trace_attribute = "";
  this.trace_attr_type = 0;
  this.temporary_ref = 0;
  this.highlight = false; 
  this.redraw = true;

  this.draw = function( g, p0, node, highlight) {
    if ( !this.redraw)
      return;
    var p = new FlowPoint(ctx);
    p.x = 0;
    p.y = 0;
    if ( this.temporary_ref != 0 || this.cc.con_type == Flow.eConType_Reference)
      this.ref_a.draw( g, p, null, highlight);
    else {
      this.line_a.draw( g, p, null, highlight);
      this.arc_a.draw( g, p, null, highlight);
      this.arrow_a.draw( g, p, null, highlight);
    }
    this.redraw = false;
  }
  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);

      if ( this.ctx.debug) console.log( "con : " + lines[i]);

      switch ( key) {
      case Flow.eSave_Con:
	break;
      case Flow.eSave_Con_cc:
	var cc_name = tokens[1];
	var found = false;
	for ( var j = 0; j < this.ctx.a_cc.size(); j++) {
	  if ( this.ctx.a_cc.get(j).cc_name === cc_name) {
	    this.cc = this.ctx.a_cc.get(j);
	    found = true;
	    break;
	  }
	}
	if ( !found)
	  console.log( "FlowCon: ConClass not found: " + cc_name);
	break;
      case Flow.eSave_Con_c_name:
	if ( tokens[1] != null)
	  this.c_name = tokens[1];
	break;
      case Flow.eSave_Con_x_right:
	this.x_right = parseFloat(tokens[1]);
	break;
      case Flow.eSave_Con_x_left:
	this.x_left = parseFloat(tokens[1]);
	break;
      case Flow.eSave_Con_y_high:
	this.y_high = parseFloat(tokens[1]);
	break;
      case Flow.eSave_Con_y_low:
	this.y_low = parseFloat(tokens[1]);
	break;
      case Flow.eSave_Con_dest_node:
	i++;
	break;
      case Flow.eSave_Con_source_node:
	i++;
	break;
      case Flow.eSave_Con_dest_conpoint:
      case Flow.eSave_Con_source_conpoint:
      case Flow.eSave_Con_dest_direction:
      case Flow.eSave_Con_source_direction:
	break;
      case Flow.eSave_Con_line_a:
	i = this.line_a.open( lines, i+1);
	break;
      case Flow.eSave_Con_arc_a:
	i = this.arc_a.open( lines, i+1);
	break;
      case Flow.eSave_Con_arrow_a:
	i = this.arrow_a.open( lines, i+1);
	break;
      case Flow.eSave_Con_ref_a:
	i = this.ref_a.open( lines, i+1);
	break;
      case Flow.eSave_Con_p_num:
	this.p_num = parseInt(tokens[1]);
	break;
      case Flow.eSave_Con_l_num:
	this.l_num = parseInt(tokens[1]);
	break;
      case Flow.eSave_Con_a_num:
	this.a_num = parseInt(tokens[1]);
	break;
      case Flow.eSave_Con_arrow_num:
	this.arrow_num = parseInt(tokens[1]);
	break;
      case Flow.eSave_Con_ref_num:
	this.ref_num = parseInt(tokens[1]);
	break;
      case Flow.eSave_Con_point_x:
	for ( var j = 0; j < this.p_num; j++) {
	  i++;
	  this.point_x[j] = parseFloat(lines[i]);
	}
	break;
      case Flow.eSave_Con_point_y:
	for ( var j = 0; j < this.p_num; j++) {
	  i++;
	  this.point_y[j] = parseFloat(lines[i]);
	}
	break;
	
      case Flow.eSave_Con_source_ref_cnt:
      case Flow.eSave_Con_dest_ref_cnt:
	break;
      case Flow.eSave_Con_trace_object:
	if ( tokens[1] != null)
	  this.trace_object = tokens[1];
	break;
      case Flow.eSave_Con_trace_attribute:
	if ( tokens[1] != null)
	  this.trace_attribute = tokens[1];
	break;
      case Flow.eSave_Con_trace_attr_type:
	this.trace_attr_type = parseInt(tokens[1]);
	break;
      case Flow.eSave_Con_temporary_ref:
	this.temporary_ref = parseInt(tokens[1]);
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in FlowCon");
	break;
      }
      if ( end)
	break;
    }    
    return i;
  }
  this.connect = function() {
  }
  this.scan = function() {
  }
}

function FlowNode( ctx) {
  this.ctx = ctx;
  this.OFFSET = 2;
  this.x_right = 0.0;
  this.x_left = 0.0;
  this.y_high = 0.0;
  this.y_low = 0.0;
  this.nc = null;
  this.pos = new FlowPoint(ctx);
  this.n_name = "";
  this.annotv = [];
  this.annotsize = [];
  this.trace_object = "";
  this.trace_attribute = "";
  this.trace_attr_type = 0;
  this.highlight = false;
  this.select = false;
  this.fill_color = 0;
  this.p = 0;
  this.old_value = 0;
  this.first_scan = true;
  this.redraw = true;

  this.draw = function( g, p, node, highlight) {
    if ( !this.redraw)
      return;

    if ( this.nc.group != Flow.eNodeGroup_Document) {
      var x = this.x_left * this.ctx.zoom_factor;
      var y = this.y_low * this.ctx.zoom_factor-1;
      var width = (this.x_right - this.x_left) * this.ctx.zoom_factor;
      var height = (this.y_high - this.y_low) * this.ctx.zoom_factor+2;
      g.fillStyle = "white";
      if ( this.select)
	g.fillStyle = "lightblue";
      g.fillRect( x, y, width, height);
    }

    this.nc.draw( g, this.pos, this, this.highlight);
    this.redraw = false;
  }
  this.open = function( lines, row) {

    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);

      if ( this.ctx.debug) console.log( "node : " + lines[i]);

      switch ( key) {
      case Flow.eSave_Node_nc:
	var nc_name = tokens[1];
	var found = false;
	for ( var j = 0; j < this.ctx.a_nc.size(); j++) {
	  if ( this.ctx.a_nc.get(j).nc_name === nc_name) {
	    this.nc = this.ctx.a_nc.get(j);
	    found = true;
	    break;
	  }
	}
	if ( !found)
	  console.log( "FlowNode: NodeClass not found: " + nc_name);
	break;
      case Flow.eSave_Node_n_name:
	if ( tokens[1] != null)
	  this.n_name = tokens[1];
	break;
      case Flow.eSave_Node_refcon_cnt:
	i += 32;
	break;
      case Flow.eSave_Node_x_right:
	this.x_right = parseFloat(tokens[1]);
	break;
      case Flow.eSave_Node_x_left:
	this.x_left = parseFloat(tokens[1]);
	break;
      case Flow.eSave_Node_y_high:
	this.y_high = parseFloat(tokens[1]);
	break;
      case Flow.eSave_Node_y_low:
	this.y_low = parseFloat(tokens[1]);
	break;
      case Flow.eSave_Node_annotsize:
	for ( var j = 0; j < 10; j++) {
	  i++;
	  this.annotsize[j] = parseInt(lines[i]);
	}
	break;
      case Flow.eSave_Node_annotv:
	// Annotation are surrounded by quotes. A quote inside a
	// annotation is preceeded by a backslash. The size is calculated
	// without backslashes
	for ( var j = 0; j < 10; j++) {
	  if ( this.annotsize[j] > 0) {
	    var sb = [];
	    var c = 0;
	    var prevc;	    	    
	    var end_found = false;
	    var first = true;

	    for ( ;;) {
	      i++;
	      if ( lines[i] == null) {
		sb.push('\n');
		continue;
	      }
		
	      for ( var k = 0; k < lines[i].length; k++) {
		prevc = c;
		c = lines[i].charAt(k);
		if ( first) {
		  first = false;
		  continue;  
		}
		if ( c === '\"' && prevc === '\\') {
		  sb[sb.length-1] = null;
		  continue;
		}
		if ( c === '\"') {
		  end_found = true;
		  break;
		}
		sb.push(c);
		if ( k == lines[i].length - 1)
		  sb.push('\n');
	      }
	      if ( end_found)
		break;
	    }
	    this.annotv[j] = sb.join("");
	  }
	}
	break;
      case Flow.eSave_Node_pos:
	i = this.pos.open( lines, i+1);
	break;
      case Flow.eSave_Node_trace_object:
	if ( tokens[1] != null)
	  this.trace_object = tokens[1];
	break;
      case Flow.eSave_Node_trace_attribute:
	if ( tokens[1] != null)
	  this.trace_attribute = tokens[1];
	break;
      case Flow.eSave_Node_trace_attr_type:
	this.trace_attr_type = parseInt(tokens[1]);
	break;
      case Flow.eSave_Node_obst_x_right:
      case Flow.eSave_Node_obst_x_left:
      case Flow.eSave_Node_obst_y_high:
      case Flow.eSave_Node_obst_y_low:
      case Flow.eSave_Node_trace_inverted:
	break;
      case Flow.eSave_End:
	end = true;
	break;
      default:
	console.log( "Syntax error in FlowNode");
	break;
      }
      if ( end)
	break;
    }
    console.log("Node", this.trace_attr_type, this.n_name);
    return i;
  }

  this.connect = function() {
    if ( this.trace_object == "" || this.trace_attribute == "")
      return;
    if ( this.trace_attr_type == Flow.eTraceType_User)
      return;

    var n = this.trace_attribute.indexOf('#');
    if ( n != -1)
      this.trace_attribute = this.trace_attribute.substring(0, n);

    var o = this.trace_object + "." + this.trace_attribute;
    var pwr_type;
    switch ( this.trace_attr_type) {      
    case Flow.eTraceType_Boolean:
      pwr_type = Pwr.eType_Boolean;
      break;
    case Flow.eTraceType_Int32:
      pwr_type = Pwr.eType_Int32;
      break;
    case Flow.eTraceType_Float32:
      pwr_type = Pwr.eType_Float32;
      break;
    case Flow.eTraceType_User:
      return;
    }

    this.p = this.ctx.gdh.refObjectInfo( o, pwr_type, 1);
    console.log("connecting", o, this.p);
  }
  this.scan = function() {
    if ( this.p == 0)
      return;

    var v1 = this.ctx.gdh.getObjectRefInfo( this.p);
    var evaluate = true;
    if ( this.first_scan)
      this.first_scan = false;
    else if ( v1 == this.old_value)
      return;

    if ( v1)
      this.highlight = true;
    else
      this.highlight = false;
    
    this.old_value = v1;
    this.redraw = true;
  }

  this.event_handler = function( x, y) {
    var zx = x / this.ctx.zoom_factor + ctx.x_left;
    var zy = y / this.ctx.zoom_factor + ctx.y_low;
    if ( zx >= this.x_left && zx <= this.x_right &&
	 zy >= this.y_low && zy <= this.y_high) {
      console.log("Hit in node");
      if ( this.select) {
	this.select = false;
	this.redraw = true;
      }
      else {
	this.ctx.set_select(false);
	this.set_select(true);
      }
      console.log("Hit in", this.n_name);
      return 1;
    }
    return 0;
  }

  this.set_select = function( select) {
    if ( select != this.select) {
      this.select = select;
      this.redraw = true;
      if ( this.select)
	this.ctx.select_object = this;
      else
	this.ctx.select_object = null;
    }
  }
}

function FlowCtx() {
  this.display_level = Flow.mDisplayLevel_1;
  this.gdh = 0;
  this.debug = false;
  this.zoom_factor = 20.0;
  this.base_zoom_factor = 20.0;
  this.offset_x = 0;
  this.offset_y = 0;
  this.x_right = 0.0;
  this.x_left = 0.0;
  this.y_high = 0.0;
  this.y_low = 0.0;
  this.a = new FlowArray(this);
  this.a_nc = new FlowArray(this);
  this.a_cc = new FlowArray(this);
  this.name = "Claes context";
  this.gdraw = new GDraw(this);
  this.select_object = null;

  this.draw = function() {
    this.a.draw( this.gdraw.gctx, null, null, false);
  }
  this.open = function( lines, row) {
    var end = false;
    for ( var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0]);

      if ( this.debug) console.log( "ctx : " + lines[i]);

      switch ( key) {
	case Flow.eSave_Ctx:
	  break;
	case Flow.eSave_Ctx_zoom_factor:
	  this.zoom_factor = parseFloat(tokens[1]);
	  break;
	case Flow.eSave_Ctx_base_zoom_factor:
	  this.base_zoom_factor = parseFloat(tokens[1]);
	  break;
	case Flow.eSave_Ctx_offset_x:
	  this.offset_x = parseInt(tokens[1]);
	  break;
	case Flow.eSave_Ctx_offset_y:
	  this.offset_y = parseInt(tokens[1]);
	  break;
	case Flow.eSave_Ctx_nav_zoom_factor:
	case Flow.eSave_Ctx_print_zoom_factor:
	case Flow.eSave_Ctx_nav_offset_x:
	case Flow.eSave_Ctx_nav_offset_y:
	  break;
	case Flow.eSave_Ctx_x_right:
	  this.x_right = parseFloat(tokens[1]);
	  break;
	case Flow.eSave_Ctx_x_left:
	  this.x_left = parseFloat(tokens[1]);
	  break;
	case Flow.eSave_Ctx_y_high:
	  this.y_high = parseFloat(tokens[1]);
	  break;
	case Flow.eSave_Ctx_y_low:
	  this.y_low = parseFloat(tokens[1]);
	  break;
	case Flow.eSave_Ctx_nav_rect_ll_x:
	case Flow.eSave_Ctx_nav_rect_ll_y:
	case Flow.eSave_Ctx_nav_rect_ur_x:
	case Flow.eSave_Ctx_nav_rect_ur_y:
	case Flow.eSave_Ctx_nav_rect_hot:
	  break;
	case Flow.eSave_Ctx_name:
	  this.name = tokens[1];
	  break;
	case Flow.eSave_Ctx_user_highlight:
	case Flow.eSave_Ctx_grid_size_x:
	case Flow.eSave_Ctx_grid_size_y:
	case Flow.eSave_Ctx_grid_on:
	case Flow.eSave_Ctx_draw_delta:
	case Flow.eSave_Ctx_refcon_width:
	case Flow.eSave_Ctx_refcon_height:
	case Flow.eSave_Ctx_refcon_textsize:
	case Flow.eSave_Ctx_refcon_linewidth:
	case Flow.eSave_Ctx_user_version:
	  break;
	case Flow.eSave_Ctx_a_nc:
	  i = this.a_nc.open( lines, i+1);
	  break;
	case Flow.eSave_Ctx_a_cc:
	  i = this.a_cc.open( lines, i+1);
	  break;
	case Flow.eSave_Ctx_a:
	  i = this.a.open( lines, i+1);
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:
	  console.log( "Syntax error in FlowCtx", key);
      }
      if ( end)
	break;
    }

    return i;
  };

  this.connect = function() {
    console.log("ctx connect", this.a.size());
    for ( var i = 0; i < this.a.size(); i++)
      this.a.get(i).connect();
  }
  this.scan = function() {
    for ( var i = 0; i < this.a.size(); i++)
      this.a.get(i).scan();
  }

  this.event_handler = function( x, y) {
    var sts = 0;
    for ( var i = 0; i < this.a.size(); i++) {
      if ( this.a.get(i) instanceof FlowNode) {
	sts = this.a.get(i).event_handler( x, y);
	if ( sts == 1)
	  break;
      }
    }
    if ( sts == 1)
      this.draw();
  }

  this.set_select = function( select) {
    for ( var i = 0; i < this.a.size(); i++) {
      if ( this.a.get(i) instanceof FlowNode)
	this.a.get(i).set_select( select);
    }
  }

  this.search_object = function( name) {
    console.log("Searching for ", name);
    var node = this.a.search_by_name( name);
    if ( node != null)
      console.log("Found", name);
    return node;
  }
  this.center_object = function( o) {
    console.log( "center_object", o.pos.x * this.zoom_factor + this.offset_x, window.innerWidth, o.pos.x * this.zoom_factor + this.offset_x- window.innerWidth / 2);
    console.log( "center_object", o.pos.y * this.zoom_factor + this.offset_y, window.innerHeight, o.pos.y * this.zoom_factor + this.offset_y - window.innerHeight / 2);
    window.scrollTo( o.pos.x * this.zoom_factor + this.offset_x - window.innerWidth / 2, 
		     o.pos.y * this.zoom_factor + this.offset_y - window.innerHeight / 2 + this.gdraw.offset_top);    
  }
}

function FlowFrame() {
  var self = this;
  this.ctx = new FlowCtx();
  this.timer = null;
  console.log("ctx: ", this.ctx);
  this.vars_object = null;

  this.readFlowWeb = function( fname, read_cb) {
    var n = window.location.href.lastIndexOf('/');
    var target = window.location.href.substring(0,n+1);
    target += fname;

    console.log("Target: ", target);

    var req = new XMLHttpRequest();
    req.read_cb = read_cb;

    req.open("GET", target, true);
    req.addEventListener( "load", function() {
			    console.log("Done:", req.status);
			    var lines = req.responseText.split('\n');
			    req.read_cb( lines, 0);
			  });
    req.send(null);
  }

  this.read_func = function( lines, row) {
    
    console.log( "this.ctx:", self.ctx);
    self.ctx.open( lines, row);

    self.ctx.gdraw.canvas.addEventListener( "click", function( event) {
				       var y = event.pageY - self.ctx.gdraw.offset_top;
				       var x = event.pageX - 10; // Correction for left offset
				       console.log("Click", x, y);
				       self.ctx.event_handler( x, y);
				     });
    document.getElementById("toolitem1").addEventListener( "click", function( event) {
							     console.log("toolitem3 event");
							   });
    document.getElementById("toolitem2").addEventListener( "click", function( event) {
							     console.log("toolitem1 event, storage");
							     console.log("storage", localStorage.getItem("XttMethodNavigator"));
							     if ( self.ctx.select_object != null) {
								 localStorage.setItem("XttMethodNavigator", self.ctx.select_object.trace_object);
							     }
							     console.log("storage", localStorage.getItem("XttMethodNavigator"));
							     console.log("toolitem2 event window", window.opener);
							     window.opener.focus();
							   });
    document.getElementById("toolitem3").addEventListener( "click", function( event) {
							   });
    document.getElementById("toolitem4").addEventListener( "click", function( event) {
							   });

    self.ctx.gdraw.canvas.width = (self.ctx.x_right - self.ctx.x_left) * self.ctx.zoom_factor;
    self.ctx.gdraw.canvas.height = (self.ctx.y_high - self.ctx.y_low) * self.ctx.zoom_factor;
    self.ctx.gdraw.gctx.translate( -self.ctx.x_left * self.ctx.zoom_factor, -self.ctx.y_low * self.ctx.zoom_factor);
    self.ctx.offset_x -= self.ctx.x_left * self.ctx.zoom_factor;
    self.ctx.offset_y -= self.ctx.y_low * self.ctx.zoom_factor;

    self.ctx.draw();
    console.log("New Gdh");
    self.ctx.gdh = new Gdh( self.flow_open);
    self.ctx.gdh.open_cb = self.flow_open;
    self.ctx.gdh.close_cb = self.flow_close;
    console.log( "ctx.gdh", self.ctx.gdh);
    self.ctx.gdh.init();

    if ( self.vars_object != null) {
      var o = self.ctx.search_object( self.vars_object);
      if ( o != null) {
	self.ctx.center_object( o);
	self.ctx.set_select(false);
	o.set_select(true);
      }
    }    
  }

  this.flow_open = function() {
    console.log("flow_open");
    console.log( "ctx.gdh", self.ctx.gdh);
    self.ctx.connect();
    self.ctx.gdh.refObjectInfoList( self.ctx.gdh.refObjectInfoListReply);  
    self.timer = setTimeout( self.flow_cyclic, 1000);
  }

  this.flow_scan = function() {
    self.ctx.scan();
  }

  this.flow_cyclic = function() {
    self.ctx.gdh.getRefObjectInfoAll( self.flow_scan);
    self.ctx.draw();
    self.timer = setTimeout( self.flow_cyclic, 1000);
  }

  this.flow_close = function() {
    console.log("Close function", self.timer);
    clearTimeout(self.timer);
    for ( var i in self.ctx.gdh.pending)
      delete self.ctx.gdh.pending[i];
  }

  this.get_filename = function() {
    var query = window.location.search.substring(1);

    console.log("query", query);
    var vars = query.split('&');
    console.log("vars", vars.length, vars[0].substring(4), vars[1].substring(4));
    var vid = parseInt(vars[0].substring(4));
    var oix = parseInt(vars[1].substring(4));
    var avid = [];
    var svid = [];
    avid[0] = vid & 0xFF;
    avid[1] = (vid >> 8) & 0xFF;
    avid[2] = (vid >> 16) & 0xFF;
    avid[3] = (vid >> 24) & 0xFF;
    for ( var i = 0; i < 4; i++) {
      svid[i] = avid[i].toString();
      for ( var j = 0; j < 3; j++) {
	if ( svid[i].length < 3)
	  svid[i] = "0" + svid[i];
      }
    }
    var soix = oix.toString(16);
    for ( var j = 0; j < 8; j++) {
      if ( soix.length < 8)
	soix = "0" + soix;
    }

    var fname = "pwr_" + svid[3] + "_" + svid[2] + "_" + svid[1] + "_" + svid[0] + "_" + soix + ".flw";
    console.log( fname);

    if ( vars.length > 2) {
      this.vars_object = vars[2].substring(4);
      console.log("vars object: ", this.vars_object);
    }

    return fname;
  }    
}

var frame = new FlowFrame();

// readFlow( "t.flw", read_func);
frame.readFlowWeb( frame.get_filename(), frame.read_func);
console.log( "eSave", Flow.eSave_Ctx);
