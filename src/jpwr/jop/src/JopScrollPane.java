package jpwr.jop;
import javax.swing.JScrollPane;
import javax.swing.JViewport;
import javax.swing.JScrollBar;

public class JopScrollPane extends JScrollPane
{
  public JopScrollPane()
  {
    super();
    setViewport(new JopViewport());
  }
  public JViewport getViewport()
  {
    JScrollBar hS = getHorizontalScrollBar();
    if(hS == null) return super.getViewport();
    int val = hS.getValue();
    if(val == 0)
      return super.getViewport();
    JopViewport vp = (JopViewport)super.getViewport();
    vp.user = true;
    return vp;      
  }
}
