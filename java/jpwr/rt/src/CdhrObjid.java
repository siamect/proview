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

public class CdhrObjid implements Serializable
{
  public PwrtObjid objid;
  public int sts;
  
  public CdhrObjid( PwrtObjid objid, int sts) {
    this.objid = objid;
    this.sts = sts;
  }
  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
}
