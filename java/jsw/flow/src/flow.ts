"use strict";

enum ConType {
  Straight,
  Fixed,
  AllFixed,
  Routed,
  StepDiv,
  StepConv,
  TransDiv,
  TransConv,
  StraightOneArrow,
  Reference
}

enum DrawType {
  Line,
  LineRed,
  LineGray,
  LineErase,
  LineDashed,
  LineDashedRed,
  TextHelvetica,
  TextHelveticaBold,
  TextHelveticaErase,
  TextHelveticaEraseBold,
  _,
  Green,
  Yellow,
  DarkGray,
  Inherit = 9999
}

enum DisplayLevel {
  One = 1 << 0,
  Two = 1 << 2,
  Three = 1 << 3,
  Four = 1 << 4,
  Five = 1 << 5,
  Six = 1 << 6
}

enum NodeGroup {
  Common,
  Document,
  Trace
}

enum TraceType {
  Boolean,
  Int32,
  Float32,
  User
}

enum Save {
  Ctx = 1,
  Array = 2,
  NodeClass = 3,
  ConClass = 4,
  Rect = 5,
  Line = 6,
  Point = 7,
  Arc = 8,
  Text = 9,
  Node = 10,
  Con = 11,
  ConPoint = 12,
  Annot = 13,
  Arrow = 14,
  Pixmap = 15,
  AnnotPixmap = 16,
  Radiobutton = 17,
  Frame = 18,
  AnnotPixmapButton = 19,
  Triangle = 20,
  End = 99,
  Ctx_zoom_factor = 100,
  Ctx_base_zoom_factor = 101,
  Ctx_offset_x = 102,
  Ctx_offset_y = 103,
  Ctx_nav_zoom_factor = 104,
  Ctx_print_zoom_factor = 105,
  Ctx_nav_offset_x = 106,
  Ctx_nav_offset_y = 107,
  Ctx_x_right = 108,
  Ctx_x_left = 109,
  Ctx_y_high = 110,
  Ctx_y_low = 111,
  Ctx_window_width = 112,
  Ctx_window_height = 113,
  Ctx_nav_window_width = 114,
  Ctx_nav_window_height = 115,
  Ctx_nav_rect_ll_x = 116,
  Ctx_nav_rect_ll_y = 117,
  Ctx_nav_rect_ur_x = 118,
  Ctx_nav_rect_ur_y = 119,
  Ctx_nav_rect_hot = 120,
  Ctx_name = 121,
  Ctx_user_highlight = 122,
  Ctx_a_nc = 123,
  Ctx_a_cc = 124,
  Ctx_a = 125,
  Ctx_grid_size_x = 126,
  Ctx_grid_size_y = 127,
  Ctx_grid_on = 128,
  Ctx_draw_delta = 129,
  Ctx_refcon_width = 130,
  Ctx_refcon_height = 131,
  Ctx_refcon_textsize = 132,
  Ctx_refcon_linewidth = 133,
  Ctx_user_version = 134,
  Array_a = 200,
  NodeClass_nc_name = 300,
  NodeClass_a = 301,
  NodeClass_group = 302,
  NodeClass_no_con_obstacle = 303,
  ConClass_cc_name = 400,
  ConClass_con_type = 401,
  ConClass_corner = 402,
  ConClass_draw_type = 403,
  ConClass_line_width = 404,
  ConClass_arrow_width = 405,
  ConClass_arrow_length = 406,
  ConClass_round_corner_amount = 407,
  ConClass_group = 408,
  Rect_draw_type = 500,
  Rect_line_width = 501,
  Rect_ll = 502,
  Rect_ur = 503,
  Rect_display_level = 504,
  Rect_fill = 505,
  Line_draw_type = 600,
  Line_line_width = 601,
  Line_p1 = 602,
  Line_p2 = 603,
  Point_x = 700,
  Point_y = 701,
  Arc_angle1 = 800,
  Arc_angle2 = 801,
  Arc_draw_type = 802,
  Arc_line_width = 803,
  Arc_ll = 804,
  Arc_ur = 805,
  Text_text_size = 900,
  Text_draw_type = 901,
  Text_text = 902,
  Text_p = 903,
  Node_nc = 1000,
  Node_pos = 1001,
  Node_n_name = 1002,
  Node_annotsize = 1003,
  Node_annotv = 1004,
  Node_refcon_cnt = 1005,
  Node_x_right = 1006,
  Node_x_left = 1007,
  Node_y_high = 1008,
  Node_y_low = 1009,
  Node_trace_object = 1010,
  Node_trace_attribute = 1011,
  Node_trace_attr_type = 1012,
  Node_obst_x_right = 1013,
  Node_obst_x_left = 1014,
  Node_obst_y_high = 1015,
  Node_obst_y_low = 1016,
  Node_trace_inverted = 1017,
  Con_x_right = 1100,
  Con_x_left = 1101,
  Con_y_high = 1102,
  Con_y_low = 1103,
  Con_cc = 1104,
  Con_dest_node = 1105,
  Con_source_node = 1106,
  Con_dest_conpoint = 1107,
  Con_source_conpoint = 1108,
  Con_dest_direction = 1109,
  Con_source_direction = 1110,
  Con_line_a = 1111,
  Con_arc_a = 1112,
  Con_arrow_a = 1113,
  Con_ref_a = 1114,
  Con_p_num = 1115,
  Con_l_num = 1116,
  Con_a_num = 1117,
  Con_arrow_num = 1118,
  Con_ref_num = 1119,
  Con_point_x = 1120,
  Con_point_y = 1121,
  Con_source_ref_cnt = 1122,
  Con_dest_ref_cnt = 1123,
  Con_c_name = 1124,
  Con_trace_object = 1125,
  Con_trace_attribute = 1126,
  Con_trace_attr_type = 1127,
  Con_temporary_ref = 1128,
  ConPoint_number = 1200,
  ConPoint_direction = 1201,
  ConPoint_p = 1202,
  ConPoint_trace_attribute = 1203,
  ConPoint_trace_attr_type = 1204,
  Annot_number = 1300,
  Annot_draw_type = 1301,
  Annot_text_size = 1302,
  Annot_p = 1303,
  Annot_annot_type = 1304,
  Annot_display_level = 1305,
  Arrow_arrow_width = 1400,
  Arrow_arrow_length = 1401,
  Arrow_draw_type = 1402,
  Arrow_line_width = 1403,
  Arrow_p_dest = 1404,
  Arrow_p1 = 1405,
  Arrow_p2 = 1406,
  Triangle_rect_part = 2000
}

