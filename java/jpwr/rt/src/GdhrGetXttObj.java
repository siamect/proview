/**
 * Title:	<p>
 * Description:	<p>
 * Copyright:	<p>
 * Company	SSAB<p>
 * @author	JN
 * @version	1.0
 */

package jpwr.rt;
import java.io.Serializable;

public class GdhrGetXttObj implements Serializable
{
  public String name;
  public String fullName;
  public String className;
  public String description;
  public CdhrObjid cdhrObjId;
  public CdhrClassId cdhrClassId;
  public int sts;
  public boolean hasChildren;
  
  public GdhrGetXttObj(String      name, 
                       String      fullName,
		       String      description, 
		       String      className, 
		       CdhrObjid   cdhrObjId, 
		       CdhrClassId cdhrClassId, 
		       int         sts,
                       boolean     hasChildren) 
  {
    this.name = name; 
    this.fullName = fullName;
    this.description = description;
    this.className = className;
    this.cdhrObjId = cdhrObjId;
    this.cdhrClassId = cdhrClassId;
    this.sts = sts;
    this.hasChildren = hasChildren;
  }
  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
}
