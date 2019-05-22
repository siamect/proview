class GrowRect extends GlowRect {
  trf: GlowTransform;
  n_name = null;
  ll_x: Number;
  ll_y: Number;
  ur_x: Number;
  ur_y: Number;
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

  constructor(ctx) {
    super(ctx);
    this.trf = new GlowTransform();
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowRect:
          break;
        case GlowSave.GrowRect_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowRect_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRect_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRect_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRect_y_low:
          this.ll_y = parseFloat(tokens[1]);
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
          i = super.open(lines, i + 1);
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

  event_handler(event, fx, fy) {
    if (event.event === Event.CursorMotion) {
      return 0;
    }

    let rp = this.trf.reverse(fx, fy);
    return (this.ll.x <= rp.x && rp.x <= this.ur.x && this.ll.y <= rp.y &&
        rp.y <= this.ur.y);
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0 || this.invisible === 0) {
      return;
    }

    if (this.invisible !== 0 && !(highlight !== 0 && node === null)) {
      return;
    }
    let chot = 0;

    if (this.fixcolor !== 0) {
      colornode = null;
    }

    let idx;
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

    let tmp = Matrix.multiply(t, this.trf);
    let p1 = tmp.apply(this.ll);
    let p2 = tmp.apply(this.ur);

    let x1 = p1.x * this.ctx.mw.zoom_factor_x + 0.5 - this.ctx.mw.offset_x;
    let y1 = p1.y * this.ctx.mw.zoom_factor_y + 0.5 - this.ctx.mw.offset_y;
    let x2 = p2.x * this.ctx.mw.zoom_factor_x + 0.5 - this.ctx.mw.offset_x;
    let y2 = p2.y * this.ctx.mw.zoom_factor_y + 0.5 - this.ctx.mw.offset_y;

    let ll_x = Math.min(x1, x2);
    let ur_x = Math.max(x1, x2);
    let ll_y = Math.min(y1, y2);
    let ur_y = Math.max(y1, y2);

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
      // Draw light shadow
      let drawtype_incr = (this.relief === Relief.Down) ? -this.shadow_contrast : this.shadow_contrast;
      let drawtype =
          GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot, colornode);

      let points = [
        new Point(ll_x, ll_y),
        new Point(ur_x, ll_y),
        new Point(ur_x - ish, ll_y + ish),
        new Point(ll_x + ish, ll_y + ish),
        new Point(ll_x + ish, ur_y - ish),
        new Point(ll_x, ur_y),
        new Point(ll_x, ll_y)
      ];
      this.ctx.gdraw.polyline(points, 7, drawtype, true, 0);

      // Draw dark shadow
      drawtype =
          GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot, colornode);

      let points = [
        new Point(ur_x, ur_y),
        new Point(ll_x, ur_y),
        new Point(ll_x + ish, ur_y - ish),
        new Point(ur_x - ish, ur_y - ish),
        new Point(ur_x - ish, ll_y + ish),
        new Point(ur_x, ll_y),
        new Point(ur_x, ur_y)
      ];
      this.ctx.gdraw.polyline(points, 7, drawtype, true, 0);
    }
    if (this.fill) {
      if (display_shadow && ish !== 0) {
        if (grad === Gradient.No || fillcolor === DrawType.ColorRed) {
          let drawtype = (chot === 0) ? fillcolor : GlowColor.shift_drawtype(fillcolor, chot, null);
          this.ctx.gdraw.rect(ll_x + ish, ll_y + ish, ur_x - ll_x - 2 *
              ish, ur_y - ll_y - 2 * ish, drawtype, true, 0);
        } else {
          let rotationa = t ? this.trf.rotation + t.rotation : this.trf.rotation;

          let fa0, fa1, fa2;
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
              ll_x - 2 * ish, ur_y - ll_y - 2 * ish, fa0, fa1, fa2, this.ctx.gdraw.gradient_rotate(rotationa, grad));
        }
      } else {
        if (grad === Gradient.No || fillcolor === DrawType.ColorRed) {
          let drawtype = (chot === 0) ? fillcolor : GlowColor.shift_drawtype(fillcolor, chot, null);
          this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
              drawtype, true, 0);
        } else {
          let rotationb = t ? this.trf.rotation + t.rotation : this.trf.rotation;

          let fb0, fb1, fb2;
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
              ll_y, fb0, fb1, fb2, this.ctx.gdraw.gradient_rotate(rotationb, grad));
        }
      }
    }
    if (this.border !== 0 ||
        !(this.fill || (display_shadow && this.shadow_width !== 0))) {
      let drawtype =
          GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
              highlight, colornode, 0, 0);
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, false, idx);
    }
  }

  get_borders(t, g) {
    let tmp = Matrix.multiply(t, this.trf);
    let p1 = tmp.apply(this.ll);
    let p2 = tmp.apply(this.ur);

    g.set(Rect.union(g, new Rect(Math.min(p1.x, p2.x), Math.min(p1.y, p2.y),
        Math.max(p1.x, p2.x), Math.max(p1.y, p2.y))));
  }

  get_node_borders() {
    let g = new Rect(1.0e37, 1.0e37, -1.0e37, -1.0e37);
    this.get_borders(null, g);
    this.ll_x = g.ll_x;
    this.ll_y = g.ll_y;
    this.ur_x = g.ur_x;
    this.ur_y = g.ur_y;
  }

  set_scale(scale_x, scale_y, x0, y0, type) {
    if (this.trf.s.a11 !== 0 && this.trf.s.a22 !== 0 &&
        Math.abs(scale_x - this.trf.a11 / this.trf.s.a11) < Number.MIN_VALUE &&
        Math.abs(scale_y - this.trf.a22 / this.trf.s.a22) < Number.MIN_VALUE) {
      return;
    }

    switch (type) {
      case ScaleType.LowerLeft:
        x0 = this.ll_x;
        y0 = this.ll_y;
        break;
      case ScaleType.LowerRight:
        x0 = this.ur_x;
        y0 = this.ll_y;
        break;
      case ScaleType.UpperRight:
        x0 = this.ur_x;
        y0 = this.ur_y;
        break;
      case ScaleType.UpperLeft:
        x0 = this.ll_x;
        y0 = this.ur_y;
        break;
      case ScaleType.FixPoint:
        break;
      case ScaleType.Center:
        x0 = (this.ll_x + this.ur_x) / 2;
        y0 = (this.ll_y + this.ur_y) / 2;
        break;
      default:
    }

    let old_x_left = this.ll_x;
    let old_x_right = this.ur_x;
    let old_y_low = this.ll_y;
    let old_y_high = this.ur_y;
    this.trf.revert();
    this.trf.scale(scale_x, scale_y, x0, y0);
    this.get_node_borders();

    switch (type) {
      case ScaleType.LowerLeft:
        this.ll_x = old_x_left;
        this.ll_y = old_y_low;
        break;
      case ScaleType.LowerRight:
        this.ur_x = old_x_right;
        this.ll_y = old_y_low;
        break;
      case ScaleType.UpperRight:
        this.ur_x = old_x_right;
        this.ur_y = old_y_high;
        break;
      case ScaleType.UpperLeft:
        this.ll_x = old_x_left;
        this.ur_y = old_y_high;
        break;
      case ScaleType.FixPoint:
        break;
      case ScaleType.Center:
        x0 = (this.ll_x + this.ur_x) / 2;
        y0 = (this.ll_y + this.ur_y) / 2;
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