/* 
 * Proview   $Id: glow_color.cpp,v 1.7 2007-08-20 14:39:59 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
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
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

#include "glow_std.h"

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include "glow_color.h"
#include "glow_grownode.h"

typedef struct {
  double r;
  double g;
  double b;
} s_rgb;

typedef struct {
  double s;
  double i_min;
  double i_max;
  double a;
} s_sat;

typedef struct {
  double h;
  double hk;
  s_sat s[3]; 
} s_hue;

static s_hue ctab[] = {

  //                low saturation        medium saturation    high satuartion
  // h    hk        s   i_min i_max a     s    i_min i_max a   s   i_min i_max a

  { 18,  -20.0, {{ 0.2, -1.4, 2.8, .9}, { 0.5, -1.4, 3, .9}, {1.5, -3.5, 5.0, 1.0}}},  // YellowGreen
  { 28,  10.0, {{ 0.2, -1.4, 2.8, .9}, { 0.5, -1.4, 3, .9}, {1.5, -3.5, 5.0, 1.0}}},  // Yellow
  { 45,  20.0, {{ 0.2, -1.3, 2.8, .9}, { 0.5, -1.3, 3, .9}, {1.5, -3.5, 4.5, 1.0}}}, // Orange
  { 90,  0.0, {{ 0.2, -1.1, 2.8, .9}, { 0.5, -1.1, 3, .9}, {1.5, -4.5, 4.5, 0.8}}}, // Red
  { 150, 0.0, {{ 0.2, -1., 2.8, .9}, { 0.5, -1., 3, .9}, {1.5, -3.5, 5.9, 1.0}}}, // Violet
  { 240, 0.0, {{ 0.2, -1., 2.8, .9}, { 0.4, -1., 3, .9}, {1.5, -3.5, 5.5, 0.8}}}, // Blue
  { 280, 0.0, {{ 0.2, -1., 2.8, .9}, { 0.4, -1., 3, .9}, {1.5, -3.5, 5.5, 1.2}}}, // Seablue
  { 355, -20.0, {{ 0.2, -1., 2.8, .9}, { 0.4, -1., 3, .9}, {1.5, -4.5, 3.5, 1.0}}}  // Green
};

static double gray_i0 = 0.32;		// Gray min
static double gray_i1 = 0.95;		// Gray max

// Table for saturated yellowgreen to orange
static s_rgb rgbtab[] = {
  { 0.254, 0.329, 0},  // YellowGreen
  { 0.357, 0.459, 0},
  { 0.498, 0.639, 0},
  { 0.624, 0.800, 0},
  { 0.764, 0.976, 0},
  { 0.808, 1.000, 0.129},
  { 0.847, 1.000, 0.310},
  { 0.898, 1.000, 0.537},
  { 0.933, 1.000, 0.710},
  { 0.949, 1.000, 0.776},
  { 0., 0., 0},		// Yellow
  { 0., 0., 0},
  { 0., 0., 0},
  { 0., 0., 0},
  { 0., 0., 0},
  { 0., 0., 0},
  { 0., 0., 0},
  { 0., 0., 0},
  { 0., 0., 0},
  { 0., 0., 0},
  { 0., 0., 0},		// Orange
  { 0., 0., 0},
  { 0., 0., 0},
  { 0., 0., 0},
  { 0., 0., 0},
  { 0., 0., 0},
  { 1.000, 0.725, 0.420},
  { 1.000, 0.796, 0.569},
  { 1.000, 0.886, 0.667},
  { 1.000, 0.937, 0.820}
};


void GlowColor::his_to_rgb( double *r, double *g, double *b, double h, double i, double s)
{
  double m1, m2, i1;

  m1 = s * sin( h * M_PI/180);
  m2 = s * cos( h * M_PI/180);
  i1 = i / sqrt(3);

  *r = m1 * 2 / sqrt(6) + i1 / sqrt(3);
  *g = - m1 / sqrt(6)  + m2 / sqrt(2) + i1 / sqrt(3);
  *b = - m1 / sqrt(6) - m2 / sqrt(2) + i1 / sqrt(3);

  *r = *r / 2 + 0.5;
  *g = *g / 2 + 0.5;
  *b = *b / 2 + 0.5;
  if ( *r > 1) *r = 1;
  if ( *r < 0) *r = 0;
  if ( *g > 1) *g = 1;
  if ( *g < 0) *g = 0;
  if ( *b > 1) *b = 1;
  if ( *b < 0) *b = 0;
}

void GlowColor::rgb_color( int idx, double *r, double *g, double *b)
{
  double h1, i1, s1;
  int i, j, k;

  
  if ( idx == 3) // White
    *r = *g = *b = 1.0;
  else if ( idx == 2) // Gray
    *r = *g = *b = 0.75;
  else if ( idx == 1) { // Red
    *r = 1.0;
    *g = *b = 0;
  }
  else if ( idx == 0) // Black
    *r = *g = *b = 0;
  else if ( idx < 20) {
    // Sixteen colors of different hue
    h1 = 360. * (idx - 4) / 16;
    s1 = 1.5;
    i1 = 1;
    his_to_rgb( r, g, b, h1, i1, s1);
  }
  else if ( idx < 60) {
    // Four sets of gray
    double i0 = gray_i0;

    if ( idx < 30 || (50 <= idx && idx < 60))
      i0 = 0.25;
    
    *r = *g = *b = i0 + (gray_i1 - i0) * pow( double(9 - idx % 10) / 9, 0.9);
  }
  else if ( idx < 300) {
    i = (idx - 60) / 30;
    j = (idx - 60 - i * 30) / 10;
    k = 9 - (idx - 60 - i * 30 - j * 10);
    
    if ( (i == 0 && j == 2) ||
	 (i == 2 && j == 2 && k > 5)) {
      // Formula doesn't work for yellow...
      *r = rgbtab[ i * 10 + k].r;
      *g = rgbtab[ i * 10 + k].g;
      *b = rgbtab[ i * 10 + k].b;
    }
    else {
      s1 = ctab[i].s[j].s;
      i1 = ctab[i].s[j].i_min + (ctab[i].s[j].i_max - ctab[i].s[j].i_min) * 
	pow(double(k)/9, ctab[i].s[j].a);
      h1 = ctab[i].h + ctab[i].hk * k / 9;
	
      his_to_rgb( r, g, b, h1, i1, s1);
    }
  }
}


glow_eDrawType GlowColor::shift_drawtype( glow_eDrawType dt, int shift, void *node)
{
  int incr;
  glow_eDrawType base_drawtype;
  glow_eDrawType drawtype;

  if ( node && ((GrowNode *)node)->color_inverse)
    shift = - shift;

  if ( dt >= 20) {
    base_drawtype = (glow_eDrawType)(dt / 10 * 10);
    incr = shift + dt - base_drawtype;
    if ( incr < 0)
      drawtype = glow_eDrawType_Color4; // White
    else if ( incr >= 10)
      drawtype = glow_eDrawType_Color30; // DarkGrey
    else
      drawtype = (glow_eDrawType)(base_drawtype + incr);
  }
  else
    drawtype = dt;

  return drawtype;
}

glow_eDrawType GlowColor::get_drawtype( glow_eDrawType local_drawtype,
		glow_eDrawType highlight_drawtype, int highlight, void *node, 
		int fill, int highlight_disabled)
{
  glow_eDrawType drawtype;
  int	base_drawtype;
  int	incr;
  int	lightness;
  int	intensity;

  if ( node) {
    lightness = ((GrowNode *)node)->color_lightness;
    intensity = ((GrowNode *)node)->color_intensity;
  }

  if ( highlight && !highlight_disabled) {
    drawtype = highlight_drawtype;
  }
  else if ( highlight && highlight_disabled && !fill && local_drawtype == 
	    glow_eDrawType_Line) {
    drawtype = highlight_drawtype;
  }
  else {
    if ( node && ((GrowNode *)node)->color_tone != glow_eDrawTone_No) {
      glow_eDrawTone tone = ((GrowNode *)node)->color_tone;

      if ( local_drawtype > 30) {
	if ( tone == glow_eDrawTone_LightGrayHighSaturation)
          drawtype = glow_eDrawType_Color31; // GrayLow1
	else if ( tone == glow_eDrawTone_DarkGrayHighSaturation)
          drawtype = glow_eDrawType_Color30; // DarkGrey
	else {
	  if ( tone >= glow_eDrawTone_GrayHighSaturation &&
	       tone < glow_eDrawTone_GrayLowSaturation) {
	    tone = glow_eDrawTone( tone - 27);
	    intensity += 2;
	  }
	  else if ( tone >= glow_eDrawTone_GrayLowSaturation) {
	    tone = glow_eDrawTone( tone - 2 * 27);
	    intensity -= 1;
	  }
	  if ( tone >= glow_eDrawTone_DarkGray &&
	       tone < glow_eDrawTone_LightGray) {
	    tone = glow_eDrawTone( tone - 9);
	    lightness -= 2;
	  }
	  else if ( tone >= glow_eDrawTone_LightGray) {
	    tone = glow_eDrawTone( tone - 18);
	    lightness += 2;
	  }
	  
	  drawtype = (glow_eDrawType) (local_drawtype - local_drawtype / 30 * 30 + 
				       30 * tone);
	}
      }
      else
        drawtype = local_drawtype;
    }
    else if ( node && fill == 1 && ((GrowNode *)node)->fill_drawtype != glow_eDrawType_No) {
      drawtype = ((GrowNode *)node)->fill_drawtype;
    }
    else if ( node && fill == 2 && ((GrowNode *)node)->text_drawtype != glow_eDrawType_No) {
      drawtype = ((GrowNode *)node)->text_drawtype;
    }
    else if ( node && !fill && ((GrowNode *)node)->draw_type != glow_eDrawType_No) {
      drawtype = ((GrowNode *)node)->draw_type;
    }
    else
      drawtype = local_drawtype;

    if ( node && lightness) {
      if ( local_drawtype >= 30) {
        base_drawtype = drawtype / 10 * 10;
        incr = -lightness + drawtype - base_drawtype;
        if ( incr < 0)
          drawtype = glow_eDrawType_Color4; // White
        else if ( incr >= 10)
          drawtype = glow_eDrawType_Color30; // DarkGrey
        else
          drawtype = (glow_eDrawType)(base_drawtype + incr);
      }
    }
    if ( node && intensity) {
      if ( drawtype >= 60) {
        base_drawtype = drawtype / 30 * 30;
        incr = drawtype - base_drawtype;
	drawtype = glow_eDrawType( drawtype + min( 2 - incr / 10, intensity) * 10);
	if ( drawtype < base_drawtype)
	  drawtype = (glow_eDrawType)( glow_eDrawType_Color41 + incr);
	/*
        if ( 0 <= incr && incr < 7) {
          if ( intensity > 0)
            drawtype = (glow_eDrawType)( drawtype + 10);
          else
            drawtype = (glow_eDrawType)( glow_eDrawType_Color41 + incr);
        }
        else {
          if ( intensity == -1)
            drawtype = (glow_eDrawType)( drawtype - 10);
          else if ( intensity < 0)
            drawtype = (glow_eDrawType)( glow_eDrawType_Color31 + incr);
        }
	*/
      }
    }
    if ( node && ((GrowNode *)node)->color_shift) {
      if ( drawtype >= 60) {
        incr = ((GrowNode *)node)->color_shift - 
		((GrowNode *)node)->color_shift / 8 * 8;
        if ( incr < 0)
          incr += 8;

        incr = drawtype + incr * 30;
        if ( incr >= 300)
          incr -= 240;
        drawtype = (glow_eDrawType)incr;
      }
    }
    if ( node && ((GrowNode *)node)->color_inverse) {
      if ( drawtype >= 30)
        drawtype = (glow_eDrawType)(drawtype + 10 - 2 * (drawtype % 10) - 1);      
    }
    if ( node && ((GrowNode *)node)->dimmed) {
      if ( drawtype == 0)
	drawtype = (glow_eDrawType) 25;
      else if ( 26 <= drawtype && drawtype <= 29)
	drawtype = (glow_eDrawType) ( drawtype - 4);
      else if ( 36 <= drawtype && drawtype <= 39)
	drawtype = (glow_eDrawType) ( drawtype - 4);
      else if ( 46 <= drawtype && drawtype <= 49)
	drawtype = (glow_eDrawType) ( drawtype - 4);
      else if ( 56 <= drawtype && drawtype <= 59)
	drawtype = (glow_eDrawType) ( drawtype - 4);
    }
  }
  if ( drawtype < 0 || drawtype >= 301) {
    printf("** Invalid drawtype : %d\n", drawtype);    
    drawtype = glow_eDrawType_Line;
  }
  return drawtype;
}

