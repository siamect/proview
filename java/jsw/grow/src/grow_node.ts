class GrowNode extends GlowNode {
  trf: GlowTransform;
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
  userdata = null;
  visible = 1;
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
    super(ctx);
    this.trf = new GlowTransform();
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

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
          i = super.open(lines, i + 1);
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

  event_handler(event, fx, fy) {
    if (this.visible === 0 || this.dimmed !== 0) {
      return 0;
    }

    if (event.event === Event.CursorMotion) {
      return 0;
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

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    node = node || this;
    colornode = colornode || this;

    if (this.visible === 0 || node.invisible !== 0) {
      return;
    }

    if (this.annot_scrollingtext !== -1) {
      this.ctx.gdraw.set_clip_rectangle(this.ll_x *
          this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x, this.ll_y *
          this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y, this.ur_x *
          this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x, this.ur_y *
          this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y);
    }

    if (this.fill_level === 1) {
      if (t === null) {
        this.nc.draw(this.trf, highlight, hot, node, node);
      } else {
        let trf_tot = Matrix.multiply(t, this.trf);
        this.nc.draw(trf_tot, highlight, hot, this, this);
      }
    } else {
      let x1, x2, y1, y2;
      let x_level = 0;
      let y_level = 0;
      let clip_sts = 0;
      let old_color_tone = 0;
      let old_fill_drawtype = 0;

      if (t === null) {
        x1 = Math.floor(this.ll_x * this.ctx.mw.zoom_factor_x + 0.5) -
            this.ctx.mw.offset_x;
        y1 = Math.floor(this.ll_y * this.ctx.mw.zoom_factor_y + 0.5) -
            this.ctx.mw.offset_y;
        x2 = Math.floor(this.ur_x * this.ctx.mw.zoom_factor_x + 0.5) -
            this.ctx.mw.offset_x;
        y2 = Math.floor(this.ur_y * this.ctx.mw.zoom_factor_y + 0.5) -
            this.ctx.mw.offset_y;
      } else {
        x1 = Math.floor(t.x(this.ll_x, this.ll_y) * this.ctx.mw.zoom_factor_x + 0.5) -
            this.ctx.mw.offset_x;
        y1 = Math.floor(t.y(this.ll_x, this.ll_y) * this.ctx.mw.zoom_factor_y + 0.5) -
            this.ctx.mw.offset_y;
        x2 =
            Math.floor(t.x(this.ur_x, this.ur_y) * this.ctx.mw.zoom_factor_x + 0.5) -
            this.ctx.mw.offset_x;
        y2 =
            Math.floor(t.y(this.ur_x, this.ur_y) * this.ctx.mw.zoom_factor_y + 0.5) -
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
        this.nc.draw(this.trf, highlight, hot, node, node);
      } else {
        let trf_tot = Matrix.multiply(t, this.trf);
        // If this node has a trace pointer, use colors for this node
        this.nc.draw(trf_tot, highlight, hot, this, this);
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
        this.nc.draw(this.trf, highlight, hot, node, node);
      } else {
        let trf_tot = Matrix.multiply(t, this.trf);
        // If this node has a trace pointer, use colors for this node
        this.nc.draw(trf_tot, highlight, hot, this, this);
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
    if (this.trf.a13 === this.trf.s.a13 + x &&
        this.trf.a23 === this.trf.s.a23 + y) {
      return;
    }
    this.trf.move_from_stored(x, y);
    this.get_node_borders();
    this.ctx.draw();
  }

  move(x, y) {
    this.trf.move(x, y);
    this.get_node_borders();
    this.ctx.draw();
  }

  move_to(x, y) {
    this.trf.move(x - this.ll_x, y - this.ll_y);
    this.get_node_borders();
    this.ctx.draw();
  }

  set_scale_pos(x, y, scale_x, scale_y, x0, y0, type) {
    this.ctx.setNodraw();
    this.set_scale(scale_x, scale_y, x0, y0, type);
    this.ctx.resetNodraw();
    this.trf.move(x - this.ll_x, y - this.ll_y);
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

    if (this.trf.s.a11 !== 0 && this.trf.s.a22 !== 0 &&
        Math.abs(scale_x - this.trf.a11 / this.trf.s.a11) < Number.MIN_VALUE &&
        Math.abs(scale_y - this.trf.a22 / this.trf.s.a22) < Number.MIN_VALUE) {
      return;
    }

    switch (type) {
      case ScaleType.LowerLeft:
        x0 = this.ll_x;
        y0 = this.ll_y;
        break;
      case ScaleType.LowerRight:
        x0 = this.ur_x;
        y0 = this.ll_y;
        break;
      case ScaleType.UpperRight:
        x0 = this.ur_x;
        y0 = this.ur_y;
        break;
      case ScaleType.UpperLeft:
        x0 = this.ll_x;
        y0 = this.ur_y;
        break;
      case ScaleType.FixPoint:
        break;
      case ScaleType.Center:
        x0 = (this.ll_x + this.ur_x) / 2;
        y0 = (this.ll_y + this.ur_y) / 2;
        break;
      default:
    }

    old_x_left = this.ll_x;
    old_x_right = this.ur_x;
    old_y_low = this.ll_y;
    old_y_high = this.ur_y;
    this.trf.revert();
    this.trf.scale(scale_x, scale_y, x0, y0);
    this.get_node_borders();

    this.ctx.draw();
    this.ctx.draw();
  }

  setRotation(angel, x0, y0, type) {
    if (Math.abs(angel - this.trf.rotation + this.trf.s.rotation) <
        Number.MIN_VALUE) {
      return;
    }

    switch (type) {
      case RotationPoint.LowerLeft:
        x0 = this.ll_x;
        y0 = this.ll_y;
        break;
      case RotationPoint.LowerRight:
        x0 = this.ur_x;
        y0 = this.ll_y;
        break;
      case RotationPoint.UpperRight:
        x0 = this.ur_x;
        y0 = this.ur_y;
        break;
      case RotationPoint.UpperLeft:
        x0 = this.ll_x;
        y0 = this.ur_y;
        break;
      case RotationPoint.Center:
        x0 = (this.ll_x + this.ur_x) / 2;
        y0 = (this.ur_y + this.ll_y) / 2;
        break;
      case RotationPoint.Zero:
        x0 = this.trf.s.a13;
        y0 = this.trf.s.a23;
        break;
      default:
        break;
    }

    let t = new GlowTransform();
    t.rotate(angel, x0, y0);
    this.trf.set(Matrix.multiply(t, this.trf.s));
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
    this.nc = new_nc;
    this.get_node_borders();
    this.ctx.draw();
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

    // Calculate max and min coordinates
    let p1 = this.trf.apply(new Point(0, this.nc.y0));
    let p2 = this.trf.apply(new Point(0, this.nc.y1));

    let rotation = (this.trf.rotation / 360 - Math.floor(this.trf.rotation / 360)) * 360;

    if (rotation <= 45 || rotation > 315) {
      limits.direction = Direction.Down;
      limits.min = p1.y;
      limits.max = p2.y;
    } else if (rotation > 45 && rotation <= 135) {
      limits.direction = Direction.Right;
      limits.min = p2.x;
      limits.max = p1.x;
    } else if (rotation > 135 && rotation <= 225) {
      limits.direction = Direction.Up;
      limits.min = p2.y;
      limits.max = p1.y;
    } else if (rotation > 225 && rotation <= 315) {
      limits.direction = Direction.Left;
      limits.min = p1.x;
      limits.max = p2.x;
    }
    limits.status = 1;
    return limits;
  }

  get_node_borders() {
    let g = new Rect(10.0e37, 10.0e37, -10.0e37, -10.0e37);
    this.nc.get_borders(this.trf, g);
    this.set(g);
  }

  get_borders(t, g) {
    let t2 = (t) ? Matrix.multiply(t, this.trf) : this.trf;
    this.nc.get_borders(t2, g);
  }

  get_background_object_limits(t, type, x, y, bo) {
    let b = bo;
    let base_nc = this.nc.get_base_nc();

    let dyn_type = base_nc.dyn_type1;
    if ((type & dyn_type) === 0) {
      return 0;
    }
    if (this.nc.y0 === 0 && this.nc.y1 === 0) {
      return 0;
    }

    let g = new Rect(10.0e37, 10.0e37, -10.0e37, -10.0e37);
    this.get_borders(t, g);
    if (g.hit(new Point(x, y))) {
      // Hit, calculate max and min koordinates
      let tmp = Matrix.multiply(this.trf, t);
      let p1 = tmp.apply(new Point(0, this.nc.y0));
      let p2 = tmp.apply(new Point(0, this.nc.y1));

      let rotation = (tmp.rotation / 360 - Math.floor(tmp.rotation / 360)) * 360;

      if (rotation <= 45 || rotation > 315) {
        b.direction = Direction.Down;
        b.min = p1.y;
        b.max = p2.y;
      } else if (rotation > 45 && rotation <= 135) {
        b.direction = Direction.Left;
        b.min = p2.x;
        b.max = p1.x;
      } else if (rotation > 135 && rotation <= 225) {
        b.direction = Direction.Up;
        b.min = p2.y;
        b.max = p1.y;
      } else if (rotation > 225 && rotation <= 315) {
        b.direction = Direction.Right;
        b.min = p1.x;
        b.max = p2.x;
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
    if (visibility === Vis.Visible) {
      this.visible = 1;
      this.dimmed = 0;
    } else if (visibility === Vis.Invisible) {
      this.visible = 0;
      this.dimmed = 0;
      this.draw();
    } else if (visibility === Vis.Dimmed) {
      this.visible = 1;
      this.dimmed = 1;
      this.draw();
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