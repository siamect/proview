class GrowRectRounded extends GrowRect {
  round_amount;

  constructor(ctx) {
    super(ctx);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowRectRounded:
          break;
        case GlowSave.GrowRectRounded_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowRectRounded_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRectRounded_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRectRounded_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRectRounded_y_low:
          this.ll_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRectRounded_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_original_fill_drawtype:
          this.original_fill_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_fill_drawtype:
          this.fill_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_round_amount:
          this.round_amount = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRectRounded_shadow_width:
          this.shadow_width = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRectRounded_shadow_contrast:
          this.shadow_contrast = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_relief:
          this.relief = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_disable_shadow:
          this.disable_shadow = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_fixposition:
          this.fixposition = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_gradient:
          this.gradient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_gradient_contrast:
          this.gradient_contrast = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_disable_gradient:
          this.disable_gradient = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowRectRounded_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case GlowSave.GrowRectRounded_rect_part:
          i = GlowRect.prototype.open.call(this, lines, i + 1);
          break;
        case GlowSave.GrowRectRounded_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowRectRounded");
          break;
      }
    }

    return i;
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let chot = 0;

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
    let amount = Math.floor(this.round_amount / 100 *
        Math.min(ur_x - ll_x, ur_y - ll_y) + 0.5);
    if (this.fill !== 0) {
      let ish = Math.floor(this.shadow_width / 100 *
          Math.min(ur_x - ll_x, ur_y - ll_y) + 0.5);
      let display_shadow = ((node !== null && node.shadow !== 0) ||
          this.shadow !== 0) && this.disable_shadow === 0;
      let fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
          DrawType.FillHighlight, highlight, colornode, 1, 0);

      let grad = this.gradient;
      if (this.gradient === Gradient.No &&
          (node !== null && node.gradient !== Gradient.No) &&
          this.disable_gradient === 0) {
        grad = node.gradient;
      }

      if (!display_shadow || ish === 0) {
        if (grad === Gradient.No) {
          let drawtype = (chot === 0) ? fillcolor : GlowColor.shift_drawtype(fillcolor, chot, null);

          this.ctx.gdraw.rect(ll_x, ll_y + amount, ur_x - ll_x, ur_y -
              ll_y - 2 * amount, drawtype, true, 0);
          this.ctx.gdraw.rect(ll_x + amount, ll_y, ur_x - ll_x - 2 *
              amount, amount, drawtype, true, 0);
          this.ctx.gdraw.rect(ll_x + amount, ur_y - amount, ur_x - ll_x -
              2 * amount, amount, drawtype, true, 0);
          this.ctx.gdraw.arc(ll_x, ll_y, 2 * amount, 2 * amount, 90, 90,
              drawtype, true, 0);
          this.ctx.gdraw.arc(ll_x, ur_y - 2 * amount, 2 * amount, 2 *
              amount, 180, 90, drawtype, true, 0);
          this.ctx.gdraw.arc(ur_x - 2 * amount, ur_y - 2 * amount, 2 *
              amount, 2 * amount, 270, 90, drawtype, true, 0);
          this.ctx.gdraw.arc(ur_x - 2 * amount, ll_y, 2 * amount, 2 *
              amount, 0, 90, drawtype, true, 0);
        } else {
          let rotationa = t ? this.trf.rot(t) : this.trf.rot();

          let fa1, fa2;
          if (this.gradient_contrast >= 0) {
            fa2 =
                GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                    chot, null);
            fa1 = GlowColor.shift_drawtype(
                fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
                null);
          } else {
            fa2 = GlowColor.shift_drawtype(
                fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
                null);
            fa1 =
                GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                    chot, null);
          }
          this.ctx.gdraw.gradient_fill_rectrounded(ll_x, ll_y, ur_x -
              ll_x, ur_y - ll_y, amount, fillcolor, fa1, fa2, this.ctx.gdraw.gradient_rotate(rotationa, grad));
        }
      } else {
        let drawtype_incr = (this.relief === Relief.Down) ? -this.shadow_contrast : this.shadow_contrast;

        // Draw light shadow
        let drawtype =
            GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot, colornode);
        this.ctx.gdraw.rect(ll_x + amount, ll_y, ur_x - ll_x - 2 * amount,
            ish, drawtype, true, 0);
        this.ctx.gdraw.rect(ll_x, ll_y + amount, ish, ur_y - ll_y - 2 *
            amount, drawtype, true, 0);
        this.ctx.gdraw.arc(ll_x, ll_y, 2 * amount, 2 * amount, 90, 90,
            drawtype, true, 0);
        this.ctx.gdraw.arc(ll_x, ur_y - 2 * amount, 2 * amount, 2 * amount,
            180, 45, drawtype, true, 0);
        this.ctx.gdraw.arc(ur_x - 2 * amount, ll_y, 2 * amount, 2 * amount,
            45, 45, drawtype, true, 0);
        // Draw dark shadow
        drawtype =
            GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot, colornode);
        this.ctx.gdraw.rect(ll_x + amount, ur_y - ish, ur_x - ll_x - 2 *
            amount, ish, drawtype, true, 0);
        this.ctx.gdraw.rect(ur_x - ish, ll_y + amount, ish, ur_y - ll_y -
            2 * amount, drawtype, true, 0);
        this.ctx.gdraw.arc(ll_x, ur_y - 2 * amount, 2 * amount, 2 * amount,
            225, 45, drawtype, true, 0);
        this.ctx.gdraw.arc(ur_x - 2 * amount, ll_y, 2 * amount, 2 * amount,
            0, 45, drawtype, true, 0);
        this.ctx.gdraw.arc(ur_x - 2 * amount, ur_y - 2 * amount, 2 *
            amount, 2 * amount, 270, 90, drawtype, true, 0);

        if (grad === Gradient.No) {
          let drawtype = (chot === 0) ? fillcolor : GlowColor.shift_drawtype(fillcolor, chot, null);

          if (amount > ish) {
            this.ctx.gdraw.rect(ll_x + ish, ll_y + amount, ur_x - ll_x -
                2 * ish, ur_y - ll_y - 2 * amount, drawtype, true, 0);
            this.ctx.gdraw.rect(ll_x + amount, ll_y + ish, ur_x - ll_x -
                2 * amount, amount - ish, drawtype, true, 0);
            this.ctx.gdraw.rect(ll_x + amount, ur_y - amount, ur_x - ll_x -
                2 * amount, amount - ish, drawtype, true, 0);
            this.ctx.gdraw.arc(ll_x + ish, ll_y + ish, 2 * amount - 2 *
                ish, 2 * amount - 2 * ish, 90, 90, drawtype, true, 0);
            this.ctx.gdraw.arc(ll_x + ish, ur_y - 2 * amount + ish, 2 *
                amount - 2 * ish, 2 * amount - 2 * ish, 180, 90, drawtype, true, 0);
            this.ctx.gdraw.arc(ur_x - 2 * amount + ish, ur_y - 2 * amount +
                ish, 2 * amount - 2 * ish, 2 * amount - 2 * ish, 270, 90,
                drawtype, true, 0);
            this.ctx.gdraw.arc(ur_x - 2 * amount + ish, ll_y + ish, 2 *
                amount - 2 * ish, 2 * amount - 2 * ish, 0, 90, drawtype, true, 0);
          } else {
            this.ctx.gdraw.rect(ll_x + amount, ll_y + amount, ur_x - ll_x -
                2 * amount, ur_y - ll_y - 2 * amount, drawtype, true, 0);
          }
        } else {
          let rotationb  = t ? this.trf.rot(t) : this.trf.rot();

          let fb1, fb2;
          if (this.gradient_contrast >= 0) {
            fb2 =
                GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                    chot, null);
            fb1 = GlowColor.shift_drawtype(
                fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
                null);
          } else {
            fb2 = GlowColor.shift_drawtype(
                fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
                null);
            fb1 =
                GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                    chot, null);
          }
          this.ctx.gdraw.gradient_fill_rectrounded(ll_x + ish, ll_y +
              ish, ur_x - ll_x - 2 * ish, ur_y - ll_y - 2 * ish, amount - ish,
              fillcolor, fb1, fb2, this.ctx.gdraw.gradient_rotate(rotationb, grad));
        }
      }
    }
    if (this.border !== 0 || this.fill === 0) {
      let drawtype =
          GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
              highlight, colornode, 0, 0);
      let c = 0;
      this.ctx.gdraw.line(ll_x + amount, ll_y, ur_x - amount, ll_y, drawtype,
          idx, 0);
      this.ctx.gdraw.line(ll_x, ll_y - c + amount, ll_x, ur_y - c - amount,
          drawtype, idx, 0);
      this.ctx.gdraw.line(ll_x + amount, ur_y, ur_x - amount, ur_y, drawtype,
          idx, 0);
      this.ctx.gdraw.line(ur_x - c, ll_y + amount, ur_x - c, ur_y - amount,
          drawtype, idx, 0);
      this.ctx.gdraw.arc(ll_x, ll_y, 2 * amount, 2 * amount, 90, 90, drawtype,
          idx, 0);
      this.ctx.gdraw.arc(ll_x, ur_y - 2 * amount, 2 * amount, 2 * amount, 180,
          90, drawtype, idx, 0);
      this.ctx.gdraw.arc(ur_x - 2 * amount, ur_y - 2 * amount, 2 * amount, 2 *
          amount, 270, 90, drawtype, idx, 0);
      this.ctx.gdraw.arc(ur_x - 2 * amount, ll_y, 2 * amount, 2 * amount, 0, 90,
          drawtype, idx, 0);
    }
  }
}