package jpwr.jop;
import java.awt.*;
import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import jpwr.rt.Mh;
import jpwr.rt.MhData;
import jpwr.rt.MhrEvent;


/* The EventTableModel is an AbstractTableModel designed to keep the 
 *result from a search made with the AlertSearch engine. The result
 *to present is contained in the MhData mhdata.*/

  class EventTableModel extends AbstractTableModel
  
  {     
  
        MhData mhData = new MhData(0,100);  
        String[][] columnNamesEventTable = {{"Prio","","Time","Description text","Event name"},{"P",
    "Typ",
    "Tid",
    "Händelsetext",
    "Objekt"}};

        int lang;

    public final Object[] longValues = {"A", "Kvittens",
      "10-12-31 12:12:12.98",
      "QWERTYUIOPÅÄÖLK_JHGFDSAZXCVBNM__POÅIUYTRQWERTYUIOPÅÄÖL",
      "QWERTYUIOPÅÄÖLK"};


    /**
     *  Constructor for the EventTableModel object
     */
    public EventTableModel() { lang=0; }
    
    // constructor with language support
    public EventTableModel(int l) { lang=l; }


    /**
     *  Gets the columnCount attribute of the EventTableModel object
     *
     *@return    The columnCount value
     */
    public int getColumnCount()
    {
      return columnNamesEventTable[lang].length;
    }


    /**
     *  Gets the rowCount attribute of the EventTableModel object
     *
     *@return    The rowCount value
     */
    public int getRowCount()
    {
      return mhData.getNrOfEvents();//eventData.size();
    }


    /**
     *  Gets the columnName attribute of the EventTableModel object
     *
     *@param  col  Description of the Parameter
     *@return      The columnName value
     */
    public String getColumnName(int col)
    {
      return (String)columnNamesEventTable[lang][col];
    }


    /**
     *  Gets the valueAt attribute of the EventTableModel object
     *
     *@param  row  Description of the Parameter
     *@param  col  Description of the Parameter
     *@return      The valueAt value
     */
    public Object getValueAt(int row, int col)
    {
      try
      {
        MhrEvent ev = mhData.getEvent(row);//(MhrEvent)eventData.get(row);
        if(col == 0)
        {
          //System.out.println("col == 0 i eventTable.getValueAt()");
          if(ev.eventPrio == Mh.mh_eEventPrio_A)
          {
            return "A";
          }
          if(ev.eventPrio == Mh.mh_eEventPrio_B)
          {
            return "B";
          }
          if(ev.eventPrio == Mh.mh_eEventPrio_C)
          {
            return "C";
          }
          if(ev.eventPrio == Mh.mh_eEventPrio_D)
          {
            return "D";
          }
          else
          {
            return "U";
          }
        }
        if(col == 1)
        {
          String returnString = " ";
          switch (ev.eventType)
          {
            case Mh.mh_eEvent_Alarm:
              returnString = "Larm";
            break;
            case Mh.mh_eEvent_Ack:
              returnString = "Kvittens";
            break;
            case Mh.mh_eEvent_Block:
              returnString = "Block";
            break;
            case Mh.mh_eEvent_Cancel:
              returnString = "Cancel";
            break;
            case Mh.mh_eEvent_CancelBlock:
              returnString = "CancelBlock";
            break;
            case Mh.mh_eEvent_Missing:
              returnString = "Missing";
            break;
            case Mh.mh_eEvent_Reblock:
              returnString = "Reblock";
            break;
            case Mh.mh_eEvent_Return:
              returnString = "Retur";
            break;
            case Mh.mh_eEvent_Unblock:
              returnString = "Unblock";
            break;
            case Mh.mh_eEvent_Info:
              returnString = "Info";
            break;
            case Mh.mh_eEvent_:
              returnString = "?";
            break;
            default:
             returnString = " ";
            break;
          }
          return returnString;
        }
        if(col == 2)
        {
          return ev.eventTime;
        }
        if(col == 3)
        {
          return ev.eventText;
        }
        if(col == 4)
        {
          return ev.eventName;
        }
      }

      catch(ArrayIndexOutOfBoundsException e)
      {
        System.out.println(e.toString());
        if(col == 1)
        {
          return new Boolean(false);
        }
      }
      return "FEEELLLL";
    }


    /**
     *  Gets the columnClass attribute of the EventTableModel object
     *
     *@param  c  Description of the Parameter
     *@return    The columnClass value
     */
    public Class getColumnClass(int c)
    {
      return longValues[c].getClass();
      //return getValueAt(0, c).getClass();
    }


    /**
     *  Sets the valueAt attribute of the EventTableModel object
     *
     *@param  value  The new valueAt value
     *@param  row    The new valueAt value
     *@param  col    The new valueAt value
     */
    public void setValueAt(Object value, int row, int col)
    {
      //alarmData[row][col] = value;
      // fireTableCellUpdated(row, col);

    }


    /**
     *  Gets the rowObject attribute of the EventTableModel object
     *
     *@param  row  Description of the Parameter
     *@return      The rowObject value
     */
    public MhrEvent getRowObject(int row)
    {
      try
      {
        return mhData.getEvent(row);//(MhrEvent)eventData.get(row);
      }
      catch(ArrayIndexOutOfBoundsException e)
      {
        System.out.println("eventable.getRowObject " + e.toString());
      }
      return null;
    }


    /**
     *  Description of the Method
     */
    public void rowInserted()
    {
      fireTableRowsInserted(0, 0);
    }


    /**
     *  Description of the Method
     *
     *@param  row  Description of the Parameter
     */
    public void rowRemoved(int row)
    {
      fireTableRowsDeleted(row, row);
      
    }


    /**
     *  Description of the Method
     */
    public void reloadTable()
    {
      fireTableStructureChanged();
    }


    /**
     *  Description of the Method
     */
    public void updateTable()
    {
      fireTableDataChanged();
    }
    
    public void setMhData(MhData data)
    {
        mhData=data;
        updateTable();
    }


  }
