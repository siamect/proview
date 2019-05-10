class GrowConPoint extends GlowConPoint {
  arc: GlowArc;
  n_name;
  ll_x: Number;
  ll_y: Number;
  ur_x: Number;
  ur_y: Number;

  constructor(ctx) {
    super(ctx);
    this.arc = new GlowArc(ctx);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowConPoint:
          break;
        case GlowSave.GrowConPoint_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowConPoint_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowConPoint_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowConPoint_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowConPoint_y_low:
          this.ll_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowConPoint_conpoint_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowConPoint_arc:
          i = this.arc.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowConPoint");
          break;
      }
    }

    return i;
  }
}