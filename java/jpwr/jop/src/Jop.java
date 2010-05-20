/* 
 * Proview   $Id: Jop.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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


/**
 * Title:
 * Description:
 * Copyright:    Copyright (c) 2000
 * Company:
 * @author 	 CS
 * @version 1.0
 */

package jpwr.jop;

public class Jop {
  public static final int BUTTON_ACTION_SET = 0;
  public static final int BUTTON_ACTION_RESET = 1;
  public static final int BUTTON_ACTION_TOGGLE = 2;
  public static final int BUTTON_ACTION_COMMAND = 3;

  public static int executeCommand( JopSession session, String command) {
    return JopSpider.command( session, command);
  }
}
