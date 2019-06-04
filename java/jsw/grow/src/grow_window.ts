class GrowWindow extends GrowRect {
  file_name = "";
  input_file_name = "";
  scrollbar_width;
  scrollbar_color;
  scrollbar_bg_color;
  vertical_scrollbar;
  horizontal_scrollbar;
  window_scale;
  owner = "";
  userdata = null;
  windowCtx = null;
  wctx_x0;
  wctx_x1;
  wctx_y0;
  wctx_y1;
  y_low_offs = 0;
  h_value = 0;
  v_value = 0;
  v_scrollbar = null;
  h_scrollbar = null;

  constructor(ctx) {
    super(ctx);
  }

  constructor(ctx, n_name, x, y, w, h, draw_type, line_width, fill, border,
              shadow, fill_drawtype) {
    super(ctx);
    this.n_name = n_name;
    this.ll.x = x;
    this.ll.y = y;
    this.ur.x = x + w;
    this.ur.y = y + h;
    this.draw_type = draw_type;
    this.line_width = line_width;
    this.fill = fill;
    this.border = border;
    this.shadow = shadow;
    this.fill_drawtype = fill_drawtype;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowWindow:
          break;
        case GlowSave.GrowWindow_file_name:
          if (tokens.length > 1) {
            this.file_name = tokens[1];
          }
          break;
        case GlowSave.GrowWindow_scrollbar_width:
          this.scrollbar_width = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowWindow_scrollbar_color:
          this.scrollbar_color = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowWindow_scrollbar_bg_color:
          this.scrollbar_bg_color = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowWindow_vertical_scrollbar:
          this.vertical_scrollbar = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowWindow_horizontal_scrollbar:
          this.horizontal_scrollbar = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowWindow_window_scale:
          this.window_scale = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowWindow_owner:
          if (tokens.length > 1) {
            this.owner = tokens[1];
          }
          break;
        case GlowSave.GrowWindow_rect_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowWindow_userdata_cb:
          if (this.ctx.appl !== null) {
            let ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
                UserdataCbType.Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case GlowSave.End:
          if (this.file_name !== null && this.file_name !== "") {
            this.new_ctx();
          }
          return i;
        default:
          console.log("Syntax error in GrowWindow");
          break;
      }
    }

    if (this.file_name !== null && this.file_name !== "") {
      this.new_ctx();
    }

    return i;
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let idx = Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);

    let tmp = Matrix.multiply(t, this.trf);
    let d1 = tmp.apply(this.ll);
    let d2 = tmp.apply(this.ur);
    d1.x = Math.min(d1.x, d2.x);
    d2.x = Math.max(d1.x, d2.x);
    d1.y = Math.min(d1.y, d2.y);
    d2.y = Math.max(d1.y, d2.y);

    if (this.v_scrollbar !== null) {
      if (this.h_scrollbar === null) {
        this.v_scrollbar.set_position(d2.x - this.scrollbar_width, d1.y +
            this.y_low_offs, this.scrollbar_width, d2.y - (d1.y + this.y_low_offs));
      } else {
        this.v_scrollbar.set_position(d2.x - this.scrollbar_width, d1.y +
            this.y_low_offs, this.scrollbar_width, d2.y - (d1.y + this.y_low_offs) -
            this.scrollbar_width);
      }
      this.v_scrollbar.draw(null, 0, 0, null, null);
    }
    if (this.h_scrollbar !== null) {
      if (this.v_scrollbar === null) {
        this.h_scrollbar.set_position(d1.x, d2.y - this.scrollbar_width, d2.x -
            d1.x, this.scrollbar_width);
      } else {
        this.h_scrollbar.set_position(d1.x, d2.y - this.scrollbar_width, d2.x -
            d1.x - this.scrollbar_width, this.scrollbar_width);
      }
      this.h_scrollbar.draw(null, 0, 0, null, null);
    }

    let ll_x = Math.floor(d1.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let ll_y = Math.floor((d1.y + this.y_low_offs) * this.ctx.mw.zoom_factor_y) -
        this.ctx.mw.offset_y;

    if (this.windowCtx !== null) {
      let ur_x = Math.floor((d2.x - this.vertical_scrollbar * this.scrollbar_width) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      let ur_y =
          Math.floor((d2.y - this.horizontal_scrollbar * this.scrollbar_width) *
              this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

      this.windowCtx.mw.window_width =
          Math.floor((this.ur_x - this.ll_x) * this.ctx.mw.zoom_factor_x);
      this.windowCtx.mw.window_height =
          Math.floor((this.ur_y - this.ll_y) * this.ctx.mw.zoom_factor_y);
      this.windowCtx.mw.subwindow_x =
          Math.floor(this.ll_x * this.ctx.mw.zoom_factor_x -
              this.ctx.mw.offset_x);
      this.windowCtx.mw.subwindow_y =
          Math.floor(this.ll_y * this.ctx.mw.zoom_factor_y -
              this.ctx.mw.offset_y);
      this.windowCtx.mw.offset_x =
          -ll_x + Math.floor(this.h_value * this.ctx.mw.zoom_factor_x);
      this.windowCtx.mw.offset_y =
          -ll_y + Math.floor(this.v_value * this.ctx.mw.zoom_factor_y);
      this.windowCtx.mw.zoom_factor_x =
          this.windowCtx.mw.subwindow_scale * this.ctx.mw.zoom_factor_x;
      this.windowCtx.mw.zoom_factor_y =
          this.windowCtx.mw.subwindow_scale * this.ctx.mw.zoom_factor_y;
      // window_ctx->draw_buffer_only = ctx->draw_buffer_only;

      if (this.fill) {
        this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
            this.fill_drawtype, true, 0);
      }

      this.ctx.gdraw.set_clip_rectangle(ll_x, ll_y, ur_x - 1, ur_y - 1);

      if (this.windowCtx.customcolors !== null) {
        this.ctx.gdraw.push_customcolors(this.windowCtx.customcolors);
      }
      this.windowCtx.draw();
      if (this.windowCtx.customcolors !== null) {
        this.ctx.gdraw.pop_customcolors();
      }

      this.ctx.gdraw.reset_clip_rectangle();
    }

    let ur_x = Math.floor(d2.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let ur_y = Math.floor(d2.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    let drawtype =
        GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
            highlight, colornode, 0, 0);
    this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, false, idx);
  }

  new_ctx() {
    if (this.file_name === "_no_" || this.file_name === "") {
      return;
    }

    let idx;
    if ((idx = this.file_name.indexOf('/')) !== -1) {
      this.file_name = this.file_name.substring(idx + 1);
    }

    if (this.file_name.indexOf('.') === -1) {
      this.file_name += ".pwg";
    }

    let windOwner = this.owner;
    for (let i = 0; i < 4; i++) {
      let idx;
      if ((idx = windOwner.indexOf("$object")) === -1) {
        break;
      } else {
        let oname = this.ctx.getOwner();
        windOwner =
            windOwner.substring(0, idx) + oname + windOwner.substring(idx + 7);
      }
    }
    this.owner = windOwner;

    this.windowCtx = this.ctx.appl.loadCtx(this.file_name, this.read_cb);
    this.windowCtx.appl = this.ctx.appl;
  }

  read_cb(lines, row) {
    let ctx_popped = this.ctx.appl.ctxPop(this.ctx);

    this.windowCtx.gdraw = this.ctx.gdraw;
    this.ctx.appl.frame.read_func(this.windowCtx, lines, row);

    this.windowCtx.owner = this.owner;
    this.windowCtx.mw.subwindow_scale = this.window_scale;
    this.windowCtx.mw.zoom_factor_x =
        this.windowCtx.mw.subwindow_scale * this.ctx.mw.zoom_factor_x;
    this.windowCtx.mw.zoom_factor_y = this.windowCtx.mw.zoom_factor_x;

    this.input_file_name = this.file_name;
    if (this.windowCtx.background_color !== DrawType.Inherit) {
      this.fill_drawtype = this.windowCtx.background_color;
      this.original_fill_drawtype = this.fill_drawtype;
      this.fill = true;
    }
    if (this.windowCtx.x0 !== this.windowCtx.x1 &&
        this.windowCtx.y0 !== this.windowCtx.y1) {
      this.wctx_x0 = this.windowCtx.x0;
      this.wctx_x1 = this.windowCtx.x1;
      this.wctx_y0 = this.windowCtx.y0;
      this.wctx_y1 = this.windowCtx.y1;
    } else {
      this.wctx_x0 = this.windowCtx.ll_x;
      this.wctx_x1 = this.windowCtx.ur_x;
      this.wctx_y0 = this.windowCtx.ll_y;
      this.wctx_y1 = this.windowCtx.ur_y;
    }
    this.windowCtx.mw.window_width =
        Math.floor((this.ur_x - this.ll_x) * this.ctx.mw.zoom_factor_x);
    this.windowCtx.mw.window_height =
        Math.floor((this.ur_y - this.ll_y) * this.ctx.mw.zoom_factor_y);
    this.windowCtx.mw.subwindow_x =
        Math.floor(this.ll_x * this.ctx.mw.zoom_factor_x -
            this.ctx.mw.offset_x);
    this.windowCtx.mw.subwindow_y =
        Math.floor(this.ll_y * this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y);
    this.windowCtx.mw.subwindow_scale =
        this.ctx.mw.subwindow_scale * this.window_scale;
    this.windowCtx.mw.zoom_factor_x =
        this.ctx.mw.zoom_factor_x * this.windowCtx.mw.subwindow_scale;
    this.windowCtx.mw.zoom_factor_y = this.windowCtx.mw.zoom_factor_x;

    this.configureScrollbars();
    if (this.windowCtx.customcolors !== null) {
      this.ctx.gdraw.pop_customcolors();
    }

    this.windowCtx.traceConnect();
    this.draw();

    if (ctx_popped) {
      this.ctx.appl.ctxPush();
    }
  }

  event_handler(event, fx, fy) {
    /*
      switch ( event.event) {
      case Event.Key_Right:
      case Event.Key_Left:
      case Event.Key_Up:
      case Event.Key_Down:
      case Event.Key_BackSpace:
      case Event.Key_Return:
      case Event.Key_Tab:
      case Event.Key_ShiftTab:
      case Event.Key_Escape:
      case Event.Key_Ascii:
      if ( input_focus) {
      this.windowCtx.eventHandler( event, fx, 0);
      if ( !this.windowCtx.inputfocus_object) {
      this.ctx.register_inputfocus( this, 0);
      input_focus = 0;
      }
      return 1;
      } else
      return 0;
      case Event.ScrollUp:
      if ( !this.ctx.trace_started)
      return 0;
      if ( v_scrollbar) {
      double rx, ry;

      // Convert koordinates to local koordinates
      rp = this.trf.reverse( fx, fy);
      sts = local_event_handler( event, rp.x, rp.y);
      if ( sts) {
      v_value -= (wctx_y1 - wctx_y0) * window_scale/50;
      if ( v_value < wctx_y0 * window_scale)
      v_value = wctx_y0 * window_scale;
      draw();
      v_scrollbar.set_value( v_value, y_high -
      (y_low + y_low_offs) - scrollbar_width * horizontal_scrollbar);
      return 1;
      } else
      return 0;
      }
      break;
      case Event.ScrollDown:
      if ( !this.ctx.trace_started)
      return 0;
      if ( v_scrollbar) {
      double rx, ry;

      // Convert koordinates to local koordinates
      rp = this.trf.reverse( fx, fy);
      sts = local_event_handler( event);
      if ( sts) {
      v_value += (wctx_y1 - wctx_y0) * window_scale/50;
      if ( v_value > wctx_y1 * window_scale - ((y_high - (y_low + y_low_offs) - scrollbar_width * horizontal_scrollbar)))
      v_value = wctx_y1 * window_scale - ((y_high - (y_low + y_low_offs) - scrollbar_width * horizontal_scrollbar));
      draw();
      v_scrollbar.set_value( v_value, y_high -
      (y_low + y_low_offs) - scrollbar_width * horizontal_scrollbar);
      return 1;
      } else
      return 0;
      }
      break;
      default:
      ;
      }

    */

    let v_sts = 0;
    let h_sts = 0;
    if (this.v_scrollbar !== null) {
      v_sts = this.v_scrollbar.event_handler(event, fx, fy);
    }
    if (this.h_scrollbar !== null) {
      h_sts = this.h_scrollbar.event_handler(event, fx, fy);
    }
    if (v_sts !== 0 || h_sts !== 0) {
      return 1;
    }

    let sts = super.event_handler(event, fx, fy);

    if (this.ctx.callback_object === this) {
      // Disable event callback for this object, let the window ctx handle it
      this.ctx.callback_object = null;
      this.ctx.callback_object_type = ObjectType.NoObject;
    }

    if (this.windowCtx !== null && sts !== 0) {
      let ur_x = Math.floor((this.ur_x - this.vertical_scrollbar *
          this.scrollbar_width) * this.ctx.mw.zoom_factor_x) -
          this.ctx.mw.offset_x;
      let ll_x = Math.floor(this.ll_x * this.ctx.mw.zoom_factor_x) -
          this.ctx.mw.offset_x;
      let ur_y = Math.floor((this.ur_y - this.horizontal_scrollbar *
          this.scrollbar_width) * this.ctx.mw.zoom_factor_y) -
          this.ctx.mw.offset_y;
      let ll_y = Math.floor((this.ll_y + this.y_low_offs) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

      // window_ctx->draw_buffer_only = ctx->draw_buffer_only;
      this.ctx.gdraw.set_clip_rectangle(ll_x, ll_y, ur_x, ur_y);

      // Set callback to redraw the background
      // this.windowCtx.redraw_callback = redraw_cb;
      // this.windowCtx.redraw_data = (Object)this;

      let e = new GlowEvent();
      e.event = event.event;
      e.type = event.type;

      e.x = (event.x - this.ll_x) / this.windowCtx.mw.subwindow_scale +
          this.h_value / this.windowCtx.mw.subwindow_scale;
      e.y = (event.y - this.ll_y) / this.windowCtx.mw.subwindow_scale +
          this.v_value / this.windowCtx.mw.subwindow_scale;
      e.object = event.object;
      sts = this.windowCtx.event_handler(e, fx, fy);
      if (sts === GLOW__TERMINATED) {
        return sts;
      }

      // this.windowCtx.redraw_callback = 0;
      // this.windowCtx.redraw_data = 0;
      this.ctx.gdraw.reset_clip_rectangle();

      /*
	if ( this.windowCtx.inputfocus_object && !input_focus) {
	this.ctx.register_inputfocus( this, 1);
	input_focus = 1;
	}
	else if ( !this.windowCtx.inputfocus_object && input_focus) {
	this.ctx.register_inputfocus( this, 0);
	input_focus = 0;
	}
      */
      // if ( window_ctx->callback_object)
      //  ctx->register_callback_object( window_ctx->callback_object_type, window_ctx->callback_object);
    }
    return sts;
  }

  configureScrollbars() {
    if (this.vertical_scrollbar !== 0 && this.v_scrollbar === null) {
      let x0 = this.ur_x - this.scrollbar_width;
      let y0 = this.ll_y + this.y_low_offs;
      let width = this.scrollbar_width;
      let height = this.ur_y - (this.ll_y + this.y_low_offs) - this.scrollbar_width * this.horizontal_scrollbar;

      this.v_scrollbar = new GrowScrollBar(this.ctx, "vScrollbar", x0, y0,
          width, height, Dir.Vertical, DrawType.Line, 1,
          this.display_level, this.scrollbar_bg_color, this.scrollbar_color,
          1, this);
      if (this.windowCtx !== null) {
        this.v_scrollbar.set_range(this.wctx_y0 *
            this.windowCtx.mw.subwindow_scale, this.wctx_y1 * this.window_scale);
        this.v_scrollbar.set_value(this.wctx_y0 *
            this.windowCtx.mw.subwindow_scale, this.ur_y -
            (this.ll_y + this.y_low_offs) - this.scrollbar_width *
            this.horizontal_scrollbar);
        this.v_value = this.wctx_y0 * this.windowCtx.mw.subwindow_scale;
      }
      this.v_scrollbar.set_shadow(this.shadow);
    } else if (this.v_scrollbar !== null) {
      if (this.windowCtx !== null) {
        this.v_scrollbar.set_range(this.wctx_y0 *
            this.windowCtx.mw.subwindow_scale, this.wctx_y1 * this.window_scale);
        this.v_scrollbar.set_value(this.wctx_y0 *
            this.windowCtx.mw.subwindow_scale, this.ur_y -
            (this.ll_y + this.y_low_offs) - this.scrollbar_width *
            this.horizontal_scrollbar);
        this.v_value = this.wctx_y0 * this.windowCtx.mw.subwindow_scale;
      }
    } else {
      this.v_value = this.wctx_y0 * this.windowCtx.mw.subwindow_scale;
    }

    if (this.horizontal_scrollbar !== 0 && this.h_scrollbar === null) {
      let x0 = this.ll_x;
      let y0 = this.ur_y - this.scrollbar_width;
      let height = this.scrollbar_width;
      let width = this.ur_x - this.ll_x - this.scrollbar_width * this.vertical_scrollbar;

      this.h_scrollbar = new GrowScrollBar(this.ctx, "vScrollbar", x0, y0,
          width, height, Dir.Horizontal, DrawType.Line, 1,
          this.display_level, this.scrollbar_bg_color, this.scrollbar_color,
          1, this);
      if (this.windowCtx !== null) {
        this.h_scrollbar.set_range(this.wctx_x0 *
            this.windowCtx.mw.subwindow_scale, this.wctx_x1 *
            this.windowCtx.mw.subwindow_scale);
        this.h_scrollbar.set_value(this.wctx_x0 *
            this.windowCtx.mw.subwindow_scale, this.ur_x - this.ll_x -
            this.scrollbar_width * this.vertical_scrollbar);
        this.h_value = this.wctx_x0 * this.windowCtx.mw.subwindow_scale;
      }
      this.h_scrollbar.set_shadow(this.shadow);
    } else if (this.h_scrollbar !== null) {
      if (this.windowCtx !== null) {
        this.h_scrollbar.set_range(this.wctx_x0 *
            this.windowCtx.mw.subwindow_scale, this.wctx_x1 *
            this.windowCtx.mw.subwindow_scale);
        this.h_scrollbar.set_value(this.wctx_x0 *
            this.windowCtx.mw.subwindow_scale, this.ur_x - this.ll_x -
            this.scrollbar_width * this.vertical_scrollbar);
        this.h_value = this.wctx_x0 * this.windowCtx.mw.subwindow_scale;
      }
    } else {
      this.h_value = this.wctx_x0 * this.windowCtx.mw.subwindow_scale;
    }
  }

  verticalScrollValueChanged(value) {
    if (this.v_value !== value) {
      this.v_value = value;
      this.draw();
    }
  }

  horizontalScrollValueChanged(value) {
    if (this.h_value !== value) {
      this.h_value = value;
      this.draw();
    }
  }

  update_attributes() {
    let ctx_popped = this.ctx.appl.ctxPop(this.ctx);

    if (this.owner === null) {
      this.owner = "";
    }
    if (this.input_file_name !== this.file_name || this.windowCtx === null ||
        this.windowCtx.owner !== this.owner) {
      // New graph, create new context
      let ur_x = Math.floor(this.ur_x * this.ctx.mw.zoom_factor_x) -
          this.ctx.mw.offset_x;
      let ll_x = Math.floor(this.ll_x * this.ctx.mw.zoom_factor_x) -
          this.ctx.mw.offset_x;
      let ur_y = Math.floor(this.ur_y * this.ctx.mw.zoom_factor_y) -
          this.ctx.mw.offset_y;
      let ll_y = Math.floor(this.ll_y * this.ctx.mw.zoom_factor_y) -
          this.ctx.mw.offset_y;
      this.ctx.gdraw.set_clip_rectangle(ll_x, ll_y, ur_x - 1, ur_y - 1);

      if (this.windowCtx !== null) {
        if (this.windowCtx.traceStarted()) {
          this.windowCtx.traceDisconnect();
        }
        this.fill_drawtype = DrawType.Inherit;
        this.original_fill_drawtype = this.fill_drawtype;
        this.fill = false;
      }
      this.file_name = this.input_file_name;
      this.new_ctx();

      this.ctx.gdraw.reset_clip_rectangle();
    }
    if (ctx_popped) {
      this.ctx.appl.ctxPush();
    }
  }

  traceConnect() {
    if (this.windowCtx) {
      this.windowCtx.traceConnect();
    }
  }

  traceDisconnect() {
    if (this.windowCtx) {
      this.windowCtx.traceDisconnect();
    }
  }

  traceScan() {
    if (this.windowCtx) {
      this.windowCtx.traceScan();
    }
  }

  setSource(file_name, owner) {
    this.input_file_name = file_name;
    this.owner = owner;
    this.update_attributes();
  }

  get_background_object_limits(t, type, x, y, b) {
    console.log("GrowWindow.get_background_object_limits");

    for (let i = 0; i < this.windowCtx.a.size(); i++) {
      console.log("background_object_limits", this.windowCtx.a.get(i).type());
      let sts = this.windowCtx.a.get(i)
          .get_background_object_limits(null, type, x, y, b);
      if ((sts & 1) !== 0) {
        return sts;
      }
    }
    return 0;
  }
}