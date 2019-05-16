class GlowPoint extends Point {
  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.Point:
          break;
        case GlowSave.Point_x:
          this.x = parseFloat(tokens[1]);
          break;
        case GlowSave.Point_y:
          this.y = parseFloat(tokens[1]);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowPoint");
          break;
      }
    }

    return i;
  }

  posit(x, y) {
    this.x = x;
    this.y = y;
  }
}