// Convert from 3.4 palett
glow_eDrawType GlowColor::convert( glow_eConvert version, glow_eDrawType color)
{
  static glow_eDrawType cvtab[100] = {
	glow_eDrawType_Line,
	glow_eDrawType_LineRed,
	glow_eDrawType_LineGray,
	glow_eDrawType_Color4,
	glow_eDrawType_Color19,
	glow_eDrawType_Color6,
	glow_eDrawType_Color14,
	glow_eDrawType_Color13,
	glow_eDrawType_Color29,
	glow_eDrawType_Color30,

	glow_eDrawType_Color41,
	glow_eDrawType_Color42,
	glow_eDrawType_Color43,
	glow_eDrawType_Color44,
	glow_eDrawType_Color47,
	glow_eDrawType_Color51,
	glow_eDrawType_Color52,
	glow_eDrawType_Color54,
	glow_eDrawType_Color57,
	glow_eDrawType_Color59,

	glow_eDrawType_Color101,
	glow_eDrawType_Color102,
	glow_eDrawType_Color103,
	glow_eDrawType_Color105,
	glow_eDrawType_Color109,
	glow_eDrawType_Color111,
	glow_eDrawType_Color113,
	glow_eDrawType_Color115,
	glow_eDrawType_Color117,
	glow_eDrawType_Color119,

	glow_eDrawType_Color101,
	glow_eDrawType_Color102,
	glow_eDrawType_Color103,
	glow_eDrawType_Color105,
	glow_eDrawType_Color109,
	glow_eDrawType_Color101,
	glow_eDrawType_Color113,
	glow_eDrawType_Color115,
	glow_eDrawType_Color117,
	glow_eDrawType_Color119,

	glow_eDrawType_Color131,
	glow_eDrawType_Color135,
	glow_eDrawType_Color133,
	glow_eDrawType_Color135,
	glow_eDrawType_Color139,
	glow_eDrawType_Color141,
	glow_eDrawType_Color143,
	glow_eDrawType_Color145,
	glow_eDrawType_Color147,
	glow_eDrawType_Color149,

	glow_eDrawType_Color161,
	glow_eDrawType_Color162,
	glow_eDrawType_Color163,
	glow_eDrawType_Color165,
	glow_eDrawType_Color169,
	glow_eDrawType_Color171,
	glow_eDrawType_Color173,
	glow_eDrawType_Color175,
	glow_eDrawType_Color177,
	glow_eDrawType_Color179,

	glow_eDrawType_Color191,
	glow_eDrawType_Color192,
	glow_eDrawType_Color193,
	glow_eDrawType_Color195,
	glow_eDrawType_Color199,
	glow_eDrawType_Color201,
	glow_eDrawType_Color203,
	glow_eDrawType_Color205,
	glow_eDrawType_Color207,
	glow_eDrawType_Color209,

	glow_eDrawType_Color221,
	glow_eDrawType_Color222,
	glow_eDrawType_Color223,
	glow_eDrawType_Color214,
	glow_eDrawType_Color229,
	glow_eDrawType_Color231,
	glow_eDrawType_Color232,
	glow_eDrawType_Color234,
	glow_eDrawType_Color236,
	glow_eDrawType_Color239,

	glow_eDrawType_Color251,
	glow_eDrawType_Color253,
	glow_eDrawType_Color255,
	glow_eDrawType_Color257,
	glow_eDrawType_Color259,
	glow_eDrawType_Color261,
	glow_eDrawType_Color263,
	glow_eDrawType_Color265,
	glow_eDrawType_Color267,
	glow_eDrawType_Color269,

	glow_eDrawType_Color282,
	glow_eDrawType_Color284,
	glow_eDrawType_Color286,
	glow_eDrawType_Color288,
	glow_eDrawType_Color289,
	glow_eDrawType_Color292,
	glow_eDrawType_Color294,
	glow_eDrawType_Color296,
	glow_eDrawType_Color298,
	glow_eDrawType_Color299 };

  switch ( version) {
  case glow_eConvert_V34: {
    if ( color > glow_eDrawType_Color100 && (int)color < 107 )
      return (glow_eDrawType)(color + 200);
    if ( color > glow_eDrawType_Color100)
      return color;
    return cvtab[ color];
  }
  }
  return glow_eDrawType_Inherit;
}



