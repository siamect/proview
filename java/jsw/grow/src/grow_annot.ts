class GrowAnnot extends GlowAnnot {
  trf: GlowTransform;
  adjustment;

  constructor(ctx) {
    super(ctx);
    this.trf = new GlowTransform();
  }

  getNumber() {
    return this.number;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowAnnot:
          break;
        case GlowSave.GrowAnnot_adjustment:
          this.adjustment = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAnnot_annot_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowAnnot_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowAnnot");
          break;
      }
    }

    return i;
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    if (node.annotv[this.number] === null) {
      return;
    }

    let trf_scale = Matrix.multiply(this.trf, t).vertical_scale();
    let idx = Math.floor(trf_scale * this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 3);
    let tsize = trf_scale * this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (8 + 2 * this.text_size);
    if (idx < 0) {
      return;
    }
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);

    let offset_x = 0;
    let offset_y = 0;
    if (node !== null && node.annot_scrollingtext === this.number) {
      offset_x = node.annot_offset_x;
      offset_y = node.annot_offset_y;
    }

    let lfont, ldraw_type;
    if (node !== null && node.text_font !== Font.No) {
      lfont = node.text_font;
      ldraw_type = node.text_type;
    } else {
      lfont = this.font;
      ldraw_type = this.draw_type;
    }

    let tmp = Matrix.multiply(this.trf, t);
    let p = tmp.apply(this.p);

    let x1 = Math.floor((p.x + offset_x) * this.ctx.mw.zoom_factor_x) -
        this.ctx.mw.offset_x;
    let y1 = Math.floor((p.y + offset_y) * this.ctx.mw.zoom_factor_y) -
        this.ctx.mw.offset_y;
    let rot = t ? this.trf.rotation + t.rotation : this.trf.rotation;
    rot = Math.floor(rot);
    rot = rot < 0 ? rot % 360 + 360 : rot % 360;

    switch (this.annot_type) {
      case AnnotType.OneLine:
        let width = 0;
        let height = 0;
        let descent = 0;

        let color = GlowColor.get_drawtype(this.color_drawtype,
            DrawType.LineHighlight, highlight, colornode, 2, 0);

        if (((rot < 45 || rot >= 315) &&
                (node.annotv_inputmode[this.number] !== 0 && node.input_selected !==
                    0)) || (!(rot < 45 || rot >= 315)) ||
            this.adjustment === Adjustment.Right ||
            this.adjustment === Adjustment.Center) {
          let p = this.getTextExtent(t, node);
          width = p.x;
          height = p.y;
          descent = height / 4;
        }

        if (this.adjustment == Adjustment.Right) {
          x1 -= width;
        } else if (this.adjustment == Adjustment.Center) {
          x1 -= width / 2;
        }

        if (rot < 45 || rot >= 315) {
          if (node.annotv_inputmode[this.number] !== 0 &&
              node.input_selected !== 0) {
            this.ctx.gdraw.rect(x1, y1 - height + descent, width, height,
                DrawType.MediumGray, true, 0);
          }
        } else {
          // Text is rotated, adjust the coordinates
          if (this.adjustment === Adjustment.Center) {
            // Only center adjustment supports text rotation
            if (rot >= 45 && rot < 135) {
              x1 += width / 2;
              y1 += width / 2;
            } else if (rot >= 135 && rot < 225) {
              y1 += height - descent;
            } else if (rot >= 225 && rot < 315) {
              x1 += width / 2 - height + descent;
              y1 -= width / 2 - height + descent;
            } else {
              x1 -= width;
            }
          } else {
            if (rot >= 45 && rot < 135) {
              y1 += height - descent;
            } else if (rot >= 135 && rot < 225) {
              x1 -= width;
              y1 += height - descent;
            } else {
              x1 -= width;
            }
            rot = 0;
          }
        }

        this.ctx.gdraw.text(x1, y1, node.annotv[this.number], ldraw_type, color,
            idx, highlight, lfont, tsize, rot);
        break;
      case AnnotType.MultiLine:
        break;
      default:
        break;
    }
  }

  getTextExtent(t, node): Point {
    if (node.annotv[this.number] === null || node.annotv[this.number] === "") {
      return new Point();
    }

    let trf_scale = Matrix.multiply(this.trf, t).vertical_scale();
    let idx = Math.floor(trf_scale * this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 4);
    let tsize = trf_scale * this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (8 + 2 * this.text_size);
    if (idx < 0) {
      return new Point();
    }
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);

    return this.ctx.gdraw.getTextExtent(node.annotv[this.number], this.draw_type, idx,
        this.font);
  }

  event_handler(event, fx, fy) {
    return 0;
  }

  get_borders(t, g) {
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}