class GlowLine {
  ctx: GrowCtx;
  p1: GlowPoint;
  p2: GlowPoint;
  draw_type;
  line_width;

  constructor(ctx) {
    this.ctx = ctx;
    this.p1 = new GlowPoint();
    this.p2 = new GlowPoint();
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

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

  draw(highlight, hot) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let p1_x = Math.floor(this.p1.x * this.ctx.mw.zoom_factor_x + 0.5) -
        this.ctx.mw.offset_x;
    let p1_y = Math.floor(this.p1.y * this.ctx.mw.zoom_factor_y + 0.5) -
        this.ctx.mw.offset_y;
    let p2_x = Math.floor(this.p2.x * this.ctx.mw.zoom_factor_x + 0.5) -
        this.ctx.mw.offset_x;
    let p2_y = Math.floor(this.p2.y * this.ctx.mw.zoom_factor_y + 0.5) -
        this.ctx.mw.offset_y;

    if (p1_x === p2_x && p1_y === p2_y) {
      return;
    }
    let idx = Math.floor(this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);
    this.ctx.gdraw.line(p1_x, p1_y, p2_x, p2_y, this.draw_type, idx, highlight);
  }

  draw_shadow(border, shadow, highlight, hot) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let p1_x = Math.floor(this.p1.x * this.ctx.mw.zoom_factor_x + 0.5) -
        this.ctx.mw.offset_x;
    let p1_y = Math.floor(this.p1.y * this.ctx.mw.zoom_factor_y + 0.5) -
        this.ctx.mw.offset_y;
    let p2_x = Math.floor(this.p2.x * this.ctx.mw.zoom_factor_x + 0.5) -
        this.ctx.mw.offset_x;
    let p2_y = Math.floor(this.p2.y * this.ctx.mw.zoom_factor_y + 0.5) -
        this.ctx.mw.offset_y;

    if (p1_x === p2_x && p1_y === p2_y) {
      return;
    }
    let idx = Math.floor(this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);
    let ish = 1;
    let offs = border;

    if (shadow !== 0 && idx > 2) {
      if (p1_x === p2_x) {
        // Vertical line
        this.ctx.gdraw.line(p1_x + idx / 2 - idx + offs, p1_y, p2_x + idx / 2 -
            idx + offs, p2_y,
            GlowColor.shift_drawtype(this.draw_type, -2, null), ish - 1,
            highlight);
        this.ctx.gdraw.line(p1_x + idx / 2 - offs, p1_y, p2_x + idx / 2 - offs,
            p2_y, GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
            highlight);
      } else {
        // Horizontal line
        this.ctx.gdraw.line(p1_x, p1_y + idx / 2 - idx + offs, p2_x, p2_y +
            idx / 2 - idx + offs,
            GlowColor.shift_drawtype(this.draw_type, -2, null), ish - 1,
            highlight);
        this.ctx.gdraw.line(p1_x, p1_y + idx / 2 - offs, p2_x, p2_y + idx / 2 -
            offs, GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
            highlight);
      }
    }
    if (border !== 0) {
      if (p1_x === p2_x) {
        // Vertical line
        this.ctx.gdraw.line(p1_x + idx / 2 - idx, p1_y, p2_x + idx / 2 - idx,
            p2_y, DrawType.Line, 0, highlight);
        if (idx > 0) {
          this.ctx.gdraw.line(p1_x + idx / 2, p1_y, p2_x + idx / 2, p2_y,
              DrawType.Line, 0, highlight);
        }
      } else {
        // Horizontal line
        this.ctx.gdraw.line(p1_x, p1_y + idx / 2 - idx, p2_x, p2_y + idx / 2 -
            idx, DrawType.Line, 0, highlight);
        if (idx > 0) {
          this.ctx.gdraw.line(p1_x, p1_y + idx / 2, p2_x, p2_y + idx / 2,
              DrawType.Line, 0, highlight);
        }
      }
    }
  }
}