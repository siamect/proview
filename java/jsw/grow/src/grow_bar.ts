class GrowBar extends GrowRect {
  max_value;
  min_value;
  bar_value;
  bar_bordercolor;
  bar_borderwidth;
  bar_drawtype;
  userdata = null;

  constructor(ctx) {
    super(ctx);
  }

  open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowBar : " + lines[i]);
      }

      switch (key) {
        case GlowSave.GrowBar:
          break;
        case GlowSave.GrowBar_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowBar_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowBar_bar_value:
          this.bar_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowBar_bar_bordercolor:
          this.bar_bordercolor = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBar_bar_borderwidth:
          this.bar_borderwidth = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBar_bar_drawtype:
          this.bar_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBar_rect_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowBar_userdata_cb:
          if (this.ctx.appl !== null) {
            let ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
                UserdataCbType.Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case GlowSave.GrowBar_trace_data1:
        case GlowSave.GrowBar_trace_data2:
        case GlowSave.GrowBar_trace_attr_type:
        case GlowSave.GrowBar_trace_color:
        case GlowSave.GrowBar_trace_data3:
        case GlowSave.GrowBar_trace_data4:
        case GlowSave.GrowBar_trace_data5:
        case GlowSave.GrowBar_trace_data6:
        case GlowSave.GrowBar_trace_data7:
        case GlowSave.GrowBar_trace_data8:
        case GlowSave.GrowBar_trace_data9:
        case GlowSave.GrowBar_trace_data10:
        case GlowSave.GrowBar_access:
        case GlowSave.GrowBar_cycle:
        case GlowSave.GrowBar_ref_object:
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowBar");
          break;
      }
    }
    return i;
  }

  tdraw(t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let idx;
    let drawtype;
    let rotation;
    let grad = this.gradient;
    if (this.gradient === Gradient.No &&
        (node !== null && node.gradient !== Gradient.No) &&
        this.disable_gradient === 0) {
      grad = node.gradient;
    }

    let bar_border_idx = Math.floor(this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * this.bar_borderwidth - 1);
    bar_border_idx =
        Math.min(DRAW_TYPE_SIZE - 1, Math.max(0, bar_border_idx));

    if (node !== null && node.line_width !== 0) {
      idx =
          Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
              node.line_width - 1);
    } else {
      idx =
          Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
              this.line_width - 1);
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);
    let x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);
    if (this.fill !== 0) {
      drawtype =
          GlowColor.get_drawtype(this.fill_drawtype, DrawType.FillHighlight,
              highlight, colornode, 1, 0);

      if (grad === Gradient.No) {
        this.ctx.gdraw.fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
            drawtype);
      } else {
        let fa1, fa2;

        if (t === null) {
          rotation = this.trf.rot();
        } else {
          rotation = this.trf.rot(t);
        }
        if (this.gradient_contrast >= 0) {
          fa2 = GlowColor.shift_drawtype(drawtype, -this.gradient_contrast / 2,
              null);
          fa1 = GlowColor.shift_drawtype(drawtype,
              Math.floor((this.gradient_contrast) / 2 + 0.6), null);
        } else {
          fa2 = GlowColor.shift_drawtype(drawtype,
              -Math.floor((this.gradient_contrast) / 2 - 0.6), null);
          fa1 = GlowColor.shift_drawtype(drawtype, this.gradient_contrast / 2,
              null);
        }
        this.ctx.gdraw.gradient_fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
            drawtype, fa1, fa2, this.ctx.gdraw.gradient_rotate(rotation, grad));
      }
    }
    drawtype =
        GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
            highlight, colornode, 0, 0);

    if (this.max_value !== this.min_value) {
      let x0, y0, width, height, l_x0, l_y0, l_x1, l_y1;

      if (t === null) {
        rotation =
            (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) * 360;
      } else {
        rotation =
            (this.trf.rot(t) / 360 - Math.floor(this.trf.rot(t) / 360)) * 360;
      }
      if (45 >= rotation || rotation > 315) {
        height = Math.floor((this.bar_value - this.min_value) /
            (this.max_value - this.min_value) * (ur_y - ll_y));
        height = Math.max(0, Math.min(height, ur_y - ll_y));
        width = ur_x - ll_x;
        x0 = ll_x;
        y0 = ur_y - height;
        l_x0 = ll_x;
        l_y0 = ur_y - height;
        l_x1 = ur_x;
        l_y1 = ur_y - height;
      } else if (45 < rotation && rotation <= 135) {
        width = Math.floor((this.bar_value - this.min_value) /
            (this.max_value - this.min_value) * (ur_x - ll_x));
        width = Math.max(0, Math.min(width, ur_x - ll_x));
        height = ur_y - ll_y;
        x0 = ll_x;
        y0 = ll_y;
        l_x0 = ll_x + width;
        l_y0 = ll_y;
        l_x1 = ll_x + width;
        l_y1 = ur_y;
      } else if (135 < rotation && rotation <= 225) {
        height = Math.floor((this.bar_value - this.min_value) /
            (this.max_value - this.min_value) * (ur_y - ll_y));
        height = Math.max(0, Math.min(height, ur_y - ll_y));
        width = ur_x - ll_x;
        x0 = ll_x;
        y0 = ll_y;
        l_x0 = ll_x;
        l_y0 = ll_y + height;
        l_x1 = ur_x;
        l_y1 = ll_y + height;
      } else { // if ( 225 < rotation && rotation <= 315)
        width = Math.floor((this.bar_value - this.min_value) /
            (this.max_value - this.min_value) * (ur_x - ll_x));
        width = Math.max(0, Math.min(width, ur_x - ll_x));
        height = ur_y - ll_y;
        x0 = ur_x - width;
        y0 = ll_y;
        l_x0 = ur_x - width;
        l_y0 = ll_y;
        l_x1 = ur_x - width;
        l_y1 = ur_y;
      }

      let dt = drawtype;
      if (this.bar_drawtype !== DrawType.Inherit) {
        dt = this.bar_drawtype;
      }

      if (grad === Gradient.No) {
        this.ctx.gdraw.fill_rect(x0, y0, width, height, dt);
      } else {
        let fb1, fb2;

        if (t === null) {
          rotation = this.trf.rot();
        } else {
          rotation = this.trf.rot(t);
        }
        if (this.gradient_contrast >= 0) {
          fb2 = GlowColor.shift_drawtype(dt, -this.gradient_contrast / 2, null);
          fb1 = GlowColor.shift_drawtype(dt,
              Math.floor((this.gradient_contrast) / 2 + 0.6), null);
        } else {
          fb2 = GlowColor.shift_drawtype(dt,
              -Math.floor((this.gradient_contrast) / 2 - 0.6), null);
          fb1 = GlowColor.shift_drawtype(dt, this.gradient_contrast / 2, null);
        }
        this.ctx.gdraw.gradient_fill_rect(x0, y0, width, height, dt, fb1, fb2,
            this.ctx.gdraw.gradient_rotate(rotation, grad));
      }

      dt = drawtype;
      if (this.bar_bordercolor !== DrawType.Inherit) {
        dt = this.bar_bordercolor;
      }
      this.ctx.gdraw.line(l_x0, l_y0, l_x1, l_y1, dt, bar_border_idx, 0);
    }

    if (this.border !== 0) {
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, idx,
          0);
    }
  }

  set_range(min, max) {
    this.max_value = max;
    this.min_value = min;
    this.draw();
  }

  set_value(value) {
    this.bar_value = value;
    this.draw();
  }

  getClassDynType1() {
    return 0;
  }

  getClassDynType2() {
    return 0;
  }

  getClassActionType1() {
    return 0;
  }

  getClassActionType2() {
    return 0;
  }

  getClassCycle() {
    return 0;
  }

  getClassUserData() {
    return null;
  }

  getUserData() {
    return this.userdata;
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}