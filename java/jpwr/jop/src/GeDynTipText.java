package jpwr.jop;
import jpwr.rt.*;
import java.awt.*;
import javax.swing.*;

public class GeDynTipText extends GeDynElem {
  String text;

  boolean attrFound = false;

  public GeDynTipText( GeDyn dyn, String text) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_TipText);
    this.text = text;
  }
  public void connect() {
    if ( text.charAt(0) == '&') {
      String attrName = dyn.getAttrName( text.substring(1));        
	
      CdhrString cdhr = dyn.en.gdh.getObjectInfoString( attrName);
      if ( cdhr.evenSts()) return;

      ((JComponent)dyn.comp).setToolTipText( cdhr.str);
      attrFound = true;
    }
    else
      ((JComponent)dyn.comp).setToolTipText( text);
  }
  public void disconnect() {
  }
  public void scan() {
  }
}









