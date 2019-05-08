class GrowSlider extends GrowNode {
  direction;
  max_value;
  min_value;
  max_pos;
  min_pos;

  constructor(ctx) {
    super(ctx);
  }

  open(lines, row) {
    let i;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowGroup : " + lines[i]);
      }

      switch (key) {
        case GlowSave.GrowSlider:
          break;
        case GlowSave.GrowSlider_direction:
          this.direction = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowSlider_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowSlider_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowSlider_max_pos:
          this.max_pos = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowSlider_min_pos:
          this.min_pos = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowSlider_grownode_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowSlider");
          break;
      }
    }

    return i;
  }

  get_origo(direction) {
    let t = new GlowTransform();
    let g = new Rect();
    g.x = 10.0e37;
    g.y = 10.0e37;
    g.width = 10.0e-37;
    g.height = 10.0e-37;

    let pos;

    // Get borders in the nodeclass coordinates
    this.nc.get_borders(t, g);
    switch (this.direction) {
      case Direction.Down:
      case Direction.Up:
        if (Math.abs(g.height) < Number.MIN_VALUE) {
          pos = this.y_low;
        } else {
          pos = (this.nc.y0 - g.y) * (this.y_high - this.y_low) / g.height;
        }
        break;
      case Direction.Left:
      case Direction.Right:
        if (Math.abs(g.height) < Number.MIN_VALUE) {
          pos = this.y_low;
        } else {
          pos = (this.nc.y0 - g.y) * (this.x_right - this.x_left) / g.height;
        }
        break;
      default:
        pos = 0;
    }
    return pos;
  }

  get_info() {
    let info = new GlowSliderInfo();

    info.direction = this.direction;
    info.max_value = this.max_value;
    info.min_value = this.min_value;
    info.max_position = this.max_pos;
    info.min_position = this.min_pos;

    return info;
  }

  set_info(info) {
    this.direction = info.direction;
    this.max_value = info.max_value;
    this.min_value = info.min_value;
    this.max_pos = info.max_position;
    this.min_pos = info.min_position;
  }
}