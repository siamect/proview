class DynParsedAttrName {
  name: string;
  tname: string;
  inverted: boolean;
  elements;
  index: number;
  type: number;
  database;
  bitmask: number;
}

class Dyn {
  elements: Array<DynElem> = [];
  graph: Graph;
  object = null;
  dyn_type1: DynType1 = 0;
  dyn_type2: DynType2 = 0;
  action_type1: ActionType1 = 0;
  action_type2: ActionType2 = 0;
  access = 0;
  cycle = 0;
  total_dyn_type1 = 0;
  total_dyn_type2 = 0;
  total_action_type1 = 0;
  total_action_type2 = 0;

  repaintNow = false;
  ignoreColor = false;
  resetColor = false;
  ignoreBgColor = false;
  resetBgColor = false;

  constructor(graph) {
    this.graph = graph;
  }

  static getColor1(object, color) {
    return (color === DrawType.Inherit) ? object.getClassTraceColor1() : color;
  }

  static getColor2(object, color) {
    return (color === DrawType.Inherit) ? object.getClassTraceColor2() : color;
  }

  merge(x) {
    this.dyn_type1 |= x.dyn_type1;
    this.total_dyn_type1 |= x.total_dyn_type1;
    this.action_type1 |= x.action_type1;
    this.total_action_type1 |= x.total_action_type1;

    x.elements.forEach(function (xelem) {
      this.elements.filter(function (elem) {
        let arrTmp = ["dyn_type1", "dyn_type2", "action_type1", "action_type2"];
        // If element exists in both, use element in x, i.e. filter element
        return (arrTmp.some(e => elem[e] !== xelem[e]) ||
            (elem.instance !== undefined && elem.instance !== xelem.instance));
      });
      // Insert copy of x element
      let e = <DynElem>Object.assign({}, xelem);
      if (e) {
        e.dyn = this;
        this.elements.push(e);
      }
    });
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      let elem = null;

      switch (key) {
        case DynSave.Dyn:
          break;
        case DynSave.Dyn_dyn_type1:
          this.dyn_type1 = parseInt(tokens[1], 10);
          break;
        case DynSave.Dyn_dyn_type2:
          this.dyn_type2 = parseInt(tokens[1], 10);
          break;
        case DynSave.Dyn_action_type1:
          this.action_type1 = parseInt(tokens[1], 10);
          break;
        case DynSave.Dyn_action_type2:
          this.action_type2 = parseInt(tokens[1], 10);
          break;
        case DynSave.Dyn_access:
          this.access = parseInt(tokens[1], 10);
          break;
        case DynSave.Dyn_cycle:
          this.cycle = parseInt(tokens[1], 10);
          break;
        case DynSave.DigLowColor:
          elem = new DynDigLowColor(this);
          break;
        case DynSave.DigColor:
          elem = new DynDigColor(this);
          break;
        case DynSave.DigWarning:
          elem = new DynDigWarning(this);
          break;
        case DynSave.DigError:
          elem = new DynDigError(this);
          break;
        case DynSave.DigFlash:
          elem = new DynDigFlash(this);
          break;
        case DynSave.Invisible:
          elem = new DynInvisible(this);
          break;
        case DynSave.DigBorder:
          elem = new DynDigBorder(this);
          break;
        case DynSave.DigText:
          elem = new DynDigText(this);
          break;
        case DynSave.Value:
          elem = new DynValue(this);
          break;
        case DynSave.ValueInput:
          elem = new DynValueInput(this);
          break;
        case DynSave.AnalogColor:
          elem = new DynAnalogColor(this);
          break;
        case DynSave.Rotate:
          elem = new DynRotate(this);
          break;
        case DynSave.Move:
          elem = new DynMove(this);
          break;
        case DynSave.AnalogShift:
          elem = new DynAnalogShift(this);
          break;
        case DynSave.DigShift:
          elem = new DynDigShift(this);
          break;
        case DynSave.DigFourShift:
          elem = new DynDigFourShift(this);
          break;
        case DynSave.ScrollingText:
          elem = new DynScrollingText(this);
          break;
        case DynSave.ColorThemeLightness:
          elem = new DynColorThemeLightness(this);
          break;
        case DynSave.DigBackgroundColor:
          elem = new DynDigBackgroundColor(this);
          break;
        case DynSave.DigSwap:
          elem = new DynDigSwap(this);
          break;
        case DynSave.DigScript:
          elem = new DynDigScript(this);
          break;
        case DynSave.Animation:
          elem = new DynAnimation(this);
          break;
        case DynSave.Video:
          elem = new DynVideo(this);
          break;
        case DynSave.Bar:
          elem = new DynBar(this);
          break;
        case DynSave.Trend:
          elem = new DynTrend(this);
          break;
        case DynSave.FillLevel:
          elem = new DynFillLevel(this);
          break;
        case DynSave.FastCurve:
          elem = new DynFastCurve(this);
          break;
        case DynSave.AnalogText:
          elem = new DynAnalogText(this);
          break;
        case DynSave.Table:
          elem = new DynTable(this);
          break;
        case DynSave.StatusColor:
          elem = new DynStatusColor(this);
          break;
        case DynSave.Pie:
          elem = new DynPie(this);
          break;
        case DynSave.BarChart:
          elem = new DynBarChart(this);
          break;
        case DynSave.Axis:
          elem = new DynAxis(this);
          break;
        case DynSave.DigTextColor:
          elem = new DynDigTextColor(this);
          break;
        case DynSave.TimeoutColor:
          elem = new DynTimeoutColor(this);
          break;
        case DynSave.HostObject:
          elem = new DynHostObject(this);
          break;
        case DynSave.DigSound:
          elem = new DynDigSound(this);
          break;
        case DynSave.XY_Curve:
          elem = new DynXY_Curve(this);
          break;
        case DynSave.DigCommand:
          elem = new DynDigCommand(this);
          break;
        case DynSave.PopupMenu:
          elem = new DynPopupMenu(this);
          break;
        case DynSave.SetDig:
          elem = new DynSetDig(this);
          break;
        case DynSave.ResetDig:
          elem = new DynResetDig(this);
          break;
        case DynSave.ToggleDig:
          elem = new DynToggleDig(this);
          break;
        case DynSave.StoDig:
          elem = new DynStoDig(this);
          break;
        case DynSave.Command:
          elem = new DynCommand(this);
          break;
        case DynSave.CommandDC:
          elem = new DynCommandDoubleClick(this);
          break;
        case DynSave.Confirm:
          elem = new DynConfirm(this);
          break;
        case DynSave.IncrAnalog:
          elem = new DynIncrAnalog(this);
          break;
        case DynSave.RadioButton:
          elem = new DynRadioButton(this);
          break;
        case DynSave.Slider:
          elem = new DynSlider(this);
          break;
        case DynSave.TipText:
          elem = new DynTipText(this);
          break;
        case DynSave.Help:
          elem = new DynHelp(this);
          break;
        case DynSave.OpenGraph:
          elem = new DynOpenGraph(this);
          break;
        case DynSave.OpenURL:
          elem = new DynOpenURL(this);
          break;
        case DynSave.InputFocus:
          elem = new DynInputFocus(this);
          break;
        case DynSave.CloseGraph:
          elem = new DynCloseGraph(this);
          break;
        case DynSave.PulldownMenu:
          elem = new DynPulldownMenu(this);
          break;
        case DynSave.OptionMenu:
          elem = new DynOptionMenu(this);
          break;
        case DynSave.SetValue:
          elem = new DynSetValue(this);
          break;
        case DynSave.MethodToolbar:
          elem = new DynMethodToolbar(this);
          break;
        case DynSave.MethodPulldownMenu:
          elem = new DynMethodPulldownMenu(this);
          break;
        case DynSave.Script:
          elem = new DynScript(this);
          break;
        case DynSave.CatchSignal:
          elem = new DynCatchSignal(this);
          break;
        case DynSave.EmitSignal:
          elem = new DynEmitSignal(this);
          break;
        case DynSave.End:
          if (elem) {
            this.elements.push(elem);
            i = elem.open(lines, i + 1);
          }
          return i;
        default:
          console.log("Syntax error in Dyn");
          break;
      }

      if (elem) {
        this.elements.push(elem);
        i = elem.open(lines, i + 1);
      }
    }

    return i;
  }

  connect(object) {
    if ((this.dyn_type1 & DynType1.Inherit) === 0) {
      this.total_dyn_type1 = this.dyn_type1;
      this.total_dyn_type2 = this.dyn_type2;
    } else {
      let inherit_dyn_type1 = object.getClassDynType1();
      let inherit_dyn_type2 = object.getClassDynType2();
      this.total_dyn_type1 = this.dyn_type1 | inherit_dyn_type1;
      this.total_dyn_type2 = this.dyn_type2 | inherit_dyn_type2;
    }

    if ((this.action_type1 & ActionType1.Inherit) === 0) {
      this.total_action_type1 = this.action_type1;
      this.total_action_type2 = this.action_type2;
    } else {
      let inherit_action_type1 = object.getClassActionType1();
      let inherit_action_type2 = object.getClassActionType2();
      this.total_action_type1 = this.action_type1 | inherit_action_type1;
      this.total_action_type2 = this.action_type2 | inherit_action_type2;
    }

    if (this.cycle === Cycle.Inherit) {
      this.cycle = object.getClassCycle();
      if (this.cycle === Cycle.Inherit) {
        this.cycle = Cycle.Slow;
      }
    }
    this.elements.forEach(e => e.connect(object));

    return 1;
  }

  disconnect() {
    this.elements.forEach(e => e.disconnect());
  }

  scan(object) {
    this.repaintNow = false;
    this.ignoreColor = false;
    this.resetColor = false;
    this.ignoreBgColor = false;
    this.resetBgColor = false;

    this.elements.forEach(e => e.scan(object));
    if (this.repaintNow) {
      object.draw();
    }
  }

  action(object, event) {
    for (let i = 0; i < this.elements.length; i++) {
      let sts = this.elements[i].action(object, event);
      if (sts === DYN__NO_PROPAGATE || sts === GLOW__TERMINATED ||
          sts === GLOW__SUBTERMINATED) {
        return sts;
      }
    }

    return 1;
  }

  setTotal(object) {
    if (object === null) {
      this.total_dyn_type1 = this.dyn_type1;
      this.total_dyn_type2 = this.dyn_type2;
      this.total_action_type1 = this.action_type1;
      this.total_action_type2 = this.action_type2;
    } else {
      if ((this.dyn_type1 & DynType1.Inherit) === 0) {
        this.total_dyn_type1 = this.dyn_type1;
        this.total_dyn_type2 = this.dyn_type2;
      } else {
        let inherit_dyn_type1 = object.getClassDynType1();
        let inherit_dyn_type2 = object.getClassDynType2();
        this.total_dyn_type1 = this.dyn_type1 | inherit_dyn_type1;
        this.total_dyn_type2 = this.dyn_type2 | inherit_dyn_type2;
      }

      if ((this.action_type1 & ActionType1.Inherit) === 0) {
        this.total_action_type1 = this.action_type1;
        this.total_action_type2 = this.action_type2;
      } else {
        let inherit_action_type1 = object.getClassActionType1();
        let inherit_action_type2 = object.getClassActionType2();
        this.total_action_type1 = this.action_type1 | inherit_action_type1;
        this.total_action_type2 = this.action_type2 | inherit_action_type2;
      }
    }
  }

  parseAttrName(name) {
    if (name === null) {
      return null;
    }
    if ((this.total_dyn_type1 & DynType1.HostObject) !== 0) {
      let idx = name.indexOf("$hostobject");
      if (idx !== -1) {
        let hostobject = this.getHostObject();
        let n = name.substring(0, idx) + hostobject + name.substring(idx + 11);
        return this.graph.parseAttrName(n);
      }
    }

    return this.graph.parseAttrName(name);
  }

  getHostObject() {
    let elem = <DynHostObject>this.elements.find(e => (e.dyn_type1 & DynType1.HostObject) !== 0);
    return elem ? elem.hostobject : "";
  }

  getDig(p, a_typeid, bitmask, db): Boolean {
    let getObjectRefInfo = function(p) {
      if (db === Database.Gdh) {
        return this.graph.getGdh().getObjectRefInfo(p);
      } else if (db === Database.Local) {
        return this.graph.getLdb().getObjectRefInfo(p);
      }
      return 0;
    };
    switch (a_typeid) {
      case Type.Boolean:
        return Boolean(getObjectRefInfo(p));
      case Type.Int32:
      case Type.UInt32:
      case Type.Int64:
      case Type.UInt64:
        return Boolean(getObjectRefInfo(p));
      case Type.Bit:
        return (getObjectRefInfo(p) & bitmask) !== 0;
      case Type.Float32:
      case Type.Float64:
        return getObjectRefInfo(p) > Number.MIN_VALUE;
      case Type.String:
        if (db === Database.Gdh) {
          return this.graph.getGdh().getObjectRefInfo(p) !== "";
        }
        return false;
      default:
        return false;
    }
  }

  confirmedAction(event, object) {
    if (this.elements === null) {
      return;
    }

    let e = new GlowEvent();
    e.event = event;
    e.object = object;
    this.action_type1 &= ~ActionType1.Confirm;
    this.elements.forEach(el => el.action(object, e));
    this.action_type1 |= ActionType1.Confirm;
  }

  static instance_to_number(instance) {
    let inst = 1;
    let m = instance;
    while (m > 1) {
      m >>= 1;
      inst++;
    }

    return inst;
  }

  valueInputAction(object, str) {
    if (this.elements === null) {
      return ValueInput.Error;
    }

    let e = object;

    let ctx_popped = e.dyn.graph.ctxPop(object.dyn.object.ctx);

    let sts = null;
    let minval = 0;
    let maxval = 0;
    if (e.minvalue_attr === null) {
      minval = e.min_value;
    } else {
      let pname = e.dyn.parseAttrName(e.minvalue_attr);
      if (pname && pname.type === Type.Float32) {
        /** TODO
         CdhrFloat ret = e.dyn.graph.getGdh().getObjectInfoFloat( pname.name);
         if (even(ret)) {
	  if ( ctx_popped)
	    e.dyn.graph.ctxPush();
	  return ValueInput.Error;
	}
         minval = ret.value;
         **/
      } else {
        minval = e.min_value;
      }
    }

    if (e.maxvalue_attr === null) {
      maxval = e.max_value;
    } else {
      let pname = e.dyn.parseAttrName(e.maxvalue_attr);
      if (pname === null || pname.name === "") {
        maxval = e.max_value;
      } else {
        if (pname.type === Type.Float32) {
          /** TODO
           CdhrFloat ret = e.dyn.graph.getGdh().getObjectInfoFloat( pname.name);
           if (even(ret)) {
	    if ( ctx_popped)
	      e.dyn.graph.ctxPush();
	    return ValueInput.Error;
	  }
           maxval = ret.value;
           **/
        } else {
          maxval = e.max_value;
        }
      }
    }

    let setObjectInfo = function(pname, inputValue, type: Type) {
      let sts = 0;
      if (pname.database === Database.Gdh) {
        sts = e.dyn.graph.getGdh().setObjectInfo(pname.name, inputValue, type);
      } else if (pname.database === Database.Local) {
        sts = e.dyn.graph.getLdb().setObjectInfo(this.graph, pname.name, inputValue);
      }

      if (odd(sts)) {
        return ValueInput.Success;
      }

      console.log("setObjectInfoError " + sts);
      return ValueInput.Error;
    };

    sts = 0;
    switch (e.a_typeid) {
      case Type.Float32:
        let inputValue = parseFloat(str.trim());
        if (minval !== 0 && maxval !== 0 && inputValue < minval) {
          if (ctx_popped) {
            e.dyn.graph.ctxPush();
          }
          return ValueInput.MinValueExceeded;
        }
        if (minval !== 0 && maxval !== 0 && inputValue > maxval) {
          if (ctx_popped) {
            e.dyn.graph.ctxPush();
          }
          return ValueInput.MaxValueExceeded;
        }

        let pname = e.dyn.parseAttrName(e.value_element.attribute);
        if (pname === null || pname.name === "") {
          break;
        }

        sts = setObjectInfo(pname, inputValue, e.a_typeid);
        break;
      case Type.Int32:
      case Type.UInt32:
      case Type.Int16:
      case Type.UInt16:
      case Type.Int8:
      case Type.UInt8:
        let inputValue = parseInt(str.trim(), 10);
        if (minval !== 0 && maxval !== 0 && inputValue < minval) {
          if (ctx_popped) {
            e.dyn.graph.ctxPush();
          }
          return ValueInput.MinValueExceeded;
        }
        if (minval !== 0 && maxval !== 0 && inputValue > maxval) {
          if (ctx_popped) {
            e.dyn.graph.ctxPush();
          }
          return ValueInput.MaxValueExceeded;
        }

        let pname = e.dyn.parseAttrName(e.value_element.attribute);
        if (pname === null || pname.name === "") {
          break;
        }

        sts = setObjectInfo(pname, inputValue, e.a_typeid);
        break;
      case Type.Boolean:
        let inputValueInt = parseInt(str.trim(), 10);
        if (inputValueInt !== 0 && inputValueInt !== 1) {
          break;
        }

        // valueElement.oldValueB = inputValue;

        let pname = e.dyn.parseAttrName(e.value_element.attribute);
        if (pname === null || pname.name === "") {
          break;
        }

        sts = setObjectInfo(pname, Boolean(inputValueInt), e.a_typeid);
        break;
      case Type.String:
        // valueElement.oldValueS = str;

        let pname = e.dyn.parseAttrName(e.value_element.attribute);
        if (pname === null || pname.name === "") {
          break;
        }

        sts = setObjectInfo(pname, inputValue, e.a_typeid);
        break;
    }
    if (ctx_popped) {
      e.dyn.graph.ctxPush();
    }

    if (sts === ValueInput.Error) {
      return sts;
    }

    return ValueInput.Success;
  }

  static value_to_msg(value) {
    if (value === 0) {
      return "";
    } else {
      let str = "NO";
      if (value & 7 < 5) {
        str = ["WARNING", "SUCCESS", "ERROR", "SUCCESS", "FATAL"][value & 7];
      }
      str = "MSG-" + str + ", ";
      switch (value) {
        case 134512649:
          return str + "Success";
        case 134512659:
          return str + "Proview startup pu";
        case 134512665:
          return str + "Proview running";
        case 134512731:
          return str + "Server running";
        case 134512795:
          return str + "Application running";
        case 134512820:
          return str + "Process timeout";
        default:
          return str + "Message number " + value;
      }
    }
  }
}

class DynReference {
  name;
  pname;
  p = 0;
  database;
  inverted;
  attrFound = false;
  oldValue;
  bittmask;
  elements;
  typeid;
  sts = false;
  bitmask;

  constructor(dyn, attribute) {
    this.name = attribute;

    this.pname = dyn.parseAttrName(this.name);
    if (this.pname === null || this.pname.name === "") {
      return;
    }
    this.sts = true;
  }

  connect(dyn) {
    if (!this.sts) {
      return;
    }
    this.sts = false;
    switch (this.pname.database) {
      case Database.Gdh:
        this.p = dyn.graph.getGdh()
            .refObjectInfo(this.pname.tname, this.pname.type,
                this.pname.elements);
        break;
      case Database.Local:
        this.p = dyn.graph.getLdb().refObjectInfo(dyn.graph, this.pname.name);
        break;
      default:
        this.p = 0;
        break;
    }

    if (this.p === 0) {
      return;
    }

    this.inverted = this.pname.inverted;
    this.typeid = this.pname.type;
    this.bitmask = this.pname.bitmask;
    this.database = this.pname.database;
    this.elements = this.pname.elements;
    this.pname = null;
    this.attrFound = true;
    this.sts = true;
  }

  disconnect(dyn) {
    if (this.sts && this.database === Database.Gdh) {
      dyn.graph.getGdh().unrefObjectInfo(this.p);
      this.sts = false;
    }
  }

  get_ref_value(dyn) {
    switch (this.database) {
      case Database.Gdh:
        return dyn.graph.getGdh().getObjectRefInfo(this.p);
      case Database.Local:
        return dyn.graph.getLdb().getObjectRefInfo(this.p);
    }
    return null;
  }
}

class DynElem {
  dyn: Dyn;
  dyn_type1: DynType1 = 0;
  dyn_type2: DynType2 = 0;
  action_type1: ActionType1 = 0;
  action_type2: ActionType2 = 0;
  attribute;
  prio: DynPrio;
  instance_mask = 0;
  instance: Instance = 0;

  constructor(dyn, prio) {
    this.dyn = dyn;
    this.prio = prio;
  }

  action(object, e) {
    return 1;
  }

  connect(o) {
    return 1;
  }

  disconnect() {
  }

  scan(o) {
  }
}

class DynDigLowColor extends DynElem {
  a = null;
  color;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.DigLowColor);
    this.dyn_type1 = DynType1.DigLowColor;
  }

  connect(o) {
    this.color = Dyn.getColor1(o, this.color);
    if (this.color < 0 || this.color >= DrawType._) {
      return 0;
    }

    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigLowColor: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    if (this.a) {
      this.a.disconnect(this.dyn);
    }
  }

  scan(o) {
    let object = o;
    if (this.a === null || !this.a.sts || this.dyn.ignoreColor) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value && !this.dyn.resetColor) {
        return;
      }
    }

    if ((this.dyn.total_dyn_type1 & DynType1.Tone) === 0) {
      if (value) {
        object.resetFillColor();
      } else {
        object.setFillColor(this.color);
      }
      this.dyn.repaintNow = true;
    } else {
      if (value) {
        if (this.color >= DrawTone._) {
          object.resetFillColor();
        }
        object.resetColorTone();
      } else {
        if (this.color >= DrawTone._) {
          object.setFillColor(this.color);
        } else {
          object.setColorTone(this.color);
        }
      }
      this.dyn.repaintNow = true;
    }
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigLowColor:
          break;
        case DynSave.DigLowColor_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigLowColor_color:
          this.color = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigLowColor");
          break;
      }
    }

    return i;
  }
}

class DynDigColor extends DynElem {
  a = null;
  color;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.DigColor);
    this.dyn_type1 = DynType1.DigColor;
  }

  connect(object) {
    this.color = Dyn.getColor2(object, this.color);
    if (this.color < 0 || this.color >= DrawType._) {
      return 0;
    }

    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigColor: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    if (this.a) {
      this.a.disconnect(this.dyn);
    }
  }

  scan(object) {
    if (this.a === null || !this.a.sts || this.dyn.ignoreColor) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value && !this.dyn.resetColor) {
        // No change since last time
        if (value) {
          this.dyn.ignoreColor = true;
        }
        return;
      }
    }

    if ((this.dyn.total_dyn_type1 & DynType1.Tone) === 0) {
      if (value) {
        object.setFillColor(this.color);
        this.dyn.ignoreColor = true;
      } else {
        object.resetFillColor();
        this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    } else {
      if (value) {
        if (this.color >= DrawTone._) {
          if (this.dyn.resetColor) {
            object.resetFillColor();
          }
          object.setFillColor(this.color);
        } else {
          object.setColorTone(this.color);
        }
        this.dyn.ignoreColor = true;
      } else {
        if (this.color >= DrawTone._) {
          object.resetFillColor();
        }
        object.resetColorTone();
        this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigColor:
          break;
        case DynSave.DigColor_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigColor_color:
          this.color = parseInt(tokens[1], 10);
          break;
        case DynSave.DigColor_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.DigColor_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigColor");
          break;
      }
    }

    return i;
  }
}

class DynDigWarning extends DynElem {
  a;
  use_colortheme;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.DigWarning);
    this.dyn_type1 = DynType1.DigWarning;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigWarning: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts || this.dyn.ignoreColor) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value && !this.dyn.resetColor) {
        // No change since last time
        if (value) {
          this.dyn.ignoreColor = true;
        }
        return;
      }
    }

    if ((this.dyn.total_dyn_type1 & DynType1.Tone) === 0) {
      if (value) {
        object.setFillColor(
            (this.use_colortheme === 0) ? DrawType.ColorYellow :
                DrawType.CustomColor41);
        this.dyn.ignoreColor = true;
      } else {
        object.resetFillColor();
        this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    } else {
      if (value) {
        object.setColorTone(DrawTone.Yellow);
        this.dyn.ignoreColor = true;
      } else {
        object.resetFillColor();
        object.resetColorTone();
        this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigWarning:
          break;
        case DynSave.DigWarning_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigWarning_use_colortheme:
          this.use_colortheme = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigWarning");
          break;
      }
    }

    return i;
  }
}

