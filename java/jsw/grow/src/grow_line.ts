class GrowLine {
  ctx: GrowCtx;
  trf: GlowTransform;
  p1: GlowPoint;
  p2: GlowPoint;
  n_name = null;
  x_right = 0;
  x_left = 0;
  y_high = 0;
  y_low = 0;
  line_type = 0;
  original_border_drawtype;
  dynamicsize = 0;

  draw_type = 0;
  line_width = 0;

  constructor(ctx) {
    this.ctx = ctx;
    this.trf = new GlowTransform();
    this.p1 = new GlowPoint();
    this.p2 = new GlowPoint();
  }

  open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowLine : " + lines[i]);
      }

      switch (key) {
        case GlowSave.GrowLine:
          break;
        case GlowSave.GrowLine_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowLine_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowLine_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowLine_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowLine_y_low:
          this.y_low = parseFloat(tokens[1]);
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
          i = this.glowline_open(lines, i + 1);
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

  glowline_open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowLine : " + lines[i]);
      }

      switch (key) {
        case GlowSave.Line:
          break;
        case GlowSave.Line_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Line_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case GlowSave.Line_p1:
          i = this.p1.open(lines, i + 1);
          break;
        case GlowSave.Line_p2:
          i = this.p2.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowLine");
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
    if (this.ctx.nodraw !== 0) {
      return;
    }

    if (hot !== 9 && this.ctx.hot_indication !== HotIndication.LineWidth) {
      hot = 0;
    }

    let drawtype;
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
    let x1, y1, x2, y2;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.p1.x, this.p1.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.p1.x, this.p1.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.p2.x, this.p2.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.p2.x, this.p2.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    } else {
      x1 = Math.floor(this.trf.x(t, this.p1.x, this.p1.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.p1.x, this.p1.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.p2.x, this.p2.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.p2.x, this.p2.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    }
    if (x1 === x2 && y1 === y2) {
      return;
    }
    drawtype =
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