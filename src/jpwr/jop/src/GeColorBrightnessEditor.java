
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

public class GeColorBrightnessEditor extends PropertyEditorSupport {

  public GeColorBrightnessEditor() {
  }
  private static String[] tagStrings = {
	"Lighter",
	"Darker",
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
    if (s.equals("Lighter")) {
      if ( ++value > 3)
        value = 3;
      setValue( new Integer(value));
    }
    else if (s.equals("Darker")) {
      if ( --value < -3)
        value = -3;
      setValue( new Integer(value));
    }
    else if (s.equals("Reset")) {
     setValue( new Integer(0));
    }
    else throw new IllegalArgumentException(s);
  }
}
