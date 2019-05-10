class GlowNode extends Rect {
  static MAX_CONPOINTS = 32;
  ctx: GrowCtx;
  hot = 0;
  nc = null;
  nc_root = null;
  pos: GlowPoint;
  highlight = 0;
  annotsize = new Array(10);
  annotv = new Array(10);
  nc_name = null;
  n_name = null;
  access = 0;
  cycle = 0;
  ref_object = null;

  constructor(ctx) {
    super();
    this.ctx = ctx;
    this.pos = new GlowPoint();
    for (let i = 0; i < 10; i++) {
      this.annotv[i] = null;
    }
  }

  open(lines, row) {
    let i, j;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.Node:
          break;
        case GlowSave.Node_nc:
          this.nc_name = tokens[1];
          if (!(this instanceof GrowGroup)) {
            this.nc = this.ctx.get_nodeclass_from_name(this.nc_name);
            this.nc_root = this.nc;
            if (this.nc === null) {
              console.log("GlowNode : ", lines[i], ", node class not found: ",
                  this.nc_name);
            }
          }
          break;
        case GlowSave.Node_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.Node_refcon_cnt:
          i += GlowNode.MAX_CONPOINTS;
          break;
        case GlowSave.Node_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case GlowSave.Node_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case GlowSave.Node_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case GlowSave.Node_y_low:
          this.ll_y = parseFloat(tokens[1]);
          break;
        case GlowSave.Node_obst_x_right:
        case GlowSave.Node_obst_x_left:
        case GlowSave.Node_obst_y_high:
        case GlowSave.Node_obst_y_low:
          break;
        case GlowSave.Node_annotsize:
          for (j = 0; j < 10; j++) {
            i++;
            this.annotsize[j] = parseInt(lines[i], 10);
          }
          break;
        case GlowSave.Node_annotv:
          for (j = 0; j < 10; j++) {
            if (this.annotsize[j] !== 0) {
              i++;
              this.annotv[j] = lines[i].substring(1, lines[i].length - 1);
            }
          }
          break;
        case GlowSave.Node_pos:
          i = this.pos.open(lines, i + 1);
          break;
        case GlowSave.Node_trace_data1:
        case GlowSave.Node_trace_data2:
        case GlowSave.Node_trace_data3:
        case GlowSave.Node_trace_data4:
        case GlowSave.Node_trace_data5:
        case GlowSave.Node_trace_data6:
        case GlowSave.Node_trace_data7:
        case GlowSave.Node_trace_data8:
        case GlowSave.Node_trace_data9:
        case GlowSave.Node_trace_data10:
        case GlowSave.Node_trace_attr_type:
        case GlowSave.Node_trace_color:
        case GlowSave.Node_trace_color2:
          break;
        case GlowSave.Node_access:
          this.access = parseInt(tokens[1], 10); // TODO, Can be unsigned
          break;
        case GlowSave.Node_cycle:
          this.cycle = parseInt(tokens[1], 10);
          break;
        case GlowSave.Node_ref_object:
          if (tokens.length > 1) {
            this.ref_object = tokens[1];
          }
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowNode");
          break;
      }
    }

    return i;
  }
}