/* 
 * Proview   $Id: JopXttApplet.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
import java.util.*;
import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;
import java.awt.font.*;
import java.awt.geom.*;
import java.awt.image.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;
import java.net.URL;
import jpwr.rt.*;

/**
 *  Description of the Class
 *
 *@author     JN3920
 *@created    November 12, 2002
 */
public class JopXttApplet extends JopApplet
{
  boolean sim = false;
  /**  Description of the Field */
  XttTree xttTree;

  /**
   *  Sets the size attribute of the JopXttApplet object
   *
   *@param  width   The new size value
   *@param  height  The new size value
   */
  public void setSize(int width, int height)
  {
    Logg.logg("JopXttApplet: setSize()", 1);

    super.setSize(width, height);

    validate();
  }


  /**
   *  Description of the Method
   *
   *@return    Description of the Return Value
   */
  public String GetAppletInfo()
  {
    return "Applet information";
  }


  /**
   *  Gets the parameterInfo attribute of the JopXttApplet object
   *
   *@return    The parameterInfo value
   */
  public String[][] getParameterInfo()
  {
    return null;
  }
  /**  Description of the Method */
  public void destroy()
  {
    if(!sim)
    {
      super.destroy();
    }
  }

  /**  Description of the Method */
  public void init()
  {
    super.init();
    setSize(new Dimension(370, 570));
    URL url = this.getCodeBase();
    Logg.loggTo = Logg.TO_SCREEN;
    Logg.loggPrio = 0;
    Logg.logg("JopXttApplet: Före XttTree-skapande", 6);
    this.xttTree = new XttTree( session, url, this);
    this.getContentPane().add(this.xttTree);
    this.xttTree.createUserInputs();
    Logg.logg("JopXttApplet: XttTree-skapande klart", 6);
  }

  /**  Description of the Method */
  public void start()
  {
    Logg.logg("JopXttApplet: start() anropad", 6);
    this.xttTree.tree.requestFocus();
  }


  /**  Description of the Method */
  public void stop()
  {
  }
}

