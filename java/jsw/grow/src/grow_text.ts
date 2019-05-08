class GrowText {
  ctx: GrowCtx;
  trf: GlowTransform;
  p: GlowPoint;
  n_name;
  x_right;
  x_left;
  y_high;
  y_low;
  original_color_drawtype;
  font;
  adjustment = Adjustment.Left;
  dynamicsize;

  text;
  draw_type;
  text_size;
  display_level;
  color_drawtype;

  constructor(ctx) {
    this.ctx = ctx;
    this.trf = new GlowTransform();
    this.p = new GlowPoint();
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
        case GlowSave.GrowText:
          break;
        case GlowSave.GrowText_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowText_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowText_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowText_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowText_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowText_original_color_drawtype:
          this.original_color_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowText_font:
          this.font = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowText_adjustment:
          this.adjustment = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowText_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowText_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case GlowSave.GrowText_text_part:
          i = this.open_textpart(lines, i + 1);
          break;
        case GlowSave.GrowText_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowText");
          break;
      }
    }

    return i;
  }

  open_textpart(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowArc : " + lines[i]);
      }

      switch (key) {
        case GlowSave.Text:
          break;
        case GlowSave.Text_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Text_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case GlowSave.Text_color_drawtype:
          this.color_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.Text_text:
          this.text = lines[i].substring(4);
          break;
        case GlowSave.Text_p:
          i = this.p.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowText");
          break;
      }
    }

    return i;
  }

  event_handler(event, fx, fy) {
    return 0;
  }

  draw() {
    this.tdraw(null, 0, 0, null, null);
  }

  tdraw(t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0 || this.text === null) {
      return;
    }

    let x1, y1, rx1, ry1;
    let z_width, z_height, z_descent;
    let rot;
    let trf_scale = this.trf.vertical_scale(t);
    let idx = Math.floor(trf_scale * this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 3);
    let tsize = trf_scale * this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (8 + 2 * this.text_size);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);
    let highl = highlight;
    if (node !== null) {
      highl = node.highlight;
    }

    let lfont;
    let ldraw_type;

    if (node !== null && node.text_font !== Font.No) {
      lfont = node.text_font;
      ldraw_type = node.text_type;
    } else {
      lfont = this.font;
      ldraw_type = this.draw_type;
    }

    rx1 = ry1 = z_width = z_height = 0;
    if (t === null) {
      x1 =
          Math.floor(this.trf.x(this.p.x, this.p.y) * this.ctx.mw.zoom_factor_x +
              0.5) - this.ctx.mw.offset_x;
      y1 =
          Math.floor(this.trf.y(this.p.x, this.p.y) * this.ctx.mw.zoom_factor_y +
              0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot());
    } else {
      x1 = Math.floor(this.trf.x(t, this.p.x, this.p.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.p.x, this.p.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot(t));
    }
    if (this.adjustment === Adjustment.Center) {
      rot = rot < 0 ? rot % 360 + 360 : rot % 360;
    } else {
      rot = 0;
    }

    if (this.text !== "") {
      if (highl !== 0 || (hot !== 0 && node === null) ||
          this.adjustment !== Adjustment.Left) {
        let dim = this.ctx.gdraw.getTextExtent(this.text, Math.max(0, idx),
            lfont, ldraw_type);
        z_width = dim.width;
        z_height = dim.height;
        z_descent = z_height / 4;
        switch (this.adjustment) {
          case Adjustment.Left:
            rx1 = x1;
            ry1 = y1 - (z_height - z_descent);
            break;
          case Adjustment.Right:
            x1 -= z_width;
            rx1 = x1;
            ry1 = y1 - (z_height - z_descent);
            break;
          case Adjustment.Center:
            switch (rot) {
              case 90:
                rx1 = x1 - z_width / 2;
                ry1 = y1 - z_height / 2;
                x1 -= z_width / 2;
                y1 += z_height / 2;
                break;
              case 270:
                rx1 = x1 - z_width / 2;
                ry1 = y1 - z_height / 2;
                x1 -= z_width / 2;
                y1 -= z_height / 2 - z_width / 2;
                break;
              default:
                rx1 = x1 - z_width / 2;
                ry1 = y1 - z_height / 2;
                x1 -= z_width / 2;
                y1 += z_height / 2 - z_descent;
                break;
            }
            break;
          default:
            break;
        }
      }

      if (highl !== 0) {
        this.ctx.gdraw.rect(rx1, ry1, z_width, z_height,
            DrawType.FillHighlight, Math.max(1, Math.min(idx + hot, 2)), 0);
      } else if (hot !== 0 && node === null) {
        this.ctx.gdraw.rect(rx1, ry1, z_width, z_height,
            DrawType.LineGray, Math.max(Math.min(idx, 2), 1), 0);
      }
      if (idx >= 0) {
        let color = GlowColor.get_drawtype(this.color_drawtype,
            DrawType.LineHighlight, highlight, colornode, 2, 0);
        this.ctx.gdraw.text(x1, y1, this.text, ldraw_type, color, idx,
            highlight, 0, lfont, tsize, rot);
      }
    } else if (idx >= 0) {
      let dim2 = this.ctx.gdraw.getTextExtent("A", Math.max(0, idx), lfont,
          ldraw_type);
      z_width = dim2.width;
      z_height = dim2.height;
      z_descent = z_height / 4;
      this.ctx.gdraw.rect(x1, y1 - (z_height - z_descent), z_width, z_height,
          DrawType.LineGray, idx, 0);
    }
  }

  get_borders(t, g) {
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}