/**
 * Title:	<p>
 * Description:	<p>
 * Copyright:	<p>
 * Company	SSAB<p>
 * @author	CS
 * @version	1.0
 */

package jpwr.rt;

public class QcomrCreateQ {
  public int qix;
  public int nid;
  public int sts;

  public QcomrCreateQ( int qix, int nid, int sts) {
    this.qix = qix;
    this.nid = nid;
    this.sts = sts;
  }
  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
}
