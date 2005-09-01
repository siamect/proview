/* 
 * Proview   $Id: JopLoginApplet.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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

public class JopLoginApplet extends JApplet {
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
  JLabel labelUser = new JLabel("User");
  JTextField textUser = new JTextField(10);
  JLabel labelPassword = new JLabel("Password");
  JPasswordField textPassword = new JPasswordField(10);
  JButton buttonOk = new JButton();
  JButton buttonClear = new JButton();
  JButton buttonLogout = new JButton();
  JLabel labelMessage = new JLabel(" ");
  Gdh gdh;
  public JopLoginApplet() {
  }
  public void init () {
    gdh = new Gdh( this);
    size = new Dimension( 300, 120);
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
//    this.setTitle("Login");
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
    buttonOk.setText("Ok");
    buttonPanel.add(buttonOk);
    buttonClear.setText("Clear");
    buttonPanel.add(buttonClear);
    buttonLogout.setText("Logout");
    buttonPanel.add(buttonLogout);
    messagePanel.add(labelMessage, BorderLayout.CENTER);
    
    buttonOk.addActionListener( new ActionListener() {
      public void actionPerformed( ActionEvent e) {
        checkUser();
      }
    });
    buttonClear.addActionListener( new ActionListener() {
      public void actionPerformed( ActionEvent e) {
	textUser.setText("");
	textPassword.setText("");
        labelMessage.setText(" ");
      }
    });
    buttonLogout.addActionListener( new ActionListener() {
      public void actionPerformed( ActionEvent e) {
        String user = gdh.getUser();
	if ( user == null)
          labelMessage.setText("Not logged in");
        else {
          gdh.logout();
          labelMessage.setText("User " + user + " logged out");
        }
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

  public void stop() {
  }
  
  public void destroy() {
    gdh.close();
  }
  
  public String GetAppletInfo() {
    return "Applet information";
  }
  
  public String[][] getParameterInfo() {
    return null;
  }

  private void checkUser() {
    String user = textUser.getText();
    String passwd = new String(textPassword.getPassword());
    int sts = gdh.login( user, passwd);
    if ( sts % 2 == 0) {
      JOptionPane.showMessageDialog( this, "User not authorized", 
		"Login message", JOptionPane.ERROR_MESSAGE);
      labelMessage.setText(" ");
    }
    else {
      labelMessage.setText( "User " + user + " logged in");
      textUser.setText("");
      textPassword.setText("");
    }
  }
}
