class GlowDraw {
  ctx: GrowCtx;
  canvas: HTMLCanvasElement;
  gctx: CanvasRenderingContext2D;
  offset_top: number;
  clip = new Array<Rect>(10);
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

  arc(x, y, width, height, a1, a2, gc_type, idx, highlight = 0) {
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

  getTextExtent(text, idx, type, gc_type): Point {
    let tsize = (idx < 0 || idx > 7) ? 3 * idx : [8, 10, 12, 14, 14, 16, 18, 18][idx];

    this.gctx.font = tsize + "px Arial";
    return new Point(this.gctx.measureText(text).width, tsize);
  }

  gradient_fill_rect(x, y, width, height, gc_type, f1, f2, gradient) {
    this.setGradient(gradient, f1, f2, x, y, width, height);

    this.gctx.fillRect(x, y, width, height);
  }

  gradient_fill_rectrounded(x, y, width, height, amount, gc_type, f1, f2, gradient) {
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
    if (points.length === 0) {
      return;
    }

    let color = this.getColor(gc_type);

    // this.gctx.fillStyle = this.getColor( gc_type);

    this.gctx.beginPath();
    this.gctx.moveTo(points[0].x, points[0].y);
    let points2 = points.slice(0, point_cnt);
    let ll_x = points2.reduce((a, b) => Math.min(a, b.x), Number.MAX_VALUE);
    let ll_y = points2.reduce((a, b) => Math.min(a, b.y), Number.MAX_VALUE);
    let ur_x = points2.reduce((a, b) => Math.max(a, b.x), -Number.MAX_VALUE);
    let ur_y = points2.reduce((a, b) => Math.max(a, b.y), -Number.MAX_VALUE);
    points2.slice(1).forEach(e => this.gctx.lineTo(e.x, e.y));

    this.setGradient(gradient, f1, f2, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y);

    this.gctx.fill();
  }

  image(image, x, y, w, h) {
    this.gctx.drawImage(image, x, y, w, h);
  }

  gradient_rotate(rotate, gradient) {
    return gradient;
  }

  setGradient(gradient, f1, f2, x, y, w, h) {
    let gr = Math.sqrt(w * w + h * h);
    if (gradient === Gradient.Globe || gradient === Gradient.RadialCenter) {
      gr /= 4;
    }

    let grad;
    switch (gradient) {
      case Gradient.HorizontalUp:
      case Gradient.HorizontalDown:
      case Gradient.HorizontalTube1:
      case Gradient.HorizontalTube2:
        grad = this.gctx.createLinearGradient(x, y, x, y + h);
        break;
      case Gradient.VerticalRight:
      case Gradient.VerticalLeft:
      case Gradient.VerticalTube1:
      case Gradient.VerticalTube2:
        grad = this.gctx.createLinearGradient(x, y, x + w, y);
        break;
      case Gradient.DiagonalUpperLeft:
        grad = this.gctx.createLinearGradient(x, y, x + w, y + h);
        break;
      case Gradient.DiagonalLowerLeft:
        grad = this.gctx.createLinearGradient(x, y + h, x + w, y);
        break;
      case Gradient.DiagonalUpperRight:
        grad = this.gctx.createLinearGradient(x + w, y, x, y + h);
        break;
      case Gradient.DiagonalLowerRight:
        grad = this.gctx.createLinearGradient(x + w, y + h, x, y);
        break;
      case Gradient.DiagonalUpTube:
        grad = this.gctx.createLinearGradient(x + w / 5, y + h / 5, x + w * 4 / 5, y + h * 4 / 5);
        break;
      case Gradient.DiagonalDownTube:
        grad = this.gctx.createLinearGradient(x + w * 4 / 5, y + h / 5, x + w / 5, y + h * 4 / 5);
        break;
      case Gradient.Globe:
        grad = this.gctx.createRadialGradient(x + 0.3 * w, y + 0.3 * h, 1, x + w, y + h, gr);
        break;
      case Gradient.RadialCenter:
        grad = this.gctx.createRadialGradient(x + 0.5 * w, y + 0.5 * h, 1, x + w, y + h, gr);
        break;
      case Gradient.RadialUpperLeft:
        grad = this.gctx.createRadialGradient(x, y, 1, x + w, y + h, gr);
        break;
      case Gradient.RadialLowerLeft:
        grad = this.gctx.createRadialGradient(x, y + h, 1, x + w, y, gr);
        break;
      case Gradient.RadialUpperRight:
        grad = this.gctx.createRadialGradient(x + w, y, 1, x, y + h, gr);
        break;
      case Gradient.RadialLowerRight:
        grad = this.gctx.createRadialGradient(x + w, y + h, 1, x, y, gr);
        break;
      default:
        return;
    }

    switch (gradient) {
      case Gradient.HorizontalUp:
      case Gradient.VerticalLeft:
      case Gradient.DiagonalUpperLeft:
      case Gradient.DiagonalLowerLeft:
        grad.addColorStop(0, this.getColor(f2));
        grad.addColorStop(1, this.getColor(f1));
        break;
      case Gradient.HorizontalTube1:
      case Gradient.VerticalTube1:
      case Gradient.DiagonalUpTube:
      case Gradient.DiagonalDownTube:
        grad.addColorStop(0, this.getColor(f1));
        grad.addColorStop(0.5, this.getColor(f2));
        grad.addColorStop(1, this.getColor(f1));
        break;
      case Gradient.HorizontalTube2:
      case Gradient.VerticalTube2:
        grad.addColorStop(0, this.getColor(f1));
        grad.addColorStop(0.3, this.getColor(f2));
        grad.addColorStop(0.95, this.getColor(f1));
        break;
      default:
        grad.addColorStop(0, this.getColor(f1));
        grad.addColorStop(1, this.getColor(f2));
        break;
    }
    this.gctx.fillStyle = grad;
  }

  set_clip_rectangle(x1, y1, x2, y2) {
    if (this.clipCount > 0) {
      if (this.clipCount >= 10) {
        return 0;
      }
      x1 = Math.min(x1, this.clip[this.clipCount - 1].ll_x);
      y1 = Math.min(y1, this.clip[this.clipCount - 1].ll_y);
      x2 = Math.max(x2, this.clip[this.clipCount - 1].ur_x);
      y2 = Math.max(y2, this.clip[this.clipCount - 1].ur_y);
    }

    this.clip[this.clipCount] = new Rect(x1, y1, x2, y2);
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
      this.gctx.rect(this.clip[this.clipCount - 1].ll_x,
          this.clip[this.clipCount - 1].ll_y,
          this.clip[this.clipCount - 1].width(),
          this.clip[this.clipCount - 1].height());
      this.gctx.clip();
    } else {
      this.gctx.restore();
      this.gctx.save();
    }
  }

  get_clip_rectangle() {
    return this.clip[this.clipCount - 1] || null;
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