class GDraw {
  ctx: FlowCtx;
  canvas: HTMLCanvasElement;
  gctx: CanvasRenderingContext2D;
  offset_top: number;
  offset_left: number;

  constructor(ctx: FlowCtx) {
    this.ctx = ctx;
    this.canvas = document.querySelector("canvas");
    this.gctx = this.canvas.getContext("2d");
    this.offset_top = this.canvas.offsetTop;
    this.offset_left = this.canvas.offsetTop;
    console.log("offset_top", this.offset_top, "offset_left", this.offset_left);
  }

  rect(x, y, width, height) {
    this.gctx.strokeRect(x, y, width, height);
  }

  line(x1, y1, x2, y2) {
    this.gctx.beginPath();
    this.gctx.moveTo(x1, y1);
    this.gctx.lineTo(x2, y2);
    this.gctx.stroke();
  }
}

class FlowArray {
  ctx: FlowCtx;
  a = [];

  constructor(ctx: FlowCtx) {
    this.ctx = ctx;
  }

  add(elem) {
    this.a.push(elem);
  }

  size() {
    return this.a.length;
  }

  get(idx) {
    return this.a[idx];
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Array:
          break;
        case Save.NodeClass:
          let nc = new FlowNodeClass(this.ctx);
          i = nc.open(lines, i + 1);
          this.a.push(nc);
          break;
        case Save.ConClass:
          let cc = new FlowConClass(this.ctx);
          i = cc.open(lines, i + 1);
          this.a.push(cc);
          break;
        case Save.Node:
          let n = new FlowNode(this.ctx);
          i = n.open(lines, i + 1);
          this.a.push(n);
          break;
        case Save.Con:
          let c = new FlowCon(this.ctx);
          i = c.open(lines, i + 1);
          this.a.push(c);
          break;
        case Save.Rect:
          let rect = new FlowRect(this.ctx);
          i = rect.open(lines, i + 1);
          this.a.push(rect);
          break;
        case Save.Line:
          let l = new FlowLine(this.ctx);
          i = l.open(lines, i + 1);
          this.a.push(l);
          break;
        case Save.Triangle:
          let t = new FlowTriangle(this.ctx);
          i = t.open(lines, i + 1);
          this.a.push(t);
          break;
        case Save.Arc:
          let arc = new FlowArc(this.ctx);
          i = arc.open(lines, i + 1);
          this.a.push(arc);
          break;
        case Save.Text:
          let text = new FlowText(this.ctx);
          i = text.open(lines, i + 1);
          this.a.push(text);
          break;
        case Save.ConPoint:
          let cp = new FlowConPoint(this.ctx);
          i = cp.open(lines, i + 1);
          this.a.push(cp);
          break;
        case Save.Annot:
          let annot = new FlowAnnot(this.ctx);
          i = annot.open(lines, i + 1);
          this.a.push(annot);
          break;
        case Save.Arrow:
          let arrow = new FlowArrow(this.ctx);
          i = arrow.open(lines, i + 1);
          this.a.push(arrow);
          break;
        case Save.Point:
          let point = new FlowPoint();
          i = point.open(lines, i + 1);
          this.a.push(point);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowArray", key);
          break;
      }
    }

    return i;
  }

  draw(g, p, node, highlight) {
    this.a.forEach(e => e.draw(g, p, node, highlight));
  }

  search_by_name(name) {
    let uname = name.toUpperCase();
    return this.a.find(function (e) {
      if (e instanceof FlowNode) {
        if (e.n_name.toUpperCase() === uname) {
          return true;
        }
      }
    });
  }

  forEach(callback) {
    this.a.forEach(callback);
  }
}

class FlowNodeClass {
  ctx: FlowCtx;
  a: FlowArray;
  nc_name = "";
  group = 0;

  constructor(ctx: FlowCtx) {
    this.a = new FlowArray(ctx);
    this.ctx = ctx;
  }

  draw(g, p, node, highlight) {
    this.a.draw(g, p, node, highlight);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);
      switch (key) {
        case Save.NodeClass_nc_name:
          this.nc_name = tokens[1];
          break;
        case Save.NodeClass_a:
          i = this.a.open(lines, i + 1);
          break;
        case Save.NodeClass_group:
          this.group = parseInt(tokens[1]);
          break;
        case Save.NodeClass_no_con_obstacle:
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowNodeClass");
          break;
      }
    }

    return i;
  }

  event_handler(x, y) {
    return 0;
  }
}

class FlowConClass {
  ctx: FlowCtx;
  cc_name = "";
  con_type = 0;
  corner = 0;
  draw_type = 0;
  line_width = 0;
  arrow_width = 0.0;
  arrow_length = 0.0;
  round_corner_amount = 0.0;
  group = 0;

