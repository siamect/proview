
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

  public static void executeCommand( JopSession session, String command) {
    JopSpider.command( session, command);
  }
}
