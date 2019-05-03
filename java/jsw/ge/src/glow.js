"use strict";

var Glow = {
  GLOW__TERMINATED: 21001,
  GLOW__SUBTERMINATED: 21003,
  GLOW__NO_PROPAGATE: 21005,

  DRAW_MP: 6,
  DRAW_TYPE_SIZE: 9,
  DRAW_FONT_SIZE: 9,
  DRAWOFFSET: 2,
  TREND_MAX_CURVES: 11,
  TABLE_MAX_COL: 30,

  eType_Boolean: 0,
  eType_Int: 1,
  eType_Double: 2,
  eType_String: 3,
  eType_DynType1: 4,
  eType_ActionType1: 5,
  eType_Direction: 6,
  eType_Color: 7,
  eType_Tone: 8,
  eType_ToneOrColor: 9,
  eType_TraceColor: 10,
  eType_Access: 11,
  eType_Cycle: 12,
  eType_MB3Action: 13,
  eType_Relief: 14,
  eType_InputFocusMark: 15,
  eType_TextSize: 16,
  eType_Adjustment: 17,
  eType_Font: 18,
  eType_Gradient: 19,
  eType_HotIndication: 20,
  eType_AnnotType: 21,
  eType_DynType2: 22,
  eType_ActionType2: 23,
  eType_AppMotion: 24,

  eCtxType_Glow: 0,
  eCtxType_Brow: 1,
  eCtxType_Grow: 2,
  eCtxType_ColPal: 3,
  eCtxType_Curve: 4,

  eSelectPolicy_Surround: 5,
  eSelectPolicy_Partial: 6,
  eSelectPolicy_Both: 7,

  eMB3Action_No: 0,
  eMB3Action_Close: 1,
  eMB3Action_PopupMenu: 2,
  eMB3Action_Both: 3,

  eTextSize_8: 0,
  eTextSize_10: 1,
  eTextSize_12: 2,
  eTextSize_14: 4,
  eTextSize_18: 6,
  eTextSize_24: 8,

  eTextCoding_ISO8859_1: 0,
  eTextCoding_UTF_8: 1,

  eLineType_Solid: 0,
  eLineType_Dashed1: 1,
  eLineType_Dashed2: 2,
  eLineType_Dashed3: 3,
  eLineType_Dotted: 4,
  eLineType_DotDashed1: 5,
  eLineType_DotDashed2: 6,

  eObjectType_NoObject: 0,
  eObjectType_Node: 1,
  eObjectType_Con: 2,
  eObjectType_Rect: 3,
  eObjectType_Line: 4,
  eObjectType_Arc: 5,
  eObjectType_Text: 6,
  eObjectType_ConPoint: 7,
  eObjectType_Annot: 8,
  eObjectType_NodeClass: 9,
  eObjectType_ConClass: 10,
  eObjectType_Arrow: 11,
  eObjectType_Pixmap: 12,
  eObjectType_AnnotPixmap: 13,
  eObjectType_Radiobutton: 14,
  eObjectType_Frame: 15,
  eObjectType_GrowRect: 16,
  eObjectType_GrowLine: 17,
  eObjectType_GrowArc: 18,
  eObjectType_GrowConPoint: 19,
  eObjectType_GrowSubAnnot: 20,
  eObjectType_PolyLine: 21,
  eObjectType_GrowPolyLine: 22,
  eObjectType_Point: 23,
  eObjectType_GrowNode: 24,
  eObjectType_GrowAnnot: 25,
  eObjectType_GrowText: 26,
  eObjectType_GrowBar: 27,
  eObjectType_GrowTrend: 28,
  eObjectType_GrowSlider: 29,
  eObjectType_GrowImage: 30,
  eObjectType_NodeGroup: 31,
  eObjectType_GrowGroup: 32,
  eObjectType_GrowAxis: 33,
  eObjectType_GrowRectRounded: 34,
  eObjectType_GrowConGlue: 35,
  eObjectType_GrowMenu: 36,
  eObjectType_GrowWindow: 37,
  eObjectType_GrowScrollBar: 38,
  eObjectType_GrowTable: 39,
  eObjectType_GrowFolder: 40,
  eObjectType_GrowXYCurve: 41,
  eObjectType_GrowAxisArc: 42,
  eObjectType_GrowPie: 43,
  eObjectType_GrowBarChart: 44,
  eObjectType_GrowToolbar: 45,
  eObjectType_GrowBarArc: 46,

  eDirection_Center: 0,
  eDirection_Right: 1,
  eDirection_Left: 2,
  eDirection_Up: 3,
  eDirection_Down: 4,

  eAdjustment_Center: 0,
  eAdjustment_Right: 1,
  eAdjustment_Left: 2,

  eDir_Vertical: 0,
  eDir_Horizontal: 1,

  eVis_Visible: 0,
  eVis_Invisible: 1,
  eVis_Dimmed: 2,

  eNodeGroup_Common: 0,
  eNodeGroup_Document: 1,
  eNodeGroup_Trace: 2,

  eConGroup_Common: 0,
  eConGroup_Trace: 1,

  eAnnotType_OneLine: 0,
  eAnnotType_MultiLine: 1,

  eCycle_Inherit: 0,
  eCycle_Slow: 1,
  eCycle_Fast: 2,

  eConType_Straight: 0,
  eConType_Fixed: 1,
  eConType_AllFixed: 2,
  eConType_Routed: 3,
  eConType_StepDiv: 4,
  eConType_StepConv: 5,
  eConType_TransDiv: 6,
  eConType_TransConv: 7,
  eConType_StraightOneArrow: 8,
  eConType_Reference: 9,

  eCorner_Rounded: 0,
  eCorner_Right: 1,

  eVisible_Full: 0,
  eVisible_Partial: 1,
  eVisible_Top: 2,
  eVisible_Bottom: 3,
  eVisible_Left: 4,
  eVisible_Right: 5,

  eFont_Helvetica: 0,
  eFont_Times: 1,
  eFont_NewCenturySchoolbook: 2,
  eFont_Courier: 3,
  eFont_LucidaSans: 4,
  eFont__: 5,
  eFont_No: 9999,

  eRelief_Up: 0,
  eRelief_Down: 1,

  eGradient_No: 0,
  eGradient_HorizontalUp: 1,
  eGradient_HorizontalDown: 2,
  eGradient_HorizontalTube1: 3,
  eGradient_HorizontalTube2: 4,
  eGradient_VerticalLeft: 5,
  eGradient_VerticalRight: 6,
  eGradient_VerticalTube1: 7,
  eGradient_VerticalTube2: 8,
  eGradient_DiagonalUpperLeft: 9,
  eGradient_DiagonalLowerLeft: 10,
  eGradient_DiagonalUpperRight: 11,
  eGradient_DiagonalLowerRight: 12,
  eGradient_DiagonalUpTube: 13,
  eGradient_DiagonalDownTube: 14,
  eGradient_Globe: 15,
  eGradient_RadialCenter: 16,
  eGradient_RadialUpperLeft: 17,
  eGradient_RadialLowerLeft: 18,
  eGradient_RadialUpperRight: 19,
  eGradient_RadialLowerRight: 20,

  eHotIndication_No: 0,
  eHotIndication_LineWidth: 1,
  eHotIndication_DarkColor: 2,
  eHotIndication_LightColor: 3,

  eAppMotion_Scroll: 0,
  eAppMotion_Slider: 1,
  eAppMotion_Both: 2,

  eDrawType_Line: 0,
  eDrawType_LineRed: 1,
  eDrawType_LineGray: 2,
  eDrawType_Color4: 3,
  eDrawType_Color5: 4,
  eDrawType_Color6: 5,
  eDrawType_Color7: 6,
  eDrawType_Color8: 7,
  eDrawType_Color9: 8,
  eDrawType_Color10: 9,
  eDrawType_Color11: 10,
  eDrawType_Color12: 11,
  eDrawType_Color13: 12,
  eDrawType_Color14: 13,
  eDrawType_Color15: 14,
  eDrawType_Color16: 15,
  eDrawType_Color17: 16,
  eDrawType_Color18: 17,
  eDrawType_Color19: 18,
  eDrawType_Color20: 19,
  eDrawType_Color21: 20,
  eDrawType_Color22: 21,
  eDrawType_Color23: 22,
  eDrawType_Color24: 23,
  eDrawType_Color25: 24,
  eDrawType_Color26: 25,
  eDrawType_Color27: 26,
  eDrawType_Color28: 27,
  eDrawType_Color29: 28,
  eDrawType_Color30: 29,
  eDrawType_Color31: 30,
  eDrawType_Color32: 31,
  eDrawType_Color33: 32,
  eDrawType_Color34: 33,
  eDrawType_Color35: 34,
  eDrawType_Color36: 35,
  eDrawType_Color37: 36,
  eDrawType_Color38: 37,
  eDrawType_Color39: 38,
  eDrawType_Color40: 39,
  eDrawType_Color41: 40,
  eDrawType_Color42: 41,
  eDrawType_Color43: 42,
  eDrawType_Color44: 43,
  eDrawType_Color45: 44,
  eDrawType_Color46: 45,
  eDrawType_Color47: 46,
  eDrawType_Color48: 47,
  eDrawType_Color49: 48,
  eDrawType_Color50: 49,
  eDrawType_Color51: 50,
  eDrawType_Color52: 51,
  eDrawType_Color53: 52,
  eDrawType_Color54: 53,
  eDrawType_Color55: 54,
  eDrawType_Color56: 55,
  eDrawType_Color57: 56,
  eDrawType_Color58: 57,
  eDrawType_Color59: 58,
  eDrawType_Color60: 59,
  eDrawType_Color61: 60,
  eDrawType_Color62: 61,
  eDrawType_Color63: 62,
  eDrawType_Color64: 63,
  eDrawType_Color65: 64,
  eDrawType_Color66: 65,
  eDrawType_Color67: 66,
  eDrawType_Color68: 67,
  eDrawType_Color69: 68,
  eDrawType_Color70: 69,
  eDrawType_Color71: 70,
  eDrawType_Color72: 71,
  eDrawType_Color73: 72,
  eDrawType_Color74: 73,
  eDrawType_Color75: 74,
  eDrawType_Color76: 75,
  eDrawType_Color77: 76,
  eDrawType_Color78: 77,
  eDrawType_Color79: 78,
  eDrawType_Color80: 79,
  eDrawType_Color81: 80,
  eDrawType_Color82: 81,
  eDrawType_Color83: 82,
  eDrawType_Color84: 83,
  eDrawType_Color85: 84,
  eDrawType_Color86: 85,
  eDrawType_Color87: 86,
  eDrawType_Color88: 87,
  eDrawType_Color89: 88,
  eDrawType_Color90: 89,
  eDrawType_Color91: 90,
  eDrawType_Color92: 91,
  eDrawType_Color93: 92,
  eDrawType_Color94: 93,
  eDrawType_Color95: 94,
  eDrawType_Color96: 95,
  eDrawType_Color97: 96,
  eDrawType_Color98: 97,
  eDrawType_Color99: 98,
  eDrawType_Color100: 99,
  eDrawType_Color101: 100,
  eDrawType_Color102: 101,
  eDrawType_Color103: 102,
  eDrawType_Color104: 103,
  eDrawType_Color105: 104,
  eDrawType_Color106: 105,
  eDrawType_Color107: 106,
  eDrawType_Color108: 107,
  eDrawType_Color109: 108,
  eDrawType_Color110: 109,
  eDrawType_Color111: 110,
  eDrawType_Color112: 111,
  eDrawType_Color113: 112,
  eDrawType_Color114: 113,
  eDrawType_Color115: 114,
  eDrawType_Color116: 115,
  eDrawType_Color117: 116,
  eDrawType_Color118: 117,
  eDrawType_Color119: 118,
  eDrawType_Color120: 119,
  eDrawType_Color121: 120,
  eDrawType_Color122: 121,
  eDrawType_Color123: 122,
  eDrawType_Color124: 123,
  eDrawType_Color125: 124,
  eDrawType_Color126: 125,
  eDrawType_Color127: 126,
  eDrawType_Color128: 127,
  eDrawType_Color129: 128,
  eDrawType_Color130: 129,
  eDrawType_Color131: 130,
  eDrawType_Color132: 131,
  eDrawType_Color133: 132,
  eDrawType_Color134: 133,
  eDrawType_Color135: 134,
  eDrawType_Color136: 135,
  eDrawType_Color137: 136,
  eDrawType_Color138: 137,
  eDrawType_Color139: 138,
  eDrawType_Color140: 139,
  eDrawType_Color141: 140,
  eDrawType_Color142: 141,
  eDrawType_Color143: 142,
  eDrawType_Color144: 143,
  eDrawType_Color145: 144,
  eDrawType_Color146: 145,
  eDrawType_Color147: 146,
  eDrawType_Color148: 147,
  eDrawType_Color149: 148,
  eDrawType_Color150: 149,
  eDrawType_Color151: 150,
  eDrawType_Color152: 151,
  eDrawType_Color153: 152,
  eDrawType_Color154: 153,
  eDrawType_Color155: 154,
  eDrawType_Color156: 155,
  eDrawType_Color157: 156,
  eDrawType_Color158: 157,
  eDrawType_Color159: 158,
  eDrawType_Color160: 159,
  eDrawType_Color161: 160,
  eDrawType_Color162: 161,
  eDrawType_Color163: 162,
  eDrawType_Color164: 163,
  eDrawType_Color165: 164,
  eDrawType_Color166: 165,
  eDrawType_Color167: 166,
  eDrawType_Color168: 167,
  eDrawType_Color169: 168,
  eDrawType_Color170: 169,
  eDrawType_Color171: 170,
  eDrawType_Color172: 171,
  eDrawType_Color173: 172,
  eDrawType_Color174: 173,
  eDrawType_Color175: 174,
  eDrawType_Color176: 175,
  eDrawType_Color177: 176,
  eDrawType_Color178: 177,
  eDrawType_Color179: 178,
  eDrawType_Color180: 179,
  eDrawType_Color181: 180,
  eDrawType_Color182: 181,
  eDrawType_Color183: 182,
  eDrawType_Color184: 183,
  eDrawType_Color185: 184,
  eDrawType_Color186: 185,
  eDrawType_Color187: 186,
  eDrawType_Color188: 187,
  eDrawType_Color189: 188,
  eDrawType_Color190: 189,
  eDrawType_Color191: 190,
  eDrawType_Color192: 191,
  eDrawType_Color193: 192,
  eDrawType_Color194: 193,
  eDrawType_Color195: 194,
  eDrawType_Color196: 195,
  eDrawType_Color197: 196,
  eDrawType_Color198: 197,
  eDrawType_Color199: 198,
  eDrawType_Color200: 199,
  eDrawType_Color201: 200,
  eDrawType_Color202: 201,
  eDrawType_Color203: 202,
  eDrawType_Color204: 203,
  eDrawType_Color205: 204,
  eDrawType_Color206: 205,
  eDrawType_Color207: 206,
  eDrawType_Color208: 207,
  eDrawType_Color209: 208,
  eDrawType_Color210: 209,
  eDrawType_Color211: 210,
  eDrawType_Color212: 211,
  eDrawType_Color213: 212,
  eDrawType_Color214: 213,
  eDrawType_Color215: 214,
  eDrawType_Color216: 215,
  eDrawType_Color217: 216,
  eDrawType_Color218: 217,
  eDrawType_Color219: 218,
  eDrawType_Color220: 219,
  eDrawType_Color221: 220,
  eDrawType_Color222: 221,
  eDrawType_Color223: 222,
  eDrawType_Color224: 223,
  eDrawType_Color225: 224,
  eDrawType_Color226: 225,
  eDrawType_Color227: 226,
  eDrawType_Color228: 227,
  eDrawType_Color229: 228,
  eDrawType_Color230: 229,
  eDrawType_Color231: 230,
  eDrawType_Color232: 231,
  eDrawType_Color233: 232,
  eDrawType_Color234: 233,
  eDrawType_Color235: 234,
  eDrawType_Color236: 235,
  eDrawType_Color237: 236,
  eDrawType_Color238: 237,
  eDrawType_Color239: 238,
  eDrawType_Color240: 239,
  eDrawType_Color241: 240,
  eDrawType_Color242: 241,
  eDrawType_Color243: 242,
  eDrawType_Color244: 243,
  eDrawType_Color245: 244,
  eDrawType_Color246: 245,
  eDrawType_Color247: 246,
  eDrawType_Color248: 247,
  eDrawType_Color249: 248,
  eDrawType_Color250: 249,
  eDrawType_Color251: 250,
  eDrawType_Color252: 251,
  eDrawType_Color253: 252,
  eDrawType_Color254: 253,
  eDrawType_Color255: 254,
  eDrawType_Color256: 255,
  eDrawType_Color257: 256,
  eDrawType_Color258: 257,
  eDrawType_Color259: 258,
  eDrawType_Color260: 259,
  eDrawType_Color261: 260,
  eDrawType_Color262: 261,
  eDrawType_Color263: 262,
  eDrawType_Color264: 263,
  eDrawType_Color265: 264,
  eDrawType_Color266: 265,
  eDrawType_Color267: 266,
  eDrawType_Color268: 267,
  eDrawType_Color269: 268,
  eDrawType_Color270: 269,
  eDrawType_Color271: 270,
  eDrawType_Color272: 271,
  eDrawType_Color273: 272,
  eDrawType_Color274: 273,
  eDrawType_Color275: 274,
  eDrawType_Color276: 275,
  eDrawType_Color277: 276,
  eDrawType_Color278: 277,
  eDrawType_Color279: 278,
  eDrawType_Color280: 279,
  eDrawType_Color281: 280,
  eDrawType_Color282: 281,
  eDrawType_Color283: 282,
  eDrawType_Color284: 283,
  eDrawType_Color285: 284,
  eDrawType_Color286: 285,
  eDrawType_Color287: 286,
  eDrawType_Color288: 287,
  eDrawType_Color289: 288,
  eDrawType_Color290: 289,
  eDrawType_Color291: 290,
  eDrawType_Color292: 291,
  eDrawType_Color293: 292,
  eDrawType_Color294: 293,
  eDrawType_Color295: 294,
  eDrawType_Color296: 295,
  eDrawType_Color297: 296,
  eDrawType_Color298: 297,
  eDrawType_Color299: 298,
  eDrawType_Color300: 299,
  eDrawType_LineErase: 300,
  eDrawType_LineDashed: 301,
  eDrawType_LineDashedRed: 302,
  eDrawType_TextHelvetica: 303,
  eDrawType_TextHelveticaBold: 304,
  eDrawType_TextHelveticaErase: 305,
  eDrawType_TextHelveticaEraseBold: 306,
  eDrawType_Color__: 307,
  eDrawType_CustomColor1: 310,
  eDrawType_CustomColor2: 314,
  eDrawType_CustomColor3: 318,
  eDrawType_CustomColor4: 322,
  eDrawType_CustomColor5: 326,
  eDrawType_CustomColor6: 330,
  eDrawType_CustomColor7: 334,
  eDrawType_CustomColor8: 338,
  eDrawType_CustomColor9: 342,
  eDrawType_CustomColor10: 346,
  eDrawType_CustomColor11: 350,
  eDrawType_CustomColor12: 354,
  eDrawType_CustomColor13: 358,
  eDrawType_CustomColor14: 362,
  eDrawType_CustomColor15: 366,
  eDrawType_CustomColor16: 370,
  eDrawType_CustomColor17: 374,
  eDrawType_CustomColor18: 378,
  eDrawType_CustomColor19: 382,
  eDrawType_CustomColor20: 386,
  eDrawType_CustomColor21: 390,
  eDrawType_CustomColor22: 394,
  eDrawType_CustomColor23: 398,
  eDrawType_CustomColor24: 402,
  eDrawType_CustomColor25: 406,
  eDrawType_CustomColor26: 410,
  eDrawType_CustomColor27: 414,
  eDrawType_CustomColor28: 418,
  eDrawType_CustomColor29: 422,
  eDrawType_CustomColor30: 426,
  eDrawType_CustomColor31: 430,
  eDrawType_CustomColor32: 434,
  eDrawType_CustomColor33: 438,
  eDrawType_CustomColor34: 442,
  eDrawType_CustomColor35: 446,
  eDrawType_CustomColor36: 450,
  eDrawType_CustomColor37: 454,
  eDrawType_CustomColor38: 458,
  eDrawType_CustomColor39: 462,
  eDrawType_CustomColor40: 466,
  eDrawType_CustomColor41: 470,
  eDrawType_CustomColor42: 474,
  eDrawType_CustomColor43: 478,
  eDrawType_CustomColor44: 482,
  eDrawType_CustomColor45: 486,
  eDrawType_CustomColor46: 490,
  eDrawType_CustomColor47: 494,
  eDrawType_CustomColor48: 498,
  eDrawType_CustomColor49: 502,
  eDrawType_CustomColor50: 506,
  eDrawType_CustomColor51: 510,
  eDrawType_CustomColor52: 514,
  eDrawType_CustomColor53: 518,
  eDrawType_CustomColor54: 522,
  eDrawType_CustomColor55: 524,
  eDrawType_CustomColor56: 530,
  eDrawType_CustomColor57: 534,
  eDrawType_CustomColor58: 538,
  eDrawType_CustomColor59: 542,
  eDrawType_CustomColor60: 546,
  eDrawType_CustomColor61: 550,
  eDrawType_CustomColor62: 554,
  eDrawType_CustomColor63: 558,
  eDrawType_CustomColor64: 562,
  eDrawType_CustomColor65: 566,
  eDrawType_CustomColor66: 570,
  eDrawType_CustomColor67: 574,
  eDrawType_CustomColor68: 578,
  eDrawType_CustomColor69: 582,
  eDrawType_CustomColor70: 586,
  eDrawType_CustomColor71: 590,
  eDrawType_CustomColor72: 594,
  eDrawType_CustomColor73: 598,
  eDrawType_CustomColor74: 602,
  eDrawType_CustomColor75: 606,
  eDrawType_CustomColor76: 610,
  eDrawType_CustomColor77: 614,
  eDrawType_CustomColor78: 618,
  eDrawType_CustomColor79: 622,
  eDrawType_CustomColor80: 626,
  eDrawType_CustomColor81: 630,
  eDrawType_CustomColor82: 634,
  eDrawType_CustomColor83: 638,
  eDrawType_CustomColor84: 642,
  eDrawType_CustomColor85: 646,
  eDrawType_CustomColor86: 650,
  eDrawType_CustomColor87: 654,
  eDrawType_CustomColor88: 658,
  eDrawType_CustomColor89: 662,
  eDrawType_CustomColor90: 666,
  eDrawType_CustomColor__: 670,
  eDrawType__: 670,
  eDrawType_Inherit: 9999,
  eDrawType_No: 10000,

  eDrawType_FillHighlight: 173,
  eDrawType_LineHighlight: 175,
  eDrawType_ColorYellow: 114,
  eDrawType_ColorRed: 176,
  eDrawType_DarkGray: 29,
  eDrawType_MediumGray: 24,
  eDrawType_LightGray: 20,

  eDrawTone_No: 0,
  eDrawTone_Gray: 1,
  eDrawTone_YellowGreen: 2,
  eDrawTone_Yellow: 3,
  eDrawTone_Orange: 4,
  eDrawTone_Red: 5,
  eDrawTone_Magenta: 6,
  eDrawTone_Blue: 7,
  eDrawTone_Seablue: 8,
  eDrawTone_Green: 9,
  eDrawTone_DarkGray: 10,
  eDrawTone_DarkYellowGreen: 11,
  eDrawTone_DarkYellow: 12,
  eDrawTone_DarkOrange: 13,
  eDrawTone_DarkRed: 14,
  eDrawTone_DarkMagenta: 15,
  eDrawTone_DarkBlue: 16,
  eDrawTone_DarkSeablue: 17,
  eDrawTone_DarkGreen: 18,
  eDrawTone_LightGray: 19,
  eDrawTone_LightYellowGreen: 20,
  eDrawTone_LightYellow: 21,
  eDrawTone_LightOrange: 22,
  eDrawTone_LightRed: 23,
  eDrawTone_LightMagenta: 24,
  eDrawTone_LightBlue: 25,
  eDrawTone_LightSeablue: 26,
  eDrawTone_LightGreen: 27,
  eDrawTone_GrayHighSaturation: 28,
  eDrawTone_YellowGreenHighSaturation: 29,
  eDrawTone_YellowHighSaturation: 30,
  eDrawTone_OrangeHighSaturation: 31,
  eDrawTone_RedHighSaturation: 32,
  eDrawTone_MagentaHighSaturation: 33,
  eDrawTone_BlueHighSaturation: 34,
  eDrawTone_SeablueHighSaturation: 35,
  eDrawTone_GreenHighSaturation: 36,
  eDrawTone_DarkGrayHighSaturation: 37,
  eDrawTone_DarkYellowGreenHighSaturation: 38,
  eDrawTone_DarkYellowHighSaturation: 39,
  eDrawTone_DarkOrangeHighSaturation: 40,
  eDrawTone_DarkRedHighSaturation: 41,
  eDrawTone_DarkMagentaHighSaturation: 42,
  eDrawTone_DarkBlueHighSaturation: 43,
  eDrawTone_DarkSeablueHighSaturation: 44,
  eDrawTone_DarkGreenHighSaturation: 45,
  eDrawTone_LightGrayHighSaturation: 46,
  eDrawTone_LightYellowGreenHighSaturation: 47,
  eDrawTone_LightYellowHighSaturation: 48,
  eDrawTone_LightOrangeHighSaturation: 49,
  eDrawTone_LightRedHighSaturation: 50,
  eDrawTone_LightMagentaHighSaturation: 51,
  eDrawTone_LightBlueHighSaturation: 52,
  eDrawTone_LightSeablueHighSaturation: 53,
  eDrawTone_LightGreenHighSaturation: 54,
  eDrawTone_GrayLowSaturation: 55,
  eDrawTone_YellowGreenLowSaturation: 56,
  eDrawTone_YellowLowSaturation: 57,
  eDrawTone_OrangeLowSaturation: 58,
  eDrawTone_RedLowSaturation: 59,
  eDrawTone_MagentaLowSaturation: 60,
  eDrawTone_BlueLowSaturation: 61,
  eDrawTone_SeablueLowSaturation: 62,
  eDrawTone_GreenLowSaturation: 63,
  eDrawTone_DarkGrayLowSaturation: 64,
  eDrawTone_DarkYellowGreenLowSaturation: 65,
  eDrawTone_DarkYellowLowSaturation: 66,
  eDrawTone_DarkOrangeLowSaturation: 67,
  eDrawTone_DarkRedLowSaturation: 68,
  eDrawTone_DarkMagentaLowSaturation: 69,
  eDrawTone_DarkBlueLowSaturation: 70,
  eDrawTone_DarkSeablueLowSaturation: 71,
  eDrawTone_DarkGreenLowSaturation: 72,
  eDrawTone_LightGrayLowSaturation: 73,
  eDrawTone_LightYellowGreenLowSaturation: 74,
  eDrawTone_LightYellowLowSaturation: 75,
  eDrawTone_LightOrangeLowSaturation: 76,
  eDrawTone_LightRedLowSaturation: 77,
  eDrawTone_LightMagentaLowSaturation: 78,
  eDrawTone_LightBlueLowSaturation: 79,
  eDrawTone_LightSeablueLowSaturation: 80,
  eDrawTone_LightGreenLowSaturation: 81,
  eDrawTone__: 82,

  eScaleType_LowerLeft: 0,
  eScaleType_LowerRight: 1,
  eScaleType_UpperRight: 2,
  eScaleType_UpperLeft: 3,
  eScaleType_Left: 4,
  eScaleType_Up: 5,
  eScaleType_Right: 6,
  eScaleType_Down: 7,
  eScaleType_Center: 8,
  eScaleType_FixPoint: 9,
  eScaleType_No: 10,

  eRotationPoint_LowerLeft: 0,
  eRotationPoint_LowerRight: 1,
  eRotationPoint_UpperRight: 2,
  eRotationPoint_UpperLeft: 3,
  eRotationPoint_Center: 4,
  eRotationPoint_FixPoint: 5,
  eRotationPoint_Zero: 6,

  eTraceType_Boolean: 0,
  eTraceType_Int32: 1,
  eTraceType_Float32: 2,
  eTraceType_User: 3,

  eMenuItem_Button: 0,
  eMenuItem_ButtonDisabled: 1,
  eMenuItem_PulldownMenu: 2,

  eUserdataCbType_Node: 0,
  eUserdataCbType_Ctx: 1,
  eUserdataCbType_NodeClass: 2,

  eSaveMode_Edit: 0,
  eSaveMode_Trace: 1,
  eSaveMode_SubGraph: 2,
  eSaveMode_ReadConfigOnly: 3,

  eSave_Comment: 0,
  eSave_Ctx: 1,
  eSave_Array: 2,
  eSave_NodeClass: 3,
  eSave_ConClass: 4,
  eSave_Rect: 5,
  eSave_Line: 6,
  eSave_Point: 7,
  eSave_Arc: 8,
  eSave_Text: 9,
  eSave_Node: 10,
  eSave_Con: 11,
  eSave_ConPoint: 12,
  eSave_Annot: 13,
  eSave_Arrow: 14,
  eSave_Pixmap: 15,
  eSave_AnnotPixmap: 16,
  eSave_Radiobutton: 17,
  eSave_Frame: 18,
  eSave_GrowRect: 19,
  eSave_GrowLine: 20,
  eSave_GrowConPoint: 21,
  eSave_GrowCtx: 22,
  eSave_GrowSubAnnot: 23,
  eSave_GrowArc: 24,
  eSave_PolyLine: 25,
  eSave_GrowPolyLine: 26,
  eSave_GrowNode: 27,
  eSave_Transform: 28,
  eSave_GrowAnnot: 29,
  eSave_GrowText: 30,
  eSave_GrowBar: 31,
  eSave_GrowTrend: 32,
  eSave_GrowSlider: 33,
  eSave_GrowImage: 34,
  eSave_GrowGroup: 35,
  eSave_NodeGroup: 36,
  eSave_GrowAxis: 37,
  eSave_GrowRectRounded: 38,
  eSave_GrowConGlue: 39,
  eSave_GrowWindow: 40,
  eSave_GrowFolder: 41,
  eSave_GrowTable: 42,
  eSave_GrowXYCurve: 43,
  eSave_GrowAxisArc: 44,
  eSave_GrowPie: 45,
  eSave_GrowBarChart: 46,
  eSave_GrowToolbar: 47,
  eSave_CustomColors: 48,
  eSave_GrowBarArc: 49,
  eSave_End: 99,
  eSave_Ctx_zoom_factor_x: 100,
  eSave_Ctx_base_zoom_factor: 101,
  eSave_Ctx_offset_x: 102,
  eSave_Ctx_offset_y: 103,
  eSave_Ctx_nav_zoom_factor_x: 104,
  eSave_Ctx_print_zoom_factor: 105,
  eSave_Ctx_nav_offset_x: 106,
  eSave_Ctx_nav_offset_y: 107,
  eSave_Ctx_x_right: 108,
  eSave_Ctx_x_left: 109,
  eSave_Ctx_y_high: 110,
  eSave_Ctx_y_low: 111,
  eSave_Ctx_window_width: 112,
  eSave_Ctx_window_height: 113,
  eSave_Ctx_nav_window_width: 114,
  eSave_Ctx_nav_window_height: 115,
  eSave_Ctx_nav_rect_ll_x: 116,
  eSave_Ctx_nav_rect_ll_y: 117,
  eSave_Ctx_nav_rect_ur_x: 118,
  eSave_Ctx_nav_rect_ur_y: 119,
  eSave_Ctx_nav_rect_hot: 120,
  eSave_Ctx_name: 121,
  eSave_Ctx_user_highlight: 122,
  eSave_Ctx_a_nc: 123,
  eSave_Ctx_a_cc: 124,
  eSave_Ctx_a: 125,
  eSave_Ctx_grid_size_x: 126,
  eSave_Ctx_grid_size_y: 127,
  eSave_Ctx_grid_on: 128,
  eSave_Ctx_draw_delta: 129,
  eSave_Ctx_refcon_width: 130,
  eSave_Ctx_refcon_height: 131,
  eSave_Ctx_refcon_textsize: 132,
  eSave_Ctx_refcon_linewidth: 133,
  eSave_Ctx_grow: 134,
  eSave_Ctx_zoom_factor_y: 135,
  eSave_Ctx_nav_zoom_factor_y: 136,
  eSave_Ctx_version: 137,
  eSave_Ctx_hot_indication: 138,
  eSave_Ctx_tiptext_size: 139,
  eSave_Ctx_app_motion: 140,
  eSave_Ctx_color_theme: 141,
  eSave_Ctx_comment: 199,
  eSave_Array_a: 200,
  eSave_NodeClass_nc_name: 300,
  eSave_NodeClass_a: 301,
  eSave_NodeClass_group: 302,
  eSave_NodeClass_dynamic: 303,
  eSave_NodeClass_dynamicsize: 304,
  eSave_NodeClass_arg_cnt: 305,
  eSave_NodeClass_argname: 306,
  eSave_NodeClass_argtype: 307,
  eSave_NodeClass_dyn_type1: 308,
  eSave_NodeClass_dyn_color1: 309,
  eSave_NodeClass_no_con_obstacle: 310,
  eSave_NodeClass_slider: 311,
  eSave_NodeClass_java_name: 312,
  eSave_NodeClass_dyn_color2: 313,
  eSave_NodeClass_next_nodeclass: 314,
  eSave_NodeClass_animation_count: 315,
  eSave_NodeClass_cycle: 316,
  eSave_NodeClass_y0: 317,
  eSave_NodeClass_y1: 318,
  eSave_NodeClass_x0: 319,
  eSave_NodeClass_x1: 320,
  eSave_NodeClass_dyn_action_type1: 321,
  eSave_NodeClass_dyn_color3: 322,
  eSave_NodeClass_dyn_color4: 323,
  eSave_NodeClass_dyn_attr1: 324,
  eSave_NodeClass_dyn_attr2: 325,
  eSave_NodeClass_dyn_attr3: 326,
  eSave_NodeClass_dyn_attr4: 327,
  eSave_NodeClass_input_focus_mark: 328,
  eSave_NodeClass_userdata_cb: 329,
  eSave_NodeClass_dyn_type2: 330,
  eSave_NodeClass_dyn_action_type2: 331,
  eSave_NodeClass_recursive_trace: 332,
  eSave_ConClass_cc_name: 400,
  eSave_ConClass_con_type: 401,
  eSave_ConClass_corner: 402,
  eSave_ConClass_draw_type: 403,
  eSave_ConClass_line_width: 404,
  eSave_ConClass_arrow_width: 405,
  eSave_ConClass_arrow_length: 406,
  eSave_ConClass_round_corner_amount: 407,
  eSave_ConClass_group: 408,
  eSave_Rect_draw_type: 500,
  eSave_Rect_line_width: 501,
  eSave_Rect_ll: 502,
  eSave_Rect_ur: 503,
  eSave_Rect_display_level: 504,
  eSave_Rect_fill: 505,
  eSave_Line_draw_type: 600,
  eSave_Line_line_width: 601,
  eSave_Line_p1: 602,
  eSave_Line_p2: 603,
  eSave_Point_x: 700,
  eSave_Point_y: 701,
  eSave_Arc_angle1: 800,
  eSave_Arc_angle2: 801,
  eSave_Arc_draw_type: 802,
  eSave_Arc_line_width: 803,
  eSave_Arc_ll: 804,
  eSave_Arc_ur: 805,
  eSave_Arc_fill: 806,
  eSave_Text_text_size: 900,
  eSave_Text_draw_type: 901,
  eSave_Text_text: 902,
  eSave_Text_p: 903,
  eSave_Text_color_drawtype: 904,
  eSave_Node_nc: 1000,
  eSave_Node_pos: 1001,
  eSave_Node_n_name: 1002,
  eSave_Node_annotsize: 1003,
  eSave_Node_annotv: 1004,
  eSave_Node_refcon_cnt: 1005,
  eSave_Node_x_right: 1006,
  eSave_Node_x_left: 1007,
  eSave_Node_y_high: 1008,
  eSave_Node_y_low: 1009,
  eSave_Node_trace_data1: 1010,
  eSave_Node_trace_data2: 1011,
  eSave_Node_trace_attr_type: 1012,
  eSave_Node_obst_x_right: 1013,
  eSave_Node_obst_x_left: 1014,
  eSave_Node_obst_y_high: 1015,
  eSave_Node_obst_y_low: 1016,
  eSave_Node_trace_color: 1017,
  eSave_Node_trace_data3: 1018,
  eSave_Node_trace_data4: 1019,
  eSave_Node_trace_data5: 1020,
  eSave_Node_trace_data6: 1021,
  eSave_Node_trace_data7: 1022,
  eSave_Node_trace_data8: 1023,
  eSave_Node_trace_data9: 1024,
  eSave_Node_trace_data10: 1025,
  eSave_Node_access: 1026,
  eSave_Node_trace_color2: 1027,
  eSave_Node_cycle: 1028,
  eSave_Node_ref_object: 1029,
  eSave_Con_x_right: 1100,
  eSave_Con_x_left: 1101,
  eSave_Con_y_high: 1102,
  eSave_Con_y_low: 1103,
  eSave_Con_cc: 1104,
  eSave_Con_dest_node: 1105,
  eSave_Con_source_node: 1106,
  eSave_Con_dest_conpoint: 1107,
  eSave_Con_source_conpoint: 1108,
  eSave_Con_dest_direction: 1109,
  eSave_Con_source_direction: 1110,
  eSave_Con_line_a: 1111,
  eSave_Con_arc_a: 1112,
  eSave_Con_arrow_a: 1113,
  eSave_Con_ref_a: 1114,
  eSave_Con_p_num: 1115,
  eSave_Con_l_num: 1116,
  eSave_Con_a_num: 1117,
  eSave_Con_arrow_num: 1118,
  eSave_Con_ref_num: 1119,
  eSave_Con_point_x: 1120,
  eSave_Con_point_y: 1121,
  eSave_Con_source_ref_cnt: 1122,
  eSave_Con_dest_ref_cnt: 1123,
  eSave_Con_c_name: 1124,
  eSave_Con_trace_object: 1125,
  eSave_Con_trace_attribute: 1126,
  eSave_Con_trace_attr_type: 1127,
  eSave_Con_temporary_ref: 1128,
  eSave_Con_border: 1129,
  eSave_Con_shadow: 1130,
  eSave_ConPoint_number: 1200,
  eSave_ConPoint_direction: 1201,
  eSave_ConPoint_p: 1202,
  eSave_ConPoint_trace_attribute: 1203,
  eSave_ConPoint_trace_attr_type: 1204,
  eSave_ConPoint_trf: 1205,
  eSave_Annot_number: 1300,
  eSave_Annot_draw_type: 1301,
  eSave_Annot_text_size: 1302,
  eSave_Annot_p: 1303,
  eSave_Annot_annot_type: 1304,
  eSave_Annot_display_level: 1305,
  eSave_Annot_color_drawtype: 1306,
  eSave_Annot_font: 1307,
  eSave_Annot_protect: 1308,
  eSave_Arrow_arrow_width: 1400,
  eSave_Arrow_arrow_length: 1401,
  eSave_Arrow_draw_type: 1402,
  eSave_Arrow_line_width: 1403,
  eSave_Arrow_p_dest: 1404,
  eSave_Arrow_p1: 1405,
  eSave_Arrow_p2: 1406,
  eSave_GrowRect_x_right: 1900,
  eSave_GrowRect_x_left: 1901,
  eSave_GrowRect_y_high: 1902,
  eSave_GrowRect_y_low: 1903,
  eSave_GrowRect_n_name: 1904,
  eSave_GrowRect_rect_part: 1905,
  eSave_GrowRect_dynamic: 1906,
  eSave_GrowRect_dynamicsize: 1907,
  eSave_GrowRect_original_border_drawtype: 1908,
  eSave_GrowRect_original_fill_drawtype: 1909,
  eSave_GrowRect_fill_drawtype: 1910,
  eSave_GrowRect_border: 1911,
  eSave_GrowRect_trf: 1912,
  eSave_GrowRect_shadow_width: 1913,
  eSave_GrowRect_relief: 1914,
  eSave_GrowRect_shadow: 1915,
  eSave_GrowRect_shadow_contrast: 1916,
  eSave_GrowRect_disable_shadow: 1917,
  eSave_GrowRect_invisible: 1918,
  eSave_GrowRect_fixcolor: 1919,
  eSave_GrowRect_fixposition: 1920,
  eSave_GrowRect_gradient: 1921,
  eSave_GrowRect_gradient_contrast: 1922,
  eSave_GrowRect_disable_gradient: 1923,
  eSave_GrowRect_bgcolor_gradient: 1924,
  eSave_GrowRect_background_drawtype: 1925,
  eSave_GrowRect_fill_eq_background: 1926,
  eSave_GrowLine_x_right: 2000,
  eSave_GrowLine_x_left: 2001,
  eSave_GrowLine_y_high: 2002,
  eSave_GrowLine_y_low: 2003,
  eSave_GrowLine_n_name: 2004,
  eSave_GrowLine_line_part: 2005,
  eSave_GrowLine_dynamic: 2006,
  eSave_GrowLine_dynamicsize: 2007,
  eSave_GrowLine_trf: 2008,
  eSave_GrowLine_original_border_drawtype: 2009,
  eSave_GrowLine_line_type: 2010,
  eSave_GrowConPoint_x_right: 2100,
  eSave_GrowConPoint_x_left: 2101,
  eSave_GrowConPoint_y_high: 2102,
  eSave_GrowConPoint_y_low: 2103,
  eSave_GrowConPoint_n_name: 2104,
  eSave_GrowConPoint_arc: 2105,
  eSave_GrowConPoint_conpoint_part: 2106,
  eSave_GrowCtx_conpoint_num_cnt: 2200,
  eSave_GrowCtx_objectname_cnt: 2201,
  eSave_GrowCtx_name: 2202,
  eSave_GrowCtx_background_color: 2203,
  eSave_GrowCtx_dynamic: 2204,
  eSave_GrowCtx_dynamicsize: 2205,
  eSave_GrowCtx_arg_cnt: 2206,
  eSave_GrowCtx_argname: 2207,
  eSave_GrowCtx_argtype: 2208,
  eSave_GrowCtx_x0: 2209,
  eSave_GrowCtx_y0: 2210,
  eSave_GrowCtx_x1: 2211,
  eSave_GrowCtx_y1: 2212,
  eSave_GrowCtx_path_cnt: 2213,
  eSave_GrowCtx_path: 2214,
  eSave_GrowCtx_dyn_type1: 2215,
  eSave_GrowCtx_dyn_color1: 2216,
  eSave_GrowCtx_no_con_obstacle: 2217,
  eSave_GrowCtx_slider: 2218,
  eSave_GrowCtx_subgraph: 2219,
  eSave_GrowCtx_java_name: 2220,
  eSave_GrowCtx_dyn_color2: 2221,
  eSave_GrowCtx_next_subgraph: 2222,
  eSave_GrowCtx_animation_count: 2223,
  eSave_GrowCtx_scantime: 2224,
  eSave_GrowCtx_animation_scantime: 2225,
  eSave_GrowCtx_java_width: 2226,
  eSave_GrowCtx_background_image: 2227,
  eSave_GrowCtx_background_tiled: 2228,
  eSave_GrowCtx_double_buffered: 2229,
  eSave_GrowCtx_is_javaapplet: 2230,
  eSave_GrowCtx_is_javaapplication: 2231,
  eSave_GrowCtx_fast_scantime: 2232,
  eSave_GrowCtx_cycle: 2233,
  eSave_GrowCtx_mb3_action: 2234,
  eSave_GrowCtx_translate_on: 2235,
  eSave_GrowCtx_dyn_action_type1: 2236,
  eSave_GrowCtx_dyn_color3: 2237,
  eSave_GrowCtx_dyn_color4: 2238,
  eSave_GrowCtx_dyn_attr1: 2239,
  eSave_GrowCtx_dyn_attr2: 2240,
  eSave_GrowCtx_dyn_attr3: 2241,
  eSave_GrowCtx_dyn_attr4: 2242,
  eSave_GrowCtx_input_focus_mark: 2243,
  eSave_GrowCtx_userdata_cb: 2244,
  eSave_GrowCtx_bitmap_fonts: 2245,
  eSave_GrowCtx_dyn_type2: 2246,
  eSave_GrowCtx_dyn_action_type2: 2247,
  eSave_GrowCtx_recursive_trace: 2248,
  eSave_GrowCtx_customcolors: 2249,
  eSave_GrowSubAnnot_x_right: 2300,
  eSave_GrowSubAnnot_x_left: 2301,
  eSave_GrowSubAnnot_y_high: 2302,
  eSave_GrowSubAnnot_y_low: 2303,
  eSave_GrowSubAnnot_n_name: 2304,
  eSave_GrowSubAnnot_text: 2305,
  eSave_GrowSubAnnot_rect: 2306,
  eSave_GrowSubAnnot_annot_part: 2307,
  eSave_GrowSubAnnot_trf: 2308,
  eSave_GrowSubAnnot_adjustment: 2309,
  eSave_GrowArc_x_right: 2400,
  eSave_GrowArc_x_left: 2401,
  eSave_GrowArc_y_high: 2402,
  eSave_GrowArc_y_low: 2403,
  eSave_GrowArc_n_name: 2404,
  eSave_GrowArc_arc_part: 2405,
  eSave_GrowArc_dynamic: 2406,
  eSave_GrowArc_dynamicsize: 2407,
  eSave_GrowArc_original_border_drawtype: 2408,
  eSave_GrowArc_original_fill_drawtype: 2409,
  eSave_GrowArc_fill_drawtype: 2410,
  eSave_GrowArc_border: 2411,
  eSave_GrowArc_trf: 2412,
  eSave_GrowArc_shadow_width: 2413,
  eSave_GrowArc_relief: 2414,
  eSave_GrowArc_shadow: 2415,
  eSave_GrowArc_shadow_contrast: 2416,
  eSave_GrowArc_disable_shadow: 2417,
  eSave_GrowArc_gradient: 2418,
  eSave_GrowArc_gradient_contrast: 2419,
  eSave_GrowArc_disable_gradient: 2420,
  eSave_GrowArc_fixcolor: 2421,
  eSave_GrowArc_fixposition: 2422,
  eSave_GrowArc_background_drawtype: 2423,
  eSave_GrowArc_fill_eq_background: 2424,
  eSave_PolyLine_draw_type: 2500,
  eSave_PolyLine_line_width: 2501,
  eSave_PolyLine_a_points: 2502,
  eSave_PolyLine_fill: 2503,
  eSave_PolyLine_closed_line: 2504,
  eSave_GrowPolyLine_x_right: 2600,
  eSave_GrowPolyLine_x_left: 2601,
  eSave_GrowPolyLine_y_high: 2602,
  eSave_GrowPolyLine_y_low: 2603,
  eSave_GrowPolyLine_n_name: 2604,
  eSave_GrowPolyLine_polyline_part: 2605,
  eSave_GrowPolyLine_dynamic: 2606,
  eSave_GrowPolyLine_dynamicsize: 2607,
  eSave_GrowPolyLine_original_border_drawtype: 2608,
  eSave_GrowPolyLine_original_fill_drawtype: 2609,
  eSave_GrowPolyLine_fill_drawtype: 2610,
  eSave_GrowPolyLine_border: 2611,
  eSave_GrowPolyLine_trf: 2612,
  eSave_GrowPolyLine_fill_eq_border: 2613,
  eSave_GrowPolyLine_shadow_width: 2614,
  eSave_GrowPolyLine_relief: 2615,
  eSave_GrowPolyLine_shadow: 2616,
  eSave_GrowPolyLine_shadow_contrast: 2617,
  eSave_GrowPolyLine_disable_shadow: 2618,
  eSave_GrowPolyLine_fill_eq_light: 2619,
  eSave_GrowPolyLine_fill_eq_shadow: 2620,
  eSave_GrowPolyLine_fixcolor: 2621,
  eSave_GrowPolyLine_fixposition: 2622,
  eSave_GrowPolyLine_gradient: 2623,
  eSave_GrowPolyLine_gradient_contrast: 2624,
  eSave_GrowPolyLine_disable_gradient: 2625,
  eSave_GrowPolyLine_fill_eq_bglight: 2626,
  eSave_GrowPolyLine_fill_eq_bgshadow: 2627,
  eSave_GrowPolyLine_fill_eq_background: 2628,
  eSave_GrowPolyLine_background_drawtype: 2629,
  eSave_GrowNode_node_part: 2700,
  eSave_GrowNode_dynamic: 2701,
  eSave_GrowNode_dynamicsize: 2702,
  eSave_GrowNode_original_border_drawtype: 2703,
  eSave_GrowNode_original_fill_drawtype: 2704,
  eSave_GrowNode_fill_drawtype: 2705,
  eSave_GrowNode_draw_type: 2706,
  eSave_GrowNode_trf: 2707,
  eSave_GrowNode_original_color_tone: 2708,
  eSave_GrowNode_color_tone: 2709,
  eSave_GrowNode_original_color_lightness: 2710,
  eSave_GrowNode_color_lightness: 2711,
  eSave_GrowNode_original_color_intensity: 2712,
  eSave_GrowNode_color_intensity: 2713,
  eSave_GrowNode_original_color_shift: 2714,
  eSave_GrowNode_color_shift: 2715,
  eSave_GrowNode_arg_cnt: 2716,
  eSave_GrowNode_argv: 2717,
  eSave_GrowNode_argsize: 2718,
  eSave_GrowNode_line_width: 2719,
  eSave_GrowNode_invisible: 2720,
  eSave_GrowNode_userdata_cb: 2721,
  eSave_GrowNode_original_text_drawtype: 2722,
  eSave_GrowNode_text_drawtype: 2723,
  eSave_GrowNode_shadow: 2724,
  eSave_GrowNode_flip_horizontal: 2725,
  eSave_GrowNode_flip_vertical: 2726,
  eSave_GrowNode_gradient: 2727,
  eSave_GrowNode_text_type: 2728,
  eSave_GrowNode_text_font: 2729,
  eSave_GrowNode_disable_cb: 2730,
  eSave_GrowNode_original_background_drawtype: 2731,
  eSave_GrowNode_background_drawtype: 2732,
  eSave_Transform_a11: 2800,
  eSave_Transform_a12: 2801,
  eSave_Transform_a13: 2802,
  eSave_Transform_a21: 2803,
  eSave_Transform_a22: 2804,
  eSave_Transform_a23: 2805,
  eSave_Transform_rotation: 2806,
  eSave_GrowAnnot_adjustment: 2901,
  eSave_GrowAnnot_annot_part: 2907,
  eSave_GrowAnnot_trf: 2908,
  eSave_GrowText_x_right: 3000,
  eSave_GrowText_x_left: 3001,
  eSave_GrowText_y_high: 3002,
  eSave_GrowText_y_low: 3003,
  eSave_GrowText_n_name: 3004,
  eSave_GrowText_text_part: 3005,
  eSave_GrowText_dynamic: 3006,
  eSave_GrowText_dynamicsize: 3007,
  eSave_GrowText_original_color_drawtype: 3008,
  eSave_GrowText_trf: 3009,
  eSave_GrowText_font: 3010,
  eSave_GrowText_adjustment: 3011,
  eSave_GrowBar_max_value: 3100,
  eSave_GrowBar_min_value: 3101,
  eSave_GrowBar_bar_value: 3102,
  eSave_GrowBar_bar_drawtype: 3103,
  eSave_GrowBar_direction: 3104,
  eSave_GrowBar_rect_part: 3105,
  eSave_GrowBar_trace_data1: 3106,
  eSave_GrowBar_trace_data2: 3107,
  eSave_GrowBar_trace_attr_type: 3108,
  eSave_GrowBar_trace_color: 3109,
  eSave_GrowBar_bar_bordercolor: 3110,
  eSave_GrowBar_bar_borderwidth: 3111,
  eSave_GrowBar_trace_data3: 3112,
  eSave_GrowBar_trace_data4: 3113,
  eSave_GrowBar_trace_data5: 3114,
  eSave_GrowBar_trace_data6: 3115,
  eSave_GrowBar_trace_data7: 3116,
  eSave_GrowBar_trace_data8: 3117,
  eSave_GrowBar_trace_data9: 3118,
  eSave_GrowBar_trace_data10: 3119,
  eSave_GrowBar_access: 3120,
  eSave_GrowBar_cycle: 3121,
  eSave_GrowBar_ref_object: 3122,
  eSave_GrowBar_userdata_cb: 3123,
  eSave_GrowTrend_y_max_value_0: 3200,
  eSave_GrowTrend_y_min_value_0: 3201,
  eSave_GrowTrend_curve_drawtype_0: 3202,
  eSave_GrowTrend_rect_part: 3203,
  eSave_GrowTrend_trace_data1: 3204,
  eSave_GrowTrend_trace_data2: 3205,
  eSave_GrowTrend_trace_attr_type: 3206,
  eSave_GrowTrend_trace_color: 3207,
  eSave_GrowTrend_horizontal_lines: 3208,
  eSave_GrowTrend_vertical_lines: 3209,
  eSave_GrowTrend_fill_curve: 3210,
  eSave_GrowTrend_curve_fill_drawtype_0: 3211,
  eSave_GrowTrend_no_of_points: 3212,
  eSave_GrowTrend_curve_width: 3213,
  eSave_GrowTrend_scan_time: 3214,
  eSave_GrowTrend_y_max_value_1: 3215,
  eSave_GrowTrend_y_min_value_1: 3216,
  eSave_GrowTrend_curve_drawtype_1: 3217,
  eSave_GrowTrend_curve_fill_drawtype_1: 3218,
  eSave_GrowTrend_trace_data3: 3219,
  eSave_GrowTrend_trace_data4: 3220,
  eSave_GrowTrend_trace_data5: 3221,
  eSave_GrowTrend_trace_data6: 3222,
  eSave_GrowTrend_trace_data7: 3223,
  eSave_GrowTrend_trace_data8: 3224,
  eSave_GrowTrend_trace_data9: 3225,
  eSave_GrowTrend_trace_data10: 3226,
  eSave_GrowTrend_access: 3227,
  eSave_GrowTrend_cycle: 3228,
  eSave_GrowTrend_ref_object: 3229,
  eSave_GrowTrend_userdata_cb: 3230,
  eSave_GrowTrend_x_max_value_0: 3231,
  eSave_GrowTrend_x_min_value_0: 3232,
  eSave_GrowTrend_x_max_value_1: 3233,
  eSave_GrowTrend_x_min_value_1: 3234,
  eSave_GrowTrend_mode: 3235,
  eSave_GrowSlider_grownode_part: 3300,
  eSave_GrowSlider_direction: 3301,
  eSave_GrowSlider_max_value: 3302,
  eSave_GrowSlider_min_value: 3303,
  eSave_GrowSlider_max_pos: 3304,
  eSave_GrowSlider_min_pos: 3305,
  eSave_GrowImage_x_right: 3400,
  eSave_GrowImage_x_left: 3401,
  eSave_GrowImage_y_high: 3402,
  eSave_GrowImage_y_low: 3403,
  eSave_GrowImage_n_name: 3404,
  eSave_GrowImage_image_filename: 3405,
  eSave_GrowImage_dynamic: 3406,
  eSave_GrowImage_dynamicsize: 3407,
  eSave_GrowImage_trf: 3408,
  eSave_GrowImage_display_level: 3409,
  eSave_GrowImage_ll: 3410,
  eSave_GrowImage_ur: 3411,
  eSave_GrowImage_color_tone: 3412,
  eSave_GrowImage_color_lightness: 3413,
  eSave_GrowImage_color_intensity: 3414,
  eSave_GrowImage_color_shift: 3415,
  eSave_GrowImage_fixposition: 3416,
  eSave_GrowGroup_grownode_part: 3500,
  eSave_GrowGroup_nc: 3501,
  eSave_NodeGroup_nodeclass_part: 3600,
  eSave_GrowAxis_max_value: 3700,
  eSave_GrowAxis_min_value: 3701,
  eSave_GrowAxis_rect_part: 3702,
  eSave_GrowAxis_lines: 3703,
  eSave_GrowAxis_longquotient: 3704,
  eSave_GrowAxis_valuequotient: 3705,
  eSave_GrowAxis_format: 3706,
  eSave_GrowAxis_text_size: 3707,
  eSave_GrowAxis_text_drawtype: 3708,
  eSave_GrowAxis_text_color_drawtype: 3709,
  eSave_GrowAxis_userdata_cb: 3710,
  eSave_GrowRectRounded_x_right: 3800,
  eSave_GrowRectRounded_x_left: 3801,
  eSave_GrowRectRounded_y_high: 3802,
  eSave_GrowRectRounded_y_low: 3803,
  eSave_GrowRectRounded_n_name: 3804,
  eSave_GrowRectRounded_rect_part: 3805,
  eSave_GrowRectRounded_dynamic: 3806,
  eSave_GrowRectRounded_dynamicsize: 3807,
  eSave_GrowRectRounded_original_border_drawtype: 3808,
  eSave_GrowRectRounded_original_fill_drawtype: 3809,
  eSave_GrowRectRounded_fill_drawtype: 3810,
  eSave_GrowRectRounded_border: 3811,
  eSave_GrowRectRounded_trf: 3812,
  eSave_GrowRectRounded_round_amount: 3813,
  eSave_GrowRectRounded_shadow_width: 3814,
  eSave_GrowRectRounded_relief: 3815,
  eSave_GrowRectRounded_shadow: 3816,
  eSave_GrowRectRounded_shadow_contrast: 3817,
  eSave_GrowRectRounded_disable_shadow: 3818,
  eSave_GrowRectRounded_gradient: 3819,
  eSave_GrowRectRounded_gradient_contrast: 3820,
  eSave_GrowRectRounded_disable_gradient: 3821,
  eSave_GrowRectRounded_fixposition: 3822,
  eSave_GrowConGlue_line_width_up: 3900,
  eSave_GrowConGlue_line_width_down: 3901,
  eSave_GrowConGlue_line_width_left: 3902,
  eSave_GrowConGlue_line_width_right: 3903,
  eSave_GrowConGlue_node_part: 3904,
  eSave_GrowConGlue_border: 3905,
  eSave_GrowWindow_file_name: 4000,
  eSave_GrowWindow_rect_part: 4001,
  eSave_GrowWindow_userdata_cb: 4002,
  eSave_GrowWindow_scrollbar_width: 4003,
  eSave_GrowWindow_vertical_scrollbar: 4004,
  eSave_GrowWindow_horizontal_scrollbar: 4005,
  eSave_GrowWindow_scrollbar_color: 4006,
  eSave_GrowWindow_scrollbar_bg_color: 4007,
  eSave_GrowWindow_window_scale: 4008,
  eSave_GrowWindow_owner: 4009,
  eSave_GrowFolder_folders: 4100,
  eSave_GrowFolder_text_size: 4101,
  eSave_GrowFolder_text_drawtype: 4102,
  eSave_GrowFolder_text_color_drawtype: 4103,
  eSave_GrowFolder_header_height: 4104,
  eSave_GrowFolder_window_part: 4105,
  eSave_GrowFolder_color_selected: 4106,
  eSave_GrowFolder_color_unselected: 4107,
  eSave_GrowFolder_folder_file_names1: 4120,
  eSave_GrowFolder_folder_text1: 4121,
  eSave_GrowFolder_folder_scale1: 4122,
  eSave_GrowFolder_folder_v_scrollbar1: 4123,
  eSave_GrowFolder_folder_h_scrollbar1: 4124,
  eSave_GrowFolder_folder_file_names2: 4125,
  eSave_GrowFolder_folder_text2: 4126,
  eSave_GrowFolder_folder_scale2: 4127,
  eSave_GrowFolder_folder_v_scrollbar2: 4128,
  eSave_GrowFolder_folder_h_scrollbar2: 4129,
  eSave_GrowFolder_folder_file_names3: 4130,
  eSave_GrowFolder_folder_text3: 4131,
  eSave_GrowFolder_folder_scale3: 4132,
  eSave_GrowFolder_folder_v_scrollbar3: 4133,
  eSave_GrowFolder_folder_h_scrollbar3: 4134,
  eSave_GrowFolder_folder_file_names4: 4135,
  eSave_GrowFolder_folder_text4: 4136,
  eSave_GrowFolder_folder_scale4: 4137,
  eSave_GrowFolder_folder_v_scrollbar4: 4138,
  eSave_GrowFolder_folder_h_scrollbar4: 4139,
  eSave_GrowFolder_folder_file_names5: 4140,
  eSave_GrowFolder_folder_text5: 4141,
  eSave_GrowFolder_folder_scale5: 4142,
  eSave_GrowFolder_folder_v_scrollbar5: 4143,
  eSave_GrowFolder_folder_h_scrollbar5: 4144,
  eSave_GrowFolder_folder_file_names6: 4145,
  eSave_GrowFolder_folder_text6: 4146,
  eSave_GrowFolder_folder_scale6: 4147,
  eSave_GrowFolder_folder_v_scrollbar6: 4148,
  eSave_GrowFolder_folder_h_scrollbar6: 4149,
  eSave_GrowFolder_folder_file_names7: 4150,
  eSave_GrowFolder_folder_text7: 4151,
  eSave_GrowFolder_folder_scale7: 4152,
  eSave_GrowFolder_folder_v_scrollbar7: 4153,
  eSave_GrowFolder_folder_h_scrollbar7: 4154,
  eSave_GrowFolder_folder_file_names8: 4155,
  eSave_GrowFolder_folder_text8: 4156,
  eSave_GrowFolder_folder_scale8: 4157,
  eSave_GrowFolder_folder_v_scrollbar8: 4158,
  eSave_GrowFolder_folder_h_scrollbar8: 4159,
  eSave_GrowFolder_folder_file_names9: 4160,
  eSave_GrowFolder_folder_text9: 4161,
  eSave_GrowFolder_folder_scale9: 4162,
  eSave_GrowFolder_folder_v_scrollbar9: 4163,
  eSave_GrowFolder_folder_h_scrollbar9: 4164,
  eSave_GrowFolder_folder_file_names10: 4165,
  eSave_GrowFolder_folder_text10: 4166,
  eSave_GrowFolder_folder_scale10: 4167,
  eSave_GrowFolder_folder_v_scrollbar10: 4168,
  eSave_GrowFolder_folder_h_scrollbar10: 4169,
  eSave_GrowFolder_folder_file_names11: 4170,
  eSave_GrowFolder_folder_text11: 4171,
  eSave_GrowFolder_folder_scale11: 4172,
  eSave_GrowFolder_folder_v_scrollbar11: 4173,
  eSave_GrowFolder_folder_h_scrollbar11: 4174,
  eSave_GrowFolder_folder_file_names12: 4175,
  eSave_GrowFolder_folder_text12: 4176,
  eSave_GrowFolder_folder_scale12: 4177,
  eSave_GrowFolder_folder_v_scrollbar12: 4178,
  eSave_GrowFolder_folder_h_scrollbar12: 4179,
  eSave_GrowFolder_folder_owner1: 4180,
  eSave_GrowFolder_folder_owner2: 4181,
  eSave_GrowFolder_folder_owner3: 4182,
  eSave_GrowFolder_folder_owner4: 4183,
  eSave_GrowFolder_folder_owner5: 4184,
  eSave_GrowFolder_folder_owner6: 4185,
  eSave_GrowFolder_folder_owner7: 4186,
  eSave_GrowFolder_folder_owner8: 4187,
  eSave_GrowFolder_folder_owner9: 4188,
  eSave_GrowFolder_folder_owner10: 4189,
  eSave_GrowFolder_folder_owner11: 4190,
  eSave_GrowFolder_folder_owner12: 4191,
  eSave_GrowTable_rect_part: 4200,
  eSave_GrowTable_userdata_cb: 4202,
  eSave_GrowTable_scrollbar_width: 4203,
  eSave_GrowTable_vertical_scrollbar: 4204,
  eSave_GrowTable_horizontal_scrollbar: 4205,
  eSave_GrowTable_scrollbar_color: 4206,
  eSave_GrowTable_scrollbar_bg_color: 4207,
  eSave_GrowTable_window_scale: 4208,
  eSave_GrowTable_rows: 4209,
  eSave_GrowTable_columns: 4210,
  eSave_GrowTable_header_row: 4211,
  eSave_GrowTable_header_column: 4212,
  eSave_GrowTable_text_size: 4213,
  eSave_GrowTable_text_drawtype: 4214,
  eSave_GrowTable_header_row_height: 4215,
  eSave_GrowTable_row_height: 4216,
  eSave_GrowTable_text_color_drawtype: 4217,
  eSave_GrowTable_header_text_size: 4218,
  eSave_GrowTable_header_text_drawtype: 4219,
  eSave_GrowTable_header_text_color: 4220,
  eSave_GrowTable_header_text_bold: 4221,
  eSave_GrowTable_options: 4222,
  eSave_GrowTable_font: 4223,
  eSave_GrowTable_column_width1: 4240,
  eSave_GrowTable_header_text1: 4241,
  eSave_GrowTable_column_width2: 4242,
  eSave_GrowTable_header_text2: 4243,
  eSave_GrowTable_column_width3: 4244,
  eSave_GrowTable_header_text3: 4245,
  eSave_GrowTable_column_width4: 4246,
  eSave_GrowTable_header_text4: 4247,
  eSave_GrowTable_column_width5: 4248,
  eSave_GrowTable_header_text5: 4249,
  eSave_GrowTable_column_width6: 4250,
  eSave_GrowTable_header_text6: 4251,
  eSave_GrowTable_column_width7: 4252,
  eSave_GrowTable_header_text7: 4253,
  eSave_GrowTable_column_width8: 4254,
  eSave_GrowTable_header_text8: 4255,
  eSave_GrowTable_column_width9: 4256,
  eSave_GrowTable_header_text9: 4257,
  eSave_GrowTable_column_width10: 4258,
  eSave_GrowTable_header_text10: 4259,
  eSave_GrowTable_column_width11: 4260,
  eSave_GrowTable_header_text11: 4261,
  eSave_GrowTable_column_width12: 4262,
  eSave_GrowTable_header_text12: 4263,
  eSave_GrowTable_select_drawtype: 4264,
  eSave_GrowTable_column_adjustment1: 4265,
  eSave_GrowTable_column_adjustment2: 4266,
  eSave_GrowTable_column_adjustment3: 4267,
  eSave_GrowTable_column_adjustment4: 4268,
  eSave_GrowTable_column_adjustment5: 4269,
  eSave_GrowTable_column_adjustment6: 4270,
  eSave_GrowTable_column_adjustment7: 4271,
  eSave_GrowTable_column_adjustment8: 4272,
  eSave_GrowTable_column_adjustment9: 4273,
  eSave_GrowTable_column_adjustment10: 4274,
  eSave_GrowTable_column_adjustment11: 4275,
  eSave_GrowTable_column_adjustment12: 4276,
  eSave_GrowXYCurve_trend_part: 4300,
  eSave_GrowAxisArc_max_value: 4400,
  eSave_GrowAxisArc_min_value: 4401,
  eSave_GrowAxisArc_arc_part: 4402,
  eSave_GrowAxisArc_lines: 4403,
  eSave_GrowAxisArc_linelength: 4404,
  eSave_GrowAxisArc_longquotient: 4405,
  eSave_GrowAxisArc_valuequotient: 4406,
  eSave_GrowAxisArc_format: 4407,
  eSave_GrowAxisArc_text_size: 4408,
  eSave_GrowAxisArc_text_drawtype: 4409,
  eSave_GrowAxisArc_text_color_drawtype: 4410,
  eSave_GrowAxisArc_userdata_cb: 4411,
  eSave_GrowPie_arc_part: 4500,
  eSave_GrowPie_sectors: 4501,
  eSave_GrowPie_min_value: 4502,
  eSave_GrowPie_max_value: 4503,
  eSave_GrowPie_sector_color1: 4504,
  eSave_GrowPie_sector_color2: 4505,
  eSave_GrowPie_sector_color3: 4506,
  eSave_GrowPie_sector_color4: 4507,
  eSave_GrowPie_sector_color5: 4508,
  eSave_GrowPie_sector_color6: 4509,
  eSave_GrowPie_sector_color7: 4510,
  eSave_GrowPie_sector_color8: 4511,
  eSave_GrowPie_sector_color9: 4512,
  eSave_GrowPie_sector_color10: 4513,
  eSave_GrowPie_sector_color11: 4514,
  eSave_GrowPie_sector_color12: 4515,
  eSave_GrowPie_sector_size1: 4516,
  eSave_GrowPie_sector_size2: 4517,
  eSave_GrowPie_sector_size3: 4518,
  eSave_GrowPie_sector_size4: 4519,
  eSave_GrowPie_sector_size5: 4520,
  eSave_GrowPie_sector_size6: 4521,
  eSave_GrowPie_sector_size7: 4522,
  eSave_GrowPie_sector_size8: 4523,
  eSave_GrowPie_sector_size9: 4524,
  eSave_GrowPie_sector_size10: 4525,
  eSave_GrowPie_sector_size11: 4526,
  eSave_GrowPie_sector_size12: 4527,
  eSave_GrowPie_userdata_cb: 4528,
  eSave_GrowBarChart_rect_part: 4600,
  eSave_GrowBarChart_bars: 4601,
  eSave_GrowBarChart_barsegments: 4602,
  eSave_GrowBarChart_min_value: 4603,
  eSave_GrowBarChart_max_value: 4604,
  eSave_GrowBarChart_bar_color1: 4605,
  eSave_GrowBarChart_bar_color2: 4606,
  eSave_GrowBarChart_bar_color3: 4607,
  eSave_GrowBarChart_bar_color4: 4608,
  eSave_GrowBarChart_bar_color5: 4609,
  eSave_GrowBarChart_bar_color6: 4610,
  eSave_GrowBarChart_bar_color7: 4611,
  eSave_GrowBarChart_bar_color8: 4612,
  eSave_GrowBarChart_bar_color9: 4613,
  eSave_GrowBarChart_bar_color10: 4614,
  eSave_GrowBarChart_bar_color11: 4615,
  eSave_GrowBarChart_bar_color12: 4616,
  eSave_GrowBarChart_vertical_lines: 4617,
  eSave_GrowBarChart_horizontal_lines: 4618,
  eSave_GrowBarChart_userdata_cb: 4619,
  eSave_GrowBarChart_line_color: 4620,
  eSave_GrowToolbar_grownode_part: 4700,
  eSave_GrowToolbar_nc: 4701,
  eSave_CustomColors_colors_size: 4800,
  eSave_CustomColors_colors: 4801,
  eSave_CustomColors_colortheme_lightness: 4802,
  eSave_CustomColors_is_default_colortheme: 4803,
  eSave_GrowBarArc_max_value: 4900,
  eSave_GrowBarArc_min_value: 4901,
  eSave_GrowBarArc_bar_width: 4902,
  eSave_GrowBarArc_bar_value: 4903,
  eSave_GrowBarArc_bar_drawtype: 4904,
  eSave_GrowBarArc_direction: 4905,
  eSave_GrowBarArc_arc_part: 4906,
  eSave_GrowBarArc_bar_bordercolor: 4907,
  eSave_GrowBarArc_bar_borderwidth: 4908,
  eSave_GrowBarArc_userdata_cb: 4909,
  eSave_GrowBarArc_bar_direction: 4910,

  ePosition_Absolute: 0,
  ePosition_Relative: 1,

  eMoveRestriction_VerticalSlider: 0,
  eMoveRestriction_HorizontalSlider: 1,
  eMoveRestriction_Disable: 2,

  eEventType_Object: 0,
  eEventType_Menu: 1,
  eEventType_Toolbar: 2,
  eEventType_Table: 3,
  eEventType_CustomColor: 4,
  eEventType_Signal: 5,

  eEvent_MB1Click: 0,
  eEvent_MB1Up: 2,
  eEvent_MB1Down: 3,
  eEvent_MB1DoubleClick: 4,
  eEvent_CursorMotion: 5,
  eEvent_ButtonMotion: 6,
  eEvent_ValueChanged: 7,
  eEvent_MenuCreate: 8,
  eEvent_MenuActivated: 9,
  eEvent_MenuDelete: 10,
  eEvent_SliderMoveStart: 11,
  eEvent_SliderMoved: 12,
  eEvent_SliderMoveEnd: 13,
  eEvent_MB3Press: 14,
  eEvent_Signal: 15
};

function GlowGeometry() {
  this.ll_x = 0;
  this.ll_y = 0;
  this.ur_x = 0;
  this.ur_y = 0;
}

function GlowFillLevelLimits() {
  this.status;
  this.direction;
  this.min;
  this.max;
}

function GlowEvent() {
  this.event = 0;
  this.type = 0;
  this.x = 0;
  this.y = 0;
  this.object = null;
  this.object_type = 0;
}

function GlowEventMenu() {
  this.event = 0;
  this.type = 0;
  this.x = 0;
  this.y = 0;
  this.object = null;
  this.object_type = 0;
  this.item = 0;
}

function GlowEventTable() {
  this.event = 0;
  this.type = 0;
  this.x = 0;
  this.y = 0;
  this.object = null;
  this.object_type = 0;
  this.column = 0;
  this.row = 0;
}

function GlowEventToolbar() {
  this.event = 0;
  this.type = 0;
  this.x = 0;
  this.y = 0;
  this.object = null;
  this.object_type = 0;
  this.category = 0;
  this.idx = 0;
}

function GlowDimension() {
  this.width = 0;
  this.height = 0;
}

function GlowRectangle() {
  this.x1;
  this.y1;
  this.x2;
  this.y2;
}
function GlowPointX() {
  this.x;
  this.y;
}

function GlowSliderInfo() {
  this.direction;
  this.max_value;
  this.min_value;
  this.max_position;
  this.min_position;
}

function MenuInfoItem() {
  this.text;
  this.type;
  this.occupied = false;
}

function GlowMenuInfo() {
  this.item = new Array(32);

  for (var i = 0; i < 32; i++) {
    this.item[i] = new MenuInfoItem();
  }
}


function GlowBackgroundObject() {
  this.background;
  this.direction;
  this.max;
  this.min;
  this.sts;
}

function GlowPoint(ctx) {
  this.x = 0.0;
  this.y = 0.0;
  this.ctx = ctx;

  this.open = function (lines, row) {
    var end = false;
    for (var i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("point : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Point:
          break;
        case Glow.eSave_Point_x:
          this.x = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Point_y:
          this.y = parseFloat(tokens[1]);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowPoint");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.posit = function (x, y) {
    this.x = x;
    this.y = y;
  };
}

function Sat(s, i_min, i_max, a) {
  this.s = s;
  this.i_min = i_min;
  this.i_max = i_max;
  this.a = a;
}

function Hue(h, hk, s) {
  this.h = h;
  this.hk = hk;
  this.s = s;
}

function Rgb(r, g, b) {
  this.r = r;
  this.g = g;
  this.b = b;
}

var GlowColor = {
  gray_i0: 0.32,		// Gray min
  gray_i1: 0.95,		// Gray max


  ctab: [new Hue(18, -20.0,
    [new Sat(0.2, -1.4, 2.8, 0.9), new Sat(0.5, -1.4, 3, 0.9),
      new Sat(1.5, -3.5, 5.0, 1.0)]), new Hue(28, 10.0,
    [new Sat(0.2, -1.4, 2.8, 0.9), new Sat(0.5, -1.4, 3, 0.9),
      new Sat(1.5, -3.5, 5.0, 1.0)]),  // Yellow
    new Hue(45, 20.0, [new Sat(0.2, -1.3, 2.8, 0.9), new Sat(0.5, -1.3, 3, 0.9),
      new Sat(1.5, -3.5, 4.5, 1.0)]), // Orange
    new Hue(90, 0.0, [new Sat(0.2, -1.1, 2.8, 0.9), new Sat(0.5, -1.1, 3, 0.9),
      new Sat(1.5, -4.5, 4.5, 0.8)]), // Red
    new Hue(150, 0.0, [new Sat(0.2, -1.0, 2.8, 0.9), new Sat(0.5, -1.0, 3, 0.9),
      new Sat(1.5, -3.5, 5.9, 1.0)]), // Violet
    new Hue(240, 0.0, [new Sat(0.2, -1.0, 2.8, 0.9), new Sat(0.4, -1.0, 3, 0.9),
      new Sat(1.5, -3.5, 5.5, 0.8)]), // Blue
    new Hue(280, 0.0, [new Sat(0.2, -1.0, 2.8, 0.9), new Sat(0.4, -1.0, 3, 0.9),
      new Sat(1.5, -3.5, 5.5, 1.2)]), // Seablue
    new Hue(355, -20.0,
      [new Sat(0.2, -1.0, 2.8, 0.9), new Sat(0.4, -1.0, 3, 0.9),
        new Sat(1.5, -4.5, 3.5, 1.0)])  // Green
  ],


  rgbtab: [new Rgb(0.254, 0.329, 0),  // YellowGreen
    new Rgb(0.357, 0.459, 0), new Rgb(0.498, 0.639, 0),
    new Rgb(0.624, 0.800, 0), new Rgb(0.764, 0.976, 0),
    new Rgb(0.808, 1.000, 0.129), new Rgb(0.847, 1.000, 0.310),
    new Rgb(0.898, 1.000, 0.537), new Rgb(0.933, 1.000, 0.710),
    new Rgb(0.949, 1.000, 0.776), new Rgb(0.0, 0.0, 0),		// Yellow
    new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0),
    new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0),
    new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0),
    new Rgb(0.0, 0.0, 0),		// Orange
    new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0),
    new Rgb(0.0, 0.0, 0), new Rgb(0.0, 0.0, 0), new Rgb(1.000, 0.725, 0.420),
    new Rgb(1.000, 0.796, 0.569), new Rgb(1.000, 0.886, 0.667),
    new Rgb(1.000, 0.937, 0.820)],


  is_shiftable: function (dt) {
    return (dt >= 20 && dt <= Glow.eDrawType_Color300);
  },

  is_custom: function (dt) {
    return (dt >= Glow.eDrawType_CustomColor1 && dt <=
      Glow.eDrawType_CustomColor__);
  },

  shift_drawtype: function (dt, shift, node) {
    var incr;
    var base_drawtype;
    var drawtype;

    if (node !== null && node.getColorInverse() !== 0) {
      shift = -shift;
    }

    if (this.is_shiftable(dt)) {
      base_drawtype = Math.floor(dt / 10) * 10;
      incr = shift + dt - base_drawtype;

      if (incr < 0) {
        drawtype = Glow.eDrawType_Color4;
      }// White
      else if (incr >= 10) {
        drawtype = Glow.eDrawType_Color30;
      }// DarkGrey
      else {
        drawtype = base_drawtype + incr;
      }
    } else if (this.is_custom(dt)) {
      if (shift === -1) {
        drawtype = (dt + 3);
      } else if (shift < 0) {
        drawtype = (dt + 2);
      } else if (shift > 0) {
        drawtype = (dt + 1);
      } else {
        drawtype = dt;
      }
    } else {
      drawtype = dt;
    }

    return Math.floor(drawtype);
  },

  get_drawtype: function (local_drawtype, highlight_drawtype, highlight, node,
                          fill, highlight_disabled) {
    var drawtype;
    var base_drawtype;
    var incr;
    var lightness = 0;
    var intensity = 0;

    if (node !== null) {
      lightness = node.getColorLightness();
      intensity = node.getColorIntensity();
    }

    if (highlight !== 0 && highlight_disabled === 0) {
      drawtype = highlight_drawtype;
    } else if (highlight !== 0 && highlight_disabled !== 0 && fill === 0 &&
      local_drawtype === Glow.eDrawType_Line) {
      drawtype = highlight_drawtype;
    } else {
      if (node !== null && node.getColorTone() !== Glow.eDrawTone_No) {
        var tone = node.getColorTone();

        if (local_drawtype > 30 && local_drawtype < 300) {
          if (tone === Glow.eDrawTone_LightGrayHighSaturation) {
            drawtype = Glow.eDrawType_Color31;
          }// GrayLow1
          else if (tone === Glow.eDrawTone_DarkGrayHighSaturation) {
            drawtype = Glow.eDrawType_Color30;
          }// DarkGrey
          else {
            if (tone >= Glow.eDrawTone_GrayHighSaturation &&
              tone < Glow.eDrawTone_GrayLowSaturation) {
              tone = tone - 27;
              intensity += 2;
            } else if (tone >= Glow.eDrawTone_GrayLowSaturation) {
              tone = tone - 2 * 27;
              intensity -= 1;
            }
            if (tone >= Glow.eDrawTone_DarkGray &&
              tone < Glow.eDrawTone_LightGray) {
              tone = (tone - 9);
              lightness -= 2;
            } else if (tone >= Glow.eDrawTone_LightGray) {
              tone = tone - 18;
              lightness += 2;
            }

            drawtype =
              local_drawtype - Math.floor(local_drawtype / 30) * 30 + 30 * tone;
          }
        } else {
          drawtype = local_drawtype;
        }
      } else if (node !== null && fill === 1 &&
        node.getFillDrawtype() !== Glow.eDrawType_No) {
        drawtype = node.getFillDrawtype();
      } else if (node !== null && fill === 2 &&
        node.getTextDrawtype() !== Glow.eDrawType_No) {
        drawtype = node.getTextDrawtype();
      } else if (node !== null && fill === 3 &&
        node.getBackgroundDrawtype() !== Glow.eDrawType_No) {
        drawtype = node.getBackgroundDrawtype();
      } else if (node !== null && fill === 0 &&
        node.getDrawtype() !== Glow.eDrawType_No) {
        drawtype = node.getDrawtype();
      } else {
        drawtype = local_drawtype;
      }

      if (node !== null && lightness !== 0) {
        if (local_drawtype >= 30 && drawtype < 300) {
          base_drawtype = Math.floor(drawtype / 10) * 10;
          incr = -lightness + drawtype - base_drawtype;
          if (incr < 0) {
            drawtype = Glow.eDrawType_Color4;
          }// White
          else if (incr >= 10) {
            drawtype = Glow.eDrawType_Color30;
          }// DarkGrey
          else {
            drawtype = base_drawtype + incr;
          }
        }
      }
      if (node !== null && intensity !== 0) {
        if (drawtype >= 60) {
          base_drawtype = Math.floor(drawtype / 30) * 30;
          incr = drawtype - base_drawtype;
          drawtype =
            (drawtype + Math.min(2 - Math.floor(incr / 10), intensity) * 10);
          if (drawtype < base_drawtype) {
            drawtype = (Glow.eDrawType_Color41 + incr);
          }
        }
      }
      if (node !== null && node.getColorShift() !== 0) {
        if (drawtype >= 60 && drawtype < 300) {
          incr =
            node.getColorShift() - Math.floor(node.getColorShift() / 8) * 8;
          if (incr < 0) {
            incr += 8;
          }

          incr = drawtype + incr * 30;
          if (incr >= 300) {
            incr -= 240;
          }
          drawtype = incr;
        }
      }
      if (node !== null && node.getColorInverse() !== 0) {
        if (drawtype >= 30 && drawtype < 300) {
          drawtype = drawtype + 10 - 2 * (drawtype % 10) - 1;
        } else if (this.is_custom(drawtype)) {
          if ((drawtype - Glow.eDrawType_CustomColor1) % 4 === 1) {
            drawtype = drawtype + 1;
          } else if ((drawtype - Glow.eDrawType_CustomColor1) % 4 === 2) {
            drawtype = drawtype - 1;
          } else if ((drawtype - Glow.eDrawType_CustomColor1) % 4 === 3) {
            drawtype = drawtype - 2;
          }
        }
      }
      if (node !== null && node.getDimmed() !== 0) {
        if (drawtype === 0) {
          drawtype = 25;
        } else if (26 <= drawtype && drawtype <= 29) {
          drawtype = (drawtype - 4);
        } else if (36 <= drawtype && drawtype <= 39) {
          drawtype = (drawtype - 4);
        } else if (46 <= drawtype && drawtype <= 49) {
          drawtype = (drawtype - 4);
        } else if (56 <= drawtype && drawtype <= 59) {
          drawtype = (drawtype - 4);
        } else if (this.is_custom(drawtype)) {
          if (drawtype === Glow.eDrawType_CustomColor37)
          // Button border color
          {
            drawtype = Glow.eDrawType_CustomColor40;
          }
          if (drawtype === Glow.eDrawType_CustomColor38)
          // Button text color
          {
            drawtype = Glow.eDrawType_CustomColor39;
          }
        }
      }
    }
    if (!((drawtype >= 0 && drawtype <= Glow.eDrawType_Color__) ||
        (drawtype >= Glow.eDrawType_CustomColor1 && drawtype <
          Glow.eDrawType_CustomColor__))) {
      console.log("** Invalid drawtype : ", drawtype);
      drawtype = Glow.eDrawType_Line;
    }
    return drawtype;
  },

  rgb_color: function (idx, customcolors) {
    var h1, i1, s1;
    var i, j, k;
    var r = 0;
    var g = 0;
    var b = 0;


    if (idx === 300) {
      idx = 31;
    }

    if (idx === 3) // White
    {
      r = g = b = 1.0;
    } else if (idx === 2) // Gray
    {
      r = g = b = 0.75;
    } else if (idx === 1) { // Red
      r = 1.0;
      g = b = 0;
    } else if (idx === 0) // Black
    {
      r = g = b = 0;
    } else if (idx < 20) {
      // Sixteen colors of different hue
      h1 = 360.0 * (idx - 4) / 16;
      s1 = 1.5;
      i1 = 1;
      return this.his_to_rgb(h1, i1, s1);
    } else if (idx < 60) {
      // Four sets of gray
      var i0 = this.gray_i0;

      if (idx < 30 || (50 <= idx && idx < 60)) {
        i0 = 0.25;
      }

      r = g = b = i0 + (this.gray_i1 - i0) * Math.pow((9 - idx % 10) / 9, 0.9);
    } else if (idx < 300) {
      i = Math.floor((idx - 60) / 30);
      j = Math.floor((idx - 60 - i * 30) / 10);
      k = 9 - (idx - 60 - i * 30 - j * 10);

      if ((i === 0 && j === 2) || (i === 2 && j === 2 && k > 5)) {
        // Formula doesn't work for yellow...
        r = this.rgbtab[i * 10 + k].r;
        g = this.rgbtab[i * 10 + k].g;
        b = this.rgbtab[i * 10 + k].b;
      } else {
        s1 = this.ctab[i].s[j].s;
        i1 = this.ctab[i].s[j].i_min +
          (this.ctab[i].s[j].i_max - this.ctab[i].s[j].i_min) *
          Math.pow(k / 9, this.ctab[i].s[j].a);
        h1 = this.ctab[i].h + this.ctab[i].hk * k / 9;

        return this.his_to_rgb(h1, i1, s1);
      }
    } else if (idx >= Glow.eDrawType_CustomColor1 &&
      idx < Glow.eDrawType_CustomColor__) {
      if (customcolors === null) {
        return new Rgb(1, 1, 1);
      } else {
        return customcolors.get_color(idx);
      }
    }
    return new Rgb(r, g, b);
  },


  his_to_rgb: function (h, i, s) {
    var m1, m2, i1;
    var r, g, b;

    m1 = s * Math.sin(h * Math.PI / 180);
    m2 = s * Math.cos(h * Math.PI / 180);
    i1 = i / Math.sqrt(3);

    r = m1 * 2 / Math.sqrt(6) + i1 / Math.sqrt(3);
    g = -m1 / Math.sqrt(6) + m2 / Math.sqrt(2) + i1 / Math.sqrt(3);
    b = -m1 / Math.sqrt(6) - m2 / Math.sqrt(2) + i1 / Math.sqrt(3);

    r = r / 2 + 0.5;
    g = g / 2 + 0.5;
    b = b / 2 + 0.5;
    if (r > 1) {
      r = 1;
    }
    if (r < 0) {
      r = 0;
    }
    if (g > 1) {
      g = 1;
    }
    if (g < 0) {
      g = 0;
    }
    if (b > 1) {
      b = 1;
    }
    if (b < 0) {
      b = 0;
    }

    return new Rgb(r, g, b);
  }
};


function GlowCustomColors() {
  var self = this;
  this.debug = false;
  this.colors_size = Glow.eDrawType_CustomColor__ - Glow.eDrawType_CustomColor1;
  this.colors = new Array(this.colors_size);
  for (var i = 0; i < this.colors_size; i++) {
    this.colors[i] = new Array(3);
  }
  this.colortheme_lightness = 0;
  this.is_default_colortheme = 0;

  for (var i = 0; i < this.colors_size; i++) {
    this.colors[i][0] = 1;
    this.colors[i][1] = 1;
    this.colors[i][2] = 1;
  }

  this.get_color = function (dtype) {
    if (dtype < Glow.eDrawType_CustomColor1 ||
      dtype >= Glow.eDrawType_CustomColor__) {
      return null;
    }

    var idx = dtype - Glow.eDrawType_CustomColor1;
    return new Rgb(this.colors[idx][0], this.colors[idx][1],
      this.colors[idx][2]);
  };


  this.open = function (lines, row) {
    var end = false;
    var i;
    var csize = 0;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.debug) {
        console.log("GlowCustomColors : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_CustomColors:
          break;
        case Glow.eSave_CustomColors_colors_size:
          csize = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_CustomColors_colortheme_lightness:
          this.colortheme_lightness = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_CustomColors_is_default_colortheme:
          this.is_default_colortheme = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_CustomColors_colors:
          for (var j = 0; j < csize; j++) {
            i++;
            tokens = lines[i].split(' ');
            this.colors[j][0] = parseFloat(tokens[0]);
            this.colors[j][1] = parseFloat(tokens[1]);
            this.colors[j][2] = parseFloat(tokens[2]);
            if (this.debug) {
              console.log(j, this.colors[j][0], this.colors[j][1],
                this.colors[j][2]);
            }
          }
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowCustomColors");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.read_colorfile = function (ctx, name) {
    ctx.appl.frame.readGrowWeb(name, this.read_cb);
  };

  this.read_cb = function (lines, row) {
    self.open(lines, row);
  };
}


function GlowArray(ctx) {
  this.a = [];
  this.ctx = ctx;

  this.add = function (elem) {
    this.a.push(elem);
  };
  this.remove = function (elem) {
    for (var i = 0; i < this.a.length; i++) {
      if (this.a[i] === elem) {
        this.a.splice(i, 1);
        return 1;
      }
    }
    return 0;
  };
  this.size = function () {
    return this.a.length;
  };
  this.get = function (idx) {
    return this.a[idx];
  };
  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);
      var o;

      if (this.ctx.debug) {
        console.log("GlowArray : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Array:
          break;
        case Glow.eSave_NodeClass: {
          o = new GlowNodeClass(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_NodeGroup: {
          o = new GlowNodeGroup(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_ConClass:
          o = new GlowConClass(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case Glow.eSave_Con: {
          o = new GlowCon(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowNode: {
          o = new GrowNode(this.ctx);
          i = o.open(lines, i + 1);
          if (o.nc === null) {
            break;
          }

          this.add(o);
          break;
        }
        case Glow.eSave_GrowSlider: {
          o = new GrowSlider(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowGroup: {
          o = new GrowGroup(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowConGlue: {
          o = new GrowConGlue(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowRect: {
          o = new GrowRect(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowRectRounded: {
          o = new GrowRectRounded(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_Line: {
          o = new GlowLine(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_Arc: {
          o = new GlowArc(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_Rect: {
          o = new GlowRect(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_Text: {
          o = new GlowText(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_Arrow: {
          o = new GlowArrow(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowArc: {
          o = new GrowArc(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowLine: {
          o = new GrowLine(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowPolyLine: {
          o = new GrowPolyline(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowText: {
          o = new GrowText(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_ConPoint: {
          o = new GlowConPoint(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowConPoint: {
          o = new GrowConPoint(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowAnnot: {
          o = new GrowAnnot(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowBar: {
          o = new GrowBar(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowBarArc: {
          o = new GrowBarArc(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowTrend: {
          o = new GrowTrend(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowXYCurve: {
          o = new GrowXYCurve(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowTable: {
          o = new GrowTable(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowAxis: {
          o = new GrowAxis(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowAxisArc: {
          o = new GrowAxisArc(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowImage: {
          o = new GrowImage(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowWindow: {
          o = new GrowWindow(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowFolder: {
          o = new GrowFolder(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowPie: {
          o = new GrowPie(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowBarChart: {
          o = new GrowBarChart(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_GrowToolbar: {
          o = new GrowToolbar(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_Point: {
          o = new GlowPoint(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        }
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowVector");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.clear = function () {
    this.a.length = 0;
  }
}

function GlowTransform() {
  this.a11 = 1;
  this.a12 = 0;
  this.a13 = 0;
  this.a21 = 0;
  this.a22 = 1;
  this.a23 = 0;
  this.rotation = 0;
  this.s_a11 = 1;
  this.s_a12 = 0;
  this.s_a13 = 0;
  this.s_a21 = 0;
  this.s_a22 = 1;
  this.s_a23 = 0;
  this.s_rotation = 0;
  this.stored = false;

  this.store = function () {
    this.s_a11 = this.a11;
    this.s_a12 = this.a12;
    this.s_a13 = this.a13;
    this.s_a21 = this.a21;
    this.s_a22 = this.a22;
    this.s_a23 = this.a23;
    this.s_rotation = this.rotation;
    this.stored = true;
  };

  this.open = function (lines, row) {
    this.debug = false;
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.debug) {
        console.log("GlowTransform : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Transform:
          break;
        case Glow.eSave_Transform_a11:
          this.a11 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Transform_a12:
          this.a12 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Transform_a13:
          this.a13 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Transform_a21:
          this.a21 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Transform_a22:
          this.a22 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Transform_a23:
          this.a23 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Transform_rotation:
          this.rotation = parseFloat(tokens[1]);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowTransform");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.rot = function () {
    if (arguments.length === 0) {
      return this.rotation;
    }
    if (arguments.length === 1) {
      var t = arguments[0];
      return t.rotation + this.rotation;
    }
  };

  this.x = function () {
    var t, x1, y1;
    if (arguments.length === 2) {
      x1 = arguments[0];
      y1 = arguments[1];
      return x1 * this.a11 + y1 * this.a12 + this.a13;
    }
    if (arguments.length === 3) {
      t = arguments[0];
      x1 = arguments[1];
      y1 = arguments[2];

      var tmp = t.multiply(this);

      return tmp.x(x1, y1);
    }
  };

  this.y = function () {
    var t, x1, y1;
    if (arguments.length === 2) {
      x1 = arguments[0];
      y1 = arguments[1];
      return x1 * this.a21 + y1 * this.a22 + this.a23;
    }
    if (arguments.length === 3) {
      t = arguments[0];
      x1 = arguments[1];
      y1 = arguments[2];

      var tmp = t.multiply(this);

      return tmp.y(x1, y1);
    }
  };

  this.multiply = function (p) {
    var tmp = new GlowTransform();
    tmp.a11 = this.a11 * p.a11 + this.a12 * p.a21;
    tmp.a12 = this.a11 * p.a12 + this.a12 * p.a22;
    tmp.a13 = this.a11 * p.a13 + this.a12 * p.a23 + this.a13;
    tmp.a21 = this.a21 * p.a11 + this.a22 * p.a21;
    tmp.a22 = this.a21 * p.a12 + this.a22 * p.a22;
    tmp.a23 = this.a21 * p.a13 + this.a22 * p.a23 + this.a23;
    tmp.rotation = this.rotation + p.rotation;
    return tmp;
  };

  this.set_from_stored = function (t) {
    this.a11 = t.a11 * this.s_a11 + t.a12 * this.s_a21;
    this.a12 = t.a11 * this.s_a12 + t.a12 * this.s_a22;
    this.a13 = t.a11 * this.s_a13 + t.a12 * this.s_a23 + t.a13;
    this.a21 = t.a21 * this.s_a11 + t.a22 * this.s_a21;
    this.a22 = t.a21 * this.s_a12 + t.a22 * this.s_a22;
    this.a23 = t.a21 * this.s_a13 + t.a22 * this.s_a23 + t.a23;
    this.rotation = this.s_rotation + t.rotation;
  };

  this.scale = function (sx, sy, x0, y0) {
    this.a13 = this.a13 * sx + x0 * (1 - sx);
    this.a23 = this.a23 * sy + y0 * (1 - sy);
    this.a11 *= sx;
    this.a12 *= sx;
    this.a21 *= sy;
    this.a22 *= sy;
  };

  this.scale_from_stored = function (sx, sy, x0, y0) {
    this.a13 = this.s_a13 * sx + x0 * (1 - sx);
    this.a23 = this.s_a23 * sy + y0 * (1 - sy);
    this.a11 = this.s_a11 * sx;
    this.a12 = this.s_a12 * sx;
    this.a21 = this.s_a21 * sy;
    this.a22 = this.s_a22 * sy;
  };

  this.rotate = function (angle, x0, y0) {
    var sin_a;
    var cos_a;
    var tmp = new GlowTransform();
    tmp.a11 = this.a11;
    tmp.a12 = this.a12;
    tmp.a13 = this.a13;
    tmp.a21 = this.a21;
    tmp.a22 = this.a22;
    tmp.a23 = this.a23;
    tmp.rotation = this.rotation;

    if (-90.01 < angle && angle < -89.99) {
      sin_a = -1.0;
      cos_a = 0.0;
    } else {
      sin_a = Math.sin(angle / 180 * 3.14159);
      cos_a = Math.cos(angle / 180 * 3.14159);
    }

    this.a11 = tmp.a11 * cos_a - tmp.a21 * sin_a;
    this.a12 = tmp.a12 * cos_a - tmp.a22 * sin_a;
    this.a13 =
      tmp.a13 * cos_a - tmp.a23 * sin_a + x0 * (1 - cos_a) + y0 * sin_a;
    this.a21 = tmp.a11 * sin_a + tmp.a21 * cos_a;
    this.a22 = tmp.a21 * sin_a + tmp.a22 * cos_a;
    this.a23 =
      tmp.a13 * sin_a + tmp.a23 * cos_a + y0 * (1 - cos_a) - x0 * sin_a;
    this.rotation += angle;
  };

  this.rotate_from_stored = function (angle, x0, y0) {

    var sin_a;
    var cos_a;
    if (-90.01 < this.s_rotation + angle && this.s_rotation + angle < -89.99) {
      sin_a = -1.0;
      cos_a = 0.0;
    } else {
      sin_a = Math.sin((this.s_rotation + angle) / 180 * 3.14159);
      cos_a = Math.cos((this.s_rotation + angle) / 180 * 3.14159);
    }

    this.a11 = this.s_a11 * cos_a - s_a21 * sin_a;
    this.a12 = this.s_a12 * cos_a - s_a22 * sin_a;
    this.a13 =
      this.s_a13 * cos_a - s_a23 * sin_a + x0 * (1 - cos_a) + y0 * sin_a;
    this.a21 = this.s_a11 * sin_a + s_a21 * cos_a;
    this.a22 = this.s_a21 * sin_a + s_a22 * cos_a;
    this.a23 =
      this.s_a13 * sin_a + s_a23 * cos_a + y0 * (1 - cos_a) - x0 * sin_a;
    this.rotation = this.s_rotation + angle;
  };

  this.move = function (x0, y0) {
    this.a13 += x0;
    this.a23 += y0;
  };

  this.move_from_stored = function (x0, y0) {
    this.a13 = this.s_a13 + x0;
    this.a23 = this.s_a23 + y0;
  };

  this.posit = function (x0, y0) {
    this.a13 = x0;
    this.a23 = y0;
  };


  this.reverse = function (x, y) {
    var p = new GlowPoint();
    if (this.a11 === 0 || (this.a12 * this.a21 - this.a11 * this.a22) === 0) {
      if (this.a11 === 0 && this.a22 === 0 && this.a12 !== 0 &&
        this.a21 !== 0) {
        p.y = (x - this.a13) / this.a12;
        p.x = (y - this.a23) / this.a21;
        return p;
      } else {
        p.y = p.x = 0;
        return p;
      }
    }
    p.y = (this.a11 * (this.a23 - y) - this.a21 * (this.a13 - x)) /
      (this.a12 * this.a21 - this.a11 * this.a22);
    p.x = (x - this.a12 * p.y - this.a13) / this.a11;
    return p;
  };

  this.vertical_scale = function (t) {
    if (t === null) {
      return Math.sqrt(this.a12 * this.a12 + this.a22 * this.a22);
    }

    var tmp = t.multiply(this);

    return Math.sqrt(tmp.a12 * tmp.a12 + tmp.a22 * tmp.a22);
  };

  this.is_stored = function () {
    return this.stored;
  };
}

function GlowNodeClass(ctx) {
  this.ctx = ctx;
  this.a = new GlowArray(ctx);
  this.nc_name = null;
  this.group = 0;
  this.slider = 0;
  this.next_nodeclass = null;
  this.animation_count = 0;
  this.cycle = 0;
  this.x0 = 0;
  this.y0 = 0;
  this.x1 = 0;
  this.y1 = 0;
  this.input_focus_mark = 0;
  this.dynamicsize = 0;
  this.userdata = null;
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.action_type1 = 0;
  this.action_type2 = 0;
  this.dyn_color = new Array(4);
  this.dyn_attr = [];
  this.next_nc = null;
  this.prev_nc = null;
  this.recursive_trace = 0;

  this.type = function () {
    return Glow.eObjectType_GlowNodeClass;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowNodeClass : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_NodeClass:
          break;
        case Glow.eSave_NodeClass_nc_name:
          this.nc_name = tokens[1];
          break;
        case Glow.eSave_NodeClass_a:
          i = this.a.open(lines, i + 1);
          break;
        case Glow.eSave_NodeClass_group:
          this.group = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_arg_cnt:
        case Glow.eSave_NodeClass_argname:
        case Glow.eSave_NodeClass_argtype:
        case Glow.eSave_NodeClass_no_con_obstacle:
          break;
        case Glow.eSave_NodeClass_dyn_type1:
          this.dyn_type1 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_dyn_type2:
          this.dyn_type2 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_dyn_action_type1:
          this.action_type1 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_dyn_action_type2:
          this.action_type2 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_dyn_color1:
          this.dyn_color[0] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_dyn_color2:
          this.dyn_color[1] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_dyn_color3:
          this.dyn_color[2] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_dyn_color4:
          if (tokens.length > 1) {
            this.dyn_color[3] = parseInt(tokens[1], 10);
          }
          break;
        case Glow.eSave_NodeClass_dyn_attr1:
          if (tokens.length > 1) {
            this.dyn_attr[0] = parseInt(tokens[1], 10);
          }
          break;
        case Glow.eSave_NodeClass_dyn_attr2:
          if (tokens.length > 1) {
            this.dyn_attr[1] = parseInt(tokens[1], 10);
          }
          break;
        case Glow.eSave_NodeClass_dyn_attr3:
          if (tokens.length > 1) {
            this.dyn_attr[2] = parseInt(tokens[1], 10);
          }
          break;
        case Glow.eSave_NodeClass_dyn_attr4:
          if (tokens.length > 1) {
            this.dyn_attr[3] = parseInt(tokens[1], 10);
          }
          break;
        case Glow.eSave_NodeClass_java_name:
          break;
        case Glow.eSave_NodeClass_slider:
          this.slider = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_next_nodeclass:
          if (tokens.length > 1) {
            this.next_nodeclass = tokens[1];
          }
          break;
        case Glow.eSave_NodeClass_animation_count:
          this.animation_count = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_cycle:
          this.cycle = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_y0:
          this.y0 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_NodeClass_y1:
          this.y1 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_NodeClass_x0:
          this.x0 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_NodeClass_x1:
          this.x1 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_NodeClass_input_focus_mark:
          this.input_focus_mark = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_recursive_trace:
          this.recursive_trace = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_NodeClass_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case Glow.eSave_NodeClass_userdata_cb:
          if (ctx.appl !== null) {
            var ret = ctx.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_NodeClass);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowNodeClass");
          break;
      }
      if (end) {
        break;
      }
    }


    if (this.next_nodeclass !== null && this.next_nodeclass !== "") {
      this.next_nc = this.ctx.get_nodeclass_from_name(this.next_nodeclass);
      if (this.next_nc === null) {
        console.log("GlowNode:next_nodeclass not found: ", this.nc_name,
          this.next_nodeclass);
        console.log("Note! nodeclass has to be intern");
      } else if (this.next_nc.prev_nc !== null) {
        this.next_nc = null;
        console.log("GlowNode:next_nodeclass already chained: ", this.nc_name,
          this.next_nodeclass);
      } else {
        this.next_nc.prev_nc = this;
      }
    }
    return i;
  };

  this.event_handler = function (event, fx, fy) {

    for (var i = 0; i < this.a.size(); i++) {
      var sts = this.a.get(i).event_handler(event, fx, fy);
      if (sts !== 0) {
        return sts;
      }
    }
    return 0;
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    for (var i = 0; i < this.a.size(); i++) {
      this.a.get(i).tdraw(t, highlight, hot, node, colornode);
    }
  };

  this.get_borders = function (t, g) {
    var base = this.get_base_nc();

    if ((t === null ||
        (t !== null && Math.abs(t.rotation / 90 - Math.floor(t.rotation / 90)) <
          Number.MIN_VALUE)) &&
      !(Math.abs(base.x0 - base.x1) < Number.MIN_VALUE ||
        Math.abs(base.y0 - base.y1) < Number.MIN_VALUE)) {
      // Borders are given i x0, y0, x1, y1
      // Will not work in rotated nodes
      var ll_x, ur_x, ll_y, ur_y, kx1, kx2, ky1, ky2;

      if (t === null) {
        kx1 = base.x0;
        kx2 = base.x1;
        ky1 = base.y0;
        ky2 = base.y1;
      } else {
        kx1 = t.x(base.x0, base.y0);
        kx2 = t.x(base.x1, base.y1);
        ky1 = t.y(base.x0, base.y0);
        ky2 = t.y(base.x1, base.y1);
      }

      ll_x = Math.min(kx1, kx2);
      ur_x = Math.max(kx1, kx2);
      ll_y = Math.min(ky1, ky2);
      ur_y = Math.max(ky1, ky2);

      if (ll_x < g.ll_x) {
        g.ll_x = ll_x;
      }
      if (ur_x > g.ur_x) {
        g.ur_x = ur_x;
      }
      if (ll_y < g.ll_y) {
        g.ll_y = ll_y;
      }
      if (ur_y > g.ur_y) {
        g.ur_y = ur_y;
      }
    } else {
      for (var i = 0; i < this.a.size(); i++) {
        this.a.get(i).get_borders(t, g);
      }
    }
  };

  this.get_base_nc = function () {
    var base;

    for (base = this; base.prev_nc !== null; base = base.prev_nc) {
    }
    return base;
  };

  this.getAnnotationTextExtent = function (t, node, num) {
    var i;

    for (i = 0; i < this.a.size(); i++) {
      if (this.a.get(i).type() === Glow.eObjectType_GrowAnnot &&
        this.a.get(i).getNumber() === num) {
        var d = this.a.get(i).getTextExtent(t, node);
        d.width /= this.ctx.mw.zoom_factor_x;
        d.height /= this.ctx.mw.zoom_factor_y;
        return d;
      }

    }
    return null;
  };

}

function GlowNodeGroup(ctx) {
  GlowNodeClass.call(this, ctx);
  this.glownodeclass_open = this.open;


  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowNodeClass : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_NodeGroup:
          break;
        case Glow.eSave_NodeGroup_nodeclass_part:
          i = this.glownodeclass_open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowNodeGroup");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };
}

GlowNodeGroup.prototype = Object.create(GlowNodeClass.prototype);

function GlowConClass(ctx) {
  this.ctx = ctx;
  this.cc_name;
  this.con_type;
  this.corner;
  this.draw_type;
  this.line_width;
  this.arrow_width;
  this.arrow_length;
  this.round_corner_amount;
  this.group;

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowConClass : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_ConClass:
          break;

        case Glow.eSave_ConClass_cc_name:
          if (tokens.length > 1) {
            this.cc_name = tokens[1];
          }
          break;
        case Glow.eSave_ConClass_con_type:
          this.con_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_ConClass_corner:
          this.corner = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_ConClass_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_ConClass_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_ConClass_arrow_width:
          this.arrow_width = parseFloat(tokens[1]);
          break;
        case Glow.eSave_ConClass_arrow_length:
          this.arrow_length = parseFloat(tokens[1]);
          break;
        case Glow.eSave_ConClass_round_corner_amount:
          this.round_corner_amount = parseFloat(tokens[1]);
          break;
        case Glow.eSave_ConClass_group:
          this.group = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowConClass");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };
}

function GlowCon(ctx) {
  this.MAX_POINT = 8;

  this.ctx = ctx;
  this.cc_name;
  this.cc;
  this.c_name;
  this.x_right;
  this.x_left;
  this.y_high;
  this.y_low;
  this.dest_node;
  this.source_node;
  this.dest_conpoint;
  this.source_conpoint;
  this.source_direction;
  this.dest_direction;
  this.p_num;
  this.l_num;
  this.a_num;
  this.arrow_num;
  this.ref_num;
  this.point_x = new Array(this.MAX_POINT);
  this.point_y = new Array(this.MAX_POINT);
  this.line_a = new GlowArray(ctx);
  this.arc_a = new GlowArray(ctx);
  this.arrow_a = new GlowArray(ctx);
  this.ref_a = new GlowArray(ctx);
  this.temporary_ref;
  this.source_ref_cnt;
  this.dest_ref_cnt;
  this.trace_object;
  this.trace_attribute;
  this.trace_attr_type;
  this.border;
  this.shadow;
  this.highlight = 0;
  this.hot = 0;

  this.type = function () {
    return Glow.eObjectType_Con;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;
    var name;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowCon : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Con:
          break;
        case Glow.eSave_Con_cc:
          this.cc_name = tokens[1];
          this.cc = this.ctx.get_conclass_from_name(this.cc_name);
          if (this.cc === null) {
            console.log("GlowCon : ", lines[i], ", con class not found: ",
              this.cc_name);
          }
          break;
        case Glow.eSave_Con_c_name:
          if (tokens.length > 1) {
            this.c_name = tokens[1];
          }
          break;
        case Glow.eSave_Con_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Con_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Con_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Con_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Con_dest_node:
          if (tokens.length > 1) {
            name = tokens[1];
            this.dest_node = this.ctx.get_node_from_name(name);
            if (this.dest_node == null) {
              console.log("GlowCon:dest_node not found");
            }
          }
          break;
        case Glow.eSave_Con_source_node:
          if (tokens.length > 1) {
            name = tokens[1];
            this.source_node = this.ctx.get_node_from_name(name);
            if (this.source_node == null) {
              console.log("GlowCon:source_node not found");
            }
          }
          break;
        case Glow.eSave_Con_dest_conpoint:
          this.dest_conpoint = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_source_conpoint:
          this.source_conpoint = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_dest_direction:
          this.dest_direction = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_source_direction:
          this.source_direction = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_line_a:
          i = this.line_a.open(lines, i + 1);
          break;
        case Glow.eSave_Con_arc_a:
          i = this.arc_a.open(lines, i + 1);
          break;
        case Glow.eSave_Con_arrow_a:
          i = this.arrow_a.open(lines, i + 1);
          break;
        case Glow.eSave_Con_ref_a:
          i = this.ref_a.open(lines, i + 1);
          break;
        case Glow.eSave_Con_p_num:
          this.p_num = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_l_num:
          this.l_num = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_a_num:
          this.a_num = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_arrow_num:
          this.arrow_num = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_ref_num:
          this.ref_num = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_point_x:
          for (var j = 0; j < this.p_num; j++) {
            this.point_x[j] = parseFloat(lines[i]);
            i++;
          }
          break;
        case Glow.eSave_Con_point_y:
          for (var j = 0; j < this.p_num; j++) {
            this.point_y[j] = parseFloat(lines[i]);
            i++;
          }
          break;
        case Glow.eSave_Con_source_ref_cnt:
          this.source_ref_cnt = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_dest_ref_cnt:
          this.dest_ref_cnt = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_trace_object:
          if (tokens.length > 1) {
            this.trace_object = tokens[1];
          }
          break;
        case Glow.eSave_Con_trace_attribute:
          if (tokens.length > 1) {
            this.trace_attribute = tokens[1];
          }
          break;
        case Glow.eSave_Con_trace_attr_type:
          this.trace_attr_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_temporary_ref:
          this.temporary_ref = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Con_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowCon");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.draw = function () {
    this.tdraw(null, this.highlight, this.hot, null, null);
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var tmp;
    var i;

    if (this.temporary_ref !== 0 ||
      this.cc.con_type === Glow.eConType_Reference) {
      // ref_a.draw( w, &cc->zero, highlight, hot, NULL);
    } else {
      for (i = 0; i < this.l_num; i++) {
        this.line_a.get(i).draw(highlight, hot);
      }
      for (i = 0; i < this.a_num; i++) {
        this.arc_a.get(i).draw(highlight, hot);
      }
      // arrow_a.draw( highlight, hot);
      if ((this.shadow !== 0 || this.border !== 0) &&
        this.cc.con_type === Glow.eConType_Routed &&
        this.cc.corner === Glow.eCorner_Rounded) {
        for (i = 0; i < this.l_num; i++) {
          this.line_a.get(i)
            .draw_shadow(this.border, this.shadow, highlight, hot);
        }
        for (i = 0; i < this.a_num; i++) {
          this.arc_a.get(i)
            .draw_shadow(this.border, this.shadow, highlight, hot);
        }
      }
    }
  };
  this.getName = function () {
    return "Con";
  };
  this.event_handler = function (event, fx, fy) {
    return 0;
  };
}

function GlowLine(ctx) {
  this.ctx = ctx;
  this.p1 = new GlowPoint(ctx);
  this.p2 = new GlowPoint(ctx);
  this.draw_type;
  this.line_width;

  this.type = function () {
    return Glow.eObjectType_Line;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowCon : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Line:
          break;
        case Glow.eSave_Line_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Line_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Line_p1:
          i = this.p1.open(lines, i + 1);
          break;
        case Glow.eSave_Line_p2:
          i = this.p2.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowLine");
          break;
      }
      if (end) {
        break;
      }

    }

    return i;
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
  };


  this.draw = function (highlight, hot) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var p1_x, p1_y, p2_x, p2_y;
    p1_x = Math.floor(this.p1.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    p1_y = Math.floor(this.p1.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;
    p2_x = Math.floor(this.p2.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    p2_y = Math.floor(this.p2.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;

    if (p1_x === p2_x && p1_y === p2_y) {
      return;
    }
    var idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    this.ctx.gdraw.line(p1_x, p1_y, p2_x, p2_y, this.draw_type, idx, highlight);
  };

  this.draw_shadow = function (border, shadow, highlight, hot) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var p1_x, p1_y, p2_x, p2_y;
    p1_x = Math.floor(this.p1.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    p1_y = Math.floor(this.p1.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;
    p2_x = Math.floor(this.p2.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    p2_y = Math.floor(this.p2.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;

    if (p1_x === p2_x && p1_y === p2_y) {
      return;
    }
    var idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var ish = 1;
    var offs = border;

    if (shadow !== 0 && idx > 2) {
      if (p1_x === p2_x) {
        // Vertical line
        this.ctx.gdraw.line(p1_x + idx / 2 - idx + offs, p1_y, p2_x + idx / 2 -
          idx + offs, p2_y,
          GlowColor.shift_drawtype(this.draw_type, -2, null), ish - 1,
          highlight);
        this.ctx.gdraw.line(p1_x + idx / 2 - offs, p1_y, p2_x + idx / 2 - offs,
          p2_y, GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
          highlight);
      } else {
        // Horizontal line
        this.ctx.gdraw.line(p1_x, p1_y + idx / 2 - idx + offs, p2_x, p2_y +
          idx / 2 - idx + offs,
          GlowColor.shift_drawtype(this.draw_type, -2, null), ish - 1,
          highlight);
        this.ctx.gdraw.line(p1_x, p1_y + idx / 2 - offs, p2_x, p2_y + idx / 2 -
          offs, GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
          highlight);
      }
    }
    if (border !== 0) {
      if (p1_x === p2_x) {
        // Vertical line
        this.ctx.gdraw.line(p1_x + idx / 2 - idx, p1_y, p2_x + idx / 2 - idx,
          p2_y, Glow.eDrawType_Line, 0, highlight);
        if (idx > 0) {
          this.ctx.gdraw.line(p1_x + idx / 2, p1_y, p2_x + idx / 2, p2_y,
            Glow.eDrawType_Line, 0, highlight);
        }
      } else {
        // Horizontal line
        this.ctx.gdraw.line(p1_x, p1_y + idx / 2 - idx, p2_x, p2_y + idx / 2 -
          idx, Glow.eDrawType_Line, 0, highlight);
        if (idx > 0) {
          this.ctx.gdraw.line(p1_x, p1_y + idx / 2, p2_x, p2_y + idx / 2,
            Glow.eDrawType_Line, 0, highlight);
        }
      }
    }
  }
}

function GlowArc(ctx) {
  this.ctx = ctx;
  this.ll = new GlowPoint(ctx);
  this.ur = new GlowPoint(ctx);
  this.draw_type;
  this.angle1;
  this.angle2;
  this.line_width;
  this.display_level;
  this.fill;

  this.type = function () {
    return Glow.eObjectType_Arc;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowCon : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Arc:
          break;
        case Glow.eSave_Arc_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_angle1:
          this.angle1 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_angle2:
          this.angle2 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case Glow.eSave_Arc_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowArc");
          break;
      }
      if (end) {
        break;
      }

    }

    return i;
  };

  this.draw = function (highlight, hot) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var ll_x, ll_y, ur_x, ur_y;
    ll_x = Math.floor(this.ll.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    ll_y = Math.floor(this.ll.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;
    ur_x = Math.floor(this.ur.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    ur_y = Math.floor(this.ur.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;

    if (ll_x === ur_x && ll_y === ur_y) {
      return;
    }
    var idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1,
      this.angle2, this.draw_type, idx, highlight);
  };

  this.draw_shadow = function (border, shadow, highlight, hot) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var ll_x, ll_y, ur_x, ur_y;
    ll_x = Math.floor(this.ll.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    ll_y = Math.floor(this.ll.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;
    ur_x = Math.floor(this.ur.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    ur_y = Math.floor(this.ur.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;
    var idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var ish = 1;
    var offs = border;

    if (shadow !== 0 && idx > 2) {
      if (this.angle1 === 0) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs *
          2, this.angle1 + 45, this.angle2 - 45,
          GlowColor.shift_drawtype(this.draw_type, -2, null), ish - 1,
          highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
          this.angle1, this.angle2 - 45,
          GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
          highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1 +
          45, this.angle2 - 45,
          GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
          highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2 -
          45, GlowColor.shift_drawtype(this.draw_type, -2, null), ish - 1,
          highlight);
      } else if (this.angle1 === 90) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
          this.angle1, this.angle2,
          GlowColor.shift_drawtype(draw_type, -2, null), ish - 1, highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2,
          GlowColor.shift_drawtype(draw_type, 2, null), ish - 1, highlight);
      } else if (this.angle1 === 180) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs *
          2, this.angle1 + 45, this.angle2 - 45,
          GlowColor.shift_drawtype(draw_type, 2, null), ish - 1, highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
          this.angle1, this.angle2 - 45,
          GlowColor.shift_drawtype(draw_type, -2, null), ish - 1, highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1 +
          45, this.angle2 - 45,
          GlowColor.shift_drawtype(draw_type, -2, null), ish - 1, highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2 -
          45, GlowColor.shift_drawtype(draw_type, 2, null), ish - 1, highlight);
      } else if (this.angle1 === 270) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
          this.angle1, this.angle2,
          GlowColor.shift_drawtype(draw_type, 2, null), ish - 1, highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2,
          GlowColor.shift_drawtype(draw_type, -2, null), ish - 1, highlight);
      }
    }
    if (border !== 0) {
      this.ctx.gdraw.arc(ll_x + idx / 2 - idx, ll_y + idx / 2 - idx, ur_x -
        ll_x + idx, ur_y - ll_y + idx, this.angle1, this.angle2,
        Glow.eDrawType_Line, 0, highlight);
      if (idx > 0) {
        this.ctx.gdraw.arc(ll_x + idx / 2, ll_y + idx / 2,
          Math.max(0, ur_x - ll_x - idx), Math.max(0, ur_y - ll_y - idx),
          this.angle1, this.angle2, Glow.eDrawType_Line, 0, highlight);
      }
    }
  };
}

function GlowRect(ctx) {
  this.ctx = ctx;
  this.ll = new GlowPoint(ctx);
  this.ur = new GlowPoint(ctx);
  this.draw_type;
  this.line_width;
  this.display_level;
  this.fill;

  this.type = function () {
    return Glow.eObjectType_Rect;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowCon : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Rect:
          break;
        case Glow.eSave_Rect_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_display_level:
          this.display_level = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case Glow.eSave_Rect_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowRect");
          break;
      }
      if (end) {
        break;
      }
    }

    return i;
  };

  this.draw = function (highlight, hot) {
  };

  this.draw_shadow = function (border, shadow, highlight, hot) {
  };
}

function GlowText(ctx) {
  this.ctx = ctx;
  this.p = new GlowPoint(ctx);
  this.text;
  this.draw_type;
  this.text_size;
  this.display_level;
  this.color_drawtype;

  this.type = function () {
    return Glow.eObjectType_Text;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowCon : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Text:
          break;
        case Glow.eSave_Text_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Text_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Text_color_drawtype:
          this.color_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Text_text:
          this.text = lines[i].substring(4);
          break;
        case Glow.eSave_Text_p:
          i = this.p.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowText");
          break;
      }
      if (end) {
        break;
      }

    }
    return i;
  };

  this.draw = function (hightlight, hot) {
  };

  this.draw_shadow = function (border, shadow, hightlight, hot) {
  };
}

function GrowNode(ctx) {
  this.MAX_CONPOINTS = 32;
  this.ctx = ctx;
  this.original_border_drawtype = Glow.eDrawType_No;
  this.border_drawtype = 0;
  this.background_drawtype = Glow.eDrawType_No;
  this.original_fill_drawtype = Glow.eDrawType_No;
  this.original_text_drawtype = Glow.eDrawType_No;
  this.original_background_drawtype = Glow.eDrawType_No;
  this.original_color_tone = Glow.eDrawType_No;
  this.original_color_lightness = 0;
  this.original_color_intensity = 0;
  this.original_color_shift = 0;
  this.color_inverse = 0;
  this.color_lightness = 0;
  this.color_intensity = 0;
  this.color_shift = 0;
  this.color_tone = Glow.eDrawType_No;
  this.fill_drawtype = Glow.eDrawType_No;
  this.text_drawtype = 0;
  this.draw_type = 0;
  this.dimmed = 0;
  this.invisible = 0;
  this.flip_horizontal = 0;
  this.flip_vertical = 0;
  this.dynamicsize = 0;
  this.trf = new GlowTransform();
  this.arg_cnt = 0;
  this.line_width = 0;
  this.shadow = 0;
  this.gradient = 0;
  this.text_type = Glow.eDrawType_TextHelvetica;
  this.text_font = Glow.eFont_No;
  this.disable_cb = 0;

  this.nc_name = null;
  this.nc = null;
  this.nc_root = null;
  this.n_name = null;
  this.x_right = 0;
  this.x_left = 0;
  this.y_high = 0;
  this.y_low = 0;
  this.annotsize = new Array(10);
  this.annotv = new Array(10);
  this.pos = new GlowPoint(this.ctx);
  this.access = 0;
  this.cycle = 0;
  this.ref_object = null;
  this.userdata = null;

  this.visible = 1;
  this.highlight = 0;
  this.hot = 0;
  this.input_selected = 0;
  this.annotv_inputmode = [];
  this.fill_level = 1;
  this.level_fill_drawtype = Glow.eDrawType_No;
  this.level_color_tone = Glow.eDrawTone_No;
  this.level_direction = 0;
  this.annot_scrollingtext = -1;
  this.annot_offset_x = 0;
  this.annot_offset_y = 0;

  for (var i = 0; i < 10; i++) {
    this.annotv[i] = null;
  }

  this.type = function () {
    return Glow.eObjectType_GrowNode;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowNode : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowNode:
          break;
        case Glow.eSave_GrowNode_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_original_fill_drawtype:
          this.original_fill_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_fill_drawtype:
          this.fill_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_text_drawtype:
          this.text_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_original_text_drawtype:
          this.original_text_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_original_color_tone:
          this.original_color_tone = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_original_background_drawtype:
          this.original_background_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_background_drawtype:
          this.background_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_color_tone:
          this.color_tone = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_original_color_lightness:
          this.original_color_lightness = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_color_lightness:
          this.color_lightness = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_original_color_intensity:
          this.original_color_intensity = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_color_intensity:
          this.color_intensity = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_original_color_shift:
          this.original_color_shift = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_color_shift:
          this.color_shift = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_invisible:
          this.invisible = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_flip_horizontal:
          this.flip_horizontal = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_flip_vertical:
          this.flip_vertical = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case Glow.eSave_GrowNode_node_part:
          i = this.glownode_open(lines, i + 1);
          break;
        case Glow.eSave_GrowNode_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case Glow.eSave_GrowNode_arg_cnt:
          this.arg_cnt = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_argsize:
          i += this.arg_cnt;
          break;
        case Glow.eSave_GrowNode_argv:
          i += this.arg_cnt;
          break;
        case Glow.eSave_GrowNode_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_gradient:
          this.gradient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_text_type:
          this.text_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_text_font:
          this.text_font = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_disable_cb:
          this.disable_cb = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowNode_userdata_cb:
          if (this.ctx.appl !== null) {
            var ret = ctx.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowNode");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.glownode_open = function (lines, row) {
    var end = false;
    var i, j;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowNode : " + lines[i]);
      }

      switch (key) {

        case Glow.eSave_Node:
          break;
        case Glow.eSave_Node_nc:
          this.nc_name = tokens[1];
          if (this.type() !== Glow.eObjectType_GrowGroup) {
            this.nc = ctx.get_nodeclass_from_name(this.nc_name);
            this.nc_root = this.nc;
            if (this.nc === null) {
              console.log("GlowNode : ", lines[i], ", node class not found: ",
                this.nc_name);
            }
          }
          break;
        case Glow.eSave_Node_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case Glow.eSave_Node_refcon_cnt:
          i += this.MAX_CONPOINTS;
          break;
        case Glow.eSave_Node_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Node_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Node_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Node_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Node_obst_x_right:
        case Glow.eSave_Node_obst_x_left:
        case Glow.eSave_Node_obst_y_high:
        case Glow.eSave_Node_obst_y_low:
          break;
        case Glow.eSave_Node_annotsize:
          for (j = 0; j < 10; j++) {
            i++;
            this.annotsize[j] = parseInt(lines[i], 10);
            if (this.ctx.debug) {
              console.log("GlowNode  annotsize: ", this.annotsize[j]);
            }
          }
          break;
        case Glow.eSave_Node_annotv:
          for (j = 0; j < 10; j++) {
            if (this.annotsize[j] !== 0) {
              i++;
              this.annotv[j] = lines[i].substring(1, lines[i].length - 1);
              if (this.ctx.debug) {
                console.log("GrowNode  annotv: ", this.annotv[j]);
              }
            }
          }
          break;
        case Glow.eSave_Node_pos:
          i = this.pos.open(lines, i + 1);
          break;
        case Glow.eSave_Node_trace_data1:
        case Glow.eSave_Node_trace_data2:
        case Glow.eSave_Node_trace_data3:
        case Glow.eSave_Node_trace_data4:
        case Glow.eSave_Node_trace_data5:
        case Glow.eSave_Node_trace_data6:
        case Glow.eSave_Node_trace_data7:
        case Glow.eSave_Node_trace_data8:
        case Glow.eSave_Node_trace_data9:
        case Glow.eSave_Node_trace_data10:
        case Glow.eSave_Node_trace_attr_type:
        case Glow.eSave_Node_trace_color:
        case Glow.eSave_Node_trace_color2:
          break;
        case Glow.eSave_Node_access:
          this.access = parseInt(tokens[1], 10); // TODO, Can be unsigned
          break;
        case Glow.eSave_Node_cycle:
          this.cycle = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Node_ref_object:
          if (tokens.length > 1) {
            this.ref_object = tokens[1];
          }
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowNode");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.event_handler = function (event, fx, fy) {

    if (this.visible === 0 || this.dimmed !== 0) {
      return 0;
    }

    switch (event.event) {
      case Glow.eEvent_CursorMotion:
        return 0;
      default:
        break;
    }

    var rp = this.trf.reverse(fx, fy);
    var sts = this.nc.event_handler(event, rp.x, rp.y);
    if (sts !== 0) {
      if (!(this.type() === Glow.eObjectType_GrowGroup &&
          this.ctx.callback_object !== null)) {
        this.ctx.register_callback_object(Glow.eObjectType_Node, this);
      }
      return sts;
    }
    return 0;
  };

  this.draw = function () {
    if (this.visible !== 0) {
      this.tdraw(null, this.highlight, this.hot, null, null);
    }
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    if (node === null) {
      node = this;
    }
    if (colornode === null) {
      colornode = this;
    } else if (colornode.fill_drawtype === Glow.eDrawType_No &&
      colornode.color_tone === Glow.eDrawTone_No) {
      colornode = this;
    }

    if (node.invisible !== 0 || this.invisible !== 0) {
      return;
    }

    if (this.annot_scrollingtext !== -1) {
      this.ctx.gdraw.set_clip_rectangle(this.x_left *
        this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x, this.y_low *
        this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y, this.x_right *
        this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x, this.y_high *
        this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y);
    }

    var trf_tot;
    if (this.fill_level === 1) {
      if (t === null) {
        this.nc.tdraw(this.trf, highlight, hot, node, node);
      } else {
        trf_tot = t.multiply(this.trf);

        this.nc.tdraw(trf_tot, highlight, hot, this, this);
      }
    } else {
      var x1, x2, y1, y2;
      var x_level = 0;
      var y_level = 0;
      var clip_sts = 0;
      var old_color_tone = 0;
      var old_fill_drawtype = 0;

      if (t === null) {
        x1 = Math.floor(this.x_left * this.ctx.mw.zoom_factor_x + 0.5) -
          this.ctx.mw.offset_x;
        y1 = Math.floor(this.y_low * this.ctx.mw.zoom_factor_y + 0.5) -
          this.ctx.mw.offset_y;
        x2 = Math.floor(this.x_right * this.ctx.mw.zoom_factor_x + 0.5) -
          this.ctx.mw.offset_x;
        y2 = Math.floor(this.y_high * this.ctx.mw.zoom_factor_y + 0.5) -
          this.ctx.mw.offset_y;
      } else {
        x1 = Math.floor(t.x(x_left, y_low) * this.ctx.mw.zoom_factor_x + 0.5) -
          this.ctx.mw.offset_x;
        y1 = Math.floor(t.y(x_left, y_low) * this.ctx.mw.zoom_factor_y + 0.5) -
          this.ctx.mw.offset_y;
        x2 =
          Math.floor(t.x(x_right, y_high) * this.ctx.mw.zoom_factor_x + 0.5) -
          this.ctx.mw.offset_x;
        y2 =
          Math.floor(t.y(x_right, y_high) * this.ctx.mw.zoom_factor_y + 0.5) -
          this.ctx.mw.offset_y;
      }
      switch (this.level_direction) {
        case Glow.eDirection_Right:
          x_level = Math.floor(this.fill_level * (x2 - x1) + 0.5);
          clip_sts =
            this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 - 1, x1 + x_level, y2 +
              1);
          break;
        case Glow.eDirection_Left:
          x_level = Math.floor((1 - fill_level) * (x2 - x1) + 0.5);
          clip_sts =
            this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 - 1, x1 + x_level, y2 +
              1);
          if (this.level_color_tone !== Glow.eDrawTone_No) {
            old_color_tone = this.color_tone;
            this.color_tone = this.level_color_tone;
          } else if (this.level_fill_drawtype !== Glow.eDrawType_No) {
            old_fill_drawtype = this.fill_drawtype;
            this.fill_drawtype = this.level_fill_drawtype;
          }
          break;
        case Glow.eDirection_Up:
          y_level = Math.floor(this.fill_level * (y2 - y1) + 0.5);
          clip_sts =
            this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 - 1, x2 + 1, y1 +
              y_level);
          break;
        case Glow.eDirection_Down:
          y_level = Math.floor((1 - this.fill_level) * (y2 - y1) + 0.5);
          clip_sts =
            this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 - 1, x2 + 1, y1 +
              y_level);
          if (this.level_color_tone !== Glow.eDrawTone_No) {
            old_color_tone = this.color_tone;
            this.color_tone = this.level_color_tone;
          } else if (this.level_fill_drawtype !== Glow.eDrawType_No) {
            old_fill_drawtype = this.fill_drawtype;
            this.fill_drawtype = this.level_fill_drawtype;
          }
          break;
        default:
          break;
      }

      if (t === null) {
        this.nc.tdraw(this.trf, highlight, hot, node, node);
      } else {
        trf_tot = t.multiply(this.trf);

        // If this node has a trace pointer, use colors for this node
        this.nc.tdraw(trf_tot, highlight, hot, this, this);
      }
      if ((clip_sts & 1) !== 0) {
        this.ctx.gdraw.reset_clip_rectangle();
      }

      switch (this.level_direction) {
        case Glow.eDirection_Right:
          if (this.level_color_tone !== Glow.eDrawTone_No) {
            old_color_tone = this.color_tone;
            this.color_tone = this.level_color_tone;
          } else if (this.level_fill_drawtype !== Glow.eDrawType_No) {
            old_fill_drawtype = this.fill_drawtype;
            this.fill_drawtype = this.level_fill_drawtype;
          }
          clip_sts =
            this.ctx.gdraw.set_clip_rectangle(x1 + x_level, y1 - 1, x2 + 1, y2 +
              1);
          break;
        case Glow.eDirection_Left:
          if (this.level_color_tone !== Glow.eDrawTone_No) {
            this.color_tone = old_color_tone;
          } else if (this.level_fill_drawtype !== Glow.eDrawType_No) {
            this.fill_drawtype = old_fill_drawtype;
          }
          clip_sts =
            this.ctx.gdraw.set_clip_rectangle(x1 + x_level, y1 - 1, x2 + 1, y2 +
              1);
          break;
        case Glow.eDirection_Up:
          if (this.level_color_tone !== Glow.eDrawTone_No) {
            old_color_tone = this.color_tone;
            this.color_tone = this.level_color_tone;
          } else if (this.level_fill_drawtype !== Glow.eDrawType_No) {
            old_fill_drawtype = this.fill_drawtype;
            this.fill_drawtype = this.level_fill_drawtype;
          }
          clip_sts =
            this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 + y_level, x2 + 1, y2 +
              1);
          break;
        case Glow.eDirection_Down:
          if (this.level_color_tone !== Glow.eDrawTone_No) {
            this.color_tone = old_color_tone;
          } else if (this.level_fill_drawtype !== Glow.eDrawType_No) {
            this.fill_drawtype = old_fill_drawtype;
          }
          clip_sts =
            this.ctx.gdraw.set_clip_rectangle(x1 - 1, y1 + y_level, x2 + 1, y2 +
              1);
          break;
        default:
          break;
      }

      if (t === null) {
        this.nc.tdraw(this.trf, highlight, hot, node, node);
      } else {
        trf_tot = t.multiply(this.trf);

        // If this node has a trace pointer, use colors for this node
        this.nc.tdraw(trf_tot, highlight, hot, this, this);
      }
      if ((clip_sts & 1) !== 0) {
        this.ctx.gdraw.reset_clip_rectangle();
      }

      switch (this.level_direction) {
        case Glow.eDirection_Right:
          if (this.level_color_tone !== Glow.eDrawTone_No) {
            this.color_tone = old_color_tone;
          } else if (this.level_fill_drawtype !== Glow.eDrawType_No) {
            this.fill_drawtype = old_fill_drawtype;
          }
          break;
        case Glow.eDirection_Left:
          break;
        case Glow.eDirection_Up:
          if (this.level_color_tone !== Glow.eDrawTone_No) {
            this.color_tone = old_color_tone;
          } else if (this.level_fill_drawtype !== Glow.eDrawType_No) {
            this.fill_drawtype = old_fill_drawtype;
          }
          break;
        case Glow.eDirection_Down:
          break;
        default:
          break;
      }
    }

    if (this.annot_scrollingtext !== -1) {
      this.ctx.gdraw.reset_clip_rectangle();
    }
  };

  this.set_position = function (x, y) {
    var old_x_left, old_x_right, old_y_low, old_y_high;

    if (this.trf.a13 === this.trf.s_a13 + x &&
      this.trf.a23 === this.trf.s_a23 + y) {
      return;
    }
    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;
    this.trf.move_from_stored(x, y);
    this.get_node_borders();
    this.ctx.draw(old_x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, old_y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, old_x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, old_y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
    this.ctx.draw(this.x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, this.y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, this.x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, this.y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
  };

  this.move = function (x, y) {
    var old_x_left = this.x_left;
    var old_x_right = this.x_right;
    var old_y_low = this.y_low;
    var old_y_high = this.y_high;

    this.trf.move(x, y);
    this.get_node_borders();

    //draw();
    this.ctx.draw(old_x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, old_y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, old_x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, old_y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
    this.ctx.draw(this.x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, this.y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, this.x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, this.y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
  };

  this.move_to = function (x, y) {
    // this.ctx.set_defered_redraw();
    this.ctx.draw(this.x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, this.y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, this.x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, this.y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
    //erase( this.ctx.mw);
    //erase( this.ctx.navw);

    this.trf.move(x - this.x_left, y - this.y_low);
    this.get_node_borders();

    this.ctx.draw(this.x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, this.y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, this.x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, this.y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
    // this.ctx.redraw_defered();
  };

  this.set_scale_pos = function (x, y, scale_x, scale_y, x0, y0, type) {
    var old_x_left, old_x_right, old_y_low, old_y_high;

    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;

    this.ctx.setNodraw();
    this.set_scale(scale_x, scale_y, x0, y0, type);
    this.ctx.resetNodraw();
    this.trf.move(x - this.x_left, y - this.y_low);
    this.get_node_borders();
  };

  this.set_scale = function (scale_x, scale_y, x0, y0, type) {
    var old_x_left, old_x_right, old_y_low, old_y_high;

    if (!((scale_x === -1 && scale_y === 1) || (scale_x === 1 && scale_y === -1))) {
      if (scale_x < 0) {
        scale_x = 0;
      }
      if (scale_y < 0) {
        scale_y = 0;
      }
    }

    if (this.trf.s_a11 !== 0 && this.trf.s_a22 !== 0 &&
      Math.abs(scale_x - this.trf.a11 / this.trf.s_a11) < Number.MIN_VALUE &&
      Math.abs(scale_y - this.trf.a22 / this.trf.s_a22) < Number.MIN_VALUE) {
      return;
    }

    switch (type) {
      case Glow.eScaleType_LowerLeft:
        x0 = this.x_left;
        y0 = this.y_low;
        break;
      case Glow.eScaleType_LowerRight:
        x0 = this.x_right;
        y0 = this.y_low;
        break;
      case Glow.eScaleType_UpperRight:
        x0 = this.x_right;
        y0 = this.y_high;
        break;
      case Glow.eScaleType_UpperLeft:
        x0 = this.x_left;
        y0 = this.y_high;
        break;
      case Glow.eScaleType_FixPoint:
        break;
      case Glow.eScaleType_Center:
        x0 = (this.x_left + this.x_right) / 2;
        y0 = (this.y_low + this.y_high) / 2;
        break;
      default:
    }

    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;
    this.trf.scale_from_stored(scale_x, scale_y, x0, y0);
    this.get_node_borders();

    this.ctx.rdraw(old_x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, old_y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, old_x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, old_y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
    this.ctx.rdraw(this.x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, this.y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, this.x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, this.y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
  };

  this.setRotation = function (angel, x0, y0, type) {
    var old_x_left, old_x_right, old_y_low, old_y_high;
    var t = new GlowTransform();

    if (Math.abs(angel - this.trf.rotation + this.trf.s_rotation) <
      Number.MIN_VALUE) {
      return;
    }

    switch (type) {
      case Glow.eRotationPoint_LowerLeft:
        x0 = this.x_left;
        y0 = this.y_low;
        break;
      case Glow.eRotationPoint_LowerRight:
        x0 = this.x_right;
        y0 = this.y_low;
        break;
      case Glow.eRotationPoint_UpperRight:
        x0 = this.x_right;
        y0 = this.y_high;
        break;
      case Glow.eRotationPoint_UpperLeft:
        x0 = this.x_left;
        y0 = this.y_high;
        break;
      case Glow.eRotationPoint_Center:
        x0 = (this.x_left + this.x_right) / 2;
        y0 = (this.y_high + this.y_low) / 2;
        break;
      case Glow.eRotationPoint_Zero:
        x0 = this.trf.s_a13;
        y0 = this.trf.s_a23;
        break;
      default:
        break;
    }

    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;

    t.rotate(angel, x0, y0);
    this.trf.set_from_stored(t);
    this.get_node_borders();
  };

  this.storeTransform = function () {
    this.trf.store();
  };
  this.transformIsStored = function () {
    return this.trf.is_stored();
  };

  this.set_next_nodeclass = function () {
    if (this.nc.next_nc !== null) {
      this.set_nodeclass(this.nc.next_nc);
      return 1;
    }
    return 0;
  };

  this.set_first_nodeclass = function () {
    this.set_root_nodeclass();
  };

  this.set_last_nodeclass = function () {
    var next;
    for (next = this.nc_root; next.next_nc !== null; next = next.next_nc) {
    }
    if (next !== this.nc) {
      this.set_nodeclass(next);
    }
  };

  this.set_nodeclass_by_index = function (idx) {
    var next;
    var i;

    next = this.nc_root;
    for (i = 0; i < idx; i++) {
      if (next.next_nc === null) {
        return 0;
      }
      next = next.next_nc;
    }
    if (next !== this.nc) {
      this.set_nodeclass(next);
    }
    return 1;
  };

  this.set_previous_nodeclass = function () {
    if (this.nc.prev_nc !== null) {
      this.set_nodeclass(this.nc.prev_nc);
      return 1;
    }
    return 0;
  };

  this.set_root_nodeclass = function () {
    if (this.nc_root !== this.nc) {
      this.set_nodeclass(this.nc_root);
    }
  };

  this.get_animation_count = function () {
    return this.nc.animation_count;
  };

  this.get_nodeclass_dyn_attr1 = function () {
    return this.nc.dyn_attr[0];
  };

  this.set_nodeclass = function (new_nc) {
    var old_x_left, old_x_right, old_y_low, old_y_high;

    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;

    this.nc = new_nc;

    this.get_node_borders();
    old_x_left = Math.min(old_x_left, this.x_left);
    old_x_right = Math.max(old_x_right, this.x_right);
    old_y_low = Math.min(old_y_low, this.y_low);
    old_y_high = Math.max(old_y_high, this.y_high);
    this.ctx.draw(old_x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, old_y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, old_x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, old_y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
  };

  this.measure = function () {
    var geom = new GlowGeometry();
    geom.ll_x = this.x_left;
    geom.ll_y = this.y_low;
    geom.ur_x = this.x_right;
    geom.ur_y = this.y_high;

    return geom;
  };

  this.getLimits = function () {
    var limits = new GlowFillLevelLimits();

    if (this.nc.y0 === 0 && this.nc.y1 === 0) {
      limits.status = 0;
      return limits;
    }
    if (!(this.nc.x0 === 0 && this.nc.x1 === 0)) {
      limits.status = 0;
      return limits;
    }

    var x1, x2, y1, y2;
    var rotation;

    // Calculate max and min coordinates

    x1 = this.trf.x(0, this.nc.y0);
    y1 = this.trf.y(0, this.nc.y0);
    x2 = this.trf.x(0, this.nc.y1);
    y2 = this.trf.y(0, this.nc.y1);

    rotation = (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) * 360;

    if (45 >= rotation || rotation > 315) {
      limits.direction = Glow.eDirection_Down;
      limits.min = y1;
      limits.max = y2;
    } else if (45 < rotation && rotation <= 135) {
      limits.direction = Glow.eDirection_Right;
      limits.min = x2;
      limits.max = x1;
    } else if (135 < rotation && rotation <= 225) {
      limits.direction = Glow.eDirection_Up;
      limits.min = y2;
      limits.max = y1;
    } else if (225 < rotation && rotation <= 315) {
      limits.direction = Glow.eDirection_Left;
      limits.min = x1;
      limits.max = x2;
    }
    limits.status = 1;
    return limits;
  };

  this.get_node_borders = function () {
    var g = new GlowGeometry();

    g.ur_x = 10e-37;
    g.ll_x = 10e37;
    g.ur_y = 10e-37;
    g.ll_y = 10e37;

    this.nc.get_borders(this.trf, g);
    this.x_left = g.ll_x;
    this.x_right = g.ur_x;
    this.y_low = g.ll_y;
    this.y_high = g.ur_y;
  };

  this.get_borders = function (t, g) {
    if (t === null) {
      this.nc.get_borders(this.trf, g);
    } else {
      var t2 = t.multiply(this.trf);
      this.nc.get_borders(t2, g);
    }
  };

  this.get_background_object_limits = function (t, type, x, y, bo) {
    var dyn_type;
    var b = bo;
    var base_nc = this.nc.get_base_nc();
    dyn_type = base_nc.dyn_type1;

    if ((type & dyn_type) === 0) {
      return 0;
    }
    if (this.nc.y0 === 0 && this.nc.y1 === 0) {
      return 0;
    }

    var x1, x2, y1, y2;
    var rotation;
    var g = new GlowGeometry();

    g.ur_x = 10e-37;
    g.ll_x = 10e37;
    g.ur_y = 10e-37;
    g.ll_y = 10e37;

    this.get_borders(t, g);
    var x1_right = g.ur_x;
    var x1_left = g.ll_x;
    var y1_high = g.ur_y;
    var y1_low = g.ll_y;

    if (x <= x1_right && x >= x1_left && y <= y1_high && y >= y1_low) {
      // Hit, calculate max and min koordinates

      if (t == null) {
        x1 = this.trf.x(0, this.nc.y0);
        y1 = this.trf.y(0, this.nc.y0);
        x2 = this.trf.x(0, this.nc.y1);
        y2 = this.trf.y(0, this.nc.y1);
      } else {
        x1 = this.trf.x(t, 0, this.nc.y0);
        y1 = this.trf.y(t, 0, this.nc.y0);
        x2 = this.trf.x(t, 0, this.nc.y1);
        y2 = this.trf.y(t, 0, this.nc.y1);
      }

      if (t == null) {
        rotation =
          (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) * 360;
      } else {
        rotation =
          (this.trf.rot(t) / 360 - Math.floor(this.trf.rot(t) / 360)) * 360;
      }

      if (45 >= rotation || rotation > 315) {
        b.direction = Glow.eDirection_Down;
        b.min = y1;
        b.max = y2;
      } else if (45 < rotation && rotation <= 135) {
        b.direction = Glow.eDirection_Left;
        b.min = x2;
        b.max = x1;
      } else if (135 < rotation && rotation <= 225) {
        b.direction = Glow.eDirection_Up;
        b.min = y2;
        b.max = y1;
      } else if (225 < rotation && rotation <= 315) {
        b.direction = Glow.eDirection_Right;
        b.min = x1;
        b.max = x2;
      }
      b.background = this;

      return 1;
    }
    return 0;
  };

  this.getColorInverse = function () {
    return this.color_inverse;
  };
  this.getColorLightness = function () {
    return this.color_lightness;
  };
  this.getColorIntensity = function () {
    return this.color_intensity;
  };
  this.getColorShift = function () {
    return this.color_shift;
  };
  this.getColorTone = function () {
    return this.color_tone;
  };
  this.getFillDrawtype = function () {
    return this.fill_drawtype;
  };
  this.getTextDrawtype = function () {
    return this.text_drawtype;
  };
  this.getBackgroundDrawtype = function () {
    return this.background_drawtype;
  };
  this.getDrawtype = function () {
    return this.draw_type;
  };
  this.getDimmed = function () {
    return this.dimmed;
  };
  this.getClassTraceColor1 = function () {
    return this.nc.dyn_color[0];
  };
  this.getClassTraceColor2 = function () {
    return this.nc.dyn_color[1];
  };
  this.getClassDynType1 = function () {
    return this.nc.dyn_type1;
  };
  this.getClassDynType2 = function () {
    return this.nc.dyn_type2;
  };
  this.getClassActionType1 = function () {
    return this.nc.action_type1;
  };
  this.getClassActionType2 = function () {
    return this.nc.action_type2;
  };
  this.getClassCycle = function () {
    return this.nc.cycle;
  };
  this.getClassUserData = function () {
    return this.nc.userdata;
  };
  this.getUserData = function () {
    return this.userdata;
  };
  this.setColorTone = function (color_tone) {
    this.color_tone = color_tone;
  };
  this.resetColorTone = function () {
    this.color_tone = this.original_color_tone;
  };
  this.setFillColor = function (fill_drawtype) {

    this.fill_drawtype = fill_drawtype;
    this.color_tone = Glow.eDrawTone_No;
    this.color_shift = 0;
    this.color_lightness = 0;
    this.color_intensity = 0;
    this.draw();
  };
  this.resetFillColor = function () {
    this.fill_drawtype = this.original_fill_drawtype;
    this.color_tone = this.original_color_tone;
    this.color_shift = this.original_color_shift;
    this.color_lightness = this.original_color_lightness;
    this.color_intensity = this.original_color_intensity;
  };
  this.setBorderColor = function (draw_type) {
    this.draw_type = draw_type;
    this.color_tone = Glow.eDrawTone_No;
    this.color_shift = 0;
    this.color_lightness = 0;
    this.color_intensity = 0;
  };
  this.resetBorderColor = function () {
    this.draw_type = this.original_border_drawtype;
  };
  this.setTextColor = function (draw_type) {
    this.text_drawtype = draw_type;
    this.color_tone = Glow.eDrawTone_No;
    this.color_shift = 0;
    this.color_lightness = 0;
    this.color_intensity = 0;
  };
  this.resetTextColor = function () {
    this.text_drawtype = this.original_text_drawtype;
  };
  this.setBackgroundColor = function (draw_type) {
    this.background_drawtype = draw_type;
  };
  this.resetBackgroundColor = function () {
    this.background_drawtype = this.original_background_drawtype;
  };
  this.setColorInverse = function (color_inverse) {
    this.color_inverse = color_inverse;
    this.draw();  // TODO...
  };
  this.setLevelFillColor = function (color) {
    this.level_fill_drawtype = color;
  };
  this.setLevelColorTone = function (tone) {
    this.level_color_tone = tone;
  };
  this.setLevelDirection = function (level_direction) {
    this.level_direction = level_direction;
  };
  this.setFillLevel = function (fill_level) {
    this.fill_level = fill_level;
  };
  this.setVisibility = function (visibility) {
    switch (visibility) {
      case Glow.eVis_Visible:
        this.visible = 1;
        this.dimmed = 0;
        break;
      case Glow.eVis_Invisible:
        this.visible = 0;
        this.dimmed = 0;
        this.draw();
        break;
      case Glow.eVis_Dimmed:
        this.visible = 1;
        this.dimmed = 1;
        this.draw();
        break;
      default:
        break;
    }
  };
  this.getAnnotation = function (number) {
    return this.annotv[number];
  };
  this.setAnnotation = function (number, annot) {
    this.annotv[number] = annot;
  };
  this.setAnnotationTextOffset = function (num, x, y) {
    if (this.annot_scrollingtext === -1) {
      this.annot_scrollingtext = num;
    }
    if (num === this.annot_scrollingtext) {
      this.annot_offset_x = x;
      this.annot_offset_y = y;
    }
  };

  this.getAnnotationTextExtent = function (num) {
    return this.nc.getAnnotationTextExtent(this.trf, this, num);
  };

  this.setColorThemeLightness = function () {
    if (this.ctx.customcolors === null) {
      return;
    }

    if (this.ctx.customcolors.colortheme_lightness !== this.color_lightness) {
      this.color_lightness = this.ctx.customcolors.colortheme_lightness;
    }
  };
}

function GrowGroup(ctx) {
  GrowNode.call(this, ctx);
  this.grownode_open = this.open;

  this.type = function () {
    return Glow.eObjectType_GrowGroup;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowGroup : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowGroup:
          break;
        case Glow.eSave_GrowGroup_grownode_part:
          i = this.grownode_open(lines, i + 1);
          break;
        case Glow.eSave_GrowGroup_nc:
          this.nc = new GlowNodeGroup(this.ctx);
          i = this.nc.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowGroup");
          break;
      }
      if (end) {
        break;
      }
    }

    return i;
  };

  this.get_object_group = function (object) {
    var sts;
    var group;

    for (var i = 0; i < this.nc.a.size(); i++) {
      if (this.nc.a.get(i).type() === Glow.eObjectType_GrowGroup) {
        group = nc.a.get(i).get_object_group(object);
        if (group != null) {
          return group;
        }
      } else if (this.nc.a.get(i) === object) {
        return this;
      }
    }
    return null;
  };

  this.get_object_list = function () {
    return this.nc.a;
  };
}

GrowGroup.prototype = Object.create(GrowNode.prototype);

function GrowSlider(ctx) {
  GrowNode.call(this, ctx);
  this.grownode_open = this.open;

  this.direction;
  this.max_value;
  this.min_value;
  this.max_pos;
  this.min_pos;

  this.type = function () {
    return Glow.eObjectType_GrowSlider;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowGroup : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowSlider:
          break;
        case Glow.eSave_GrowSlider_direction:
          this.direction = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowSlider_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowSlider_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowSlider_max_pos:
          this.max_pos = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowSlider_min_pos:
          this.min_pos = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowSlider_grownode_part:
          i = this.grownode_open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowSlider");
          break;
      }
      if (end) {
        break;
      }
    }

    return i;
  };

  this.get_origo = function (direction) {
    var t = new GlowTransform();
    var g = new GlowGeometry();

    g.ur_x = 10e-37;
    g.ll_x = 10e37;
    g.ur_y = 10e-37;
    g.ll_y = 10e37;
    var pos;

    // Get borders in the nodeclass coordinates
    this.nc.get_borders(t, g);
    switch (this.direction) {
      case Glow.eDirection_Down:
      case Glow.eDirection_Up:
        if (Math.abs(g.ur_y - g.ll_y) < Number.MIN_VALUE) {
          pos = this.y_low;
        } else {
          pos = (this.nc.y0 - g.ll_y) * (this.y_high - this.y_low) /
            (g.ur_y - g.ll_y);
        }
        break;
      case Glow.eDirection_Left:
      case Glow.eDirection_Right:
        if (Math.abs(g.ur_y - g.ll_y) < Number.MIN_VALUE) {
          pos = this.y_low;
        } else {
          pos = (this.nc.y0 - g.ll_y) * (this.x_right - this.x_left) /
            (g.ur_y - g.ll_y);
        }
        break;
      default:
        pos = 0;
    }
    return pos;
  };

  this.get_info = function () {
    var info = new GlowSliderInfo();

    info.direction = this.direction;
    info.max_value = this.max_value;
    info.min_value = this.min_value;
    info.max_position = this.max_pos;
    info.min_position = this.min_pos;

    return info;
  };

  this.set_info = function (info) {
    this.direction = info.direction;
    this.max_value = info.max_value;
    this.min_value = info.min_value;
    this.max_pos = info.max_position;
    this.min_pos = info.min_position;
  };
}

GrowSlider.prototype = Object.create(GrowNode.prototype);


function GrowToolbar(ctx) {
  GrowNode.call(this, ctx);
  this.grownode_open = this.open;

  this.TOOLBAR_SPACING = 0.2;

  this.tools1_mask;
  this.tools2_mask;


  this.type = function () {
    return Glow.eObjectType_GrowToolbar;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowToolbar : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowToolbar:
          break;
        case Glow.eSave_GrowToolbar_grownode_part:
          i = this.grownode_open(lines, i + 1);
          break;
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowToolbar");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.event_handler = function (event, fx, fy) {
    var rp;
    var sts;
    var idx;

    if (this.visible === 0 || this.dimmed !== 0) {
      return 0;
    }

    switch (event.event) {
      case Glow.eEvent_CursorMotion:
        return 0;
      default:
    }

    rp = this.trf.reverse(fx, fy);

    switch (event.event) {
      case Glow.eEvent_MB1Down:
        sts = this.nc_event_handler(event, rp.x, rp.y);
        if (sts !== 0) {
          idx = sts - 1;
          this.nc.a.get(idx).setColorInverse(1);
        }
        break;
      case Glow.eEvent_MB1Up:
        sts = this.nc_event_handler(event, rp.x, rp.y);
        if (sts !== 0) {
          idx = sts - 1;
          this.nc.a.get(idx).setColorInverse(0);
        }
        break;
      case Glow.eEvent_MB1Click:
        sts = this.nc_event_handler(event, rp.x, rp.y);
        console.log("MB1 Click toolbar", sts);
        if (sts !== 0) {
          idx = sts - 1;
          var lsts = this.get_mask_index(idx);
          if (lsts == null) {
            break;
          }

          var category = lsts[0];
          var mask_idx = lsts[1];

          console.log("MB1 Click category: " + category + " idx " + mask_idx +
            " sts" + idx);

          var csts = this.ctx.send_toolbar_callback(this, category, mask_idx,
            Glow.eEvent_MB1Click, fx, fy);
        }
        break;
      default:
        sts = this.nc.event_handler(event, rp.x, rp.y);
        if (sts !== 0) {
          return sts;
        }
    }
    return 0;
  };

  this.nc_event_handler = function (event, x, y) {
    var i;
    var sts;

    for (i = 0; i < this.nc.a.size(); i++) {
      sts = this.nc.a.get(i).event_handler(event, x, y);
      if (sts !== 0) {
        return i + 1;
      }
    }
    return 0;
  };

  this.configure =
    function (tools1, tools2, tools1_cnt, tools2_cnt, show_mask1, show_mask2,
              insensitive_mask1, insensitive_mask2) {
      var x1, y1;
      var ll_x, ll_y, ur_x, ur_y;

      var nc1;
      var subg_name;
      var mask;
      var sts;

      console.log("Configure show_mask " + show_mask1 + " " + show_mask2);
      x1 = 0;
      y1 = 0;

      // Clear nc
      this.nc.a.clear();

      this.tools1_mask = 0;
      mask = 1;
      for (var i = 0; i < tools1_cnt; i++) {
        if ((mask & show_mask1) !== 0) {
          if (tools1[i] === "") {
            mask = mask << 1;
            continue;
          }

          subg_name = tools1[i];

          nc1 = this.ctx.get_nodeclass_from_name(subg_name);
          if (nc1 === null) {
            var fname = subg_name + ".pwsg";
            sts = this.ctx.loadSubgraph(fname);
            if ((sts & 1) !== 0) {
              nc1 = this.ctx.get_nodeclass_from_name(subg_name);
            }
          }
          if (nc1 !== null) {
            var n1;
            n1 = new GrowNode(ctx);
            n1.n_name = subg_name;
            console.log("Button " + subg_name + " " + x1);
            n1.nc = nc1;
            n1.move_to(x1, y1);
            // n1.disable_callback();
            this.nc.a.add(n1);

            if ((mask & insensitive_mask1) !== 0) {
              n1.setVisibility(Glow.eVis_Dimmed);
            }

            var g = n1.measure();
            x1 += g.ur_x - g.ll_x + this.TOOLBAR_SPACING;
            this.tools1_mask |= mask;
          }
        }
        mask = mask << 1;
      }
      if (x1 !== 0) {
        x1 += this.TOOLBAR_SPACING * 2;
      }

      this.tools2_mask = 0;
      mask = 1;
      for (var i = 0; i < tools2_cnt; i++) {
        if ((mask & show_mask2) !== 0) {
          if (tools2[i] === "") {
            mask = mask << 1;
            continue;
          }

          subg_name = tools2[i];

          nc1 = this.ctx.get_nodeclass_from_name(subg_name);
          if (nc1 === null) {
            var fname = subg_name + ".pwsg";
            sts = this.ctx.loadSubgraph(fname);
            if ((sts & 1) !== 0) {
              nc1 = this.ctx.get_nodeclass_from_name(subg_name);
            }
          }
          if (nc1 !== null) {
            var n1;
            n1 = new GrowNode(ctx);
            n1.n_name = subg_name;
            n1.nc = nc1;
            console.log("Button " + subg_name + " " + x1);
            n1.move_to(x1, y1);
            // n1.disable_callback();
            this.nc.a.add(n1);

            if ((mask & insensitive_mask2) !== 0) {
              n1.setVisibility(Glow.eVis_Dimmed);
            }

            var g = n1.measure();
            x1 += g.ur_x - g.ll_x + this.TOOLBAR_SPACING;
            this.tools2_mask |= mask;
          }
        }
        mask = mask << 1;
      }

      this.get_node_borders();
      this.scale();

      console.log("Toolbar configure tools_mask " + this.tools1_mask + " " +
        this.tools2_mask);
    };

  this.get_mask_index = function (idx) {
    // Calculate category and index
    var mask;
    var category;
    var mask_idx;
    var cnt = 0;
    var i;
    for (i = 0; i < 64; i++) {
      if (i < 32) {
        mask = 1 << i;
      } else {
        mask = 1 << (i - 32);
      }
      if ((i < 32 && (this.tools1_mask & mask) !== 0) ||
        (i >= 32 && (this.tools2_mask & mask) !== 0)) {
        cnt++;
      }
      if (cnt === idx + 1) {
        mask_idx = i % 32;
        category = i / 32 + 1;
        var ret = new Array(2);
        ret[0] = category;
        ret[1] = mask_idx;
        return ret;
      }
    }
    return null;
  };

  this.scale = function () {
    var scale;

    if (this.ctx.mw.window_width === 0) {
      return;
    }

    console.log("Toolbar.scale offset_x " + this.ctx.mw.offset_x + " wwidth " +
      this.ctx.mw.window_width);
    if (this.x_right * this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x >
      this.ctx.mw.window_width) {
      scale = (this.ctx.mw.window_width -
        (this.x_left * this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x) - 10) /
        ((this.x_right - this.x_left) * this.ctx.mw.zoom_factor_x);

      thix.trf.scale(scale, 1, thix.x_left, thix.y_low);
      thix.get_node_borders();
    }
  };

}

GrowToolbar.prototype = Object.create(GrowNode.prototype);

function GrowRect(ctx) {
  this.ctx = ctx;
  this.n_name = null;
  this.x_right = 0;
  this.x_left = 0;
  this.y_high = 0;
  this.y_low = 0;
  this.original_border_drawtype = 0;
  this.original_fill_drawtype = 0;
  this.fill_drawtype = 0;
  this.background_drawtype = Glow.eDrawType_No;
  this.border = 0;
  this.shadow_width = 5;
  this.shadow_contrast = 2;
  this.shadow = 0;
  this.relief = Glow.eRelief_Up;
  this.disable_shadow = 0;
  this.invisible = 0;
  this.fixcolor = 0;
  this.fixposition = 0;
  this.gradient = Glow.eGradient_No;
  this.gradient_contrast = 4;
  this.disable_gradient = 0;
  this.bgcolor_gradient = 0;
  this.fill_eq_background = 0;
  this.dynamicsize = 0;
  this.trf = new GlowTransform();

  this.ll = new GlowPoint(this.ctx);
  this.ur = new GlowPoint(this.ctx);
  this.draw_type = 0;
  this.line_width = 0;
  this.display_level = 0;
  this.fill = 0;

  this.type = function () {
    return Glow.eObjectType_GrowRect;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowRect : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowRect:
          break;
        case Glow.eSave_GrowRect_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case Glow.eSave_GrowRect_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowRect_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowRect_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowRect_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowRect_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_original_fill_drawtype:
          this.original_fill_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_fill_drawtype:
          this.fill_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_background_drawtype:
          this.background_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_shadow_width:
          this.shadow_width = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowRect_shadow_contrast:
          this.shadow_contrast = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_relief:
          this.relief = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_disable_shadow:
          this.disable_shadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_invisible:
          this.invisible = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_fixcolor:
          this.fixcolor = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_fixposition:
          this.fixposition = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_gradient:
          this.gradient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_gradient_contrast:
          this.gradient_contrast = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_disable_gradient:
          this.disable_gradient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_bgcolor_gradient:
          this.bgcolor_gradient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_fill_eq_background:
          this.fill_eq_background = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRect_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case Glow.eSave_GrowRect_rect_part:
          i = this.glowrect_open(lines, i + 1);
          break;
        case Glow.eSave_GrowRect_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowRect");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.glowrect_open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowRect : " + lines[i]);
      }

      switch (key) {

        case Glow.eSave_Rect:
          break;
        case Glow.eSave_Rect_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_display_level:
          this.display_level = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case Glow.eSave_Rect_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowRect");
          break;
      }
      if (end) {
        break;
      }

    }
    return i;
  };

  this.event_handler = function (event, fx, fy) {

    switch (event.event) {
      case Glow.eEvent_CursorMotion:
        return 0;
      default:
        break;
    }

    var rp = this.trf.reverse(fx, fy);

    if (this.ll.x <= rp.x && rp.x <= this.ur.x && this.ll.y <= rp.y &&
      rp.y <= this.ur.y) {
      return 1;
    } else {
      return 0;
    }
  };

  this.draw = function () {
    if (this.visible !== 0) {
      this.tdraw(null, 0, 0, null, null);
    }
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    if (this.invisible !== 0 && !(highlight !== 0 && node === null)) {
      return;
    }
    var chot = 0;
    var idx;
    var drawtype;
    if (this.fixcolor !== 0) {
      colornode = null;
    }

    if (node !== null && node.line_width !== 0) {
      idx = this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        node.line_width - 1;
    } else {
      idx = this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        this.line_width - 1;
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

    if (t === null) {
      x1 = this.trf.x(this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_x + 0.5 -
        this.ctx.mw.offset_x;
      y1 = this.trf.y(this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_y + 0.5 -
        this.ctx.mw.offset_y;
      x2 = this.trf.x(this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_x + 0.5 -
        this.ctx.mw.offset_x;
      y2 = this.trf.y(this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_y + 0.5 -
        this.ctx.mw.offset_y;
    } else {
      x1 =
        this.trf.x(t, this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_x + 0.5 -
        this.ctx.mw.offset_x;
      y1 =
        this.trf.y(t, this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_y + 0.5 -
        this.ctx.mw.offset_y;
      x2 =
        this.trf.x(t, this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_x + 0.5 -
        this.ctx.mw.offset_x;
      y2 =
        this.trf.y(t, this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_y + 0.5 -
        this.ctx.mw.offset_y;
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    var ish = this.shadow_width / 100 * Math.min(ur_x - ll_x, ur_y - ll_y) +
      0.5;
    var display_shadow = ((node !== null && node.shadow !== 0) ||
      this.shadow !== 0) && this.disable_shadow === 0;
    var fillcolor;
    if (this.fill_eq_background === 0) {
      fillcolor =
        GlowColor.get_drawtype(this.fill_drawtype, Glow.eDrawType_FillHighlight,
          highlight, colornode, 1, 0);
    } else {
      fillcolor = GlowColor.get_drawtype(this.background_drawtype,
        Glow.eDrawType_FillHighlight, highlight, colornode, 3, 0);
    }

    var grad = this.gradient;
    if (this.gradient === Glow.eGradient_No &&
      (node !== null && node.gradient !== Glow.eGradient_No) &&
      this.disable_gradient === 0) {
      grad = node.gradient;
    }

    if (display_shadow && ish !== 0) {
      var points = new Array(7);
      for (var i = 0; i < 7; i++) {
        points[i] = new GlowPoint();
      }

      // Draw light shadow
      var drawtype_incr = this.shadow_contrast;
      if (this.relief === Glow.eRelief_Down) {
        drawtype_incr = -this.shadow_contrast;
      }

      drawtype =
        GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot, colornode);

      points[0].x = ll_x;
      points[0].y = ll_y;
      points[1].x = ur_x;
      points[1].y = ll_y;
      points[2].x = ur_x - ish;
      points[2].y = ll_y + ish;
      points[3].x = ll_x + ish;
      points[3].y = ll_y + ish;
      points[4].x = ll_x + ish;
      points[4].y = ur_y - ish;
      points[5].x = ll_x;
      points[5].y = ur_y;
      points[6].x = ll_x;
      points[6].y = ll_y;
      this.ctx.gdraw.fill_polyline(points, 7, drawtype, 0);

      // Draw dark shadow
      drawtype =
        GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot, colornode);

      points[0].x = ur_x;
      points[0].y = ur_y;
      points[1].x = ll_x;
      points[1].y = ur_y;
      points[2].x = ll_x + ish;
      points[2].y = ur_y - ish;
      points[3].x = ur_x - ish;
      points[3].y = ur_y - ish;
      points[4].x = ur_x - ish;
      points[4].y = ll_y + ish;
      points[5].x = ur_x;
      points[5].y = ll_y;
      points[6].x = ur_x;
      points[6].y = ur_y;
      this.ctx.gdraw.fill_polyline(points, 7, drawtype, 0);
    }
    if (this.fill !== 0) {
      if (display_shadow && ish !== 0) {
        if (grad === Glow.eGradient_No || fillcolor === Glow.eDrawType_ColorRed) {
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }
          this.ctx.gdraw.fill_rect(ll_x + ish, ll_y + ish, ur_x - ll_x - 2 *
            ish, ur_y - ll_y - 2 * ish, drawtype);
        } else {
          var fa0, fa1, fa2;
          var rotationa;
          if (t === null) {
            rotationa = this.trf.rot();
          } else {
            rotationa = this.trf.rot(t);
          }

          if (this.bgcolor_gradient !== 0 &&
            this.background_drawtype !== Glow.eDrawType_No) {
            fa2 = fillcolor;
            fa1 = GlowColor.get_drawtype(this.background_drawtype,
              Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);
            fa0 = fa1;
          } else if (this.gradient_contrast >= 0) {
            fa2 =
              GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                0.5 + chot, null);
            fa1 =
              GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                0.6 + chot, null);
            fa0 = fillcolor;
          } else {
            fa2 =
              GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 -
                0.6 + chot, null);
            fa1 =
              GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                chot, null);
            fa0 = fillcolor;
          }
          this.ctx.gdraw.gradient_fill_rect(ll_x + ish, ll_y + ish, ur_x -
            ll_x - 2 * ish, ur_y - ll_y - 2 * ish, fa0, fa1, fa2,
            this.ctx.gdraw.gradient_rotate(rotationa, grad));
        }
      } else {
        if (grad === Glow.eGradient_No || fillcolor === Glow.eDrawType_ColorRed) {
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }
          this.ctx.gdraw.fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
            drawtype);
        } else {
          var fb0, fb1, fb2;
          var rotationb;
          if (t === null) {
            rotationb = this.trf.rot();
          } else {
            rotationb = this.trf.rot(t);
          }
          if (this.bgcolor_gradient !== 0 &&
            this.background_drawtype !== Glow.eDrawType_No) {
            fb2 = fillcolor;
            fb1 = GlowColor.get_drawtype(this.background_drawtype,
              Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);
            fb0 = fb1;
          } else if (this.gradient_contrast >= 0) {
            fb2 =
              GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                0.5 + chot, null);
            fb1 =
              GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                0.6 + chot, null);
            fb0 = fillcolor;
          } else {
            fb2 =
              GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 -
                0.6 + chot, null);
            fb1 =
              GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2,
                null);
            fb0 = fillcolor;
          }
          this.ctx.gdraw.gradient_fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y -
            ll_y, fb0, fb1, fb2,
            this.ctx.gdraw.gradient_rotate(rotationb, grad));
        }
      }
    }
    if (this.border !== 0 ||
      !(this.fill !== 0 || (display_shadow && this.shadow_width !== 0))) {
      drawtype =
        GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
          highlight, colornode, 0, 0);
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, idx,
        0);
    }
  };

  this.get_borders = function (t, g) {
    var ll_x, ur_x, ll_y, ur_y, x1, x2, y1, y2;

    if (t === null) {
      x1 = this.trf.x(this.ll.x, this.ll.y);
      x2 = this.trf.x(this.ur.x, this.ur.y);
      y1 = this.trf.y(this.ll.x, this.ll.y);
      y2 = this.trf.y(this.ur.x, this.ur.y);
    } else {
      x1 = this.trf.x(t, this.ll.x, this.ll.y);
      x2 = this.trf.x(t, this.ur.x, this.ur.y);
      y1 = this.trf.y(t, this.ll.x, this.ll.y);
      y2 = this.trf.y(t, this.ur.x, this.ur.y);
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    if (ll_x < g.ll_x) {
      g.ll_x = ll_x;
    }
    if (ur_x > g.ur_x) {
      g.ur_x = ur_x;
    }
    if (ll_y < g.ll_y) {
      g.ll_y = ll_y;
    }
    if (ur_y > g.ur_y) {
      g.ur_y = ur_y;
    }
    return g;
  };

  this.get_node_borders = function () {
    var g = new GlowGeometry();
    g.ll_x = g.ll_y = 1e37;
    g.ur_x = g.ur_y = -1e37;
    this.get_borders(null, g);
    this.x_left = g.ll_x;
    this.x_right = g.ur_x;
    this.y_low = g.ll_y;
    this.y_high = g.ur_y;
  };

  this.set_scale = function (scale_x, scale_y, x0, y0, type) {
    var old_x_left, old_x_right, old_y_low, old_y_high;

    if (this.trf.s_a11 !== 0 && this.trf.s_a22 !== 0 &&
      Math.abs(scale_x - this.trf.a11 / this.trf.s_a11) < Number.MIN_VALUE &&
      Math.abs(scale_y - this.trf.a22 / this.trf.s_a22) < Number.MIN_VALUE) {
      return;
    }

    switch (type) {
      case Glow.eScaleType_LowerLeft:
        x0 = this.x_left;
        y0 = this.y_low;
        break;
      case Glow.eScaleType_LowerRight:
        x0 = this.x_right;
        y0 = this.y_low;
        break;
      case Glow.eScaleType_UpperRight:
        x0 = this.x_right;
        y0 = this.y_high;
        break;
      case Glow.eScaleType_UpperLeft:
        x0 = this.x_left;
        y0 = this.y_high;
        break;
      case Glow.eScaleType_FixPoint:
        break;
      case Glow.eScaleType_Center:
        x0 = (this.x_left + this.x_right) / 2;
        y0 = (this.y_low + this.y_high) / 2;
        break;
      default:
    }

    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;
    this.trf.scale_from_stored(scale_x, scale_y, x0, y0);
    this.get_node_borders();

    switch (type) {
      case Glow.eScaleType_LowerLeft:
        this.x_left = old_x_left;
        this.y_low = old_y_low;
        break;
      case Glow.eScaleType_LowerRight:
        this.x_right = old_x_right;
        this.y_low = old_y_low;
        break;
      case Glow.eScaleType_UpperRight:
        this.x_right = old_x_right;
        this.y_high = old_y_high;
        break;
      case Glow.eScaleType_UpperLeft:
        this.x_left = old_x_left;
        this.y_high = old_y_high;
        break;
      case Glow.eScaleType_FixPoint:
        break;
      case Glow.eScaleType_Center:
        x0 = (this.x_left + this.x_right) / 2;
        y0 = (this.y_low + this.y_high) / 2;
        break;
      default:
    }
    this.ctx.draw(old_x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, old_y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, old_x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, old_y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
    this.ctx.draw(this.x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, this.y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, this.x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, this.y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
  };

  this.getUserData = function () {
    return null;
  };
  this.getClassDynType1 = function () {
    return 0;
  };
  this.getClassDynType2 = function () {
    return 0;
  };
  this.getClassActionType1 = function () {
    return 0;
  };
  this.getClassActionType2 = function () {
    return 0;
  };
  this.getClassCycle = function () {
    return Glow.eCycle_Slow;
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };

}

function GrowRectRounded(ctx) {
  this.ctx = ctx;
  this.n_name;
  this.x_right;
  this.x_left;
  this.y_high;
  this.y_low;
  this.original_border_drawtype;
  this.original_fill_drawtype;
  this.fill_drawtype;
  this.border;
  this.shadow_width;
  this.shadow_contrast;
  this.shadow;
  this.relief;
  this.disable_shadow;
  this.fixposition;
  this.gradient;
  this.gradient_contrast;
  this.disable_gradient;
  this.dynamicsize;
  this.trf = new GlowTransform();
  this.round_amount;

  this.ll = new GlowPoint(ctx);
  this.ur = new GlowPoint(ctx);
  this.draw_type;
  this.line_width;
  this.display_level;
  this.fill;

  this.type = function () {
    return Glow.eObjectType_GrowRectRounded;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowRectRounded : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowRectRounded:
          break;
        case Glow.eSave_GrowRectRounded_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case Glow.eSave_GrowRectRounded_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowRectRounded_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowRectRounded_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowRectRounded_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowRectRounded_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_original_fill_drawtype:
          this.original_fill_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_fill_drawtype:
          this.fill_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_round_amount:
          this.round_amount = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowRectRounded_shadow_width:
          this.shadow_width = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowRectRounded_shadow_contrast:
          this.shadow_contrast = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_relief:
          this.relief = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_disable_shadow:
          this.disable_shadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_fixposition:
          this.fixposition = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_gradient:
          this.gradient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_gradient_contrast:
          this.gradient_contrast = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_disable_gradient:
          this.disable_gradient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowRectRounded_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case Glow.eSave_GrowRectRounded_rect_part:
          i = this.glowrect_open(lines, i + 1);
          break;
        case Glow.eSave_GrowRectRounded_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowRectRounded");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.glowrect_open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowRectRounded : " + lines[i]);
      }

      switch (key) {

        case Glow.eSave_Rect:
          break;
        case Glow.eSave_Rect_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_display_level:
          this.display_level = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Rect_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case Glow.eSave_Rect_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowRect");
          break;
      }
      if (end) {
        break;
      }

    }
    return i;
  };

  this.event_handler = function (event, fx, fy) {
    switch (event.event) {
      case Glow.eEvent_CursorMotion:
        return 0;
      default:
        break;
    }

    var rp = this.trf.reverse(fx, fy);

    if (this.ll.x <= rp.x && rp.x <= this.ur.x && this.ll.y <= rp.y &&
      rp.y <= this.ur.y) {
      console.log("Event handler: Hit in rect");
      return 1;
    } else {
      return 0;
    }
  };

  this.draw = function () {
    this.tdraw(null, 0, 0, null, null);
  };


  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var chot = 0;
    var idx;
    var drawtype;

    if (node !== null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1);
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);
    var amount = Math.floor(this.round_amount / 100 *
      Math.min(ur_x - ll_x, ur_y - ll_y) + 0.5);
    if (this.fill !== 0) {

      var ish = Math.floor(this.shadow_width / 100 *
        Math.min(ur_x - ll_x, ur_y - ll_y) + 0.5);
      var display_shadow = ((node !== null && node.shadow !== 0) ||
        this.shadow !== 0) && this.disable_shadow === 0;
      var fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
        Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);

      var grad = this.gradient;
      if (this.gradient === Glow.eGradient_No &&
        (node !== null && node.gradient !== Glow.eGradient_No) &&
        this.disable_gradient === 0) {
        grad = node.gradient;
      }

      if (!display_shadow || ish === 0) {
        if (grad === Glow.eGradient_No) {
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }

          this.ctx.gdraw.fill_rect(ll_x, ll_y + amount, ur_x - ll_x, ur_y -
            ll_y - 2 * amount, drawtype);
          this.ctx.gdraw.fill_rect(ll_x + amount, ll_y, ur_x - ll_x - 2 *
            amount, amount, drawtype);
          this.ctx.gdraw.fill_rect(ll_x + amount, ur_y - amount, ur_x - ll_x -
            2 * amount, amount, drawtype);
          this.ctx.gdraw.fill_arc(ll_x, ll_y, 2 * amount, 2 * amount, 90, 90,
            drawtype);
          this.ctx.gdraw.fill_arc(ll_x, ur_y - 2 * amount, 2 * amount, 2 *
            amount, 180, 90, drawtype);
          this.ctx.gdraw.fill_arc(ur_x - 2 * amount, ur_y - 2 * amount, 2 *
            amount, 2 * amount, 270, 90, drawtype);
          this.ctx.gdraw.fill_arc(ur_x - 2 * amount, ll_y, 2 * amount, 2 *
            amount, 0, 90, drawtype);
        } else {
          var fa1, fa2;
          var rotationa;
          if (t === null) {
            rotationa = this.trf.rot();
          } else {
            rotationa = this.trf.rot(t);
          }

          if (this.gradient_contrast >= 0) {
            fa2 =
              GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                chot, null);
            fa1 = GlowColor.shift_drawtype(
              fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
              null);
          } else {
            fa2 = GlowColor.shift_drawtype(
              fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
              null);
            fa1 =
              GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                chot, null);
          }
          this.ctx.gdraw.gradient_fill_rectrounded(ll_x, ll_y, ur_x -
            ll_x, ur_y - ll_y, amount, fillcolor, fa1, fa2,
            this.ctx.gdraw.gradient_rotate(rotationa, grad));
        }
      } else {

        var drawtype_incr = this.shadow_contrast;
        if (this.relief === Glow.eRelief_Down) {
          drawtype_incr = -this.shadow_contrast;
        }

        // Draw light shadow
        drawtype =
          GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot, colornode);
        this.ctx.gdraw.fill_rect(ll_x + amount, ll_y, ur_x - ll_x - 2 * amount,
          ish, drawtype);
        this.ctx.gdraw.fill_rect(ll_x, ll_y + amount, ish, ur_y - ll_y - 2 *
          amount, drawtype);
        this.ctx.gdraw.fill_arc(ll_x, ll_y, 2 * amount, 2 * amount, 90, 90,
          drawtype);
        this.ctx.gdraw.fill_arc(ll_x, ur_y - 2 * amount, 2 * amount, 2 * amount,
          180, 45, drawtype);
        this.ctx.gdraw.fill_arc(ur_x - 2 * amount, ll_y, 2 * amount, 2 * amount,
          45, 45, drawtype);
        // Draw dark shadow
        drawtype =
          GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot, colornode);
        this.ctx.gdraw.fill_rect(ll_x + amount, ur_y - ish, ur_x - ll_x - 2 *
          amount, ish, drawtype);
        this.ctx.gdraw.fill_rect(ur_x - ish, ll_y + amount, ish, ur_y - ll_y -
          2 * amount, drawtype);
        this.ctx.gdraw.fill_arc(ll_x, ur_y - 2 * amount, 2 * amount, 2 * amount,
          225, 45, drawtype);
        this.ctx.gdraw.fill_arc(ur_x - 2 * amount, ll_y, 2 * amount, 2 * amount,
          0, 45, drawtype);
        this.ctx.gdraw.fill_arc(ur_x - 2 * amount, ur_y - 2 * amount, 2 *
          amount, 2 * amount, 270, 90, drawtype);

        if (grad === Glow.eGradient_No) {
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }

          if (amount > ish) {

            this.ctx.gdraw.fill_rect(ll_x + ish, ll_y + amount, ur_x - ll_x -
              2 * ish, ur_y - ll_y - 2 * amount, drawtype);
            this.ctx.gdraw.fill_rect(ll_x + amount, ll_y + ish, ur_x - ll_x -
              2 * amount, amount - ish, drawtype);
            this.ctx.gdraw.fill_rect(ll_x + amount, ur_y - amount, ur_x - ll_x -
              2 * amount, amount - ish, drawtype);
            this.ctx.gdraw.fill_arc(ll_x + ish, ll_y + ish, 2 * amount - 2 *
              ish, 2 * amount - 2 * ish, 90, 90, drawtype);
            this.ctx.gdraw.fill_arc(ll_x + ish, ur_y - 2 * amount + ish, 2 *
              amount - 2 * ish, 2 * amount - 2 * ish, 180, 90, drawtype);
            this.ctx.gdraw.fill_arc(ur_x - 2 * amount + ish, ur_y - 2 * amount +
              ish, 2 * amount - 2 * ish, 2 * amount - 2 * ish, 270, 90,
              drawtype);
            this.ctx.gdraw.fill_arc(ur_x - 2 * amount + ish, ll_y + ish, 2 *
              amount - 2 * ish, 2 * amount - 2 * ish, 0, 90, drawtype);
          } else {
            this.ctx.gdraw.fill_rect(ll_x + amount, ll_y + amount, ur_x - ll_x -
              2 * amount, ur_y - ll_y - 2 * amount, drawtype);
          }
        } else {
          var fb1, fb2;
          var rotationb;
          if (t === null) {
            rotationb = this.trf.rot();
          } else {
            rotationb = this.trf.rot(t);
          }

          if (this.gradient_contrast >= 0) {
            fb2 =
              GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                chot, null);
            fb1 = GlowColor.shift_drawtype(
              fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
              null);
          } else {
            fb2 = GlowColor.shift_drawtype(
              fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
              null);
            fb1 =
              GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                chot, null);
          }
          this.ctx.gdraw.gradient_fill_rectrounded(ll_x + ish, ll_y +
            ish, ur_x - ll_x - 2 * ish, ur_y - ll_y - 2 * ish, amount - ish,
            fillcolor, fb1, fb2,
            this.ctx.gdraw.gradient_rotate(rotationb, grad));
        }
      }
    }
    if (this.border !== 0 || this.fill === 0) {
      drawtype =
        GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
          highlight, colornode, 0, 0);
      var c = 0;
      this.ctx.gdraw.line(ll_x + amount, ll_y, ur_x - amount, ll_y, drawtype,
        idx, 0);
      this.ctx.gdraw.line(ll_x, ll_y - c + amount, ll_x, ur_y - c - amount,
        drawtype, idx, 0);
      this.ctx.gdraw.line(ll_x + amount, ur_y, ur_x - amount, ur_y, drawtype,
        idx, 0);
      this.ctx.gdraw.line(ur_x - c, ll_y + amount, ur_x - c, ur_y - amount,
        drawtype, idx, 0);
      this.ctx.gdraw.arc(ll_x, ll_y, 2 * amount, 2 * amount, 90, 90, drawtype,
        idx, 0);
      this.ctx.gdraw.arc(ll_x, ur_y - 2 * amount, 2 * amount, 2 * amount, 180,
        90, drawtype, idx, 0);
      this.ctx.gdraw.arc(ur_x - 2 * amount, ur_y - 2 * amount, 2 * amount, 2 *
        amount, 270, 90, drawtype, idx, 0);
      this.ctx.gdraw.arc(ur_x - 2 * amount, ll_y, 2 * amount, 2 * amount, 0, 90,
        drawtype, idx, 0);
    }
  };
}

function GrowLine(ctx) {
  this.ctx = ctx;
  this.n_name = null;
  this.x_right = 0;
  this.x_left = 0;
  this.y_high = 0;
  this.y_low = 0;
  this.line_type = 0;
  this.original_border_drawtype;
  this.dynamicsize = 0;
  this.trf = new GlowTransform();

  this.p1 = new GlowPoint(this.ctx);
  this.p2 = new GlowPoint(this.ctx);
  this.draw_type = 0;
  this.line_width = 0;

  this.type = function () {
    return Glow.eObjectType_GrowLine;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowLine : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowLine:
          break;
        case Glow.eSave_GrowLine_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case Glow.eSave_GrowLine_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowLine_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowLine_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowLine_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowLine_line_type:
          this.line_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowLine_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowLine_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowLine_dynamic:
          i += this.dynamicsize;
          break;
        case Glow.eSave_GrowLine_line_part:
          i = this.glowline_open(lines, i + 1);
          break;
        case Glow.eSave_GrowLine_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowLine");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.glowline_open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowLine : " + lines[i]);
      }

      switch (key) {

        case Glow.eSave_Line:
          break;
        case Glow.eSave_Line_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Line_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Line_p1:
          i = this.p1.open(lines, i + 1);
          break;
        case Glow.eSave_Line_p2:
          i = this.p2.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowLine");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.event_handler = function (event, fx, fy) {
    return 0;
  };

  this.draw = function () {
    this.tdraw(null, 0, 0, null, null);
  };


  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    if (hot !== 9 && this.ctx.hot_indication !== Glow.eHotIndication_LineWidth) {
      hot = 0;
    }

    var drawtype;
    var idx;
    if (node !== null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1);
    }
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.p1.x, this.p1.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.p1.x, this.p1.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.p2.x, this.p2.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.p2.x, this.p2.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    } else {
      x1 = Math.floor(this.trf.x(t, this.p1.x, this.p1.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.p1.x, this.p1.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.p2.x, this.p2.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.p2.x, this.p2.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    }
    if (x1 === x2 && y1 === y2) {
      return;
    }
    drawtype =
      GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
        highlight, colornode, 0, 0);

    if (this.line_type === Glow.eLineType_Solid) {
      this.ctx.gdraw.line(x1, y1, x2, y2, drawtype, idx, 0);
    } else {
      this.ctx.gdraw.line_dashed(x1, y1, x2, y2, drawtype, idx, 0,
        this.line_type);
    }

  };

  this.get_borders = function (t, g) {
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

function GrowArc(ctx) {
  this.ctx = ctx;
  this.n_name = null;
  this.x_right;
  this.x_left;
  this.y_high;
  this.y_low;
  this.original_border_drawtype;
  this.original_fill_drawtype;
  this.fill_drawtype;
  this.background_drawtype = Glow.eDrawType_No;
  this.border;
  this.shadow_width;
  this.shadow_contrast;
  this.shadow;
  this.relief;
  this.disable_shadow;
  this.fixcolor;
  this.fixposition;
  this.gradient;
  this.gradient_contrast;
  this.disable_gradient;
  this.fill_eq_background;
  this.dynamicsize;
  this.trf = new GlowTransform();

  this.ll = new GlowPoint(ctx);
  this.ur = new GlowPoint(ctx);
  this.draw_type;
  this.angle1;
  this.angle2;
  this.line_width;
  this.display_level;
  this.fill;

  this.type = function () {
    return Glow.eObjectType_GrowArc;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowArc : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowArc:
          break;
        case Glow.eSave_GrowArc_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case Glow.eSave_GrowArc_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowArc_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowArc_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowArc_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowArc_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_original_fill_drawtype:
          this.original_fill_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_fill_drawtype:
          this.fill_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_background_drawtype:
          this.background_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_shadow_width:
          this.shadow_width = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowArc_shadow_contrast:
          this.shadow_contrast = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_relief:
          this.relief = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_disable_shadow:
          this.disable_shadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_fixcolor:
          this.fixcolor = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_fixposition:
          this.fixposition = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_gradient:
          this.gradient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_gradient_contrast:
          this.gradient_contrast = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_disable_gradient:
          this.disable_gradient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_fill_eq_background:
          this.fill_eq_background = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowArc_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case Glow.eSave_GrowArc_arc_part:
          i = this.glowarc_open(lines, i + 1);
          break;
        case Glow.eSave_GrowArc_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowArc");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.glowarc_open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowArc : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Arc:
          break;
        case Glow.eSave_Arc_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_angle1:
          this.angle1 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_angle2:
          this.angle2 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case Glow.eSave_Arc_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowArc");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.event_handler = function (event, fx, fy) {

    switch (event.event) {
      case Glow.eEvent_CursorMotion:
        return 0;
      default:
        break;
    }

    var rp = this.trf.reverse(fx, fy);
    if (this.ll.x <= rp.x && rp.x <= this.ur.x && this.ll.y <= rp.y &&
      rp.y <= this.ur.y) {
      return 1;
    } else {
      return 0;
    }
  };

  this.draw = function () {
    this.tdraw(null, 0, 0, null, null);
  };


  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var drawtype;
    var idx;
    var chot = 0;

    if (this.ctx.hot_indication === Glow.eHotIndication_No) {
      hot = 0;
    } else if (this.ctx.hot_indication === Glow.eHotIndication_DarkColor) {
      chot = hot;
      hot = 0;
    } else if (this.ctx.hot_indication === Glow.eHotIndication_LightColor) {
      chot = -hot;
      hot = 0;
    }

    if (this.fixcolor !== 0) {
      colornode = null;
    }

    if (node !== null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1);
    }
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y, rot;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot());
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot(t));
    }

    if (rot % 90 !== 0 &&
      Math.abs((this.ur.x - this.ll.x) - (this.ur.y - this.ll.y)) <
      Float.MIN_VALUE) {
      var scale;
      var x_c;
      var y_c;
      if (t === null) {
        scale = this.trf.vertical_scale(this.trf);
        x_c = ((this.trf.x(this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_x -
          this.ctx.mw.offset_x) +
          (this.trf.x(this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_x -
            this.ctx.mw.offset_x)) / 2;
        y_c = ((this.trf.y(this.ll.x, this.ll.y) * this.ctx.mw.zoom_factor_y -
          this.ctx.mw.offset_y) +
          (this.trf.y(this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_y -
            this.ctx.mw.offset_y)) / 2;
      } else {
        var tmp = t.multiply(this.trf);
        scale = trf.vertical_scale(tmp);
        x_c = ((trf.x(t, ll.x, ll.y) * this.ctx.mw.zoom_factor_x -
          this.ctx.mw.offset_x) +
          (trf.x(t, ur.x, ur.y) * this.ctx.mw.zoom_factor_x -
            this.ctx.mw.offset_x)) / 2;
        y_c = ((trf.y(t, ll.x, ll.y) * this.ctx.mw.zoom_factor_y -
          this.ctx.mw.offset_y) +
          (trf.y(t, ur.x, ur.y) * this.ctx.mw.zoom_factor_y -
            this.ctx.mw.offset_y)) / 2;
      }

      x1 = Math.floor(-scale *
        ((this.ur.x - this.ll.x) / 2 * this.ctx.mw.zoom_factor_x) + x_c + 0.5);
      y1 = Math.floor(-scale *
        ((this.ur.y - this.ll.y) / 2 * this.ctx.mw.zoom_factor_y) + y_c + 0.5);
      x2 = Math.floor(scale *
        ((this.ur.x - this.ll.x) / 2 * this.ctx.mw.zoom_factor_x) + x_c + 0.5);
      y2 = Math.floor(scale *
        ((this.ur.y - this.ll.y) / 2 * this.ctx.mw.zoom_factor_y) + y_c + 0.5);
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    if (this.fill !== 0) {
      var display_shadow = ((node !== null && node.shadow !== 0) ||
        this.shadow !== 0) && this.disable_shadow === 0;
      var fillcolor;
      if (this.fill_eq_background === 0) {
        fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
          Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);
      } else {
        fillcolor = GlowColor.get_drawtype(this.background_drawtype,
          Glow.eDrawType_FillHighlight, highlight, colornode, 3, 0);
      }

      var grad = this.gradient;
      if (this.gradient === Glow.eGradient_No &&
        (node !== null && node.gradient !== Glow.eGradient_No) &&
        this.disable_gradient === 0) {
        grad = node.gradient;
      }

      if (!display_shadow || this.shadow_width === 0 || this.angle2 !== 360) {
        if (grad === Glow.eGradient_No || fillcolor === Glow.eDrawType_ColorRed) {
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }
          this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y -
            ll_y, this.angle1 - rot, this.angle2, drawtype);
        } else {
          var fa1, fa2;
          if (this.gradient_contrast >= 0) {
            fa2 =
              GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                chot, null);
            fa1 = GlowColor.shift_drawtype(
              fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
              null);
          } else {
            fa2 = GlowColor.shift_drawtype(
              fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
              null);
            fa1 =
              GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                chot, null);
          }
          this.ctx.gdraw.gradient_fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y -
            ll_y, this.angle1 - rot, this.angle2, fillcolor, fa1, fa2, grad);
        }
      } else {
        var ish = Math.floor(this.shadow_width / 100 *
          Math.min(ur_x - ll_x, ur_y - ll_y) + 0.5);
        var drawtype_incr = this.shadow_contrast;
        if (this.relief === Glow.eRelief_Down) {
          drawtype_incr = -this.shadow_contrast;
        }

        if (grad === Glow.eGradient_No || fillcolor === Glow.eDrawType_ColorRed) {

          // Draw light shadow
          drawtype = GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot,
            colornode);

          this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, 35, 140,
            drawtype);

          // Draw dark shadow
          drawtype = GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot,
            colornode);

          this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, 215,
            140, drawtype);

          // Draw medium shadow and body
          if (chot === 0) {
            drawtype = fillcolor;
          } else {
            drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
          }

          this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, -5, 40,
            drawtype);
          this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, 175, 40,
            drawtype);

          this.ctx.gdraw.fill_arc(ll_x + ish, ll_y + ish, ur_x - ll_x - 2 *
            ish, ur_y - ll_y - 2 * ish, this.angle1 - rot, this.angle2,
            drawtype);
        } else {
          var fb1, fb2;

          // Draw shadow
          fb1 = GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot,
            colornode);
          fb2 = GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot,
            colornode);

          this.ctx.gdraw.gradient_fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y -
            ll_y, this.angle1 - rot, this.angle2, fillcolor, fb2, fb1,
            Glow.eGradient_DiagonalUpperLeft);

          // Draw circle
          if (this.gradient_contrast >= 0) {
            fb2 =
              GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
                chot, null);
            fb1 = GlowColor.shift_drawtype(
              fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
              null);
          } else {
            fb2 = GlowColor.shift_drawtype(
              fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
              null);
            fb1 =
              GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
                chot, null);
          }
          this.ctx.gdraw.gradient_fill_arc(ll_x + ish, ll_y + ish, ur_x - ll_x -
            2 * ish, ur_y - ll_y - 2 * ish, this.angle1 - rot, this.angle2,
            fillcolor, fb1, fb2, grad);
        }
      }
    }
    if (this.border !== 0 || this.fill === 0) {
      drawtype =
        GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
          highlight, colornode, 0, 0);
      this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1 -
        rot, this.angle2, drawtype, idx, 0);
    }
  };

  this.get_borders = function (t, g) {
    var ll_x, ur_x, ll_y, ur_y, x1, x2, y1, y2;

    if (t === null) {
      x1 = this.trf.x(this.ll.x, this.ll.y);
      x2 = this.trf.x(this.ur.x, this.ur.y);
      y1 = this.trf.y(this.ll.x, this.ll.y);
      y2 = this.trf.y(this.ur.x, this.ur.y);
    } else {
      x1 = this.trf.x(t, this.ll.x, this.ll.y);
      x2 = this.trf.x(t, this.ur.x, this.ur.y);
      y1 = this.trf.y(t, this.ll.x, this.ll.y);
      y2 = this.trf.y(t, this.ur.x, this.ur.y);
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    if (ll_x < g.ll_x) {
      g.ll_x = ll_x;
    }
    if (ur_x > g.ur_x) {
      g.ur_x = ur_x;
    }
    if (ll_y < g.ll_y) {
      g.ll_y = ll_y;
    }
    if (ur_y > g.ur_y) {
      g.ur_y = ur_y;
    }
    return g;
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };

}

function GrowText(ctx) {
  this.ctx = ctx;
  this.n_name;
  this.x_right;
  this.x_left;
  this.y_high;
  this.y_low;
  this.original_color_drawtype;
  this.font;
  this.adjustment = Glow.eAdjustment_Left;
  this.dynamicsize;
  this.trf = new GlowTransform();

  this.p = new GlowPoint(ctx);
  this.text;
  this.draw_type;
  this.text_size;
  this.display_level;
  this.color_drawtype;

  this.type = function () {
    return Glow.eObjectType_GrowText;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowArc : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowText:
          break;
        case Glow.eSave_GrowText_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case Glow.eSave_GrowText_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowText_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowText_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowText_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowText_original_color_drawtype:
          this.original_color_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowText_font:
          this.font = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowText_adjustment:
          this.adjustment = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowText_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowText_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case Glow.eSave_GrowText_text_part:
          i = this.glowtext_open(lines, i + 1);
          break;
        case Glow.eSave_GrowText_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowText");
          break;
      }
      if (end) {
        break;
      }
    }

    return i;
  };

  this.glowtext_open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowArc : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Text:
          break;
        case Glow.eSave_Text_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Text_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Text_color_drawtype:
          this.color_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Text_text:
          this.text = lines[i].substring(4);
          break;
        case Glow.eSave_Text_p:
          i = this.p.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowText");
          break;
      }
      if (end) {
        break;
      }

    }

    return i;
  };

  this.event_handler = function (event, fx, fy) {
    return 0;
  };

  this.draw = function () {
    this.tdraw(null, 0, 0, null, null);
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0 || this.text === null) {
      return;
    }

    var x1, y1, rx1, ry1;
    var z_width, z_height, z_descent;
    var rot;
    var trf_scale = this.trf.vertical_scale(t);
    var idx = Math.floor(trf_scale * this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 3);
    var tsize = trf_scale * this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (8 + 2 * this.text_size);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var highl = highlight;
    if (node !== null) {
      highl = node.highlight;
    }

    var lfont;
    var ldraw_type;

    if (node !== null && node.text_font !== Glow.eFont_No) {
      lfont = node.text_font;
      ldraw_type = node.text_type;
    } else {
      lfont = this.font;
      ldraw_type = this.draw_type;
    }

    rx1 = ry1 = z_width = z_height = 0;
    if (t === null) {
      x1 =
        Math.floor(this.trf.x(this.p.x, this.p.y) * this.ctx.mw.zoom_factor_x +
          0.5) - this.ctx.mw.offset_x;
      y1 =
        Math.floor(this.trf.y(this.p.x, this.p.y) * this.ctx.mw.zoom_factor_y +
          0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot());
    } else {
      x1 = Math.floor(this.trf.x(t, this.p.x, this.p.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.p.x, this.p.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot(t));
    }
    if (this.adjustment === Glow.eAdjustment_Center) {
      rot = rot < 0 ? rot % 360 + 360 : rot % 360;
    } else {
      rot = 0;
    }

    if (this.text !== "") {
      if (highl !== 0 || (hot !== 0 && node === null) ||
        this.adjustment !== Glow.eAdjustment_Left) {
        var dim = this.ctx.gdraw.getTextExtent(this.text, Math.max(0, idx),
          lfont, ldraw_type);
        z_width = dim.width;
        z_height = dim.height;
        z_descent = z_height / 4;
        switch (this.adjustment) {
          case Glow.eAdjustment_Left:
            rx1 = x1;
            ry1 = y1 - (z_height - z_descent);
            break;
          case Glow.eAdjustment_Right:
            x1 -= z_width;
            rx1 = x1;
            ry1 = y1 - (z_height - z_descent);
            break;
          case Glow.eAdjustment_Center:
            switch (rot) {
              case 90:
                rx1 = x1 - z_width / 2;
                ry1 = y1 - z_height / 2;
                x1 -= z_width / 2;
                y1 += z_height / 2;
                break;
              case 270:
                rx1 = x1 - z_width / 2;
                ry1 = y1 - z_height / 2;
                x1 -= z_width / 2;
                y1 -= z_height / 2 - z_width / 2;
                break;
              default:
                rx1 = x1 - z_width / 2;
                ry1 = y1 - z_height / 2;
                x1 -= z_width / 2;
                y1 += z_height / 2 - z_descent;
                break;
            }
            break;
          default:
            break;
        }
      }

      if (highl !== 0) {
        this.ctx.gdraw.rect(rx1, ry1, z_width, z_height,
          Glow.eDrawType_FillHighlight, Math.max(1, Math.min(idx + hot, 2)), 0);
      } else if (hot !== 0 && node === null) {
        this.ctx.gdraw.rect(rx1, ry1, z_width, z_height,
          Glow.eDrawType_LineGray, Math.max(Math.min(idx, 2), 1), 0);
      }
      if (idx >= 0) {
        var color = GlowColor.get_drawtype(this.color_drawtype,
          Glow.eDrawType_LineHighlight, highlight, colornode, 2, 0);
        this.ctx.gdraw.text(x1, y1, this.text, ldraw_type, color, idx,
          highlight, 0, lfont, tsize, rot);
      }
    } else if (idx >= 0) {
      var dim2 = this.ctx.gdraw.getTextExtent("A", Math.max(0, idx), lfont,
        ldraw_type);
      z_width = dim2.width;
      z_height = dim2.height;
      z_descent = z_height / 4;
      this.ctx.gdraw.rect(x1, y1 - (z_height - z_descent), z_width, z_height,
        Glow.eDrawType_LineGray, idx, 0);
    }

  };

  this.get_borders = function (t, g) {
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

function GrowAnnot(ctx) {
  this.ctx = ctx;
  this.p = new GlowPoint(ctx);
  this.adjustment;
  this.trf = new GlowTransform();

  this.number;
  this.draw_type;
  this.color_drawtype;
  this.text_size;
  this.display_level;
  this.annot_type;
  this.font;
  this.protect;

  this.type = function () {
    return Glow.eObjectType_GrowAnnot;
  };

  this.getNumber = function () {
    return this.number;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowArc : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowAnnot:
          break;
        case Glow.eSave_GrowAnnot_adjustment:
          this.adjustment = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAnnot_annot_part:
          i = this.glowannot_open(lines, i + 1);
          break;
        case Glow.eSave_GrowAnnot_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowAnnot");
          break;
      }
      if (end) {
        break;
      }
    }

    return i;
  };

  this.glowannot_open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowArc : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Annot:
          break;
        case Glow.eSave_Annot_number:
          this.number = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Annot_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Annot_color_drawtype:
          this.color_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Annot_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Annot_display_level:
          this.display_level = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Annot_p:
          i = this.p.open(lines, i + 1);
          break;
        case Glow.eSave_Annot_annot_type:
          this.annot_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Annot_font:
          this.font = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Annot_protect:
          this.protect = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowAnnot");
          break;
      }
      if (end) {
        break;
      }
    }

    return i;
  };

  this.draw = function () {
    this.tdraw(null, 0, 0, null, null);
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    var x1, y1;
    if (this.ctx.nodraw !== 0) {
      return;
    }

    if (node.annotv[this.number] === null) {
      return;
    }

    var color;
    var rot;
    var offset_x = 0;
    var offset_y = 0;
    var trf_scale = this.trf.vertical_scale(t);
    var idx = Math.floor(trf_scale * this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 3);
    var tsize = trf_scale * this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (8 + 2 * this.text_size);
    if (idx < 0) {
      return;
    }
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);

    var lfont;
    var ldraw_type;

    if (node !== null && node.annot_scrollingtext === this.number) {
      offset_x = node.annot_offset_x;
      offset_y = node.annot_offset_y;
    }

    if (node !== null && node.text_font !== Glow.eFont_No) {
      lfont = node.text_font;
      ldraw_type = node.text_type;
    } else {
      lfont = this.font;
      ldraw_type = this.draw_type;
    }

    if (t === null) {
      x1 = Math.floor((this.trf.x(this.p.x, this.p.y) + offset_x) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor((this.trf.y(this.p.x, this.p.y) + offset_y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot());
    } else {
      x1 = Math.floor((this.trf.x(t, this.p.x, this.p.y) + offset_x) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor((this.trf.y(t, this.p.x, this.p.y) + offset_y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot(t));
    }
    rot = rot < 0 ? rot % 360 + 360 : rot % 360;

    switch (this.annot_type) {
      case Glow.eAnnotType_OneLine: {
        var width = 0;
        var height = 0;
        var descent = 0;

        color = GlowColor.get_drawtype(this.color_drawtype,
          Glow.eDrawType_LineHighlight, highlight, colornode, 2, 0);


        if (((rot < 45 || rot >= 315) &&
            (node.annotv_inputmode[this.number] !== 0 && node.input_selected !==
              0)) || (!(rot < 45 || rot >= 315)) ||
          this.adjustment === Glow.eAdjustment_Right ||
          this.adjustment === Glow.eAdjustment_Center) {
          var d = this.getTextExtent(t, node);
          width = d.width;
          height = d.height;
          descent = height / 4;
        }

        switch (this.adjustment) {
          case Glow.eAdjustment_Left:
            break;
          case Glow.eAdjustment_Right:
            x1 -= width;
            break;
          case Glow.eAdjustment_Center:
            x1 -= width / 2;
            break;
          default:
            break;
        }


        if (rot < 45 || rot >= 315) {
          if (node.annotv_inputmode[this.number] !== 0 &&
            node.input_selected !== 0) {
            this.ctx.gdraw.fill_rect(x1, y1 - height + descent, width, height,
              Glow.eDrawType_MediumGray);
          }
        } else {
          // Text is rotated, adjust the coordinates
          if (this.adjustment === Glow.eAdjustment_Center) {
            // Only center adjustment supports text rotation
            if (45 <= rot && rot < 135) {
              x1 += width / 2;
              y1 += width / 2;
            } else if (135 <= rot && rot < 225) {
              y1 += height - descent;
            } else if (225 <= rot && rot < 315) {
              x1 += width / 2 - height + descent;
              y1 -= width / 2 - height + descent;
            } else {
              x1 -= width;
            }
          } else {
            if (45 <= rot && rot < 135) {
              y1 += height - descent;
            } else if (135 <= rot && rot < 225) {
              x1 -= width;
              y1 += height - descent;
            } else {
              x1 -= width;
            }
            rot = 0;
          }
        }

        this.ctx.gdraw.text(x1, y1, node.annotv[this.number], ldraw_type, color,
          idx, highlight, 0, lfont, tsize, rot);
        break;
      }
      case Glow.eAnnotType_MultiLine: {
        break;
      }
      default:
        break;
    }
  };

  this.getTextExtent = function (t, node) {
    var dim = new GlowDimension();

    if (node.annotv[this.number] === null || node.annotv[this.number] === "") {
      dim.width = 0;
      dim.height = 0;
      return dim;
    }

    var z_width, z_height, descent;
    var trf_scale = this.trf.vertical_scale(t);
    var idx = Math.floor(trf_scale * this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 4);
    var tsize = trf_scale * this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (8 + 2 * this.text_size);
    if (idx < 0) {
      dim.width = 0;
      dim.height = 0;
      return dim;
    }
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);

    var d = this.ctx.gdraw.getTextExtent(node.annotv[this.number], idx,
      this.font, this.draw_type);

    dim.width = d.width;
    dim.height = d.height;
    return dim;
  };

  this.event_handler = function (event, fx, fy) {
    return 0;
  };
  this.get_borders = function (t, g) {
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

function GlowShadowInfo() {
  this.drawtype;
  this.x;
  this.y;
}

function GrowPolyline(ctx) {
  this.ctx = ctx;
  this.n_name = null;
  this.x_right;
  this.x_left;
  this.y_high;
  this.y_low;
  this.original_border_drawtype = 0;
  this.original_fill_drawtype = 0;
  this.fill_drawtype = 0;
  this.background_drawtype = 0;
  this.border = 1;
  this.fill_eq_border = 0;
  this.shadow_width = 0;
  this.shadow_contrast = 0;
  this.shadow = 0;
  this.relief = 0;
  this.disable_shadow = 0;
  this.fixcolor = 0;
  this.fixposition = 0;
  this.gradient = 0;
  this.gradient_contrast = 0;
  this.disable_gradient = 0;
  this.fill_eq_light = 0;
  this.fill_eq_shadow = 0;
  this.fill_eq_bglight = 0;
  this.fill_eq_bgshadow = 0;
  this.fill_eq_background = 0;
  this.dynamicsize = 0;
  this.trf = new GlowTransform();

  this.a_points = new GlowArray(ctx);
  this.points = [];
  this.draw_type = 0;
  this.line_width = 0;
  this.fill = 0;
  this.closed_line = 0;

  this.init =
    function (name, pointarray, point_cnt, border_d_type, line_w, fix_line_w,
              fill, display_border, display_shadow, fill_d_type) {
      this.original_border_drawtype = border_d_type;
      this.fill_drawtype = fill_d_type;
      this.border = display_border;
      this.shadow = display_shadow;
      this.shadow_width = 5;
      this.relief = Glow.eRelief_Up;
      this.shadow_contrast = 2;
      this.gradient = Glow.eGradient_No;
      this.gradient_contrast = 4;
      this.n_name = name;
      this.draw_type = border_d_type;
      this.line_width = line_w;
      this.fill = fill;
      this.background_drawtype = Glow.eDrawType_No;

      for (var i = 0; i < point_cnt; i++) {
        var p = new GlowPoint(this.ctx);
        p.x = pointarray[i].x;
        p.y = pointarray[i].y;
        this.a_points.add(p);
      }
      this.points = new Array(this.a_points.size());
      for (var i = 0; i < this.a_points.size(); i++) {
        this.points[i] = new GlowPointX();
      }
    };

  this.type = function () {
    return Glow.eObjectType_GrowPolyLine;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowPolyline : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowPolyLine:
          break;
        case Glow.eSave_GrowPolyLine_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case Glow.eSave_GrowPolyLine_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPolyLine_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPolyLine_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPolyLine_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPolyLine_original_border_drawtype:
          this.original_border_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_original_fill_drawtype:
          this.original_fill_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_fill_drawtype:
          this.fill_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_background_drawtype:
          this.background_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_shadow_width:
          this.shadow_width = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPolyLine_shadow_contrast:
          this.shadow_contrast = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_shadow:
          this.shadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_relief:
          this.relief = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_disable_shadow:
          this.disable_shadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_fixcolor:
          this.fixcolor = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_fixposition:
          this.fixposition = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_gradient:
          this.gradient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_gradient_contrast:
          this.gradient_contrast = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_disable_gradient:
          this.disable_gradient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_fill_eq_border:
          this.fill_eq_border = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_fill_eq_light:
          this.fill_eq_light = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_fill_eq_shadow:
          this.fill_eq_shadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_fill_eq_bglight:
          this.fill_eq_bglight = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_fill_eq_bgshadow:
          this.fill_eq_bgshadow = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_fill_eq_background:
          this.fill_eq_background = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPolyLine_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case Glow.eSave_GrowPolyLine_polyline_part:
          i = this.glowpolyline_open(lines, i + 1);
          break;
        case Glow.eSave_GrowPolyLine_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowPolyline");
          break;
      }
      if (end) {
        break;
      }
    }

    for (var j = 0; j < this.a_points.size(); j++) {
      this.points[j] = new GlowPointX();
    }

    return i;
  };

  this.glowpolyline_open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowPolyline : " + lines[i]);
      }

      switch (key) {

        case Glow.eSave_PolyLine:
          break;
        case Glow.eSave_PolyLine_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_PolyLine_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_PolyLine_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_PolyLine_closed_line:
          this.closed_line = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_PolyLine_a_points:
          i = this.a_points.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowPolyline");
          break;
      }
      if (end) {
        break;
      }

    }
    return i;
  };

  this.event_handler = function (event, fx, fy) {
    return 0;
  };

  this.draw = function () {
    this.tdraw(null, 0, 0, null, null);
  };


  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var chot = 0;
    if (hot !== 0) {
      if (this.ctx.hot_indication === Glow.eHotIndication_No) {
        hot = 0;
      } else if (this.ctx.hot_indication === Glow.eHotIndication_DarkColor) {
        chot = hot;
        hot = 0;
      } else if (this.ctx.hot_indication === Glow.eHotIndication_LightColor) {
        chot = -hot;
        hot = 0;
      }
    }
    var i;
    var drawtype;
    var idx;

    if (this.fixcolor !== 0) {
      colornode = null;
    }

    if (node !== null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1);
    }
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1;

    for (i = 0; i < this.a_points.size(); i++) {
      if (t === null) {
        x1 = this.trf.x(this.a_points.get(i).x, this.a_points.get(i).y);
        y1 = this.trf.y(this.a_points.get(i).x, this.a_points.get(i).y);
      } else {
        x1 = this.trf.x(t, this.a_points.get(i).x, this.a_points.get(i).y);
        y1 = this.trf.y(t, this.a_points.get(i).x, this.a_points.get(i).y);
      }
      this.points[i].x =
        Math.floor(x1 * this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      this.points[i].y =
        Math.floor(y1 * this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    }
    if (this.fill !== 0) {
      var grad = this.gradient;
      if (this.gradient === Glow.eGradient_No &&
        (node !== null && node.gradient !== Glow.eGradient_No) &&
        this.disable_gradient === 0) {
        grad = node.gradient;
      }

      if (this.fill_eq_border !== 0) {
        drawtype =
          GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
            highlight, colornode, 0, 0);
      } else if (this.fill_eq_background !== 0) {
        drawtype = GlowColor.get_drawtype(this.background_drawtype,
          Glow.eDrawType_FillHighlight, highlight, colornode, 3, 0);
      } else {
        drawtype = GlowColor.get_drawtype(this.fill_drawtype,
          Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);
      }
      if (this.fill_eq_light !== 0 && node !== null && node.shadow !== 0) {
        drawtype =
          GlowColor.shift_drawtype(drawtype, -this.shadow_contrast + chot,
            colornode);
      } else if (this.fill_eq_bglight !== 0) {
        if (colornode !== null &&
          node.background_drawtype !== Glow.eDrawType_No) {
          drawtype = GlowColor.shift_drawtype(
            colornode.background_drawtype, -this.shadow_contrast + chot,
            colornode);
        } else {
          drawtype = GlowColor.shift_drawtype(
            this.original_fill_drawtype, -this.shadow_contrast + chot,
            colornode);
        }
      } else if (this.fill_eq_shadow !== 0 && node !== null &&
        node.shadow !== 0) {
        drawtype =
          GlowColor.shift_drawtype(drawtype, this.shadow_contrast + chot,
            colornode);
      } else if (this.fill_eq_bgshadow !== 0) {
        if (colornode !== null &&
          node.background_drawtype !== Glow.eDrawType_No) {
          drawtype = GlowColor.shift_drawtype(
            colornode.background_drawtype, this.shadow_contrast + chot,
            colornode);
        } else {
          drawtype = GlowColor.shift_drawtype(
            this.original_fill_drawtype, this.shadow_contrast + chot,
            colornode);
        }
      } else if (chot !== 0) {
        drawtype = GlowColor.shift_drawtype(drawtype, chot, null);
      }

      if (grad === Glow.eGradient_No || drawtype === Glow.eDrawType_ColorRed) {
        this.ctx.gdraw.fill_polyline(this.points, this.a_points.size(),
          drawtype, 0);
      } else {
        var f1, f2;
        var rotation;
        if (t === null) {
          rotation = this.trf.rot();
        } else {
          rotation = this.trf.rot(t);
        }

        if (this.gradient_contrast >= 0) {
          f2 = GlowColor.shift_drawtype(drawtype, -this.gradient_contrast / 2,
            null);
          f1 = GlowColor.shift_drawtype(drawtype,
            Math.floor(this.gradient_contrast / 2 + 0.6), null);
        } else {
          f2 = GlowColor.shift_drawtype(drawtype, -Math.floor(
            this.gradient_contrast) / 2 - 0.6, null);
          f1 = GlowColor.shift_drawtype(drawtype, this.gradient_contrast / 2,
            null);
        }
        this.ctx.gdraw.gradient_fill_polyline(this.points, this.a_points.size(),
          drawtype, f1, f2, this.ctx.gdraw.gradient_rotate(rotation, grad));
      }
    }

    var display_shadow = ((node !== null && node.shadow !== 0) ||
      this.shadow !== 0) && this.disable_shadow === 0 && this.fill_eq_light ===
      0 && this.fill_eq_shadow === 0;

    if (display_shadow && this.shadow_width !== 0) {
      var sp;
      var p_num;

      var trf_scale = this.trf.vertical_scale(t);
      var ish = Math.floor(this.shadow_width / 100 * trf_scale *
        Math.min((this.x_right - this.x_left) *
          this.ctx.mw.zoom_factor_x, (this.y_high - this.y_low) *
          this.ctx.mw.zoom_factor_y) + 0.5);

      if (ish >= 1) {
        sp = this.calculate_shadow(ish, highlight, colornode, 0, chot);
        p_num = sp.length;

        var p = [];
        p[0] = new GlowPointX();
        p[1] = new GlowPointX();
        p[2] = new GlowPointX();
        p[3] = new GlowPointX();
        for (i = 0; i < p_num - 1; i++) {
          p[0].x = this.points[i].x;
          p[0].y = this.points[i].y;
          p[1].x = sp[i].x;
          p[1].y = sp[i].y;
          p[3].x = this.points[i + 1].x;
          p[3].y = this.points[i + 1].y;
          p[2].x = sp[i + 1].x;
          p[2].y = sp[i + 1].y;

          this.ctx.gdraw.fill_polyline(p, 4, sp[i].drawtype, 0);
        }
      }
    }
    if (this.border !== 0 ||
      !(this.fill !== 0 || (display_shadow && this.shadow_width !== 0))) {
      drawtype =
        GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
          highlight, colornode, 0, 0);
      this.ctx.gdraw.polyline(this.points, this.a_points.size(), drawtype, idx,
        0);
    }
  };

  this.calculate_shadow =
    function (ish, highlight, colornode, javaexport, chot) {
      var sp;
      var x;
      var pos01;
      var pos12;
      var i;
      var light_drawtype;
      var dark_drawtype;

      var p_num = this.a_points.size();
      if (this.points[0].x === this.points[p_num - 1].x &&
        this.points[0].y === this.points[p_num - 1].y) {
        p_num--;
      }

      if (this.points[0].x === this.points[this.a_points.size() - 1].x &&
        this.points[0].y === this.points[this.a_points.size() - 1].y) {
        sp = new Array(p_num + 1);
      } else {
        sp = new Array(p_num);
      }

      var fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
        Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);

      var drawtype_incr = this.shadow_contrast;
      if (this.relief === Glow.eRelief_Down) {
        drawtype_incr = -this.shadow_contrast;
      }
      light_drawtype =
        GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot, colornode);
      dark_drawtype =
        GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot, colornode);

      pos01 = this.shadow_direction();

      for (i = 0; i < p_num; i++) {
        var sx0, sx1, sx2, sy0, sy1, sy2;
        var k01, m01, k12, m12;

        sp[i] = new GlowShadowInfo();

        if (i === 0) {
          sx0 = this.points[p_num - 1].x;
          sy0 = this.points[p_num - 1].y;
        } else {
          sx0 = this.points[i - 1].x;
          sy0 = this.points[i - 1].y;
        }
        sx1 = this.points[i].x;
        sy1 = this.points[i].y;
        if (i === p_num - 1) {
          sx2 = this.points[0].x;
          sy2 = this.points[0].y;
        } else {
          sx2 = this.points[i + 1].x;
          sy2 = this.points[i + 1].y;
        }

        pos12 = pos01;
        if (i !== -1) {
          if (Math.abs(sx0 - sx1) < Number.MIN_VALUE) {
            if (sx1 > sx2 && sy1 < sy0) {
              pos12 = -pos01;
            } else if (sx1 < sx2 && sy1 > sy0) {
              pos12 = -pos01;
            }
          } else if (Math.abs(sx1 - sx2) < Number.MIN_VALUE) {
            if (sx0 < sx1 && sy2 > sy1) {
              pos12 = -pos01;
            } else if (sx0 > sx1 && sy2 < sy1) {
              pos12 = -pos01;
            }
          } else if (sx1 > sx0 && sx2 < sx1) {
            pos12 = -pos01;
          } else if (sx1 < sx0 && sx2 > sx1) {
            pos12 = -pos01;
          }
        }

        if (Math.abs(sx0 - sx1) < Number.MIN_VALUE) {
          if (Math.abs(sx1 - sx2) < Number.MIN_VALUE) {
            x = sx1 + pos01 * ish;
            sp[i].x = Math.floor(x + 0.5);
            sp[i].y = Math.floor(sy1 + 0.5);
          } else {
            k12 = (sy2 - sy1) / (sx2 - sx1);
            m12 = sy1 - sx1 * k12 + pos12 * ish /
              Math.abs(Math.cos(Math.atan(k12)));

            x = sx1 + pos01 * ish;
            sp[i].x = Math.floor(x + 0.5);
            sp[i].y = Math.floor(k12 * x + m12 + 0.5);
          }
        } else if (Math.abs(sx1 - sx2) < Number.MIN_VALUE) {
          k01 = (sy1 - sy0) / (sx1 - sx0);
          m01 =
            sy0 - sx0 * k01 + pos01 * ish / Math.abs(Math.cos(Math.atan(k01)));

          x = sx1 + pos12 * ish;
          sp[i].x = Math.floor(x + 0.5);
          sp[i].y = Math.floor(k01 * x + m01 + 0.5);
        } else {
          k01 = (sy1 - sy0) / (sx1 - sx0);
          k12 = (sy2 - sy1) / (sx2 - sx1);
          m01 =
            sy0 - sx0 * k01 + pos01 * ish / Math.abs(Math.cos(Math.atan(k01)));
          m12 =
            sy1 - sx1 * k12 + pos12 * ish / Math.abs(Math.cos(Math.atan(k12)));
          if (Math.abs(k01 - k12) < Number.MIN_VALUE) {
            // Identical lines
            if (Math.abs(k01) < Number.MIN_VALUE) {
              sp[i].x = Math.floor(sx1 + 0.5);
              sp[i].y = Math.floor(m01 + 0.5);
            } else {
              k12 = -k12;
              m12 = sy2 - k12 * sx2;

              x = (m12 - m01) / (k01 - k12);
              sp[i].x = Math.floor(x + 0.5);
              sp[i].y = Math.floor(k12 * x + m12 + 0.5);
              k12 = k01;
              m12 = m01;
            }
          } else {
            x = (m12 - m01) / (k01 - k12);
            sp[i].x = Math.floor(x + 0.5);
            sp[i].y = Math.floor(k12 * x + m12 + 0.5);
          }
        }
        if (pos12 === 1) {
          sp[i].drawtype = light_drawtype;
        } else {
          sp[i].drawtype = dark_drawtype;
        }
        pos01 = pos12;
      }
      if (this.points[0].x === this.points[this.a_points.size() - 1].x &&
        this.points[0].y === this.points[this.a_points.size() - 1].y) {
        sp[p_num] = new GlowShadowInfo();
        sp[p_num].x = sp[0].x;
        sp[p_num].y = sp[0].y;
      }

      return sp;
    };

  this.shadow_direction = function () {
    var a1, a2, a1_old, a0_old;
    var a_sum1 = 0;
    var a_sum2 = 0;
    var p_num = this.a_points.size();
    a1_old = 0;
    a0_old = 0;

    if ((this.points[p_num - 1].x === this.points[0].x &&
        this.points[p_num - 1].y === this.points[0].y)) {
      p_num--;
    }

    for (var i = 0; i < p_num; i++) {
      if (i === p_num - 1) {
        if (this.points[0].x === this.points[i].x) {
          a1 = 90;
          if (this.points[0].y < this.points[i].y) {
            a1 += 180;
          }
        } else {
          a1 = Math.atan((this.points[0].y - this.points[i].y) /
            (this.points[0].x - this.points[i].x)) * 180 / Math.PI;
        }
        if (this.points[0].x < this.points[i].x) {
          a1 += 180;
        }
      } else {
        if (this.points[i + 1].x === this.points[i].x) {
          a1 = 90;
          if (this.points[i + 1].y < this.points[i].y) {
            a1 += 180;
          }
        } else {
          a1 = Math.atan((this.points[i + 1].y - this.points[i].y) /
            (this.points[i + 1].x - this.points[i].x)) * 180 / Math.PI;
        }
        if (this.points[i + 1].x < this.points[i].x) {
          a1 += 180;
        }
      }
      if (i === 0) {
        a0_old = a1;
      } else if (i > 0) {
        a2 = a1 - (a1_old - 180);
        if (a2 < 0) {
          a2 += 360;
        } else if (a2 >= 360) {
          a2 -= 360;
        }
        a_sum1 += a2;
        a_sum2 += 360 - a2;

      }
      if (i === p_num - 1) {
        a2 = a0_old - (a1 - 180);
        if (a2 < 0) {
          a2 += 360;
        } else if (a2 >= 360) {
          a2 -= 360;
        }
        a_sum1 += a2;
        a_sum2 += 360 - a2;
      }
      a1_old = a1;
    }
    var dir;
    var last_point;
    if (this.points[0].x === this.points[this.a_points.size() - 1].x &&
      this.points[0].y === this.points[this.a_points.size() - 1].y) {
      last_point = this.points[this.a_points.size() - 2];
    } else {
      last_point = this.points[this.a_points.size() - 1];
    }

    if (a_sum2 - a_sum1 < 0) {
      if (this.points[0].x === last_point.x) {
        if (this.points[0].x < this.points[1].x) {
          dir = 1;
        } else {
          dir = -1;
        }
      } else if (this.points[0].x > last_point.x) {
        dir = 1;
      } else {
        dir = -1;
      }
    } else {
      if (this.points[0].x === last_point.x) {
        if (this.points[0].x < this.points[1].x) {
          dir = 1;
        } else {
          dir = -1;
        }
      } else if (this.points[0].x < last_point.x) {
        dir = 1;
      } else {
        dir = -1;
      }
    }
    return dir;
  };

  this.add_and_shift_y_value = function (value) {
    for (var i = this.a_points.size() - 1; i > 0; i--) {
      this.a_points.get(i).y = this.a_points.get(i - 1).y;
    }
    this.a_points.get(0).y = value;
  };

  this.add_and_shift_y_value_filled = function (value) {
    for (var i = this.a_points.size() - 2; i > 1; i--) {
      this.a_points.get(i).y = this.a_points.get(i - 1).y;
    }
    this.a_points.get(1).y = value;
  };

  this.get_borders = function (t, g) {
    var i;
    var x1, y1, x2 = 0, y2 = 0;

    for (i = 0; i < this.a_points.size() - 1; i++) {
      if (t === null) {
        if (i === 0) {
          x1 = this.trf.x(this.a_points.get(i).x, this.a_points.get(i).y);
          y1 = this.trf.y(this.a_points.get(i).x, this.a_points.get(i).y);
        } else {
          x1 = x2;
          y1 = y2;
        }
        x2 = this.trf.x(this.a_points.get(i + 1).x, this.a_points.get(i + 1).y);
        y2 = this.trf.y(this.a_points.get(i + 1).x, this.a_points.get(i + 1).y);
      } else {
        if (i === 0) {
          x1 = this.trf.x(t, this.a_points.get(i).x, this.a_points.get(i).y);
          y1 = this.trf.y(t, this.a_points.get(i).x, this.a_points.get(i).y);
        } else {
          x1 = x2;
          y1 = y2;
        }
        x2 = this.trf.x(t, this.a_points.get(i).x, this.a_points.get(i).y);
        y2 = this.trf.y(t, this.a_points.get(i).x, this.a_points.get(i).y);
      }

      if (x1 < g.ll_x) {
        g.ll_x = x1;
      }
      if (x2 < g.ll_x) {
        g.ll_x = x2;
      }
      if (x1 > g.ur_x) {
        g.ur_x = x1;
      }
      if (x2 > g.ur_x) {
        g.ur_x = x2;
      }
      if (y1 < g.ll_y) {
        g.ll_y = y1;
      }
      if (y2 < g.ll_y) {
        g.ll_y = y2;
      }
      if (y1 > g.ur_y) {
        g.ur_y = y1;
      }
      if (y2 > g.ur_y) {
        g.ur_y = y2;
      }
    }
  };

  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

function GlowConPoint(ctx) {
  this.ctx = ctx;
  this.p = new GlowPoint(ctx);
  this.number;
  this.direction;
  this.trace_attribute;
  this.trace_attr_type;
  this.trf = new GlowTransform();

  this.type = function () {
    return Glow.eObjectType_ConPoint;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowPolyline : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_ConPoint:
          break;
        case Glow.eSave_ConPoint_number:
          this.number = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_ConPoint_direction:
          this.direction = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_ConPoint_p:
          i = this.p.open(lines, i + 1);
          break;
        case Glow.eSave_ConPoint_trace_attr_type:
          this.trace_attr_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_ConPoint_trace_attribute:
          if (tokens.length > 1) {
            this.trace_attribute = tokens[1];
          }
          break;
        case Glow.eSave_ConPoint_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowConPoint");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.draw = function () {
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
  };

  this.event_handler = function (event, fx, fy) {
    return 0;
  };

  this.get_borders = function (t, g) {
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

function GrowConPoint(ctx) {
  this.ctx = ctx;
  this.n_name;
  this.x_right;
  this.x_left;
  this.y_high;
  this.y_low;

  this.arc = new GlowArc(ctx);

  this.p = new GlowPoint(ctx);
  this.number;
  this.direction;
  this.trace_attribute;
  this.trace_attr_type;
  this.trf = new GlowTransform();

  this.type = function () {
    return Glow.eObjectType_GrowConPoint;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowConPoint : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowConPoint:
          break;
        case Glow.eSave_GrowConPoint_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case Glow.eSave_GrowConPoint_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowConPoint_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowConPoint_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowConPoint_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowConPoint_conpoint_part:
          i = this.glowconpoint_open(lines, i + 1);
          break;
        case Glow.eSave_GrowConPoint_arc:
          i = this.arc.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowConPoint");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.glowconpoint_open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowConPoint : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_ConPoint:
          break;
        case Glow.eSave_ConPoint_number:
          this.number = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_ConPoint_direction:
          this.number = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_ConPoint_p:
          i = this.p.open(lines, i + 1);
          break;
        case Glow.eSave_ConPoint_trace_attr_type:
          this.trace_attr_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_ConPoint_trace_attribute:
          if (tokens.length > 1) {
            this.trace_attribute = tokens[1];
          }
          break;
        case Glow.eSave_ConPoint_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowConPoint");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };


  this.event_handler = function (event, fx, fy) {
    return 0;
  };

  this.draw = function () {
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
  };

  this.get_borders = function (t, g) {
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

function GlowArc(ctx) {
  this.ctx = ctx;
  this.ll = new GlowPoint(ctx);
  this.ur = new GlowPoint(ctx);
  this.draw_type;
  this.angle1;
  this.angle2;
  this.line_width;
  this.display_level;
  this.fill;

  this.type = function () {
    return Glow.eObjectType_Arc;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowArc : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Arc:
          break;
        case Glow.eSave_Arc_draw_type:
          this.draw_type = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_angle1:
          this.angle1 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_angle2:
          this.angle2 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_line_width:
          this.line_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_fill:
          this.fill = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Arc_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case Glow.eSave_Arc_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GlowArc");
          break;
      }
      if (end) {
        break;
      }

    }
    return i;
  };

  this.draw = function () {
    this.tdraw(null, 0, 0, null, null);
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var ll_x, ll_y, ur_x, ur_y;
    ll_x = Math.floor(this.ll.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    ll_y = Math.floor(this.ll.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;
    ur_x = Math.floor(this.ur.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    ur_y = Math.floor(this.ur.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;

    if (ll_x === ur_x && ll_y === ur_y) {
      return;
    }
    var idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1,
      this.angle2, this.draw_type, idx, highlight);
  };


  this.draw_shadow = function (border, shadow, highlight, hot) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var ll_x, ll_y, ur_x, ur_y;
    ll_x = Math.floor(this.ll.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    ll_y = Math.floor(this.ll.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;
    ur_x = Math.floor(this.ur.x * this.ctx.mw.zoom_factor_x + 0.5) -
      this.ctx.mw.offset_x;
    ur_y = Math.floor(this.ur.y * this.ctx.mw.zoom_factor_y + 0.5) -
      this.ctx.mw.offset_y;
    var idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var ish = 1;
    var offs = this.border;

    if (this.shadow !== 0 && idx > 2) {
      if (this.angle1 === 0) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs *
          2, this.angle1 + 45, this.angle2 - 45,
          GlowColor.shift_drawtype(this.draw_type, -2, null), ish - 1,
          highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
          angle1, angle2 - 45,
          GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
          highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1 +
          45, this.angle2 - 45,
          GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
          highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2 -
          45, GlowColor.shift_drawtype(this.draw_type, -2, null), ish - 1,
          highlight);
      } else if (this.angle1 === 90) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
          this.angle1, this.angle2,
          GlowColor.shift_drawtype(this.draw_type, -2, null), ish - 1,
          highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2,
          GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
          highlight);
      } else if (this.angle1 === 180) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs *
          2, this.angle1 + 45, this.angle2 - 45,
          GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
          highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
          this.angle1, this.angle2 - 45,
          GlowColor.shift_drawtype(this.draw_type, -2, null), ish - 1,
          highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1 +
          45, this.angle2 - 45,
          GlowColor.shift_drawtype(this.draw_type, -2, null), ish - 1,
          highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2 -
          45, GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
          highlight);
      } else if (this.angle1 === 270) {
        this.ctx.gdraw.arc(ll_x + idx / 2 - idx + offs, ll_y + idx / 2 - idx +
          offs, ur_x - ll_x + idx - offs * 2, ur_y - ll_y + idx - offs * 2,
          this.angle1, this.angle2,
          GlowColor.shift_drawtype(this.draw_type, 2, null), ish - 1,
          highlight);
        this.ctx.gdraw.arc(ll_x + idx / 2 - offs, ll_y + idx / 2 - offs,
          Math.max(0, ur_x - ll_x - idx + offs * 2),
          Math.max(0, ur_y - ll_y - idx + offs * 2), this.angle1, this.angle2,
          GlowColor.shift_drawtype(dthis.raw_type, -2, null), ish - 1,
          highlight);
      }
    }
    if (border !== 0) {
      this.ctx.gdraw.arc(ll_x + idx / 2 - idx, ll_y + idx / 2 - idx, ur_x -
        ll_x + idx, ur_y - ll_y + idx, this.angle1, this.angle2,
        Glow.eDrawType_Line, 0, highlight);
      if (idx > 0) {
        this.ctx.gdraw.arc(ll_x + idx / 2, ll_y + idx / 2,
          Math.max(0, ur_x - ll_x - idx), Math.max(0, ur_y - ll_y - idx),
          this.angle1, this.angle2, Glow.eDrawType_Line, 0, highlight);
      }
    }
  };
}

function GrowImage(ctx) {
  var self = this;
  this.ctx = ctx;
  this.n_name;
  this.x_right;
  this.x_left;
  this.y_high;
  this.y_low;
  this.image_filename = null;
  this.dynamicsize;
  this.trf = new GlowTransform();
  this.color_lightness;
  this.color_intensity;
  this.color_shift;
  this.color_tone;
  this.fixposition;

  this.ll = new GlowPoint(ctx);
  this.ur = new GlowPoint(ctx);
  this.display_level;
  this.image = new Image();
  this.image.onLoad = function () {
    console.log("Loading image");
    self.draw();
  };

  this.type = function () {
    return Glow.eObjectType_GrowImage;
  };


  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowImage : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowImage:
          break;
        case Glow.eSave_GrowImage_n_name:
          if (tokens.length > 1) {
            this.n_name = tokens[1];
          }
          break;
        case Glow.eSave_GrowImage_image_filename:
          if (tokens.length > 1) {
            this.image_filename = tokens[1];
            //var n = window.location.href.lastIndexOf('/');
            //this.image.src = window.location.href.substring(0,n+1) + this.image_filename;
            this.image.src = this.image_filename;
          }
          break;
        case Glow.eSave_GrowImage_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowImage_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowImage_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowImage_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowImage_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowImage_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case Glow.eSave_GrowImage_trf:
          i = this.trf.open(lines, i + 1);
          break;
        case Glow.eSave_GrowImage_display_level:
          this.display_level = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowImage_ll:
          i = this.ll.open(lines, i + 1);
          break;
        case Glow.eSave_GrowImage_ur:
          i = this.ur.open(lines, i + 1);
          break;
        case Glow.eSave_GrowImage_color_tone:
          this.color_tone = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowImage_color_lightness:
          this.color_lightness = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowImage_color_intensity:
          this.color_intensity = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowImage_color_shift:
          this.color_shift = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowImage_fixposition:
          this.fixposition = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowImage");
          break;
      }
      if (end) {
        break;
      }
    }

    return i;
  };


  this.event_handler = function (event, fx, fy) {
    var rp;

    switch (event.event) {
      case Glow.eEvent_CursorMotion:
        return 0;
      default:
    }

    rp = this.trf.reverse(fx, fy);
    if (this.ll.x <= rp.x && rp.x <= this.ur.x && this.ll.y <= rp.y &&
      rp.y <= this.ur.y) {
      console.log("Event handler: Hit in image");
      return 1;
    } else {
      return 0;
    }
  };

  this.draw = function () {
    if (this.image.naturalWidth !== 0) {
      this.tdraw(null, 0, 0, null, null);
    }
  };


  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var chot = 0;
    var idx;
    var drawtype;

    if (node !== null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor -
          1);
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    // this.ctx.gdraw.rect( ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, Glow.eDrawType_Line, idx, 0);
    this.ctx.gdraw.image(this.image, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y);
  };

  this.get_borders = function (t, g) {
    var ll_x, ur_x, ll_y, ur_y, x1, x2, y1, y2;

    if (t === null) {
      x1 = this.trf.x(this.ll.x, this.ll.y);
      x2 = this.trf.x(this.ur.x, this.ur.y);
      y1 = this.trf.y(this.ll.x, this.ll.y);
      y2 = this.trf.y(this.ur.x, this.ur.y);
    } else {
      x1 = this.trf.x(t, this.ll.x, this.ll.y);
      x2 = this.trf.x(t, this.ur.x, this.ur.y);
      y1 = this.trf.y(t, this.ll.x, this.ll.y);
      y2 = this.trf.y(t, this.ur.x, this.ur.y);
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    if (ll_x < g.ll_x) {
      g.ll_x = ll_x;
    }
    if (ur_x > g.ur_x) {
      g.ur_x = ur_x;
    }
    if (ll_y < g.ll_y) {
      g.ll_y = ll_y;
    }
    if (ur_y > g.ur_y) {
      g.ur_y = ur_y;
    }
  };

  this.get_node_borders = function () {
    var g = new GlowGeometry();
    g.ll_x = g.ll_y = 1e37;
    g.ur_x = g.ur_y = -1e37;
    this.get_borders(null, g);
    this.x_left = g.ll_x;
    this.x_right = g.ur_x;
    this.y_low = g.ll_y;
    this.y_high = g.ur_y;
  };

  this.set_scale = function (scale_x, scale_y, x0, y0, type) {
    var old_x_left, old_x_right, old_y_low, old_y_high;

    if (this.trf.s_a11 !== 0 && this.trf.s_a22 !== 0 &&
      Math.abs(scale_x - this.trf.a11 / this.trf.s_a11) < Number.MIN_VALUE &&
      Math.abs(scale_y - this.trf.a22 / this.trf.s_a22) < Number.MIN_VALUE) {
      return;
    }

    switch (type) {
      case Glow.eScaleType_LowerLeft:
        x0 = this.x_left;
        y0 = this.y_low;
        break;
      case Glow.eScaleType_LowerRight:
        x0 = this.x_right;
        y0 = this.y_low;
        break;
      case Glow.eScaleType_UpperRight:
        x0 = this.x_right;
        y0 = this.y_high;
        break;
      case Glow.eScaleType_UpperLeft:
        x0 = this.x_left;
        y0 = this.y_high;
        break;
      case Glow.eScaleType_FixPoint:
        break;
      case Glow.eScaleType_Center:
        x0 = (this.x_left + this.x_right) / 2;
        y0 = (this.y_low + this.y_high) / 2;
        break;
      default:
    }

    old_x_left = this.x_left;
    old_x_right = this.x_right;
    old_y_low = this.y_low;
    old_y_high = this.y_high;
    this.trf.scale_from_stored(scale_x, scale_y, x0, y0);
    this.get_node_borders();

    switch (type) {
      case Glow.eScaleType_LowerLeft:
        this.x_left = old_x_left;
        this.y_low = old_y_low;
        break;
      case Glow.eScaleType_LowerRight:
        this.x_right = old_x_right;
        this.y_low = old_y_low;
        break;
      case Glow.eScaleType_UpperRight:
        this.x_right = old_x_right;
        this.y_high = old_y_high;
        break;
      case Glow.eScaleType_UpperLeft:
        this.x_left = old_x_left;
        this.y_high = old_y_high;
        break;
      case Glow.eScaleType_FixPoint:
        break;
      case Glow.eScaleType_Center:
        x0 = (x_left + x_right) / 2;
        y0 = (y_low + y_high) / 2;
        break;
      default:
    }
    this.ctx.draw(old_x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, old_y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, old_x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, old_y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
    this.ctx.draw(this.x_left * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x - Glow.DRAW_MP, this.y_low *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
      Glow.DRAW_MP, this.x_right * this.ctx.mw.zoom_factor_x -
      this.ctx.mw.offset_x + Glow.DRAW_MP, this.y_high *
      this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
  };

  this.getName = function () {
    return this.n_name;
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

function GrowConGlue(ctx) {
  GrowNode.call(this, ctx);
  this.grownode_open = this.open;

  this.line_width_up;
  this.line_width_down;
  this.line_width_left;
  this.line_width_right;
  this.border;

  this.type = function () {
    return Glow.eObjectType_GrowConGlue;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GlowConGlue : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowConGlue:
          break;
        case Glow.eSave_GrowConGlue_line_width_up:
          this.line_width_up = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowConGlue_line_width_down:
          this.line_width_down = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowConGlue_line_width_left:
          this.line_width_left = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowConGlue_line_width_right:
          this.line_width_right = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowConGlue_border:
          this.border = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowConGlue_node_part:
          i = this.grownode_open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowConGlue");
          break;
      }
      if (end) {
        break;
      }
    }

    return i;
  };

  this.draw = function () {
    this.tdraw(null, this.highlight, this.hot, null, null);
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }
    var idx, idx_up, idx_down, idx_left, idx_right;
    var drawtype;
    var shift_drawtype;

    idx = Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
      this.line_width - 1);
    idx += hot;
    var x1, y1, x2, y2;
    var ll_x, ll_y, ur_x, ur_y, m_x, m_y;

    if (t == null) {
      x1 = this.x_left * this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x;
      y1 = this.y_low * this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y;
      x2 = this.x_right * this.ctx.mw.zoom_factor_x - this.ctx.mw.offset_x;
      y2 = this.y_high * this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y;
    } else {
      x1 = t.x(this.x_left, this.y_low) * this.ctx.mw.zoom_factor_x -
        this.ctx.mw.offset_x;
      y1 = t.y(this.x_left, this.y_low) * this.ctx.mw.zoom_factor_y -
        this.ctx.mw.offset_y;
      x2 = t.x(this.x_right, this.y_high) * this.ctx.mw.zoom_factor_x -
        this.ctx.mw.offset_x;
      y2 = t.y(this.x_right, this.y_high) * this.ctx.mw.zoom_factor_y -
        this.ctx.mw.offset_y;
    }

    ll_x = Math.floor(Math.min(x1, x2) + 0.5);
    ur_x = Math.floor(Math.max(x1, x2) + 0.5);
    ll_y = Math.floor(Math.min(y1, y2) + 0.5);
    ur_y = Math.floor(Math.max(y1, y2) + 0.5);
    m_x = Math.floor((x1 + x2) / 2 + 0.5);
    m_y = Math.floor((y1 + y2) / 2 + 0.5);

    drawtype =
      GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
        highlight, colornode, 0, 0);

    var lw_up, lw_down, lw_left, lw_right;

    lw_down = this.line_width_down;
    lw_right = this.line_width_right;
    lw_up = this.line_width_up;
    lw_left = this.line_width_left;

    idx_up =
      Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        lw_up - 1);
    idx_up += hot;
    idx_down =
      Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
        lw_down - 1);
    idx_down += hot;
    idx_left =
      Math.floor(this.ctx.mw.zoom_factor_x / this.ctx.mw.base_zoom_factor *
        lw_left - 1);
    idx_left += hot;
    idx_right =
      Math.floor(this.ctx.mw.zoom_factor_x / this.ctx.mw.base_zoom_factor *
        lw_right - 1);
    idx_right += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    idx_up = Math.max(0, idx_up);
    idx_up = Math.min(idx_up, Glow.DRAW_TYPE_SIZE - 1);
    idx_down = Math.max(0, idx_down);
    idx_down = Math.min(idx_down, Glow.DRAW_TYPE_SIZE - 1);
    idx_left = Math.max(0, idx_left);
    idx_left = Math.min(idx_left, Glow.DRAW_TYPE_SIZE - 1);
    idx_right = Math.max(0, idx_right);
    idx_right = Math.min(idx_right, Glow.DRAW_TYPE_SIZE - 1);

    if (lw_up !== -1 && lw_down === -1 && lw_right === -1 && lw_left === -1) {
      // Up termination
      this.ctx.gdraw.fill_rect(m_x - idx_up + idx_up / 2, m_y, idx_up +
        1, ur_y - m_y, drawtype);
      if (this.shadow !== 0 && idx_up > 2) {
        if (this.border !== 0) {
          idx_up -= 2;
          m_y++;
        }

        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y, m_x + idx_up / 2, ur_y,
          shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x - idx_up +
          idx_up / 2, ur_y, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x + idx_up / 2,
          m_y, shift_drawtype, 0, 0);

        if (this.border !== 0) {
          idx_up += 2;
          m_y--;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x - idx_up +
          idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y, m_x + idx_up / 2, ur_y,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x + idx_up / 2,
          m_y, Glow.eDrawType_Line, 0, 0);
      }
    } else if (lw_up === -1 && lw_down !== -1 && lw_right === -1 &&
      lw_left === -1) {
      // Down termination
      this.ctx.gdraw.fill_rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
        1, m_y - ll_y, drawtype);
      if (this.shadow !== 0 && idx_down > 2) {
        if (this.border !== 0) {
          idx_down -= 2;
          m_y--;
        }

        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y, m_x + idx_down /
          2, m_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y, shift_drawtype, 0, 0);

        if (this.border !== 0) {
          idx_down += 2;
          m_y++;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y, m_x + idx_down /
          2, m_y, Glow.eDrawType_Line, 0, 0);
      }
    } else if (lw_up === -1 && lw_down === -1 && lw_right !== -1 &&
      lw_left === -1) {
      // Right termination
      this.ctx.gdraw.fill_rect(m_x, m_y - idx_right + idx_right / 2, ur_x -
        m_x, idx_right + 1, drawtype);
      if (this.shadow !== 0 && idx_right > 2) {
        if (this.border !== 0) {
          idx_right -= 2;
          m_x++;
        }

        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x, m_y + idx_right / 2, ur_x, m_y + idx_right / 2,
          shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, ur_x, m_y -
          idx_right + idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, m_x, m_y +
          idx_right / 2, shift_drawtype, 0, 0);

        if (this.border !== 0) {
          idx_right += 2;
          m_x--;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, ur_x, m_y -
          idx_right + idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x, m_y + idx_right / 2, ur_x, m_y + idx_right / 2,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, m_x, m_y +
          idx_right / 2, Glow.eDrawType_Line, 0, 0);
      }
    } else if (lw_up === -1 && lw_down === -1 && lw_right === -1 &&
      lw_left !== -1) {
      // Left termination
      this.ctx.gdraw.fill_rect(ll_x, m_y - idx_left + idx_left / 2, m_x -
        ll_x, idx_left + 1, drawtype);
      if (this.shadow !== 0 && idx_left > 2) {
        if (this.border !== 0) {
          idx_left -= 2;
          m_x--;
        }

        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x, m_y + idx_left / 2,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_left + idx_left / 2, m_x, m_y +
          idx_left / 2, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x, m_y -
          idx_left + idx_left / 2, shift_drawtype, 0, 0);

        if (this.border !== 0) {
          idx_left += 2;
          m_x++;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x, m_y -
          idx_left + idx_left / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x, m_y + idx_left / 2,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_left + idx_left / 2, m_x, m_y +
          idx_left / 2, Glow.eDrawType_Line, 0, 0);
      }
    } else if (lw_up !== -1 && lw_down !== -1 && lw_right === -1 &&
      lw_left === -1) {
      // Vertical glue
      this.ctx.gdraw.fill_rect(m_x - idx_up + idx_up / 2, m_y, idx_up +
        1, ur_y - m_y, drawtype);
      this.ctx.gdraw.fill_rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
        1, m_y - ll_y, drawtype);

      if (this.shadow !== 0 && idx_down > 2) {
        if (this.border !== 0) {
          idx_up -= 2;
          idx_down -= 2;
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x - idx_up +
          idx_up / 2, ur_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y, m_x + idx_up / 2, ur_y,
          shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_up += 2;
          idx_down += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x - idx_up +
          idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y, m_x + idx_up / 2, ur_y,
          Glow.eDrawType_Line, 0, 0);
        if (idx_down !== idx_up) {
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y, m_x - idx_down +
            idx_down / 2, m_y, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y, m_x + idx_down / 2, m_y,
            Glow.eDrawType_Line, 0, 0);
        }
      }
    } else if (lw_left !== -1 && lw_right !== -1 && lw_up === -1 &&
      lw_down === -1) {
      // Horizontal glue
      this.ctx.gdraw.fill_rect(m_x, m_y - idx_right + idx_right / 2, ur_x -
        m_x, idx_right + 1, drawtype);
      this.ctx.gdraw.fill_rect(ll_x, m_y - idx_left + idx_left / 2, m_x -
        ll_x, idx_left + 1, drawtype);
      if (this.shadow !== 0 && (idx_left > 2 || idx_right > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_right -= 2;
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x, m_y -
          idx_left + idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, ur_x, m_y -
          idx_right + idx_right / 2, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x, m_y + idx_left / 2,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x, m_y + idx_right / 2, ur_x, m_y + idx_right / 2,
          shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_left += 2;
          idx_right += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x, m_y -
          idx_left + idx_left / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x, m_y + idx_left / 2,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, ur_x, m_y -
          idx_right + idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x, m_y + idx_right / 2, ur_x, m_y + idx_right / 2,
          Glow.eDrawType_Line, 0, 0);
        if (idx_left !== idx_right) {
          this.ctx.gdraw.line(m_x, m_y - idx_right + idx_right / 2, m_x, m_y -
            idx_left + idx_left / 2, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x, m_y + idx_right / 2, m_x, m_y + idx_left / 2,
            Glow.eDrawType_Line, 0, 0);
        }
      }
    } else if (lw_left !== -1 && lw_right === -1 && lw_up !== -1 &&
      lw_down === -1) {
      // Left up corner
      this.ctx.gdraw.fill_rect(m_x - idx_up + idx_up / 2, m_y + idx_left /
        2, idx_up + 1, ur_y - (m_y + idx_left / 2), drawtype);
      this.ctx.gdraw.fill_rect(ll_x, m_y - idx_left + idx_left / 2, m_x +
        idx_up / 2 - ll_x + 1, idx_left + 1, drawtype);
      if (this.shadow !== 0 && (idx_left > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_up -= 2;
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x + idx_up /
          2, m_y - idx_left + idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
          idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
          2, m_y + idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
          2, m_x + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_left += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x + idx_up /
          2, m_y - idx_left + idx_left / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
          2, m_y + idx_left / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
          idx_up + idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
          2, m_x + idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
      }
    } else if (lw_left === -1 && lw_right !== -1 && lw_up !== -1 &&
      lw_down === -1) {
      // Right up corner
      this.ctx.gdraw.fill_rect(m_x - idx_up + idx_up / 2, m_y - idx_right +
        idx_right / 2, idx_up + 1, ur_y - (m_y - idx_right + idx_right / 2),
        drawtype);
      this.ctx.gdraw.fill_rect(m_x + idx_up / 2, m_y - idx_right + idx_right /
        2, ur_x - (m_x + idx_up / 2), idx_right + 1, drawtype);
      if (this.shadow !== 0 && (idx_right > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_right -= 2;
          idx_up -= 2;
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
          idx_right / 2, ur_x, m_y - idx_right + idx_right / 2, shift_drawtype,
          0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
          idx_right / 2, m_x - idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
          idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
          idx_up / 2, ur_y, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_right += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
          idx_right / 2, ur_x, m_y - idx_right + idx_right / 2,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
          idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
          idx_right / 2, m_x - idx_up + idx_up / 2, ur_y, Glow.eDrawType_Line,
          0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
          idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
      }
    } else if (lw_left === -1 && lw_right !== -1 && lw_up === -1 &&
      lw_down !== -1) {
      // Right down corner
      this.ctx.gdraw.fill_rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
        1, m_y - idx_right + idx_right / 2 - ll_y, drawtype);
      this.ctx.gdraw.fill_rect(m_x - idx_down + idx_down / 2, m_y - idx_right +
        idx_right / 2, ur_x - (m_x - idx_down + idx_down / 2), idx_right + 1,
        drawtype);
      if (this.shadow !== 0 && (idx_right > 2 || idx_down > 2)) {
        if (this.border !== 0) {
          idx_right -= 2;
          idx_down -= 2;
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
          ur_x, m_y - idx_right + idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y + idx_right / 2, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y + idx_right / 2,
          ur_x, m_y + idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
          idx_right + idx_right / 2, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_right += 2;
          idx_down += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y + idx_right / 2,
          ur_x, m_y + idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
          ur_x, m_y - idx_right + idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y + idx_right / 2, Glow.eDrawType_Line, 0,
          0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
          idx_right + idx_right / 2, Glow.eDrawType_Line, 0, 0);
      }
    } else if (lw_left !== -1 && lw_right === -1 && lw_up === -1 &&
      lw_down !== -1) {
      // Left down corner
      this.ctx.gdraw.fill_rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
        1, m_y + idx_left / 2 - ll_y + 1, drawtype);
      this.ctx.gdraw.fill_rect(ll_x, m_y - idx_left + idx_left / 2, m_x -
        idx_down + idx_down / 2 - ll_x, idx_left + 1, drawtype);
      if (this.shadow !== 0 && (idx_left > 2 || idx_down > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_down -= 2;
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x + idx_down / 2, m_y +
          idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y +
          idx_left / 2, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_left += 2;
          idx_down += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x + idx_down / 2, m_y +
          idx_left / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y +
          idx_left / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          Glow.eDrawType_Line, 0, 0);
      }
    } else if (lw_left !== -1 && lw_right !== -1 && lw_up === -1 &&
      lw_down !== -1) {
      // Left right down threeway
      this.ctx.gdraw.fill_rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
        1, m_y - ll_y + 1, drawtype);
      this.ctx.gdraw.fill_rect(ll_x, m_y - idx_left + idx_left / 2, m_x +
        idx_down / 2 - ll_x, idx_left + 1, drawtype);
      this.ctx.gdraw.fill_rect(m_x - idx_down + idx_down / 2, m_y - idx_right +
        idx_right / 2, ur_x - (m_x - idx_down + idx_down / 2), idx_right + 1,
        drawtype);
      if (this.shadow !== 0 && (idx_left > 2 || idx_right > 2 || idx_down > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_right -= 2;
          idx_down -= 2;
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
          ur_x, m_y - idx_right + idx_right / 2, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
          idx_right + idx_right / 2, shift_drawtype, 0, 0);
        if (idx_right === idx_left) {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, ur_x, m_y + idx_left /
            2, shift_drawtype, 0, 0);
        } else if (idx_right > idx_left) {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_down +
            idx_down / 2, m_y + idx_left / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y + idx_right /
            2, ur_x, m_y + idx_right / 2, shift_drawtype, 0, 0);
        } else {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x + idx_down /
            2, m_y + idx_left / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_down / 2, m_y + idx_right / 2,
            ur_x, m_y + idx_right / 2, shift_drawtype, 0, 0);
        }
        if (this.border !== 0) {
          idx_left += 2;
          idx_right += 2;
          idx_down += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
          ur_x, m_y - idx_right + idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
          idx_right + idx_right / 2, Glow.eDrawType_Line, 0, 0);
        if (idx_right === idx_left) {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, ur_x, m_y + idx_left /
            2, Glow.eDrawType_Line, 0, 0);
        } else if (idx_right > idx_left) {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_down +
            idx_down / 2, m_y + idx_left / 2, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y + idx_right /
            2, ur_x, m_y + idx_right / 2, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, m_y + idx_right /
            2, m_x - idx_down + idx_down / 2, m_y + idx_left / 2,
            Glow.eDrawType_Line, 0, 0);
        } else {
          this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x + idx_down /
            2, m_y + idx_left / 2, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_down / 2, m_y + idx_right / 2,
            ur_x, m_y + idx_right / 2, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_down / 2, m_y + idx_right / 2, m_x +
            idx_down / 2, m_y + idx_left / 2, Glow.eDrawType_Line, 0, 0);
        }
      }
    } else if (lw_left !== -1 && lw_right !== -1 && lw_up !== -1 &&
      lw_down === -1) {
      // Left right up threeway
      this.ctx.gdraw.fill_rect(m_x - idx_up + idx_up / 2, m_y, idx_up +
        1, ur_y - m_y, drawtype);
      this.ctx.gdraw.fill_rect(ll_x, m_y - idx_left + idx_left / 2, m_x +
        idx_up / 2 - ll_x, idx_left + 1, drawtype);
      this.ctx.gdraw.fill_rect(m_x - idx_up + idx_up / 2, m_y - idx_right +
        idx_right / 2, ur_x - (m_x - idx_up + idx_up / 2), idx_right + 1,
        drawtype);
      if (this.shadow !== 0 && (idx_left > 2 || idx_right > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_right -= 2;
          idx_up -= 2;
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
          idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        if (idx_right === idx_left) {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, ur_x, m_y -
            idx_left + idx_left / 2, shift_drawtype, 0, 0);
        } else if (idx_right > idx_left) {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
            idx_up + idx_up / 2, m_y - idx_left + idx_left / 2, shift_drawtype,
            0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, ur_x, m_y - idx_right + idx_right / 2,
            shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, m_x - idx_up + idx_up / 2, m_y - idx_left +
            idx_left / 2, shift_drawtype, 0, 0);
        } else {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x +
            idx_up / 2, m_y - idx_left + idx_left / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_right + idx_right / 2,
            ur_x, m_y - idx_right + idx_right / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_right + idx_right /
            2, m_x + idx_up / 2, m_y - idx_left + idx_left / 2, shift_drawtype,
            0, 0);
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
          2, m_y + idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
          idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
          idx_up / 2, ur_y, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_left += 2;
          idx_right += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
          2, m_y + idx_left / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
          idx_up + idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
          idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
          idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
        if (idx_right === idx_left) {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, ur_x, m_y -
            idx_left + idx_left / 2, Glow.eDrawType_Line, 0, 0);
        } else if (idx_right > idx_left) {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
            idx_up + idx_up / 2, m_y - idx_left + idx_left / 2,
            Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, ur_x, m_y - idx_right + idx_right / 2,
            Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, m_x - idx_up + idx_up / 2, m_y - idx_left +
            idx_left / 2, Glow.eDrawType_Line, 0, 0);
        } else {
          this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x +
            idx_up / 2, m_y - idx_left + idx_left / 2, Glow.eDrawType_Line, 0,
            0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_right + idx_right / 2,
            ur_x, m_y - idx_right + idx_right / 2, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_right + idx_right /
            2, m_x + idx_up / 2, m_y - idx_left + idx_left / 2,
            Glow.eDrawType_Line, 0, 0);
        }
      }
    } else if (lw_left !== -1 && lw_right === -1 && lw_up !== -1 &&
      lw_down !== -1) {
      // Left up down threeway
      this.ctx.gdraw.fill_rect(ll_x, m_y - idx_left + idx_left / 2, m_x -
        ll_x, idx_left + 1, drawtype);
      this.ctx.gdraw.fill_rect(m_x - idx_up + idx_up / 2, m_y - idx_left +
        idx_left / 2, idx_up + 1, ur_y - (m_y - idx_left + idx_left / 2),
        drawtype);
      this.ctx.gdraw.fill_rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
        1, m_y + idx_left / 2 - ll_y, drawtype);
      if (this.shadow !== 0 && (idx_left > 2 || idx_down > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_left -= 2;
          idx_down -= 2;
          idx_up -= 2;
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
          idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
          2, m_y + idx_left / 2, shift_drawtype, 0, 0);

        if (idx_up === idx_down) {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2,
            ur_y, shift_drawtype, 0, 0);
        } else if (idx_down > idx_up) {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down /
            2, m_y + idx_left / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_left / 2, m_x +
            idx_up / 2, ur_y, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_left / 2, m_x +
            idx_down / 2, m_y + idx_left / 2, shift_drawtype, 0, 0);
        } else {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down /
            2, m_y - idx_left + idx_left / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
            2, m_x + idx_up / 2, ur_y, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
            2, m_x + idx_down / 2, m_y - idx_left + idx_left / 2,
            shift_drawtype, 0, 0);
        }
        if (this.border !== 0) {
          idx_left += 2;
          idx_down += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
          2, m_y + idx_left / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
          idx_up + idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);

        if (idx_up === idx_down) {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2,
            ur_y, Glow.eDrawType_Line, 0, 0);
        } else if (idx_down > idx_up) {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down /
            2, m_y + idx_left / 2, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_left / 2, m_x +
            idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_left / 2, m_x +
            idx_down / 2, m_y + idx_left / 2, Glow.eDrawType_Line, 0, 0);
        } else {
          this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down /
            2, m_y - idx_left + idx_left / 2, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
            2, m_x + idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x + idx_up / 2, m_y - idx_left + idx_left /
            2, m_x + idx_down / 2, m_y - idx_left + idx_left / 2,
            Glow.eDrawType_Line, 0, 0);
        }
      }
    } else if (lw_left === -1 && lw_right !== -1 && lw_up !== -1 &&
      lw_down !== -1) {
      // Right up down threeway
      this.ctx.gdraw.fill_rect(m_x, m_y - idx_right + idx_right / 2, ur_x -
        m_x, idx_right + 1, drawtype);
      this.ctx.gdraw.fill_rect(m_x - idx_up + idx_up / 2, m_y - idx_right +
        idx_right / 2, idx_up + 1, ur_y - (m_y - idx_right + idx_right / 2),
        drawtype);
      this.ctx.gdraw.fill_rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
        1, m_y + idx_right / 2 - ll_y, drawtype);
      if (this.shadow !== 0 && (idx_right > 2 || idx_down > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_right -= 2;
          idx_down -= 2;
          idx_up -= 2;
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
          ur_x, m_y - idx_right + idx_right / 2, shift_drawtype, 0, 0);

        if (idx_up === idx_down) {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, ur_y, shift_drawtype, 0, 0);
        } else if (idx_down > idx_up) {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y + idx_right / 2, shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_right /
            2, m_x - idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
          // this.ctx.gdraw.line( m_x - idx_up + idx_up/2, m_y + idx_right/2, m_x - idx_down + idx_down/2, m_y + idx_right/2, shift_drawtype, 0, 0);
        } else {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y - idx_right + idx_right / 2,
            shift_drawtype, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, m_x - idx_up + idx_up / 2, ur_y, shift_drawtype, 0,
            0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, m_x - idx_down + idx_down / 2, m_y - idx_right +
            idx_right / 2, shift_drawtype, 0, 0);
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
          idx_right + idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
          idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
          idx_up / 2, ur_y, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_right += 2;
          idx_down += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
          ur_x, m_y - idx_right + idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
          idx_right + idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
          idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
          idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);

        if (idx_up === idx_down) {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, ur_y, Glow.eDrawType_Line, 0, 0);
        } else if (idx_down > idx_up) {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y + idx_right / 2, Glow.eDrawType_Line,
            0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_right /
            2, m_x - idx_up + idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_right /
            2, m_x - idx_down + idx_down / 2, m_y + idx_right / 2,
            Glow.eDrawType_Line, 0, 0);
        } else {
          this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
            idx_down + idx_down / 2, m_y - idx_right + idx_right / 2,
            Glow.eDrawType_Line, 0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, m_x - idx_up + idx_up / 2, ur_y, Glow.eDrawType_Line,
            0, 0);
          this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y - idx_right +
            idx_right / 2, m_x - idx_down + idx_down / 2, m_y - idx_right +
            idx_right / 2, Glow.eDrawType_Line, 0, 0);
        }
      }
    } else if (lw_left !== -1 && lw_right !== -1 && lw_up !== -1 &&
      lw_down !== -1) {
      // Right left up down fourway
      this.ctx.gdraw.fill_rect(m_x, m_y - idx_right + idx_right / 2, ur_x -
        m_x, idx_right + 1, drawtype);
      this.ctx.gdraw.fill_rect(ll_x, m_y - idx_left + idx_left / 2, m_x -
        ll_x, idx_left + 1, drawtype);
      this.ctx.gdraw.fill_rect(m_x - idx_up + idx_up / 2, m_y, idx_up +
        1, ur_y - m_y, drawtype);
      this.ctx.gdraw.fill_rect(m_x - idx_down + idx_down / 2, ll_y, idx_down +
        1, m_y - ll_y, drawtype);
      if (this.shadow !== 0 &&
        (idx_left > 2 || idx_right > 2 || idx_down > 2 || idx_up > 2)) {
        if (this.border !== 0) {
          idx_right -= 2;
          idx_left -= 2;
          idx_down -= 2;
          idx_up -= 2;
        }
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, -2, null); // Light
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
          ur_x, m_y - idx_right + idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
          idx_up + idx_up / 2, ur_y, shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.draw_type, 2, null); // Dark
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
          idx_right / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
          2, m_y + idx_left / 2, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
          idx_up / 2, ur_y, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
          idx_right + idx_right / 2, shift_drawtype, 0, 0);
        if (this.border !== 0) {
          idx_right += 2;
          idx_left += 2;
          idx_down += 2;
          idx_up += 2;
        }
      }
      if (this.border !== 0) {
        this.ctx.gdraw.line(m_x + idx_down / 2, m_y - idx_right + idx_right / 2,
          ur_x, m_y - idx_right + idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_down / 2, ll_y, m_x + idx_down / 2, m_y -
          idx_right + idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, ur_x, m_y +
          idx_right / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x + idx_up / 2, m_y + idx_right / 2, m_x +
          idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y - idx_left + idx_left / 2, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_down + idx_down / 2, ll_y, m_x -
          idx_down + idx_down / 2, m_y - idx_left + idx_left / 2,
          Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(ll_x, m_y + idx_left / 2, m_x - idx_up + idx_up /
          2, m_y + idx_left / 2, Glow.eDrawType_Line, 0, 0);
        this.ctx.gdraw.line(m_x - idx_up + idx_up / 2, m_y + idx_left / 2, m_x -
          idx_up + idx_up / 2, ur_y, Glow.eDrawType_Line, 0, 0);
      }
    } else {
      this.ctx.gdraw.line(m_x, ll_y, m_x, m_y, drawtype, idx, 0);
      this.ctx.gdraw.line(m_x, m_y, m_x, ur_y, drawtype, idx, 0);
      this.ctx.gdraw.line(ll_x, m_y, m_x, m_y, drawtype, idx, 0);
      this.ctx.gdraw.line(m_x, m_y, ur_x, m_y, drawtype, idx, 0);
    }
  };

  this.event_handler = function (event, fx, fy) {
    return 0;
  };
  this.get_borders = function (t, g) {
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

GrowConGlue.prototype = Object.create(GrowNode.prototype);


function GrowBar(ctx) {
  GrowRect.call(this, ctx);
  this.growrect_open = this.open;

  this.max_value;
  this.min_value;
  this.bar_value;
  this.bar_bordercolor;
  this.bar_borderwidth;
  this.bar_drawtype;
  this.userdata = null;

  this.type = function () {
    return Glow.eObjectType_GrowBar;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowBar : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowBar:
          break;
        case Glow.eSave_GrowBar_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowBar_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowBar_bar_value:
          this.bar_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowBar_bar_bordercolor:
          this.bar_bordercolor = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBar_bar_borderwidth:
          this.bar_borderwidth = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBar_bar_drawtype:
          this.bar_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBar_rect_part:
          i = this.growrect_open(lines, i + 1);
          break;
        case Glow.eSave_GrowBar_userdata_cb:
          if (this.ctx.appl !== null) {
            var ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case Glow.eSave_GrowBar_trace_data1:
        case Glow.eSave_GrowBar_trace_data2:
        case Glow.eSave_GrowBar_trace_attr_type:
        case Glow.eSave_GrowBar_trace_color:
        case Glow.eSave_GrowBar_trace_data3:
        case Glow.eSave_GrowBar_trace_data4:
        case Glow.eSave_GrowBar_trace_data5:
        case Glow.eSave_GrowBar_trace_data6:
        case Glow.eSave_GrowBar_trace_data7:
        case Glow.eSave_GrowBar_trace_data8:
        case Glow.eSave_GrowBar_trace_data9:
        case Glow.eSave_GrowBar_trace_data10:
        case Glow.eSave_GrowBar_access:
        case Glow.eSave_GrowBar_cycle:
        case Glow.eSave_GrowBar_ref_object:
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowBar");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var idx;
    var drawtype;
    var rotation;
    var grad = this.gradient;
    if (this.gradient === Glow.eGradient_No &&
      (node !== null && node.gradient !== Glow.eGradient_No) &&
      this.disable_gradient === 0) {
      grad = node.gradient;
    }

    var bar_border_idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * this.bar_borderwidth - 1);
    bar_border_idx =
      Math.min(Glow.DRAW_TYPE_SIZE - 1, Math.max(0, bar_border_idx));

    if (node !== null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1);
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);
    if (this.fill !== 0) {
      drawtype =
        GlowColor.get_drawtype(this.fill_drawtype, Glow.eDrawType_FillHighlight,
          highlight, colornode, 1, 0);

      if (grad === Glow.eGradient_No) {
        this.ctx.gdraw.fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
          drawtype);
      } else {
        var fa1, fa2;

        if (t === null) {
          rotation = this.trf.rot();
        } else {
          rotation = this.trf.rot(t);
        }
        if (this.gradient_contrast >= 0) {
          fa2 = GlowColor.shift_drawtype(drawtype, -this.gradient_contrast / 2,
            null);
          fa1 = GlowColor.shift_drawtype(drawtype,
            Math.floor((this.gradient_contrast) / 2 + 0.6), null);
        } else {
          fa2 = GlowColor.shift_drawtype(drawtype,
            -Math.floor((this.gradient_contrast) / 2 - 0.6), null);
          fa1 = GlowColor.shift_drawtype(drawtype, this.gradient_contrast / 2,
            null);
        }
        this.ctx.gdraw.gradient_fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
          drawtype, fa1, fa2, this.ctx.gdraw.gradient_rotate(rotation, grad));
      }
    }
    drawtype =
      GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
        highlight, colornode, 0, 0);

    if (this.max_value !== this.min_value) {
      var x0, y0, width, height, l_x0, l_y0, l_x1, l_y1;

      if (t === null) {
        rotation =
          (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) * 360;
      } else {
        rotation =
          (this.trf.rot(t) / 360 - Math.floor(this.trf.rot(t) / 360)) * 360;
      }
      if (45 >= rotation || rotation > 315) {
        height = Math.floor((this.bar_value - this.min_value) /
          (this.max_value - this.min_value) * (ur_y - ll_y));
        height = Math.max(0, Math.min(height, ur_y - ll_y));
        width = ur_x - ll_x;
        x0 = ll_x;
        y0 = ur_y - height;
        l_x0 = ll_x;
        l_y0 = ur_y - height;
        l_x1 = ur_x;
        l_y1 = ur_y - height;
      } else if (45 < rotation && rotation <= 135) {
        width = Math.floor((this.bar_value - this.min_value) /
          (this.max_value - this.min_value) * (ur_x - ll_x));
        width = Math.max(0, Math.min(width, ur_x - ll_x));
        height = ur_y - ll_y;
        x0 = ll_x;
        y0 = ll_y;
        l_x0 = ll_x + width;
        l_y0 = ll_y;
        l_x1 = ll_x + width;
        l_y1 = ur_y;
      } else if (135 < rotation && rotation <= 225) {
        height = Math.floor((this.bar_value - this.min_value) /
          (this.max_value - this.min_value) * (ur_y - ll_y));
        height = Math.max(0, Math.min(height, ur_y - ll_y));
        width = ur_x - ll_x;
        x0 = ll_x;
        y0 = ll_y;
        l_x0 = ll_x;
        l_y0 = ll_y + height;
        l_x1 = ur_x;
        l_y1 = ll_y + height;
      } else { // if ( 225 < rotation && rotation <= 315)
        width = Math.floor((this.bar_value - this.min_value) /
          (this.max_value - this.min_value) * (ur_x - ll_x));
        width = Math.max(0, Math.min(width, ur_x - ll_x));
        height = ur_y - ll_y;
        x0 = ur_x - width;
        y0 = ll_y;
        l_x0 = ur_x - width;
        l_y0 = ll_y;
        l_x1 = ur_x - width;
        l_y1 = ur_y;
      }

      var dt = drawtype;
      if (this.bar_drawtype !== Glow.eDrawType_Inherit) {
        dt = this.bar_drawtype;
      }

      if (grad === Glow.eGradient_No) {
        this.ctx.gdraw.fill_rect(x0, y0, width, height, dt);
      } else {
        var fb1, fb2;

        if (t === null) {
          rotation = this.trf.rot();
        } else {
          rotation = this.trf.rot(t);
        }
        if (this.gradient_contrast >= 0) {
          fb2 = GlowColor.shift_drawtype(dt, -this.gradient_contrast / 2, null);
          fb1 = GlowColor.shift_drawtype(dt,
            Math.floor((this.gradient_contrast) / 2 + 0.6), null);
        } else {
          fb2 = GlowColor.shift_drawtype(dt,
            -Math.floor((this.gradient_contrast) / 2 - 0.6), null);
          fb1 = GlowColor.shift_drawtype(dt, this.gradient_contrast / 2, null);
        }
        this.ctx.gdraw.gradient_fill_rect(x0, y0, width, height, dt, fb1, fb2,
          this.ctx.gdraw.gradient_rotate(rotation, grad));
      }

      dt = drawtype;
      if (this.bar_bordercolor !== Glow.eDrawType_Inherit) {
        dt = this.bar_bordercolor;
      }
      this.ctx.gdraw.line(l_x0, l_y0, l_x1, l_y1, dt, bar_border_idx, 0);
    }

    if (this.border !== 0) {
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, idx,
        0);
    }
  };

  this.set_range = function (min, max) {
    this.max_value = max;
    this.min_value = min;
    this.draw();
  };

  this.set_value = function (value) {
    this.bar_value = value;
    this.draw();
  };
  this.getClassDynType1 = function () {
    return 0;
  };
  this.getClassDynType2 = function () {
    return 0;
  };
  this.getClassActionType1 = function () {
    return 0;
  };
  this.getClassActionType2 = function () {
    return 0;
  };
  this.getClassCycle = function () {
    return 0;
  };
  this.getClassUserData = function () {
    return null;
  };
  this.getUserData = function () {
    return this.userdata;
  };

  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

GrowBar.prototype = Object.create(GrowRect.prototype);

function GrowBarArc(ctx) {
  GrowArc.call(this, ctx);
  this.growarc_open = this.open;

  this.max_value;
  this.min_value;
  this.bar_width;
  this.bar_value;
  this.bar_bordercolor;
  this.bar_borderwidth;
  this.bar_drawtype;
  this.bar_direction;
  this.userdata = null;

  this.type = function () {
    return Glow.eObjectType_GrowBarArc;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowBarArc : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowBarArc:
          break;
        case Glow.eSave_GrowBarArc_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowBarArc_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowBarArc_bar_width:
          this.bar_width = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowBarArc_bar_direction:
          this.bar_direction = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarArc_bar_value:
          this.bar_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowBarArc_bar_bordercolor:
          this.bar_bordercolor = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarArc_bar_borderwidth:
          this.bar_borderwidth = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarArc_bar_drawtype:
          this.bar_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarArc_arc_part:
          i = this.growarc_open(lines, i + 1);
          break;
        case Glow.eSave_GrowBarArc_userdata_cb:
          if (this.ctx.appl !== null) {
            var ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowBarArc");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var idx;
    var rotation;
    var ang;
    var drawtype;
    var bg_drawtype;
    var yscale;
    var width = Math.floor(this.bar_width * this.ctx.mw.zoom_factor_x);
    var value = Math.max(this.min_value,
      Math.min(this.bar_value, this.max_value));

    if (node !== null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1);
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      rotation =
        (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) * 360;
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      rotation =
        (this.trf.rot(t) / 360 - Math.floor(this.trf.rot(t) / 360)) * 360;
    }


    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    yscale = (ur_y - ll_y) / (ur_x - ll_x);
    if (width > ur_x - ll_x) {
      width = ur_x - ll_x;
    }
    drawtype =
      GlowColor.get_drawtype(this.fill_drawtype, Glow.eDrawType_FillHighlight,
        highlight, colornode, 1, 0);

    if (this.background_drawtype === Glow.eDrawType_No) {
      bg_drawtype = this.ctx.background_color;
    } else {
      bg_drawtype = this.background_drawtype;
    }

    // Draw circle background
    this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, 0, 360,
      bg_drawtype);

    // Draw bar background
    this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1 -
      Math.floor(rotation), this.angle2, drawtype);

    // Draw bar
    if (this.bar_direction === 0) {
      ang = this.angle1 - rotation;
    } else {
      ang = this.angle1 + this.angle2 * (this.max_value - value) /
        (this.max_value - this.min_value) - rotation;
    }

    if (this.gradient === Glow.eGradient_No) {
      this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
        ang, this.angle2 * (value - this.min_value) /
        (this.max_value - this.min_value), this.bar_drawtype);
    } else {
      var f1, f2;

      if (this.gradient_contrast >= 0) {
        f2 =
          GlowColor.shift_drawtype(this.bar_drawtype, -this.gradient_contrast /
            2, null);
        f1 = GlowColor.shift_drawtype(this.bar_drawtype,
          Math.floor(this.gradient_contrast / 2 + 0.6), null);
      } else {
        f2 = GlowColor.shift_drawtype(this.bar_drawtype,
          -Math.floor(this.gradient_contrast / 2 - 0.6), null);
        f1 =
          GlowColor.shift_drawtype(this.bar_drawtype, this.gradient_contrast /
            2, null);
      }
      this.ctx.gdraw.gradient_fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
        ang, this.angle2 * (value - this.min_value) /
        (this.max_value - this.min_value), this.bar_drawtype, f1, f2,
        this.gradient);
    }

    // Draw inner circle background
    this.ctx.gdraw.fill_arc(ll_x + width, ll_y + yscale * width, ur_x - ll_x -
      2 * width, ur_y - ll_y - yscale * 2 * width, 0, 360, bg_drawtype);

    if (this.bar_direction === 0) {
      ang = Math.PI *
        (this.angle1 - rotation + this.angle2 * (value - this.min_value) /
          (this.max_value - this.min_value)) / 180;
    } else {
      ang = Math.PI *
        (this.angle1 - rotation + this.angle2 * (this.max_value - value) /
          (this.max_value - this.min_value)) / 180;
    }

    this.ctx.gdraw.line((ur_x + ll_x) / 2 + (ur_x - ll_x) / 2 *
      Math.cos(ang), (ur_y + ll_y) / 2 - (ur_y - ll_y) / 2 *
      Math.sin(ang), (ur_x + ll_x) / 2 + ((ur_x - ll_x) / 2 - width) *
      Math.cos(ang), (ur_y + ll_y) / 2 - yscale * ((ur_x - ll_x) / 2 - width) *
      Math.sin(ang), this.bar_bordercolor, this.bar_borderwidth, 0);

    if (this.border !== 0) {
      var bordercolor = GlowColor.get_drawtype(this.draw_type,
        Glow.eDrawType_LineHighlight, highlight, colornode, 0, 0);

      this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1 -
        Math.floor(rotation), this.angle2, bordercolor, idx);

      this.ctx.gdraw.arc(ll_x + width, ll_y + yscale * width, ur_x - ll_x - 2 *
        width, ur_y - ll_y - yscale * 2 * width, this.angle1 -
        Math.floor(rotation), this.angle2, bordercolor, idx);

      ang = Math.PI * (this.angle1 - rotation) / 180;
      this.ctx.gdraw.line((ur_x + ll_x) / 2 + (ur_x - ll_x) / 2 *
        Math.cos(ang), (ur_y + ll_y) / 2 - (ur_y - ll_y) / 2 *
        Math.sin(ang), (ur_x + ll_x) / 2 + ((ur_x - ll_x) / 2 - width) *
        Math.cos(ang), (ur_y + ll_y) / 2 - yscale *
        ((ur_x - ll_x) / 2 - width) * Math.sin(ang), bordercolor, idx, 0);

      ang = Math.PI * (this.angle1 + this.angle2 - rotation) / 180;
      this.ctx.gdraw.line((ur_x + ll_x) / 2 + (ur_x - ll_x) / 2 *
        Math.cos(ang), (ur_y + ll_y) / 2 - (ur_y - ll_y) / 2 *
        Math.sin(ang), (ur_x + ll_x) / 2 + ((ur_x - ll_x) / 2 - width) *
        Math.cos(ang), (ur_y + ll_y) / 2 - yscale *
        ((ur_x - ll_x) / 2 - width) * Math.sin(ang), bordercolor, idx, 0);

    }
  };

  this.set_range = function (min, max) {
    this.max_value = max;
    this.min_value = min;
    this.draw();
  };

  this.set_value = function (value) {
    this.bar_value = value;
    this.draw();
  };
  this.getClassDynType1 = function () {
    return 0;
  };
  this.getClassDynType2 = function () {
    return 0;
  };
  this.getClassActionType1 = function () {
    return 0;
  };
  this.getClassActionType2 = function () {
    return 0;
  };
  this.getClassCycle = function () {
    return 0;
  };
  this.getClassUserData = function () {
    return null;
  };
  this.getUserData = function () {
    return this.userdata;
  };

  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

GrowBarArc.prototype = Object.create(GrowArc.prototype);

function GrowTrend(ctx) {
  GrowRect.call(this, ctx);
  this.growrect_open = this.open;

  this.y_max_value = new Array(Glow.TREND_MAX_CURVES);
  this.y_min_value = new Array(Glow.TREND_MAX_CURVES);
  this.x_max_value = new Array(Glow.TREND_MAX_CURVES);
  this.x_min_value = new Array(Glow.TREND_MAX_CURVES);
  this.vertical_lines;
  this.horizontal_lines;
  this.fill_curve;
  this.no_of_points;
  this.curve_width;
  this.curve_drawtype = new Array(Glow.TREND_MAX_CURVES);
  this.curve_fill_drawtype = new Array(Glow.TREND_MAX_CURVES);
  this.scan_time;
  this.curve = new Array(Glow.TREND_MAX_CURVES);
  this.curve_cnt = 1;
  this.userdata = null;
  this.display_x_mark1 = 0;
  this.display_x_mark2 = 0;
  this.display_y_mark1 = 0;
  this.display_y_mark2 = 0;
  this.x_mark1;
  this.x_mark2;
  this.y_mark1;
  this.y_mark2;
  this.mark1_color;
  this.mark2_color;

  for (var i = 0; i < Glow.TREND_MAX_CURVES; i++) {
    this.curve[i] = null;
  }

  this.type = function () {
    return Glow.eObjectType_GrowTrend;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowTrend : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowTrend:
          break;

        case Glow.eSave_GrowTrend_y_max_value_0:
          this.y_max_value[0] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTrend_y_max_value_1:
          this.y_max_value[1] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTrend_y_min_value_0:
          this.y_min_value[0] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTrend_y_min_value_1:
          this.y_min_value[1] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTrend_x_max_value_0:
          this.x_max_value[0] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTrend_x_max_value_1:
          this.x_max_value[1] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTrend_x_min_value_0:
          this.x_min_value[0] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTrend_x_min_value_1:
          this.x_min_value[1] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTrend_horizontal_lines:
          this.horizontal_lines = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTrend_vertical_lines:
          this.vertical_lines = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTrend_fill_curve:
          this.fill_curve = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTrend_curve_drawtype_0:
          this.curve_drawtype[0] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTrend_curve_drawtype_1:
          this.curve_drawtype[1] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTrend_curve_fill_drawtype_0:
          this.curve_fill_drawtype[0] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTrend_curve_fill_drawtype_1:
          this.curve_fill_drawtype[1] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTrend_no_of_points:
          this.no_of_points = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTrend_curve_width:
          this.curve_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTrend_scan_time:
          this.scan_time = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTrend_rect_part:
          i = this.growrect_open(lines, i + 1);
          break;
        case Glow.eSave_GrowTrend_trace_data1:
        case Glow.eSave_GrowTrend_trace_data2:
        case Glow.eSave_GrowTrend_trace_data3:
        case Glow.eSave_GrowTrend_trace_data4:
        case Glow.eSave_GrowTrend_trace_data5:
        case Glow.eSave_GrowTrend_trace_data6:
        case Glow.eSave_GrowTrend_trace_data7:
        case Glow.eSave_GrowTrend_trace_data8:
        case Glow.eSave_GrowTrend_trace_data9:
        case Glow.eSave_GrowTrend_trace_data10:
        case Glow.eSave_GrowTrend_trace_attr_type:
        case Glow.eSave_GrowTrend_trace_color:
        case Glow.eSave_GrowTrend_access:
        case Glow.eSave_GrowTrend_cycle:
        case Glow.eSave_GrowTrend_ref_object:
          break;
        case Glow.eSave_GrowTrend_userdata_cb:
          if (ctx.appl !== null) {
            var ret = ctx.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;

        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowTrend", i, key);
          break;
      }
      if (end) {
        break;
      }
    }

    this.configure_curves();
    return i;
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }
    var i;
    var idx;
    var drawtype;

    if (node !== null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1);
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);
    if (this.fill !== 0) {
      var grad = this.gradient;
      if (this.gradient === Glow.eGradient_No &&
        (node !== null && node.gradient !== Glow.eGradient_No) &&
        this.disable_gradient === 0) {
        grad = node.gradient;
      }

      drawtype =
        GlowColor.get_drawtype(this.fill_drawtype, Glow.eDrawType_FillHighlight,
          highlight, colornode, 1, 0);
      if (grad === Glow.eGradient_No) {
        this.ctx.gdraw.fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
          drawtype);
      } else {
        var f1, f2;
        var rotation;

        if (t === null) {
          rotation = this.trf.rot();
        } else {
          rotation = this.trf.rot(t);
        }
        if (this.gradient_contrast >= 0) {
          f2 = GlowColor.shift_drawtype(drawtype, -this.gradient_contrast / 2,
            null);
          f1 = GlowColor.shift_drawtype(drawtype,
            Math.floor(this.gradient_contrast / 2 + 0.6), null);
        } else {
          f2 = GlowColor.shift_drawtype(drawtype,
            -Math.floor(this.gradient_contrast / 2 - 0.6), null);
          f1 = GlowColor.shift_drawtype(drawtype, this.gradient_contrast / 2,
            null);
        }
        this.ctx.gdraw.gradient_fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
          drawtype, f1, f2, this.ctx.gdraw.gradient_rotate(rotation, grad));
      }
    }
    drawtype =
      GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
        highlight, colornode, 0, 0);
    if (this.fill_curve !== 0) {
      for (i = 0; i < this.curve_cnt; i++) {
        if (this.curve[i] !== null) {
          this.curve[i].border = 0;
        }
      }
      if (t === null) {
        for (i = 0; i < this.curve_cnt; i++) {
          if (this.curve[i] !== null) {
            //for ( var j = 0; j < 10; j++)
            //  console.log( "tdraw", i, this.curve[i].points[j].x, this.curve[i].points[j].y);
            this.curve[i].tdraw(this.trf, highlight, hot, node, colornode);
          }
        }
      } else {
        var tmp1 = t.multiply(this.trf);
        for (i = 0; i < this.curve_cnt; i++) {
          if (curve[i] !== null) {
            curve[i].tdraw(tmp1, highlight, hot, node, colornode);
          }
        }
      }
      for (i = 0; i < this.curve_cnt; i++) {
        if (this.curve[i] !== null) {
          this.curve[i].border = 1;
        }
      }
    }

    for (i = 0; i < this.vertical_lines; i++) {
      var x = Math.floor(ll_x + (ur_x - ll_x) / (this.vertical_lines + 1) *
        (i + 1));
      this.ctx.gdraw.line(x, ll_y, x, ur_y, drawtype, 0, 0);
    }

    for (i = 0; i < this.horizontal_lines; i++) {
      var y = Math.floor(ll_y + (ur_y - ll_y) / (this.horizontal_lines + 1) *
        (i + 1));
      this.ctx.gdraw.line(ll_x, y, ur_x, y, drawtype, 0, 0);
    }

    if (this.fill_curve !== 0) {
      for (i = 0; i < this.curve_cnt; i++) {
        if (this.curve[i] !== null) {
          this.curve[i].fill = 0;
        }
      }
    }
    if (t === null) {
      for (i = 0; i < this.curve_cnt; i++) {
        if (this.curve[i] !== null) {
          this.curve[i].tdraw(this.trf, highlight, hot, node, colornode);
        }
      }
    } else {
      var tmp2 = t.multiply(this.trf);
      for (i = 0; i < this.curve_cnt; i++) {
        if (this.curve[i] !== null) {
          this.curve[i].tdraw(tmp2, highlight, hot, node, colornode);
        }
      }
    }

    if (this.fill_curve !== 0) {
      for (i = 0; i < this.curve_cnt; i++) {
        if (this.curve[i] !== null) {
          this.curve[i].fill = 1;
        }
      }
    }

    if (this.display_x_mark1 !== 0) {
      var xm1;
      if (t === null) {
        xm1 = Math.floor(this.trf.x(this.x_mark1, this.ll.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      } else {
        xm1 = Math.floor(this.trf.x(t, this.x_mark1, this.ll.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      }
      if (xm1 >= ll_x && xm1 <= ur_x) {
        drawtype = this.mark1_color;
        if (drawtype === Glow.eDrawType_Inherit) {
          drawtype = Glow.eDrawType_ColorYellow;
        }
        this.ctx.gdraw.line(xm1, ll_y, xm1, ur_y, drawtype, idx, 0);
      }
    }
    if (this.display_x_mark2 !== 0) {
      var xm2;
      if (t === null) {
        xm2 = Math.floor(this.trf.x(this.x_mark2, this.ll.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      } else {
        xm2 = Math.floor(this.trf.x(t, this.x_mark2, this.ll.y) *
          this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      }
      if (xm2 >= ll_x && xm2 <= ur_x) {
        drawtype = this.mark2_color;
        if (drawtype === Glow.eDrawType_Inherit) {
          drawtype = Glow.eDrawType_ColorYellow;
        }
        this.ctx.gdraw.line(xm2, ll_y, xm2, ur_y, drawtype, idx, 0);
      }
    }
    if (this.display_y_mark1 !== 0) {
      var ym1;
      if (t === null) {
        ym1 = Math.floor(this.trf.y(this.ll.x, this.y_mark1) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      } else {
        ym1 = Math.floor(this.trf.y(t, this.ll.x, this.y_mark1) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      }
      if (ym1 >= ll_y && ym1 <= ur_y) {
        drawtype = this.mark1_color;
        if (drawtype === Glow.eDrawType_Inherit) {
          drawtype = Glow.eDrawType_ColorYellow;
        }
        this.ctx.gdraw.line(ll_x, ym1, ur_x, ym1, drawtype, idx, 0);
      }
    }
    if (this.display_y_mark2 !== 0) {
      var ym2;
      if (t === null) {
        ym2 = Math.floor(this.trf.y(this.ll.x, this.y_mark2) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      } else {
        ym2 = Math.floor(this.trf.y(t, this.ll.x, this.y_mark2) *
          this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      }
      if (ym2 >= ll_y && ym2 <= ur_y) {
        drawtype = this.mark2_color;
        if (drawtype === Glow.eDrawType_Inherit) {
          drawtype = Glow.eDrawType_ColorYellow;
        }
        this.ctx.gdraw.line(ll_x, ym2, ur_x, ym2, drawtype, idx, 0);
      }
    }

    if (this.border !== 0) {
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, idx,
        0);
    }
  };

  this.set_range = function (curve, min, max) {
    if (!(curve === 0 || curve === 1)) {
      return;
    }
    this.y_max_value[curve] = max;
    this.y_min_value[curve] = min;
    this.configure_curves();
  };

  this.configure_curves = function () {
    var dt, dt_fill;
    var points;
    var pointarray = [];
    var point_p = new GlowPoint();
    var i;


    this.curve_cnt = 2;

    this.no_of_points = Math.max(2, this.no_of_points);
    points = this.no_of_points;
    if (this.fill_curve !== 0) {
      points += 2;
    }
    this.curve_width =
      Math.min(Glow.DRAW_TYPE_SIZE, Math.max(1, this.curve_width));

    pointarray = new Array(points);
    for (i = 0; i < points; i++) {
      pointarray[i] = new GlowPoint();
      point_p = pointarray[i];
      if (this.fill_curve === 0) {
        point_p.y = this.ur.y;
        point_p.x = this.ur.x - i * (this.ur.x - this.ll.x) / (points - 1);
      } else {
        point_p.y = this.ur.y;
        if (i === 0) {
          point_p.x = this.ur.x;
        } else if (i === points - 1) {
          point_p.x = this.ll.x;
        } else {
          point_p.x =
            this.ur.x - (i - 1) * (this.ur.x - this.ll.x) / (points - 3);
        }
      }
    }

    for (i = 0; i < this.curve_cnt; i++) {
      if (this.curve[i] !== null) {
        this.curve[i] = null;
      }

      if (this.curve_drawtype[i] === Glow.eDrawType_Inherit) {
        dt = this.draw_type;
      } else {
        dt = this.curve_drawtype[i];
      }

      if (this.curve_fill_drawtype[i] === Glow.eDrawType_Inherit) {
        dt_fill = this.draw_type;
      } else {
        dt_fill = this.curve_fill_drawtype[i];
      }

      this.ctx.nodraw++;
      this.curve[i] = new GrowPolyline(this.ctx);
      this.curve[i].init("", pointarray, points, dt, this.curve_width, 0,
        this.fill_curve, 1, 0, dt_fill);
      this.ctx.nodraw--;

    }
  };

  this.set_range_y = function (curve, min, max) {
    if (!(curve === 0 || curve === 1)) {
      return;
    }

    var mark;
    if (curve === 0) {
      if (this.display_y_mark1 !== 0) {
        mark = this.y_min_value[0] - (this.y_mark1 - this.ur.y) *
          (this.y_max_value[0] - this.y_min_value[0]) / (this.ur.y - this.ll.y);
        this.y_mark1 =
          this.ur.y - (mark - min) / (max - min) * (this.ur.y - this.ll.y);
      }
      if (this.display_y_mark2 !== 0) {
        mark = this.y_min_value[0] - (this.y_mark2 - ur.y) *
          (this.y_max_value[0] - this.y_min_value[0]) / (this.ur.y - this.ll.y);
        this.y_mark2 =
          this.ur.y - (mark - min) / (max - min) * (this.ur.y - this.ll.y);
      }
    }

    this.y_max_value[curve] = max;
    this.y_min_value[curve] = min;
    this.configure_curves();
  };


  this.add_value = function (value, idx) {
    var curve_value = 0;
    if (idx >= this.curve_cnt) {
      return;
    }
    if (this.y_max_value[idx] !== this.y_min_value[idx]) {
      curve_value = this.ur.y - (value - this.y_min_value[idx]) /
        (this.y_max_value[idx] - this.y_min_value[idx]) *
        (this.ur.y - this.ll.y);
    }

    curve_value = Math.max(this.ll.y, Math.min(curve_value, this.ur.y));
    if (this.fill_curve === 0) {
      this.curve[idx].add_and_shift_y_value(curve_value);
    } else {
      this.curve[idx].add_and_shift_y_value_filled(curve_value);
    }
    this.draw();
  };

  this.set_scan_time = function (time) {
    this.scan_time = time;
    this.configure_curves();
  };

  this.get_scan_time = function () {
    return this.scan_time;
  };
  this.get_no_of_points = function () {
    return this.no_of_points;
  };
  this.set_x_mark1 = function (mark) {
    this.display_x_mark1 = 1;
    this.x_mark1 = this.ll.x + (mark - this.x_min_value[0]) /
      (this.x_max_value[0] - this.x_min_value[0]) * (this.ur.x - this.ll.x);
    this.draw();
  };
  this.set_x_mark2 = function (mark) {
    this.display_x_mark2 = 1;
    this.x_mark2 = this.ll.x + (mark - this.x_min_value[0]) /
      (this.x_max_value[0] - this.x_min_value[0]) * (this.ur.x - this.ll.x);
    this.draw();
  };
  this.set_y_mark1 = function (mark) {
    this.display_y_mark1 = 1;
    this.y_mark1 = this.ur.y - (mark - this.y_min_value[0]) /
      (this.y_max_value[0] - this.y_min_value[0]) * (this.ur.y - this.ll.y);
    this.draw();
  };
  this.set_y_mark2 = function (mark) {
    this.display_y_mark2 = 1;
    this.y_mark2 = this.ur.y - (mark - this.y_min_value[0]) /
      (this.y_max_value[0] - this.y_min_value[0]) * (this.ur.y - this.ll.y);
    this.draw();
  };

  this.set_mark_color = function (m1color, m2color) {
    this.mark1_color = m1color;
    this.mark2_color = m2color;
  };
  this.getClassDynType1 = function () {
    return 0;
  };
  this.getClassDynType2 = function () {
    return 0;
  };
  this.getClassActionType1 = function () {
    return 0;
  };
  this.getClassActionType2 = function () {
    return 0;
  };
  this.getClassCycle = function () {
    return 0;
  };
  this.getClassUserData = function () {
    return null;
  };
  this.getClassUserData = function () {
    return nc.userdata;
  };
  this.getUserData = function () {
    return this.userdata;
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

GrowTrend.prototype = Object.create(GrowRect.prototype);

function GrowXYCurve(ctx) {
  GrowTrend.call(this, ctx);
  this.growtrend_open = this.open;

  this.type = function () {
    return Glow.eObjectType_GrowXYCurve;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowXYCurve : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowXYCurve:
          break;

        case Glow.eSave_GrowXYCurve_trend_part:
          i = this.growtrend_open(lines, i + 1);
          break;

        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowXYCurve");
          break;
      }
      if (end) {
        break;
      }
    }

    return i;
  };

  this.set_xy_range_x = function (curve, min, max) {
    if (curve > Glow.TREND_MAX_CURVES || curve < 0) {
      return;
    }
    if (curve === 0) {
      if (this.display_x_mark1 !== 0) {
        var mark = (this.x_mark1 - this.ll.x) *
          (this.x_max_value[0] - this.x_min_value[0]) /
          (this.ur.x - this.ll.x) + this.x_min_value[0];
        this.x_mark1 =
          this.ll.x + (mark - min) / (max - min) * (this.ur.x - this.ll.x);
      }
      if (this.display_x_mark2 !== 0) {
        var mark = (this.x_mark2 - this.ll.x) *
          (this.x_max_value[0] - this.x_min_value[0]) /
          (this.ur.x - this.ll.x) + this.x_min_value[0];
        this.x_mark2 =
          this.ll.x + (mark - min) / (max - min) * (this.ur.x - this.ll.x);
      }
    }
    this.x_max_value[curve] = max;
    this.x_min_value[curve] = min;
  };

  this.set_xy_range_y = function (curve, min, max) {
    if (curve > Glow.TREND_MAX_CURVES || curve < 0) {
      return;
    }

    if (curve === 0) {
      if (this.display_y_mark1 !== 0) {
        var mark = this.y_min_value[0] - (this.y_mark1 - this.ur.y) *
          (this.y_max_value[0] - this.y_min_value[0]) / (this.ur.y - this.ll.y);
        this.y_mark1 =
          this.ur.y - (mark - min) / (max - min) * (this.ur.y - this.ll.y);
      }
      if (this.display_y_mark2 !== 0) {
        var mark = this.y_min_value[0] - (this.y_mark2 - this.ur.y) *
          (this.y_max_value[0] - this.y_min_value[0]) / (this.ur.y - this.ll.y);
        this.y_mark2 =
          this.ur.y - (mark - min) / (max - min) * (this.ur.y - this.ll.y);
      }
    }

    this.y_max_value[curve] = max;
    this.y_min_value[curve] = min;
  };

  this.set_xy_noofcurves = function (noofcurves) {
    this.curve_cnt = noofcurves;
  };

  this.set_xy_curve_color = function (curve, curve_color, fill_color) {
    if (curve > Glow.TREND_MAX_CURVES || curve < 0) {
      return;
    }
    this.curve_drawtype[curve] = curve_color;
    this.curve_fill_drawtype[curve] = fill_color;
  };

  this.set_xy_data = function (y_data, x_data, curve_idx, data_points) {
    var dt, dt_fill;
    var points;
    var cpoints;
    var pointarray;
    var point_p;
    var i, j, idx;

    if (curve_idx > Glow.TREND_MAX_CURVES || curve_idx < 0) {
      return;
    }

    this.no_of_points = Math.max(2, this.no_of_points);
    points = cpoints = Math.min(this.no_of_points, data_points);
    if (this.fill_curve !== 0) {
      cpoints += 2;
    }

    this.curve_width =
      Math.min(Glow.DRAW_TYPE_SIZE, Math.max(1, this.curve_width));

    pointarray = new Array(cpoints);
    j = curve_idx;
    for (i = 0, idx = 0; i < cpoints; i++, idx++) {
      point_p = pointarray[i] = new GlowPointX();
      if (this.fill_curve === 0) {
        idx = i;
        if (this.y_max_value[j] !== this.y_min_value[j]) {
          point_p.y = this.ur.y - (y_data[idx] - this.y_min_value[j]) /
            (this.y_max_value[j] - this.y_min_value[j]) *
            (this.ur.y - this.ll.y);
        }

        point_p.y = Math.max(this.ll.y, Math.min(point_p.y, this.ur.y));

        if (this.x_max_value[j] !== this.x_min_value[j]) {
          point_p.x = this.ll.x + (x_data[idx] - this.x_min_value[j]) /
            (this.x_max_value[j] - this.x_min_value[j]) *
            (this.ur.x - this.ll.x);
        }

        point_p.x = Math.max(this.ll.x, Math.min(point_p.x, this.ur.x));

      } else {

        if (i === 0) {
          if (this.x_max_value[j] !== this.x_min_value[j]) {
            point_p.x = this.ll.x + (x_data[idx] - this.x_min_value[j]) /
              (this.x_max_value[j] - this.x_min_value[j]) *
              (this.ur.x - this.ll.x);
          }

          point_p.x = Math.max(this.ll.x, Math.min(point_p.x, this.ur.x));
          point_p.y = this.ur.y;
          idx--;
        } else if (i === cpoints - 1) {
          if (this.x_max_value[j] !== this.x_min_value[j]) {
            point_p.x = this.ll.x + (x_data[idx - 1] - this.x_min_value[j]) /
              (this.x_max_value[j] - this.x_min_value[j]) *
              (this.ur.x - this.ll.x);
          }

          point_p.x = Math.max(this.ll.x, Math.min(point_p.x, this.ur.x));
          point_p.y = this.ur.y;
        } else {
          if (this.y_max_value[j] !== this.y_min_value[j]) {
            point_p.y = this.ur.y - (y_data[idx] - this.y_min_value[j]) /
              (this.y_max_value[j] - this.y_min_value[j]) *
              (this.ur.y - this.ll.y);
          }

          point_p.y = Math.max(this.ll.y, Math.min(point_p.y, this.ur.y));

          if (this.x_max_value[j] !== this.x_min_value[j]) {
            point_p.x = this.ll.x + (x_data[idx] - this.x_min_value[j]) /
              (this.x_max_value[j] - this.x_min_value[j]) *
              (this.ur.x - this.ll.x);
          }

          point_p.x = Math.max(this.ll.x, Math.min(point_p.x, this.ur.x));
        }
      }
    }

    if (this.curve_drawtype[j] === Glow.eDrawType_Inherit) {
      dt = this.draw_type;
    } else {
      dt = this.curve_drawtype[j];
    }

    if (this.curve_fill_drawtype[j] === Glow.eDrawType_Inherit) {
      dt_fill = this.draw_type;
    } else {
      dt_fill = this.curve_fill_drawtype[j];
    }

    this.ctx.nodraw++;
    this.curve[j] = new GrowPolyline(ctx);
    this.curve[j].init("", pointarray, cpoints, dt, this.curve_width, 0,
      this.fill_curve, 1, 0, dt_fill);
    this.ctx.nodraw--;
    this.draw();

  };
}

GrowXYCurve.prototype = Object.create(GrowTrend.prototype);

function GrowMenu(ctx) {
  GrowRect.call(this, ctx);

  this.info;			//!< Button text and types.
  this.text_size;		//!< Button text size.
  this.text_drawtype;		//!< Drawtype  for button text.
  this.text_color;		//!< Color for button text.
  this.text_color_disabled;	//!< Color for disabled button text.
  this.item_cnt;		//!< Number of menu items.
  this.item_height;		//!< Menu item height in pixel.
  this.current_item = -1;     	//!< Hot item.
  this.current_idx = -1;        //!< Hot index.
  this.new_item = -1;		//!< New current item.
  this.old_item = -1;		//!< Current item in previous event.
  this.parent_menu = null;	//!< Parent menu.
  this.min_width;		//!< Minimum width of menu.
  this.input_focus;		//!< This menu has input focus.
  this.font;			//!< Text font.
  this.hot;

  this.init =
    function (n_name, menu_info, x, y, min_w, draw_type, line_width, fill,
              border, fill_drawtype, text_size, text_drawtype, text_color,
              text_color_disabled, text_font) {
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

      this.tdraw(null, 0, 0, null, null);
    };

  this.type = function () {
    return Glow.eObjectType_GrowMenu;
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }
    var idx;
    var text_idx = Math.floor(this.trf.vertical_scale(t) *
      this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
      (this.text_size + 4) - 4);
    var tsize = this.trf.vertical_scale(t) * this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (8 + 2 * this.text_size);
    text_idx = Math.min(text_idx, Glow.DRAW_TYPE_SIZE - 1);
    text_idx = Math.max(0, text_idx);

    idx = Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
      this.line_width - 1);
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var ll_x, ll_y, ur_x, ur_y;

    var z_width, z_descent;
    var z_height = 0;
    var max_z_width = 0;
    var tot_z_height = 0;
    var i;
    var pulldown_found = 0;
    this.item_cnt = 0;
    for (i = 0; i < this.info.item.length; i++) {
      if (this.info.item[i].occupied) {
        var dim = this.ctx.gdraw.getTextExtent(this.info.item[i].text,
          Math.max(0, text_idx), this.font, this.text_drawtype);
        z_width = dim.width;
        z_height = dim.height;
        z_descent = z_height / 4;
        if (z_width > max_z_width) {
          max_z_width = z_width;
        }
        tot_z_height += Math.floor(1.6 * z_height);
        if (this.info.item[i].type === Glow.eMenuItem_PulldownMenu) {
          pulldown_found = 1;
        }
        this.item_cnt++;
      }
    }
    if (this.item_cnt === 0) {
      return;
    }

    var arrow_size = z_height * 4 / 5;
    ll_x =
      Math.floor(this.ll.x * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    ll_y =
      Math.floor(this.ll.y * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    ur_x = ll_x + max_z_width + 15 + Math.floor(pulldown_found * arrow_size);
    if (ur_x - ll_x < Math.floor(this.min_width * this.ctx.mw.zoom_factor_x)) {
      ur_x = ll_x + Math.floor(this.min_width * this.ctx.mw.zoom_factor_x);
    }
    ur_y = ll_y + Math.floor(tot_z_height);

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
      this.ctx.gdraw.fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
        this.fill_drawtype);
    }
    this.item_height = tot_z_height / this.item_cnt;
    var x_text = ll_x + 3;
    var y_text;
    var drawtype;

    var item_idx = 0;
    for (i = 0; i < this.info.item.length; i++) {
      if (this.info.item[i].occupied) {
        y_text = ll_y + Math.floor(this.item_height * item_idx + z_height + 1);
        if (i === this.current_item) {
          if (this.fill_drawtype % 10 < 7) {
            drawtype = GlowColor.shift_drawtype(this.fill_drawtype, 2, null);
          } else {
            drawtype = GlowColor.shift_drawtype(this.fill_drawtype, -2, null);
          }
          this.ctx.gdraw.fill_rect(ll_x,
            Math.floor(ll_y + item_idx * this.item_height), ur_x - ll_x,
            Math.floor(this.item_height), drawtype);
        }
        if (this.info.item[i].type === Glow.eMenuItem_ButtonDisabled) {
          this.ctx.gdraw.text(x_text, y_text, this.info.item[i].text,
            this.text_drawtype, this.text_color_disabled, text_idx, highlight,
            0, this.font, tsize, 0);
        } else {
          this.ctx.gdraw.text(x_text, y_text, this.info.item[i].text,
            this.text_drawtype, this.text_color, text_idx, highlight, 0,
            this.font, tsize, 0);
        }
        if (this.info.item[i].type === Glow.eMenuItem_PulldownMenu) {
          // Draw arrow
          var p = new Array(4);
          for (var j = 0; j < 4; j++) {
            p[j] = new GlowPointX();
          }
          p[0].x = ur_x - arrow_size - 2;
          p[0].y =
            Math.floor(ll_y + item_idx * this.item_height + this.item_height /
              2 - arrow_size / 2);
          p[1].x = ur_x - 2;
          p[1].y =
            Math.floor(ll_y + item_idx * this.item_height + this.item_height /
              2);
          p[2].x = ur_x - arrow_size - 2;
          p[2].y =
            Math.floor(ll_y + item_idx * this.item_height + this.item_height /
              2 + arrow_size / 2);
          p[3].x = ur_x - arrow_size - 2;
          p[3].y =
            Math.floor(ll_y + item_idx * this.item_height + this.item_height /
              2 - arrow_size / 2);

          this.ctx.gdraw.fill_polyline(p, 4, Glow.eDrawType_MediumGray, 0);
        }
        item_idx++;
      }
    }
    if (this.border !== 0) {
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.draw_type,
        idx, 0);
    }
  };

  this.local_event_handler = function (event, x, y) {
    var ll_x, ur_x, ll_y, ur_y;

    ll_x = Math.min(this.ll.x, this.ur.x);
    ur_x = Math.max(this.ll.x, this.ur.x);
    ll_y = Math.min(this.ll.y, this.ur.y);
    ur_y = Math.max(this.ll.y, this.ur.y);

    if (ll_x <= x && x <= ur_x && ll_y <= y && y <= ur_y) {
      var item;
      var vscale = this.trf.vertical_scale(null);

      item = Math.floor((y - this.ll.y) /
        (this.item_height / vscale / this.ctx.mw.zoom_factor_y));
      if (item > this.item_cnt - 1) {
        item = this.item_cnt - 1;
      }
      if (item < 0) {
        item = 0;
      }
      if (item === this.current_idx) {
        this.new_item = 0;
        this.old_item = this.current_item;
      } else {
        this.new_item = 1;
        this.old_item = this.current_item;
        this.current_idx = item;
        for (var i = 0, item_idx = 0; i < 32; i++) {
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
  };

  this.event_handler = function (event, fx, fy) {
    var sts;
    var csts;

    var rx, ry;

    // Convert coordinates to local coordinates
    var rp = this.trf.reverse(fx, fy);

    sts = 0;
    switch (event.event) {
      case Glow.eEvent_CursorMotion: {
        var redraw = 0;
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
          this.ctx.draw(this.x_left * this.ctx.mw.zoom_factor_x -
            this.ctx.mw.offset_x - Glow.DRAW_MP, this.y_low *
            this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y -
            Glow.DRAW_MP, this.x_right * this.ctx.mw.zoom_factor_x -
            this.ctx.mw.offset_x + Glow.DRAW_MP, this.y_high *
            this.ctx.mw.zoom_factor_y - this.ctx.mw.offset_y + Glow.DRAW_MP);
        }
        if (this.old_item !== this.current_item && this.old_item !== -1) {
          if (this.info.item[this.old_item].type ===
            Glow.eMenuItem_PulldownMenu) {
            // Delete all child menu objects.
            // TODO ctx->delete_menu_child( this);
          }
        }
        if (this.hot !== 0 && this.new_item !== 0) {
          console.log("GrowMenu new item,send MenuCreate");
          if (this.info.item[this.current_item].type ===
            Glow.eMenuItem_PulldownMenu) {
            this.ctx.send_menu_callback(this, this.current_item,
              Glow.eEvent_MenuCreate, this.ur.x, this.ll.y + this.item_height /
              this.ctx.mw.zoom_factor_y * this.current_item);
          }
        }
        break;
      }
      case Glow.eEvent_MB1Click: {
        sts = this.local_event_handler(event, rp.x, rp.y);
        if (sts !== 0 && this.current_item !== -1) {
          if (this.info.item[this.current_item].type === Glow.eMenuItem_Button) {
            csts = this.ctx.send_menu_callback(this, this.current_item,
              Glow.eEvent_MenuActivated, fx, fy);
            if (csts === Glow.GLOW__TERMINATED) {
              return csts;
            }

            // Send delete callback for the root menu of this node
            var e;
            for (e = this; e.parent_menu != null; e = e.parent_menu) {
            }
            this.ctx.send_menu_callback(e, 0, Glow.eEvent_MenuDelete, 0, 0);
            return Glow.GLOW__NO_PROPAGATE;
          }
        }
      }
      default:
        sts = this.local_event_handler(event, rp.x, rp.y);
    }
    if (sts !== 0) {
      this.ctx.register_callback_object(Glow.eObjectType_Node, this);
    }

    return sts;
  };
}

GrowMenu.prototype = Object.create(GrowRect.prototype);


function GrowScrollBar(ctx) {
  GrowRect.call(this, ctx);

  this.max_value;
  this.min_value;
  this.bar_value;
  this.bar_length;
  this.user_data = null;
  this.direction;
  this.movement_active = 0;
  this.start_pos;
  this.start_value;
  this.parent;
  //    void			(*value_changed_cb)( void *, double); //!< Callback when value is changed
  this.bar_color;
  this.hot;
  this.highlight;

  this.init =
    function (name, x, y, w, h, dir, border_d_type, line_width, display_lev,
              fill_d_type, bar_d_type, nodraw, parent) {
      this.ll.x = x;
      this.ll.y = y;
      this.ur.x = x + w;
      this.ur.y = y + h;
      this.draw_type = border_d_type;
      this.line_width = line_width;
      this.fill = 1;
      this.border = 1;
      this.shadow = 0;
      this.fill_drawtype = fill_d_type;
      this.n_name = name;
      this.display_level = display_lev;
      this.bar_color = bar_d_type;
      this.direction = dir;
      this.parent = parent;
      if (nodraw !== 0) {
        this.draw(null, this.highlight, this.hot, null, null);
      }
    };


  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var idx;

    idx = Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
      this.line_width - 1);
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;
    var x0 = 0, y0 = 0, width = 0, height = 0;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = (this.trf.x(this.ur.x, this.ur.y) * this.ctx.mw.zoom_factor_x) -
        this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    var fdrawtype = GlowColor.get_drawtype(this.fill_drawtype,
      Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);
    var bdrawtype = GlowColor.get_drawtype(this.draw_type,
      Glow.eDrawType_LineHighlight, highlight, colornode, 0, 0);
    var shift_drawtype;

    this.ctx.gdraw.fill_rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, fdrawtype);
    if (this.shadow !== 0) {
      shift_drawtype = GlowColor.shift_drawtype(this.fill_drawtype, 2, null); // Dark
      this.ctx.gdraw.line(ll_x + 1, ll_y + 1, ll_x + 1, ur_y - 1,
        shift_drawtype, 0, 0);
      this.ctx.gdraw.line(ll_x + 1, ll_y + 1, ur_x - 1, ll_y + 1,
        shift_drawtype, 0, 0);
      shift_drawtype = GlowColor.shift_drawtype(this.fill_drawtype, -2, null); // Light
      this.ctx.gdraw.line(ll_x + 1, ur_y - 1, ur_x - 1, ur_y - 1,
        shift_drawtype, 0, 0);
      this.ctx.gdraw.line(ur_x - 1, ll_y + 1, ur_x - 1, ur_y - 1,
        shift_drawtype, 0, 0);
    }

    if (this.max_value !== this.min_value) {

      switch (this.direction) {
        case Glow.eDir_Vertical:
          height =
            Math.floor(this.bar_length / (this.max_value - this.min_value) *
              (ur_y - ll_y));
          height = Math.max(0, Math.min(height, ur_y - ll_y));
          width = ur_x - ll_x;
          x0 = ll_x;
          y0 = ll_y + Math.floor((this.bar_value - this.min_value) /
            (this.max_value - this.min_value) * (ur_y - ll_y));
          break;
        case Glow.eDir_Horizontal:
          width =
            Math.floor(this.bar_length / (this.max_value - this.min_value) *
              (ur_x - ll_x));
          width = Math.max(0, Math.min(width, ur_x - ll_x));
          height = ur_y - ll_y;
          x0 = ll_x + Math.floor((this.bar_value - this.min_value) /
            (this.max_value - this.min_value) * (ur_x - ll_x));
          y0 = ll_y;
          break;
      }
      this.ctx.gdraw.fill_rect(x0, y0, width, height, this.bar_color);
      if (this.shadow !== 0) {
        shift_drawtype = GlowColor.shift_drawtype(this.bar_color, -2, null); // Light
        this.ctx.gdraw.line(x0 + 1, y0 + 1, x0 + 1, y0 + height - 1,
          shift_drawtype, 0, 0);
        this.ctx.gdraw.line(x0 + 1, y0 + 1, x0 + width - 1, y0 + 1,
          shift_drawtype, 0, 0);
        shift_drawtype = GlowColor.shift_drawtype(this.bar_color, 2, null); // Dark
        this.ctx.gdraw.line(x0 + 1, y0 + height - 1, x0 + width - 1, y0 +
          height - 1, shift_drawtype, 0, 0);
        this.ctx.gdraw.line(x0 + width - 1, y0 + 1, x0 + width - 1, y0 +
          height - 1, shift_drawtype, 0, 0);
      }
      this.ctx.gdraw.rect(x0, y0, width, height, bdrawtype, idx, 0);
    }

    this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, bdrawtype, idx,
      0);
  };

  this.set_value = function (value, length) {
    console.log("Value", value);
    this.bar_value = value;
    if (length !== 0) {
      this.bar_length = length;
    }

    if (this.bar_value < this.min_value) {
      this.bar_value = this.min_value;
    }
    if (this.bar_value > this.max_value) {
      this.bar_value = this.max_value;
    }

    this.draw();
    return this.bar_value;
  };

  this.event_handler = function (event, fx, fy) {
    var ll_x, ur_x, ll_y, ur_y;

    ll_x = Math.min(this.ll.x, this.ur.x);
    ur_x = Math.max(this.ll.x, this.ur.x);
    ll_y = Math.min(this.ll.y, this.ur.y);
    ur_y = Math.max(this.ll.y, this.ur.y);

    if (this.movement_active === 0 &&
      !(ll_x <= fx && fx <= ur_x && ll_y <= fy && fy <= ur_y)) {
      return 0;
    }

    if (event.event === Glow.eEvent_ButtonMotion && this.movement_active === 0) {
      return 0;
    }

    if (this.min_value === this.max_value) {
      return 1;
    }

    switch (event.event) {
      case Glow.eEvent_MB1Down:
        switch (this.direction) {
          case Glow.eDir_Vertical:
            if (ll_y + (this.bar_value - this.min_value) /
              (this.max_value - this.min_value) * (ur_y - ll_y) <= fy && fy <=
              ll_y + (this.bar_value - this.min_value + this.bar_length) /
              (this.max_value - this.min_value) * (ur_y - ll_y)) {
              this.start_pos = fy;
              this.start_value = this.bar_value;
              this.movement_active = 1;
              this.ctx.setSliderActive(true);
            }
            break;
          case Glow.eDir_Horizontal:
            if (ll_x + (this.bar_value - this.min_value) /
              (this.max_value - this.min_value) * (ur_x - ll_x) <= fx && fx <=
              ll_x + (this.bar_value - this.min_value + this.bar_length) /
              (this.max_value - this.min_value) * (ur_x - ll_x)) {
              this.start_pos = fx;
              this.start_value = this.bar_value;
              this.movement_active = 1;
              this.ctx.setSliderActive(true);
            }
            break;
        }
        break;
      case Glow.eEvent_MB1Up:
        this.movement_active = 0;
        this.ctx.setSliderActive(false);
        break;
      case Glow.eEvent_ButtonMotion:
        if (this.movement_active !== 0) {
          if (this.bar_length >= this.max_value - this.min_value) {
            break;
          }
          switch (this.direction) {
            case Glow.eDir_Vertical:
              this.bar_value =
                this.start_value + (fy - this.start_pos) / (ur_y - ll_y) *
                (this.max_value - this.min_value);
              if (this.bar_value < this.min_value) {
                this.bar_value = this.min_value;
              }
              if (this.bar_value > this.max_value - this.bar_length) {
                this.bar_value = this.max_value - this.bar_length;
              }
              this.parent.verticalScrollValueChanged(this.bar_value);
              break;
            case Glow.eDir_Horizontal:
              this.bar_value =
                this.start_value + (fx - this.start_pos) / (ur_x - ll_x) *
                (this.max_value - this.min_value);
              if (this.bar_value < this.min_value) {
                this.bar_value = this.min_value;
              }
              if (this.bar_value > this.max_value - this.bar_length) {
                this.bar_value = this.max_value - this.bar_length;
              }
              this.parent.horizontalScrollValueChanged(this.bar_value);
              break;
          }
        }
        break;
      default:
    }
    return 1;
  };

  this.set_range = function (min, max) {
    this.max_value = max;
    this.min_value = min;
    this.draw();
  };

  this.set_shadow = function (shadow) {
    this.shadow = shadow;
  };

  this.set_position = function (x, y, width, height) {
    this.ll.posit(x, y);
    this.ur.posit(x + width, y + height);
  };
}

GrowScrollBar.prototype = Object.create(GrowRect.prototype);

function GrowWindow(ctx) {
  var self = this;
  GrowRect.call(this, ctx);
  this.growrect_open = this.open;
  this.growrect_event_handler = this.event_handler;

  this.file_name = "";
  this.input_file_name = "";
  this.scrollbar_width;
  this.scrollbar_color;
  this.scrollbar_bg_color;
  this.vertical_scrollbar;
  this.horizontal_scrollbar;
  this.window_scale;
  this.owner = "";
  this.userdata = null;
  this.windowCtx = null;
  this.wctx_x0;
  this.wctx_x1;
  this.wctx_y0;
  this.wctx_y1;
  this.y_low_offs = 0;
  this.h_value = 0;
  this.v_value = 0;
  this.v_scrollbar = null;
  this.h_scrollbar = null;

  this.init =
    function (n_name, x, y, w, h, draw_type, line_width, fill, border, shadow,
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
    };

  this.type = function () {
    return Glow.eObjectType_GrowWindow;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowWindow : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowWindow:
          break;
        case Glow.eSave_GrowWindow_file_name:
          if (tokens.length > 1) {
            this.file_name = tokens[1];
          }
          break;
        case Glow.eSave_GrowWindow_scrollbar_width:
          this.scrollbar_width = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowWindow_scrollbar_color:
          this.scrollbar_color = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowWindow_scrollbar_bg_color:
          this.scrollbar_bg_color = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowWindow_vertical_scrollbar:
          this.vertical_scrollbar = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowWindow_horizontal_scrollbar:
          this.horizontal_scrollbar = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowWindow_window_scale:
          this.window_scale = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowWindow_owner:
          if (tokens.length > 1) {
            this.owner = tokens[1];
          }
          break;
        case Glow.eSave_GrowWindow_rect_part:
          i = this.growrect_open(lines, i + 1);
          break;
        case Glow.eSave_GrowWindow_userdata_cb:
          if (this.ctx.appl != null) {
            var ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowWindow");
          break;
      }
      if (end) {
        break;
      }
    }

    if (this.file_name !== null && this.file_name !== "") {
      this.new_ctx();
    }

    return i;
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var idx;
    var drawtype;

    idx = Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
      this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var ll_x, ll_y, ur_x, ur_y;
    var dx1, dy1, dx2, dy2;

    if (t == null) {
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
        this.v_scrollbar.set_position(dx2 - scrollbar_width, dy1 +
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

    if (this.windowCtx != null) {
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
      GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
        highlight, colornode, 0, 0);
    this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, idx, 0);
  };


  this.new_ctx = function () {
    if (this.file_name === "_no_" || this.file_name === "") {
      return;
    }

    var idx;
    if ((idx = this.file_name.indexOf('/')) !== -1) {
      this.file_name = this.file_name.substring(idx + 1);
    }

    if (this.file_name.indexOf('.') === -1) {
      this.file_name = this.file_name + ".pwg";
    }

    var windOwner = this.owner;
    for (var i = 0; i < 4; i++) {
      var idx;
      if ((idx = windOwner.indexOf("$object")) === -1) {
        break;
      } else {
        var oname = this.ctx.getOwner();
        windOwner =
          windOwner.substring(0, idx) + oname + windOwner.substring(idx + 7);
      }
    }
    this.owner = windOwner;

    this.windowCtx = this.ctx.appl.loadCtx(this.file_name, this.read_cb);
    this.windowCtx.appl = this.ctx.appl;
  };

  this.read_cb = function (lines, row) {
    var ctx_popped = self.ctx.appl.ctxPop(self.ctx);

    self.windowCtx.gdraw = self.ctx.gdraw;
    self.ctx.appl.frame.read_func(self.windowCtx, lines, row);

    self.windowCtx.owner = self.owner;
    self.windowCtx.mw.subwindow_scale = self.window_scale;
    self.windowCtx.mw.zoom_factor_x = self.windowCtx.mw.zoom_factor_y =
      self.windowCtx.mw.subwindow_scale * self.ctx.mw.zoom_factor_x;

    self.input_file_name = self.file_name;
    if (self.windowCtx.background_color !== Glow.eDrawType_Inherit) {
      self.fill_drawtype =
        self.original_fill_drawtype = self.windowCtx.background_color;
      self.fill = 1;
    }
    if (self.windowCtx.x0 !== self.windowCtx.x1 &&
      self.windowCtx.y0 !== self.windowCtx.y1) {
      self.wctx_x0 = self.windowCtx.x0;
      self.wctx_x1 = self.windowCtx.x1;
      self.wctx_y0 = self.windowCtx.y0;
      self.wctx_y1 = self.windowCtx.y1;
    } else {
      self.wctx_x0 = self.windowCtx.x_left;
      self.wctx_x1 = self.windowCtx.x_right;
      self.wctx_y0 = self.windowCtx.y_low;
      self.wctx_y1 = self.windowCtx.y_high;
    }
    self.windowCtx.mw.window_width =
      Math.floor((self.x_right - self.x_left) * self.ctx.mw.zoom_factor_x);
    self.windowCtx.mw.window_height =
      Math.floor((self.y_high - self.y_low) * self.ctx.mw.zoom_factor_y);
    self.windowCtx.mw.subwindow_x =
      Math.floor(self.x_left * self.ctx.mw.zoom_factor_x -
        self.ctx.mw.offset_x);
    self.windowCtx.mw.subwindow_y =
      Math.floor(self.y_low * self.ctx.mw.zoom_factor_y - self.ctx.mw.offset_y);
    self.windowCtx.mw.subwindow_scale =
      self.ctx.mw.subwindow_scale * self.window_scale;
    self.windowCtx.mw.zoom_factor_x = self.windowCtx.mw.zoom_factor_y =
      self.ctx.mw.zoom_factor_x * self.windowCtx.mw.subwindow_scale;

    self.configureScrollbars();
    if (self.windowCtx.customcolors !== null) {
      self.ctx.gdraw.pop_customcolors();
    }

    self.windowCtx.traceConnect();
    self.draw();

    if (ctx_popped) {
      self.ctx.appl.ctxPush();
    }
  };

  this.event_handler = function (event, fx, fy) {
    var sts, v_sts, h_sts;
    var rp;


    /*
      switch ( event.event) {
      case Glow.eEvent_Key_Right:
      case Glow.eEvent_Key_Left:
      case Glow.eEvent_Key_Up:
      case Glow.eEvent_Key_Down:
      case Glow.eEvent_Key_BackSpace:
      case Glow.eEvent_Key_Return:
      case Glow.eEvent_Key_Tab:
      case Glow.eEvent_Key_ShiftTab:
      case Glow.eEvent_Key_Escape:
      case Glow.eEvent_Key_Ascii:
      if ( input_focus) {
      this.windowCtx.eventHandler( event, fx, 0);
      if ( !this.windowCtx.inputfocus_object) {
      this.ctx.register_inputfocus( this, 0);
      input_focus = 0;
      }
      return 1;
      }
      else
      return 0;
      case Glow.eEvent_ScrollUp:
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
      }
      else
      return 0;
      }
      break;
      case Glow.eEvent_ScrollDown:
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
      }
      else
      return 0;
      }
      break;
      default:
      ;
      }

    */

    v_sts = h_sts = 0;
    if (this.v_scrollbar != null) {
      v_sts = this.v_scrollbar.event_handler(event, fx, fy);
    }
    if (this.h_scrollbar != null) {
      h_sts = h_scrollbar.event_handler(event, fx, fy);
    }
    if (v_sts !== 0 || h_sts !== 0) {
      return 1;
    }

    sts = this.growrect_event_handler(event, fx, fy);

    if (this.ctx.callback_object === this) {
      // Disable event callback for this object, let the window ctx handle it
      this.ctx.callback_object = null;
      thix.ctx.callback_object_type = Glow.eObjectType_NoObject;
    }

    if (this.windowCtx !== null && sts !== 0) {

      var ur_x = Math.floor((this.x_right - this.vertical_scrollbar *
        this.scrollbar_width) * this.ctx.mw.zoom_factor_x) -
        this.ctx.mw.offset_x;
      var ll_x = Math.floor(this.x_left * this.ctx.mw.zoom_factor_x) -
        this.ctx.mw.offset_x;
      var ur_y = Math.floor((this.y_high - this.horizontal_scrollbar *
        this.scrollbar_width) * this.ctx.mw.zoom_factor_y) -
        this.ctx.mw.offset_y;
      var ll_y = Math.floor((this.y_low + this.y_low_offs) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

      // window_ctx->draw_buffer_only = ctx->draw_buffer_only;
      this.ctx.gdraw.set_clip_rectangle(ll_x, ll_y, ur_x, ur_y);

      // Set callback to redraw the background
      // this.windowCtx.redraw_callback = redraw_cb;
      // this.windowCtx.redraw_data = (Object)this;

      var e = new GlowEvent();
      e.event = event.event;
      e.type = event.type;

      e.x = (event.x - this.x_left) / this.windowCtx.mw.subwindow_scale +
        this.h_value / this.windowCtx.mw.subwindow_scale;
      e.y = (event.y - this.y_low) / this.windowCtx.mw.subwindow_scale +
        this.v_value / this.windowCtx.mw.subwindow_scale;
      e.object = event.object;
      e.object_type = event.object_type;
      sts = this.windowCtx.event_handler(e, fx, fy);
      if (sts === Glow.GLOW__TERMINATED) {
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
  };

  this.configureScrollbars = function () {
    var x0, y0, width, height;

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
        Glow.eDir_Vertical, Glow.eDrawType_Line, 1, this.display_level,
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
        Glow.eDir_Horizontal, Glow.eDrawType_Line, 1, this.display_level,
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

  };

  this.verticalScrollValueChanged = function (value) {
    if (this.v_value !== value) {
      this.v_value = value;
      this.draw();
    }
  };

  this.horizontalScrollValueChanged = function (value) {
    if (this.h_value !== value) {
      this.h_value = value;
      this.draw();
    }
  };

  this.update_attributes = function () {
    var ctx_popped = this.ctx.appl.ctxPop(self.ctx);

    if (this.owner === null) {
      this.owner = "";
    }
    if (this.input_file_name !== this.file_name || this.windowCtx == null ||
      this.windowCtx.owner !== this.owner) {
      // New graph, create new context
      var ur_x = Math.floor(this.x_right * this.ctx.mw.zoom_factor_x) -
        this.ctx.mw.offset_x;
      var ll_x = Math.floor(this.x_left * this.ctx.mw.zoom_factor_x) -
        this.ctx.mw.offset_x;
      var ur_y = Math.floor(this.y_high * this.ctx.mw.zoom_factor_y) -
        this.ctx.mw.offset_y;
      var ll_y = Math.floor(this.y_low * this.ctx.mw.zoom_factor_y) -
        this.ctx.mw.offset_y;
      this.ctx.gdraw.set_clip_rectangle(ll_x, ll_y, ur_x - 1, ur_y - 1);

      if (this.windowCtx !== null) {
        if (this.windowCtx.traceStarted()) {
          this.windowCtx.traceDisconnect();
        }
        this.fill_drawtype =
          this.original_fill_drawtype = Glow.eDrawType_Inherit;
        this.fill = 0;
      }
      this.file_name = this.input_file_name;
      this.new_ctx();

      this.ctx.gdraw.reset_clip_rectangle();
    }
    if (ctx_popped) {
      this.ctx.appl.ctxPush();
    }
  };

  this.traceConnect = function () {
    if (this.windowCtx === null) {
      return;
    }
    this.windowCtx.traceConnect();
  };
  this.traceDisconnect = function () {
    if (this.windowCtx === null) {
      return;
    }
    this.windowCtx.traceDisconnect();
  };
  this.traceScan = function () {
    if (this.windowCtx === null) {
      return;
    }
    this.windowCtx.traceScan();
  };

  this.setSource = function (file_name, owner) {

    this.input_file_name = file_name;
    this.owner = owner;
    this.update_attributes();
  };

  this.get_background_object_limits = function (t, type, x, y, b) {
    console.log("GrowWindow.get_background_object_limits");
    var sts = 0;

    for (var i = 0; i < this.windowCtx.a.size(); i++) {
      console.log("background_object_limits", this.windowCtx.a.get(i).type());
      sts = this.windowCtx.a.get(i)
        .get_background_object_limits(null, type, x, y, b);
      if ((sts & 1) !== 0) {
        break;
      }
    }
    return sts;
  };
}

GrowWindow.prototype = Object.create(GrowRect.prototype);

function GrowFolder(ctx) {
  var self = this;
  GrowWindow.call(this, ctx);
  this.growwindow_open = this.open;
  this.growwindow_tdraw = this.tdraw;
  this.growwindow_event_handler = this.event_handler;

  this.MAX_FOLDERS = 12;
  this.folders;
  this.text_size;
  this.text_drawtype;
  this.text_color_drawtype;
  this.header_height;
  this.folder_file_names = new Array(this.MAX_FOLDERS);
  this.folder_text = new Array(this.MAX_FOLDERS);
  this.folder_scale = new Array(this.MAX_FOLDERS);
  this.folder_v_scrollbar = new Array(this.MAX_FOLDERS);
  this.folder_h_scrollbar = new Array(this.MAX_FOLDERS);
  this.folder_owner = new Array(this.MAX_FOLDERS);
  this.current_folder = 0;
  this.color_selected;
  this.color_unselected;
  for (var i = 0; i < this.MAX_FOLDERS; i++) {
    this.folder_text[i] = null;
  }

  this.type = function () {
    return Glow.eObjectType_GrowFolder;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowFolder : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowFolder:
          break;
        case Glow.eSave_GrowFolder_folders:
          this.folders = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_text_drawtype:
          this.text_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_text_color_drawtype:
          this.text_color_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_header_height:
          this.header_height = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_color_selected:
          this.color_selected = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_color_unselected:
          this.color_unselected = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names1:
          if (tokens.length > 1) {
            this.folder_file_names[0] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text1:
          if (tokens.length > 1) {
            this.folder_text[0] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale1:
          this.folder_scale[0] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar1:
          this.folder_v_scrollbar[0] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar1:
          this.folder_h_scrollbar[0] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names2:
          if (tokens.length > 1) {
            this.folder_file_names[1] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text2:
          if (tokens.length > 1) {
            this.folder_text[1] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale2:
          this.folder_scale[1] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar2:
          this.folder_v_scrollbar[1] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar2:
          this.folder_h_scrollbar[1] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names3:
          if (tokens.length > 1) {
            this.folder_file_names[2] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text3:
          if (tokens.length > 1) {
            this.folder_text[2] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale3:
          this.folder_scale[2] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar3:
          this.folder_v_scrollbar[2] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar3:
          this.folder_h_scrollbar[2] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names4:
          if (tokens.length > 1) {
            this.folder_file_names[3] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text4:
          if (tokens.length > 1) {
            this.folder_text[3] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale4:
          this.folder_scale[3] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar4:
          this.folder_v_scrollbar[3] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar4:
          this.folder_h_scrollbar[3] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names5:
          if (tokens.length > 1) {
            this.folder_file_names[4] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text5:
          if (tokens.length > 1) {
            this.folder_text[4] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale5:
          this.folder_scale[4] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar5:
          this.folder_v_scrollbar[4] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar5:
          this.folder_h_scrollbar[4] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names6:
          if (tokens.length > 1) {
            this.folder_file_names[5] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text6:
          if (tokens.length > 1) {
            this.folder_text[5] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale6:
          this.folder_scale[5] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar6:
          this.folder_v_scrollbar[5] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar6:
          this.folder_h_scrollbar[5] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names7:
          if (tokens.length > 1) {
            this.folder_file_names[6] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text7:
          if (tokens.length > 1) {
            this.folder_text[6] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale7:
          this.folder_scale[6] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar7:
          this.folder_v_scrollbar[6] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar7:
          this.folder_h_scrollbar[6] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names8:
          if (tokens.length > 1) {
            this.folder_file_names[7] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text8:
          if (tokens.length > 1) {
            this.folder_text[7] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale8:
          this.folder_scale[7] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar8:
          this.folder_v_scrollbar[7] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar8:
          this.folder_h_scrollbar[7] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names9:
          if (tokens.length > 1) {
            this.folder_file_names[8] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text9:
          if (tokens.length > 1) {
            this.folder_text[8] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale9:
          this.folder_scale[8] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar9:
          this.folder_v_scrollbar[8] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar9:
          this.folder_h_scrollbar[8] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names10:
          if (tokens.length > 1) {
            this.folder_file_names[9] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text10:
          if (tokens.length > 1) {
            this.folder_text[9] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale10:
          this.folder_scale[9] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar10:
          this.folder_v_scrollbar[9] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar10:
          this.folder_h_scrollbar[9] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names11:
          if (tokens.length > 1) {
            this.folder_file_names[10] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text11:
          if (tokens.length > 1) {
            this.folder_text[10] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale11:
          this.folder_scale[10] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar11:
          this.folder_v_scrollbar[10] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar11:
          this.folder_h_scrollbar[10] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_file_names12:
          if (tokens.length > 1) {
            this.folder_file_names[11] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_text12:
          if (tokens.length > 1) {
            this.folder_text[11] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_scale12:
          this.folder_scale[11] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowFolder_folder_v_scrollbar12:
          this.folder_v_scrollbar[11] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_h_scrollbar12:
          this.folder_h_scrollbar[11] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowFolder_folder_owner1:
          if (tokens.length > 1) {
            this.folder_owner[0] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_owner2:
          if (tokens.length > 1) {
            this.folder_owner[1] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_owner3:
          if (tokens.length > 1) {
            this.folder_owner[2] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_owner4:
          if (tokens.length > 1) {
            this.folder_owner[3] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_owner5:
          if (tokens.length > 1) {
            this.folder_owner[4] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_owner6:
          if (tokens.length > 1) {
            this.folder_owner[5] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_owner7:
          if (tokens.length > 1) {
            this.folder_owner[6] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_owner8:
          if (tokens.length > 1) {
            this.folder_owner[7] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_owner9:
          if (tokens.length > 1) {
            this.folder_owner[8] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_owner10:
          if (tokens.length > 1) {
            this.folder_owner[9] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_owner11:
          if (tokens.length > 1) {
            this.folder_owner[10] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_folder_owner12:
          if (tokens.length > 1) {
            this.folder_owner[11] = tokens[1];
          }
          break;
        case Glow.eSave_GrowFolder_window_part:
          i = this.growwindow_open(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowFolder");
          break;
      }
      if (end) {
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
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var idx;
    var drawtype;

    idx = Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
      this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);

    var text_idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 4);
    var tsize = this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
      (8 + 2 * this.text_size);
    text_idx = Math.min(text_idx, Glow.DRAW_TYPE_SIZE - 1);

    var ll_x, ll_y, ur_x, ur_y;
    var dx1, dy1, dx2, dy2;

    if (t == null) {
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

    ll_x = Math.floor(dx1 * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    ur_x = Math.floor(dx2 * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    ur_y = Math.floor((dy1 + this.y_low_offs) * this.ctx.mw.zoom_factor_y) -
      this.ctx.mw.offset_y;
    ll_y = Math.floor(dy1 * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    drawtype =
      GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
        highlight, colornode, 0, 0);
    var drawtype_light = GlowColor.shift_drawtype(this.color_unselected, -2,
      colornode);
    var drawtype_dark = GlowColor.shift_drawtype(this.color_unselected, 2,
      colornode);
    var x;
    var width = (ur_x - ll_x) / this.folders;
    var h = ur_y - ll_y;
    var p = new Array(4);
    for (var j = 0; j < 4; j++) {
      p[j] = new GlowPointX();
    }
    for (var i = this.folders - 1; i >= -1; i--) {
      if (i === this.current_folder)
      // Draw this last
      {
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
        this.ctx.gdraw.fill_polyline(p, 4, this.color_selected, 0);
      } else {
        this.ctx.gdraw.fill_polyline(p, 4, this.color_unselected, 0);
        if (this.shadow !== 0) {
          this.ctx.gdraw.line(p[0].x + 1, p[0].y, p[1].x + 1, p[1].y,
            drawtype_light, 0, 0);
          if (i !== 0) {
            var ps = new Array(4);
            for (var j = 0; j < 4; j++) {
              ps[j] = new GlowPointX();
            }

            ps[0].x = x + h / 4;
            ps[0].y = ll_y + h;
            ps[1].x = x;
            ps[1].y = ll_y + h / 2;
            ps[2].x = x + h / 8;
            ps[2].y = ll_y + h / 4;
            ps[3].x = x + h / 2;
            ps[3].y = ll_y + h;

            this.ctx.gdraw.fill_polyline(ps, 4, drawtype_dark, 0);
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
      this.ctx.gdraw.polyline(p, 4, drawtype, idx, 0);

      if (text_idx >= 0 && this.folder_text[i] !== null) {
        this.ctx.gdraw.text(x + h / 2, ll_y + h - 2, this.folder_text[i],
          this.text_drawtype, this.text_color_drawtype, text_idx, highlight, 0,
          Glow.eFont_Helvetica, tsize, 0);
      }
      if (i === this.current_folder) {
        break;
      }
    }

    this.growwindow_tdraw(t, highlight, hot, node, colornode);
    this.ctx.gdraw.line(p[0].x + 1, p[0].y, p[3].x - 1, p[3].y,
      this.color_selected, idx, 0);

  };

  this.event_handler = function (event, fx, fy) {
    var sts;

    if (event.event === Glow.eEvent_MB1Click) {
      if (this.x_left <= fx && fx <= this.x_right && this.y_low <= fy &&
        fy <= this.y_low + this.header_height) {
        var w = (this.x_right - this.x_left) / this.folders;
        var x = this.x_left + w;
        for (var i = 0; i < this.folders; i++) {
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

    var e = new GlowEvent();
    e.event = event.event;
    e.type = event.type;

    e.x = event.x;
    e.y = event.y - this.header_height;
    e.object = event.object;
    e.object_type = event.object_type;
    sts = this.growwindow_event_handler(e, fx, fy);
    return sts;
  };
}

GrowFolder.prototype = Object.create(GrowWindow.prototype);

function Range(lines, longq, vvalq, hvalq, format) {
  this.lines = lines;
  this.longq = longq;
  this.vvalq = vvalq;
  this.hvalq = hvalq;
  this.format = format;
}

var RangeData = [[new Range(26, 5, 5, 10, "%3.1f"), // 1
  new Range(21, 5, 5, 10, "%3.1f"), // 2
  new Range(31, 5, 10, 10, "%3.1f"), // 3
  new Range(41, 5, 10, 20, "%3.1f"), // 4
  new Range(26, 5, 5, 10, "%1.0f"), // 5
  new Range(31, 5, 10, 10, "%1.0f"), // 6
  new Range(36, 5, 10, 10, "%1.0f"), // 7
  new Range(17, 2, 4, 4, "%1.0f"), // 8
  new Range(19, 2, 4, 4, "%1.0f"), // 9
  new Range(21, 2, 4, 8, "%2.0f"), // 10
  new Range(23, 2, 4, 8, "%2.0f"), // 11
  new Range(13, 4, 4, 4, "%2.0f"), // 12
  new Range(14, 4, 4, 4, "%2.0f"), // 13
  new Range(15, 4, 4, 4, "%2.0f"), // 14
  new Range(16, 5, 5, 5, "%2.0f"), // 15
  new Range(17, 5, 5, 5, "%2.0f"), // 16
  new Range(18, 5, 5, 5, "%2.0f"), // 17
  new Range(19, 5, 5, 5, "%2.0f"), // 18
  new Range(20, 5, 5, 5, "%2.0f"), // 19
  new Range(21, 5, 5, 5, "%2.0f"), // 20
  new Range(22, 5, 5, 5, "%2.0f"), // 21
  new Range(23, 5, 5, 5, "%2.0f"), // 22
  new Range(24, 5, 5, 5, "%2.0f"), // 23
  new Range(25, 5, 5, 5, "%2.0f"), // 24
  new Range(26, 5, 5, 10, "%2.0f")],// 25
  [new Range(101, 5, 10, 20, "%3.1f"), // 1
    new Range(101, 5, 10, 20, "%3.1f"), // 2
    new Range(61, 10, 10, 20, "%3.1f"), // 3
    new Range(81, 10, 10, 20, "%3.1f"), // 4
    new Range(101, 10, 20, 20, "%1.0f"), // 5
    new Range(61, 5, 10, 20, "%1.0f"), // 6
    new Range(71, 5, 10, 20, "%1.0f"), // 7
    new Range(81, 5, 10, 20, "%1.0f"), // 8
    new Range(91, 5, 10, 20, "%1.0f"), // 9
    new Range(101, 5, 10, 20, "%2.0f"), // 10
    new Range(56, 5, 5, 5, "%2.0f"), // 11
    new Range(61, 5, 10, 10, "%2.0f"), // 12
    new Range(66, 5, 10, 20, "%2.0f"), // 13
    new Range(71, 5, 10, 20, "%2.0f"), // 14
    new Range(76, 5, 10, 20, "%2.0f"), // 15
    new Range(81, 5, 10, 20, "%2.0f"), // 16
    new Range(86, 5, 10, 20, "%2.0f"), // 17
    new Range(91, 5, 10, 20, "%2.0f"), // 18
    new Range(96, 5, 10, 20, "%2.0f"), // 19
    new Range(101, 5, 10, 25, "%2.0f"), // 20
    new Range(43, 2, 6, 6, "%2.0f"), // 21
    new Range(45, 2, 4, 8, "%2.0f"), // 22
    new Range(47, 2, 4, 8, "%2.0f"), // 23
    new Range(49, 2, 4, 8, "%2.0f"), // 24
    new Range(26, 5, 5, 5, "%2.0f")]  // 25
];

function GrowAxis(ctx) {
  GrowRect.call(this, ctx);
  this.growrect_open = this.open;

  this.text_size;
  this.text_drawtype;
  this.text_color_drawtype;
  this.max_value;
  this.min_value;
  this.lines;
  this.linelength;
  this.longquotient;
  this.valuequotient;
  this.increment;
  this.format = null;
  this.userdata = null;
  this.cFormat = null;
  this.sb;

  this.type = function () {
    return Glow.eObjectType_GrowAxis;
  };

  this.configure = function () {
    if (this.lines <= 1) {
      this.lines = 2;
    }
    if (this.longquotient <= 0) {
      this.longquotient = 1;
    }
    if (this.valuequotient <= 0) {
      this.valuequotient = 1;
    }
    this.increment = (this.max_value - this.min_value) / (this.lines - 1);
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowAxis : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowAxis:
          break;
        case Glow.eSave_GrowAxis_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxis_text_drawtype:
          this.text_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxis_text_color_drawtype:
          this.text_color_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxis_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowAxis_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowAxis_lines:
          this.lines = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxis_longquotient:
          this.longquotient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxis_valuequotient:
          this.valuequotient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxis_format:
          if (tokens.length > 1) {
            this.format = tokens[1];
          }
          break;
        case Glow.eSave_GrowAxis_rect_part:
          i = this.growrect_open(lines, i + 1);
          break;
        case Glow.eSave_GrowAxis_userdata_cb:
          if (this.ctx.appl !== null) {
            var ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowAxis");
          break;
      }
      if (end) {
        break;
      }
    }

    this.configure();
    if (this.format !== null) {
      this.cFormat = new GlowCFormat(this.format);
    }

    return i;
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }
    var i;
    var draw_text = (Math.abs(this.increment) > Number.MIN_VALUE);
    var idx;
    var x, y;
    var text;
    var line_length;
    var x_text, y_text;
    var z_height = 0, z_width, z_descent = 0;
    var max_z_width = 0;
    var rotation;
    var drawtype;
    var text_idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 4);
    var tsize = this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
      (8 + 2 * this.text_size);
    text_idx = Math.min(text_idx, Glow.DRAW_TYPE_SIZE - 1);

    if ((node !== null && node.invisible !== 0) || this.invisible !== 0) {
      return;
    }

    if (node !== null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1);
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      rotation =
        (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) * 360;
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      rotation =
        (this.trf.rot(t) / 360 - Math.floor(this.trf.rot(t) / 360)) * 360;
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);
    drawtype =
      GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
        highlight, colornode, 0, 0);

    if (45 >= rotation || rotation > 315) {
      // Vertical line to the right and values to the left

      this.ctx.gdraw.line(ur_x, ll_y, ur_x, ur_y, drawtype, idx, 0);

      // Calculate max value text width
      if (draw_text) {
        for (i = 0; i < this.lines; i++) {
          if (i % this.valuequotient === 0) {
            text = this.format_text(this.format, this.max_value - i *
              this.increment);
            var d = this.ctx.gdraw.getTextExtent(text, Math.max(0, text_idx),
              Glow.eFont_Helvetica, this.text_drawtype);
            z_width = d.width;
            z_height = d.height;
            z_descent = z_height / 4;
            if (max_z_width < z_width) {
              max_z_width = z_width;
            }
          }
        }
        x_text = ll_x + max_z_width;
        line_length = ur_x - ll_x - max_z_width;
        if (line_length < 3) {
          line_length = 3;
        }
      } else {
        x_text = ll_x;
        line_length = ur_x - ll_x;
      }

      for (i = 0; i < this.lines; i++) {
        y = Math.floor(ll_y + (ur_y - ll_y) / (this.lines - 1) * i);
        if (i % this.longquotient === 0) {
          this.ctx.gdraw.line(ur_x - line_length, y, ur_x, y, drawtype, idx, 0);
        } else {
          this.ctx.gdraw.line(ur_x - Math.floor(2.0 / 3 * line_length), y, ur_x,
            y, drawtype, idx, 0);
        }
        if (draw_text) {
          text =
            this.format_text(this.format, this.max_value - i * this.increment);

          if (text_idx >= 0 && max_z_width < ur_x - ll_x &&
            i % this.valuequotient === 0) {
            if (i === this.lines - 1) {
              y_text = y;
            } else if (i === 0) {
              y_text = y + z_height - z_descent - 3;
            } else {
              y_text = y + (z_height - z_descent) / 2;
            }
            this.ctx.gdraw.text(ll_x, y_text, text, this.text_drawtype,
              this.text_color_drawtype, text_idx, highlight, 0,
              Glow.eFont_Helvetica, tsize, 0);
          }
        }
      }
    } else if (45 < rotation && rotation <= 135) {
      // Horizontal line at bottom and values to the top

      this.ctx.gdraw.line(ll_x, ur_y, ur_x, ur_y, drawtype, idx, 0);

      // Calculate max value text height
      if (draw_text) {
        var d2 = this.ctx.gdraw.getTextExtent("0", Math.max(0, text_idx),
          Glow.eFont_Helvetica, this.text_drawtype);

        z_width = d2.width;
        z_height = d2.height;
        z_descent = z_height / 4;
        line_length = ur_y - ll_y - z_height;
        if (line_length < 3) {
          line_length = 3;
        }
      } else {
        line_length = ur_y - ll_y;
      }

      for (i = 0; i < this.lines; i++) {
        x = Math.floor(ll_x + (ur_x - ll_x) / (this.lines - 1) *
          (this.lines - 1 - i));
        if (i % this.longquotient === 0) {
          this.ctx.gdraw.line(x, ur_y - line_length, x, ur_y, drawtype, idx, 0);
        } else {
          this.ctx.gdraw.line(x, ur_y - Math.floor(2.0 / 3 * line_length), x,
            ur_y, drawtype, idx, 0);
        }

        if (draw_text && i % this.valuequotient === 0) {
          text =
            this.format_text(this.format, this.max_value - i * this.increment);
          var d3 = this.ctx.gdraw.getTextExtent(text, Math.max(0, text_idx),
            Glow.eFont_Helvetica, this.text_drawtype);
          z_width = d3.width;
          z_height = d3.height;
          z_descent = z_height / 4;

          if (text_idx >= 0 && z_height < ur_y - ll_y) {
            if (i === this.lines - 1) {
              x_text = x;
            } else if (i === 0) {
              x_text = x - z_width;
            } else {
              x_text = x - (z_width) / 2;
            }
            this.ctx.gdraw.text(x_text, ll_y + z_height - z_descent, text,
              this.text_drawtype, this.text_color_drawtype, text_idx, highlight,
              0, Glow.eFont_Helvetica, tsize, 0);
          }
        }
      }
    } else if (135 < rotation && rotation <= 225) {
      // Vertical line to the left and values to the right

      this.ctx.gdraw.line(ll_x, ll_y, ll_x, ur_y, drawtype, idx, 0);

      // Calculate max value text width
      if (draw_text) {
        for (i = 0; i < this.lines; i++) {
          if (i % this.valuequotient === 0) {
            text = this.format_text(this.format, this.max_value - i *
              this.increment);
            var d4 = this.ctx.gdraw.getTextExtent(text, Math.max(0, text_idx),
              Glow.eFont_Helvetica, this.text_drawtype);
            z_width = d4.width;
            z_height = d4.height;
            z_descent = z_height / 4;
            if (max_z_width < z_width) {
              max_z_width = z_width;
            }
          }
        }
        x_text = ur_x - max_z_width;
        line_length = ur_x - ll_x - max_z_width;
        if (line_length < 3) {
          line_length = 3;
        }
      } else {
        x_text = ur_x;
        line_length = ur_x - ll_x;
      }

      for (i = 0; i < this.lines; i++) {
        y = Math.floor(ll_y + (ur_y - ll_y) / (this.lines - 1) *
          (this.lines - 1 - i));
        if (i % this.longquotient === 0) {
          this.ctx.gdraw.line(ll_x, y, ll_x + line_length, y, drawtype, idx, 0);
        } else {
          this.ctx.gdraw.line(ll_x, y, ll_x + Math.floor(2.0 / 3 * line_length),
            y, drawtype, idx, 0);
        }
        text =
          this.format_text(this.format, this.max_value - i * this.increment);

        if (draw_text && text_idx >= 0 && max_z_width < ur_x - ll_x &&
          i % this.valuequotient === 0) {
          if (i === this.lines - 1) {
            y_text = y + z_height - z_descent - 3;
          } else if (i === 0) {
            y_text = y;
          } else {
            y_text = y + (z_height - z_descent) / 2;
          }
          this.ctx.gdraw.text(x_text, y_text, text, this.text_drawtype,
            this.text_color_drawtype, text_idx, highlight, 0,
            Glow.eFont_Helvetica, tsize, 0);
        }
      }
    } else { // if ( 225 < rotation && rotation <= 315)
      // Horizontal line at top and values at the bottom

      this.ctx.gdraw.line(ll_x, ll_y, ur_x, ll_y, drawtype, idx, 0);

      // Calculate max value text height
      if (draw_text) {
        var d5 = this.ctx.gdraw.getTextExtent("0", Math.max(0, text_idx),
          Glow.eFont_Helvetica, this.text_drawtype);

        z_width = d5.width;
        z_height = d5.height;
        z_descent = z_height / 4;
        line_length = ur_y - ll_y - (z_height - z_descent);
        if (line_length < 3) {
          line_length = 3;
        }
      } else {
        line_length = ur_y - ll_y;
      }

      for (i = 0; i < this.lines; i++) {
        x = Math.floor(ll_x + (ur_x - ll_x) / (this.lines - 1) * i);
        if (i % this.longquotient === 0) {
          this.ctx.gdraw.line(x, ll_y, x, ll_y + line_length, drawtype, idx, 0);
        } else {
          this.ctx.gdraw.line(x, ll_y, x, ll_y +
            Math.floor(2.0 / 3 * line_length), drawtype, idx, 0);
        }
        if (draw_text && i % this.valuequotient === 0) {
          text =
            this.format_text(this.format, this.max_value - i * this.increment);
          var d6 = this.ctx.gdraw.getTextExtent(text, Math.max(0, text_idx),
            Glow.eFont_Helvetica, this.text_drawtype);
          z_width = d6.width;
          z_height = d6.height;
          z_descent = z_height / 4;

          if (text_idx >= 0 && z_height - z_descent < ur_y - ll_y) {
            if (i === this.lines - 1) {
              x_text = x - z_width;
            } else if (i === 0) {
              x_text = x;
            } else {
              x_text = x - (z_width) / 2;
            }
            this.ctx.gdraw.text(x_text, ur_y, text, this.text_drawtype,
              this.text_color_drawtype, text_idx, highlight, 0,
              Glow.eFont_Helvetica, tsize, 0);
          }
        }
      }
    }

  };

  this.format_text = function (fmt, value) {
    if (Math.abs(value) < Number.MIN_VALUE) {
      value = 0;
    }
    return this.cFormat.format(value, sb);
  };

  this.getUserData = function () {
    return userdata;
  };

  this.set_range = function (minval, maxval, keep_settings) {
    this.max_value = maxval;
    this.min_value = minval;

    var x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
      this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    var y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
      this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    var x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
      this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    var y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
      this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    var rotation = (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) *
      360;

    if (keep_settings === 0) {
      var len;
      var lix;
      var di;
      var horizontal = (rotation < 45 || (rotation > 135 && rotation < 225) ||
        rotation > 315) ? 0 : 1;
      if (horizontal === 0) {
        len = Math.abs(y2 - y1);
      } else {
        len = Math.abs(x2 - x1);
      }

      if (len < 150) {
        lix = 0;
      } else {
        lix = 1;
      }

      var d = Math.abs(maxval - minval);
      if (d < 5) {
        d = 1000 * d;
      }

      di = Math.floor(d + 0.5);
      while (di >= 25) {
        di = Math.floor(di / 10 + 0.5);
      }

      if (di > 0 && di <= 25) {
        this.lines = RangeData[lix][di - 1].lines;
        this.longquotient = RangeData[lix][di - 1].longq;
        if (this.horizontal === 0) {
          this.valuequotient = RangeData[lix][di - 1].vvalq;
        } else {
          this.valuequotient = RangeData[lix][di - 1].hvalq;
        }
      }

      var m = Math.max(Math.abs(maxval), Math.abs(minval));
      switch (lix) {
        case 0: {
          if (m < 0.01) {
            this.format = "%g";
          } else if (m < 0.1) {
            this.format = "%5.3f";
          } else if (m < 1) {
            this.format = "%4.2f";
          } else if (m < 3) {
            this.format = "%3.1f";
          } else if (m <= 20) {
            this.format = "%2.0f";
          } else if (m <= 200) {
            this.format = "%3.0f";
          } else if (m < 2000) {
            this.format = "%4.0f";
          } else if (m < 20000) {
            this.format = "%5.0f";
          } else {
            this.format = "%g";
          }
          break;
        }
        case 1: {
          if (m < 0.01) {
            this.format = "%g";
          } else if (m < 0.1) {
            this.format = "%5.3f";
          } else if (m < 1) {
            this.format = "%4.2f";
          } else if (m <= 4) {
            this.format = "%3.1f";
          } else if (m <= 20) {
            this.format = "%2.0f";
          } else if (m <= 200) {
            this.format = "%3.0f";
          } else if (m < 2000) {
            this.format = "%4.0f";
          } else if (m < 20000) {
            this.format = "%5.0f";
          } else {
            this.format = "%g";
          }
          break;
        }
        default:
          break;
      }
    }
    this.configure();
    this.draw();
  };

  this.setVisibility = function (visibility) {
    switch (visibility) {
      case Glow.eVis_Visible:
        this.invisible = 0;
        break;
      case Glow.eVis_Invisible:
        this.invisible = 1;
        this.draw();
        break;
      case Glow.eVis_Dimmed:
        break;
      default:
        break;
    }
  };

  this.getClassDynType1 = function () {
    return 0;
  };
  this.getClassDynType2 = function () {
    return 0;
  };
  this.getClassActionType1 = function () {
    return 0;
  };
  this.getClassActionType2 = function () {
    return 0;
  };
  this.getClassCycle = function () {
    return 0;
  };
  this.getClassUserData = function () {
    return null;
  };
  this.getUserData = function () {
    return this.userdata;
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

GrowAxis.prototype = Object.create(GrowRect.prototype);

function GrowAxisArc(ctx) {
  GrowArc.call(this, ctx);
  this.growarc_open = this.open;

  this.text_size;
  this.text_drawtype;
  this.text_color_drawtype;
  this.max_value;
  this.min_value;
  this.lines;
  this.linelength;
  this.longquotient;
  this.valuequotient;
  this.increment;
  this.format = null;
  this.userdata = null;
  this.cFormat = null;
  this.sb = null;

  this.type = function () {
    return Glow.eObjectType_GrowAxisArc;
  };

  this.configure = function () {
    if (this.lines <= 1) {
      this.lines = 2;
    }
    if (this.longquotient <= 0) {
      this.longquotient = 1;
    }
    if (this.valuequotient <= 0) {
      this.valuequotient = 1;
    }
    this.increment = (this.max_value - this.min_value) / (this.lines - 1);
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowAxisArc : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowAxisArc:
          break;
        case Glow.eSave_GrowAxisArc_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxisArc_text_drawtype:
          this.text_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxisArc_text_color_drawtype:
          this.text_color_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxisArc_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowAxisArc_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowAxisArc_lines:
          this.lines = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxisArc_linelength:
          this.linelength = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowAxisArc_longquotient:
          this.longquotient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxisArc_valuequotient:
          this.valuequotient = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowAxisArc_format:
          if (tokens.length > 1) {
            this.format = tokens[1];
          }
          break;
        case Glow.eSave_GrowAxisArc_arc_part:
          i = this.growarc_open(lines, i + 1);
          break;
        case Glow.eSave_GrowAxisArc_userdata_cb:
          if (this.ctx.appl !== null) {
            var ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowAxisArc");
          break;
      }
      if (end) {
        break;
      }
    }

    this.configure();
    if (this.format !== null) {
      this.cFormat = new GlowCFormat(this.format);
    }

    return i;
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }
    var i;
    var draw_text = (Math.abs(this.increment) > Number.MIN_VALUE);
    var idx;
    var x, y;
    var text;
    var line_length;
    var x_text, y_text;
    var z_height = 0, z_width, z_descent = 0;
    var max_z_width = 0;
    var rotation;
    var drawtype;
    var text_idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 4);
    var tsize = this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
      (8 + 2 * this.text_size);
    text_idx = Math.min(text_idx, Glow.DRAW_TYPE_SIZE - 1);

    if (node !== null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1);
    }
    idx += hot;

    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y, xt, yt;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      rotation =
        (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) * 360;
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
      rotation =
        (this.trf.rot(t) / 360 - Math.floor(this.trf.rot(t) / 360)) * 360;
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);
    drawtype =
      GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
        highlight, colornode, 0, 0);

    if (true) {
      // Lines inwards
      this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1 -
        rotation, this.angle2, drawtype, idx, 0);

      if (this.lines === 1) {
        return;
      }

      if (this.increment > 0) {
        text = this.format_text(this.format, this.min_value + (this.lines - 2) *
          this.increment);
      } else {
        text = this.format_text(this.format, this.min_value + this.increment);
      }
      var d = this.ctx.gdraw.getTextExtent(text, Math.max(0, text_idx),
        Glow.eFont_Helvetica, this.text_drawtype);
      z_width = d.width;
      z_height = d.height;
      z_descent = z_height / 4;
      if (max_z_width < z_width) {
        max_z_width = z_width;
      }

      var line_angle = this.angle2 / (this.lines - 1);
      for (i = 0; i < this.lines; i++) {
        var sin1 = Math.sin((this.angle1 + i * line_angle) / 180 * Math.PI);
        var cos1 = Math.cos((this.angle1 + i * line_angle) / 180 * Math.PI);
        y1 = Math.floor((ur_y - ll_y) / 2 * (-sin1 + 1) + ll_y);
        x1 = Math.floor((ur_x - ll_x) / 2 * (cos1 + 1) + ll_x);
        if (i % this.longquotient === 0) {
          y2 = Math.floor((ur_y - ll_y) / 2 *
            (-sin1 * (1.0 - this.linelength) + 1) + ll_y);
          x2 = Math.floor((ur_x - ll_x) / 2 *
            (cos1 * (1.0 - this.linelength) + 1) + ll_x);
        } else {
          y2 = Math.floor((ur_y - ll_y) / 2 *
            (-sin1 * (1.0 - this.linelength / 2) + 1) + ll_y);
          x2 = Math.floor((ur_x - ll_x) / 2 *
            (cos1 * (1.0 - this.linelength / 2) + 1) + ll_x);
        }
        yt =
          Math.floor((ur_y - ll_y) / 2 * (-sin1 * (1.0 - this.linelength) + 1) +
            ll_y + sin1 * (z_height - z_descent) / 2);
        xt =
          Math.floor((ur_x - ll_x) / 2 * (cos1 * (1.0 - this.linelength) + 1) +
            ll_x - cos1 * z_width / 2);

        this.ctx.gdraw.line(x1, y1, x2, y2, drawtype, idx, 0);
        if (draw_text) {
          if (text_idx >= 0 && i % this.valuequotient === 0 &&
            !(this.angle2 === 360 &&
              ((this.increment > 0 && i === this.lines - 1) ||
                (this.increment < 0 && i === 0)))) {
            text = this.format_text(this.format, this.min_value + i *
              this.increment);
            d = this.ctx.gdraw.getTextExtent(text, Math.max(0, text_idx),
              Glow.eFont_Helvetica, this.text_drawtype);
            z_width = d.width;
            z_height = d.height;
            z_descent = z_height / 4;
            if (max_z_width < z_width) {
              max_z_width = z_width;
            }

            if (i === this.lines - 1 && this.angle1 === 0 &&
              this.angle2 === 180) {
              xt = xt - z_width / 2;
            } else if (i === 0 && this.angle1 === 0 && this.angle2 !== 360) {
              xt = xt - z_width / 2;
            } else {
              yt = yt + (z_height - z_descent) / 2;
              xt = xt - z_width / 2;
            }
            this.ctx.gdraw.text(xt, yt, text, this.text_drawtype,
              this.text_color_drawtype, text_idx, highlight, 0,
              Glow.eFont_Helvetica, tsize, 0);
          }
        }
      }
    }

  };

  this.format_text = function (fmt, value) {
    if (Math.abs(value) < Number.MIN_VALUE) {
      value = 0;
    }
    return this.cFormat.format(value, sb);
  };

  this.getUserData = function () {
    return userdata;
  };

  this.set_range = function (minval, maxval, keep_settings) {
    this.max_value = maxval;
    this.min_value = minval;

    var x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
      this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    var y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
      this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    var x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
      this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    var y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
      this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    var rotation = (this.trf.rot() / 360 - Math.floor(this.trf.rot() / 360)) *
      360;

    if (keep_settings === 0) {
      var len;
      var lix;
      var di;
      var horizontal = (rotation < 45 || (rotation > 135 && rotation < 225) ||
        rotation > 315) ? 0 : 1;
      if (horizontal === 0) {
        len = Math.abs(y2 - y1);
      } else {
        len = Math.abs(x2 - x1);
      }

      if (len < 150) {
        lix = 0;
      } else {
        lix = 1;
      }

      var d = Math.abs(maxval - minval);
      if (d < 5) {
        d = 1000 * d;
      }

      di = Math.floor(d + 0.5);
      while (di >= 25) {
        di /= 10;
      }

      if (di > 0 && di <= 25) {
        this.lines = RangeData[lix][di - 1].lines;
        this.longquotient = RangeData[lix][di - 1].longq;
        if (horizontal === 0) {
          this.valuequotient = RangeData[lix][di - 1].vvalq;
        } else {
          this.valuequotient = RangeData[lix][di - 1].hvalq;
        }
      }

      var m = Math.max(Math.abs(maxval), Math.abs(minval));
      switch (lix) {
        case 0: {
          if (m < 0.01) {
            this.format = "%g";
          } else if (m < 0.1) {
            this.format = "%5.3f";
          } else if (m < 1) {
            this.format = "%4.2f";
          } else if (m < 3) {
            this.format = "%3.1f";
          } else if (m <= 20) {
            this.format = "%2.0f";
          } else if (m <= 200) {
            this.format = "%3.0f";
          } else if (m < 2000) {
            this.format = "%4.0f";
          } else if (m < 20000) {
            this.format = "%5.0f";
          } else {
            this.format = "%g";
          }
          break;
        }
        case 1: {
          if (m < 0.01) {
            this.format = "%g";
          } else if (m < 0.1) {
            this.format = "%5.3f";
          } else if (m < 1) {
            this.format = "%4.2f";
          } else if (m <= 4) {
            this.format = "%3.1f";
          } else if (m <= 20) {
            this.format = "%2.0f";
          } else if (m <= 200) {
            this.format = "%3.0f";
          } else if (m < 2000) {
            this.format = "%4.0f";
          } else if (m < 20000) {
            this.format = "%5.0f";
          } else {
            this.format = "%g";
          }
          break;
        }
        default:
          break;
      }
    }
    this.configure();
    this.draw();
  };

  this.getClassDynType1 = function () {
    return 0;
  };
  this.getClassDynType2 = function () {
    return 0;
  };
  this.getClassActionType1 = function () {
    return 0;
  };
  this.getClassActionType2 = function () {
    return 0;
  };
  this.getClassCycle = function () {
    return 0;
  };
  this.getClassUserData = function () {
    return null;
  };
  this.getUserData = function () {
    return this.userdata;
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

GrowAxisArc.prototype = Object.create(GrowArc.prototype);


function GrowPie(ctx) {
  GrowArc.call(this, ctx);
  this.growarc_open = this.open;

  this.PIE_MAX_SECTORS = 12;

  this.sectors;
  this.min_value;
  this.max_value;
  this.sector_color = new Array(this.PIE_MAX_SECTORS);
  this.sector_size = new Array(this.PIE_MAX_SECTORS);
  this.userdata = null;

  this.type = function () {
    return Glow.eObjectType_GrowPie;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowPie : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowPie:
          break;
        case Glow.eSave_GrowPie_arc_part:
          i = this.growarc_open(lines, i + 1);
          break;
        case Glow.eSave_GrowPie_sectors:
          this.sectors = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_color1:
          this.sector_color[0] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_color2:
          this.sector_color[1] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_color3:
          this.sector_color[2] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_color4:
          this.sector_color[3] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_color5:
          this.sector_color[4] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_color6:
          this.sector_color[5] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_color7:
          this.sector_color[6] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_color8:
          this.sector_color[7] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_color9:
          this.sector_color[8] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_color10:
          this.sector_color[9] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_color11:
          this.sector_color[10] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_color12:
          this.sector_color[11] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowPie_sector_size1:
          this.sector_size[0] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_size2:
          this.sector_size[1] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_size3:
          this.sector_size[2] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_size4:
          this.sector_size[3] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_size5:
          this.sector_size[4] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_size6:
          this.sector_size[5] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_size7:
          this.sector_size[6] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_size8:
          this.sector_size[7] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_size9:
          this.sector_size[8] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_size10:
          this.sector_size[9] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_size11:
          this.sector_size[10] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_sector_size12:
          this.sector_size[11] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowPie_userdata_cb:
          if (this.ctx.appl !== null) {
            var ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;

        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowPie");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var drawtype;
    var idx;
    var chot = 0;

    if (this.ctx.hot_indication === Glow.eHotIndication_No) {
      hot = 0;
    } else if (this.ctx.hot_indication === Glow.eHotIndication_DarkColor) {
      chot = hot;
      hot = 0;
    } else if (this.ctx.hot_indication === Glow.eHotIndication_LightColor) {
      chot = -hot;
      hot = 0;
    }

    if (this.fixcolor !== 0) {
      colornode = null;
    }

    if (node !== null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1);
    }
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y, rot;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot());
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot(t));
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    var display_shadow = ((node !== null && node.shadow !== 0) ||
      this.shadow !== 0) && this.disable_shadow === 0;
    var grad = this.gradient;
    if (this.gradient === Glow.eGradient_No &&
      (node !== null && node.gradient !== Glow.eGradient_No) &&
      this.disable_gradient === 0) {
      grad = node.gradient;
    }

    var a1 = this.angle1;
    var a2 = 0;
    var ia1 = this.angle1;
    var ia2;
    for (var i = 0; i < this.sectors + 1; i++) {
      var fillcolor;

      if (i === this.sectors) {
        if (ia1 >= this.angle1 + this.angle2) {
          break;
        }
        ia2 = this.angle1 + this.angle2 - ia1;
        fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
          Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);
      } else {
        if (this.sector_size[i] <= this.min_value) {
          continue;
        }
        if (a1 >= this.angle2 + this.angle1) {
          break;
        }
        a2 =
          this.sector_size[i] / (this.max_value - this.min_value) * this.angle2;
        if (a1 + a2 > this.angle1 + this.angle2) {
          a2 = this.angle1 + this.angle2 - a1;
        }
        ia2 = Math.floor(a2 + a1 - ia1 + 0.5);

        fillcolor = GlowColor.get_drawtype(this.sector_color[i],
          Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);
      }


      if (grad === Glow.eGradient_No || fillcolor === Glow.eDrawType_ColorRed) {
        if (chot === 0) {
          drawtype = fillcolor;
        } else {
          drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
        }
        this.ctx.gdraw.fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, ia1 - rot,
          ia2, drawtype);
      } else if (!display_shadow || this.shadow_width === 0) {
        var f1, f2;
        if (this.gradient_contrast >= 0) {
          f2 = GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
            chot, null);
          f1 = GlowColor.shift_drawtype(
            fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
            null);
        } else {
          f2 = GlowColor.shift_drawtype(
            fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
            null);
          f1 = GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
            chot, 0);
        }
        this.ctx.gdraw.gradient_fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y -
          ll_y, ia1 - rot, ia2, fillcolor, f1, f2, grad);
      } else {
        var ish = Math.floor(this.shadow_width / 100 *
          Math.min(ur_x - ll_x, ur_y - ll_y) + 0.5);
        var drawtype_incr = this.shadow_contrast;
        if (this.relief === Glow.eRelief_Down) {
          drawtype_incr = -this.shadow_contrast;
        }

        var f1, f2;

        // Draw shadow
        f1 =
          GlowColor.shift_drawtype(fillcolor, -drawtype_incr + chot, colornode);
        f2 =
          GlowColor.shift_drawtype(fillcolor, drawtype_incr + chot, colornode);

        this.ctx.gdraw.gradient_fill_arc(ll_x, ll_y, ur_x - ll_x, ur_y -
          ll_y, ia1 - rot, ia2, fillcolor, f2, f1,
          Glow.eGradient_DiagonalUpperLeft);

        // Draw circle
        if (this.gradient_contrast >= 0) {
          f2 = GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast / 2 +
            chot, null);
          f1 = GlowColor.shift_drawtype(
            fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) + chot,
            null);
        } else {
          f2 = GlowColor.shift_drawtype(
            fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) + chot,
            null);
          f1 = GlowColor.shift_drawtype(fillcolor, this.gradient_contrast / 2 +
            chot, null);
        }
        this.ctx.gdraw.gradient_fill_arc(ll_x + ish, ll_y + ish, ur_x - ll_x -
          2 * ish, ur_y - ll_y - 2 * ish, ia1 - rot, ia2, fillcolor, f1, f2,
          grad);
      }
      a1 += a2;
      ia1 += ia2;
    }

    if (this.border !== 0) {
      drawtype =
        GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
          highlight, colornode, 0, 0);

      this.ctx.gdraw.arc(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, this.angle1 -
        rot, this.angle2, drawtype, idx, 0);
    }
  };

  this.set_conf = function (sector_num, min_val, max_val, color) {
    this.sectors = sector_num;
    this.min_value = min_val;
    this.max_value = max_val;
    for (var i = 0; i < this.sectors; i++) {
      this.sector_color[i] = color[i];
    }
    this.draw();
  };

  this.get_conf = function () {
    var info = new GlowPieInfo();
    info.sector_num = this.sectors;
    info.min_val = this.min_value;
    info.max_val = this.max_value;
    return info;
  };

  this.set_values = function (values) {
    for (var i = 0; i < this.sectors; i++) {
      this.sector_size[i] = values[i];
    }
    this.draw();
  };

  this.getUserData = function () {
    return userdata;
  };
  this.getClassDynType1 = function () {
    return 0;
  };
  this.getClassDynType2 = function () {
    return 0;
  };
  this.getClassActionType1 = function () {
    return 0;
  };
  this.getClassActionType2 = function () {
    return 0;
  };
  this.getClassCycle = function () {
    return Glow.eCycle_Slow;
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

GrowPie.prototype = Object.create(GrowArc.prototype);

function GrowBarChart(ctx) {
  GrowRect.call(this, ctx);
  this.growrect_open = this.open;

  this.BARCHART_MAX_BARSEGMENTS = 12;
  this.BARCHART_MAX_BARS = 200;

  this.bars;
  this.barsegments;
  this.min_value;
  this.max_value;
  this.vertical_lines;
  this.horizontal_lines;
  this.line_color;
  this.bar_color = new Array(this.BARCHART_MAX_BARSEGMENTS);
  this.bar_values =
    new Array(this.BARCHART_MAX_BARSEGMENTS * this.BARCHART_MAX_BARS);
  this.userdata = null;

  this.type = function () {
    return Glow.eObjectType_GrowBarChart;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowPie : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowBarChart:
          break;
        case Glow.eSave_GrowBarChart_rect_part:
          i = this.growrect_open(lines, i + 1);
          break;
        case Glow.eSave_GrowBarChart_bars:
          this.bars = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_barsegments:
          this.barsegments = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_max_value:
          this.max_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowBarChart_min_value:
          this.min_value = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowBarChart_horizontal_lines:
          this.horizontal_lines = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_vertical_lines:
          this.vertical_lines = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_line_color:
          this.line_color = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color1:
          this.bar_color[0] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color2:
          this.bar_color[1] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color3:
          this.bar_color[2] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color4:
          this.bar_color[3] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color5:
          this.bar_color[4] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color6:
          this.bar_color[5] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color7:
          this.bar_color[6] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color8:
          this.bar_color[7] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color9:
          this.bar_color[8] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color10:
          this.bar_color[9] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color11:
          this.bar_color[10] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_bar_color12:
          this.bar_color[11] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowBarChart_userdata_cb:
          if (this.ctx.appl !== null) {
            var ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;

        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowBarChart");
          break;
      }
      if (end) {
        break;
      }
    }
    return i;
  };

  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var drawtype;
    var idx;
    var chot = 0;

    if (this.ctx.hot_indication === Glow.eHotIndication_No) {
      hot = 0;
    } else if (this.ctx.hot_indication === Glow.eHotIndication_DarkColor) {
      chot = hot;
      hot = 0;
    } else if (this.ctx.hot_indication === Glow.eHotIndication_LightColor) {
      chot = -hot;
      hot = 0;
    }

    if (node != null && node.line_width !== 0) {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          node.line_width - 1);
    } else {
      idx =
        Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
          this.line_width - 1);
    }
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);
    var x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y, rot;

    if (t === null) {
      x1 = Math.floor(this.trf.x(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot());
    } else {
      x1 = Math.floor(this.trf.x(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y1 = Math.floor(this.trf.y(t, this.ll.x, this.ll.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      x2 = Math.floor(this.trf.x(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_x + 0.5) - this.ctx.mw.offset_x;
      y2 = Math.floor(this.trf.y(t, this.ur.x, this.ur.y) *
        this.ctx.mw.zoom_factor_y + 0.5) - this.ctx.mw.offset_y;
      rot = Math.floor(this.trf.rot(t));
    }

    ll_x = Math.min(x1, x2);
    ur_x = Math.max(x1, x2);
    ll_y = Math.min(y1, y2);
    ur_y = Math.max(y1, y2);

    var grad = this.gradient;
    if (this.gradient === Glow.eGradient_No &&
      (node !== null && node.gradient !== Glow.eGradient_No) &&
      this.disable_gradient === 0) {
      grad = node.gradient;
    }

    var skip;
    var bar_ll_x, bar_ur_x;
    var bar_up_ll_y, bar_up_ur_y;
    var f_bar_up_ll_y;
    var bar_down_ll_y, bar_down_ur_y, border_bar_down_ur_y;
    var f_bar_down_ur_y;
    var width;
    var brect_ll_x = 0;
    var brect_ll_y = 0;
    var brect_width = 0;
    var brect_height = 0;

    width = (ur_x - ll_x) / this.bars;
    bar_ur_x = ll_x;
    for (var j = 0; j < this.bars; j++) {
      bar_ll_x = bar_ur_x;

      if (j === this.bars - 1) {
        bar_ur_x = ur_x;
      } else {
        bar_ur_x = ll_x + (j + 1) * width;
      }

      if (this.min_value >= 0) {
        bar_up_ll_y = ur_y;
        f_bar_up_ll_y = ur_y;

      } else {
        bar_up_ll_y = ur_y + this.min_value * (ur_y - ll_y) /
          (this.max_value - this.min_value);
        f_bar_up_ll_y = bar_up_ll_y;
        bar_down_ur_y = ur_y + this.min_value * (ur_y - ll_y) /
          (this.max_value - this.min_value);
        f_bar_down_ur_y = bar_down_ur_y;
      }

      for (var i = 0; i < this.barsegments + 1; i++) {
        if (typeof this.bar_values[i] === 'undefined') {
          continue;
        }
        var fillcolor = 0;

        if (this.min_value > 0 || this.bar_values[i * this.bars + j] > 0) {
          skip = 0;
          bar_up_ur_y = bar_up_ll_y;

          if (i === this.barsegments) {
            if (bar_up_ll_y <= ll_y) {
              skip = 1;
            } else {
              bar_up_ll_y = ll_y;
            }
            fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
              Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);
          } else {
            if (this.bar_values[i * this.bars + j] <= this.min_value) {
              skip = 1;
            } else if (bar_up_ur_y <= ll_y) {
              skip = 1;
            } else {
              f_bar_up_ll_y -=
                this.bar_values[i * this.bars + j] * (ur_y - ll_y) /
                (this.max_value - this.min_value);
              bar_up_ll_y = Math.floor(f_bar_up_ll_y);
              if (bar_up_ll_y < ll_y) {
                bar_up_ll_y = ll_y;
              }

              fillcolor = GlowColor.get_drawtype(this.bar_color[i],
                Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);
            }
          }
          if (skip === 0) {
            if (grad === Glow.eGradient_No ||
              fillcolor === Glow.eDrawType_ColorRed || i === this.barsegments) {
              if (chot === 0) {
                drawtype = fillcolor;
              } else {
                drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
              }
              this.ctx.gdraw.fill_rect(bar_ll_x, bar_up_ll_y, bar_ur_x -
                bar_ll_x, bar_up_ur_y - bar_up_ll_y, drawtype);
            } else {
              var f1, f2;
              if (this.gradient_contrast >= 0) {
                f2 =
                  GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast /
                    2 + chot, null);
                f1 = GlowColor.shift_drawtype(
                  fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) +
                  chot, null);
              } else {
                f2 = GlowColor.shift_drawtype(
                  fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) +
                  chot, null);
                f1 =
                  GlowColor.shift_drawtype(fillcolor, this.gradient_contrast /
                    2 + chot, null);
              }
              this.ctx.gdraw.gradient_fill_rect(bar_ll_x,
                bar_up_ll_y, bar_ur_x - bar_ll_x, bar_up_ur_y - bar_up_ll_y,
                fillcolor, f1, f2, grad);
            }
          }

        } else {
          // negative value, draw bar downwards
          skip = 0;
          bar_down_ll_y = bar_down_ur_y;

          if (i === this.barsegments) {
            border_bar_down_ur_y = bar_down_ur_y;
            if (bar_down_ur_y >= ur_y) {
              skip = 1;
            } else {
              bar_down_ur_y = ur_y;
            }
            fillcolor = GlowColor.get_drawtype(this.fill_drawtype,
              Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);
          } else {
            if (this.bar_values[i * this.bars + j] >= 0) {
              skip = 1;
            } else if (bar_down_ll_y >= ur_y) {
              skip = 1;
            } else {
              f_bar_down_ur_y -=
                this.bar_values[i * this.bars + j] * (ur_y - ll_y) /
                (this.max_value - this.min_value);
              bar_down_ur_y = Math.floor(f_bar_down_ur_y);
              if (bar_down_ur_y > ur_y) {
                bar_down_ur_y = ur_y;
              }

              fillcolor = GlowColor.get_drawtype(this.bar_color[i],
                Glow.eDrawType_FillHighlight, highlight, colornode, 1, 0);
            }
          }

          if (!skip) {
            if (grad === Glow.eGradient_No ||
              fillcolor === Glow.eDrawType_ColorRed || i === this.barsegments) {
              if (chot === 0) {
                drawtype = fillcolor;
              } else {
                drawtype = GlowColor.shift_drawtype(fillcolor, chot, null);
              }
              this.ctx.gdraw.fill_rect(bar_ll_x, bar_down_ll_y, bar_ur_x -
                bar_ll_x, bar_down_ur_y - bar_down_ll_y, drawtype);
            } else {
              var f1, f2;
              if (this.gradient_contrast >= 0) {
                f2 =
                  GlowColor.shift_drawtype(fillcolor, -this.gradient_contrast /
                    2 + chot, null);
                f1 = GlowColor.shift_drawtype(
                  fillcolor, Math.floor((this.gradient_contrast) / 2 + 0.6) +
                  chot, null);
              } else {
                f2 = GlowColor.shift_drawtype(
                  fillcolor, -Math.floor((this.gradient_contrast) / 2 - 0.6) +
                  chot, null);
                f1 =
                  GlowColor.shift_drawtype(fillcolor, this.gradient_contrast /
                    2 + chot, null);
              }
              this.ctx.gdraw.gradient_fill_rect(bar_ll_x,
                bar_down_ll_y, bar_ur_x - bar_ll_x, bar_down_ur_y -
                bar_down_ll_y, fillcolor, f1, f2, grad);
            }
          }
        }

        if (this.border !== 0 && i === this.barsegments) {
          // Draw previous bar border
          if (j > 0) {
            drawtype = GlowColor.get_drawtype(this.draw_type,
              Glow.eDrawType_LineHighlight, highlight, colornode, 0, 0);

            this.ctx.gdraw.rect(brect_ll_x, brect_ll_y, brect_width,
              brect_height, drawtype, idx, 0);
          }
          if (this.min_value >= 0) {
            brect_ll_x = bar_ll_x;
            brect_ll_y = bar_up_ll_y;
            brect_width = bar_ur_x - bar_ll_x;
            brect_height = ur_y - bar_up_ll_y;
          } else {
            brect_ll_x = bar_ll_x;
            brect_ll_y = bar_up_ll_y;
            brect_width = bar_ur_x - bar_ll_x;
            brect_height = ur_y + this.min_value * (ur_y - ll_y) /
              (this.max_value - this.min_value) - bar_up_ll_y;
          }
          if (j === this.bars - 1) {
            // Draw last bar border
            drawtype = GlowColor.get_drawtype(this.draw_type,
              Glow.eDrawType_LineHighlight, highlight, colornode, 0, 0);

            this.ctx.gdraw.rect(brect_ll_x, brect_ll_y, brect_width,
              brect_height, drawtype, idx, 0);
          }


          // Draw negative bar border
          if (this.min_value < 0) {
            if (j > 0) {
              drawtype = GlowColor.get_drawtype(this.draw_type,
                Glow.eDrawType_LineHighlight, highlight, colornode, 0, 0);

              this.ctx.gdraw.rect(brect_ll_x, brect_ll_y, brect_width,
                brect_height, drawtype, idx, 0);
            }
            brect_ll_x = bar_ll_x;
            brect_ll_y = ur_y + this.min_value * (ur_y - ll_y) /
              (this.max_value - this.min_value);
            brect_width = bar_ur_x - bar_ll_x;
            brect_height = border_bar_down_ur_y - brect_ll_y;
            if (j === this.bars - 1) {
              // Draw last bar border
              drawtype = GlowColor.get_drawtype(this.draw_type,
                Glow.eDrawType_LineHighlight, highlight, colornode, 0, 0);

              this.ctx.gdraw.rect(brect_ll_x, brect_ll_y, brect_width,
                brect_height, drawtype, idx, 0);
            }
          }
        }

      }
    }


    drawtype =
      GlowColor.get_drawtype(this.line_color, Glow.eDrawType_LineHighlight,
        highlight, colornode, 0, 0);

    for (var i = 0; i < this.vertical_lines; i++) {
      var x = Math.floor(ll_x + (ur_x - ll_x) / (this.vertical_lines + 1) *
        (i + 1));
      this.ctx.gdraw.line(x, ll_y, x, ur_y, drawtype, 0, 0);
    }

    for (var i = 0; i < this.horizontal_lines; i++) {
      var y = Math.floor(ll_y + (ur_y - ll_y) / (this.horizontal_lines + 1) *
        (i + 1));
      this.ctx.gdraw.line(ll_x, y, ur_x, y, drawtype, 0, 0);
    }

    if (this.border !== 0) {
      drawtype =
        GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
          highlight, colornode, 0, 0);
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, drawtype, idx,
        0);
    }

  };

  this.set_conf =
    function (bar_num, barsegment_num, min_val, max_val, vert_lines,
              horiz_lines, lcolor, color) {
      this.bars = bar_num;
      this.barsegments = barsegment_num;
      this.min_value = min_val;
      this.max_value = max_val;
      this.vertical_lines = vert_lines;
      this.horizontal_lines = horiz_lines;
      this.line_color = lcolor;
      for (var i = 0; i < this.bars; i++) {
        this.bar_color[i] = color[i];
      }
      this.draw();
    };


  this.get_conf = function () {
    var info = new GlowBarChartInfo();

    info.bars = this.bars;
    info.barsegments = this.barsegments;
    info.max_value = this.max_value;
    info.min_value = this.min_value;
    return info;
  };

  this.set_values =
    function (values1, values2, values3, values4, values5, values6, values7,
              values8, values9, values10, values11, values12) {
      var i;
      var j = 0;
      if (j < this.barsegments && values1 !== null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values1[i];
        }
      }
      j++;
      if (j < this.barsegments && values2 !== null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values2[i];
        }
      }
      j++;
      if (j < this.barsegments && values3 !== null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values3[i];
        }
      }
      j++;
      if (j < this.barsegments && values4 != null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values4[i];
        }
      }
      j++;
      if (j < this.barsegments && values5 != null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values5[i];
        }
      }
      j++;
      if (j < this.barsegments && values6 != null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values6[i];
        }
      }
      j++;
      if (j < this.barsegments && values7 != null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values7[i];
        }
      }
      j++;
      if (j < this.barsegments && values8 != null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values8[i];
        }
      }
      j++;
      if (j < this.barsegments && values9 != null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values9[i];
        }
      }
      j++;
      if (j < this.barsegments && values10 != null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values10[i];
        }
      }
      j++;
      if (j < this.barsegments && values11 != null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values11[i];
        }
      }
      j++;
      if (j < this.barsegments && values12 != null) {
        for (i = 0; i < this.bars; i++) {
          this.bar_values[j * this.bars + i] = values12[i];
        }
      }
    };

  this.getUserData = function () {
    return userdata;
  };
  this.getClassDynType1 = function () {
    return 0;
  };
  this.getClassDynType2 = function () {
    return 0;
  };
  this.getClassActionType1 = function () {
    return 0;
  };
  this.getClassActionType2 = function () {
    return 0;
  };
  this.getClassCycle = function () {
    return Glow.eCycle_Slow;
  };
  this.get_background_object_limits = function (t, type, x, y, bo) {
    return 0;
  };
}

GrowBarChart.prototype = Object.create(GrowRect.prototype);

function GrowTable(ctx) {
  GrowRect.call(this, ctx);
  this.growrect_open = this.open;
  this.growrect_event_handler = this.event_handler;

  this.mTableOptions_No = 0;
  this.mTableOptions_ZeroIfHeader = 1 << 0;
  this.mTableOptions_ZeroIfHeaderIs0 = 1 << 1;

  this.file_name;
  this.userdata = null;
  this.vertical_scrollbar;
  this.horizontal_scrollbar;
  this.scrollbar_width;
  this.v_scrollbar = null;
  this.h_scrollbar = null;
  this.v_value;
  this.h_value;
  this.table_x0;
  this.table_x1;
  this.table_y0;
  this.table_y1;
  this.scrollbar_color;
  this.scrollbar_bg_color;
  this.window_scale = 1;
  this.y_low_offs;
  this.x_left_offs;
  this.rows;
  this.columns;
  this.header_row;
  this.header_column;
  this.text_size;
  this.text_drawtype;
  this.text_color_drawtype;
  this.header_text_size;
  this.header_text_drawtype;
  this.header_text_color;
  this.header_row_height;
  this.row_height;
  this.column_width = new Array(Glow.TABLE_MAX_COL);
  this.header_text = new Array(Glow.TABLE_MAX_COL);
  this.column_size = new Array(Glow.TABLE_MAX_COL);
  this.column_adjustment = new Array(Glow.TABLE_MAX_COL);
  this.value_size;
  this.cell_value;
  this.selected_cell_row = -1;
  this.selected_cell_column = -1;
  this.select_drawtype;
  this.input_focus = 0;
  this.header_text_bold;
  this.options;
  this.font;

  this.type = function () {
    return Glow.eObjectType_GrowTable;
  };

  this.open = function (lines, row) {
    var end = false;
    var i;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.ctx.debug) {
        console.log("GrowPie : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowTable:
          break;
        case Glow.eSave_GrowTable_scrollbar_width:
          this.scrollbar_width = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_scrollbar_color:
          this.scrollbar_color = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_scrollbar_bg_color:
          this.scrollbar_bg_color = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_vertical_scrollbar:
          this.vertical_scrollbar = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_horizontal_scrollbar:
          this.horizontal_scrollbar = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_window_scale:
          this.window_scale = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_rows:
          this.rows = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_columns:
          this.columns = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_header_row:
          this.header_row = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_header_column:
          this.header_column = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_text_size:
          this.text_size = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_text_drawtype:
          this.text_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_text_color_drawtype:
          this.text_color_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_header_text_size:
          this.header_text_size = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_header_text_drawtype:
          this.header_text_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_header_text_color:
          this.header_text_color = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_header_text_bold:
          this.header_text_bold = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_header_row_height:
          this.header_row_height = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_row_height:
          this.row_height = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_options:
          this.options = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_select_drawtype:
          this.select_drawtype = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_font:
          this.font = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_width1:
          this.column_width[0] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text1:
          if (tokens.length > 1) {
            this.header_text[0] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_width2:
          this.column_width[1] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text2:
          if (tokens.length > 1) {
            this.header_text[1] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_width3:
          this.column_width[2] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text3:
          if (tokens.length > 1) {
            this.header_text[2] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_width4:
          this.column_width[3] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text4:
          if (tokens.length > 1) {
            this.header_text[3] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_width5:
          this.column_width[4] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text5:
          if (tokens.length > 1) {
            this.header_text[4] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_width6:
          this.column_width[5] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text6:
          if (tokens.length > 1) {
            this.header_text[5] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_width7:
          this.column_width[6] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text7:
          if (tokens.length > 1) {
            this.header_text[6] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_width8:
          this.column_width[7] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text8:
          if (tokens.length > 1) {
            this.header_text[7] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_width9:
          this.column_width[8] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text9:
          if (tokens.length > 1) {
            this.header_text[8] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_width10:
          this.column_width[9] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text10:
          if (tokens.length > 1) {
            this.header_text[9] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_width11:
          this.column_width[10] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text11:
          if (tokens.length > 1) {
            this.header_text[10] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_width12:
          this.column_width[11] = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowTable_header_text12:
          if (tokens.length > 1) {
            this.header_text[11] = lines[i].substring(5);
          }
          break;
        case Glow.eSave_GrowTable_column_adjustment1:
          this.column_adjustment[0] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_adjustment2:
          this.column_adjustment[1] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_adjustment3:
          this.column_adjustment[2] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_adjustment4:
          this.column_adjustment[3] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_adjustment5:
          this.column_adjustment[4] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_adjustment6:
          this.column_adjustment[5] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_adjustment7:
          this.column_adjustment[6] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_adjustment8:
          this.column_adjustment[7] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_adjustment9:
          this.column_adjustment[8] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_adjustment10:
          this.column_adjustment[9] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_adjustment11:
          this.column_adjustment[10] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_column_adjustment12:
          this.column_adjustment[11] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowTable_rect_part:
          i = this.growrect_open(lines, i + 1);
          break;
        case Glow.eSave_GrowTable_userdata_cb:
          if (this.ctx.appl !== null) {
            var ret = this.ctx.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_Node);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case Glow.eSave_End:
          end = true;
          break;
        default:
          console.log("Syntax error in GrowTable");
          break;
      }
      if (end) {
        break;
      }
    }

    if (this.ctx.debug) {
      console.log("GeTable: call_value size: " + columns * rows);
    }

    this.configure();
    this.configure_scrollbars();
    return i;
  };

  this.configure = function () {
    this.table_x0 = this.table_y0 = 0;
    this.table_x1 = 0;
    for (var i = 0; i < this.columns; i++) {
      if (this.header_column !== 0 && i === 0) {
        continue;
      }
      this.table_x1 += this.column_width[i];
    }
    this.table_y1 = this.row_height * this.rows;

    if (this.header_row === 0) {
      this.y_low_offs = 0;
    } else {
      this.y_low_offs = this.header_row_height;
    }

    if (this.header_column === 0) {
      this.x_left_offs = 0;
    } else {
      this.x_left_offs = this.column_width[0];
    }

    this.cell_value = new Array(this.columns * this.rows);
  };

  this.configure_scrollbars = function () {
    var x0, y0, width, height;

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

      this.v_scrollbar = new GrowScrollBar(ctx);
      this.v_scrollbar.init("vScrollbar", x0, y0, width, height,
        Glow.eDir_Vertical, Glow.eDrawType_Line, 1, this.display_level,
        this.scrollbar_bg_color, this.scrollbar_color, 1, this);
      this.v_scrollbar.set_range(this.table_y0 *
        this.window_scale, this.table_y1 * this.window_scale);
      this.v_scrollbar.set_value(this.table_y0 *
        this.window_scale, this.y_high - (this.y_low + this.y_low_offs) -
        this.scrollbar_width * this.horizontal_scrollbar);
      this.v_scrollbar.set_shadow(this.shadow);
      this.v_value = this.table_y0 * this.window_scale;
    } else if (this.vertical_scrollbar === 0 && this.v_scrollbar != null) {
      this.v_scrollbar = null;
      this.v_value = this.table_y0 * this.window_scale;
    } else if (this.v_scrollbar !== null) {
      // Reconfigure range and length
      this.v_scrollbar.set_range(this.table_y0 *
        this.window_scale, this.table_y1 * this.window_scale);
      this.v_scrollbar.set_value(this.table_y0 *
        this.window_scale, this.y_high - (this.y_low + this.y_low_offs) -
        this.scrollbar_width * this.horizontal_scrollbar);
      this.v_value = this.table_y0 * this.window_scale;
      this.v_scrollbar.set_shadow(this.shadow);
    } else {
      this.v_value = this.table_y0 * this.window_scale;
    }

    if (this.horizontal_scrollbar !== 0 && this.h_scrollbar === null) {
      x0 = this.x_left + this.x_left_offs;
      y0 = this.y_high - this.scrollbar_width;
      height = this.scrollbar_width;
      if (this.vertical_scrollbar === 0) {
        width = this.x_right - (this.x_left + this.x_left_offs);
      } else {
        width = this.x_right - (this.x_left + this.x_left_offs) -
          this.scrollbar_width;
      }

      this.h_scrollbar = new GrowScrollBar(ctx);
      this.h_scrollbar.init("hScrollbar", x0, y0, width, height,
        Glow.eDir_Horizontal, Glow.eDrawType_Line, 1, this.display_level,
        this.scrollbar_bg_color, this.scrollbar_color, 1, this);
      this.h_scrollbar.set_range(this.table_x0 *
        this.window_scale, this.table_x1 * this.window_scale);
      this.h_scrollbar.set_value(this.table_x0 *
        this.window_scale, this.x_right - (this.x_left + this.x_left_offs) -
        this.scrollbar_width * this.vertical_scrollbar);
      this.h_scrollbar.set_shadow(this.shadow);
      this.h_value = this.table_x0 * this.window_scale;
    } else if (this.horizontal_scrollbar === 0 && this.h_scrollbar !== null) {
      this.h_scrollbar = null;
      this.h_value = this.table_x0 * this.window_scale;
    } else if (this.h_scrollbar !== null) {
      // Reconfigure lenght and range
      this.h_scrollbar.set_value(this.table_x0 *
        this.window_scale, this.x_right - (this.x_left + this.x_left_offs) -
        this.scrollbar_width * this.vertical_scrollbar);
      this.h_value = this.table_x0 * this.window_scale;
      this.h_scrollbar.set_range(this.table_x0 *
        this.window_scale, this.table_x1 * this.window_scale);
      this.h_scrollbar.set_shadow(this.shadow);
    } else {
      this.h_value = this.table_x0 * this.window_scale;
    }
  };


  this.draw = function () {
    this.tdraw(null, 0, 0, null, null);
  };
  this.tdraw = function (t, highlight, hot, node, colornode) {
    if (this.ctx.nodraw !== 0) {
      return;
    }

    var idx;
    var drawtype;

    idx = Math.floor(this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
      this.line_width - 1);
    idx += hot;
    idx = Math.max(0, idx);
    idx = Math.min(idx, Glow.DRAW_TYPE_SIZE - 1);

    var text_idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (this.text_size + 4) - 4);
    var tsize = this.ctx.mw.zoom_factor_y / this.ctx.mw.base_zoom_factor *
      (8 + 2 * this.text_size);
    text_idx = Math.min(text_idx, Glow.DRAW_TYPE_SIZE - 1);
    var header_text_idx = Math.floor(this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (this.header_text_size + 4) - 4);
    header_text_idx = Math.min(header_text_idx, Glow.DRAW_TYPE_SIZE - 1);
    var header_tsize = this.ctx.mw.zoom_factor_y /
      this.ctx.mw.base_zoom_factor * (8 + 2 * this.header_text_size);

    var ll_x, ll_y, ur_x, ur_y;
    var dx1, dy1, dx2, dy2;

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

    if (this.v_scrollbar != null) {
      if (this.h_scrollbar == null) {
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
        this.h_scrollbar.set_position(dx1 + this.x_left_offs, dy2 -
          this.scrollbar_width, dx2 - (dx1 + this.x_left_offs),
          this.scrollbar_width);
      } else {
        this.h_scrollbar.set_position(dx1 + this.x_left_offs, dy2 -
          this.scrollbar_width, dx2 - (dx1 + this.x_left_offs) -
          this.scrollbar_width, this.scrollbar_width);
      }
      this.h_scrollbar.tdraw(null, 0, 0, null, null);
    }

    drawtype =
      GlowColor.get_drawtype(this.draw_type, Glow.eDrawType_LineHighlight,
        highlight, colornode, 0, 0);

    var light_drawtype;
    var dark_drawtype;
    var sel_drawtype;

    dark_drawtype = GlowColor.shift_drawtype(this.fill_drawtype, 2, null);
    light_drawtype = GlowColor.shift_drawtype(this.fill_drawtype, -2, null);
    if (this.select_drawtype === Glow.eDrawType_Inherit) {
      sel_drawtype = dark_drawtype;
    } else {
      sel_drawtype = this.select_drawtype;
    }

    ll_x = Math.floor(dx1 * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    ll_y = Math.floor(dy1 * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    ur_x = Math.floor(dx2 * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    ur_y = Math.floor(dy2 * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    var o_ll_x = Math.floor((dx1 + this.x_left_offs) *
      this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    var o_ll_y = Math.floor((dy1 + this.y_low_offs) *
      this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;
    var o_ur_x = Math.floor((dx2 - this.vertical_scrollbar *
      this.scrollbar_width) * this.ctx.mw.zoom_factor_x) - this.ctx.mw.offset_x;
    var o_ur_y = Math.floor((dy2 - this.horizontal_scrollbar *
      this.scrollbar_width) * this.ctx.mw.zoom_factor_y) - this.ctx.mw.offset_y;

    var t_ll_x = o_ll_x - Math.floor(this.h_value * this.ctx.mw.zoom_factor_x);
    var t_ll_y = o_ll_y - Math.floor(this.v_value * this.ctx.mw.zoom_factor_y);
    var t_ur_x = t_ll_x + Math.floor(this.table_x1 * this.ctx.mw.zoom_factor_x);
    var t_ur_y = t_ll_y + Math.floor(this.table_y1 * this.ctx.mw.zoom_factor_y);

    var x;
    var y;
    var offs;
    var header_h = 0;
    var header_w = 0;
    var text_offs = 10;

    if (this.header_row !== 0) {
      header_h = Math.floor(this.header_row_height * this.ctx.mw.zoom_factor_y);
    }
    if (this.header_column !== 0) {
      header_w = Math.floor(this.column_width[0] * this.ctx.mw.zoom_factor_x);
    }

    if (this.header_row !== 0) {

      if (this.fill !== 0) {
        this.ctx.gdraw.fill_rect(ll_x, ll_y, ur_x - ll_x, header_h,
          this.fill_drawtype);
      }

      this.ctx.gdraw.set_clip_rectangle(ll_x + header_w, ll_y, ur_x, ll_y +
        header_h);

      if (this.shadow !== 0) {
        x = t_ll_x;
        y = ll_y;
        for (var i = this.header_column; i < this.columns + 1; i++) {
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
      x = t_ll_x;
      y = ll_y;

      for (var i = this.header_column; i < this.columns + 1; i++) {
        if (x > ur_x) {
          break;
        }
        this.ctx.gdraw.line(Math.floor(x), ll_y, Math.floor(x), ll_y + header_h,
          drawtype, idx, 0);
        x += this.column_width[i] * this.ctx.mw.zoom_factor_x;
      }

      x = t_ll_x;
      y = ll_y;

      for (var i = this.header_column; i < this.columns; i++) {
        if (header_text_idx >= 0 && this.header_text[i] !== "") {
          this.ctx.gdraw.text(Math.floor(x + text_offs),
            Math.floor(y + header_h - 4), this.header_text[i],
            this.header_text_drawtype, this.header_text_color, header_text_idx,
            highlight, 0, this.font, header_tsize, 0);
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
      this.ctx.gdraw.rect(ll_x, ll_y, ur_x - ll_x, header_h, drawtype, idx, 0);

      if (this.header_column !== 0) {
        // Draw header of header column header
        x = ll_x;
        y = ll_y;
        if (header_text_idx >= 0 && this.header_text[0] !== "") {
          this.ctx.gdraw.text(Math.floor(x + text_offs),
            Math.floor(y + header_h - 4), this.header_text[0],
            this.header_text_drawtype, this.header_text_color, header_text_idx,
            highlight, 0, this.font, tsize, 0);
        }
      }
    }

    if (this.header_column !== 0) {

      if (this.fill !== 0) {
        this.ctx.gdraw.fill_rect(ll_x, ll_y + header_h, header_w, ur_y - ll_y -
          header_h, this.fill_drawtype);
      }

      this.ctx.gdraw.set_clip_rectangle(ll_x, ll_y + header_h, ll_x + header_w,
        ur_y);

      // Draw selected cell, if cell in header column
      if (this.selected_cell_row >= 0 && this.selected_cell_column === 0) {
        x = ll_x;
        y = t_ll_y + this.row_height * this.ctx.mw.zoom_factor_y *
          this.selected_cell_row;
        this.ctx.gdraw.fill_rect(Math.floor(x), Math.floor(y), header_w,
          Math.floor(this.row_height * this.ctx.mw.zoom_factor_y),
          sel_drawtype);
      }

      if (this.shadow !== 0) {
        x = ll_x;
        y = t_ll_y;
        for (var i = 0; i < this.rows + 1; i++) {
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
      x = ll_x;
      y = t_ll_y;

      for (var i = 0; i < this.rows; i++) {
        y += this.row_height * this.ctx.mw.zoom_factor_y;
        if (y - this.row_height * this.ctx.mw.zoom_factor_y > ur_y) {
          break;
        }
        if (y > ll_y) {
          this.ctx.gdraw.line(ll_x, Math.floor(y), ll_x + header_w,
            Math.floor(y), drawtype, idx, 0);

          offs = i;
          if (text_idx >= 0 && this.cell_value[offs] != null &&
            this.cell_value[offs] !== "") {
            var text_x = Math.floor(x) + text_offs;

            if (this.column_adjustment[0] === Glow.eAdjustment_Right ||
              this.column_adjustment[0] === Glow.eAdjustment_Center) {
              var width, height, descent;
              var dim = this.ctx.gdraw.getTextExtent(this.cell_value[offs],
                text_idx, this.font, this.text_drawtype);
              width = dim.width;
              height = dim.height;
              descent = height / 4;

              switch (this.column_adjustment[0]) {
                case Glow.eAdjustment_Left:
                  break;
                case Glow.eAdjustment_Right:
                  text_x = Math.floor(x) + header_w - text_offs - width;
                  break;
                case Glow.eAdjustment_Center:
                  text_x = Math.floor(x + (header_w - width) / 2);
                  break;
              }
            }
            this.ctx.gdraw.text(text_x, Math.floor(y - 5),
              this.cell_value[offs], this.text_drawtype,
              this.text_color_drawtype, text_idx, highlight, 0, this.font,
              tsize, 0);
          }
        }
      }
      this.ctx.gdraw.reset_clip_rectangle();
      this.ctx.gdraw.rect(ll_x, ll_y + header_h - 1, header_w, ur_y - ll_y -
        header_h + 1, drawtype, idx, 0);
    }

    // Draw table
    if (this.fill !== 0) {
      this.ctx.gdraw.fill_rect(o_ll_x, o_ll_y, o_ur_x - o_ll_x, o_ur_y - o_ll_y,
        this.fill_drawtype);
    }

    this.ctx.gdraw.set_clip_rectangle(o_ll_x, o_ll_y, o_ur_x, o_ur_y);

    if (this.selected_cell_row >= 0 &&
      !(this.header_column !== 0 && this.selected_cell_column === 0)) {
      // Draw selected cell, if cell not in header column
      x = t_ll_x;
      for (var i = this.header_column; i < this.selected_cell_column; i++) {
        x += this.column_width[i] * this.ctx.mw.zoom_factor_x;
      }
      y = t_ll_y + this.row_height * this.ctx.mw.zoom_factor_y *
        this.selected_cell_row;
      this.ctx.gdraw.fill_rect(Math.floor(x), Math.floor(y),
        Math.floor(this.column_width[selected_cell_column] *
          this.ctx.mw.zoom_factor_x),
        Math.floor(this.row_height * this.ctx.mw.zoom_factor_y),
        this.sel_drawtype);
    }

    if (this.shadow !== 0) {
      x = t_ll_x;
      y = t_ll_y;

      for (var i = this.header_column; i < this.columns + 1; i++) {
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

      for (var i = 0; i < this.rows + 1; i++) {
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

    x = t_ll_x;
    y = t_ll_y;

    for (var i = this.header_column; i < this.columns + 1; i++) {
      if (x > ur_x) {
        break;
      }
      if (x > ll_x) {
        this.ctx.gdraw.line(Math.floor(x), t_ll_y, Math.floor(x), t_ur_y,
          drawtype, idx, 0);
      }
      x += this.column_width[i] * this.ctx.mw.zoom_factor_x;
    }

    for (var i = 0; i < this.rows + 1; i++) {
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
    var column_offs = this.header_column * this.rows;
    x = t_ll_x;
    var x_border = ll_x -
      Math.floor(this.column_width[0] * this.ctx.mw.zoom_factor_x);
    for (var i = this.header_column; i < this.columns; i++) {
      if (x > ur_x) {
        break;
      }

      if (x > x_border) {
        // Draw column i
        y = t_ll_y;
        for (var j = 0; j < this.rows; j++) {
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
            offs = column_offs + j;
            if (text_idx >= 0 && this.cell_value[offs] !== null &&
              this.cell_value[offs] !== "") {
              var text_x = Math.floor(x) + text_offs;

              if (this.column_adjustment[i] === Glow.eAdjustment_Right ||
                this.column_adjustment[i] === Glow.eAdjustment_Center) {
                var width, height, descent;
                var dim = this.ctx.gdraw.getTextExtent(this.cell_value[offs],
                  text_idx, this.font, this.text_drawtype);
                width = dim.width;
                height = dim.height;
                descent = height / 4;

                switch (this.column_adjustment[i]) {
                  case Glow.eAdjustment_Left:
                    break;
                  case Glow.eAdjustment_Right:
                    text_x = Math.floor(x + this.column_width[i] *
                      this.ctx.mw.zoom_factor_x) - text_offs - width;
                    break;
                  case Glow.eAdjustment_Center:
                    text_x = Math.floor(x +
                      (this.column_width[i] * this.ctx.mw.zoom_factor_x -
                        width) / 2);
                    break;
                }
              }

              this.ctx.gdraw.text(text_x, Math.floor(y - 5),
                this.cell_value[offs], this.text_drawtype,
                this.text_color_drawtype, text_idx, highlight, 0, this.font,
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
      idx, 0);
    if (this.input_focus !== 0) {
      this.ctx.gdraw.line(ll_x - 2, ll_y - 2, ll_x - 2, ur_y + 2,
        Glow.eDrawType_DarkGray, 0, 0);
      this.ctx.gdraw.line(ll_x - 2, ll_y - 2, ur_x + 2, ll_y - 2,
        Glow.eDrawType_DarkGray, 0, 0);
      this.ctx.gdraw.line(ll_x - 2, ur_y + 2, ur_x + 2, ur_y + 2,
        Glow.eDrawType_LightGray, 0, 0);
      this.ctx.gdraw.line(ur_x + 2, ll_y - 2, ur_x + 2, ur_y + 2,
        Glow.eDrawType_LightGray, 0, 0);
    }
  };

  this.getUserData = function () {
    return this.userdata;
  };

  this.verticalScrollValueChanged = function (value) {
    if (this.v_value !== value) {
      this.v_value = value;
      this.draw();
    }
  };

  this.horizontalScrollValueChanged = function (value) {
    if (this.h_value !== value) {
      this.h_value = value;
      this.draw();
    }
  };

  this.event_handler = function (event, fx, fy) {
    var sts, v_sts, h_sts;

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

    sts = this.growrect_event_handler(event, fx, fy);
    if (event.event === Glow.eEvent_ButtonMotion) {
      return 0;
    }

    switch (event.event) {
      case Glow.eEvent_MB1Click: {
        var column = -1;
        var row = -1;

        // Find out which cell
        var o_ll_x = this.x_left + this.x_left_offs;
        var o_ll_y = this.y_low + this.y_low_offs;
        var o_ur_x = this.x_right - this.vertical_scrollbar *
          this.scrollbar_width;
        var o_ur_y = this.y_high - this.horizontal_scrollbar *
          this.scrollbar_width;

        var t_ll_x = o_ll_x - this.h_value;
        var t_ll_y = o_ll_y - this.v_value;

        if (o_ll_y <= fy && fy <= o_ur_y) {
          if (this.header_column !== 0 && this.x_left <= fx &&
            fx <= this.x_left + this.x_left_offs) {
            column = 0;
          } else if (o_ll_x <= fx && fx <= o_ur_x) {
            var column_x_right = t_ll_x;
            for (var i = this.header_column; i < this.columns; i++) {
              column_x_right += this.column_width[i];
              if (fx <= column_x_right) {
                column = i;
                break;
              }
            }
          }
          var row_y_high = t_ll_y;
          for (var i = 0; i < this.rows; i++) {
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
      }
      default:
    }

    return sts;
  };

  this.getTableInfo = function () {
    var info = new GlowTableInfo();

    info.columns = this.columns;
    info.rows = this.rows;
    for (var i = 0; i < Glow.TABLE_MAX_COL; i++) {
      info.column_size[i] = this.column_size[i];
    }
    return info;
  };

  this.setTableInfo = function (info) {
    this.columns = info.columns;
    this.rows = info.rows;
    for (var i = 0; i < Glow.TABLE_MAX_COL; i++) {
      this.column_size[i] = info.column_size[i];
    }
  };

  this.setValue = function (value, col, row) {
    this.cell_value[col * this.rows + row] = value;
  };

  this.setSelectedCell = function (column, row) {
    if (this.selected_cell_column === column && this.selected_cell_row === row) {
      return;
    }

    if (column >= this.columns || row >= this.rows) {
      return;
    }

    this.selected_cell_column = column;
    this.selected_cell_row = row;
    this.draw();
  };

  this.getSelectedCellRow = function () {
    return this.selected_cell_row;
  };
  this.getSelectedCellColumn = function () {
    if (this.selected_cell_row === -1) {
      return -1;
    }
    return this.selected_cell_column;
  };
}

GrowTable.prototype = Object.create(GrowRect.prototype);

function GrowCtxWindow() {
  this.zoom_factor_x = 1;
  this.zoom_factor_y = 1;
  this.base_zoom_factor = 1;
  this.offset_x = 0;
  this.offset_y = 0;
  this.window_width = 0;
  this.window_height = 0;
  this.subwindow_x = 0;
  this.subwindow_y = 0;
  this.subwindow_scale = 1;
}

function GlowDraw(ctx) {
  this.ctx = ctx;
  this.canvas = document.querySelector("canvas");
  this.gctx = this.canvas.getContext("2d");
  this.offset_top = this.canvas.offsetTop;
  this.clip_x1 = new Array(10);
  this.clip_y1 = new Array(10);
  this.clip_x2 = new Array(10);
  this.clip_y2 = new Array(10);
  this.clipCount = 0;
  this.CUSTOMCOLORS_STACK_SIZE = 10;
  this.customcolors = new Array(this.CUSTOMCOLORS_STACK_SIZE);
  this.customcolors_cnt = 0;

  this.gctx.save();

  this.rect = function (x, y, width, height, gc_type, idx, highlight) {
    this.gctx.lineWidth = idx + 1;
    this.gctx.strokeStyle = this.getColor(gc_type);
    this.gctx.strokeRect(x, y, width, height);
  };

  this.fill_rect = function (x, y, width, height, gc_type) {
    this.gctx.fillStyle = this.getColor(gc_type);
    this.gctx.fillRect(x, y, width, height);
  };

  this.line = function (x1, y1, x2, y2, gc_type, idx, highlight) {
    this.gctx.strokeStyle = this.getColor(gc_type);
    this.gctx.lineWidth = idx + 1;
    this.gctx.beginPath();
    this.gctx.moveTo(x1, y1);
    this.gctx.lineTo(x2, y2);
    this.gctx.stroke();
  };

  this.line_dashed =
    function (x1, y1, x2, y2, gc_type, idx, highlight, line_type) {

      var dash = null;
      switch (line_type) {
        case Glow.eLineType_Solid:
          break;
        case Glow.eLineType_Dashed1:
          dash = [idx, idx];
          break;
        case Glow.eLineType_Dashed2:
          dash = [idx * 2, idx * 2];
          break;
        case Glow.eLineType_Dashed3:
          dash = [idx * 4, idx * 4];
          break;
        case Glow.eLineType_Dotted:
          dash = [idx, idx * 6];
          break;
        case Glow.eLineType_DotDashed1:
          dash = [idx, idx * 2, idx * 4, idx * 2];
          break;
        case Glow.eLineType_DotDashed2:
          dash = [idx, idx * 4, idx * 8, idx * 4];
          break;
        default:
          break;
      }

      this.gctx.strokeStyle = this.getColor(gc_type);
      this.gctx.lineWidth = idx + 1;
      this.gctx.beginPath();
      if (dash !== null) {
        this.gctx.setLineDash(dash);
      }
      this.gctx.moveTo(x1, y1);
      this.gctx.lineTo(x2, y2);
      this.gctx.stroke();

      if (dash !== null) {
        this.gctx.setLineDash([]);
      }
    };

  this.arc = function (x, y, width, height, a1, a2, gc_type, idx, highlight) {
    this.gctx.strokeStyle = this.getColor(gc_type);
    this.gctx.lineWidth = idx + 1;
    this.gctx.beginPath();
    if (typeof this.gctx.ellipse === 'function') {
      this.gctx.ellipse(x + width / 2, y + height / 2, width / 2, height / 2,
        0, -a1 * Math.PI / 180, -(a1 + a2) * Math.PI / 180, true);
    } else {
      this.gctx.arc(x + width / 2, y + height / 2, width / 2, -a1 * Math.PI /
        180, -(a1 + a2) * Math.PI / 180, true);
    }
    this.gctx.stroke();
  };

  this.fill_arc = function (x, y, width, height, a1, a2, gc_type) {
    this.gctx.fillStyle = this.getColor(gc_type);
    this.gctx.beginPath();
    if (a2 !== 360) {
      this.gctx.moveTo(x + width / 2, y + height / 2);
    }
    if (typeof this.gctx.ellipse === 'function') {
      this.gctx.ellipse(x + width / 2, y + height / 2, width / 2, height / 2,
        0, -a1 * Math.PI / 180, -(a1 + a2) * Math.PI / 180, true);
    } else {
      this.gctx.arc(x + width / 2, y + height / 2, width / 2, -a1 * Math.PI /
        180, -(a1 + a2) * Math.PI / 180, true);
    }
    this.gctx.fill();
  };

  this.polyline = function (points, point_cnt, gc_type, idx, highlight) {
    if (this.nodraw) {
      return;
    }
    if (points.length === 0) {
      return;
    }

    this.gctx.lineWidth = idx + 1;
    this.gctx.strokeStyle = this.getColor(gc_type);
    this.gctx.beginPath();
    this.gctx.moveTo(points[0].x, points[0].y);
    for (var i = 1; i < point_cnt; i++) {
      this.gctx.lineTo(points[i].x, points[i].y);
    }

    this.gctx.stroke();
  };

  this.fill_polyline = function (points, point_cnt, gc_type, highlight) {
    if (this.nodraw) {
      return;
    }

    this.gctx.fillStyle = this.getColor(gc_type);

    this.gctx.beginPath();
    this.gctx.moveTo(points[0].x, points[0].y);
    for (var i = 1; i < point_cnt; i++) {
      this.gctx.lineTo(points[i].x, points[i].y);
    }
    this.gctx.fill();
  };

  this.text =
    function (x, y, text, gc_type, color, idx, highlight, line, font_idx, size,
              rot) {
      var tsize;

      switch (idx) {
        case 0:
          tsize = 8;
          break;
        case 1:
          tsize = 10;
          break;
        case 2:
          tsize = 12;
          break;
        case 3:
          tsize = 14;
          break;
        case 4:
          tsize = 14;
          break;
        case 5:
          tsize = 16;
          break;
        case 6:
          tsize = 18;
          break;
        case 7:
          tsize = 18;
          break;
        default:
          tsize = 3 * idx;
          break;
      }
      this.gctx.font = tsize + "px Arial";
      this.gctx.lineWidth = 0.5;

      this.gctx.fillStyle = this.getColor(color);
      this.gctx.fillText(text, x, y);
    };

  this.getColor = function (gc_type) {
    //console.log("CustomColor cnt", this.customcolors_cnt);
    var rgb = GlowColor.rgb_color(gc_type, this.get_customcolors());

    return "rgb(" + Math.floor(255 * rgb.r) + "," + Math.floor(255 * rgb.g) +
      "," + Math.floor(255 * rgb.b) + ")";
  };

  this.getTextExtent = function (text, idx, type, gc_type) {
    var tsize;
    var dim = new GlowDimension();

    switch (idx) {
      case 0:
        tsize = 8;
        break;
      case 1:
        tsize = 10;
        break;
      case 2:
        tsize = 12;
        break;
      case 3:
        tsize = 14;
        break;
      case 4:
        tsize = 14;
        break;
      case 5:
        tsize = 8;
        break;
      case 6:
        tsize = 18;
        break;
      case 7:
        tsize = 18;
        break;
      default:
        tsize = 3 * idx;
        break;
    }
    this.gctx.font = tsize + "px Arial";
    dim.width = this.gctx.measureText(text).width;
    dim.height = tsize;
    return dim;
  };

  this.gradient_fill_rect =
    function (x, y, width, height, gc_type, f1, f2, gradient) {
      if (this.nodraw) {
        return;
      }

      this.setGradient(gradient, f1, f2, x, y, width, height);

      this.gctx.fillRect(x, y, width, height);
    };

  this.gradient_fill_rectrounded =
    function (x, y, width, height, amount, gc_type, f1, f2, gradient) {
      if (this.nodraw) {
        return;
      }

      this.setGradient(gradient, f1, f2, x, y, width, height);

      this.gctx.beginPath();
      this.gctx.moveTo(x + amount, y);
      this.gctx.arcTo(x + width, y, x + width, y + height, amount);
      this.gctx.arcTo(x + width, y + height, x, y + height, amount);
      this.gctx.arcTo(x, y + height, x, y, amount);
      this.gctx.arcTo(x, y, x + width, y, amount);
      this.gctx.closePath();
      this.gctx.fill();
    };

  this.gradient_fill_arc =
    function (x, y, width, height, a1, a2, gc_type, f1, f2, gradient) {
      if (this.nodraw) {
        return;
      }

      this.setGradient(gradient, f1, f2, x, y, width, height);

      this.gctx.beginPath();
      if (a2 !== 360) {
        this.gctx.moveTo(x + width / 2, y + height / 2);
      }
      if (typeof this.gctx.ellipse === 'function') {
        this.gctx.ellipse(x + width / 2, y + height / 2, width / 2, height / 2,
          0, -a1 * Math.PI / 180, -(a1 + a2) * Math.PI / 180, true);
      } else {
        this.gctx.arc(x + width / 2, y + height / 2, width / 2, -a1 * Math.PI /
          180, -(a1 + a2) * Math.PI / 180, true);
      }
      this.gctx.fill();
    };

  this.gradient_fill_polyline =
    function (points, point_cnt, gc_type, f1, f2, gradient) {
      if (this.nodraw) {
        return;
      }
      if (points.length === 0) {
        return;
      }

      var color = this.getColor(gc_type);

      var x_low = Number.MAX_VALUE;
      var x_high = -Number.MAX_VALUE;
      var y_low = Number.MAX_VALUE;
      var y_high = -Number.MAX_VALUE;

      // this.gctx.fillStyle = this.getColor( gc_type);

      this.gctx.beginPath();
      this.gctx.moveTo(points[0].x, points[0].y);
      for (var i = 1; i < point_cnt; i++) {
        this.gctx.lineTo(points[i].x, points[i].y);
        if (points[i].x > x_high) {
          x_high = points[i].x;
        }
        if (points[i].x < x_low) {
          x_low = points[i].x;
        }
        if (points[i].y > y_high) {
          y_high = points[i].y;
        }
        if (points[i].y < y_low) {
          y_low = points[i].y;
        }
      }

      this.setGradient(gradient, f1, f2, x_low, y_low, x_high - x_low, y_high -
        y_low);

      this.gctx.fill();
    };

  this.image = function (image, x, y, w, h) {
    this.gctx.drawImage(image, x, y, w, h);
  };

  this.gradient_rotate = function (rotate, gradient) {
    return gradient;
  };

  this.setGradient = function (gradient, f1, f2, x, y, w, h) {
    var gx0 = 0;
    var gy0 = 0;
    var gx1 = 0;
    var gy1 = 0;
    var gr = 0;

    switch (gradient) {
      case Glow.eGradient_HorizontalUp:
        gx0 = x;
        gy0 = y;
        gx1 = x;
        gy1 = y + h;
        break;
      case Glow.eGradient_HorizontalDown:
        gx0 = x;
        gy0 = y + h;
        gx1 = x;
        gy1 = y;
        break;
      case Glow.eGradient_HorizontalTube1:
        gx0 = x;
        gy0 = y + h;
        gx1 = x;
        gy1 = y;
        break;
      case Glow.eGradient_HorizontalTube2:
        gx0 = x;
        gy0 = y;
        gx1 = x;
        gy1 = y + h;
        break;
      case Glow.eGradient_VerticalLeft:
        gx0 = x;
        gy0 = y;
        gx1 = x + w;
        gy1 = y;
        break;
      case Glow.eGradient_VerticalRight:
        gx0 = x + w;
        gy0 = y;
        gx1 = x;
        gy1 = y;
        break;
      case Glow.eGradient_VerticalTube1:
        gx0 = x + w;
        gy0 = y;
        gx1 = x;
        gy1 = y;
        break;
      case Glow.eGradient_VerticalTube2:
        gx0 = x + w;
        gy0 = y;
        gx1 = x;
        gy1 = y;
        break;
      case Glow.eGradient_DiagonalUpperLeft:
        gx0 = x;
        gy0 = y;
        gx1 = x + w;
        gy1 = y + h;
        break;
      case Glow.eGradient_DiagonalLowerLeft:
        gx0 = x;
        gy0 = y + h;
        gx1 = x + w;
        gy1 = y;
        break;
      case Glow.eGradient_DiagonalUpperRight:
        gx0 = x + w;
        gy0 = y;
        gx1 = x;
        gy1 = y + h;
        break;
      case Glow.eGradient_DiagonalLowerRight:
        gx0 = x + w;
        gy0 = y + h;
        gx1 = x;
        gy1 = y;
        break;
      case Glow.eGradient_DiagonalUpTube:
        gx0 = x + w / 5;
        gy0 = y + h / 5;
        gx1 = x + w * 4 / 5;
        gy1 = y + h * 4 / 5;
        break;
      case Glow.eGradient_DiagonalDownTube:
        gx0 = x + w * 4 / 5;
        gy0 = y + h / 5;
        gx1 = x + w / 5;
        gy1 = y + h * 4 / 5;
        break;
      case Glow.eGradient_Globe:
        gx0 = x + 0.3 * w;
        gy0 = y + 0.3 * h;
        gx1 = x + w;
        gy1 = y + h;
        gr = Math.sqrt(w * w / 4 + h * h / 4);
        break;
      case Glow.eGradient_RadialCenter:
        gx0 = x + 0.5 * w;
        gy0 = y + 0.5 * h;
        gx1 = x + w;
        gy1 = y + h;
        gr = Math.sqrt(w * w / 4 + h * h / 4);
        break;
      case Glow.eGradient_RadialUpperLeft:
        gx0 = x;
        gy0 = y;
        gx1 = x + w;
        gy1 = y + h;
        gr = Math.sqrt(w * w + h * h);
        break;
      case Glow.eGradient_RadialLowerLeft:
        gx0 = x;
        gy0 = y + h;
        gx1 = x + w;
        gy1 = y;
        gr = Math.sqrt(w * w + h * h);
        break;
      case Glow.eGradient_RadialUpperRight:
        gx0 = x + w;
        gy0 = y;
        gx1 = x;
        gy1 = y + h;
        gr = Math.sqrt(w * w + h * h);
        break;
      case Glow.eGradient_RadialLowerRight:
        gx0 = x + w;
        gy0 = y + h;
        gx1 = x;
        gy1 = y;
        gr = Math.sqrt(w * w + h * h);
        break;
      default:
        break;
    }

    switch (gradient) {
      case Glow.eGradient_HorizontalUp:
      case Glow.eGradient_HorizontalDown:
      case Glow.eGradient_VerticalLeft:
      case Glow.eGradient_VerticalRight:
      case Glow.eGradient_DiagonalUpperLeft:
      case Glow.eGradient_DiagonalLowerLeft:
      case Glow.eGradient_DiagonalUpperRight:
      case Glow.eGradient_DiagonalLowerRight: {
        var linear = this.gctx.createLinearGradient(gx0, gy0, gx1, gy1);
        linear.addColorStop(0, this.getColor(f2));
        linear.addColorStop(1, this.getColor(f1));
        this.gctx.fillStyle = linear;
        break;
      }
      case Glow.eGradient_HorizontalTube1:
      case Glow.eGradient_VerticalTube1:
      case Glow.eGradient_DiagonalUpTube:
      case Glow.eGradient_DiagonalDownTube: {
        var linear = this.gctx.createLinearGradient(gx0, gy0, gx1, gy1);
        linear.addColorStop(0, this.getColor(f1));
        linear.addColorStop(0.5, this.getColor(f2));
        linear.addColorStop(1, this.getColor(f1));
        this.gctx.fillStyle = linear;
        break;
      }
      case Glow.eGradient_HorizontalTube2:
      case Glow.eGradient_VerticalTube2: {
        var linear = this.gctx.createLinearGradient(gx0, gy0, gx1, gy1);
        linear.addColorStop(0, this.getColor(f1));
        linear.addColorStop(0.3, this.getColor(f2));
        linear.addColorStop(0.95, this.getColor(f1));
        this.gctx.fillStyle = linear;
        break;
      }
      case Glow.eGradient_Globe:
      case Glow.eGradient_RadialCenter:
      case Glow.eGradient_RadialUpperLeft:
      case Glow.eGradient_RadialLowerLeft:
      case Glow.eGradient_RadialUpperRight:
      case Glow.eGradient_RadialLowerRight: {
        var radial = this.gctx.createRadialGradient(gx0, gy0, 1, gx0, gy0, gr);
        radial.addColorStop(0, this.getColor(f2));
        radial.addColorStop(1, this.getColor(f1));
        this.gctx.fillStyle = radial;
        break;
      }
      default:
        break;
    }
  };

  this.set_clip_rectangle = function (x1, y1, x2, y2) {
    if (this.clipCount > 0) {
      if (this.clipCount >= 10) {
        return 0;
      }
      if (x1 < this.clip_x1[this.clipCount - 1]) {
        x1 = this.clip_x1[this.clipCount - 1];
      }
      if (y1 < this.clip_y1[this.clipCount - 1]) {
        y1 = this.clip_y1[this.clipCount - 1];
      }
      if (x2 > this.clip_x2[this.clipCount - 1]) {
        x2 = this.clip_x2[this.clipCount - 1];
      }
      if (y2 > this.clip_y2[this.clipCount - 1]) {
        y2 = this.clip_y2[this.clipCount - 1];
      }
    }

    this.clip_x1[this.clipCount] = x1;
    this.clip_y1[this.clipCount] = y1;
    this.clip_x2[this.clipCount] = x2;
    this.clip_y2[this.clipCount] = y2;
    this.clipCount++;

    this.gctx.restore();
    this.gctx.save();
    this.gctx.beginPath();
    this.gctx.rect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
    this.gctx.clip();

    return 1;
  };

  this.reset_clip_rectangle = function () {
    if (this.clipCount === 0) {
      console.log("Clip mismatch");
      return;
    }
    this.clipCount--;
    if (this.clipCount > 0) {
      this.gctx.restore();
      this.gctx.save();
      this.gctx.beginPath();
      this.gctx.rect(this.clip_x1[this.clipCount - 1],
        this.clip_y1[this.clipCount - 1],
        (this.clip_x2[this.clipCount - 1] - this.clip_x1[this.clipCount - 1]),
        (this.clip_y2[this.clipCount - 1] - this.clip_y1[this.clipCount - 1]));
      this.gctx.clip();
    } else {
      this.gctx.restore();
      this.gctx.save();
    }
  };

  this.get_clip_rectangle = function () {
    if (this.clipCount === 0) {
      return null;
    }

    var rect = new GlowRectangle();
    rect.x1 = this.clip_x1[this.clipCount - 1];
    rect.y1 = this.clip_y1[this.clipCount - 1];
    rect.x2 = this.clip_x2[this.clipCount - 1];
    rect.y2 = this.clip_y2[this.clipCount - 1];
    return rect;
  };

  this.set_customcolors = function (cc) {
    this.customcolors[0] = cc;
    this.customcolors_cnt = 1;
  };

  this.push_customcolors = function (cc) {
    if (this.customcolors_cnt > this.CUSTOMCOLORS_STACK_SIZE) {
      console.log("** Max number custom colors exceede");
      return;
    }

    for (var i = this.customcolors_cnt; i > 0; i--) {
      this.customcolors[i] = this.customcolors[i - 1];
    }
    this.customcolors[0] = cc;
    this.customcolors_cnt++;
  };

  this.pop_customcolors = function () {
    if (this.customcolors_cnt <= 0) {
      // console.log( "** Customcolor stack disorder");
    }
    for (var i = 0; i < this.customcolors_cnt - 1; i++) {
      this.customcolors[i] = this.customcolors[i + 1];
    }
    this.customcolors_cnt--;
  };

  this.get_customcolors = function () {
    if (this.customcolors_cnt === 0) {
      return null;
    }
    return this.customcolors[0];
  };
}

function GrowCtx(ctx) {
  this.appl = null;
  this.debug = false;
  this.antiAliasing = 0;

  this.mw = new GrowCtxWindow();
  this.x_right = 0;
  this.x_left = 0;
  this.y_high = 0;
  this.y_low = 0;
  this.name = null;
  this.version = 0;
  this.background_color = 0;
  this.dynamicsize = 0;
  this.arg_cnt = 0;
  this.argname = [];
  this.argtype = [];
  this.x0 = 0;
  this.y0 = 0;
  this.x1 = 0;
  this.y1 = 0;
  this.path_cnt = 0;
  this.path = [];
  this.dyn_type1 = 0;
  this.dyn_type2 = 0;
  this.dyn_action_type1 = 0;
  this.dyn_action_type2 = 0;
  this.dyn_color = [];
  this.dyn_attr = [];
  this.slider = 0;
  this.subgraph = 0;
  this.scantime = 0;
  this.fast_scantime = 0;
  this.animation_scantime = 0;
  this.java_width = 0;
  this.background_image = null;
  this.background_tiled = 0;
  this.double_buffered = 0;
  this.cycle = 0;
  this.mb3_action = 0;
  this.translate_on = 0;
  this.input_focus_mark = 0;
  this.hot_indication = 0;
  this.app_motion = Glow.eAppMotion_Both;
  this.color_theme = null;
  this.default_color_theme = "pwr_colortheme1.pwgc";
  this.userdata = null;
  this.callback_object = null;
  this.callback_object_type = 0;
  this.hot_mode = 0;
  this.hot_found = 0;
  this.restriction_object = null;
  this.move_restriction = 0;
  this.restriction_max_limit = 0;
  this.restriction_min_limit = 0;
  this.recursive_trace = 0;
  this.nodraw = 0;
  this.owner = "";
  this.customcolors = new GlowCustomColors();

  this.a = new GlowArray(this);
  this.a_nc = new GlowArray(this);
  this.a_cc = new GlowArray(this);

  if (ctx == null) {
    this.gdraw = new GlowDraw(this);
  } else {
    this.gdraw = ctx.gdraw;
  }

  this.name = null;
  this.sliderActive = 0;
  this.sliderObject = null;
  this.node_move_last_x = 0;
  this.node_move_last_y = 0;
  this.slider_cursor_offset = 0;
  this.trace_started = false;
  this.drawing = false;

  this.open = function (lines, row) {
    var end = false;
    var zoom_y_found = false;
    var grow_loaded = false;

    for (var i = row; i < lines.length; i++) {
      if (lines[i].length > 2 && lines[i].substring(0, 2) === "0!") {
        continue;
      }
      if (lines[i].length > 1 && lines[i].substring(0, 1) === "!") {
        continue;
      }

      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.debug) {
        console.log("ctx : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_Ctx:
          break;
        case Glow.eSave_Ctx_zoom_factor_x:
          this.mw.zoom_factor_x = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Ctx_zoom_factor_y:
          this.mw.zoom_factor_y = parseFloat(tokens[1]);
          zoom_y_found = true;
          break;
        case Glow.eSave_Ctx_base_zoom_factor:
          this.mw.base_zoom_factor = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Ctx_offset_x:
          this.mw.offset_x = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Ctx_offset_y:
          this.mw.offset_y = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Ctx_nav_zoom_factor_x:
        case Glow.eSave_Ctx_nav_zoom_factor_y:
        case Glow.eSave_Ctx_print_zoom_factor:
        case Glow.eSave_Ctx_nav_offset_x:
        case Glow.eSave_Ctx_nav_offset_y:
          break;
        case Glow.eSave_Ctx_x_right:
          this.x_right = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Ctx_x_left:
          this.x_left = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Ctx_y_high:
          this.y_high = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Ctx_y_low:
          this.y_low = parseFloat(tokens[1]);
          break;
        case Glow.eSave_Ctx_nav_rect_ll_x:
        case Glow.eSave_Ctx_nav_rect_ll_y:
        case Glow.eSave_Ctx_nav_rect_ur_x:
        case Glow.eSave_Ctx_nav_rect_ur_y:
        case Glow.eSave_Ctx_nav_rect_hot:
          break;
        case Glow.eSave_Ctx_name:
          this.name = tokens[1];
          break;
        case Glow.eSave_Ctx_user_highlight:
        case Glow.eSave_Ctx_grid_size_x:
        case Glow.eSave_Ctx_grid_size_y:
        case Glow.eSave_Ctx_grid_on:
        case Glow.eSave_Ctx_draw_delta:
        case Glow.eSave_Ctx_refcon_width:
        case Glow.eSave_Ctx_refcon_height:
        case Glow.eSave_Ctx_refcon_textsize:
        case Glow.eSave_Ctx_refcon_linewidth:
          break;
        case Glow.eSave_Ctx_version:
          this.version = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Ctx_hot_indication:
          this.hot_indication = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Ctx_tiptext_size:
          break;
        case Glow.eSave_Ctx_app_motion:
          this.app_motion = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_Ctx_color_theme:
          if (tokens.length > 1) {
            this.color_theme = tokens[1];
          }
          break;
        case Glow.eSave_Ctx_grow:
          i = this.open_grow(lines, i + 1);
          grow_loaded = true;
          break;
        case Glow.eSave_Ctx_a_nc:
          i = this.a_nc.open(lines, i + 1);
          break;
        case Glow.eSave_Ctx_a_cc:
          i = this.a_cc.open(lines, i + 1);
          break;
        case Glow.eSave_Ctx_a:
          i = this.a.open(lines, i + 1);
          break;
        case Glow.eSave_Ctx_comment:
          i = this.open_comment(lines, i + 1);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        case Glow.eSave_Comment:
          break;
        default:
          console.log("Syntax error in GlowCtx", key);
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
  };

  this.open_comment = function (lines, row) {
    var i;
    for (i = row; i < lines.length; i++) {
      if (!lines[i].substring(0, 1) === "!") {
        break;
      }
      if (lines[i].substring(0, 3) === "!*/") {
        break;
      }
    }
    return i;
  };

  this.open_grow = function (lines, row) {
    var end = false;
    var i, j;

    for (i = row; i < lines.length; i++) {
      var tokens = lines[i].split(' ');
      var key = parseInt(tokens[0], 10);

      if (this.debug) {
        console.log("ctx : " + lines[i]);
      }

      switch (key) {
        case Glow.eSave_GrowCtx:
          break;
        case Glow.eSave_GrowCtx_conpoint_num_cnt:
        case Glow.eSave_GrowCtx_objectname_cnt:
        case Glow.eSave_GrowCtx_name:
          this.name = tokens[1];
          break;
        case Glow.eSave_GrowCtx_background_color:
          this.background_color = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dynamicsize:
          this.dynamicsize = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dynamic:
          if (this.dynamicsize > 0) {
            i += this.dynamicsize;
          }
          break;
        case Glow.eSave_GrowCtx_arg_cnt:
          this.arg_cnt = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_argname:
          for (j = 0; j < this.arg_cnt; j++) {
            this.argname[j] = tokens[1];
          }
          break;
        case Glow.eSave_GrowCtx_argtype:
          for (j = 0; j < this.arg_cnt; j++) {
            this.argtype[j] = parseInt(tokens[1], 10);
          }
          break;
        case Glow.eSave_GrowCtx_x0:
          this.x0 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowCtx_y0:
          this.y0 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowCtx_x1:
          this.x1 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowCtx_y1:
          this.y1 = parseFloat(tokens[1]);
          break;
        case Glow.eSave_End:
          end = true;
          break;
        case Glow.eSave_GrowCtx_path_cnt:
          this.path_cnt = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_path:
          for (j = 0; j < this.path_cnt; j++) {
            i++;
            this.path[j] = lines[i];
          }
          break;
        case Glow.eSave_GrowCtx_dyn_type1:
          this.dyn_type1 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dyn_type2:
          this.dyn_type2 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dyn_action_type1:
          this.dyn_action_type1 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dyn_action_type2:
          this.dyn_action_type2 = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dyn_color1:
          this.dyn_color[0] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dyn_color2:
          this.dyn_color[1] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dyn_color3:
          this.dyn_color[2] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dyn_color4:
          this.dyn_color[3] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dyn_attr1:
          this.dyn_attr[0] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dyn_attr2:
          this.dyn_attr[1] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dyn_attr3:
          this.dyn_attr[2] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_dyn_attr4:
          this.dyn_attr[3] = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_no_con_obstacle:
          break;
        case Glow.eSave_GrowCtx_slider:
          this.slider = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_subgraph:
          this.subgraph = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_java_name:
        case Glow.eSave_GrowCtx_is_javaapplet:
        case Glow.eSave_GrowCtx_is_javaapplication:
        case Glow.eSave_GrowCtx_next_subgraph:
        case Glow.eSave_GrowCtx_animation_count:
          break;
        case Glow.eSave_GrowCtx_scantime:
          this.scantime = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowCtx_fast_scantime:
          this.fast_scantime = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowCtx_animation_scantime:
          this.animation_scantime = parseFloat(tokens[1]);
          break;
        case Glow.eSave_GrowCtx_java_width:
          this.java_width = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_background_image:
          if (tokens.length > 1) {
            this.background_image = tokens[1];
          }
          break;
        case Glow.eSave_GrowCtx_background_tiled:
          this.background_tiled = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_double_buffered:
          this.double_buffered = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_cycle:
          this.cycle = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_mb3_action:
          this.mb3_action = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_translate_on:
          this.translate_on = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_input_focus_mark:
          this.input_focus_mark = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_recursive_trace:
          this.recursive_trace = parseInt(tokens[1], 10);
          break;
        case Glow.eSave_GrowCtx_userdata_cb:
          if (this.appl !== null) {
            var ret = this.appl.growUserdataOpen(lines, i + 1, this,
              Glow.eUserdataCbType_Ctx);
            this.userdata = ret.userdata;
            i = ret.row;
          }
          break;
        case Glow.eSave_GrowCtx_bitmap_fonts:
          break;
        case Glow.eSave_GrowCtx_customcolors:
          i = this.customcolors.open(lines, i + 1);
          break;
        default:
          console.log("Syntax error in GrowCtx open_growctx", key);
          break;
      }
      if (end) {
        break;
      }
    }
    this.mw.offset_x = this.x0 * this.mw.zoom_factor_x;
    this.mw.offset_y = this.y0 * this.mw.zoom_factor_y;

    return i;
  };

  this.get_nodeclass_from_name = function (nc_name) {
    for (var i = 0; i < this.a_nc.size(); i++) {
      if (nc_name === this.a_nc.get(i).nc_name) {
        return this.a_nc.get(i);
      }
    }
    return null;
  };

  this.get_conclass_from_name = function (cc_name) {
    for (var i = 0; i < this.a_cc.size(); i++) {
      if (cc_name === this.a_cc.get(i).cc_name) {
        return this.a_cc.get(i);
      }
    }
    return null;
  };

  this.event_handler = function (e) {
    var sts = 0;
    var x = e.x * this.mw.zoom_factor_x - this.mw.offset_x;
    var y = e.y * this.mw.zoom_factor_y - this.mw.offset_y;
    var se;

    this.callback_object = null;
    this.callback_object_type = Glow.eObjectType_NoObject;

    for (var i = this.a.size() - 1; i >= 0; i--) {
      sts = this.a.get(i).event_handler(e, e.x, e.y);
      if (sts === Glow.GLOW__NO_PROPAGATE) {
        break;
      }
      if (sts === Glow.GLOW__TERMINATED) {
        return sts;
      }
      if (sts === 1) {
        break;
      }
    }

    switch (e.event) {
      case Glow.eEvent_MB1Down:
        if (sts === 1 && this.callback_object !== null &&
          this.callback_object.type() === Glow.eObjectType_GrowSlider) {
          this.sliderActive = true;
          this.sliderObject = this.callback_object;

          se = new GlowEvent();
          se.event = Glow.eEvent_SliderMoveStart;
          se.type = Glow.eEventType_Object;
          se.x = e.x;
          se.y = e.y;
          se.object = this.callback_object;
          se.object_type = this.callback_object.type();
          this.appl.event_handler(se);

          if (this.restriction_object !== null) {
            var g = this.restriction_object.measure();
            if (this.move_restriction === Glow.eMoveRestriction_VerticalSlider) {
              this.slider_cursor_offset = g.ll_y - e.y;
            } else {
              this.slider_cursor_offset = g.ll_x - e.x;
            }
          }
          this.node_move_last_x = e.x;
          this.node_move_last_y = e.y;
        }
        break;
      case Glow.eEvent_MB1Up:
        if (this.sliderActive) {
          if (this.restriction_object !== null) {

            se = new GlowEvent();
            se.event = Glow.eEvent_SliderMoveEnd;
            se.type = Glow.eEventType_Object;
            se.x = e.x;
            se.y = e.y;
            se.object = this.restriction_object;
            se.object_type = this.restriction_object.type();
            this.appl.event_handler(se);

            this.restriction_object = null;
          }
          this.sliderObject = null;
          this.sliderActive = false;
        }
        break;
      case Glow.eEvent_ButtonMotion:
        if (this.sliderActive && this.restriction_object !== null) {

          var move_x, move_y;
          var cursor_y, cursor_x;

          switch (this.move_restriction) {
            case Glow.eMoveRestriction_VerticalSlider: {
              cursor_y = e.y;
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
              se.event = Glow.eEvent_SliderMoved;
              se.type = Glow.eEventType_Object;
              se.x = e.x;
              se.y = this.node_move_last_y + move_y;
              se.object = this.restriction_object;
              se.object_type = this.restriction_object.type();
              this.appl.event_handler(se);

              this.node_move_last_x = e.x;
              this.node_move_last_y = e.y;
              break;
            }
            case Glow.eMoveRestriction_HorizontalSlider: {
              cursor_x = e.x;
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
              se.event = Glow.eEvent_SliderMoved;
              se.type = Glow.eEventType_Object;
              se.x = this.node_move_last_x + move_x;
              se.y = e.y;
              se.object = this.restriction_object;
              se.object_type = this.restriction_object.type();
              this.appl.event_handler(se);

              this.node_move_last_x = e.x;
              this.node_move_last_y = e.y;
              break;
            }
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
      e.object_type = this.callback_object_type;
      this.appl.event_handler(e);
    }
    return sts;
  };

  this.getWidth = function () {
    return this.gdraw.canvas.width;
  };
  this.getHeight = function () {
    return this.gdraw.canvas.height;
  };

  this.setNodraw = function () {
    this.nodraw++;
  };
  this.resetNodraw = function () {
    if (this.nodraw > 0) {
      this.nodraw--;
    }
  };
  this.getNodraw = function () {
    return this.nodraw;
  };

  this.draw = function () {
    this.tdraw(null, 0, 0, null, null);
  };

  this.rdraw = function (ll_x, ll_y, ur_x, ur_y) {
    // TODO
    if (this.drawing) {
      return;
    }

    var i;

    for (i = 0; i < this.a.size(); i++) {
      if (this.a.get(i).type() === Glow.eObjectType_Con) {
        this.a.get(i).draw();
      }
    }
    for (i = 0; i < this.a.size(); i++) {
      if (this.a.get(i).type() !== Glow.eObjectType_Con) {
        this.a.get(i).draw();
      }
    }
  };

  this.tdraw = function (t, highlight, hot, color, colornode) {
    var drawing = true;
    var i;

    // Draw background color
    this.gdraw.fill_rect(0, 0, this.gdraw.canvas.width,
      this.gdraw.canvas.height, this.background_color);
    // Draw connections
    for (i = 0; i < this.a.size(); i++) {
      if (this.a.get(i).type() === Glow.eObjectType_Con) {
        this.a.get(i).draw();
      }
    }
    // Draw nodes
    for (i = 0; i < this.a.size(); i++) {
      if (this.a.get(i).type() !== Glow.eObjectType_Con) {
        this.a.get(i).draw();
      }
    }
    drawing = false;
  };

  this.traceConnect = function () {
    var sts;
    this.nodraw++;
    for (var i = 0; i < this.a.size(); i++) {
      if (this.a.get(i).type() === Glow.eObjectType_GrowNode ||
        this.a.get(i).type() === Glow.eObjectType_GrowGroup ||
        this.a.get(i).type() === Glow.eObjectType_GrowBar ||
        this.a.get(i).type() === Glow.eObjectType_GrowBarArc ||
        this.a.get(i).type() === Glow.eObjectType_GrowTrend ||
        this.a.get(i).type() === Glow.eObjectType_GrowTable ||
        this.a.get(i).type() === Glow.eObjectType_GrowSlider ||
        this.a.get(i).type() === Glow.eObjectType_GrowWindow ||
        this.a.get(i).type() === Glow.eObjectType_GrowFolder ||
        this.a.get(i).type() === Glow.eObjectType_GrowXYCurve ||
        this.a.get(i).type() === Glow.eObjectType_GrowPie ||
        this.a.get(i).type() === Glow.eObjectType_GrowBarChart ||
        this.a.get(i).type() === Glow.eObjectType_GrowToolbar ||
        this.a.get(i).type() === Glow.eObjectType_GrowAxis ||
        this.a.get(i).type() === Glow.eObjectType_GrowAxisArc) {
        this.appl.traceConnect(this.a.get(i));
        if (this.a.get(i).type() === Glow.eObjectType_GrowGroup) {
          for (var j = 0; j < this.a.get(i).nc.a.size(); j++) {
            if (this.a.get(i).nc.a.get(j).type() === Glow.eObjectType_GrowNode ||
              this.a.get(i).nc.a.get(j).type() === Glow.eObjectType_GrowGroup) {
              this.appl.traceConnect(this.a.get(i).nc.a.get(j));
            }
          }

        } else if (this.a.get(i).type() === Glow.eObjectType_GrowWindow ||
          this.a.get(i).type() === Glow.eObjectType_GrowFolder) {
          this.a.get(i).traceConnect();
        }
      }
    }
    this.nodraw--;
    this.trace_started = true;
  };

  this.traceDisconnect = function () {
    var sts;
    this.trace_started = false;
    for (var i = 0; i < this.a.size(); i++) {
      if (this.a.get(i).type() === Glow.eObjectType_GrowNode ||
        this.a.get(i).type() === Glow.eObjectType_GrowGroup ||
        this.a.get(i).type() === Glow.eObjectType_GrowBar ||
        this.a.get(i).type() === Glow.eObjectType_GrowBarArc ||
        this.a.get(i).type() === Glow.eObjectType_GrowTrend ||
        this.a.get(i).type() === Glow.eObjectType_GrowTable ||
        this.a.get(i).type() === Glow.eObjectType_GrowSlider ||
        this.a.get(i).type() === Glow.eObjectType_GrowWindow ||
        this.a.get(i).type() === Glow.eObjectType_GrowFolder ||
        this.a.get(i).type() === Glow.eObjectType_GrowXYCurve ||
        this.a.get(i).type() === Glow.eObjectType_GrowPie ||
        this.a.get(i).type() === Glow.eObjectType_GrowBarChart ||
        this.a.get(i).type() === Glow.eObjectType_GrowAxis ||
        this.a.get(i).type() === Glow.eObjectType_GrowAxisArc) {
        this.appl.traceDisconnect(this.a.get(i));
        if (this.a.get(i).type() === Glow.eObjectType_GrowGroup) {
          for (var j = 0; j < this.a.get(i).nc.a.size(); j++) {
            if (this.a.get(i).nc.a.get(j).type() === Glow.eObjectType_GrowNode ||
              this.a.get(i).nc.a.get(j).type() === Glow.eObjectType_GrowGroup) {
              this.appl.traceDisconnect(this.a.get(i).nc.a.get(j));
            }
          }
        } else if (this.a.get(i).type() === Glow.eObjectType_GrowWindow ||
          this.a.get(i).type() === Glow.eObjectType_GrowFolder) {
          this.a.get(i).traceDisconnect();
        }
      }
    }
  };

  this.traceScan = function () {
    var sts;

    for (var i = 0; i < this.a.size(); i++) {
      if (this.a.get(i).type() === Glow.eObjectType_GrowNode ||
        this.a.get(i).type() === Glow.eObjectType_GrowGroup ||
        this.a.get(i).type() === Glow.eObjectType_GrowBar ||
        this.a.get(i).type() === Glow.eObjectType_GrowBarArc ||
        this.a.get(i).type() === Glow.eObjectType_GrowTrend ||
        this.a.get(i).type() === Glow.eObjectType_GrowTable ||
        this.a.get(i).type() === Glow.eObjectType_GrowSlider ||
        this.a.get(i).type() === Glow.eObjectType_GrowWindow ||
        this.a.get(i).type() === Glow.eObjectType_GrowFolder ||
        this.a.get(i).type() === Glow.eObjectType_GrowXYCurve ||
        this.a.get(i).type() === Glow.eObjectType_GrowPie ||
        this.a.get(i).type() === Glow.eObjectType_GrowBarChart ||
        this.a.get(i).type() === Glow.eObjectType_GrowAxis ||
        this.a.get(i).type() === Glow.eObjectType_GrowAxisArc) {
        this.appl.traceScan(this.a.get(i));
        if (this.a.get(i).type() === Glow.eObjectType_GrowGroup) {
          for (var j = 0; j < this.a.get(i).nc.a.size(); j++) {
            if (this.a.get(i).nc.a.get(j).type() === Glow.eObjectType_GrowNode ||
              this.a.get(i).nc.a.get(j).type() === Glow.eObjectType_GrowGroup) {
              this.appl.traceScan(this.a.get(i).nc.a.get(j));
            }
          }

        } else if (this.a.get(i).type() === Glow.eObjectType_GrowWindow ||
          this.a.get(i).type() === Glow.eObjectType_GrowFolder) {
          this.a.get(i).traceScan();
        }
      }
    }
  };

  this.traceStarted = function () {
    return this.trace_started;
  };

  this.get_node_from_name = function (name) {
    for (var i = 0; i < this.a.size(); i++) {
      if (this.a.get(i).type() === Glow.eObjectType_GrowNode) {
        if (name === this.a.get(i).n_name) {
          return this.a.get(i);
        }
      }
    }
    return null;
  };

  this.get_object_group = function (object) {
    var sts;
    var group;

    for (var i = 0; i < this.a.size(); i++) {
      if (this.a.get(i).type() === Glow.eObjectType_GrowGroup) {
        group = this.a.get(i).get_object_group(object);
        if (group != null) {
          return group;
        }
      }
    }
    return null;
  };

  this.insert = function (e) {
    this.a.add(e);
  };

  this.remove = function (e) {
    this.a.remove(e);
  };

  this.register_callback_object = function (type, o) {
    this.callback_object = o;
    this.callback_object_type = type;
  };
  this.send_menu_callback = function (object, item, event, x, y) {
    /* Send a host request callback */
    var e = new GlowEventMenu();

    e.event = event;
    e.type = Glow.eEventType_Menu;
    e.x = x;
    e.y = y;
    e.object_type = object.type();
    e.object = object;
    e.item = item;
    this.appl.event_handler(e);

    return 1;
  };
  this.send_table_callback = function (object, event, x, y, column, row) {
    /* Send a table callback */
    var e = new GlowEventTable();

    e.event = event;
    e.type = Glow.eEventType_Table;
    e.x = x;
    e.y = y;
    e.object_type = object.type();
    e.object = object;
    e.column = column;
    e.row = row;
    this.appl.event_handler(e);

    return 1;
  };
  this.send_toolbar_callback = function (object, category, idx, event, x, y) {
    /* Send a toolbar callback */
    var e = new GlowEventToolbar();

    e.event = event;
    e.type = Glow.eEventType_Toolbar;
    e.x = x;
    e.y = y;
    e.object_type = object.type();
    e.object = object;
    e.category = category;
    e.idx = idx;
    this.appl.event_handler(e);

    return 1;
  };
  this.setOwner = function (owner) {
    this.owner = owner;
  };
  this.getOwner = function () {
    return this.owner;
  };

  this.get_object_list = function () {
    return this.a;
  };

  this.getBackgroundObjectLimits = function (type, x, y) {
    var sts = 0;
    var b = new GlowBackgroundObject();

    for (var i = 0; i < this.a.size(); i++) {
      sts = this.a.get(i).get_background_object_limits(null, type, x, y, b);
      if ((sts & 1) !== 0) {
        break;
      }
    }
    b.sts = sts;
    return b;
  };

  this.setMoveRestrictions =
    function (restriction, max_limit, min_limit, object) {
      this.move_restriction = restriction;
      this.restriction_max_limit = max_limit;
      this.restriction_min_limit = min_limit;
      this.restriction_object = object;
    };

  this.findByName = function (name) {
    var uname = name.toUpperCase();
    for (var i = 0; i < this.a.size(); i++) {
      if (this.a.get(i).n_name.toUpperCase() === uname) {
        return this.a.get(i);
      }
    }
    return null;
  };

  this.setSubwindowSource = function (name, source, owner) {
    var e = this.findByName(name);
    if (e == null || e.type() !== Glow.eObjectType_GrowWindow) {
      console.log("Window " + name + " not found " + e);
      return;
    }

    e.setSource(source, owner);
  };

  this.getSliderActive = function () {
    return this.sliderActive;
  };

  this.setSliderActive = function (active) {
    this.sliderActive = active;
  };

  this.loadSubgraph = function (fname) {
    // TODO
    return 0;
  }
}

function GrowFrame(appl) {
  var self = this;
  this.appl = appl;

  this.get_nogdh = function () {
    var vars = window.location.search.substring(1).split('&');
    return vars.length > 2 && vars[2].substring(4) === "no";

  };
  this.nogdh = this.get_nogdh();
  console.log("Nogdh:", this.nogdh);

  this.appl.ctx = new GrowCtx(null);
  this.appl.baseCtx = this.appl.ctx;
  this.appl.ctx.appl = appl;
  this.timer = null;
  this.vars_object = null;
  this.tmp_ctx = null;

  this.readGrowWeb = function (fname, read_cb) {
    var n = window.location.href.lastIndexOf('/');
    var target = window.location.href.substring(0, n + 1);
    target += fname;

    // console.log("Target: ", target);

    var req = new XMLHttpRequest();
    req.read_cb = read_cb;

    req.open("GET", target, true);
    req.addEventListener("load", function () {
      var lines = req.responseText.split('\n');
      req.read_cb(lines, 0);
    });
    req.send(null);
  };


  this.read_func = function (ctx, lines, row) {
    if (ctx === self.appl.baseCtx) {
      if (lines[0].substring(3, 15) === "DefaultWidth" &&
        lines[1].substring(3, 16) === "DefaultHeight") {
        var width = parseInt(lines[0].substring(16), 10);
        var height = parseInt(lines[1].substring(17), 10);
        ctx.gdraw.canvas.width = width;
        ctx.gdraw.canvas.height = height;
      }
    }

    ctx.open(lines, row);


    ctx.draw();
  };

  this.register_events = function (ctx) {
    self.appl.ctx.gdraw.canvas.addEventListener("click", function (event) {
      var y = event.pageY - self.appl.ctx.gdraw.offset_top;
      var x = event.pageX - 10; // Correction for left offset
      self.event_handler(x, y, Glow.eEvent_MB1Click);
    });

    self.appl.ctx.gdraw.canvas.addEventListener("mousedown", function (event) {
      var y = event.pageY - self.appl.ctx.gdraw.offset_top;
      var x = event.pageX - 10;
      self.event_handler(x, y, Glow.eEvent_MB1Down);
    });
    self.appl.ctx.gdraw.canvas.addEventListener("mouseup", function (event) {
      var y = event.pageY - self.appl.ctx.gdraw.offset_top;
      var x = event.pageX - 10;
      self.event_handler(x, y, Glow.eEvent_MB1Up);
    });
    self.appl.ctx.gdraw.canvas.addEventListener("mousemove", function (event) {
      var y = event.pageY - self.appl.ctx.gdraw.offset_top;
      var x = event.pageX - 10;
      var button_motion = 0;
      if (event.buttons == null) {
        button_motion = (event.witch === 1);
      } else {
        button_motion = (event.buttons === 1);
      }
      if (button_motion) {
        self.event_handler(x, y, Glow.eEvent_ButtonMotion);
      } else {
        self.event_handler(x, y, Glow.eEvent_CursorMotion);
      }
    });
  };

  this.readGrowWindow = function (fname, read_cb) {
    var nctx = new GrowCtx(this.appl.baseCtx);
    nctx.appl = this.appl;

    var n = window.location.href.lastIndexOf('/');
    var target = window.location.href.substring(0, n + 1);
    target += fname;

    // console.log("Target: ", target);

    var req = new XMLHttpRequest();
    req.read_cb = read_cb;

    req.open("GET", target, true);
    req.addEventListener("load", function () {
      var lines = req.responseText.split('\n');
      req.read_cb(lines, 0);
    });
    req.send(null);

    return nctx;
  };

  this.grow_open = function () {
    console.log("grow_open");
  };

  this.grow_scan = function () {
    // self.appl.ctx.scan();
  };

  this.grow_cyclic = function () {
    // self.appl.ctx.gdh.getRefObjectInfoAll( self.flow_scan);
    // self.appl.ctx.draw();
    // self.timer = setTimeout( self.flow_cyclic, 2000);
  };

  this.grow_close = function () {
    console.log("Close function", self.timer);
    // clearTimeout(self.timer);
    // for ( var i in self.appl.ctx.gdh.pending)
    //  delete self.ctx.gdh.pending[i];
  };

  this.get_filename = function () {
    return window.location.search.substring(1).split('&')[0].substring(6) + ".pwg";
  };

  this.get_instance = function () {
    var query = window.location.search.substring(1);

    var vars = query.split('&');
    if (vars.length > 1) {
      return vars[1].substring(9);
    }
    return "";
  };

  this.event_handler = function (x, y, event) {
    var e = new GlowEvent();
    e.event = event;
    e.x = (x + this.appl.ctx.mw.offset_x) / this.appl.ctx.mw.zoom_factor_x;
    e.y = (y + this.appl.ctx.mw.offset_y) / this.appl.ctx.mw.zoom_factor_y;
    this.appl.ctx.event_handler(e);
  };

  this.appl.ctx.setOwner(this.get_instance());
}
