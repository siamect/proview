package jpwr.jop;
import java.awt.*;

public class GeColor {

  // Color tones
  public static final int COLOR_TONE_NO = 0;
  public static final int COLOR_TONE_GRAY = 1;
  public static final int COLOR_TONE_YELLOW = 2;
  public static final int COLOR_TONE_GOLD = 3;
  public static final int COLOR_TONE_ORANGE = 4;
  public static final int COLOR_TONE_RED = 5;
  public static final int COLOR_TONE_MAGENTA = 6;
  public static final int COLOR_TONE_BLUE = 7;
  public static final int COLOR_TONE_SEABLUE = 8;
  public static final int COLOR_TONE_GREEN = 9;
  public static final int COLOR_TONE_DARKGRAY = 10;
  public static final int COLOR_TONE_MAX = 10;

  public static final int COLOR_BLACK = 0;
  public static final int COLOR_RED = 1;
  public static final int COLOR_GRAY = 2;

  public static final int COLOR_1 = 0;
  public static final int COLOR_2 = 1;
  public static final int COLOR_3 = 2;
  public static final int COLOR_4 = 3;
  public static final int COLOR_5 = 4;
  public static final int COLOR_6 = 5;
  public static final int COLOR_7 = 6;
  public static final int COLOR_8 = 7;
  public static final int COLOR_9 = 8;
  public static final int COLOR_10 = 9;
  public static final int COLOR_11 = 10;
  public static final int COLOR_12 = 11;
  public static final int COLOR_13 = 12;
  public static final int COLOR_14 = 13;
  public static final int COLOR_15 = 14;
  public static final int COLOR_16 = 15;
  public static final int COLOR_17 = 16;
  public static final int COLOR_18 = 17;
  public static final int COLOR_19 = 18;
  public static final int COLOR_20 = 19;
  public static final int COLOR_21 = 20;
  public static final int COLOR_22 = 21;
  public static final int COLOR_23 = 22;
  public static final int COLOR_24 = 23;
  public static final int COLOR_25 = 24;
  public static final int COLOR_26 = 25;
  public static final int COLOR_27 = 26;
  public static final int COLOR_28 = 27;
  public static final int COLOR_29 = 28;
  public static final int COLOR_30 = 29;
  public static final int COLOR_31 = 30;
  public static final int COLOR_32 = 31;
  public static final int COLOR_33 = 32;
  public static final int COLOR_34 = 33;
  public static final int COLOR_35 = 34;
  public static final int COLOR_36 = 35;
  public static final int COLOR_37 = 36;
  public static final int COLOR_38 = 37;
  public static final int COLOR_39 = 38;
  public static final int COLOR_40 = 39;
  public static final int COLOR_41 = 40;
  public static final int COLOR_42 = 41;
  public static final int COLOR_43 = 42;
  public static final int COLOR_44 = 43;
  public static final int COLOR_45 = 44;
  public static final int COLOR_46 = 45;
  public static final int COLOR_47 = 46;
  public static final int COLOR_48 = 47;
  public static final int COLOR_49 = 48;
  public static final int COLOR_50 = 49;
  public static final int COLOR_51 = 50;
  public static final int COLOR_52 = 51;
  public static final int COLOR_53 = 52;
  public static final int COLOR_54 = 53;
  public static final int COLOR_55 = 54;
  public static final int COLOR_56 = 55;
  public static final int COLOR_57 = 56;
  public static final int COLOR_58 = 57;
  public static final int COLOR_59 = 58;
  public static final int COLOR_60 = 59;
  public static final int COLOR_61 = 60;
  public static final int COLOR_62 = 61;
  public static final int COLOR_63 = 62;
  public static final int COLOR_64 = 63;
  public static final int COLOR_65 = 64;
  public static final int COLOR_66 = 65;
  public static final int COLOR_67 = 66;
  public static final int COLOR_68 = 67;
  public static final int COLOR_69 = 68;
  public static final int COLOR_70 = 69;
  public static final int COLOR_71 = 70;
  public static final int COLOR_72 = 71;
  public static final int COLOR_73 = 72;
  public static final int COLOR_74 = 73;
  public static final int COLOR_75 = 74;
  public static final int COLOR_76 = 75;
  public static final int COLOR_77 = 76;
  public static final int COLOR_78 = 77;
  public static final int COLOR_79 = 78;
  public static final int COLOR_80 = 79;
  public static final int COLOR_81 = 80;
  public static final int COLOR_82 = 81;
  public static final int COLOR_83 = 82;
  public static final int COLOR_84 = 83;
  public static final int COLOR_85 = 84;
  public static final int COLOR_86 = 85;
  public static final int COLOR_87 = 86;
  public static final int COLOR_88 = 87;
  public static final int COLOR_89 = 88;
  public static final int COLOR_90 = 89;
  public static final int COLOR_91 = 90;
  public static final int COLOR_92 = 91;
  public static final int COLOR_93 = 92;
  public static final int COLOR_94 = 93;
  public static final int COLOR_95 = 94;
  public static final int COLOR_96 = 95;
  public static final int COLOR_97 = 96;
  public static final int COLOR_98 = 97;
  public static final int COLOR_99 = 98;
  public static final int COLOR_100 = 99;
  public static final int COLOR_INHERIT = 9999;

