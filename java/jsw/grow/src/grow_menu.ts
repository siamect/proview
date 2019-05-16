class GrowMenu extends GrowRect {
  info;			//!< Button text and types.
  text_size;		//!< Button text size.
  text_drawtype;		//!< Drawtype  for button text.
  text_color;		//!< Color for button text.
  text_color_disabled;	//!< Color for disabled button text.
  item_cnt;		//!< Number of menu items.
  item_height;		//!< Menu item height in pixel.
  current_item = -1;     	//!< Hot item.
  current_idx = -1;        //!< Hot index.
  new_item = -1;		//!< New current item.
  old_item = -1;		//!< Current item in previous event.
  parent_menu = null;	//!< Parent menu.
  min_width;		//!< Minimum width of menu.
  input_focus;		//!< This menu has input focus.
  font;			//!< Text font.
  hot;

  constructor(ctx, n_name, menu_info, x, y, min_w, draw_type, line_width, fill,
              border, fill_drawtype, text_size, text_drawtype, text_color,
              text_color_disabled, text_font) {
    super(ctx);
    this.ll.x = x;
    this.ll.y = y;
    this.ur.x = x + 10;
    this.ur.y = y + 10;
    this.draw_type = draw_type;
    this.line_width = line_width;
    this.fill = fill;
    this.border = border;
    this.shadow = 0;
    this.fill_drawtype = fill_drawtype;
    this.n_name = n_name;

    this.info = menu_info;
    this.text_size = text_size;
    this.text_drawtype = text_drawtype;
    this.text_color = text_color;
    this.text_color_disabled = text_color_disabled;
    this.font = text_font;

    this.draw();
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0) {
      return;
    }
    let text_idx = Math.floor(this.trf.vertical_scale(t) *
        this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        (this.text_size + 4) - 4);
    let tsize = this.trf.vertical_scale(t) * this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (8 + 2 * this.text_size);
    text_idx = Math.min(text_idx, DRAW_TYPE_SIZE - 1);
    text_idx = Math.max(0, text_idx);

    let idx = Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        this.line_width - 1);
    idx += hot;
    idx = clamp(idx, 0, DRAW_TYPE_SIZE-1);

    let z_width, z_descent;
    let z_height = 0;
    let max_z_width = 0;
    let tot_z_height = 0;
    let pulldown_found = 0;
    this.item_cnt = 0;
    this.info.item.forEach(function (e) {
      if (e.occupied) {
        let p = this.ctx.gdraw.getTextExtent(e.text, this.text_drawtype,
            Math.max(0, text_idx), this.font);
        z_width = p.x;
        z_height = p.y;
        z_descent = z_height / 4;
        max_z_width = Math.max(max_z_width, z_width);
        tot_z_height += Math.floor(1.6 * z_height);
        if (e.type === MenuItem.PulldownMenu) {
          pulldown_found = 1;
        }
        this.item_cnt++;
      }
    });
    if (this.item_cnt === 0) {
      return;
    }

    let arrow_size = z_height * 4 / 5;
    let ll_x =
        Math.floor(this.ll.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let ll_y =
        Math.floor(this.ll.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let ur_x = ll_x + max_z_width + 15 + Math.floor(pulldown_found * arrow_size);
    if (ur_x - ll_x < Math.floor(this.min_width * this.ctx.mw.zoom_factor_x)) {
      ur_x = ll_x + Math.floor(this.min_width * this.ctx.mw.zoom_factor_x);
    }
    let ur_y = ll_y + Math.floor(tot_z_height);

    if (ur_y > this.ctx.getHeight() + this.ctx.mw.subwindow_y) {
      // Outside window border
      ur_y = this.ctx.getHeight() + this.ctx.mw.subwindow_y;
      ll_y = ur_y - Math.floor(tot_z_height);
      this.ll.posit(this.ll.x, (ll_y + this.ctx.mw.offset_y) /
          this.ctx.mw.zoom_factor_x);
      this.ur.posit(this.ur.x, (ur_y + this.ctx.mw.offset_y) /
          this.ctx.mw.zoom_factor_y); // Might not always be correct?
      ll_x = Math.floor(this.ll.x * this.ctx.mw.zoom_factor_x) -
          this.ctx.mw.offset_x;
      ll_y = Math.floor(this.ll.y * this.ctx.mw.zoom_factor_y) -
          this.ctx.mw.offset_y;
      ur_x = ll_x + max_z_width + 15 + Math.floor(pulldown_found * arrow_size);
      ur_y = ll_y + Math.floor(tot_z_height);
    } else {
      this.ur.posit((ur_x + this.ctx.mw.offset_x) /
          this.ctx.mw.zoom_factor_x, (ur_y + this.ctx.mw.offset_y) /
          this.ctx.mw.zoom_factor_y);
    }
    ll_y =
        Math.floor(this.ll.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    this.get_node_borders();

    if (this.fill !== 0) {
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
          this.fill_drawtype, true, 0);
    }
    this.item_height = tot_z_height / this.item_cnt;

    let item_idx = 0;
    this.info.item.forEach(function (e, i) {
      if (e.occupied) {
        let y_text = ll_y + Math.floor(this.item_height * item_idx + z_height + 1);
        if (i === this.current_item) {
          let drawtype;
          if (this.fill_drawtype % 10 < 7) {
            drawtype = GlowColor.shift_drawtype(this.fill_drawtype, 2, null);
          } else {
            drawtype = GlowColor.shift_drawtype(this.fill_drawtype, -2, null);
          }
          this.ctx.gdraw.rect(ll_x, Math.floor(ll_y + item_idx * this.item_height), ur_x - ll_x,
              Math.floor(this.item_height), drawtype, true, 0);
        }
        let x_text = ll_x + 3;
        if (e.type === MenuItem.ButtonDisabled) {
          this.ctx.gdraw.text(x_text, y_text, e.text, this.text_drawtype,
              this.text_color_disabled, text_idx, highlight, this.font, tsize, 0);
        } else {
          this.ctx.gdraw.text(x_text, y_text, e.text, this.text_drawtype,
              this.text_color, text_idx, highlight, this.font, tsize, 0);
        }
        if (e.type === MenuItem.PulldownMenu) {
          // Draw arrow
          let p = [
            new Point(ur_x - arrow_size - 2, Math.floor(ll_y + item_idx *
                this.item_height + this.item_height / 2 - arrow_size / 2)),
            new Point(ur_x - 2, Math.floor(ll_y + item_idx *
                this.item_height + this.item_height / 2)),
            new Point(ur_x - arrow_size - 2, Math.floor(ll_y + item_idx *
                this.item_height + this.item_height / 2 + arrow_size / 2)),
            new Point(ur_x - arrow_size - 2, Math.floor(ll_y + item_idx *
                this.item_height + this.item_height / 2 - arrow_size / 2)),
          ];
          this.ctx.gdraw.polyline(p, 4, DrawType.MediumGray, true, 0);
        }
        item_idx++;
      }
    });
    if (this.border !== 0) {
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.draw_type,
          false, idx);
    }
  }

  local_event_handler(event, x, y) {
    let ll_x = Math.min(this.ll.x, this.ur.x);
    let ur_x = Math.max(this.ll.x, this.ur.x);
    let ll_y = Math.min(this.ll.y, this.ur.y);
    let ur_y = Math.max(this.ll.y, this.ur.y);

    if (ll_x <= x && x <= ur_x && ll_y <= y && y <= ur_y) {
      let vscale = this.trf.vertical_scale(null);

      let item = Math.floor((y - this.ll.y) /
          (this.item_height / vscale / this.ctx.mw.zoom_factor_y));
      item = clamp(item, 0, this.item_cnt-1);
      this.new_item = Number(item !== this.current_idx);
      this.old_item = this.current_item;
      if (this.new_item) {
        this.current_idx = item;
        let item_idx = 0;
        for (let i = 0; i < 32; i++) {
          if (!this.info.item[i].occupied) {
            continue;
          }
          if (item_idx === item) {
            this.current_item = i;
            break;
          }
          item_idx++;
        }
      }
      return 1;
    } else {
      this.new_item = 0;
      return 0;
    }
  }

  event_handler(event, fx, fy) {
    let csts;

    // Convert coordinates to local coordinates
    let rp = this.trf.reverse(fx, fy);

    let sts = 0;
    switch (event.event) {
      case Event.CursorMotion:
        let redraw = 0;
        sts = this.local_event_handler(event, rp.x, rp.y);
        if (sts !== 0) {
          this.ctx.hot_found = 1;
          redraw = 1;
        }
        if (sts !== 0 && this.hot === 0) {
          this.hot = 1;
          redraw = 1;
        }
        if (sts === 0 && this.hot !== 0) {
          if (this.ctx.hot_found === 0) {
            this.hot = 0;
          }
          redraw = 1;
        }
        if (redraw !== 0) {
          this.ctx.draw();
        }
        if (this.old_item !== this.current_item && this.old_item !== -1) {
          if (this.info.item[this.old_item].type ===
              MenuItem.PulldownMenu) {
            // Delete all child menu objects.
            // TODO ctx->delete_menu_child( this);
          }
        }
        if (this.hot !== 0 && this.new_item !== 0) {
          console.log("GrowMenu new item,send MenuCreate");
          if (this.info.item[this.current_item].type ===
              MenuItem.PulldownMenu) {
            this.ctx.send_menu_callback(this, this.current_item,
                Event.MenuCreate, this.ur.x, this.ll.y + this.item_height /
                this.ctx.mw.zoom_factor_y * this.current_item);
          }
        }
        break;
      case Event.MB1Click:
        sts = this.local_event_handler(event, rp.x, rp.y);
        if (sts !== 0 && this.current_item !== -1) {
          if (this.info.item[this.current_item].type === MenuItem.Button) {
            csts = this.ctx.send_menu_callback(this, this.current_item,
                Event.MenuActivated, fx, fy);
            if (csts === GLOW__TERMINATED) {
              return csts;
            }

            // Send delete callback for the root menu of this node
            let e;
            for (e = this; e.parent_menu !== null; e = e.parent_menu) {
            }
            this.ctx.send_menu_callback(e, 0, Event.MenuDelete, 0, 0);
            return GLOW__NO_PROPAGATE;
          }
        }
      default:
        sts = this.local_event_handler(event, rp.x, rp.y);
    }
    if (sts !== 0) {
      this.ctx.register_callback_object(ObjectType.Node, this);
    }

    return sts;
  }
}