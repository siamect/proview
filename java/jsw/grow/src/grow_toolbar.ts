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

      if (this.ctx.debug) {
        console.log("GrowToolbar : " + lines[i]);
      }

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
    let rp;
    let sts;
    let idx;

    if (this.visible === 0 || this.dimmed !== 0) {
      return 0;
    }

    switch (event.event) {
      case Event.CursorMotion:
        return 0;
      default:
    }

    rp = this.trf.reverse(fx, fy);

    switch (event.event) {
      case Event.MB1Down:
        sts = this.nc_event_handler(event, rp.x, rp.y);
        if (sts !== 0) {
          idx = sts - 1;
          this.nc.a.get(idx).setColorInverse(1);
        }
        break;
      case Event.MB1Up:
        sts = this.nc_event_handler(event, rp.x, rp.y);
        if (sts !== 0) {
          idx = sts - 1;
          this.nc.a.get(idx).setColorInverse(0);
        }
        break;
      case Event.MB1Click:
        sts = this.nc_event_handler(event, rp.x, rp.y);
        console.log("MB1 Click toolbar", sts);
        if (sts !== 0) {
          idx = sts - 1;
          let lsts = this.get_mask_index(idx);
          if (lsts === null) {
            break;
          }

          let category = lsts[0];
          let mask_idx = lsts[1];

          console.log("MB1 Click category: " + category + " idx " + mask_idx +
              " sts" + idx);

          let csts = this.ctx.send_toolbar_callback(this, category, mask_idx,
              Event.MB1Click, fx, fy);
        }
        break;
      default:
        sts = this.nc.event_handler(event, rp.x, rp.y);
        if (sts !== 0) {
          return sts;
        }
    }
    return 0;
  }

  nc_event_handler(event, x, y) {
    let i;
    let sts;

    for (i = 0; i < this.nc.a.size(); i++) {
      sts = this.nc.a.get(i).event_handler(event, x, y);
      if (sts !== 0) {
        return i + 1;
      }
    }
    return 0;
  }

  configure(tools1, tools2, tools1_cnt, tools2_cnt, show_mask1, show_mask2,
            insensitive_mask1, insensitive_mask2) {
    let x1, y1;
    let ll_x, ll_y, ur_x, ur_y;

    let nc1;
    let subg_name;
    let mask;
    let sts;

    console.log("Configure show_mask " + show_mask1 + " " + show_mask2);
    x1 = 0;
    y1 = 0;

    // Clear nc
    this.nc.a.clear();

    this.tools1_mask = 0;
    mask = 1;
    for (let i = 0; i < tools1_cnt; i++) {
      if ((mask & show_mask1) !== 0) {
        if (tools1[i] === "") {
          mask = mask << 1;
          continue;
        }

        subg_name = tools1[i];

        nc1 = this.ctx.get_nodeclass_from_name(subg_name);
        if (nc1 === null) {
          let fname = subg_name + ".pwsg";
          sts = this.ctx.loadSubgraph(fname);
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
          x1 += g.ur_x - g.ll_x + GrowToolbar.TOOLBAR_SPACING;
          this.tools1_mask |= mask;
        }
      }
      mask = mask << 1;
    }
    if (x1 !== 0) {
      x1 += GrowToolbar.TOOLBAR_SPACING * 2;
    }

    this.tools2_mask = 0;
    mask = 1;
    for (let i = 0; i < tools2_cnt; i++) {
      if ((mask & show_mask2) !== 0) {
        if (tools2[i] === "") {
          mask = mask << 1;
          continue;
        }

        subg_name = tools2[i];

        nc1 = this.ctx.get_nodeclass_from_name(subg_name);
        if (nc1 === null) {
          let fname = subg_name + ".pwsg";
          sts = this.ctx.loadSubgraph(fname);
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
          x1 += g.ur_x - g.ll_x + GrowToolbar.TOOLBAR_SPACING;
          this.tools2_mask |= mask;
        }
      }
      mask = mask << 1;
    }

    this.get_node_borders();
    this.scale();

    console.log("Toolbar configure tools_mask " + this.tools1_mask + " " +
        this.tools2_mask);
  }

  get_mask_index(idx) {
    // Calculate category and index
    let mask;
    let category;
    let mask_idx;
    let cnt = 0;
    let i;
    for (i = 0; i < 64; i++) {
      if (i < 32) {
        mask = 1 << i;
      } else {
        mask = 1 << (i - 32);
      }
      if ((i < 32 && (this.tools1_mask & mask) !== 0) ||
          (i >= 32 && (this.tools2_mask & mask) !== 0)) {
        cnt++;
      }
      if (cnt === idx + 1) {
        mask_idx = i % 32;
        category = i / 32 + 1;
        let ret = new Array(2);
        ret[0] = category;
        ret[1] = mask_idx;
        return ret;
      }
    }
    return null;
  }

  scale() {
    let scale;

    if (this.ctx.mw.window_width === 0) {
      return;
    }

    console.log("Toolbar.scale offset_x " + this.ctx.mw.offset_x + " wwidth " +
        this.ctx.mw.window_width);
    if (this.x_right * this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x >
        this.ctx.mw.window_width) {
      scale = (this.ctx.mw.window_width -
          (this.x_left * this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x) - 10) /
          ((this.x_right - this.x_left) * this.ctx.mw.zoom_factor_x);

      this.trf.scale(scale, 1, this.x_left, this.y_low);
      this.get_node_borders();
    }
  }
}