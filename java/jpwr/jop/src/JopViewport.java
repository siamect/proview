package jpwr.jop;
import javax.swing.JViewport;
import java.awt.Point;

public class JopViewport extends JViewport
{
  public boolean user = false;
  public void setViewPosition(Point p)
  {
    if(user)
    {
      user = false;
    }
    else
      p.x = 0;
    super.setViewPosition(p);
  }
}