  constructor(ctx: FlowCtx) {
    this.ctx = ctx;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.ConClass:
          break;
        case Save.ConClass_cc_name:
          this.cc_name = tokens[1];
          break;
        case Save.ConClass_con_type:
          this.con_type = parseInt(tokens[1]);
          break;
        case Save.ConClass_corner:
          this.corner = parseInt(tokens[1]);
          break;
        case Save.ConClass_draw_type:
          this.draw_type = parseInt(tokens[1]);
          break;
        case Save.ConClass_line_width:
          this.line_width = parseInt(tokens[1]);
          break;
        case Save.ConClass_arrow_width:
          this.arrow_width = parseFloat(tokens[1]);
          break;
        case Save.ConClass_arrow_length:
          this.arrow_length = parseFloat(tokens[1]);
          break;
        case Save.ConClass_round_corner_amount:
          this.round_corner_amount = parseFloat(tokens[1]);
          break;
        case Save.ConClass_group:
          this.group = parseInt(tokens[1]);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowConClass");
          break;
      }
    }

    return i;
  }
}

class FlowPoint {
  x = 0.0;
  y = 0.0;

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Point:
          break;
        case Save.Point_x:
          this.x = parseFloat(tokens[1]);
          break;
        case Save.Point_y:
          this.y = parseFloat(tokens[1]);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowPoint");
          break;
      }
    }

    return i;
  }
}

class FlowLine {
  ctx: FlowCtx;
  p1 = new FlowPoint();
  p2 = new FlowPoint();
  draw_type: DrawType = 0;
  line_width = 0;

  constructor(ctx: FlowCtx) {
    this.ctx = ctx;
  }

  draw(g, p, node, highlight) {
    let x1 = (this.p1.x + p.x) * this.ctx.zoom_factor;
    let y1 = (this.p1.y + p.y) * this.ctx.zoom_factor;
    let x2 = (this.p2.x + p.x) * this.ctx.zoom_factor;
    let y2 = (this.p2.y + p.y) * this.ctx.zoom_factor;

    g.lineWidth =
        this.ctx.zoom_factor / this.ctx.base_zoom_factor * this.line_width;
    if (g.lineWidth < 1) {
      g.lineWidth = 1;
    }

    if (this.draw_type == DrawType.LineDashed || this.draw_type == DrawType.LineDashedRed) {
      g.setLineDash([8, 8]);
    }

    switch (this.draw_type) {
      case DrawType.LineGray:
        g.strokeStyle = "lightgray";
        break;
      case DrawType.LineRed:
      case DrawType.LineDashedRed:
        g.strokeStyle = "red";
        break;
      case DrawType.LineErase:
        return;
      default:
        g.strokeStyle = "black";
    }
    if (highlight) {
      g.strokeStyle = "red";
    }
    g.beginPath();
    g.moveTo(x1, y1);
    g.lineTo(x2, y2);
    g.stroke();

    if (this.draw_type == DrawType.LineDashed || this.draw_type == DrawType.LineDashedRed) {
      g.setLineDash([]);
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Line:
          break;
        case Save.Line_draw_type:
          this.draw_type = parseInt(tokens[1]);
          break;
        case Save.Line_line_width:
          this.line_width = parseInt(tokens[1]);
          break;
        case Save.Line_p1:
          i = this.p1.open(lines, i + 1);
          break;
        case Save.Line_p2:
          i = this.p2.open(lines, i + 1);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowLine");
          break;
      }
    }

    return i;
  }
}

class FlowRect {
  ctx: FlowCtx;
  ll = new FlowPoint();
  ur = new FlowPoint();
  draw_type: DrawType = 0;
  line_width = 0;
  display_level: DisplayLevel = 0;
  fill = 0;

  constructor(ctx: FlowCtx) {
    this.ctx = ctx;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Rect:
          break;
        case Save.Rect_draw_type:
          this.draw_type = parseInt(tokens[1]);
          break;
        case Save.Rect_line_width:
          this.line_width = parseInt(tokens[1]);
          break;
        case Save.Rect_display_level:
          this.display_level = parseInt(tokens[1]);
          break;
        case Save.Rect_fill:
          this.fill = parseInt(tokens[1]);
          break;
        case Save.Rect_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case Save.Rect_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowRect");
          break;
      }
    }

    return i;
  }

  draw(g, p, node, highlight) {
    if ((this.display_level & this.ctx.display_level) === 0) {
      return;
    }

    let x = (this.ll.x + p.x) * this.ctx.zoom_factor;
    let y = (this.ll.y + p.y) * this.ctx.zoom_factor;
    let width = (this.ur.x - this.ll.x) * this.ctx.zoom_factor;
    let height = (this.ur.y - this.ll.y) * this.ctx.zoom_factor;

    g.lineWidth =
        this.ctx.zoom_factor / this.ctx.base_zoom_factor * this.line_width;
    if (g.lineWidth < 1) {
      g.lineWidth = 1;
    }
    switch (this.draw_type) {
      case DrawType.LineGray:
        g.strokeStyle = "grey";
        break;
      case DrawType.LineRed:
      case DrawType.LineDashedRed:
        g.strokeStyle = "red";
        break;
      case DrawType.LineErase:
        return;
      default:
        g.strokeStyle = "black";
    }
    if (highlight) {
      g.strokeStyle = "red";
    }
    g.strokeRect(x, y, width, height);
  }
}

class FlowArc {
  ctx: FlowCtx;
  ll = new FlowPoint();
  ur = new FlowPoint();
  angle1 = 0.0;
  angle2 = 0.0;
  draw_type: DrawType = 0;
  line_width = 0;

  constructor(ctx: FlowCtx) {
    this.ctx = ctx;
  }

