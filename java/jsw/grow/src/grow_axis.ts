class Range {
  lines;
  longq;
  vvalq;
  hvalq;
  format;

  constructor(lines, longq, vvalq, hvalq, format) {
    this.lines = lines;
    this.longq = longq;
    this.vvalq = vvalq;
    this.hvalq = hvalq;
    this.format = format;
  }
}

const RangeData = [[new Range(26, 5, 5, 10, "%3.1f"), // 1
  new Range(21, 5, 5, 10, "%3.1f"), // 2
  new Range(31, 5, 10, 10, "%3.1f"), // 3
  new Range(41, 5, 10, 20, "%3.1f"), // 4
  new Range(26, 5, 5, 10, "%1.0f"), // 5
  new Range(31, 5, 10, 10, "%1.0f"), // 6
  new Range(36, 5, 10, 10, "%1.0f"), // 7
  new Range(17, 2, 4, 4, "%1.0f"), // 8
  new Range(19, 2, 4, 4, "%1.0f"), // 9
  new Range(21, 2, 4, 8, "%2.0f"), // 10
  new Range(23, 2, 4, 8, "%2.0f"), // 11
  new Range(13, 4, 4, 4, "%2.0f"), // 12
  new Range(14, 4, 4, 4, "%2.0f"), // 13
  new Range(15, 4, 4, 4, "%2.0f"), // 14
  new Range(16, 5, 5, 5, "%2.0f"), // 15
  new Range(17, 5, 5, 5, "%2.0f"), // 16
  new Range(18, 5, 5, 5, "%2.0f"), // 17
  new Range(19, 5, 5, 5, "%2.0f"), // 18
  new Range(20, 5, 5, 5, "%2.0f"), // 19
  new Range(21, 5, 5, 5, "%2.0f"), // 20
  new Range(22, 5, 5, 5, "%2.0f"), // 21
  new Range(23, 5, 5, 5, "%2.0f"), // 22
  new Range(24, 5, 5, 5, "%2.0f"), // 23
  new Range(25, 5, 5, 5, "%2.0f"), // 24
  new Range(26, 5, 5, 10, "%2.0f")],// 25
  [new Range(101, 5, 10, 20, "%3.1f"), // 1
    new Range(101, 5, 10, 20, "%3.1f"), // 2
    new Range(61, 10, 10, 20, "%3.1f"), // 3
    new Range(81, 10, 10, 20, "%3.1f"), // 4
    new Range(101, 10, 20, 20, "%1.0f"), // 5
    new Range(61, 5, 10, 20, "%1.0f"), // 6
    new Range(71, 5, 10, 20, "%1.0f"), // 7
    new Range(81, 5, 10, 20, "%1.0f"), // 8
    new Range(91, 5, 10, 20, "%1.0f"), // 9
    new Range(101, 5, 10, 20, "%2.0f"), // 10
    new Range(56, 5, 5, 5, "%2.0f"), // 11
    new Range(61, 5, 10, 10, "%2.0f"), // 12
    new Range(66, 5, 10, 20, "%2.0f"), // 13
    new Range(71, 5, 10, 20, "%2.0f"), // 14
    new Range(76, 5, 10, 20, "%2.0f"), // 15
    new Range(81, 5, 10, 20, "%2.0f"), // 16
    new Range(86, 5, 10, 20, "%2.0f"), // 17
    new Range(91, 5, 10, 20, "%2.0f"), // 18
    new Range(96, 5, 10, 20, "%2.0f"), // 19
    new Range(101, 5, 10, 25, "%2.0f"), // 20
    new Range(43, 2, 6, 6, "%2.0f"), // 21
    new Range(45, 2, 4, 8, "%2.0f"), // 22
    new Range(47, 2, 4, 8, "%2.0f"), // 23
    new Range(49, 2, 4, 8, "%2.0f"), // 24
    new Range(26, 5, 5, 5, "%2.0f")]  // 25
];

class GrowAxis extends GrowRect {
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
  sb;

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
        case GlowSave.GrowAxis:
          break;
        case GlowSave.GrowAxis_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxis_text_drawtype:
          this.text_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxis_text_color_drawtype:
          this.text_color_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxis_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowAxis_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowAxis_lines:
          this.lines = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxis_longquotient:
          this.longquotient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxis_valuequotient:
          this.valuequotient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAxis_format:
          if (tokens.length > 1) {
            this.format = tokens[1];
          }
          break;
        case GlowSave.GrowAxis_rect_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowAxis_userdata_cb:
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
          console.log("Syntax error in GrowAxis");
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

