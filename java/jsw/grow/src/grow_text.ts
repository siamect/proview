class GrowText extends GlowText {
  trf: GlowTransform;
  n_name;
  ll_x: Number;
  ll_y: Number;
  ur_x: Number;
  ur_y: Number;
  original_color_drawtype;
  font;
  adjustment = Adjustment.Left;
  dynamicsize;

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
        case GlowSave.GrowText:
          break;
        case GlowSave.GrowText_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowText_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowText_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowText_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowText_y_low:
          this.ll_y = parseFloat(tokens[1]);
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
          i = super.open(lines, i + 1);
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

  event_handler(event, fx, fy) {
    return 0;
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0 || this.text === null) {
      return;
    }

    let z_descent;
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

    let rx1 = 0;
    let ry1 = 0;
    let z_width = 0;
    let z_height = 0;
    let x1 = Math.floor(this.trf.x(t, this.p.x, this.p.y) * this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
    let y1 = Math.floor(this.trf.y(t, this.p.x, this.p.y) * this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    let rot = Math.floor(this.trf.rot(t));
    if (this.adjustment === Adjustment.Center) {
      rot = rot < 0 ? rot % 360 + 360 : rot % 360;
    } else {
      rot = 0;
    }

    if (this.text !== "") {
      if (highl !== 0 || (hot !== 0 && node === null) ||
          this.adjustment !== Adjustment.Left) {
        let p = this.ctx.gdraw.getTextExtent(this.text, ldraw_type, Math.max(0, idx),
            lfont);
        z_width = p.x;
        z_height = p.y;
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
        this.ctx.gdraw.rect(rx1, ry1, z_width, z_height, DrawType.FillHighlight,
            false, Math.max(1, Math.min(idx + hot, 2)));
      } else if (hot !== 0 && node === null) {
        this.ctx.gdraw.rect(rx1, ry1, z_width, z_height, DrawType.LineGray,
            false, Math.max(Math.min(idx, 2), 1));
      }
      if (idx >= 0) {
        let color = GlowColor.get_drawtype(this.color_drawtype,
            DrawType.LineHighlight, highlight, colornode, 2, 0);
        this.ctx.gdraw.text(x1, y1, this.text, ldraw_type, color, idx,
            highlight, lfont, tsize, rot);
      }
    } else if (idx >= 0) {
      let p2 = this.ctx.gdraw.getTextExtent("A", ldraw_type, Math.max(0, idx), lfont);
      z_width = p2.x;
      z_height = p2.y;
      z_descent = z_height / 4;
      this.ctx.gdraw.rect(x1, y1 - (z_height - z_descent), z_width, z_height,
          DrawType.LineGray, false, idx);
    }
  }

  get_borders(t, g) {
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}