/* 
 * Proview   $Id: Logg.java,v 1.2 2005-09-01 14:57:51 claes Exp $
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
import java.sql.Timestamp;
import java.util.Date;
import javax.swing.*;
/**
 *  Description of the Class
 *
 *@author     JN3920
 *@created    November 12, 2002
 */
public class Logg
{
  /**  Description of the Field */
  public static int NOTHING = 0;
  /**  Description of the Field */
  public static int TO_FILE = 1;
  /**  Description of the Field */
  public static int TO_SCREEN = 2;
  /**  Description of the Field */
  public static int loggPrio = 4;
  //public File loggFile;
  /**  Description of the Field */
  public static int loggTo = Logg.NOTHING;
  /**  Description of the Field */
  public static JLabel text_lbl = null;

  public static JPanel messagePanel = null;
  /**
   *  Description of the Method
   *
   *@param  loggText  Description of the Parameter
   *@param  prio      Description of the Parameter
   */
  public static void logg(String loggText, int prio)
  {
    if(prio <= Logg.loggPrio)
    {
      if((Logg.loggTo & Logg.TO_SCREEN) > 0)
      {
        System.out.println(loggText + " Tid: " + (new Timestamp(new Date().getTime())).toString());
      }
      if((Logg.loggTo & Logg.TO_FILE) > 0)
      {
        //to be written
      }
    }
  }


  /**
   *  Description of the Method
   *
   *@param  loggText  Description of the Parameter
   */
  public static void loggToApplet(String loggText)
  {
    if(text_lbl != null)
    {
      text_lbl.setText(loggText
      /*
       *  + " Tid: " + (new Timestamp(new Date().getTime())).toString()
       */
          );
      if(messagePanel != null)
      {
        messagePanel.doLayout();
	messagePanel.repaint();
      }
    }
    else
    {
      Logg.logg("To text_lbl: " + loggText, 1);
    }
  }
  public static String getAppletLogStr()
  {
    if(text_lbl != null)
      return text_lbl.getText();
    else
      return "text_lbl = null";
  }

  /**
   *  The main program for the Logg class
   *
   *@param  args  The command line arguments
   */
  public static void main(String[] args)
  {
    Logg.loggTo = Logg.TO_SCREEN;
    System.out.println(Logg.loggTo);
    Logg.logg("Tjoflöjt", 3);
  }
}

