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
 *strings. The integer lang indicates which language to be used, (0=eng
 *1=swe) and all texts on screen is updated at once by calling updateText().
 */

public class HistSearch extends JFrame implements ActionListener
{
        
    public JopEngine engine;
    public JopSession session;
    public Object root;
;
    
    /*integers for keeping track of the language setting and the number of 
      search results.*/
    int lang=1;
    int num=0;
    
    JPanel contentPane;
    
    // ****TOP****  
    //top contains the search interface
    JPanel top = new JPanel();
    
    //check contains the arrays of checkboxes       
    JPanel check = new JPanel();
    
    // labels describing different objects in the search interface. 
    JLabel lblStart = new JLabel("", JLabel.LEFT);
    JLabel lblStop = new JLabel("", JLabel.LEFT);
    JLabel lblType = new JLabel("", JLabel.LEFT);
    JLabel lblPriority = new JLabel("", JLabel.LEFT);
    JLabel lblName = new JLabel("", JLabel.LEFT);
    JLabel lblText = new JLabel("", JLabel.LEFT); 
    JLabel lblNum = new JLabel("",JLabel.LEFT);
    
    String[] descStart = {"Start time:","Starttid:"};
    String[] descStop = {"Stop time:","Stopptid:"};
    String[] descType = {"Event type:","Händelsetyp:"};
    String[] descPriority = {"Priority:","Prioritet:"};
    String[] descName = {"Event name:","Händelsenamn:"};
    String[] descText = {"Event text:","Händelsetext:"};
    String[] descStat = {"Statistics", "Statistik"};    
    
    
    // textfields for different parameters in the search interface
    JTextField txStart,txStop,txName,txText;
    
    // cbWhen is a combo box giving choices of search intervals     
    
    String[][] choice = {{"All alarms","This year","This month","This week","Today","Enter dates"},
             {"Alla alarm","Detta år","Denna månad","Den här veckan","Idag","Egna datum"}};
    JComboBox cbWhen ;
    
    // type is an array of radiobuttons, one for each type of event
    JCheckBox[] type = new JCheckBox[4];
    String[][] nameType = {{"Active","Message","Return","Ack"},
               {"Aktiv","Meddelande","Retur","Kvittens"}};
    
    // priority is an array of checkboxes, one for each priority
    JCheckBox[] priority = new JCheckBox[4];
    String[] namePriority = {"A-Alarm","B-Alarm","C-Alarm","D-Alarm"};
    
    // btnSearch is the search button
    JButton btnSearch ;
    String[] descSearch= {"Search","Sök"};
    
    // two calendars for start and stop dates       
    Calendar calStop,calStart;
    
    String defaultDate="YYYY-MM-DD HH:MM:SS";
    String[] strNum={"Number of events: ","Antal händelser: "};
    String[] msgStart={"Enter start date and time","Ange startdatum och tid"};
    String[] msgStop={"Enter stop date and time","Ange stoppdatum och tid"};
    String[] errorDate={"Dates must be specified on the form YYYY-MM-DD HH:MM:SS \n"+
            "e.g 1980-09-08 00:00:00", "Datum måste skrivas på formen YYYY-MM-DD HH:MM:SS \n"+
            "t.ex 1980-09-08 00:00:00"};
    
    /**** Menu + textstrings ****/
    JMenu fileMenu, langMenu;
    
    String[][] menuTitle = {{"File","Byt Språk"},{"Arkiv","Change Language"}};
    String[][] fileItems = {{"Copy result to excel","Quit"},{"Kopiera resultat till Excel","Avsluta"}};
    String[][] langItems = {{"Svenska","English"},{"Svenska","English"}};
    
    
    //****MIDDLE****        
    //middle contains the search condition and the search result.
    JPanel middle = new JPanel();
    
    //resultTable is the HistTable containing the search results.
    HistTable result;
    
    // result contains the table showing the search result.
    //JTable result = new JTable(resultTable);
    
    // lblCondition is the label showing the current search conditions.
    JLabel lblCondition = new JLabel("",JLabel.LEFT);
    
    // lblHead is the label showing the static text "Search condition"      
    JLabel lblHead = new JLabel("", JLabel.LEFT);
    String[] descHead = {"<HTML><U>Search condition</U></HTML>",
			 "<HTML><U>Sökvillkor</U></HTML>"};
    
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
    result =new HistTable(lang,session);
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
    btnSearch= new JButton(descSearch[lang]);
    btnSearch.addActionListener(this);
    btnSearch.setActionCommand("search");
    btnSearch.setMnemonic('s');
    // setup the statisticsbutton
    btnStat= new JButton(descStat[lang]);
    btnStat.setMnemonic('t');
    btnStat.setActionCommand("stat");
    btnStat.addActionListener(this);
    //setup tht combobox.
    cbWhen=new JComboBox(choice[lang]);
    cbWhen.addActionListener(this); 

    //setup the start and stop dates.
    calStop = Calendar.getInstance();
    calStart = Calendar.getInstance();
    calStart.set(Calendar.YEAR,1970);
    //      setup the array of checkboxes
    checkSetup();
    //make sure all buttons and labels have text.
    menuSetup();
    updateText();
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
    // set up the menu structure, with language choices.
    this.setJMenuBar (new JMenuBar());
    fileMenu=new JMenu(menuTitle[lang][0]);
    fileMenu.setMnemonic(fileMenu.getText().charAt(0));
    langMenu=new JMenu(menuTitle[lang][1]);
    langMenu.setMnemonic(langMenu.getText().charAt(0));
    
