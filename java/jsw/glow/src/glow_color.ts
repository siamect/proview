class Sat {
  s: number;
  i_min: number;
  i_max: number;
  a: number;

  constructor(s, i_min, i_max, a) {
    this.s = s;
    this.i_min = i_min;
    this.i_max = i_max;
    this.a = a;
  }
}

class Hue {
  h: number;
  hk: number;
  s: Array<Sat>;

  constructor(h, hk, s) {
    this.h = h;
    this.hk = hk;
    this.s = s;
  }
}

class Rgb {
  r: number;
  g: number;
  b: number;

  constructor(r, g, b) {
    this.r = r;
    this.g = g;
    this.b = b;
  }
}

class GlowColor {
  static gray_i0 = 0.32;		// Gray min
  static gray_i1 = 0.95;		// Gray max

  static ctab = [new Hue(18, -20.0,
      [new Sat(0.2, -1.4, 2.8, 0.9), new Sat(0.5, -1.4, 3, 0.9),
        new Sat(1.5, -3.5, 5.0, 1.0)]), new Hue(28, 10.0,
      [new Sat(0.2, -1.4, 2.8, 0.9), new Sat(0.5, -1.4, 3, 0.9),
        new Sat(1.5, -3.5, 5.0, 1.0)]),  // Yellow
    new Hue(45, 20.0, [new Sat(0.2, -1.3, 2.8, 0.9), new Sat(0.5, -1.3, 3, 0.9),
      new Sat(1.5, -3.5, 4.5, 1.0)]), // Orange
    new Hue(90, 0.0, [new Sat(0.2, -1.1, 2.8, 0.9), new Sat(0.5, -1.1, 3, 0.9),
      new Sat(1.5, -4.5, 4.5, 0.8)]), // Red
    new Hue(150, 0.0, [new Sat(0.2, -1.0, 2.8, 0.9), new Sat(0.5, -1.0, 3, 0.9),
      new Sat(1.5, -3.5, 5.9, 1.0)]), // Violet
    new Hue(240, 0.0, [new Sat(0.2, -1.0, 2.8, 0.9), new Sat(0.4, -1.0, 3, 0.9),
      new Sat(1.5, -3.5, 5.5, 0.8)]), // Blue
    new Hue(280, 0.0, [new Sat(0.2, -1.0, 2.8, 0.9), new Sat(0.4, -1.0, 3, 0.9),
      new Sat(1.5, -3.5, 5.5, 1.2)]), // Seablue
    new Hue(355, -20.0,
        [new Sat(0.2, -1.0, 2.8, 0.9), new Sat(0.4, -1.0, 3, 0.9),
          new Sat(1.5, -4.5, 3.5, 1.0)])  // Green
  ];

