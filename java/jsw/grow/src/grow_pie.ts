class GrowPie extends GrowArc {
  static PIE_MAX_SECTORS = 12;
  sectors;
  min_value;
  max_value;
  sector_color = new Array(GrowPie.PIE_MAX_SECTORS);
  sector_size = new Array(GrowPie.PIE_MAX_SECTORS);
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
        case GlowSave.GrowPie:
          break;
        case GlowSave.GrowPie_arc_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowPie_sectors:
          this.sectors = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_color1:
          this.sector_color[0] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_color2:
          this.sector_color[1] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_color3:
          this.sector_color[2] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_color4:
          this.sector_color[3] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_color5:
          this.sector_color[4] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_color6:
          this.sector_color[5] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_color7:
          this.sector_color[6] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_color8:
          this.sector_color[7] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_color9:
          this.sector_color[8] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_color10:
          this.sector_color[9] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_color11:
          this.sector_color[10] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_color12:
          this.sector_color[11] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowPie_sector_size1:
          this.sector_size[0] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_size2:
          this.sector_size[1] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_size3:
          this.sector_size[2] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_size4:
          this.sector_size[3] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_size5:
          this.sector_size[4] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_size6:
          this.sector_size[5] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_size7:
          this.sector_size[6] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_size8:
          this.sector_size[7] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_size9:
          this.sector_size[8] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_size10:
          this.sector_size[9] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_size11:
          this.sector_size[10] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_sector_size12:
          this.sector_size[11] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowPie_userdata_cb:
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
          console.log("Syntax error in GrowPie");
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
    if (this.ctx.hot_indication === HotIndication.No) {
      hot = 0;
    } else if (this.ctx.hot_indication === HotIndication.DarkColor) {
      chot = hot;
      hot = 0;
    } else if (this.ctx.hot_indication === HotIndication.LightColor) {
      chot = -hot;
      hot = 0;
    }

    if (this.fixcolor !== 0) {
      colornode = null;
    }

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

    let tmp = Matrix.multiply(t, this.trf);
    let p1 = tmp.apply(this.ll);
    let p2 = tmp.apply(this.ur);

    let x1 = Math.floor(p1.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y1 = Math.floor(p1.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let x2 = Math.floor(p2.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let y2 = Math.floor(p2.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    let rot = t ? this.trf.rotation + t.rotation : this.trf.rotation;
    rot = Math.floor(rot);

    let ll_x = Math.min(x1, x2);
    let ur_x = Math.max(x1, x2);
    let ll_y = Math.min(y1, y2);
    let ur_y = Math.max(y1, y2);

    let display_shadow = ((node !== null && node.shadow !== 0) ||
        this.shadow !== 0) && this.disable_shadow === 0;
    let grad = this.gradient;
    if (this.gradient === Gradient.No &&
        (node !== null && node.gradient !== Gradient.No) &&
        this.disable_gradient === 0) {
      grad = node.gradient;
    }

    let a1 = this.angle1;
    let a2 = 0;
    let ia1 = this.angle1;
    let ia2;
    for (let i = 0; i < this.sectors + 1; i++) {
      let fillcolor;

      if (i === this.sectors) {
        if (ia1 >= this.angle1 + this.angle2) {
          break;
        }
        ia2 = this.angle1 + this.angle2 - ia1;
        fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
            DrawType.FillHighlight, highlight, colornode, 1, 0);
      } else {
        if (this.sector_size[i] <= this.min_value) {
          continue;
        }
        if (a1 >= this.angle2 + this.angle1) {
          break;
        }
        a2 =
            this.sector_size[i] / (this.max_value - this.min_value) * this.angle2;
        if (a1 + a2 > this.angle1 + this.angle2) {
          a2 = this.angle1 + this.angle2 - a1;
        }
        ia2 = Math.floor(a2 + a1 - ia1 + 0.5);

        fillcolor = GlowColor.get_drawtype(this.sector_color[i],
            DrawType.FillHighlight, highlight, colornode, 1, 0);
      }

      if (grad === Gradient.No || fillcolor === DrawType.ColorRed) {
        let drawtype = (chot === 0) ? fillcolor : GlowColor.shift_drawtype(fillcolor, chot, null);
        this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, ia1 - rot,
            ia2, drawtype, true, 0);
      } else if (!display_shadow || this.shadow_width === 0) {
        let f1, f2;
        if (this.gradient_contrast >= 0) {
          f2 = GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
              chot, null);
          f1 = GlowColor.shift_drawtype(
              fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
              null);
        } else {
          f2 = GlowColor.shift_drawtype(
              fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
              null);
          f1 = GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
              chot, 0);
        }
        this.ctx.gdraw.gradient_fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y -
            ll_y, ia1 - rot, ia2, fillcolor, f1, f2, grad);
      } else {
        let ish = Math.floor(this.shadow_width / 100 *
            Math.min(ur_x - ll_x, ur_y - ll_y) + 0.5);
        let drawtype_incr = this.shadow_contrast;
        if (this.relief === Relief.Down) {
          drawtype_incr = -this.shadow_contrast;
        }

        let f1, f2;

        // Draw shadow
        f1 =
            GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot, colornode);
        f2 =
            GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot, colornode);

        this.ctx.gdraw.gradient_fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y -
            ll_y, ia1 - rot, ia2, fillcolor, f2, f1,
            Gradient.DiagonalUpperLeft);

        // Draw circle
        if (this.gradient_contrast >= 0) {
          f2 = GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
              chot, null);
          f1 = GlowColor.shift_drawtype(
              fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
              null);
        } else {
          f2 = GlowColor.shift_drawtype(
              fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
              null);
          f1 = GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
              chot, null);
        }
        this.ctx.gdraw.gradient_fill_arc(ll_x + ish, ll_y + ish, ur_x - ll_x -
            2 * ish, ur_y - ll_y - 2 * ish, ia1 - rot, ia2, fillcolor, f1, f2,
            grad);
      }
      a1 += a2;
      ia1 += ia2;
    }

    if (this.border !== 0) {
      let drawtype =
          GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
              highlight, colornode, 0, 0);

      this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1 -
          rot, this.angle2, drawtype, idx, 0);
    }
  }

  set_conf(sector_num, min_val, max_val, color) {
    this.sectors = sector_num;
    this.min_value = min_val;
    this.max_value = max_val;
    for (let i = 0; i < this.sectors; i++) {
      this.sector_color[i] = color[i];
    }
    this.draw();
  }

  get_conf() {
    let info = new GlowPieInfo();
    info.sector_num = this.sectors;
    info.min_val = this.min_value;
    info.max_val = this.max_value;
    return info;
  }

  set_values(values) {
    for (let i = 0; i < this.sectors; i++) {
      this.sector_size[i] = values[i];
    }
    this.draw();
  }

  getUserData() {
    return this.userdata;
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
    return Cycle.Slow;
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}