class DynDigError extends DynElem {
  a;
  use_colortheme;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.DigError);
    this.dyn_type1 = DynType1.DigError;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigError: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts || this.dyn.ignoreColor) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value && !this.dyn.resetColor) {
        // No change since last time
        if (value) {
          this.dyn.ignoreColor = true;
        }
        return;
      }
    }

    if ((this.dyn.total_dyn_type1 & DynType1.Tone) === 0) {
      if (value) {
        object.setFillColor(
            (this.use_colortheme === 0) ? DrawType.ColorRed :
                DrawType.CustomColor11);
        this.dyn.ignoreColor = true;
      } else {
        object.resetFillColor();
        this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    } else {
      if (value) {
        object.setColorTone(DrawTone.Red);
        this.dyn.ignoreColor = true;
      } else {
        object.resetFillColor();
        object.resetColorTone();
        this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigError:
          break;
        case DynSave.DigError_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigError_use_colortheme:
          this.use_colortheme = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigError");
          break;
      }
    }

    return i;
  }
}

class DynDigFlash extends DynElem {
  a;
  color;
  color2;
  firstScan = true;
  on = false;

  constructor(dyn) {
    super(dyn, DynPrio.DigFlash);
    this.dyn_type1 = DynType1.DigFlash;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigFlash: " + this.attribute);
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts || this.dyn.ignoreColor) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value && !this.dyn.resetColor) {
        // No change since last time
        if (!value) {
          return;
        }
      }
    }

    if ((this.dyn.total_dyn_type1 & DynType1.Tone) === 0) {
      if (value) {
        if (this.on) {
          object.setFillColor(this.color);
          this.dyn.ignoreColor = true;
        } else {
          if (this.color2 === DrawType.Inherit) {
            object.resetFillColor();
            this.dyn.resetColor = true;
          } else {
            object.setFillColor(this.color2);
            this.dyn.ignoreColor = true;
          }
        }
        this.on = !this.on;
      } else {
        object.resetFillColor();
        this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    } else {
      if (value) {
        if (this.on) {
          if (this.color >= DrawTone._) {
            object.setColorTone(this.color);
          } else {
            object.setFillColor(this.color);
          }
          this.dyn.ignoreColor = true;
        } else {
          object.resetFillColor();
          object.resetColorTone();
          this.dyn.resetColor = true;
        }
        this.on = !this.on;
      } else {
        object.resetFillColor();
        object.resetColorTone();
        this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigFlash:
          break;
        case DynSave.DigFlash_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigFlash_color:
          this.color = parseInt(tokens[1], 10);
          break;
        case DynSave.DigFlash_color2:
          this.color2 = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigFlash");
          break;
      }
    }

    return i;
  }
}

class DynInvisible extends DynElem {
  a;
  dimmed;
  firstScan = true;
  cmd;
  command;

  constructor(dyn) {
    super(dyn, DynPrio.Invisible);
    this.dyn_type1 = DynType1.Invisible;
  }

  connect(object) {
    if (this.attribute &&
        this.attribute.toLowerCase().substring(0, 5) === "$cmd(") {
      this.cmd = true;
      let idx = this.attribute.lastIndexOf(')');
      if (idx !== -1 && idx > 5) {
        this.command = this.attribute.substring(5, idx);
      } else {
        this.command = this.attribute.substring(5);
      }
      this.command = this.dyn.graph.getCommand(this.command);
    } else {
      this.a = new DynReference(this.dyn, this.attribute);
      this.a.connect(this.dyn);
      if (!this.a.sts) {
        console.log("DigFlash: " + this.attribute);
        return 1;
      }
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(o) {
    if (this.cmd) {
      if (this.firstScan) {
        let sts = this.dyn.graph.command(this.command);
        console.log("DynInvisible: " + this.command + ", value: " + sts);
        if (sts === 0) {
          if (this.dimmed === 0) {
            o.setVisibility(Vis.Invisible);
          } else {
            o.setVisibility(Vis.Dimmed);
          }
        }
        this.firstScan = false;
      }
      return;
    }

    if (!this.a.sts || this.dyn.ignoreColor) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    if (this.a.typeid === Type.String) {
      value = !value;
    }

    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value && !this.dyn.resetColor) {
        // No change since last time
        if (!value) {
          return;
        }
      }
    }

    if (value) {
      if (this.dimmed === 0) {
        o.setVisibility(Vis.Invisible);
      } else {
        o.setVisibility(Vis.Dimmed);
      }
    } else {
      o.setVisibility(Vis.Visible);
    }
    this.dyn.repaintNow = true;
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Invisible:
          break;
        case DynSave.Invisible_attribute:
          if (tokens.length > 1) {
            this.attribute = lines[i].substring(4);
          }
          break;
        case DynSave.Invisible_dimmed:
          this.dimmed = parseInt(tokens[1], 10);
          break;
        case DynSave.Invisible_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.Invisible_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynInvisible");
          break;
      }
    }

    return i;
  }
}

class DynDigTextColor extends DynElem {
  a = null;
  color;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.DigTextColor);
    this.dyn_type2 = DynType2.DigTextColor;
  }

  connect(object) {
    this.color = Dyn.getColor1(object, this.color);
    if (this.color < 0 || this.color >= DrawType._) {
      return 0;
    }

    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigTextColor: " + this.attribute);
    }

    return 1;
  }

  disconnect() {
    if (this.a) {
      this.a.disconnect(this.dyn);
    }
  }

  scan(object) {
    if (this.a === null || !this.a.sts || this.dyn.ignoreColor) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value && !this.dyn.resetColor) {
        return;
      }
    }

    if (value) {
      object.setTextColor(this.color);
    } else {
      object.resetTextColor();
    }
    this.dyn.repaintNow = true;

    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigTextColor:
          break;
        case DynSave.DigTextColor_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigTextColor_color:
          this.color = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigTextColor");
          break;
      }
    }

    return i;
  }
}

class DynDigText extends DynElem {
  a;
  low_text;
  high_text;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.DigText);
    this.dyn_type1 = DynType1.DigText;
  }

  connect(object) {
    if (this.low_text === null) {
      return 1;
    }

    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigText: " + this.attribute);
      return 1;
    }
    this.high_text = object.getAnnotation(1);
    if (this.high_text === null) {
      this.high_text = "";
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value) {
        return;
      }
    }

    if (value) {
      object.setAnnotation(1, this.high_text);
    } else {
      object.setAnnotation(1, this.low_text);
    }
    this.dyn.repaintNow = true;
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigText:
          break;
        case DynSave.DigText_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigText_low_text:
          if (tokens.length > 1) {
            this.low_text = lines[i].substring(5);
          }
          break;
        case DynSave.DigText_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.DigText_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigText");
          break;
      }
    }

    return i;
  }
}

class DynDigBorder extends DynElem {
  a;
  color;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.DigBorder);
    this.dyn_type1 = DynType1.DigBorder;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigBorder: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value) {
        // No change since last time
        return;
      }
    }

    if (value) {
      object.setBorderColor(this.color);
    } else {
      object.resetBorderColor();
    }
    this.dyn.repaintNow = true;
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigBorder:
          break;
        case DynSave.DigBorder_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigBorder_color:
          this.color = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigBorder");
          break;
      }
    }

    return i;
  }
}

class DynValue extends DynElem {
  a;
  format = null;
  zero_blank;
  decimals_attr;
  decimals_decr;
  firstScan = true;
  oldValueB;
  oldValueF;
  oldValueI;
  oldValueS;
  cFormat = null;

  constructor(dyn) {
    super(dyn, DynPrio.Value);
    this.dyn_type1 = DynType1.Value;
  }

  read_decimals(dyn, attr, decr, format) {
    let decimals;

    let pname = dyn.parseAttrName(attr);
    if (pname === null || pname.name === "") {
      return format;
    }

    switch (pname.type) {
      case Type.Int32:
      case Type.UInt32:
      case Type.Enum:
      case Type.Mask:
        break;
      default:
        return format;
    }

    let sts = this.dyn.graph.getGdh().getObjectInfoInt(pname.name);
    if (even(sts)) {
      return format;
    }

    decimals = Math.max(sts - decr, 0);
    if (decimals >= 10) {
      return format;
    }

    if (!format) {
      return "%." + decimals + "f";
    }

    // Print format, of replace digit between . and f
    let s = format.indexOf('f');
    if (s === -1) {
      return "%." + decimals + "f";
    } else {
      if (s < 2 || format.charAt(s - 2) !== '.') {
        return "%." + decimals + "f";
      } else {
        return format.substring(0, format.indexOf('.') + 1) + decimals + "f";
      }
    }
  }

  connect(object) {
    if (this.decimals_attr) {
      this.format =
          this.read_decimals(this.dyn, this.decimals_attr, this.decimals_decr,
              this.format);
      console.log("read_decimals " + this.format);
    }
    if (this.format) {
      this.cFormat = new GlowCFormat(this.format);

      this.a = new DynReference(this.dyn, this.attribute);
      this.a.connect(this.dyn);
      if (!this.a.sts) {
        console.log("Value: " + this.attribute);
      }
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts) {
      return;
    }

    let annot_num = Dyn.instance_to_number(this.instance);

    let value0 = this.a.get_ref_value(this.dyn);
    if (value0 === undefined) {
      return;
    }

    switch (this.a.typeid) {
      case Type.Float32:
        if (value0 !== this.oldValueF || this.firstScan) {
          if (this.cFormat) {
            let sb = this.cFormat.format(value0);
            object.setAnnotation(annot_num, sb);
          }
          this.dyn.repaintNow = true;
          this.oldValueF = value0;
        }
        break;
      case Type.Int32:
      case Type.UInt32:
      case Type.Int16:
      case Type.UInt16:
      case Type.Int8:
      case Type.UInt8:
        if (value0 !== this.oldValueI || this.firstScan) {
          if (this.cFormat) {
            let sb = this.cFormat.format(value0);
            object.setAnnotation(annot_num, sb);
          }
          this.dyn.repaintNow = true;
          this.oldValueI = value0;
        }
        break;
      case Type.Boolean:
        if (value0 !== this.oldValueB || this.firstScan) {
          if (this.cFormat) {
            let sb = this.cFormat.format(value0);
            object.setAnnotation(annot_num, sb);
          }
          this.dyn.repaintNow = true;
          this.oldValueB = value0;
        }
        break;
      case Type.String:
      case Type.Objid:
      case Type.Time:
      case Type.DeltaTime:
        if (this.firstScan || !(value0 === this.oldValueS)) {
          if (this.cFormat) {
            if (this.a.typeid === Type.String) {
              let sb = this.cFormat.format(value0);
              object.setAnnotation(annot_num, sb);
            } else { // TODO time format
              object.setAnnotation(annot_num, value0);
            }
          }
          this.dyn.repaintNow = true;
          this.oldValueS = value0;
        }
        break;
      case Type.Status:
      case Type.NetStatus:
        if (value0 !== this.oldValueI || this.firstScan) {
          if (value0 === 0) {
            object.setAnnotation(annot_num, "");
            this.dyn.repaintNow = true;
          } else {
            if (this.a.database === Database.Gdh) {
              this.dyn.graph.getGdh().getMsg(value0).then(this.scan2(object));
            }
          }
        }
        /** TODO
         switch ( this.cFormat.type()) {
	case GlowCFormat.FRM_M: {
	  let cstr = this.dyn.graph.getGdh().getMsg( value0);
	  object.setAnnotation( annot_num, cstr.str);
	  this.dyn.repaintNow = true;
	  break;
	}
	case GlowCFormat.FRM_1M: {
	  let cstr = this.dyn.graph.getGdh().getMsgText( value0);
	  object.setAnnotation( annot_num, cstr.str);
	  this.dyn.repaintNow = true;
	  break;
	}
	}
         **/
        this.oldValueI = value0;
        break;
    }
    if (this.firstScan) {
      this.firstScan = false;
    }
  }

  scan2(object) {
    return function(res) {
      let annot_num = Dyn.instance_to_number(this.instance);
      if (res.sts & 1 !== 0) {
        object.setAnnotation(annot_num, res.value);
      } else {
        object.setAnnotation(annot_num, "Unknown message");
      }
      this.dyn.repaintNow = true;
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Value:
          break;
        case DynSave.Value_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.Value_format:
          if (tokens.length > 1) {
            this.format = tokens[1];
          }
          break;
        case DynSave.Value_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.Value_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.Value_zero_blank:
          this.zero_blank = parseInt(tokens[1], 10);
          break;
        case DynSave.Value_decimals_attr:
          if (tokens.length > 1) {
            this.decimals_attr = tokens[1];
          }
          break;
        case DynSave.Value_decimals_decr:
          this.decimals_decr = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          if (this.format) {
            this.cFormat = new GlowCFormat(this.format);
          }
          return i;
        default:
          console.log("Syntax error in DynValue");
          break;
      }
    }

    if (this.format) {
      this.cFormat = new GlowCFormat(this.format);
    }

    return i;
  }
}

class DynValueInput extends DynElem {
  min_value;
  max_value;
  clear;
  popup;
  unselect;
  minvalue_attr;
  maxvalue_attr;
  escape_store;
  keyboard_type;
  update_open;
  value_element = null;
  a_typeid;

  constructor(dyn) {
    super(dyn, DynPrio.ValueInput);
    this.action_type1 = ActionType1.ValueInput;
  }

  connect(object) {
    // Get the Value element
    this.value_element = this.dyn.elements.find(function (e) {
      if (e.dyn_type1 === DynType1.Value) {
        this.a_typeid = this.value_element.a.typeid;
        return true;
      }
    }) || null;

    return 1;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Click:
        this.dyn.object = object;
        this.dyn.graph.openValueInputDialog(this.dyn, "Enter value", this);
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.ValueInput:
          break;
        case DynSave.ValueInput_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.ValueInput_format:
          break;
        case DynSave.ValueInput_unselect:
          this.unselect = parseInt(tokens[1], 10);
          break;
        case DynSave.ValueInput_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case DynSave.ValueInput_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case DynSave.ValueInput_clear:
          this.clear = parseInt(tokens[1], 10);
          break;
        case DynSave.ValueInput_popup:
          this.popup = parseInt(tokens[1], 10);
          break;
        case DynSave.ValueInput_minvalue_attr:
          if (tokens.length > 1) {
            this.minvalue_attr = tokens[1];
          }
          break;
        case DynSave.ValueInput_maxvalue_attr:
          if (tokens.length > 1) {
            this.maxvalue_attr = tokens[1];
          }
          break;
        case DynSave.ValueInput_escape_store:
          this.escape_store = parseInt(tokens[1], 10);
          break;
        case DynSave.ValueInput_keyboard_type:
          this.keyboard_type = parseInt(tokens[1], 10);
          break;
        case DynSave.ValueInput_update_open:
          this.update_open = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynValueInput");
          break;
      }
    }

    return i;
  }
}

class DynAnalogColor extends DynElem {
  a = null;
  limit;
  limit_type;
  color;
  border;
  common_attr;
  firstScan = true;
  oldValueF;
  oldValueI;
  oldState;
  isMainInstance = false;

  constructor(dyn) {
    super(dyn, DynPrio.AnalogColor);
    this.dyn_type1 = DynType1.AnalogColor;
  }

  connect(object) {
    // Find the main instance
    let mainInstance = null;
    for (let i = this.dyn.elements.length - 1; i >= 0; i--) {
      if (this.dyn.elements[i].dyn_type1 === DynType1.AnalogColor) {
        mainInstance = this.dyn.elements[i];
        break;
      }
    }

    if (mainInstance) {
      if (mainInstance.a === null || !mainInstance.a.attrFound) {
        mainInstance.a = new DynReference(this.dyn, mainInstance.attribute);
        mainInstance.a.connect(this.dyn);
        if (!mainInstance.a.sts) {
          return 1;
        }
        mainInstance.isMainInstance = true;
      }
      if (!this.isMainInstance) {
        this.a = new DynReference(this.dyn, mainInstance.attribute);
        this.a.p = mainInstance.a.p;
        this.a.attrFound = mainInstance.a.attrFound;
        this.a.typeid = mainInstance.a.typeid;
        this.a.database = mainInstance.a.database;
        this.a.sts = mainInstance.a.sts;
      }
    }

    return 1;
  }

  disconnect() {
    if (this.isMainInstance) {
      this.a.disconnect(this.dyn);
    }
  }

  scan(object) {
    if (!this.a.sts || this.dyn.ignoreColor) {
      return;
    }

    let state = false;
    let set_color = false;
    let reset_color = false;

    if (this.a.typeid === Type.Float32) {
      let value = this.a.get_ref_value(this.dyn);
      let i;

      if (this.firstScan) {
        this.firstScan = false;
      } else {
        if (!this.dyn.resetColor && value === this.oldValueF) {
          if (this.oldState) {
            this.dyn.ignoreColor = true;
          }
          return;
        }
      }

      switch (this.limit_type) {
        case LimitType.Gt:
          state = (value > this.limit);
          break;
        case LimitType.Lt:
          state = (value < this.limit);
          break;
        case LimitType.Ge:
          state = (value >= this.limit);
          break;
        case LimitType.Le:
          state = (value >= this.limit);
          break;
        case LimitType.Eq:
          state = (value === this.limit);
          break;
      }
      this.oldValueF = value;
    } else if (this.a.typeid === Type.Int32 ||
        this.a.typeid === Type.UInt32) {
      let value = this.dyn.graph.getGdh().getObjectRefInfo(this.a.p);
      let i;
      if (this.firstScan) {
        this.firstScan = false;
      } else {
        if (!this.dyn.resetColor && value === this.oldValueI) {
          if (this.oldState) {
            this.dyn.ignoreColor = true;
          }
          return;
        }
      }

      switch (this.limit_type) {
        case LimitType.Gt:
          state = (value > this.limit);
          break;
        case LimitType.Lt:
          state = (value < this.limit);
          break;
        case LimitType.Ge:
          state = (value >= this.limit);
          break;
        case LimitType.Le:
          state = (value >= this.limit);
          break;
        case LimitType.Eq:
          state = (value === this.limit);
          break;
      }
      this.oldValueI = value;
    }

    if (state !== this.oldState || this.dyn.resetColor || this.firstScan) {
      if (state) {
        set_color = true;
        this.dyn.ignoreColor = true;
      } else {
        reset_color = true;
        this.dyn.resetColor = true;
      }
      this.oldState = state;
    } else if (state) {
      this.dyn.ignoreColor = true;
    }

    if (!set_color && !reset_color) {
      return;
    }

    if ((this.dyn.total_dyn_type1 & DynType1.Tone) === 0) {
      if (set_color) {
        if (this.border === 0) {
          object.setFillColor(this.color);
        } else {
          object.setBorderColor(this.color);
        }
        this.dyn.ignoreColor = true;
      } else {
        if (this.border === 0) {
          object.resetFillColor();
        } else {
          object.resetBorderColor();
        }
        this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    } else {
      if (set_color) {
        if (this.color >= DrawTone._) {
          if (this.border === 0) {
            object.setFillColor(this.color);
          } else {
            object.setBorderColor(this.color);
          }
        } else {
          object.setColorTone(this.color);
        }
        this.dyn.ignoreColor = true;
      } else {
        if (this.color >= DrawTone._) {
          if (this.border === 0) {
            object.resetFillColor();
          } else {
            object.resetBorderColor();
          }
        }
        object.resetColorTone();
        this.dyn.resetColor = true;
      }
      this.dyn.repaintNow = true;
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.AnalogColor:
          break;
        case DynSave.AnalogColor_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.AnalogColor_limit:
          this.limit = parseFloat(tokens[1]);
          break;
        case DynSave.AnalogColor_limit_type:
          this.limit_type = parseInt(tokens[1], 10);
          break;
        case DynSave.AnalogColor_color:
          this.color = parseInt(tokens[1], 10);
          break;
        case DynSave.AnalogColor_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.AnalogColor_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.AnalogColor_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case DynSave.AnalogColor_common_attr:
          this.common_attr = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynAnalogColor");
          break;
      }
    }

    return i;
  }
}

class DynRotate extends DynElem {
  a;
  x0;
  y0;
  factor;
  firstScan = true;
  rotation_point;

  constructor(dyn) {
    super(dyn, DynPrio.Rotate);
    this.dyn_type1 = DynType1.Rotate;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("Rotate: " + this.attribute);
      return 1;
    }

    if (this.x0 !== 0 || this.y0 !== 0) {
      this.rotation_point = RotationPoint.FixPoint;
    } else {
      this.rotation_point = RotationPoint.Zero;
    }

    object.storeTransform();
    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts) {
      return;
    }
    let value0 = this.a.get_ref_value(this.dyn);
    if (value0 !== this.a.oldValue || this.firstScan) {
      object.setRotation(value0 * this.factor, this.x0, this.y0,
          this.rotation_point);

      this.dyn.repaintNow = true;
      this.a.oldValue = value0;
    }
    if (this.firstScan) {
      this.firstScan = false;
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Rotate:
          break;
        case DynSave.Rotate_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.Rotate_x0:
          this.x0 = parseFloat(tokens[1]);
          break;
        case DynSave.Rotate_y0:
          this.y0 = parseFloat(tokens[1]);
          break;
        case DynSave.Rotate_factor:
          this.factor = parseFloat(tokens[1]);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynRotate");
          break;
      }
    }

    return i;
  }
}

class DynMove extends DynElem {
  move_x_a;
  move_y_a;
  scale_x_a;
  scale_y_a;
  move_x_attribute;
  move_y_attribute;
  scale_x_attribute;
  scale_y_attribute;
  x_factor;
  y_factor;
  x_offset;
  y_offset;
  scale_x_factor;
  scale_y_factor;
  scale_type;
  firstScan = true;
  x_orig;
  y_orig;
  width_orig;
  height_orig;

  constructor(dyn) {
    super(dyn, DynPrio.Move);
    this.dyn_type1 = DynType1.Move;
  }

  connect(object) {
    this.move_x_a = new DynReference(this.dyn, this.move_x_attribute);
    this.move_x_a.connect(this.dyn);

    let tmp = [Type.Float32, Type.Float64, Type.Int32, Type.UInt32];
    if (this.move_x_a.sts && tmp.indexOf(this.move_x_a.typeid) === -1) {
      this.move_x_a.disconnect(this.dyn);
      console.log("Move: " + this.move_x_attribute);
      return 1;
    }

    this.move_y_a = new DynReference(this.dyn, this.move_y_attribute);
    this.move_y_a.connect(this.dyn);
    if (this.move_y_a.sts && tmp.indexOf(this.move_y_a.typeid) === -1) {
      this.move_y_a.disconnect(this.dyn);
      console.log("Move: " + this.move_y_attribute);
      return 1;
    }

    this.scale_x_a = new DynReference(this.dyn, this.scale_x_attribute);
    this.scale_x_a.connect(this.dyn);
    if (this.scale_x_a.sts && tmp.indexOf(this.scale_x_a.typeid) === -1) {
      this.scale_x_a.disconnect(this.dyn);
      console.log("Move: " + this.scale_x_attribute);
      return 1;
    }

    this.scale_y_a = new DynReference(this.dyn, this.scale_y_attribute);
    this.scale_y_a.connect(this.dyn);
    if (this.scale_y_a.sts && tmp.indexOf(this.scale_y_a.typeid) === -1) {
      this.scale_y_a.disconnect(this.dyn);
      console.log("Move: " + this.scale_y_attribute);
      return 1;
    }

    if (!object.transformIsStored()) {
      object.storeTransform();
      let geom = object.measure();
      this.x_orig = geom.ll_x;
      this.y_orig = geom.ll_y;
      this.width_orig = geom.width();
      this.height_orig = geom.height();
    }

    return 1;
  }

  disconnect() {
    this.move_x_a.disconnect(this.dyn);
    this.move_y_a.disconnect(this.dyn);
    this.scale_x_a.disconnect(this.dyn);
    this.scale_y_a.disconnect(this.dyn);
  }

