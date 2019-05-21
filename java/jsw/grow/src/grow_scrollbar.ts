class GrowScrollBar extends GrowRect {
  max_value;
  min_value;
  bar_value;
  bar_length;
  user_data = null;
  direction;
  movement_active = 0;
  start_pos;
  start_value;
  parent;
  bar_color;
  hot;
  highlight;

  constructor(ctx, name, x, y, w, h, dir, border_d_type, line_width, display_lev,
       fill_d_type, bar_d_type, nodraw, parent) {
    super(ctx);
    this.ll.x = x;
    this.ll.y = y;
    this.ur.x = x + w;
    this.ur.y = y + h;
    this.draw_type = border_d_type;
    this.line_width = line_width;
    this.fill = true;
    this.border = 1;
    this.shadow = 0;
    this.fill_drawtype = fill_d_type;
    this.n_name = name;
    this.display_level = display_lev;
    this.bar_color = bar_d_type;
    this.direction = dir;
    this.parent = parent;
    if (nodraw !== 0) {
      this.draw();
    }
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let idx = Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        this.line_width - 1);
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);

    let tmp = Matrix.multiply(t, this.trf);
    let p1 = tmp.apply(this.ll);
    let p2 = tmp.apply(this.ur);

    let x1 = Math.floor(p1.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y1 = Math.floor(p1.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let x2 = Math.floor(p2.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y2 = Math.floor(p2.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    let ll_x = Math.min(x1, x2);
    let ur_x = Math.max(x1, x2);
    let ll_y = Math.min(y1, y2);
    let ur_y = Math.max(y1, y2);

    let fdrawtype = GlowColor.get_drawtype(this.fill_drawtype,
        DrawType.FillHighlight, highlight, colornode, 1, 0);
    let bdrawtype = GlowColor.get_drawtype(this.draw_type,
        DrawType.LineHighlight, highlight, colornode, 0, 0);
    let shift_drawtype;

    this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, fdrawtype, true, 0);
    if (this.shadow !== 0) {
      shift_drawtype = GlowColor.shift_drawtype(this.fill_drawtype, 2, null); // Dark
      this.ctx.gdraw.line(ll_x + 1, ll_y + 1, ll_x + 1, ur_y - 1,
          shift_drawtype, 0, 0);
      this.ctx.gdraw.line(ll_x + 1, ll_y + 1, ur_x - 1, ll_y + 1,
          shift_drawtype, 0, 0);
      shift_drawtype = GlowColor.shift_drawtype(this.fill_drawtype, -2, null); // Light
      this.ctx.gdraw.line(ll_x + 1, ur_y - 1, ur_x - 1, ur_y - 1,
          shift_drawtype, 0, 0);
      this.ctx.gdraw.line(ur_x - 1, ll_y + 1, ur_x - 1, ur_y - 1,
          shift_drawtype, 0, 0);
    }

    if (this.max_value !== this.min_value) {
      let x0, y0, width, height;
      switch (this.direction) {
        case Dir.Vertical:
          height =
              Math.floor(this.bar_length / (this.max_value - this.min_value) *
                  (ur_y - ll_y));
          height = Math.max(0, Math.min(height, ur_y - ll_y));
          width = ur_x - ll_x;
          x0 = ll_x;
          y0 = ll_y + Math.floor((this.bar_value - this.min_value) /
              (this.max_value - this.min_value) * (ur_y - ll_y));
          break;
        case Dir.Horizontal:
          width =
              Math.floor(this.bar_length / (this.max_value - this.min_value) *
                  (ur_x - ll_x));
          width = Math.max(0, Math.min(width, ur_x - ll_x));
          height = ur_y - ll_y;
          x0 = ll_x + Math.floor((this.bar_value - this.min_value) /
              (this.max_value - this.min_value) * (ur_x - ll_x));
          y0 = ll_y;
          break;
      }
      this.ctx.gdraw.rect(x0, y0, width, height, this.bar_color, true, 0);
      if (this.shadow !== 0) {
        shift_drawtype = GlowColor.shift_drawtype(this.bar_color, -2, null); // Light
        this.ctx.gdraw.line(x0 + 1, y0 + 1, x0 + 1, y0 + height - 1,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(x0 + 1, y0 + 1, x0 + width - 1, y0 + 1,
            shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.bar_color, 2, null); // Dark
        this.ctx.gdraw.line(x0 + 1, y0 + height - 1, x0 + width - 1, y0 +
            height - 1, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(x0 + width - 1, y0 + 1, x0 + width - 1, y0 +
            height - 1, shift_drawtype, 0, 0);
      }
      this.ctx.gdraw.rect(x0, y0, width, height, bdrawtype, false, idx);
    }

    this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, bdrawtype, false, idx);
  }

  set_value(value, length) {
    console.log("Value", value);
    this.bar_value = value;
    if (length !== 0) {
      this.bar_length = length;
    }

    this.bar_value = clamp(this.bar_value, this.min_value, this.max_value);

    this.draw();
    return this.bar_value;
  }

  event_handler(event, fx, fy) {
    let ll_x = Math.min(this.ll.x, this.ur.x);
    let ur_x = Math.max(this.ll.x, this.ur.x);
    let ll_y = Math.min(this.ll.y, this.ur.y);
    let ur_y = Math.max(this.ll.y, this.ur.y);

    if (this.movement_active === 0 &&
        !(ll_x <= fx && fx <= ur_x && ll_y <= fy && fy <= ur_y)) {
      return 0;
    }

    if (event.event === Event.ButtonMotion && this.movement_active === 0) {
      return 0;
    }

    if (this.min_value === this.max_value) {
      return 1;
    }

    switch (event.event) {
      case Event.MB1Down:
        switch (this.direction) {
          case Dir.Vertical:
            if (ll_y + (this.bar_value - this.min_value) /
                (this.max_value - this.min_value) * (ur_y - ll_y) <= fy && fy <=
                ll_y + (this.bar_value - this.min_value + this.bar_length) /
                (this.max_value - this.min_value) * (ur_y - ll_y)) {
              this.start_pos = fy;
              this.start_value = this.bar_value;
              this.movement_active = 1;
              this.ctx.setSliderActive(true);
            }
            break;
          case Dir.Horizontal:
            if (ll_x + (this.bar_value - this.min_value) /
                (this.max_value - this.min_value) * (ur_x - ll_x) <= fx && fx <=
                ll_x + (this.bar_value - this.min_value + this.bar_length) /
                (this.max_value - this.min_value) * (ur_x - ll_x)) {
              this.start_pos = fx;
              this.start_value = this.bar_value;
              this.movement_active = 1;
              this.ctx.setSliderActive(true);
            }
            break;
        }
        break;
      case Event.MB1Up:
        this.movement_active = 0;
        this.ctx.setSliderActive(false);
        break;
      case Event.ButtonMotion:
        if (this.movement_active !== 0) {
          if (this.bar_length >= this.max_value - this.min_value) {
            break;
          }
          switch (this.direction) {
            case Dir.Vertical:
              this.bar_value =
                  this.start_value + (fy - this.start_pos) / (ur_y - ll_y) *
                  (this.max_value - this.min_value);
              if (this.bar_value < this.min_value) {
                this.bar_value = this.min_value;
              }
              if (this.bar_value > this.max_value - this.bar_length) {
                this.bar_value = this.max_value - this.bar_length;
              }
              this.parent.verticalScrollValueChanged(this.bar_value);
              break;
            case Dir.Horizontal:
              this.bar_value =
                  this.start_value + (fx - this.start_pos) / (ur_x - ll_x) *
                  (this.max_value - this.min_value);
              if (this.bar_value < this.min_value) {
                this.bar_value = this.min_value;
              }
              if (this.bar_value > this.max_value - this.bar_length) {
                this.bar_value = this.max_value - this.bar_length;
              }
              this.parent.horizontalScrollValueChanged(this.bar_value);
              break;
          }
        }
        break;
      default:
    }
    return 1;
  }

  set_range(min, max) {
    this.max_value = max;
    this.min_value = min;
    this.draw();
  }

  set_shadow(shadow) {
    this.shadow = shadow;
  }

  set_position(x, y, width, height) {
    this.ll.posit(x, y);
    this.ur.posit(x + width, y + height);
  }
}