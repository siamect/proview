class GrowBarArc extends GrowArc {
  max_value;
  min_value;
  bar_width;
  bar_value;
  bar_bordercolor;
  bar_borderwidth;
  bar_drawtype;
  bar_direction;
  userdata = null;

  constructor(ctx) {
    super(ctx);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowBarArc:
          break;
        case GlowSave.GrowBarArc_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowBarArc_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowBarArc_bar_width:
          this.bar_width = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowBarArc_bar_direction:
          this.bar_direction = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarArc_bar_value:
          this.bar_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowBarArc_bar_bordercolor:
          this.bar_bordercolor = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarArc_bar_borderwidth:
          this.bar_borderwidth = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarArc_bar_drawtype:
          this.bar_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarArc_arc_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowBarArc_userdata_cb:
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
          console.log("Syntax error in GrowBarArc");
          break;
      }
    }

    return i;
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let width = Math.floor(this.bar_width * this.ctx.mw.zoom_factor_x);
    let value = Math.max(this.min_value,
        Math.min(this.bar_value, this.max_value));

    let idx;
    if (node !== null && node.line_width !== 0) {
      idx =
          Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
              node.line_width - 1);
    } else {
      idx =
          Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
              this.line_width - 1);
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);

    let x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let rotation =
          (this.trf.rot(t) / 360 - Math.floor(this.trf.rot(t) / 360)) * 360;

    let ll_x = Math.min(x1, x2);
    let ur_x = Math.max(x1, x2);
    let ll_y = Math.min(y1, y2);
    let ur_y = Math.max(y1, y2);
    let yscale = (ur_y - ll_y) / (ur_x - ll_x);
    if (width > ur_x - ll_x) {
      width = ur_x - ll_x;
    }
    let drawtype =
        GlowColor.get_drawtype(this.fill_drawtype, DrawType.FillHighlight,
            highlight, colornode, 1, 0);

    let bg_drawtype = (this.background_drawtype === DrawType.No) ? this.ctx.background_color : this.background_drawtype;

    // Draw circle background
    this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, 0, 360,
        bg_drawtype, true, 0);

    // Draw bar background
    this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1 -
        Math.floor(rotation), this.angle2, drawtype, true, 0);

    // Draw bar
    let ang = this.angle1 - rotation;
    if (this.bar_direction !== 0) {
      ang += this.angle2 * (this.max_value - value) / (this.max_value - this.min_value);
    }

    if (this.gradient === Gradient.No) {
      this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, ang,
          this.angle2 * (value - this.min_value) / (this.max_value - this.min_value), this.bar_drawtype, true, 0);
    } else {
      let f1, f2;
      if (this.gradient_contrast >= 0) {
        f2 =
            GlowColor.shift_drawtype(this.bar_drawtype, -this.gradient_contrast /
                2, null);
        f1 = GlowColor.shift_drawtype(this.bar_drawtype,
            Math.floor(this.gradient_contrast / 2 + 0.6), null);
      } else {
        f2 = GlowColor.shift_drawtype(this.bar_drawtype,
            -Math.floor(this.gradient_contrast / 2 - 0.6), null);
        f1 =
            GlowColor.shift_drawtype(this.bar_drawtype, this.gradient_contrast /
                2, null);
      }
      this.ctx.gdraw.gradient_fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
          ang, this.angle2 * (value - this.min_value) /
          (this.max_value - this.min_value), this.bar_drawtype, f1, f2,
          this.gradient);
    }

    // Draw inner circle background
    this.ctx.gdraw.arc(ll_x + width, ll_y + yscale * width, ur_x - ll_x -
        2 * width, ur_y - ll_y - yscale * 2 * width, 0, 360, bg_drawtype, true, 0);

    if (this.bar_direction === 0) {
      ang = Math.PI *
          (this.angle1 - rotation + this.angle2 * (value - this.min_value) /
              (this.max_value - this.min_value)) / 180;
    } else {
      ang = Math.PI *
          (this.angle1 - rotation + this.angle2 * (this.max_value - value) /
              (this.max_value - this.min_value)) / 180;
    }

    this.ctx.gdraw.line((ur_x + ll_x) / 2 + (ur_x - ll_x) / 2 *
        Math.cos(ang), (ur_y + ll_y) / 2 - (ur_y - ll_y) / 2 *
        Math.sin(ang), (ur_x + ll_x) / 2 + ((ur_x - ll_x) / 2 - width) *
        Math.cos(ang), (ur_y + ll_y) / 2 - yscale * ((ur_x - ll_x) / 2 - width) *
        Math.sin(ang), this.bar_bordercolor, this.bar_borderwidth, 0);

    if (this.border !== 0) {
      let bordercolor = GlowColor.get_drawtype(this.draw_type,
          DrawType.LineHighlight, highlight, colornode, 0, 0);

      this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1 -
          Math.floor(rotation), this.angle2, bordercolor, false, idx);

      this.ctx.gdraw.arc(ll_x + width, ll_y + yscale * width, ur_x - ll_x - 2 *
          width, ur_y - ll_y - yscale * 2 * width, this.angle1 -
          Math.floor(rotation), this.angle2, bordercolor, false, idx);

      ang = Math.PI * (this.angle1 - rotation) / 180;
      this.ctx.gdraw.line((ur_x + ll_x) / 2 + (ur_x - ll_x) / 2 *
          Math.cos(ang), (ur_y + ll_y) / 2 - (ur_y - ll_y) / 2 *
          Math.sin(ang), (ur_x + ll_x) / 2 + ((ur_x - ll_x) / 2 - width) *
          Math.cos(ang), (ur_y + ll_y) / 2 - yscale *
          ((ur_x - ll_x) / 2 - width) * Math.sin(ang), bordercolor, idx, 0);

      ang = Math.PI * (this.angle1 + this.angle2 - rotation) / 180;
      this.ctx.gdraw.line((ur_x + ll_x) / 2 + (ur_x - ll_x) / 2 *
          Math.cos(ang), (ur_y + ll_y) / 2 - (ur_y - ll_y) / 2 *
          Math.sin(ang), (ur_x + ll_x) / 2 + ((ur_x - ll_x) / 2 - width) *
          Math.cos(ang), (ur_y + ll_y) / 2 - yscale *
          ((ur_x - ll_x) / 2 - width) * Math.sin(ang), bordercolor, idx, 0);

    }
  }

  set_range(min, max) {
    this.max_value = max;
    this.min_value = min;
    this.draw();
  }

  set_value(value) {
    this.bar_value = value;
    this.draw();
  }

  getClassDynType1() {
    return 0;
  }

  getClassDynType2() {
    return 0;
  }

  getClassActionType1() {
    return 0;
  }

  getClassActionType2() {
    return 0;
  }

  getClassCycle() {
    return 0;
  }

  getClassUserData() {
    return null;
  }

  getUserData() {
    return this.userdata;
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}