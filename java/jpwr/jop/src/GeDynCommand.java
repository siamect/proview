package jpwr.jop;
import jpwr.rt.*;
import java.awt.event.*;

public class GeDynCommand extends GeDynElem {
  String command;

  public GeDynCommand( GeDyn dyn, String command) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_Command);
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

      if ( dyn.instance == null)
	Jop.executeCommand( dyn.session, command);
      else
	Jop.executeCommand( dyn.session, RtUtilities.strReplace(command, "$object", dyn.instance));

      break;
    }
  }
}