  scan(object) {
    let update = false;

    let move_x_value = 0;
    let move_y_value = 0;
    let scale_x_value = 0;
    let scale_y_value = 0;
    if (this.move_x_a.sts) {
      move_x_value = this.move_x_a.get_ref_value(this.dyn);
    }

    if (this.move_y_a.sts) {
      move_y_value = this.move_y_a.get_ref_value(this.dyn);
    }

    if (this.scale_x_a.sts) {
      scale_x_value = this.scale_x_a.get_ref_value(this.dyn);
    }

    if (this.scale_y_a.sts) {
      scale_y_value = this.scale_y_a.get_ref_value(this.dyn);
    }

    update = false;
    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.move_x_a.sts && move_x_value !== this.move_x_a.oldValue) {
        update = true;
      } else if (this.move_y_a.sts && move_y_value !== this.move_y_a.oldValue) {
        update = true;
      } else if (this.scale_x_a.sts &&
          scale_x_value !== this.scale_x_a.oldValue) {
        update = true;
      } else if (this.scale_y_a.sts &&
          scale_y_value !== this.scale_y_a.oldValue) {
        update = true;
      }
      if (!update) {
        return;
      }
    }

    let move_x = 0;
    let move_y = 0;
    let scale_x = 1;
    let scale_y = 1;

    if (this.scale_x_a.sts) {
      scale_x = scale_x_value * this.scale_x_factor;
    }

    if (this.scale_y_a.sts) {
      scale_y = scale_y_value * this.scale_y_factor;
    }

    if (!(this.move_x_a.sts || this.move_y_a.sts)) {
      object.set_scale(scale_x, scale_y, 0, 0, this.scale_type);
    }
    if (this.scale_x_a.sts) {
      this.scale_x_a.oldValue = scale_x_value;
    }
    if (this.scale_y_a.sts) {
      this.scale_y_a.oldValue = scale_y_value;
    }

    if (this.move_x_a.sts || this.move_y_a.sts) {
      if (this.move_x_a.sts) {
        let scale_offs = 0;
        // Adjust position for different scaletypes
        switch (this.scale_type) {
          case ScaleType.LowerRight:
          case ScaleType.UpperRight:
            scale_offs = this.width_orig * (1 - scale_x);
            break;
          case ScaleType.Center:
            scale_offs = this.width_orig * (1 - scale_x) / 2;
            break;
          default:
            break;
        }

        move_x = this.x_orig + scale_offs + (move_x_value - this.x_offset) *
            this.x_factor;
      } else {
        move_x = this.x_orig;
      }

      if (this.move_y_a.sts) {
        let scale_offs = 0;
        // Adjust position for different scaletypes
        switch (this.scale_type) {
          case ScaleType.UpperRight:
          case ScaleType.UpperLeft:
            scale_offs = this.height_orig * (1 - scale_y);
            break;
          case ScaleType.Center:
            scale_offs = this.height_orig * (1 - scale_y) / 2;
            break;
          default:
            break;
        }

        move_y = this.y_orig + scale_offs + (move_y_value - this.y_offset) *
            this.y_factor;
      } else {
        move_y = this.y_orig;
      }

      if (Math.abs(scale_x) < Number.MIN_VALUE) {
        scale_x = 10.0e-5;
      }
      if (Math.abs(scale_y) < Number.MIN_VALUE) {
        scale_y = 10.0e-5;
      }

      object.set_scale_pos(move_x, move_y, scale_x, scale_y, 0, 0,
          this.scale_type);
      if (this.move_x_a.sts) {
        this.move_x_a.oldValue = move_x_value;
      }
      if (this.move_y_a.sts) {
        this.move_y_a.oldValue = move_y_value;
      }
    } else {
      if (this.move_x_a.sts) {
        move_x = (move_x_value - this.x_offset) * this.x_factor;
      } else {
        move_x = 0;
      }

      if (this.move_y_a.sts) {
        move_y = (move_y_value - this.y_offset) * this.y_factor;
      } else {
        move_y = 0;
      }

      object.set_position(move_x, move_y);

      if (this.move_x_a.sts) {
        this.move_x_a.oldValue = move_x_value;
      }
      if (this.move_y_a.sts) {
        this.move_y_a.oldValue = move_y_value;
      }
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Move:
          break;
        case DynSave.Move_move_x_attribute:
          if (tokens.length > 1) {
            this.move_x_attribute = tokens[1];
          }
          break;
        case DynSave.Move_move_y_attribute:
          if (tokens.length > 1) {
            this.move_y_attribute = tokens[1];
          }
          break;
        case DynSave.Move_scale_x_attribute:
          if (tokens.length > 1) {
            this.scale_x_attribute = tokens[1];
          }
          break;
        case DynSave.Move_scale_y_attribute:
          if (tokens.length > 1) {
            this.scale_y_attribute = tokens[1];
          }
          break;
        case DynSave.Move_x_offset:
          this.x_offset = parseFloat(tokens[1]);
          break;
        case DynSave.Move_y_offset:
          this.y_offset = parseFloat(tokens[1]);
          break;
        case DynSave.Move_x_factor:
          this.x_factor = parseFloat(tokens[1]);
          break;
        case DynSave.Move_y_factor:
          this.y_factor = parseFloat(tokens[1]);
          break;
        case DynSave.Move_scale_x_factor:
          this.scale_x_factor = parseFloat(tokens[1]);
          break;
        case DynSave.Move_scale_y_factor:
          this.scale_y_factor = parseFloat(tokens[1]);
          break;
        case DynSave.Move_scale_type:
          this.scale_type = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynMove");
          break;
      }
    }

    return i;
  }
}

class DynAnalogShift extends DynElem {
  a;
  firstScan = true;
  oldValueF;
  oldValueI;

  constructor(dyn) {
    super(dyn, DynPrio.AnalogShift);
    this.dyn_type1 = DynType1.AnalogShift;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("AnalogShift: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts) {
      return;
    }

    let index = 0;

    switch (this.a.typeid) {
      case Type.Float32:
        let value = this.a.get_ref_value(this.dyn);

        if (this.firstScan) {
          this.firstScan = false;
        } else {
          if (value === this.oldValueF) {
            return;
          }
        }

        index = Math.floor(value + 0.5);
        this.oldValueF = value;

        break;
      case Type.Int32:
        let value = this.a.get_ref_value(this.dyn);

        if (this.firstScan) {
          this.firstScan = false;
        } else {
          if (value === this.oldValueI) {
            return;
          }
        }

        index = value;
        this.oldValueI = value;

        break;
    }
    object.set_nodeclass_by_index(index);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.AnalogShift:
          break;
        case DynSave.AnalogShift_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynAnalogShift");
          break;
      }
    }

    return i;
  }
}

class DynDigShift extends DynElem {
  a;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.DigShift);
    this.dyn_type1 = DynType1.DigShift;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigShift: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value) {
        return;
      }
    }

    if (value) {
      object.set_last_nodeclass();
    } else {
      object.set_root_nodeclass();
    }
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigShift:
          break;
        case DynSave.DigShift_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigShift");
          break;
      }
    }

    return i;
  }
}

class DynDigFourShift extends DynElem {
  a1;
  a2;
  a3;
  attribute1;
  attribute2;
  attribute3;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.DigFourShift);
    this.dyn_type2 = DynType2.DigFourShift;
  }

  connect(object) {
    this.a1 = new DynReference(this.dyn, this.attribute1);
    this.a1.connect(this.dyn);
    if (!this.a1.sts) {
      console.log("DigFourShift: " + this.attribute1);
      return 1;
    }

    this.a2 = new DynReference(this.dyn, this.attribute2);
    this.a2.connect(this.dyn);
    if (!this.a2.sts) {
      console.log("DigFourShift: " + this.attribute2);
      return 1;
    }

    this.a3 = new DynReference(this.dyn, this.attribute3);
    this.a3.connect(this.dyn);
    if (!this.a3.sts) {
      console.log("DigFourShift: " + this.attribute3);
      return 1;
    }

    return 1;
  }

  disconnect() {
    this.a1.disconnect(this.dyn);
    this.a2.disconnect(this.dyn);
    this.a3.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a1.attrFound || !this.a2.attrFound || !this.a3.attrFound) {
      return;
    }
    let value1 = this.dyn.getDig(this.a1.p, this.a1.typeid, this.a1.bitmask,
        this.a1.database);
    let value2 = this.dyn.getDig(this.a2.p, this.a2.typeid, this.a2.bitmask,
        this.a2.database);
    let value3 = this.dyn.getDig(this.a3.p, this.a3.typeid, this.a3.bitmask,
        this.a3.database);

    value1 = (this.a1.inverted) ? !value1 : value1;
    value2 = (this.a2.inverted) ? !value2 : value2;
    value3 = (this.a3.inverted) ? !value3 : value3;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a1.oldValue === value1 && this.a2.oldValue === value2 &&
          this.a3.oldValue === value3) {
        return;
      }
    }

    if (value3) {
      object.set_nodeclass_by_index(2);
    } else if (value2) {
      object.set_nodeclass_by_index(1);
    } else if (value1) {
      object.set_root_nodeclass();
    } else {
      object.set_last_nodeclass();
    }
    this.a1.oldValue = value1;
    this.a2.oldValue = value2;
    this.a3.oldValue = value3;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigFourShift:
          break;
        case DynSave.DigFourShift_attribute1:
          if (tokens.length > 1) {
            this.attribute1 = tokens[1];
          }
          break;
        case DynSave.DigFourShift_attribute2:
          if (tokens.length > 1) {
            this.attribute2 = tokens[1];
          }
          break;
        case DynSave.DigFourShift_attribute3:
          if (tokens.length > 1) {
            this.attribute3 = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigFourShift");
          break;
      }
    }

    return i;
  }
}

class DynScrollingText extends DynElem {
  a;
  direction;
  speed;
  bounce;
  firstScan = true;
  offset = 0;
  osize;

  constructor(dyn) {
    super(dyn, DynPrio.ScrollingText);
    this.dyn_type2 = DynType2.ScrollingText;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("ScrollingText: " + this.attribute);
      return 1;
    }
    if (!(this.a.typeid === Type.String || this.a.typeid ===
            Type.Text)) {
      console.log("ScrollingText: " + this.attribute);
      this.a.disconnect(this.dyn);
      return 1;
    }

    let geom = object.measure();
    if (this.direction == Direction.Left || this.direction == Direction.Right) {
      this.osize = geom.width();
    } else {
      this.osize = geom.height();
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    let d = object.getAnnotationTextExtent(1);
    if (d === null) {
      return;
    }

    switch (this.direction) {
      case Direction.Left:
        this.offset -= this.speed * this.dyn.graph.getAnimationScanTime();
        if (this.bounce === 0) {
          if (this.offset < -d.width) {
            this.offset = this.osize;
          }
        } else {
          if (d.width < this.osize) {
            if (this.offset < 0) {
              this.offset = -this.offset;
              this.direction = Direction.Right;
            }
          } else {
            if (this.offset < this.osize - d.width) {
              this.offset += (this.osize - d.width) - this.offset;
              this.direction = Direction.Right;
            }
          }
        }
        object.setAnnotationTextOffset(1, this.offset, 0);
        break;
      case Direction.Right:
        this.offset += this.speed * this.dyn.graph.getAnimationScanTime();
        if (this.bounce === 0) {
          if (this.offset > this.osize) {
            this.offset = -d.width;
          }
        } else {
          if (d.width < this.osize) {
            if (this.offset > this.osize - d.width) {
              this.offset -= this.offset - (this.osize - d.width);
              this.direction = Direction.Left;
            }
          } else {
            if (this.offset > 0) {
              this.offset = -this.offset;
              this.direction = Direction.Left;
            }
          }
        }
        object.setAnnotationTextOffset(1, this.offset, 0);
        break;
      case Direction.Up:
        this.offset += this.speed * this.dyn.graph.getAnimationScanTime();
        if (this.bounce === 0) {
          if (this.offset > this.osize) {
            this.offset = -d.height;
          }
        } else {
          if (this.offset > this.osize - d.height) {
            this.offset -= this.offset - (this.osize - d.height);
            this.direction = Direction.Down;
          }
        }
        object.setAnnotationTextOffset(1, 0, this.offset);
        break;
      case Direction.Down:
        this.offset -= this.speed * this.dyn.graph.getAnimationScanTime();
        if (this.bounce === 0) {
          if (this.offset < -d.height) {
            this.offset = this.osize;
          }
        } else {
          if (this.offset < 0) {
            this.offset = -this.offset;
            this.direction = Direction.Up;
          }
        }
        object.setAnnotationTextOffset(1, 0, this.offset);
        break;
      default:
    }

    let value0 = this.a.get_ref_value(this.dyn);

    if (this.firstScan || !(value0 === this.a.oldValue)) {
      object.setAnnotation(1, value0);
      this.dyn.repaintNow = true;
      this.a.oldValue = value0;
    }
    if (this.firstScan) {
      this.firstScan = false;
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.ScrollingText:
          break;
        case DynSave.ScrollingText_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.ScrollingText_direction:
          this.direction = parseInt(tokens[1], 10);
          break;
        case DynSave.ScrollingText_speed:
          this.speed = parseFloat(tokens[1]);
          break;
        case DynSave.ScrollingText_bounce:
          this.bounce = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynScrollingText");
          break;
      }
    }

    return i;
  }
}

class DynColorThemeLightness extends DynElem {
  constructor(dyn) {
    super(dyn, DynPrio.ColorThemeLightness);
    this.dyn_type2 = DynType2.ColorThemeLightness;
  }

  scan(o) {
    o.setColorThemeLightness();
    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.ColorThemeLightness:
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynColorThemeLightness");
          break;
      }
    }

    return i;
  }
}

class DynDigBackgroundColor extends DynElem {
  a = null;
  color;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.DigBackgroundColor);
    this.dyn_type2 = DynType2.DigBackgroundColor;
  }

  connect(object) {
    if (this.color < 0 || this.color >= DrawType._) {
      return 0;
    }

    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigBackgroundColor: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    if (this.a) {
      this.a.disconnect(this.dyn);
    }
  }

  scan(object) {
    if (this.a === null || !this.a.sts || this.dyn.ignoreBgColor) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value && !this.dyn.resetColor) {
        // No change since last time
        if (value) {
          this.dyn.ignoreBgColor = true;
        }
        return;
      }
    }

    if (value) {
      object.setBackgroundColor(this.color);
      this.dyn.ignoreBgColor = true;
    } else {
      object.resetBackgroundColor();
      this.dyn.resetBgColor = true;
    }
    this.dyn.repaintNow = true;
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigBackgroundColor:
          break;
        case DynSave.DigBackgroundColor_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigBackgroundColor_color:
          this.color = parseInt(tokens[1], 10);
          break;
        case DynSave.DigBackgroundColor_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.DigBackgroundColor_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigBackgroundColor");
          break;
      }
    }

    return i;
  }
}

class DynDigSwap extends DynElem {
  a;
  reset_value;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.DigSwap);
    this.dyn_type2 = DynType2.DigSwap;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigSwap: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);

    if (!this.firstScan) {
      this.a.oldValue = value;
      this.firstScan = false;
      return;
    }

    value = (this.a.inverted) ? !value : value;

    if (this.a.oldValue === value) {
      // No change since last time
      return;
    }

    if (value) {
      // Todo
      // this.dyn.graph.swap();
    }
    this.dyn.repaintNow = true;
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigSwap:
          break;
        case DynSave.DigSwap_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigSwap_reset_value:
          this.reset_value = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigSwap");
          break;
      }
    }

    return i;
  }
}

class DynAnimation extends DynElem {
  a;
  sequence;
  firstScan = true;
  animation_count;
  animation_direction;

  constructor(dyn) {
    super(dyn, DynPrio.Animation);
    this.dyn_type1 = DynType1.Animation;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("Animation: " + this.attribute);
      return 1;
    }

    if (this.sequence === AnimSequence.Inherit) {
      this.sequence = object.get_nodeclass_dyn_attr1();
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    let max_count;
    let sts;

    if (!this.a.sts) {
      return;
    }

    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);
    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.animation_count = 0;
      this.animation_direction = 0;
      this.firstScan = false;
      if (this.sequence === AnimSequence.Dig) {
        if (value) {
          object.set_last_nodeclass();
        }
        this.a.oldValue = value;
      }
    }

    if (this.sequence === AnimSequence.Dig) {
      if (this.a.oldValue !== value) {
        // Start animation
        if (value) {
          this.animation_direction = 1;
        } else if (!value) {
          this.animation_direction = 2;
        }
        this.animation_count = 0;
      }

      if (this.animation_direction !== 0) {
        max_count = object.get_animation_count();
        this.animation_count++;
        if (this.animation_count >= max_count) {
          // Shift nodeclass
          if (this.animation_direction === 1) {
            // Shift forward
            sts = object.set_next_nodeclass();
            if ((sts & 1) === 0) {
              // End of animation
              this.animation_direction = 0;
            }
            this.animation_count = 0;
          } else {
            // Shift backward
            sts = object.set_previous_nodeclass();
            if ((sts & 1) === 0) {
              // End of animation
              this.animation_direction = 0;
            }
            this.animation_count = 0;
          }
        }
      }
    } else {
      if (value) {
        if (this.animation_direction === 0) {
          // Animation has been stopped
          this.animation_count = 0;
          this.animation_direction = 1;
        }

        max_count = object.get_animation_count();
        this.animation_count++;
        if (this.animation_count >= max_count) {
          // Shift nodeclass
          if (this.animation_direction === 1) {
            // Shift forward

            sts = object.set_next_nodeclass();
            if ((sts & 1) === 0) {
              if (this.sequence === AnimSequence.Cycle) {
                // Start from the beginning again
                object.set_nodeclass_by_index(1);
              } else {
                // Change direction
                this.animation_direction = 2;
                object.set_previous_nodeclass();
              }
            }
            this.animation_count = 0;
          } else {
            // Shift backward

            sts = object.set_previous_nodeclass();
            if ((sts & 1) === 0) {
              // Change direction
              this.animation_direction = 1;
              sts = object.set_next_nodeclass();
            }
            this.animation_count = 0;
          }
        }
      } else {
        if (this.animation_direction !== 0) {
          // Stop and reset animation
          this.animation_direction = 0;
          object.set_first_nodeclass();
        }
      }
    }
    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Animation:
          break;
        case DynSave.Animation_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.Animation_sequence:
          this.sequence = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynAnimation");
          break;
      }
    }

    return i;
  }
}

class DynVideo extends DynElem {
  constructor(dyn) {
    super(dyn, DynPrio.Video);
    this.dyn_type1 = DynType1.Video;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Video:
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynVideo");
          break;
      }
    }

    return i;
  }
}

class DynBar extends DynElem {
  a;
  minvalue_a;
  maxvalue_a;
  attribute = null;
  minvalue_attr;
  maxvalue_attr;
  maxval = 0;
  minval;
  firstScan = true;
  old_value;
  old_ivalue;

  constructor(dyn) {
    super(dyn, DynPrio.Bar);
    this.dyn_type1 = DynType1.Bar;
  }

  connect(object) {
    if (this.attribute === null) {
      return 0;
    }

    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("Bar: " + this.attribute);
      return 1;
    }

    this.minvalue_a = new DynReference(this.dyn, this.minvalue_attr);
    this.minvalue_a.connect(this.dyn);

    this.maxvalue_a = new DynReference(this.dyn, this.maxvalue_attr);
    this.maxvalue_a.connect(this.dyn);

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
    this.minvalue_a.disconnect(this.dyn);
    this.maxvalue_a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts) {
      return;
    }

    if (this.maxvalue_a.sts && this.minvalue_a.sts) {
      let minval = this.minvalue_a.get_ref_value(this.dyn);
      let maxval = this.maxvalue_a.get_ref_value(this.dyn);
      if (minval !== this.minvalue_a.oldValue ||
          maxval !== this.maxvalue_a.oldValue) {
        object.set_range(minval, maxval);
        this.minvalue_a.oldValue = minval;
        this.maxvalue_a.oldValue = maxval;
      }
    }

    switch (this.a.typeid) {
      case Type.Float32:
        let value = this.a.get_ref_value(this.dyn);

        if (this.firstScan) {
          this.firstScan = false;
        } else {
          if (Math.abs(this.old_value - value) < Number.MIN_VALUE) { // No change since last time
            return;
          }
        }

        object.set_value(value);
        this.old_value = value;
        break;
      case Type.Int32:
      case Type.UInt32:
        let value = this.a.get_ref_value(this.dyn);
        if (this.firstScan) {
          this.firstScan = false;
        } else {
          if (this.old_ivalue === value) { // No change since last time
            return;
          }
        }

        object.set_value(value);
        this.old_ivalue = value;
        break;
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Bar:
          break;
        case DynSave.Bar_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.Bar_minvalue_attr:
          if (tokens.length > 1) {
            this.minvalue_attr = tokens[1];
          }
          break;
        case DynSave.Bar_maxvalue_attr:
          if (tokens.length > 1) {
            this.maxvalue_attr = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynBar", i, key);
          break;
      }
    }

    return i;
  }
}

class DynTrend extends DynElem {
  a1 = null;
  a2 = null;
  minvalue_a1 = null;
  maxvalue_a1 = null;
  minvalue_a2 = null;
  maxvalue_a2 = null;
  hold_a = null;
  timerange_a = null;
  mark1_a = null;
  mark2_a = null;
  attribute1 = null;
  attribute2 = null;
  minvalue_attr1 = null;
  maxvalue_attr1 = null;
  minvalue_attr2 = null;
  maxvalue_attr2 = null;
  hold_attr = null;
  timerange_attr = null;
  mark1_attr = null;
  mark2_attr = null;
  mark1_color;
  mark2_color;
  firstScan = true;
  scan_time;
  acc_time;
  no_of_points;
  trend_hold;
  orig_graph_scan_time;
  orig_graph_fast_scan_time;
  orig_graph_animation_scan_time;

  constructor(dyn) {
    super(dyn, DynPrio.Trend);
    this.dyn_type1 = DynType1.Trend;
  }

  connect(object) {
    this.a1 = new DynReference(this.dyn, this.attribute1);
    this.a1.connect(this.dyn);
    if (!this.a1.sts) {
      console.log("Trend: " + this.attribute1);
      return 1;
    }

    if (this.attribute2.trim() !== "") {
      this.a2 = new DynReference(this.dyn, this.attribute2);
      this.a2.connect(this.dyn);
      if (!this.a2.sts) {
        console.log("Trend: " + this.attribute2);
        return 1;
      }
    }

    this.no_of_points = object.get_no_of_points();
    this.scan_time = object.get_scan_time();
    this.acc_time = this.scan_time;
    this.trend_hold = 0;

    if (this.minvalue_attr1.trim() !== "") {
      this.minvalue_a1 = new DynReference(this.dyn, this.minvalue_attr1);
      this.minvalue_a1.connect(this.dyn);
      if (!this.minvalue_a1.sts) {
        this.minvalue_a1 = null;
        console.log("Trend: " + this.minvalue_attr1);
      }
    }

    if (this.maxvalue_attr1.trim() !== "") {
      this.maxvalue_a1 = new DynReference(this.dyn, this.maxvalue_attr1);
      this.maxvalue_a1.connect(this.dyn);
      if (!this.maxvalue_a1.sts) {
        this.maxvalue_a1 = null;
        console.log("Trend: " + this.maxvalue_attr1);
      }
    }

    if (this.minvalue_attr2.trim() !== "") {
      this.minvalue_a2 = new DynReference(this.dyn, this.minvalue_attr2);
      this.minvalue_a2.connect(this.dyn);
      if (!this.minvalue_a2.sts) {
        this.minvalue_a2 = null;
        console.log("Trend: " + this.minvalue_attr2);
      }
    }

    if (this.maxvalue_attr2.trim() !== "") {
      this.maxvalue_a2 = new DynReference(this.dyn, this.maxvalue_attr2);
      this.maxvalue_a2.connect(this.dyn);
      if (!this.maxvalue_a2.sts) {
        this.maxvalue_a2 = null;
        console.log("Trend: " + this.maxvalue_attr2);
      }
    }

    if (this.hold_attr.trim() !== "") {
      this.hold_a = new DynReference(this.dyn, this.hold_attr);
      this.hold_a.connect(this.dyn);
      if (!this.hold_a.sts) {
        this.hold_a = null;
        console.log("Trend: " + this.hold_attr);
      }
    }

    if (this.timerange_attr.trim() !== "") {
      this.timerange_a = new DynReference(this.dyn, this.timerange_attr);
      this.timerange_a.connect(this.dyn);
      if (!this.timerange_a.sts) {
        this.timerange_a = null;
        console.log("Trend: " + this.timerange_attr);
      }

      this.orig_graph_scan_time = this.dyn.graph.getScanTime();
      this.orig_graph_fast_scan_time = this.dyn.graph.getFastScanTime();
      this.orig_graph_animation_scan_time =
          this.dyn.graph.getAnimationScanTime();
    }

    if (this.mark1_attr && this.mark1_attr.trim() !== "") {
      this.mark1_a = new DynReference(this.dyn, this.mark1_attr);
      this.mark1_a.connect(this.dyn);
      if (!this.mark1_a.sts) {
        this.mark1_a = null;
        console.log("Trend: " + this.mark1_attr);
      }
    }

    if (this.mark2_attr && this.mark2_attr.trim() !== "") {
      this.mark2_a = new DynReference(this.dyn, this.mark2_attr);
      this.mark2_a.connect(this.dyn);
      if (!this.mark2_a.sts) {
        this.mark2_a = null;
        console.log("Trend: " + this.mark2_attr);
      }
    }

    if (this.mark1_color !== DrawType.Inherit ||
        this.mark2_color !== DrawType.Inherit) {
      object.set_mark_color(this.mark1_color, this.mark2_color);
    }

    return 1;
  }

