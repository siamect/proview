/* 
 * Proview   $Id: GdhServerMonitor.java,v 1.2 2005-09-01 14:57:52 claes Exp $
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

package jpwr.rt;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;
import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import java.awt.event.*;
import javax.swing.Timer;
public class GdhServerMonitor extends JFrame implements ActionListener
{
  public final static int GET_SUBSCRIPTIONS = 41;
    /**  Description of the Field */
  JLabel labelMessage = new JLabel("MhClient version 1.0");

  /**  Description of the Field */
  JPanel messagePanel = new JPanel();

  JPanel contentPane;
  Socket gdhSocket;
  ObjectOutputStream out;
  ObjectInputStream in;
  JTable table;
  Object data[][];
  Timer timer;
  int scantime = 5000;
  int oldSize = 0;
  long freeMem = 0;
  long totalMem = 0;
  boolean closingDown = false;
  boolean DEBUG = false;
  boolean moreOrLessRows = false;
  JScrollPane scrollPane;
      String[] columnNames = {"ObjectAttributeName",
        "ThreadNumbersConnected",
        "NumberOfSubscriptions",
	"Status",
	"Refid"};
	
  public GdhServerMonitor(boolean DEBUG)
  {
    
    super("GdhServerMonitor");
    this.DEBUG = DEBUG;
//    System.out.println("I konstruktor");

    timer = new Timer( scantime, this); 
    timer.start();

    addWindowListener(
      new WindowAdapter()
      {
        public void windowClosing(WindowEvent e)
        {
          close();
          System.exit(0);
        }
      });
      

  

//    System.out.println("innan init");
    this.init();
//    System.out.println("efter init");
//    this.getData();
    System.out.println("efter getdata");
    MyTableModel myModel = new MyTableModel();
    table = new JTable(myModel);
    table.setCellEditor(null);
    table.setPreferredScrollableViewportSize(new Dimension(500, 70));
    //Create the scroll pane and add the table to it.
    scrollPane = new JScrollPane(table);
    //Add the scroll pane to this window.
    getContentPane().add(scrollPane, BorderLayout.CENTER);
    messagePanel.add(labelMessage, BorderLayout.CENTER);
    getContentPane().add(messagePanel, BorderLayout.NORTH);
    
  }

    public void actionPerformed( ActionEvent e) {
      if ( closingDown) {
        //close();
        return;
      }
      update();
    }
  private void init()
  {
    try
    {
      gdhSocket = new Socket("127.0.0.1", 4445);
      out = new ObjectOutputStream(new BufferedOutputStream(gdhSocket.getOutputStream()));
      System.out.println("efter skapande av out");
      in = new ObjectInputStream(new BufferedInputStream(gdhSocket.getInputStream()));
      System.out.println("efter skapande av in");
/* 
      out.writeInt(27);
      out.flush();
      int poll = in.readInt();
*/ 
    }
    catch(UnknownHostException e)
    {
      System.err.println("Don't know about host: taranis.");
      System.exit(1);
    }
    catch(IOException e)
    {
      System.err.println("Couldn't get I/O for the connection");
      System.exit(1);
    }
  }


  public void close()
  {
    closingDown = true;
    try
    {
      System.out.println("Closing socket");
      
      out.close();
      in.close();
      gdhSocket.close();
    }
    catch(IOException e)
    {
      System.err.println("Couldn't close I/O connection");
    }
  }
  public void update()
  {
    try
    {
//      System.out.println("Innan GET_SUBSCRIPTIONS");
      out.writeInt(GET_SUBSCRIPTIONS);
      out.flush();
      int size = in.readInt();
      int nrOfSubscriptions = in.readInt();
      String nrOfSubStr = "NumberOfSubscriptions" + nrOfSubscriptions;

      Object ObjectAttributeNames[] = (Object[])in.readObject();
      Object ThreadNumbersConnected[] = (Object[])in.readObject();
      Object NumberOfPrenumerations[] = (Object[])in.readObject();
      Object Status[] = (Object[])in.readObject();
      Object Refid[] = (Object[])in.readObject();
      freeMem = in.readLong();
      totalMem = in.readLong();
      freeMem = freeMem / 1024;
      totalMem = totalMem / 1024;
      labelMessage.setText("Ledigt minne(av allokerat): " + freeMem + " Allokerat minne: " + totalMem +
                           "  Antal prenumerationer: " + nrOfSubscriptions);
      //qqq behövs detta???
      data = new Object[size][5];
      for(int i = 0; i < size;i++)
      {
        table.getModel().setValueAt(ObjectAttributeNames[i], i, 0);
        table.getModel().setValueAt(ThreadNumbersConnected[i], i, 1);
        table.getModel().setValueAt(NumberOfPrenumerations[i], i, 2);
        table.getModel().setValueAt(Status[i], i, 3);
        table.getModel().setValueAt(Refid[i], i, 4);
      }

      if(size != oldSize || columnNames[2].compareTo(nrOfSubStr) != 0)
      {
        columnNames[2] = nrOfSubStr;
	((MyTableModel)table.getModel()).reloadTable();
      }
      else
      {
        ((MyTableModel)table.getModel()).updateTable();
      }
      oldSize = size;
//      System.out.println("efter GET_SUBSCRIPTIONS");
    }
    catch(IOException e)
    {
      System.err.println("ServerMonitor IOException");
    }
    catch(ClassNotFoundException e)
    {
      System.err.println("ServerMonitor ClassNotFoundException");
    }

  }

  public void getData()
  {
    try
    {
//      System.out.println("Innan GET_SUBSCRIPTIONS");
      out.writeInt(GET_SUBSCRIPTIONS);
      out.flush();
      int size = in.readInt();
      int nrOfSubscriptions = in.readInt();
      columnNames[2] = "NumberOfSubscriptions" + nrOfSubscriptions;
      Object ObjectAttributeNames[] = (Object[])in.readObject();
      Object ThreadNumbersConnected[] = (Object[])in.readObject();
      Object NumberOfPrenumerations[] = (Object[])in.readObject();
      Object Status[] = (Object[])in.readObject();
      Object Refid[] = (Object[])in.readObject();
      freeMem = in.readLong();
      totalMem = in.readLong();
      labelMessage.setText("Ledigt minne(av allokerat): " + freeMem + " Allokerat minne: " + totalMem +
                           "  Antal prenumerationer: " + nrOfSubscriptions);
      data = new Object[size][5];
      for(int i = 0; i < size;i++)
      {
      
        data[i][0] = ObjectAttributeNames[i];
        data[i][1] = ThreadNumbersConnected[i];
        data[i][2] = NumberOfPrenumerations[i];
        data[i][3] = Status[i];
        data[i][4] = Refid[i];

      }
//      System.out.println("efter GET_SUBSCRIPTIONS");
    }
    catch(IOException e)
    {
      System.err.println("ServerMonitor IOException");
    }
    catch(ClassNotFoundException e)
    {
      System.err.println("ServerMonitor ClassNotFoundException");
    }
  }
  
  class MyTableModel extends AbstractTableModel 
  {
    public MyTableModel()
    {
      getData();
    }
    public int getColumnCount() {
            return columnNames.length;
        }
        
        public int getRowCount() {
            return data.length;
        }

        public String getColumnName(int col) {
            return columnNames[col];
        }

        public Object getValueAt(int row, int col) {
            return data[row][col];
        }
	 public Class getColumnClass(int c) {
            return getValueAt(0, c).getClass();
        }
	public void setValueAt(Object value, int row, int col) {
                data[row][col] = value;
               // fireTableCellUpdated(row, col);

        }
	public void reloadTable()
	{
	  fireTableStructureChanged();
	}
	public void updateTable()
	{
	  fireTableDataChanged();
	}


    }



  public static void main(String[] args)
  {
    boolean debug = false;
    for(int i = 0; i < args.length; i++)
    {
      if(args[i].equals("-d") || args[i].equals("-D"))
      {
        debug = true;
      }
    }

    GdhServerMonitor frame = new GdhServerMonitor(debug);
    frame.pack();
    frame.setVisible(true);
  }

}

