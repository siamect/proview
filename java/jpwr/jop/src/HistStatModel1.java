package jpwr.jop;
import java.util.*;
import javax.swing.table.AbstractTableModel;
import jpwr.rt.*;

/*The HistStatModel1 class is used to derive how many times an Alarm has been
  set off during the interval of the search provided in a MhData.*/

public class HistStatModel1 extends AbstractTableModel{
    // The List storing the resulting statistics.
    public List result;
    private String[][] freqNames={{"Object", "Nr. of alarms" },{"Objekt","Antal larm"}};
    int lang;
    
    public HistStatModel1(MhData m, int l ){
        lang=l;
	clearData();
	setData(m);
        sortData();
    }

    public void clearData(){
	result = new ArrayList(); 
    }
    
    public void setData(MhData m){
    /* The MhData is Stepped through. If an Alarm is found which eventName
       is not yet present in the result List, a copy of the Alarm's 
       MhrEvent is added to the result List. If an alarm is found that is 
       present in the result List the nr of times the alarm has been found
       is increased by one. The nr of times is stored in the int 
       eventFlags of each MhrEvent in the result List*/
 
    for(int i=0; i<m.eventVec.size();i++){
            boolean found=false;
            for (int j = 0; j<result.size();j++){
                if (((MhrEvent) m.eventVec.get(i)).eventName.equals(((MhrEvent)(result.get(j))).eventName)){
                    if(((MhrEvent) m.eventVec.get(i)).eventType == Mh.mh_eEvent_Alarm){
                        ((MhrEvent)(result.get(j))).eventFlags++;
                        found=true;
                    }
                }
                if (found) break;
            }
            if (! found){
                if(((MhrEvent) m.eventVec.get(i)).eventType == Mh.mh_eEvent_Alarm){
            result.add(((MhrEvent)(m.eventVec.get(i))).getCopy());
            ((MhrEvent)result.get(result.size()-1)).eventFlags=1;
        }
        }      
        
    }
    }
    
    
    public Object getValueAt(int r,int c){
        if (c==0) return ((MhrEvent)result.get(r)).eventName;
        else return new Integer(((MhrEvent)result.get(r)).eventFlags);
    } 
    
    public int getRowCount(){
        return result.size();
    }   
    
    public int getColumnCount(){
        return freqNames.length;
    }
    
    public String getColumnName(int i){
        return freqNames[lang][i];
    }
    
    public void sortData(){
    //sortData sorts the result List. Largest number of times found first.
    
    Collections.sort(result, new Comparator(){
          public int compare(Object o1, Object o2){
        if (((MhrEvent)o1).eventFlags < ((MhrEvent)o2).eventFlags) 
        return 1;
        else if (((MhrEvent)o1).eventFlags > ((MhrEvent)o2).eventFlags) 
        return -1;
        else return 0;
        }
        }); 
    
    }  
}
