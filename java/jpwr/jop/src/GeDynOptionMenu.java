package jpwr.jop;
import jpwr.rt.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

public class GeDynOptionMenu extends GeDynElem {
  String attribute;
  String[] itemsText;
  int[] itemsEnum;

  public boolean attrFound;
  PwrtRefId subid;
  int p;
  public int typeId;
  public boolean oldValueB;
  public float oldValueF;
  public int oldValueI;
  boolean firstScan = true;
  int color = GeColor.COLOR_32;

  public GeDynOptionMenu( GeDyn dyn, String attribute, String[] itemsText, int[] itemsEnum) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_OptionMenu);
    this.attribute = attribute;
    this.itemsText = itemsText;
    this.itemsEnum = itemsEnum;
  }

  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "OptionMenu: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	typeId = ret.typeId;
      }
    }
  }
  public void disconnect() {
    if ( attrFound)
      dyn.en.gdh.unrefObjectInfo( subid);
  }
  public void scan() {
    if ( !attrFound)
      return;

    int value = 0;
    boolean update = false;
    if ( typeId == Pwr.eType_Float32) {
      float value0 = dyn.en.gdh.getObjectRefInfoFloat( p);
      if ( value0 != oldValueF  || firstScan) {
	value = (int) value0;
	update = true;
        oldValueF = value0;
      }
    }
    else if ( typeId == Pwr.eType_Int32 ||
	      typeId == Pwr.eType_UInt32 ||
              typeId == Pwr.eType_Int16 ||
	      typeId == Pwr.eType_UInt16 ||
              typeId == Pwr.eType_Int8 ||
	      typeId == Pwr.eType_UInt8) {
      int value0 = dyn.en.gdh.getObjectRefInfoInt( p);
      if ( value0 != oldValueI || firstScan) {
	value = value0;
	update = true;
        oldValueI = value0;
      }
    }
    else
      return;

    if ( update) {
      boolean found = false;
      for ( int i = 0; i < 32; i++) {
	if ( itemsEnum[i] == value) {
	  dyn.comp.setAnnot1( itemsText[i]);
	  dyn.repaintNow = true;
	  found = true;
	  break;
	}
      }
      if ( !found)
	dyn.comp.setAnnot1( "");
    }

    if ( firstScan)
      firstScan = false;
  }

  public void action( int eventType, MouseEvent e) {
    switch ( eventType) {
    case GeDyn.eEvent_MB1Down:
      break;
    case GeDyn.eEvent_MB1Up:
      break;
    case GeDyn.eEvent_MB1Click:
      new OptionMenu((Component)dyn.comp, 
		     2, ((Component)dyn.comp).getHeight() - 2,
		     GeColor.getColor( color,
 				       ((GeComponent)dyn.comp).colorTone,
 				       ((GeComponent)dyn.comp).colorShift,
				       ((GeComponent)dyn.comp).colorIntensity,
				       ((GeComponent)dyn.comp).colorBrightness,
				       ((GeComponent)dyn.comp).colorInverse,
				       ((GeComponent)dyn.comp).fillColor),
		     ((Component)dyn.comp).getWidth() - 4);
      break;
    }
  }

  class OptionMenu implements ActionListener, PopupMenuListener {
    Component invoker;

    OptionMenu( Component invoker, int x, int y, Color color, int width) {
      this.invoker = invoker;
      JMenuItem item;
      JPopupMenu popup = new JPopupMenu();

      for ( int i = 0; i < 32; i++) {
        if ( itemsText[i] != null) {
	  popup.add( item = new JMenuItem( itemsText[i]));
	  item.addActionListener( this);
	  item.setBackground(color);
        }
      }
      popup.setBackground(color);
      popup.addPopupMenuListener( this);
      popup.show( invoker, x, y);
      popup.setPopupSize( width, popup.getHeight());
    }

    public void popupMenuWillBecomeVisible( PopupMenuEvent e) {}
    public void popupMenuWillBecomeInvisible( PopupMenuEvent e) {}
    public void popupMenuCanceled( PopupMenuEvent e) {}

    public void actionPerformed( ActionEvent event) {

      for ( int i = 0; i < 32; i++) {
        if ( itemsText[i] != null && itemsText[i].equals( event.getActionCommand())) {
	  PwrtStatus sts;

	  if ( typeId == Pwr.eType_Float32) {
	    float value = (float) itemsEnum[i];

	    String attrName = dyn.getAttrNameNoSuffix( attribute);        
	    sts = dyn.en.gdh.setObjectInfo( attrName, value);
	    if ( sts.evenSts())
	      System.out.println( "setObjectInfoError " + attrName + " " + sts);
	  }
	  else if ( typeId == Pwr.eType_Int32 ||
		    typeId == Pwr.eType_UInt32 ||
		    typeId == Pwr.eType_Int16 ||
		    typeId == Pwr.eType_UInt16 ||
		    typeId == Pwr.eType_Int8 ||
		    typeId == Pwr.eType_UInt8) {
	    String attrName = dyn.getAttrNameNoSuffix( attribute);        
	    sts = dyn.en.gdh.setObjectInfo( attrName, itemsEnum[i]);
	    if ( sts.evenSts())
	      System.out.println( "setObjectInfoError " + sts);
	  }
	  else
	    return;
	  // System.out.println("Action " + event.getActionCommand());
	  break;
        }
      }
    }  

  }
}













