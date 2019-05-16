"use strict";

enum EvType {
  AlarmList,
  EventList,
  BlockList
}

class Ev {
  ctx: PlowCtx;
  ncAlarmA: PlowNodeClass;
  ncAlarmB: PlowNodeClass;
  ncAlarm: PlowNodeClass;
  ncInfo: PlowNodeClass;
  ncSuccess: PlowNodeClass;
  scan_update: boolean;
  priv: number;
  mhSyncIdx = 0;
  maxEve = 30;
  type: EvType;
  timer: number;

  constructor() {
    this.type = this.get_type();
    if (this.type === EvType.EventList) {
      document.title = "Event List";
    }

    this.priv = Number(sessionStorage.getItem("pwr_privilege"));
    console.log("pwr_privilege", this.priv);

    this.methods_init();
    this.ctx = new PlowCtx();
    this.ctx.event_cb = this.plow_event;
    this.createNodeClasses();

    this.ctx.gdh = new Gdh(this.gdh_init_cb);

    this.ctx.gdraw.canvas.addEventListener("click", function (event) {
      let y = event.pageY - this.ctx.gdraw.offset_top;
      let x = event.pageX;
      if (event.shiftKey) {
        ev.ctx.event_handler(Event.MB1ClickShift, x, y);
      } else {
        ev.ctx.event_handler(Event.MB1Click, x, y);
      }
    });
    document.addEventListener("keydown", function (event) {
      if (event.keyCode === 40) {
        this.ctx.event_handler(Event.Key_Down);
        event.preventDefault();
      } else if (event.keyCode === 39) {
        if (event.shiftKey) {
          this.ctx.event_handler(Event.Key_ShiftRight);
        } else {
          this.ctx.event_handler(Event.Key_Right);
        }
        event.preventDefault();
      } else if (event.keyCode === 37) {
        this.ctx.event_handler(Event.Key_Left);
        event.preventDefault();
      } else if (event.keyCode === 38) {
        this.ctx.event_handler(Event.Key_Up);
        event.preventDefault();
      } else if (event.keyCode === 82) {
        if (event.ctrlKey) {
          this.ctx.event_handler(Event.Key_CtrlR);
        }
        event.preventDefault();
      } else if (event.keyCode === 76) {
        if (event.ctrlKey) {
          this.ctx.event_handler(Event.Key_CtrlL);
        }
        event.preventDefault();
      } else if (event.keyCode === 71) {
        if (event.ctrlKey) {
          this.ctx.event_handler(Event.Key_CtrlG);
        }
        event.preventDefault();
      }
    });
    // Ack
    document.getElementById("toolitem1")
        .addEventListener("click", function (event) {
          console.log("Ack");
          this.ack();
        });
    // Navigator sup object
    document.getElementById("toolitem2")
        .addEventListener("click", function (event) {
          let o = this.ctx.get_select();
          if (o === null || typeof o === 'undefined') {
            return;
          }
          console.log("toolitem2", o.userdata.e.supObject.vid,
              o.userdata.e.supObject.oix);
          this.ctx.gdh.getObjectFromAref(o.userdata.e.supObject,
              GdhOp.GET_OP_SELF).then(this.open_navigator_cb);
          console.log("toolitem2 event");
        });
    // Trace sup object
    document.getElementById("toolitem3")
        .addEventListener("click", function (event) {
          console.log("toolitem5 event");
          let o = this.ctx.get_select();
          if (o === null) {
            return;
          }
          this.ctx.gdh.getObjectFromAref(o.userdata.e.supObject,
              GdhOp.GET_OP_METHOD_PLC).then(this.open_plc_cb);
        });
    // Graph event name
    document.getElementById("toolitem4")
        .addEventListener("click", function (event) {
          let o = this.ctx.get_select();
          if (o.userdata instanceof EvItemAlarm) {
            this.ctx.gdh.getObjectFromName(o.userdata.e.eventName,
                GdhOp.GET_OP_METHOD_GRAPH).then(this.open_graph_cb);
          }
        });
    // Object raph event name
    document.getElementById("toolitem5")
        .addEventListener("click", function (event) {
          let o = this.ctx.get_select();
          if (o.userdata instanceof EvItemAlarm) {
            this.ctx.gdh.getObjectFromName(o.userdata.e.eventName,
                GdhOp.GET_OP_METHOD_OBJECTGRAPH).then(this.open_graph_cb);
          }
        });
    // Navigator event name
    document.getElementById("toolitem6")
        .addEventListener("click", function (event) {
          let o = this.ctx.get_select();
          console.log("storage", localStorage.getItem("XttMethodNavigator"));
          localStorage.setItem("XttMethodNavigator", o.userdata.e.eventName);
          console.log("storage", localStorage.getItem("XttMethodNavigator"));
          console.log("toolitem6 event window", window.opener);
          window.opener.focus();
        });
    // Trace event name
    document.getElementById("toolitem7")
        .addEventListener("click", function (event) {
          let o = this.ctx.get_select();
          if (o === null) {
            return;
          }
          this.ctx.gdh.getObjectFromName(o.userdata.e.eventName,
              GdhOp.GET_OP_METHOD_PLC).then(this.open_plc_cb);
          console.log("toolitem7 event");
        });
    // History event name
    document.getElementById("toolitem8")
        .addEventListener("click", function (event) {
          console.log("toolitem8 event");
        });
    // Object event name
    document.getElementById("toolitem9")
        .addEventListener("click", function (event) {
          console.log("toolitem9 event");
          let o = this.ctx.get_select();
          if (o === null) {
            return;
          }
          let item = o.userdata;
          item.open_attributes(this);
        });
    // Crossref event name
    document.getElementById("toolitem10")
        .addEventListener("click", function (event) {
          let o = this.ctx.get_select();
          if (o === null) {
            return;
          }
          this.ctx.gdh.crrSignal(o.userdata.e.eventName).then(this.open_crr_cb(o));
          console.log("toolitem10 event");
        });
    // Help event name
    document.getElementById("toolitem11")
        .addEventListener("click", function (event) {
          console.log("toolitem11 event");
        });
    // Class help event name
    document.getElementById("toolitem12")
        .addEventListener("click", function (event) {
          console.log("toolitem12 event");
          let o = this.ctx.get_select();
          if (o === null) {
            return;
          }
          if (o.userdata instanceof EvItemAlarm) {
            this.ctx.gdh.getObjectFromName(o.userdata.e.eventName,
                GdhOp.GET_OP_METHOD_HELPCLASS).then(this.open_helpclass_cb);
          }
        });
  }

