
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
