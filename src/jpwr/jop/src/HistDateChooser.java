package jpwr.jop;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;

public class HistDateChooser extends JPanel implements ActionListener{
        
        private Calendar date;
    SpinnerDateModel yearModel;
    private JComboBox month;
        private JSpinner year, hour, minute, second;
        private JPanel dayGrid = new JPanel();
        private JPanel timeGrid = new JPanel();
        private int selectedDay;

        private int lang =0;
        
        private String[][] months = {{"January", "February", "March", "April", "May", "June", "July"
            ,"August", "September", "October","November", "December"},
        {"januari", "februari", "mars", "april", "maj", "juni", "juli"
        ,"augusti", "september", "oktober","november", "december"}};
        
    private String[][] days = {{"Mon","Tue","Wed","Thu","Fri","Sat","Sun"},
         {"Mån","Tis","Ons","Tor","Fre","Lör","Sön"}}; 
         
    private String[][] time = {{"Hour","Minute","Second"},{"Timme","Minut","Sekund"}};
        
        public HistDateChooser(){
                date = Calendar.getInstance();
                init();
        }
        
    public HistDateChooser(int l){
        lang=l;
        date = Calendar.getInstance();
        init();
    }
    
    public HistDateChooser(Calendar Cal,int l){
        lang=l;
        date = (Calendar) Cal.clone();
        init();
    }
        

        private void init(){
       selectedDay=date.get(Calendar.DAY_OF_MONTH); 
       month = new JComboBox(months[lang]);
       month.setSelectedIndex(date.get(Calendar.MONTH));
       yearModel = new SpinnerDateModel();
       yearModel.setValue(date.getTime());
       yearModel.setCalendarField(Calendar.YEAR);    
       year = new JSpinner(yearModel);
       year.setEditor(new JSpinner.DateEditor(year, "yyyy"));
       this.setLayout(new BorderLayout());
       this.add("North",year);
       dayGrid.setLayout(new GridLayout(7,7));
       month.addActionListener(this);
       yearModel.addChangeListener( new YearChangeListener());
       this.add("Center",dayGrid);
       this.add("West",month);
       hour=new JSpinner(new SpinnerNumberModel(date.get(Calendar.HOUR_OF_DAY),0,23,1));
       minute=new JSpinner(new SpinnerNumberModel(date.get(Calendar.MINUTE),0,59,1));
       second=new JSpinner(new SpinnerNumberModel(date.get(Calendar.SECOND),0,59,1));
       setupTimeGrid();
       this.add("South",timeGrid); 
       updateGrid();
    }
    
    public Calendar getDate(){
        
        updateDate();
        return date;
        }
        
    private void updateDate(){
        date.setTime(yearModel.getDate());
        date.set(Calendar.MONTH,month.getSelectedIndex());
        date.set(Calendar.DAY_OF_MONTH,selectedDay);
        date.set(Calendar.HOUR_OF_DAY,((Integer)hour.getValue()).intValue());
        date.set(Calendar.MINUTE,((Integer)minute.getValue()).intValue());
        date.set(Calendar.SECOND,((Integer)second.getValue()).intValue());     
    }
    
    private void updateGrid(){
        updateDate();
        dayGrid.removeAll();
        int i;
        for (i=0;i<7;i++) dayGrid.add(new JLabel(days[lang][i],JLabel.CENTER));
        Calendar temp =(Calendar) date.clone();
        temp.set(Calendar.DAY_OF_MONTH,1);
        int startDay = temp.get(Calendar.DAY_OF_WEEK)-Calendar.MONDAY;
        if (startDay<0) startDay=6;
        for (i=0;i<startDay;i++) dayGrid.add(new JLabel(""));
        for (i=1;i<=date.getActualMaximum(Calendar.DAY_OF_MONTH);i++) {
            JButton day = new JButton(String.valueOf(i));
            day.addActionListener(this);
            if (i==selectedDay)
                day.setEnabled(false);
            dayGrid.add(day);                
        }
        for (i=startDay+date.getActualMaximum(Calendar.DAY_OF_MONTH)+1;i<42;i++) dayGrid.add(new Label(""));
       repaint();
       validate();     
     }
     
     private void setupTimeGrid(){
       timeGrid.setLayout(new GridLayout(3,3));
       for (int i=0; i<3; i++){
        timeGrid.add(new JLabel(""));
       }
       for (int i=0; i<3; i++){
        timeGrid.add(new JLabel(time[lang][i],JLabel.CENTER));
       }
       timeGrid.add(hour);  
       timeGrid.add(minute);  
       timeGrid.add(second);
     }
     
     public void actionPerformed(ActionEvent e){
        
        if (e.getSource().getClass() == (new JButton()).getClass()){
            try{
                JButton pressed = (JButton) e.getSource();
                selectedDay =  (new Integer(pressed.getText())).intValue();
            }
            catch (Exception ex) {
                System.out.println("Error:" + ex);
            }
        }
        
        updateGrid();
            
        
     }
     
     
     private class YearChangeListener implements ChangeListener{
        
        
        
      public void stateChanged(ChangeEvent e){
            updateGrid();
        }
        
     }
    
}
