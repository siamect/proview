package jpwr.jop;
import jpwr.rt.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

public class GeDynAnalogText extends GeDynOptionMenu {

  public GeDynAnalogText( GeDyn dyn, String attribute, String[] itemsText, int[] itemsEnum) {
    super( dyn, attribute, itemsText, itemsEnum);
    this.dynType = GeDyn.mDynType_AnalogText;
    this.actionType = GeDyn.mActionType_No;
  }

  public void action( int eventType, MouseEvent e) {
  }
}













