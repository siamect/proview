/* 
 * Proview   $Id: HistTable.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableColumn;
import java.io.*;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.ClipboardOwner;
import java.awt.datatransfer.StringSelection;
import jpwr.rt.*;

/* HistTable is an extended JScrollPane containing the 
 * a JTable alarmTable. The HistTable also contains methods for exporting
 * the alarmTable to an excel file, and for performing a search in the historic
 * event list using a HistSender */
 
public class HistTable extends JScrollPane{
    
    JopSession session;
    boolean DEBUG=false;
    public EventTableModel atModel;
    public JTable alarmTable;
    String[][] columnNamesEventTable = {{"P","Type","Time","Description text","Event name"},{"P",
                                                 "Typ",
                                                 "Tid",
                                                 "Händelsetext",
                                                 "Objekt"}};
    
    String[][] excelMess=
    {
    {"Copy to Excel","Your search result has been put on the Clipboard.\nOpen a new document in Excel and choose \"Paste\" from the Edit menu\nto create a Worksheet with your search result."},
    {"Kopiera till Excel","Ditt sökresultat har nu lagrats i Urklipp.\nÖppna ett nytt dokument i Excel och välj \"Klistra in\" i Redigeramenyn\nför att skapa ett Excelark med ditt sökresultat."}
    };
    
    int lang=0;
    
    // Language sensitive constructor
    public HistTable(int l, JopSession s){
    session = s;
    lang=l;
    setup();            
    }
    
    //Layout the HistTable. Add a mouse Listener to the JTable alarmTable
    //and set it up to support JopMethodsMenu.
    private void setup(){
               
    atModel = new EventTableModel(lang);
    alarmTable = new JTable(atModel);
    alarmTable.setCellEditor(null);
    this.initColumnSizes(alarmTable, atModel);
    alarmTable.getTableHeader().setReorderingAllowed(false);
    alarmTable.getColumn((Object)columnNamesEventTable[lang][0]).setCellRenderer(new EventTableCellRender());
    
    
    this.setViewportView(alarmTable);
    this.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
    this.getViewport().setBackground(Color.white);
    
    ((EventTableModel)alarmTable.getModel()).updateTable(); 
    
    }
    
    //Update the table headers to the correct language.
    public void updateLang(int l){
    lang=l;
    for (int i=0; i<alarmTable.getColumnCount();i++)
        alarmTable.getColumnModel().getColumn(i).setHeaderValue(columnNamesEventTable[lang][i]);
    alarmTable.getTableHeader().resizeAndRepaint();

    }
    
    /*Get a pointer to the local Clipboard, format a string with all cell
      elements with \t between columns and \n between rows and put it on
      the clipboard. Display a message telling how to paste the string to
      excel*/
    
    public void exportExcel(){
    StringBuffer copybuffer = new StringBuffer("");
    for(int i=0;i<alarmTable.getRowCount();i++){
        for (int j=0; j<alarmTable.getColumnCount();j++){
        copybuffer.append((String) alarmTable.getValueAt(i,j));
        copybuffer.append("\t");
        }
        copybuffer.append("\n");
    }  
    Clipboard cb= Toolkit.getDefaultToolkit().getSystemClipboard();
    StringSelection output = new StringSelection(copybuffer.toString());
    cb.setContents(output, output);
    JOptionPane.showMessageDialog(this.getParent(),excelMess[lang][1],excelMess[lang][0],JOptionPane.INFORMATION_MESSAGE);
    
        
    }
    
    //XLS
    //Creates a String for sending to a javascript which can save to a file.
    public String exportExcelFromApplet(){
	String copyString = "";
	    for(int i=0;i<alarmTable.getRowCount();i++){
		for (int j=0; j<alarmTable.getColumnCount();j++){
		    copyString = copyString +(String) alarmTable.getValueAt(i,j);
		    copyString = copyString+"\t";
		}
		copyString = copyString + "\n" ;
	    }  
	return copyString;
    }
    
    
    //Set column sizes to fit the table contents...
    private void initColumnSizes(JTable table, AbstractTableModel model)
    {
    TableColumn column = null;
    Component comp = null;
    int headerWidth = 0;
    int cellWidth = 0;
    Object[] longValues;
    
    
    longValues = ((EventTableModel)(model)).longValues;
    
    
    for(int i = 0; i < longValues.length; i++)
        {
        column = table.getColumnModel().getColumn(i);
        
        comp = table.getDefaultRenderer(model.getColumnClass(i)).
            getTableCellRendererComponent(
                          table, longValues[i],
                          false, false, 0, i);
        cellWidth = comp.getPreferredSize().width;
        
        if(DEBUG)
            {
            System.out.println("Initializing width of column "
                       + i + ". "
                       + "headerWidth = " + headerWidth
                       + "; cellWidth = " + cellWidth);
            }
        
        //XXX: Before Swing 1.1 Beta 2, use setMinWidth instead.
        column.setPreferredWidth(Math.max(headerWidth, cellWidth));
        if(i == 0)
            {
            column.setMaxWidth(Math.max(headerWidth, cellWidth));
            column.setResizable(false);
            }
        
        }
    }
    
    public void presentStat(){        
	// Derive interesting statistics from the current searchresult and
        //display it in a JFrame...
	HistStatistics statistics = new HistStatistics(atModel.mhData,lang,session);
	JFrame frame = new JFrame();
        JPanel panel = (JPanel)frame.getContentPane();
        panel.setLayout(new FlowLayout());
        panel.add(statistics);
        frame.pack();
        frame.show();

    }

    public int getNrOfResults(){
    return alarmTable.getRowCount();
    }
    // Make a search in the Historical Event list based on the HistQuery search
    public void performSearch(Object root, HistQuery search){
        HistSender sender = new HistSender(root);
        atModel.setMhData(sender.SearchRequest(search));
    }
    
    
}
