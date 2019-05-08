class GlowConClass {
  ctx: GrowCtx;
  cc_name;
  con_type;
  corner;
  draw_type;
  line_width;
  arrow_width;
  arrow_length;
  round_corner_amount;
  group;

  constructor(ctx) {
    this.ctx = ctx;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowConClass : " + lines[i]);
      }

      switch (key) {
        case GlowSave.ConClass:
          break;

        case GlowSave.ConClass_cc_name:
          if (tokens.length > 1) {
            this.cc_name = tokens[1];
          }
          break;
        case GlowSave.ConClass_con_type:
          this.con_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.ConClass_corner:
          this.corner = parseInt(tokens[1], 10);
          break;
        case GlowSave.ConClass_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.ConClass_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case GlowSave.ConClass_arrow_width:
          this.arrow_width = parseFloat(tokens[1]);
          break;
        case GlowSave.ConClass_arrow_length:
          this.arrow_length = parseFloat(tokens[1]);
          break;
        case GlowSave.ConClass_round_corner_amount:
          this.round_corner_amount = parseFloat(tokens[1]);
          break;
        case GlowSave.ConClass_group:
          this.group = parseInt(tokens[1], 10);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowConClass");
          break;
      }
    }
    return i;
  }
}