class GrowToolbar extends GrowNode {
  static TOOLBAR_SPACING = 0.2;

  tools1_mask;
  tools2_mask;

  constructor(ctx) {
    super(ctx);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowToolbar:
          break;
        case GlowSave.GrowToolbar_grownode_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GrowToolbar");
          break;
      }
    }

    return i;
  }

  event_handler(event, fx, fy) {
    if (this.visible === 0 || this.dimmed !== 0) {
      return 0;
    }

    if (event.event === Event.CursorMotion) {
      return 0;
    }

    let rp = this.trf.reverse(fx, fy);

    switch (event.event) {
      case Event.MB1Down:
        let sts = this.nc_event_handler(event, rp.x, rp.y);
        if (sts !== 0) {
          this.nc.a.get(sts - 1).setColorInverse(1);
        }
        break;
      case Event.MB1Up:
        let sts = this.nc_event_handler(event, rp.x, rp.y);
        if (sts !== 0) {
          this.nc.a.get(sts - 1).setColorInverse(0);
        }
        break;
      case Event.MB1Click:
        let sts = this.nc_event_handler(event, rp.x, rp.y);
        if (sts !== 0) {
          let lsts = this.get_mask_index(sts - 1);
          if (lsts === null) {
            break;
          }

          this.ctx.send_toolbar_callback(this, lsts[0], lsts[1],
              Event.MB1Click, fx, fy);
        }
        break;
      default:
        let sts = this.nc.event_handler(event, rp.x, rp.y);
        if (sts !== 0) {
          return sts;
        }
    }
    return 0;
  }

  nc_event_handler(event, x, y) {
    for (let i = 0; i < this.nc.a.size(); i++) {
      let sts = this.nc.a.get(i).event_handler(event, x, y);
      if (sts !== 0) {
        return i + 1;
      }
    }
    return 0;
  }

  configure(tools1, tools2, tools1_cnt, tools2_cnt, show_mask1, show_mask2,
            insensitive_mask1, insensitive_mask2) {
    console.log("Configure show_mask " + show_mask1 + " " + show_mask2);
    let x1 = 0;
    let y1 = 0;

    // Clear nc
    this.nc.a.clear();

    this.tools1_mask = 0;
    let mask = 1;
    for (let i = 0; i < tools1_cnt; i++) {
      if ((mask & show_mask1) !== 0) {
        if (tools1[i] === "") {
          mask <<= 1;
          continue;
        }

        let subg_name = tools1[i];

        let nc1 = this.ctx.get_nodeclass_from_name(subg_name);
        if (nc1 === null) {
          let sts = this.ctx.loadSubgraph(subg_name + ".pwsg");
          if ((sts & 1) !== 0) {
            nc1 = this.ctx.get_nodeclass_from_name(subg_name);
          }
        }
        if (nc1 !== null) {
          let n1;
          n1 = new GrowNode(this.ctx);
          n1.n_name = subg_name;
          console.log("Button " + subg_name + " " + x1);
          n1.nc = nc1;
          n1.move_to(x1, y1);
          // n1.disable_callback();
          this.nc.a.add(n1);

          if ((mask & insensitive_mask1) !== 0) {
            n1.setVisibility(Vis.Dimmed);
          }

          let g = n1.measure();
          x1 += g.width() + GrowToolbar.TOOLBAR_SPACING;
          this.tools1_mask |= mask;
        }
      }
      mask <<= 1;
    }
    if (x1 !== 0) {
      x1 += GrowToolbar.TOOLBAR_SPACING * 2;
    }

    this.tools2_mask = 0;
    mask = 1;
    for (let i = 0; i < tools2_cnt; i++) {
      if ((mask & show_mask2) !== 0) {
        if (tools2[i] === "") {
          mask <<= 1;
          continue;
        }

        let subg_name = tools2[i];

        let nc1 = this.ctx.get_nodeclass_from_name(subg_name);
        if (nc1 === null) {
          let sts = this.ctx.loadSubgraph(subg_name + ".pwsg");
          if ((sts & 1) !== 0) {
            nc1 = this.ctx.get_nodeclass_from_name(subg_name);
          }
        }
        if (nc1 !== null) {
          let n1;
          n1 = new GrowNode(this.ctx);
          n1.n_name = subg_name;
          n1.nc = nc1;
          console.log("Button " + subg_name + " " + x1);
          n1.move_to(x1, y1);
          // n1.disable_callback();
          this.nc.a.add(n1);

          if ((mask & insensitive_mask2) !== 0) {
            n1.setVisibility(Vis.Dimmed);
          }

          let g = n1.measure();
          x1 += g.width() + GrowToolbar.TOOLBAR_SPACING;
          this.tools2_mask |= mask;
        }
      }
      mask <<= 1;
    }

    this.get_node_borders();
    this.scale();

    console.log("Toolbar configure tools_mask " + this.tools1_mask + " " +
        this.tools2_mask);
  }

  get_mask_index(idx) {
    // Calculate category and index
    let cnt = 0;
    for (let i = 0; i < 64; i++) {
      let mask = (i < 32) ? (1 << i) : (1 << (i - 32));
      if ((i < 32 && (this.tools1_mask & mask) !== 0) ||
          (i >= 32 && (this.tools2_mask & mask) !== 0)) {
        cnt++;
      }
      if (cnt === idx + 1) {
        let mask_idx = i % 32;
        let category = i / 32 + 1;
        let ret = new Array(2);
        ret[0] = category;
        ret[1] = mask_idx;
        return ret;
      }
    }
    return null;
  }

  scale() {
    if (this.ctx.mw.window_width === 0) {
      return;
    }

    if (this.ur_x * this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x >
        this.ctx.mw.window_width) {
      let scale = (this.ctx.mw.window_width -
          (this.ll_x * this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x) - 10) /
          (this.width() * this.ctx.mw.zoom_factor_x);

      this.trf.scale(scale, 1, this.ll_x, this.ll_y);
      this.get_node_borders();
    }
  }
}