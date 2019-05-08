class GlowConPoint {
  ctx: GrowCtx;
  trf: GlowTransform;
  p: GlowPoint;
  number;
  direction;
  trace_attribute;
  trace_attr_type;

  constructor(ctx) {
    this.ctx = ctx;
    this.trf = new GlowTransform();
    this.p = new GlowPoint();
  }

  open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowPolyline : " + lines[i]);
      }

      switch (key) {
        case GlowSave.ConPoint:
          break;
        case GlowSave.ConPoint_number:
          this.number = parseInt(tokens[1], 10);
          break;
        case GlowSave.ConPoint_direction:
          this.direction = parseInt(tokens[1], 10);
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

  draw() {
  }

  tdraw(t, highlight, hot, node, colornode) {
  }

  event_handler(event, fx, fy) {
    return 0;
  }

  get_borders(t, g) {
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}