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
  public int elements;
  public int size;
    
  public GdhrRefObjectInfo( PwrtRefId refid, int id, int sts, int typeId) {
    this.refid = refid;
    this.id = id;
    this.sts = sts;
    this.typeId = typeId;
    //default values in case we have forgotten to use the new constructor
    this.elements = 1;
    this.size = 4;
  }
  public GdhrRefObjectInfo( PwrtRefId refid, int id, int sts, int typeId, int elements, int size) {
    this.refid = refid;
    this.id = id;
    this.sts = sts;
    this.typeId = typeId;
    this.elements = elements;
    this.size = size;
  }

  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
  public int getElements(){ return elements;}
  public int getSize(){ return size;}
}

