class GrowNode {
  MAX_CONPOINTS = 32;
  ctx: GrowCtx;
  trf: GlowTransform;
  pos: GlowPoint;
  original_border_drawtype = DrawType.No;
  border_drawtype = 0;
  background_drawtype = DrawType.No;
  original_fill_drawtype = DrawType.No;
  original_text_drawtype = DrawType.No;
  original_background_drawtype = DrawType.No;
  original_color_tone = DrawType.No;
  original_color_lightness = 0;
  original_color_intensity = 0;
  original_color_shift = 0;
  color_inverse = 0;
  color_lightness = 0;
  color_intensity = 0;
  color_shift = 0;
  color_tone = DrawType.No;
  fill_drawtype = DrawType.No;
  text_drawtype = 0;
  draw_type = 0;
  dimmed = 0;
  invisible = 0;
  flip_horizontal = 0;
  flip_vertical = 0;
  dynamicsize = 0;
  arg_cnt = 0;
  line_width = 0;
  shadow = 0;
  gradient = 0;
  text_type = DrawType.TextHelvetica;
  text_font = Font.No;
  disable_cb = 0;
  nc_name = null;
  nc = null;
  nc_root = null;
  n_name = null;
  x_right = 0;
  x_left = 0;
  y_high = 0;
  y_low = 0;
  annotsize = new Array(10);
  annotv = new Array(10);
  access = 0;
  cycle = 0;
  ref_object = null;
  userdata = null;
  visible = 1;
  highlight = 0;
  hot = 0;
  input_selected = 0;
  annotv_inputmode = [];
  fill_level = 1;
  level_fill_drawtype = DrawType.No;
  level_color_tone = DrawTone.No;
  level_direction = 0;
  annot_scrollingtext = -1;
  annot_offset_x = 0;
  annot_offset_y = 0;

  constructor(ctx) {
    this.ctx = ctx;
    this.trf = new GlowTransform();
    this.pos = new GlowPoint();
    for (let i = 0; i < 10; i++) {
      this.annotv[i] = null;
    }
  }

