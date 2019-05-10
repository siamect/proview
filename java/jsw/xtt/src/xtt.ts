"use strict";

class XttOpenChildrenData {
  node: PlowNode;
  open_next: PlowNode;

  constructor(node, open_next) {
    this.node = node;
    this.open_next = open_next;
  }
}

class Xtt {
  ncObject: PlowNodeClass = null;
  scan_update: boolean;
  priv: number;
  ctx: PlowCtx;
  timer = null;

  constructor() {
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
        xtt.ctx.event_handler(Event.MB1ClickShift, x, y);
      } else {
        xtt.ctx.event_handler(Event.MB1Click, x, y);
      }
    });
    document.addEventListener("keydown", function (event) {
      if (event.keyCode === 40) {
        this.ctx.event_handler(Event.Key_Down);
      } else if (event.keyCode === 39) {
        this.ctx.event_handler(event.shiftKey ? Event.Key_ShiftRight : Event.Key_Right);
      } else if (event.keyCode === 37) {
        this.ctx.event_handler(Event.Key_Left);
      } else if (event.keyCode === 38) {
        this.ctx.event_handler(Event.Key_Up);
      } else if (event.keyCode === 82 && event.ctrlKey) {
        this.ctx.event_handler(Event.Key_CtrlR);
      } else if (event.keyCode === 76 && event.ctrlKey) {
          this.ctx.event_handler(Event.Key_CtrlL);
      } else if (event.keyCode === 71 && event.ctrlKey) {
        this.ctx.event_handler(Event.Key_CtrlG);
      }

      if ([37, 38, 39, 40, 71, 76, 82].indexOf(event.keyCode) != -1) {
        event.preventDefault();
      }
    });
    document.getElementById("toolitem1")
        .addEventListener("click", function (event) {
          let o = this.ctx.get_select();
          if (o.userdata instanceof XttItemObject) {
            let newwindow = window.open("", "_blank");
            this.ctx.gdh.getObject(o.userdata.objid, GdhOp.GET_OP_METHOD_GRAPH,
                this.open_graph_cb, newwindow);
          }
          console.log("toolitem1 event");
        });
    document.getElementById("toolitem2")
        .addEventListener("click", function (event) {
          let o = this.ctx.get_select();
          if (o.userdata instanceof XttItemObject) {
            let newwindow = window.open("", "_blank");
            this.ctx.gdh.getObject(o.userdata.objid,
                GdhOp.GET_OP_METHOD_OBJECTGRAPH, this.open_objectgraph_cb,
                newwindow);
          }
          console.log("toolitem2 event");
        });
    document.getElementById("toolitem3")
        .addEventListener("click", function (event) {
          console.log("toolitem1 event");
          let o = this.ctx.get_select();
          if (o.userdata instanceof XttItemObject) {
            let newwindow = window.open("", "_blank");
            this.ctx.gdh.getObject(o.userdata.objid, GdhOp.GET_OP_METHOD_PLC,
                this.open_plc_cb, newwindow);
          } else if (o.userdata instanceof XttItemCrr) {
            let idx = o.userdata.name.lastIndexOf('-');
            let ostring = "";
            if (idx !== -1) {
              ostring = "&obj=" + o.userdata.name.substring(idx + 1);
            }
            console.log("flow.html?vid=" + o.userdata.objid.vid + "&oix=" +
                o.userdata.objid.oix + ostring);
            window.open("flow.html?vid=" + o.userdata.objid.vid + "&oix=" +
                o.userdata.objid.oix + ostring);
          }
          console.log("toolitem3 event");
        });
    document.getElementById("toolitem4")
        .addEventListener("click", function (event) {
          console.log("toolitem4 event");
        });
    document.getElementById("toolitem5")
        .addEventListener("click", function (event) {
          console.log("toolitem5 event");
          let o = this.ctx.get_select();
          if (o === null) {
            return;
          }
          let item = o.userdata;
          item.open_attributes(this);
        });
    document.getElementById("toolitem6")
        .addEventListener("click", function (event) {
          let o = this.ctx.get_select();
          this.ctx.gdh.crrSignal(o.userdata.objid, this.open_crr_cb, o);
          console.log("toolitem6 event");
        });
    document.getElementById("toolitem7")
        .addEventListener("click", function (event) {
          console.log("toolitem7 event");
        });
    document.getElementById("toolitem8")
        .addEventListener("click", function (event) {
          console.log("toolitem8 event");
          let o = this.ctx.get_select();
          if (o === null) {
            return;
          }
          if (o.userdata instanceof XttItemObject) {
            let newwindow = window.open("", "_blank");
            this.ctx.gdh.getObject(o.userdata.objid,
                GdhOp.GET_OP_METHOD_HELPCLASS, this.open_helpclass_cb, newwindow);
          }
        });

    window.addEventListener("storage", function (event) {
      if (event.newValue === "") {
        return;
      }
      localStorage.setItem("XttMethodNavigator", "");
      this.display_object(event.newValue);
    });
  }

  is_authorized(access) {
    return !!(this.priv & access);
  }

  gdh_init_cb() {
    if (this.priv === null) {
      this.ctx.gdh.login("", "", this.login_cb, this);
    }

    let oid = new PwrtObjid(0, 0);
    this.ctx.gdh.getAllXttChildren(oid, this.open_children_cb,
        new XttOpenChildrenData(null, null));

    this.ctx.gdh.listSent = true;
    this.trace_cyclic();
  }

  login_cb(id, data, sts, result) {
    console.log("Login:", sts, result);
    this.priv = (sts & 1) ? result : 0;
  }

  open_children_cb(id, data, sts, result) {
    this.ctx.set_nodraw();
    for (let i = 0; i < result.length; i++) {
      if (data.node === null) {
        result[i].full_name = result[i].name;
        new XttItemObject(this, result[i], null, Dest.AFTER);
      } else {
        result[i].full_name =
            data.node.userdata.full_name + "-" + result[i].name;
        new XttItemObject(this, result[i], data.node, Dest.INTOLAST);
      }
    }
    this.ctx.configure();

    if (data.open_next !== null) {
      if (data.open_next.length === 0) {
        this.ctx.reset_nodraw();
        return;
      }
      let child = this.ctx.a.get_first_child(data.node);
      while (child !== null) {
        if (child.userdata.name === data.open_next[0]) {
          if (data.open_next.length === 1) {
            child.set_select(true);
            child.set_invert(true);
            if (!this.ctx.is_visible(child)) {
              this.ctx.scroll(child.ll_y, 0.50);
            }
            window.focus(); // Doesn't work
          } else {
            data.open_next.splice(0, 1);
            if (data.open_next[0] === '.') {
              data.open_next.splice(0, 1);
              child.userdata.open_attributes(this, data.open_next);
            } else {
              child.userdata.open_children(this, data.open_next);
            }
          }
          break;
        }
        child = this.ctx.a.get_next_sibling(child);
      }
    }

    this.ctx.reset_nodraw();
    this.ctx.draw();
  }

  open_attributes_cb(id, data, sts, result) {
    this.ctx.set_nodraw();
    for (let i = 0; i < result.length; i++) {
      result[i].objid = data.node.userdata.objid;
      result[i].full_name = data.node.userdata.full_name + "." + result[i].name;
      if ((result[i].flags & Adef.Array) !== 0) {
        new XttItemAttrArray(this, result[i], data.node, Dest.INTOLAST);
      } else if ((result[i].flags & Adef.Class) !== 0) {
        new XttItemAttrObject(this, result[i], data.node, Dest.INTOLAST);
      } else {
        new XttItemAttr(this, result[i], data.node, Dest.INTOLAST);
      }
    }

    this.ctx.configure();

    if (data.open_next !== null) {
      if (data.open_next.length === 0) {
        this.ctx.reset_nodraw();
        return;
      }
      let child = this.ctx.a.get_first_child(data.node);
      while (child !== null) {
        if (child.userdata.name === data.open_next[0]) {
          if (data.open_next.length === 1) {
            child.set_select(true);
            child.set_invert(true);
            if (!this.ctx.is_visible(child)) {
              this.ctx.scroll(child.ll_y, 0.50);
            }
            window.focus(); // Doesn't work
          } else {
            data.open_next.splice(0, 1);
            child.userdata.open_attributes(this, data.open_next);
          }
          break;
        }
        child = this.ctx.a.get_next_sibling(child);
      }
    }

    this.ctx.reset_nodraw();
    this.ctx.draw();
  }

  open_plc_cb(id, data, sts, result: ObjectInfo) {
    if ((sts & 1) === 0) {
      data.document.write("Error status " + sts);
    } else {
      let param1;
      if (result.param1 === "") {
        param1 = "";
      } else {
        param1 = "&obj=" + result.param1;
      }
      console.log("flow.html?vid=" + result.objid.vid + "&oix=" +
          result.objid.oix + param1);
      data.location.href =
          "flow.html?vid=" + result.objid.vid + "&oix=" + result.objid.oix +
          param1;
      data.document.title = "Trace " + result.fullname;
    }
  }

  open_objectgraph_cb(id, data, sts, result: ObjectInfo) {
    if ((sts & 1) === 0) {
      data.document.write("Error status " + sts);
    } else {
      data.location.href =
          "ge.html?graph=" + result.param1 + "&instance=" + result.fullname;
      data.document.title = result.fullname;
    }
  }

  open_graph_cb(id, data, sts, result: ObjectInfo) {
    if ((sts & 1) === 0) {
      data.document.write("Error status " + sts);
    } else {
      let idx = result.param1.indexOf('.');
      if (idx !== -1) {
        result.param1 = result.param1.substring(0, idx);
      }

      let instancestr = "";
      if (result.fullname !== "") {
        instancestr = "&instance=" + result.fullname;
      }

      data.location.href = "ge.html?graph=" + result.param1 + instancestr;
      data.document.title = result.param1;
    }
  }

  open_crr_cb(id, node, sts, crrdata) {
    if ((sts & 1) === 0) {
      return;
    }
    node.userdata.open_crossreferences(this, crrdata);
  }

  open_helpclass_cb(id, data, sts, result: ObjectInfo) {
    if ((sts & 1) === 0) {
      data.document.write("Error status " + sts);
    } else {
      console.log("open_helpclass", result.param1);
      data.location.href =
          location.protocol + "//" + location.host + result.param1;
    }
  }

  plow_event(event, object, x, y) {
    let item = null;
    if (object !== null) {
      item = object.get_userdata();
    }

    switch (event) {
      case Event.ObjectDeleted:
        if (object.userdata instanceof XttItemAttr) {
          object.userdata.scan_close(this);
        }
        break;
      case Event.MB1Click:
        if (object.in_icon(x, y)) {
          item.open_children(this, null);
        } else {
          if (object.select) {
            object.set_select(false);
            object.set_invert(false);
          } else {
            this.ctx.set_select(false);
            object.set_select(true);
            this.ctx.set_invert(false);
            object.set_invert(true);
            this.ctx.draw();
          }
        }
        break;
      case Event.MB1ClickShift:
        if (object.in_icon(x, y)) {
          item.open_attributes(this, null);
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
      case Event.Key_Right:
        let o = this.ctx.get_select();
        if (o !== null) {
          item = o.userdata;
          item.open_children(this, null);
        }
        break;
      case Event.Key_Left:
        let o = this.ctx.get_select();
        if (o !== null) {
          item = o.userdata;
          item.close(this);
        }
        break;
      case Event.Key_ShiftRight:
        let o = this.ctx.get_select();
        if (o !== null) {
          item = o.userdata;
          item.open_attributes(this, null);
        }
        break;
      case Event.Key_CtrlR:
        let o = this.ctx.get_select();
        if (o !== null) {
          this.ctx.gdh.crrSignal(o.userdata.objid, this.open_crr_cb, o);
        }
        break;
      case Event.Key_CtrlL:
        let o = this.ctx.get_select();
        if (o.userdata instanceof XttItemObject) {
          let newwindow = window.open("", "_blank");
          this.ctx.gdh.getObject(o.userdata.objid, GdhOp.GET_OP_METHOD_PLC,
              this.open_plc_cb, newwindow);
        } else if (o.userdata instanceof XttItemCrr) {
          let idx = o.userdata.name.lastIndexOf('-');
          let ostring = "";
          if (idx !== -1) {
            ostring = "&obj=" + o.userdata.name.substring(idx + 1);
          }
          window.open("flow.html?vid=" + o.userdata.objid.vid + "&oix=" +
              o.userdata.objid.oix + ostring);
        }
        break;
      case Event.Key_CtrlG:
        let o = this.ctx.get_select();
        if (o.userdata instanceof XttItemObject) {
          console.log("CtrlG", o.userdata.objid.vid, o.userdata.objid.oix);
          let newwindow = window.open("", "_blank");
          this.ctx.gdh.getObject(o.userdata.objid,
              GdhOp.GET_OP_METHOD_OBJECTGRAPH, this.open_objectgraph_cb,
              newwindow);
        }
        break;
      default:
        break;
    }
  }

  createNodeClasses() {
    let r1 = new PlowRect(this.ctx, 0, 0, 50, 1.0, Color.WHITE,
        Color.WHITE, true, false);
    let a1 = new PlowAnnot(this.ctx, 3, 0.9, 4, Color.BLACK,
        NEXT_RELATIVE_POSITION, 0);
    let a11 = new PlowAnnot(this.ctx, 7.5, 0.9, 4, Color.BLACK,
        RELATIVE_POSITION, 1);
    let a12 = new PlowAnnot(this.ctx, 12, 0.9, 4, Color.BLACK,
        RELATIVE_POSITION, 2);
    let p1 = new PlowAnnotPixmap(this.ctx, 0.4, 0.2, 0);

    this.ncObject = new PlowNodeClass(this.ctx);
    this.ncObject.insert(r1);
    this.ncObject.insert(a1);
    this.ncObject.insert(a11);
    this.ncObject.insert(a12);
    this.ncObject.insert(p1);
    this.ctx.insert_nc(this.ncObject);
  }

  methods_init() {
    localStorage.setItem("XttMethodNavigator", "");
  }

  collapse() {
    this.ctx.set_nodraw();
    this.ctx.a.forEach(function (e) {
      if (e.level === 0) {
        e.userdata.close(this);
      }
    });
    this.ctx.reset_nodraw();
    this.ctx.draw();
  }

  display_object(name) {
    let i = name.indexOf('.');
    let attr = null;
    let path;
    if (i === -1) {
      path = name.split('-');
    } else {
      attr = name.substring(i + 1);
      name = name.substring(0, i);
      let p1 = name.split('-');
      let p2 = attr.split('.');
      path = new Array(p1.length + p2.length + 1);
      let j;
      for (j = 0; j < p1.length; j++) {
        path[j] = p1[j];
      }
      path[p1.length] = '.';
      for (j = 0; j < p2.length; j++) {
        path[j + p1.length + 1] = p2[j];
      }
    }

    let idx = 0;

    this.collapse();
    for (let j = idx; j < this.ctx.a.size(); j++) {
      if (this.ctx.a.get(j).userdata.name === path[0]) {
        if (j === this.ctx.a.size() - 1) {
          let node = this.ctx.a.get(j);
          node.set_select(true);
          node.set_invert(true);
          if (!this.ctx.is_visible(node)) {
            this.ctx.scroll(node.ll_y, 0.50);
          }
          window.focus(); // Doesn't work
        } else {
          path.splice(0, 1);
          (<XttItemObject> this.ctx.a.get(j).userdata).open_children(this, path);
        }
        break;
      }
    }
  }

  trace_cyclic() {
    this.ctx.gdh.getRefObjectInfoAll(this.trace_scan);
  }

  trace_scan(id, sts) {
    this.scan_update = false;
    let self = this;
    this.ctx.a.forEach(function (e) {
      let item = e.userdata;
      if (item instanceof XttItemAttr) {
        item.scan(self);
      }
    });
    if (this.scan_update) {
      this.ctx.draw();
    }
    this.timer = setTimeout(this.trace_cyclic, 1000);
  }

  openValueInputDialog(item, text) {
    console.log("priv acc", this.priv, Access.RtWrite |
        Access.System);
    if (this.is_authorized(Access.RtWrite | Access.System)) {
      let value = prompt(text, "");
      if (value !== null) {
        item.set_value(this, value);
      }
    } else {
      alert("Not authorized for this operation");
    }
  }
}

