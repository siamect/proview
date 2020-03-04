"use strict";

#jsc_include pwr.jsi
#jsc_include cli.jsi
#jsc_include gdh.jsi
#jsc_include plow.jsi



var GraphIfc = {
  eType_Bit 		: (1 << 15) + 1
};


/** Start Xtt **/
function XttOpenChildrenData( node, open_next) {
  this.node = node;
  this.open_next = open_next;
}

function Xtt() {
  this.ctx = null;
  this.ncObject = null;
  this.scan_update;
  this.priv = 0;
  var self = this;
  

  this.init = function() {
    this.priv = sessionStorage.getItem("pwr_privilege");
    console.log("pwr_privilege" ,this.priv);

    this.methods_init();
    this.ctx = new PlowCtx();
    this.ctx.event_cb = this.plow_event;
    this.createNodeClasses();

    this.ctx.gdh = new Gdh();
    this.ctx.gdh.open_cb = this.gdh_init_cb;
    this.ctx.gdh.init()

    this.ctx.gdraw.canvas.addEventListener( "click", function( event) {
					      var y = event.pageY - self.ctx.gdraw.offset_top;
					      var x = event.pageX;
					      if ( event.shiftKey)
						xtt.ctx.event_handler( Plow.eEvent_MB1ClickShift, x, y);
					      else
						xtt.ctx.event_handler( Plow.eEvent_MB1Click, x, y);
					    });
    document.addEventListener( "keydown", function( event) {
				 if ( event.keyCode == 40) {
				   self.ctx.event_handler( Plow.eEvent_Key_Down);
				   event.preventDefault();
				 }
				 else if ( event.keyCode == 39) {
				   if ( event.shiftKey)
				     self.ctx.event_handler( Plow.eEvent_Key_ShiftRight);
				   else
				     self.ctx.event_handler( Plow.eEvent_Key_Right);
				   event.preventDefault();
				 }
				 else if ( event.keyCode == 37) {
				   self.ctx.event_handler( Plow.eEvent_Key_Left);
				   event.preventDefault();
				 }
				 else if ( event.keyCode == 38) {
				   self.ctx.event_handler( Plow.eEvent_Key_Up);
				   event.preventDefault();
				 }
				 else if ( event.keyCode == 82) {
				   if ( event.ctrlKey)
				     self.ctx.event_handler( Plow.eEvent_Key_CtrlR);
				   event.preventDefault();
				 }
				 else if ( event.keyCode == 76) {
				   if ( event.ctrlKey)
				     self.ctx.event_handler( Plow.eEvent_Key_CtrlL);
				   event.preventDefault();
				 }
				 else if ( event.keyCode == 71) {
				   if ( event.ctrlKey)
				     self.ctx.event_handler( Plow.eEvent_Key_CtrlG);
				   event.preventDefault();
				 }
			       });
    document.getElementById("toolitem1").addEventListener( "click", function( event) {
							     var o = self.ctx.get_select();
							     if ( o.userdata instanceof XttItemObject) {
							       var newwindow = window.open("", "_blank");
							       self.ctx.gdh.getObject( o.userdata.objid, GdhOp.GET_OP_METHOD_GRAPH, self.open_graph_cb, newwindow);
							     }
 							     console.log("toolitem1 event");
							   });
    document.getElementById("toolitem2").addEventListener( "click", function( event) {
							     var o = self.ctx.get_select();
							     if ( o.userdata instanceof XttItemObject) {
							       var newwindow = window.open("", "_blank");
							       self.ctx.gdh.getObject( o.userdata.objid, GdhOp.GET_OP_METHOD_OBJECTGRAPH, self.open_objectgraph_cb, newwindow);
							     }
 							     console.log("toolitem2 event");
							   });
    document.getElementById("toolitem3").addEventListener( "click", function( event) {
							     console.log("toolitem1 event");
							     var o = self.ctx.get_select();
							     if ( o.userdata instanceof XttItemObject) {
							       var newwindow = window.open("", "_blank");
							       self.ctx.gdh.getObject( o.userdata.objid, GdhOp.GET_OP_METHOD_PLC, self.open_plc_cb, newwindow);
							     }
							     else if ( o.userdata instanceof XttItemCrr) {
							       var idx = o.userdata.name.lastIndexOf('-');
							       var ostring = "";
							       if ( idx != -1) {
								 ostring = "&obj=" + o.userdata.name.substring(idx+1);
							       }	  
							       console.log("flow.html?vid=" + o.userdata.objid.vid + "&oix=" + o.userdata.objid.oix + ostring);
							       window.open("flow.html?vid=" + o.userdata.objid.vid + "&oix=" + o.userdata.objid.oix + ostring);
							     }
 							     console.log("toolitem3 event");
							   });
    document.getElementById("toolitem4").addEventListener( "click", function( event) {
							     console.log("toolitem4 event");
							   });
    document.getElementById("toolitem5").addEventListener( "click", function( event) {
							     console.log("toolitem5 event");
							     var o = self.ctx.get_select();
							     if ( o == null)
							       return;
							     var item = o.userdata;
							     item.open_attributes( self);
							   });
    document.getElementById("toolitem6").addEventListener( "click", function( event) {
							     var o = self.ctx.get_select();
							     self.ctx.gdh.crrSignal( o.userdata.objid, self.open_crr_cb, o);
							     console.log("toolitem6 event");
							   });
    document.getElementById("toolitem7").addEventListener( "click", function( event) {
							     console.log("toolitem7 event");
							   });
    document.getElementById("toolitem8").addEventListener( "click", function( event) {
							     console.log("toolitem8 event");
							     var o = self.ctx.get_select();
							     if ( o == null)
							       return;
							     if ( o.userdata instanceof XttItemObject) {
							       var newwindow = window.open("", "_blank");
							       self.ctx.gdh.getObject( o.userdata.objid, GdhOp.GET_OP_METHOD_HELPCLASS, self.open_helpclass_cb, newwindow);
							     }
							   });

    window.addEventListener( "storage", function( event) {			       
			       if ( event.newValue == "")
				 return;
			       localStorage.setItem( "XttMethodNavigator", "");
			       self.display_object( event.newValue);
			     });

  }

  this.is_authorized = function( access) {
    return ( this.priv & access) ? true : false;
  }

  this.gdh_init_cb = function() {
    if ( self.priv == null)
      self.ctx.gdh.login( "", "", self.login_cb, self);

    var oid = new PwrtObjid( 0, 0);
    self.ctx.gdh.getAllXttChildren( oid, self.open_children_cb, new XttOpenChildrenData(null,null));

    self.ctx.gdh.listSent = true;
    self.trace_cyclic();
  }

  this.login_cb = function( id, data, sts, result) {
    console.log( "Login:", sts, result);
    if ( sts & 1)
      self.priv = result;
    else
      self.priv = 0;
  };

  this.open_children_cb = function( id, data, sts, result) {
    self.ctx.set_nodraw();
    for ( var i = 0; i < result.length; i++) {
      if ( data.node == null) {
	result[i].full_name = result[i].name;
	new XttItemObject( self, result[i], null, Plow.DEST_AFTER);
      }
      else {
	result[i].full_name = data.node.userdata.full_name + "-" + result[i].name;
	new XttItemObject( self, result[i], data.node, Plow.DEST_INTOLAST);
      }
    }
    self.ctx.configure();

    if ( data.open_next != null) {
      if ( data.open_next.length == 0) {
	self.ctx.reset_nodraw();
	return;
      }
      var child = self.ctx.a.get_first_child( data.node);
      while ( child != null) {
	if ( child.userdata.name == data.open_next[0]) {
	  if ( data.open_next.length == 1) {
	    child.set_select( true);
	    child.set_invert( true);
	    if ( !self.ctx.is_visible( child))
	      self.ctx.scroll( child.y_low, 0.50);
	    window.focus(); // Doesn't work
	  }
	  else {
	    data.open_next.splice( 0, 1);
	    if ( data.open_next[0] == '.') {
	      data.open_next.splice( 0, 1);
	      child.userdata.open_attributes( self, data.open_next);
	    }
	    else
	      child.userdata.open_children( self, data.open_next);
	  }
	  break;
	}
	child = self.ctx.a.get_next_sibling( child);
      }
    }

    self.ctx.reset_nodraw();
    self.ctx.draw();
  }

  this.open_attributes_cb = function( id, data, sts, result) {
    self.ctx.set_nodraw();
    for ( var i = 0; i < result.length; i++) {      
      result[i].objid = data.node.userdata.objid;
      result[i].full_name = data.node.userdata.full_name + "." + result[i].name;
      if ( (result[i].flags & Pwr.mAdef_array) != 0)
	new XttItemAttrArray( self, result[i], data.node, Plow.DEST_INTOLAST);
      else if ( (result[i].flags & Pwr.mAdef_class) != 0)
	new XttItemAttrObject( self, result[i], data.node, Plow.DEST_INTOLAST);
      else
	new XttItemAttr( self, result[i], data.node, Plow.DEST_INTOLAST);
    }

    self.ctx.configure();

    if ( data.open_next != null) {
      if ( data.open_next.length == 0) {
	self.ctx.reset_nodraw();
	return;
      }
      var child = self.ctx.a.get_first_child( data.node);
      while ( child != null) {
	if ( child.userdata.name == data.open_next[0]) {
	  if ( data.open_next.length == 1) {
	    child.set_select( true);
	    child.set_invert( true);
	    if ( !self.ctx.is_visible( child))
	      self.ctx.scroll( child.y_low, 0.50);
	    window.focus(); // Doesn't work
	  }
	  else {
	    data.open_next.splice( 0, 1);
	    child.userdata.open_attributes( self, data.open_next);
	  }
	  break;
	}
	child = self.ctx.a.get_next_sibling( child);
      }
    }

    self.ctx.reset_nodraw();
    self.ctx.draw();
  }

  this.open_plc_cb = function( id, data, sts, result) {
    if ( (sts & 1) != 0) {
      var param1;
      if ( result.param1 !== "")
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

  this.open_objectgraph_cb = function( id, data, sts, result) {
    if ( (sts & 1) != 0) {
      data.location.href = "ge.html?graph=" + result.param1 + "&instance=" + result.fullname;
      data.document.title = result.fullname;
    }
    else
      data.document.write("Error status " + sts);
  }

  this.open_graph_cb = function( id, data, sts, result) {
    if ( (sts & 1) != 0) {
      var idx = result.param1.indexOf('.');
      if ( idx != -1)
	result.param1 = result.param1.substring(0, idx);

      var instancestr = "";
      if ( result.fullname !== "")
	instancestr = "&instance=" + result.fullname;

      data.location.href = "ge.html?graph=" + result.param1 + instancestr;
      data.document.title = result.param1;
    }
    else
      data.document.write("Error status " + sts);
  }

  this.open_crr_cb = function( id, node, sts, crrdata) {
    if ( (sts & 1) == 0) {
      return;
    }
    node.userdata.open_crossreferences( self, crrdata);
  }

  this.open_helpclass_cb = function( id, data, sts, result) {
    if ( (sts & 1) != 0) {
      console.log("open_helpclass", result.param1);
      var url = location.protocol + "//" + location.host + result.param1;
      data.location.href = url;
    }
    else
      data.document.write("Error status " + sts);
  }

  this.plow_event = function( event, object, x, y) {

    var item = null;
    if ( object != null)
      item = object.get_userdata();

    switch ( event) {
    case Plow.eEvent_ObjectDeleted:
      if ( object.userdata instanceof XttItemAttr) {
	object.userdata.scan_close( self);
      }
      break;
    case Plow.eEvent_MB1Click:
      if ( object.in_icon( x, y)) {
	item.open_children( self, null);
      }
      else {
	if ( object.select) {
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
      }
      break;
    case Plow.eEvent_MB1ClickShift:
      if ( object.in_icon( x, y)) {
	item.open_attributes( self, null);
      }
      break;
    case Plow.eEvent_Key_Down: {
      var o = self.ctx.get_select();
      if ( o != null) {
	var next = self.ctx.get_next_object( o);
	if ( next != null) {
	  o.set_select(false);
	  o.set_invert(false);
	  next.set_select(true);
	  next.set_invert(true);
	  if ( !self.ctx.is_visible( next))
	    self.ctx.scroll( next.y_low, 0.10);
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
      if ( o != null) {
	var next = self.ctx.get_previous_object( o);
	if ( next != null) {
	  o.set_select(false);
	  o.set_invert(false);
	  next.set_select(true);
	  next.set_invert(true);
	  o.draw( self.ctx.gdraw.gctx, null, null, 0);
	  next.draw( self.ctx.gdraw.gctx, null, null, 0);
	  if ( !self.ctx.is_visible( next))
	    self.ctx.scroll( next.y_low, 0.90);
	}
      }
	
      break;
    }
    case Plow.eEvent_Key_Right: {
      var o = self.ctx.get_select();
      if ( o != null) {
	item = o.userdata;
	item.open_children( self, null);
      }	
      break;
    }
    case Plow.eEvent_Key_Left: {
      var o = self.ctx.get_select();
      if ( o != null) {
	item = o.userdata;
	item.close( self);
      }	
      break;
    }
    case Plow.eEvent_Key_ShiftRight: {
      var o = self.ctx.get_select();
      if ( o != null) {
	item = o.userdata;
	item.open_attributes( self, null);
      }	
      break;
    }
    case Plow.eEvent_Key_CtrlR: {
      var o = self.ctx.get_select();
      if ( o != null)
	self.ctx.gdh.crrSignal( o.userdata.objid, self.open_crr_cb, o);
      break;
    }
    case Plow.eEvent_Key_CtrlL: {
      var o = self.ctx.get_select();
      if ( o.userdata instanceof XttItemObject) {
	var newwindow = window.open("", "_blank");
	self.ctx.gdh.getObject( o.userdata.objid, GdhOp.GET_OP_METHOD_PLC, self.open_plc_cb, newwindow);
      }
      else if ( o.userdata instanceof XttItemCrr) {
	var idx = o.userdata.name.lastIndexOf('-');
	var ostring = "";
	if ( idx != -1) {
	  ostring = "&obj=" + o.userdata.name.substring(idx+1);
	}	  
	window.open("flow.html?vid=" + o.userdata.objid.vid + "&oix=" + o.userdata.objid.oix + ostring);
      }
      break;
    }
    case Plow.eEvent_Key_CtrlG: {
      var o = self.ctx.get_select();
      if ( o.userdata instanceof XttItemObject) {
	console.log("CtrlG", o.userdata.objid.vid, o.userdata.objid.oix);
	var newwindow = window.open("", "_blank");
	self.ctx.gdh.getObject( o.userdata.objid, GdhOp.GET_OP_METHOD_OBJECTGRAPH, self.open_objectgraph_cb, newwindow);
      }
      break;
    }
    }
  }

  this.createNodeClasses = function() {
    var r1 = new PlowRect( this.ctx, 0, 0, 50, 1.0, Plow.COLOR_WHITE, Plow.COLOR_WHITE, true, false);
    var a1 = new PlowAnnot(this.ctx, 3, 0.9, 4, Plow.COLOR_BLACK, Plow.NEXT_RELATIVE_POSITION, 0);
    var a11 = new PlowAnnot(this.ctx, 7.5, 0.9, 4, Plow.COLOR_BLACK, Plow.RELATIVE_POSITION, 1);
    var a12 = new PlowAnnot(this.ctx, 12, 0.9, 4, Plow.COLOR_BLACK, Plow.RELATIVE_POSITION, 2);
    var p1 = new PlowAnnotPixmap(this.ctx, 0.4, 0.2 , 0);

    this.ncObject = new PlowNodeClass( this.ctx);
    this.ncObject.insert(r1);
    this.ncObject.insert(a1);
    this.ncObject.insert(a11);
    this.ncObject.insert(a12);
    this.ncObject.insert(p1);
    this.ctx.insert_nc( this.ncObject);
  }

  this.methods_init = function() {
    localStorage.setItem("XttMethodNavigator", "");
  }

  this.collapse = function() {
    this.ctx.set_nodraw();
    for ( var i = 0; i < this.ctx.a.size(); i++) {
      var node = this.ctx.a.get(i);
      if ( node.level == 0)
	node.userdata.close( this);
    }
    this.ctx.reset_nodraw();
    this.ctx.draw();
  }

  this.display_object = function( name) {    
    var i = name.indexOf('.');
    var attr = null;
    var path;
    if ( i != -1) {
      attr = name.substring(i+1);
      name = name.substring(0,i);      
      var p1 = name.split('-');
      var p2 = attr.split('.');
      path = new Array(p1.length + p2.length + 1);
      var j;
      for ( j = 0; j < p1.length; j++)
	path[j] = p1[j];
      path[p1.length] = '.';
      for ( j = 0; j < p2.length; j++)
	path[j+p1.length+1] = p2[j];
    }
    else
      path = name.split('-');

    var idx = 0;

    this.collapse();
    for ( var j = idx; j < this.ctx.a.size(); j++) {
      if ( this.ctx.a.get(j).userdata.name == path[0]) {
	if ( j == this.ctx.a.size() - 1) {
	  var node = this.ctx.a.get(j); 
	  node.set_select( true);
	  node.set_invert( true);
	  if ( !this.ctx.is_visible( node))
	    this.ctx.scroll( node.y_low, 0.50);
	  window.focus(); // Doesn't work
	}
	else {
	  path.splice( 0, 1);
	  this.ctx.a.get(j).userdata.open_children( this, path);
	}
	break;
      }
    }
  }

  this.trace_cyclic = function() {
    self.ctx.gdh.getRefObjectInfoAll( self.trace_scan);
  };

  this.trace_scan = function( id, sts) {
    self.scan_update = false;
    for ( var i = 0; i < self.ctx.a.size(); i++) {
      var item = self.ctx.a.get(i).userdata;
      if ( item instanceof XttItemAttr) {
	item.scan( self);
      }
    }
    if ( self.scan_update)
      self.ctx.draw();
    self.timer = setTimeout( self.trace_cyclic, 1000);
  };

  this.openValueInputDialog = function( item, text) {
    console.log( "priv acc", this.priv, Pwr.mAccess_RtWrite | Pwr.mAccess_System);
    if ( this.is_authorized( Pwr.mAccess_RtWrite | Pwr.mAccess_System)) {
      var value = prompt( text, "");
      if ( value !== null)
	item.set_value( this, value);
    }
    else
      alert( "Not authorized for this operation");
  };
}

function XttItemObject( xtt, object_info, destination, destCode) {
  this.objid = object_info.objid;
  this.cid = object_info.cid;
  this.name = object_info.name;
  this.full_name = object_info.full_name;
  this.has_children = object_info.has_children;
  this.node = new PlowNode(xtt.ctx, xtt.ncObject, 0);
  this.node.set_userdata(this);
  this.node.set_annotation( 0, object_info.name);
  this.node.set_annotation( 1, object_info.classname);
  this.node.set_annotation( 2, object_info.description);
  this.node.set_annotation_pixmap( 0, Bitmaps.map);
  xtt.ctx.insertNode( this.node, destination, destCode);
  if ( object_info.has_children)
    this.node.set_annotation_pixmap( 0, Bitmaps.map);      
  else
    this.node.set_annotation_pixmap( 0, Bitmaps.leaf);      

  this.open_children = function( xtt, open_next) {
    if ( this.node.node_open != 0)
      this.close( xtt);
    else if ( !this.has_children)
      this.open_attributes( xtt, null);
    else {
      xtt.ctx.gdh.getAllXttChildren( this.objid, xtt.open_children_cb, new XttOpenChildrenData(this.node,open_next));
      this.node.node_open |= Plow.OPEN_CHILDREN;
      this.node.set_annotation_pixmap( 0, Bitmaps.openmap);      
    }
  }

  this.open_attributes = function( xtt, open_next) {
    if ( this.node.node_open != 0)
      this.close( xtt);
    else {
      xtt.ctx.gdh.getAllClassAttributes( this.cid, this.objid, xtt.open_attributes_cb, new XttOpenChildrenData(this.node,open_next));

      this.node.node_open |= Plow.OPEN_ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    }
  }

  this.open_crossreferences = function( xtt, crrdata) {
    if ( this.node.node_open != 0)
      this.close( xtt);
    else {
      for ( var i = 0; i < crrdata.length; i++) {
	new XttItemCrr( xtt, crrdata[i], this.node, Plow.DEST_INTOLAST);
      }

      this.node.node_open |= Plow.OPEN_CROSSREFERENCES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    }
  }

  this.close = function( xtt) {
    if ( this.node.node_open & Plow.OPEN_CHILDREN) {
      xtt.ctx.close_node( this.node);
      this.node.node_open &= ~Plow.OPEN_CHILDREN;
      this.node.set_annotation_pixmap( 0, Bitmaps.map);      
      xtt.ctx.configure();
      xtt.ctx.draw();
    }
    else if ( this.node.node_open & Plow.OPEN_ATTRIBUTES) {
      xtt.ctx.close_node( this.node);
      this.node.node_open &= ~Plow.OPEN_ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    }
    else if ( this.node.node_open & Plow.OPEN_CROSSREFERENCES) {
      xtt.ctx.close_node( this.node);
      this.node.node_open &= ~Plow.OPEN_CROSSREFERENCES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    }
    else {
      var parent = xtt.ctx.get_parent_object( this.node);
      if ( parent != null) {
	parent.userdata.close(xtt);
	parent.set_select( true);
	parent.set_invert( true);
      }
    }
  }

}

function XttItemAttr( xtt, info, destination, destCode) {
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
  this.node.set_annotation( 0, this.name);
  this.node.set_annotation_pixmap( 0, Bitmaps.attr);
  xtt.ctx.insertNode( this.node, destination, destCode);

  this.refid = xtt.ctx.gdh.refObjectInfo( this.full_name, info.type, 1);

  this.open_children = function( xtt, open_next) {
    xtt.openValueInputDialog( this, "Enter value");    
  };

  this.set_value = function( xtt, value) {
    var sts;

    switch ( this.type) {
    case Pwr.eType_Float32:
      var inputValue = parseFloat( value.trim());
      sts = xtt.ctx.gdh.setObjectInfoFloat( this.full_name, inputValue);
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
      var inputValue = parseInt( value.trim(), 10);
      sts = xtt.ctx.gdh.setObjectInfoInt( this.full_name, inputValue);
      break;
    case Pwr.eType_String:
    case Pwr.eType_Time:
    case Pwr.eType_DeltaTime:
    case Pwr.eType_AttrRef:
    case Pwr.eType_Objid:
      sts = xtt.ctx.gdh.setObjectInfoString( this.full_name, value);
      break;
    default:
      break;
    }
  };

  this.open_attributes = function( xtt, open_next) {
  }

  this.close = function( xtt) {
    var parent = xtt.ctx.get_parent_object( this.node);
    if ( parent != null) {
      parent.userdata.close( xtt);
      parent.set_select( true);
      parent.set_invert( true);
    }
  }

  this.scan = function( xtt) {
    if ( !this.refid)
      return;
    var value = xtt.ctx.gdh.getObjectRefInfo( this.refid);
    if ( this.firstScan || value != this.old_value) {
      var value_str;

      switch ( this.type) {
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
      this.node.set_annotation( 1, value_str);
      xtt.scan_update = true;
    }
    this.firstScan = false;
  }
  this.scan_close = function( xtt) {
    xtt.ctx.gdh.unrefObjectInfo( this.refid);
  };
}

function XttItemAttrArray( xtt, info, destination, destCode) {
  this.name = info.name;
  this.objid = info.objid;
  this.full_name = info.full_name;
  this.type = info.type;
  this.flags = info.flags;
  this.size = info.size;
  this.elements = info.elements;
  this.node = new PlowNode(xtt.ctx, xtt.ncObject, 0);
  this.node.set_userdata(this);
  this.node.set_annotation( 0, this.name);
  this.node.set_annotation_pixmap( 0, Bitmaps.attrarra);
  xtt.ctx.insertNode( this.node, destination, destCode);

  this.open_children = function( xtt, open_next) {    
    this.open_attributes( xtt, open_next);
  }

  this.open_attributes = function( xtt, open_next) {
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
    for ( var i = 0; i < this.elements; i++) {
      info.name = this.name + "[" + i + "]";
      info.full_name = this.full_name + "[" + i + "]";
      if ( (info.flags & Pwr.mAdef_array) != 0)
	new XttItemAttrArray( xtt, info, this.node, Plow.DEST_INTOLAST);
      else if ( (info.flags & Pwr.mAdef_class) != 0)
	new XttItemAttrObject( xtt, info, this.node, Plow.DEST_INTOLAST);
      else
	new XttItemAttr( xtt, info, this.node, Plow.DEST_INTOLAST);
    }
    this.node.node_open |= Plow.OPEN_ATTRIBUTES;
    xtt.ctx.configure();
    xtt.ctx.reset_nodraw();
    xtt.ctx.draw();
  };

  this.close = function( xtt) {
    if ( this.node.node_open & Plow.OPEN_ATTRIBUTES) {
      xtt.ctx.close_node( this.node);
      this.node.node_open &= ~Plow.OPEN_ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    }
    else {
      var parent = xtt.ctx.get_parent_object( this.node);
      if ( parent != null) {
	parent.userdata.close( xtt);
	parent.set_select( true);
	parent.set_invert( true);
      }
    }
  };

}

function XttItemAttrObject( xtt, info, destination, destCode) {
  this.name = info.name;
  this.classname = info.classname;
  this.objid = info.objid;
  this.full_name = info.full_name;
  this.cid = info.type;
  this.flags = info.flags;
  this.size = info.size;
  this.node = new PlowNode(xtt.ctx, xtt.ncObject, 0);
  this.node.set_userdata(this);
  this.node.set_annotation( 0, this.name);
  this.node.set_annotation( 1, this.classname);
  this.node.set_annotation_pixmap( 0, Bitmaps.object);
  xtt.ctx.insertNode( this.node, destination, destCode);

  this.open_children = function( xtt, open_next) {
    this.open_attributes( xtt, null);
  }

  this.open_attributes = function( xtt, open_next) {
    if ( this.node.node_open != 0)
      this.close( xtt);
    else {
      xtt.ctx.gdh.getAllClassAttributes( this.cid, this.objid, xtt.open_attributes_cb, new XttOpenChildrenData(this.node,open_next))

      this.node.node_open |= Plow.OPEN_ATTRIBUTES;
    }
  }

  this.close = function( xtt) {
    if ( this.node.node_open & Plow.OPEN_ATTRIBUTES) {
      xtt.ctx.close_node( this.node);
      this.node.node_open &= ~Plow.OPEN_ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    }
    else if ( this.node.node_open & Plow.OPEN_CROSSREFERENCES) {
      xtt.ctx.close_node( this.node);
      this.node.node_open &= ~Plow.OPEN_CROSSREFERENCES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    }
    else {
      var parent = xtt.ctx.get_parent_object( this.node);
      if ( parent != null) {
	parent.userdata.close( xtt);
	parent.set_select( true);
	parent.set_invert( true);
      }
    }
  };
}

function XttItemCrr( xtt, info, destination, destCode) {
  this.name = info.name;
  this.classname = info.classname;
  this.objid = info.objid;
  this.type = info.type;
  this.node = new PlowNode(xtt.ctx, xtt.ncObject, 0);
  this.node.set_userdata(this);
  this.node.set_annotation( 0, this.name);
  this.node.set_annotation( 1, this.classname);

  switch ( this.type) {
  case 0:
    this.node.set_annotation_pixmap( 0, Bitmaps.crrread);
    break;
  case 1:
    this.node.set_annotation_pixmap( 0, Bitmaps.crrwrite);
    break;
  case 2:
    this.node.set_annotation_pixmap( 0, Bitmaps.crrwrite);
    // this.node.set_annotation_pixmap( 1, Bitmaps.crrread);
    break;
  }
  xtt.ctx.insertNode( this.node, destination, destCode);

  this.close = function( xtt) {
    var parent = xtt.ctx.get_parent_object( this.node);
    if ( parent != null) {
      parent.userdata.close( xtt);
      parent.set_select( true);
      parent.set_invert( true);
    }
  }
}


var xtt = new Xtt();
xtt.init();

/** End Xtt **/
