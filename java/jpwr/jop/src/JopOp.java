package jpwr.jop;
import java.awt.*;
import java.io.*;
import javax.swing.*;
import jpwr.rt.*;

public class JopOp {
  boolean packFrame = false;

  //Construct the application
  public JopOp() {

    JopOpWindowFrame frame = new JopOpWindowFrame();
    //Validate frames that have preset sizes
    //Pack frames that have useful preferred size info, e.g. from their layout
    if (packFrame) {
      frame.pack();
    }
    else {
      frame.validate();
    }
    //Center the window
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    Dimension frameSize = frame.getSize();
    if (frameSize.height > screenSize.height) {
      frameSize.height = screenSize.height;
    }
    if (frameSize.width > screenSize.width) {
      frameSize.width = screenSize.width;
    }
    frame.setBounds( 0, 0, frameSize.width, screenSize.height);
    // frame.setLocation((screenSize.width - frameSize.width) / 2, (screenSize.height - frameSize.height) / 2);
    frame.setVisible(true);
  }

  //Main method
  public static void main(String[] args) {
    if ( args.length > 0)
      System.out.println("Arg: " + args[0]);
    try {
      UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
      // UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
    }
    catch(Exception e) {
      System.out.println("setLookAndFeel exception");
      // e.printStackTrace();
    }
    new JopOp();
    byte[] buff = new byte[100];
    try {
     System.in.read(buff);
    }
    catch ( IOException e) {}
  }
}









