class GlowNodeClass {
  ctx: GrowCtx;
  a: GlowArray;
  nc_name = null;
  group = 0;
  slider = 0;
  next_nodeclass = null;
  animation_count = 0;
  cycle = 0;
  x0 = 0;
  y0 = 0;
  x1 = 0;
  y1 = 0;
  input_focus_mark = 0;
  dynamicsize = 0;
  userdata = null;
  dyn_type1 = 0;
  dyn_type2 = 0;
  action_type1 = 0;
  action_type2 = 0;
  dyn_color = new Array(4);
  dyn_attr = [];
  next_nc = null;
  prev_nc = null;
  recursive_trace = 0;

  constructor(ctx) {
    this.ctx = ctx;
    this.a = new GlowArray(ctx);
  }

  open(lines, row) {
    let end = false;
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.NodeClass:
          break;
        case GlowSave.NodeClass_nc_name:
          this.nc_name = tokens[1];
          break;
        case GlowSave.NodeClass_a:
          i = this.a.open(lines, i + 1);
          break;
        case GlowSave.NodeClass_group:
          this.group = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_arg_cnt:
        case GlowSave.NodeClass_argname:
        case GlowSave.NodeClass_argtype:
        case GlowSave.NodeClass_no_con_obstacle:
          break;
        case GlowSave.NodeClass_dyn_type1:
          this.dyn_type1 = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_dyn_type2:
          this.dyn_type2 = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_dyn_action_type1:
          this.action_type1 = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_dyn_action_type2:
          this.action_type2 = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_dyn_color1:
          this.dyn_color[0] = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_dyn_color2:
          this.dyn_color[1] = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_dyn_color3:
          this.dyn_color[2] = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_dyn_color4:
          if (tokens.length > 1) {
            this.dyn_color[3] = parseInt(tokens[1], 10);
          }
          break;
        case GlowSave.NodeClass_dyn_attr1:
          if (tokens.length > 1) {
            this.dyn_attr[0] = parseInt(tokens[1], 10);
          }
          break;
        case GlowSave.NodeClass_dyn_attr2:
          if (tokens.length > 1) {
            this.dyn_attr[1] = parseInt(tokens[1], 10);
          }
          break;
        case GlowSave.NodeClass_dyn_attr3:
          if (tokens.length > 1) {
            this.dyn_attr[2] = parseInt(tokens[1], 10);
          }
          break;
        case GlowSave.NodeClass_dyn_attr4:
          if (tokens.length > 1) {
            this.dyn_attr[3] = parseInt(tokens[1], 10);
          }
          break;
        case GlowSave.NodeClass_java_name:
          break;
        case GlowSave.NodeClass_slider:
          this.slider = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_next_nodeclass:
          if (tokens.length > 1) {
            this.next_nodeclass = tokens[1];
          }
          break;
        case GlowSave.NodeClass_animation_count:
          this.animation_count = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_cycle:
          this.cycle = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_y0:
          this.y0 = parseFloat(tokens[1]);
          break;
        case GlowSave.NodeClass_y1:
          this.y1 = parseFloat(tokens[1]);
          break;
        case GlowSave.NodeClass_x0:
          this.x0 = parseFloat(tokens[1]);
          break;
        case GlowSave.NodeClass_x1:
          this.x1 = parseFloat(tokens[1]);
          break;
        case GlowSave.NodeClass_input_focus_mark:
          this.input_focus_mark = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_recursive_trace:
          this.recursive_trace = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case GlowSave.NodeClass_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case GlowSave.NodeClass_userdata_cb:
          if (this.ctx.appl !== null) {
            let ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
                UserdataCbType.NodeClass);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case GlowSave.End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowNodeClass");
          break;
      }
      if (end) {
        break;
      }
    }

    if (this.next_nodeclass !== null && this.next_nodeclass !== "") {
      this.next_nc = this.ctx.get_nodeclass_from_name(this.next_nodeclass);
      if (this.next_nc === null) {
        console.log("GlowNode:next_nodeclass not found: ", this.nc_name,
            this.next_nodeclass);
        console.log("Note! nodeclass has to be intern");
      } else if (this.next_nc.prev_nc !== null) {
        this.next_nc = null;
        console.log("GlowNode:next_nodeclass already chained: ", this.nc_name,
            this.next_nodeclass);
      } else {
        this.next_nc.prev_nc = this;
      }
    }

    return i;
  }

  event_handler(event, fx, fy) {
    for (let i = 0; i < this.a.size(); i++) {
      let sts = this.a.get(i).event_handler(event, fx, fy);
      if (sts !== 0) {
        return sts;
      }
    }
    return 0;
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    this.a.forEach(e => e.draw(t, highlight, hot, node, colornode));
  }

  get_borders(t, g) {
    let base = this.get_base_nc();

    if ((t === null ||
            (t !== null && Math.abs(t.rotation / 90 - Math.floor(t.rotation / 90)) <
                Number.MIN_VALUE)) &&
        !(Math.abs(base.x0 - base.x1) < Number.MIN_VALUE ||
            Math.abs(base.y0 - base.y1) < Number.MIN_VALUE)) {
      // Borders are given i x0, y0, x1, y1
      // Will not work in rotated nodes
      let kx1, kx2, ky1, ky2;

      if (t === null) {
        kx1 = base.x0;
        kx2 = base.x1;
        ky1 = base.y0;
        ky2 = base.y1;
      } else {
        kx1 = t.x(base.x0, base.y0);
        kx2 = t.x(base.x1, base.y1);
        ky1 = t.y(base.x0, base.y0);
        ky2 = t.y(base.x1, base.y1);
      }

      g.set(Rect.union(g, new Rect(Math.min(kx1, kx2), Math.min(ky1, ky2), Math.max(kx1, kx2), Math.max(ky1, ky2))));
    } else {
      this.a.forEach(e => e.get_borders(t, g));
    }
  }

  get_base_nc() {
    let base;
    for (base = this; base.prev_nc !== null; base = base.prev_nc) {
    }
    return base;
  }

  getAnnotationTextExtent(t, node, num) {
    for (let i = 0; i < this.a.size(); i++) {
      if (this.a.get(i) instanceof GrowAnnot &&
          this.a.get(i).getNumber() === num) {
        let p = this.a.get(i).getTextExtent(t, node);
        p.x /= this.ctx.mw.zoom_factor_x;
        p.y /= this.ctx.mw.zoom_factor_y;
        return p;
      }
    }
    return null;
  }
}