  draw(g, p, node, highlight) {
    let r = (this.ur.x - this.ll.x) / 2 * this.ctx.zoom_factor;
    let x = (this.ll.x + p.x) * this.ctx.zoom_factor + r;
    let y = (this.ll.y + p.y) * this.ctx.zoom_factor + r;
    let a1 = 0;
    if (this.angle1 === 90 || this.angle1 === 270) {
      a1 = (this.angle1 + 90) / 180 * Math.PI;
    } else {
      a1 = (this.angle1 - 90) / 180 * Math.PI;
    }
    let a2 = a1 + this.angle2 / 180 * Math.PI;
    g.lineWidth = Math.max(this.ctx.zoom_factor / this.ctx.base_zoom_factor * this.line_width, 1);

    g.strokeStyle = highlight ? "red" : "black";

    g.beginPath();
    g.arc(x, y, r, a1, a2, false);
    g.stroke();
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Arc:
          break;
        case Save.Arc_draw_type:
          this.draw_type = parseInt(tokens[1]);
          break;
        case Save.Arc_line_width:
          this.line_width = parseInt(tokens[1]);
          break;
        case Save.Arc_angle1:
          this.angle1 = parseFloat(tokens[1]);
          break;
        case Save.Arc_angle2:
          this.angle2 = parseFloat(tokens[1]);
          break;
        case Save.Arc_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case Save.Arc_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowArc");
          break;
      }
    }

    return i;
  }
}

class FlowText {
  ctx: FlowCtx;
  p = new FlowPoint();
  draw_type: DrawType = 0;
  text_size = 0;
  text = "";

  constructor(ctx: FlowCtx) {
    this.ctx = ctx;
  }

  draw(g, p0, node, highlight) {
    let idx = this.ctx.zoom_factor / this.ctx.base_zoom_factor *
        (this.text_size + 4) - 4;
    if (idx < 0) {
      return;
    }

    let x = (this.p.x + p0.x) * this.ctx.zoom_factor;
    let y = (this.p.y + p0.y) * this.ctx.zoom_factor;

    let tsize = (idx < 0 || idx > 7) ? (idx * 3) : ([8, 10, 12, 14, 14, 18, 18, 18][idx]);
    g.font = tsize + "px Arial";
    g.lineWidth = 0.5;

    g.fillStyle = highlight ? "red" : "black";
    g.fillText(this.text, x, y);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Text:
          break;
        case Save.Text_text_size:
          this.text_size = parseInt(tokens[1]);
          break;
        case Save.Text_draw_type:
          this.draw_type = parseInt(tokens[1]);
          break;
        case Save.Text_text:
          this.text = tokens[1];
          break;
        case Save.Text_p:
          i = this.p.open(lines, i + 1);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowText");
          break;
      }
    }

    return i;
  }
}

class FlowArrow {
  ctx: FlowCtx;
  p1 = new FlowPoint();
  p2 = new FlowPoint();
  p_dest = new FlowPoint();
  arrow_width = 0.0;
  arrow_length = 0.0;
  draw_type: DrawType = 0;
  line_width = 0;

  constructor(ctx: FlowCtx) {
    this.ctx = ctx;
  }

  draw(g, p, node, highlight) {
    let x1 = (this.p1.x + p.x) * this.ctx.zoom_factor;
    let x2 = (this.p2.x + p.x) * this.ctx.zoom_factor;
    let x3 = (this.p_dest.x + p.x) * this.ctx.zoom_factor;
    let y1 = (this.p1.y + p.y) * this.ctx.zoom_factor;
    let y2 = (this.p2.y + p.y) * this.ctx.zoom_factor;
    let y3 = (this.p_dest.y + p.y) * this.ctx.zoom_factor;

    switch (this.draw_type) {
      case DrawType.LineGray:
        g.fillStyle = "lightgray";
        break;
      case DrawType.LineRed:
      case DrawType.LineDashedRed:
        g.fillStyle = "red";
        break;
      case DrawType.LineErase:
        return;
      default:
        g.fillStyle = "black";
    }
    if (highlight) {
      g.fillStyle = "red";
    }
    g.beginPath();
    g.moveTo(x1, y1);
    g.lineTo(x2, y2);
    g.lineTo(x3, y3);
    g.lineTo(x1, y1);
    g.fill();
  }

  open(lines, row) {
    for (let i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Arrow:
          break;
        case Save.Arrow_arrow_width:
          this.arrow_width = parseFloat(tokens[1]);
          break;
        case Save.Arrow_arrow_length:
          this.arrow_length = parseFloat(tokens[1]);
          break;
        case Save.Arrow_draw_type:
          this.draw_type = parseInt(tokens[1]);
          break;
        case Save.Arrow_line_width:
          this.line_width = parseInt(tokens[1]);
          break;
        case Save.Arrow_p_dest:
          i = this.p_dest.open(lines, i + 1);
          break;
        case Save.Arrow_p1:
          i = this.p1.open(lines, i + 1);
          break;
        case Save.Arrow_p2:
          i = this.p2.open(lines, i + 1);
          break;
        case Save.End:
          return;
        default:
          console.log("Syntax error in FlowArrow");
          break;
      }
    }
  }
}

class FlowTriangle extends FlowRect {
  ctx: FlowCtx;

  constructor(ctx: FlowCtx) {
    super(ctx);
  }

  draw(g, p, node, highlight) {
    let x1 = (this.ll.x + p.x) * this.ctx.zoom_factor;
    let y1 = (this.ll.y + p.y) * this.ctx.zoom_factor;
    let x2 = (this.ur.x + p.x) * this.ctx.zoom_factor;
    let y2 = (this.ur.y + p.y) * this.ctx.zoom_factor;

    g.lineWidth = this.ctx.zoom_factor / this.ctx.base_zoom_factor * this.line_width;
    if (g.lineWidth < 1) {
      g.lineWidth = 1;
    }

    let dtype = this.draw_type;
    if (dtype === DrawType.Inherit && node !== null) {
      dtype = node.fill_color;
    }

    if (this.fill === 1) {
      switch (dtype) {
        case DrawType.LineRed:
          g.fillStyle = "red";
          break;
        case DrawType.Green:
          g.fillStyle = "green";
          break;
        case DrawType.Yellow:
          g.fillStyle = "yellow";
          break;
        default:
          g.fillStyle = "gray";
      }

      g.beginPath();
      g.moveTo(x1, y2);
      g.lineTo((x1 + x2) / 2, y1);
      g.lineTo(x2, y2);
      g.lineTo(x1, y2);
      g.fill();
    } else {
      g.strokeStyle = "black";
      g.beginPath();
      g.moveTo(x1, y2);
      g.lineTo((x1 + x2) / 2, y1);
      g.lineTo(x2, y2);
      g.lineTo(x1, y2);
      g.stroke();
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Triangle_rect_part:
          i = this.open(lines, i + 1);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowTriangle");
          break;
      }
    }