  is_authorized(access) {
    return !!(this.priv & access);
  }

  gdh_init_cb() {
    if (!this.priv) {
      this.ctx.gdh.login("", "").then(this.login_cb);
    }

    //this.ctx.gdh.mhSync(this.mhSyncIdx).then(this.sync_cb);

    this.ctx.gdh.listSent = true;
    this.trace_cyclic();
  }

  login_cb(res) {
    console.log("Login:", res.sts, res.value);
    this.priv = (res.sts & 1) ? res.value : 0;
  }

  sync_cb(res) {
    if (!(res.sts & 1)) {
      return;
    }

    let result = res.value;
    if (result.length === 0) {
      return;
    }

    if (this.type === EvType.AlarmList) {
      this.ctx.set_nodraw();
      for (let i = result.length - 1; i >= 0; i--) {
        let e = result[i];
        switch (e.eventType) {
          case Event.Alarm:
          case Event.MaintenanceAlarm:
          case Event.SystemAlarm:
          case Event.UserAlarm1:
          case Event.UserAlarm2:
          case Event.UserAlarm3:
          case Event.UserAlarm4:
          case Event.Info:
          case Event.InfoSuccess:
            if (this.ctx.size() >= this.maxEve) {
              let o = this.ctx.get_last_object();
              this.ctx.remove(o);
            }

            new EvItemAlarm(this, e, null, Dest.BEFORE);
            break;
          case Event.Ack:
            let item = this.find_event(e.targetId);
            if (item !== null) {
              if (item.e.eventStatus & EventStatus.NotRet) {
                item.e.eventStatus &= ~EventStatus.NotAck;
                item.update();
              } else {
                this.ctx.remove(item.node);
              }
            }
            break;
          case Event.Return:
            let item = this.find_event(e.targetId);
            if (item !== null) {
              if (item.e.eventStatus & EventStatus.NotAck) {
                item.e.eventStatus &= ~EventStatus.NotRet;
                item.update();
              } else {
                this.ctx.remove(item.node);
              }
            }
            break;
        }
        this.mhSyncIdx = Math.max(this.mhSyncIdx, result[i].syncIdx);
      }
      this.ctx.configure();
      this.ctx.reset_nodraw();
      this.ctx.draw();
    } else if (this.type === EvType.EventList) {
      this.ctx.set_nodraw();
      for (let i = result.length - 1; i >= 0; i--) {
        let e = result[i];
        if (this.ctx.size() >= this.maxEve) {
          let o = this.ctx.get_last_object();
          this.ctx.remove(o);
        }

        new EvItemAlarm(this, e, null, Dest.BEFORE);
        this.mhSyncIdx = Math.max(this.mhSyncIdx, result[i].syncIdx);
      }
      this.ctx.configure();
      this.ctx.reset_nodraw();
      this.ctx.draw();
    }
  }

