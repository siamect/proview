class GlowPolyline {
  ctx: GrowCtx;
  a_points: GlowArray;
  points = [];
  draw_type = 0;
  line_width = 0;
  fill = 0;
  closed_line = 0;

  constructor(ctx) {
    this.ctx = ctx;
    this.a_points = new GlowArray(ctx);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.PolyLine:
          break;
        case GlowSave.PolyLine_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.PolyLine_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case GlowSave.PolyLine_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case GlowSave.PolyLine_closed_line:
          this.closed_line = parseInt(tokens[1], 10);
          break;
        case GlowSave.PolyLine_a_points:
          i = this.a_points.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowPolyline");
          break;
      }
    }

    return i;
  }
}