  static rgbtab = [new Rgb(0.254, 0.329, 0),  // YellowGreen
    new Rgb(0.357, 0.459, 0), new Rgb(0.498, 0.639, 0),
    new Rgb(0.624, 0.800, 0), new Rgb(0.764, 0.976, 0),
    new Rgb(0.808, 1.000, 0.129), new Rgb(0.847, 1.000, 0.310),
    new Rgb(0.898, 1.000, 0.537), new Rgb(0.933, 1.000, 0.710),
    new Rgb(0.949, 1.000, 0.776), new Rgb(0.0, 0.0, 0),		// Yellow
    new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0),
    new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0),
    new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0),
    new Rgb(0.0, 0.0, 0),		// Orange
    new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0),
    new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0), new Rgb(1.000, 0.725, 0.420),
    new Rgb(1.000, 0.796, 0.569), new Rgb(1.000, 0.886, 0.667),
    new Rgb(1.000, 0.937, 0.820)];

  static is_shiftable(dt) {
    return (dt >= 20 && dt <= DrawType.Color300);
  }

  static is_custom(dt) {
    return (dt >= DrawType.CustomColor1 && dt <= DrawType.CustomColor__);
  }

  static shift_drawtype(dt, shift, node) {
    let drawtype;

    if (node !== null && node.getColorInverse() !== 0) {
      shift = -shift;
    }

    if (this.is_shiftable(dt)) {
      let base_drawtype = Math.floor(dt / 10) * 10;
      let incr = shift + dt - base_drawtype;

      if (incr < 0) { // White
        drawtype = DrawType.Color4;
      } else if (incr >= 10) { // DarkGrey
        drawtype = DrawType.Color30;
      } else {
        drawtype = base_drawtype + incr;
      }
    } else if (this.is_custom(dt)) {
      if (shift === -1) {
        drawtype = (dt + 3);
      } else if (shift < 0) {
        drawtype = (dt + 2);
      } else if (shift > 0) {
        drawtype = (dt + 1);
      } else {
        drawtype = dt;
      }
    } else {
      drawtype = dt;
    }

    return Math.floor(drawtype);
  }

  static get_drawtype(local_drawtype, highlight_drawtype, highlight, node, fill,
                      highlight_disabled) {
    let drawtype;
    let lightness = (node === null) ? 0 : node.getColorLightness();
    let intensity = (node === null) ? 0 : node.getColorIntensity();

    if (highlight !== 0 && highlight_disabled === 0) {
      drawtype = highlight_drawtype;
    } else if (highlight !== 0 && highlight_disabled !== 0 && fill === 0 &&
        local_drawtype === DrawType.Line) {
      drawtype = highlight_drawtype;
    } else {
      if (node !== null && node.getColorTone() !== DrawTone.No) {
        let tone = node.getColorTone();

        if (local_drawtype > 30 && local_drawtype < 300) {
          if (tone === DrawTone.LightGrayHighSaturation) { // GrayLow1
            drawtype = DrawType.Color31;
          } else if (tone === DrawTone.DarkGrayHighSaturation) { // DarkGrey
            drawtype = DrawType.Color30;
          } else {
            if (tone >= DrawTone.GrayHighSaturation &&
                tone < DrawTone.GrayLowSaturation) {
              tone -= 27;
              intensity += 2;
            } else if (tone >= DrawTone.GrayLowSaturation) {
              tone -= 2 * 27;
              intensity -= 1;
            }
            if (tone >= DrawTone.DarkGray &&
                tone < DrawTone.LightGray) {
              tone = (tone - 9);
              lightness -= 2;
            } else if (tone >= DrawTone.LightGray) {
              tone -= 18;
              lightness += 2;
            }

            drawtype =
                local_drawtype - Math.floor(local_drawtype / 30) * 30 + 30 * tone;
          }
        } else {
          drawtype = local_drawtype;
        }
      } else if (node !== null && fill === 1 &&
          node.getFillDrawtype() !== DrawType.No) {
        drawtype = node.getFillDrawtype();
      } else if (node !== null && fill === 2 &&
          node.getTextDrawtype() !== DrawType.No) {
        drawtype = node.getTextDrawtype();
      } else if (node !== null && fill === 3 &&
          node.getBackgroundDrawtype() !== DrawType.No) {
        drawtype = node.getBackgroundDrawtype();
      } else if (node !== null && fill === 0 &&
          node.getDrawtype() !== DrawType.No) {
        drawtype = node.getDrawtype();
      } else {
        drawtype = local_drawtype;
      }

      if (node !== null && lightness !== 0) {
        if (local_drawtype >= 30 && drawtype < 300) {
          let base_drawtype = Math.floor(drawtype / 10) * 10;
          let incr = -lightness + drawtype - base_drawtype;
          if (incr < 0) { // White
            drawtype = DrawType.Color4;
          } else if (incr >= 10) { // DarkGrey
            drawtype = DrawType.Color30;
          } else {
            drawtype = base_drawtype + incr;
          }
        }
      }
      if (node !== null && intensity !== 0) {
        if (drawtype >= 60) {
          let base_drawtype = Math.floor(drawtype / 30) * 30;
          let incr = drawtype - base_drawtype;
          drawtype =
              (drawtype + Math.min(2 - Math.floor(incr / 10), intensity) * 10);
          if (drawtype < base_drawtype) {
            drawtype = (DrawType.Color41 + incr);
          }
        }
      }
      if (node !== null && node.getColorShift() !== 0) {
        if (drawtype >= 60 && drawtype < 300) {
          let incr =
              node.getColorShift() - Math.floor(node.getColorShift() / 8) * 8;
          if (incr < 0) {
            incr += 8;
          }

          incr = drawtype + incr * 30;
          if (incr >= 300) {
            incr -= 240;
          }
          drawtype = incr;
        }
      }
      if (node !== null && node.getColorInverse() !== 0) {
        if (drawtype >= 30 && drawtype < 300) {
          drawtype = drawtype + 10 - 2 * (drawtype % 10) - 1;
        } else if (this.is_custom(drawtype)) {
          if ((drawtype - DrawType.CustomColor1) % 4 === 1) {
            drawtype += 1;
          } else if ((drawtype - DrawType.CustomColor1) % 4 === 2) {
            drawtype -= 1;
          } else if ((drawtype - DrawType.CustomColor1) % 4 === 3) {
            drawtype -= 2;
          }
        }
      }
      if (node !== null && node.getDimmed() !== 0) {
        if (drawtype === 0) {
          drawtype = 25;
        } else if (drawtype >= 26 && drawtype <= 29) {
          drawtype = (drawtype - 4);
        } else if (drawtype >= 36 && drawtype <= 39) {
          drawtype = (drawtype - 4);
        } else if (drawtype >= 46 && drawtype <= 49) {
          drawtype = (drawtype - 4);
        } else if (drawtype >= 56 && drawtype <= 59) {
          drawtype = (drawtype - 4);
        } else if (this.is_custom(drawtype)) {
          if (drawtype === DrawType.CustomColor37) { // Button border color
            drawtype = DrawType.CustomColor40;
          }
          if (drawtype === DrawType.CustomColor38) { // Button text color
            drawtype = DrawType.CustomColor39;
          }
        }
      }
    }
    if (!((drawtype >= 0 && drawtype <= DrawType.Color__) ||
            (drawtype >= DrawType.CustomColor1 && drawtype <
                DrawType.CustomColor__))) {
      console.log("** Invalid drawtype : ", drawtype);
      drawtype = DrawType.Line;
    }
    return drawtype;
  }

  static rgb_color(idx, customcolors) {
    if (idx === 300) {
      idx = 31;
    }

    if (idx === 3) { // White
      return new Rgb(1, 1, 1);
    } else if (idx === 2) { // Gray
      return new Rgb(0.75, 0.75, 0.75);
    } else if (idx === 1) { // Red
      return new Rgb(1, 0, 0);
    } else if (idx < 20) {
      // Sixteen colors of different hue
      return this.his_to_rgb(360.0 * (idx - 4) / 16, 1, 1.5);
    } else if (idx < 60) {
      // Four sets of gray
      let i0 = GlowColor.gray_i0;

      if (idx < 30 || (idx >= 50 && idx < 60)) {
        i0 = 0.25;
      }

      let r = i0 + (GlowColor.gray_i1 - i0) * Math.pow((9 - idx % 10) / 9, 0.9);
      return new Rgb(r, r, r);
    } else if (idx < 300) {
      let i = Math.floor((idx - 60) / 30);
      let j = Math.floor((idx - 60 - i * 30) / 10);
      let k = 9 - (idx - 60 - i * 30 - j * 10);

      if ((i === 0 && j === 2) || (i === 2 && j === 2 && k > 5)) {
        // Formula doesn't work for yellow...
        return new Rgb(this.rgbtab[i * 10 + k].r, this.rgbtab[i * 10 + k].g, this.rgbtab[i * 10 + k].b);
      } else {
        let s1 = this.ctab[i].s[j].s;
        let i1 = this.ctab[i].s[j].i_min +
            (this.ctab[i].s[j].i_max - this.ctab[i].s[j].i_min) *
            Math.pow(k / 9, this.ctab[i].s[j].a);
        let h1 = this.ctab[i].h + this.ctab[i].hk * k / 9;

        return this.his_to_rgb(h1, i1, s1);
      }
    } else if (idx >= DrawType.CustomColor1 &&
        idx < DrawType.CustomColor__) {
      if (customcolors === null) {
        return new Rgb(1, 1, 1);
      } else {
        return customcolors.get_color(idx);
      }
    }
    return new Rgb(0, 0, 0);
  }

  static his_to_rgb(h, i, s) {
    let m1 = s * Math.sin(h * Math.PI / 180);
    let m2 = s * Math.cos(h * Math.PI / 180);
    let i1 = i / Math.sqrt(3);

    let r = m1 * 2 / Math.sqrt(6) + i1 / Math.sqrt(3);
    let g = -m1 / Math.sqrt(6) + m2 / Math.sqrt(2) + i1 / Math.sqrt(3);
    let b = -m1 / Math.sqrt(6) - m2 / Math.sqrt(2) + i1 / Math.sqrt(3);

    r = clamp(r / 2 + 0.5, 0, 1);
    g = clamp(g / 2 + 0.5, 0, 1);
    b = clamp(b / 2 + 0.5, 0, 1);
    return new Rgb(r, g, b);
  }
}

