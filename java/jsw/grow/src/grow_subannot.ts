class GrowSubAnnot extends GlowAnnot {
  trf: GlowTransform;
  rect: GlowRect;
  n_name = null;
  ll_x: Number;
  ll_y: Number;
  ur_x: Number;
  ur_y: Number;
  adjustment: Adjustment;
  text: GlowText;

  constructor(ctx) {
    super(ctx);
    this.trf = new GlowTransform();
    this.rect = new GlowRect(ctx);
    this.text = new GlowText(ctx);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowSubAnnot:
          break;
        case GlowSave.GrowSubAnnot_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case GlowSave.GrowSubAnnot_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowSubAnnot_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowSubAnnot_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowSubAnnot_y_low:
          this.ll_y = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowSubAnnot_text:
          this.text = tokens[1];
          break;
        case GlowSave.GrowSubAnnot_rect:
          i = this.rect.open(lines, i + 1);
          break;
        case GlowSave.GrowSubAnnot_annot_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowSubAnnot_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case GlowSave.GrowSubAnnot_adjustment:
          this.adjustment = parseInt(tokens[1], 10);
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowSubAnnot");
          break;
      }
    }

    return i;
  }

  event_handler(event, fx, fy) {
    return 0;
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    let p = new Point();
    if (t) {
      p = Matrix.multiply(t, this.trf).apply(this.p);
      let p2 = this.trf.apply(this.p);
      p.x -= p2.x;
      p.y -= p2.y;
    }
    this.rect.draw(p, highlight, hot);
    this.text.draw(p, highlight, hot);
  }
}