  disconnect() {
    [this.a1, this.a2, this.minvalue_a1, this.maxvalue_a1,
      this.minvalue_a2, this.maxvalue_a2, this.hold_a, this.timerange_a,
      this.mark1_a, this.mark2_a].forEach(function (e) {
      if (e) {
        e.disconnect(this.dyn);
      }
    });
  }

  scan(object) {
    if (!this.a1.attrFound) {
      return;
    }

    if (this.hold_a) {
      let holdval = this.hold_a.get_ref_value(this.dyn);
      if (holdval) {
        return;
      }
    }

    if (this.timerange_a) {
      let timerangeval = this.timerange_a.get_ref_value(this.dyn);
      if (Math.abs(timerangeval - this.timerange_a.oldValue) >
          Number.MIN_VALUE) {
        let dt = timerangeval / this.no_of_points;

        if (dt >= 0.001) {
          object.set_scan_time(dt);
          this.scan_time = dt;
          if (this.dyn.cycle === Cycle.Slow) {
            let current_graph_scan_time = this.dyn.graph.getScanTime();
            if (current_graph_scan_time > this.scan_time || this.scan_time <= this.orig_graph_scan_time) {
              this.dyn.graph.setScanTime(this.scan_time);
            } else {
              this.dyn.graph.setScanTime(this.orig_graph_scan_time);
            }
          }
          let current_graph_fast_scan_time = this.dyn.graph.getFastScanTime();
          let current_graph_animation_scan_time = this.dyn.graph.getAnimationScanTime();
          if (current_graph_fast_scan_time > this.scan_time || this.scan_time <= this.orig_graph_fast_scan_time) {
            this.dyn.graph.setFastScanTime(this.scan_time);
          } else {
            this.dyn.graph.setFastScanTime(this.orig_graph_fast_scan_time);
          }
          if (current_graph_animation_scan_time > this.scan_time || this.scan_time <= this.orig_graph_animation_scan_time) {
            this.dyn.graph.setAnimationScanTime(this.scan_time);
          } else {
            this.dyn.graph.setAnimationScanTime(this.orig_graph_animation_scan_time);
          }
        }
        this.timerange_a.oldValue = timerangeval;
      }
    }

    if (this.maxvalue_a1 && this.minvalue_a1) {
      let minval = this.minvalue_a1.get_ref_value(this.dyn);
      let maxval = this.maxvalue_a1.get_ref_value(this.dyn);
      if (minval !== this.minvalue_a1.oldValue ||
          maxval !== this.maxvalue_a1.oldValue) {
        if (Math.abs(maxval - minval) > Number.MIN_VALUE) {
          object.set_range_y(0, minval, maxval);
        }
        this.minvalue_a1.oldValue = minval;
        this.maxvalue_a1.oldValue = maxval;
      }
    }

    if (this.maxvalue_a2 && this.minvalue_a2) {
      let minval = this.minvalue_a2.get_ref_value(this.dyn);
      let maxval = this.maxvalue_a2.get_ref_value(this.dyn);
      if (minval !== this.minvalue_a2.oldValue ||
          maxval !== this.maxvalue_a2.oldValue) {
        if (Math.abs(maxval - minval) > Number.MIN_VALUE) {
          object.set_range_y(1, minval, maxval);
        }
        this.minvalue_a2.oldValue = minval;
        this.maxvalue_a2.oldValue = maxval;
      }
    }

    if (this.mark1_a) {
      let mark1val = this.mark1_a.get_ref_value(this.dyn);
      if (this.firstScan ||
          Math.abs(mark1val - this.mark1_a.oldValue) > Number.MIN_VALUE) {
        object.set_y_mark1(mark1val);
        this.mark1_a.oldValue = mark1val;
      }
    }
    if (this.mark2_a) {
      let mark2val = this.mark2_a.get_ref_value(this.dyn);
      if (this.firstScan ||
          Math.abs(mark2val - this.mark2_a.oldValue) > Number.MIN_VALUE) {
        object.set_y_mark2(mark2val);
        this.mark2_a.oldValue = mark2val;
      }
    }

    if (this.firstScan) {
      this.firstScan = false;
    }

    if (this.dyn.cycle === Cycle.Slow) {
      this.acc_time += this.dyn.graph.getScanTime();
    } else {
      this.acc_time += this.dyn.graph.getFastScanTime();
    }
    if (this.acc_time + Number.MIN_VALUE >= this.scan_time) {
      if (this.a1 !== 0) {
        switch (this.a1.typeid) {
          case Type.Boolean:
            object.add_value(Boolean(this.a1.get_ref_value(this.dyn)), 0);
            break;
          case Type.Float32:
          case Type.Int32:
          case Type.UInt32:
            object.add_value(this.a1.get_ref_value(this.dyn), 0);
            break;
          default:
        }
      }
      if (this.a2 && this.a2.sts) {
        switch (this.a2.typeid) {
          case Type.Boolean:
            object.add_value(Boolean(this.a2.get_ref_value(this.dyn)), 0);
            break;
          case Type.Float32:
          case Type.Int32:
          case Type.UInt32:
            object.add_value(this.a2.get_ref_value(this.dyn), 0);
            break;
          default:
            break;
        }
      }

      this.acc_time = 0;
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Trend:
          break;
        case DynSave.Trend_attribute1:
          if (tokens.length > 1) {
            this.attribute1 = tokens[1];
          }
          break;
        case DynSave.Trend_attribute2:
          if (tokens.length > 1) {
            this.attribute2 = tokens[1];
          }
          break;
        case DynSave.Trend_minvalue_attr1:
          if (tokens.length > 1) {
            this.minvalue_attr1 = tokens[1];
          }
          break;
        case DynSave.Trend_maxvalue_attr1:
          if (tokens.length > 1) {
            this.maxvalue_attr1 = tokens[1];
          }
          break;
        case DynSave.Trend_minvalue_attr2:
          if (tokens.length > 1) {
            this.minvalue_attr2 = tokens[1];
          }
          break;
        case DynSave.Trend_maxvalue_attr2:
          if (tokens.length > 1) {
            this.maxvalue_attr2 = tokens[1];
          }
          break;
        case DynSave.Trend_hold_attr:
          if (tokens.length > 1) {
            this.hold_attr = tokens[1];
          }
          break;
        case DynSave.Trend_timerange_attr:
          if (tokens.length > 1) {
            this.timerange_attr = tokens[1];
          }
          break;
        case DynSave.Trend_mark1_attr:
          if (tokens.length > 1) {
            this.mark1_attr = tokens[1];
          }
          break;
        case DynSave.Trend_mark2_attr:
          if (tokens.length > 1) {
            this.mark2_attr = tokens[1];
          }
          break;
        case DynSave.Trend_mark1_color:
          this.mark1_color = parseInt(tokens[1], 10);
          break;
        case DynSave.Trend_mark2_color:
          this.mark2_color = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynTrend", row, key);
          break;
      }
    }

    return i;
  }
}

class DynXY_Curve extends DynElem {
  firstScan = true;
  x_a = null;
  y_a = null;
  y_minvalue_a = null;
  y_maxvalue_a = null;
  x_minvalue_a = null;
  x_maxvalue_a = null;
  noofpoints_a = null;
  update_a = null;
  x_mark1_a = null;
  x_mark2_a = null;
  y_mark1_a = null;
  y_mark2_a = null;
  x_attr = null;
  y_attr = null;
  y_minvalue_attr;
  y_maxvalue_attr;
  x_minvalue_attr;
  x_maxvalue_attr;
  noofpoints_attr;
  update_attr;
  x_mark1_attr = null;
  x_mark2_attr = null;
  y_mark1_attr = null;
  y_mark2_attr = null;
  mark1_color;
  mark2_color;
  y_min_value;
  y_max_value;
  x_min_value;
  x_max_value;
  horizontal_padding;
  datatype;
  curve_color;
  fill_color;
  noofpoints;
  noOfPoints;
  xAttrType;
  yAttrType;
  curveX = [];
  curveY = [];
  curve_number;
  object;

