package jpwr.rt;
import java.io.Serializable; 
/**
 *  Description of the Class
 *
 *@author     claes
 *@created    November 26, 2002
 */
public class MhrsEventId implements Serializable
{
  public int nix;
  public String birthTime;
  public int idx;
  
  MhrsEventId(int nix, String birthTime, int idx)
  {
    this.nix = nix;
    this.birthTime = birthTime;
    this.idx = idx;
  }
  
  public boolean isSameObject(MhrsEventId o)
  {
    return (o.nix == this.nix/* && o.idx == this.idx*/);
  }
}
      
