/* 
 * Proview   $Id: HistSearch.java,v 1.4 2007-01-30 06:51:55 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

package jpwr.jop;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import jpwr.rt.*;
import java.applet.Applet;
//XLS import netscape.javascript.*;
// Import of netscape.javascript.* from jaws.jar is needed !!!

/* HistSearch is a search engine for performing searches from the historic 
 *event list. It's mainly a GUI for getting input on the criteria on which
 *the search should be based, launching the search and presenting the result. 
 *It uses a HistTable called result for presenting the search result. 
 *The HistQuery currentSearch is set to the criteria chosen via the GUI when
 *the JButton btnSearch is pressed and currentSearch is then sent to a class
 *in the HistTable result for evaluating the search and updating the table.
 *The GUI is made to support English and Swedish by keeping arrays of all 
 *strings.
 */

public class HistSearch extends JFrame implements ActionListener
{
        
    public JopEngine engine;
    public JopSession session;
    public Object root;
;
    
    /*integers for keeping track of the number of 
      search results.*/
    int num=0;
    
    JPanel contentPane;
    
    // ****TOP****  
    //top contains the search interface
    JPanel top = new JPanel();
    
    //check contains the arrays of checkboxes       
    JPanel check = new JPanel();
    
    // labels describing different objects in the search interface. 
    JLabel lblStart = new JLabel( JopLang.transl("Start time:"), JLabel.LEFT);
    JLabel lblStop = new JLabel( JopLang.transl("Stop time:"), JLabel.LEFT);
    JLabel lblType = new JLabel( JopLang.transl("Event type:"), JLabel.LEFT);
    JLabel lblPriority = new JLabel( JopLang.transl("Priority:"), JLabel.LEFT);
    JLabel lblName = new JLabel( JopLang.transl("Event name:"), JLabel.LEFT);
    JLabel lblText = new JLabel( JopLang.transl("Event text:"), JLabel.LEFT); 
    JLabel lblNum = new JLabel( JopLang.transl("Statistics"),JLabel.LEFT);
    
    
    // textfields for different parameters in the search interface
    JTextField txStart,txStop,txName,txText;
    
    // cbWhen is a combo box giving choices of search intervals     
    
    String[] choice = {JopLang.transl("All events"),
		       JopLang.transl("This Year"), 
		       JopLang.transl("This Month"),
		       JopLang.transl("This Week"),
		       JopLang.transl("Today"),
		       JopLang.transl("Enter dates")};
    JComboBox cbWhen ;
    
    // type is an array of radiobuttons, one for each type of event
    JCheckBox[] type = new JCheckBox[4];
    String[] nameType = {JopLang.transl("Active"),
			   JopLang.transl("Message"),
			   JopLang.transl("Return"),
			   JopLang.transl("Ack")};
    
    // priority is an array of checkboxes, one for each priority
    JCheckBox[] priority = new JCheckBox[4];
    String[] namePriority = { JopLang.transl("A-Alarm"),
			      JopLang.transl("B-Alarm"),
			      JopLang.transl("C-Alarm"),
			      JopLang.transl("D-Alarm")};
    
    // btnSearch is the search button
    JButton btnSearch ;
    
    // two calendars for start and stop dates    
    Calendar calStop,calStart;
    
    String defaultDate="YYYY-MM-DD HH:MM:SS";
    String strNum=JopLang.transl("Number of Events:") + " ";
    String msgStart=JopLang.transl("Enter start date and time");
    String msgStop =JopLang.transl("Enter stop date and time");
    
    /**** Menu + textstrings ****/
    JMenu fileMenu;
    
    
    //****MIDDLE****        
    //middle contains the search condition and the search result.
    JPanel middle = new JPanel();
    
    //resultTable is the HistTable containing the search results.
    HistTable result;
    
    // result contains the table showing the search result.
    //JTable result = new JTable(resultTable);
    
    // lblCondition is the label showing the current search conditions.
    JLabel lblCondition = new JLabel("",JLabel.LEFT);
    
    // lblHead is the label showing the static text "Search Condition"      
    String descHead = "<HTML><U>" + JopLang.transl("Search Condition") + "</U></HTML>";
    JLabel lblHead = new JLabel(descHead, JLabel.LEFT);
    
    HistQuery currentSearch;

    public JButton btnStat;    

    class WindowExit extends WindowAdapter{
    public void windowClosing(WindowEvent ev){
    
    }
    }
    void HistSearch_WindowClosing(WindowEvent event) {
    dispose();
    }
    
    //Default constructor
    public HistSearch(){
    engine = new JopEngine( 1000, (Object)this);
    session = new JopSession( engine, (Object)this);
    root = (Object) this;
    setup();
    }

