class GrowAnnot {
  ctx: GrowCtx;
  p: GlowPoint;
  trf: GlowTransform;
  adjustment;
  number;
  draw_type;
  color_drawtype;
  text_size;
  display_level;
  annot_type;
  font;
  protect;

  constructor(ctx) {
    this.ctx = ctx;
    this.p = new GlowPoint();
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

      if (this.ctx.debug) {
        console.log("GrowArc : " + lines[i]);
      }

      switch (key) {
        case GlowSave.GrowAnnot:
          break;
        case GlowSave.GrowAnnot_adjustment:
          this.adjustment = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowAnnot_annot_part:
          i = this.glowannot_open(lines, i + 1);
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

  glowannot_open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowArc : " + lines[i]);
      }

      switch (key) {
        case GlowSave.Annot:
          break;
        case GlowSave.Annot_number:
          this.number = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_color_drawtype:
          this.color_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_display_level:
          this.display_level = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_p:
          i = this.p.open(lines, i + 1);
          break;
        case GlowSave.Annot_annot_type:
          this.annot_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_font:
          this.font = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_protect:
          this.protect = parseInt(tokens[1], 10);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowAnnot");
          break;
      }
    }

    return i;
  }

  draw() {
    this.tdraw(null, 0, 0, null, null);
  }

  tdraw(t, highlight, hot, node, colornode) {
    let x1, y1;
    if (this.ctx.nodraw !== 0) {
      return;
    }

    if (node.annotv[this.number] === null) {
      return;
    }

    let color;
    let rot;
    let offset_x = 0;
    let offset_y = 0;
    let trf_scale = this.trf.vertical_scale(t);
    let idx = Math.floor(trf_scale * this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 3);
    let tsize = trf_scale * this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (8 + 2 * this.text_size);
    if (idx < 0) {
      return;
    }
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);

    let lfont;
    let ldraw_type;

    if (node !== null && node.annot_scrollingtext === this.number) {
      offset_x = node.annot_offset_x;
      offset_y = node.annot_offset_y;
    }

    if (node !== null && node.text_font !== Font.No) {
      lfont = node.text_font;
      ldraw_type = node.text_type;
    } else {
      lfont = this.font;
      ldraw_type = this.draw_type;
    }

    if (t === null) {
      x1 = Math.floor((this.trf.x(this.p.x, this.p.y) + offset_x) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor((this.trf.y(this.p.x, this.p.y) + offset_y) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot());
    } else {
      x1 = Math.floor((this.trf.x(t, this.p.x, this.p.y) + offset_x) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor((this.trf.y(t, this.p.x, this.p.y) + offset_y) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot(t));
    }
    rot = rot < 0 ? rot % 360 + 360 : rot % 360;

    switch (this.annot_type) {
      case AnnotType.OneLine:
        let width = 0;
        let height = 0;
        let descent = 0;

        color = GlowColor.get_drawtype(this.color_drawtype,
            DrawType.LineHighlight, highlight, colornode, 2, 0);

        if (((rot < 45 || rot >= 315) &&
                (node.annotv_inputmode[this.number] !== 0 && node.input_selected !==
                    0)) || (!(rot < 45 || rot >= 315)) ||
            this.adjustment === Adjustment.Right ||
            this.adjustment === Adjustment.Center) {
          let d = this.getTextExtent(t, node);
          width = d.width;
          height = d.height;
          descent = height / 4;
        }

        switch (this.adjustment) {
          case Adjustment.Left:
            break;
          case Adjustment.Right:
            x1 -= width;
            break;
          case Adjustment.Center:
            x1 -= width / 2;
            break;
          default:
            break;
        }

        if (rot < 45 || rot >= 315) {
          if (node.annotv_inputmode[this.number] !== 0 &&
              node.input_selected !== 0) {
            this.ctx.gdraw.fill_rect(x1, y1 - height + descent, width, height,
                DrawType.MediumGray);
          }
        } else {
          // Text is rotated, adjust the coordinates
          if (this.adjustment === Adjustment.Center) {
            // Only center adjustment supports text rotation
            if (45 <= rot && rot < 135) {
              x1 += width / 2;
              y1 += width / 2;
            } else if (135 <= rot && rot < 225) {
              y1 += height - descent;
            } else if (225 <= rot && rot < 315) {
              x1 += width / 2 - height + descent;
              y1 -= width / 2 - height + descent;
            } else {
              x1 -= width;
            }
          } else {
            if (45 <= rot && rot < 135) {
              y1 += height - descent;
            } else if (135 <= rot && rot < 225) {
              x1 -= width;
              y1 += height - descent;
            } else {
              x1 -= width;
            }
            rot = 0;
          }
        }

        this.ctx.gdraw.text(x1, y1, node.annotv[this.number], ldraw_type, color,
            idx, highlight, 0, lfont, tsize, rot);
        break;
      case AnnotType.MultiLine:
        break;
      default:
        break;
    }
  }

  getTextExtent(t, node) {
    let dim = new Rect();

    if (node.annotv[this.number] === null || node.annotv[this.number] === "") {
      dim.width = 0;
      dim.height = 0;
      return dim;
    }

    let z_width, z_height, descent;
    let trf_scale = this.trf.vertical_scale(t);
    let idx = Math.floor(trf_scale * this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 4);
    let tsize = trf_scale * this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (8 + 2 * this.text_size);
    if (idx < 0) {
      dim.width = 0;
      dim.height = 0;
      return dim;
    }
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);

    let d = this.ctx.gdraw.getTextExtent(node.annotv[this.number], idx,
        this.font, this.draw_type);

    dim.width = d.width;
    dim.height = d.height;
    return dim;
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