  constructor(dyn) {
    super(dyn, DynPrio.XY_Curve);
    this.dyn_type1 = DynType1.XY_Curve;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.XY_Curve:
          break;
        case DynSave.XY_Curve_x_attr:
          if (tokens.length > 1) {
            this.x_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_y_attr:
          if (tokens.length > 1) {
            this.y_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_y_minvalue_attr:
          if (tokens.length > 1) {
            this.y_minvalue_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_y_maxvalue_attr:
          if (tokens.length > 1) {
            this.y_maxvalue_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_x_minvalue_attr:
          if (tokens.length > 1) {
            this.x_minvalue_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_x_maxvalue_attr:
          if (tokens.length > 1) {
            this.x_maxvalue_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_noofpoints_attr:
          if (tokens.length > 1) {
            this.noofpoints_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_update_attr:
          if (tokens.length > 1) {
            this.update_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_y_min_value:
          this.y_min_value = parseFloat(tokens[1]);
          break;
        case DynSave.XY_Curve_y_max_value:
          this.y_max_value = parseFloat(tokens[1]);
          break;
        case DynSave.XY_Curve_x_min_value:
          this.x_min_value = parseFloat(tokens[1]);
          break;
        case DynSave.XY_Curve_x_max_value:
          this.x_max_value = parseFloat(tokens[1]);
          break;
        case DynSave.XY_Curve_datatype:
          this.datatype = parseInt(tokens[1], 10);
          break;
        case DynSave.XY_Curve_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.XY_Curve_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.XY_Curve_curve_color:
          this.curve_color = parseInt(tokens[1], 10);
          break;
        case DynSave.XY_Curve_fill_color:
          this.fill_color = parseInt(tokens[1], 10);
          break;
        case DynSave.XY_Curve_horizontal_padding:
          this.horizontal_padding = parseInt(tokens[1], 10);
          break;
        case DynSave.XY_Curve_x_mark1_attr:
          if (tokens.length > 1) {
            this.x_mark1_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_x_mark2_attr:
          if (tokens.length > 1) {
            this.x_mark2_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_y_mark1_attr:
          if (tokens.length > 1) {
            this.y_mark1_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_y_mark2_attr:
          if (tokens.length > 1) {
            this.y_mark2_attr = tokens[1];
          }
          break;
        case DynSave.XY_Curve_mark1_color:
          this.mark1_color = parseInt(tokens[1], 10);
          break;
        case DynSave.XY_Curve_mark2_color:
          this.mark2_color = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynXYCurve");
          break;
      }
    }

    return i;
  }

  connect(object) {
    if (this.x_attr === null || this.y_attr === null) {
      return 1;
    }

    let createRef = function(field) {
      let attr = this[field + "_attr"];
      field += "_a";
      if (attr.trim() !== "") {
        this[field] = new DynReference(this.dyn, attr);
        this[field].connect(this.dyn);
        if (!this[field].sts) {
          console.log("XYCurve: " + attr);
          this[field] = null;
        }
      }
    };

    createRef("update");
    createRef("noofpoints");
    createRef("y_minvalue");
    createRef("y_maxvalue");
    createRef("x_minvalue");
    createRef("x_maxvalue");
    createRef("y_mark1");
    createRef("y_mark2");

    // Get curve number
    this.curve_number = 0;
    let m = this.instance;
    while (m !== 0) {
      m >>= 1;
      this.curve_number++;
    }

    // Get number of curves
    if (this.instance === Instance.One) {
      m = this.instance_mask;
      let noofcurves = 0;
      while (m !== 0) {
        m >>= 1;
        noofcurves++;
      }
      object.set_xy_noofcurves(noofcurves);
    }

    this.noofpoints = object.get_no_of_points();
    if (Math.abs(this.y_max_value - this.y_min_value) > Number.MIN_VALUE) {
      object.set_xy_range_y(this.curve_number - 1, this.y_min_value,
          this.y_max_value);
    }
    if (Math.abs(this.x_max_value - this.x_min_value) > Number.MIN_VALUE) {
      object.set_xy_range_x(this.curve_number - 1, this.x_min_value,
          this.x_max_value);
    }
    object.set_xy_curve_color(this.curve_number - 1, this.curve_color,
        this.fill_color);
    if (this.mark1_color !== DrawType.Inherit ||
        this.mark2_color !== DrawType.Inherit) {
      object.set_mark_color(this.mark1_color, this.mark2_color);
    }

    return 1;
  }

  disconnect() {
    [this.update_a, this.noofpoints_a, this.y_minvalue_a, this.y_maxvalue_a,
      this.x_minvalue_a, this.x_maxvalue_a, this.x_mark1_a, this.x_mark2_a,
      this.y_mark1_a, this.y_mark2_a].forEach(function (e) {
      if (e) {
        e.disconnect(this.dyn);
      }
    });
  }

  scan(object) {
    let pname;
    let attrSize;
    let update = false;
    let size = 1;
    this.object = object;
    this.noOfPoints = this.noofpoints;

    if (this.x_attr === null || this.y_attr === null) {
      return;
    }

    if (this.firstScan) {
      update = true;
    }

    if (this.update_a) {
      let value = this.update_a.get_ref_value(this.dyn);
      if (value && !this.update_a.oldValue) {
        update = true;
      }
      this.update_a.oldValue = value;
    }

    if (this.noofpoints_a) {
      let value = this.noofpoints_a.get_ref_value(this.dyn);
      if (value !== this.noofpoints_a.oldValue) {
        update = true;
        this.noofpoints = value;
        this.noOfPoints = value;
        this.noofpoints_a.oldValue = value;
      }
    }

    if (this.y_minvalue_a) {
      let value = this.y_minvalue_a.get_ref_value(this.dyn);
      if (value !== this.y_min_value) {
        this.y_min_value = value;
        if (Math.abs(this.y_max_value - this.y_min_value) > Number.MIN_VALUE) {
          object.set_xy_range_y(this.curve_number - 1, this.y_min_value,
              this.y_max_value);
        }
        update = true;
      }
    }
    if (this.y_maxvalue_a) {
      let value = this.y_maxvalue_a.get_ref_value(this.dyn);
      if (value !== this.y_max_value) {
        this.y_max_value = value;
        if (Math.abs(this.y_max_value - this.y_min_value) > Number.MIN_VALUE) {
          object.set_xy_range_y(this.curve_number - 1, this.y_min_value,
              this.y_max_value);
        }
        update = true;
      }
    }

    if (this.x_minvalue_a) {
      let value = this.x_minvalue_a.get_ref_value(this.dyn);
      if (value !== this.x_min_value) {
        this.x_min_value = value;
        if (Math.abs(this.x_max_value - this.x_min_value) > Number.MIN_VALUE) {
          object.set_xy_range_x(this.curve_number - 1, this.x_min_value,
              this.x_max_value);
        }
        update = true;
      }
    }
    if (this.x_maxvalue_a) {
      let value = this.x_maxvalue_a.get_ref_value(this.dyn);
      if (value !== this.x_max_value) {
        this.x_max_value = value;
        if (Math.abs(this.x_max_value - this.x_min_value) > Number.MIN_VALUE) {
          object.set_xy_range_x(this.curve_number - 1, this.x_min_value,
              this.x_max_value);
        }
        update = true;
      }
    }

    if (this.x_mark1_a) {
      let mark1val = this.x_mark1_a.get_ref_value(this.dyn);
      if (this.firstScan ||
          Math.abs(mark1val - this.x_mark1_a.oldValue) > Number.MIN_VALUE) {
        object.set_x_mark1(mark1val);
        this.x_mark1_a.oldValue = mark1val;
      }
    }
    if (this.x_mark2_a) {
      let mark2val = this.x_mark2_a.get_ref_value(this.dyn);
      if (this.firstScan ||
          Math.abs(mark2val - this.x_mark2_a.oldValue) > Number.MIN_VALUE) {
        object.set_x_mark2(mark2val);
        this.x_mark2_a.oldValue = mark2val;
      }
    }
    if (this.y_mark1_a) {
      let mark1val = this.y_mark1_a.get_ref_value(this.dyn);
      if (this.firstScan ||
          Math.abs(mark1val - this.y_mark1_a.oldValue) > Number.MIN_VALUE) {
        object.set_y_mark1(mark1val);
        this.y_mark1_a.oldValue = mark1val;
      }
    }
    if (this.y_mark2_a) {
      let mark2val = this.y_mark2_a.get_ref_value(this.dyn);
      if (this.firstScan ||
          Math.abs(mark2val - this.y_mark2_a.oldValue) > Number.MIN_VALUE) {
        object.set_y_mark2(mark2val);
        this.y_mark2_a.oldValue = mark2val;
      }
    }

    if (update) {
      pname = this.dyn.parseAttrName(this.x_attr);
      attrSize = pname.elements;
      this.xAttrType = pname.type;

      switch (this.datatype) {
        case CurveDataType.XYArrays:
          if (attrSize < this.noOfPoints) {
            this.noOfPoints = attrSize;
          }
          size = this.noOfPoints;
          break;
        case CurveDataType.PointArray:
          if (attrSize / 2 < this.noOfPoints) {
            this.noOfPoints = attrSize / 2;
          }
          size = this.noOfPoints * 2;
          break;
        case CurveDataType.TableObject:
          if ((attrSize - 1) / 2 < this.noOfPoints) {
            this.noOfPoints = (attrSize - 1) / 2;
          }
          size = this.noOfPoints * 2 + 1;
          break;
      }

      // Read x-array
      switch (this.xAttrType) {
        case Type.Float32:
          this.dyn.graph.getGdh()
              .getObjectInfoFloatArray(pname.name, size).then(this.scan2);
          break;
        case Type.Int32:
        case Type.Int16:
        case Type.Int8:
        case Type.UInt32:
        case Type.UInt16:
        case Type.UInt8:
          this.dyn.graph.getGdh()
              .getObjectInfoIntArray(pname.name, size).then(this.scan2);
          break;
        default:
          return;
      }
    }
    this.firstScan = false;
  }

  scan2(res) {
    switch (this.xAttrType) {
      case Type.Float32:
      case Type.Int32:
      case Type.Int16:
      case Type.Int8:
      case Type.UInt32:
      case Type.UInt16:
      case Type.UInt8:
        if (!(res.sts & 1)) {
          return;
        }
        switch (this.datatype) {
          case CurveDataType.XYArrays:
            this.curveX = res.value.slice(0, this.noOfPoints);
            break;
          case CurveDataType.PointArray:
            this.curveX = new Array(this.noOfPoints);
            this.curveY = new Array(this.noOfPoints);
            for (let i = 0; i < this.noOfPoints; i++) {
              this.curveX[i] = res.value[2 * i];
              this.curveY[i] = res.value[2 * i + 1];
            }
            this.dyn.repaintNow = true;
            break;
          case CurveDataType.TableObject:
            this.noOfPoints = Math.min(Math.floor(res.value[0]), this.noofpoints);
            this.curveY = new Array(this.noOfPoints);
            this.curveX = new Array(this.noOfPoints);
            for (let i = 0; i < this.noOfPoints; i++) {
              this.curveX[i] = res.value[2 * i + 1];
              this.curveY[i] = res.value[2 * i + 2];
            }
            this.dyn.repaintNow = true;
            break;
        }
        break;
      default:
        return;
    }

    // Read y-array
    switch (this.datatype) {
      case CurveDataType.XYArrays:
        let pname = this.dyn.parseAttrName(this.y_attr);
        this.noOfPoints = Math.min(this.noOfPoints, pname.elements);
        this.yAttrType = pname.type;
        this.curveY = new Array(this.noOfPoints);

        switch (this.yAttrType) {
          case Type.Float32:
            this.dyn.graph.getGdh()
                .getObjectInfoFloatArray(pname.name, this.noOfPoints).then(this.scan3);
            break;
          case Type.Int32:
          case Type.Int16:
          case Type.Int8:
          case Type.UInt32:
          case Type.UInt16:
          case Type.UInt8:
            this.dyn.graph.getGdh()
                .getObjectInfoIntArray(pname.name, this.noOfPoints).then(this.scan3);
            break;
          default:
            return;
        }
        break;
    }
  }

  scan3(res) {
    if (!(res.sts & 1)) {
      return;
    }

    switch (this.datatype) {
      case CurveDataType.XYArrays:
        switch (this.yAttrType) {
          case Type.Float32:
          case Type.Int32:
          case Type.Int16:
          case Type.Int8:
          case Type.UInt32:
          case Type.UInt16:
          case Type.UInt8:
            this.curveY = res.value.slice(0, this.noOfPoints);
            this.dyn.repaintNow = true;
            break;
          default:
            return;
        }
        break;
    }

    this.object.set_xy_data(this.curveY, this.curveX, this.curve_number - 1,
        this.noOfPoints);
  }
}

class DynPie extends DynElem {
  a: Array<DynReference>;
  attribute = new Array(GrowPie.PIE_MAX_SECTORS);
  sectors;
  min_value;
  max_value;
  fix_range;
  attr_type;
  connected = false;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.Pie);
    this.dyn_type1 = DynType1.Pie;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Pie:
          break;
        case DynSave.Pie_fix_range:
          this.fix_range = parseInt(tokens[1], 10);
          break;
        case DynSave.Pie_attribute1:
          if (tokens.length > 1) {
            this.attribute[0] = tokens[1];
          }
          break;
        case DynSave.Pie_attribute2:
          if (tokens.length > 1) {
            this.attribute[1] = tokens[1];
          }
          break;
        case DynSave.Pie_attribute3:
          if (tokens.length > 1) {
            this.attribute[2] = tokens[1];
          }
          break;
        case DynSave.Pie_attribute4:
          if (tokens.length > 1) {
            this.attribute[3] = tokens[1];
          }
          break;
        case DynSave.Pie_attribute5:
          if (tokens.length > 1) {
            this.attribute[4] = tokens[1];
          }
          break;
        case DynSave.Pie_attribute6:
          if (tokens.length > 1) {
            this.attribute[5] = tokens[1];
          }
          break;
        case DynSave.Pie_attribute7:
          if (tokens.length > 1) {
            this.attribute[6] = tokens[1];
          }
          break;
        case DynSave.Pie_attribute8:
          if (tokens.length > 1) {
            this.attribute[7] = tokens[1];
          }
          break;
        case DynSave.Pie_attribute9:
          if (tokens.length > 1) {
            this.attribute[8] = tokens[1];
          }
          break;
        case DynSave.Pie_attribute10:
          if (tokens.length > 1) {
            this.attribute[9] = tokens[1];
          }
          break;
        case DynSave.Pie_attribute11:
          if (tokens.length > 1) {
            this.attribute[10] = tokens[1];
          }
          break;
        case DynSave.Pie_attribute12:
          if (tokens.length > 1) {
            this.attribute[11] = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynPie");
          break;
      }
    }

    return i;
  }

  connect(object) {
    let info = object.get_conf();
    this.min_value = info.min_val;
    this.max_value = info.max_val;
    this.sectors = info.sector_num;
    if (this.sectors > GrowPie.PIE_MAX_SECTORS) {
      this.sectors = GrowPie.PIE_MAX_SECTORS;
    }

    this.a = new Array(this.sectors);
    for (let i = 0; i < this.sectors; i++) {
      if (this.attribute[i] === null || this.attribute[i].trim() === "") {
        this.a[i] = null;
        continue;
      }

      this.a[i] = new DynReference(this.dyn, this.attribute[i]);
      this.a[i].connect(this.dyn);
      if (!this.a[i].sts) {
        this.a[i] = null;
        console.log("Pie: " + this.attribute[i]);
        return 1;
      }

      if (i === 0) {
        this.attr_type = this.a[0].typeid;
      }
    }
    this.connected = true;
    return 1;
  }

  disconnect() {
    if (!this.connected) {
      return;
    }
    this.a.slice(0, this.sectors).forEach(function (e) {
      if (e) {
        e.disconnect(this.dyn);
      }
    });
  }

  scan(object) {
    if (!this.connected) {
      return;
    }

    switch (this.attr_type) {
      case Type.Float32:
        let val = this.a.slice(0, this.sectors).map(e => e.get_ref_value(this.dyn));

        if (this.firstScan) {
          this.firstScan = false;
        } else {
          let update = 0;
          for (let i = 0; i < this.sectors; i++) {
            if (Math.abs(this.a[i].oldValue - val[i]) > Number.MIN_VALUE) {
              update = 1;
              break;
            }
          }
          if (update === 0) {
            return;
          }
        }

        if (Math.abs(this.max_value - this.min_value) < Number.MIN_VALUE) {
          return;
        }

        let dval = new Array(GrowPie.PIE_MAX_SECTORS);
        if (this.fix_range !== 0 || this.sectors === 1) {
          for (let i = 0; i < this.sectors; i++) {
            dval[i] = val[i] - this.min_value;
          }
        } else {
          let sum = 0;
          for (let i = 0; i < this.sectors; i++) {
            sum += val[i] - this.min_value;
          }
          for (let i = 0; i < this.sectors; i++) {
            if (Math.abs(sum) < Number.MIN_VALUE) {
              dval[i] = 0;
            } else {
              dval[i] = (val[i] - this.min_value) / sum *
                  (this.max_value - this.min_value);
            }
          }
        }

        object.set_values(dval);
        for (let i = 0; i < this.sectors; i++) {
          this.a[i].oldValue = val[i];
        }
        break;
      case Type.Int32:
      default:
        break;
    }
  }
}

class DynBarChart extends DynElem {
  a: Array<DynReference>;
  attribute = new Array(GrowBarChart.BARCHART_MAX_BARSEGMENTS);
  bars;
  barsegments;
  min_value;
  max_value;
  fix_range;
  valueF;
  valueI;
  oldValueF;
  oldValueI;
  attr_type;
  firstScan = true;
  connected = false;

  constructor(dyn) {
    super(dyn, DynPrio.BarChart);
    this.dyn_type1 = DynType1.BarChart;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (key === DynSave.End) {
        return i;
      } else if (key === DynSave.BarChart_fix_range) {
        this.fix_range = parseInt(tokens[1], 10);
      } else if (key >= DynSave.BarChart_attribute1 && key <= DynSave.BarChart_attribute12) {
        if (tokens.length > 1) {
          this.attribute[key - DynSave.BarChart_attribute1] = tokens[1];
        }
      } else {
        console.log("Syntax error in DynBarChart");
      }
    }

    return i;
  }

  connect(object) {
    let info = object.get_conf();
    this.min_value = info.min_value;
    this.max_value = info.max_value;
    this.bars = info.bars;
    this.barsegments = info.barsegments;

    this.a = new Array(this.barsegments);

    for (let i = 0; i < this.barsegments; i++) {
      if (this.attribute[i] === null || this.attribute[i].trim() === "") {
        console.log("BarChart", this.attribute[i]);
        return 1;
      }

      this.a[i] = new DynReference(this.dyn, this.attribute[i]);
      this.a[i].connect(this.dyn);

      if (this.a[i].elements === 0) {
        return 1;
      }

      this.bars = Math.min(this.bars, this.a[i].elements);
      if (i === 0) {
        this.attr_type = this.a[i].typeid;
        switch (this.attr_type) {
          case Type.Float32:
            this.valueF = new Array(this.barsegments);
            break;
          case Type.Int32:
            this.valueI = new Array(this.barsegments);
            break;
          default:
            return 1;
        }
      } else {
        if (this.attr_type !== this.a[i].typeid) {
          console.log("BarChart type", this.attribute[i], this.a[i].typeid);
          return 1;
        }
      }
    }
    this.connected = true;
    return 1;
  }

  disconnect() {
    if (!this.connected) {
      return;
    }
    this.a.slice(0, this.barsegments).forEach(e => e.disconnect(this.dyn));
  }

  scan(object) {
    if (!this.connected) {
      return;
    }
    switch (this.attr_type) {
      case Type.Float32:
        this.valueF = new Array(GrowBarChart.BARCHART_MAX_BARSEGMENTS);
        for (let i = 0; i < this.barsegments; i++) {
          this.valueF[i] = this.a[i].get_ref_value(this.dyn);
          if (this.valueF[i].length < this.bars) {
            console.log("BarChart, value array to short",
                this.valueF[i].length);
            break;
          }
        }

        if (this.firstScan) {
          this.firstScan = false;
        } else {
          let update = 0;
          if (this.oldValueF) {
            for (let i = 0; i < this.barsegments && this.valueF[i]; i++) {
              for (let j = 0; j < this.bars; j++) {
                if (Math.abs(this.oldValueF[i][j] - this.valueF[i][j]) >
                    Number.MIN_VALUE) {
                  update = 1;
                  break;
                }
              }
              if (update === 1) {
                break;
              }
            }
            if (update === 0) {
              return;
            }
          }
        }

        if (Math.abs(this.max_value - this.min_value) < Number.MIN_VALUE) {
          return;
        }

        object.set_values(this.valueF[0], this.valueF[1], this.valueF[2],
            this.valueF[3], this.valueF[4], this.valueF[5], this.valueF[6],
            this.valueF[7], this.valueF[8], this.valueF[9], this.valueF[10],
            this.valueF[11]);
        this.oldValueF = this.valueF;

        break;
      case Type.Int32:
      default:
        break;
    }
  }
}

class DynTable extends DynElem {
  attribute = new Array(12);
  format = new Array(12);
  sel_attribute = new Array(12);
  cFormat;
  firstScan = true;
  oldValueB;
  oldValueF;
  oldValueI;
  oldValueS;
  p;
  hp = null;
  db;
  elements;
  type_id;
  size;
  headerRef;
  columns;
  rows;
  sel_p;
  sel_elements;
  bitmask;

  constructor(dyn) {
    super(dyn, DynPrio.Table);
    this.dyn_type1 = DynType1.Table;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (key === DynSave.End) {
        return i;
      } else if (key >= DynSave.Table_attribute1 && key <= DynSave.Table_attribute12) {
        if (tokens.length > 1) {
          this.attribute[key - DynSave.Table_attribute1] = tokens[1];
        }
      } else if (key >= DynSave.Table_format1 && key <= DynSave.Table_format12) {
        if (tokens.length > 1) {
          this.format[key - DynSave.Table_format1] = tokens[1];
        }
      } else if (key >= DynSave.Table_sel_attribute1 && key <= DynSave.Table_sel_attribute12) {
        if (tokens.length > 1) {
          this.sel_attribute[key - DynSave.Table_sel_attribute1] = tokens[1];
        }
      } else {
        console.log("Syntax error in DynTable");
      }
    }

    return i;
  }

  connect(object) {
    let sts;
    let aname0 = null;

    let info = object.getTableInfo();
    this.columns = info.columns;
    this.rows = info.rows;

    this.db = new Array(this.columns);
    this.size = new Array(this.columns);
    this.elements = new Array(this.columns);
    this.type_id = new Array(this.columns);
    this.headerRef = new Array(this.columns);
    this.p = new Array(this.columns);
    for (let i = 0; i < this.columns; i++) {
      this.p[i] = 0;
    }
    this.oldValueB = new Array(this.columns);
    this.oldValueF = new Array(this.columns);
    this.oldValueI = new Array(this.columns);
    this.oldValueS = new Array(this.columns);
    this.cFormat = new Array(this.columns);
    for (let i = 0; i < this.columns; i++) {
      this.cFormat[i] = new GlowCFormat(this.format[i]);
    }
    this.sel_p = new Array(this.columns);
    this.sel_elements = new Array(this.columns);
    this.bitmask = new Array(this.columns);

    for (let i = 0; i < this.columns; i++) {
      let pname = this.dyn.parseAttrName(this.attribute[i]);
      if (pname === null || pname.name === "") {
        continue;
      }

      if (pname.elements === 0) {
        continue;
      }

      this.db[i] = pname.database;

      if (pname.tname.substring(0, 7) === "$header") {
        this.headerRef[i] = true;

        if (aname0 === null) {
          continue;
        }

        for (let j = 0; j < this.elements[0]; j++) {
          let hname = aname0 + "[" + j + "]";
          let aname = "&(" + hname + ")" + pname.tname.substring(7);

          if (this.hp === null) {
            this.hp = new Array(this.columns);
            for (let n = 0; n < this.columns; n++) {
              this.hp[n] = null;
            }
          }
          if (this.hp[i] === null) {
            this.hp[i] = new Array(this.elements[0]);
            for (let k = 0; k < this.elements[0]; k++) {
              this.hp[i][k] = 0;
            }
          }

          this.hp[i][j] = this.dyn.graph.getGdh()
              .refObjectInfo(aname, pname.type, pname.elements);
          console.log("Table $header " + i + " hp " + this.hp[i][j]);
          if (j === 0) {
            this.size[i] = pname.elements;
            this.type_id[i] = pname.type;
            this.elements[i] = this.elements[0];
            this.bitmask[i] = pname.bitmask;
          }
        }
      } else {
        switch (this.db[i]) {
          case Database.Gdh:
            this.p[i] = this.dyn.graph.getGdh()
                .refObjectInfo(pname.tname, pname.type, pname.elements);
            if (i === 0) {
              aname0 = pname.name;
            }
            break;
          default:
        }
        if (this.p[i] === 0) {
          continue;
        }

        this.size[i] = pname.elements;
        this.type_id[i] = pname.type;
        this.elements[i] = Math.min(pname.elements, this.rows);
        this.bitmask[i] = pname.bitmask;
      }

      switch (this.type_id[i]) {
        case Type.Float32:
          this.oldValueF[i] = new Array(this.rows);
          break;
        case Type.Boolean:
        case Type.Bit:
          this.oldValueB[i] = new Array(this.rows);
          break;
        case Type.Int32:
        case Type.Int16:
        case Type.Int8:
        case Type.UInt32:
        case Type.UInt16:
        case Type.UInt8:
          this.oldValueI[i] = new Array(this.rows);
          break;
        case Type.String:
        case Type.Objid:
        case Type.AttrRef:
        case Type.Time:
        case Type.DeltaTime:
          this.oldValueS[i] = new Array(this.rows);
          break;
      }

      // Connect select array
      this.sel_p[i] = 0;
      pname = this.dyn.parseAttrName(this.sel_attribute[i]);
      if (pname === null || pname.name === "" || pname.type !== Type.Boolean) {
        continue;
      }

      switch (pname.database) {
        case Database.Gdh:
          this.sel_p[i] = this.dyn.graph.getGdh()
              .refObjectInfo(pname.tname, pname.type, pname.elements);
          if (this.sel_p[i] !== 0) {
            if (pname.elements === 0) {
              break;
            }
            this.sel_elements[i] = Math.min(pname.elements, this.elements[i]);
          }
          break;
        default:
      }
    }
    object.setTableInfo(info);

    return 1;
  }

  disconnect() {
    for (let i = 0; i < this.columns; i++) {
      if (this.headerRef[i]) {
        for (let j = 0; j < this.elements[i]; j++) {
          if (this.hp[i][j] !== 0) {
            this.dyn.graph.getGdh()
                .unrefObjectInfo(this.hp[i][j]);
          }
        }
      } else {
        if (this.p[i] !== 0 && this.db[i] === Database.Gdh) {
          this.dyn.graph.getGdh()
              .unrefObjectInfo(this.p[i]);
        }
        this.p[i] = 0;
      }
      switch (this.type_id[i]) {
        case Type.Float32:
          if (this.oldValueF[i]) {
            this.oldValueF[i] = null;
          }
          break;
        case Type.Boolean:
        case Type.Bit:
          if (this.oldValueB[i]) {
            this.oldValueB[i] = null;
          }
          break;
        case Type.Int32:
        case Type.Int16:
        case Type.Int8:
        case Type.UInt32:
        case Type.UInt16:
        case Type.UInt8:
          if (this.oldValueI[i]) {
            this.oldValueI[i] = null;
          }
          break;
        case Type.String:
        case Type.Objid:
        case Type.AttrRef:
        case Type.Time:
        case Type.DeltaTime:
          if (this.oldValueS[i]) {
            this.oldValueS[i] = null;
          }
          break;
      }
      if (this.sel_p[i] !== 0) {
        this.dyn.graph.getGdh().unrefObjectInfo(this.sel_p[i]);
        this.sel_p[i] = 0;
      }
    }
  }

  scan(object) {
    if (this.p[0] === 0) {
      return;
    }

    for (let i = 0; i < this.columns; i++) {
      if (this.headerRef[i]) {
        // $header reference
        for (let j = 0; j < this.elements[i]; j++) {
          if (this.hp[i][j] === 0) {
            continue;
          }

          switch (this.type_id[i]) {
            case Type.Float32:
              let val = this.dyn.graph.getGdh().getObjectRefInfo(this.hp[i][j]);
              if (val !== undefined && (this.oldValueF[i][j] !== val || this.firstScan)) {
                let sb = this.cFormat[i].format(val);
                object.setValue(sb, i, j);
                this.oldValueF[i][j] = val;
              }
              break;
            case Type.Boolean:
              let val = this.dyn.graph.getGdh().getObjectRefInfo(this.hp[i][j]);
              if (val !== undefined && (this.firstScan || this.oldValueB[i][j] !== val)) {
                if (val) {
                  object.setValue("1", i, j);
                } else {
                  object.setValue("0", i, j);
                }
                this.oldValueB[i][j] = val;
              }
              break;
            case Type.Int32:
            case Type.Int16:
            case Type.Int8:
            case Type.UInt32:
            case Type.UInt16:
            case Type.UInt8:
              let val = this.dyn.graph.getGdh().getObjectRefInfo(this.hp[i][j]);
              if (val !== undefined && (this.oldValueI[i][j] !== val || this.firstScan)) {
                let sb = this.cFormat[i].format(val);
                object.setValue(sb, i, j);
                this.oldValueI[i][j] = val;
              }
              break;
            case Type.String:
            case Type.Objid:
            case Type.AttrRef:
            case Type.Time:
            case Type.DeltaTime:
              let val = this.dyn.graph.getGdh().getObjectRefInfo(this.hp[i][j]);
              if (val !== undefined && (this.firstScan || this.oldValueS[i][j] !== val)) {
                object.setValue(val, i, j);
                this.oldValueS[i][j] = val;
              }
              break;
            case Type.Bit:
              let val = this.dyn.graph.getGdh().getObjectRefInfo(this.hp[i][j]);
              if (val !== undefined) {
                let bitval = ((this.bitmask[i] & val) !== 0);
                if (this.oldValueB[i][j] !== bitval || this.firstScan) {
                  if (bitval) {
                    object.setValue("1", i, j);
                  } else {
                    object.setValue("0", i, j);
                  }
                  this.oldValueB[i][j] = bitval;
                }
              }
              break;
          }
        }
      } else {
        if (this.p[i] === 0) {
          continue;
        }

        switch (this.type_id[i]) {
          case Type.Float32:
            let val = this.dyn.graph.getGdh().getObjectRefInfo(this.p[i]);
            if (val === null) {
              break;
            }
            for (let j = 0; j < Math.min(this.elements[i], val.length); j++) {
              if (this.oldValueF[i][j] !== val[j] || this.firstScan) {
                let sb = this.cFormat[i].format(val[j]);
                object.setValue(sb, i, j);
                this.oldValueF[i][j] = val[j];
              }
            }
            break;
          case Type.Boolean:
            let val = this.dyn.graph.getGdh().getObjectRefInfo(this.p[i]);
            if (val === null) {
              break;
            }
            for (let j = 0; j < Math.min(this.elements[i], val.length); j++) {
              if (this.firstScan || this.oldValueB[i][j] !== val[j]) {
                if (val[j]) {
                  object.setValue("1", i, j);
                } else {
                  object.setValue("0", i, j);
                }
                this.oldValueB[i][j] = val[j];
              }
            }
            break;
          case Type.Int32:
          case Type.Int16:
          case Type.Int8:
          case Type.UInt32:
          case Type.UInt16:
          case Type.UInt8:
            let val = this.dyn.graph.getGdh().getObjectRefInfo(this.p[i]);
            if (val === null) {
              break;
            }
            for (let j = 0; j < Math.min(this.elements[i], val.length); j++) {
              if (this.oldValueI[i][j] !== val[j] || this.firstScan) {
                let sb = this.cFormat[i].format(val[j]);
                object.setValue(sb, i, j);
                this.oldValueI[i][j] = val[j];
              }
            }
            break;
          case Type.String:
          case Type.Objid:
          case Type.AttrRef:
          case Type.Time:
          case Type.DeltaTime:
            let val = this.dyn.graph.getGdh().getObjectRefInfo(this.p[i]);
            if (val === null) {
              break;
            }
            for (let j = 0; j < Math.min(this.elements[i], val.length); j++) {
              if (this.firstScan || this.oldValueS[i][j] !== val[j]) {
                switch (this.type_id[i]) {
                  case Type.AttrRef:
                  case Type.Objid:
                    if (this.format[i] === "%1o") {
                      object.setValue(val[j], i, j);
                    } else {
                      let idx = val[j].lastIndexOf('-');
                      if (idx === -1) {
                        object.setValue(val[j], i, j);
                      } else {
                        object.setValue(val[j].substring(idx + 1), i, j);
                      }
                    }

                    break;
                  default:
                    object.setValue(val[j], i, j);
                }
                this.oldValueS[i][j] = val[j];
              }
            }
            break;
          case Type.Bit:
            let val = this.dyn.graph.getGdh().getObjectRefInfo(this.p[i]);
            if (val === null) {
              break;
            }
            for (let j = 0; j < Math.min(this.elements[i], val.length); j++) {
              let bitval = ((this.bitmask[i] & val[j]) !== 0);
              if (this.oldValueB[i][j] !== bitval || this.firstScan) {
                if (bitval) {
                  object.setValue("1", i, j);
                } else {
                  object.setValue("0", i, j);
                }
                this.oldValueB[i][j] = bitval;
              }
            }
            break;
        }
      }
    }

    // Examine select array
    let sel_found = false;
    for (let i = 0; i < this.columns; i++) {
      if (this.sel_p[i] === 0) {
        continue;
      }
      let val = this.dyn.graph.getGdh().getObjectRefInfo(this.sel_p[i]);
      if (val === null) {
        continue;
      }
      for (let j = 0; j < this.sel_elements[i]; j++) {
        if (val[j]) {
          sel_found = true;
          object.setSelectedCell(i, j);
        }
      }
    }
    if (!sel_found) {
      object.setSelectedCell(-1, -1);
    }

    if (this.firstScan) {
      this.firstScan = false;
    }
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Click:
        if (e.type !== EventType.Table) {
          break;
        }

        let event = e;
        let row = object.getSelectedCellRow();
        let column = object.getSelectedCellColumn();
        if (row >= 0 && this.sel_p[column] !== 0) {
          // Reset previously selected
          let pname = this.dyn.parseAttrName(this.sel_attribute[column]);
          if (pname === null || pname.name === "") {
            break;
          }
          let aname = pname.name + "[" + row + "]";
          this.dyn.graph.getGdh().setObjectInfoBoolean(aname, false);
        }
        if (this.sel_p[event.column] !== 0 &&
            !(event.column === column && event.row === row)) {
          // Set new selected, if not same as previous selected
          let pname = this.dyn.parseAttrName(this.sel_attribute[event.column]);
          if (pname === null || pname.name === "") {
            break;
          }
          let aname = pname.name + "[" + event.row + "]";
          this.dyn.graph.getGdh().setObjectInfoBoolean(aname, true);
          object.setSelectedCell(event.column, event.row);
        }
        break;
    }

    return 1;
  }
}

class DynStatusColor extends DynElem {
  a;
  nostatus_color;
  use_colortheme;
  firstScan = true;
  oldStatus;
  on;

  constructor(dyn) {
    super(dyn, DynPrio.StatusColor);
    this.dyn_type1 = DynType1.StatusColor;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("StatusColor: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts || this.dyn.ignoreColor) {
      return;
    }

    if (this.a.database === Database.Gdh &&
        this.a.typeid === Type.NetStatus) {
      // TODO get subscription oldness
    }

    let value = this.a.get_ref_value(this.dyn);

    if (!this.firstScan && this.oldStatus !== PwrStatus.Fatal) {
      if (this.a.oldValue === value && this.dyn.resetColor) { // No change since last time
        return;
      }
    }

    this.a.oldValue = value;
    if (value !== 0 && value & 7 < 5) {
        value = [PwrStatus.Warning, PwrStatus.Success, PwrStatus.Error,
        PwrStatus.Success, PwrStatus.Fatal][value & 7];
    } else {
      value = PwrStatus.No;
    }
    if (!this.firstScan && this.oldStatus === value &&
        this.oldStatus !== PwrStatus.Fatal) {
      return;
    } else {
      this.firstScan = false;
    }

    if ((this.dyn.total_dyn_type1 & DynType1.Tone) === 0) {
      switch (value) {
        case PwrStatus.No:
          object.setFillColor(this.nostatus_color);
          break;
        case PwrStatus.Success:
          object.resetFillColor();
          break;
        case PwrStatus.Warning:
          object.setFillColor(DrawType.ColorYellow);
          break;
        case PwrStatus.Error:
          object.setFillColor(DrawType.ColorRed);
          break;
        case PwrStatus.Fatal:
          this.on = !this.on;
          if (this.on) {
            object.setFillColor(DrawType.ColorRed);
          } else {
            object.setFillColor(this.nostatus_color);
          }
          break;
      }
      this.dyn.repaintNow = true;
    } else {
      switch (value) {
        case PwrStatus.No:
          if (this.nostatus_color >= DrawTone._) {
            object.setFillColor(this.nostatus_color);
          } else {
            object.setColorTone(this.nostatus_color);
          }
          break;
        case PwrStatus.Success:
          if (this.nostatus_color >= DrawTone._) {
            object.resetFillColor();
          }
          object.resetColorTone();
          break;
        case PwrStatus.Warning:
          object.setColorTone(
              (this.use_colortheme === 0) ? DrawTone.Yellow :
                  DrawType.CustomColor42);
          break;
        case PwrStatus.Error:
          object.setColorTone(
              (this.use_colortheme === 0) ? DrawTone.Red :
                  DrawType.CustomColor11);
          break;
        case PwrStatus.Fatal:
          this.on = !this.on;
          if (this.on) {
            object.setColorTone(
                (this.use_colortheme === 0) ? DrawTone.Red :
                    DrawType.CustomColor11);
          } else {
            if (this.nostatus_color >= DrawTone._) {
              object.setFillColor(this.nostatus_color);
            } else {
              object.setColorTone(this.nostatus_color);
            }
          }
          break;
      }
      this.dyn.repaintNow = true;
    }
    this.oldStatus = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.StatusColor:
          break;
        case DynSave.StatusColor_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.StatusColor_nostatus_color:
          this.nostatus_color = parseInt(tokens[1], 10);
          break;
        case DynSave.StatusColor_use_colortheme:
          this.use_colortheme = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynStatusColor");
          break;
      }
    }

    return i;
  }
}

class DynAxis extends DynElem {
  minvalue_a;
  maxvalue_a;
  minvalue_attr;
  maxvalue_attr;
  attr_type;
  firstScan = true;
  keep_settings = 0;

  constructor(dyn) {
    super(dyn, DynPrio.Axis);
    this.dyn_type2 = DynType2.Axis;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Axis:
          break;
        case DynSave.Axis_minvalue_attr:
          if (tokens.length > 1) {
            this.minvalue_attr = tokens[1];
          }
          break;
        case DynSave.Axis_maxvalue_attr:
          if (tokens.length > 1) {
            this.maxvalue_attr = tokens[1];
          }
          break;
        case DynSave.Axis_keep_settings:
          this.keep_settings = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynAxis");
          break;
      }
    }