    public HistSearch( JopSession s)
    {
    session = s;
    engine = session.getEngine();
    root = session.getRoot();
    setup();
    }

    public HistSearch(String q , JopSession s)
    {
    session = s;
    engine = session.getEngine();
    root = session.getRoot();
    setup();
    txName.setText("*" + q + "*");
    doSearch();
    
    }

    private void setup(){
    result =new HistTable(session);
    /* A mouse listener is added to the alarmTable from here because
       the result of the double click affects components of the HistSearch.
       To add the listener like this avoids compilation order problems.*/
    result.alarmTable.addMouseListener(new MouseAdapter() {

	    /***** MOUSE LISTENER CODE START *****/

        //Bring up a JopMethodsMenu
        public void mouseReleased(MouseEvent e) {
            if ( e.isPopupTrigger()) {
            int row=result.alarmTable.rowAtPoint(e.getPoint());
            MhrEvent event = (MhrEvent) result.atModel.mhData.eventVec.get(row);
            String trace_object= event.eventName;
            new JopMethodsMenu( session, 
                        trace_object, 
                        JopUtility.TRACE,(Component) result.alarmTable, 
                        e.getX(), e.getY());
            return;
            }
        }
        public void mousePressed(MouseEvent e) {
	    int row=result.alarmTable.rowAtPoint(e.getPoint());
	    result.alarmTable.setRowSelectionInterval(row,row);
            if ( e.isPopupTrigger()) {
            MhrEvent event = (MhrEvent) result.atModel.mhData.eventVec.get(row);
            String trace_object= event.eventName;
            new JopMethodsMenu(session, 
                       trace_object, 
                       JopUtility.TRACE,(Component) result.alarmTable, 
                       e.getX(), e.getY());
            return;
            }
        }
	    //On double click copy name or text to corresponding textfields.
	    public void mouseClicked(MouseEvent e) {
		if (e.getClickCount() == 2) {
		    String tempStr="";
		    int row=result.alarmTable.rowAtPoint(e.getPoint());
		    if(result.alarmTable.columnAtPoint(e.getPoint())==3){
			if (txText.getText().length() > 0)
			    tempStr=txText.getText() + ";";
			tempStr=tempStr + "*" +
			    (String)result.alarmTable.getValueAt(row,3) + "*";
			txText.setText(tempStr);
		    }
		    else{
			if (txName.getText().length() > 0 )
			    tempStr=txName.getText() + ";";
			tempStr=tempStr + "*" +
			    (String)result.alarmTable.getValueAt(row,4) + "*";
			txName.setText(tempStr);
		    }
		}
	    }

	    /**** MOUSE LISTENER CODE STOP ****/

        });
    addWindowListener(new WindowExit());
    addComponentListener(new minSizeListener(this));
    this.setTitle("Hist Search");
    // setup the searchbutton
    btnSearch= new JButton( JopLang.transl("Search"));
    btnSearch.addActionListener(this);
    btnSearch.setActionCommand("search");
    btnSearch.setMnemonic('s');
    // setup the statisticsbutton
    btnStat= new JButton(JopLang.transl("Statistics"));
    btnStat.setMnemonic('t');
    btnStat.setActionCommand("stat");
    btnStat.addActionListener(this);
    //setup tht combobox.
    cbWhen=new JComboBox(choice);
    cbWhen.addActionListener(this); 

    //setup the start and stop dates.
    calStop = Calendar.getInstance();
    calStart = Calendar.getInstance();
    calStart.set(Calendar.YEAR,1970);
    //      setup the array of checkboxes
    checkSetup();
    //make sure all buttons and labels have text.
    menuSetup();
    place();
    //set comboBox to "thisYear" by deafult. setSelectedItem calls
    //the action listener, so the dates update.
    cbWhen.setSelectedItem(cbWhen.getItemAt(1));
    updateDate();
    
    
    }
    
                
    
    //Place all onscreen objects    
    private void place(){
        contentPane = (JPanel)this.getContentPane();
        contentPane.setLayout(new BorderLayout());
        middleSetup();
        topSetup();
        contentPane.add("North",top);
        contentPane.add("Center",middle);
	contentPane.add("South",btnStat);
    }
    
