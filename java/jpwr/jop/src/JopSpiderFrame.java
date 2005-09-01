/* 
 * Proview   $Id: JopSpiderFrame.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
