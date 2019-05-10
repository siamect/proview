class GrowCtxWindow {
  zoom_factor_x = 1;
  zoom_factor_y = 1;
  base_zoom_factor = 1;
  offset_x = 0;
  offset_y = 0;
  window_width = 0;
  window_height = 0;
  subwindow_x = 0;
  subwindow_y = 0;
  subwindow_scale = 1;
}

class GrowCtx extends Rect {
  appl = null;
  debug = false;
  antiAliasing = 0;
  name = null;
  version = 0;
  background_color = 0;
  dynamicsize = 0;
  arg_cnt = 0;
  argname = [];
  argtype = [];
  x0 = 0;
  y0 = 0;
  x1 = 0;
  y1 = 0;
  path_cnt = 0;
  path = [];
  dyn_type1 = 0;
  dyn_type2 = 0;
  dyn_action_type1 = 0;
  dyn_action_type2 = 0;
  dyn_color = [];
  dyn_attr = [];
  slider = 0;
  subgraph = 0;
  scantime = 0;
  fast_scantime = 0;
  animation_scantime = 0;
  java_width = 0;
  background_image = null;
  background_tiled = 0;
  double_buffered = 0;
  cycle = 0;
  mb3_action = 0;
  translate_on = 0;
  input_focus_mark = 0;
  hot_indication = 0;
  app_motion = AppMotion.Both;
  color_theme = null;
  default_color_theme = "pwr_colortheme1.pwgc";
  userdata = null;
  callback_object = null;
  callback_object_type = 0;
  hot_mode = 0;
  hot_found = 0;
  restriction_object = null;
  move_restriction = 0;
  restriction_max_limit = 0;
  restriction_min_limit = 0;
  recursive_trace = 0;
  nodraw = 0;
  owner = "";

  name = null;
  sliderActive = false;
  sliderObject = null;
  node_move_last_x = 0;
  node_move_last_y = 0;
  slider_cursor_offset = 0;
  trace_started = false;
  drawing = false;

  mw: GrowCtxWindow;
  customcolors: GlowCustomColors;
  a: GlowArray;
  a_nc: GlowArray;
  a_cc: GlowArray;
  gdraw: GlowDraw;

  constructor(ctx) {
    super();
    this.mw = new GrowCtxWindow();
    this.customcolors = new GlowCustomColors();
    this.a = new GlowArray(this);
    this.a_nc = new GlowArray(this);
    this.a_cc = new GlowArray(this);

    if (ctx) {
      this.gdraw = ctx.gdraw;
    } else {
      this.gdraw = new GlowDraw(this);
    }
  }