  static final double colorValues[] = {
0, 		0, 		0,		// 4 Black
1, 		0.2, 		0.2,		// 4 Red
0.7, 		0.7, 		0.7,		// 4 Gray
1, 		1, 		1,		// 4 White
0,		0.8714, 	0.2857,		// 5 SpringGreen3
1, 		1, 		0,		// 6 Yellow
0.2879, 	0.4945, 	0.9067,		// 7 SteelBlue3
0.593, 		0, 		0.6421,		// 8 Magenta3
0.270, 		0.270, 		0.270,		// 9 Grey
0.160, 		0.160,		0.160,		// 10 Grey
// Grey
0.950, 		0.950, 		0.950,		// 1 Grey
0.900, 		0.900, 		0.900,		// 2 Grey
0.850, 		0.850, 		0.850,		// 3 Grey
0.700, 		0.700, 		0.700,		// 4 Grey93
0.500, 		0.500, 		0.500,		// 5 Grey80
0.935, 		0.935, 		0.935,		// 6 Grey68
0.880, 		0.880, 		0.880,		// 7 Grey48
0.750, 		0.750, 		0.750,		// 8 Grey35
0.550, 		0.550, 		0.550,		// 9 Grey
0.350, 		0.350, 		0.350,		// 10 Grey
1,		1,		0.878,		// 1 LightYellow0
1,		1,		0.640,		// 2 LightYellow1
0.900,		0.900,		0.500,		// 3 LightYellow2
0.804,		0.804,		0.400,		// 4 LightYellow3
0.545,		0.545,		0.300,		// 5 LightYellow4
1,		1,		0.300,		// 6 yellow0
1,		1,		0,		// 7 yellow1
0.933,		0.933,		0,		// 8 yellow2
0.804,		0.804,		0,		// 9 yellow3
0.545,		0.545,		0,		// 10 yellow4
1,		0.970,		0.800,		// 1 Lightgoldenrod0
1,		0.925,		0.545,		// 2 Lightgoldenrod1
0.933,		0.862,		0.510,		// 3 Lightgoldenrod2
0.804,		0.745,		0.439,		// 4 Lightgoldenrod3
0.545,		0.506,		0.298,		// 5 Lightgoldenrod4
1,		0.930,		0.500,		// 6 gold00
1,		0.843,		0,		// 7 gold1
0.933,		0.788,		0,		// 8 gold2
0.804,		0.678,		0,		// 9 gold3
0.545,		0.459,		0,		// 10 gold4
1,		0.941,		0.800,		// 1 LightOrange (Modified)
0.950,		0.870,		0.678,		// 2 NavajoWhite
0.900,		0.628,		0.478,		// 3 LightSalmon1
0.804,		0.506,		0.384,		// 4 LightSalmon3
0.545,		0.341,		0.259,		// 5 LightSalmon4
1,		0.850,		0.550,		// 6 DarkOrange (Modified)
1,		0.650,		0.300,		// 7 DarkOrange (Modified)
1,		0.498,		0,		// 8 DarkOrange1
0.804,		0.400,		0,		// 9 DarkOrange3
0.545,		0.153,		0,		// 10 DarkOrange4
1,		0.894,		0.867,		// 1 MistyRose1
0.933,		0.835,		0.823,		// 2 MistyRose2
0.933,		0.682,		0.725,		// 3 LightPink (Modified)
0.804,		0.547,		0.584,		// 4 LightPink3
0.545,		0.357,		0.396,		// 5 LightPink4
1,		0.733,		0.733,		// 6 IndianRed0 (Modified)
1,		0.415,		0.415,		// 7 IndianRed1
1,		0.188,		0.188,		// 8 firebrick1
0.804,		0.149,		0.149,		// 9 firebrick3
0.545,		0.102,		0.102,		// 10 firebrick4
1,		0.890,		1,		// 1 Plum1 (Modified)
1,		0.733,		1,		// 2 Plum1
0.933,		0.682,		0.933,		// 3 Plum2
0.804,		0.588,		0.804,		// 4 Plum3
0.545,		0.400,		0.545,		// 5 Plum4
1,		0.800,		1,		// 6 Magenta00 (Mofified)
1,		0.500,		1,		// 7 Magenta0 (Modified)
1,		0,		1,		// 8 Magenta1
0.804,		0,		0.804,		// 9 Magenta3
0.545,		0,		0.545,		// 10 Magenta4
0.829,		0.919,		1,		// 1 LightSteelBlue1 (Modified)
0.792,		0.882,		1,		// 2 LightSteelBlue1
0.737,		0.823,		0.933,		// 3 LightSteelBlue2
0.635,		0.710,		0.804,		// 4 LightSteelBlue3
0.431,		0.482,		0.545,		// 5 LightSteelBlue4
0.829,		0.919,		1,		// 6 LightCyan
0.690,		0.878,		0.902,		// 7 Powder blue
0.388,		0.722,		1,		// 8 SteelBlue1
0.275,		0.510,		0.706,		// 9 SteelBlue
0,		0,		0.545,		// 10 blue4
// Bluegreen
0.700,		0.960,		0.900,		// 1 aquamarine4 (Modified)
0.600,		0.930,		0.850,		// 2 aquamarine4
0.463,		0.900,		0.700,		// 3 aquamarine4
0.400,		0.804,		0.667,		// 4 aquamarine4
0.271,		0.545,		0.455,		// 5 aquamarine4
0.600,		1,		0.950,		// 6 turqoise
0,		0.930,		0.930,		// 7 turqoise
0,		0.870,		0.720,		// 8 turqoise
0,		0.700,		0.500,		// 9 turqoise
0,		0.500,		0.300,		// 10 turqoise
0.829,		1,		0.7,		// 1 DarkOliveGreen1 (Modified)
0.737,		0.933,		0.408,		// 2 DarkOliveGreen2
0.635,		0.804,		0.353,		// 3 DarkOliveGreen3
0.520,		0.700,		0.275,		// 4 DarkOliveGreen4
0.333,	  	0.420,		0.184,		// 5 DarkOliveGreen
0.698,		1,		0.400,		// 6 chartreuse
0.463,		0.933,		0,		// 7 chartreuse2
0.4,		0.804,		0,		// 8 chartreuse3
0.27,		0.545,		0,		// 9 chartreuse4
0,		0.392,		0};		// 10 DarkGreen
  static Color[] colors = new Color[100];
  public static final int NO_COLOR = 9999;
  public static final int NO_TONE = 0;
  public static Color getColor( int idx, int default_color) {
    if ( default_color < 100)
    {
//      System.out.println("Using default color");
      idx = default_color;
    }
//    System.out.println("Using ge color");
    if ( idx < 0 || idx > 99)
      idx = 1;
    if ( colors[idx] == null)
      colors[idx] = new Color( (int)(colorValues[3*idx] * 255),
		(int)(colorValues[3*idx + 1] * 255),
		(int)(colorValues[3*idx + 2] * 255));
    return colors[idx];
  }

