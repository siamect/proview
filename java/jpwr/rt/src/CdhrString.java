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
public class CdhrString implements Serializable
{
  public String str;
  public int sts;
  
  public CdhrString( String str, int sts) {
    this.str = str;
    this.sts = sts;
  }
  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
}
