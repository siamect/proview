package jpwr.jop;
import java.util.*;
import java.util.regex.*;
import javax.swing.table.AbstractTableModel;
import jpwr.rt.*;

/* HistStatModel2 is an AbstractTableModel that derives information on the
   duration of alarms (Active to Return) supplied in a MhData. Proper methods
   for displaying the results as a table ar included aswell... */

public class HistStatModel2 extends AbstractTableModel{
    // The List holding the statistics
    List result;  
    private String[][] names={{"Object", "Duration time" },{"Objekt", "Varaktighet"}};
    int lang;
    
    //Constructor
    public HistStatModel2(MhData m, int l ){
	lang=l;
	clearData();
        setData(m);
        sortData();
    }
    
    public void clearData(){
	result = new ArrayList();
    }

    public void setData(MhData m){
    /* All elements in the MhData eventVec are stepped through, looking for
       Alarms. When one is found the rest are looked through, looking for a
       matching Return (the Return's target.birthTime = the Alarm's 
       birthTime). If a matching Return is found the a copy of the alarm's 
       MhrEvent is stored in the result List with the int eventFlags used 
       as a storage for the duration time.*/

    
    for(int i=m.eventVec.size()-1; i>0;i--){
            boolean found=false;
            int timeDiff=0;
            if (((MhrEvent)m.eventVec.get(i)).eventType==Mh.mh_eEvent_Alarm){
                for(int j=i-1; j>=0;j--){ 
                    if (((MhrEvent) m.eventVec.get(j)).targetId.birthTime.equals(((MhrEvent) m.eventVec.get(i)).eventId.birthTime))
                        if (((MhrEvent)m.eventVec.get(j)).eventType==Mh.mh_eEvent_Return){
                found=true;
                // The duration is calculated in seconds. The 
                //duration is limited to at most two months. 
                Pattern p = Pattern.compile("[\\s-:.]");
                
                String[] stop=p.split(((MhrEvent) m.eventVec.get(j)).eventTime);
                int month = new Integer(stop[1]).intValue();
                    int day = new Integer(stop[2]).intValue();
                int hour  = new Integer(stop[3]).intValue();
                int minute  = new Integer(stop[4]).intValue();
                int second  = new Integer(stop[5]).intValue();
                int millis = new Integer(stop[6]).intValue();
                
                String[] start=p.split(((MhrEvent) m.eventVec.get(i)).eventTime);
                
                
                month -= new Integer(start[1]).intValue();
                if (month > 0){
                month = new Integer(stop[1].charAt(2)).intValue();
                    switch(month){
                   
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12: 
                    day+=31;
                    break;
                case 4:
                case 6:
                case 9:
                case 11: 
                    day+=30;
                    break;
                case 2:
                    day+=28;
                default:
                    break;
                }
                }
                day = day - new Integer(start[2]).intValue();
                hour  = hour- new Integer(start[3]).intValue();
                minute  -= new Integer(start[4]).intValue();
                second  = second - new Integer(start[5]).intValue();
                millis  -= new Integer(start[6]).intValue();
                timeDiff= second + 60*minute + 3600*hour + 
                3600*24*day;
                
                }
            if (found) break;    
        }
        }                      
            if (found){
                result.add(((MhrEvent)m.eventVec.get(i)).getCopy());
        ((MhrEvent)result.get(result.size()-1)).eventFlags=timeDiff;
                
        }
    }
    }
    
        
    public Object getValueAt(int r,int c){
        if (c==0) return ((MhrEvent)result.get(r)).eventName;
        else {
        int temp = ((MhrEvent)result.get(r)).eventFlags;        
        //Format a String showing the duration in days, hours, minutes
        // and seconds.
        String retString= "" + (temp/(3600*24)) + "D ";
        temp = temp%(3600*24);
        retString= retString + (temp/(3600)) + "H ";
        temp = temp%(3600);
        retString= retString + (temp/(60)) + "M ";
        temp = temp%(60);
        retString= retString + (temp) + "S " ;
        return retString;
    }
    } 
    
    public int getRowCount(){
        return result.size();
    }   
    
    public int getColumnCount(){
        return names.length;
    }
    
    public String getColumnName(int i){
        return names[lang][i];
    }
    
    //sortData sorts the result List based on duration (the value of 
    //eventFlags).Longest duration on top.)    
    public void sortData(){
    
    Collections.sort(result, new Comparator(){
         public int compare(Object o1, Object o2){
            if (((MhrEvent)o1).eventFlags < ((MhrEvent)o2).eventFlags) return 1;
            else if (((MhrEvent)o1).eventFlags > ((MhrEvent)o2).eventFlags) return -1;
            else return 0;
        }
    }); 
             
   }  
}

