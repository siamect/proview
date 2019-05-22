class GrowTable extends GrowRect {
  mTableOptions_No = 0;
  mTableOptions_ZeroIfHeader = 1 << 0;
  mTableOptions_ZeroIfHeaderIs0 = 1 << 1;
  file_name;
  userdata = null;
  vertical_scrollbar;
  horizontal_scrollbar;
  scrollbar_width;
  v_scrollbar = null;
  h_scrollbar = null;
  v_value;
  h_value;
  table_x0;
  table_x1;
  table_y0;
  table_y1;
  scrollbar_color;
  scrollbar_bg_color;
  window_scale = 1;
  y_low_offs;
  x_left_offs;
  rows;
  columns;
  header_row;
  header_column;
  text_size;
  text_drawtype;
  text_color_drawtype;
  header_text_size;
  header_text_drawtype;
  header_text_color;
  header_row_height;
  row_height;
  column_width = new Array(TABLE_MAX_COL);
  header_text = new Array(TABLE_MAX_COL);
  column_size = new Array(TABLE_MAX_COL);
  column_adjustment = new Array(TABLE_MAX_COL);
  value_size;
  cell_value;
  selected_cell_row = -1;
  selected_cell_column = -1;
  select_drawtype;
  input_focus = 0;
  header_text_bold;
  options;
  font;

  constructor(ctx) {
    super(ctx);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowTable:
          break;
        case GlowSave.GrowTable_scrollbar_width:
          this.scrollbar_width = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_scrollbar_color:
          this.scrollbar_color = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_scrollbar_bg_color:
          this.scrollbar_bg_color = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_vertical_scrollbar:
          this.vertical_scrollbar = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_horizontal_scrollbar:
          this.horizontal_scrollbar = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_window_scale:
          this.window_scale = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_rows:
          this.rows = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_columns:
          this.columns = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_header_row:
          this.header_row = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_header_column:
          this.header_column = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_text_drawtype:
          this.text_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_text_color_drawtype:
          this.text_color_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_header_text_size:
          this.header_text_size = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_header_text_drawtype:
          this.header_text_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_header_text_color:
          this.header_text_color = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_header_text_bold:
          this.header_text_bold = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_header_row_height:
          this.header_row_height = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_row_height:
          this.row_height = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_options:
          this.options = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_select_drawtype:
          this.select_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_font:
          this.font = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_width1:
          this.column_width[0] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text1:
          if (tokens.length > 1) {
            this.header_text[0] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_width2:
          this.column_width[1] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text2:
          if (tokens.length > 1) {
            this.header_text[1] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_width3:
          this.column_width[2] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text3:
          if (tokens.length > 1) {
            this.header_text[2] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_width4:
          this.column_width[3] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text4:
          if (tokens.length > 1) {
            this.header_text[3] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_width5:
          this.column_width[4] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text5:
          if (tokens.length > 1) {
            this.header_text[4] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_width6:
          this.column_width[5] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text6:
          if (tokens.length > 1) {
            this.header_text[5] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_width7:
          this.column_width[6] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text7:
          if (tokens.length > 1) {
            this.header_text[6] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_width8:
          this.column_width[7] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text8:
          if (tokens.length > 1) {
            this.header_text[7] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_width9:
          this.column_width[8] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text9:
          if (tokens.length > 1) {
            this.header_text[8] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_width10:
          this.column_width[9] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text10:
          if (tokens.length > 1) {
            this.header_text[9] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_width11:
          this.column_width[10] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text11:
          if (tokens.length > 1) {
            this.header_text[10] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_width12:
          this.column_width[11] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowTable_header_text12:
          if (tokens.length > 1) {
            this.header_text[11] = lines[i].substring(5);
          }
          break;
        case GlowSave.GrowTable_column_adjustment1:
          this.column_adjustment[0] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_adjustment2:
          this.column_adjustment[1] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_adjustment3:
          this.column_adjustment[2] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_adjustment4:
          this.column_adjustment[3] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_adjustment5:
          this.column_adjustment[4] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_adjustment6:
          this.column_adjustment[5] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_adjustment7:
          this.column_adjustment[6] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_adjustment8:
          this.column_adjustment[7] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_adjustment9:
          this.column_adjustment[8] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_adjustment10:
          this.column_adjustment[9] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_adjustment11:
          this.column_adjustment[10] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_column_adjustment12:
          this.column_adjustment[11] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowTable_rect_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.GrowTable_userdata_cb:
          if (this.ctx.appl !== null) {
            let ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
                UserdataCbType.Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case GlowSave.End:
          this.configure();
          this.configure_scrollbars();
          return i;
        default:
          console.log("Syntax error in GrowTable");
          break;
      }
    }

    this.configure();
    this.configure_scrollbars();
    return i;
  }

  configure() {
    this.table_x0 = 0;
    this.table_y0 = 0;
    this.table_x1 = 0;
    for (let i = 0; i < this.columns; i++) {
      if (this.header_column !== 0 && i === 0) {
        continue;
      }
      this.table_x1 += this.column_width[i];
    }
    this.table_y1 = this.row_height * this.rows;

    this.y_low_offs = (this.header_row === 0) ? 0 : this.header_row_height;
    this.x_left_offs = (this.header_column === 0) ? 0 : this.column_width[0];

    this.cell_value = new Array(this.columns * this.rows);
  }

  configure_scrollbars() {
    if (this.vertical_scrollbar !== 0 && this.v_scrollbar === null) {
      let x0 = this.ur_x - this.scrollbar_width;
      let y0 = this.ll_y + this.y_low_offs;
      let width = this.scrollbar_width;
      let height = this.ur_y - (this.ll_y + this.y_low_offs) - this.scrollbar_width * this.horizontal_scrollbar;

      this.v_scrollbar = new GrowScrollBar(this.ctx, "vScrollbar", x0, y0,
          width, height, Dir.Vertical, DrawType.Line, 1,
          this.display_level, this.scrollbar_bg_color, this.scrollbar_color,
          1, this);
      this.v_scrollbar.set_range(this.table_y0 *
          this.window_scale, this.table_y1 * this.window_scale);
      this.v_scrollbar.set_value(this.table_y0 *
          this.window_scale, this.ur_y - (this.ll_y + this.y_low_offs) -
          this.scrollbar_width * this.horizontal_scrollbar);
      this.v_scrollbar.set_shadow(this.shadow);
      this.v_value = this.table_y0 * this.window_scale;
    } else if (this.vertical_scrollbar === 0 && this.v_scrollbar !== null) {
      this.v_scrollbar = null;
      this.v_value = this.table_y0 * this.window_scale;
    } else if (this.v_scrollbar !== null) {
      // Reconfigure range and length
      this.v_scrollbar.set_range(this.table_y0 *
          this.window_scale, this.table_y1 * this.window_scale);
      this.v_scrollbar.set_value(this.table_y0 *
          this.window_scale, this.ur_y - (this.ll_y + this.y_low_offs) -
          this.scrollbar_width * this.horizontal_scrollbar);
      this.v_value = this.table_y0 * this.window_scale;
      this.v_scrollbar.set_shadow(this.shadow);
    } else {
      this.v_value = this.table_y0 * this.window_scale;
    }

    if (this.horizontal_scrollbar !== 0 && this.h_scrollbar === null) {
      let x0 = this.ll_x + this.x_left_offs;
      let y0 = this.ur_y - this.scrollbar_width;
      let height = this.scrollbar_width;
      let width = this.ur_x - (this.ll_x + this.x_left_offs) - this.scrollbar_width * this.vertical_scrollbar;

      this.h_scrollbar = new GrowScrollBar(this.ctx, "hScrollbar", x0, y0,
          width, height, Dir.Horizontal, DrawType.Line, 1,
          this.display_level, this.scrollbar_bg_color, this.scrollbar_color,
          1, this);
      this.h_scrollbar.set_range(this.table_x0 *
          this.window_scale, this.table_x1 * this.window_scale);
      this.h_scrollbar.set_value(this.table_x0 *
          this.window_scale, this.ur_x - (this.ll_x + this.x_left_offs) -
          this.scrollbar_width * this.vertical_scrollbar);
      this.h_scrollbar.set_shadow(this.shadow);
      this.h_value = this.table_x0 * this.window_scale;
    } else if (this.horizontal_scrollbar === 0 && this.h_scrollbar !== null) {
      this.h_scrollbar = null;
      this.h_value = this.table_x0 * this.window_scale;
    } else if (this.h_scrollbar !== null) {
      // Reconfigure lenght and range
      this.h_scrollbar.set_value(this.table_x0 *
          this.window_scale, this.ur_x - (this.ll_x + this.x_left_offs) -
          this.scrollbar_width * this.vertical_scrollbar);
      this.h_value = this.table_x0 * this.window_scale;
      this.h_scrollbar.set_range(this.table_x0 *
          this.window_scale, this.table_x1 * this.window_scale);
      this.h_scrollbar.set_shadow(this.shadow);
    } else {
      this.h_value = this.table_x0 * this.window_scale;
    }
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

    let text_idx = Math.floor(this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 4);
    let tsize = this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        (8 + 2 * this.text_size);
    text_idx = Math.min(text_idx, DRAW_TYPE_SIZE - 1);
    let header_text_idx = Math.floor(this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (this.header_text_size + 4) - 4);
    header_text_idx = Math.min(header_text_idx, DRAW_TYPE_SIZE - 1);
    let header_tsize = this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (8 + 2 * this.header_text_size);

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
      this.v_scrollbar.draw();

    }
    if (this.h_scrollbar !== null) {
      if (this.v_scrollbar === null) {
        this.h_scrollbar.set_position(d1.x + this.x_left_offs, d2.y -
            this.scrollbar_width, d2.x - (d1.x + this.x_left_offs),
            this.scrollbar_width);
      } else {
        this.h_scrollbar.set_position(d1.x + this.x_left_offs, d2.y -
            this.scrollbar_width, d2.x - (d1.x + this.x_left_offs) -
            this.scrollbar_width, this.scrollbar_width);
      }
      this.h_scrollbar.draw();
    }

    let drawtype =
        GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
            highlight, colornode, 0, 0);

    let dark_drawtype = GlowColor.shift_drawtype(this.fill_drawtype, 2, null);
    let light_drawtype = GlowColor.shift_drawtype(this.fill_drawtype, -2, null);
    let sel_drawtype = (this.select_drawtype === DrawType.Inherit) ? dark_drawtype : this.select_drawtype;

    let ll_x = Math.floor(d1.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let ll_y = Math.floor(d1.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let ur_x = Math.floor(d2.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let ur_y = Math.floor(d2.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    let o_ll_x = Math.floor((d1.x + this.x_left_offs) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let o_ll_y = Math.floor((d1.y + this.y_low_offs) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    let o_ur_x = Math.floor((d2.x - this.vertical_scrollbar *
        this.scrollbar_width) * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let o_ur_y = Math.floor((d2.y - this.horizontal_scrollbar *
        this.scrollbar_width) * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    let t_ll_x = o_ll_x - Math.floor(this.h_value * this.ctx.mw.zoom_factor_x);
    let t_ll_y = o_ll_y - Math.floor(this.v_value * this.ctx.mw.zoom_factor_y);
    let t_ur_x = t_ll_x + Math.floor(this.table_x1 * this.ctx.mw.zoom_factor_x);
    let t_ur_y = t_ll_y + Math.floor(this.table_y1 * this.ctx.mw.zoom_factor_y);

    let text_offs = 10;

    let header_h = 0;
    let header_w = 0;
    if (this.header_row !== 0) {
      header_h = Math.floor(this.header_row_height * this.ctx.mw.zoom_factor_y);
    }
    if (this.header_column !== 0) {
      header_w = Math.floor(this.column_width[0] * this.ctx.mw.zoom_factor_x);
    }

    if (this.header_row !== 0) {
      if (this.fill) {
        this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, header_h,
            this.fill_drawtype, true, 0);
      }

      this.ctx.gdraw.set_clip_rectangle(ll_x + header_w, ll_y, ur_x, ll_y +
          header_h);

      if (this.shadow !== 0) {
        let x = t_ll_x;
        for (let i = this.header_column; i < this.columns + 1; i++) {
          if (x > ur_x) {
            break;
          }
          if (i !== this.header_column) {
            this.ctx.gdraw.line(Math.floor(x) - 1, ll_y, Math.floor(x) -
                1, ll_y + header_h, dark_drawtype, 0, 0);
          }
          if (i !== this.columns) {
            this.ctx.gdraw.line(Math.floor(x) + 1, ll_y, Math.floor(x) +
                1, ll_y + header_h, light_drawtype, 0, 0);
          }
          x += this.column_width[i] * this.ctx.mw.zoom_factor_x;
        }
        this.ctx.gdraw.line(ll_x, ll_y + 1, t_ur_x, ll_y + 1, light_drawtype, 0,
            0);
        this.ctx.gdraw.line(ll_x, ll_y + header_h - 1, t_ur_x, ll_y + header_h -
            1, dark_drawtype, 1, 0);
      }
      let x = t_ll_x;

      for (let i = this.header_column; i < this.columns + 1; i++) {
        if (x > ur_x) {
          break;
        }
        this.ctx.gdraw.line(Math.floor(x), ll_y, Math.floor(x), ll_y + header_h,
            drawtype, idx, 0);
        x += this.column_width[i] * this.ctx.mw.zoom_factor_x;
      }

      x = t_ll_x;

      for (let i = this.header_column; i < this.columns; i++) {
        if (header_text_idx >= 0 && this.header_text[i] !== "") {
          this.ctx.gdraw.text(Math.floor(x + text_offs),
              Math.floor(y + header_h - 4), this.header_text[i],
              this.header_text_drawtype, this.header_text_color, header_text_idx,
              highlight, this.font, header_tsize, 0);
        }
        x += this.column_width[i] * this.ctx.mw.zoom_factor_x;
        if (x > ur_x) {
          break;
        }
      }
      this.ctx.gdraw.reset_clip_rectangle();

      if (header_w !== 0) {
        if (this.shadow !== 0) {
          this.ctx.gdraw.line(ll_x + header_w - 1, ll_y, ll_x + header_w -
              1, ll_y + header_h, dark_drawtype, 1, 0);
          this.ctx.gdraw.line(ll_x - 1, ll_y + header_h - 1, ll_x +
              header_w, ll_y + header_h - 1, dark_drawtype, 1, 0);
          this.ctx.gdraw.line(ll_x, ll_y + 1, ll_x + header_w, ll_y + 1,
              light_drawtype, 0, 0);
          this.ctx.gdraw.line(ll_x + 1, ll_y, ll_x + 1, ll_y + header_h,
              light_drawtype, 0, 0);
        }
        this.ctx.gdraw.line(ll_x + header_w, ll_y, ll_x + header_w, ll_y +
            header_h, drawtype, idx, 0);
      }
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, header_h, drawtype, false, idx);

      if (this.header_column !== 0) {
        // Draw header of header column header
        x = ll_x;
        if (header_text_idx >= 0 && this.header_text[0] !== "") {
          this.ctx.gdraw.text(Math.floor(x + text_offs),
              Math.floor(y + header_h - 4), this.header_text[0],
              this.header_text_drawtype, this.header_text_color, header_text_idx,
              highlight, this.font, tsize, 0);
        }
      }
    }

    if (this.header_column !== 0) {
      if (this.fill) {
        this.ctx.gdraw.rect(ll_x, ll_y + header_h, header_w, ur_y - ll_y -
            header_h, this.fill_drawtype, true, 0);
      }

      this.ctx.gdraw.set_clip_rectangle(ll_x, ll_y + header_h, ll_x + header_w,
          ur_y);

      // Draw selected cell, if cell in header column
      if (this.selected_cell_row >= 0 && this.selected_cell_column === 0) {
        let x = ll_x;
        let y = t_ll_y + this.row_height * this.ctx.mw.zoom_factor_y *
            this.selected_cell_row;
        this.ctx.gdraw.rect(Math.floor(x), Math.floor(y), header_w,
            Math.floor(this.row_height * this.ctx.mw.zoom_factor_y),
            sel_drawtype, true, 0);
      }

      if (this.shadow !== 0) {
        let y = t_ll_y;
        for (let i = 0; i < this.rows + 1; i++) {
          if (y > ur_y) {
            break;
          }
          if (y > ll_y) {
            if (i !== 0) {
              this.ctx.gdraw.line(ll_x, Math.floor(y) - 1, ll_x +
                  header_w, Math.floor(y) - 1, dark_drawtype, 0, 0);
            }
            if (i !== this.rows) {
              this.ctx.gdraw.line(ll_x, Math.floor(y) + 1, ll_x +
                  header_w, Math.floor(y) + 1, light_drawtype, 0, 0);
            }
          }
          y += this.row_height * this.ctx.mw.zoom_factor_y;
        }
        this.ctx.gdraw.line(ll_x + 1, ll_y + header_h, ll_x + 1, ur_y,
            light_drawtype, 0, 0);
        this.ctx.gdraw.line(ll_x + header_w - 1, ll_y + header_h, ll_x +
            header_w - 1, ur_y, dark_drawtype, 1, 0);
      }
      let x = ll_x;
      let y = t_ll_y;

      for (let i = 0; i < this.rows; i++) {
        y += this.row_height * this.ctx.mw.zoom_factor_y;
        if (y - this.row_height * this.ctx.mw.zoom_factor_y > ur_y) {
          break;
        }
        if (y > ll_y) {
          this.ctx.gdraw.line(ll_x, Math.floor(y), ll_x + header_w,
              Math.floor(y), drawtype, idx, 0);

          let offs = i;
          if (text_idx >= 0 && this.cell_value[offs] !== null &&
              this.cell_value[offs] !== "") {
            let text_x = Math.floor(x) + text_offs;

            if (this.column_adjustment[0] === Adjustment.Right ||
                this.column_adjustment[0] === Adjustment.Center) {
              let width, height, descent;
              let p = this.ctx.gdraw.getTextExtent(this.cell_value[offs], this.text_drawtype, text_idx, this.font);
              width = p.x;
              height = p.y;
              descent = height / 4;

              switch (this.column_adjustment[0]) {
                case Adjustment.Left:
                  break;
                case Adjustment.Right:
                  text_x = Math.floor(x) + header_w - text_offs - width;
                  break;
                case Adjustment.Center:
                  text_x = Math.floor(x + (header_w - width) / 2);
                  break;
              }
            }
            this.ctx.gdraw.text(text_x, Math.floor(y - 5),
                this.cell_value[offs], this.text_drawtype,
                this.text_color_drawtype, text_idx, highlight, this.font,
                tsize, 0);
          }
        }
      }
      this.ctx.gdraw.reset_clip_rectangle();
      this.ctx.gdraw.rect(ll_x, ll_y + header_h - 1, header_w, ur_y - ll_y -
          header_h + 1, drawtype, false, idx);
    }

    // Draw table
    if (this.fill) {
      this.ctx.gdraw.rect(o_ll_x, o_ll_y, o_ur_x - o_ll_x, o_ur_y - o_ll_y,
          this.fill_drawtype, true, 0);
    }

    this.ctx.gdraw.set_clip_rectangle(o_ll_x, o_ll_y, o_ur_x, o_ur_y);

    if (this.selected_cell_row >= 0 &&
        !(this.header_column !== 0 && this.selected_cell_column === 0)) {
      // Draw selected cell, if cell not in header column
      let x = t_ll_x;
      for (let i = this.header_column; i < this.selected_cell_column; i++) {
        x += this.column_width[i] * this.ctx.mw.zoom_factor_x;
      }
      let y = t_ll_y + this.row_height * this.ctx.mw.zoom_factor_y *
          this.selected_cell_row;
      this.ctx.gdraw.rect(Math.floor(x), Math.floor(y),
          Math.floor(this.column_width[this.selected_cell_column] *
              this.ctx.mw.zoom_factor_x),
          Math.floor(this.row_height * this.ctx.mw.zoom_factor_y),
          sel_drawtype, true, 0);
    }

    if (this.shadow !== 0) {
      let x = t_ll_x;
      let y = t_ll_y;

      for (let i = this.header_column; i < this.columns + 1; i++) {
        if (x > ur_x) {
          break;
        }
        if (x > ll_x) {
          if (i !== this.header_column) {
            this.ctx.gdraw.line(Math.floor(x) - 1, t_ll_y, Math.floor(x) - 1,
                t_ur_y, dark_drawtype, 0, 0);
          }
          if (i !== this.columns) {
            this.ctx.gdraw.line(Math.floor(x) + 1, t_ll_y, Math.floor(x) + 1,
                t_ur_y, light_drawtype, 0, 0);
          }
        }
        x += this.column_width[i] * this.ctx.mw.zoom_factor_x;
      }

      for (let i = 0; i < this.rows + 1; i++) {
        if (y > ur_y) {
          break;
        }
        if (y > ll_y) {
          if (i !== 0) {
            this.ctx.gdraw.line(t_ll_x, Math.floor(y - 1), t_ur_x, Math.floor(
                y) - 1, dark_drawtype, 0, 0);
          }
          if (i !== this.rows) {
            this.ctx.gdraw.line(t_ll_x, Math.floor(y) + 1, t_ur_x, Math.floor(
                y) + 1, light_drawtype, 0, 0);
          }
        }
        y += this.row_height * this.ctx.mw.zoom_factor_y;
      }
    }

    let x = t_ll_x;
    let y = t_ll_y;

    for (let i = this.header_column; i < this.columns + 1; i++) {
      if (x > ur_x) {
        break;
      }
      if (x > ll_x) {
        this.ctx.gdraw.line(Math.floor(x), t_ll_y, Math.floor(x), t_ur_y,
            drawtype, idx, 0);
      }
      x += this.column_width[i] * this.ctx.mw.zoom_factor_x;
    }

    for (let i = 0; i < this.rows + 1; i++) {
      if (y > ur_y) {
        break;
      }
      if (y > ll_y) {
        this.ctx.gdraw.line(t_ll_x, Math.floor(y), t_ur_x, Math.floor(y),
            drawtype, idx, 0);
      }
      y += this.row_height * this.ctx.mw.zoom_factor_y;
    }

    // Draw text values
    let column_offs = this.header_column * this.rows;
    x = t_ll_x;
    let x_border = ll_x -
        Math.floor(this.column_width[0] * this.ctx.mw.zoom_factor_x);
    for (let i = this.header_column; i < this.columns; i++) {
      if (x > ur_x) {
        break;
      }

      if (x > x_border) {
        // Draw column i
        y = t_ll_y;
        for (let j = 0; j < this.rows; j++) {
          y += this.row_height * this.ctx.mw.zoom_factor_y;
          if (y - this.row_height * this.ctx.mw.zoom_factor_y > ur_y) {
            break;
          }

          if ((this.options & this.mTableOptions_ZeroIfHeader) !== 0) {
            // Don't draw the row if the value in the first column is zero
            if (this.cell_value[j] === null || this.cell_value[j] === "") {
              continue;
            }
          }
          if ((this.options & this.mTableOptions_ZeroIfHeaderIs0) !== 0) {
            // Don't draw the row if the value in the first column is zero
            if (this.cell_value[j] === null || this.cell_value[j] === "0") {
              continue;
            }
          }

          if (y > ll_y) {
            let offs = column_offs + j;
            if (text_idx >= 0 && this.cell_value[offs] !== null &&
                this.cell_value[offs] !== "") {
              let text_x = Math.floor(x) + text_offs;

              if (this.column_adjustment[i] === Adjustment.Right ||
                  this.column_adjustment[i] === Adjustment.Center) {
                let width, height, descent;
                let p = this.ctx.gdraw.getTextExtent(this.cell_value[offs], this.text_drawtype, text_idx, this.font);
                width = p.x;
                height = p.y;
                descent = height / 4;

                switch (this.column_adjustment[i]) {
                  case Adjustment.Left:
                    break;
                  case Adjustment.Right:
                    text_x = Math.floor(x + this.column_width[i] *
                        this.ctx.mw.zoom_factor_x) - text_offs - width;
                    break;
                  case Adjustment.Center:
                    text_x = Math.floor(x +
                        (this.column_width[i] * this.ctx.mw.zoom_factor_x -
                            width) / 2);
                    break;
                }
              }

              this.ctx.gdraw.text(text_x, Math.floor(y - 5),
                  this.cell_value[offs], this.text_drawtype,
                  this.text_color_drawtype, text_idx, highlight, this.font,
                  tsize, 0);
            }
          }
        }
      }
      x += this.column_width[i] * this.ctx.mw.zoom_factor_x;
      column_offs += this.rows;
    }

    this.ctx.gdraw.reset_clip_rectangle();

    // Draw frame
    this.ctx.gdraw.rect(o_ll_x, o_ll_y, ur_x - o_ll_x, ur_y - o_ll_y, drawtype,
        false, idx);
    if (this.input_focus !== 0) {
      this.ctx.gdraw.line(ll_x - 2, ll_y - 2, ll_x - 2, ur_y + 2,
          DrawType.DarkGray, 0, 0);
      this.ctx.gdraw.line(ll_x - 2, ll_y - 2, ur_x + 2, ll_y - 2,
          DrawType.DarkGray, 0, 0);
      this.ctx.gdraw.line(ll_x - 2, ur_y + 2, ur_x + 2, ur_y + 2,
          DrawType.LightGray, 0, 0);
      this.ctx.gdraw.line(ur_x + 2, ll_y - 2, ur_x + 2, ur_y + 2,
          DrawType.LightGray, 0, 0);
    }
  }

  getUserData() {
    return this.userdata;
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

  event_handler(event, fx, fy) {
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
    if (event.event === Event.ButtonMotion) {
      return 0;
    }

    switch (event.event) {
      case Event.MB1Click:
        let column = -1;
        let row = -1;

        // Find out which cell
        let o_ll_x = this.ll_x + this.x_left_offs;
        let o_ll_y = this.ll_y + this.y_low_offs;
        let o_ur_x = this.ur_x - this.vertical_scrollbar * this.scrollbar_width;
        let o_ur_y = this.ur_y - this.horizontal_scrollbar * this.scrollbar_width;

        let t_ll_x = o_ll_x - this.h_value;
        let t_ll_y = o_ll_y - this.v_value;

        if (o_ll_y <= fy && fy <= o_ur_y) {
          if (this.header_column !== 0 && this.ll_x <= fx &&
              fx <= this.ll_x + this.x_left_offs) {
            column = 0;
          } else if (o_ll_x <= fx && fx <= o_ur_x) {
            let column_x_right = t_ll_x;
            for (let i = this.header_column; i < this.columns; i++) {
              column_x_right += this.column_width[i];
              if (fx <= column_x_right) {
                column = i;
                break;
              }
            }
          }
          let row_y_high = t_ll_y;
          for (let i = 0; i < this.rows; i++) {
            row_y_high += this.row_height;
            if (fy <= row_y_high) {
              row = i;
              break;
            }
          }
        }

        if (row !== -1 && column !== -1) {
          this.ctx.send_table_callback(this, event.event, fx, fy, column, row);
        }
        break;
      default:
    }

    return sts;
  }

  getTableInfo() {
    let info = new GlowTableInfo();

    info.columns = this.columns;
    info.rows = this.rows;
    for (let i = 0; i < TABLE_MAX_COL; i++) {
      info.column_size[i] = this.column_size[i];
    }
    return info;
  }

  setTableInfo(info) {
    this.columns = info.columns;
    this.rows = info.rows;
    for (let i = 0; i < TABLE_MAX_COL; i++) {
      this.column_size[i] = info.column_size[i];
    }
  }

  setValue(value, col, row) {
    this.cell_value[col * this.rows + row] = value;
  }

  setSelectedCell(column, row) {
    if (this.selected_cell_column === column && this.selected_cell_row === row) {
      return;
    }

    if (column >= this.columns || row >= this.rows) {
      return;
    }

    this.selected_cell_column = column;
    this.selected_cell_row = row;
    this.draw();
  }

  getSelectedCellRow() {
    return this.selected_cell_row;
  }

  getSelectedCellColumn() {
    if (this.selected_cell_row === -1) {
      return -1;
    }
    return this.selected_cell_column;
  }
}