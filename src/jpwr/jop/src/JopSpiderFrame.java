
package jpwr.jop;
import java.awt.*;
import javax.swing.*;
import jpwr.rt.*;

public class JopSpiderFrame {
  Object frame;

  //Construct the application
  public JopSpiderFrame( String className) {

    try {
      Class clazz = Class.forName( className);
      try {
        frame = clazz.newInstance();
        run();
      }
      catch ( InstantiationException e) {
        System.out.println("InstatiationException");
      }
      catch ( IllegalAccessException e) {
        System.out.println("IllegalAccessException");
      }
    }
    catch (ClassNotFoundException e) {
      System.out.println("Class not found");
    }
  }
  public void run() {
    boolean packFrame = false;
    if (packFrame) {
      ((JFrame)frame).pack();
    }
    else {
      ((JFrame)frame).validate();
    }
    //Center the window
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    Dimension frameSize = ((JFrame)frame).getSize();
    if (frameSize.height > screenSize.height) {
      frameSize.height = screenSize.height;
    }
    if (frameSize.width > screenSize.width) {
      frameSize.width = screenSize.width;
    }
    ((JFrame)frame).setLocation((screenSize.width - frameSize.width) / 2, (screenSize.height - frameSize.height) / 2);
    ((JFrame)frame).setVisible(true);
  }

  //Main method
  public static void main(String[] args) {
    try {
      UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
    }
    catch(Exception e) {
      e.printStackTrace();
    }
    new Gdh( (Object)null);
    new JopSpiderFrame("Frame");
  }

}