  open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowNode : " + lines[i]);
      }

      switch (key) {
        case GlowSave.GrowNode:
          break;
        case GlowSave.GrowNode_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_original_fill_drawtype:
          this.original_fill_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_fill_drawtype:
          this.fill_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_text_drawtype:
          this.text_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_original_text_drawtype:
          this.original_text_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_original_color_tone:
          this.original_color_tone = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_original_background_drawtype:
          this.original_background_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_background_drawtype:
          this.background_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_color_tone:
          this.color_tone = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_original_color_lightness:
          this.original_color_lightness = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_color_lightness:
          this.color_lightness = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_original_color_intensity:
          this.original_color_intensity = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_color_intensity:
          this.color_intensity = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_original_color_shift:
          this.original_color_shift = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_color_shift:
          this.color_shift = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_invisible:
          this.invisible = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_flip_horizontal:
          this.flip_horizontal = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_flip_vertical:
          this.flip_vertical = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case GlowSave.GrowNode_node_part:
          i = this.glownode_open(lines, i + 1);
          break;
        case GlowSave.GrowNode_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case GlowSave.GrowNode_arg_cnt:
          this.arg_cnt = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_argsize:
          i += this.arg_cnt;
          break;
        case GlowSave.GrowNode_argv:
          i += this.arg_cnt;
          break;
        case GlowSave.GrowNode_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_gradient:
          this.gradient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_text_type:
          this.text_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_text_font:
          this.text_font = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_disable_cb:
          this.disable_cb = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowNode_userdata_cb:
          if (this.ctx.appl !== null) {
            let ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
                UserdataCbType.Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowNode");
          break;
      }
    }
    return i;
  }

  glownode_open(lines, row) {
    let i, j;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowNode : " + lines[i]);
      }

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
          i += this.MAX_CONPOINTS;
          break;
        case GlowSave.Node_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case GlowSave.Node_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case GlowSave.Node_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case GlowSave.Node_y_low:
          this.y_low = parseFloat(tokens[1]);
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
            if (this.ctx.debug) {
              console.log("GlowNode  annotsize: ", this.annotsize[j]);
            }
          }
          break;
        case GlowSave.Node_annotv:
          for (j = 0; j < 10; j++) {
            if (this.annotsize[j] !== 0) {
              i++;
              this.annotv[j] = lines[i].substring(1, lines[i].length - 1);
              if (this.ctx.debug) {
                console.log("GrowNode  annotv: ", this.annotv[j]);
              }
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

  event_handler(event, fx, fy) {
    if (this.visible === 0 || this.dimmed !== 0) {
      return 0;
    }

    switch (event.event) {
      case Event.CursorMotion:
        return 0;
      default:
        break;
    }

    let rp = this.trf.reverse(fx, fy);
    let sts = this.nc.event_handler(event, rp.x, rp.y);
    if (sts !== 0) {
      if (!(this instanceof GrowGroup && this.ctx.callback_object !== null)) {
        this.ctx.register_callback_object(ObjectType.Node, this);
      }
      return sts;
    }
    return 0;
  }

  draw() {
    if (this.visible !== 0) {
      this.tdraw(null, this.highlight, this.hot, null, null);
    }
  }

  tdraw(t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    if (node === null) {
      node = this;
    }
    if (colornode === null) {
      colornode = this;
    } else if (colornode.fill_drawtype === DrawType.No &&
        colornode.color_tone === DrawTone.No) {
      colornode = this;
    }

    if (node.invisible !== 0 || this.invisible !== 0) {
      return;
    }

    if (this.annot_scrollingtext !== -1) {
      this.ctx.gdraw.set_clip_rectangle(this.x_left *
          this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x, this.y_low *
          this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y, this.x_right *
          this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x, this.y_high *
          this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y);
    }

    let trf_tot;
    if (this.fill_level === 1) {
      if (t === null) {
        this.nc.tdraw(this.trf, highlight, hot, node, node);
      } else {
        trf_tot = t.multiply(this.trf);

        this.nc.tdraw(trf_tot, highlight, hot, this, this);
      }
    } else {
      let x1, x2, y1, y2;
      let x_level = 0;
      let y_level = 0;
      let clip_sts = 0;
      let old_color_tone = 0;
      let old_fill_drawtype = 0;

      if (t === null) {
        x1 = Math.floor(this.x_left * this.ctx.mw.zoom_factor_x + 0.5) -
            this.ctx.mw.offset_x;
        y1 = Math.floor(this.y_low * this.ctx.mw.zoom_factor_y + 0.5) -
            this.ctx.mw.offset_y;
        x2 = Math.floor(this.x_right * this.ctx.mw.zoom_factor_x + 0.5) -
            this.ctx.mw.offset_x;
        y2 = Math.floor(this.y_high * this.ctx.mw.zoom_factor_y + 0.5) -
            this.ctx.mw.offset_y;
      } else {
        x1 = Math.floor(t.x(this.x_left, this.y_low) * this.ctx.mw.zoom_factor_x + 0.5) -
            this.ctx.mw.offset_x;
        y1 = Math.floor(t.y(this.x_left, this.y_low) * this.ctx.mw.zoom_factor_y + 0.5) -
            this.ctx.mw.offset_y;
        x2 =
            Math.floor(t.x(this.x_right, this.y_high) * this.ctx.mw.zoom_factor_x + 0.5) -
            this.ctx.mw.offset_x;
        y2 =
            Math.floor(t.y(this.x_right, this.y_high) * this.ctx.mw.zoom_factor_y + 0.5) -
            this.ctx.mw.offset_y;
      }
      switch (this.level_direction) {
        case Direction.Right:
          x_level = Math.floor(this.fill_level * (x2 - x1) + 0.5);
          clip_sts =
              this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 - 1, x1 + x_level, y2 +
                  1);
          break;
        case Direction.Left:
          x_level = Math.floor((1 - this.fill_level) * (x2 - x1) + 0.5);
          clip_sts =
              this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 - 1, x1 + x_level, y2 +
                  1);
          if (this.level_color_tone !== DrawTone.No) {
            old_color_tone = this.color_tone;
            this.color_tone = this.level_color_tone;
          } else if (this.level_fill_drawtype !== DrawType.No) {
            old_fill_drawtype = this.fill_drawtype;
            this.fill_drawtype = this.level_fill_drawtype;
          }
          break;
        case Direction.Up:
          y_level = Math.floor(this.fill_level * (y2 - y1) + 0.5);
          clip_sts =
              this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 - 1, x2 + 1, y1 +
                  y_level);
          break;
        case Direction.Down:
          y_level = Math.floor((1 - this.fill_level) * (y2 - y1) + 0.5);
          clip_sts =
              this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 - 1, x2 + 1, y1 +
                  y_level);
          if (this.level_color_tone !== DrawTone.No) {
            old_color_tone = this.color_tone;
            this.color_tone = this.level_color_tone;
          } else if (this.level_fill_drawtype !== DrawType.No) {
            old_fill_drawtype = this.fill_drawtype;
            this.fill_drawtype = this.level_fill_drawtype;
          }
          break;
        default:
          break;
      }

      if (t === null) {
        this.nc.tdraw(this.trf, highlight, hot, node, node);
      } else {
        trf_tot = t.multiply(this.trf);

        // If this node has a trace pointer, use colors for this node
        this.nc.tdraw(trf_tot, highlight, hot, this, this);
      }
      if ((clip_sts & 1) !== 0) {
        this.ctx.gdraw.reset_clip_rectangle();
      }

      switch (this.level_direction) {
        case Direction.Right:
          if (this.level_color_tone !== DrawTone.No) {
            old_color_tone = this.color_tone;
            this.color_tone = this.level_color_tone;
          } else if (this.level_fill_drawtype !== DrawType.No) {
            old_fill_drawtype = this.fill_drawtype;
            this.fill_drawtype = this.level_fill_drawtype;
          }
          clip_sts =
              this.ctx.gdraw.set_clip_rectangle(x1 + x_level, y1 - 1, x2 + 1, y2 +
                  1);
          break;
        case Direction.Left:
          if (this.level_color_tone !== DrawTone.No) {
            this.color_tone = old_color_tone;
          } else if (this.level_fill_drawtype !== DrawType.No) {
            this.fill_drawtype = old_fill_drawtype;
          }
          clip_sts =
              this.ctx.gdraw.set_clip_rectangle(x1 + x_level, y1 - 1, x2 + 1, y2 +
                  1);
          break;
        case Direction.Up:
          if (this.level_color_tone !== DrawTone.No) {
            old_color_tone = this.color_tone;
            this.color_tone = this.level_color_tone;
          } else if (this.level_fill_drawtype !== DrawType.No) {
            old_fill_drawtype = this.fill_drawtype;
            this.fill_drawtype = this.level_fill_drawtype;
          }
          clip_sts =
              this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 + y_level, x2 + 1, y2 +
                  1);
          break;
        case Direction.Down:
          if (this.level_color_tone !== DrawTone.No) {
            this.color_tone = old_color_tone;
          } else if (this.level_fill_drawtype !== DrawType.No) {
            this.fill_drawtype = old_fill_drawtype;
          }
          clip_sts =
              this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 + y_level, x2 + 1, y2 +
                  1);
          break;
        default:
          break;
      }

      if (t === null) {
        this.nc.tdraw(this.trf, highlight, hot, node, node);
      } else {
        trf_tot = t.multiply(this.trf);

        // If this node has a trace pointer, use colors for this node
        this.nc.tdraw(trf_tot, highlight, hot, this, this);
      }
      if ((clip_sts & 1) !== 0) {
        this.ctx.gdraw.reset_clip_rectangle();
      }

      switch (this.level_direction) {
        case Direction.Right:
          if (this.level_color_tone !== DrawTone.No) {
            this.color_tone = old_color_tone;
          } else if (this.level_fill_drawtype !== DrawType.No) {
            this.fill_drawtype = old_fill_drawtype;
          }
          break;
        case Direction.Left:
          break;
        case Direction.Up:
          if (this.level_color_tone !== DrawTone.No) {
            this.color_tone = old_color_tone;
          } else if (this.level_fill_drawtype !== DrawType.No) {
            this.fill_drawtype = old_fill_drawtype;
          }
          break;
        case Direction.Down:
          break;
        default:
          break;
      }
    }

    if (this.annot_scrollingtext !== -1) {
      this.ctx.gdraw.reset_clip_rectangle();
    }
  }

  set_position(x, y) {
    let old_x_left, old_x_right, old_y_low, old_y_high;

    if (this.trf.a13 === this.trf.s_a13 + x &&
        this.trf.a23 === this.trf.s_a23 + y) {
      return;
    }
    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;
    this.trf.move_from_stored(x, y);
    this.get_node_borders();
    this.ctx.draw();
  }

  move(x, y) {
    let old_x_left = this.x_left;
    let old_x_right = this.x_right;
    let old_y_low = this.y_low;
    let old_y_high = this.y_high;

    this.trf.move(x, y);
    this.get_node_borders();

    this.ctx.draw();
  }

  move_to(x, y) {
    this.trf.move(x - this.x_left, y - this.y_low);
    this.get_node_borders();

    this.ctx.draw();
  }

  set_scale_pos(x, y, scale_x, scale_y, x0, y0, type) {
    let old_x_left, old_x_right, old_y_low, old_y_high;

    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;

    this.ctx.setNodraw();
    this.set_scale(scale_x, scale_y, x0, y0, type);
    this.ctx.resetNodraw();
    this.trf.move(x - this.x_left, y - this.y_low);
    this.get_node_borders();
  }

  set_scale(scale_x, scale_y, x0, y0, type) {
    let old_x_left, old_x_right, old_y_low, old_y_high;

    if (!((scale_x === -1 && scale_y === 1) || (scale_x === 1 && scale_y === -1))) {
      if (scale_x < 0) {
        scale_x = 0;
      }
      if (scale_y < 0) {
        scale_y = 0;
      }
    }

    if (this.trf.s_a11 !== 0 && this.trf.s_a22 !== 0 &&
        Math.abs(scale_x - this.trf.a11 / this.trf.s_a11) < Number.MIN_VALUE &&
        Math.abs(scale_y - this.trf.a22 / this.trf.s_a22) < Number.MIN_VALUE) {
      return;
    }

    switch (type) {
      case ScaleType.LowerLeft:
        x0 = this.x_left;
        y0 = this.y_low;
        break;
      case ScaleType.LowerRight:
        x0 = this.x_right;
        y0 = this.y_low;
        break;
      case ScaleType.UpperRight:
        x0 = this.x_right;
        y0 = this.y_high;
        break;
      case ScaleType.UpperLeft:
        x0 = this.x_left;
        y0 = this.y_high;
        break;
      case ScaleType.FixPoint:
        break;
      case ScaleType.Center:
        x0 = (this.x_left + this.x_right) / 2;
        y0 = (this.y_low + this.y_high) / 2;
        break;
      default:
    }

    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;
    this.trf.scale_from_stored(scale_x, scale_y, x0, y0);
    this.get_node_borders();

    this.ctx.rdraw(old_x_left * this.ctx.mw.zoom_factor_x -
        this.ctx.mw.offset_x - DRAW_MP, old_y_low *
        this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
        DRAW_MP, old_x_right * this.ctx.mw.zoom_factor_x -
        this.ctx.mw.offset_x + DRAW_MP, old_y_high *
        this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + DRAW_MP);
    this.ctx.rdraw(this.x_left * this.ctx.mw.zoom_factor_x -
        this.ctx.mw.offset_x - DRAW_MP, this.y_low *
        this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
        DRAW_MP, this.x_right * this.ctx.mw.zoom_factor_x -
        this.ctx.mw.offset_x + DRAW_MP, this.y_high *
        this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + DRAW_MP);
  }

  setRotation(angel, x0, y0, type) {
    let old_x_left, old_x_right, old_y_low, old_y_high;
    let t = new GlowTransform();

    if (Math.abs(angel - this.trf.rotation + this.trf.s_rotation) <
        Number.MIN_VALUE) {
      return;
    }

    switch (type) {
      case RotationPoint.LowerLeft:
        x0 = this.x_left;
        y0 = this.y_low;
        break;
      case RotationPoint.LowerRight:
        x0 = this.x_right;
        y0 = this.y_low;
        break;
      case RotationPoint.UpperRight:
        x0 = this.x_right;
        y0 = this.y_high;
        break;
      case RotationPoint.UpperLeft:
        x0 = this.x_left;
        y0 = this.y_high;
        break;
      case RotationPoint.Center:
        x0 = (this.x_left + this.x_right) / 2;
        y0 = (this.y_high + this.y_low) / 2;
        break;
      case RotationPoint.Zero:
        x0 = this.trf.s_a13;
        y0 = this.trf.s_a23;
        break;
      default:
        break;
    }

    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;

    t.rotate(angel, x0, y0);
    this.trf.set_from_stored(t);
    this.get_node_borders();
  }

  storeTransform() {
    this.trf.store();
  }

  transformIsStored() {
    return this.trf.is_stored();
  }

  set_next_nodeclass() {
    if (this.nc.next_nc !== null) {
      this.set_nodeclass(this.nc.next_nc);
      return 1;
    }
    return 0;
  }

  set_first_nodeclass() {
    this.set_root_nodeclass();
  }

  set_last_nodeclass() {
    let next;
    for (next = this.nc_root; next.next_nc !== null; next = next.next_nc) {
    }
    if (next !== this.nc) {
      this.set_nodeclass(next);
    }
  }

  set_nodeclass_by_index(idx) {
    let next;
    let i;

    next = this.nc_root;
    for (i = 0; i < idx; i++) {
      if (next.next_nc === null) {
        return 0;
      }
      next = next.next_nc;
    }
    if (next !== this.nc) {
      this.set_nodeclass(next);
    }
    return 1;
  }

  set_previous_nodeclass() {
    if (this.nc.prev_nc !== null) {
      this.set_nodeclass(this.nc.prev_nc);
      return 1;
    }
    return 0;
  }

  set_root_nodeclass() {
    if (this.nc_root !== this.nc) {
      this.set_nodeclass(this.nc_root);
    }
  }

  get_animation_count() {
    return this.nc.animation_count;
  }

  get_nodeclass_dyn_attr1() {
    return this.nc.dyn_attr[0];
  }

  set_nodeclass(new_nc) {
    let old_x_left, old_x_right, old_y_low, old_y_high;

    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;

    this.nc = new_nc;

    this.get_node_borders();
    old_x_left = Math.min(old_x_left, this.x_left);
    old_x_right = Math.max(old_x_right, this.x_right);
    old_y_low = Math.min(old_y_low, this.y_low);
    old_y_high = Math.max(old_y_high, this.y_high);
    this.ctx.draw();
  }

  measure() {
    return new Rect(this.x_left, this.y_low, this.x_right - this.x_left, this.y_high - this.y_low);
  }

  getLimits() {
    let limits = new GlowFillLevelLimits();

    if (this.nc.y0 === 0 && this.nc.y1 === 0) {
      limits.status = 0;
      return limits;
    }
    if (!(this.nc.x0 === 0 && this.nc.x1 === 0)) {
      limits.status = 0;
      return limits;
    }

    let x1, x2, y1, y2;
    let rotation;

    // Calculate max and min coordinates

    x1 = this.trf.x(0, this.nc.y0);
    y1 = this.trf.y(0, this.nc.y0);
    x2 = this.trf.x(0, this.nc.y1);
    y2 = this.trf.y(0, this.nc.y1);

    rotation = (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) * 360;

    if (45 >= rotation || rotation > 315) {
      limits.direction = Direction.Down;
      limits.min = y1;
      limits.max = y2;
    } else if (45 < rotation && rotation <= 135) {
      limits.direction = Direction.Right;
      limits.min = x2;
      limits.max = x1;
    } else if (135 < rotation && rotation <= 225) {
      limits.direction = Direction.Up;
      limits.min = y2;
      limits.max = y1;
    } else if (225 < rotation && rotation <= 315) {
      limits.direction = Direction.Left;
      limits.min = x1;
      limits.max = x2;
    }
    limits.status = 1;
    return limits;
  }

  get_node_borders() {
    let g = new Rect();

    g.x = 10.0e37;
    g.y = 10.0e37;
    g.width = 10.0e-37;
    g.height = 10.0e-37;

    this.nc.get_borders(this.trf, g);
    this.x_left = g.x;
    this.x_right = g.x + g.width;
    this.y_low = g.y;
    this.y_high = g.y + g.height;
  }

  get_borders(t, g) {
    if (t === null) {
      this.nc.get_borders(this.trf, g);
    } else {
      let t2 = t.multiply(this.trf);
      this.nc.get_borders(t2, g);
    }
  }

  get_background_object_limits(t, type, x, y, bo) {
    let dyn_type;
    let b = bo;
    let base_nc = this.nc.get_base_nc();
    dyn_type = base_nc.dyn_type1;

    if ((type & dyn_type) === 0) {
      return 0;
    }
    if (this.nc.y0 === 0 && this.nc.y1 === 0) {
      return 0;
    }

    let x1, x2, y1, y2;
    let rotation;
    let g = new Rect();

    g.x = 10.0e37;
    g.y = 10.0e37;
    g.width = 10.0e-37;
    g.height = 10.0e-37;

    this.get_borders(t, g);
    let x1_right = g.x + g.width;
    let x1_left = g.x;
    let y1_high = g.y + g.height;
    let y1_low = g.y;

    if (x <= x1_right && x >= x1_left && y <= y1_high && y >= y1_low) {
      // Hit, calculate max and min koordinates

      if (t === null) {
        x1 = this.trf.x(0, this.nc.y0);
        y1 = this.trf.y(0, this.nc.y0);
        x2 = this.trf.x(0, this.nc.y1);
        y2 = this.trf.y(0, this.nc.y1);
      } else {
        x1 = this.trf.x(t, 0, this.nc.y0);
        y1 = this.trf.y(t, 0, this.nc.y0);
        x2 = this.trf.x(t, 0, this.nc.y1);
        y2 = this.trf.y(t, 0, this.nc.y1);
      }

      if (t === null) {
        rotation =
            (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) * 360;
      } else {
        rotation =
            (this.trf.rot(t) / 360 - Math.floor(this.trf.rot(t) / 360)) * 360;
      }

      if (45 >= rotation || rotation > 315) {
        b.direction = Direction.Down;
        b.min = y1;
        b.max = y2;
      } else if (45 < rotation && rotation <= 135) {
        b.direction = Direction.Left;
        b.min = x2;
        b.max = x1;
      } else if (135 < rotation && rotation <= 225) {
        b.direction = Direction.Up;
        b.min = y2;
        b.max = y1;
      } else if (225 < rotation && rotation <= 315) {
        b.direction = Direction.Right;
        b.min = x1;
        b.max = x2;
      }
      b.background = this;

      return 1;
    }
    return 0;
  }

  getColorInverse() {
    return this.color_inverse;
  }

  getColorLightness() {
    return this.color_lightness;
  }

  getColorIntensity() {
    return this.color_intensity;
  }

  getColorShift() {
    return this.color_shift;
  }

  getColorTone() {
    return this.color_tone;
  }

  getFillDrawtype() {
    return this.fill_drawtype;
  }

  getTextDrawtype() {
    return this.text_drawtype;
  }

  getBackgroundDrawtype() {
    return this.background_drawtype;
  }

  getDrawtype() {
    return this.draw_type;
  }

  getDimmed() {
    return this.dimmed;
  }

  getClassTraceColor1() {
    return this.nc.dyn_color[0];
  }

  getClassTraceColor2() {
    return this.nc.dyn_color[1];
  }

  getClassDynType1() {
    return this.nc.dyn_type1;
  }

  getClassDynType2() {
    return this.nc.dyn_type2;
  }

  getClassActionType1() {
    return this.nc.action_type1;
  }

  getClassActionType2() {
    return this.nc.action_type2;
  }

  getClassCycle() {
    return this.nc.cycle;
  }

  getClassUserData() {
    return this.nc.userdata;
  }

  getUserData() {
    return this.userdata;
  }

  setColorTone(color_tone) {
    this.color_tone = color_tone;
  }

  resetColorTone() {
    this.color_tone = this.original_color_tone;
  }

  setFillColor(fill_drawtype) {
    this.fill_drawtype = fill_drawtype;
    this.color_tone = DrawTone.No;
    this.color_shift = 0;
    this.color_lightness = 0;
    this.color_intensity = 0;
    this.draw();
  }

  resetFillColor() {
    this.fill_drawtype = this.original_fill_drawtype;
    this.color_tone = this.original_color_tone;
    this.color_shift = this.original_color_shift;
    this.color_lightness = this.original_color_lightness;
    this.color_intensity = this.original_color_intensity;
  }

  setBorderColor(draw_type) {
    this.draw_type = draw_type;
    this.color_tone = DrawTone.No;
    this.color_shift = 0;
    this.color_lightness = 0;
    this.color_intensity = 0;
  }

  resetBorderColor() {
    this.draw_type = this.original_border_drawtype;
  }

  setTextColor(draw_type) {
    this.text_drawtype = draw_type;
    this.color_tone = DrawTone.No;
    this.color_shift = 0;
    this.color_lightness = 0;
    this.color_intensity = 0;
  }

  resetTextColor() {
    this.text_drawtype = this.original_text_drawtype;
  }

  setBackgroundColor(draw_type) {
    this.background_drawtype = draw_type;
  }

  resetBackgroundColor() {
    this.background_drawtype = this.original_background_drawtype;
  }

  setColorInverse(color_inverse) {
    this.color_inverse = color_inverse;
    this.draw();  // TODO...
  }

  setLevelFillColor(color) {
    this.level_fill_drawtype = color;
  }

  setLevelColorTone(tone) {
    this.level_color_tone = tone;
  }

  setLevelDirection(level_direction) {
    this.level_direction = level_direction;
  }

  setFillLevel(fill_level) {
    this.fill_level = fill_level;
  }

  setVisibility(visibility) {
    switch (visibility) {
      case Vis.Visible:
        this.visible = 1;
        this.dimmed = 0;
        break;
      case Vis.Invisible:
        this.visible = 0;
        this.dimmed = 0;
        this.draw();
        break;
      case Vis.Dimmed:
        this.visible = 1;
        this.dimmed = 1;
        this.draw();
        break;
      default:
        break;
    }
  }

  getAnnotation(number) {
    return this.annotv[number];
  }

  setAnnotation(number, annot) {
    this.annotv[number] = annot;
  }

  setAnnotationTextOffset(num, x, y) {
    if (this.annot_scrollingtext === -1) {
      this.annot_scrollingtext = num;
    }
    if (num === this.annot_scrollingtext) {
      this.annot_offset_x = x;
      this.annot_offset_y = y;
    }
  }

  getAnnotationTextExtent(num) {
    return this.nc.getAnnotationTextExtent(this.trf, this, num);
  }

  setColorThemeLightness() {
    if (this.ctx.customcolors === null) {
      return;
    }

    if (this.ctx.customcolors.colortheme_lightness !== this.color_lightness) {
      this.color_lightness = this.ctx.customcolors.colortheme_lightness;
    }
  }
}