  public static Color getColor( int local_drawtype, int color_tone, int color_shift,
	int color_intensity,
	int color_brightness, int color_inverse, int default_color) {
    int drawtype;
    int	base_drawtype;
    int	incr;

    if ( default_color == NO_COLOR && color_tone != NO_TONE) {
      if ( local_drawtype > 10) {
        if ( color_tone == COLOR_TONE_DARKGRAY) {
          switch( local_drawtype % 5) {
            case 0: drawtype = COLOR_14; break;
            case 1: drawtype = COLOR_19; break;
            case 2: drawtype = COLOR_15; break;
            case 3: drawtype = COLOR_20; break;
            default: drawtype = COLOR_9;
          }            
        }
        else
          drawtype = (local_drawtype - local_drawtype / 10 * 10 +
		10 * color_tone);
      }
      else
        drawtype = local_drawtype;
    }
    else if ( default_color != NO_COLOR) {
      drawtype = default_color;
    }
    else
      drawtype = local_drawtype;

    if ( color_brightness != 0) {
      if ( local_drawtype >= 10) {
        base_drawtype = drawtype / 5 * 5;
        incr = -color_brightness + drawtype - base_drawtype;
        if ( incr < 0)
          drawtype = 3; // White
        else if ( incr >= 5)
          drawtype = 9; // DarkGrey
        else
          drawtype = (base_drawtype + incr);
      }
    }
    if ( color_intensity != 0 ) {
      if ( drawtype >= 20) {
        base_drawtype = drawtype / 10 * 10;
        incr = drawtype - base_drawtype;
        if ( 0 <= incr && incr < 5) {
          if ( color_intensity > 0)
            drawtype = ( drawtype + 5);
          else
            drawtype = ( 15 + incr);
        }
        else {
          if ( color_intensity == -1)
            drawtype = ( drawtype - 5);
          else if ( color_intensity < 0)
            drawtype = ( 15 + incr - 5);
        }
      }
    }
    if ( color_shift != 0) {
      if ( drawtype >= 20) {
        incr = color_shift -
		color_shift / 8 * 8;
        if ( incr < 0)
          incr += 8;

        incr = drawtype + incr * 10;
        if ( incr >= 100)
          incr -= 80;
        drawtype = incr;
      }
    }
    if ( color_inverse == 1)
    {
      if ( drawtype >= 10)
        drawtype = drawtype + 5 - 2 * (drawtype % 5) - 1;
    }
    if ( drawtype < 0 || drawtype >= 100) {
      System.out.println("** Invalid drawtype");
      drawtype = 0;
    }
    if ( colors[drawtype] == null)
      colors[drawtype] = new Color( (int)(colorValues[3*drawtype] * 255),
		(int)(colorValues[3*drawtype + 1] * 255),
		(int)(colorValues[3*drawtype + 2] * 255));
    return colors[drawtype];
  }
}

