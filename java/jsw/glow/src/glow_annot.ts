class GlowAnnot {
  ctx: GrowCtx;
  number;
  p: GlowPoint;
  draw_type;
  text_size;
  annot_type;
  display_level;
  color_drawtype;
  font;
  protect;

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
        case GlowSave.Annot:
          break;
        case GlowSave.Annot_number:
          this.number = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_color_drawtype:
          this.color_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_display_level:
          this.display_level = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_p:
          i = this.p.open(lines, i + 1);
          break;
        case GlowSave.Annot_annot_type:
          this.annot_type = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_font:
          this.font = parseInt(tokens[1], 10);
          break;
        case GlowSave.Annot_protect:
          this.protect = parseInt(tokens[1], 10);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowAnnot");
          break;
      }
    }

    return i;
  }
}