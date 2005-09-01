/* 
 * Proview   $Id: GeColorIntensityEditor.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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

public class GeColorIntensityEditor extends PropertyEditorSupport {

  public GeColorIntensityEditor() {
  }
  private static String[] tagStrings = {
	"More",
	"Less",
	"Reset",
	};
  public String[] getTags() {
    return tagStrings;
  }
  public String getJavaInitializationString() {
    return java.lang.String.valueOf(((Number)getValue()).intValue());
  }
  public void setAsText(String s) throws IllegalArgumentException {
    int value = ((Number)getValue()).intValue();
    if (s.equals("More")) {
      if ( ++value > 2)
        value = 2;
      setValue( new Integer(value));
    }
    else if (s.equals("Less")) {
      if ( --value < -2)
        value = -2;
      setValue( new Integer(value));
    }
    else if (s.equals("Reset")) {
     setValue( new Integer(0));
    }
    else throw new IllegalArgumentException(s);
  }
}