char *GlowColor::color_to_name( glow_eDrawType color)
{
  char default_color_names[301][32] = {
        /* glow_eDrawType_Line */      	"Black",
	/* glow_eDrawType_LineRed */ 	"Red",
	/* glow_eDrawType_LineGray */ 	"Grey",
	/* glow_eDrawType_Color4 */    	"White",
	/* glow_eDrawType_Color5 */    	"YellowGreen",
	/* glow_eDrawType_Color6 */    	"Yellow",
	/* glow_eDrawType_Color7 */    	"Gold",
	/* glow_eDrawType_Color8 */    	"Orange",
	/* glow_eDrawType_Color9 */ 	"OrangeRed",
	/* glow_eDrawType_Color10 */ 	"Red",
	/* glow_eDrawType_Color11 */ 	"RedViolet",
	/* glow_eDrawType_Color12 */ 	"Violet",
	/* glow_eDrawType_Color13 */ 	"BlueViolet",
	/* glow_eDrawType_Color14 */ 	"BlueBlueViolet",
	/* glow_eDrawType_Color15 */ 	"Blue",
	/* glow_eDrawType_Color16 */ 	"BlueBlueGreen",
	/* glow_eDrawType_Color17 */ 	"BlueGreen",
	/* glow_eDrawType_Color18 */ 	"GreenGreenBlue",
	/* glow_eDrawType_Color19 */ 	"Green",
	/* glow_eDrawType_Color20 */ 	"GreenGreenYellow",
	/* glow_eDrawType_Color21 */ 	"GrayFix1",
	/* glow_eDrawType_Color22 */ 	"GrayFix2",
	/* glow_eDrawType_Color23 */ 	"GrayFix3",
	/* glow_eDrawType_Color24 */ 	"GrayFix4",
	/* glow_eDrawType_Color25 */ 	"GrayFix5",
	/* glow_eDrawType_Color26 */ 	"GrayFix6",
	/* glow_eDrawType_Color27 */ 	"GrayFix7",
	/* glow_eDrawType_Color28 */ 	"GrayFix8",
	/* glow_eDrawType_Color29 */ 	"GrayFix9",
	/* glow_eDrawType_Color30 */ 	"GrayFix10",
	/* glow_eDrawType_Color31 */ 	"GrayLow1",
	/* glow_eDrawType_Color32 */ 	"GrayLow2",
	/* glow_eDrawType_Color33 */ 	"GrayLow3",
	/* glow_eDrawType_Color34 */ 	"GrayLow4",
	/* glow_eDrawType_Color35 */ 	"GrayLow5",
	/* glow_eDrawType_Color36 */ 	"GrayLow6",
	/* glow_eDrawType_Color37 */ 	"GrayLow7",
	/* glow_eDrawType_Color38 */ 	"GrayLow8",
	/* glow_eDrawType_Color39 */ 	"GrayLow9",
	/* glow_eDrawType_Color40 */ 	"GrayLow10",
	/* glow_eDrawType_Color41 */ 	"GrayMedium1",
	/* glow_eDrawType_Color42 */ 	"GrayMedium2",
	/* glow_eDrawType_Color43 */ 	"GrayMedium3",
	/* glow_eDrawType_Color44 */ 	"GrayMedium4",
	/* glow_eDrawType_Color45 */ 	"GrayMedium5",
	/* glow_eDrawType_Color46 */ 	"GrayMedium6",
	/* glow_eDrawType_Color47 */ 	"GrayMedium7",
	/* glow_eDrawType_Color48 */ 	"GrayMedium8",
	/* glow_eDrawType_Color49 */ 	"GrayMedium9",
	/* glow_eDrawType_Color50 */ 	"GrayMedium10",
	/* glow_eDrawType_Color51 */ 	"GrayHigh1",
	/* glow_eDrawType_Color52 */ 	"GrayHigh2",
	/* glow_eDrawType_Color53 */ 	"GrayHigh3",
	/* glow_eDrawType_Color54 */ 	"GrayHigh4",
	/* glow_eDrawType_Color55 */ 	"GrayHigh5",
	/* glow_eDrawType_Color56 */ 	"GrayHigh6",
	/* glow_eDrawType_Color57 */ 	"GrayHigh7",
	/* glow_eDrawType_Color58 */ 	"GrayHigh8",
	/* glow_eDrawType_Color59 */ 	"GrayHigh9",
	/* glow_eDrawType_Color60 */ 	"GrayHigh10",
	/* glow_eDrawType_Color61 */ 	"YellowGreenLow1",
	/* glow_eDrawType_Color62 */ 	"YellowGreenLow2",
	/* glow_eDrawType_Color63 */ 	"YellowGreenLow3",
	/* glow_eDrawType_Color64 */ 	"YellowGreenLow4",
	/* glow_eDrawType_Color65 */ 	"YellowGreenLow5",
	/* glow_eDrawType_Color66 */ 	"YellowGreenLow6",
	/* glow_eDrawType_Color67 */ 	"YellowGreenLow7",
	/* glow_eDrawType_Color68 */ 	"YellowGreenLow8",
	/* glow_eDrawType_Color69 */ 	"YellowGreenLow9",
	/* glow_eDrawType_Color70 */ 	"YellowGreenLow10",
	/* glow_eDrawType_Color71 */ 	"YellowGreenMedium1",
	/* glow_eDrawType_Color72 */ 	"YellowGreenMedium2",
	/* glow_eDrawType_Color73 */ 	"YellowGreenMedium3",
	/* glow_eDrawType_Color74 */ 	"YellowGreenMedium4",
	/* glow_eDrawType_Color75 */ 	"YellowGreenMedium5",
	/* glow_eDrawType_Color76 */ 	"YellowGreenMedium6",
	/* glow_eDrawType_Color77 */ 	"YellowGreenMedium7",
	/* glow_eDrawType_Color78 */ 	"YellowGreenMedium8",
	/* glow_eDrawType_Color79 */ 	"YellowGreenMedium9",
	/* glow_eDrawType_Color80 */ 	"YellowGreenMedium10",
	/* glow_eDrawType_Color81 */ 	"YellowGreenHigh1",
	/* glow_eDrawType_Color82 */ 	"YellowGreenHigh2",
	/* glow_eDrawType_Color83 */ 	"YellowGreenHigh3",
	/* glow_eDrawType_Color84 */ 	"YellowGreenHigh4",
	/* glow_eDrawType_Color85 */ 	"YellowGreenHigh5",
	/* glow_eDrawType_Color86 */ 	"YellowGreenHigh6",
	/* glow_eDrawType_Color87 */ 	"YellowGreenHigh7",
	/* glow_eDrawType_Color88 */ 	"YellowGreenHigh8",
	/* glow_eDrawType_Color89 */ 	"YellowGreenHigh9",
	/* glow_eDrawType_Color90 */ 	"YellowGreenHigh10",
	/* glow_eDrawType_Color91 */ 	"YellowLow1",
	/* glow_eDrawType_Color92 */ 	"YellowLow2",
	/* glow_eDrawType_Color93 */ 	"YellowLow3",
	/* glow_eDrawType_Color94 */ 	"YellowLow4",
	/* glow_eDrawType_Color95 */ 	"YellowLow5",
	/* glow_eDrawType_Color96 */ 	"YellowLow6",
	/* glow_eDrawType_Color97 */ 	"YellowLow7",
	/* glow_eDrawType_Color98 */ 	"YellowLow8",
	/* glow_eDrawType_Color99 */ 	"YellowLow9",
	/* glow_eDrawType_Color100 */ 	"YellowLow10",
	/* glow_eDrawType_Color101 */ 	"YellowMedium1",
	/* glow_eDrawType_Color102 */ 	"YellowMedium2",
	/* glow_eDrawType_Color103 */ 	"YellowMedium3",
	/* glow_eDrawType_Color104 */ 	"YellowMedium4",
	/* glow_eDrawType_Color105 */ 	"YellowMedium5",
	/* glow_eDrawType_Color106 */ 	"YellowMedium6",
	/* glow_eDrawType_Color107 */ 	"YellowMedium7",
	/* glow_eDrawType_Color108 */ 	"YellowMedium8",
	/* glow_eDrawType_Color109 */ 	"YellowMedium9",
	/* glow_eDrawType_Color110 */ 	"YellowMedium10",
	/* glow_eDrawType_Color111 */ 	"YellowHigh1",
	/* glow_eDrawType_Color112 */ 	"YellowHigh2",
	/* glow_eDrawType_Color113 */ 	"YellowHigh3",
	/* glow_eDrawType_Color114 */ 	"YellowHigh4",
	/* glow_eDrawType_Color115 */ 	"YellowHigh5",
	/* glow_eDrawType_Color116 */ 	"YellowHigh6",
	/* glow_eDrawType_Color117 */ 	"YellowHigh7",
	/* glow_eDrawType_Color118 */ 	"YellowHigh8",
	/* glow_eDrawType_Color119 */ 	"YellowHigh9",
	/* glow_eDrawType_Color120 */ 	"YellowHigh10",
	/* glow_eDrawType_Color121 */ 	"OrangeLow1",
	/* glow_eDrawType_Color122 */ 	"OrangeLow2",
	/* glow_eDrawType_Color123 */ 	"OrangeLow3",
	/* glow_eDrawType_Color124 */ 	"OrangeLow4",
	/* glow_eDrawType_Color125 */ 	"OrangeLow5",
	/* glow_eDrawType_Color126 */ 	"OrangeLow6",
	/* glow_eDrawType_Color127 */ 	"OrangeLow7",
	/* glow_eDrawType_Color128 */ 	"OrangeLow8",
	/* glow_eDrawType_Color129 */ 	"OrangeLow9",
	/* glow_eDrawType_Color130 */ 	"OrangeLow10",
	/* glow_eDrawType_Color131 */ 	"OrangeMedium1",
	/* glow_eDrawType_Color132 */ 	"OrangeMedium2",
	/* glow_eDrawType_Color133 */ 	"OrangeMedium3",
	/* glow_eDrawType_Color134 */ 	"OrangeMedium4",
	/* glow_eDrawType_Color135 */ 	"OrangeMedium5",
	/* glow_eDrawType_Color136 */ 	"OrangeMedium6",
	/* glow_eDrawType_Color137 */ 	"OrangeMedium7",
	/* glow_eDrawType_Color138 */ 	"OrangeMedium8",
	/* glow_eDrawType_Color139 */ 	"OrangeMedium9",
	/* glow_eDrawType_Color140 */ 	"OrangeMedium10",
	/* glow_eDrawType_Color141 */ 	"OrangeHigh1",
	/* glow_eDrawType_Color142 */ 	"OrangeHigh2",
	/* glow_eDrawType_Color143 */ 	"OrangeHigh3",
	/* glow_eDrawType_Color144 */ 	"OrangeHigh4",
	/* glow_eDrawType_Color145 */ 	"OrangeHigh5",
	/* glow_eDrawType_Color146 */ 	"OrangeHigh6",
	/* glow_eDrawType_Color147 */ 	"OrangeHigh7",
	/* glow_eDrawType_Color148 */ 	"OrangeHigh8",
	/* glow_eDrawType_Color149 */ 	"OrangeHigh9",
	/* glow_eDrawType_Color150 */ 	"OrangeHigh10",
	/* glow_eDrawType_Color151 */ 	"RedLow1",
	/* glow_eDrawType_Color152 */ 	"RedLow2",
	/* glow_eDrawType_Color153 */ 	"RedLow3",
	/* glow_eDrawType_Color154 */ 	"RedLow4",
	/* glow_eDrawType_Color155 */ 	"RedLow5",
	/* glow_eDrawType_Color156 */ 	"RedLow6",
	/* glow_eDrawType_Color157 */ 	"RedLow7",
	/* glow_eDrawType_Color158 */ 	"RedLow8",
	/* glow_eDrawType_Color159 */ 	"RedLow9",
	/* glow_eDrawType_Color160 */ 	"RedLow10",
	/* glow_eDrawType_Color161 */ 	"RedMedium1",
	/* glow_eDrawType_Color162 */ 	"RedMedium2",
	/* glow_eDrawType_Color163 */ 	"RedMedium3",
	/* glow_eDrawType_Color164 */ 	"RedMedium4",
	/* glow_eDrawType_Color165 */ 	"RedMedium5",
	/* glow_eDrawType_Color166 */ 	"RedMedium6",
	/* glow_eDrawType_Color167 */ 	"RedMedium7",
	/* glow_eDrawType_Color168 */ 	"RedMedium8",
	/* glow_eDrawType_Color169 */ 	"RedMedium9",
	/* glow_eDrawType_Color170 */ 	"RedMedium10",
	/* glow_eDrawType_Color171 */ 	"RedHigh1",
	/* glow_eDrawType_Color172 */ 	"RedHigh2",
	/* glow_eDrawType_Color173 */ 	"RedHigh3",
	/* glow_eDrawType_Color174 */ 	"RedHigh4",
	/* glow_eDrawType_Color175 */ 	"RedHigh5",
	/* glow_eDrawType_Color176 */ 	"RedHigh6",
	/* glow_eDrawType_Color177 */ 	"RedHigh7",
	/* glow_eDrawType_Color178 */ 	"RedHigh8",
	/* glow_eDrawType_Color179 */ 	"RedHigh9",
	/* glow_eDrawType_Color180 */ 	"RedHigh10",
	/* glow_eDrawType_Color181 */ 	"MagentaLow1",
	/* glow_eDrawType_Color182 */ 	"MagentaLow2",
	/* glow_eDrawType_Color183 */ 	"MagentaLow3",
	/* glow_eDrawType_Color184 */ 	"MagentaLow4",
	/* glow_eDrawType_Color185 */ 	"MagentaLow5",
	/* glow_eDrawType_Color186 */ 	"MagentaLow6",
	/* glow_eDrawType_Color187 */ 	"MagentaLow7",
	/* glow_eDrawType_Color188 */ 	"MagentaLow8",
	/* glow_eDrawType_Color189 */ 	"MagentaLow9",
	/* glow_eDrawType_Color190 */ 	"MagentaLow10",
	/* glow_eDrawType_Color191 */ 	"MagentaMedium1",
	/* glow_eDrawType_Color192 */ 	"MagentaMedium2",
	/* glow_eDrawType_Color193 */ 	"MagentaMedium3",
	/* glow_eDrawType_Color194 */ 	"MagentaMedium4",
	/* glow_eDrawType_Color195 */ 	"MagentaMedium5",
	/* glow_eDrawType_Color196 */ 	"MagentaMedium6",
	/* glow_eDrawType_Color197 */ 	"MagentaMedium7",
	/* glow_eDrawType_Color198 */ 	"MagentaMedium8",
	/* glow_eDrawType_Color199 */ 	"MagentaMedium9",
	/* glow_eDrawType_Color200 */ 	"MagentaMedium10",
	/* glow_eDrawType_Color201 */ 	"MagentaHigh1",
	/* glow_eDrawType_Color202 */ 	"MagentaHigh2",
	/* glow_eDrawType_Color203 */ 	"MagentaHigh3",
	/* glow_eDrawType_Color204 */ 	"MagentaHigh4",
	/* glow_eDrawType_Color205 */ 	"MagentaHigh5",
	/* glow_eDrawType_Color206 */ 	"MagentaHigh6",
	/* glow_eDrawType_Color207 */ 	"MagentaHigh7",
	/* glow_eDrawType_Color208 */ 	"MagentaHigh8",
	/* glow_eDrawType_Color209 */ 	"MagentaHigh9",
	/* glow_eDrawType_Color210 */ 	"MagentaHigh10",
	/* glow_eDrawType_Color211 */ 	"BlueLow1",
	/* glow_eDrawType_Color212 */ 	"BlueLow2",
	/* glow_eDrawType_Color213 */ 	"BlueLow3",
	/* glow_eDrawType_Color214 */ 	"BlueLow4",
	/* glow_eDrawType_Color215 */ 	"BlueLow5",
	/* glow_eDrawType_Color216 */ 	"BlueLow6",
	/* glow_eDrawType_Color217 */ 	"BlueLow7",
	/* glow_eDrawType_Color218 */ 	"BlueLow8",
	/* glow_eDrawType_Color219 */ 	"BlueLow9",
	/* glow_eDrawType_Color220 */ 	"BlueLow10",
	/* glow_eDrawType_Color221 */ 	"BlueMedium1",
	/* glow_eDrawType_Color222 */ 	"BlueMedium2",
	/* glow_eDrawType_Color223 */ 	"BlueMedium3",
	/* glow_eDrawType_Color224 */ 	"BlueMedium4",
	/* glow_eDrawType_Color225 */ 	"BlueMedium5",
	/* glow_eDrawType_Color226 */ 	"BlueMedium6",
	/* glow_eDrawType_Color227 */ 	"BlueMedium7",
	/* glow_eDrawType_Color228 */ 	"BlueMedium8",
	/* glow_eDrawType_Color229 */ 	"BlueMedium9",
	/* glow_eDrawType_Color230 */ 	"BlueMedium10",
	/* glow_eDrawType_Color231 */ 	"BlueHigh1",
	/* glow_eDrawType_Color232 */ 	"BlueHigh2",
	/* glow_eDrawType_Color233 */ 	"BlueHigh3",
	/* glow_eDrawType_Color234 */ 	"BlueHigh4",
	/* glow_eDrawType_Color235 */ 	"BlueHigh5",
	/* glow_eDrawType_Color236 */ 	"BlueHigh6",
	/* glow_eDrawType_Color237 */ 	"BlueHigh7",
	/* glow_eDrawType_Color238 */ 	"BlueHigh8",
	/* glow_eDrawType_Color239 */ 	"BlueHigh9",
	/* glow_eDrawType_Color240 */ 	"BlueHigh10",
	/* glow_eDrawType_Color241 */ 	"SeaBlueLow1",
	/* glow_eDrawType_Color242 */ 	"SeaBlueLow2",
	/* glow_eDrawType_Color243 */ 	"SeaBlueLow3",
	/* glow_eDrawType_Color244 */ 	"SeaBlueLow4",
	/* glow_eDrawType_Color245 */ 	"SeaBlueLow5",
	/* glow_eDrawType_Color246 */ 	"SeaBlueLow6",
	/* glow_eDrawType_Color247 */ 	"SeaBlueLow7",
	/* glow_eDrawType_Color248 */ 	"SeaBlueLow8",
	/* glow_eDrawType_Color249 */ 	"SeaBlueLow9",
	/* glow_eDrawType_Color250 */ 	"SeaBlueLow10",
	/* glow_eDrawType_Color251 */ 	"SeaBlueMedium1",
	/* glow_eDrawType_Color252 */ 	"SeaBlueMedium2",
	/* glow_eDrawType_Color253 */ 	"SeaBlueMedium3",
	/* glow_eDrawType_Color224 */ 	"SeaBlueMedium4",
	/* glow_eDrawType_Color255 */ 	"SeaBlueMedium5",
	/* glow_eDrawType_Color256 */ 	"SeaBlueMedium6",
	/* glow_eDrawType_Color257 */ 	"SeaBlueMedium7",
	/* glow_eDrawType_Color258 */ 	"SeaBlueMedium8",
	/* glow_eDrawType_Color259 */ 	"SeaBlueMedium9",
	/* glow_eDrawType_Color260 */ 	"SeaBlueMedium10",
	/* glow_eDrawType_Color261 */ 	"SeaBlueHigh1",
	/* glow_eDrawType_Color262 */ 	"SeaBlueHigh2",
	/* glow_eDrawType_Color263 */ 	"SeaBlueHigh3",
	/* glow_eDrawType_Color264 */ 	"SeaBlueHigh4",
	/* glow_eDrawType_Color265 */ 	"SeaBlueHigh5",
	/* glow_eDrawType_Color266 */ 	"SeaBlueHigh6",
	/* glow_eDrawType_Color267 */ 	"SeaBlueHigh7",
	/* glow_eDrawType_Color268 */ 	"SeaBlueHigh8",
	/* glow_eDrawType_Color269 */ 	"SeaBlueHigh9",
	/* glow_eDrawType_Color270 */ 	"SeaBlueHigh10",
	/* glow_eDrawType_Color271 */ 	"GreenLow1",
	/* glow_eDrawType_Color272 */ 	"GreenLow2",
	/* glow_eDrawType_Color273 */ 	"GreenLow3",
	/* glow_eDrawType_Color274 */ 	"GreenLow4",
	/* glow_eDrawType_Color275 */ 	"GreenLow5",
	/* glow_eDrawType_Color276 */ 	"GreenLow6",
	/* glow_eDrawType_Color277 */ 	"GreenLow7",
	/* glow_eDrawType_Color278 */ 	"GreenLow8",
	/* glow_eDrawType_Color279 */ 	"GreenLow9",
	/* glow_eDrawType_Color280 */ 	"GreenLow10",
	/* glow_eDrawType_Color281 */ 	"GreenMedium1",
	/* glow_eDrawType_Color282 */ 	"GreenMedium2",
	/* glow_eDrawType_Color283 */ 	"GreenMedium3",
	/* glow_eDrawType_Color284 */ 	"GreenMedium4",
	/* glow_eDrawType_Color285 */ 	"GreenMedium5",
	/* glow_eDrawType_Color286 */ 	"GreenMedium6",
	/* glow_eDrawType_Color287 */ 	"GreenMedium7",
	/* glow_eDrawType_Color288 */ 	"GreenMedium8",
	/* glow_eDrawType_Color289 */ 	"GreenMedium9",
	/* glow_eDrawType_Color290 */ 	"GreenMedium10",
	/* glow_eDrawType_Color291 */ 	"GreenHigh1",
	/* glow_eDrawType_Color292 */ 	"GreenHigh2",
	/* glow_eDrawType_Color293 */ 	"GreenHigh3",
	/* glow_eDrawType_Color294 */ 	"GreenHigh4",
	/* glow_eDrawType_Color295 */ 	"GreenHigh5",
	/* glow_eDrawType_Color296 */ 	"GreenHigh6",
	/* glow_eDrawType_Color297 */ 	"GreenHigh7",
	/* glow_eDrawType_Color298 */ 	"GreenHigh8",
	/* glow_eDrawType_Color299 */ 	"GreenHigh9",
        /* glow_eDrawType_Color300 */  	"GreenHigh10",
        /* glow_eDrawType_LineErase */  "Background"};
  static char color_name[32];

  if ( (unsigned int) color > 
        sizeof(default_color_names)/sizeof(default_color_names[0])-1)
    strcpy( color_name, "");
  else
    strcpy( color_name, default_color_names[color]);

  return color_name;
}

