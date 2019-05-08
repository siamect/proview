class GlowNodeGroup extends GlowNodeClass {
  constructor(ctx) {
    super(ctx);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowNodeClass : " + lines[i]);
      }

      switch (key) {
        case GlowSave.NodeGroup:
          break;
        case GlowSave.NodeGroup_nodeclass_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowNodeGroup");
          break;
      }
    }
    return i;
  }
}