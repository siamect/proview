/* 
 * Proview   $Id: JopLoginFrame.java,v 1.3 2008-06-24 13:34:14 claes Exp $
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

import jpwr.rt.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopLoginFrame extends JFrame {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  BorderLayout borderLayout3 = new BorderLayout();
  BorderLayout borderLayout4 = new BorderLayout();
  BorderLayout borderLayout5 = new BorderLayout();
  JPanel userPanel = new JPanel();
  JPanel passwordPanel = new JPanel();
  JPanel buttonPanel = new JPanel();
  JPanel messagePanel = new JPanel();
  Dimension size;
  JLabel labelUser = new JLabel(JopLang.transl("User"));
  JTextField textUser = new JTextField(10);
  JLabel labelPassword = new JLabel(JopLang.transl("Password"));
  JPasswordField textPassword = new JPasswordField(10);
  JButton buttonOk = new JButton();
  JButton buttonClear = new JButton();
  JLabel labelMessage = new JLabel(" ");
  JopSession session;

  public JopLoginFrame( JopSession session) {
    this.session = session;
    size = new Dimension( 300, 200);
    contentPane = (JPanel) this.getContentPane();
    contentPane.setLayout(borderLayout1);
    contentPane.add(userPanel, BorderLayout.NORTH);
    contentPane.add(buttonPanel, BorderLayout.CENTER);
    contentPane.add(messagePanel, BorderLayout.SOUTH);
    contentPane.setOpaque(true);
    userPanel.setLayout(new GridBagLayout());
    GridBagConstraints gbc = new GridBagConstraints();
    gbc.anchor = GridBagConstraints.WEST;
    gbc.insets = new Insets(2, 2, 2, 2);
    
    messagePanel.setLayout(borderLayout5);
    contentPane.setOpaque(true);
    contentPane.setBackground(GeColor.getColor(40, GeColor.NO_COLOR));
    this.setSize(size);
    this.setTitle(JopLang.transl("Login"));
    gbc.gridy = 0;
    gbc.gridx = 0;
    userPanel.add(labelUser, gbc);
    gbc.gridy = 1;
    gbc.gridx = 0;
    userPanel.add(labelPassword, gbc);
    gbc.gridy = 0;
    gbc.gridx = 1;
    userPanel.add(textUser, gbc);
    gbc.gridy = 1;
    gbc.gridx = 1;
    userPanel.add(textPassword, gbc);
    buttonOk.setText(JopLang.transl("Ok"));
    buttonPanel.add(buttonOk);
    buttonClear.setText(JopLang.transl("Clear"));
    buttonPanel.add(buttonClear);
    messagePanel.add(labelMessage, BorderLayout.CENTER);
    
    buttonOk.addActionListener( new ActionListener() {
      public void actionPerformed( ActionEvent e) {
        checkUser();
      }
    });
    buttonClear.addActionListener( new ActionListener() {
      public void actionPerformed( ActionEvent e) {
	clear();
      }
    });
    textUser.addActionListener( new ActionListener() {
      public void actionPerformed( ActionEvent e) {
	javax.swing.FocusManager.getCurrentManager().focusNextComponent( textUser);
      }
    });
    textPassword.addActionListener( new ActionListener() {
      public void actionPerformed( ActionEvent e) {
        checkUser();
	javax.swing.FocusManager.getCurrentManager().focusPreviousComponent( textPassword);
      }
    });
  }

  public void clear() {
    textUser.setText("");
    textPassword.setText("");
    labelMessage.setText(" ");
    javax.swing.FocusManager.getCurrentManager().focusNextComponent( textUser);
  }

  private void checkUser() {
    String user = textUser.getText();
    String passwd = new String(textPassword.getPassword());
    passwd = JopCrypt.crypt( "aa", passwd);
    int sts = session.getEngine().gdh.login( user, passwd);
    if ( sts % 2 == 0) {
      JOptionPane.showMessageDialog( this, JopLang.transl("User not authorized"), 
		"Login message", JOptionPane.ERROR_MESSAGE);
      labelMessage.setText(" ");
    }
    else {
      textUser.setText("");
      textPassword.setText("");
      labelMessage.setText( JopLang.transl("User") + " " + user + " " + JopLang.transl("logged in"));
      session.setOpWindowLabelText( JopLang.transl("User") + " " + user + " " + JopLang.transl("logged in"));
      setVisible(false);
    }
  }
}
