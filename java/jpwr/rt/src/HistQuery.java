package jpwr.rt;
import java.io.Serializable;

/*The HistQuery class represents the search criteria needed to perform a Search
 *in the eventlist. It is used as the argument for performing SearchRequest from
 *a SearchSender	*/

public class HistQuery implements Serializable{
  public String startTime;
  public String stopTime;
  public boolean[] type;
  public boolean[] priority;
  public String name;
  public String text;
  	
  public HistQuery(String start, String stop, boolean[] ty, boolean[] p, String n,String tx)
  {
    this.startTime=start;
    this.stopTime=stop;
    this.type=ty;
    this.priority=p;
    this.name=n;
    this.text=tx;
  }
}