    return i;
  }

  connect(o) {
    this.minvalue_a = new DynReference(this.dyn, this.minvalue_attr);
    this.minvalue_a.connect(this.dyn);
    if (!this.minvalue_a.sts) {
      console.log("Axis: " + this.minvalue_attr);
    }

    this.maxvalue_a = new DynReference(this.dyn, this.maxvalue_attr);
    this.maxvalue_a.connect(this.dyn);
    if (!this.maxvalue_a.sts) {
      console.log("Axis: " + this.maxvalue_attr);
    }

    if (this.minvalue_a.sts && this.maxvalue_a.sts) {
      if (this.minvalue_a.typeid === this.maxvalue_a.typeid) {
        this.attr_type = this.maxvalue_a.typeid;
      } else {
        this.attr_type = 0;
      }
    } else if (this.maxvalue_a.sts) {
      this.attr_type = this.maxvalue_a.typeid;
    } else if (this.minvalue_a.sts) {
      this.attr_type = this.minvalue_a.typeid;
    }

    return 1;
  }

  disconnect() {
    this.minvalue_a.disconnect(this.dyn);
    this.maxvalue_a.disconnect(this.dyn);
  }

  scan(o) {
    switch (this.attr_type) {
      case Type.Float32:
        if (!this.maxvalue_a.sts && !this.minvalue_a.sts) {
          return;
        }

        let min_value = 0;
        let max_value = 0;

        if (this.minvalue_a.sts) {
          min_value = this.minvalue_a.get_ref_value(this.dyn);
        }
        if (this.maxvalue_a.sts) {
          max_value = this.maxvalue_a.get_ref_value(this.dyn);
        }

        if (!(this.firstScan ||
                (this.maxvalue_a.sts && (max_value !== this.maxvalue_a.oldValue)) ||
                (this.minvalue_a.sts && (min_value !== this.minvalue_a.oldValue)))) {
          return;
        }

        if (this.firstScan) {
          this.firstScan = true;
        }
        if (this.minvalue_a.sts) {
          this.minvalue_a.oldValue = min_value;
        }
        if (this.maxvalue_a.sts) {
          this.maxvalue_a.oldValue = max_value;
        }

        if (max_value === min_value) {
          return;
        }

        if (o instanceof GrowAxis || o instanceof GrowAxisArc) {
          o.set_range(min_value, max_value, this.keep_settings);
        }
        break;
      default:
    }
  }
}

class DynTimeoutColor extends DynElem {
  time;
  color;

  constructor(dyn) {
    super(dyn, DynPrio.TimeoutColor);
    this.dyn_type2 = DynType2.TimeoutColor;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.TimeoutColor:
          break;
        case DynSave.TimeoutColor_time:
          this.time = Number(tokens[1]);
          break;
        case DynSave.TimeoutColor_color:
          this.color = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynTimeoutColor");
          break;
      }
    }

    return i;
  }
}

class DynHostObject extends DynElem {
  hostobject;

  constructor(dyn) {
    super(dyn, DynPrio.HostObject);
    this.dyn_type1 = DynType1.HostObject;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.HostObject:
          break;
        case DynSave.HostObject_object:
          if (tokens.length > 1) {
            this.hostobject = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynHostObject");
          break;
      }
    }

    return i;
  }
}

class DynDigSound extends DynElem {
  soundobject;
  level;
  interval;

  constructor(dyn) {
    super(dyn, DynPrio.DigSound);
    this.dyn_type1 = DynType1.DigSound;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigSound:
          break;
        case DynSave.DigSound_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigSound_soundobject:
          if (tokens.length > 1) {
            this.soundobject = tokens[1];
          }
          break;
        case DynSave.DigSound_level:
          this.level = parseInt(tokens[1], 10);
          break;
        case DynSave.DigSound_interval:
          this.interval = parseInt(tokens[1], 10);
          break;
        case DynSave.DigSound_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.DigSound_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigSound");
          break;
      }
    }

    return i;
  }
}

class DynFillLevel extends DynElem {
  a = null;
  minvalue_a;
  maxvalue_a;
  color;
  direction;
  max_value;
  min_value;
  minvalue_attr;
  maxvalue_attr;
  firstScan = true;
  limits_found = false;
  limit_min;
  limit_max;

  constructor(dyn) {
    super(dyn, DynPrio.FillLevel);
    this.dyn_type1 = DynType1.FillLevel;
  }

  connect(object) {
    this.color = Dyn.getColor2(object, this.color);

    if (this.color < 0 || this.color >= DrawType._) {
      return 0;
    }

    if (this.attribute.trim() === null) {
      return 0;
    }

    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("FillLevel: " + this.attribute);
      return 1;
    }

    if ((this.dyn.total_dyn_type1 & DynType1.Tone) === 0) {
      object.setLevelFillColor(this.color);
    } else {
      if (this.color >= DrawTone._) {
        object.setLevelFillColor(this.color);
      } else {
        object.setLevelColorTone(this.color);
      }
    }

    let limits = object.getLimits();
    if ((limits.status & 1) !== 0) {
      this.limits_found = true;
      this.limit_min = limits.min;
      this.limit_max = limits.max;
      this.direction = limits.direction;
    }
    object.setLevelDirection(this.direction);

    this.minvalue_a = new DynReference(this.dyn, this.minvalue_attr);
    this.minvalue_a.connect(this.dyn);

    this.maxvalue_a = new DynReference(this.dyn, this.maxvalue_attr);
    this.maxvalue_a.connect(this.dyn);

    return 1;
  }

  disconnect() {
    if (this.a === null) {
      return;
    }
    this.a.disconnect(this.dyn);
    this.minvalue_a.disconnect(this.dyn);
    this.maxvalue_a.disconnect(this.dyn);
  }

  scan(object) {
    if (this.a === null || !this.a.sts) {
      return;
    }

    let minval, maxval;
    let pvalue = this.a.get_ref_value(this.dyn);
    if (this.maxvalue_a.sts && this.minvalue_a.sts) {
      minval = this.minvalue_a.get_ref_value(this.dyn);
      maxval = this.maxvalue_a.get_ref_value(this.dyn);
      if (minval !== this.min_value || maxval !== this.max_value) {
        this.min_value = minval;
        this.max_value = maxval;
        this.firstScan = true;
      }
    }

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (Math.abs(this.a.oldValue - pvalue) < Number.MIN_VALUE) { // No change since last time
        return;
      }
    }

    if (this.max_value === this.min_value) {
      return;
    }

    let value = (pvalue - this.min_value) / (this.max_value - this.min_value);
    if (this.limits_found) {
      let geom = object.measure();

      value *= (this.limit_max - this.limit_min);

      switch (this.direction) {
        case Direction.Right:
          value = (value + (this.limit_min - geom.ll_x)) / geom.width();
          break;
        case Direction.Left:
          value = (value + (geom.ur_x - this.limit_max)) / geom.width();
          break;
        case Direction.Up:
          value = (value + (this.limit_min - geom.ll_y)) / geom.height();
          break;
        case Direction.Down:
          value = (value + (geom.ur_y - this.limit_max)) / geom.height();
          break;
        default:
          value = 0;
      }
    }
    object.setFillLevel(value);
    this.a.oldValue = pvalue;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.FillLevel:
          break;
        case DynSave.FillLevel_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.FillLevel_color:
          this.color = parseInt(tokens[1], 10);
          break;
        case DynSave.FillLevel_direction:
          this.direction = parseInt(tokens[1], 10);
          break;
        case DynSave.FillLevel_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case DynSave.FillLevel_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case DynSave.FillLevel_minvalue_attr:
          if (tokens.length > 1) {
            this.minvalue_attr = tokens[1];
          }
          break;
        case DynSave.FillLevel_maxvalue_attr:
          if (tokens.length > 1) {
            this.maxvalue_attr = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynFillLevel");
          break;
      }
    }

    return i;
  }
}

class DynDigCommand extends DynElem {
  command = "";
  a = null;
  first_scan = true;
  level = 0;

  constructor(dyn) {
    super(dyn, DynPrio.DigCommand);
    this.dyn_type1 = DynType1.DigCommand;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("DigCommand: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    if (this.a) {
      this.a.disconnect();
    }
  }

  scan(object) {
    if (this.a === null || !this.a.sts) {
      return;
    }

    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask, this.a.database);
    value = (this.a.inverted) ? !value : value;

    if (this.first_scan) {
      this.a.oldValue = value;
      this.first_scan = false;
      return;
    }

    if ((!this.level && value && !this.a.oldValue) || (this.level && value)) {
      let cmd = this.dyn.graph.getCommand(this.command);
      this.dyn.graph.command(cmd);
      return;
    }

    this.a.oldValue = value;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.DigCommand:
          break;
        case DynSave.DigCommand_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.DigCommand_command:
          if (tokens.length > 1) {
            this.command = tokens[1];
          }
          break;
        case DynSave.DigCommand_level:
          this.level = parseInt(tokens[1], 10);
          break;
        case DynSave.DigCommand_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.DigCommand_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynDigCommand");
          break;
      }
    }

    return i;
  }
}

class DynSetDig extends DynElem {
  constructor(dyn) {
    super(dyn, DynPrio.SetDig);
    this.action_type1 = ActionType1.SetDig;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(event === Event.MB1Down);
        this.dyn.repaintNow = true;
        break;
      case Event.MB1Click:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        let pname = this.dyn.parseAttrName(this.attribute);
        if (pname === null) {
          return 1;
        }
        let sts = null;
        switch (pname.database) {
          case Database.Gdh:
            sts =
                this.dyn.graph.getGdh().setObjectInfoBoolean(pname.name, true);
            break;
          case Database.Local:
            sts = this.dyn.graph.getLdb()
                .setObjectInfo(this.dyn.graph, pname.name, true);
            break;
          default:
            break;
        }
        if (even(sts)) {
          console.log("SetDig: " + pname.name);
        }
        break;
      default:
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.SetDig:
          break;
        case DynSave.SetDig_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.SetDig_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.SetDig_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynSetDig");
          break;
      }
    }

    return i;
  }
}

class DynResetDig extends DynElem {
  constructor(dyn) {
    super(dyn, DynPrio.ResetDig);
    this.action_type1 = ActionType1.ResetDig;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(event === Event.MB1Down);
        this.dyn.repaintNow = true;
        break;
      case Event.MB1Click:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        let pname = this.dyn.parseAttrName(this.attribute);
        if (pname === null) {
          return 1;
        }
        let sts = null;
        switch (pname.database) {
          case Database.Gdh:
            sts =
                this.dyn.graph.getGdh().setObjectInfoBoolean(pname.name, false);
            break;
          case Database.Local:
            sts = this.dyn.graph.getLdb()
                .setObjectInfo(this.dyn.graph, pname.name, false);
            break;
          default:
            break;
        }
        if (even(sts)) {
          console.log("ResetDig: " + pname.name);
        }
        break;
      default:
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.ResetDig:
          break;
        case DynSave.ResetDig_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.ResetDig_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.ResetDig_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynResetDig");
          break;
      }
    }

    return i;
  }
}

class DynToggleDig extends DynElem {
  constructor(dyn) {
    super(dyn, DynPrio.ToggleDig);
    this.action_type1 = ActionType1.ToggleDig;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(e.event === Event.MB1Down);
        this.dyn.repaintNow = true;
        this.dyn.graph.setClickActive(e.event === Event.MB1Down);
        break;
      case Event.MB1Click:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        let pname = this.dyn.parseAttrName(this.attribute);
        if (pname === null) {
          return 1;
        }
        let sts = null;
        switch (pname.database) {
          case Database.Gdh:
            sts = this.dyn.graph.getGdh().toggleObjectInfo(pname.name);
            break;
          case Database.Local:
            sts = this.dyn.graph.getLdb()
                .toggleObjectInfo(this.dyn.graph, pname.name);
            break;
        }
        if (even(sts)) {
          console.log("ToggleDig: " + pname.name);
        }
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.ToggleDig:
          break;
        case DynSave.ToggleDig_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynToggleDig");
          break;
      }
    }

    return i;
  }
}

class DynStoDig extends DynElem {
  constructor(dyn) {
    super(dyn, DynPrio.StoDig);
    this.action_type1 = ActionType1.StoDig;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(e.event === Event.MB1Down);
        this.dyn.repaintNow = true;

        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        let pname = this.dyn.parseAttrName(this.attribute);
        let sts = null;
        switch (pname.database) {
          case Database.Gdh:
            sts =
                this.dyn.graph.getGdh().setObjectInfoBoolean(pname.name, e.event === Event.MB1Down);
            break;
          case Database.Local:
            sts = this.dyn.graph.getLdb()
                .setObjectInfo(this.dyn.graph, pname.name, e.event === Event.MB1Down);
            break;
        }
        if (even(sts)) {
          console.log("StoDig: " + pname.name);
        }
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.StoDig:
          break;
        case DynSave.StoDig_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynStoDig");
          break;
      }
    }

    return i;
  }
}

class DynCommand extends DynElem {
  command;

  constructor(dyn) {
    super(dyn, DynPrio.Command);
    this.action_type1 = ActionType1.Command;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(e.event === Event.MB1Down);
        this.dyn.repaintNow = true;
        break;
      case Event.MB1Click:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        if (this.command === null) {
          return 1;
        }

        let cmd = this.dyn.graph.getCommand(this.command);

        let r = this.dyn.graph.ctx.gdraw.get_clip_rectangle();
        if (r) {
          this.dyn.graph.ctx.gdraw.reset_clip_rectangle();
        }

        this.dyn.graph.command(cmd);

        if (r) {
          this.dyn.graph.ctx.gdraw.set_clip_rectangle(r.x1, r.y1, r.x2, r.y2);
        }

        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Command:
          break;
        case DynSave.Command_command:
          if (tokens.length > 1) {
            this.command = lines[i].substring(5);
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynCommand");
          break;
      }
    }

    return i;
  }
}

class DynScript extends DynElem {
  script;
  script_len;

  constructor(dyn) {
    super(dyn, DynPrio.Script);
    this.action_type1 = ActionType1.Script;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(e.event === Event.MB1Down);
        this.dyn.repaintNow = true;
        break;
      case Event.MB1Click:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        if (this.script === null) {
          return 1;
        }

        this.dyn.graph.script(this.script);
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Script:
          break;
        case DynSave.Script_script_len:
          this.script_len = parseInt(tokens[1], 10);
          break;
        case DynSave.Script_script:
          let idx;
          let send = false;
          this.script = "";
          i++;
          let line = lines[i].trim().substring(1);

          idx = 0;
          while (line !== null) {
            while ((idx = line.indexOf('"', idx)) !== -1) {
              if (idx > 0 && (line.charAt(idx - 1) === '\\')) {
                line = line.substring(0, idx - 1) + line.substring(idx);
              } else {
                if (idx > 0) {
                  line = line.substring(0, idx - 1);
                } else {
                  line = "";
                }
                this.script += line;
                send = true;
                break;
              }
            }
            if (send) {
              break;
            }
            this.script += line + "\n";
            i++;
            if (i >= lines.length) {
              break;
            }

            line = lines[i];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynScript");
          break;
      }
    }

    return i;
  }
}

class DynCommandDoubleClick extends DynElem {
  command;

  constructor(dyn) {
    super(dyn, DynPrio.CommandDoubleClick);
    this.action_type1 = ActionType1.CommandDoubleClick;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(e.event === Event.MB1Down);
        this.dyn.repaintNow = true;
        break;
      case Event.MB1DoubleClick:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        if (this.command === null) {
          return 1;
        }

        let cmd = this.dyn.graph.getCommand(this.command);

        this.dyn.graph.command(cmd);
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.CommandDC:
          break;
        case DynSave.CommandDC_command:
          if (tokens.length > 1) {
            this.command = lines[i].substring(5);
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynCommandDoubleClick");
          break;
      }
    }

    return i;
  }
}

class DynConfirm extends DynElem {
  text;
  on_set;
  on_reset;

  constructor(dyn) {
    super(dyn, DynPrio.Confirm);
    this.action_type1 = ActionType1.Confirm;
  }

  action(object, e) {
    if ((this.dyn.action_type1 & ActionType1.Confirm) === 0) {
      return 1;
    }

    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
        break;
      case Event.Key_Return:
      case Event.MB1Click:
        let skip = 0;
        if (((this.on_set !== 0 && this.on_reset === 0) ||
                (this.on_reset !== 0 && this.on_set === 0)) &&
            (this.dyn.total_action_type1 & ActionType1.ToggleDig) !== 0) {
          for (let j = 0; j < this.dyn.elements.length; j++) {
            if (this.dyn.elements[j].action_type1 === ActionType1.ToggleDig) {
              let pname = this.dyn.parseAttrName(this.dyn.elements[j].attribute);
              if (pname.name.substring(0, 1) === "&") {
                pname.name = this.dyn.graph.getReferenceName(pname.name);
              }

              switch (pname.database) {
                case Database.Gdh:
                  // TODO
                  let ret = this.dyn.graph.getGdh()
                      .getObjectInfoBoolean(pname.name);
                  if (odd(ret)) {
                    if ((this.on_set !== 0 && ret.value) ||
                        (this.on_reset !== 0 && !ret.value)) {
                      skip = 1;
                    }
                  } else {
                    console.log("Confirm: " + ret);
                  }
                  break;
                case Database.Ccm:
                  // TODO
                  break;
                default:
              }
              break;
            }
          }
        }
        if (skip !== 0) {
          this.dyn.confirmedAction(e.event, object);
          return 1;
        }

        this.dyn.graph.openConfirmDialog(this.dyn, this.text, object);
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Confirm:
          break;
        case DynSave.Confirm_text:
          if (tokens.length > 1) {
            this.text = lines[i].substring(5);
          }
          break;
        case DynSave.Confirm_on_set:
          this.on_set = parseInt(tokens[1], 10);
          break;
        case DynSave.Confirm_on_reset:
          this.on_reset = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynConfirm");
          break;
      }
    }

    return i;
  }
}

class DynIncrAnalog extends DynElem {
  increment;
  min_value;
  max_value;

  constructor(dyn) {
    super(dyn, DynPrio.IncrAnalog);
    this.action_type1 = ActionType1.IncrAnalog;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(e.event === Event.MB1Down);
        this.dyn.repaintNow = true;
        break;
      case Event.MB1Click:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        let pname = this.dyn.parseAttrName(this.attribute);
        if (pname === null) {
          return 1;
        }
        let typeId = pname.type;
        if (pname.type < 0) {
          typeId = Type.Float32;
        }
        switch (pname.database) {
          case Database.Gdh:
            if (typeId === Type.Int32) {
              this.dyn.graph.getGdh()
                  .getObjectInfoInt(pname.name).then(this.action2);
            } else {
              this.dyn.graph.getGdh()
                  .getObjectInfoFloat(pname.name).then(this.action2);
            }
            break;
          case Database.Local:
            let ret = this.dyn.graph.getLdb()
                .getObjectInfo(this.dyn.graph, pname.name);
            this.action2({id: 0, sts: 1, value: ret.value});
            break;
        }
        break;
    }

    return 1;
  }

  action2(res) {
    if (!(res.sts & 1)) {
      return;
    }

    let pname = this.dyn.parseAttrName(this.attribute);
    if (pname === null) {
      return 1;
    }
    let typeId = (pname.type < 0) ? Type.Float32 : pname.type;
    let sts = res.sts;
    let value = res.value + this.increment;
    if (!(this.min_value === 0 && this.max_value === 0)) {
      if (typeId === Type.Int32) {
        value = clamp(value, Math.floor(this.min_value), Math.floor(this.max_value));
      } else {
        value = clamp(value, this.min_value, this.max_value);
      }
    }

    if (pname.database === Database.Gdh) {
      if (typeId === Type.Int32) {
        sts = this.dyn.graph.getGdh().setObjectInfoInt(pname.name, value);
      } else {
        sts = this.dyn.graph.getGdh().setObjectInfoFloat(pname.name, value);
      }
    } else if (pname.database === Database.Local) {
      sts = this.dyn.graph.getLdb().setObjectInfo(this.dyn.graph, pname.name, value);
    }
    if (even(sts)) {
      console.log("IncrAnalog " + pname.name);
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.IncrAnalog:
          break;
        case DynSave.IncrAnalog_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.IncrAnalog_increment:
          this.increment = parseFloat(tokens[1]);
          break;
        case DynSave.IncrAnalog_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case DynSave.IncrAnalog_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynIncrAnalog");
          break;
      }
    }

    return i;
  }
}

class DynRadioButton extends DynElem {
  a;
  firstScan = true;

  constructor(dyn) {
    super(dyn, DynPrio.RadioButton);
    this.action_type1 = ActionType1.RadioButton;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("RadioButton: " + this.attribute);
      return 1;
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
  }

  scan(object) {
    if (!this.a.sts) {
      return;
    }
    let value = this.dyn.getDig(this.a.p, this.a.typeid, this.a.bitmask,
        this.a.database);
    value = (this.a.inverted) ? !value : value;

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      if (this.a.oldValue === value) {
        return;
      }
    }

    if (value) {
      object.set_last_nodeclass();
    } else {
      object.set_root_nodeclass();
    }
    this.a.oldValue = value;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(e.event === Event.MB1Down);
        this.dyn.repaintNow = true;
        break;
      case Event.MB1Click:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        let value;
        let group = this.dyn.graph.getCtx().get_object_group(object);
        if (group === null) {
          break;
        }

        let setObjectInfo = function(pname, value) {
          if (pname.database === Database.Gdh) {
            this.dyn.graph.getGdh()
                .setObjectInfoBoolean(pname.name, value);
          } else if (pname.database === Database.Local) {
            this.dyn.graph.getLdb()
                .setObjectInfo(this.dyn.graph, pname.name, value);
          }
        };

        let list = group.get_object_list();

        for (let i = 0; i < list.size(); i++) {
          let oe = list.get(i);
          if (oe !== e.object && oe instanceof GrowNode) {
            value = false;
            let gm_dyn = oe.getUserData();

            if ((gm_dyn.total_action_type1 & ActionType1.RadioButton) !==
                0) {
              for (let j = 0; j < gm_dyn.elements.length; j++) {
                if (gm_dyn.elements[j].action_type1 ===
                    ActionType1.RadioButton) {
                  let pname = this.dyn.parseAttrName(
                      gm_dyn.elements[j].attribute);
                  if (pname.name.substring(0, 1) === "&") {
                    pname.name = this.dyn.graph.getReferenceName(pname.name);
                  }
                  setObjectInfo(pname, value);
                }
              }
            }
          }
        }

        let pname = this.dyn.parseAttrName(this.attribute);
        if (pname.name.substring(0, 1) === "&") {
          pname.name = this.dyn.graph.getReferenceName(pname.name);
        }

        value = true;
        setObjectInfo(pname, value);
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.RadioButton:
          break;
        case DynSave.RadioButton_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynRadioButton");
          break;
      }
    }

    return i;
  }
}

class DynTipText extends DynElem {
  text;

  constructor(dyn) {
    super(dyn, DynPrio.TipText);
    this.action_type1 = ActionType1.TipText;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.TipText:
          break;
        case DynSave.TipText_text:
          if (tokens.length > 1) {
            this.text = lines[i].substring(5);
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynTipText");
          break;
      }
    }

    return i;
  }
}

class DynHelp extends DynElem {
  topic = null;
  bookmark = null;

  constructor(dyn) {
    super(dyn, DynPrio.Help);
    this.action_type1 = ActionType1.Help;
  }

  action(object, e) {
    if (this.topic === null) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(event === Event.MB1Down);
        this.dyn.repaintNow = true;
        break;
      case Event.MB1Click:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        let command = "help " + this.topic;
        if (this.bookmark) {
          command += " /bookmark=" + this.bookmark;
        }

        this.dyn.graph.command(command);
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Help:
          break;
        case DynSave.Help_topic:
          if (tokens.length > 1) {
            this.topic = tokens[1];
          }
          break;
        case DynSave.Help_bookmark:
          if (tokens.length > 1) {
            this.bookmark = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynHelp");
          break;
      }
    }

    return i;
  }
}

class DynOpenGraph extends DynElem {
  graph_object;

  constructor(dyn) {
    super(dyn, DynPrio.OpenGraph);
    this.action_type1 = ActionType1.OpenGraph;
  }

  action(o, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        o.setColorInverse(event === Event.MB1Down);
        this.dyn.repaintNow = true;
        break;
      case Event.MB1Click:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }
        if (this.graph_object === null || this.graph_object === "") {
          // Open classgraph for popup menu object
          if ((this.dyn.total_action_type1 & ActionType1.PopupMenu) !== 0) {
            let obj = <DynPopupMenu>this.dyn.elements.find(e => e.action_type1 === ActionType1.PopupMenu);
            if (obj) {
              this.dyn.graph.command("open graph/class/instance=" + obj.ref_object);
            }
          } else if ((this.dyn.total_dyn_type1 & DynType1.HostObject) !== 0) {
            let obj = <DynHostObject>this.dyn.elements.find(e => e.dyn_type1 === DynType1.HostObject);
            if (obj) {
              this.dyn.graph.command("open graph/class/instance=" + obj.hostobject);
            }
          }
        } else {
          this.dyn.graph.command("open graph/object=" + this.graph_object);
        }
        return 1;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.OpenGraph:
          break;
        case DynSave.OpenGraph_graph_object:
          if (tokens.length > 1) {
            this.graph_object = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynOpenGraph");
          break;
      }
    }

    return i;
  }
}

