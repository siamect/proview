class GrowImage {
  ctx: GrowCtx;
  trf: GlowTransform;
  ll: GlowPoint;
  ur: GlowPoint;
  image: HTMLImageElement;
  n_name;
  x_right;
  x_left;
  y_high;
  y_low;
  image_filename = null;
  dynamicsize;
  color_lightness;
  color_intensity;
  color_shift;
  color_tone;
  fixposition;
  display_level;

  constructor(ctx) {
    this.ctx = ctx;
    this.trf = new GlowTransform();
    this.ll = new GlowPoint();
    this.ur = new GlowPoint();
    this.image = new Image();
    this.image.onload = function () {
      console.log("Loading image");
      this.draw();
    }
  }

  open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowImage : " + lines[i]);
      }

      switch (key) {
        case GlowSave.GrowImage:
          break;
        case GlowSave.GrowImage_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowImage_image_filename:
          if (tokens.length > 1) {
            this.image_filename = tokens[1];
            //let n = window.location.href.lastIndexOf('/');
            //this.image.src = window.location.href.substring(0,n+1) + this.image_filename;
            this.image.src = this.image_filename;
          }
          break;
        case GlowSave.GrowImage_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowImage_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowImage_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowImage_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowImage_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowImage_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case GlowSave.GrowImage_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case GlowSave.GrowImage_display_level:
          this.display_level = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowImage_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case GlowSave.GrowImage_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case GlowSave.GrowImage_color_tone:
          this.color_tone = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowImage_color_lightness:
          this.color_lightness = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowImage_color_intensity:
          this.color_intensity = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowImage_color_shift:
          this.color_shift = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowImage_fixposition:
          this.fixposition = parseInt(tokens[1], 10);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowImage");
          break;
      }
    }

    return i;
  }

  event_handler(event, fx, fy) {
    let rp;

    switch (event.event) {
      case Event.CursorMotion:
        return 0;
      default:
    }

    rp = this.trf.reverse(fx, fy);
    if (this.ll.x <= rp.x && rp.x <= this.ur.x && this.ll.y <= rp.y &&
        rp.y <= this.ur.y) {
      console.log("Event handler: Hit in image");
      return 1;
    } else {
      return 0;
    }
  }

  draw() {
    if (this.image.naturalWidth !== 0) {
      this.tdraw(null, 0, 0, null, null);
    }
  }

  tdraw(t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let chot = 0;
    let idx;
    let drawtype;

    if (node !== null && node.line_width !== 0) {
      idx =
          Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
              node.line_width - 1);
    } else {
      idx =
          Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor -
              1);
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);
    let x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    // this.ctx.gdraw.rect( ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, DrawType.Line, idx, 0);
    this.ctx.gdraw.image(this.image, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y);
  }

  get_borders(t, g) {
    let ll_x, ur_x, ll_y, ur_y, x1, x2, y1, y2;

    if (t === null) {
      x1 = this.trf.x(this.ll.x, this.ll.y);
      x2 = this.trf.x(this.ur.x, this.ur.y);
      y1 = this.trf.y(this.ll.x, this.ll.y);
      y2 = this.trf.y(this.ur.x, this.ur.y);
    } else {
      x1 = this.trf.x(t, this.ll.x, this.ll.y);
      x2 = this.trf.x(t, this.ur.x, this.ur.y);
      y1 = this.trf.y(t, this.ll.x, this.ll.y);
      y2 = this.trf.y(t, this.ur.x, this.ur.y);
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    if (ll_x < g.ll_x) {
      g.ll_x = ll_x;
    }
    if (ur_x > g.ur_x) {
      g.ur_x = ur_x;
    }
    if (ll_y < g.ll_y) {
      g.ll_y = ll_y;
    }
    if (ur_y > g.ur_y) {
      g.ur_y = ur_y;
    }
  }

  get_node_borders() {
    let g = new Rect();
    g.x = 1.0e37;
    g.y = 1.0e37;
    g.width = -1.0e37;
    g.height = -1.0e37;
    this.get_borders(null, g);
    this.x_left = g.x;
    this.x_right = g.x + g.width;
    this.y_low = g.y;
    this.y_high = g.y + g.height;
  }

  set_scale(scale_x, scale_y, x0, y0, type) {
    let old_x_left, old_x_right, old_y_low, old_y_high;

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

    switch (type) {
      case ScaleType.LowerLeft:
        this.x_left = old_x_left;
        this.y_low = old_y_low;
        break;
      case ScaleType.LowerRight:
        this.x_right = old_x_right;
        this.y_low = old_y_low;
        break;
      case ScaleType.UpperRight:
        this.x_right = old_x_right;
        this.y_high = old_y_high;
        break;
      case ScaleType.UpperLeft:
        this.x_left = old_x_left;
        this.y_high = old_y_high;
        break;
      case ScaleType.FixPoint:
        break;
      case ScaleType.Center:
        x0 = (this.x_left + this.x_right) / 2;
        y0 = (this.y_low + this.y_high) / 2;
        break;
      default:
    }
    this.ctx.draw();
  }

  getName() {
    return this.n_name;
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}