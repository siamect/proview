class GrowConGlue extends GrowNode {
  line_width_up;
  line_width_down;
  line_width_left;
  line_width_right;
  border;

  constructor(ctx) {
    super(ctx);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowConGlue:
          break;
        case GlowSave.GrowConGlue_line_width_up:
          this.line_width_up = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowConGlue_line_width_down:
          this.line_width_down = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowConGlue_line_width_left:
          this.line_width_left = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowConGlue_line_width_right:
          this.line_width_right = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowConGlue_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowConGlue_node_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowConGlue");
          break;
      }
    }

    return i;
  }

  draw(t = null, highlight = this.highlight, hot = this.hot, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let idx = Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        this.line_width - 1);
    idx += hot;

    let x1, y1, x2, y2;
    if (t === null) {
      x1 = this.ll_x * this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x;
      y1 = this.ll_y * this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y;
      x2 = this.ur_x * this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x;
      y2 = this.ur_y * this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y;
    } else {
      x1 = t.x(this.ll_x, this.ll_y) * this.ctx.mw.zoom_factor_x -
          this.ctx.mw.offset_x;
      y1 = t.y(this.ll_x, this.ll_y) * this.ctx.mw.zoom_factor_y -
          this.ctx.mw.offset_y;
      x2 = t.x(this.ur_x, this.ur_y) * this.ctx.mw.zoom_factor_x -
          this.ctx.mw.offset_x;
      y2 = t.y(this.ur_x, this.ur_y) * this.ctx.mw.zoom_factor_y -
          this.ctx.mw.offset_y;
    }

    let ll_x = Math.floor(Math.min(x1, x2) + 0.5);
    let ur_x = Math.floor(Math.max(x1, x2) + 0.5);
    let ll_y = Math.floor(Math.min(y1, y2) + 0.5);
    let ur_y = Math.floor(Math.max(y1, y2) + 0.5);
    let m_x = Math.floor((x1 + x2) / 2 + 0.5);
    let m_y = Math.floor((y1 + y2) / 2 + 0.5);

    let drawtype =
        GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
            highlight, colornode, 0, 0);

    let lw_down = this.line_width_down;
    let lw_right = this.line_width_right;
    let lw_up = this.line_width_up;
    let lw_left = this.line_width_left;

    let idx_up =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
            lw_up - 1);
    let idx_down =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
            lw_down - 1);
    let idx_left =
        Math.floor(this.ctx.mw.zoom_factor_x / this.ctx.mw.base_zoom_factor *
            lw_left - 1);
    let idx_right =
        Math.floor(this.ctx.mw.zoom_factor_x / this.ctx.mw.base_zoom_factor *
            lw_right - 1);

    idx = clamp(idx, 0, DRAW_TYPE_SIZE - 1);
    idx_up = clamp(idx_up + hot, 0, DRAW_TYPE_SIZE - 1);
    idx_down = clamp(idx_down + hot, 0, DRAW_TYPE_SIZE - 1);
    idx_left = clamp(idx_left + hot, 0, DRAW_TYPE_SIZE - 1);
    idx_right = clamp(idx_right + hot, 0, DRAW_TYPE_SIZE - 1);

    if (lw_up !== -1 && lw_down === -1 && lw_right === -1 && lw_left === -1) {
      // Up termination
      this.ctx.gdraw.rect(m_x - idx_up + idx_up / 2, m_y, idx_up +
          1, ur_y - m_y, drawtype, true, 0);
      if (this.shadow !== 0 && idx_up > 2) {
        if (this.border !== 0) {
          idx_up -= 2;
          m_y++;
        }

        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y, m_x + idx_up / 2, ur_y,
            shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x - idx_up +
            idx_up / 2, ur_y, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x + idx_up / 2,
            m_y, shift_drawtype, 0, 0);

        if (this.border !== 0) {
          idx_up += 2;
          m_y--;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x - idx_up +
            idx_up / 2, ur_y, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y, m_x + idx_up / 2, ur_y,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x + idx_up / 2,
            m_y, DrawType.Line, 0, 0);
      }
    } else if (lw_up === -1 && lw_down !== -1 && lw_right === -1 &&
        lw_left === -1) {
      // Down termination
      this.ctx.gdraw.rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
          1, m_y - ll_y, drawtype, true, 0);
      if (this.shadow !== 0 && idx_down > 2) {
        if (this.border !== 0) {
          idx_down -= 2;
          m_y--;
        }

        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y, m_x + idx_down /
            2, m_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y, shift_drawtype, 0, 0);

        if (this.border !== 0) {
          idx_down += 2;
          m_y++;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y, m_x + idx_down /
            2, m_y, DrawType.Line, 0, 0);
      }
    } else if (lw_up === -1 && lw_down === -1 && lw_right !== -1 &&
        lw_left === -1) {
      // Right termination
      this.ctx.gdraw.rect(m_x, m_y - idx_right + idx_right / 2, ur_x -
          m_x, idx_right + 1, drawtype, true, 0);
      if (this.shadow !== 0 && idx_right > 2) {
        if (this.border !== 0) {
          idx_right -= 2;
          m_x++;
        }

        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x, m_y + idx_right / 2, ur_x, m_y + idx_right / 2,
            shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, ur_x, m_y -
            idx_right + idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, m_x, m_y +
            idx_right / 2, shift_drawtype, 0, 0);

        if (this.border !== 0) {
          idx_right += 2;
          m_x--;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, ur_x, m_y -
            idx_right + idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x, m_y + idx_right / 2, ur_x, m_y + idx_right / 2,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, m_x, m_y +
            idx_right / 2, DrawType.Line, 0, 0);
      }
    } else if (lw_up === -1 && lw_down === -1 && lw_right === -1 &&
        lw_left !== -1) {
      // Left termination
      this.ctx.gdraw.rect(ll_x, m_y - idx_left + idx_left / 2, m_x -
          ll_x, idx_left + 1, drawtype, true, 0);
      if (this.shadow !== 0 && idx_left > 2) {
        if (this.border !== 0) {
          idx_left -= 2;
          m_x--;
        }

        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x, m_y + idx_left / 2,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_left + idx_left / 2, m_x, m_y +
            idx_left / 2, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x, m_y -
            idx_left + idx_left / 2, shift_drawtype, 0, 0);

        if (this.border !== 0) {
          idx_left += 2;
          m_x++;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x, m_y -
            idx_left + idx_left / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x, m_y + idx_left / 2,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_left + idx_left / 2, m_x, m_y +
            idx_left / 2, DrawType.Line, 0, 0);
      }
    } else if (lw_up !== -1 && lw_down !== -1 && lw_right === -1 &&
        lw_left === -1) {
      // Vertical glue
      this.ctx.gdraw.rect(m_x - idx_up + idx_up / 2, m_y, idx_up +
          1, ur_y - m_y, drawtype, true, 0);
      this.ctx.gdraw.rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
          1, m_y - ll_y, drawtype, true, 0);

      if (this.shadow !== 0 && idx_down > 2) {
        if (this.border !== 0) {
          idx_up -= 2;
          idx_down -= 2;
        }
        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x - idx_up +
            idx_up / 2, ur_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y, m_x + idx_up / 2, ur_y,
            shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_up += 2;
          idx_down += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x - idx_up +
            idx_up / 2, ur_y, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y, m_x + idx_up / 2, ur_y,
            DrawType.Line, 0, 0);
        if (idx_down !== idx_up) {
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x - idx_down +
              idx_down / 2, m_y, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y, m_x + idx_down / 2, m_y,
              DrawType.Line, 0, 0);
        }
      }
    } else if (lw_left !== -1 && lw_right !== -1 && lw_up === -1 &&
        lw_down === -1) {
      // Horizontal glue
      this.ctx.gdraw.rect(m_x, m_y - idx_right + idx_right / 2, ur_x -
          m_x, idx_right + 1, drawtype, true, 0);
      this.ctx.gdraw.rect(ll_x, m_y - idx_left + idx_left / 2, m_x -
          ll_x, idx_left + 1, drawtype, true, 0);
      if (this.shadow !== 0 && (idx_left > 2 || idx_right > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_right -= 2;
        }
        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x, m_y -
            idx_left + idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, ur_x, m_y -
            idx_right + idx_right / 2, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x, m_y + idx_left / 2,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x, m_y + idx_right / 2, ur_x, m_y + idx_right / 2,
            shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_left += 2;
          idx_right += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x, m_y -
            idx_left + idx_left / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x, m_y + idx_left / 2,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, ur_x, m_y -
            idx_right + idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x, m_y + idx_right / 2, ur_x, m_y + idx_right / 2,
            DrawType.Line, 0, 0);
        if (idx_left !== idx_right) {
          this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, m_x, m_y -
              idx_left + idx_left / 2, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x, m_y + idx_right / 2, m_x, m_y + idx_left / 2,
              DrawType.Line, 0, 0);
        }
      }
    } else if (lw_left !== -1 && lw_right === -1 && lw_up !== -1 &&
        lw_down === -1) {
      // Left up corner
      this.ctx.gdraw.rect(m_x - idx_up + idx_up / 2, m_y + idx_left /
          2, idx_up + 1, ur_y - (m_y + idx_left / 2), drawtype, true, 0);
      this.ctx.gdraw.rect(ll_x, m_y - idx_left + idx_left / 2, m_x +
          idx_up / 2 - ll_x + 1, idx_left + 1, drawtype, true, 0);
      if (this.shadow !== 0 && (idx_left > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_up -= 2;
        }
        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x + idx_up /
            2, m_y - idx_left + idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
            idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
            2, m_y + idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
            2, m_x + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_left += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x + idx_up /
            2, m_y - idx_left + idx_left / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
            2, m_y + idx_left / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
            idx_up + idx_up / 2, ur_y, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
            2, m_x + idx_up / 2, ur_y, DrawType.Line, 0, 0);
      }
    } else if (lw_left === -1 && lw_right !== -1 && lw_up !== -1 &&
        lw_down === -1) {
      // Right up corner
      this.ctx.gdraw.rect(m_x - idx_up + idx_up / 2, m_y - idx_right +
          idx_right / 2, idx_up + 1, ur_y - (m_y - idx_right + idx_right / 2),
          drawtype, true, 0);
      this.ctx.gdraw.rect(m_x + idx_up / 2, m_y - idx_right + idx_right /
          2, ur_x - (m_x + idx_up / 2), idx_right + 1, drawtype, true, 0);
      if (this.shadow !== 0 && (idx_right > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_right -= 2;
          idx_up -= 2;
        }
        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, ur_x, m_y - idx_right + idx_right / 2, shift_drawtype,
            0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, m_x - idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
            idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
            idx_up / 2, ur_y, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_right += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, ur_x, m_y - idx_right + idx_right / 2,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
            idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, m_x - idx_up + idx_up / 2, ur_y, DrawType.Line,
            0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
            idx_up / 2, ur_y, DrawType.Line, 0, 0);
      }
    } else if (lw_left === -1 && lw_right !== -1 && lw_up === -1 &&
        lw_down !== -1) {
      // Right down corner
      this.ctx.gdraw.rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
          1, m_y - idx_right + idx_right / 2 - ll_y, drawtype, true, 0);
      this.ctx.gdraw.rect(m_x - idx_down + idx_down / 2, m_y - idx_right +
          idx_right / 2, ur_x - (m_x - idx_down + idx_down / 2), idx_right + 1,
          drawtype, true, 0);
      if (this.shadow !== 0 && (idx_right > 2 || idx_down > 2)) {
        if (this.border !== 0) {
          idx_right -= 2;
          idx_down -= 2;
        }
        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
            ur_x, m_y - idx_right + idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y + idx_right / 2, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y + idx_right / 2,
            ur_x, m_y + idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
            idx_right + idx_right / 2, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_right += 2;
          idx_down += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y + idx_right / 2,
            ur_x, m_y + idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
            ur_x, m_y - idx_right + idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y + idx_right / 2, DrawType.Line, 0,
            0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
            idx_right + idx_right / 2, DrawType.Line, 0, 0);
      }
    } else if (lw_left !== -1 && lw_right === -1 && lw_up === -1 &&
        lw_down !== -1) {
      // Left down corner
      this.ctx.gdraw.rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
          1, m_y + idx_left / 2 - ll_y + 1, drawtype, true, 0);
      this.ctx.gdraw.rect(ll_x, m_y - idx_left + idx_left / 2, m_x -
          idx_down + idx_down / 2 - ll_x, idx_left + 1, drawtype, true, 0);
      if (this.shadow !== 0 && (idx_left > 2 || idx_down > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_down -= 2;
        }
        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x + idx_down / 2, m_y +
            idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y +
            idx_left / 2, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_left += 2;
          idx_down += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x + idx_down / 2, m_y +
            idx_left / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y +
            idx_left / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            DrawType.Line, 0, 0);
      }
    } else if (lw_left !== -1 && lw_right !== -1 && lw_up === -1 &&
        lw_down !== -1) {
      // Left right down threeway
      this.ctx.gdraw.rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
          1, m_y - ll_y + 1, drawtype, true, 0);
      this.ctx.gdraw.rect(ll_x, m_y - idx_left + idx_left / 2, m_x +
          idx_down / 2 - ll_x, idx_left + 1, drawtype, true, 0);
      this.ctx.gdraw.rect(m_x - idx_down + idx_down / 2, m_y - idx_right +
          idx_right / 2, ur_x - (m_x - idx_down + idx_down / 2), idx_right + 1,
          drawtype, true, 0);
      if (this.shadow !== 0 && (idx_left > 2 || idx_right > 2 || idx_down > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_right -= 2;
          idx_down -= 2;
        }
        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
            ur_x, m_y - idx_right + idx_right / 2, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
            idx_right + idx_right / 2, shift_drawtype, 0, 0);
        if (idx_right === idx_left) {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, ur_x, m_y + idx_left /
              2, shift_drawtype, 0, 0);
        } else if (idx_right > idx_left) {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_down +
              idx_down / 2, m_y + idx_left / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y + idx_right /
              2, ur_x, m_y + idx_right / 2, shift_drawtype, 0, 0);
        } else {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x + idx_down /
              2, m_y + idx_left / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_down / 2, m_y + idx_right / 2,
              ur_x, m_y + idx_right / 2, shift_drawtype, 0, 0);
        }
        if (this.border !== 0) {
          idx_left += 2;
          idx_right += 2;
          idx_down += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
            ur_x, m_y - idx_right + idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
            idx_right + idx_right / 2, DrawType.Line, 0, 0);
        if (idx_right === idx_left) {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, ur_x, m_y + idx_left /
              2, DrawType.Line, 0, 0);
        } else if (idx_right > idx_left) {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_down +
              idx_down / 2, m_y + idx_left / 2, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y + idx_right /
              2, ur_x, m_y + idx_right / 2, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y + idx_right /
              2, m_x - idx_down + idx_down / 2, m_y + idx_left / 2,
              DrawType.Line, 0, 0);
        } else {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x + idx_down /
              2, m_y + idx_left / 2, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_down / 2, m_y + idx_right / 2,
              ur_x, m_y + idx_right / 2, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_down / 2, m_y + idx_right / 2, m_x +
              idx_down / 2, m_y + idx_left / 2, DrawType.Line, 0, 0);
        }
      }
    } else if (lw_left !== -1 && lw_right !== -1 && lw_up !== -1 &&
        lw_down === -1) {
      // Left right up threeway
      this.ctx.gdraw.rect(m_x - idx_up + idx_up / 2, m_y, idx_up +
          1, ur_y - m_y, drawtype, true, 0);
      this.ctx.gdraw.rect(ll_x, m_y - idx_left + idx_left / 2, m_x +
          idx_up / 2 - ll_x, idx_left + 1, drawtype, true, 0);
      this.ctx.gdraw.rect(m_x - idx_up + idx_up / 2, m_y - idx_right +
          idx_right / 2, ur_x - (m_x - idx_up + idx_up / 2), idx_right + 1,
          drawtype, true, 0);
      if (this.shadow !== 0 && (idx_left > 2 || idx_right > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_right -= 2;
          idx_up -= 2;
        }
        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
            idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        if (idx_right === idx_left) {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, ur_x, m_y -
              idx_left + idx_left / 2, shift_drawtype, 0, 0);
        } else if (idx_right > idx_left) {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
              idx_up + idx_up / 2, m_y - idx_left + idx_left / 2, shift_drawtype,
              0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
              idx_right / 2, ur_x, m_y - idx_right + idx_right / 2,
              shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
              idx_right / 2, m_x - idx_up + idx_up / 2, m_y - idx_left +
              idx_left / 2, shift_drawtype, 0, 0);
        } else {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x +
              idx_up / 2, m_y - idx_left + idx_left / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_right + idx_right / 2,
              ur_x, m_y - idx_right + idx_right / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_right + idx_right /
              2, m_x + idx_up / 2, m_y - idx_left + idx_left / 2, shift_drawtype,
              0, 0);
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
            2, m_y + idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
            idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
            idx_up / 2, ur_y, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_left += 2;
          idx_right += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
            2, m_y + idx_left / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
            idx_up + idx_up / 2, ur_y, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
            idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
            idx_up / 2, ur_y, DrawType.Line, 0, 0);
        if (idx_right === idx_left) {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, ur_x, m_y -
              idx_left + idx_left / 2, DrawType.Line, 0, 0);
        } else if (idx_right > idx_left) {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
              idx_up + idx_up / 2, m_y - idx_left + idx_left / 2,
              DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
              idx_right / 2, ur_x, m_y - idx_right + idx_right / 2,
              DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
              idx_right / 2, m_x - idx_up + idx_up / 2, m_y - idx_left +
              idx_left / 2, DrawType.Line, 0, 0);
        } else {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x +
              idx_up / 2, m_y - idx_left + idx_left / 2, DrawType.Line, 0,
              0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_right + idx_right / 2,
              ur_x, m_y - idx_right + idx_right / 2, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_right + idx_right /
              2, m_x + idx_up / 2, m_y - idx_left + idx_left / 2,
              DrawType.Line, 0, 0);
        }
      }
    } else if (lw_left !== -1 && lw_right === -1 && lw_up !== -1 &&
        lw_down !== -1) {
      // Left up down threeway
      this.ctx.gdraw.rect(ll_x, m_y - idx_left + idx_left / 2, m_x -
          ll_x, idx_left + 1, drawtype, true, 0);
      this.ctx.gdraw.rect(m_x - idx_up + idx_up / 2, m_y - idx_left +
          idx_left / 2, idx_up + 1, ur_y - (m_y - idx_left + idx_left / 2),
          drawtype, true, 0);
      this.ctx.gdraw.rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
          1, m_y + idx_left / 2 - ll_y, drawtype, true, 0);
      if (this.shadow !== 0 && (idx_left > 2 || idx_down > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_down -= 2;
          idx_up -= 2;
        }
        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
            idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
            2, m_y + idx_left / 2, shift_drawtype, 0, 0);

        if (idx_up === idx_down) {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2,
              ur_y, shift_drawtype, 0, 0);
        } else if (idx_down > idx_up) {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down /
              2, m_y + idx_left / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_left / 2, m_x +
              idx_up / 2, ur_y, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_left / 2, m_x +
              idx_down / 2, m_y + idx_left / 2, shift_drawtype, 0, 0);
        } else {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down /
              2, m_y - idx_left + idx_left / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
              2, m_x + idx_up / 2, ur_y, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
              2, m_x + idx_down / 2, m_y - idx_left + idx_left / 2,
              shift_drawtype, 0, 0);
        }
        if (this.border !== 0) {
          idx_left += 2;
          idx_down += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
            2, m_y + idx_left / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
            idx_up + idx_up / 2, ur_y, DrawType.Line, 0, 0);

        if (idx_up === idx_down) {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2,
              ur_y, DrawType.Line, 0, 0);
        } else if (idx_down > idx_up) {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down /
              2, m_y + idx_left / 2, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_left / 2, m_x +
              idx_up / 2, ur_y, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_left / 2, m_x +
              idx_down / 2, m_y + idx_left / 2, DrawType.Line, 0, 0);
        } else {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down /
              2, m_y - idx_left + idx_left / 2, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
              2, m_x + idx_up / 2, ur_y, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
              2, m_x + idx_down / 2, m_y - idx_left + idx_left / 2,
              DrawType.Line, 0, 0);
        }
      }
    } else if (lw_left === -1 && lw_right !== -1 && lw_up !== -1 &&
        lw_down !== -1) {
      // Right up down threeway
      this.ctx.gdraw.rect(m_x, m_y - idx_right + idx_right / 2, ur_x -
          m_x, idx_right + 1, drawtype, true, 0);
      this.ctx.gdraw.rect(m_x - idx_up + idx_up / 2, m_y - idx_right +
          idx_right / 2, idx_up + 1, ur_y - (m_y - idx_right + idx_right / 2),
          drawtype, true, 0);
      this.ctx.gdraw.rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
          1, m_y + idx_right / 2 - ll_y, drawtype, true, 0);
      if (this.shadow !== 0 && (idx_right > 2 || idx_down > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_right -= 2;
          idx_down -= 2;
          idx_up -= 2;
        }
        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
            ur_x, m_y - idx_right + idx_right / 2, shift_drawtype, 0, 0);

        if (idx_up === idx_down) {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
              idx_down + idx_down / 2, ur_y, shift_drawtype, 0, 0);
        } else if (idx_down > idx_up) {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
              idx_down + idx_down / 2, m_y + idx_right / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_right /
              2, m_x - idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
          // this.ctx.gdraw.line( m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, 0);
        } else {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
              idx_down + idx_down / 2, m_y - idx_right + idx_right / 2,
              shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
              idx_right / 2, m_x - idx_up + idx_up / 2, ur_y, shift_drawtype, 0,
              0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
              idx_right / 2, m_x - idx_down + idx_down / 2, m_y - idx_right +
              idx_right / 2, shift_drawtype, 0, 0);
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
            idx_right + idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
            idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
            idx_up / 2, ur_y, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_right += 2;
          idx_down += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
            ur_x, m_y - idx_right + idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
            idx_right + idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
            idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
            idx_up / 2, ur_y, DrawType.Line, 0, 0);

        if (idx_up === idx_down) {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
              idx_down + idx_down / 2, ur_y, DrawType.Line, 0, 0);
        } else if (idx_down > idx_up) {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
              idx_down + idx_down / 2, m_y + idx_right / 2, DrawType.Line,
              0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_right /
              2, m_x - idx_up + idx_up / 2, ur_y, DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_right /
              2, m_x - idx_down + idx_down / 2, m_y + idx_right / 2,
              DrawType.Line, 0, 0);
        } else {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
              idx_down + idx_down / 2, m_y - idx_right + idx_right / 2,
              DrawType.Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
              idx_right / 2, m_x - idx_up + idx_up / 2, ur_y, DrawType.Line,
              0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
              idx_right / 2, m_x - idx_down + idx_down / 2, m_y - idx_right +
              idx_right / 2, DrawType.Line, 0, 0);
        }
      }
    } else if (lw_left !== -1 && lw_right !== -1 && lw_up !== -1 &&
        lw_down !== -1) {
      // Right left up down fourway
      this.ctx.gdraw.rect(m_x, m_y - idx_right + idx_right / 2, ur_x -
          m_x, idx_right + 1, drawtype, true, 0);
      this.ctx.gdraw.rect(ll_x, m_y - idx_left + idx_left / 2, m_x -
          ll_x, idx_left + 1, drawtype, true, 0);
      this.ctx.gdraw.rect(m_x - idx_up + idx_up / 2, m_y, idx_up +
          1, ur_y - m_y, drawtype, true, 0);
      this.ctx.gdraw.rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
          1, m_y - ll_y, drawtype, true, 0);
      if (this.shadow !== 0 &&
          (idx_left > 2 || idx_right > 2 || idx_down > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_right -= 2;
          idx_left -= 2;
          idx_down -= 2;
          idx_up -= 2;
        }
        let shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
            ur_x, m_y - idx_right + idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
            idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
            idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
            2, m_y + idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
            idx_up / 2, ur_y, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
            idx_right + idx_right / 2, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_right += 2;
          idx_left += 2;
          idx_down += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
            ur_x, m_y - idx_right + idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
            idx_right + idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
            idx_right / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
            idx_up / 2, ur_y, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
            DrawType.Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
            2, m_y + idx_left / 2, DrawType.Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
            idx_up + idx_up / 2, ur_y, DrawType.Line, 0, 0);
      }
    } else {
      this.ctx.gdraw.line(m_x, ll_y, m_x, m_y, drawtype, idx, 0);
      this.ctx.gdraw.line(m_x, m_y, m_x, ur_y, drawtype, idx, 0);
      this.ctx.gdraw.line(ll_x, m_y, m_x, m_y, drawtype, idx, 0);
      this.ctx.gdraw.line(m_x, m_y, ur_x, m_y, drawtype, idx, 0);
    }
  }

  event_handler(event, fx, fy) {
    return 0;
  }

  get_borders(t, g) {
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}