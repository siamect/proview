class GlowArc {
  ctx: GrowCtx;
  ll: GlowPoint;
  ur: GlowPoint;
  draw_type;
  angle1;
  angle2;
  line_width;
  display_level;
  fill;
  border;
  shadow;

  constructor(ctx) {
    this.ctx = ctx;
    this.ll = new GlowPoint();
    this.ur = new GlowPoint();
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.Arc:
          break;
        case GlowSave.Arc_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Arc_angle1:
          this.angle1 = parseInt(tokens[1], 10);
          break;
        case GlowSave.Arc_angle2:
          this.angle2 = parseInt(tokens[1], 10);
          break;
        case GlowSave.Arc_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case GlowSave.Arc_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case GlowSave.Arc_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case GlowSave.Arc_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowArc");
          break;
      }
    }

    return i;
  }

  draw(highlight, hot) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let ll_x = Math.floor(this.ll.x * this.ctx.mw.zoom_factor_x + 0.5) -
        this.ctx.mw.offset_x;
    let ll_y = Math.floor(this.ll.y * this.ctx.mw.zoom_factor_y + 0.5) -
        this.ctx.mw.offset_y;
    let ur_x = Math.floor(this.ur.x * this.ctx.mw.zoom_factor_x + 0.5) -
        this.ctx.mw.offset_x;
    let ur_y = Math.floor(this.ur.y * this.ctx.mw.zoom_factor_y + 0.5) -
        this.ctx.mw.offset_y;

    if (ll_x === ur_x && ll_y === ur_y) {
      return;
    }
    let idx = Math.floor(this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);
    this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1,
        this.angle2, this.draw_type, false, idx, highlight);
  }

  draw_shadow(border, shadow, highlight, hot) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let ll_x = Math.floor(this.ll.x * this.ctx.mw.zoom_factor_x + 0.5) -
        this.ctx.mw.offset_x;
    let ll_y = Math.floor(this.ll.y * this.ctx.mw.zoom_factor_y + 0.5) -
        this.ctx.mw.offset_y;
    let ur_x = Math.floor(this.ur.x * this.ctx.mw.zoom_factor_x + 0.5) -
        this.ctx.mw.offset_x;
    let ur_y = Math.floor(this.ur.y * this.ctx.mw.zoom_factor_y + 0.5) -
        this.ctx.mw.offset_y;
    let idx = Math.floor(this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);
    let ish = 1;
    let offs = this.border;

    if (this.shadow !== 0 && idx > 2) {
      if (this.angle1 === 0) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
            offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs *
            2, this.angle1 + 45, this.angle2 - 45,
            GlowColor.shift_drawtype(this.draw_type, -2, null), false, ish - 1,
            highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
            offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
            this.angle1, this.angle2 - 45,
            GlowColor.shift_drawtype(this.draw_type, 2, null), false, ish - 1,
            highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
            Math.max(0, ur_x - ll_x - idx + offs * 2),
            Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1 +
            45, this.angle2 - 45,
            GlowColor.shift_drawtype(this.draw_type, 2, null), false, ish - 1,
            highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
            Math.max(0, ur_x - ll_x - idx + offs * 2),
            Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2 -
            45, GlowColor.shift_drawtype(this.draw_type, -2, null), false, ish - 1,
            highlight);
      } else if (this.angle1 === 90) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
            offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
            this.angle1, this.angle2,
            GlowColor.shift_drawtype(this.draw_type, -2, null), false, ish - 1,
            highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
            Math.max(0, ur_x - ll_x - idx + offs * 2),
            Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2,
            GlowColor.shift_drawtype(this.draw_type, 2, null), false, ish - 1,
            highlight);
      } else if (this.angle1 === 180) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
            offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs *
            2, this.angle1 + 45, this.angle2 - 45,
            GlowColor.shift_drawtype(this.draw_type, 2, null), false, ish - 1,
            highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
            offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
            this.angle1, this.angle2 - 45,
            GlowColor.shift_drawtype(this.draw_type, -2, null), false, ish - 1,
            highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
            Math.max(0, ur_x - ll_x - idx + offs * 2),
            Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1 +
            45, this.angle2 - 45,
            GlowColor.shift_drawtype(this.draw_type, -2, null), false, ish - 1,
            highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
            Math.max(0, ur_x - ll_x - idx + offs * 2),
            Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2 -
            45, GlowColor.shift_drawtype(this.draw_type, 2, null), false, ish - 1,
            highlight);
      } else if (this.angle1 === 270) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
            offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
            this.angle1, this.angle2,
            GlowColor.shift_drawtype(this.draw_type, 2, null), false, ish - 1,
            highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
            Math.max(0, ur_x - ll_x - idx + offs * 2),
            Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2,
            GlowColor.shift_drawtype(this.draw_type, -2, null), false, ish - 1,
            highlight);
      }
    }
    if (border !== 0) {
      this.ctx.gdraw.arc(ll_x + idx / 2 - idx, ll_y + idx / 2 - idx, ur_x -
          ll_x + idx, ur_y - ll_y + idx, this.angle1, this.angle2,
          DrawType.Line, false, 0, highlight);
      if (idx > 0) {
        this.ctx.gdraw.arc(ll_x + idx / 2, ll_y + idx / 2,
            Math.max(0, ur_x - ll_x - idx), Math.max(0, ur_y - ll_y - idx),
            this.angle1, this.angle2, DrawType.Line, false, 0, highlight);
      }
    }
  }
}