class DynOpenURL extends DynElem {
  url;

  constructor(dyn) {
    super(dyn, DynPrio.OpenURL);
    this.action_type1 = ActionType1.OpenURL;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(event === Event.MB1Down);
        this.dyn.repaintNow = true;
        break;
      case Event.MB1Click:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        this.dyn.graph.command("open url \"" + this.url + "\"");
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.OpenURL:
          break;
        case DynSave.OpenURL_url:
          if (tokens.length > 1) {
            this.url = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynOpenURL");
          break;
      }
    }

    return i;
  }
}

class DynInputFocus extends DynElem {
  initial_focus;
  next_horizontal;
  next_vertical;
  next_tab;

  constructor(dyn) {
    super(dyn, DynPrio.InputFocus);
    this.action_type1 = ActionType1.InputFocus;
  }

  action(o, e) {
    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.InputFocus:
          break;
        case DynSave.InputFocus_initial_focus:
          this.initial_focus = parseInt(tokens[1], 10);
          break;
        case DynSave.InputFocus_next_horizontal:
          if (tokens.length > 1) {
            this.next_horizontal = tokens[1];
          }
          break;
        case DynSave.InputFocus_next_vertical:
          if (tokens.length > 1) {
            this.next_vertical = tokens[1];
          }
          break;
        case DynSave.InputFocus_next_tab:
          if (tokens.length > 1) {
            this.next_tab = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynInputFocus");
          break;
      }
    }

    return i;
  }
}

class DynCloseGraph extends DynElem {
  constructor(dyn) {
    super(dyn, DynPrio.CloseGraph);
    this.action_type1 = ActionType1.CloseGraph;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(event === Event.MB1Down);
        this.dyn.repaintNow = true;
        break;
      case Event.MB1Click:
        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        this.dyn.graph.closeGraph();
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.CloseGraph:
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynCloseGraph");
          break;
      }
    }

    return i;
  }
}

class DynSlider extends DynElem {
  a = null;
  maxvalue_a;
  minvalue_a;
  insensitive_a;
  attribute = null;
  minvalue_attr = null;
  maxvalue_attr = null;
  insensitive_attr = null;
  release_attr = null;
  old_value;
  old_ivalue;
  firstScan = true;
  min_value;
  max_value;
  slider_disabled = true;
  direction;

  constructor(dyn) {
    super(dyn, DynPrio.Slider);
    this.action_type1 = ActionType1.Slider;
  }

  connect(object) {
    if (this.attribute.trim() === "") {
      return 0;
    }

    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("FillLevel: " + this.attribute);
      return 1;
    }

    // Get min and max position from slider background
    if (!object.transformIsStored()) {
      object.storeTransform();
      let g = object.measure();
      let info = object.get_info();
      let b = this.dyn.graph.getCtx()
          .getBackgroundObjectLimits(DynType1.SliderBackground, (g.ll_x + g.ur_x) / 2, (g.ll_y + g.ur_y) / 2);
      if ((b.sts & 1) === 0) {
        this.direction = info.direction;
      } else {
        this.direction = b.direction;
        let origo = object.get_origo(info.direction);

        switch (this.direction) {
          case Direction.Down:
            info.max_position = b.max - origo;
            info.min_position = b.min - origo;
            object.set_info(info);

            object.move_to(g.x, info.min_position);
            break;
          case Direction.Up:
            info.max_position = b.max - (g.height - origo);
            info.min_position = b.min - (g.height - origo);
            object.set_info(info);
            object.move_to(g.x, info.min_position);
            break;
          case Direction.Left:
            info.max_position = b.max - (g.width - origo);
            info.min_position = b.min - (g.width - origo);
            object.set_info(info);
            object.move_to(info.min_position, g.y);
            break;
          case Direction.Right:
            info.max_position = b.max - origo;
            info.min_position = b.min - origo;
            object.set_info(info);
            object.move_to(info.min_position, g.y);
            break;
          default:
        }
      }

      object.storeTransform();
    }

    this.maxvalue_a = new DynReference(this.dyn, this.maxvalue_attr);
    this.maxvalue_a.connect(this.dyn);

    this.minvalue_a = new DynReference(this.dyn, this.minvalue_attr);
    this.minvalue_a.connect(this.dyn);

    this.insensitive_a = new DynReference(this.dyn, this.insensitive_attr);
    this.insensitive_a.connect(this.dyn);

    return 1;
  }

  disconnect() {
    if (this.a === null || !this.a.sts) {
      return;
    }
    this.a.disconnect(this.dyn);
    this.minvalue_a.disconnect(this.dyn);
    this.maxvalue_a.disconnect(this.dyn);
    this.insensitive_a.disconnect(this.dyn);
  }

  scan(object) {
    if (this.a === null || !this.a.sts) {
      return;
    }

    let value = 0;
    let ivalue = 0;

    switch (this.a.typeid) {
      case Type.Float32:
        value = this.a.get_ref_value(this.dyn);
        if (value === undefined) {
          value = 0;
        }
        break;
      case Type.Int32:
        ivalue = this.a.get_ref_value(this.dyn);
        if (ivalue === undefined) {
          ivalue = 0;
        }
        break;
      case Type.Boolean:
        let b;
        b = this.a.get_ref_value(this.dyn);
        if (b === undefined) {
          b = false;
        }
        ivalue = b ? 1 : 0;
        break;
    }

    if (this.insensitive_a.sts) {
      this.slider_disabled = this.insensitive_a.get_ref_value(this.dyn);
      if (this.insensitive_a.inverted) {
        this.slider_disabled = !this.slider_disabled;
      }
    } else {
      this.slider_disabled = false;
    }

    if (this.maxvalue_a.sts && this.minvalue_a.sts) {
      this.max_value = this.maxvalue_a.get_ref_value(this.dyn);
      this.min_value = this.minvalue_a.get_ref_value(this.dyn);
      if (this.max_value !== this.maxvalue_a.oldValue ||
          this.min_value !== this.minvalue_a.oldValue) {
        if (Math.abs(this.max_value - this.min_value) > Number.MIN_VALUE) {
          this.firstScan = true;
        }
      }
      this.minvalue_a.oldValue = this.min_value;
      this.maxvalue_a.oldValue = this.max_value;
    }

    if (this.firstScan) {
      this.firstScan = false;
    } else {
      switch (this.a.typeid) {
        case Type.Float32:
          if (Math.abs(this.old_value - value) < Number.MIN_VALUE) { // No change since last time
            return;
          }
          break;
        case Type.Int32:
        case Type.Boolean:
          if (ivalue === this.old_ivalue) {
            return;
          }
          break;
        default:
      }
    }

    switch (this.a.typeid) {
      case Type.Float32:
        this.old_value = value;
        break;
      case Type.Int32:
      case Type.Boolean:
        this.old_ivalue = ivalue;
        break;
      default:
    }

    let info = object.get_info();

    if (!(this.maxvalue_a.sts && this.minvalue_a.sts && this.max_value !==
            this.min_value)) {
      this.max_value = info.max_value;
      this.min_value = info.min_value;
    }
    if (info.min_position !== info.max_position) {
      if (this.dyn.graph.getCurrentSlider() !== object &&
          this.max_value !== this.min_value) {
        switch (this.a.typeid) {
          case Type.Float32:
            break;
          default:
            value = ivalue;
            break;
        }

        let diff = info.max_position - info.min_position;

        let pos = this.max_value - value;
        if (this.direction === Direction.Right || this.direction === Direction.Up) {
          pos = value - this.min_value;
        }

        pos = pos / (this.max_value - this.min_value) * diff;
        if (this.direction === Direction.Right || this.direction === Direction.Left) {
          object.set_position(clamp(diff - pos, 0, diff), 0);
        } else {
          object.set_position(0, clamp(pos, 0, diff));
        }
      }
    }
  }

  action(object, e) {
    if (this.slider_disabled) {
      return 1;
    }

    switch (e.event) {
      case Event.SliderMoveEnd:
        let pname = this.dyn.parseAttrName(this.release_attr);
        if (!(pname === null || pname.name === "")) {
          let sts = null;
          switch (pname.type) {
            case Type.Boolean:
              switch (pname.database) {
                case Database.Gdh:
                  sts =
                      this.dyn.graph.getGdh().setObjectInfoBoolean(pname.name, 1);
                  break;
                case Database.Local:
                  sts = this.dyn.graph.getLdb()
                      .setObjectInfo(this.dyn.graph, pname.name, 1);
                  break;
              }
              break;
            default:
          }
        }
        if (this.dyn.graph.getCurrentSlider() === object) {
          this.dyn.graph.setCurrentSlider(null);
        }
        break;
      case Event.SliderMoveStart:
        if (!this.dyn.graph.isAuthorized(this.dyn.access) ||
            this.slider_disabled) {
          this.dyn.graph.getCtx()
              .setMoveRestrictions(MoveRestriction.Disable, 0, 0, null);
          this.dyn.graph.setCurrentSlider(null);
          break;
        }
        let info = object.get_info();
        if (this.direction === Direction.Right ||
            this.direction === Direction.Left) {
          this.dyn.graph.getCtx()
              .setMoveRestrictions(MoveRestriction.HorizontalSlider,
                  info.max_position, info.min_position, e.object);
        } else {
          this.dyn.graph.getCtx()
              .setMoveRestrictions(MoveRestriction.VerticalSlider,
                  info.max_position, info.min_position, e.object);
        }

        this.dyn.graph.setCurrentSlider(object);
        break;
      case Event.SliderMoved:
        let value;

        let info = object.get_info();
        if (info.min_position !== info.max_position) {
          if (!(this.max_value !== 0 && this.min_value !== 0 &&
                  this.max_value !== this.min_value)) {
            this.max_value = info.max_value;
            this.min_value = info.min_value;
          }
          let g = object.measure();

          switch (this.direction) {
            case Direction.Down:
              value = info.max_position - g.y;
              break;
            case Direction.Right:
              value = info.max_position - g.x;
              break;
            case Direction.Left:
              value = g.x - info.min_position;
              break;
            default:
              value = g.y - info.min_position;
          }
          value = value / (info.max_position - info.min_position) *
              (this.max_value - this.min_value) + this.min_value;
          value = clamp(value, this.min_value, this.max_value);

          let pname = this.dyn.parseAttrName(this.attribute);
          if (pname === null || pname.name === "") {
            return 1;
          }

          let setObjectInfo = function(pname, value, type) {
            if (pname.database === Database.Gdh) {
              return this.dyn.graph.getGdh().setObjectInfo(pname.name, value, type);
            } else if (pname.database === Database.Local) {
              return this.dyn.graph.getLdb().setObjectInfo(this.dyn.graph, pname.name, value);
            } else {
              return 1;
            }
          };

          let sts;
          switch (pname.type) {
            case Type.Float32:
              sts = setObjectInfo(pname, value, pname.type);
              break;
            case Type.Boolean:
              sts = setObjectInfo(pname, (value > 0.5), pname.type);
              break;
            default:
              let ivalue = Math.floor(value > 0 ? value + 0.5 : value - 0.5);
              sts = setObjectInfo(pname, ivalue, pname.type);
          }
          if (even(sts)) {
            console.log("Slider error: " + this.attribute);
          }
        }
        break;
      default:
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.Slider:
          break;
        case DynSave.Slider_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.Slider_minvalue_attr:
          if (tokens.length > 1) {
            this.minvalue_attr = tokens[1];
          }
          break;
        case DynSave.Slider_maxvalue_attr:
          if (tokens.length > 1) {
            this.maxvalue_attr = tokens[1];
          }
          break;
        case DynSave.Slider_insensitive_attr:
          if (tokens.length > 1) {
            this.insensitive_attr = tokens[1];
          }
          break;
        case DynSave.Slider_release_attr:
          if (tokens.length > 1) {
            this.release_attr = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynSlider");
          break;
      }
    }

    return i;
  }
}

class DynFastCurve extends DynElem {
  fast_object;
  curve_index1;
  curve_index2;

  constructor(dyn) {
    super(dyn, DynPrio.FastCurve);
    this.dyn_type1 = DynType1.FastCurve;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.FastCurve:
          break;
        case DynSave.FastCurve_fast_object:
          if (tokens.length > 1) {
            this.fast_object = tokens[1];
          }
          break;
        case DynSave.FastCurve_curve_index1:
          this.curve_index1 = parseInt(tokens[1], 10);
          break;
        case DynSave.FastCurve_curve_index2:
          this.curve_index2 = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynFastCurve");
          break;
      }
    }

    return i;
  }
}

class DynPulldownMenu extends DynElem {
  button_mask;
  items_text = new Array(32);
  items_dyn = new Array(32);
  menu_object = null;
  text_size;

  constructor(dyn) {
    super(dyn, DynPrio.PulldownMenu);
    this.action_type1 = ActionType1.PulldownMenu;
    for (let i = 0; i < 32; i++) {
      this.items_dyn[i] = null;
    }
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Click:
        if (this.menu_object) {
          // Close, delete this menu and all childmenues
          for (let j = 0; j < 32; j++) {
            if (this.items_dyn[j] && (this.items_dyn[j].action_type1 &
                    ActionType1.PulldownMenu) !== 0) {
              this.items_dyn[j].action(null, e);
            }
          }
          this.dyn.graph.getCtx().remove(this.menu_object);
          this.menu_object = null;
        } else if (object) {
          let info = new GlowMenuInfo();

          let b_mask = 1;
          for (let i = 0; i < 32; i++) {
            if ((b_mask & this.button_mask) === 0) {
              info.item[i].occupied = false;
            } else {
              info.item[i].occupied = true;
              info.item[i].text = this.items_text[i];

              // Check access
              if ((this.items_dyn[i].action_type1 &
                      ActionType1.PulldownMenu) === 0) {
                // Check access
                if (this.dyn.graph.isAuthorized(this.items_dyn[i].access)) {
                  info.item[i].type = MenuItem.Button;
                  if ((this.items_dyn[i].dyn_type1 &
                          DynType1.Invisible) !== 0) {
                    let sts;
                    let invis_element = null;
                    let command;

                    for (let j = 0; j < this.dyn.elements.length; j++) {
                      if (this.dyn.elements[j].dyn_type1 ===
                          DynType1.Invisible) {
                        invis_element = this.dyn.elements[j];
                      }
                    }
                    if (invis_element) {
                      let pname = this.dyn.parseAttrName(
                          invis_element.attribute);
                      if (pname && pname.name !== "") {
                        if (pname.name.substring(0, 5) === "$cmd(") {
                          let idx = pname.name.lastIndexOf(")");
                          if (idx !== -1) {
                            command = pname.name.substring(5, idx);
                            command = this.dyn.graph.getCommand(command);
                            sts = this.dyn.graph.command(command);
                            if ((sts & 1) === 0) {
                              info.item[i].type = MenuItem.ButtonDisabled;
                            }
                          }
                        }
                      } else {
                        /** TODO
                         this.dyn.graph.getGdh().getObjectInfoBoolean( pname.name, this.action2, data);
                         if (odd(ret)) {
			if ( (!pname.inverted && ret.value) || (pname.inverted && !ret.value)) {
			  if ( invis_element.dimmed != 0)
			    info.item[i].type = MenuItem.ButtonDisabled;
			  else
			    info.item[i].occupied = false;
			}
		      }
                         **/
                      }
                    }
                  }
                } else {
                  info.item[i].type = MenuItem.ButtonDisabled;
                }
              } else {
                info.item[i].type = MenuItem.PulldownMenu;
              }
            }
            b_mask <<= 1;
          }

          // Get fillcolor, and textattributes from object
          let text_drawtype = 0, text_color = 0, bg_color = 0;
          let text_size = 0;
          let sts;
          let scale = 1;
          let text_font = 0;

          sts = 0; // TODO sts = grow_GetObjectAnnotInfo( object, 1, &text_size, &text_drawtype, &text_color, &bg_color, &scale, &text_font);
          if ((sts & 1) === 0) {
            text_size = 3;
            text_drawtype = DrawType.TextHelvetica;
            text_font = Font.LucidaSans;
            text_color = DrawType.Line;
            bg_color = DrawType.LightGray;
          } else if (bg_color === DrawType.No ||
              bg_color === DrawType.Inherit) {
            bg_color = DrawType.LightGray;
          }

          let g = object.measure();
          this.menu_object = new GrowMenu(this.dyn.graph.getCtx(),
              "__Menu", info, g.x, g.ur_y, g.width(),
              DrawType.Line, 0, 1, 1, bg_color, text_size,
              text_drawtype, text_color, DrawType.MediumGray, text_font);
          this.menu_object.set_scale(scale, scale, 0, 0,
              ScaleType.LowerLeft);
          this.dyn.graph.getCtx().insert(this.menu_object);

          // grow_SetMenuInputFocus( menu_object, 1);
        }

        break;
      case Event.MenuActivated:
        if (this.menu_object === null) {
          break;
        }
        if (e.object === this.menu_object) {
          if (this.items_dyn[e.item]) {
            let event = new GlowEvent();
            event.event = Event.MB1Click;
            return this.items_dyn[e.item].action(e.object, event);
          }
        } else {
          for (let j = 0; j < 32; j++) {
            if (this.items_dyn[j] &&
                (this.items_dyn[j].action_type1 & ActionType1.PulldownMenu) !==
                0) {
              this.items_dyn[j].action(null, e);
            }
          }
        }
        break;
      case Event.MenuDelete:
        if (this.menu_object === null) {
          break;
        }
        if (e.object === null || e.object === this.menu_object) {
          // Delete this menu
          this.dyn.graph.getCtx().remove(this.menu_object);
          this.menu_object = null;
        }
        break;

      default:
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (key === DynSave.End) {
        return i;
      } else if (key === DynSave.PulldownMenu_button_mask) {
        this.button_mask = parseInt(tokens[1], 10);
      } else if (key >= DynSave.PulldownMenu_items_text0 && key <= DynSave.PulldownMenu_items_text31) {
        if (tokens.length > 1) {
          this.items_text[key - DynSave.PulldownMenu_items_text0] = tokens[1];
        }
      } else if (key >= DynSave.PulldownMenu_items_dyn0 && key <= DynSave.PulldownMenu_items_dyn31) {
        this.items_dyn[key - DynSave.PulldownMenu_items_dyn0] = new Dyn(this.dyn.graph);
        i = this.items_dyn[key - DynSave.PulldownMenu_items_dyn0].open(lines, i + 1);
      } else {
        console.log("Syntax error in DynPulldownMenu");
      }
    }

    return i;
  }
}

class DynOptionMenu extends DynElem {
  a;
  update_a = null;
  text_attribute;
  size_attribute;
  update_attribute;
  optionmenu_type;
  button_mask;
  items_text = new Array(32);
  items_enum = new Array(32);
  firstScan = true;
  oldValueF;
  oldValueI;
  menu_object = null;
  text_size;

  constructor(dyn) {
    super(dyn, DynPrio.OptionMenu);
    this.action_type1 = ActionType1.OptionMenu;
  }

  connect(object) {
    this.a = new DynReference(this.dyn, this.attribute);
    this.a.connect(this.dyn);
    if (!this.a.sts) {
      console.log("OptionMenu: " + this.attribute);
      return 1;
    }

    if (this.text_attribute && this.update_attribute) {
      this.update_a = new DynReference(this.dyn, this.update_attribute);
      this.update_a.connect(this.dyn);
      if (!this.update_a.sts) {
        console.log("OptionsMenu: " + this.update_attribute);
        return 1;
      }
    }

    return 1;
  }

  disconnect() {
    this.a.disconnect(this.dyn);
    if (this.update_a) {
      this.update_a.disconnect(this.dyn);
    }
  }

  scan(object) {
    if (!this.a.sts) {
      return;
    }

    let update_texts = false;
    if (this.update_a && this.update_a.sts) {
      let value = this.dyn.getDig(this.update_a.p, this.update_a.typeid,
          this.update_a.bitmask, this.update_a.database);

      value = (this.update_a.inverted) ? !value : value;

      if (this.firstScan) {
        update_texts = false;
      } else {
        if (this.update_a.oldValue !== value) {
          update_texts = true;
        }
      }
      this.update_a.oldValue = value;
    }

    if (update_texts) {
    }

    let enum_value = 0;
    switch (this.a.typeid) {
      case Type.Float32:
        let value = this.a.get_ref_value(this.dyn);

        if (this.firstScan) {
          this.firstScan = false;
        } else {
          if (value === this.oldValueF) {
            return;
          }
        }

        enum_value = Math.floor(value + 0.5);
        this.oldValueF = value;

        break;
      case Type.UInt32:
      case Type.Int32:
        let value = this.a.get_ref_value(this.dyn);

        if (this.firstScan) {
          this.firstScan = false;
        } else {
          if (value === this.oldValueI) {
            return;
          }
        }

        enum_value = value;
        this.oldValueI = value;

        break;
      default:
        return;
    }

    if (this.update_a === null || !this.update_a.sts) {
      let found = this.items_enum.slice(0, 32).find(e => e === enum_value);
      object.setAnnotation(1, found || "");
    }
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Click:
        if (this.menu_object === null) {
          let info = new GlowMenuInfo();

          if (this.update_a === null || !this.update_a.sts) {
            let b_mask = 1;
            for (let i = 0; i < 32; i++) {
              info.item[i].occupied = ((b_mask & this.button_mask) !== 0);
              if (info.item[i].occupied) {
                info.item[i].text = this.items_text[i];

                // Check access
                info.item[i].type = MenuItem.Button;
              }
              b_mask <<= 1;
            }
          } else {
            for (let i = 0; i < 32; i++) {
              if (i < this.text_size) {
                info.item[i].text = this.items_text[i];
                info.item[i].type = MenuItem.Button;
              }
              info.item[i].occupied = (i < this.text_size);
            }
          }

          // Get fillcolor, and textattributes from object
          let text_drawtype = 4, text_color = DrawType.Line,
              bg_color = DrawType.Color32;
          let tsize = 2;
          let sts;
          let scale = 1;
          let text_font = Font.Helvetica;

          sts = 0; //grow_GetObjectAnnotInfo( object, 1, &tsize, &text_drawtype, &text_color, &bg_color, &scale, &text_font);
          if ((sts & 1) === 0) {
            tsize = 2;
            text_drawtype = DrawType.TextHelveticaBold;
            text_color = DrawType.Line;
            bg_color = DrawType.LightGray;
          } else if (bg_color === DrawType.No ||
              bg_color === DrawType.Inherit) {
            bg_color = DrawType.LightGray;
          }

          let g = object.measure();
          this.menu_object = new GrowMenu(this.dyn.graph.getCtx(),
              "__Menu", info, g.ll_x, g.ur_y, g.width(),
              DrawType.Line, 0, 1, 1, bg_color, tsize,
              text_drawtype, text_color, DrawType.MediumGray, text_font);
          this.menu_object.set_scale(scale, scale, 0, 0,
              ScaleType.LowerLeft);
          this.dyn.graph.getCtx().insert(this.menu_object);
        } else {
          // Close, delete this menu
          this.dyn.graph.getCtx().remove(this.menu_object);
          this.menu_object = null;
        }
        break;
      case Event.MenuActivated:
        if (this.menu_object === null) {
          break;
        }
        if (e.object === this.menu_object) {
          // Set enum value to attribute

          let pname = this.dyn.parseAttrName(this.attribute);
          if (pname === null || pname.name === "") {
            return 1;
          }

          let sts = null;

          let value = this.items_enum[e.item];
          switch (pname.database) {
            case Database.Gdh:
              switch (pname.type) {
                case Type.Float32:
                  sts = this.dyn.graph.getGdh()
                      .setObjectInfoFloat(pname.name, value);
                  break;
                case Type.Int32:
                case Type.UInt32:
                case Type.Int16:
                case Type.UInt16:
                case Type.Int8:
                case Type.UInt8:
                  let value = this.items_enum[e.item];
                  sts =
                      this.dyn.graph.getGdh().setObjectInfoInt(pname.name, value);
                  break;
                default:
              }
              break;
            case Database.Local:
              switch (pname.type) {
                case Type.Float32:
                case Type.Int32:
                case Type.UInt32:
                case Type.Int16:
                case Type.UInt16:
                case Type.Int8:
                case Type.UInt8:
                  sts = this.dyn.graph.getLdb()
                      .setObjectInfo(this.dyn.graph, pname.name, value);
                  break;
                default:
              }
              break;
            case Database.Ccm:
              /* TODO
	     pwr_tInt32 value = items_enum[event->menu.item];
	     sts = dyn->graph->ccm_set_variable( parsed_name, type_id, &value);
	  */
              break;
            default :
          }

          if (sts === null || even(sts)) {
            console.log("Option menu error: " + pname.name);
          }
        }
        break;
      case Event.MenuDelete:
        if (this.menu_object === null) {
          break;
        }
        if (e.object === null || e.object === this.menu_object) {
          // Delete this menu
          this.dyn.graph.getCtx().remove(this.menu_object);
          this.menu_object = null;
        }
        break;
      default:
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (key >= DynSave.OptionMenu_items_text0 && key <= DynSave.OptionMenu_items_text31) {
        if (tokens.length > 1) {
          this.items_text[key - DynSave.OptionMenu_items_text0] = lines[i].substring(5);
        }
        continue;
      } else if (key >= DynSave.OptionMenu_items_enum0 && key <= DynSave.OptionMenu_items_enum31) {
        this.items_enum[key - DynSave.OptionMenu_items_enum0] = parseInt(tokens[1], 10);
        continue;
      }

      switch (key) {
        case DynSave.OptionMenu:
          break;
        case DynSave.OptionMenu_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.OptionMenu_text_attribute:
          if (tokens.length > 1) {
            this.text_attribute = tokens[1];
          }
          break;
        case DynSave.OptionMenu_size_attribute:
          if (tokens.length > 1) {
            this.size_attribute = tokens[1];
          }
          break;
        case DynSave.OptionMenu_update_attribute:
          if (tokens.length > 1) {
            this.update_attribute = tokens[1];
          }
          break;
        case DynSave.OptionMenu_optionmenu_type:
          this.optionmenu_type = parseInt(tokens[1], 10);
          break;
        case DynSave.OptionMenu_button_mask:
          this.button_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynOptionMenu");
          break;
      }
    }

