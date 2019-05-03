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
