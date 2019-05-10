class GrowBarChart extends GrowRect {
  static BARCHART_MAX_BARSEGMENTS = 12;
  static BARCHART_MAX_BARS = 200;
  bars;
  barsegments;
  min_value;
  max_value;
  vertical_lines;
  horizontal_lines;
  line_color;
  bar_color = new Array(GrowBarChart.BARCHART_MAX_BARSEGMENTS);
  bar_values =
      new Array(GrowBarChart.BARCHART_MAX_BARSEGMENTS * GrowBarChart.BARCHART_MAX_BARS);
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
        case GlowSave.GrowBarChart:
          break;
        case GlowSave.GrowBarChart_rect_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowBarChart_bars:
          this.bars = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_barsegments:
          this.barsegments = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowBarChart_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowBarChart_horizontal_lines:
          this.horizontal_lines = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_vertical_lines:
          this.vertical_lines = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_line_color:
          this.line_color = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color1:
          this.bar_color[0] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color2:
          this.bar_color[1] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color3:
          this.bar_color[2] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color4:
          this.bar_color[3] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color5:
          this.bar_color[4] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color6:
          this.bar_color[5] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color7:
          this.bar_color[6] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color8:
          this.bar_color[7] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color9:
          this.bar_color[8] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color10:
          this.bar_color[9] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color11:
          this.bar_color[10] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_bar_color12:
          this.bar_color[11] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowBarChart_userdata_cb:
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
          console.log("Syntax error in GrowBarChart");
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
    let rot = Math.floor(this.trf.rot(t));

    let ll_x = Math.min(x1, x2);
    let ur_x = Math.max(x1, x2);
    let ll_y = Math.min(y1, y2);
    let ur_y = Math.max(y1, y2);

    let grad = this.gradient;
    if (this.gradient === Gradient.No &&
        (node !== null && node.gradient !== Gradient.No) &&
        this.disable_gradient === 0) {
      grad = node.gradient;
    }

    let border_bar_down_ur_y;
    let brect_ll_x = 0;
    let brect_ll_y = 0;
    let brect_width = 0;
    let brect_height = 0;

    let width = (ur_x - ll_x) / this.bars;
    let bar_ur_x = ll_x;
    for (let j = 0; j < this.bars; j++) {
      let bar_ll_x = bar_ur_x;

      if (j === this.bars - 1) {
        bar_ur_x = ur_x;
      } else {
        bar_ur_x = ll_x + (j + 1) * width;
      }

      let bar_up_ll_y = ur_y;
      let f_bar_up_ll_y = ur_y;
      let bar_down_ur_y, f_bar_down_ur_y;
      if (this.min_value < 0) {
        bar_up_ll_y += this.min_value * (ur_y - ll_y) / (this.max_value - this.min_value);
        f_bar_up_ll_y = bar_up_ll_y;
        bar_down_ur_y = ur_y + this.min_value * (ur_y - ll_y) /
            (this.max_value - this.min_value);
        f_bar_down_ur_y = bar_down_ur_y;
      }

      for (let i = 0; i < this.barsegments + 1; i++) {
        if (typeof this.bar_values[i] === 'undefined') {
          continue;
        }
        let fillcolor = 0;

        if (this.min_value > 0 || this.bar_values[i * this.bars + j] > 0) {
          let skip = 0;
          let bar_up_ur_y = bar_up_ll_y;

          if (i === this.barsegments) {
            if (bar_up_ll_y <= ll_y) {
              skip = 1;
            } else {
              bar_up_ll_y = ll_y;
            }
            fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
                DrawType.FillHighlight, highlight, colornode, 1, 0);
          } else {
            if (this.bar_values[i * this.bars + j] <= this.min_value) {
              skip = 1;
            } else if (bar_up_ur_y <= ll_y) {
              skip = 1;
            } else {
              f_bar_up_ll_y -=
                  this.bar_values[i * this.bars + j] * (ur_y - ll_y) /
                  (this.max_value - this.min_value);
              bar_up_ll_y = Math.floor(f_bar_up_ll_y);
              if (bar_up_ll_y < ll_y) {
                bar_up_ll_y = ll_y;
              }

              fillcolor = GlowColor.get_drawtype(this.bar_color[i],
                  DrawType.FillHighlight, highlight, colornode, 1, 0);
            }
          }
          if (skip === 0) {
            if (grad === Gradient.No ||
                fillcolor === DrawType.ColorRed || i === this.barsegments) {
              if (chot === 0) {
                drawtype = fillcolor;
              } else {
                drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
              }
              this.ctx.gdraw.fill_rect(bar_ll_x, bar_up_ll_y, bar_ur_x -
                  bar_ll_x, bar_up_ur_y - bar_up_ll_y, drawtype);
            } else {
              let f1, f2;
              if (this.gradient_contrast >= 0) {
                f2 =
                    GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast /
                        2 + chot, null);
                f1 = GlowColor.shift_drawtype(
                    fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) +
                    chot, null);
              } else {
                f2 = GlowColor.shift_drawtype(
                    fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) +
                    chot, null);
                f1 =
                    GlowColor.shift_drawtype(fillcolor, this.gradient_contrast /
                        2 + chot, null);
              }
              this.ctx.gdraw.gradient_fill_rect(bar_ll_x,
                  bar_up_ll_y, bar_ur_x - bar_ll_x, bar_up_ur_y - bar_up_ll_y,
                  fillcolor, f1, f2, grad);
            }
          }
        } else {
          // negative value, draw bar downwards
          let skip = 0;
          let bar_down_ll_y = bar_down_ur_y;

          if (i === this.barsegments) {
            border_bar_down_ur_y = bar_down_ur_y;
            if (bar_down_ur_y >= ur_y) {
              skip = 1;
            } else {
              bar_down_ur_y = ur_y;
            }
            fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
                DrawType.FillHighlight, highlight, colornode, 1, 0);
          } else {
            if (this.bar_values[i * this.bars + j] >= 0) {
              skip = 1;
            } else if (bar_down_ll_y >= ur_y) {
              skip = 1;
            } else {
              f_bar_down_ur_y -=
                  this.bar_values[i * this.bars + j] * (ur_y - ll_y) /
                  (this.max_value - this.min_value);
              bar_down_ur_y = Math.floor(f_bar_down_ur_y);
              if (bar_down_ur_y > ur_y) {
                bar_down_ur_y = ur_y;
              }

              fillcolor = GlowColor.get_drawtype(this.bar_color[i],
                  DrawType.FillHighlight, highlight, colornode, 1, 0);
            }
          }

          if (!skip) {
            if (grad === Gradient.No ||
                fillcolor === DrawType.ColorRed || i === this.barsegments) {
              let drawtype = (chot === 0) ? fillcolor : GlowColor.shift_drawtype(fillcolor, chot, null);
              this.ctx.gdraw.fill_rect(bar_ll_x, bar_down_ll_y, bar_ur_x -
                  bar_ll_x, bar_down_ur_y - bar_down_ll_y, drawtype);
            } else {
              let f1, f2;
              if (this.gradient_contrast >= 0) {
                f2 =
                    GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast /
                        2 + chot, null);
                f1 = GlowColor.shift_drawtype(
                    fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) +
                    chot, null);
              } else {
                f2 = GlowColor.shift_drawtype(
                    fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) +
                    chot, null);
                f1 =
                    GlowColor.shift_drawtype(fillcolor, this.gradient_contrast /
                        2 + chot, null);
              }
              this.ctx.gdraw.gradient_fill_rect(bar_ll_x,
                  bar_down_ll_y, bar_ur_x - bar_ll_x, bar_down_ur_y -
                  bar_down_ll_y, fillcolor, f1, f2, grad);
            }
          }
        }

        if (this.border !== 0 && i === this.barsegments) {
          // Draw previous bar border
          if (j > 0) {
            let drawtype = GlowColor.get_drawtype(this.draw_type,
                DrawType.LineHighlight, highlight, colornode, 0, 0);

            this.ctx.gdraw.rect(brect_ll_x, brect_ll_y, brect_width,
                brect_height, drawtype, idx, 0);
          }
          if (this.min_value >= 0) {
            brect_ll_x = bar_ll_x;
            brect_ll_y = bar_up_ll_y;
            brect_width = bar_ur_x - bar_ll_x;
            brect_height = ur_y - bar_up_ll_y;
          } else {
            brect_ll_x = bar_ll_x;
            brect_ll_y = bar_up_ll_y;
            brect_width = bar_ur_x - bar_ll_x;
            brect_height = ur_y + this.min_value * (ur_y - ll_y) /
                (this.max_value - this.min_value) - bar_up_ll_y;
          }
          if (j === this.bars - 1) {
            // Draw last bar border
            let drawtype = GlowColor.get_drawtype(this.draw_type,
                DrawType.LineHighlight, highlight, colornode, 0, 0);

            this.ctx.gdraw.rect(brect_ll_x, brect_ll_y, brect_width,
                brect_height, drawtype, idx, 0);
          }

          // Draw negative bar border
          if (this.min_value < 0) {
            if (j > 0) {
              let drawtype = GlowColor.get_drawtype(this.draw_type,
                  DrawType.LineHighlight, highlight, colornode, 0, 0);

              this.ctx.gdraw.rect(brect_ll_x, brect_ll_y, brect_width,
                  brect_height, drawtype, idx, 0);
            }
            brect_ll_x = bar_ll_x;
            brect_ll_y = ur_y + this.min_value * (ur_y - ll_y) /
                (this.max_value - this.min_value);
            brect_width = bar_ur_x - bar_ll_x;
            brect_height = border_bar_down_ur_y - brect_ll_y;
            if (j === this.bars - 1) {
              // Draw last bar border
              let drawtype = GlowColor.get_drawtype(this.draw_type,
                  DrawType.LineHighlight, highlight, colornode, 0, 0);

              this.ctx.gdraw.rect(brect_ll_x, brect_ll_y, brect_width,
                  brect_height, drawtype, idx, 0);
            }
          }
        }
      }
    }

    let drawtype =
        GlowColor.get_drawtype(this.line_color, DrawType.LineHighlight,
            highlight, colornode, 0, 0);

    for (let i = 0; i < this.vertical_lines; i++) {
      let x = Math.floor(ll_x + (ur_x - ll_x) / (this.vertical_lines + 1) *
          (i + 1));
      this.ctx.gdraw.line(x, ll_y, x, ur_y, drawtype, 0, 0);
    }

    for (let i = 0; i < this.horizontal_lines; i++) {
      let y = Math.floor(ll_y + (ur_y - ll_y) / (this.horizontal_lines + 1) *
          (i + 1));
      this.ctx.gdraw.line(ll_x, y, ur_x, y, drawtype, 0, 0);
    }

    if (this.border !== 0) {
      drawtype =
          GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
              highlight, colornode, 0, 0);
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, idx,
          0);
    }
  }

  set_conf(bar_num, barsegment_num, min_val, max_val, vert_lines, horiz_lines,
           lcolor, color) {
    this.bars = bar_num;
    this.barsegments = barsegment_num;
    this.min_value = min_val;
    this.max_value = max_val;
    this.vertical_lines = vert_lines;
    this.horizontal_lines = horiz_lines;
    this.line_color = lcolor;
    for (let i = 0; i < this.bars; i++) {
      this.bar_color[i] = color[i];
    }
    this.draw();
  }

  get_conf() {
    let info = new GlowBarChartInfo();

    info.bars = this.bars;
    info.barsegments = this.barsegments;
    info.max_value = this.max_value;
    info.min_value = this.min_value;
    return info;
  }

  set_values(values1, values2, values3, values4, values5, values6, values7,
             values8, values9, values10, values11, values12) {
    let j = 0;
    [values1, values2, values3, values4, values5, values6, values7, values8,
      values9, values10, values11, values12].forEach(function (values) {
      if (j < this.barsegments && values !== null) {
        for (let i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values[i];
        }
      }
      j++;
    });
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

  getClassCycle(): Cycle {
    return Cycle.Slow;
  }

  get_background_object_limits(t, type, x, y, bo) {
    return 0;
  }
}