    return i;
  }
}

class FlowConPoint {
  ctx: FlowCtx;
  p = new FlowPoint();
  number = 0;
  direction = 0;
  trace_attribute = "";
  trace_attr_type = 0;

  constructor(ctx: FlowCtx) {
    this.ctx = ctx;
  }

  draw(g, p, node, highlight) {
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.ConPoint:
          break;
        case Save.ConPoint_number:
          this.number = parseInt(tokens[1]);
          break;
        case Save.ConPoint_direction:
          this.direction = parseInt(tokens[1]);
          break;
        case Save.ConPoint_p:
          i = this.p.open(lines, i + 1);
          break;
        case Save.ConPoint_trace_attribute:
          this.trace_attribute = tokens[1];
          break;
        case Save.ConPoint_trace_attr_type:
          this.trace_attr_type = parseInt(tokens[1]);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowConPoint");
          break;
      }
    }

    return i;
  }
}

class FlowAnnot {
  ctx: FlowCtx;
  p = new FlowPoint();
  draw_type: DrawType = 0;
  text_size = 0;
  display_level: DisplayLevel = 0;
  annot_type = 0;
  number = 0;

  constructor(ctx: FlowCtx) {
    this.ctx = ctx;
  }

  draw(g, p0, node, highlight) {
    if (node === null) {
      return;
    }
    if (node.annotv[this.number] === null) {
      return;
    }
    if ((this.display_level & this.ctx.display_level) === 0) {
      return;
    }

    let idx = this.ctx.zoom_factor / this.ctx.base_zoom_factor *
        (this.text_size + 4) - 4;
    if (idx < 0) {
      return;
    }

    let tsize = (idx < 0 || idx > 7) ? (idx * 3) : ([8, 10, 12, 14, 14, 18, 18, 18][idx]);
    g.font = tsize + "px Arial";
    if (this.draw_type == DrawType.TextHelveticaBold) {
      g.font = "bold " + g.font;
    }
    g.fillStyle = highlight ? "red" : "black";
    g.lineWidth = 0.5;

    let x = (this.p.x + p0.x) * this.ctx.zoom_factor;
    let y = (this.p.y + p0.y) * this.ctx.zoom_factor - tsize / 4;

    let tokens = node.annotv[this.number].split('\n');
    for (let i = 0; i < tokens.length; i++) {
      g.fillText(tokens[i], x, y);
      y += tsize * 1.4;
    }
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Annot:
          break;
        case Save.Annot_number:
          this.number = parseInt(tokens[1]);
          break;
        case Save.Annot_draw_type:
          this.draw_type = parseInt(tokens[1]);
          break;
        case Save.Annot_text_size:
          this.text_size = parseInt(tokens[1]);
          break;
        case Save.Annot_display_level:
          this.display_level = parseInt(tokens[1]);
          break;
        case Save.Annot_p:
          i = this.p.open(lines, i + 1);
          break;
        case Save.Annot_annot_type:
          this.annot_type = parseInt(tokens[1]);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowAnnot");
          break;
      }
    }

    return i;
  }
}

class FlowCon extends Rect {
  ctx: FlowCtx;
  point_x: FlowArray;
  point_y: FlowArray;
  line_a: FlowArray;
  arc_a: FlowArray;
  arrow_a: FlowArray;
  ref_a: FlowArray;
  cc = null;
  p_num = 0;
  l_num = 0;
  a_num = 0;
  arrow_num = 0;
  ref_num = 0;
  c_name = "";
  trace_object = "";
  trace_attribute = "";
  trace_attr_type = 0;
  temporary_ref = 0;
  highlight = false;
  redraw = true;

  constructor(ctx: FlowCtx) {
    super();
    this.ctx = ctx;
    this.point_x = new FlowArray(ctx);
    this.point_y = new FlowArray(ctx);
    this.line_a = new FlowArray(ctx);
    this.arc_a = new FlowArray(ctx);
    this.arrow_a = new FlowArray(ctx);
    this.ref_a = new FlowArray(ctx);
  }

