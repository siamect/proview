/**
 * Title:	<p>
 * Description:	<p>
 * Copyright:	<p>
 * Company	SSAB<p>
 * @author	Claes Sjofors
 * @version	1.0
 */

package jpwr.rt;

/**
 * Interface to cdh functions.
 */

public class Cdh {
  public static final int cUserVolMin    	= (0 + (0 << 24) + (1 << 16) + (1 << 8) + 1);
  public static final int cUserVolMax  		= (0 + (0 << 24) + (254 << 16) + (254 << 8) + 254);
  public static final int cUserClassVolMin  	= (0 + (0 << 24) + (0 << 16) + (2 << 8) + 1);
  public static final int cUserClassVolMax  	= (0 + (0 << 24) + (2 << 16) + (254 << 8) + 254);

  public static final int mName_volume 		= 1 << 0;
  public static final int mName_path 		= 1 << 1;
  public static final int mName_object 		= 1 << 2;
  public static final int mName_bodyId 		= 1 << 3;
  public static final int mName_bodyName 	= 1 << 4;
  public static final int mName_attribute 	= 1 << 5;
  public static final int mName_index 		= 1 << 6;
  public static final int mName_escapeGMS 	= 1 << 7;
  public static final int mName_separator 	= 1 << 8;
  public static final int mName_idString 	= 1 << 9;
  public static final int mName_parent 		= 1 << 10;
  public static final int mName_form_std 	= 1 << 16;
  public static final int mName_form_root 	= 1 << 17;
  public static final int mName_form_id 	= 1 << 18;
  public static final int mName_fallback_bestTry = 1 << 24;
  public static final int mName_fallback_strict	= 1 << 25;
  public static final int mName_fallback_export	= 1 << 26;
  public static final int mName_fallback_volumeDump = 1 << 27;
  public static final int mName_pathBestTry	=
	mName_path | mName_object | mName_attribute |
	mName_index | mName_form_std | mName_fallback_bestTry;
  public static final int mName_volumeBestTry	=
	mName_volume | mName_path | mName_object | mName_attribute |
	mName_index | mName_form_std | mName_fallback_bestTry;
  public static final int mName_pathStrict	=
	mName_path | mName_object | mName_attribute |
	mName_index | mName_form_std | mName_fallback_strict;
  public static final int mName_volumeStrict	=
	mName_volume | mName_path | mName_object | mName_attribute |
	mName_index | mName_form_std | mName_fallback_strict;
  
}






