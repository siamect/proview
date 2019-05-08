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

      if (this.ctx.debug) {
        console.log("GlowRectRounded : " + lines[i]);
      }

      switch (key) {
        case GlowSave.GrowRectRounded:
          break;
        case GlowSave.GrowRectRounded_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowRectRounded_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRectRounded_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRectRounded_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowRectRounded_y_low:
          this.y_low = parseFloat(tokens[1]);
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
          i = super.open_rect(lines, i + 1);
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
          Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
              this.line_width - 1);
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
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }

          this.ctx.gdraw.fill_rect(ll_x, ll_y + amount, ur_x - ll_x, ur_y -
              ll_y - 2 * amount, drawtype);
          this.ctx.gdraw.fill_rect(ll_x + amount, ll_y, ur_x - ll_x - 2 *
              amount, amount, drawtype);
          this.ctx.gdraw.fill_rect(ll_x + amount, ur_y - amount, ur_x - ll_x -
              2 * amount, amount, drawtype);
          this.ctx.gdraw.fill_arc(ll_x, ll_y, 2 * amount, 2 * amount, 90, 90,
              drawtype);
          this.ctx.gdraw.fill_arc(ll_x, ur_y - 2 * amount, 2 * amount, 2 *
              amount, 180, 90, drawtype);
          this.ctx.gdraw.fill_arc(ur_x - 2 * amount, ur_y - 2 * amount, 2 *
              amount, 2 * amount, 270, 90, drawtype);
          this.ctx.gdraw.fill_arc(ur_x - 2 * amount, ll_y, 2 * amount, 2 *
              amount, 0, 90, drawtype);
        } else {
          let fa1, fa2;
          let rotationa;
          if (t === null) {
            rotationa = this.trf.rot();
          } else {
            rotationa = this.trf.rot(t);
          }

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
              ll_x, ur_y - ll_y, amount, fillcolor, fa1, fa2,
              this.ctx.gdraw.gradient_rotate(rotationa, grad));
        }
      } else {
        let drawtype_incr = this.shadow_contrast;
        if (this.relief === Relief.Down) {
          drawtype_incr = -this.shadow_contrast;
        }

        // Draw light shadow
        drawtype =
            GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot, colornode);
        this.ctx.gdraw.fill_rect(ll_x + amount, ll_y, ur_x - ll_x - 2 * amount,
            ish, drawtype);
        this.ctx.gdraw.fill_rect(ll_x, ll_y + amount, ish, ur_y - ll_y - 2 *
            amount, drawtype);
        this.ctx.gdraw.fill_arc(ll_x, ll_y, 2 * amount, 2 * amount, 90, 90,
            drawtype);
        this.ctx.gdraw.fill_arc(ll_x, ur_y - 2 * amount, 2 * amount, 2 * amount,
            180, 45, drawtype);
        this.ctx.gdraw.fill_arc(ur_x - 2 * amount, ll_y, 2 * amount, 2 * amount,
            45, 45, drawtype);
        // Draw dark shadow
        drawtype =
            GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot, colornode);
        this.ctx.gdraw.fill_rect(ll_x + amount, ur_y - ish, ur_x - ll_x - 2 *
            amount, ish, drawtype);
        this.ctx.gdraw.fill_rect(ur_x - ish, ll_y + amount, ish, ur_y - ll_y -
            2 * amount, drawtype);
        this.ctx.gdraw.fill_arc(ll_x, ur_y - 2 * amount, 2 * amount, 2 * amount,
            225, 45, drawtype);
        this.ctx.gdraw.fill_arc(ur_x - 2 * amount, ll_y, 2 * amount, 2 * amount,
            0, 45, drawtype);
        this.ctx.gdraw.fill_arc(ur_x - 2 * amount, ur_y - 2 * amount, 2 *
            amount, 2 * amount, 270, 90, drawtype);

        if (grad === Gradient.No) {
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }

          if (amount > ish) {
            this.ctx.gdraw.fill_rect(ll_x + ish, ll_y + amount, ur_x - ll_x -
                2 * ish, ur_y - ll_y - 2 * amount, drawtype);
            this.ctx.gdraw.fill_rect(ll_x + amount, ll_y + ish, ur_x - ll_x -
                2 * amount, amount - ish, drawtype);
            this.ctx.gdraw.fill_rect(ll_x + amount, ur_y - amount, ur_x - ll_x -
                2 * amount, amount - ish, drawtype);
            this.ctx.gdraw.fill_arc(ll_x + ish, ll_y + ish, 2 * amount - 2 *
                ish, 2 * amount - 2 * ish, 90, 90, drawtype);
            this.ctx.gdraw.fill_arc(ll_x + ish, ur_y - 2 * amount + ish, 2 *
                amount - 2 * ish, 2 * amount - 2 * ish, 180, 90, drawtype);
            this.ctx.gdraw.fill_arc(ur_x - 2 * amount + ish, ur_y - 2 * amount +
                ish, 2 * amount - 2 * ish, 2 * amount - 2 * ish, 270, 90,
                drawtype);
            this.ctx.gdraw.fill_arc(ur_x - 2 * amount + ish, ll_y + ish, 2 *
                amount - 2 * ish, 2 * amount - 2 * ish, 0, 90, drawtype);
          } else {
            this.ctx.gdraw.fill_rect(ll_x + amount, ll_y + amount, ur_x - ll_x -
                2 * amount, ur_y - ll_y - 2 * amount, drawtype);
          }
        } else {
          let fb1, fb2;
          let rotationb;
          if (t === null) {
            rotationb = this.trf.rot();
          } else {
            rotationb = this.trf.rot(t);
          }

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
              fillcolor, fb1, fb2,
              this.ctx.gdraw.gradient_rotate(rotationb, grad));
        }
      }
    }
    if (this.border !== 0 || this.fill === 0) {
      drawtype =
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