
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
      dia.setDefaultCloseOperation( dia.DO_NOTHING_ON_CLOSE);
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
          dia.dispose();
//          dia = null;
        }
      });
      JButton buttonCancel = new JButton("Cancel");
      buttonCancel.addActionListener( new ActionListener() {
        public void actionPerformed( ActionEvent ev) {
//          dia.hide();
//          dia.setVisible( false);
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