  open(lines, row) {
    let zoom_y_found = false;
    let grow_loaded = false;
    let end = false;
    for (let i = row; i < lines.length; i++) {
      if (lines[i].length > 2 && lines[i].substring(0, 2) === "0!") {
        continue;
      }
      if (lines[i].length > 1 && lines[i].substring(0, 1) === "!") {
        continue;
      }

      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      if (this.debug) {
        console.log("ctx : " + lines[i]);
      }

      switch (key) {
        case GlowSave.Ctx:
          break;
        case GlowSave.Ctx_zoom_factor_x:
          this.mw.zoom_factor_x = parseFloat(tokens[1]);
          break;
        case GlowSave.Ctx_zoom_factor_y:
          this.mw.zoom_factor_y = parseFloat(tokens[1]);
          zoom_y_found = true;
          break;
        case GlowSave.Ctx_base_zoom_factor:
          this.mw.base_zoom_factor = parseFloat(tokens[1]);
          break;
        case GlowSave.Ctx_offset_x:
          this.mw.offset_x = parseInt(tokens[1], 10);
          break;
        case GlowSave.Ctx_offset_y:
          this.mw.offset_y = parseInt(tokens[1], 10);
          break;
        case GlowSave.Ctx_nav_zoom_factor_x:
        case GlowSave.Ctx_nav_zoom_factor_y:
        case GlowSave.Ctx_print_zoom_factor:
        case GlowSave.Ctx_nav_offset_x:
        case GlowSave.Ctx_nav_offset_y:
          break;
        case GlowSave.Ctx_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case GlowSave.Ctx_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case GlowSave.Ctx_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case GlowSave.Ctx_y_low:
          this.ll_y = parseFloat(tokens[1]);
          break;
        case GlowSave.Ctx_nav_rect_ll_x:
        case GlowSave.Ctx_nav_rect_ll_y:
        case GlowSave.Ctx_nav_rect_ur_x:
        case GlowSave.Ctx_nav_rect_ur_y:
        case GlowSave.Ctx_nav_rect_hot:
          break;
        case GlowSave.Ctx_name:
          this.name = tokens[1];
          break;
        case GlowSave.Ctx_user_highlight:
        case GlowSave.Ctx_grid_size_x:
        case GlowSave.Ctx_grid_size_y:
        case GlowSave.Ctx_grid_on:
        case GlowSave.Ctx_draw_delta:
        case GlowSave.Ctx_refcon_width:
        case GlowSave.Ctx_refcon_height:
        case GlowSave.Ctx_refcon_textsize:
        case GlowSave.Ctx_refcon_linewidth:
          break;
        case GlowSave.Ctx_version:
          this.version = parseInt(tokens[1], 10);
          break;
        case GlowSave.Ctx_hot_indication:
          this.hot_indication = parseInt(tokens[1], 10);
          break;
        case GlowSave.Ctx_tiptext_size:
          break;
        case GlowSave.Ctx_app_motion:
          this.app_motion = parseInt(tokens[1], 10);
          break;
        case GlowSave.Ctx_color_theme:
          if (tokens.length > 1) {
            this.color_theme = tokens[1];
          }
          break;
        case GlowSave.Ctx_grow:
          i = this.open_grow(lines, i + 1);
          grow_loaded = true;
          break;
        case GlowSave.Ctx_a_nc:
          i = this.a_nc.open(lines, i + 1);
          break;
        case GlowSave.Ctx_a_cc:
          i = this.a_cc.open(lines, i + 1);
          break;
        case GlowSave.Ctx_a:
          i = this.a.open(lines, i + 1);
          break;
        case GlowSave.Ctx_comment:
          i = this.open_comment(lines, i + 1);
          break;
        case GlowSave.End:
          end = true;
          break;
        case GlowSave.Comment:
          break;
        default:
          console.log("Syntax error in GrowCtx", key);
          break;
      }
      if (end) {
        break;
      }
    }
    if (!zoom_y_found) {
      this.mw.zoom_factor_y = this.mw.zoom_factor_x;
    }

    if (this.color_theme !== null && this.color_theme !== "") {
      if (this.color_theme === "$default") {
        if (this.default_color_theme !== null) {
          this.customcolors.read_colorfile(this, this.default_color_theme);
          this.gdraw.push_customcolors(this.customcolors);
        }
      } else {
        this.customcolors.read_colorfile(this, this.color_theme);
        this.gdraw.push_customcolors(this.customcolors);
      }
    }
  }

  open_comment(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      if (lines[i].substring(0, 1) === '!') {
        break;
      }
      if (lines[i].substring(0, 3) === "!*/") {
        break;
      }
    }