  find_event(event_id) {
    for (let o = this.ctx.get_first_object(); o !== null;
         o = this.ctx.get_next_object(o)) {
      let item = o.get_userdata();
      if (item.e.eventId.nix === event_id.nix &&
          item.e.eventId.idx === event_id.idx) {
        return item;
      }
    }
    return null;
  }

  is_authorized(access) {
    return !!(this.priv & access);
  }

  ack() {
    if (!this.is_authorized(Access.RtEventsAck)) {
      window.alert("Not authorzed for this operation");
      return;
    }

    // Ack last
    let node = this.ctx.get_first_object();
    if (node === null) {
      return;
    }

    let item = node.get_userdata();
    console.log("Ack", item.e.eventText);
    this.ctx.gdh.mhAcknowledge(item.e.eventId).then(this.ack_cb);

    if (item.e.eventStatus & EventStatus.NotRet) {
      item.e.eventStatus &= ~EventStatus.NotAck;
      item.update();
    } else {
      this.ctx.remove(item.node);
    }
    this.ctx.configure();
    this.ctx.draw();
  }

  ack_cb(res) {
    console.log("ack sts", res.sts);
  }

  open_objectgraph_cb(res) {
    let w = window.open("", "_blank");
    if ((res.sts & 1) === 0) {
      w.document.write("Error status " + res.sts);
    } else {
      let result = res.value;
      w.location.href =
          "ge.html?graph=" + result.param1 + "&instance=" + result.fullname;
      w.document.title = result.fullname;
    }
  }

  open_graph_cb(res) {
    let w = window.open("", "_blank");
    if ((res.sts & 1) === 0) {
      w.document.write("Error status " + res.sts);
    } else {
      let result = res.value;
      let idx = result.param1.indexOf('.');
      if (idx !== -1) {
        result.param1 = result.param1.substring(0, idx);
      }

      let instancestr = "";
      if (result.fullname !== "") {
        instancestr = "&instance=" + result.fullname;
      }

      w.location.href = "ge.html?graph=" + result.param1 + instancestr;
      w.document.title = result.param1;
    }
  }

  open_plc_cb(res) {
    let w = window.open("", "_blank");
    if ((res.sts & 1) === 0) {
      w.document.write("Error status " + res.sts);
    } else {
      let result = res.value;
      let param1 = result.param1 ? ("&obj=" + result.param1) : "";
      console.log("flow.html?vid=" + result.objid.vid + "&oix=" +
          result.objid.oix + param1);
      w.location.href =
          "flow.html?vid=" + result.objid.vid + "&oix=" + result.objid.oix +
          param1;
      w.document.title = "Trace " + result.fullname;
    }
  }

  open_navigator_cb(res) {
    console.log("Open navigator", res.sts);
    if ((res.sts & 1) === 0) {
      console.log("Error status " + res.sts);
    } else {
      localStorage.setItem("XttMethodNavigator", res.value.fullname);
      console.log("storage", localStorage.getItem("XttMethodNavigator"));
    }
  }

