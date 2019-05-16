class GrowGroup extends GrowNode {
  constructor(ctx) {
    super(ctx);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowGroup:
          break;
        case GlowSave.GrowGroup_grownode_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowGroup_nc:
          this.nc = new GlowNodeGroup(this.ctx);
          i = this.nc.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowGroup");
          break;
      }
    }

    return i;
  }

  get_object_group(object) {
    for (let i = 0; i < this.nc.a.size(); i++) {
      if (this.nc.a.get(i) instanceof GrowGroup) {
        let group = this.nc.a.get(i).get_object_group(object);
        if (group !== null) {
          return group;
        }
      } else if (this.nc.a.get(i) === object) {
        return this;
      }
    }
    return null;
  }

  get_object_list() {
    return this.nc.a;
  }
}