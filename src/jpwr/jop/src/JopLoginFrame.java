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
  JLabel labelUser = new JLabel("User");
  JTextField textUser = new JTextField(10);
  JLabel labelPassword = new JLabel("Password");
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
    this.setTitle("Login");
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

  private void checkUser() {
    String user = textUser.getText();
    String passwd = new String(textPassword.getPassword());
    int sts = session.getEngine().gdh.login( user, passwd);
    if ( sts % 2 == 0) {
      JOptionPane.showMessageDialog( this, "User not authorized", 
		"Login message", JOptionPane.ERROR_MESSAGE);
      labelMessage.setText(" ");
    }
    else {
      labelMessage.setText( "User " + user + " logged in");
    }
  }
}
