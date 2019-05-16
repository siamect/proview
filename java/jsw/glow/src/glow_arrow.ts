class GlowArrow {
  ctx: GrowCtx;
  p_dest: GlowPoint;
  p1: GlowPoint;
  p2: GlowPoint;
  arrow_width: Number;
  arrow_length: Number;
  draw_type: DrawType;
  line_width: Number;

  constructor(ctx) {
    this.ctx = ctx;
    this.p_dest = new GlowPoint();
    this.p1 = new GlowPoint();
    this.p2 = new GlowPoint();
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.Arrow:
          break;
        case GlowSave.Arrow_arrow_width:
          this.arrow_width = Number(tokens[1]);
          break;
        case GlowSave.Arrow_arrow_length:
          this.arrow_length = Number(tokens[1]);
          break;
        case GlowSave.Arrow_draw_type:
          this.draw_type = DrawType(tokens[1]);
          break;
        case GlowSave.Arrow_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case GlowSave.Arrow_p_dest:
          i = this.p_dest.open(lines, i + 1);
          break;
        case GlowSave.Arrow_p1:
          i = this.p1.open(lines, i + 1);
          break;
        case GlowSave.Arrow_p2:
          i = this.p2.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowArrow");
          break;
      }
    }

    return i;
  }
}