char *GlowColor::colortone_to_name( glow_eDrawType drawtype)
{
  char color_tone_names[][40] = {
	/* glow_eDrawTone_No */ 				"NoTone",
	/* glow_eDrawTone_Gray */ 				"ToneGray",
	/* glow_eDrawTone_YellowGreen */    		        "ToneYellowGreen",
	/* glow_eDrawTone_Yellow */ 				"ToneYellow",
	/* glow_eDrawTone_Orange */ 				"ToneOrange",
	/* glow_eDrawTone_Red */ 				"ToneRed",
	/* glow_eDrawTone_Magenta */ 				"ToneMagenta",
	/* glow_eDrawTone_Blue */ 				"ToneBlue",
	/* glow_eDrawTone_Seablue */ 				"ToneSeablue",
	/* glow_eDrawTone_Green */ 				"ToneGreen",
	/* glow_eDrawTone_DarkGray */				"ToneDarkGray",
	/* glow_eDrawTone_LightGray */				"ToneLightGray",
	/* glow_eDrawTone_DarkYellowGreen */			"ToneDarkYellowGreen",
	/* glow_eDrawTone_LightYellowGreen */	       		"ToneLightYellowGreen",
	/* glow_eDrawTone_YellowGreenHighSaturation */      	"ToneYellowGreenHighSaturation",
	/* glow_eDrawTone_YellowGreenLowSaturation */       	"ToneYellowGreenLowSaturation",
	/* glow_eDrawTone_DarkYellowGreenHighSaturation */	"ToneDarkYellowGreenHighSaturation",
	/* glow_eDrawTone_DarkYellowGreenLowSaturation */   	"ToneDarkYellowGreenLowSaturation",
	/* glow_eDrawTone_LightYellowGreenHighSaturation */	"ToneLightYellowGreenHighSaturation",
	/* glow_eDrawTone_LightYellowGreenLowSaturation */	"ToneLightYellowGreenLowSaturation",
	/* glow_eDrawTone_DarkYellow */				"ToneDarkYellow",
	/* glow_eDrawTone_LightYellow */			"ToneLightYellow",
	/* glow_eDrawTone_YellowHighSaturation */		"ToneYellowHighSaturation",
	/* glow_eDrawTone_YellowLowSaturation */		"ToneYellowLowSaturation",
	/* glow_eDrawTone_DarkYellowHighSaturation */       	"ToneDarkYellowHighSaturation",
	/* glow_eDrawTone_DarkYellowLowSaturation */		"ToneDarkYellowLowSaturation",
	/* glow_eDrawTone_LightYellowHighSaturation */      	"ToneLightYellowHighSaturation",
	/* glow_eDrawTone_LightYellowLowSaturation */       	"ToneLightYellowLowSaturation",
	/* glow_eDrawTone_DarkOrange */				"ToneDarkOrange",
	/* glow_eDrawTone_LightOrange */			"ToneLightOrange",
	/* glow_eDrawTone_OrangeHighSaturation */		"ToneOrangeHighSaturation",
	/* glow_eDrawTone_OrangeLowSaturation */		"ToneOrangeLowSaturation",
	/* glow_eDrawTone_DarkOrangeHighSaturation */		"ToneDarkOrangeHighSaturation",
	/* glow_eDrawTone_DarkOrangeLowSaturation */		"ToneDarkOrangeLowSaturation",
	/* glow_eDrawTone_LightOrangeHighSaturation */		"ToneLightOrangeHighSaturation",
	/* glow_eDrawTone_LightOrangeLowSaturation */		"ToneLightOrangeLowSaturation",
	/* glow_eDrawTone_DarkRed */				"ToneDarkRed",
	/* glow_eDrawTone_LightRed */				"ToneLightRed",
	/* glow_eDrawTone_RedHighSaturation */			"ToneRedHighSaturation",
	/* glow_eDrawTone_RedLowSaturation */			"ToneRedLowSaturation",
	/* glow_eDrawTone_DarkRedHighSaturation */		"ToneDarkRedHighSaturation",
	/* glow_eDrawTone_DarkRedLowSaturation */		"ToneDarkRedLowSaturation",
	/* glow_eDrawTone_LightRedHighSaturation */		"ToneLightRedHighSaturation",
	/* glow_eDrawTone_LightRedLowSaturation */		"ToneLightRedLowSaturation",
	/* glow_eDrawTone_DarkMagenta */			"ToneDarkMagenta",
	/* glow_eDrawTone_LightMagenta */			"ToneLightMagenta",
	/* glow_eDrawTone_MagentaHighSaturation */		"ToneMagentaHighSaturation",
	/* glow_eDrawTone_MagentaLowSaturation */		"ToneMagentaLowSaturation",
	/* glow_eDrawTone_DarkMagentaHighSaturation */		"ToneDarkMagentaHighSaturation",
	/* glow_eDrawTone_DarkMagentaLowSaturation */		"ToneDarkMagentaLowSaturation",
	/* glow_eDrawTone_LightMagentaHighSaturation */		"ToneLightMagentaHighSaturation",
	/* glow_eDrawTone_LightMagentaLowSaturation */		"ToneLightMagentaLowSaturation",
	/* glow_eDrawTone_DarkBlue */				"ToneDarkBlue",
	/* glow_eDrawTone_LightBlue */				"ToneLightBlue",
	/* glow_eDrawTone_BlueHighSaturation */			"ToneBlueHighSaturation",
	/* glow_eDrawTone_BlueLowSaturation */			"ToneBlueLowSaturation",
	/* glow_eDrawTone_DarkBlueHighSaturation */		"ToneDarkBlueHighSaturation",
	/* glow_eDrawTone_DarkBlueLowSaturation */		"ToneDarkBlueLowSaturation",
	/* glow_eDrawTone_LightBlueHighSaturation */		"ToneLightBlueHighSaturation",
	/* glow_eDrawTone_LightBlueLowSaturation */		"ToneLightBlueLowSaturation",
	/* glow_eDrawTone_DarkSeablue */			"ToneDarkSeablue",
	/* glow_eDrawTone_LightSeablue */			"ToneLightSeablue",
	/* glow_eDrawTone_SeablueHighSaturation */		"ToneSeablueHighSaturation",
	/* glow_eDrawTone_SeablueLowSaturation */		"ToneSeablueLowSaturation",
	/* glow_eDrawTone_DarkSeablueHighSaturation */		"ToneDarkSeablueHighSaturation",
	/* glow_eDrawTone_DarkSeablueLowSaturation */	 	"ToneDarkSeablueLowSaturation",
	/* glow_eDrawTone_LightSeablueHighSaturation */		"ToneLightSeablueHighSaturation",
	/* glow_eDrawTone_LightSeablueLowSaturation */		"ToneLightSeablueLowSaturation",
	/* glow_eDrawTone_DarkGreen */				"ToneDarkGreen",
	/* glow_eDrawTone_LightGreen */				"ToneLightGreen",
	/* glow_eDrawTone_GreenHighSaturation */		"ToneGreenHighSaturation",
	/* glow_eDrawTone_GreenLowSaturation */			"ToneGreenLowSaturation",
	/* glow_eDrawTone_DarkGreenHighSaturation */		"ToneDarkGreenHighSaturation",
	/* glow_eDrawTone_DarkGreenLowSaturation */		"ToneDarkGreenLowSaturation",
	/* glow_eDrawTone_LightGreenHighSaturation */		"ToneLightGreenHighSaturation",
	/* glow_eDrawTone_LightGreenLowSaturation */		"ToneLightGreenLowSaturation"};

  static char tone_name[40];

  if ( (unsigned int) drawtype > 
        sizeof(color_tone_names)/sizeof(color_tone_names[0])-1)
    strcpy( tone_name, "");
  else
    strcpy( tone_name, color_tone_names[drawtype]);
  return tone_name;
}


