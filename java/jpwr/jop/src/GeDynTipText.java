package jpwr.jop;
import jpwr.rt.*;
import java.awt.*;
import javax.swing.*;

public class GeDynTipText extends GeDynElem {
  String text;

  public GeDynTipText( GeDyn dyn, String text) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_TipText);
    this.text = text;
  }
  public void connect() {
    ((JComponent)dyn.comp).setToolTipText( text);
  }
  public void disconnect() {
  }
  public void scan() {
  }
}









