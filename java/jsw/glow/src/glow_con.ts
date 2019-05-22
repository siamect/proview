class GlowCon extends Rect {
  static MAX_POINT = 8;
  ctx: GrowCtx;
  line_a: GlowArray;
  arc_a: GlowArray;
  arrow_a: GlowArray;
  ref_a: GlowArray;
  cc_name;
  cc;
  c_name;
  dest_node;
  source_node;
  dest_conpoint;
  source_conpoint;
  source_direction;
  dest_direction;
  p_num;
  l_num;
  a_num;
  arrow_num;
  ref_num;
  point_x = new Array(GlowCon.MAX_POINT);
  point_y = new Array(GlowCon.MAX_POINT);
  temporary_ref;
  source_ref_cnt;
  dest_ref_cnt;
  trace_object;
  trace_attribute;
  trace_attr_type;
  border;
  shadow;
  highlight = 0;
  hot = 0;

  constructor(ctx) {
    super();
    this.ctx = ctx;
    this.line_a = new GlowArray(ctx);
    this.arc_a = new GlowArray(ctx);
    this.arrow_a = new GlowArray(ctx);
    this.ref_a = new GlowArray(ctx);
  }

  open(lines, row) {
    let i;
    let name;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.Con:
          break;
        case GlowSave.Con_cc:
          this.cc_name = tokens[1];
          this.cc = this.ctx.get_conclass_from_name(this.cc_name);
          if (this.cc === null) {
            console.log("GlowCon : ", lines[i], ", con class not found: ",
                this.cc_name);
          }
          break;
        case GlowSave.Con_c_name:
          if (tokens.length > 1) {
            this.c_name = tokens[1];
          }
          break;
        case GlowSave.Con_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case GlowSave.Con_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case GlowSave.Con_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case GlowSave.Con_y_low:
          this.ll_y = parseFloat(tokens[1]);
          break;
        case GlowSave.Con_dest_node:
          if (tokens.length > 1) {
            name = tokens[1];
            this.dest_node = this.ctx.get_node_from_name(name);
            if (this.dest_node === null) {
              console.log("GlowCon:dest_node not found");
            }
          }
          break;
        case GlowSave.Con_source_node:
          if (tokens.length > 1) {
            name = tokens[1];
            this.source_node = this.ctx.get_node_from_name(name);
            if (this.source_node === null) {
              console.log("GlowCon:source_node not found");
            }
          }
          break;
        case GlowSave.Con_dest_conpoint:
          this.dest_conpoint = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_source_conpoint:
          this.source_conpoint = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_dest_direction:
          this.dest_direction = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_source_direction:
          this.source_direction = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_line_a:
          i = this.line_a.open(lines, i + 1);
          break;
        case GlowSave.Con_arc_a:
          i = this.arc_a.open(lines, i + 1);
          break;
        case GlowSave.Con_arrow_a:
          i = this.arrow_a.open(lines, i + 1);
          break;
        case GlowSave.Con_ref_a:
          i = this.ref_a.open(lines, i + 1);
          break;
        case GlowSave.Con_p_num:
          this.p_num = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_l_num:
          this.l_num = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_a_num:
          this.a_num = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_arrow_num:
          this.arrow_num = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_ref_num:
          this.ref_num = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_point_x:
          for (let j = 0; j < this.p_num; j++) {
            this.point_x[j] = parseFloat(lines[i]);
            i++;
          }
          break;
        case GlowSave.Con_point_y:
          for (let j = 0; j < this.p_num; j++) {
            this.point_y[j] = parseFloat(lines[i]);
            i++;
          }
          break;
        case GlowSave.Con_source_ref_cnt:
          this.source_ref_cnt = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_dest_ref_cnt:
          this.dest_ref_cnt = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_trace_object:
          if (tokens.length > 1) {
            this.trace_object = tokens[1];
          }
          break;
        case GlowSave.Con_trace_attribute:
          if (tokens.length > 1) {
            this.trace_attribute = tokens[1];
          }
          break;
        case GlowSave.Con_trace_attr_type:
          this.trace_attr_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_temporary_ref:
          this.temporary_ref = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case GlowSave.Con_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowCon");
          break;
      }
    }

    return i;
  }

  draw(t = null, highlight = this.highlight, hot = this.hot, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    if (this.temporary_ref !== 0 ||
        this.cc.con_type === ConType.Reference) {
      // ref_a.draw( w, &cc->zero, highlight, hot, NULL);
    } else {
      let ltmp = this.line_a.slice(0, this.l_num);
      let atmp = this.arc_a.slice(0, this.a_num);
      ltmp.forEach(e => e.draw(highlight, hot));
      atmp.forEach(e => e.draw(highlight, hot));
      // arrow_a.draw( highlight, hot);
      if ((this.shadow !== 0 || this.border !== 0) &&
          this.cc.con_type === ConType.Routed &&
          this.cc.corner === Corner.Rounded) {
        ltmp.forEach(e => e.draw_shadow(this.border, this.shadow, highlight, hot));
        atmp.forEach(e => e.draw_shadow(this.border, this.shadow, highlight, hot));
      }
    }
  }

  static getName() {
    return "Con";
  }

  event_handler(event, fx, fy) {
    return 0;
  }
}