    return i;
  }
}

class DynAnalogText extends DynOptionMenu {
  constructor(dyn) {
    super(dyn);
    this.prio = DynPrio.AnalogText;
    this.dyn_type1 = DynType1.AnalogText;
  }

  action(o, e) {
    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.AnalogText:
          break;
        case DynSave.AnalogText_super:
          i = super.open(lines, i + 1);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynAnalogText");
          break;
      }
    }

    return i;
  }
}

class DynSetValue extends DynElem {
  value;

  constructor(dyn) {
    super(dyn, DynPrio.SetValue);
    this.action_type1 = ActionType1.SetValue;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.SetValue:
          break;
        case DynSave.SetValue_attribute:
          if (tokens.length > 1) {
            this.attribute = tokens[1];
          }
          break;
        case DynSave.SetValue_value:
          if (tokens.length > 1) {
            this.value = tokens[1];
          }
          break;
        case DynSave.SetValue_instance:
          this.instance = parseInt(tokens[1], 10);
          break;
        case DynSave.SetValue_instance_mask:
          this.instance_mask = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynSetValue");
          break;
      }
    }

    return i;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(event === Event.MB1Down);
        break;
      case Event.MB1Click:
        let sts;

        if ((this.dyn.action_type1 & ActionType1.Confirm) !== 0) {
          break;
        }

        let pname = this.dyn.parseAttrName(this.attribute);

        let setObjectInfo = function(pname, value, type) {
          let sts = 0;
          if (pname.database === Database.Gdh) {
            sts = this.dyn.graph.getGdh().setObjectInfo(pname.name, value, type);
          } else if (pname.database === Database.Local) {
            sts = this.dyn.graph.getLdb().setObjectInfo(this.dyn.graph, pname.name, value);
          }

          if (even(sts)) {
            console.log("setObjectInfoError " + sts);
            return 0;
          }
        };

        switch (pname.type) {
          case Type.Float32:
            let inputValue = parseFloat(this.value.trim());
            sts = setObjectInfo(pname, inputValue, pname.type);
            if (even(sts)) {
              return 0;
            }
            break;
          case Type.Int32:
          case Type.UInt32:
          case Type.Int16:
          case Type.UInt16:
          case Type.Int8:
          case Type.UInt8:
          case Type.Mask:
          case Type.Enum:
            let inputValue = parseInt(this.value.trim(), 10);
            sts = setObjectInfo(pname, inputValue, pname.type);
            if (even(sts)) {
              return 0;
            }
            break;
          case Type.Boolean:
            let inputValueInt = parseInt(this.value.trim(), 10);
            let inputValue;
            if (inputValueInt === 0) {
              inputValue = false;
            } else if (inputValueInt === 1) {
              inputValue = true;
            } else {
              break;
            }

            sts = setObjectInfo(pname, inputValue, pname.type);
            if (even(sts)) {
              return 0;
            }
            break;
          case Type.String:
            sts = setObjectInfo(pname, inputValue, pname.type);
            if (even(sts)) {
              return 0;
            }
            break;
          default:
        }

        break;
      default:
        return 0;
    }

    return 1;
  }
}

class DynMethodToolbar extends DynElem {
  method_object;
  toolbar_type;

  pname_name;
  xm_mask_flags = 0;
  xm_mask_opmethods = 0;
  xm_mask_mntmethods = 0;
  mask_configure = 0;
  mask_store = 0;
  o;

  constructor(dyn) {
    super(dyn, DynPrio.MethodToolbar);
    this.action_type1 = ActionType1.MethodToolbar;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.MethodToolbar:
          break;
        case DynSave.MethodToolbar_method_object:
          if (tokens.length > 1) {
            this.method_object = tokens[1];
          }
          break;
        case DynSave.MethodToolbar_toolbar_type:
          this.toolbar_type = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynMethodToolbar");
          break;
      }
    }

    return i;
  }

  connect(object) {
    this.o = object;

    let pname = this.dyn.parseAttrName(this.method_object);
    if (pname === null || pname.name === "") {
      return 1;
    }

    this.pname_name = pname.name;

    let parsed_name = this.pname_name + ".XttMethodsMask.Flags";
    this.dyn.graph.getGdh().getObjectInfoInt(parsed_name).then(this.connect2);
  }

  connect2(res) {
    if (res.sts & 1) {
      this.xm_mask_flags = res.value;

      if ((this.xm_mask_flags & XttMethodsFlagsMask.IsConfigured) === 0) {
        this.mask_configure = 1;
        this.mask_store = 1;
        this.connect5();
      } else {
        let parsed_name = this.pname_name + ".XttMethodsMask.OpMethods";
        this.dyn.graph.getGdh().getObjectInfoInt(parsed_name).then(this.connect3);
      }
    } else {
      this.mask_configure = 1;
      this.connect5();
    }
  }

  connect3(res) {
    if (res.sts & 1) {
      this.xm_mask_opmethods = res.value;
    } else {
      console.log("DynMethodToolbar: " + this.pname_name);
      this.mask_configure = 1;
    }
    let parsed_name = this.pname_name + ".XttMethodsMask.MntMethods";
    this.dyn.graph.getGdh().getObjectInfoInt(parsed_name).then(this.connect4);
  }

  connect4(res) {
    if (res.sts & 1) {
      this.xm_mask_mntmethods = res.value;
    } else {
      console.log("DynMethodToolbar: " + this.pname_name);
      this.mask_configure = 1;
    }
    this.connect5();
  }

  connect5() {
    let cmd, command;
    let sts;

    if (this.mask_configure !== 0) {
      this.xm_mask_opmethods = 0;
      for (let i = 0; i < MethodToolbarOp.Cnt; i++) {
        if (MethodToolbarOp.Methods[i] === "") {
          continue;
        }
        if (MethodToolbarOp.Methods[i] === "Parent Object Graph") {
          if (this.method_object.indexOf('.') !== -1) {
            this.xm_mask_opmethods |= 1 << i;
          }
        } else {
          command =
              "check method/method=\"" + MethodToolbarOp.Methods[i] +
              "\"/object=" + this.method_object;

          command = this.dyn.graph.getCommand(command);
          sts = this.dyn.graph.command(command);
          if ((sts & 1) !== 0) {
            this.xm_mask_opmethods |= 1 << i;
          }
        }
      }
      this.xm_mask_mntmethods = 0;
      for (let i = 0; i < MethodToolbarMnt.Cnt; i++) {
        if (MethodToolbarMnt.Methods[i] === "") {
          continue;
        }
        command =
            "check method/method=\"" + MethodToolbarMnt.Methods[i] +
            "\"/object=" + this.method_object;

        command = this.dyn.graph.getCommand(command);
        sts = this.dyn.graph.command(command);
        if ((sts & 1) !== 0) {
          this.xm_mask_mntmethods |= 1 << i;
        }
      }
      this.xm_mask_flags |= XttMethodsFlagsMask.IsConfigured;
    }
    console.log("Method mask: " + this.xm_mask_opmethods + " " +
        this.xm_mask_mntmethods);

    let opmask = this.xm_mask_opmethods;
    let mntmask = this.xm_mask_mntmethods;
    let insensitive_opmask = 0;
    let insensitive_mntmask = 0;

    opmask &= ~XttOpMethodsMask.OpenObjectGraph;
    if (!this.dyn.graph.isAuthorized(Access.RtEventsBlock |
            Access.System)) {
      insensitive_opmask |= XttOpMethodsMask.BlockEvents;
    }
    if (!this.dyn.graph.isAuthorized(Access.RtNavigator |
            Access.System | Access.RtWrite | Access.Maintenance |
            Access.Process | Access.Instrument)) {
      insensitive_opmask |= XttOpMethodsMask.Note;
    }
    if (!this.dyn.graph.isAuthorized(Access.RtNavigator |
            Access.System)) {
      insensitive_mntmask |= XttMntMethodsMask.RtNavigator;
    }
    if (!this.dyn.graph.isAuthorized(Access.RtPlc | Access.System)) {
      insensitive_mntmask |= XttMntMethodsMask.OpenTrace;
    }

    if (this.toolbar_type === MethodToolbarType.Simulate) {
      opmask = 0;
    }

    this.o.configure(MethodToolbarOp.Subgraph,
        MethodToolbarMnt.Subgraph, MethodToolbarOp.Cnt,
        MethodToolbarMnt.Cnt, opmask, mntmask, insensitive_opmask,
        insensitive_mntmask);

    this.mask_store = 0; // Check call is not universal
    if (this.mask_store !== 0) {
      let parsed_name = this.pname_name + ".XttMethodsMask.OpMethods";
      let psts = this.dyn.graph.getGdh()
          .setObjectInfoInt(parsed_name, this.xm_mask_opmethods);
      if (even(psts)) {
        console.log("Set mask error " + parsed_name);
      }

      parsed_name = this.pname_name + ".XttMethodsMask.MntMethods";
      psts = this.dyn.graph.getGdh()
          .setObjectInfoInt(parsed_name, this.xm_mask_mntmethods);
      if (even(psts)) {
        console.log("Set mask error " + parsed_name);
      }

      parsed_name = this.pname_name + ".XttMethodsMask.Flags";
      psts = this.dyn.graph.getGdh()
          .setObjectInfoInt(parsed_name, this.xm_mask_flags);
      if (even(psts)) {
        console.log("Set mask error " + parsed_name);
      }
    }

    return 1;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
      case Event.MB1Up:
        object.setColorInverse(event === Event.MB1Down);
        break;
      case Event.MB1Click:
        let sts;
        let command;
        let idx = e.idx;
        let category = e.category;

        console.log("Toolbar action " + category + " " + idx);

        if (category === 1) {
          if (MethodToolbarOp.Methods[idx] === "Parent Object Graph") {
            command = "open graph/class/parent/instance=" + this.method_object;
          } else {
            command =
                "call method/method=\"" + MethodToolbarOp.Methods[idx] +
                "\"/object=" + this.method_object;
          }
        } else {
          command =
              "call method/method=\"" + MethodToolbarMnt.Methods[idx] +
              "\"/object=" + this.method_object;
        }

        command = this.dyn.graph.getCommand(command);
        sts = this.dyn.graph.command(command);

        break;
      default:
        return 0;
    }

    return 1;
  }
}

class DynMethodPulldownMenu extends DynElem {
  method_object;
  menu_type;
  opmask;
  mntmask;
  menu_object = null;
  text_size;
  o;

  pname_name;
  mask_configure = 0;
  mask_store = 0;
  xm_mask_flags = 0;
  xm_mask_opmethods = 0;
  xm_mask_mntmethods = 0;

  constructor(dyn) {
    super(dyn, DynPrio.MethodPulldownMenu);
    this.action_type1 = ActionType1.MethodPulldownMenu;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.MethodPulldownMenu:
          break;
        case DynSave.MethodPulldownMenu_method_object:
          if (tokens.length > 1) {
            this.method_object = tokens[1];
          }
          break;
        case DynSave.MethodPulldownMenu_menu_type:
          this.menu_type = parseInt(tokens[1], 10);
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynMethodPulldownMenu");
          break;
      }
    }

    return i;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    this.o = object;
    switch (e.event) {
      case Event.MB1Click:
        if (this.menu_object) {
          // Close, delete this menu
          this.dyn.graph.getCtx().remove(this.menu_object);
          this.menu_object = null;
        } else if (object) {
          let pname = this.dyn.parseAttrName(this.method_object);
          if (pname === null || pname.name === "") {
            return 1;
          }

          this.pname_name = pname.name;
          let parsed_name = this.pname_name + ".XttMethodsMask.Flags";

          this.dyn.graph.getGdh().getObjectInfoInt(parsed_name).then(this.action2);
        }

        break;
      case Event.MenuActivated:
        if (this.menu_object === null) {
          break;
        }
        if (e.object === this.menu_object) {
          let sts;
          let idx = 0;
          let found = 0;

          for (let i = 0; i < MethodToolbarOp.Cnt; i++) {
            if (this.menu_type !== MethodsMenuType.Help &&
                (MethodToolbarOp.Helpmask & (1 << i)) === 0) {
              continue;
            }
            if (this.menu_type !== MethodsMenuType.Simulate) {
              continue;
            }

            if ((this.opmask & (1 << i)) !== 0) {
              idx++;
            }
            if (e.item + 1 === idx) {
              let command =
                  "call method/method=\"" + MethodToolbarOp.Methods[i] +
                  "\"/object=" + this.method_object;
              command = this.dyn.graph.getCommand(command);
              sts = this.dyn.graph.command(command);
              found = 1;
              break;
            }
          }

          if (found === 0) {
            for (let i = 0; i < MethodToolbarMnt.Cnt; i++) {
              if (this.menu_type === MethodsMenuType.Help &&
                  (MethodToolbarMnt.Helpmask & (1 << i)) === 0) {
                continue;
              }

              if ((this.mntmask & (1 << i)) !== 0) {
                idx++;
              }
              if (e.item + 1 === idx) {
                let command =
                    "call method/method=\"" + MethodToolbarMnt.Methods[i] +
                    "\"/object=" + this.method_object;
                command = this.dyn.graph.getCommand(command);
                sts = this.dyn.graph.command(command);
                found = 1;
                break;
              }
            }
          }
        }
        break;
      case Event.MenuDelete:
        if (this.menu_object === null) {
          break;
        }
        if (e.object === null || e.object === this.menu_object) {
          // Delete this menu
          this.dyn.graph.getCtx().remove(this.menu_object);
          this.menu_object = null;
        }
        break;

      default:
    }

    return 1;
  }

  action2(res) {
    if (res.sts & 1) {
      this.xm_mask_flags = res.value;

      if ((this.xm_mask_flags & XttMethodsFlagsMask.IsConfigured) === 0) {
        this.mask_configure = 1;
        this.mask_store = 1;
        this.action5();
      } else {
        let parsed_name = this.pname_name + ".XttMethodsMask.OpMethods";

        this.dyn.graph.getGdh().getObjectInfoInt(parsed_name).then(this.action3);
      }
    } else {
      this.mask_configure = 1;
      this.action5();
    }
  }

  action3(res) {
    if (res.sts & 1) {
      this.xm_mask_opmethods = res.value;
    } else {
      console.log("DynMethodToolbar: " + this.pname_name);
      this.mask_configure = 1;
    }

    let parsed_name = this.pname_name + ".XttMethodsMask.MntMethods";
    this.dyn.graph.getGdh().getObjectInfoInt(parsed_name).then(this.action4);
  }

  action4(res) {
    if (res.sts & 1) {
      this.xm_mask_mntmethods = res.value;
    } else {
      console.log("DynMethodToolbar: " + this.pname_name);
      this.mask_configure = 1;
    }
    this.action5();
  }

  action5() {
    let command;
    let sts;

    if (this.mask_configure !== 0) {
      this.xm_mask_opmethods = 0;
      for (let i = 0; i < MethodToolbarOp.Cnt; i++) {
        if (MethodToolbarOp.Methods[i] === "Parent Object Graph") {
          if (this.method_object.indexOf('.') !== -1) {
            this.xm_mask_opmethods |= 1 << i;
          }
        } else {
          command =
              "check method/method=\"" + MethodToolbarOp.Methods[i] +
              "\"/object=" + this.method_object;

          command = this.dyn.graph.getCommand(command);
          sts = this.dyn.graph.command(command);
          if ((sts & 1) !== 0) {
            this.xm_mask_opmethods |= 1 << i;
          }
        }
      }
      this.xm_mask_mntmethods = 0;
      for (let i = 0; i < MethodToolbarMnt.Cnt; i++) {
        command =
            "check method/method=\"" + MethodToolbarMnt.Methods[i] +
            "\"/object=" + this.method_object;

        command = this.dyn.graph.getCommand(command);
        let sts = this.dyn.graph.command(command);
        if ((sts & 1) !== 0) {
          this.xm_mask_mntmethods |= 1 << i;
        }
      }
      this.xm_mask_flags |= XttMethodsFlagsMask.IsConfigured;
    }

    this.opmask = this.xm_mask_opmethods;
    this.mntmask = this.xm_mask_mntmethods;
    let insensitive_opmask = 0;
    let insensitive_mntmask = 0;
    let info = new GlowMenuInfo();

    this.opmask &= ~XttOpMethodsMask.OpenObjectGraph;
    if (!this.dyn.graph.isAuthorized(Access.RtEventsBlock |
            Access.System)) {
      insensitive_opmask |= XttOpMethodsMask.BlockEvents;
    }
    if (!this.dyn.graph.isAuthorized(Access.RtNavigator |
            Access.System | Access.RtWrite | Access.Maintenance |
            Access.Process | Access.Instrument)) {
      insensitive_opmask |= XttOpMethodsMask.Note;
    }
    if (!this.dyn.graph.isAuthorized(Access.RtNavigator |
            Access.System)) {
      insensitive_mntmask |= XttMntMethodsMask.RtNavigator;
    }
    if (!this.dyn.graph.isAuthorized(Access.RtPlc | Access.System)) {
      insensitive_mntmask |= XttMntMethodsMask.OpenTrace;
    }

    let menu_idx = 0;
    for (let i = 0; i < 32; i++) {
      info.item[i].occupied = false;
    }

    for (let i = 0; i < MethodToolbarOp.Cnt; i++) {
      if (menu_idx >= 32) {
        break;
      }

      if (this.menu_type === MethodsMenuType.Help &&
          (MethodToolbarOp.Helpmask & (1 << i)) === 0) {
        continue;
      }
      if (this.menu_type === MethodsMenuType.Simulate) {
        continue;
      }

      if ((this.opmask & (1 << i)) !== 0) {
        info.item[menu_idx].type = MenuItem.Button;
        info.item[menu_idx].text = String(MethodToolbarOp.Methods[i]);
        info.item[menu_idx].occupied = true;
        if ((insensitive_opmask & (1 << i)) !== 0) {
          info.item[menu_idx].type = MenuItem.ButtonDisabled;
        }
        menu_idx++;
      }
    }
    for (let i = 0; i < MethodToolbarMnt.Cnt; i++) {
      if (menu_idx >= 32) {
        break;
      }

      if (this.menu_type === MethodsMenuType.Help &&
          (MethodToolbarMnt.Helpmask & (1 << i)) === 0) {
        continue;
      }

      if ((this.mntmask & (1 << i)) !== 0) {
        info.item[menu_idx].type = MenuItem.Button;
        info.item[menu_idx].text = MethodToolbarMnt.Methods[i];
        info.item[menu_idx].occupied = true;
        if ((insensitive_mntmask & (1 << i)) !== 0) {
          info.item[menu_idx].type = MenuItem.ButtonDisabled;
        }
        menu_idx++;
      }
    }

    // Get fillcolor, and textattributes from object
    let text_drawtype = 0, text_color = 0, bg_color = 0;
    let text_size = 0;
    let scale = 1;
    let text_font = 0;

    sts = 0; // TODO sts = grow_GetObjectAnnotInfo( object, 1, &text_size, &text_drawtype, &text_color, &bg_color, &scale, &text_font);
    if ((sts & 1) === 0) {
      text_size = 3;
      text_drawtype = DrawType.TextHelvetica;
      text_font = Font.LucidaSans;
      text_color = DrawType.Line;
      bg_color = DrawType.LightGray;
    } else if (bg_color === DrawType.No ||
        bg_color === DrawType.Inherit) {
      bg_color = DrawType.LightGray;
    }

    let g = this.o.measure();
    this.menu_object = new GrowMenu(this.dyn.graph.getCtx(),
        "__Menu", info, g.ll_x, g.ur_y, g.width(), DrawType.Line,
        0, 1, 1, bg_color, text_size, text_drawtype,
        text_color, DrawType.MediumGray, text_font);
    this.menu_object.set_scale(scale, scale, 0, 0, ScaleType.LowerLeft);
    this.dyn.graph.getCtx().insert(this.menu_object);

    // grow_SetMenuInputFocus( menu_object, 1);
  }
}

class DynCatchSignal extends DynElem {
  signal_name = "";

  constructor(dyn) {
    super(dyn, DynPrio.CatchSignal);
    this.action_type1 = ActionType1.CatchSignal;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    if (e.event === Event.Signal && e.signal.signal_name === this.signal_name) {
      let e = new GlowEvent();
      e.event = Event.MB1Click;
      e.object = object;
      let sts = this.dyn.action(object, e);
      if (sts === GLOW__NO_PROPAGATE || sts === GLOW__TERMINATED || sts === GLOW__SUBTERMINATED) {
        return sts;
      }
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.CatchSignal:
          break;
        case DynSave.CatchSignal_signal_name:
          this.signal_name = tokens[1];
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynCatchSignal");
          break;
      }
    }

    return i;
  }
}

class DynEmitSignal extends DynElem {
  signal_name = "";
  global = 0;

  constructor(dyn) {
    super(dyn, DynPrio.EmitSignal);
    this.action_type1 = ActionType1.EmitSignal;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB1Down:
        object.setColorInverse(1);
        break;
      case Event.MB1Up:
        object.setColorInverse(0);
        break;
      case Event.Key_Return:
      case Event.MB1Click:
        if (this.global) {
          let command = "emit signal/signalname=" + this.signal_name;
          let cmd = this.dyn.graph.getCommand(command);
          this.dyn.graph.command(cmd);
        } else {
          //TODO:
          //this.dyn.graph.signalSend(this.signal_name);
        }
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.EmitSignal:
          break;
        case DynSave.EmitSignal_signal_name:
          this.signal_name = tokens[1];
          break;
        case DynSave.EmitSignal_global:
          this.global = Number(tokens[1]);
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynEmitSignal");
          break;
      }
    }

    return i;
  }
}

class DynPopupMenu extends DynElem {
  ref_object;

  constructor(dyn) {
    super(dyn, DynPrio.PopupMenu);
    this.action_type1 = ActionType1.PopupMenu;
  }

  action(object, e) {
    if (!this.dyn.graph.isAuthorized(this.dyn.access)) {
      return 1;
    }

    switch (e.event) {
      case Event.MB3Press:
        // TODO
        //this.dyn.graph.openPopupMenu( ref_object, e.x, e.y);
        break;
    }

    return 1;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case DynSave.PopupMenu:
          break;
        case DynSave.PopupMenu_ref_object:
          if (tokens.length > 1) {
            this.ref_object = tokens[1];
          }
          break;
        case DynSave.End:
          return i;
        default:
          console.log("Syntax error in DynPopupMenu");
          break;
      }
    }

    return i;
  }
}