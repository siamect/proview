class GrowRect {
  ctx: GrowCtx;
  trf: GlowTransform;
  ll: GlowPoint;
  ur: GlowPoint;
  n_name = null;
  x_right = 0;
  x_left = 0;
  y_high = 0;
  y_low = 0;
  original_border_drawtype = 0;
  original_fill_drawtype = 0;
  fill_drawtype = 0;
  background_drawtype = DrawType.No;
  border = 0;
  shadow_width = 5;
  shadow_contrast = 2;
  shadow = 0;
  relief = Relief.Up;
  disable_shadow = 0;
  invisible = 0;
  fixcolor = 0;
  fixposition = 0;
  gradient = Gradient.No;
  gradient_contrast = 4;
  disable_gradient = 0;
  bgcolor_gradient = 0;
  fill_eq_background = 0;
  dynamicsize = 0;

  draw_type = 0;
  line_width = 0;
  display_level = 0;
  fill = 0;

  constructor(ctx) {
    this.ctx = ctx;
    this.trf = new GlowTransform();
    this.ll = new GlowPoint();
    this.ur = new GlowPoint();
  }

  open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowRect : " + lines[i]);
      }

      switch (key) {
        case GlowSave.GrowRect:
          break;
        case GlowSave.GrowRect_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowRect_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRect_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRect_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRect_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRect_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_original_fill_drawtype:
          this.original_fill_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_fill_drawtype:
          this.fill_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_background_drawtype:
          this.background_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_shadow_width:
          this.shadow_width = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRect_shadow_contrast:
          this.shadow_contrast = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_relief:
          this.relief = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_disable_shadow:
          this.disable_shadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_invisible:
          this.invisible = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_fixcolor:
          this.fixcolor = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_fixposition:
          this.fixposition = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_gradient:
          this.gradient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_gradient_contrast:
          this.gradient_contrast = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_disable_gradient:
          this.disable_gradient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_bgcolor_gradient:
          this.bgcolor_gradient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_fill_eq_background:
          this.fill_eq_background = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRect_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case GlowSave.GrowRect_rect_part:
          i = this.open_rect(lines, i + 1);
          break;
        case GlowSave.GrowRect_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowRect");
          break;
      }
    }
    return i;
  }

  open_rect(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowRect : " + lines[i]);
      }

      switch (key) {
        case GlowSave.Rect:
          break;
        case GlowSave.Rect_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Rect_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case GlowSave.Rect_display_level:
          this.display_level = parseInt(tokens[1], 10);
          break;
        case GlowSave.Rect_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case GlowSave.Rect_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case GlowSave.Rect_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowRect");
          break;
      }
    }
    return i;
  }

  event_handler(event, fx, fy) {
    switch (event.event) {
      case Event.CursorMotion:
        return 0;
      default:
        break;
    }

    let rp = this.trf.reverse(fx, fy);

    if (this.ll.x <= rp.x && rp.x <= this.ur.x && this.ll.y <= rp.y &&
        rp.y <= this.ur.y) {
      return 1;
    } else {
      return 0;
    }
  }

  draw() {
    if (this.invisible !== 0) {
      this.tdraw(null, 0, 0, null, null);
    }
  }

  tdraw(t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    if (this.invisible !== 0 && !(highlight !== 0 && node === null)) {
      return;
    }
    let chot = 0;
    let idx;
    let drawtype;
    if (this.fixcolor !== 0) {
      colornode = null;
    }

    if (node !== null && node.line_width !== 0) {
      idx = this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1;
    } else {
      idx = this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1;
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);
    let x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

    if (t === null) {
      x1 = this.trf.x(this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_x + 0.5 -
          this.ctx.mw.offset_x;
      y1 = this.trf.y(this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_y + 0.5 -
          this.ctx.mw.offset_y;
      x2 = this.trf.x(this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_x + 0.5 -
          this.ctx.mw.offset_x;
      y2 = this.trf.y(this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_y + 0.5 -
          this.ctx.mw.offset_y;
    } else {
      x1 =
          this.trf.x(t, this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_x + 0.5 -
          this.ctx.mw.offset_x;
      y1 =
          this.trf.y(t, this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_y + 0.5 -
          this.ctx.mw.offset_y;
      x2 =
          this.trf.x(t, this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_x + 0.5 -
          this.ctx.mw.offset_x;
      y2 =
          this.trf.y(t, this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_y + 0.5 -
          this.ctx.mw.offset_y;
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    let ish = this.shadow_width / 100 * Math.min(ur_x - ll_x, ur_y - ll_y) +
        0.5;
    let display_shadow = ((node !== null && node.shadow !== 0) ||
        this.shadow !== 0) && this.disable_shadow === 0;
    let fillcolor;
    if (this.fill_eq_background === 0) {
      fillcolor =
          GlowColor.get_drawtype(this.fill_drawtype, DrawType.FillHighlight,
              highlight, colornode, 1, 0);
    } else {
      fillcolor = GlowColor.get_drawtype(this.background_drawtype,
          DrawType.FillHighlight, highlight, colornode, 3, 0);
    }

    let grad = this.gradient;
    if (this.gradient === Gradient.No &&
        (node !== null && node.gradient !== Gradient.No) &&
        this.disable_gradient === 0) {
      grad = node.gradient;
    }

    if (display_shadow && ish !== 0) {
      let points = new Array(7);
      for (let i = 0; i < 7; i++) {
        points[i] = new GlowPoint();
      }

      // Draw light shadow
      let drawtype_incr = this.shadow_contrast;
      if (this.relief === Relief.Down) {
        drawtype_incr = -this.shadow_contrast;
      }

      drawtype =
          GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot, colornode);

      points[0].x = ll_x;
      points[0].y = ll_y;
      points[1].x = ur_x;
      points[1].y = ll_y;
      points[2].x = ur_x - ish;
      points[2].y = ll_y + ish;
      points[3].x = ll_x + ish;
      points[3].y = ll_y + ish;
      points[4].x = ll_x + ish;
      points[4].y = ur_y - ish;
      points[5].x = ll_x;
      points[5].y = ur_y;
      points[6].x = ll_x;
      points[6].y = ll_y;
      this.ctx.gdraw.fill_polyline(points, 7, drawtype, 0);

      // Draw dark shadow
      drawtype =
          GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot, colornode);

      points[0].x = ur_x;
      points[0].y = ur_y;
      points[1].x = ll_x;
      points[1].y = ur_y;
      points[2].x = ll_x + ish;
      points[2].y = ur_y - ish;
      points[3].x = ur_x - ish;
      points[3].y = ur_y - ish;
      points[4].x = ur_x - ish;
      points[4].y = ll_y + ish;
      points[5].x = ur_x;
      points[5].y = ll_y;
      points[6].x = ur_x;
      points[6].y = ur_y;
      this.ctx.gdraw.fill_polyline(points, 7, drawtype, 0);
    }
    if (this.fill !== 0) {
      if (display_shadow && ish !== 0) {
        if (grad === Gradient.No || fillcolor === DrawType.ColorRed) {
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }
          this.ctx.gdraw.fill_rect(ll_x + ish, ll_y + ish, ur_x - ll_x - 2 *
              ish, ur_y - ll_y - 2 * ish, drawtype);
        } else {
          let fa0, fa1, fa2;
          let rotationa;
          if (t === null) {
            rotationa = this.trf.rot();
          } else {
            rotationa = this.trf.rot(t);
          }

          if (this.bgcolor_gradient !== 0 &&
              this.background_drawtype !== DrawType.No) {
            fa2 = fillcolor;
            fa1 = GlowColor.get_drawtype(this.background_drawtype,
                DrawType.FillHighlight, highlight, colornode, 1, 0);
            fa0 = fa1;
          } else if (this.gradient_contrast >= 0) {
            fa2 =
                GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                    0.5 + chot, null);
            fa1 =
                GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                    0.6 + chot, null);
            fa0 = fillcolor;
          } else {
            fa2 =
                GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 -
                    0.6 + chot, null);
            fa1 =
                GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                    chot, null);
            fa0 = fillcolor;
          }
          this.ctx.gdraw.gradient_fill_rect(ll_x + ish, ll_y + ish, ur_x -
              ll_x - 2 * ish, ur_y - ll_y - 2 * ish, fa0, fa1, fa2,
              this.ctx.gdraw.gradient_rotate(rotationa, grad));
        }
      } else {
        if (grad === Gradient.No || fillcolor === DrawType.ColorRed) {
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }
          this.ctx.gdraw.fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
              drawtype);
        } else {
          let fb0, fb1, fb2;
          let rotationb;
          if (t === null) {
            rotationb = this.trf.rot();
          } else {
            rotationb = this.trf.rot(t);
          }
          if (this.bgcolor_gradient !== 0 &&
              this.background_drawtype !== DrawType.No) {
            fb2 = fillcolor;
            fb1 = GlowColor.get_drawtype(this.background_drawtype,
                DrawType.FillHighlight, highlight, colornode, 1, 0);
            fb0 = fb1;
          } else if (this.gradient_contrast >= 0) {
            fb2 =
                GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                    0.5 + chot, null);
            fb1 =
                GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                    0.6 + chot, null);
            fb0 = fillcolor;
          } else {
            fb2 =
                GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 -
                    0.6 + chot, null);
            fb1 =
                GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2,
                    null);
            fb0 = fillcolor;
          }
          this.ctx.gdraw.gradient_fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y -
              ll_y, fb0, fb1, fb2,
              this.ctx.gdraw.gradient_rotate(rotationb, grad));
        }
      }
    }
    if (this.border !== 0 ||
        !(this.fill !== 0 || (display_shadow && this.shadow_width !== 0))) {
      drawtype =
          GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
              highlight, colornode, 0, 0);
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, idx,
          0);
    }
  }

  get_borders(t, g) {
    let ll_x, ur_x, ll_y, ur_y, x1, x2, y1, y2;

    if (t === null) {
      x1 = this.trf.x(this.ll.x, this.ll.y);
      x2 = this.trf.x(this.ur.x, this.ur.y);
      y1 = this.trf.y(this.ll.x, this.ll.y);
      y2 = this.trf.y(this.ur.x, this.ur.y);
    } else {
      x1 = this.trf.x(t, this.ll.x, this.ll.y);
      x2 = this.trf.x(t, this.ur.x, this.ur.y);
      y1 = this.trf.y(t, this.ll.x, this.ll.y);
      y2 = this.trf.y(t, this.ur.x, this.ur.y);
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    if (ll_x < g.ll_x) {
      g.ll_x = ll_x;
    }
    if (ur_x > g.ur_x) {
      g.ur_x = ur_x;
    }
    if (ll_y < g.ll_y) {
      g.ll_y = ll_y;
    }
    if (ur_y > g.ur_y) {
      g.ur_y = ur_y;
    }
    return g;
  }

  get_node_borders() {
    let g = new Rect();
    g.x = 1.0e37;
    g.y = 1.0e37;
    g.width = -1.0e37;
    g.height = -1.0e37;
    this.get_borders(null, g);
    this.x_left = g.x;
    this.x_right = g.x + g.width;
    this.y_low = g.y;
    this.y_high = g.y + g.height;
  }

  set_scale(scale_x, scale_y, x0, y0, type) {
    let old_x_left, old_x_right, old_y_low, old_y_high;

    if (this.trf.s_a11 !== 0 && this.trf.s_a22 !== 0 &&
        Math.abs(scale_x - this.trf.a11 / this.trf.s_a11) < Number.MIN_VALUE &&
        Math.abs(scale_y - this.trf.a22 / this.trf.s_a22) < Number.MIN_VALUE) {
      return;
    }

    switch (type) {
      case ScaleType.LowerLeft:
        x0 = this.x_left;
        y0 = this.y_low;
        break;
      case ScaleType.LowerRight:
        x0 = this.x_right;
        y0 = this.y_low;
        break;
      case ScaleType.UpperRight:
        x0 = this.x_right;
        y0 = this.y_high;
        break;
      case ScaleType.UpperLeft:
        x0 = this.x_left;
        y0 = this.y_high;
        break;
      case ScaleType.FixPoint:
        break;
      case ScaleType.Center:
        x0 = (this.x_left + this.x_right) / 2;
        y0 = (this.y_low + this.y_high) / 2;
        break;
      default:
    }

    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;
    this.trf.scale_from_stored(scale_x, scale_y, x0, y0);
    this.get_node_borders();

    switch (type) {
      case ScaleType.LowerLeft:
        this.x_left = old_x_left;
        this.y_low = old_y_low;
        break;
      case ScaleType.LowerRight:
        this.x_right = old_x_right;
        this.y_low = old_y_low;
        break;
      case ScaleType.UpperRight:
        this.x_right = old_x_right;
        this.y_high = old_y_high;
        break;
      case ScaleType.UpperLeft:
        this.x_left = old_x_left;
        this.y_high = old_y_high;
        break;
      case ScaleType.FixPoint:
        break;
      case ScaleType.Center:
        x0 = (this.x_left + this.x_right) / 2;
        y0 = (this.y_low + this.y_high) / 2;
        break;
      default:
    }
    this.ctx.draw();
  }

  getUserData() {
    return null;
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

  getClassCycle(): Cycle {
    return Cycle.Slow;
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}