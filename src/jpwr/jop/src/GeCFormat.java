package jpwr.jop;
import java.text.*;


public class GeCFormat {
    public GeCFormat( String format) {
      int f_idx, p_idx;
    
      try {
        if ( (f_idx = format.indexOf('f')) != -1) {
	  format_type = FRM_F;
          p_idx = format.indexOf('.');
          if ( format.charAt(1) == '-')
	    no_space = 1;

          if ( p_idx == -1) {
            d = 0;
	    h = Integer.valueOf(format.substring( 1+no_space, f_idx)).intValue();
          }
          else {
            d = Integer.valueOf(format.substring( p_idx + 1, f_idx)).intValue();
	    h = Integer.valueOf(format.substring( 1+no_space, p_idx)).intValue();
          }
        }
        else if ( (f_idx = format.indexOf('d')) != -1) {
	  format_type = FRM_D;
	  if ( format.charAt(1) == '-')
	    no_space = 1;
	  if ( f_idx-no_space != 1) {
            h = Integer.valueOf(format.substring( 1+no_space, f_idx)).intValue();
	  }
	  else
	    h = 4;
        }
        else if ( (f_idx = format.indexOf('s')) != -1) {
	  format_type = FRM_S;
          if ( format.charAt(1) == '-')
	    no_space = 1;
          p_idx = format.indexOf('.');
          if ( p_idx == -1) {
            d = 0;
	    if ( f_idx == 1)
	      h = 0;
	    else
	      h = Integer.valueOf(format.substring( 1+no_space, f_idx)).intValue();
          }
          else {
            d = 0;
	    h = Integer.valueOf(format.substring( 1+no_space, p_idx)).intValue();
          }
        }
        else if ( (f_idx = format.indexOf('o')) != -1) {
	  if ( f_idx >= 1 && format.charAt(f_idx-1) == '1')
	    format_type = FRM_1O;
	  else if ( f_idx >= 1 && format.charAt(f_idx-1) == '2')
	    format_type = FRM_2O;
	  else
	    format_type = FRM_O;
	}
        else if ( (f_idx = format.indexOf('t')) != -1) {
	  if ( f_idx >= 1 && format.charAt(f_idx-1) == '1')
	    format_type = FRM_1T;
	  else if ( f_idx >= 1 && format.charAt(f_idx-1) == '2')
	    format_type = FRM_2T;
	  else if ( f_idx >= 1 && format.charAt(f_idx-1) == '3')
	    format_type = FRM_3T;
	  else
	    format_type = FRM_T;
	}
        else if ( (f_idx = format.indexOf('m')) != -1) {
	  if ( f_idx >= 1 && format.charAt(f_idx-1) == '1')
	    format_type = FRM_1M;
	  else
	    format_type = FRM_M;
	}
      }
      catch ( NumberFormatException e) {
        System.out.println( "NumberFormatException: " + format);
        h = -1;
      }
    }
    int d;
    int h;
    int no_space = 0;
    int format_type;

    public static final int FRM_S  = 0;  // String
    public static final int FRM_O  = 1;  // Objid object name
    public static final int FRM_1O = 2;  // Objid path
    public static final int FRM_2O = 3;  // Objid volume and path
    public static final int FRM_T  = 4;   // Time, date and time
    public static final int FRM_1T = 5;  // Time, only time, no hundredth
    public static final int FRM_2T = 6;  // Time, only time with hundreth
    public static final int FRM_3T = 7;  // Time, commpressed date and time, no hundredth
    public static final int FRM_M  = 8;  // Message
    public static final int FRM_1M = 9;  // Message, text only
    public static final int FRM_D  = 10; // Integer
    public static final int FRM_F  = 11; // Float


    public int type() {
      return format_type;
    }
    public StringBuffer format( float value, StringBuffer buff) {
      int j, len;
      int t2 = (int) value;
      int m;
      buff.setLength(0);

      if ( h == -1)
        return buff;
	
      buff.append(t2);
      len = buff.length();
      if ( d > 0) {
	if ( no_space == 0) {
          for ( j = 0; j < h-d-1-len; j++)
            buff.insert(0, " ");
        }
        buff.append(".");
        
        m = 1;
	for ( j = 0; j < d; j++)
	  m *= 10;
        t2 = (int)((value - (float)t2) * m + 0.5f);
        t2 = Math.abs( t2);
        for ( j = 0; j < d - Integer.toString(t2).length(); j++)
	  buff.append('0');
        buff.append(t2);
      }
      else {
        for ( j = 0; j < h-d-len; j++)
	  buff.insert(0," ");
      }
      return buff;
    }
    
    public StringBuffer format( int value, StringBuffer buff) {
      int j, len;
      int t2 = value;

      if ( h == -1)
	return buff;

      buff.setLength(0);
      buff.append(value);
      len = buff.length();
      for ( j = 0; j < h-len; j++)
        buff.insert(0," ");
      return buff;
    }

    public StringBuffer format( boolean value, StringBuffer buff) {
      buff.setLength(0);
      if ( value)
        buff.append('1');
      else
	buff.append('0');
      return buff;
    }

    public StringBuffer format( String value, StringBuffer buff) {
      switch( format_type) {
      case FRM_S: {
	int j, len;

	if ( h == -1)
	  return buff;

	len = value.length();
	if ( len > h && h != 0)
	  len = h;
	buff.setLength(0);
	for ( j = 0; j < len; j++)
	  buff.append(value.charAt(j));
	return buff;
      }
      case FRM_O: {
	int idx, j, len;

	len = value.length();
	idx = value.lastIndexOf('-');
	if ( idx == -1)
	  idx = 0;
	else
	  idx++;

	buff.setLength(0);
	for ( j = idx; j < len; j++)
	  buff.append(value.charAt(j));
	return buff;	
      }
      case FRM_1O: 
      case FRM_2O: {
	int j, len;

	len = value.length();
	buff.setLength(0);
	for ( j = 0; j < len; j++)
	  buff.append(value.charAt(j));
	return buff;	
      }
      case FRM_T: 
      case FRM_1T: 
      case FRM_2T: 
      case FRM_3T: {
	int j, len;

	len = value.length();
	buff.setLength(0);
	for ( j = 0; j < len; j++)
	  buff.append(value.charAt(j));
	return buff;	
      }
      case FRM_M: 
      case FRM_1M: {
	int j, len;

	len = value.length();
	buff.setLength(0);
	for ( j = 0; j < len; j++)
	  buff.append(value.charAt(j));
	return buff;	
      }
      }
      buff.setLength(0);
      return buff;
    }
}

