class GrowImage extends Rect {
  ctx: GrowCtx;
  trf: GlowTransform;
  ll: GlowPoint;
  ur: GlowPoint;
  image: HTMLImageElement;
  n_name;
  image_filename = null;
  dynamicsize;
  color_lightness;
  color_intensity;
  color_shift;
  color_tone;
  fixposition;
  display_level;

  constructor(ctx) {
    super();
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
          this.ur_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowImage_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowImage_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowImage_y_low:
          this.ll_y = parseFloat(tokens[1]);
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
    if (event.event == Event.CursorMotion) {
      return 0;
    }

    let rp = this.trf.reverse(fx, fy);
    return (this.ll.x <= rp.x && rp.x <= this.ur.x && this.ll.y <= rp.y &&
        rp.y <= this.ur.y);
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0 || this.image.naturalWidth === 0) {
      return;
    }

    let idx;
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

    let x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
    let y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    let x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
    let y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
          this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;

    let ll_x = Math.min(x1, x2);
    let ur_x = Math.max(x1, x2);
    let ll_y = Math.min(y1, y2);
    let ur_y = Math.max(y1, y2);

    // this.ctx.gdraw.rect( ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, DrawType.Line, idx, 0);
    this.ctx.gdraw.image(this.image, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y);
  }

  get_borders(t, g) {
    let x1 = this.trf.x(t, this.ll.x, this.ll.y);
    let x2 = this.trf.x(t, this.ur.x, this.ur.y);
    let y1 = this.trf.y(t, this.ll.x, this.ll.y);
    let y2 = this.trf.y(t, this.ur.x, this.ur.y);

    g.set(Rect.union(g, new Rect(Math.min(x1, x2), Math.min(y1, y2), Math.max(x1, x2), Math.max(y1, y2))));
  }

  get_node_borders() {
    let g = new Rect(1.0e37, 1.0e37, -1.0e37, -1.0e37);
    this.get_borders(null, g);
    this.set(g);
  }

  set_scale(scale_x, scale_y, x0, y0, type) {
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

    let old_x_left = this.ll_x;
    let old_x_right = this.ur_x;
    let old_y_low = this.ll_y;
    let old_y_high = this.ur_y;
    this.trf.scale_from_stored(scale_x, scale_y, x0, y0);
    this.get_node_borders();

    switch (type) {
      case ScaleType.LowerLeft:
        this.ll_x = old_x_left;
        this.ll_y = old_y_low;
        break;
      case ScaleType.LowerRight:
        this.ur_x = old_x_right;
        this.ll_y = old_y_low;
        break;
      case ScaleType.UpperRight:
        this.ur_x = old_x_right;
        this.ur_y = old_y_high;
        break;
      case ScaleType.UpperLeft:
        this.ll_x = old_x_left;
        this.ur_y = old_y_high;
        break;
      case ScaleType.FixPoint:
        break;
      case ScaleType.Center:
        x0 = (this.ll_x + this.ur_x) / 2;
        y0 = (this.ll_y + this.ur_y) / 2;
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