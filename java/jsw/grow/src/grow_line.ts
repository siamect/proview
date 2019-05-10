class GrowLine extends GlowLine {
  trf: GlowTransform;
  n_name = null;
  ll_x: Number;
  ll_y: Number;
  ur_x: Number;
  ur_y: Number;
  line_type = 0;
  original_border_drawtype;
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
        case GlowSave.GrowLine:
          break;
        case GlowSave.GrowLine_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowLine_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowLine_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowLine_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowLine_y_low:
          this.ll_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowLine_line_type:
          this.line_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowLine_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowLine_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowLine_dynamic:
          i += this.dynamicsize;
          break;
        case GlowSave.GrowLine_line_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowLine_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowLine");
          break;
      }
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

    if (hot !== 9 && this.ctx.hot_indication !== HotIndication.LineWidth) {
      hot = 0;
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

    let x1 = Math.floor(this.trf.x(t, this.p1.x, this.p1.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
    let y1 = Math.floor(this.trf.y(t, this.p1.x, this.p1.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    let x2 = Math.floor(this.trf.x(t, this.p2.x, this.p2.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
    let y2 = Math.floor(this.trf.y(t, this.p2.x, this.p2.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;

    if (x1 === x2 && y1 === y2) {
      return;
    }

    let drawtype =
        GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
            highlight, colornode, 0, 0);

    if (this.line_type === LineType.Solid) {
      this.ctx.gdraw.line(x1, y1, x2, y2, drawtype, idx, 0);
    } else {
      this.ctx.gdraw.line_dashed(x1, y1, x2, y2, drawtype, idx, 0,
          this.line_type);
    }
  }

  get_borders(t, g) {
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}