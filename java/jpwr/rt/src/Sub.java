  /**
   * Objekt som lagras i vektorn som sköter varje tråds, i GdhServer, prenumerationer
   *
   *@author     JN3920
   *@created    den 18 november 2002
   */
  package jpwr.rt;
  import java.io.Serializable;
  public class Sub  extends GdhrRefObjectInfo implements Serializable
  {
    public String attrName = "hej";
    public int subscriptionsIndex;
    public int valueInt;
    public float valueFloat;
    public boolean valueBoolean;
    public String valueString;
    /*
    public PwrtRefId refid;
    public int id;
    public int sts;
    public int typeId;
    public int subId;
    */
    public Sub(String attrName, PwrtRefId refid, int id, int typeId, int
	subscriptionsIndex, int valueInt, float valueFloat, boolean valueBoolean, String valueString)
    {
      /*
      this.refid = refid;
      this.id = id;
      this.sts = 0;
      this.typeId = typeId;
      */
      super(refid, id, 0, typeId); 
      this.subscriptionsIndex = subscriptionsIndex;
      this.valueInt = valueInt;
      this.valueFloat = valueFloat;
      this.valueBoolean = valueBoolean;
      this.valueString = valueString;
    }
    public void setValue(Object o)
    {
      if(o instanceof String)
        this.valueString = (String) o;
      else if(o instanceof Boolean)
        this.valueBoolean = ((Boolean)o).booleanValue();
      else if(o instanceof Integer)
        this.valueInt = ((Integer)o).intValue();
      else if(o instanceof Float)
        this.valueFloat = ((Float)o).floatValue();
    }
    public void setValue(String o)
    {
      this.valueString = o;
    }
    public void setValue(int o)
    {
      this.valueInt = o;
    }
    public void setValue(float o)
    {
      this.valueFloat = o;
    }
    public void setValue(boolean o)
    {
      this.valueBoolean = o;
    }


    


    public Sub(String attrName, PwrtRefId refid, int id, int typeId, int subscriptionsIndex)
    {
      /*
      this.refid = refid;
      this.id = id;
      this.sts = sts;
      this.typeId = typeId;
      */
      super(refid, id, 0, typeId);
      this.attrName = attrName;
      this.subscriptionsIndex = subscriptionsIndex;
    }

    //public boolean evenSts() { return (sts % 2 == 0);}
    //public boolean oddSts() { return (sts % 2 == 1);}
    //public int getSts() { return sts;}

    int getIndex()
    {
      return subscriptionsIndex;
    }
  }
