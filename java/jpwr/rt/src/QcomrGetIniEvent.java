/**
 * Title:	<p>
 * Description:	<p>
 * Copyright:	<p>
 * Company	SSAB<p>
 * @author	JN
 * @version	1.0
 */

package jpwr.rt;

public class QcomrGetIniEvent {
  public boolean terminate;
  public boolean swapInit;
  public boolean swapDone;
  public boolean timeout;
  public int sts;

  public QcomrGetIniEvent( boolean terminate,
			   boolean swapInit,
			   boolean swapDone,
			   boolean timeout,
			   int sts )
  {
    this.terminate = terminate;
    this.swapInit  = swapInit;
    this.swapDone  = swapDone;
    this.timeout   = timeout;
    this.sts       = sts;
  }

  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
}