    private void menuSetup(){
    // set up the menu structure.
    this.setJMenuBar (new JMenuBar());
    fileMenu=new JMenu(JopLang.transl("File"));
    fileMenu.setMnemonic(fileMenu.getText().charAt(0));
    
    JMenuItem item = new JMenuItem(JopLang.transl("Export result to Excel"));
    item.addActionListener(this);
    item.setMnemonic(item.getText().charAt(0));
    fileMenu.add(item);

    item = new JMenuItem(JopLang.transl("Close"));
    item.addActionListener(this);
    item.setMnemonic(item.getText().charAt(0));
    fileMenu.add(item);
    
    this.getJMenuBar().add(fileMenu);
    }
    
    
    private void middleSetup(){
    //Setup the layout for the middle part of the screen.
        
    middle.setLayout(new BorderLayout());
    JPanel condition = new JPanel();
    condition.setLayout(new BorderLayout());
    condition.add("North",lblHead);
    Dimension d = lblCondition.getSize();
    d.height +=80;
    lblCondition.setPreferredSize(d);
    condition.add("Center", lblCondition);
    middle.add("North",condition);
    d = result.getSize();
    result.setPreferredSize(d);
    middle.add("Center", result);
    }
    
    private void topSetup(){
    GridBagLayout topLayout = new GridBagLayout();  
    GridBagConstraints topC = new GridBagConstraints();
    top.setLayout(topLayout);
    
    
    // 1st row
    topC.gridx=0;
    topC.gridy=0;
    topC.weightx=0;
    topC.fill=GridBagConstraints.BOTH;
    topC.anchor=GridBagConstraints.CENTER;
    topC.insets= new Insets(2,2,5,5);
    
    // Start time label
    topLayout.setConstraints(lblStart, topC);
    top.add(lblStart);
    
    // Start time textfield         
    topC.gridx=1;
    topC.weightx=1;
    txStart = new JTextField (defaultDate);
    txStart.setEditable(false);
    txStart.setColumns(19);
    topLayout.setConstraints(txStart, topC);
    top.add(txStart);
    
    //Stop time label
    topC.gridx=2;
    topC.weightx=0;
    topLayout.setConstraints(lblStop, topC);
    top.add(lblStop);
    
    //Stop time textfield
    topC.gridx=3;
    topC.weightx=1;
    topC.gridwidth=GridBagConstraints.RELATIVE;
    txStop = new JTextField (defaultDate);
    txStop.setEditable(false);
    txStop.setColumns(19);
    topLayout.setConstraints(txStop, topC);
    top.add(txStop);
    
    //Search interval combobox.
    topC.gridx=4;
    topC.gridwidth=GridBagConstraints.REMAINDER;
    topLayout.setConstraints(cbWhen, topC);
    top.add(cbWhen);
    
    
    
    // 2nd & 3rd rows               
    
        
    // add the array of check boxes
    topC.gridx=0;
    topC.gridy=1;
    topC.weightx=0;
    topC.gridwidth=4;
    topC.gridheight=2;
    topLayout.setConstraints(check, topC);
    top.add(check);
    
    
        
    // 4th row
    topC.gridx=0;
    topC.gridy=3;
    topC.gridwidth=1;
    topC.gridheight=1;
    
    // Event name time label
    topLayout.setConstraints(lblName, topC);
    top.add(lblName);       
    
    //Event name textfield
    topC.gridx=1;
    topC.gridwidth=GridBagConstraints.REMAINDER;
    txName = new JTextField();
    topLayout.setConstraints(txName, topC);
    top.add(txName);
    
    // 5th row
    topC.gridx=0;
    topC.gridy=4;
    topC.gridwidth=1;
        
    // Event name time label
    topLayout.setConstraints(lblText, topC);
    top.add(lblText);       
    
    //Event name textfield
    topC.gridx=1;
    topC.gridwidth=GridBagConstraints.REMAINDER;
    txText = new JTextField();
    topLayout.setConstraints(txText, topC);
    top.add(txText);
        
    // 6th row
    topC.gridx=0;
    topC.gridy=5;
    topC.gridwidth=1;       
    
    // Number of events label
    updateNum();
    topLayout.setConstraints(lblNum, topC);
    top.add(lblNum);        
    
    //The search button
    topC.gridx=2;
    topLayout.setConstraints(btnSearch, topC);
    top.add(btnSearch);
    }
    
    private void checkSetup(){
        
    // make the layout of the checkbox array a GridLayout. Add the event Type label.
    check.setLayout(new GridLayout(2,5));   
    check.add(lblType);
    
    //For loop adding the four type checkboxes      with name tags  
    for (int i=0;i<4;i++){
        
        type[i]=new JCheckBox(nameType[i]);
        check.add(type[i]);
                }
    
    // Add the Event priority label         
    check.add(lblPriority);
    
    //For loop adding the four priority checkboxes with name tags.          
    for (int i=0;i<4;i++){
        priority[i]=new JCheckBox(namePriority[i]);
        check.add(priority[i]);
    }       
        
    
    }
    
