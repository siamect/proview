
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

public class CdhrClassId implements Serializable
{
  public int 	classId;
  public int 	sts;
  
  public CdhrClassId( int classId, int sts) {
    this.classId = classId;
    this.sts = sts;
  }
  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
  public int getClassId() { return classId;}
}
