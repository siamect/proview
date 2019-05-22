class GlowRect {
  ctx: GrowCtx;
  ll: GlowPoint;
  ur: GlowPoint;
  draw_type;
  line_width;
  display_level;
  fill: boolean;

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
          this.fill = Boolean(parseInt(tokens[1], 10));
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

  draw(pos, highlight = 0, hot = 0, node = null) {
    let idx = this.line_width + hot;
    idx = clamp(idx, 0, DRAW_TYPE_SIZE - 1);
    this.ctx.gdraw.rect(this.ll.x + pos.x, this.ll.y + pos.y,
        this.ur.x - this.ll.x, this.ur.y - this.ll.y,
        this.draw_type, this.fill, idx * this.fill, highlight);
  }
}