package jpwr.rt;

public class RtSecurity {

  static {
    System.loadLibrary("jpwr_rt_gdh");
  }

  public static native CdhrInt checkUser( String group, String user, String password);

  public static CdhrInt checkUser2( String group, String user, String password) {
    CdhrInt cdhrInt;
    int	sts = 1;
    int priv;
    if ( user.toUpperCase().equals("CLAES") &&
         password.toUpperCase().equals("HEJHOPP")) {
      priv = Pwr.mAccess_AllPwr;
      return new CdhrInt( priv, sts);
    }
    else if ( user.toUpperCase().equals("SKIFTEL") &&
         password.toUpperCase().equals("SKIFTEL")) {
      priv = Pwr.mPrv_RtRead | Pwr.mPrv_RtWrite;
      return new CdhrInt( priv, sts);
    }
    else if ( user.toUpperCase().equals("OP55") &&
         password.toUpperCase().equals("OP55")) {
      priv = Pwr.mPrv_RtRead | Pwr.mPrv_Operator1;
      return new CdhrInt( priv, sts);
    }
    return new CdhrInt( 0, 0);
  }
}
