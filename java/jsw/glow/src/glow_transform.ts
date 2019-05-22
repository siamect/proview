class Matrix {
  a11 = 1;
  a12 = 0;
  a13 = 0;
  a21 = 0;
  a22 = 1;
  a23 = 0;
  rotation = 0;

  set(m: Matrix) {
    this.a11 = m.a11;
    this.a12 = m.a12;
    this.a13 = m.a13;
    this.a21 = m.a21;
    this.a22 = m.a22;
    this.a23 = m.a23;
    this.rotation = m.rotation;
  }

  apply(p: Point) {
    return new Point(
        p.x * this.a11 + p.y * this.a12 + this.a13,
        p.x * this.a21 + p.y * this.a22 + this.a23
    );
  }

  static multiply(a, b) {
    if (b === null) {
      return a;
    }
    if (a === null) {
      return b;
    }
    let tmp = new Matrix();
    tmp.a11 = a.a11 * b.a11 + a.a12 * b.a21;
    tmp.a12 = a.a11 * b.a12 + a.a12 * b.a22;
    tmp.a13 = a.a11 * b.a13 + a.a12 * b.a23 + a.a13;
    tmp.a21 = a.a21 * b.a11 + a.a22 * b.a21;
    tmp.a22 = a.a21 * b.a12 + a.a22 * b.a22;
    tmp.a23 = a.a21 * b.a13 + a.a22 * b.a23 + a.a23;
    tmp.rotation = a.rotation + b.rotation;
    return tmp;
  }

  vertical_scale() {
    return Math.sqrt(this.a12 * this.a12 + this.a22 * this.a22);
  }
}

class GlowTransform extends Matrix {
  s: Matrix;
  stored = false;

  store() {
    this.s.set(this);
    this.stored = true;
  }

  revert() {
    this.set(this.s);
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

  scale(sx, sy, x0, y0) {
    this.a13 = this.a13 * sx + x0 * (1 - sx);
    this.a23 = this.a23 * sy + y0 * (1 - sy);
    this.a11 *= sx;
    this.a12 *= sx;
    this.a21 *= sy;
    this.a22 *= sy;
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
    let p = new Point();
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

  is_stored() {
    return this.stored;
  }
}