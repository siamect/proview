class GrowFolder extends GrowWindow {
  static MAX_FOLDERS = 12;
  folders;
  text_size;
  text_drawtype;
  text_color_drawtype;
  header_height;
  folder_file_names = new Array(GrowFolder.MAX_FOLDERS);
  folder_text = new Array(GrowFolder.MAX_FOLDERS);
  folder_scale = new Array(GrowFolder.MAX_FOLDERS);
  folder_v_scrollbar = new Array(GrowFolder.MAX_FOLDERS);
  folder_h_scrollbar = new Array(GrowFolder.MAX_FOLDERS);
  folder_owner = new Array(GrowFolder.MAX_FOLDERS);
  current_folder = 0;
  color_selected;
  color_unselected;

  constructor(ctx) {
    super(ctx);
    for (let i = 0; i < GrowFolder.MAX_FOLDERS; i++) {
      this.folder_text[i] = null;
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);

      switch (key) {
        case GlowSave.GrowFolder:
          break;
        case GlowSave.GrowFolder_folders:
          this.folders = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_text_drawtype:
          this.text_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_text_color_drawtype:
          this.text_color_drawtype = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_header_height:
          this.header_height = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_color_selected:
          this.color_selected = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_color_unselected:
          this.color_unselected = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names1:
          if (tokens.length > 1) {
            this.folder_file_names[0] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text1:
          if (tokens.length > 1) {
            this.folder_text[0] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale1:
          this.folder_scale[0] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar1:
          this.folder_v_scrollbar[0] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar1:
          this.folder_h_scrollbar[0] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names2:
          if (tokens.length > 1) {
            this.folder_file_names[1] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text2:
          if (tokens.length > 1) {
            this.folder_text[1] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale2:
          this.folder_scale[1] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar2:
          this.folder_v_scrollbar[1] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar2:
          this.folder_h_scrollbar[1] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names3:
          if (tokens.length > 1) {
            this.folder_file_names[2] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text3:
          if (tokens.length > 1) {
            this.folder_text[2] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale3:
          this.folder_scale[2] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar3:
          this.folder_v_scrollbar[2] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar3:
          this.folder_h_scrollbar[2] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names4:
          if (tokens.length > 1) {
            this.folder_file_names[3] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text4:
          if (tokens.length > 1) {
            this.folder_text[3] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale4:
          this.folder_scale[3] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar4:
          this.folder_v_scrollbar[3] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar4:
          this.folder_h_scrollbar[3] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names5:
          if (tokens.length > 1) {
            this.folder_file_names[4] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text5:
          if (tokens.length > 1) {
            this.folder_text[4] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale5:
          this.folder_scale[4] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar5:
          this.folder_v_scrollbar[4] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar5:
          this.folder_h_scrollbar[4] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names6:
          if (tokens.length > 1) {
            this.folder_file_names[5] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text6:
          if (tokens.length > 1) {
            this.folder_text[5] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale6:
          this.folder_scale[5] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar6:
          this.folder_v_scrollbar[5] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar6:
          this.folder_h_scrollbar[5] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names7:
          if (tokens.length > 1) {
            this.folder_file_names[6] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text7:
          if (tokens.length > 1) {
            this.folder_text[6] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale7:
          this.folder_scale[6] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar7:
          this.folder_v_scrollbar[6] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar7:
          this.folder_h_scrollbar[6] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names8:
          if (tokens.length > 1) {
            this.folder_file_names[7] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text8:
          if (tokens.length > 1) {
            this.folder_text[7] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale8:
          this.folder_scale[7] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar8:
          this.folder_v_scrollbar[7] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar8:
          this.folder_h_scrollbar[7] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names9:
          if (tokens.length > 1) {
            this.folder_file_names[8] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text9:
          if (tokens.length > 1) {
            this.folder_text[8] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale9:
          this.folder_scale[8] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar9:
          this.folder_v_scrollbar[8] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar9:
          this.folder_h_scrollbar[8] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names10:
          if (tokens.length > 1) {
            this.folder_file_names[9] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text10:
          if (tokens.length > 1) {
            this.folder_text[9] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale10:
          this.folder_scale[9] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar10:
          this.folder_v_scrollbar[9] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar10:
          this.folder_h_scrollbar[9] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names11:
          if (tokens.length > 1) {
            this.folder_file_names[10] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text11:
          if (tokens.length > 1) {
            this.folder_text[10] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale11:
          this.folder_scale[10] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar11:
          this.folder_v_scrollbar[10] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar11:
          this.folder_h_scrollbar[10] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_file_names12:
          if (tokens.length > 1) {
            this.folder_file_names[11] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_text12:
          if (tokens.length > 1) {
            this.folder_text[11] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_scale12:
          this.folder_scale[11] = parseFloat(tokens[1]);
          break;
        case GlowSave.GrowFolder_folder_v_scrollbar12:
          this.folder_v_scrollbar[11] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_h_scrollbar12:
          this.folder_h_scrollbar[11] = parseInt(tokens[1], 10);
          break;
        case GlowSave.GrowFolder_folder_owner1:
          if (tokens.length > 1) {
            this.folder_owner[0] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_owner2:
          if (tokens.length > 1) {
            this.folder_owner[1] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_owner3:
          if (tokens.length > 1) {
            this.folder_owner[2] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_owner4:
          if (tokens.length > 1) {
            this.folder_owner[3] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_owner5:
          if (tokens.length > 1) {
            this.folder_owner[4] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_owner6:
          if (tokens.length > 1) {
            this.folder_owner[5] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_owner7:
          if (tokens.length > 1) {
            this.folder_owner[6] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_owner8:
          if (tokens.length > 1) {
            this.folder_owner[7] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_owner9:
          if (tokens.length > 1) {
            this.folder_owner[8] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_owner10:
          if (tokens.length > 1) {
            this.folder_owner[9] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_owner11:
          if (tokens.length > 1) {
            this.folder_owner[10] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_folder_owner12:
          if (tokens.length > 1) {
            this.folder_owner[11] = tokens[1];
          }
          break;
        case GlowSave.GrowFolder_window_part:
          i = super.open(lines, i + 1);
          break;
        case GlowSave.End:
          this.y_low_offs = this.header_height;
          this.input_file_name = this.folder_file_names[0];
          this.window_scale = this.folder_scale[0];
          this.vertical_scrollbar = this.folder_v_scrollbar[0];
          this.horizontal_scrollbar = this.folder_h_scrollbar[0];
          this.owner = this.folder_owner[0];
          this.update_attributes();
          return i;
        default:
          console.log("Syntax error in GrowFolder");
          break;
      }
    }

    this.y_low_offs = this.header_height;
    this.input_file_name = this.folder_file_names[0];
    this.window_scale = this.folder_scale[0];
    this.vertical_scrollbar = this.folder_v_scrollbar[0];
    this.horizontal_scrollbar = this.folder_h_scrollbar[0];
    this.owner = this.folder_owner[0];
    this.update_attributes();

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

    let text_idx = Math.floor(this.ctx.mw.zoom_factor_y /
        this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 4);
    let tsize = this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        (8 + 2 * this.text_size);
    text_idx = Math.min(text_idx, DRAW_TYPE_SIZE - 1);

    let tmp = Matrix.multiply(t, this.trf);
    let d1 = tmp.apply(this.ll);
    let d2 = tmp.apply(this.ur);
    d1.x = Math.min(d1.x, d2.x);
    d2.x = Math.max(d1.x, d2.x);
    d1.y = Math.min(d1.y, d2.y);
    d2.y = Math.max(d1.y, d2.y);

    let ll_x = Math.floor(d1.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let ur_x = Math.floor(d2.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    let ur_y = Math.floor((d1.y + this.y_low_offs) * this.ctx.mw.zoom_factor_y) -
        this.ctx.mw.offset_y;
    let ll_y = Math.floor(d1.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    let drawtype =
        GlowColor.get_drawtype(this.draw_type, DrawType.LineHighlight,
            highlight, colornode, 0, 0);
    let drawtype_light = GlowColor.shift_drawtype(this.color_unselected, -2,
        colornode);
    let drawtype_dark = GlowColor.shift_drawtype(this.color_unselected, 2,
        colornode);
    let x;
    let width = (ur_x - ll_x) / this.folders;
    let h = ur_y - ll_y;
    let p = new Array(4);
    for (let j = 0; j < 4; j++) {
      p[j] = new Point();
    }
    for (let i = this.folders - 1; i >= -1; i--) {
      if (i === this.current_folder) { // Draw this last
        continue;
      }

      if (i === -1) {
        i = this.current_folder;
      }

      x = ll_x + i * width;
      if (i === 0) {
        p[0].x = ll_x;
      } else {
        p[0].x = x - h / 4;
      }
      p[0].y = ll_y + h;
      p[1].x = x + h / 4;
      p[1].y = ll_y;
      p[2].x = x + width - h / 4;
      p[2].y = ll_y;
      if (i === this.folders - 1) {
        p[3].x = ur_x;
      } else {
        p[3].x = x + width + h / 4;
      }
      p[3].y = ll_y + h;

      if (i === this.current_folder) {
        this.ctx.gdraw.polyline(p, 4, this.color_selected, true, 0);
      } else {
        this.ctx.gdraw.polyline(p, 4, this.color_unselected, true, 0);
        if (this.shadow !== 0) {
          this.ctx.gdraw.line(p[0].x + 1, p[0].y, p[1].x + 1, p[1].y,
              drawtype_light, 0, 0);
          if (i !== 0) {
            let ps = [
              new Point(x + h / 4, ll_y + h),
              new Point(x, ll_y + h / 2),
              new Point(x + h / 8, ll_y + h / 4),
              new Point(x + h / 2, ll_y + h)];

            this.ctx.gdraw.polyline(ps, 4, drawtype_dark, true, 0);
          }
        }
      }

      if (this.shadow !== 0) {
        if (i === this.current_folder) {
          drawtype_light =
              GlowColor.shift_drawtype(this.color_selected, -2, colornode);
          this.ctx.gdraw.line(p[0].x + 1, p[0].y, p[1].x + 1, p[1].y,
              drawtype_light, 0, 0);
        }
        this.ctx.gdraw.line(p[1].x, p[1].y + 1, p[2].x, p[2].y + 1,
            drawtype_light, 0, 0);
      }
      this.ctx.gdraw.polyline(p, 4, drawtype, false, idx);

      if (text_idx >= 0 && this.folder_text[i] !== null) {
        this.ctx.gdraw.text(x + h / 2, ll_y + h - 2, this.folder_text[i],
            this.text_drawtype, this.text_color_drawtype, text_idx, highlight, Font.Helvetica, tsize, 0);
      }
      if (i === this.current_folder) {
        break;
      }
    }
    super.draw(t, highlight, hot, node, colornode);
    this.ctx.gdraw.line(p[0].x + 1, p[0].y, p[3].x - 1, p[3].y,
        this.color_selected, idx, 0);
  }

  event_handler(event, fx, fy) {
    let sts;

    if (event.event === Event.MB1Click) {
      if (this.ll_x <= fx && fx <= this.ur_x && this.ll_y <= fy &&
          fy <= this.ll_y + this.header_height) {
        let w = (this.ur_x - this.ll_x) / this.folders;
        let x = this.ll_x + w;
        for (let i = 0; i < this.folders; i++) {
          if (fx <= x) {
            // Change file
            this.input_file_name = this.folder_file_names[i];
            this.window_scale = this.folder_scale[i];
            this.vertical_scrollbar = this.folder_v_scrollbar[i];
            this.horizontal_scrollbar = this.folder_h_scrollbar[i];
            this.owner = this.folder_owner[i];
            this.current_folder = i;

            this.update_attributes();
            this.draw();
            break;
          }
          x += w;
        }
        return 1;
      }
    }

    let e = new GlowEvent();
    e.event = event.event;
    e.type = event.type;

    e.x = event.x;
    e.y = event.y - this.header_height;
    e.object = event.object;
    return super.event_handler(e, fx, fy);
  }
}