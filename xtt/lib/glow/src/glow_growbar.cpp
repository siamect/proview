/*
 * ProviewR   Open Source Process Control.
 * Copyright (C) 2005-2019 SSAB EMEA AB.
 *
 * This file is part of ProviewR.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProviewR. If not, see <http://www.gnu.org/licenses/>
 *
 * Linking ProviewR statically or dynamically with other modules is
 * making a combined work based on ProviewR. Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * In addition, as a special exception, the copyright holders of
 * ProviewR give you permission to, from the build function in the
 * ProviewR Configurator, combine ProviewR with modules generated by the
 * ProviewR PLC Editor to a PLC program, regardless of the license
 * terms of these modules. You may copy and distribute the resulting
 * combined work under the terms of your choice, provided that every
 * copy of the combined work is accompanied by a complete copy of
 * the source code of ProviewR (the version used to produce the
 * combined work), being distributed under the terms of the GNU
 * General Public License plus this exception.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "glow_growbar.h"
#include "glow_grownode.h"
#include "glow_draw.h"

GrowBar::GrowBar(GrowCtx* glow_ctx, const char* name, double x, double y,
    double w, double h, glow_eDrawType border_d_type, int line_w,
    glow_mDisplayLevel display_lev, int fill_rect, int display_border,
    glow_eDrawType fill_d_type, int nodraw)
    : GrowRect(glow_ctx, name, x, y, w, h, border_d_type, line_w, 0,
          display_lev, fill_rect, display_border, 0, fill_d_type, nodraw),
      max_value(100), min_value(0), bar_value(35),
      bar_drawtype(glow_eDrawType_Inherit),
      bar_bordercolor(glow_eDrawType_Inherit), bar_borderwidth(1), user_data(0)
{
  if (!nodraw)
    ctx->set_dirty();
}

GrowBar::~GrowBar()
{
  ctx->set_dirty();
}

void GrowBar::save(std::ofstream& fp, glow_eSaveMode mode)
{
  fp << int(glow_eSave_GrowBar) << '\n';
  fp << int(glow_eSave_GrowBar_max_value) << FSPACE << max_value << '\n';
  fp << int(glow_eSave_GrowBar_min_value) << FSPACE << min_value << '\n';
  fp << int(glow_eSave_GrowBar_bar_value) << FSPACE << bar_value << '\n';
  fp << int(glow_eSave_GrowBar_bar_drawtype) << FSPACE << int(bar_drawtype)
     << '\n';
  fp << int(glow_eSave_GrowBar_bar_bordercolor) << FSPACE
     << int(bar_bordercolor) << '\n';
  fp << int(glow_eSave_GrowBar_bar_borderwidth) << FSPACE << bar_borderwidth
     << '\n';
  fp << int(glow_eSave_GrowBar_rect_part) << '\n';
  GrowRect::save(fp, mode);
  if (user_data && ctx->userdata_save_callback) {
    fp << int(glow_eSave_GrowBar_userdata_cb) << '\n';
    (ctx->userdata_save_callback)(&fp, this, glow_eUserdataCbType_Node);
  }
  fp << int(glow_eSave_End) << '\n';
}

void GrowBar::open(std::ifstream& fp)
{
  int type = 0;
  int end_found = 0;
  char dummy[40];
  int tmp;
  unsigned int utmp;

  for (;;) {
    if (!fp.good()) {
      fp.clear();
      fp.getline(dummy, sizeof(dummy));
      printf("** Read error GrowBar: \"%d %s\"\n", type, dummy);
    }

    fp >> type;
    switch (type) {
    case glow_eSave_GrowBar:
      break;
    case glow_eSave_GrowBar_max_value:
      fp >> max_value;
      break;
    case glow_eSave_GrowBar_min_value:
      fp >> min_value;
      break;
    case glow_eSave_GrowBar_bar_value:
      fp >> bar_value;
      break;
    case glow_eSave_GrowBar_bar_bordercolor:
      fp >> tmp;
      bar_bordercolor = (glow_eDrawType)tmp;
      break;
    case glow_eSave_GrowBar_bar_borderwidth:
      fp >> bar_borderwidth;
      break;
    case glow_eSave_GrowBar_bar_drawtype:
      fp >> tmp;
      bar_drawtype = (glow_eDrawType)tmp;
      break;
    case glow_eSave_GrowBar_rect_part:
      GrowRect::open(fp);
      break;
    case glow_eSave_GrowBar_trace_data1:
    case glow_eSave_GrowBar_trace_data2:
    case glow_eSave_GrowBar_trace_data3:
    case glow_eSave_GrowBar_trace_data4:
    case glow_eSave_GrowBar_trace_data5:
    case glow_eSave_GrowBar_trace_data6:
    case glow_eSave_GrowBar_trace_data7:
    case glow_eSave_GrowBar_trace_data8:
    case glow_eSave_GrowBar_trace_data9:
    case glow_eSave_GrowBar_trace_data10:
    case glow_eSave_GrowBar_ref_object:
      fp.get();
      fp.getline( dummy, sizeof(dummy));
      break;
    case glow_eSave_GrowBar_trace_attr_type: fp >> tmp; break;
    case glow_eSave_GrowBar_trace_color: fp >> tmp; break;
    case glow_eSave_GrowBar_access: fp >> utmp; break;
    case glow_eSave_GrowBar_cycle: fp >> tmp; break;
    case glow_eSave_GrowBar_userdata_cb:
      if (ctx->userdata_open_callback)
        (ctx->userdata_open_callback)(&fp, this, glow_eUserdataCbType_Node);
      break;
    case glow_eSave_End:
      end_found = 1;
      break;
    default:
      std::cout << "GrowBar:open syntax error\n";
      fp.getline(dummy, sizeof(dummy));
    }
    if (end_found)
      break;
  }
}

void GrowBar::draw(GlowWind* w, int ll_x, int ll_y, int ur_x, int ur_y)
{
  int tmp;

  if (ll_x > ur_x) {
    /* Shift */
    tmp = ll_x;
    ll_x = ur_x;
    ur_x = tmp;
  }
  if (ll_y > ur_y) {
    /* Shift */
    tmp = ll_y;
    ll_y = ur_y;
    ur_y = tmp;
  }

  if (x_right * w->zoom_factor_x - w->offset_x + 1 >= ll_x
      && x_left * w->zoom_factor_x - w->offset_x <= ur_x
      && y_high * w->zoom_factor_y - w->offset_y + 1 >= ll_y
      && y_low * w->zoom_factor_y - w->offset_y <= ur_y) {
    draw(w, (GlowTransform*)NULL, highlight, hot, NULL, NULL);
  }
}

