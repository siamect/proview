  /**
   * Object som lagras i den statiska vektorn, i GdhServer, som handhar alla prenumerationer
   *
   *@author     JN3920
   *@created    den 18 november 2002
   */
  package jpwr.rt;
  import java.io.Serializable;
  import java.util.*;
  public class SubElement
  {
    public int[] reffedByThread;
    public BitSet reffedByThreadBitSet;
    Sub sub = new Sub(" ", new PwrtRefId(0, 0), 0, 0, 0,0,0);


    public SubElement(int maxConnections, int threadNumber)
    {
      reffedByThread = new int[maxConnections];
      reffedByThreadBitSet = new BitSet(maxConnections);
      reffedByThreadBitSet.set(threadNumber);
      for(int i = 0; i < reffedByThread.length; i++)
      {
        reffedByThread[i] = 0;
      }
    }
    public SubElement(String attrName)
    {
      this.sub.attrName = attrName;
    }

    public Sub getSub()
    {
      return sub;
    }
    public void setSub(GdhrRefObjectInfo obj, int subscriptionsIndex, String attrName)
    {
      this.sub.attrName = attrName;
      this.sub.refid = obj.refid;
      this.sub.id = obj.id;
      this.sub.sts = obj.sts;
      this.sub.typeId = obj.typeId;
      this.sub.size = obj.size;
      this.sub.elements = obj.elements;
      this.sub.subscriptionsIndex = subscriptionsIndex;

    }
    public boolean equals(Object o)
    {
      return (this.sub.attrName.equalsIgnoreCase(((SubElement)o).sub.attrName) && (((SubElement)o).sub.sts != 0));
    }
  }

