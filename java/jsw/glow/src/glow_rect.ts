class GlowRect {
  ctx: GrowCtx;
  ll: GlowPoint;
  ur: GlowPoint;
  draw_type;
  line_width;
  display_level;
  fill;

  constructor(ctx) {
    this.ctx = ctx;
    this.ll = new GlowPoint();
    this.ur = new GlowPoint();
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowCon : " + lines[i]);
      }

      switch (key) {
        case GlowSave.Rect:
          break;
        case GlowSave.Rect_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Rect_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case GlowSave.Rect_display_level:
          this.display_level = parseInt(tokens[1], 10);
          break;
        case GlowSave.Rect_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case GlowSave.Rect_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case GlowSave.Rect_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowRect");
          break;
      }
    }

    return i;
  }

  draw(highlight, hot) {
  }

  draw_shadow(border, shadow, highlight, hot) {
  }
}