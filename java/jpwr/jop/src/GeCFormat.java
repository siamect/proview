package jpwr.jop;
import java.text.*;


public class GeCFormat {
    public GeCFormat( String format) {
      int f_idx, p_idx;
    
      try {
        if ( (f_idx = format.indexOf('f')) != -1) {
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
           if ( format.charAt(1) == '-')
	     no_space = 1;
	   if ( f_idx-no_space != 1) {
             h = Integer.valueOf(format.substring( 1+no_space, f_idx)).intValue();
           }
           else
             h = 4;
        }
        else if ( (f_idx = format.indexOf('s')) != -1) {
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
      }
      catch ( NumberFormatException e) {
        System.out.println( "NumberFormatException: " + format);
        h = -1;
      }
    }
    int d;
    int h;
    int no_space = 0;

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

    public StringBuffer format( String value, StringBuffer buff) {
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
}