void GrowBar::draw(GlowWind* w, int* ll_x, int* ll_y, int* ur_x, int* ur_y)
{
  int tmp;
  int obj_ur_x = int(x_right * w->zoom_factor_x) - w->offset_x;
  int obj_ll_x = int(x_left * w->zoom_factor_x) - w->offset_x;
  int obj_ur_y = int(y_high * w->zoom_factor_y) - w->offset_y;
  int obj_ll_y = int(y_low * w->zoom_factor_y) - w->offset_y;

  if (*ll_x > *ur_x) {
    /* Shift */
    tmp = *ll_x;
    *ll_x = *ur_x;
    *ur_x = tmp;
  }
  if (*ll_y > *ur_y) {
    /* Shift */
    tmp = *ll_y;
    *ll_y = *ur_y;
    *ur_y = tmp;
  }

  if (obj_ur_x >= *ll_x && obj_ll_x <= *ur_x && obj_ur_y >= *ll_y
      && obj_ll_y <= *ur_y) {
    draw(w, (GlowTransform*)NULL, highlight, hot, NULL, NULL);

    // Increase the redraw area
    if (obj_ur_x > *ur_x)
      *ur_x = obj_ur_x;
    if (obj_ur_y > *ur_y)
      *ur_y = obj_ur_y;
    if (obj_ll_x < *ll_x)
      *ll_x = obj_ll_x;
    if (obj_ll_y < *ll_y)
      *ll_y = obj_ll_y;
  }
}

