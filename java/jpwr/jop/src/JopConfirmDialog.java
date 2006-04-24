/* 
 * Proview   $Id: JopConfirmDialog.java,v 1.4 2006-04-24 13:21:46 claes Exp $
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
import javax.swing.*;
import java.awt.event.*;

public class JopConfirmDialog {
  static JDialog dia = null;
  static Component par;
  static JLabel label;
  public static void open( Component parent, String text) {
    par = parent;
    if ( dia == null) {
      dia = new JDialog();
      dia.setTitle("Confirm");
      dia.setResizable(false);
      dia.setDefaultCloseOperation( JDialog.DO_NOTHING_ON_CLOSE);
      dia.setSize( 400, 150);
      label = new JLabel( text, JLabel.CENTER);
      dia.getContentPane().setLayout( new BorderLayout());
      dia.getContentPane().add( label, BorderLayout.CENTER);
      JButton buttonYes = new JButton("Yes");
      buttonYes.addActionListener( new ActionListener() {
        public void actionPerformed( ActionEvent ev) {
//          dia.hide();
//          dia.setVisible( false);
          ((JopConfirm) par).confirmYes();
          dia.dispose();
//          dia = null;
        }
      });
      JButton buttonNo = new JButton("No");
      buttonNo.addActionListener( new ActionListener() {
        public void actionPerformed( ActionEvent ev) {
//          dia.hide();
//          dia.setVisible( false);
          ((JopConfirm) par).confirmNo();
          dia.dispose();
//          dia = null;
        }
      });
      JButton buttonCancel = new JButton("Cancel");
      buttonCancel.addActionListener( new ActionListener() {
        public void actionPerformed( ActionEvent ev) {
//          dia.hide();
//          dia.setVisible( false);
          ((JopConfirm) par).confirmNo();
          dia.dispose();
//          dia = null;
        }
      });
      JPanel panel = new JPanel();
      panel.add( buttonYes);
      panel.add( buttonNo);
      panel.add( buttonCancel);
      dia.getContentPane().add( panel, BorderLayout.SOUTH);
      dia.setLocationRelativeTo( par);
//      dia.setVisible( true);
      dia.show();
    }
    else {
      dia.setLocationRelativeTo( par);
      label.setText( text);
      dia.setVisible( true);
//      dia.show();
    }
  }
}