    for(int i=0; i< fileItems.length; i++){
        JMenuItem item = new JMenuItem(fileItems[lang][i]);
        item.addActionListener(this);
        item.setMnemonic(item.getText().charAt(0));
        fileMenu.add(item);
    }
    
    for(int i=0; i< langItems.length; i++){
        JMenuItem item = new JMenuItem(langItems[lang][i]);
        item.addActionListener(this);
        item.setMnemonic(item.getText().charAt(0));
        langMenu.add(item);
    }
        this.getJMenuBar().add(fileMenu);
        this.getJMenuBar().add(langMenu);
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
        
        type[i]=new JCheckBox(nameType[lang][i]);
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
    
    //Update the number of events label in the correct language.
    private void updateNum(){
    num=result.getNrOfResults();
    lblNum.setText(strNum[lang]+num);
    }       
    
    // All labels are updated in the current language (0 = English, 1 = Swedish)    
    private void updateText(){
    
    // the combo box needs to be relableled according to language
    // but not during initiation, hence the if statement...
    if(txStart!=null){
        cbWhen.removeAllItems();
        for(int i=0;i<choice[lang].length;i++)
        cbWhen.addItem(choice[lang][i]);
    }
    // the search button needs to be relabeled each time the language changes
    btnSearch.setText(descSearch[lang]);
    
    /*the checkboxes for type needs relabeling aswell. Not the priority checkboxes
      since there's no difference in language...*/
    for(int i=0; i<nameType[lang].length;i++){
        type[i].setText(nameType[lang][i]);
    }
    
    // All labels are updated to show the right language.           
    lblStart.setText(descStart[lang]);
    lblStop.setText(descStop[lang]);
    lblType.setText(descType[lang]);
    lblPriority.setText(descPriority[lang]);
    lblName.setText(descName[lang]);
    lblText.setText(descText[lang]);
    lblHead.setText(descHead[lang]);
    updateNum();
    
    //Update the text in the menus. Easily done by remaking it...
    menuSetup();
    
    //update the result table headers
    result.updateLang(lang);
    
    //update the btnStat text.
    btnStat.setText(descStat[lang]);
    
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
           
        HistDateChooser tempDate = new HistDateChooser(calStart,lang);
        int buttonPressed = JOptionPane.showOptionDialog(this,tempDate,msgStart[lang],JOptionPane.OK_CANCEL_OPTION,JOptionPane.QUESTION_MESSAGE,null,null,null);
        if (buttonPressed == JOptionPane.OK_OPTION){
            calStart=tempDate.getDate();
            //routine for Stop time input dialog
            tempDate = new HistDateChooser(calStop,lang);
            buttonPressed = JOptionPane.showOptionDialog(this,tempDate,msgStop[lang],JOptionPane.OK_CANCEL_OPTION,JOptionPane.QUESTION_MESSAGE,null,null,null);
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
    
    if(e.getActionCommand()=="Svenska"){
        lang=1;
        updateText();
    }
    
    if(e.getActionCommand()=="English"){
        lang=0;
        updateText();
    }
    
    }       

    private void doSearch(){
    lblCondition.setFont(new Font("SansSerif", Font.PLAIN, 14));
    lblCondition.setForeground(new Color(0x000000));
    lblCondition.setText(evalSearch(lang));
    lblCondition.validate();
    this.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
    result.performSearch(root,currentSearch);
    this.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
    updateNum();
    }
    
    
    // Returns a string showing all search criteria and sets the currentSearch      
    public String evalSearch(int lang){
    
    //Useful words in english and swedish.
    String[][] words ={{"All ", " Active", " Message", " Return", " Acknowlegement", " events"," with"," priority"," name"," text", " from "," to "," or"},
               {"Alla", " Aktiv", " Meddelande", " Retur", " Kvittens", " händelser"," med"," prioritet"," namn"," text", " fr.o.m. ", " t.o.m. ", " eller"}};
    String eval="<html>" + words[lang][0];
    String temp="";
    boolean[] noCheck ={true,true};
    boolean[] typeSelected = {false,false,false,false};
    boolean[] prioSelected = {false,false,false,false};
    
    for (int i=0;i<4;i++){
        if (type[i].isSelected()){
        typeSelected[i]=true;
        if (noCheck[0]) noCheck[0]=false;
        else eval=eval+words[lang][12];
        eval=eval + "<b>" +words[lang][i+1] + "</b>";
        }
        if (priority[i].isSelected()){
        prioSelected[i]=true;
        if (noCheck[1]) noCheck[1]=false;
        else temp=temp+words[lang][12];
        int charA =(int)'A';
        charA=charA+i;                          
        temp=temp + " <b>" + (char) charA + "</b>";
        }
    }
    eval= eval + words[lang][5] + " ";
    if (temp.length()>0) {
        eval=eval+ words[lang][6] + temp + words[lang][7] ;
    }
    
    eval=eval+"<br>"+words[lang][6]+words[lang][8]+" <b>"+txName.getText() +"</b><br>";
    eval=eval+words[lang][6]+words[lang][9]+" <b>"+txText.getText() +"</b><br>";
    eval=eval + words[lang][10] + "<b>"+ dateText(calStart)+ "</b>" + words[lang][11]  + "<b>" + dateText(calStop)+"</b> </html>";
    
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
        ASWindow.show();
    }
    
    
}