  draw(g, p0, node, highlight) {
    if (!this.redraw) {
      return;
    }
    let p = new FlowPoint();
    if (this.temporary_ref !== 0 ||
        this.cc.con_type === ConType.Reference) {
      this.ref_a.draw(g, p, null, highlight);
    } else {
      this.line_a.draw(g, p, null, highlight);
      this.arc_a.draw(g, p, null, highlight);
      this.arrow_a.draw(g, p, null, highlight);
    }
    this.redraw = false;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Con:
          break;
        case Save.Con_cc:
          let cc_name = tokens[1];
          let found = false;
          for (let j = 0; j < this.ctx.a_cc.size(); j++) {
            if (this.ctx.a_cc.get(j).cc_name === cc_name) {
              this.cc = this.ctx.a_cc.get(j);
              found = true;
              break;
            }
          }
          if (!found) {
            console.log("FlowCon: ConClass not found: " + cc_name);
          }
          break;
        case Save.Con_c_name:
          if (tokens[1] !== null) {
            this.c_name = tokens[1];
          }
          break;
        case Save.Con_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case Save.Con_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case Save.Con_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case Save.Con_y_low:
          this.ll_y = parseFloat(tokens[1]);
          break;
        case Save.Con_dest_node:
          i++;
          break;
        case Save.Con_source_node:
          i++;
          break;
        case Save.Con_dest_conpoint:
        case Save.Con_source_conpoint:
        case Save.Con_dest_direction:
        case Save.Con_source_direction:
          break;
        case Save.Con_line_a:
          i = this.line_a.open(lines, i + 1);
          break;
        case Save.Con_arc_a:
          i = this.arc_a.open(lines, i + 1);
          break;
        case Save.Con_arrow_a:
          i = this.arrow_a.open(lines, i + 1);
          break;
        case Save.Con_ref_a:
          i = this.ref_a.open(lines, i + 1);
          break;
        case Save.Con_p_num:
          this.p_num = parseInt(tokens[1]);
          break;
        case Save.Con_l_num:
          this.l_num = parseInt(tokens[1]);
          break;
        case Save.Con_a_num:
          this.a_num = parseInt(tokens[1]);
          break;
        case Save.Con_arrow_num:
          this.arrow_num = parseInt(tokens[1]);
          break;
        case Save.Con_ref_num:
          this.ref_num = parseInt(tokens[1]);
          break;
        case Save.Con_point_x:
          for (let j = 0; j < this.p_num; j++) {
            i++;
            this.point_x[j] = parseFloat(lines[i]);
          }
          break;
        case Save.Con_point_y:
          for (let j = 0; j < this.p_num; j++) {
            i++;
            this.point_y[j] = parseFloat(lines[i]);
          }
          break;

        case Save.Con_source_ref_cnt:
        case Save.Con_dest_ref_cnt:
          break;
        case Save.Con_trace_object:
          if (tokens[1] !== null) {
            this.trace_object = tokens[1];
          }
          break;
        case Save.Con_trace_attribute:
          if (tokens[1] !== null) {
            this.trace_attribute = tokens[1];
          }
          break;
        case Save.Con_trace_attr_type:
          this.trace_attr_type = parseInt(tokens[1]);
          break;
        case Save.Con_temporary_ref:
          this.temporary_ref = parseInt(tokens[1]);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowCon");
          break;
      }
    }

    return i;
  }

  connect() {
  }

  scan() {
  }
}

class FlowNode extends Rect {
  ctx: FlowCtx;
  nc: FlowNodeClass = null;
  pos = new FlowPoint();
  n_name = "";
  annotv: Array = [];
  annotsize: Array = [];
  trace_object = "";
  trace_attribute = "";
  trace_attr_type = 0;
  highlight = false;
  select = false;
  fill_color = 0;
  p = 0;
  old_value = 0;
  first_scan = true;
  redraw = true;

  constructor(ctx: FlowCtx) {
    super();
    this.ctx = ctx;
  }

  draw(g, p, node, highlight) {
    if (!this.redraw) {
      return;
    }

    if (this.nc.group !== NodeGroup.Document) {
      let x = this.ll_x * this.ctx.zoom_factor;
      let y = this.ll_y * this.ctx.zoom_factor - 1;
      let width = this.width() * this.ctx.zoom_factor;
      let height = this.height() * this.ctx.zoom_factor + 2;
      g.fillStyle = (this.select) ? "lightblue" : "white";
      g.fillRect(x, y, width, height);
    }

    this.nc.draw(g, this.pos, this, this.highlight);
    this.redraw = false;
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Node_nc:
          let nc_name = tokens[1];
          let found = false;
          for (let j = 0; j < this.ctx.a_nc.size(); j++) {
            if (this.ctx.a_nc.get(j).nc_name === nc_name) {
              this.nc = this.ctx.a_nc.get(j);
              found = true;
              break;
            }
          }
          if (!found) {
            console.log("FlowNode: NodeClass not found: " + nc_name);
          }
          break;
        case Save.Node_n_name:
          if (tokens[1] !== null) {
            this.n_name = tokens[1];
          }
          break;
        case Save.Node_refcon_cnt:
          i += 32;
          break;
        case Save.Node_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case Save.Node_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case Save.Node_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case Save.Node_y_low:
          this.ll_y = parseFloat(tokens[1]);
          break;
        case Save.Node_annotsize:
          for (let j = 0; j < 10; j++) {
            i++;
            this.annotsize[j] = parseInt(lines[i]);
          }
          break;
        case Save.Node_annotv:
          // Annotation are surrounded by quotes. A quote inside a
          // annotation is preceeded by a backslash. The size is calculated
          // without backslashes
          for (let j = 0; j < 10; j++) {
            if (this.annotsize[j] > 0) {
              let sb = [];
              let c = '';
              let prevc;
              let end_found = false;
              let first = true;

              while (true) {
                i++;
                if (lines[i] === null) {
                  sb.push('\n');
                  continue;
                }

                for (let k = 0; k < lines[i].length; k++) {
                  prevc = c;
                  c = lines[i].charAt(k);
                  if (first) {
                    first = false;
                    continue;
                  }
                  if (c === '\"' && prevc === '\\') {
                    sb[sb.length - 1] = null;
                    continue;
                  }
                  if (c === '\"') {
                    end_found = true;
                    break;
                  }
                  sb.push(c);
                  if (k === lines[i].length - 1) {
                    sb.push('\n');
                  }
                }
                if (end_found) {
                  break;
                }
              }
              this.annotv[j] = sb.join("");
            }
          }
          break;
        case Save.Node_pos:
          i = this.pos.open(lines, i + 1);
          break;
        case Save.Node_trace_object:
          if (tokens[1] !== null) {
            this.trace_object = tokens[1];
          }
          break;
        case Save.Node_trace_attribute:
          if (tokens[1] !== null) {
            this.trace_attribute = tokens[1];
          }
          break;
        case Save.Node_trace_attr_type:
          this.trace_attr_type = parseInt(tokens[1]);
          break;
        case Save.Node_obst_x_right:
        case Save.Node_obst_x_left:
        case Save.Node_obst_y_high:
        case Save.Node_obst_y_low:
        case Save.Node_trace_inverted:
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowNode");
          break;
      }
    }

    return i;
  }

  connect() {
    if (this.trace_object === "" || this.trace_attribute === "") {
      return;
    }
    if (this.trace_attr_type === TraceType.User) {
      return;
    }

    let n = this.trace_attribute.indexOf('#');
    if (n !== -1) {
      this.trace_attribute = this.trace_attribute.substring(0, n);
    }

    let o = this.trace_object + "." + this.trace_attribute;
    let pwr_type;
    switch (this.trace_attr_type) {
      case TraceType.Boolean:
        pwr_type = Type.Boolean;
        break;
      case TraceType.Int32:
        pwr_type = Type.Int32;
        break;
      case TraceType.Float32:
        pwr_type = Type.Float32;
        break;
      case TraceType.User:
        return;
    }

    this.p = this.ctx.gdh.refObjectInfo(o, pwr_type, 1);
    console.log("connecting", o, this.p);
  }

  scan() {
    if (this.p === 0) {
      return;
    }

    let v1 = this.ctx.gdh.getObjectRefInfo(this.p);
    let evaluate = true;
    if (this.first_scan) {
      this.first_scan = false;
    } else if (v1 === this.old_value) {
      return;
    }

    this.highlight = !!v1;

    this.old_value = v1;
    this.redraw = true;
  }

  event_handler(x, y) {
    let zx = x / this.ctx.zoom_factor + this.ctx.ll_x;
    let zy = y / this.ctx.zoom_factor + this.ctx.ll_y;
    if (this.hit(new Point(zx, zy))) {
      console.log("Hit in node");
      if (this.select) {
        this.select = false;
        this.redraw = true;
      } else {
        this.ctx.set_select(false);
        this.set_select(true);
      }
      console.log("Hit in", this.n_name);
      return 1;
    }
    return 0;
  }

  set_select(select) {
    if (select !== this.select) {
      this.select = select;
      this.redraw = true;
      if (this.select) {
        this.ctx.select_object = this;
      } else {
        this.ctx.select_object = null;
      }
    }
  }
}

