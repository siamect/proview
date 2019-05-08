class GrowConPoint {
  ctx: GrowCtx;
  trf: GlowTransform;
  arc: GlowArc;
  p: GlowPoint;
  n_name;
  x_right;
  x_left;
  y_high;
  y_low;
  number;
  direction;
  trace_attribute;
  trace_attr_type;

  constructor(ctx) {
    this.ctx = ctx;
    this.trf = new GlowTransform();
    this.arc = new GlowArc(ctx);
    this.p = new GlowPoint();
  }

  open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowConPoint : " + lines[i]);
      }

      switch (key) {
        case GlowSave.GrowConPoint:
          break;
        case GlowSave.GrowConPoint_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowConPoint_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowConPoint_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowConPoint_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowConPoint_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowConPoint_conpoint_part:
          i = this.glowconpoint_open(lines, i + 1);
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

  glowconpoint_open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowConPoint : " + lines[i]);
      }

      switch (key) {
        case GlowSave.ConPoint:
          break;
        case GlowSave.ConPoint_number:
          this.number = parseInt(tokens[1], 10);
          break;
        case GlowSave.ConPoint_direction:
          this.number = parseInt(tokens[1], 10);
          break;
        case GlowSave.ConPoint_p:
          i = this.p.open(lines, i + 1);
          break;
        case GlowSave.ConPoint_trace_attr_type:
          this.trace_attr_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.ConPoint_trace_attribute:
          if (tokens.length > 1) {
            this.trace_attribute = tokens[1];
          }
          break;
        case GlowSave.ConPoint_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowConPoint");
          break;
      }
    }
    return i;
  }

  event_handler(event, fx, fy) {
    return 0;
  }

  draw() {
  }

  tdraw(t, highlight, hot, node, colornode) {
  }

  get_borders(t, g) {
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}