void GrowBar::set_highlight(int on)
{
  if (highlight != on) {
    highlight = on;
    ctx->set_dirty();
  }
}

void GrowBar::draw(GlowWind* w, GlowTransform* t, int highlight, int hot,
    void* node, void* colornode)
{
  if (!(display_level & ctx->display_level))
    return;
  hot = (w == &ctx->navw) ? 0 : hot;
  int idx;
  glow_eDrawType drawtype;
  glow_eGradient grad = gradient;
  if (gradient == glow_eGradient_No
      && (node && ((GrowNode*)node)->gradient != glow_eGradient_No)
      && !disable_gradient)
    grad = ((GrowNode*)node)->gradient;

  int bar_border_idx
      = int(w->zoom_factor_y / w->base_zoom_factor * bar_borderwidth - 1);
  bar_border_idx = MIN(DRAW_TYPE_SIZE - 1, MAX(0, bar_border_idx));

  if (fix_line_width) {
    idx = line_width;
    idx += hot;
    if (idx < 0) {
      erase(w, t, hot, node);
      return;
    }
  } else {
    if (node && ((GrowNode*)node)->line_width)
      idx = int(
          w->zoom_factor_y / w->base_zoom_factor * ((GrowNode*)node)->line_width
          - 1);
    else
      idx = int(w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = MAX(0, idx);
  idx = MIN(idx, DRAW_TYPE_SIZE - 1);

  Matrix tmp = t ? (*t * trf) : trf;
  glow_sPoint p1 = tmp * ll;
  glow_sPoint p2 = tmp * ur;
  p1.x = p1.x * w->zoom_factor_x - w->offset_x;
  p1.y = p1.y * w->zoom_factor_y - w->offset_y;
  p2.x = p2.x * w->zoom_factor_x - w->offset_x;
  p2.y = p2.y * w->zoom_factor_y - w->offset_y;

  int ll_x = ROUND(MIN(p1.x, p2.x));
  int ur_x = ROUND(MAX(p1.x, p2.x));
  int ll_y = ROUND(MIN(p1.y, p2.y));
  int ur_y = ROUND(MAX(p1.y, p2.y));

  if (fill) {
    drawtype = ctx->get_drawtype(fill_drawtype, glow_eDrawType_FillHighlight,
        highlight, (GrowNode*)colornode, 1);

    if (grad == glow_eGradient_No)
      ctx->gdraw->rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, 1, 0);
    else {
      glow_eDrawType f1, f2;
      if (gradient_contrast >= 0) {
        f2 = GlowColor::shift_drawtype(drawtype, -gradient_contrast / 2, 0);
        f1 = GlowColor::shift_drawtype(
            drawtype, int(float(gradient_contrast) / 2 + 0.6), 0);
      } else {
        f2 = GlowColor::shift_drawtype(
            drawtype, -int(float(gradient_contrast) / 2 - 0.6), 0);
        f1 = GlowColor::shift_drawtype(drawtype, gradient_contrast / 2, 0);
      }

      ctx->gdraw->gradient_fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
          drawtype, f1, f2, ctx->gdraw->gradient_rotate(tmp.rotation, grad));
    }
  }
  drawtype = ctx->get_drawtype(draw_type, glow_eDrawType_LineHighlight,
      highlight, (GrowNode*)colornode, 0);

  if (!feq(max_value, min_value)) {
    int x0 = ll_x, y0 = ll_y, width = ur_x - ll_x, height = ur_y - ll_y;
    int l_x0 = ll_x, l_y0 = ll_y, l_x1 = ur_x, l_y1 = ur_y;

    double rotation = (tmp.rotation / 360 - floor(tmp.rotation / 360)) * 360;

    if (45 >= rotation || rotation > 315) {
      height = int(
          (bar_value - min_value) / (max_value - min_value) * (ur_y - ll_y));
      height = MAX(0, MIN(height, ur_y - ll_y));
      y0 = ur_y - height;
      l_y0 = ur_y - height;
      l_y1 = ur_y - height;
    } else if (45 < rotation && rotation <= 135) {
      width = int(
          (bar_value - min_value) / (max_value - min_value) * (ur_x - ll_x));
      width = MAX(0, MIN(width, ur_x - ll_x));
      l_x0 = ll_x + width;
      l_x1 = ll_x + width;
    } else if (135 < rotation && rotation <= 225) {
      height = int(
          (bar_value - min_value) / (max_value - min_value) * (ur_y - ll_y));
      height = MAX(0, MIN(height, ur_y - ll_y));
      l_y0 = ll_y + height;
      l_y1 = ll_y + height;
    } else { // if ( 225 < rotation && rotation <= 315)
      width = int(
          (bar_value - min_value) / (max_value - min_value) * (ur_x - ll_x));
      width = MAX(0, MIN(width, ur_x - ll_x));
      x0 = ur_x - width;
      l_x0 = ur_x - width;
      l_x1 = ur_x - width;
    }

    glow_eDrawType dt = drawtype;
    if (bar_drawtype != glow_eDrawType_Inherit)
      dt = bar_drawtype;

    if (grad == glow_eGradient_No)
      ctx->gdraw->rect(x0, y0, width, height, dt, 1, 0);
    else {
      glow_eDrawType f1, f2;
      if (gradient_contrast >= 0) {
        f2 = GlowColor::shift_drawtype(dt, -gradient_contrast / 2, 0);
        f1 = GlowColor::shift_drawtype(
            dt, int(float(gradient_contrast) / 2 + 0.6), 0);
      } else {
        f2 = GlowColor::shift_drawtype(
            dt, -int(float(gradient_contrast) / 2 - 0.6), 0);
        f1 = GlowColor::shift_drawtype(dt, gradient_contrast / 2, 0);
      }

      ctx->gdraw->gradient_fill_rect(x0, y0, width, height, dt, f1, f2,
          ctx->gdraw->gradient_rotate(tmp.rotation, grad));
    }

    dt = drawtype;
    if (bar_bordercolor != glow_eDrawType_Inherit)
      dt = bar_bordercolor;
    ctx->gdraw->line(l_x0, l_y0, l_x1, l_y1, dt, bar_border_idx, 0);
  }

  if (border) {
    ctx->gdraw->rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, 0, idx);
  }
}