    return i;
  }

  open_grow(lines, row) {
    let i, j;

    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowCtx:
          break;
        case GlowSave.GrowCtx_conpoint_num_cnt:
        case GlowSave.GrowCtx_objectname_cnt:
        case GlowSave.GrowCtx_name:
          this.name = tokens[1];
          break;
        case GlowSave.GrowCtx_background_color:
          this.background_color = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case GlowSave.GrowCtx_arg_cnt:
          this.arg_cnt = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_argname:
          for (j = 0; j < this.arg_cnt; j++) {
            this.argname[j] = tokens[1];
          }
          break;
        case GlowSave.GrowCtx_argtype:
          for (j = 0; j < this.arg_cnt; j++) {
            this.argtype[j] = parseInt(tokens[1], 10);
          }
          break;
        case GlowSave.GrowCtx_x0:
          this.x0 = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowCtx_y0:
          this.y0 = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowCtx_x1:
          this.x1 = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowCtx_y1:
          this.y1 = parseFloat(tokens[1]);
          break;
        case GlowSave.End:
          this.mw.offset_x = this.x0 * this.mw.zoom_factor_x;
          this.mw.offset_y = this.y0 * this.mw.zoom_factor_y;
          return i;
        case GlowSave.GrowCtx_path_cnt:
          this.path_cnt = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_path:
          for (j = 0; j < this.path_cnt; j++) {
            i++;
            this.path[j] = lines[i];
          }
          break;
        case GlowSave.GrowCtx_dyn_type1:
          this.dyn_type1 = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dyn_type2:
          this.dyn_type2 = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dyn_action_type1:
          this.dyn_action_type1 = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dyn_action_type2:
          this.dyn_action_type2 = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dyn_color1:
          this.dyn_color[0] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dyn_color2:
          this.dyn_color[1] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dyn_color3:
          this.dyn_color[2] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dyn_color4:
          this.dyn_color[3] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dyn_attr1:
          this.dyn_attr[0] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dyn_attr2:
          this.dyn_attr[1] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dyn_attr3:
          this.dyn_attr[2] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_dyn_attr4:
          this.dyn_attr[3] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_no_con_obstacle:
          break;
        case GlowSave.GrowCtx_slider:
          this.slider = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_subgraph:
          this.subgraph = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_java_name:
        case GlowSave.GrowCtx_is_javaapplet:
        case GlowSave.GrowCtx_is_javaapplication:
        case GlowSave.GrowCtx_next_subgraph:
        case GlowSave.GrowCtx_animation_count:
          break;
        case GlowSave.GrowCtx_scantime:
          this.scantime = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowCtx_fast_scantime:
          this.fast_scantime = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowCtx_animation_scantime:
          this.animation_scantime = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowCtx_java_width:
          this.java_width = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_background_image:
          if (tokens.length > 1) {
            this.background_image = tokens[1];
          }
          break;
        case GlowSave.GrowCtx_background_tiled:
          this.background_tiled = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_double_buffered:
          this.double_buffered = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_cycle:
          this.cycle = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_mb3_action:
          this.mb3_action = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_translate_on:
          this.translate_on = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_input_focus_mark:
          this.input_focus_mark = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_recursive_trace:
          this.recursive_trace = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowCtx_userdata_cb:
          if (this.appl !== null) {
            let ret = this.appl.growUserdataOpen(lines, i + 1, this,
                UserdataCbType.Ctx);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case GlowSave.GrowCtx_bitmap_fonts:
          break;
        case GlowSave.GrowCtx_customcolors:
          i = this.customcolors.open(lines, i + 1);
          break;
        default:
          console.log("Syntax error in GrowCtx open_growctx", key);
          break;
      }
    }
    this.mw.offset_x = this.x0 * this.mw.zoom_factor_x;
    this.mw.offset_y = this.y0 * this.mw.zoom_factor_y;

    return i;
  }

  get_nodeclass_from_name(nc_name) {
    return this.a_nc.find(e => e.nc_name == nc_name);
  }

  get_conclass_from_name(cc_name) {
    return this.a_cc.find(e => e.cc_name == cc_name);
  }

  event_handler(e) {
    let sts = 0;
    let x = e.x * this.mw.zoom_factor_x - this.mw.offset_x;
    let y = e.y * this.mw.zoom_factor_y - this.mw.offset_y;
    let se;

    this.callback_object = null;
    this.callback_object_type = ObjectType.NoObject;

    for (let i = this.a.size() - 1; i >= 0; i--) {
      sts = this.a.get(i).event_handler(e, e.x, e.y);
      if (sts === GLOW__TERMINATED) {
        return sts;
      }
      if (sts === 1 || sts === GLOW__NO_PROPAGATE) {
        break;
      }
    }

    switch (e.event) {
      case Event.MB1Down:
        if (sts === 1 && this.callback_object !== null &&
            this.callback_object instanceof GrowSlider) {
          this.sliderActive = true;
          this.sliderObject = this.callback_object;

          se = new GlowEvent();
          se.event = Event.SliderMoveStart;
          se.type = EventType.Object;
          se.x = e.x;
          se.y = e.y;
          se.object = this.callback_object;
          this.appl.event_handler(se);

          if (this.restriction_object !== null) {
            let g = this.restriction_object.measure();
            if (this.move_restriction === MoveRestriction.VerticalSlider) {
              this.slider_cursor_offset = g.ll_y - e.y;
            } else {
              this.slider_cursor_offset = g.ll_x - e.x;
            }
          }
          this.node_move_last_x = e.x;
          this.node_move_last_y = e.y;
        }
        break;
      case Event.MB1Up:
        if (this.sliderActive) {
          if (this.restriction_object !== null) {
            se = new GlowEvent();
            se.event = Event.SliderMoveEnd;
            se.type = EventType.Object;
            se.x = e.x;
            se.y = e.y;
            se.object = this.restriction_object;
            this.appl.event_handler(se);

            this.restriction_object = null;
          }
          this.sliderObject = null;
          this.sliderActive = false;
        }
        break;
      case Event.ButtonMotion:
        if (this.sliderActive && this.restriction_object !== null) {
          switch (this.move_restriction) {
            case MoveRestriction.VerticalSlider:
              let cursor_y = e.y;
              let move_y;
              if (cursor_y + this.slider_cursor_offset >
                  this.restriction_max_limit) {
                if (this.node_move_last_y + this.slider_cursor_offset >
                    this.restriction_max_limit) {
                  break;
                } else {
                  move_y = this.restriction_max_limit - this.node_move_last_y -
                      this.slider_cursor_offset;
                }
              } else if (cursor_y + this.slider_cursor_offset <
                  this.restriction_min_limit) {
                if (this.node_move_last_y + this.slider_cursor_offset <
                    this.restriction_min_limit) {
                  break;
                } else {
                  move_y = this.restriction_min_limit - this.node_move_last_y -
                      this.slider_cursor_offset;
                }
              } else {
                if (this.node_move_last_y + this.slider_cursor_offset >
                    this.restriction_max_limit) {
                  move_y = cursor_y + this.slider_cursor_offset -
                      this.restriction_max_limit;
                } else if (this.node_move_last_y + this.slider_cursor_offset <
                    this.restriction_min_limit) {
                  move_y = cursor_y + this.slider_cursor_offset -
                      this.restriction_min_limit;
                } else {
                  move_y = e.y - this.node_move_last_y;
                }
              }
              if (move_y === 0) {
                break;
              }
              // set_defered_redraw();
              this.restriction_object.move(0, move_y);
              // redraw_defered();

              se = new GlowEvent();
              se.event = Event.SliderMoved;
              se.type = EventType.Object;
              se.x = e.x;
              se.y = this.node_move_last_y + move_y;
              se.object = this.restriction_object;
              this.appl.event_handler(se);

              this.node_move_last_x = e.x;
              this.node_move_last_y = e.y;
              break;
            case MoveRestriction.HorizontalSlider:
              let cursor_x = e.x;
              let move_x;
              if (cursor_x + this.slider_cursor_offset >
                  this.restriction_max_limit) {
                if (this.node_move_last_x + this.slider_cursor_offset >
                    this.restriction_max_limit) {
                  break;
                } else {
                  move_x = this.restriction_max_limit - this.node_move_last_x -
                      this.slider_cursor_offset;
                }
              } else if (cursor_x + this.slider_cursor_offset <
                  this.restriction_min_limit) {
                if (this.node_move_last_x + this.slider_cursor_offset <
                    this.restriction_min_limit) {
                  break;
                } else {
                  move_x = this.restriction_min_limit - this.node_move_last_x -
                      this.slider_cursor_offset;
                }
              } else {
                if (this.node_move_last_x + this.slider_cursor_offset >
                    this.restriction_max_limit) {
                  move_x = cursor_x + this.slider_cursor_offset -
                      this.restriction_max_limit;
                } else if (this.node_move_last_x + this.slider_cursor_offset <
                    this.restriction_min_limit) {
                  move_x = cursor_x + this.slider_cursor_offset -
                      this.restriction_min_limit;
                } else {
                  move_x = e.x - this.node_move_last_x;
                }
              }
              if (move_x === 0) {
                break;
              }
              // set_defered_redraw();
              this.restriction_object.move(move_x, 0);
              // redraw_defered();

              se = new GlowEvent();
              se.event = Event.SliderMoved;
              se.type = EventType.Object;
              se.x = this.node_move_last_x + move_x;
              se.y = e.y;
              se.object = this.restriction_object;
              this.appl.event_handler(se);

              this.node_move_last_x = e.x;
              this.node_move_last_y = e.y;
              break;
            default:
              break;
          }
        }
        break;
      default:
        break;
    }

    if (sts === 1 && this.appl !== null) {
      e.object = this.callback_object;
      this.appl.event_handler(e);
    }
    return sts;
  }

  getWidth() {
    return this.gdraw.canvas.width;
  }

  getHeight() {
    return this.gdraw.canvas.height;
  }

  setNodraw() {
    this.nodraw++;
  }

  resetNodraw() {
    if (this.nodraw > 0) {
      this.nodraw--;
    }
  }

  getNodraw() {
    return this.nodraw;
  }

  draw(t = null, highlight = 0, hot = 0, node = null, colornode = null) {
    // Draw background color
    this.gdraw.fill_rect(0, 0, this.gdraw.canvas.width,
        this.gdraw.canvas.height, this.background_color);

    // Draw connections
    this.a.forEach(function (e) {
      if (e instanceof GlowCon) {
        e.draw();
      }
    });
    // Draw nodes
    this.a.forEach(function (e) {
      if (!(e instanceof GlowCon)) {
        e.draw();
      }
    });
  }

  traceConnect() {
    this.nodraw++;
    let appl = this.appl;
    this.a.forEach(function (e) {
      if (e instanceof GrowNode ||
          e instanceof GrowGroup ||
          e instanceof GrowBar ||
          e instanceof GrowBarArc ||
          e instanceof GrowTrend ||
          e instanceof GrowTable ||
          e instanceof GrowSlider ||
          e instanceof GrowWindow ||
          e instanceof GrowFolder ||
          e instanceof GrowXYCurve ||
          e instanceof GrowPie ||
          e instanceof GrowBarChart ||
          e instanceof GrowToolbar ||
          e instanceof GrowAxis ||
          e instanceof GrowAxisArc) {
        appl.traceConnect(e);
        if (e instanceof GrowGroup) {
          e.nc.a.forEach(function (f) {
            if (f instanceof GrowNode || f instanceof GrowGroup) {
              appl.traceConnect(f);
            }
          });
        } else if (e instanceof GrowWindow ||
            e instanceof GrowFolder) {
          e.traceConnect();
        }
      }
    });
    this.nodraw--;
    this.trace_started = true;
  }

  traceDisconnect() {
    this.trace_started = false;
    let appl = this.appl;
    this.a.forEach(function (e) {
      if (e instanceof GrowNode ||
          e instanceof GrowGroup ||
          e instanceof GrowBar ||
          e instanceof GrowBarArc ||
          e instanceof GrowTrend ||
          e instanceof GrowTable ||
          e instanceof GrowSlider ||
          e instanceof GrowWindow ||
          e instanceof GrowFolder ||
          e instanceof GrowXYCurve ||
          e instanceof GrowPie ||
          e instanceof GrowBarChart ||
          e instanceof GrowAxis ||
          e instanceof GrowAxisArc) {
        appl.traceDisconnect(e);
        if (e instanceof GrowGroup) {
          e.nc.a.forEach(function (f) {
            if (f instanceof GrowNode || f instanceof GrowGroup) {
              appl.traceDisconnect(f);
            }
          });
        } else if (e instanceof GrowWindow ||
            e instanceof GrowFolder) {
          e.traceDisconnect();
        }
      }
    });
  }

  traceScan() {
    let appl = this.appl;
    this.a.forEach(function (e) {
      if (e instanceof GrowNode ||
          e instanceof GrowGroup ||
          e instanceof GrowBar ||
          e instanceof GrowBarArc ||
          e instanceof GrowTrend ||
          e instanceof GrowTable ||
          e instanceof GrowSlider ||
          e instanceof GrowWindow ||
          e instanceof GrowFolder ||
          e instanceof GrowXYCurve ||
          e instanceof GrowPie ||
          e instanceof GrowBarChart ||
          e instanceof GrowAxis ||
          e instanceof GrowAxisArc) {
        appl.traceScan(e);
        if (e instanceof GrowGroup) {
          e.nc.a.forEach(function (f) {
            if (f instanceof GrowNode || f instanceof GrowGroup) {
              appl.traceScan(f);
            }
          });
        } else if (e instanceof GrowWindow ||
            e instanceof GrowFolder) {
          e.traceScan();
        }
      }
    });
  }

  traceStarted() {
    return this.trace_started;
  }

  get_node_from_name(name) {
    return this.a.find(e => (e instanceof GrowNode && name === e.n_name));
  }

  get_object_group(object) {
    return this.a.find(e => (e instanceof GrowGroup && e.get_object_group(object) !== null));
  }

  insert(e) {
    this.a.add(e);
  }

  remove(e) {
    this.a.remove(e);
  }

  register_callback_object(type, o) {
    this.callback_object = o;
    this.callback_object_type = type;
  }

  send_menu_callback(object, item, event, x, y) {
    /* Send a host request callback */
    let e = new GlowEventMenu();

    e.event = event;
    e.type = EventType.Menu;
    e.x = x;
    e.y = y;
    e.object = object;
    e.item = item;
    this.appl.event_handler(e);

    return 1;
  }

  send_table_callback(object, event, x, y, column, row) {
    /* Send a table callback */
    let e = new GlowEventTable();

    e.event = event;
    e.type = EventType.Table;
    e.x = x;
    e.y = y;
    e.object = object;
    e.column = column;
    e.row = row;
    this.appl.event_handler(e);

    return 1;
  }

  send_toolbar_callback(object, category, idx, event, x, y) {
    /* Send a toolbar callback */
    let e = new GlowEventToolbar();

    e.event = event;
    e.type = EventType.Toolbar;
    e.x = x;
    e.y = y;
    e.object = object;
    e.category = category;
    e.idx = idx;
    this.appl.event_handler(e);

    return 1;
  }

  setOwner(owner) {
    this.owner = owner;
  }

  getOwner() {
    return this.owner;
  }

  get_object_list() {
    return this.a;
  }

  getBackgroundObjectLimits(type, x, y) {
    let sts = 0;
    let b = new GlowBackgroundObject();

    for (let i = 0; i < this.a.size(); i++) {
      sts = this.a.get(i).get_background_object_limits(null, type, x, y, b);
      if ((sts & 1) !== 0) {
        break;
      }
    }
    b.sts = sts;
    return b;
  }

  setMoveRestrictions(restriction, max_limit, min_limit, object) {
    this.move_restriction = restriction;
    this.restriction_max_limit = max_limit;
    this.restriction_min_limit = min_limit;
    this.restriction_object = object;
  }

  findByName(name) {
    let uname = name.toUpperCase();
    return this.a.find(e => e.n_name.toUpperCase() === uname);
  }

  setSubwindowSource(name, source, owner) {
    let e = this.findByName(name);
    if (e === null || !(e instanceof GrowWindow)) {
      console.log("Window " + name + " not found " + e);
      return;
    }

    e.setSource(source, owner);
  }

  getSliderActive() {
    return this.sliderActive;
  }

  setSliderActive(active) {
    this.sliderActive = active;
  }

  loadSubgraph(fname) {
    // TODO
    return 0;
  }
}