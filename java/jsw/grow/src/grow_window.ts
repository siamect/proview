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

  init(n_name, x, y, w, h, draw_type, line_width, fill, border, shadow,
       fill_drawtype) {
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

      if (this.ctx.debug) {
        console.log("GrowWindow : " + lines[i]);
      }

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

  tdraw(t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    let idx;
    let drawtype;

    idx = Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, DRAW_TYPE_SIZE - 1);
    let ll_x, ll_y, ur_x, ur_y;
    let dx1, dy1, dx2, dy2;

    if (t === null) {
      dx1 = this.trf.x(this.ll.x, this.ll.y);
      dy1 = this.trf.y(this.ll.x, this.ll.y);
      dx2 = this.trf.x(this.ur.x, this.ur.y);
      dy2 = this.trf.y(this.ur.x, this.ur.y);
    } else {
      dx1 = this.trf.x(t, this.ll.x, this.ll.y);
      dy1 = this.trf.y(t, this.ll.x, this.ll.y);
      dx2 = this.trf.x(t, this.ur.x, this.ur.y);
      dy2 = this.trf.y(t, this.ur.x, this.ur.y);
    }
    dx1 = Math.min(dx1, dx2);
    dx2 = Math.max(dx1, dx2);
    dy1 = Math.min(dy1, dy2);
    dy2 = Math.max(dy1, dy2);

    if (this.v_scrollbar !== null) {
      if (this.h_scrollbar === null) {
        this.v_scrollbar.set_position(dx2 - this.scrollbar_width, dy1 +
            this.y_low_offs, this.scrollbar_width, dy2 - (dy1 + this.y_low_offs));
      } else {
        this.v_scrollbar.set_position(dx2 - this.scrollbar_width, dy1 +
            this.y_low_offs, this.scrollbar_width, dy2 - (dy1 + this.y_low_offs) -
            this.scrollbar_width);
      }
      this.v_scrollbar.tdraw(null, 0, 0, null, null);
    }
    if (this.h_scrollbar !== null) {
      if (this.v_scrollbar === null) {
        this.h_scrollbar.set_position(dx1, dy2 - this.scrollbar_width, dx2 -
            dx1, this.scrollbar_width);
      } else {
        this.h_scrollbar.set_position(dx1, dy2 - this.scrollbar_width, dx2 -
            dx1 - this.scrollbar_width, this.scrollbar_width);
      }
      this.h_scrollbar.tdraw(null, 0, 0, null, null);
    }

    ll_x = Math.floor(dx1 * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    ll_y = Math.floor((dy1 + this.y_low_offs) * this.ctx.mw.zoom_factor_y) -
        this.ctx.mw.offset_y;

    if (this.windowCtx !== null) {
      ur_x = Math.floor((dx2 - this.vertical_scrollbar * this.scrollbar_width) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      ur_y =
          Math.floor((dy2 - this.horizontal_scrollbar * this.scrollbar_width) *
              this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

      this.windowCtx.mw.window_width =
          Math.floor((this.x_right - this.x_left) * this.ctx.mw.zoom_factor_x);
      this.windowCtx.mw.window_height =
          Math.floor((this.y_high - this.y_low) * this.ctx.mw.zoom_factor_y);
      this.windowCtx.mw.subwindow_x =
          Math.floor(this.x_left * this.ctx.mw.zoom_factor_x -
              this.ctx.mw.offset_x);
      this.windowCtx.mw.subwindow_y =
          Math.floor(this.y_low * this.ctx.mw.zoom_factor_y -
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

      if (this.fill !== 0) {
        this.ctx.gdraw.fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
            this.fill_drawtype);
      }

      this.ctx.gdraw.set_clip_rectangle(ll_x, ll_y, ur_x - 1, ur_y - 1);

      if (this.windowCtx.customcolors !== null) {
        this.ctx.gdraw.push_customcolors(this.windowCtx.customcolors);
      }
      this.windowCtx.rdraw(ll_x, ll_y, ur_x, ur_y);
      if (this.windowCtx.customcolors !== null) {
        this.ctx.gdraw.pop_customcolors();
      }

      this.ctx.gdraw.reset_clip_rectangle();
    }

    ur_x = Math.floor(dx2 * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    ur_y = Math.floor(dy2 * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    drawtype =
        GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
            highlight, colornode, 0, 0);
    this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, idx, 0);
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
      this.file_name = this.file_name + ".pwg";
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
    this.windowCtx.mw.zoom_factor_x = this.windowCtx.mw.zoom_factor_y =
        this.windowCtx.mw.subwindow_scale * this.ctx.mw.zoom_factor_x;

    this.input_file_name = this.file_name;
    if (this.windowCtx.background_color !== DrawType.Inherit) {
      this.fill_drawtype =
          this.original_fill_drawtype = this.windowCtx.background_color;
      this.fill = 1;
    }
    if (this.windowCtx.x0 !== this.windowCtx.x1 &&
        this.windowCtx.y0 !== this.windowCtx.y1) {
      this.wctx_x0 = this.windowCtx.x0;
      this.wctx_x1 = this.windowCtx.x1;
      this.wctx_y0 = this.windowCtx.y0;
      this.wctx_y1 = this.windowCtx.y1;
    } else {
      this.wctx_x0 = this.windowCtx.x_left;
      this.wctx_x1 = this.windowCtx.x_right;
      this.wctx_y0 = this.windowCtx.y_low;
      this.wctx_y1 = this.windowCtx.y_high;
    }
    this.windowCtx.mw.window_width =
        Math.floor((this.x_right - this.x_left) * this.ctx.mw.zoom_factor_x);
    this.windowCtx.mw.window_height =
        Math.floor((this.y_high - this.y_low) * this.ctx.mw.zoom_factor_y);
    this.windowCtx.mw.subwindow_x =
        Math.floor(this.x_left * this.ctx.mw.zoom_factor_x -
            this.ctx.mw.offset_x);
    this.windowCtx.mw.subwindow_y =
        Math.floor(this.y_low * this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y);
    this.windowCtx.mw.subwindow_scale =
        this.ctx.mw.subwindow_scale * this.window_scale;
    this.windowCtx.mw.zoom_factor_x = this.windowCtx.mw.zoom_factor_y =
        this.ctx.mw.zoom_factor_x * this.windowCtx.mw.subwindow_scale;

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
    let sts, v_sts, h_sts;
    let rp;

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

    v_sts = h_sts = 0;
    if (this.v_scrollbar !== null) {
      v_sts = this.v_scrollbar.event_handler(event, fx, fy);
    }
    if (this.h_scrollbar !== null) {
      h_sts = this.h_scrollbar.event_handler(event, fx, fy);
    }
    if (v_sts !== 0 || h_sts !== 0) {
      return 1;
    }

    sts = super.event_handler(event, fx, fy);

    if (this.ctx.callback_object === this) {
      // Disable event callback for this object, let the window ctx handle it
      this.ctx.callback_object = null;
      this.ctx.callback_object_type = ObjectType.NoObject;
    }

    if (this.windowCtx !== null && sts !== 0) {
      let ur_x = Math.floor((this.x_right - this.vertical_scrollbar *
          this.scrollbar_width) * this.ctx.mw.zoom_factor_x) -
          this.ctx.mw.offset_x;
      let ll_x = Math.floor(this.x_left * this.ctx.mw.zoom_factor_x) -
          this.ctx.mw.offset_x;
      let ur_y = Math.floor((this.y_high - this.horizontal_scrollbar *
          this.scrollbar_width) * this.ctx.mw.zoom_factor_y) -
          this.ctx.mw.offset_y;
      let ll_y = Math.floor((this.y_low + this.y_low_offs) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

      // window_ctx->draw_buffer_only = ctx->draw_buffer_only;
      this.ctx.gdraw.set_clip_rectangle(ll_x, ll_y, ur_x, ur_y);

      // Set callback to redraw the background
      // this.windowCtx.redraw_callback = redraw_cb;
      // this.windowCtx.redraw_data = (Object)this;

      let e = new GlowEvent();
      e.event = event.event;
      e.type = event.type;

      e.x = (event.x - this.x_left) / this.windowCtx.mw.subwindow_scale +
          this.h_value / this.windowCtx.mw.subwindow_scale;
      e.y = (event.y - this.y_low) / this.windowCtx.mw.subwindow_scale +
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
    let x0, y0, width, height;

    if (this.vertical_scrollbar !== 0 && this.v_scrollbar === null) {
      x0 = this.x_right - this.scrollbar_width;
      y0 = this.y_low + this.y_low_offs;
      width = this.scrollbar_width;
      if (this.horizontal_scrollbar === 0) {
        height = this.y_high - (this.y_low + this.y_low_offs);
      } else {
        height =
            this.y_high - (this.y_low + this.y_low_offs) - this.scrollbar_width;
      }

      this.v_scrollbar = new GrowScrollBar(this.ctx);
      this.v_scrollbar.init("vScrollbar", x0, y0, width, height,
          Dir.Vertical, DrawType.Line, 1, this.display_level,
          this.scrollbar_bg_color, this.scrollbar_color, 1, this);
      if (this.windowCtx !== null) {
        this.v_scrollbar.set_range(this.wctx_y0 *
            this.windowCtx.mw.subwindow_scale, this.wctx_y1 * this.window_scale);
        this.v_scrollbar.set_value(this.wctx_y0 *
            this.windowCtx.mw.subwindow_scale, this.y_high -
            (this.y_low + this.y_low_offs) - this.scrollbar_width *
            this.horizontal_scrollbar);
        this.v_value = this.wctx_y0 * this.windowCtx.mw.subwindow_scale;
      }
      this.v_scrollbar.set_shadow(this.shadow);
    } else if (this.v_scrollbar !== null) {
      if (this.windowCtx !== null) {
        this.v_scrollbar.set_range(this.wctx_y0 *
            this.windowCtx.mw.subwindow_scale, this.wctx_y1 * this.window_scale);
        this.v_scrollbar.set_value(this.wctx_y0 *
            this.windowCtx.mw.subwindow_scale, this.y_high -
            (this.y_low + this.y_low_offs) - this.scrollbar_width *
            this.horizontal_scrollbar);
        this.v_value = this.wctx_y0 * this.windowCtx.mw.subwindow_scale;
      }
    } else {
      this.v_value = this.wctx_y0 * this.windowCtx.mw.subwindow_scale;
    }

    if (this.horizontal_scrollbar !== 0 && this.h_scrollbar === null) {
      x0 = this.x_left;
      y0 = this.y_high - this.scrollbar_width;
      height = this.scrollbar_width;
      if (this.vertical_scrollbar === 0) {
        width = this.x_right - this.x_left;
      } else {
        width = this.x_right - this.x_left - this.scrollbar_width;
      }

      this.h_scrollbar = new GrowScrollBar(this.ctx);
      this.h_scrollbar.init("vScrollbar", x0, y0, width, height,
          Dir.Horizontal, DrawType.Line, 1, this.display_level,
          this.scrollbar_bg_color, this.scrollbar_color, 1, this);
      if (this.windowCtx !== null) {
        this.h_scrollbar.set_range(this.wctx_x0 *
            this.windowCtx.mw.subwindow_scale, this.wctx_x1 *
            this.windowCtx.mw.subwindow_scale);
        this.h_scrollbar.set_value(this.wctx_x0 *
            this.windowCtx.mw.subwindow_scale, this.x_right - this.x_left -
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
            this.windowCtx.mw.subwindow_scale, this.x_right - this.x_left -
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
      let ur_x = Math.floor(this.x_right * this.ctx.mw.zoom_factor_x) -
          this.ctx.mw.offset_x;
      let ll_x = Math.floor(this.x_left * this.ctx.mw.zoom_factor_x) -
          this.ctx.mw.offset_x;
      let ur_y = Math.floor(this.y_high * this.ctx.mw.zoom_factor_y) -
          this.ctx.mw.offset_y;
      let ll_y = Math.floor(this.y_low * this.ctx.mw.zoom_factor_y) -
          this.ctx.mw.offset_y;
      this.ctx.gdraw.set_clip_rectangle(ll_x, ll_y, ur_x - 1, ur_y - 1);

      if (this.windowCtx !== null) {
        if (this.windowCtx.traceStarted()) {
          this.windowCtx.traceDisconnect();
        }
        this.fill_drawtype =
            this.original_fill_drawtype = DrawType.Inherit;
        this.fill = 0;
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
    if (this.windowCtx === null) {
      return;
    }
    this.windowCtx.traceConnect();
  }

  traceDisconnect() {
    if (this.windowCtx === null) {
      return;
    }
    this.windowCtx.traceDisconnect();
  }

  traceScan() {
    if (this.windowCtx === null) {
      return;
    }
    this.windowCtx.traceScan();
  }

  setSource(file_name, owner) {
    this.input_file_name = file_name;
    this.owner = owner;
    this.update_attributes();
  }

  get_background_object_limits(t, type, x, y, b) {
    console.log("GrowWindow.get_background_object_limits");
    let sts = 0;

    for (let i = 0; i < this.windowCtx.a.size(); i++) {
      console.log("background_object_limits", this.windowCtx.a.get(i).type());
      sts = this.windowCtx.a.get(i)
          .get_background_object_limits(null, type, x, y, b);
      if ((sts & 1) !== 0) {
        break;
      }
    }
    return sts;
  }
}