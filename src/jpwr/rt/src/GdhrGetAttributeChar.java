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
public class GdhrGetAttributeChar implements Serializable
{
  public int typeId;
  public int size;
  public int offset;
  public int elements;
  public int sts;
  
  public GdhrGetAttributeChar( int typeId, int size, int offset,
					  int elements, int sts) {
    this.typeId = typeId;
    this.size = size;
    this.offset = offset;
    this.elements = elements;
    this.sts = sts;
  }
  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
}