    //Update the number of events label.
    private void updateNum(){
    num=result.getNrOfResults();
    lblNum.setText(strNum+num);
    }       
    
    
    private void updateDate(){
    txStart.setText(dateText(calStart));
    txStop.setText(dateText(calStop));              
    }
    
    // Update the text in the Start & Stop Textfields. Adjust x<10 to 0x    
    private String dateText(Calendar cal){
    String text=""+cal.get(Calendar.YEAR)+"-";
    if(cal.get(Calendar.MONTH)<9) text = text + "0";
    text = text + (cal.get(Calendar.MONTH)+1)+"-";
    if(cal.get(Calendar.DATE)<10) text = text + "0";
    text = text +cal.get(Calendar.DATE)+" ";
    if(cal.get(Calendar.HOUR_OF_DAY)<10) text = text +"0";
    text = text+ cal.get(Calendar.HOUR_OF_DAY)+":";
    if(cal.get(Calendar.MINUTE)<10) text = text +"0";
    text = text +cal.get(Calendar.MINUTE)+":";
    if(cal.get(Calendar.SECOND)<10) text = text + "0";
    text = text + cal.get(Calendar.SECOND);
    return text;                    
    }
    
    /* the action listener methods of this object. Checks on ComboBox, searchbutton
       and menu options */
    public void actionPerformed(ActionEvent e){
    if (e.getSource()==cbWhen){
        
        switch(cbWhen.getSelectedIndex()){
        
                                //all entries           
        case 0:
        calStop = Calendar.getInstance();
        calStart = Calendar.getInstance();
        calStart.set(Calendar.YEAR,1970);
        updateDate();
        break;
        
                                //this year
        case 1:
        calStop = Calendar.getInstance();
        calStart = Calendar.getInstance();
        calStart.set(Calendar.MONTH,0);
        calStart.set(Calendar.DATE,1);
        calStart.set(Calendar.HOUR_OF_DAY,0);
        calStart.set(Calendar.MINUTE,0);
        calStart.set(Calendar.SECOND,0);
        updateDate();
        break;
        
                                //this month
        case 2:
        calStop = Calendar.getInstance();
        calStart = Calendar.getInstance();
        calStart.set(Calendar.DATE,1);
        calStart.set(Calendar.HOUR_OF_DAY,0);
        calStart.set(Calendar.MINUTE,0);
        calStart.set(Calendar.SECOND,0);
        updateDate();
        break;
        
                                //this week
        case 3:
        calStop = Calendar.getInstance();
        calStart = Calendar.getInstance();
        int diff = calStart.get(Calendar.DAY_OF_WEEK)-Calendar.MONDAY;
        calStart.add(Calendar.DATE,-1*diff);
        if (diff<0) calStart.add(Calendar.DATE,-7);
        calStart.set(Calendar.HOUR_OF_DAY,0);
        calStart.set(Calendar.MINUTE,0);
        calStart.set(Calendar.SECOND,0);
        updateDate();
        break;
        
                                //today
        case 4:
        calStop = Calendar.getInstance();
        calStart = Calendar.getInstance();
        calStart.set(Calendar.HOUR_OF_DAY,0);
        calStart.set(Calendar.MINUTE,0);
        calStart.set(Calendar.SECOND,0);
        updateDate();
        break;
                                
                                //enter dates
        case 5:
        
                                //routine for Start time input dialog
           
        HistDateChooser tempDate = new HistDateChooser(calStart);
        int buttonPressed = JOptionPane.showOptionDialog(this,tempDate,msgStart,JOptionPane.OK_CANCEL_OPTION,JOptionPane.QUESTION_MESSAGE,null,null,null);
        if (buttonPressed == JOptionPane.OK_OPTION){
            calStart=tempDate.getDate();
            //routine for Stop time input dialog
            tempDate = new HistDateChooser(calStop);
            buttonPressed = JOptionPane.showOptionDialog(this,tempDate,msgStop,JOptionPane.OK_CANCEL_OPTION,JOptionPane.QUESTION_MESSAGE,null,null,null);
            if (buttonPressed == JOptionPane.OK_OPTION){
            calStop=tempDate.getDate();
            }
        }
        
        updateDate();
        break;
        default:
        updateDate();
        break;
        }
    }
    
    //  Handling of search button event.
    
    if(e.getActionCommand()=="search"){
        doSearch();
    }

    if (e.getActionCommand()=="stat"){
	result.presentStat();
    }
    
    // the handling of menu events
    if(e.getActionCommand()=="Copy result to excel"||e.getActionCommand()=="Kopiera resultat till Excel"){
        // check if the root is an applet
	try{	    
	    Applet source = (Applet) root;
	    /* //XLS Method for exporting an excel file from an applet
	       // Uses a javascript from the page that started the applet.
	    JSObject javaScript = JSObject.getWindow(source);
	    if (javaScript != null){
	        Object[] args = new Object[1];
                args[0]=result.exportExcelFromApplet();
                javaScript.call("appletHtml", args);
	    }	    */
	    
        }
	catch(Exception ex){
	    //No applet, copy to system clipboard.
	    result.exportExcel();
	}
    }
    
    if(e.getActionCommand()=="Quit"||e.getActionCommand()=="Avsluta"){
       this.dispose();
    }
    
    }       