  open_objectgraph_cb(res) {
    let w = window.open("", "_blank");
    if ((res.sts & 1) === 0) {
      w.document.write("Error status " + res.sts);
    } else {
      let classname = res.value.classname.toLowerCase();
      if (classname.substring(0, 1) === "$") {
        classname = classname.substring(1);
      }
      let graphname = "pwr_c_" + classname;
      w.location.href =
          "ge.html?graph=" + graphname + "&instance=" + res.value.fullname;
      w.document.title = graphname + " " + res.value.fullname;
    }
  }

  open_helpclass_cb(res) {
    let w = window.open("", "_blank");
    if ((res.sts & 1) === 0) {
      w.document.write("Error status " + res.sts);
    } else {
      console.log("open_helpclass", res.value.param1);
      w.location.href =
          location.protocol + "//" + location.host + res.value.param1;
    }
  }

  methods_init() {
    localStorage.setItem("EvMethodNavigator", "");
  }

  collapse() {
    this.ctx.set_nodraw();
    for (let i = 0; i < this.ctx.a.size(); i++) {
      let node = this.ctx.a.get(i);
      if (node.level === 0) {
        node.userdata.close(this);
      }
    }
    this.ctx.reset_nodraw();
    this.ctx.draw();
  }

  trace_cyclic() {
    this.ctx.gdh.mhSync(this.mhSyncIdx).then(this.sync_cb);

    this.timer = setTimeout(this.trace_cyclic, 1000);
  }

  trace_scan(res) {
    this.scan_update = false;
    if (this.scan_update) {
      this.ctx.draw();
    }
  }

  createNodeClasses() {
    let r1 = new PlowRect(this.ctx, 0, 0, 50, 1.0, Color.WHITE,
        Color.WHITE, true, false);
    let r2a = new PlowRect(this.ctx, 0.1, 0.15, 0.7, 0.7, Color.RED,
        Color.BLACK, true, false);
    let r2b = new PlowRect(this.ctx, 0.1, 0.15, 0.7, 0.7, Color.YELLOW,
        Color.BLACK, true, false);
    let r2info = new PlowRect(this.ctx, 0.1, 0.15, 0.7, 0.7, Color.WHITE,
        Color.BLACK, true, false);
    let r2success = new PlowRect(this.ctx, 0.1, 0.15, 0.7, 0.7,
        Color.GREEN, Color.BLACK, true, false);
    let a1 = new PlowAnnot(this.ctx, 1, 0.9, 4, Color.BLACK, 0, 0);
    let p1 = new PlowAnnotPixmap(this.ctx, 1.8, 0.2, 0);
    let p2 = new PlowAnnotPixmap(this.ctx, 2.4, 0.2, 1);
    let p3 = new PlowAnnotPixmap(this.ctx, 3.2, 0.2, 2);
    let p4 = new PlowAnnotPixmap(this.ctx, 4.0, 0.2, 3);
    let a2 = new PlowAnnot(this.ctx, 4.8, 0.9, 4, Color.BLACK, 0, 1);
    let a3 = new PlowAnnot(this.ctx, 11.5, 0.9, 4, Color.BLACK,
        NEXT_RELATIVE_POSITION, 2);
    let a4 = new PlowAnnot(this.ctx, 22.5, 0.9, 4, Color.BLACK,
        RELATIVE_POSITION, 3);

    // A alarm with red square
    this.ncAlarmA = new PlowNodeClass(this.ctx);
    this.ncAlarmA.insert(r1);
    this.ncAlarmA.insert(r2a);

    // A alarm with yellow square
    this.ncAlarmB = new PlowNodeClass(this.ctx);
    this.ncAlarmB.insert(r1);
    this.ncAlarmB.insert(r2b);

    // D and C alarm with no square
    this.ncAlarm = new PlowNodeClass(this.ctx);
    this.ncAlarm.insert(r1);

    // Info with white square
    this.ncInfo = new PlowNodeClass(this.ctx);
    this.ncInfo.insert(r1);
    this.ncInfo.insert(r2info);

    // InfoSuccess with green square
    this.ncSuccess = new PlowNodeClass(this.ctx);
    this.ncSuccess.insert(r1);
    this.ncSuccess.insert(r2success);

    [a1, p1, p2, p3, p4, a2, a3, a4].forEach(function (a) {
      this.ncAlarmA.insert(a);
      this.ncAlarmB.insert(a);
      this.ncAlarm.insert(a);
      this.ncInfo.insert(a);
      this.ncSuccess.insert(a);
    });

    this.ctx.insert_nc(this.ncAlarmA);
    this.ctx.insert_nc(this.ncAlarmB);
    this.ctx.insert_nc(this.ncAlarm);
    this.ctx.insert_nc(this.ncInfo);
    this.ctx.insert_nc(this.ncSuccess);
  }