class GlowCustomColors {
  debug = false;
  colors_size = DrawType.CustomColor__ - DrawType.CustomColor1;
  colors = new Array(this.colors_size);
  colortheme_lightness = 0;
  is_default_colortheme = 0;

  constructor() {
    for (let i = 0; i < this.colors_size; i++) {
      this.colors[i] = new Array(3);
      this.colors[i][0] = 1;
      this.colors[i][1] = 1;
      this.colors[i][2] = 1;
    }
  }

  get_color(dtype) {
    if (dtype < DrawType.CustomColor1 || dtype >= DrawType.CustomColor__) {
      return null;
    }

    let idx = dtype - DrawType.CustomColor1;
    return new Rgb(this.colors[idx][0], this.colors[idx][1], this.colors[idx][2]);
  }

  open(lines, row) {
    let i;
    let csize = 0;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.CustomColors:
          break;
        case GlowSave.CustomColors_colors_size:
          csize = parseInt(tokens[1], 10);
          break;
        case GlowSave.CustomColors_colortheme_lightness:
          this.colortheme_lightness = parseInt(tokens[1], 10);
          break;
        case GlowSave.CustomColors_is_default_colortheme:
          this.is_default_colortheme = parseInt(tokens[1], 10);
          break;
        case GlowSave.CustomColors_colors:
          for (let j = 0; j < csize; j++) {
            i++;
            tokens = lines[i].split(' ');
            this.colors[j][0] = parseFloat(tokens[0]);
            this.colors[j][1] = parseFloat(tokens[1]);
            this.colors[j][2] = parseFloat(tokens[2]);
            if (this.debug) {
              console.log(j, this.colors[j][0], this.colors[j][1],
                  this.colors[j][2]);
            }
          }
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowCustomColors");
          break;
      }
    }

    return i;
  }

  read_colorfile(ctx, name) {
    ctx.appl.frame.readGrowWeb(name, this.read_cb);
  }

  read_cb(lines, row) {
    this.open(lines, row);
  }
}