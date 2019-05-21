class GrowAxisArc extends GrowArc {
  text_size;
  text_drawtype;
  text_color_drawtype;
  max_value;
  min_value;
  lines;
  linelength;
  longquotient;
  valuequotient;
  increment;
  format = null;
  userdata = null;
  cFormat = null;
  sb = null;

  constructor(ctx) {
    super(ctx);
  }

  configure() {
    if (this.lines <= 1) {
      this.lines = 2;
    }
    if (this.longquotient <= 0) {
      this.longquotient = 1;
    }
    if (this.valuequotient <= 0) {
      this.valuequotient = 1;
    }
    this.increment = (this.max_value - this.min_value) / (this.lines - 1);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowAxisArc:
          break;
        case GlowSave.GrowAxisArc_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxisArc_text_drawtype:
          this.text_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxisArc_text_color_drawtype:
          this.text_color_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxisArc_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowAxisArc_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowAxisArc_lines:
          this.lines = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxisArc_linelength:
          this.linelength = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowAxisArc_longquotient:
          this.longquotient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxisArc_valuequotient:
          this.valuequotient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxisArc_format:
          if (tokens.length > 1) {
            this.format = tokens[1];
          }
          break;
        case GlowSave.GrowAxisArc_arc_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowAxisArc_userdata_cb:
          if (this.ctx.appl !== null) {
            let ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
                UserdataCbType.Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case GlowSave.End:
          this.configure();
          if (this.format !== null) {
            this.cFormat = new GlowCFormat(this.format);
          }
          return i;
        default:
          console.log("Syntax error in GrowAxisArc");
          break;
      }
    }

    this.configure();
    if (this.format !== null) {
      this.cFormat = new GlowCFormat(this.format);
    }

    return i;
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0) {
      return;
    }
    let draw_text = (Math.abs(this.increment) > Number.MIN_VALUE);
    let line_length;
    let x_text;
    let z_height = 0, z_width, z_descent = 0;
    let max_z_width = 0;
    let text_idx = Math.floor(this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 4);
    let tsize = this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        (8 + 2 * this.text_size);
    text_idx = Math.min(text_idx, DRAW_TYPE_SIZE - 1);

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

    let rotation = t ? this.trf.rotation + t.rotation : this.trf.rotation;
    rotation = (rotation / 360 - Math.floor(rotation / 360)) * 360;

    let ll_x = Math.min(x1, x2);
    let ur_x = Math.max(x1, x2);
    let ll_y = Math.min(y1, y2);
    let ur_y = Math.max(y1, y2);
    let drawtype =
        GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
            highlight, colornode, 0, 0);

    if (true) {
      // Lines inwards
      this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1 -
          rotation, this.angle2, drawtype, idx, 0);

      if (this.lines === 1) {
        return;
      }

      let text;
      if (this.increment > 0) {
        text = this.format_text(this.format, this.min_value + (this.lines - 2) *
            this.increment);
      } else {
        text = this.format_text(this.format, this.min_value + this.increment);
      }
      let p = this.ctx.gdraw.getTextExtent(text, this.text_drawtype, Math.max(0, text_idx),
          Font.Helvetica);
      z_width = p.x;
      z_height = p.y;
      z_descent = z_height / 4;
      if (max_z_width < z_width) {
        max_z_width = z_width;
      }

      let line_angle = this.angle2 / (this.lines - 1);
      for (let i = 0; i < this.lines; i++) {
        let sin1 = Math.sin((this.angle1 + i * line_angle) / 180 * Math.PI);
        let cos1 = Math.cos((this.angle1 + i * line_angle) / 180 * Math.PI);
        y1 = Math.floor((ur_y - ll_y) / 2 * (-sin1 + 1) + ll_y);
        x1 = Math.floor((ur_x - ll_x) / 2 * (cos1 + 1) + ll_x);
        if (i % this.longquotient === 0) {
          y2 = Math.floor((ur_y - ll_y) / 2 *
              (-sin1 * (1.0 - this.linelength) + 1) + ll_y);
          x2 = Math.floor((ur_x - ll_x) / 2 *
              (cos1 * (1.0 - this.linelength) + 1) + ll_x);
        } else {
          y2 = Math.floor((ur_y - ll_y) / 2 *
              (-sin1 * (1.0 - this.linelength / 2) + 1) + ll_y);
          x2 = Math.floor((ur_x - ll_x) / 2 *
              (cos1 * (1.0 - this.linelength / 2) + 1) + ll_x);
        }
        let yt =
            Math.floor((ur_y - ll_y) / 2 * (-sin1 * (1.0 - this.linelength) + 1) +
                ll_y + sin1 * (z_height - z_descent) / 2);
        let xt =
            Math.floor((ur_x - ll_x) / 2 * (cos1 * (1.0 - this.linelength) + 1) +
                ll_x - cos1 * z_width / 2);

        this.ctx.gdraw.line(x1, y1, x2, y2, drawtype, idx, 0);
        if (draw_text) {
          if (text_idx >= 0 && i % this.valuequotient === 0 &&
              !(this.angle2 === 360 &&
                  ((this.increment > 0 && i === this.lines - 1) ||
                      (this.increment < 0 && i === 0)))) {
            text = this.format_text(this.format, this.min_value + i *
                this.increment);
            p = this.ctx.gdraw.getTextExtent(text, this.text_drawtype, Math.max(0, text_idx),
                Font.Helvetica);
            z_width = p.x;
            z_height = p.y;
            z_descent = z_height / 4;
            if (max_z_width < z_width) {
              max_z_width = z_width;
            }

            if (i === this.lines - 1 && this.angle1 === 0 &&
                this.angle2 === 180) {
              xt -= z_width / 2;
            } else if (i === 0 && this.angle1 === 0 && this.angle2 !== 360) {
              xt -= z_width / 2;
            } else {
              yt += (z_height - z_descent) / 2;
              xt -= z_width / 2;
            }
            this.ctx.gdraw.text(xt, yt, text, this.text_drawtype,
                this.text_color_drawtype, text_idx, highlight, Font.Helvetica, tsize, 0);
          }
        }
      }
    }
  }

  format_text(fmt, value) {
    if (Math.abs(value) < Number.MIN_VALUE) {
      value = 0;
    }
    return this.cFormat.format(value, this.sb);
  }

  getUserData() {
    return this.userdata;
  }

  set_range(minval, maxval, keep_settings) {
    this.max_value = maxval;
    this.min_value = minval;

    let p1 = this.trf.apply(this.ll);
    let p2 = this.trf.apply(this.ur);

    let x1 = Math.floor(p1.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y1 = Math.floor(p1.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let x2 = Math.floor(p2.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y2 = Math.floor(p2.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    let rotation = (this.trf.rotation / 360 - Math.floor(this.trf.rotation / 360)) *
        360;

    if (keep_settings === 0) {
      let horizontal = (rotation < 45 || (rotation > 135 && rotation < 225) ||
          rotation > 315) ? 0 : 1;
      let len = (horizontal === 0) ? Math.abs(y2 - y1) : Math.abs(x2 - x1);

      let lix = Number(!(len < 150));

      let d = Math.abs(maxval - minval);
      if (d < 5) {
        d = 1000 * d;
      }

      let di = Math.floor(d + 0.5);
      while (di >= 25) {
        di /= 10;
      }

      if (di > 0 && di <= 25) {
        this.lines = RangeData[lix][di - 1].lines;
        this.longquotient = RangeData[lix][di - 1].longq;
        if (horizontal === 0) {
          this.valuequotient = RangeData[lix][di - 1].vvalq;
        } else {
          this.valuequotient = RangeData[lix][di - 1].hvalq;
        }
      }

      let m = Math.max(Math.abs(maxval), Math.abs(minval));
      if (m < 0.01) {
        this.format = "%g";
      } else if (m < 0.1) {
        this.format = "%5.3f";
      } else if (m < 1) {
        this.format = "%4.2f";
      } else if ((lix === 0 && m < 3) || (lix === 1 && m <= 4)) {
        this.format = "%3.1f";
      } else if (m <= 20) {
        this.format = "%2.0f";
      } else if (m <= 200) {
        this.format = "%3.0f";
      } else if (m < 2000) {
        this.format = "%4.0f";
      } else if (m < 20000) {
        this.format = "%5.0f";
      } else {
        this.format = "%g";
      }
    }
    this.configure();
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