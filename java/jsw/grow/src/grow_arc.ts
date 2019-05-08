class GrowArc {
  ctx: GrowCtx;
  trf: GlowTransform;
  ll: GlowPoint;
  ur: GlowPoint;
  n_name = null;
  x_right;
  x_left;
  y_high;
  y_low;
  original_border_drawtype;
  original_fill_drawtype;
  fill_drawtype;
  background_drawtype = DrawType.No;
  border;
  shadow_width;
  shadow_contrast;
  shadow;
  relief;
  disable_shadow;
  fixcolor;
  fixposition;
  gradient;
  gradient_contrast;
  disable_gradient;
  fill_eq_background;
  dynamicsize;

  draw_type;
  angle1;
  angle2;
  line_width;
  display_level;
  fill;

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
        console.log("GrowArc : " + lines[i]);
      }

      switch (key) {
        case GlowSave.GrowArc:
          break;
        case GlowSave.GrowArc_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowArc_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowArc_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowArc_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowArc_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowArc_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_original_fill_drawtype:
          this.original_fill_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_fill_drawtype:
          this.fill_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_background_drawtype:
          this.background_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_shadow_width:
          this.shadow_width = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowArc_shadow_contrast:
          this.shadow_contrast = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_relief:
          this.relief = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_disable_shadow:
          this.disable_shadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_fixcolor:
          this.fixcolor = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_fixposition:
          this.fixposition = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_gradient:
          this.gradient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_gradient_contrast:
          this.gradient_contrast = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_disable_gradient:
          this.disable_gradient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_fill_eq_background:
          this.fill_eq_background = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowArc_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case GlowSave.GrowArc_arc_part:
          i = this.glowarc_open(lines, i + 1);
          break;
        case GlowSave.GrowArc_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowArc");
          break;
      }
    }
    return i;
  }

  glowarc_open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowArc : " + lines[i]);
      }

      switch (key) {
        case GlowSave.Arc:
          break;
        case GlowSave.Arc_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Arc_angle1:
          this.angle1 = parseInt(tokens[1], 10);
          break;
        case GlowSave.Arc_angle2:
          this.angle2 = parseInt(tokens[1], 10);
          break;
        case GlowSave.Arc_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case GlowSave.Arc_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case GlowSave.Arc_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case GlowSave.Arc_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowArc");
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
    this.tdraw(null, 0, 0, null, null);
  }

  tdraw(t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let drawtype;
    let idx;
    let chot = 0;

    if (this.ctx.hot_indication === HotIndication.No) {
      hot = 0;
    } else if (this.ctx.hot_indication === HotIndication.DarkColor) {
      chot = hot;
      hot = 0;
    } else if (this.ctx.hot_indication === HotIndication.LightColor) {
      chot = -hot;
      hot = 0;
    }

    if (this.fixcolor !== 0) {
      colornode = null;
    }

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
    let x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y, rot;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot());
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot(t));
    }

    if (rot % 90 !== 0 &&
        Math.abs((this.ur.x - this.ll.x) - (this.ur.y - this.ll.y)) <
        Number.MIN_VALUE) {
      let scale;
      let x_c;
      let y_c;
      if (t === null) {
        scale = this.trf.vertical_scale(this.trf);
        x_c = ((this.trf.x(this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_x -
            this.ctx.mw.offset_x) +
            (this.trf.x(this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_x -
                this.ctx.mw.offset_x)) / 2;
        y_c = ((this.trf.y(this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_y -
            this.ctx.mw.offset_y) +
            (this.trf.y(this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_y -
                this.ctx.mw.offset_y)) / 2;
      } else {
        let tmp = t.multiply(this.trf);
        scale = this.trf.vertical_scale(tmp);
        x_c = ((this.trf.x(t, this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_x -
            this.ctx.mw.offset_x) +
            (this.trf.x(t, this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_x -
                this.ctx.mw.offset_x)) / 2;
        y_c = ((this.trf.y(t, this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_y -
            this.ctx.mw.offset_y) +
            (this.trf.y(t, this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_y -
                this.ctx.mw.offset_y)) / 2;
      }

      x1 = Math.floor(-scale *
          ((this.ur.x - this.ll.x) / 2 * this.ctx.mw.zoom_factor_x) + x_c + 0.5);
      y1 = Math.floor(-scale *
          ((this.ur.y - this.ll.y) / 2 * this.ctx.mw.zoom_factor_y) + y_c + 0.5);
      x2 = Math.floor(scale *
          ((this.ur.x - this.ll.x) / 2 * this.ctx.mw.zoom_factor_x) + x_c + 0.5);
      y2 = Math.floor(scale *
          ((this.ur.y - this.ll.y) / 2 * this.ctx.mw.zoom_factor_y) + y_c + 0.5);
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    if (this.fill !== 0) {
      let display_shadow = ((node !== null && node.shadow !== 0) ||
          this.shadow !== 0) && this.disable_shadow === 0;
      let fillcolor;
      if (this.fill_eq_background === 0) {
        fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
            DrawType.FillHighlight, highlight, colornode, 1, 0);
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

      if (!display_shadow || this.shadow_width === 0 || this.angle2 !== 360) {
        if (grad === Gradient.No || fillcolor === DrawType.ColorRed) {
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }
          this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y -
              ll_y, this.angle1 - rot, this.angle2, drawtype);
        } else {
          let fa1, fa2;
          if (this.gradient_contrast >= 0) {
            fa2 =
                GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                    chot, null);
            fa1 = GlowColor.shift_drawtype(
                fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
                null);
          } else {
            fa2 = GlowColor.shift_drawtype(
                fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
                null);
            fa1 =
                GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                    chot, null);
          }
          this.ctx.gdraw.gradient_fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y -
              ll_y, this.angle1 - rot, this.angle2, fillcolor, fa1, fa2, grad);
        }
      } else {
        let ish = Math.floor(this.shadow_width / 100 *
            Math.min(ur_x - ll_x, ur_y - ll_y) + 0.5);
        let drawtype_incr = this.shadow_contrast;
        if (this.relief === Relief.Down) {
          drawtype_incr = -this.shadow_contrast;
        }

        if (grad === Gradient.No || fillcolor === DrawType.ColorRed) {
          // Draw light shadow
          drawtype = GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot,
              colornode);

          this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, 35, 140,
              drawtype);

          // Draw dark shadow
          drawtype = GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot,
              colornode);

          this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, 215,
              140, drawtype);

          // Draw medium shadow and body
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }

          this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, -5, 40,
              drawtype);
          this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, 175, 40,
              drawtype);

          this.ctx.gdraw.fill_arc(ll_x + ish, ll_y + ish, ur_x - ll_x - 2 *
              ish, ur_y - ll_y - 2 * ish, this.angle1 - rot, this.angle2,
              drawtype);
        } else {
          let fb1, fb2;

          // Draw shadow
          fb1 = GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot,
              colornode);
          fb2 = GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot,
              colornode);

          this.ctx.gdraw.gradient_fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y -
              ll_y, this.angle1 - rot, this.angle2, fillcolor, fb2, fb1,
              Gradient.DiagonalUpperLeft);

          // Draw circle
          if (this.gradient_contrast >= 0) {
            fb2 =
                GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                    chot, null);
            fb1 = GlowColor.shift_drawtype(
                fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
                null);
          } else {
            fb2 = GlowColor.shift_drawtype(
                fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
                null);
            fb1 =
                GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                    chot, null);
          }
          this.ctx.gdraw.gradient_fill_arc(ll_x + ish, ll_y + ish, ur_x - ll_x -
              2 * ish, ur_y - ll_y - 2 * ish, this.angle1 - rot, this.angle2,
              fillcolor, fb1, fb2, grad);
        }
      }
    }
    if (this.border !== 0 || this.fill === 0) {
      drawtype =
          GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
              highlight, colornode, 0, 0);
      this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1 -
          rot, this.angle2, drawtype, idx, 0);
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

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}