    private void doSearch(){
    lblCondition.setFont(new Font("SansSerif", Font.PLAIN, 14));
    lblCondition.setForeground(new Color(0x000000));
    lblCondition.setText(evalSearch());
    lblCondition.validate();
    this.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
    result.performSearch(root,currentSearch);
    this.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
    updateNum();
    }
    
    
    // Returns a string showing all search criteria and sets the currentSearch      
    public String evalSearch(){
    
	//Useful words in english and swedish.
	String[] words ={JopLang.transl("All "), 
			   JopLang.transl(" Active"), 
			   JopLang.transl(" Message"), 
			   JopLang.transl(" Return"), 
			   JopLang.transl(" Acknowlegement"), 
			   JopLang.transl(" events"),
			   JopLang.transl(" with"),
			   JopLang.transl(" priority"),
			   JopLang.transl(" name"),
			   JopLang.transl(" text"), 
			   JopLang.transl(" from "),
			   JopLang.transl(" to "),
			   JopLang.transl(" or")};
    String eval="<html>" + words[0];
    String temp="";
    boolean[] noCheck ={true,true};
    boolean[] typeSelected = {false,false,false,false};
    boolean[] prioSelected = {false,false,false,false};
    
    for (int i=0;i<4;i++){
        if (type[i].isSelected()){
        typeSelected[i]=true;
        if (noCheck[0]) noCheck[0]=false;
        else eval=eval+words[12];
        eval=eval + "<b>" +words[i+1] + "</b>";
        }
        if (priority[i].isSelected()){
        prioSelected[i]=true;
        if (noCheck[1]) noCheck[1]=false;
        else temp=temp+words[12];
        int charA =(int)'A';
        charA=charA+i;                          
        temp=temp + " <b>" + (char) charA + "</b>";
        }
    }
    eval= eval + words[5] + " ";
    if (temp.length()>0) {
        eval=eval+ words[6] + temp + words[7] ;
    }
    
    eval=eval+"<br>"+words[6]+words[8]+" <b>"+txName.getText() +"</b><br>";
    eval=eval+words[6]+words[9]+" <b>"+txText.getText() +"</b><br>";
    eval=eval + words[10] + "<b>"+ dateText(calStart)+ "</b>" + words[11]  + "<b>" + dateText(calStop)+"</b> </html>";
    
    String nameFormatted= txName.getText().replace('"','*');
	String textFormatted= txText.getText().replace('"','*');
    //Setup the current search for transfer to server
    currentSearch=new HistQuery(dateText(calStart), dateText(calStop),typeSelected,prioSelected,nameFormatted,textFormatted);
    
    return eval;            
    }       

    
    public class minSizeListener implements ComponentListener {
        
        static final int MIN_WIDTH=600;
        static final int MIN_HEIGHT=480;
    
    private HistSearch as;
        
        public minSizeListener(HistSearch a){
        as=a;
        }
        
        public void componentResized(ComponentEvent e) {
        
        int width = as.getWidth();
        int height = as.getHeight();
        boolean resize=false;
        
        
        //we check if either the width
        //or the height are below minimum
        
        
        
        if (width < MIN_WIDTH) {
                width = MIN_WIDTH;
                resize=true;
                
        }
        if (height < MIN_HEIGHT) {
                height = MIN_HEIGHT;
        resize=true;
        }
        
        
        if (resize)
        as.setSize(width, height);
        
        
        
        
        
        
        }
    
        public void componentMoved(ComponentEvent e) {
        }
    
        public void componentShown(ComponentEvent e) {
        }
    
        public void componentHidden(ComponentEvent e) {
        }
    
    }
    
    
    //Testprogram   
    public static void main(String args[]){
        HistSearch ASWindow = new HistSearch();
        ASWindow.pack();        
        ASWindow.setVisible(true);
    }
    
    
}
