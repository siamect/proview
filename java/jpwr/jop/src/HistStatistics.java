/* 
 * Proview   $Id: HistStatistics.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
import java.util.Vector;
import jpwr.rt.*;

/* The HistStatistics class is a JPanel containing two JTables showing 
statistics derived from the MhData supplied in the constructor. The tables
get their contents from a HistStatModel1 and an HistStatModel2. The tables support
the use of JopMethodsMenu.*/

public class HistStatistics extends JPanel{
    
    // The session is needed for the use of JopMethodsMenu
    private JopSession session;
    private JTable fTable, eTable;
    private HistStatModel1 fModel;
    private HistStatModel2 eModel;
    
    JScrollPane fScroll, eScroll;
    int lang;

    String[] names = {JopLang.transl("Most frequent alarms"),
		      JopLang.transl("Longest duration")};
    
    public HistStatistics(MhData m, JopSession s){
    //Setup and layout. (The only method of this class..)
    session = s;
    this.setLayout(new GridLayout(2,1));
    JPanel upper = new JPanel();
    upper.setLayout(new BorderLayout());
    fModel=new HistStatModel1(m,lang);
    fTable = new JTable(fModel);
    fScroll= new JScrollPane(fTable );
    fTable.getColumnModel().getColumn(0).setPreferredWidth(420);
    fTable.getColumnModel().getColumn(1).setPreferredWidth(70);
    //Mouse listener for fTable JopMethodMenu support.
    fTable.addMouseListener(new MouseAdapter() {
        public void mouseReleased(MouseEvent e) {
            if ( e.isPopupTrigger()) {
            int row=fTable.rowAtPoint(e.getPoint());
            MhrEvent event = (MhrEvent) fModel.result.get(row);
            String trace_object= event.eventName;
            new JopMethodsMenu( session, 
                        trace_object, 
                        JopUtility.TRACE,(Component) fTable, 
                        e.getX(), e.getY());
            return;
            }
        }
        public void mousePressed(MouseEvent e) {
           if ( e.isPopupTrigger()) {
            int row=fTable.rowAtPoint(e.getPoint());
            MhrEvent event = (MhrEvent) fModel.result.get(row);
            String trace_object= event.eventName;
            new JopMethodsMenu( session, 
                        trace_object, 
                        JopUtility.TRACE,(Component) fTable, 
                        e.getX(), e.getY());
            return;
            
            }
        }
        public void mouseClicked(MouseEvent e) {
        }
        });
    upper.add(fScroll,BorderLayout.CENTER);
    upper.add(new JLabel(names[0]), BorderLayout.NORTH);
    upper.add(new JLabel("\n\n") , BorderLayout.SOUTH);
    JPanel lower = new JPanel();
    lower.setLayout(new BorderLayout());
        eModel=new HistStatModel2(m);
        eTable = new JTable(eModel);
        eScroll= new JScrollPane(eTable );
    eTable.getColumnModel().getColumn(0).setPreferredWidth(420);
    eTable.getColumnModel().getColumn(1).setPreferredWidth(70);
    //Mouse listener for eTable JopMethodMenu support.
    eTable.addMouseListener(new MouseAdapter() {
        public void mouseReleased(MouseEvent e) {
            if ( e.isPopupTrigger()) {
            int row=eTable.rowAtPoint(e.getPoint());
            MhrEvent event = (MhrEvent) eModel.result.get(row);
            String trace_object= event.eventName;
            new JopMethodsMenu( session, 
                        trace_object, 
                        JopUtility.TRACE,(Component) eTable, 
                        e.getX(), e.getY());
            return;
            }
        }
        public void mousePressed(MouseEvent e) {
           if ( e.isPopupTrigger()) {
            int row=eTable.rowAtPoint(e.getPoint());
            MhrEvent event = (MhrEvent) eModel.result.get(row);
            String trace_object= event.eventName;
            new JopMethodsMenu( session, 
                        trace_object, 
                        JopUtility.TRACE,(Component) eTable, 
                        e.getX(), e.getY());
            return;
            
            }
        }
        public void mouseClicked(MouseEvent e) {
        }
        });
    lower.add(eScroll,BorderLayout.CENTER);
    lower.add(new JLabel(names[1]), BorderLayout.NORTH);
    this.setPreferredSize(new Dimension(600, 500));
        this.add(upper); 
        this.add(lower);
               
    }
}
