package jpwr.jop;
import jpwr.rt.*;
import java.awt.event.*;
import javax.swing.*;

public class GeDynValueInput extends GeDynElem {
  double minValue;
  double maxValue;

  boolean attrFound = false;
  int typeId;
  GeDynValue valueElement;

  public GeDynValueInput( GeDyn dyn, double minValue, double maxValue) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_ValueInput);
    this.minValue = minValue;
    this.maxValue = maxValue;
  }
  public void connect() {
    // Find the value element
    if ( dyn.elements == null)
      return;

    for ( int i = 0; i < dyn.elements.length; i++) {
      if ( dyn.elements[i].getDynType() == GeDyn.mDynType_Value) {
	valueElement = (GeDynValue)dyn.elements[i];
	typeId = valueElement.typeId;
	attrFound = valueElement.attrFound;
	break;
      }
    }
  }
  public void disconnect() {
  }
  public void action( int eventType, MouseEvent e) {
    switch ( eventType) {
    case GeDyn.eEvent_FocusLost: {
      if (!attrFound)
	break;

      valueElement.firstScan = true;
      break;
    }
    case GeDyn.eEvent_ValueChanged: {
      if (!attrFound)
	break;

      if ( (dyn.actionType & GeDyn.mActionType_Confirm) != 0)
	break;

      String text = ((JTextField)dyn.comp).getText();
      PwrtStatus sts;

      try {
	if ( typeId == Pwr.eType_Float32) {
	  float inputValue = Float.parseFloat( text.trim());
	  valueElement.oldValueF = inputValue;
	  if ( minValue == 0 && maxValue == 0) {
	    String attrName = dyn.getAttrNameNoSuffix( valueElement.attribute);
	    sts = dyn.en.gdh.setObjectInfo( attrName, inputValue);
	    if ( sts.evenSts())
	      System.out.println( "setObjectInfoError " + sts);
	  }
	  else {
	    if ( inputValue >= minValue && inputValue <= maxValue ) {
	      String attrName = dyn.getAttrNameNoSuffix( valueElement.attribute);        
	      sts = dyn.en.gdh.setObjectInfo( attrName, inputValue);
	      if ( sts.evenSts())
		System.out.println( "setObjectInfoError " + attrName + " " + sts);
	    }
	    else
	      valueElement.oldValueF = -10000;
	  }
	}
	else if ( typeId == Pwr.eType_Int32 ||
		  typeId == Pwr.eType_UInt32 ||
		  typeId == Pwr.eType_Int16 ||
		  typeId == Pwr.eType_UInt16 ||
		  typeId == Pwr.eType_Int8 ||
		  typeId == Pwr.eType_UInt8) {
	  int inputValue = Integer.parseInt( text.trim(), 10);
	  valueElement.oldValueI = inputValue;
	  if ( minValue == 0 && maxValue == 0) {
	    String attrName = dyn.getAttrNameNoSuffix( valueElement.attribute);        
	    sts = dyn.en.gdh.setObjectInfo( attrName, inputValue);
	    if ( sts.evenSts())
	      System.out.println( "setObjectInfoError " + sts);
	  }
	  else {
	    if ( inputValue >= minValue && inputValue <= maxValue ) {
	      String attrName = dyn.getAttrNameNoSuffix( valueElement.attribute);        
	      sts = dyn.en.gdh.setObjectInfo( attrName, inputValue);
	      if ( sts.evenSts())
		System.out.println( "setObjectInfoError " + sts);
	    }
	    else
	      valueElement.oldValueI = -10000;
	  }
	}
	else if ( typeId == Pwr.eType_String) {
	  valueElement.oldValueS = text;
	  String attrName = dyn.getAttrNameNoSuffix( valueElement.attribute);        
	  sts = dyn.en.gdh.setObjectInfo( attrName, text);
	  if ( sts.evenSts())
	    System.out.println( "setObjectInfoError " + sts);
	}
      }
      catch(NumberFormatException ex) {
	  System.out.println( ex.toString() );
      }
      break;
    }
    }
  }
}