class FlowCtx extends Rect {
  a: FlowArray;
  a_nc: FlowArray;
  a_cc: FlowArray;
  gdraw: GDraw;
  display_level = DisplayLevel.One;
  gdh: Gdh = null;
  zoom_factor = 20.0;
  base_zoom_factor = 20.0;
  offset_x = 0;
  offset_y = 0;
  name = "Claes context";
  select_object: FlowNode = null;

  constructor() {
    super();
    this.a = new FlowArray(this);
    this.a_nc = new FlowArray(this);
    this.a_cc = new FlowArray(this);
    this.gdraw = new GDraw(this);
  }

  draw() {
    this.a.draw(this.gdraw.gctx, null, null, false);
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0]);

      switch (key) {
        case Save.Ctx:
          break;
        case Save.Ctx_zoom_factor:
          this.zoom_factor = parseFloat(tokens[1]);
          break;
        case Save.Ctx_base_zoom_factor:
          this.base_zoom_factor = parseFloat(tokens[1]);
          break;
        case Save.Ctx_offset_x:
          this.offset_x = parseInt(tokens[1]);
          break;
        case Save.Ctx_offset_y:
          this.offset_y = parseInt(tokens[1]);
          break;
        case Save.Ctx_nav_zoom_factor:
        case Save.Ctx_print_zoom_factor:
        case Save.Ctx_nav_offset_x:
        case Save.Ctx_nav_offset_y:
          break;
        case Save.Ctx_x_right:
          this.ur_x = parseFloat(tokens[1]);
          break;
        case Save.Ctx_x_left:
          this.ll_x = parseFloat(tokens[1]);
          break;
        case Save.Ctx_y_high:
          this.ur_y = parseFloat(tokens[1]);
          break;
        case Save.Ctx_y_low:
          this.ll_y = parseFloat(tokens[1]);
          break;
        case Save.Ctx_nav_rect_ll_x:
        case Save.Ctx_nav_rect_ll_y:
        case Save.Ctx_nav_rect_ur_x:
        case Save.Ctx_nav_rect_ur_y:
        case Save.Ctx_nav_rect_hot:
          break;
        case Save.Ctx_name:
          this.name = tokens[1];
          break;
        case Save.Ctx_user_highlight:
        case Save.Ctx_grid_size_x:
        case Save.Ctx_grid_size_y:
        case Save.Ctx_grid_on:
        case Save.Ctx_draw_delta:
        case Save.Ctx_refcon_width:
        case Save.Ctx_refcon_height:
        case Save.Ctx_refcon_textsize:
        case Save.Ctx_refcon_linewidth:
        case Save.Ctx_user_version:
          break;
        case Save.Ctx_a_nc:
          i = this.a_nc.open(lines, i + 1);
          break;
        case Save.Ctx_a_cc:
          i = this.a_cc.open(lines, i + 1);
          break;
        case Save.Ctx_a:
          i = this.a.open(lines, i + 1);
          break;
        case Save.End:
          return i;
        default:
          console.log("Syntax error in FlowCtx", key);
      }
    }

    return i;
  }

  connect() {
    this.a.forEach(e => e.connect());
  }

  scan() {
    this.a.forEach(e => e.scan());
  }

  event_handler(x, y) {
    let sts = 0;
    for (let i = 0; i < this.a.size(); i++) {
      if (this.a.get(i) instanceof FlowNode) {
        sts = this.a.get(i).event_handler(x, y);
        if (sts === 1) {
          break;
        }
      }
    }
    if (sts === 1) {
      this.draw();
    }
  }

  set_select(select) {
    this.a.forEach(function (e) {
      if (e instanceof FlowNode) {
        e.set_select(select);
      }
    });
  }

  search_object(name) {
    return this.a.search_by_name(name);
  }

  center_object(o) {
    console.log("center_object", o.pos.x * this.zoom_factor + this.offset_x,
        window.innerWidth, o.pos.x * this.zoom_factor + this.offset_x -
        window.innerWidth / 2);
    console.log("center_object", o.pos.y * this.zoom_factor + this.offset_y,
        window.innerHeight, o.pos.y * this.zoom_factor + this.offset_y -
        window.innerHeight / 2);
    window.scrollTo(o.pos.x * this.zoom_factor + this.offset_x -
        window.innerWidth / 2, o.pos.y * this.zoom_factor + this.offset_y -
        window.innerHeight / 2 + this.gdraw.offset_top);
  }
}

