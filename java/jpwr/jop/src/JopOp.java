package jpwr.jop;
import java.awt.*;
import java.io.*;
import javax.swing.*;
import jpwr.rt.*;

/**
   Main class for jop operator environment.
   <p>
   The operator enviroment can be opened with the command 
   > jpwr_opwind.sh.
 */
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

    UIManager.put("Button.font", Jop.defFont14);
    UIManager.put("ToggleButton.font", Jop.defFont14);
    UIManager.put("RadioButton.font", Jop.defFont14);
    UIManager.put("CheckBox.font", Jop.defFont14);
    UIManager.put("ColorChooser.font", Jop.defFont14);
    UIManager.put("ComboBox.font", Jop.defFont14);
    UIManager.put("Label.font", Jop.defFont14);
    UIManager.put("List.font", Jop.defFont14);
    UIManager.put("MenuBar.font", Jop.defFont14);
    UIManager.put("MenuItem.font", Jop.defFont14);
    UIManager.put("RadioButtonMenuItem.font", Jop.defFont14);
    UIManager.put("CheckBoxMenuItem.font", Jop.defFont14);
    UIManager.put("Menu.font", Jop.defFont14);
    UIManager.put("PopupMenu.font", Jop.defFont14);
    UIManager.put("OptionPane.font", Jop.defFont14);
    UIManager.put("Panel.font", Jop.defFont14);
    UIManager.put("ProgressBar.font", Jop.defFont14);
    UIManager.put("ScrollPane.font", Jop.defFont14);
    UIManager.put("Viewport.font", Jop.defFont14);
    UIManager.put("TabbedPane.font", Jop.defFont14);
    UIManager.put("Table.font", Jop.defFont14);
    UIManager.put("TableHeader.font", Jop.defFont14);
    UIManager.put("TextField.font", Jop.defFont14);
    UIManager.put("PasswordField.font", Jop.defFont14);
    UIManager.put("TextArea.font", Jop.defFont14);
    UIManager.put("TextPane.font", Jop.defFont14);
    UIManager.put("EditorPane.font", Jop.defFont14);
    UIManager.put("TitledBorder.font", Jop.defFont14);
    UIManager.put("ToolBar.font", Jop.defFont14);
    UIManager.put("ToolTip.font", Jop.defFont14);
    UIManager.put("Tree.font", Jop.defFont14);
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









