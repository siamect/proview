/**
 * Title:	<p>
 * Description:	<p>
 * Copyright:	<p>
 * Company	SSAB<p>
 * @author	JN
 * @version	1.1
 */

package jpwr.rt;
import java.io.Serializable;

public class CdhrObjAttr implements Serializable
{
  public PwrtObjid objid;
  public GdhrRefObjectInfo refObj;
  public String name;
  public String fullName = null;
  public int type;
  public int size;
  public int flags;
  public int elements;
  public int sts;
  
  public CdhrObjAttr(PwrtObjid objid,
                     String  name,
                     int     type,
                     int     size,
                     int     flags,
                     int     elements)
 
  {
    this.objid = objid;
    this.name = name;
    this.type = type;
    this.size = size;
    this.flags = flags;
    this.elements = elements;
  }
  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
}