class FlowFrame {
  ctx: FlowCtx;
  timer: number = null;
  vars_object: string = null;

  constructor() {
    this.ctx = new FlowCtx();
  }

  readFlowWeb(fname, read_cb) {
    let n = window.location.href.lastIndexOf('/');
    let target = window.location.href.substring(0, n + 1);
    target += fname;

    console.log("Target: ", target);

    let req = new XMLHttpRequest();
    req.open("GET", target, true);
    req.addEventListener("load", function () {
      console.log("Done:", req.status);
      let lines = req.responseText.split('\n');
      read_cb(lines, 0);
    });
    req.send(null);
  }

  read_func(lines, row) {
    console.log("this.ctx:", this.ctx);
    this.ctx.open(lines, row);

    this.ctx.gdraw.canvas.addEventListener("click", function (event) {
      let y = event.pageY - this.ctx.gdraw.offset_top;
      let x = event.pageX - 10; // Correction for left offset
      console.log("Click", x, y);
      this.ctx.event_handler(x, y);
    });
    document.getElementById("toolitem1")
        .addEventListener("click", function (event) {
          console.log("toolitem3 event");
        });
    document.getElementById("toolitem2")
        .addEventListener("click", function (event) {
          console.log("toolitem1 event, storage");
          console.log("storage", localStorage.getItem("XttMethodNavigator"));
          if (this.ctx.select_object !== null) {
            localStorage.setItem("XttMethodNavigator",
                this.ctx.select_object.trace_object);
          }
          console.log("storage", localStorage.getItem("XttMethodNavigator"));
          console.log("toolitem2 event window", window.opener);
          window.opener.focus();
        });
    document.getElementById("toolitem3")
        .addEventListener("click", function (event) {
        });
    document.getElementById("toolitem4")
        .addEventListener("click", function (event) {
        });

    this.ctx.gdraw.canvas.width = this.ctx.width() * this.ctx.zoom_factor;
    this.ctx.gdraw.canvas.height = this.ctx.height() * this.ctx.zoom_factor;
    this.ctx.gdraw.gctx.translate(-this.ctx.ll_x *
        this.ctx.zoom_factor, -this.ctx.ll_y * this.ctx.zoom_factor);
    this.ctx.offset_x -= this.ctx.ll_x * this.ctx.zoom_factor;
    this.ctx.offset_y -= this.ctx.ll_y * this.ctx.zoom_factor;

    this.ctx.draw();
    this.ctx.gdh = new Gdh(this.flow_open, this.flow_close);

    if (this.vars_object !== null) {
      let o = this.ctx.search_object(this.vars_object);
      if (o) {
        this.ctx.center_object(o);
        this.ctx.set_select(false);
        o.set_select(true);
      }
    }
  }

  flow_open() {
    this.ctx.connect();
    this.ctx.gdh.refObjectInfoList();
    this.timer = setTimeout(this.flow_cyclic, 1000);
  }

  flow_scan() {
    this.ctx.scan();
  }

  flow_cyclic() {
    this.ctx.gdh.getRefObjectInfoAll(this.flow_scan);
    this.ctx.draw();
    this.timer = setTimeout(this.flow_cyclic, 1000);
  }

  flow_close() {
    clearTimeout(this.timer);
    for (let i in this.ctx.gdh.pending) {
      delete this.ctx.gdh.pending[i];
    }
  }

  get_filename() {
    let query = window.location.search.substring(1);

    console.log("query", query);
    let vars = query.split('&');
    console.log("vars", vars.length, vars[0].substring(4),
        vars[1].substring(4));
    let vid = parseInt(vars[0].substring(4));
    let oix = parseInt(vars[1].substring(4));
    let avid = [];
    let svid = [];
    avid[0] = vid & 0xFF;
    avid[1] = (vid >> 8) & 0xFF;
    avid[2] = (vid >> 16) & 0xFF;
    avid[3] = (vid >> 24) & 0xFF;
    for (let i = 0; i < 4; i++) {
      svid[i] = avid[i].toString();
      for (let j = 0; j < 3; j++) {
        if (svid[i].length < 3) {
          svid[i] = "0" + svid[i];
        }
      }
    }
    let soix = oix.toString(16);
    for (let j = 0; j < 8; j++) {
      if (soix.length < 8) {
        soix = "0" + soix;
      }
    }

    let fname = "pwr_" + svid[3] + "_" + svid[2] + "_" + svid[1] + "_" +
        svid[0] + "_" + soix + ".flw";
    console.log(fname);

    if (vars.length > 2) {
      this.vars_object = vars[2].substring(4);
      console.log("vars object: ", this.vars_object);
    }

    return fname;
  }
}

let frame = new FlowFrame();

// readFlow( "t.flw", read_func);
frame.readFlowWeb(frame.get_filename(), frame.read_func);
console.log("eSave", Save.Ctx);
