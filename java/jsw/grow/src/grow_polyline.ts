class GrowPolyline extends GlowPolyline {
  trf: GlowTransform;
  n_name = null;
  ll_x: Number;
  ll_y: Number;
  ur_x: Number;
  ur_y: Number;
  original_border_drawtype = 0;
  original_fill_drawtype = 0;
  fill_drawtype = 0;
  background_drawtype = 0;
  border = 1;
  fill_eq_border = 0;
  shadow_width = 0;
  shadow_contrast = 0;
  shadow = 0;
  relief = 0;
  disable_shadow = 0;
  fixcolor = 0;
  fixposition = 0;
  gradient = 0;
  gradient_contrast = 0;
  disable_gradient = 0;
  fill_eq_light = 0;
  fill_eq_shadow = 0;
  fill_eq_bglight = 0;
  fill_eq_bgshadow = 0;
  fill_eq_background = 0;
  dynamicsize = 0;

  constructor(ctx) {
    super(ctx);
    this.trf = new GlowTransform();
  }

  constructor(ctx, name, pointarray, point_cnt, border_d_type, line_w,
       fix_line_w, fill, display_border, display_shadow, fill_d_type) {
    super(ctx);
    this.trf = new GlowTransform();
    this.original_border_drawtype = border_d_type;
    this.fill_drawtype = fill_d_type;
    this.border = display_border;
    this.shadow = display_shadow;
    this.shadow_width = 5;
    this.relief = Relief.Up;
    this.shadow_contrast = 2;
    this.gradient = Gradient.No;
    this.gradient_contrast = 4;
    this.n_name = name;
    this.draw_type = border_d_type;
    this.line_width = line_w;
    this.fill = fill;
    this.background_drawtype = DrawType.No;

    for (let i = 0; i < point_cnt; i++) {
      let p = new GlowPoint();
      p.x = pointarray[i].x;
      p.y = pointarray[i].y;
      this.a_points.add(p);
    }
    this.points = new Array(this.a_points.size());
    for (let i = 0; i < this.a_points.size(); i++) {
      this.points[i] = new Point();
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowPolyLine:
          break;
        case GlowSave.GrowPolyLine_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowPolyLine_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPolyLine_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPolyLine_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPolyLine_y_low:
          this.ll_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPolyLine_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_original_fill_drawtype:
          this.original_fill_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_fill_drawtype:
          this.fill_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_background_drawtype:
          this.background_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_shadow_width:
          this.shadow_width = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPolyLine_shadow_contrast:
          this.shadow_contrast = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_relief:
          this.relief = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_disable_shadow:
          this.disable_shadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_fixcolor:
          this.fixcolor = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_fixposition:
          this.fixposition = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_gradient:
          this.gradient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_gradient_contrast:
          this.gradient_contrast = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_disable_gradient:
          this.disable_gradient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_fill_eq_border:
          this.fill_eq_border = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_fill_eq_light:
          this.fill_eq_light = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_fill_eq_shadow:
          this.fill_eq_shadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_fill_eq_bglight:
          this.fill_eq_bglight = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_fill_eq_bgshadow:
          this.fill_eq_bgshadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_fill_eq_background:
          this.fill_eq_background = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPolyLine_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case GlowSave.GrowPolyLine_polyline_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowPolyLine_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case GlowSave.End:
          for (let j = 0; j < this.a_points.size(); j++) {
            this.points[j] = new Point();
          }
          return i;
        default:
          console.log("Syntax error in GrowPolyline");
          break;
      }
    }

    for (let j = 0; j < this.a_points.size(); j++) {
      this.points[j] = new Point();
    }

    return i;
  }

  event_handler(event, fx, fy) {
    return 0;
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let chot = 0;
    if (hot !== 0) {
      if (this.ctx.hot_indication === HotIndication.No) {
        hot = 0;
      } else if (this.ctx.hot_indication === HotIndication.DarkColor) {
        chot = hot;
        hot = 0;
      } else if (this.ctx.hot_indication === HotIndication.LightColor) {
        chot = -hot;
        hot = 0;
      }
    }

    if (this.fixcolor !== 0) {
      colornode = null;
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

    this.a_points.forEach(function (e) {
      let x1 = this.trf.x(t, e.x, e.y);
      let y1 = this.trf.y(t, e.x, e.y);
      e.x = Math.floor(x1 * this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      e.y = Math.floor(y1 * this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    });
    if (this.fill !== 0) {
      let grad = this.gradient;
      if (this.gradient === Gradient.No &&
          (node !== null && node.gradient !== Gradient.No) &&
          this.disable_gradient === 0) {
        grad = node.gradient;
      }

      let drawtype;
      if (this.fill_eq_border !== 0) {
        drawtype =
            GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
                highlight, colornode, 0, 0);
      } else if (this.fill_eq_background !== 0) {
        drawtype = GlowColor.get_drawtype(this.background_drawtype,
            DrawType.FillHighlight, highlight, colornode, 3, 0);
      } else {
        drawtype = GlowColor.get_drawtype(this.fill_drawtype,
            DrawType.FillHighlight, highlight, colornode, 1, 0);
      }
      if (this.fill_eq_light !== 0 && node !== null && node.shadow !== 0) {
        drawtype =
            GlowColor.shift_drawtype(drawtype, -this.shadow_contrast + chot,
                colornode);
      } else if (this.fill_eq_bglight !== 0) {
        if (colornode !== null &&
            node.background_drawtype !== DrawType.No) {
          drawtype = GlowColor.shift_drawtype(
              colornode.background_drawtype, -this.shadow_contrast + chot,
              colornode);
        } else {
          drawtype = GlowColor.shift_drawtype(
              this.original_fill_drawtype, -this.shadow_contrast + chot,
              colornode);
        }
      } else if (this.fill_eq_shadow !== 0 && node !== null &&
          node.shadow !== 0) {
        drawtype =
            GlowColor.shift_drawtype(drawtype, this.shadow_contrast + chot,
                colornode);
      } else if (this.fill_eq_bgshadow !== 0) {
        if (colornode !== null &&
            node.background_drawtype !== DrawType.No) {
          drawtype = GlowColor.shift_drawtype(
              colornode.background_drawtype, this.shadow_contrast + chot,
              colornode);
        } else {
          drawtype = GlowColor.shift_drawtype(
              this.original_fill_drawtype, this.shadow_contrast + chot,
              colornode);
        }
      } else if (chot !== 0) {
        drawtype = GlowColor.shift_drawtype(drawtype, chot, null);
      }

      if (grad === Gradient.No || drawtype === DrawType.ColorRed) {
        this.ctx.gdraw.polyline(this.points, this.a_points.size(),
            drawtype, true, 0);
      } else {
        let rotation = t ? this.trf.rot(t) : this.trf.rot();

        let f1, f2;
        if (this.gradient_contrast >= 0) {
          f2 = GlowColor.shift_drawtype(drawtype, -this.gradient_contrast / 2,
              null);
          f1 = GlowColor.shift_drawtype(drawtype,
              Math.floor(this.gradient_contrast / 2 + 0.6), null);
        } else {
          f2 = GlowColor.shift_drawtype(drawtype, -Math.floor(
              this.gradient_contrast) / 2 - 0.6, null);
          f1 = GlowColor.shift_drawtype(drawtype, this.gradient_contrast / 2,
              null);
        }
        this.ctx.gdraw.gradient_fill_polyline(this.points, this.a_points.size(),
            drawtype, f1, f2, this.ctx.gdraw.gradient_rotate(rotation, grad));
      }
    }

    let display_shadow = ((node !== null && node.shadow !== 0) ||
        this.shadow !== 0) && this.disable_shadow === 0 && this.fill_eq_light ===
        0 && this.fill_eq_shadow === 0;

    if (display_shadow && this.shadow_width !== 0) {
      let trf_scale = this.trf.vertical_scale(t);
      let ish = Math.floor(this.shadow_width / 100 * trf_scale *
          Math.min((this.ur_x - this.ll_x) * this.ctx.mw.zoom_factor_x,
              (this.ur_y - this.ll_y) * this.ctx.mw.zoom_factor_y) + 0.5);

      if (ish >= 1) {
        let sp = this.calculate_shadow(ish, highlight, colornode, 0, chot);
        for (let i = 0; i < sp.length - 1; i++) {
          let p = [
            this.points[i],
            sp[i],
            sp[i + 1],
            this.points[i + 1]
          ];
          this.ctx.gdraw.polyline(p, 4, sp[i].drawtype, true, 0);
        }
      }
    }
    if (this.border !== 0 ||
        !(this.fill !== 0 || (display_shadow && this.shadow_width !== 0))) {
      let drawtype =
          GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
              highlight, colornode, 0, 0);
      this.ctx.gdraw.polyline(this.points, this.a_points.size(), drawtype, idx,
          0);
    }
  }

  calculate_shadow(ish, highlight, colornode, javaexport, chot) {
    let p_num = this.a_points.size();
    if (this.points[0].x === this.points[p_num - 1].x &&
        this.points[0].y === this.points[p_num - 1].y) {
      p_num--;
    }

    let sp;
    if (this.points[0].x === this.points[this.a_points.size() - 1].x &&
        this.points[0].y === this.points[this.a_points.size() - 1].y) {
      sp = new Array(p_num + 1);
    } else {
      sp = new Array(p_num);
    }

    let fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
        DrawType.FillHighlight, highlight, colornode, 1, 0);

    let drawtype_incr = this.shadow_contrast;
    if (this.relief === Relief.Down) {
      drawtype_incr = -this.shadow_contrast;
    }
    let light_drawtype =
        GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot, colornode);
    let dark_drawtype =
        GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot, colornode);

    let pos01 = this.shadow_direction();

    for (let i = 0; i < p_num; i++) {
      sp[i] = new GlowShadowInfo();

      let sx0, sy0;
      if (i === 0) {
        sx0 = this.points[p_num - 1].x;
        sy0 = this.points[p_num - 1].y;
      } else {
        sx0 = this.points[i - 1].x;
        sy0 = this.points[i - 1].y;
      }
      let sx1 = this.points[i].x;
      let sy1 = this.points[i].y;
      let sx2, sy2;
      if (i === p_num - 1) {
        sx2 = this.points[0].x;
        sy2 = this.points[0].y;
      } else {
        sx2 = this.points[i + 1].x;
        sy2 = this.points[i + 1].y;
      }

      let pos12 = pos01;
      if (i !== -1) {
        if (Math.abs(sx0 - sx1) < Number.MIN_VALUE) {
          if (sx1 > sx2 && sy1 < sy0) {
            pos12 = -pos01;
          } else if (sx1 < sx2 && sy1 > sy0) {
            pos12 = -pos01;
          }
        } else if (Math.abs(sx1 - sx2) < Number.MIN_VALUE) {
          if (sx0 < sx1 && sy2 > sy1) {
            pos12 = -pos01;
          } else if (sx0 > sx1 && sy2 < sy1) {
            pos12 = -pos01;
          }
        } else if (sx1 > sx0 && sx2 < sx1) {
          pos12 = -pos01;
        } else if (sx1 < sx0 && sx2 > sx1) {
          pos12 = -pos01;
        }
      }

      if (Math.abs(sx0 - sx1) < Number.MIN_VALUE) {
        if (Math.abs(sx1 - sx2) < Number.MIN_VALUE) {
          let x = sx1 + pos01 * ish;
          sp[i].x = Math.floor(x + 0.5);
          sp[i].y = Math.floor(sy1 + 0.5);
        } else {
          let k12 = (sy2 - sy1) / (sx2 - sx1);
          let m12 = sy1 - sx1 * k12 + pos12 * ish /
              Math.abs(Math.cos(Math.atan(k12)));

          let x = sx1 + pos01 * ish;
          sp[i].x = Math.floor(x + 0.5);
          sp[i].y = Math.floor(k12 * x + m12 + 0.5);
        }
      } else if (Math.abs(sx1 - sx2) < Number.MIN_VALUE) {
        let k01 = (sy1 - sy0) / (sx1 - sx0);
        let m01 =
            sy0 - sx0 * k01 + pos01 * ish / Math.abs(Math.cos(Math.atan(k01)));

        let x = sx1 + pos12 * ish;
        sp[i].x = Math.floor(x + 0.5);
        sp[i].y = Math.floor(k01 * x + m01 + 0.5);
      } else {
        let k01 = (sy1 - sy0) / (sx1 - sx0);
        let k12 = (sy2 - sy1) / (sx2 - sx1);
        let m01 =
            sy0 - sx0 * k01 + pos01 * ish / Math.abs(Math.cos(Math.atan(k01)));
        let m12 =
            sy1 - sx1 * k12 + pos12 * ish / Math.abs(Math.cos(Math.atan(k12)));
        if (Math.abs(k01 - k12) < Number.MIN_VALUE) {
          // Identical lines
          if (Math.abs(k01) < Number.MIN_VALUE) {
            sp[i].x = Math.floor(sx1 + 0.5);
            sp[i].y = Math.floor(m01 + 0.5);
          } else {
            k12 = -k12;
            m12 = sy2 - k12 * sx2;

            let x = (m12 - m01) / (k01 - k12);
            sp[i].x = Math.floor(x + 0.5);
            sp[i].y = Math.floor(k12 * x + m12 + 0.5);
            k12 = k01;
            m12 = m01;
          }
        } else {
          let x = (m12 - m01) / (k01 - k12);
          sp[i].x = Math.floor(x + 0.5);
          sp[i].y = Math.floor(k12 * x + m12 + 0.5);
        }
      }
      sp[i].drawtype = (pos12 === 1) ? light_drawtype : dark_drawtype;
      pos01 = pos12;
    }
    if (this.points[0].x === this.points[this.a_points.size() - 1].x &&
        this.points[0].y === this.points[this.a_points.size() - 1].y) {
      sp[p_num] = new GlowShadowInfo();
      sp[p_num].x = sp[0].x;
      sp[p_num].y = sp[0].y;
    }

    return sp;
  }

  shadow_direction() {
    let p_num = this.a_points.size();
    if ((this.points[p_num - 1].x === this.points[0].x &&
            this.points[p_num - 1].y === this.points[0].y)) {
      p_num--;
    }

    let a_sum1 = 0;
    let a_sum2 = 0;
    let a1_old = 0;
    let a0_old = 0;
    let a1, a2;
    for (let i = 0; i < p_num; i++) {
      if (i === p_num - 1) {
        if (this.points[0].x === this.points[i].x) {
          a1 = 90;
          if (this.points[0].y < this.points[i].y) {
            a1 += 180;
          }
        } else {
          a1 = Math.atan((this.points[0].y - this.points[i].y) /
              (this.points[0].x - this.points[i].x)) * 180 / Math.PI;
        }
        if (this.points[0].x < this.points[i].x) {
          a1 += 180;
        }
      } else {
        if (this.points[i + 1].x === this.points[i].x) {
          a1 = 90;
          if (this.points[i + 1].y < this.points[i].y) {
            a1 += 180;
          }
        } else {
          a1 = Math.atan((this.points[i + 1].y - this.points[i].y) /
              (this.points[i + 1].x - this.points[i].x)) * 180 / Math.PI;
        }
        if (this.points[i + 1].x < this.points[i].x) {
          a1 += 180;
        }
      }
      if (i === 0) {
        a0_old = a1;
      } else if (i > 0) {
        a2 = a1 - (a1_old - 180);
        if (a2 < 0) {
          a2 += 360;
        } else if (a2 >= 360) {
          a2 -= 360;
        }
        a_sum1 += a2;
        a_sum2 += 360 - a2;

      }
      if (i === p_num - 1) {
        a2 = a0_old - (a1 - 180);
        if (a2 < 0) {
          a2 += 360;
        } else if (a2 >= 360) {
          a2 -= 360;
        }
        a_sum1 += a2;
        a_sum2 += 360 - a2;
      }
      a1_old = a1;
    }

    let last_point;
    if (this.points[0].x === this.points[this.a_points.size() - 1].x &&
        this.points[0].y === this.points[this.a_points.size() - 1].y) {
      last_point = this.points[this.a_points.size() - 2];
    } else {
      last_point = this.points[this.a_points.size() - 1];
    }

    let dir = -1;
    if (a_sum2 - a_sum1 < 0) {
      if (this.points[0].x === last_point.x) {
        if (this.points[0].x < this.points[1].x) {
          dir = 1;
        }
      } else if (this.points[0].x > last_point.x) {
        dir = 1;
      }
    } else {
      if (this.points[0].x === last_point.x) {
        if (this.points[0].x < this.points[1].x) {
          dir = 1;
        }
      } else if (this.points[0].x < last_point.x) {
        dir = 1;
      }
    }
    return dir;
  }

  add_and_shift_y_value(value) {
    for (let i = this.a_points.size() - 1; i > 0; i--) {
      this.a_points.get(i).y = this.a_points.get(i - 1).y;
    }
    this.a_points.get(0).y = value;
  }

  add_and_shift_y_value_filled(value) {
    for (let i = this.a_points.size() - 2; i > 1; i--) {
      this.a_points.get(i).y = this.a_points.get(i - 1).y;
    }
    this.a_points.get(1).y = value;
  }

  get_borders(t, g) {
    let x2 = 0, y2 = 0;
    for (let i = 0; i < this.a_points.size() - 1; i++) {
      let e = this.a_points.get(i);
      let x1 = x2;
      let y1 = y2;
      if (i === 0) {
        x1 = this.trf.x(t, e.x, e.y);
        y1 = this.trf.y(t, e.x, e.y);
      }
      x2 = this.trf.x(t, e.x, e.y);
      y2 = this.trf.y(t, e.x, e.y);

      g.set(Rect.union(g, new Rect(Math.min(x1, x2), Math.min(y1, y2), Math.max(x1, x2), Math.max(y1, y2))));
    }
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}