void GrowBar::erase(GlowWind* w, GlowTransform* t, int hot, void* node)
{
  if (!(display_level & ctx->display_level))
    return;
  hot = (w == &ctx->navw) ? 0 : hot;
  int idx;
  if (fix_line_width) {
    idx = line_width;
    idx += hot;
    if (idx < 0)
      return;
  } else {
    if (node && ((GrowNode*)node)->line_width)
      idx = int(
          w->zoom_factor_y / w->base_zoom_factor * ((GrowNode*)node)->line_width
          - 1);
    else
      idx = int(w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = MAX(0, idx);
  idx = MIN(idx, DRAW_TYPE_SIZE - 1);

  Matrix tmp = t ? (*t * trf) : trf;
  glow_sPoint p1 = tmp * ll;
  glow_sPoint p2 = tmp * ur;
  p1.x = p1.x * w->zoom_factor_x - w->offset_x;
  p1.y = p1.y * w->zoom_factor_y - w->offset_y;
  p2.x = p2.x * w->zoom_factor_x - w->offset_x;
  p2.y = p2.y * w->zoom_factor_y - w->offset_y;

  int ll_x = ROUND(MIN(p1.x, p2.x));
  int ur_x = ROUND(MAX(p1.x, p2.x));
  int ll_y = ROUND(MIN(p1.y, p2.y));
  int ur_y = ROUND(MAX(p1.y, p2.y));

  if (border)
    ctx->gdraw->rect(
        ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase, 0, idx);
  ctx->gdraw->rect(
      ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase, 1, 0);
}

int GrowBar::trace_scan()
{
  if (!trace.p)
    return 1;

  if (ctx->trace_scan_func)
    ctx->trace_scan_func((void*)this, trace.p);
  return 1;
}

int GrowBar::trace_init()
{
  int sts;

  sts = ctx->trace_connect_func((void*)this, &trace);
  return sts;
}

void GrowBar::trace_close()
{
  if (trace.p)
    ctx->trace_disconnect_func((void*)this);
}

void GrowBar::align(double x, double y, glow_eAlignDirection direction)
{
  double dx, dy;

  switch (direction) {
  case glow_eAlignDirection_CenterVert:
    dx = x - (x_right + x_left) / 2;
    dy = 0;
    break;
  case glow_eAlignDirection_CenterHoriz:
    dx = 0;
    dy = y - (y_high + y_low) / 2;
    break;
  case glow_eAlignDirection_CenterCenter:
    dx = x - (x_right + x_left) / 2;
    dy = y - (y_high + y_low) / 2;
    break;
  case glow_eAlignDirection_Right:
    dx = x - x_right;
    dy = 0;
    break;
  case glow_eAlignDirection_Left:
    dx = x - x_left;
    dy = 0;
    break;
  case glow_eAlignDirection_Up:
    dx = 0;
    dy = y - y_high;
    break;
  case glow_eAlignDirection_Down:
    dx = 0;
    dy = y - y_low;
    break;
  }
  trf.move(dx, dy);
  x_right += dx;
  x_left += dx;
  y_high += dy;
  y_low += dy;
  ctx->set_dirty();
}

void GrowBar::set_trace_attr(GlowTraceData* attr)
{
  memcpy(&trace, attr, sizeof(trace));
}

void GrowBar::get_trace_attr(GlowTraceData** attr)
{
  *attr = &trace;
}

void GrowBar::get_range(double *min, double *max)
{
  *max = max_value;
  *min = min_value;
}

void GrowBar::set_range(double min, double max)
{
  max_value = max;
  min_value = min;
  ctx->set_dirty();
}

void GrowBar::export_javabean(GlowTransform* t, void* node,
    glow_eExportPass pass, int* shape_cnt, int node_cnt, int in_nc,
    std::ofstream& fp)
{
  Matrix tmp = t ? (*t * trf) : trf;
  glow_sPoint p1 = tmp * ll;
  glow_sPoint p2 = tmp * ur;
  p1.x = p1.x * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
  p1.y = p1.y * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
  p2.x = p2.x * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
  p2.y = p2.y * ctx->mw.zoom_factor_y - ctx->mw.offset_y;

  int ll_x = ROUND(MIN(p1.x, p2.x));
  int ur_x = ROUND(MAX(p1.x, p2.x));
  int ll_y = ROUND(MIN(p1.y, p2.y));
  int ur_y = ROUND(MAX(p1.y, p2.y));
  double rotation = (tmp.rotation / 360 - floor(tmp.rotation / 360)) * 360;

  ctx->export_jbean->bar(ll_x, ll_y, ur_x, ur_y, draw_type, fill_drawtype,
      bar_drawtype, bar_bordercolor, fill, border, min_value, max_value,
      bar_borderwidth, line_width, rotation, pass, shape_cnt, node_cnt, fp);
}

void GrowBar::set_bar_info(glow_sBarInfo* info)
{
  max_value = info->max_value;
  min_value = info->min_value;
  bar_drawtype = info->bar_drawtype;
  bar_bordercolor = info->bar_bordercolor;
  bar_borderwidth = info->bar_borderwidth;
}

void GrowBar::get_bar_info(glow_sBarInfo* info)
{
  info->max_value = max_value;
  info->min_value = min_value;
  info->bar_drawtype = bar_drawtype;
  info->bar_bordercolor = bar_bordercolor;
  info->bar_borderwidth = bar_borderwidth;
}

void GrowBar::convert(glow_eConvert version)
{
  switch (version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    GrowRect::convert(version);
    bar_drawtype = GlowColor::convert(version, bar_drawtype);
    bar_bordercolor = GlowColor::convert(version, bar_bordercolor);

    break;
  }
  }
}
