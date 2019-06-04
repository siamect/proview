class GrowTrend extends GrowRect {
  y_max_value = new Array(TREND_MAX_CURVES);
  y_min_value = new Array(TREND_MAX_CURVES);
  x_max_value = new Array(TREND_MAX_CURVES);
  x_min_value = new Array(TREND_MAX_CURVES);
  vertical_lines;
  horizontal_lines;
  fill_curve;
  no_of_points;
  curve_width;
  curve_drawtype = new Array(TREND_MAX_CURVES);
  curve_fill_drawtype = new Array(TREND_MAX_CURVES);
  scan_time;
  curve = new Array(TREND_MAX_CURVES);
  curve_cnt = 1;
  userdata = null;
  display_x_mark1 = 0;
  display_x_mark2 = 0;
  display_y_mark1 = 0;
  display_y_mark2 = 0;
  x_mark1;
  x_mark2;
  y_mark1;
  y_mark2;
  mark1_color;
  mark2_color;

  constructor(ctx) {
    super(ctx);
    for (let i = 0; i < TREND_MAX_CURVES; i++) {
      this.curve[i] = null;
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowTrend:
          break;

        case GlowSave.GrowTrend_y_max_value_0:
          this.y_max_value[0] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTrend_y_max_value_1:
          this.y_max_value[1] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTrend_y_min_value_0:
          this.y_min_value[0] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTrend_y_min_value_1:
          this.y_min_value[1] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTrend_x_max_value_0:
          this.x_max_value[0] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTrend_x_max_value_1:
          this.x_max_value[1] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTrend_x_min_value_0:
          this.x_min_value[0] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTrend_x_min_value_1:
          this.x_min_value[1] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTrend_horizontal_lines:
          this.horizontal_lines = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTrend_vertical_lines:
          this.vertical_lines = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTrend_fill_curve:
          this.fill_curve = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTrend_curve_drawtype_0:
          this.curve_drawtype[0] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTrend_curve_drawtype_1:
          this.curve_drawtype[1] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTrend_curve_fill_drawtype_0:
          this.curve_fill_drawtype[0] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTrend_curve_fill_drawtype_1:
          this.curve_fill_drawtype[1] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTrend_no_of_points:
          this.no_of_points = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTrend_curve_width:
          this.curve_width = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTrend_scan_time:
          this.scan_time = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTrend_rect_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowTrend_trace_data1:
        case GlowSave.GrowTrend_trace_data2:
        case GlowSave.GrowTrend_trace_data3:
        case GlowSave.GrowTrend_trace_data4:
        case GlowSave.GrowTrend_trace_data5:
        case GlowSave.GrowTrend_trace_data6:
        case GlowSave.GrowTrend_trace_data7:
        case GlowSave.GrowTrend_trace_data8:
        case GlowSave.GrowTrend_trace_data9:
        case GlowSave.GrowTrend_trace_data10:
        case GlowSave.GrowTrend_trace_attr_type:
        case GlowSave.GrowTrend_trace_color:
        case GlowSave.GrowTrend_access:
        case GlowSave.GrowTrend_cycle:
        case GlowSave.GrowTrend_ref_object:
          break;
        case GlowSave.GrowTrend_userdata_cb:
          if (this.ctx.appl !== null) {
            let ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
                UserdataCbType.Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;

        case GlowSave.End:
          this.configure_curves();
          return i;
        default:
          console.log("Syntax error in GrowTrend", i, key);
          break;
      }
    }

    this.configure_curves();
    return i;
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let idx;
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

    let tmp = Matrix.multiply(t, this.trf);
    let p1 = tmp.apply(this.ll);
    let p2 = tmp.apply(this.ur);

    let x1 = Math.floor(p1.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y1 = Math.floor(p1.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let x2 = Math.floor(p2.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y2 = Math.floor(p2.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    let ll_x = Math.min(x1, x2);
    let ur_x = Math.max(x1, x2);
    let ll_y = Math.min(y1, y2);
    let ur_y = Math.max(y1, y2);

    if (this.fill) {
      let grad = this.gradient;
      if (this.gradient === Gradient.No &&
          (node !== null && node.gradient !== Gradient.No) &&
          this.disable_gradient === 0) {
        grad = node.gradient;
      }

      let drawtype =
          GlowColor.get_drawtype(this.fill_drawtype, DrawType.FillHighlight,
              highlight, colornode, 1, 0);
      if (grad === Gradient.No) {
        this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
            drawtype, true, 0);
      } else {
        let rotation = t ? this.trf.rotation + t.rotation : this.trf.rotation;

        let f1, f2;
        if (this.gradient_contrast >= 0) {
          f2 = GlowColor.shift_drawtype(drawtype, -this.gradient_contrast / 2,
              null);
          f1 = GlowColor.shift_drawtype(drawtype,
              Math.floor(this.gradient_contrast / 2 + 0.6), null);
        } else {
          f2 = GlowColor.shift_drawtype(drawtype,
              -Math.floor(this.gradient_contrast / 2 - 0.6), null);
          f1 = GlowColor.shift_drawtype(drawtype, this.gradient_contrast / 2,
              null);
        }
        this.ctx.gdraw.gradient_fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
            drawtype, f1, f2, this.ctx.gdraw.gradient_rotate(rotation, grad));
      }
    }
    let drawtype =
        GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
            highlight, colornode, 0, 0);

    let curvetmp = this.curve.slice(0, this.curve_cnt).filter(e => e !== null);

    if (this.fill_curve !== 0) {
      curvetmp.forEach(e => e.border = 0);
      curvetmp.forEach(e => e.draw(Matrix.multiply(t, this.trf), highlight, hot, node, colornode));
      curvetmp.forEach(e => e.border = 1);
    }

    for (let i = 0; i < this.vertical_lines; i++) {
      let x = Math.floor(ll_x + (ur_x - ll_x) / (this.vertical_lines + 1) *
          (i + 1));
      this.ctx.gdraw.line(x, ll_y, x, ur_y, drawtype, 0, 0);
    }

    for (let i = 0; i < this.horizontal_lines; i++) {
      let y = Math.floor(ll_y + (ur_y - ll_y) / (this.horizontal_lines + 1) *
          (i + 1));
      this.ctx.gdraw.line(ll_x, y, ur_x, y, drawtype, 0, 0);
    }

    if (this.fill_curve !== 0) {
      curvetmp.forEach(e => e.fill = 0);
    }
    curvetmp.forEach(e => e.draw(Matrix.multiply(t, this.trf), highlight, hot, node, colornode));
    if (this.fill_curve !== 0) {
      curvetmp.forEach(e => e.fill = 1);
    }

    let tmp = Matrix.multiply(t, this.trf);

    if (this.display_x_mark1 !== 0) {
      let p = tmp.apply(new Point(this.x_mark1, this.ll.y));
      let xm1 = Math.floor(p.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      if (xm1 >= ll_x && xm1 <= ur_x) {
        drawtype = this.mark1_color;
        if (drawtype === DrawType.Inherit) {
          drawtype = DrawType.ColorYellow;
        }
        this.ctx.gdraw.line(xm1, ll_y, xm1, ur_y, drawtype, idx, 0);
      }
    }
    if (this.display_x_mark2 !== 0) {
      let p = tmp.apply(new Point(this.x_mark2, this.ll.y));
      let xm2 = Math.floor(p.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      if (xm2 >= ll_x && xm2 <= ur_x) {
        drawtype = this.mark2_color;
        if (drawtype === DrawType.Inherit) {
          drawtype = DrawType.ColorYellow;
        }
        this.ctx.gdraw.line(xm2, ll_y, xm2, ur_y, drawtype, idx, 0);
      }
    }
    if (this.display_y_mark1 !== 0) {
      let p = tmp.apply(new Point(this.ll.x, this.y_mark1));
      let ym1 = Math.floor(p.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      if (ym1 >= ll_y && ym1 <= ur_y) {
        drawtype = this.mark1_color;
        if (drawtype === DrawType.Inherit) {
          drawtype = DrawType.ColorYellow;
        }
        this.ctx.gdraw.line(ll_x, ym1, ur_x, ym1, drawtype, idx, 0);
      }
    }
    if (this.display_y_mark2 !== 0) {
      let p = tmp.apply(new Point(this.ll.x, this.y_mark2));
      let ym2 = Math.floor(p.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      if (ym2 >= ll_y && ym2 <= ur_y) {
        drawtype = this.mark2_color;
        if (drawtype === DrawType.Inherit) {
          drawtype = DrawType.ColorYellow;
        }
        this.ctx.gdraw.line(ll_x, ym2, ur_x, ym2, drawtype, idx, 0);
      }
    }

    if (this.border !== 0) {
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, false, idx);
    }
  }

  set_range(curve, min, max) {
    if (!(curve === 0 || curve === 1)) {
      return;
    }
    this.y_max_value[curve] = max;
    this.y_min_value[curve] = min;
    this.configure_curves();
  }

  configure_curves() {
    this.curve_cnt = 2;

    this.no_of_points = Math.max(2, this.no_of_points);
    let points = this.no_of_points;
    if (this.fill_curve !== 0) {
      points += 2;
    }
    this.curve_width =
        Math.min(DRAW_TYPE_SIZE, Math.max(1, this.curve_width));

    let pointarray = new Array(points);
    for (let i = 0; i < points; i++) {
      pointarray[i] = new GlowPoint();
      let point_p = pointarray[i];
      if (this.fill_curve === 0) {
        point_p.y = this.ur.y;
        point_p.x = this.ur.x - i * (this.ur.x - this.ll.x) / (points - 1);
      } else {
        point_p.y = this.ur.y;
        if (i === 0) {
          point_p.x = this.ur.x;
        } else if (i === points - 1) {
          point_p.x = this.ll.x;
        } else {
          point_p.x =
              this.ur.x - (i - 1) * (this.ur.x - this.ll.x) / (points - 3);
        }
      }
    }

    for (let i = 0; i < this.curve_cnt; i++) {
      let dt = (this.curve_drawtype[i] === DrawType.Inherit) ? this.draw_type : this.curve_drawtype[i];
      let dt_fill = (this.curve_fill_drawtype[i] === DrawType.Inherit) ? this.draw_type : this.curve_fill_drawtype[i];

      this.ctx.nodraw++;
      this.curve[i] = new GrowPolyline(this.ctx, "", pointarray, points,
          dt, this.curve_width, 0, this.fill_curve, 1,
          0, dt_fill);
      this.ctx.nodraw--;
    }
  }

  set_range_y(curve, min, max) {
    if (!(curve === 0 || curve === 1)) {
      return;
    }

    if (curve === 0) {
      if (this.display_y_mark1 !== 0) {
        let mark = this.y_min_value[0] - (this.y_mark1 - this.ur.y) *
            (this.y_max_value[0] - this.y_min_value[0]) / (this.ur.y - this.ll.y);
        this.y_mark1 =
            this.ur.y - (mark - min) / (max - min) * (this.ur.y - this.ll.y);
      }
      if (this.display_y_mark2 !== 0) {
        let mark = this.y_min_value[0] - (this.y_mark2 - this.ur.y) *
            (this.y_max_value[0] - this.y_min_value[0]) / (this.ur.y - this.ll.y);
        this.y_mark2 =
            this.ur.y - (mark - min) / (max - min) * (this.ur.y - this.ll.y);
      }
    }

    this.y_max_value[curve] = max;
    this.y_min_value[curve] = min;
    this.configure_curves();
  }

  add_value(value, idx) {
    if (idx >= this.curve_cnt) {
      return;
    }

    let curve_value = 0;
    if (this.y_max_value[idx] !== this.y_min_value[idx]) {
      curve_value = this.ur.y - (value - this.y_min_value[idx]) /
          (this.y_max_value[idx] - this.y_min_value[idx]) *
          (this.ur.y - this.ll.y);
    }

    curve_value = Math.max(this.ll.y, Math.min(curve_value, this.ur.y));
    if (this.fill_curve === 0) {
      this.curve[idx].add_and_shift_y_value(curve_value);
    } else {
      this.curve[idx].add_and_shift_y_value_filled(curve_value);
    }
    this.draw();
  }

  set_scan_time(time) {
    this.scan_time = time;
    this.configure_curves();
  }

  get_scan_time() {
    return this.scan_time;
  }

  get_no_of_points() {
    return this.no_of_points;
  }

  set_x_mark1(mark) {
    this.display_x_mark1 = 1;
    this.x_mark1 = this.ll.x + (mark - this.x_min_value[0]) /
        (this.x_max_value[0] - this.x_min_value[0]) * (this.ur.x - this.ll.x);
    this.draw();
  }

  set_x_mark2(mark) {
    this.display_x_mark2 = 1;
    this.x_mark2 = this.ll.x + (mark - this.x_min_value[0]) /
        (this.x_max_value[0] - this.x_min_value[0]) * (this.ur.x - this.ll.x);
    this.draw();
  }

  set_y_mark1(mark) {
    this.display_y_mark1 = 1;
    this.y_mark1 = this.ur.y - (mark - this.y_min_value[0]) /
        (this.y_max_value[0] - this.y_min_value[0]) * (this.ur.y - this.ll.y);
    this.draw();
  }

  set_y_mark2(mark) {
    this.display_y_mark2 = 1;
    this.y_mark2 = this.ur.y - (mark - this.y_min_value[0]) /
        (this.y_max_value[0] - this.y_min_value[0]) * (this.ur.y - this.ll.y);
    this.draw();
  }

  set_mark_color(m1color, m2color) {
    this.mark1_color = m1color;
    this.mark2_color = m2color;
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