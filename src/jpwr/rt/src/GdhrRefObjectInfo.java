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
public class GdhrRefObjectInfo implements Serializable
{
  public PwrtRefId refid;
  public int id;
  public int sts;
  public int typeId;
  public int subId;
    
  public GdhrRefObjectInfo( PwrtRefId refid, int id, int sts, int typeId) {
    this.refid = refid;
    this.id = id;
    this.sts = sts;
    this.typeId = typeId;
  }
  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
}