  plow_event(event, object, x, y) {
    let item = null;
    if (object !== null) {
      item = object.get_userdata();
    }

    switch (event) {
      case Event.ObjectDeleted:
        //if ( object.userdata instanceof EvItemAttr) {
        //	object.userdata.scan_close( this);
        //}
        break;
      case Event.MB1Click:
        if (item !== null && item.e.eventMoreText !== "" &&
            x >= object.measure().ll_x + 70 && x <= object.measure().ll_x + 90) {
          window.alert(item.e.eventMoreText);
        } else if (object.select) {
          object.set_select(false);
          object.set_invert(false);
        } else {
          this.ctx.set_select(false);
          object.set_select(true);
          this.ctx.set_invert(false);
          object.set_invert(true);
          this.ctx.draw();
        }
        break;
      case Event.Key_Down:
        let o = this.ctx.get_select();
        if (o === null) {
          o = this.ctx.a.a[0];
          o.set_select(true);
          o.set_invert(true);
        } else {
          let next = this.ctx.get_next_object(o);
          if (next !== null) {
            o.set_select(false);
            o.set_invert(false);
            next.set_select(true);
            next.set_invert(true);
            if (!this.ctx.is_visible(next)) {
              this.ctx.scroll(next.ll_y, 0.10);
            }
          }
        }
        break;
      case Event.Key_Up:
        let o = this.ctx.get_select();
        if (o !== null) {
          let next = this.ctx.get_previous_object(o);
          if (next !== null) {
            o.set_select(false);
            o.set_invert(false);
            next.set_select(true);
            next.set_invert(true);
            o.draw(this.ctx.gdraw.gctx, null, null, 0);
            next.draw(this.ctx.gdraw.gctx, null, null, 0);
            if (!this.ctx.is_visible(next)) {
              this.ctx.scroll(next.ll_y, 0.90);
            }
          }
        }
        break;
      case Event.Key_CtrlL:
        let o = this.ctx.get_select();
        if (o.userdata instanceof EvItemAlarm) {
          this.ctx.gdh.getObject(o.userdata.objid, GdhOp.GET_OP_METHOD_PLC).then(this.open_plc_cb);
        }
        break;
      case Event.Key_CtrlG:
        let o = this.ctx.get_select();
        if (o.userdata instanceof EvItemAlarm) {
          this.ctx.gdh.getObject(o.userdata.objid, GdhOp.GET_OP_METHOD_PLC).then(this.open_objectgraph_cb);
        }
        break;
      default:
        break;
    }
  }

  // Query 'list' = alarm/event/block. Alarm is default.
  get_type() {
    let query = window.location.search.substring(1);

    if (query === "") {
      return EvType.AlarmList;
    }

    let vars = query.split('&');
    let typestr = vars[0].substring(5);
    if (typestr === "event") {
      return EvType.EventList;
    } else if (typestr === "block") {
      return EvType.BlockList;
    } else {
      return EvType.AlarmList;
    }
  }
}

class EvItemAlarm {
  node: PlowNode;
  e: MhEvent;

