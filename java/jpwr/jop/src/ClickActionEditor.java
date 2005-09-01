/* 
 * Proview   $Id: ClickActionEditor.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
import java.beans.*;

/**
 * Title:
 * Description:
 * Copyright:    Copyright (c) 2000
 * Company:
 * @author 	 CS
 * @version 1.0
 */

public class ClickActionEditor extends PropertyEditorSupport {

  public ClickActionEditor() {
  }
  private static String[] tagStrings = { "Set", "Reset", "Toggle", "Command", };
  public String[] getTags() {
    return tagStrings;
  }
  public String getJavaInitializationString() {
    switch (((Number)getValue()).intValue()) {
      default:
      case Jop.BUTTON_ACTION_SET:
        return "Jop.BUTTON_ACTION_SET";
      case Jop.BUTTON_ACTION_RESET:
       return "Jop.BUTTON_ACTION_RESET";
      case Jop.BUTTON_ACTION_TOGGLE:
        return "Jop.BUTTON_ACTION_TOGGLE";
      case Jop.BUTTON_ACTION_COMMAND:
        return "Jop.BUTTON_ACTION_COMMAND";
    }
  }
  public void setAsText(String s) throws IllegalArgumentException {
    if (s.equals("Set")) setValue( new Integer(Jop.BUTTON_ACTION_SET));
    else if (s.equals("Reset")) setValue( new Integer(Jop.BUTTON_ACTION_RESET));
    else if (s.equals("Toggle")) setValue( new Integer(Jop.BUTTON_ACTION_TOGGLE));
    else if (s.equals("Command")) setValue( new Integer(Jop.BUTTON_ACTION_COMMAND));
    else throw new IllegalArgumentException(s);
  }
}
