class GlowText {
  ctx: GrowCtx;
  p: GlowPoint;
  text;
  draw_type;
  text_size;
  display_level;
  color_drawtype;

  constructor(ctx) {
    this.ctx = ctx;
    this.p = new GlowPoint();
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.Text:
          break;
        case GlowSave.Text_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Text_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case GlowSave.Text_color_drawtype:
          this.color_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.Text_text:
          this.text = lines[i].substring(4);
          break;
        case GlowSave.Text_p:
          i = this.p.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowText");
          break;
      }
    }

    return i;
  }

  draw(hightlight, hot) {
  }

  draw_shadow(border, shadow, hightlight, hot) {
  }
}