
package jpwr.jop;
import java.beans.*;

/**
 * Title:
 * Description:
 * Copyright:    Copyright (c) 2000
 * Company:
 * @author 	 CS
 * @version 1.0
 */

public class GeColorToneEditor extends PropertyEditorSupport {

  public GeColorToneEditor() {
  }
  private static String[] tagStrings = {
	"NoTone",
	"ToneGray",
	"ToneYellow",
	"ToneGold",
	"ToneOrange",
	"ToneRed",
	"ToneMagenta",
	"ToneBlue",
	"ToneSeablue",
	"ToneGreen",
	"Grey5",
	"Grey10",
	"Grey15",
	"Grey30",
	"Grey50",
	"Grey7",
	"Grey12",
	"Grey25",
	"Grey45",
	"Grey65",
	"LightYellow0",
	"LightYellow1",
	"LightYellow2",
	"LightYellow3",
	"LightYellow4",
	"yellow0",
	"yellow1",
	"yellow2",
	"yellow3",
	"yellow4",
	"Lightgoldenrod0",
	"Lightgoldenrod1",
	"Lightgoldenrod2",
	"Lightgoldenrod3",
	"Lightgoldenrod4",
	"gold00",
	"gold1",
	"gold2",
	"gold3",
	"gold4",
	"LightOrange",
	"NavajoWhite",
	"LightSalmon1",
	"LightSalmon3",
	"LightSalmon4",
	"DarkOrange",
	"DarkOrange0",
	"DarkOrange1",
	"DarkOrange3",
	"DarkOrange4",
	"MistyRose1",
	"MistyRose2",
	"LightPink",
	"LightPink3",
	"LightPink4",
	"IndianRed0",
	"IndianRed1",
	"firebrick1",
	"firebrick3",
	"firebrick4",
	"Plum1 ",
	"Plum1",
	"Plum2",
	"Plum3",
	"Plum4",
	"Magenta00",
	"Magenta0",
	"Magenta1",
	"Magenta3",
	"Magenta4",
	"LightSteelBlue1",
	"LightSteelBlue1",
	"LightSteelBlue2",
	"LightSteelBlue3",
	"LightSteelBlue4",
	"LightCyan",
	"PowderBlue",
	"SteelBlue1",
	"SteelBlue",
	"blue4",
	"aquamarine0",
	"aquamarine1",
	"aquamarine2",
	"aquamarine3",
	"aquamarine4",
	"turqoise0",
	"turqoise1",
	"turqoise2",
	"turqoise3",
	"turqoise4",
	"DarkOliveGreen1",
	"DarkOliveGreen2",
	"DarkOliveGreen3",
	"DarkOliveGreen4",
	"DarkOliveGreen5",
	"chartreuse",
	"chartreuse2",
	"chartreuse3",
	"chartreuse4",
	"DarkGreen",
	};
  public String[] getTags() {
    return tagStrings;
  }
  public String getJavaInitializationString() {
    return java.lang.String.valueOf(((Number)getValue()).intValue());
  }
  public void setAsText(String s) throws IllegalArgumentException {
    if (s.equals("NoTone")) setValue( new Integer(GeColor.COLOR_TONE_NO));
    else if (s.equals("ToneGray")) setValue( new Integer(GeColor.COLOR_TONE_GRAY));
    else if (s.equals("ToneYellow")) setValue( new Integer(GeColor.COLOR_TONE_YELLOW));
    else if (s.equals("ToneGold")) setValue( new Integer(GeColor.COLOR_TONE_GOLD));
    else if (s.equals("ToneOrange")) setValue( new Integer(GeColor.COLOR_TONE_ORANGE));
    else if (s.equals("ToneRed")) setValue( new Integer(GeColor.COLOR_TONE_RED));
    else if (s.equals("ToneMagenta")) setValue( new Integer(GeColor.COLOR_TONE_MAGENTA));
    else if (s.equals("ToneBlue")) setValue( new Integer(GeColor.COLOR_TONE_BLUE));
    else if (s.equals("ToneSeablue")) setValue( new Integer(GeColor.COLOR_TONE_SEABLUE));
    else if (s.equals("ToneGreen")) setValue( new Integer(GeColor.COLOR_TONE_GREEN));
    else if (s.equals("Grey5")) setValue( new Integer(GeColor.COLOR_11));
    else if (s.equals("Grey10")) setValue( new Integer(GeColor.COLOR_12));
    else if (s.equals("Grey15")) setValue( new Integer(GeColor.COLOR_13));
    else if (s.equals("Grey30")) setValue( new Integer(GeColor.COLOR_14));
    else if (s.equals("Grey50")) setValue( new Integer(GeColor.COLOR_15));
    else if (s.equals("Grey7")) setValue( new Integer(GeColor.COLOR_16));
    else if (s.equals("Grey12")) setValue( new Integer(GeColor.COLOR_17));
    else if (s.equals("Grey25")) setValue( new Integer(GeColor.COLOR_18));
    else if (s.equals("Grey45")) setValue( new Integer(GeColor.COLOR_19));
    else if (s.equals("Grey65")) setValue( new Integer(GeColor.COLOR_20));
    else if (s.equals("LightYellow0")) setValue( new Integer(GeColor.COLOR_21));
    else if (s.equals("LightYellow1")) setValue( new Integer(GeColor.COLOR_22));
    else if (s.equals("LightYellow2")) setValue( new Integer(GeColor.COLOR_23));
    else if (s.equals("LightYellow3")) setValue( new Integer(GeColor.COLOR_24));
    else if (s.equals("LightYellow4")) setValue( new Integer(GeColor.COLOR_25));
    else if (s.equals("yellow0")) setValue( new Integer(GeColor.COLOR_26));
    else if (s.equals("yellow1")) setValue( new Integer(GeColor.COLOR_27));
    else if (s.equals("yellow2")) setValue( new Integer(GeColor.COLOR_28));
    else if (s.equals("yellow3")) setValue( new Integer(GeColor.COLOR_29));
    else if (s.equals("yellow4")) setValue( new Integer(GeColor.COLOR_30));
    else if (s.equals("Lightgoldenrod0")) setValue( new Integer(GeColor.COLOR_31));
    else if (s.equals("Lightgoldenrod1")) setValue( new Integer(GeColor.COLOR_32));
    else if (s.equals("Lightgoldenrod2")) setValue( new Integer(GeColor.COLOR_33));
    else if (s.equals("Lightgoldenrod3")) setValue( new Integer(GeColor.COLOR_34));
    else if (s.equals("Lightgoldenrod4")) setValue( new Integer(GeColor.COLOR_35));
    else if (s.equals("gold00")) setValue( new Integer(GeColor.COLOR_36));
    else if (s.equals("gold1")) setValue( new Integer(GeColor.COLOR_37));
    else if (s.equals("gold2")) setValue( new Integer(GeColor.COLOR_38));
    else if (s.equals("gold3")) setValue( new Integer(GeColor.COLOR_39));
    else if (s.equals("gold4")) setValue( new Integer(GeColor.COLOR_40));
    else if (s.equals("LightOrange")) setValue( new Integer(GeColor.COLOR_41));
    else if (s.equals("NavajoWhite")) setValue( new Integer(GeColor.COLOR_42));
    else if (s.equals("LightSalmon1")) setValue( new Integer(GeColor.COLOR_43));
    else if (s.equals("LightSalmon3")) setValue( new Integer(GeColor.COLOR_44));
    else if (s.equals("LightSalmon4")) setValue( new Integer(GeColor.COLOR_45));
    else if (s.equals("DarkOrange")) setValue( new Integer(GeColor.COLOR_46));
    else if (s.equals("DarkOrange0")) setValue( new Integer(GeColor.COLOR_47));
    else if (s.equals("DarkOrange1")) setValue( new Integer(GeColor.COLOR_48));
    else if (s.equals("DarkOrange3")) setValue( new Integer(GeColor.COLOR_49));
    else if (s.equals("DarkOrange4")) setValue( new Integer(GeColor.COLOR_50));
    else if (s.equals("MistyRose1")) setValue( new Integer(GeColor.COLOR_51));
    else if (s.equals("MistyRose2")) setValue( new Integer(GeColor.COLOR_52));
    else if (s.equals("LightPink")) setValue( new Integer(GeColor.COLOR_53));
    else if (s.equals("LightPink3")) setValue( new Integer(GeColor.COLOR_54));
    else if (s.equals("LightPink4")) setValue( new Integer(GeColor.COLOR_55));
    else if (s.equals("IndianRed0")) setValue( new Integer(GeColor.COLOR_56));
    else if (s.equals("IndianRed1")) setValue( new Integer(GeColor.COLOR_57));
    else if (s.equals("firebrick1")) setValue( new Integer(GeColor.COLOR_58));
    else if (s.equals("firebrick3")) setValue( new Integer(GeColor.COLOR_59));
    else if (s.equals("firebrick4")) setValue( new Integer(GeColor.COLOR_60));
    else if (s.equals("Plum1")) setValue( new Integer(GeColor.COLOR_61));
    else if (s.equals("Plum1")) setValue( new Integer(GeColor.COLOR_62));
    else if (s.equals("Plum2")) setValue( new Integer(GeColor.COLOR_63));
    else if (s.equals("Plum3")) setValue( new Integer(GeColor.COLOR_64));
    else if (s.equals("Plum4")) setValue( new Integer(GeColor.COLOR_65));
    else if (s.equals("Magenta00")) setValue( new Integer(GeColor.COLOR_66));
    else if (s.equals("Magenta0")) setValue( new Integer(GeColor.COLOR_67));
    else if (s.equals("Magenta1")) setValue( new Integer(GeColor.COLOR_68));
    else if (s.equals("Magenta3")) setValue( new Integer(GeColor.COLOR_69));
    else if (s.equals("Magenta4")) setValue( new Integer(GeColor.COLOR_70));
    else if (s.equals("LightSteelBlue1")) setValue( new Integer(GeColor.COLOR_71));
    else if (s.equals("LightSteelBlue1")) setValue( new Integer(GeColor.COLOR_72));
    else if (s.equals("LightSteelBlue2")) setValue( new Integer(GeColor.COLOR_73));
    else if (s.equals("LightSteelBlue3")) setValue( new Integer(GeColor.COLOR_74));
    else if (s.equals("LightSteelBlue4")) setValue( new Integer(GeColor.COLOR_75));
    else if (s.equals("LightCyan")) setValue( new Integer(GeColor.COLOR_76));
    else if (s.equals("PowderBlue")) setValue( new Integer(GeColor.COLOR_77));
    else if (s.equals("SteelBlue1")) setValue( new Integer(GeColor.COLOR_78));
    else if (s.equals("SteelBlue")) setValue( new Integer(GeColor.COLOR_79));
    else if (s.equals("blue4")) setValue( new Integer(GeColor.COLOR_80));
    else if (s.equals("aquamarine0")) setValue( new Integer(GeColor.COLOR_81));
    else if (s.equals("aquamarine1")) setValue( new Integer(GeColor.COLOR_82));
    else if (s.equals("aquamarine2")) setValue( new Integer(GeColor.COLOR_83));
    else if (s.equals("aquamarine3")) setValue( new Integer(GeColor.COLOR_84));
    else if (s.equals("aquamarine4")) setValue( new Integer(GeColor.COLOR_85));
    else if (s.equals("turqoise0")) setValue( new Integer(GeColor.COLOR_86));
    else if (s.equals("turqoise1")) setValue( new Integer(GeColor.COLOR_87));
    else if (s.equals("turqoise2")) setValue( new Integer(GeColor.COLOR_88));
    else if (s.equals("turqoise3")) setValue( new Integer(GeColor.COLOR_89));
    else if (s.equals("turqoise4")) setValue( new Integer(GeColor.COLOR_90));
    else if (s.equals("DarkOliveGreen1")) setValue( new Integer(GeColor.COLOR_91));
    else if (s.equals("DarkOliveGreen2")) setValue( new Integer(GeColor.COLOR_92));
    else if (s.equals("DarkOliveGreen3")) setValue( new Integer(GeColor.COLOR_93));
    else if (s.equals("DarkOliveGreen4")) setValue( new Integer(GeColor.COLOR_94));
    else if (s.equals("DarkOliveGreen5")) setValue( new Integer(GeColor.COLOR_95));
    else if (s.equals("chartreuse")) setValue( new Integer(GeColor.COLOR_96));
    else if (s.equals("chartreuse2")) setValue( new Integer(GeColor.COLOR_97));
    else if (s.equals("chartreuse3")) setValue( new Integer(GeColor.COLOR_98));
    else if (s.equals("chartreuse4")) setValue( new Integer(GeColor.COLOR_99));
    else if (s.equals("DarkGreen")) setValue( new Integer(GeColor.COLOR_100));
    else throw new IllegalArgumentException(s);
  }
}
