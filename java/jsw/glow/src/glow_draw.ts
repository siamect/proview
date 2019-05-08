class GlowDraw {
  ctx: GrowCtx;
  canvas: HTMLCanvasElement;
  gctx: CanvasRenderingContext2D;
  offset_top: number;
  clip_x1 = new Array(10);
  clip_y1 = new Array(10);
  clip_x2 = new Array(10);
  clip_y2 = new Array(10);
  clipCount = 0;
  CUSTOMCOLORS_STACK_SIZE = 10;
  customcolors = new Array(this.CUSTOMCOLORS_STACK_SIZE);
  customcolors_cnt = 0;
  nodraw: boolean;

  constructor(ctx) {
    this.ctx = ctx;
    this.canvas = document.querySelector("canvas");
    this.gctx = this.canvas.getContext("2d");
    this.offset_top = this.canvas.offsetTop;
    this.gctx.save();
  }

  rect(x, y, width, height, gc_type, idx, highlight) {
    this.gctx.lineWidth = idx + 1;
    this.gctx.strokeStyle = this.getColor(gc_type);
    this.gctx.strokeRect(x, y, width, height);
  }

  fill_rect(x, y, width, height, gc_type) {
    this.gctx.fillStyle = this.getColor(gc_type);
    this.gctx.fillRect(x, y, width, height);
  }

  line(x1, y1, x2, y2, gc_type, idx, highlight) {
    this.gctx.strokeStyle = this.getColor(gc_type);
    this.gctx.lineWidth = idx + 1;
    this.gctx.beginPath();
    this.gctx.moveTo(x1, y1);
    this.gctx.lineTo(x2, y2);
    this.gctx.stroke();
  }

  line_dashed(x1, y1, x2, y2, gc_type, idx, highlight, line_type) {
    let dash = null;
    switch (line_type) {
      case LineType.Solid:
        break;
      case LineType.Dashed1:
        dash = [idx, idx];
        break;
      case LineType.Dashed2:
        dash = [idx * 2, idx * 2];
        break;
      case LineType.Dashed3:
        dash = [idx * 4, idx * 4];
        break;
      case LineType.Dotted:
        dash = [idx, idx * 6];
        break;
      case LineType.DotDashed1:
        dash = [idx, idx * 2, idx * 4, idx * 2];
        break;
      case LineType.DotDashed2:
        dash = [idx, idx * 4, idx * 8, idx * 4];
        break;
      default:
        break;
    }

    this.gctx.strokeStyle = this.getColor(gc_type);
    this.gctx.lineWidth = idx + 1;
    this.gctx.beginPath();
    if (dash !== null) {
      this.gctx.setLineDash(dash);
    }
    this.gctx.moveTo(x1, y1);
    this.gctx.lineTo(x2, y2);
    this.gctx.stroke();

    if (dash !== null) {
      this.gctx.setLineDash([]);
    }
  }

  arc(x, y, width, height, a1, a2, gc_type, idx, highlight) {
    this.gctx.strokeStyle = this.getColor(gc_type);
    this.gctx.lineWidth = idx + 1;
    this.gctx.beginPath();
    if (typeof this.gctx.ellipse === 'function') {
      this.gctx.ellipse(x + width / 2, y + height / 2, width / 2, height / 2,
          0, -a1 * Math.PI / 180, -(a1 + a2) * Math.PI / 180, true);
    } else {
      this.gctx.arc(x + width / 2, y + height / 2, width / 2, -a1 * Math.PI /
          180, -(a1 + a2) * Math.PI / 180, true);
    }
    this.gctx.stroke();
  }

  fill_arc(x, y, width, height, a1, a2, gc_type) {
    this.gctx.fillStyle = this.getColor(gc_type);
    this.gctx.beginPath();
    if (a2 !== 360) {
      this.gctx.moveTo(x + width / 2, y + height / 2);
    }
    if (typeof this.gctx.ellipse === 'function') {
      this.gctx.ellipse(x + width / 2, y + height / 2, width / 2, height / 2,
          0, -a1 * Math.PI / 180, -(a1 + a2) * Math.PI / 180, true);
    } else {
      this.gctx.arc(x + width / 2, y + height / 2, width / 2, -a1 * Math.PI /
          180, -(a1 + a2) * Math.PI / 180, true);
    }
    this.gctx.fill();
  }

  polyline(points, point_cnt, gc_type, idx, highlight) {
    if (this.nodraw) {
      return;
    }
    if (points.length === 0) {
      return;
    }

    this.gctx.lineWidth = idx + 1;
    this.gctx.strokeStyle = this.getColor(gc_type);
    this.gctx.beginPath();
    this.gctx.moveTo(points[0].x, points[0].y);
    for (let i = 1; i < point_cnt; i++) {
      this.gctx.lineTo(points[i].x, points[i].y);
    }

    this.gctx.stroke();
  }

  fill_polyline(points, point_cnt, gc_type, highlight) {
    if (this.nodraw) {
      return;
    }

    this.gctx.fillStyle = this.getColor(gc_type);

    this.gctx.beginPath();
    this.gctx.moveTo(points[0].x, points[0].y);
    for (let i = 1; i < point_cnt; i++) {
      this.gctx.lineTo(points[i].x, points[i].y);
    }
    this.gctx.fill();
  }

  text(x, y, text, gc_type, color, idx, highlight, line, font_idx, size, rot) {
    let sizes = [8, 10, 12, 14, 14, 16, 18, 18];
    let tsize = (idx <= 7) ? sizes[idx] : 3 * idx;

    this.gctx.font = tsize + "px Arial";
    this.gctx.lineWidth = 0.5;

    this.gctx.fillStyle = this.getColor(color);
    this.gctx.fillText(text, x, y);
  }

  getColor(gc_type) {
    //console.log("CustomColor cnt", this.customcolors_cnt);
    let rgb = GlowColor.rgb_color(gc_type, this.get_customcolors());

    return "rgb(" + Math.floor(255 * rgb.r) + "," + Math.floor(255 * rgb.g) +
        "," + Math.floor(255 * rgb.b) + ")";
  }

  getTextExtent(text, idx, type, gc_type) {
    let dim = new Rect();
    let sizes = [8, 10, 12, 14, 14, 16, 18, 18];
    let tsize = (idx <= 7) ? sizes[idx] : 3 * idx;

    this.gctx.font = tsize + "px Arial";
    dim.width = this.gctx.measureText(text).width;
    dim.height = tsize;
    return dim;
  }

  gradient_fill_rect(x, y, width, height, gc_type, f1, f2, gradient) {
    if (this.nodraw) {
      return;
    }

    this.setGradient(gradient, f1, f2, x, y, width, height);

    this.gctx.fillRect(x, y, width, height);
  }

  gradient_fill_rectrounded(x, y, width, height, amount, gc_type, f1, f2, gradient) {
    if (this.nodraw) {
      return;
    }

    this.setGradient(gradient, f1, f2, x, y, width, height);

    this.gctx.beginPath();
    this.gctx.moveTo(x + amount, y);
    this.gctx.arcTo(x + width, y, x + width, y + height, amount);
    this.gctx.arcTo(x + width, y + height, x, y + height, amount);
    this.gctx.arcTo(x, y + height, x, y, amount);
    this.gctx.arcTo(x, y, x + width, y, amount);
    this.gctx.closePath();
    this.gctx.fill();
  }

  gradient_fill_arc(x, y, width, height, a1, a2, gc_type, f1, f2, gradient) {
    if (this.nodraw) {
      return;
    }

    this.setGradient(gradient, f1, f2, x, y, width, height);

    this.gctx.beginPath();
    if (a2 !== 360) {
      this.gctx.moveTo(x + width / 2, y + height / 2);
    }
    if (typeof this.gctx.ellipse === 'function') {
      this.gctx.ellipse(x + width / 2, y + height / 2, width / 2, height / 2,
          0, -a1 * Math.PI / 180, -(a1 + a2) * Math.PI / 180, true);
    } else {
      this.gctx.arc(x + width / 2, y + height / 2, width / 2, -a1 * Math.PI /
          180, -(a1 + a2) * Math.PI / 180, true);
    }
    this.gctx.fill();
  }

  gradient_fill_polyline(points, point_cnt, gc_type, f1, f2, gradient) {
    if (this.nodraw) {
      return;
    }
    if (points.length === 0) {
      return;
    }

    let color = this.getColor(gc_type);

    let x_low = Number.MAX_VALUE;
    let x_high = -Number.MAX_VALUE;
    let y_low = Number.MAX_VALUE;
    let y_high = -Number.MAX_VALUE;

    // this.gctx.fillStyle = this.getColor( gc_type);

    this.gctx.beginPath();
    this.gctx.moveTo(points[0].x, points[0].y);
    for (let i = 1; i < point_cnt; i++) {
      this.gctx.lineTo(points[i].x, points[i].y);
      if (points[i].x > x_high) {
        x_high = points[i].x;
      }
      if (points[i].x < x_low) {
        x_low = points[i].x;
      }
      if (points[i].y > y_high) {
        y_high = points[i].y;
      }
      if (points[i].y < y_low) {
        y_low = points[i].y;
      }
    }

    this.setGradient(gradient, f1, f2, x_low, y_low, x_high - x_low, y_high -
        y_low);

    this.gctx.fill();
  }

  image(image, x, y, w, h) {
    this.gctx.drawImage(image, x, y, w, h);
  }

  gradient_rotate(rotate, gradient) {
    return gradient;
  }

  setGradient(gradient, f1, f2, x, y, w, h) {
    let gx0 = 0;
    let gy0 = 0;
    let gx1 = 0;
    let gy1 = 0;
    let gr = 0;

    switch (gradient) {
      case Gradient.HorizontalUp:
        gx0 = x;
        gy0 = y;
        gx1 = x;
        gy1 = y + h;
        break;
      case Gradient.HorizontalDown:
        gx0 = x;
        gy0 = y + h;
        gx1 = x;
        gy1 = y;
        break;
      case Gradient.HorizontalTube1:
        gx0 = x;
        gy0 = y + h;
        gx1 = x;
        gy1 = y;
        break;
      case Gradient.HorizontalTube2:
        gx0 = x;
        gy0 = y;
        gx1 = x;
        gy1 = y + h;
        break;
      case Gradient.VerticalLeft:
        gx0 = x;
        gy0 = y;
        gx1 = x + w;
        gy1 = y;
        break;
      case Gradient.VerticalRight:
        gx0 = x + w;
        gy0 = y;
        gx1 = x;
        gy1 = y;
        break;
      case Gradient.VerticalTube1:
        gx0 = x + w;
        gy0 = y;
        gx1 = x;
        gy1 = y;
        break;
      case Gradient.VerticalTube2:
        gx0 = x + w;
        gy0 = y;
        gx1 = x;
        gy1 = y;
        break;
      case Gradient.DiagonalUpperLeft:
        gx0 = x;
        gy0 = y;
        gx1 = x + w;
        gy1 = y + h;
        break;
      case Gradient.DiagonalLowerLeft:
        gx0 = x;
        gy0 = y + h;
        gx1 = x + w;
        gy1 = y;
        break;
      case Gradient.DiagonalUpperRight:
        gx0 = x + w;
        gy0 = y;
        gx1 = x;
        gy1 = y + h;
        break;
      case Gradient.DiagonalLowerRight:
        gx0 = x + w;
        gy0 = y + h;
        gx1 = x;
        gy1 = y;
        break;
      case Gradient.DiagonalUpTube:
        gx0 = x + w / 5;
        gy0 = y + h / 5;
        gx1 = x + w * 4 / 5;
        gy1 = y + h * 4 / 5;
        break;
      case Gradient.DiagonalDownTube:
        gx0 = x + w * 4 / 5;
        gy0 = y + h / 5;
        gx1 = x + w / 5;
        gy1 = y + h * 4 / 5;
        break;
      case Gradient.Globe:
        gx0 = x + 0.3 * w;
        gy0 = y + 0.3 * h;
        gx1 = x + w;
        gy1 = y + h;
        gr = Math.sqrt(w * w / 4 + h * h / 4);
        break;
      case Gradient.RadialCenter:
        gx0 = x + 0.5 * w;
        gy0 = y + 0.5 * h;
        gx1 = x + w;
        gy1 = y + h;
        gr = Math.sqrt(w * w / 4 + h * h / 4);
        break;
      case Gradient.RadialUpperLeft:
        gx0 = x;
        gy0 = y;
        gx1 = x + w;
        gy1 = y + h;
        gr = Math.sqrt(w * w + h * h);
        break;
      case Gradient.RadialLowerLeft:
        gx0 = x;
        gy0 = y + h;
        gx1 = x + w;
        gy1 = y;
        gr = Math.sqrt(w * w + h * h);
        break;
      case Gradient.RadialUpperRight:
        gx0 = x + w;
        gy0 = y;
        gx1 = x;
        gy1 = y + h;
        gr = Math.sqrt(w * w + h * h);
        break;
      case Gradient.RadialLowerRight:
        gx0 = x + w;
        gy0 = y + h;
        gx1 = x;
        gy1 = y;
        gr = Math.sqrt(w * w + h * h);
        break;
      default:
        break;
    }

    switch (gradient) {
      case Gradient.HorizontalUp:
      case Gradient.HorizontalDown:
      case Gradient.VerticalLeft:
      case Gradient.VerticalRight:
      case Gradient.DiagonalUpperLeft:
      case Gradient.DiagonalLowerLeft:
      case Gradient.DiagonalUpperRight:
      case Gradient.DiagonalLowerRight:
        let linear = this.gctx.createLinearGradient(gx0, gy0, gx1, gy1);
        linear.addColorStop(0, this.getColor(f2));
        linear.addColorStop(1, this.getColor(f1));
        this.gctx.fillStyle = linear;
        break;
      case Gradient.HorizontalTube1:
      case Gradient.VerticalTube1:
      case Gradient.DiagonalUpTube:
      case Gradient.DiagonalDownTube:
        let linear = this.gctx.createLinearGradient(gx0, gy0, gx1, gy1);
        linear.addColorStop(0, this.getColor(f1));
        linear.addColorStop(0.5, this.getColor(f2));
        linear.addColorStop(1, this.getColor(f1));
        this.gctx.fillStyle = linear;
        break;
      case Gradient.HorizontalTube2:
      case Gradient.VerticalTube2:
        let linear = this.gctx.createLinearGradient(gx0, gy0, gx1, gy1);
        linear.addColorStop(0, this.getColor(f1));
        linear.addColorStop(0.3, this.getColor(f2));
        linear.addColorStop(0.95, this.getColor(f1));
        this.gctx.fillStyle = linear;
        break;
      case Gradient.Globe:
      case Gradient.RadialCenter:
      case Gradient.RadialUpperLeft:
      case Gradient.RadialLowerLeft:
      case Gradient.RadialUpperRight:
      case Gradient.RadialLowerRight:
        let radial = this.gctx.createRadialGradient(gx0, gy0, 1, gx0, gy0, gr);
        radial.addColorStop(0, this.getColor(f2));
        radial.addColorStop(1, this.getColor(f1));
        this.gctx.fillStyle = radial;
        break;
      default:
        break;
    }
  }

  set_clip_rectangle(x1, y1, x2, y2) {
    if (this.clipCount > 0) {
      if (this.clipCount >= 10) {
        return 0;
      }
      if (x1 < this.clip_x1[this.clipCount - 1]) {
        x1 = this.clip_x1[this.clipCount - 1];
      }
      if (y1 < this.clip_y1[this.clipCount - 1]) {
        y1 = this.clip_y1[this.clipCount - 1];
      }
      if (x2 > this.clip_x2[this.clipCount - 1]) {
        x2 = this.clip_x2[this.clipCount - 1];
      }
      if (y2 > this.clip_y2[this.clipCount - 1]) {
        y2 = this.clip_y2[this.clipCount - 1];
      }
    }

    this.clip_x1[this.clipCount] = x1;
    this.clip_y1[this.clipCount] = y1;
    this.clip_x2[this.clipCount] = x2;
    this.clip_y2[this.clipCount] = y2;
    this.clipCount++;

    this.gctx.restore();
    this.gctx.save();
    this.gctx.beginPath();
    this.gctx.rect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
    this.gctx.clip();

    return 1;
  }

  reset_clip_rectangle() {
    if (this.clipCount === 0) {
      console.log("Clip mismatch");
      return;
    }
    this.clipCount--;
    if (this.clipCount > 0) {
      this.gctx.restore();
      this.gctx.save();
      this.gctx.beginPath();
      this.gctx.rect(this.clip_x1[this.clipCount - 1],
          this.clip_y1[this.clipCount - 1],
          (this.clip_x2[this.clipCount - 1] - this.clip_x1[this.clipCount - 1]),
          (this.clip_y2[this.clipCount - 1] - this.clip_y1[this.clipCount - 1]));
      this.gctx.clip();
    } else {
      this.gctx.restore();
      this.gctx.save();
    }
  }

  get_clip_rectangle() {
    if (this.clipCount === 0) {
      return null;
    }

    let rect = new Rect();
    rect.x = this.clip_x1[this.clipCount - 1];
    rect.y = this.clip_y1[this.clipCount - 1];
    rect.width = this.clip_x2[this.clipCount - 1];
    rect.height = this.clip_y2[this.clipCount - 1];
    return rect;
  }

  set_customcolors(cc) {
    this.customcolors[0] = cc;
    this.customcolors_cnt = 1;
  }

  push_customcolors(cc) {
    if (this.customcolors_cnt > this.CUSTOMCOLORS_STACK_SIZE) {
      console.log("** Max number custom colors exceede");
      return;
    }

    for (let i = this.customcolors_cnt; i > 0; i--) {
      this.customcolors[i] = this.customcolors[i - 1];
    }
    this.customcolors[0] = cc;
    this.customcolors_cnt++;
  }

  pop_customcolors() {
    if (this.customcolors_cnt <= 0) {
      // console.log( "** Customcolor stack disorder");
    }
    for (let i = 0; i < this.customcolors_cnt - 1; i++) {
      this.customcolors[i] = this.customcolors[i + 1];
    }
    this.customcolors_cnt--;
  }

  get_customcolors() {
    if (this.customcolors_cnt === 0) {
      return null;
    }
    return this.customcolors[0];
  }
}