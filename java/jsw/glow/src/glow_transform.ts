class GlowTransform {
  a11 = 1;
  a12 = 0;
  a13 = 0;
  a21 = 0;
  a22 = 1;
  a23 = 0;
  rotation = 0;
  s: GlowTransform;
  stored = false;

  set(o: GlowTransform) {
    this.a11 = o.a11;
    this.a12 = o.a12;
    this.a13 = o.a13;
    this.a21 = o.a21;
    this.a22 = o.a22;
    this.a23 = o.a23;
    this.rotation = o.rotation;
  }

  store() {
    this.s.set(this);
    this.stored = true;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.Transform:
          break;
        case GlowSave.Transform_a11:
          this.a11 = parseFloat(tokens[1]);
          break;
        case GlowSave.Transform_a12:
          this.a12 = parseFloat(tokens[1]);
          break;
        case GlowSave.Transform_a13:
          this.a13 = parseFloat(tokens[1]);
          break;
        case GlowSave.Transform_a21:
          this.a21 = parseFloat(tokens[1]);
          break;
        case GlowSave.Transform_a22:
          this.a22 = parseFloat(tokens[1]);
          break;
        case GlowSave.Transform_a23:
          this.a23 = parseFloat(tokens[1]);
          break;
        case GlowSave.Transform_rotation:
          this.rotation = parseFloat(tokens[1]);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowTransform");
          break;
      }
    }

    return i;
  }

  rot() {
    if (arguments.length === 1 && arguments[0] !== null) {
      let t = arguments[0];
      return t.rotation + this.rotation;
    } else {
      return this.rotation;
    }
  }

  x() {
    if (arguments.length === 3 && arguments[0] !== null) {
      let t = arguments[0];
      let x1 = arguments[1];
      let y1 = arguments[2];

      let tmp = GlowTransform.multiply(t, this);
      return x1 * tmp.a11 + y1 * tmp.a12 + tmp.a13;
    } else {
      let x1 = arguments[0];
      let y1 = arguments[1];
      return x1 * this.a11 + y1 * this.a12 + this.a13;
    }
  }

  y() {
    if (arguments.length === 3 && arguments[0] !== null) {
      let t = arguments[0];
      let x1 = arguments[1];
      let y1 = arguments[2];

      let tmp = GlowTransform.multiply(t, this);
      return x1 * tmp.a21 + y1 * tmp.a22 + tmp.a23;
    } else {
      let x1 = arguments[0];
      let y1 = arguments[1];
      return x1 * this.a21 + y1 * this.a22 + this.a23;
    }
  }

  static multiply(a, b) {
    let tmp = new GlowTransform();
    tmp.a11 = a.a11 * b.a11 + a.a12 * b.a21;
    tmp.a12 = a.a11 * b.a12 + a.a12 * b.a22;
    tmp.a13 = a.a11 * b.a13 + a.a12 * b.a23 + a.a13;
    tmp.a21 = a.a21 * b.a11 + a.a22 * b.a21;
    tmp.a22 = a.a21 * b.a12 + a.a22 * b.a22;
    tmp.a23 = a.a21 * b.a13 + a.a22 * b.a23 + a.a23;
    tmp.rotation = a.rotation + b.rotation;
    return tmp;
  }

  set_from_stored(t) {
    this.set(GlowTransform.multiply(t, this.s));
    this.a11 = t.a11 * this.s.a11 + t.a12 * this.s.a21;
    this.a12 = t.a11 * this.s.a12 + t.a12 * this.s.a22;
    this.a13 = t.a11 * this.s.a13 + t.a12 * this.s.a23 + t.a13;
    this.a21 = t.a21 * this.s.a11 + t.a22 * this.s.a21;
    this.a22 = t.a21 * this.s.a12 + t.a22 * this.s.a22;
    this.a23 = t.a21 * this.s.a13 + t.a22 * this.s.a23 + t.a23;
    this.rotation = this.s.rotation + t.rotation;
  }

  scale(sx, sy, x0, y0) {
    this.a13 = this.a13 * sx + x0 * (1 - sx);
    this.a23 = this.a23 * sy + y0 * (1 - sy);
    this.a11 *= sx;
    this.a12 *= sx;
    this.a21 *= sy;
    this.a22 *= sy;
  }

  scale_from_stored(sx, sy, x0, y0) {
    this.a13 = this.s.a13 * sx + x0 * (1 - sx);
    this.a23 = this.s.a23 * sy + y0 * (1 - sy);
    this.a11 = this.s.a11 * sx;
    this.a12 = this.s.a12 * sx;
    this.a21 = this.s.a21 * sy;
    this.a22 = this.s.a22 * sy;
  }

  rotate(angle, x0, y0) {
    let sin_a;
    let cos_a;
    let tmp = new GlowTransform();
    tmp.a11 = this.a11;
    tmp.a12 = this.a12;
    tmp.a13 = this.a13;
    tmp.a21 = this.a21;
    tmp.a22 = this.a22;
    tmp.a23 = this.a23;
    tmp.rotation = this.rotation;

    if (-90.01 < angle && angle < -89.99) {
      sin_a = -1.0;
      cos_a = 0.0;
    } else {
      sin_a = Math.sin(angle / 180 * 3.14159);
      cos_a = Math.cos(angle / 180 * 3.14159);
    }

    this.a11 = tmp.a11 * cos_a - tmp.a21 * sin_a;
    this.a12 = tmp.a12 * cos_a - tmp.a22 * sin_a;
    this.a13 =
        tmp.a13 * cos_a - tmp.a23 * sin_a + x0 * (1 - cos_a) + y0 * sin_a;
    this.a21 = tmp.a11 * sin_a + tmp.a21 * cos_a;
    this.a22 = tmp.a21 * sin_a + tmp.a22 * cos_a;
    this.a23 =
        tmp.a13 * sin_a + tmp.a23 * cos_a + y0 * (1 - cos_a) - x0 * sin_a;
    this.rotation += angle;
  }

  rotate_from_stored(angle, x0, y0) {
    let sin_a;
    let cos_a;
    if (-90.01 < this.s.rotation + angle && this.s.rotation + angle < -89.99) {
      sin_a = -1.0;
      cos_a = 0.0;
    } else {
      sin_a = Math.sin((this.s.rotation + angle) / 180 * 3.14159);
      cos_a = Math.cos((this.s.rotation + angle) / 180 * 3.14159);
    }

    this.a11 = this.s.a11 * cos_a - this.s.a21 * sin_a;
    this.a12 = this.s.a12 * cos_a - this.s.a22 * sin_a;
    this.a13 =
        this.s.a13 * cos_a - this.s.a23 * sin_a + x0 * (1 - cos_a) + y0 * sin_a;
    this.a21 = this.s.a11 * sin_a + this.s.a21 * cos_a;
    this.a22 = this.s.a21 * sin_a + this.s.a22 * cos_a;
    this.a23 =
        this.s.a13 * sin_a + this.s.a23 * cos_a + y0 * (1 - cos_a) - x0 * sin_a;
    this.rotation = this.s.rotation + angle;
  }

  move(x0, y0) {
    this.a13 += x0;
    this.a23 += y0;
  }

  move_from_stored(x0, y0) {
    this.a13 = this.s.a13 + x0;
    this.a23 = this.s.a23 + y0;
  }

  posit(x0, y0) {
    this.a13 = x0;
    this.a23 = y0;
  }

  reverse(x, y) {
    let p = new GlowPoint();
    if (this.a11 === 0 || (this.a12 * this.a21 - this.a11 * this.a22) === 0) {
      if (this.a11 === 0 && this.a22 === 0 && this.a12 !== 0 &&
          this.a21 !== 0) {
        p.y = (x - this.a13) / this.a12;
        p.x = (y - this.a23) / this.a21;
        return p;
      } else {
        p.x = 0;
        p.y = 0;
        return p;
      }
    }
    p.y = (this.a11 * (this.a23 - y) - this.a21 * (this.a13 - x)) /
        (this.a12 * this.a21 - this.a11 * this.a22);
    p.x = (x - this.a12 * p.y - this.a13) / this.a11;
    return p;
  }

  vertical_scale(t) {
    if (t === null) {
      return Math.sqrt(this.a12 * this.a12 + this.a22 * this.a22);
    }

    let tmp = GlowTransform.multiply(t, this);

    return Math.sqrt(tmp.a12 * tmp.a12 + tmp.a22 * tmp.a22);
  }

  is_stored() {
    return this.stored;
  }
}