  constructor(ev, e, destination, destCode) {
    this.e = e;

    let nodeclass = ev.ncAlarm;
    switch (this.e.eventType) {
      case Event.Alarm:
      case Event.MaintenanceAlarm:
      case Event.SystemAlarm:
      case Event.UserAlarm1:
      case Event.UserAlarm2:
      case Event.UserAlarm3:
      case Event.UserAlarm4:
      case Event.Block:
      case Event.Reblock:
      case Event.CancelBlock:
      case Event.Unblock:
        if (this.e.eventPrio === EventPrio.A) {
          nodeclass = ev.ncAlarmA;
        } else if (this.e.eventPrio === EventPrio.B) {
          nodeclass = ev.ncAlarmB;
        } else {
          nodeclass = ev.ncAlarm;
        }
        break;
      case Event.Info:
        nodeclass = ev.ncInfo;
        break;
      case Event.InfoSuccess:
        nodeclass = ev.ncSuccess;
        break;
    }
    this.node = new PlowNode(ev.ctx, nodeclass, 0);
    ev.ctx.insertNode(this.node, destination, destCode);
    this.node.set_userdata(this);

    switch (this.e.eventType) {
      case Event.Alarm:
      case Event.MaintenanceAlarm:
      case Event.SystemAlarm:
      case Event.UserAlarm1:
      case Event.UserAlarm2:
      case Event.UserAlarm3:
      case Event.UserAlarm4:
      case Event.Block:
      case Event.Reblock:
      case Event.CancelBlock:
      case Event.Unblock:
        this.node.set_annotation(0, String.fromCharCode(132 - this.e.eventPrio));
        break;
    }
    switch (this.e.eventType) {
      case Event.MaintenanceAlarm:
        this.node.set_annotation_pixmap(3, Bitmaps.maintenance);
        break;
      case Event.SystemAlarm:
        this.node.set_annotation_pixmap(3, Bitmaps.system);
        break;
      case Event.Block:
        this.e.eventText = "Blocked";
        break;
      case Event.Reblock:
        this.e.eventText = "Reblocked";
        break;
      case Event.CancelBlock:
        this.e.eventText = "Block canceled";
        break;
      case Event.Unblock:
        this.e.eventText = "Unblocked";
        break;
    }
    this.node.set_annotation(1, this.e.eventTime.substring(0, 17));
    this.node.set_annotation(2, this.e.eventText);
    this.node.set_annotation(3, this.e.eventName);

    switch (ev.type) {
      case EvType.EventList:
        switch (this.e.eventType) {
          case Event.Alarm:
          case Event.MaintenanceAlarm:
          case Event.SystemAlarm:
          case Event.UserAlarm1:
          case Event.UserAlarm2:
          case Event.UserAlarm3:
          case Event.UserAlarm4:
          case Event.Info:
          case Event.InfoSuccess:
            this.node.set_annotation_pixmap(0, Bitmaps.eventalarm);
            break;
          case Event.Ack:
            this.node.set_annotation_pixmap(0, Bitmaps.eventacked);
            break;
          case Event.Return:
            this.node.set_annotation_pixmap(0, Bitmaps.eventreturn);
            break;
          case Event.Block:
          case Event.Reblock:
            this.node.set_annotation_pixmap(0, Bitmaps.blockl);
            break;
          case Event.CancelBlock:
          case Event.Unblock:
            this.node.set_annotation_pixmap(0, Bitmaps.blockr);
            break;
        }
        break;
      case EvType.AlarmList:
        if (this.e.eventStatus & EventStatus.NotRet) {
          this.node.set_annotation_pixmap(1, Bitmaps.alarm);
        }
        if (this.e.eventStatus & EventStatus.NotAck) {
          this.node.set_annotation_pixmap(0, Bitmaps.ack);
        }
        if (this.e.eventMoreText !== "") {
          this.node.set_annotation_pixmap(2, Bitmaps.info);
        }
        break;
    }
  }

  update() {
    if (this.e.eventStatus & EventStatus.NotRet) {
      this.node.set_annotation_pixmap(1, Bitmaps.alarm);
    } else {
      this.node.set_annotation_pixmap(1, null);
    }

    if (this.e.eventStatus & EventStatus.NotAck) {
      this.node.set_annotation_pixmap(0, Bitmaps.ack);
    } else {
      this.node.set_annotation_pixmap(0, null);
    }
  }
}

let ev = new Ev();
