package jpwr.jop;
import jpwr.rt.*;
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;

public class GeDynCloseGraph extends GeDynElem {
  String command;

  public GeDynCloseGraph( GeDyn dyn) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_CloseGraph);
    this.command = command;
  }
  public void action( int eventType, MouseEvent e) {
    switch ( eventType) {
    case GeDyn.eEvent_MB1Down:
      ((GeComponent)dyn.comp).colorInverse = 1;
      dyn.repaintNow = true;
      break;
    case GeDyn.eEvent_MB1Up:
      ((GeComponent)dyn.comp).colorInverse = 0;
      dyn.repaintNow = true;
      break;
    case GeDyn.eEvent_MB1Click:
      if ( (dyn.actionType & GeDyn.mActionType_Confirm) != 0)
	break;

      JopDynamic comp = (JopDynamic)dyn.comp;
      if ( comp.dynamicGetRoot() instanceof JFrame)
	((JFrame)comp.dynamicGetRoot()).dispose();
      break;
    }
  }
}









