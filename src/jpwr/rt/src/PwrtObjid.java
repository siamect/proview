/**
 * Title:	<p>
 * Description:	<p>
 * Copyright:	<p>
 * Company	SSAB<p>
 * @author	CS
 * @version	1.0
 */

package jpwr.rt;
import java.io.Serializable;

public class PwrtObjid implements Serializable
{
  public int oix;
  public int vid;
  
  public PwrtObjid( int oix, int vid) {
    this.oix = oix;
    this.vid = vid;
  }
  public int getOix() { return oix;}
  public int getVid() { return vid;}

  public boolean isNull() {
    return ( oix == 0 && vid == 0);
  }
  public boolean isEqual( PwrtObjid objid) {
    return ( objid.oix == oix && objid.vid == vid);
  }
}