    if ((node !== null && node.invisible !== 0) || this.invisible !== 0) {
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

    let x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let rotation =
          (this.trf.rot(t) / 360 - Math.floor(this.trf.rot(t) / 360)) * 360;

    let ll_x = Math.min(x1, x2);
    let ur_x = Math.max(x1, x2);
    let ll_y = Math.min(y1, y2);
    let ur_y = Math.max(y1, y2);
    let drawtype =
        GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
            highlight, colornode, 0, 0);

    if (rotation <= 45 || rotation > 315) {
      // Vertical line to the right and values to the left

      this.ctx.gdraw.line(ur_x, ll_y, ur_x, ur_y, drawtype, idx, 0);

      // Calculate max value text width
      if (draw_text) {
        for (let i = 0; i < this.lines; i++) {
          if (i % this.valuequotient === 0) {
            let text = this.format_text(this.format, this.max_value - i *
                this.increment);
            let p = this.ctx.gdraw.getTextExtent(text, Math.max(0, text_idx),
                Font.Helvetica, this.text_drawtype);
            z_width = p.x;
            z_height = p.y;
            z_descent = z_height / 4;
            if (max_z_width < z_width) {
              max_z_width = z_width;
            }
          }
        }
        x_text = ll_x + max_z_width;
        line_length = ur_x - ll_x - max_z_width;
        if (line_length < 3) {
          line_length = 3;
        }
      } else {
        x_text = ll_x;
        line_length = ur_x - ll_x;
      }

      for (let i = 0; i < this.lines; i++) {
        let y = Math.floor(ll_y + (ur_y - ll_y) / (this.lines - 1) * i);
        if (i % this.longquotient === 0) {
          this.ctx.gdraw.line(ur_x - line_length, y, ur_x, y, drawtype, idx, 0);
        } else {
          this.ctx.gdraw.line(ur_x - Math.floor(2.0 / 3 * line_length), y, ur_x,
              y, drawtype, idx, 0);
        }
        if (draw_text) {
          let text =
              this.format_text(this.format, this.max_value - i * this.increment);

          if (text_idx >= 0 && max_z_width < ur_x - ll_x &&
              i % this.valuequotient === 0) {
            let y_text;
            if (i === this.lines - 1) {
              y_text = y;
            } else if (i === 0) {
              y_text = y + z_height - z_descent - 3;
            } else {
              y_text = y + (z_height - z_descent) / 2;
            }
            this.ctx.gdraw.text(ll_x, y_text, text, this.text_drawtype,
                this.text_color_drawtype, text_idx, highlight, 0,
                Font.Helvetica, tsize, 0);
          }
        }
      }
    } else if (rotation > 45 && rotation <= 135) {
      // Horizontal line at bottom and values to the top

      this.ctx.gdraw.line(ll_x, ur_y, ur_x, ur_y, drawtype, idx, 0);

      // Calculate max value text height
      if (draw_text) {
        let p2 = this.ctx.gdraw.getTextExtent("0", Math.max(0, text_idx),
            Font.Helvetica, this.text_drawtype);

        z_width = p2.x;
        z_height = p2.y;
        z_descent = z_height / 4;
        line_length = ur_y - ll_y - z_height;
        if (line_length < 3) {
          line_length = 3;
        }
      } else {
        line_length = ur_y - ll_y;
      }

      for (let i = 0; i < this.lines; i++) {
        let x = Math.floor(ll_x + (ur_x - ll_x) / (this.lines - 1) *
            (this.lines - 1 - i));
        if (i % this.longquotient === 0) {
          this.ctx.gdraw.line(x, ur_y - line_length, x, ur_y, drawtype, idx, 0);
        } else {
          this.ctx.gdraw.line(x, ur_y - Math.floor(2.0 / 3 * line_length), x,
              ur_y, drawtype, idx, 0);
        }

        if (draw_text && i % this.valuequotient === 0) {
          let text =
              this.format_text(this.format, this.max_value - i * this.increment);
          let p3 = this.ctx.gdraw.getTextExtent(text, Math.max(0, text_idx),
              Font.Helvetica, this.text_drawtype);
          z_width = p3.x;
          z_height = p3.y;
          z_descent = z_height / 4;

          if (text_idx >= 0 && z_height < ur_y - ll_y) {
            if (i === this.lines - 1) {
              x_text = x;
            } else if (i === 0) {
              x_text = x - z_width;
            } else {
              x_text = x - (z_width) / 2;
            }
            this.ctx.gdraw.text(x_text, ll_y + z_height - z_descent, text,
                this.text_drawtype, this.text_color_drawtype, text_idx, highlight,
                0, Font.Helvetica, tsize, 0);
          }
        }
      }
    } else if (rotation > 135 && rotation <= 225) {
      // Vertical line to the left and values to the right

      this.ctx.gdraw.line(ll_x, ll_y, ll_x, ur_y, drawtype, idx, 0);

      // Calculate max value text width
      if (draw_text) {
        for (let i = 0; i < this.lines; i++) {
          if (i % this.valuequotient === 0) {
            let text = this.format_text(this.format, this.max_value - i *
                this.increment);
            let p4 = this.ctx.gdraw.getTextExtent(text, Math.max(0, text_idx),
                Font.Helvetica, this.text_drawtype);
            z_width = p4.x;
            z_height = p4.y;
            z_descent = z_height / 4;
            if (max_z_width < z_width) {
              max_z_width = z_width;
            }
          }
        }
        x_text = ur_x - max_z_width;
        line_length = ur_x - ll_x - max_z_width;
        if (line_length < 3) {
          line_length = 3;
        }
      } else {
        x_text = ur_x;
        line_length = ur_x - ll_x;
      }

      for (let i = 0; i < this.lines; i++) {
        let y = Math.floor(ll_y + (ur_y - ll_y) / (this.lines - 1) *
            (this.lines - 1 - i));
        if (i % this.longquotient === 0) {
          this.ctx.gdraw.line(ll_x, y, ll_x + line_length, y, drawtype, idx, 0);
        } else {
          this.ctx.gdraw.line(ll_x, y, ll_x + Math.floor(2.0 / 3 * line_length),
              y, drawtype, idx, 0);
        }
        let text =
            this.format_text(this.format, this.max_value - i * this.increment);

        if (draw_text && text_idx >= 0 && max_z_width < ur_x - ll_x &&
            i % this.valuequotient === 0) {
          let y_text;
          if (i === this.lines - 1) {
            y_text = y + z_height - z_descent - 3;
          } else if (i === 0) {
            y_text = y;
          } else {
            y_text = y + (z_height - z_descent) / 2;
          }
          this.ctx.gdraw.text(x_text, y_text, text, this.text_drawtype,
              this.text_color_drawtype, text_idx, highlight, 0,
              Font.Helvetica, tsize, 0);
        }
      }
    } else { // if ( 225 < rotation && rotation <= 315)
      // Horizontal line at top and values at the bottom

      this.ctx.gdraw.line(ll_x, ll_y, ur_x, ll_y, drawtype, idx, 0);

      // Calculate max value text height
      if (draw_text) {
        let p5 = this.ctx.gdraw.getTextExtent("0", Math.max(0, text_idx),
            Font.Helvetica, this.text_drawtype);

        z_width = p5.x;
        z_height = p5.y;
        z_descent = z_height / 4;
        line_length = ur_y - ll_y - (z_height - z_descent);
        if (line_length < 3) {
          line_length = 3;
        }
      } else {
        line_length = ur_y - ll_y;
      }

      for (let i = 0; i < this.lines; i++) {
        let x = Math.floor(ll_x + (ur_x - ll_x) / (this.lines - 1) * i);
        if (i % this.longquotient === 0) {
          this.ctx.gdraw.line(x, ll_y, x, ll_y + line_length, drawtype, idx, 0);
        } else {
          this.ctx.gdraw.line(x, ll_y, x, ll_y +
              Math.floor(2.0 / 3 * line_length), drawtype, idx, 0);
        }
        if (draw_text && i % this.valuequotient === 0) {
          let text =
              this.format_text(this.format, this.max_value - i * this.increment);
          let p6 = this.ctx.gdraw.getTextExtent(text, Math.max(0, text_idx),
              Font.Helvetica, this.text_drawtype);
          z_width = p6.x;
          z_height = p6.y;
          z_descent = z_height / 4;

          if (text_idx >= 0 && z_height - z_descent < ur_y - ll_y) {
            if (i === this.lines - 1) {
              x_text = x - z_width;
            } else if (i === 0) {
              x_text = x;
            } else {
              x_text = x - (z_width) / 2;
            }
            this.ctx.gdraw.text(x_text, ur_y, text, this.text_drawtype,
                this.text_color_drawtype, text_idx, highlight, 0,
                Font.Helvetica, tsize, 0);
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

    let x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let rotation = (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) *
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
        di = Math.floor(di / 10 + 0.5);
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

  setVisibility(visibility) {
    if (visibility === Vis.Visible) {
      this.invisible = 0;
    } else if (visibility === Vis.Invisible) {
      this.invisible = 1;
      this.draw();
    }
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