class XttItemObject {
  objid: PwrtObjid;
  cid: number;
  name: string;
  full_name: string;
  has_children: boolean;
  node: PlowNode;

  constructor(xtt, object_info, destination, destCode) {
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
    this.node.set_annotation_pixmap(0, object_info.has_children ? Bitmaps.map : Bitmaps.leaf);
  }

  open_children(xtt, open_next) {
    if (this.node.node_open !== 0) {
      this.close(xtt);
    } else if (!this.has_children) {
      this.open_attributes(xtt, null);
    } else {
      xtt.ctx.gdh.getAllXttChildren(this.objid, xtt.open_children_cb,
          new XttOpenChildrenData(this.node, open_next));
      this.node.node_open |= Open.CHILDREN;
      this.node.set_annotation_pixmap(0, Bitmaps.openmap);
    }
  }

  open_attributes(xtt, open_next) {
    if (this.node.node_open === 0) {
      xtt.ctx.gdh.getAllClassAttributes(this.cid, this.objid,
          xtt.open_attributes_cb, new XttOpenChildrenData(this.node, open_next));

      this.node.node_open |= Open.ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else {
      this.close(xtt);
    }
  }

  open_crossreferences(xtt, crrdata) {
    if (this.node.node_open === 0) {
      for (let i = 0; i < crrdata.length; i++) {
        new XttItemCrr(xtt, crrdata[i], this.node, Dest.INTOLAST);
      }

      this.node.node_open |= Open.CROSSREFERENCES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else {
      this.close(xtt);
    }
  }

  close(xtt) {
    if (this.node.node_open & Open.CHILDREN) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Open.CHILDREN;
      this.node.set_annotation_pixmap(0, Bitmaps.map);
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else if (this.node.node_open & Open.ATTRIBUTES) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Open.ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else if (this.node.node_open & Open.CROSSREFERENCES) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Open.CROSSREFERENCES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else {
      let parent = xtt.ctx.get_parent_object(this.node);
      if (parent !== null) {
        parent.userdata.close(xtt);
        parent.set_select(true);
        parent.set_invert(true);
      }
    }
  }
}

class XttItemAttr {
  name: string;
  objid: PwrtObjid;
  full_name: string;
  type: number;
  flags: number;
  size: number;
  refid: number;
  firstScan: boolean;
  old_value: number;
  node: PlowNode;

  constructor(xtt, info, destination, destCode) {
    this.name = info.name;
    this.objid = info.objid;
    this.full_name = info.full_name;
    this.type = info.type;
    this.flags = info.flags;
    this.size = info.size;
    this.firstScan = true;
    this.node = new PlowNode(xtt.ctx, xtt.ncObject, 0);
    this.node.set_userdata(this);
    this.node.set_annotation(0, this.name);
    this.node.set_annotation_pixmap(0, Bitmaps.attr);
    xtt.ctx.insertNode(this.node, destination, destCode);

    this.refid = xtt.ctx.gdh.refObjectInfo(this.full_name, info.type, 1);
  }

  open_children(xtt, open_next) {
    xtt.openValueInputDialog(this, "Enter value");
  }

  set_value(xtt, value) {
    switch (this.type) {
      case Type.Float32:
        let inputValue = parseFloat(value.trim());
        xtt.ctx.gdh.setObjectInfoFloat(this.full_name, inputValue);
        break;
      case Type.Int8:
      case Type.Int16:
      case Type.Int32:
      case Type.UInt8:
      case Type.UInt16:
      case Type.UInt32:
      case Type.Status:
      case Type.Mask:
      case Type.Enum:
      case Type.Boolean:
        let inputValue = parseInt(value.trim(), 10);
        xtt.ctx.gdh.setObjectInfoInt(this.full_name, inputValue);
        break;
      case Type.String:
      case Type.Time:
      case Type.DeltaTime:
      case Type.AttrRef:
      case Type.Objid:
        xtt.ctx.gdh.setObjectInfoString(this.full_name, value);
        break;
      default:
        break;
    }
  }

  open_attributes(xtt, open_next) {
  }

  close(xtt) {
    let parent = xtt.ctx.get_parent_object(this.node);
    if (parent !== null) {
      parent.userdata.close(xtt);
      parent.set_select(true);
      parent.set_invert(true);
    }
  }

  scan(xtt) {
    if (!this.refid) {
      return;
    }
    let value = xtt.ctx.gdh.getObjectRefInfo(this.refid);
    if (this.firstScan || value !== this.old_value) {
      let value_str;

      switch (this.type) {
        case Type.Float32:
          value_str = "" + value;
          break;
        case Type.Boolean:
          value_str = value ? "1" : "0";
          break;
        case Type.Int32:
        case Type.Int16:
        case Type.Int8:
        case Type.UInt32:
        case Type.UInt16:
        case Type.UInt8:
          value_str = "" + value;
          break;
        case Type.String:
        case Type.AttrRef:
        case Type.Objid:
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

  scan_close(xtt) {
    xtt.ctx.gdh.unrefObjectInfo(this.refid);
  }
}

class XttItemAttrArray {
  name: string;
  objid: PwrtObjid;
  full_name: string;
  type: number;
  flags: number;
  size: number;
  elements: number;
  node: PlowNode;

  constructor(xtt, info, destination, destCode) {
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
  }

  open_children(xtt, open_next) {
    this.open_attributes(xtt, open_next);
  }

  open_attributes(xtt, open_next) {
    let info = new AttributeInfo();

    info.objid = this.objid;
    info.type = this.type;
    info.flags = this.flags & ~Adef.Array;
    info.size = this.size / this.elements;
    info.elements = 1;
    info.name = "";
    info.full_name = "";
    info.classname = "";

    xtt.ctx.set_nodraw();
    for (let i = 0; i < this.elements; i++) {
      info.name = this.name + "[" + i + "]";
      info.full_name = this.full_name + "[" + i + "]";
      if ((info.flags & Adef.Array) !== 0) {
        new XttItemAttrArray(xtt, info, this.node, Dest.INTOLAST);
      } else if ((info.flags & Adef.Class) !== 0) {
        new XttItemAttrObject(xtt, info, this.node, Dest.INTOLAST);
      } else {
        new XttItemAttr(xtt, info, this.node, Dest.INTOLAST);
      }
    }
    this.node.node_open |= Open.ATTRIBUTES;
    xtt.ctx.configure();
    xtt.ctx.reset_nodraw();
    xtt.ctx.draw();
  }

  close(xtt) {
    if (this.node.node_open & Open.ATTRIBUTES) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Open.ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else {
      let parent = xtt.ctx.get_parent_object(this.node);
      if (parent !== null) {
        parent.userdata.close(xtt);
        parent.set_select(true);
        parent.set_invert(true);
      }
    }
  }
}

class XttItemAttrObject {
  name: string;
  classname: string;
  objid: PwrtObjid;
  full_name: string;
  cid: number;
  flags: number;
  size: number;
  node: PlowNode;

  constructor(xtt, info, destination, destCode) {
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
  }

  open_children(xtt, open_next) {
    this.open_attributes(xtt, null);
  }

  open_attributes(xtt, open_next) {
    if (this.node.node_open === 0) {
      xtt.ctx.gdh.getAllClassAttributes(this.cid, this.objid,
          xtt.open_attributes_cb, new XttOpenChildrenData(this.node, open_next));

      this.node.node_open |= Open.ATTRIBUTES;
    } else {
      this.close(xtt);
    }
  }

  close(xtt) {
    if (this.node.node_open & Open.ATTRIBUTES) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Open.ATTRIBUTES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else if (this.node.node_open & Open.CROSSREFERENCES) {
      xtt.ctx.close_node(this.node);
      this.node.node_open &= ~Open.CROSSREFERENCES;
      xtt.ctx.configure();
      xtt.ctx.draw();
    } else {
      let parent = xtt.ctx.get_parent_object(this.node);
      if (parent !== null) {
        parent.userdata.close(xtt);
        parent.set_select(true);
        parent.set_invert(true);
      }
    }
  }
}

class XttItemCrr {
  name: string;
  classname: string;
  objid: PwrtObjid;
  type: number;
  node: PlowNode;

  constructor(xtt, info, destination, destCode) {
    this.name = info.name;
    this.classname = info.classname;
    this.objid = info.objid;
    this.type = info.type;
    this.node = new PlowNode(xtt.ctx, xtt.ncObject, 0);
    this.node.set_userdata(this);
    this.node.set_annotation(0, this.name);
    this.node.set_annotation(1, this.classname);

    let tmp = [Bitmaps.crrread, Bitmaps.crrwrite, Bitmaps.crrwrite];
    this.node.set_annotation_pixmap(0, tmp[this.type]);
    xtt.ctx.insertNode(this.node, destination, destCode);
  }

  close(xtt) {
    let parent = xtt.ctx.get_parent_object(this.node);
    if (parent) {
      parent.userdata.close(xtt);
      parent.set_select(true);
      parent.set_invert(true);
    }
  }
}

let xtt = new Xtt();
