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
public class PwrtRefId implements Serializable
{
  public int rix;
  public int nid;
  
  public PwrtRefId( int rix, int nid) {
    this.rix = rix;
    this.nid = nid;
  }
  public int getRix() { return rix;}
  public int getNid() { return nid;}
}
