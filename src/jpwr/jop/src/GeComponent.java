
package jpwr.jop;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import javax.swing.Timer;
import java.awt.event.*;
import jpwr.rt.*;

public class GeComponent extends JComponent implements JopDynamic, JopConfirm,
	ActionListener {
  public Dimension size;
  public Object root;
  public JopEngine en;
  public GeDyndata dd = new GeDyndata();
  public int level;
  Timer timer = new Timer(100, this);
  StringBuffer sb = new StringBuffer();
  public JopSession session;
  public GeComponent component = this;
  public GeComponent()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    timer.start();
  }
  public void actionPerformed(ActionEvent e) {
    boolean engine_found = false;

    root = getTopLevelAncestor();
    if ( root != null) {
      if ( root instanceof JopApplet)
	session = ((JopApplet)root).session;
      else if ( root instanceof JopFrame)
	session = ((JopFrame) root).session;
      en = session.getEngine();

      // Get level
      Container parent = getParent();
      while ( parent != root && parent != null) {
	level++;
        parent = parent.getParent();
      }
      
      if ( !en.isReady())
        return;
      en.add(this);
      engine_found = true;
    }
    if ( engine_found) {
      timer.stop();
      timer = null; 
      if ( (dd.type >= 1000 && en.gdh.isAuthorized( dd.access)) ||
	   dd.refObject != null) {
        this.addMouseListener(new MouseAdapter() {
          public void mouseReleased(MouseEvent e) {
	    if ( e.isPopupTrigger()) {
	      if ( dd.refObject != null) {
		if ( dd.refObject.startsWith("!")) {
		  // Name of an attribute that contains the objid of the reference object
		  CdhrObjid reto = en.gdh.getObjectInfoObjid( dd.refObject.substring(1));

		  if ( reto.oddSts() && !reto.objid.isNull()) {
		    CdhrString rets = en.gdh.objidToName( reto.objid, Cdh.mName_volumeStrict);
		    System.out.println( "str: " + rets.str + " " + rets.getSts());
		    if ( rets.oddSts() && ! rets.str.equals(""))
	              new JopMethodsMenu( session, rets.str, JopUtility.GRAPH, 
				    component, e.getX(), e.getY());
		  }
		}
		else {
	          new JopMethodsMenu( session, dd.refObject, JopUtility.GRAPH, 
				    component, e.getX(), e.getY());
		}
	      }
	      return;
	    }

	    if ( !en.gdh.isAuthorized( dd.access) || (e.getModifiers() & MouseEvent.BUTTON1_MASK) == 0)
	      return;

            colorInverse = 0;
	    repaintForeground();

	    //          public void mouseClicked(MouseEvent e) {
            PwrtStatus sts;
            String attrName;
            switch ( dd.type) {
	      case GeDyndata.eTrace_SetDig:
	      case GeDyndata.eTrace_SetDigWithTone:
                attrName = dd.getAttrName( dd.data[0]);        
                sts = en.gdh.setObjectInfo( attrName, true);
                if ( sts.evenSts())
                  System.out.println( "setObjectInfoError " + sts);
                break;
	      case GeDyndata.eTrace_ResetDig:
	      case GeDyndata.eTrace_ResetDigWithTone:
                attrName = dd.getAttrName( dd.data[0]);        
                sts = en.gdh.setObjectInfo( attrName, false);
                if ( sts.evenSts())
                  System.out.println( "setObjectInfoError " + sts);
                break;
	      case GeDyndata.eTrace_ToggleDig:
	      case GeDyndata.eTrace_ToggleDigWithTone:
	      case GeDyndata.eTrace_DigShiftWithToggleDig:
                attrName = dd.getAttrName( dd.data[0]);        
                sts = en.gdh.toggleObjectInfo( attrName);
                if ( sts.evenSts())
                  System.out.println( "setObjectInfoError " + sts);
                break;
	      case GeDyndata.eTrace_StoDigWithTone:
                attrName = dd.getAttrName( dd.data[0]);        
                sts = en.gdh.setObjectInfo( attrName, false);
                if ( sts.evenSts())
                  System.out.println( "setObjectInfoError " + sts);
                break;
	      case GeDyndata.eTrace_IncrAnalog:
                attrName = dd.getAttrName( dd.data[0]);
	        CdhrFloat ret = en.gdh.getObjectInfoFloat( attrName);
                float incr, minvalue, maxvalue;
                if ( ret.evenSts())
                  System.out.println( "getObjectInfoError " + ret.getSts());
	        // Increment value
	        try {
                  incr = Float.valueOf( dd.data[1]).floatValue();
	        }
	        catch ( NumberFormatException e_format) {
	          incr = 1;
	        }
	        // Minimum value
	        try {
                  minvalue = Float.valueOf( dd.data[2]).floatValue();
	        }
	        catch ( NumberFormatException e_format) {
	          minvalue = 0;
	        }
	        // Maximum value
	        try {
                  maxvalue = Float.valueOf( dd.data[3]).floatValue();
	        }
	        catch ( NumberFormatException e_format) {
	          maxvalue = 0;
	        }

	        ret.value += incr;
	        if ( !( minvalue == 0 && maxvalue == 0)) {
                  if ( ret.value < minvalue)
                    ret.value = minvalue;
                  if ( ret.value > maxvalue)
                    ret.value = maxvalue;
	        }
                sts = en.gdh.setObjectInfo( attrName, ret.value);
                if ( sts.evenSts())
                  System.out.println( "setObjectInfoError " + sts);
                break;
	      case GeDyndata.eTrace_Command:
                if ( dd.data[0].compareTo("") != 0)
	          Jop.executeCommand( session, dd.data[0]);
	        break;
	      case GeDyndata.eTrace_DigWithCommand:
	      case GeDyndata.eTrace_DigWithErrorAndCommand:
	      case GeDyndata.eTrace_DigToneWithCommand:
	      case GeDyndata.eTrace_DigToneWithErrorAndCommand:
	      case GeDyndata.eTrace_DigTwoWithCommand:
	      case GeDyndata.eTrace_DigToneTwoWithCommand:
                if ( dd.data[3].compareTo("") != 0)
	          Jop.executeCommand( session, dd.data[3]);
	        break;
              case GeDyndata.eTrace_SetDigConfirm:	    
              case GeDyndata.eTrace_ResetDigConfirm:	    
              case GeDyndata.eTrace_ToggleDigConfirm:	    
              case GeDyndata.eTrace_CommandConfirm:	    
              case GeDyndata.eTrace_SetDigConfirmWithTone:	    
              case GeDyndata.eTrace_ResetDigConfirmWithTone:	    
              case GeDyndata.eTrace_ToggleDigConfirmWithTone:
                // TODO...
                JopConfirmDialog.open( component, dd.data[1]);
//                JOptionPane.showMessageDialog( (Component) root, dd.data[1], 
//			"Click OK", JOptionPane.INFORMATION_MESSAGE);
	        break;
              case GeDyndata.eTrace_AnnotInput:
              case GeDyndata.eTrace_AnnotInputWithTone:
                // TODO...
	        break;  	  
	      case GeDyndata.eTrace_RadioButton:
		// Get all radiobutton in the same group
		Container parent = getParent();
                if ( parent == null) break;

                for ( int i = 0; i < parent.getComponentCount(); i++) {
		  GeComponent c = (GeComponent) parent.getComponent(i);
                  if ( c.dd.type == GeDyndata.eTrace_RadioButton &&
                       c.dd.data[0] != dd.data[0]) {
                    attrName = dd.getAttrName( c.dd.data[0]);        
                    sts = en.gdh.setObjectInfo( attrName, false);
		  }
                }
                attrName = dd.getAttrName( dd.data[0]);        
                sts = en.gdh.setObjectInfo( attrName, true);
                if ( sts.evenSts())
                  System.out.println( "setObjectInfoError " + sts);
                break;
            }
          }
          public void mousePressed(MouseEvent e) {
	    if ( e.isPopupTrigger()) {
	      if ( dd.refObject != null) {
		if ( dd.refObject.startsWith("!")) {
		  // Name of an attribute that contains the objid of the reference object
		  CdhrObjid reto = en.gdh.getObjectInfoObjid( dd.refObject.substring(1));
		  System.out.println( "Ref: " + dd.refObject.substring(1) + " " + reto.getSts());
		  if ( reto.oddSts() && !reto.objid.isNull()) {
		    CdhrString rets = en.gdh.objidToName( reto.objid, Cdh.mName_volumeStrict);
		    if ( rets.oddSts() && ! rets.str.equals(""))
	              new JopMethodsMenu( session, rets.str, JopUtility.GRAPH, 
				    component, e.getX(), e.getY());
		  }
		}
		else {
	          new JopMethodsMenu( session, dd.refObject, JopUtility.GRAPH, 
				    component, e.getX(), e.getY());
		}
	      }
	      return;
	    }

	    if ( !en.gdh.isAuthorized( dd.access) || (e.getModifiers() & MouseEvent.BUTTON1_MASK) == 0)
	      return;

            colorInverse = 1;
	    repaintForeground();

            PwrtStatus sts;
            String attrName;
            switch ( dd.type) {
	      case GeDyndata.eTrace_StoDigWithTone:
                attrName = dd.getAttrName( dd.data[0]);        
                sts = en.gdh.setObjectInfo( attrName, true);
                if ( sts.evenSts())
                  System.out.println( "setObjectInfoError " + sts);
                break;
            }
          }
        });
      }
  
    }
  }
  public void confirmYes() {
    PwrtStatus sts;
    String attrName;
    switch ( dd.type) {
      case GeDyndata.eTrace_SetDigConfirm:	    
      case GeDyndata.eTrace_SetDigConfirmWithTone:	    
        attrName = dd.getAttrName( dd.data[0]);        
        sts = en.gdh.setObjectInfo( attrName, true);
        if ( sts.evenSts())
          System.out.println( "setObjectInfoError " + sts);
        break;
      case GeDyndata.eTrace_ResetDigConfirm:	    
      case GeDyndata.eTrace_ResetDigConfirmWithTone:	    
        attrName = dd.getAttrName( dd.data[0]);        
        sts = en.gdh.setObjectInfo( attrName, false);
        if ( sts.evenSts())
          System.out.println( "setObjectInfoError " + sts);
        break;
      case GeDyndata.eTrace_ToggleDigConfirm:	    
      case GeDyndata.eTrace_ToggleDigConfirmWithTone:
        attrName = dd.getAttrName( dd.data[0]);        
        sts = en.gdh.toggleObjectInfo( attrName);
        if ( sts.evenSts())
          System.out.println( "setObjectInfoError " + sts);
        break;
      case GeDyndata.eTrace_CommandConfirm:    
        if ( dd.data[0].compareTo("") != 0)
	  Jop.executeCommand( session, dd.data[0]);
        break;
    }
  }
  public String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  public Font annot1Font = new Font("Helvetica", Font.BOLD, 14);
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public int fillColor = 9999;
  public int originalFillColor = 9999;
  public int borderColor = 9999;
  public int originalBorderColor = 9999;
  public int colorTone = 0;
  public int originalColorTone = 0;
  public int colorShift = 0;
  public int originalColorShift = 0;
  public int colorBrightness = 0;
  public int originalColorBrightness = 0;
  public int colorIntensity = 0;
  public int originalColorIntensity = 0;
  public int colorInverse = 0;
  public int originalColorInverse = 0;
  public void setColorTone( int colorTone) {
    this.colorTone = colorTone;
    originalColorTone = colorTone;
  }
  public int getColorTone() {
    return colorTone;
  }
  public void setColorShift( int colorShift) {
    this.colorShift = colorShift;
    originalColorShift = colorShift;
  }
  public int getColorShift() {
    return colorShift;
  }
  public void setColorBrightness( int colorBrightness) {
    this.colorBrightness = colorBrightness;
    originalColorBrightness = colorBrightness;
  }
  public int getColorBrightness() {
    return colorBrightness;
  }
  public void setColorIntensity( int colorIntensity) {
    this.colorIntensity = colorIntensity;
    originalColorIntensity = colorIntensity;
  }
  public int getColorIntensity() {
    return colorIntensity;
  }
  public void setFillColor( int fillColor) {
    this.fillColor = fillColor;
    this.originalFillColor = fillColor;
  }
  public void resetFillColor() {
    fillColor = originalFillColor;
  }
  public int getFillColor() {
    return fillColor;
  }
  public void setBorderColor( int borderColor) {
    this.borderColor = borderColor;
    this.originalBorderColor = borderColor;
  }
  public int getBorderColor() {
    return borderColor;
  }
  public int animationCount = 1;
  public int page = 1;
  public int pages = 1;
  public void setPage( int page) {
    if ( page <= pages && page > 0)
      this.page = page;
  }
  public int setNextPage() {
    if ( page == pages)
      return 0;
    page++;
    return 1;
  }
  public int setPreviousPage() {
    if ( page == 1)
      return 0;
    page--;
    return 1;
  }
  public void setFirstPage() {
    page = 1;
  }
  public void setLastPage() {
    page = pages;
  }
  public double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  public GdhrRefObjectInfo ret = null;
  public Object dynamicGetRoot() {
    return root;
  }
  public void dynamicOpen() {
    String attrName;
    if ( en.isInstance())
      dd.setInstance( en.getInstance());
    switch ( dd.type) {
      case GeDyndata.eTrace_Dig:
      case GeDyndata.eTrace_DigWithCommand:
      case GeDyndata.eTrace_DigBorder:
      case GeDyndata.eTrace_DigTone:
      case GeDyndata.eTrace_DigToneWithCommand:
        attrName = dd.getAttrName( dd.data[0]);
//	System.out.println("Attribute: " + attrName + " inv " +
//		GeDyndata.getAttrInverted( dd.data[0]));
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[0] = true;
            dd.p[0] = ret.id;
	    dd.subid[0] = ret.refid;
	    dd.typeId[0] = ret.typeId;
            dd.inverted[0] = GeDyndata.getAttrInverted( dd.data[0]);
          }
	}
        if ( dd.data[1] != null) {
          attrName = dd.getAttrName( dd.data[1]);
          if ( attrName.compareTo("") != 0) {
            ret = en.gdh.refObjectInfo( attrName);
            if ( ret.evenSts())
              System.out.println( "ObjectInfoError " + attrName);
            else {
              dd.attrFound[1] = true;
              dd.p[1] = ret.id;
	      dd.subid[1] = ret.refid;
	      dd.typeId[1] = ret.typeId;
              dd.inverted[1] = GeDyndata.getAttrInverted( dd.data[1]);
            }
          }
	}
	break;
      case GeDyndata.eTrace_Invisible:
      case GeDyndata.eTrace_RadioButton:
        attrName = dd.getAttrName( dd.data[0]);
//	System.out.println("Attribute: " + attrName + " inv " +
//		GeDyndata.getAttrInverted( dd.data[0]));
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[0] = true;
            dd.p[0] = ret.id;
	    dd.subid[0] = ret.refid;
	    dd.typeId[0] = ret.typeId;
            dd.inverted[0] = GeDyndata.getAttrInverted( dd.data[0]);
          }
	}
	break;
      case GeDyndata.eTrace_DigShiftWithToggleDig:
        attrName = dd.getAttrName( dd.data[1]);
//	System.out.println("Attribute: " + attrName + " inv " +
//		GeDyndata.getAttrInverted( dd.data[1]));
        if ( attrName.compareTo("") == 0)
          attrName = dd.getAttrName( dd.data[0]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[0] = true;
            dd.p[0] = ret.id;
	    dd.subid[0] = ret.refid;
	    dd.typeId[0] = ret.typeId;
            dd.inverted[0] = GeDyndata.getAttrInverted( dd.data[0]);
          }
	}
	break;
      case GeDyndata.eTrace_DigWithText:
        attrName = dd.getAttrName( dd.data[0]);
//	System.out.println("Attribute: " + attrName + " inv " +
//		GeDyndata.getAttrInverted( dd.data[0]));
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[0] = true;
            dd.p[0] = ret.id;
	    dd.subid[0] = ret.refid;
	    dd.typeId[0] = ret.typeId;
            dd.inverted[0] = GeDyndata.getAttrInverted( dd.data[0]);
            dd.lowText = dd.data[1];
            dd.highText = getAnnot1();
          }
	}
	break;
      case GeDyndata.eTrace_DigWithError:
      case GeDyndata.eTrace_DigWithErrorAndCommand:
      case GeDyndata.eTrace_DigToneWithError:
      case GeDyndata.eTrace_DigToneWithErrorAndCommand:
      case GeDyndata.eTrace_DigTwo:
      case GeDyndata.eTrace_DigTwoWithCommand:
      case GeDyndata.eTrace_DigToneTwo:
      case GeDyndata.eTrace_DigToneTwoWithCommand:
      case GeDyndata.eTrace_AnalogShift:
      case GeDyndata.eTrace_Animation:
      case GeDyndata.eTrace_AnimationForwBack:
      case GeDyndata.eTrace_DigAnimation:
      case GeDyndata.eTrace_DigShift:
        attrName = dd.getAttrName( dd.data[0]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[0] = true;
            dd.p[0] = ret.id;
	    dd.subid[0] = ret.refid;
	    dd.typeId[0] = ret.typeId;
            dd.inverted[0] = GeDyndata.getAttrInverted( dd.data[0]);
          }
	}
        attrName = dd.getAttrName( dd.data[1]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[1] = true;
            dd.p[1] = ret.id;
	    dd.subid[1] = ret.refid;
	    dd.typeId[1] = ret.typeId;
            dd.inverted[1] = GeDyndata.getAttrInverted( dd.data[1]);
          }
	}
        if  (dd.data[2] != null) {
          attrName = dd.getAttrName( dd.data[2]);
          if ( attrName.compareTo("") != 0) {
            ret = en.gdh.refObjectInfo( attrName);
            if ( ret.evenSts())
              System.out.println( "ObjectInfoError " + attrName);
            else {
              dd.attrFound[2] = true;
              dd.p[2] = ret.id;
	      dd.subid[2] = ret.refid;
	      dd.typeId[2] = ret.typeId;
              dd.inverted[2] = GeDyndata.getAttrInverted( dd.data[2]);
            }
          }
	}
	break;
      case GeDyndata.eTrace_Annot:
      case GeDyndata.eTrace_AnnotInput:
        attrName = dd.getAttrName( dd.data[0]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[0] = true;
            dd.p[0] = ret.id;
	    dd.subid[0] = ret.refid;
	    dd.typeId[0] = ret.typeId;
            dd.setFormat( dd.data[1]);
          }
	}
	break;
      case GeDyndata.eTrace_AnnotWithTone:
        attrName = dd.getAttrName( dd.data[0]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[0] = true;
            dd.p[0] = ret.id;
	    dd.subid[0] = ret.refid;
	    dd.typeId[0] = ret.typeId;
            dd.setFormat( dd.data[1]);
          }
	}
        attrName = dd.getAttrName( dd.data[2]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[1] = true;
            dd.p[1] = ret.id;
	    dd.subid[1] = ret.refid;
	    dd.typeId[1] = ret.typeId;
            dd.inverted[1] = GeDyndata.getAttrInverted( dd.data[2]);
          }
	}
        attrName = dd.getAttrName( dd.data[3]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[2] = true;
            dd.p[2] = ret.id;
	    dd.subid[2] = ret.refid;
	    dd.typeId[2] = ret.typeId;
            dd.inverted[2] = GeDyndata.getAttrInverted( dd.data[3]);
          }
	}
	break;
      case GeDyndata.eTrace_AnnotInputWithTone:
        attrName = dd.getAttrName( dd.data[0]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[0] = true;
            dd.p[0] = ret.id;
	    dd.subid[0] = ret.refid;
	    dd.typeId[0] = ret.typeId;
            dd.setFormat( dd.data[1]);
          }
	}
        attrName = dd.getAttrName( dd.data[2]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[1] = true;
            dd.p[1] = ret.id;
	    dd.subid[1] = ret.refid;
	    dd.typeId[1] = ret.typeId;
            dd.inverted[1] = GeDyndata.getAttrInverted( dd.data[2]);
          }
	}
	break;
      case GeDyndata.eTrace_Move:
	// XAttr
        attrName = dd.getAttrName( dd.data[0]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[0] = true;
            dd.p[0] = ret.id;
	    dd.subid[0] = ret.refid;
	    dd.typeId[0] = ret.typeId;
          }
	}
        // YAttr
        attrName = dd.getAttrName( dd.data[1]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[1] = true;
            dd.p[1] = ret.id;
	    dd.subid[1] = ret.refid;
	    dd.typeId[1] = ret.typeId;
          }
	}
        // ToneAttr
        attrName = dd.getAttrName( dd.data[2]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[2] = true;
            dd.p[2] = ret.id;
	    dd.subid[2] = ret.refid;
	    dd.typeId[2] = ret.typeId;
          }
	}
        // InvisAttr
        attrName = dd.getAttrName( dd.data[3]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[3] = true;
            dd.p[3] = ret.id;
	    dd.subid[3] = ret.refid;
	    dd.typeId[3] = ret.typeId;
          }
	}
        // ScaleXAttr
        attrName = dd.getAttrName( dd.data[4]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[4] = true;
            dd.p[4] = ret.id;
	    dd.subid[4] = ret.refid;
	    dd.typeId[4] = ret.typeId;
          }
	}
        // ScaleYAttr
        attrName = dd.getAttrName( dd.data[5]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[5] = true;
            dd.p[5] = ret.id;
	    dd.subid[5] = ret.refid;
	    dd.typeId[5] = ret.typeId;
          }
	}
	break;
      case GeDyndata.eTrace_Rotate:
	// Attr
        attrName = dd.getAttrName( dd.data[0]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[0] = true;
            dd.p[0] = ret.id;
	    dd.subid[0] = ret.refid;
	    dd.typeId[0] = ret.typeId;
          }
	}
        // ToneAttr
        attrName = dd.getAttrName( dd.data[3]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[3] = true;
            dd.p[3] = ret.id;
	    dd.subid[3] = ret.refid;
	    dd.typeId[3] = ret.typeId;
          }
	}
        // InvisAttr
        attrName = dd.getAttrName( dd.data[4]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[4] = true;
            dd.p[4] = ret.id;
	    dd.subid[4] = ret.refid;
	    dd.typeId[4] = ret.typeId;
          }
	}
	break;
      case GeDyndata.eTrace_SetDigConfirmWithTone:
      case GeDyndata.eTrace_ResetDigConfirmWithTone:
      case GeDyndata.eTrace_ToggleDigConfirmWithTone:
      case GeDyndata.eTrace_SetDigWithTone:
      case GeDyndata.eTrace_ResetDigWithTone:
      case GeDyndata.eTrace_ToggleDigWithTone:
      case GeDyndata.eTrace_StoDigWithTone:
        attrName = dd.getAttrName( dd.data[2]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[0] = true;
            dd.p[0] = ret.id;
	    dd.subid[0] = ret.refid;
	    dd.typeId[0] = ret.typeId;
            dd.inverted[0] = GeDyndata.getAttrInverted( dd.data[2]);
            dd.lowText = dd.data[4];
            dd.highText = getAnnot1();
          }
	}
        attrName = dd.getAttrName( dd.data[3]);
        if ( attrName.compareTo("") != 0) {
          ret = en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
            System.out.println( "ObjectInfoError " + attrName);
          else {
            dd.attrFound[1] = true;
            dd.p[1] = ret.id;
	    dd.subid[1] = ret.refid;
	    dd.typeId[1] = ret.typeId;
            dd.inverted[1] = GeDyndata.getAttrInverted( dd.data[3]);
          }
	}
	break;
    }
  }
  public void dynamicClose() {
    for ( int i = 0; i < 6; i++) {
      if ( dd.attrFound[i])
        en.gdh.unrefObjectInfo( dd.subid[i]);
    }
  }
  public void dynamicUpdate( boolean animationOnly) {
    // System.out.println( "Dynamic update: " + this);
    if ( animationOnly &&
         !( dd.type == GeDyndata.eTrace_Animation ||
            dd.type == GeDyndata.eTrace_AnimationForwBack ||
            dd.type == GeDyndata.eTrace_DigAnimation))
      return;
    switch ( dd.type) {
      case GeDyndata.eTrace_Dig:
      case GeDyndata.eTrace_DigWithCommand:
      {
        boolean repaintNow = false;

        if ( !dd.attrFound[0] && !dd.attrFound[1])
          return;
        if ( dd.attrFound[0]) {
          boolean value = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
          if ( value != dd.oldValueB[0] || dd.firstScan) {
            if ( (!dd.inverted[0] && !value) || (dd.inverted[0] && value))
	      fillColor = dd.color;
	    else
	      fillColor = originalFillColor;
	    repaintNow = true;
	    dd.oldValueB[0] = value;
          }
        }
        if ( dd.attrFound[1]) {
          boolean value = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value != dd.oldValueB[1] || dd.firstScan) {
            if ( (!dd.inverted[1] && value) || (dd.inverted[1] && !value))
              dd.invisible = true;
	    else
              dd.invisible = false;
	    repaintNow = true;
	    dd.oldValueB[1] = value;
          }
        }
        if ( repaintNow)
	  repaintForeground();
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_DigWithText:
      {
        if ( !dd.attrFound[0])
          return;
        boolean value = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
        if ( value != dd.oldValueB[0] || dd.firstScan) {
          if ( (!dd.inverted[0] && value) || (dd.inverted[0] && !value)) {
            colorTone = originalColorTone;
	    fillColor = originalFillColor;
            if ( dd.lowText != null && !dd.lowText.equals(new String("")))
              setAnnot1( dd.highText);
          }
	  else {
            if ( dd.color <= GeColor.COLOR_TONE_MAX)
              colorTone = dd.color;
            else
              fillColor = dd.color;
            if ( dd.lowText != null && !dd.lowText.equals(new String("")))
              setAnnot1( dd.lowText);
          }
	  repaintForeground();
	  dd.oldValueB[0] = value;
        }
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_DigBorder:
      {
        boolean repaintNow = false;

        if ( !dd.attrFound[0] && !dd.attrFound[1])
          return;
        if ( dd.attrFound[0]) {
          boolean value = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
          if ( value != dd.oldValueB[0] || dd.firstScan) {
            if ( (!dd.inverted[0] && !value) || (dd.inverted[0] && value))
	      borderColor = dd.color;
	    else
	      borderColor = originalBorderColor;
	    repaintNow = true;
	    dd.oldValueB[0] = value;
          }
        }
        if ( dd.attrFound[1]) {
          boolean value = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value != dd.oldValueB[1] || dd.firstScan) {
            if ( (!dd.inverted[1] && value) || (dd.inverted[1] && !value))
              dd.invisible = true;
	    else
              dd.invisible = false;
	    repaintNow = true;
	    dd.oldValueB[1] = value;
          }
        }
        if ( repaintNow)
	  repaintForeground();

        if ( !dd.attrFound[0])
          return;
        boolean value = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
        if ( value != dd.oldValueB[0] || dd.firstScan) {
          if ( (!dd.inverted[0] && !value) || (dd.inverted[0] && value))
	    borderColor = dd.color;
	  else
	    borderColor = originalBorderColor;
	  repaintForeground();
	  dd.oldValueB[0] = value;
        }
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }
      case GeDyndata.eTrace_Invisible:
      {
        boolean repaintNow = false;

        if ( !dd.attrFound[0])
          return;
        if ( dd.attrFound[0]) {
          boolean value = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
          if ( value != dd.oldValueB[0] || dd.firstScan) {
            if ( (!dd.inverted[0] && value) || (dd.inverted[0] && !value))
              dd.invisible = true;
	    else
              dd.invisible = false;
	    repaintNow = true;
	    dd.oldValueB[0] = value;
          }
        }
        if ( repaintNow)
	  repaintForeground();

        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }
      case GeDyndata.eTrace_DigWithError:
      case GeDyndata.eTrace_DigWithErrorAndCommand:
      {
        if ( !dd.attrFound[0])
          return;
        boolean value0 = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
        boolean value1;
        boolean value2;

        if ( dd.attrFound[1] && !dd.attrFound[2]) {
          value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value1 != dd.oldValueB[1] || value0 != dd.oldValueB[0] || 
		dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1))
              fillColor = GeColor.COLOR_RED;
            else if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0))
              fillColor = originalFillColor;
            else
              fillColor = dd.color;
            repaintForeground();
            dd.oldValueB[1] = value1;
            dd.oldValueB[0] = value0;
          }
        }
        else if ( dd.attrFound[2]  && !dd.attrFound[1]) {
          value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || value0 != dd.oldValueB[0] || 
		dd.firstScan) {
            if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2))
              fillColor = GeColor.COLOR_6;  // Yellow
            else if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0))
              fillColor = originalFillColor;
            else
              fillColor = dd.color;
            repaintForeground();
            dd.oldValueB[2] = value2;
            dd.oldValueB[0] = value0;
          }
        }
        else if ( dd.attrFound[2] && dd.attrFound[1]) {
          value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || value1 != dd.oldValueB[1] ||
	       value0 != dd.oldValueB[0] || dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1))
              fillColor = GeColor.COLOR_RED;
            else if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2))
              fillColor = GeColor.COLOR_6;  // Yellow
            else if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0))
              fillColor = originalFillColor;
            else
              fillColor = dd.color;
            repaintForeground();
            dd.oldValueB[2] = value2;
            dd.oldValueB[1] = value1;
            dd.oldValueB[0] = value0;
          }
        }
        else {
          if ( dd.oldValueB[0] != value0 || dd.firstScan) {
            if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0))
              fillColor = originalFillColor;
            else
              fillColor = dd.color;
            repaintForeground();
            dd.oldValueB[0] = value0;
          }
        }
        if ( dd.firstScan)
          dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_DigTwo:
      case GeDyndata.eTrace_DigTwoWithCommand:
      {
        if ( !dd.attrFound[0])
          return;
        boolean value0 = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
        boolean value1;
        boolean value2;

        if ( dd.attrFound[1] && !dd.attrFound[2]) {
          value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value1 != dd.oldValueB[1] || value0 != dd.oldValueB[0] || 
		dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1))
              fillColor = GeColor.COLOR_RED;
            else if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0))
              fillColor = originalFillColor;
            else
              fillColor = dd.color;
            repaintForeground();
            dd.oldValueB[1] = value1;
            dd.oldValueB[0] = value0;
          }
        }
        else if ( dd.attrFound[2]  && !dd.attrFound[1]) {
          value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || value0 != dd.oldValueB[0] || 
		dd.firstScan) {
            if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2))
              fillColor = dd.color2;
            else if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0))
              fillColor = originalFillColor;
            else
              fillColor = dd.color;
            repaintForeground();
            dd.oldValueB[2] = value2;
            dd.oldValueB[0] = value0;
          }
        }
        else if ( dd.attrFound[2] && dd.attrFound[1]) {
          value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || value1 != dd.oldValueB[1] ||
	       value0 != dd.oldValueB[0] || dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1))
              fillColor = GeColor.COLOR_RED;
            else if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2))
              fillColor = dd.color2;
            else if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0))
              fillColor = originalFillColor;
            else
              fillColor = dd.color;
            repaintForeground();
            dd.oldValueB[2] = value2;
            dd.oldValueB[1] = value1;
            dd.oldValueB[0] = value0;
          }
        }
        else {
          if ( dd.oldValueB[0] != value0 || dd.firstScan) {
            if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0))
              fillColor = originalFillColor;
            else
              fillColor = dd.color;
            repaintForeground();
            dd.oldValueB[0] = value0;
          }
        }
        if ( dd.firstScan)
          dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_DigTone:
      case GeDyndata.eTrace_DigToneWithCommand:
      case GeDyndata.eTrace_SetDigConfirmWithTone:
      case GeDyndata.eTrace_ResetDigConfirmWithTone:
      case GeDyndata.eTrace_ToggleDigConfirmWithTone:
      case GeDyndata.eTrace_SetDigWithTone:
      case GeDyndata.eTrace_ResetDigWithTone:
      case GeDyndata.eTrace_ToggleDigWithTone:
      case GeDyndata.eTrace_StoDigWithTone:
      {
        boolean repaintNow = false;

        if ( !dd.attrFound[0] && !dd.attrFound[1])
          return;
        if ( dd.attrFound[0]) {
          boolean value = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
          if ( value != dd.oldValueB[0] || dd.firstScan) {
            if ( (!dd.inverted[0] && value) || (dd.inverted[0] && !value)) {
              colorTone = originalColorTone;
	      fillColor = originalFillColor;
              if ( dd.lowText != null && !dd.lowText.equals(new String("")))
                setAnnot1( dd.highText);
            }
	    else {
              if ( dd.color <= GeColor.COLOR_TONE_MAX)
                colorTone = dd.color;
              else
                fillColor = dd.color;
              if ( dd.lowText != null && !dd.lowText.equals(new String("")))
                setAnnot1( dd.lowText);
            }
	    repaintNow = true;
	    dd.oldValueB[0] = value;
          }
        }
        if ( dd.attrFound[1]) {
          boolean value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value1 != dd.oldValueB[1] || dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1)) {
              dd.invisible = true;
            }
	    else {
              dd.invisible = false;
            }
	    repaintNow = true;
	    dd.oldValueB[1] = value1;
          }
        }
        if ( repaintNow)
	  repaintForeground();
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_DigToneWithError:
      case GeDyndata.eTrace_DigToneWithErrorAndCommand:
      {
        if ( !dd.attrFound[0])
          return;
        boolean value0 = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
        boolean value1;
        boolean value2;

        if ( dd.attrFound[1] && !dd.attrFound[2]) {
          value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value1 != dd.oldValueB[1] || value0 != dd.oldValueB[0] || 
		dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = GeColor.COLOR_TONE_RED;
            }
            else if ( (!dd.inverted[0] && !value0) || (dd.inverted[0] && value0)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = dd.color;
              else
                colorTone = dd.color;
            }
            else {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = originalColorTone;
            }
            repaintForeground();
            dd.oldValueB[1] = value1;
            dd.oldValueB[0] = value0;
          }
        }
        else if ( dd.attrFound[2] && !dd.attrFound[1]) {
          value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || value0 != dd.oldValueB[0] || 
		dd.firstScan) {
            if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = GeColor.COLOR_TONE_YELLOW;
            }
            else if ( (!dd.inverted[0] && !value0) || (dd.inverted[0] && value0)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = dd.color;
              else
                colorTone = dd.color;
            }
            else {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = originalColorTone;
            }
            repaintForeground();
            dd.oldValueB[2] = value2;
            dd.oldValueB[0] = value0;
          }
        }
        else if ( dd.attrFound[2] && dd.attrFound[1]) {
          value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value1 != dd.oldValueB[1] || value2 != dd.oldValueB[2] || 
		value0 != dd.oldValueB[0] || dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = GeColor.COLOR_TONE_RED;
            }
            else if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = GeColor.COLOR_TONE_YELLOW;
            }
            else if ( (!dd.inverted[0] && !value0) || (dd.inverted[0] && value0)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = dd.color;
              else
                colorTone = dd.color;
            }
            else {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = originalColorTone;
            }
            repaintForeground();
            dd.oldValueB[2] = value2;
            dd.oldValueB[1] = value1;
            dd.oldValueB[0] = value0;
          }
        }
        else {
          if ( dd.oldValueB[0] != value0 || dd.firstScan) {
            if ( (!dd.inverted[0] && !value0) || (dd.inverted[0] && value0)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = dd.color;
              else
                colorTone = dd.color;
            }
            else {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = originalColorTone;
            }
            repaintForeground();
            dd.oldValueB[0] = value0;
          }
        }
        if ( dd.firstScan)
          dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_DigToneTwo:
      case GeDyndata.eTrace_DigToneTwoWithCommand:
      {
        if ( !dd.attrFound[0])
          return;
        boolean value0 = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
        boolean value1;
        boolean value2;

        if ( dd.attrFound[1] && !dd.attrFound[2]) {
          value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value1 != dd.oldValueB[1] || value0 != dd.oldValueB[0] || 
		dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = GeColor.COLOR_TONE_RED;
            }
            else if ( (!dd.inverted[0] && !value0) || (dd.inverted[0] && value0)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = dd.color;
              else
                colorTone = dd.color;
            }
            else {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = originalColorTone;
            }
            repaintForeground();
            dd.oldValueB[1] = value1;
            dd.oldValueB[0] = value0;
          }
        }
        else if ( dd.attrFound[2] && !dd.attrFound[1]) {
          value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || value0 != dd.oldValueB[0] || 
		dd.firstScan) {
            if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = dd.color2;
              else
                colorTone = dd.color2;
            }
            else if ( (!dd.inverted[0] && !value0) || (dd.inverted[0] && value0)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = dd.color;
              else
                colorTone = dd.color;
            }
            else {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = originalColorTone;
            }
            repaintForeground();
            dd.oldValueB[2] = value2;
            dd.oldValueB[0] = value0;
          }
        }
        else if ( dd.attrFound[2] && dd.attrFound[1]) {
          value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value1 != dd.oldValueB[1] || value2 != dd.oldValueB[2] || 
		value0 != dd.oldValueB[0] || dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = GeColor.COLOR_TONE_RED;
            }
            else if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = dd.color2;
              else
                colorTone = dd.color2;
            }
            else if ( (!dd.inverted[0] && !value0) || (dd.inverted[0] && value0)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = dd.color;
              else
                colorTone = dd.color;
            }
            else {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = originalColorTone;
            }
            repaintForeground();
            dd.oldValueB[2] = value2;
            dd.oldValueB[1] = value1;
            dd.oldValueB[0] = value0;
          }
        }
        else {
          if ( dd.oldValueB[0] != value0 || dd.firstScan) {
            if ( (!dd.inverted[0] && !value0) || (dd.inverted[0] && value0)) {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = dd.color;
              else
                colorTone = dd.color;
            }
            else {
              if ( dd.color > GeColor.COLOR_TONE_MAX)
                fillColor = originalFillColor;
              colorTone = originalColorTone;
            }
            repaintForeground();
            dd.oldValueB[0] = value0;
          }
        }
        if ( dd.firstScan)
          dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_AnnotWithTone:
      case GeDyndata.eTrace_AnnotInputWithTone:
      {
        if ( !dd.attrFound[0])
          return;

        boolean repaintNow = false;
        if ( dd.typeId[0] == Pwr.eType_Float32) {
          float value0 = en.gdh.getObjectRefInfoFloat( dd.p[0]);
          if ( value0 != dd.oldValueF[0]  || dd.firstScan) {
            sb = dd.format( value0, sb);
	    setAnnot1(new String(sb));
            repaintNow = true;
            dd.oldValueF[0] = value0;
          }
        }
        else if ( dd.typeId[0] == Pwr.eType_Int32 ||
		  dd.typeId[0] == Pwr.eType_UInt32 ||
                  dd.typeId[0] == Pwr.eType_Int16 ||
		  dd.typeId[0] == Pwr.eType_UInt16 ||
                  dd.typeId[0] == Pwr.eType_Int8 ||
		  dd.typeId[0] == Pwr.eType_UInt8) {
          int value0 = en.gdh.getObjectRefInfoInt( dd.p[0]);
          if ( value0 != dd.oldValueI || dd.firstScan) {
            sb = dd.format( value0, sb);
	    setAnnot1(new String(sb));
            repaintNow = true;
            dd.oldValueI = value0;
          }
        }
        else if ( dd.typeId[0] == Pwr.eType_String || 
                  dd.typeId[0] == Pwr.eType_Objid) {
          String value0 = en.gdh.getObjectRefInfoString( dd.p[0], dd.typeId[0]);
          if ( dd.firstScan || value0.compareTo( dd.oldValueS) != 0) {
            sb = dd.format( value0, sb);
	    setAnnot1(new String(sb));
            repaintNow = true;
	    dd.oldValueS = value0;
          }
        }
        if ( dd.attrFound[1]) {
          boolean value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value1 != dd.oldValueB[1] || dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1)) {
              colorTone = originalColorTone;
	      fillColor = originalFillColor;
            }
	    else {
              if ( dd.color <= GeColor.COLOR_TONE_MAX)
                colorTone = dd.color;
              else
                fillColor = dd.color;
            }
	    repaintNow = true;
	    dd.oldValueB[1] = value1;
          }
        }
        if ( dd.attrFound[2]) {
          boolean value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || dd.firstScan) {
            if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2)) {
              dd.invisible = true;
            }
	    else {
              dd.invisible = false;
            }
	    repaintNow = true;
	    dd.oldValueB[2] = value2;
          }
        }
        if ( repaintNow)
	  repaintForeground();
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_Annot:
      case GeDyndata.eTrace_AnnotInput:
      {
        if ( !dd.attrFound[0])
          return;

        boolean repaintNow = false;
        if ( dd.typeId[0] == Pwr.eType_Float32) {
          float value0 = en.gdh.getObjectRefInfoFloat( dd.p[0]);
          if ( value0 != dd.oldValueF[0] || dd.firstScan) {
            sb = dd.format( value0, sb);
            setAnnot1(new String(sb));
            repaintNow = true;
            dd.oldValueF[0] = value0;
          }
        }
        else if ( dd.typeId[0] == Pwr.eType_Int32 ||
		  dd.typeId[0] == Pwr.eType_UInt32 ||
                  dd.typeId[0] == Pwr.eType_Int16 ||
		  dd.typeId[0] == Pwr.eType_UInt16 ||
                  dd.typeId[0] == Pwr.eType_Int8 ||
		  dd.typeId[0] == Pwr.eType_UInt8) {
          int value0 = en.gdh.getObjectRefInfoInt( dd.p[0]);
          if ( value0 != dd.oldValueI || dd.firstScan) {
            sb = dd.format( value0, sb);
            setAnnot1(new String(sb));
            repaintNow = true;
            dd.oldValueI = value0;
          }
        }
        else if ( dd.typeId[0] == Pwr.eType_String || 
                  dd.typeId[0] == Pwr.eType_Objid ||
                  dd.typeId[0] == Pwr.eType_DeltaTime ||
                  dd.typeId[0] == Pwr.eType_Time) {
          String value0 = en.gdh.getObjectRefInfoString( dd.p[0], dd.typeId[0]);
          if ( dd.firstScan || value0.compareTo( dd.oldValueS) != 0) {
            sb = dd.format( value0, sb);
	    setAnnot1(new String(sb));
            repaintNow = true;
	    dd.oldValueS = value0;
          }
        }
        if ( dd.attrFound[1]) {
          boolean value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value1 != dd.oldValueB[1] || dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1))
	      fillColor = originalFillColor;
	    else
              fillColor = dd.color;
	    repaintNow = true;
	    dd.oldValueB[1] = value1;
          }
        }
        if ( repaintNow)
	  repaintForeground();
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }
      case GeDyndata.eTrace_Move:
      {
        boolean repaintNow = false;

        if ( dd.firstScan) {
          Point loc = getLocation();
          dd.xOrig = (double) loc.x;
          dd.yOrig = (double) loc.y;
        }

        if ( dd.attrFound[0] || dd.attrFound[1]) {
	  // Move
          Point loc = getLocation();
          float value0 = 0;
          float value1 = 0;

          if ( dd.attrFound[0]) {
            value0 = en.gdh.getObjectRefInfoFloat( dd.p[0]);
            if ( value0 != dd.oldValueF[0]  || dd.firstScan) {
              repaintNow = true;
              dd.oldValueF[0] = value0;
            }
          }
          if ( dd.attrFound[1]) {
            value1 = en.gdh.getObjectRefInfoFloat( dd.p[1]);
            if ( value1 != dd.oldValueF[1]  || dd.firstScan) {
              repaintNow = true;
	      dd.oldValueF[1] = value1;
	    }
          }
          if ( repaintNow) {
            if ( dd.attrFound[0])
	      loc.x = (int) (dd.xOrig + (value0 - dd.x0) * dd.factor);
            if ( dd.attrFound[1])
	      loc.y = (int) (dd.yOrig + (value1 - dd.y0) * dd.factor);
            setLocation( loc);

          }
        }
        if ( dd.attrFound[4] || dd.attrFound[5]) {
	  // Scale
          if ( dd.firstScan) {
            dd.xScale = getWidth();
            dd.yScale = getHeight();
          }
          float value4 = 0;
          float value5 = 0;

          if ( dd.attrFound[4]) {
            value4 = en.gdh.getObjectRefInfoFloat( dd.p[4]);
            if ( value4 != dd.oldValueF[2]  || dd.firstScan) {
              repaintNow = true;
              dd.oldValueF[2] = value4;
            }
          }
          if ( dd.attrFound[5]) {
            value5 = en.gdh.getObjectRefInfoFloat( dd.p[5]);
            if ( value5 != dd.oldValueF[3]  || dd.firstScan) {
              repaintNow = true;
	      dd.oldValueF[3] = value5;
	    }
          }
          if ( repaintNow) {
            Rectangle rect = getBounds();
            if ( dd.attrFound[4]) {
	      rect.width = (int) (dd.xScale * value4);
              if ( rect.width < 1)
                rect.width = 1;
            }
            if ( dd.attrFound[5]) {
	      rect.height = (int) (dd.yScale * value5);
              if ( rect.height < 1)
                rect.height = 1;
            }
            setBounds( rect);
          }
        }
        if ( dd.attrFound[2]) {
          boolean value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || dd.firstScan) {
            if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2)) {
              colorTone = originalColorTone;
	      fillColor = originalFillColor;
            }
	    else {
              if ( dd.color <= GeColor.COLOR_TONE_MAX)
                colorTone = dd.color;
              else
                fillColor = dd.color;
            }
	    repaintNow = true;
	    dd.oldValueB[2] = value2;
          }
        }
        if ( dd.attrFound[3]) {
          boolean value3 = en.gdh.getObjectRefInfoBoolean( dd.p[3]);
          if ( value3 != dd.oldValueB[3] || dd.firstScan) {
            if ( (!dd.inverted[3] && value3) || (dd.inverted[3] && !value3)) {
              dd.invisible = true;
            }
	    else {
              dd.invisible = false;
            }
	    repaintNow = true;
	    dd.oldValueB[3] = value3;
          }
        }
        if ( repaintNow)
	  repaintForeground();
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_Rotate:
      {
        boolean repaintNow = false;

        if ( dd.attrFound[0]) {
	  // Rotate
	  float value0 = 0;
          if ( dd.attrFound[0]) {
            value0 = en.gdh.getObjectRefInfoFloat( dd.p[0]);
            if ( value0 != dd.oldValueF[0]  || dd.firstScan) {
              repaintNow = true;
              dd.oldValueF[0] = value0;
            }
          }
          if ( repaintNow) {
	    dd.setRotate( value0 * dd.factor);
          }
        }
        if ( dd.attrFound[3]) {
          boolean value3 = en.gdh.getObjectRefInfoBoolean( dd.p[3]);
          if ( value3 != dd.oldValueB[3] || dd.firstScan) {
            if ( (!dd.inverted[3] && value3) || (dd.inverted[3] && !value3)) {
              colorTone = originalColorTone;
	      fillColor = originalFillColor;
            }
	    else {
              if ( dd.color <= GeColor.COLOR_TONE_MAX)
                colorTone = dd.color;
              else
                fillColor = dd.color;
            }
	    repaintNow = true;
	    dd.oldValueB[3] = value3;
          }
        }
        if ( dd.attrFound[4]) {
          boolean value4 = en.gdh.getObjectRefInfoBoolean( dd.p[4]);
          if ( value4 != dd.oldValueB[4] || dd.firstScan) {
            if ( (!dd.inverted[4] && value4) || (dd.inverted[4] && !value4)) {
              dd.invisible = true;
            }
	    else {
              dd.invisible = false;
            }
	    repaintNow = true;
	    dd.oldValueB[4] = value4;
          }
        }
        if ( repaintNow) {
	  // repaintForeground();
          repaint();
        }
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_AnalogShift:
      {
        if ( !dd.attrFound[0])
          return;

        boolean repaintNow = false;
        float value0 = en.gdh.getObjectRefInfoFloat( dd.p[0]);
        if ( value0 != dd.oldValueF[0]  || dd.firstScan) {
          setPage( (int)value0 + 1);
          repaintNow = true;
          dd.oldValueF[0] = value0;
        }
        if ( dd.attrFound[1]) {
          boolean value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value1 != dd.oldValueB[1] || dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1)) {
              colorTone = originalColorTone;
	      fillColor = originalFillColor;
            }
	    else {
              if ( dd.color <= GeColor.COLOR_TONE_MAX)
                colorTone = dd.color;
              else
                fillColor = dd.color;
            }
	    repaintNow = true;
	    dd.oldValueB[1] = value1;
          }
        }
        if ( dd.attrFound[2]) {
          boolean value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || dd.firstScan) {
            if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2)) {
              dd.invisible = true;
            }
	    else {
              dd.invisible = false;
            }
	    repaintNow = true;
	    dd.oldValueB[2] = value2;
          }
        }
        if ( repaintNow)
	  repaintForeground();         
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_Animation:
      case GeDyndata.eTrace_AnimationForwBack:
      {
        if ( !dd.attrFound[0])
          return;

        boolean repaintNow = false;
        int sts;
        int maxCount;
        boolean value0 = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
        if ( dd.firstScan) {
          dd.animationCount = 0;
          dd.animationDirection = 1;
        }
        if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0)) {
          if ( dd.animationDirection == 0) {
            dd.animationCount = 0;
            dd.animationDirection = 1;
          }
          maxCount = animationCount;
          dd.animationCount++;
          if ( dd.animationCount >= maxCount) {
            // Shift nodeclass
            if ( dd.animationDirection == 1) {
              // Shift forward
              sts = setNextPage();
              if ( sts == 0) {
                if ( dd.type == GeDyndata.eTrace_Animation)
                  // Start from beginning the again
                  setPage( 2);
                else {
                  // Change direction
                  dd.animationDirection = 2;
                  setPreviousPage();
                }
              }
              dd.animationCount = 0;
              repaintNow = true;
            }
            else {
              // Shift backwards
              sts = setPreviousPage();
              if ( sts == 0) {
                // Change direction
                dd.animationDirection = 1;
                setNextPage();
              }
              dd.animationCount = 0;
              repaintNow = true;
            }
          }
        }
        else {
          if ( dd.animationDirection != 0) {
            dd.animationDirection = 0;
            setFirstPage();
            repaintNow = true;
          }
        }
        dd.oldValueB[0] = value0;
       
        if ( dd.attrFound[1]) {
          boolean value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value1 != dd.oldValueB[1] || dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1)) {
              colorTone = originalColorTone;
	      fillColor = originalFillColor;
            }
	    else {
              if ( dd.color <= GeColor.COLOR_TONE_MAX)
                colorTone = dd.color;
              else
                fillColor = dd.color;
            }
	    repaintNow = true;
	    dd.oldValueB[1] = value1;
          }
        }
        if ( dd.attrFound[2]) {
          boolean value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || dd.firstScan) {
            if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2)) {
              dd.invisible = true;
            }
	    else {
              dd.invisible = false;
            }
	    repaintNow = true;
	    dd.oldValueB[2] = value2;
          }
        }
        if ( repaintNow)
          repaint();
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_DigAnimation:
      {
        if ( !dd.attrFound[0])
          return;

        boolean repaintNow = false;
        int sts;
        int maxCount;
        boolean value0 = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
        if ( dd.firstScan) {
          dd.animationCount = 0;
          dd.animationDirection = 0;
          if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0)) {
            setLastPage();
            repaintNow = true;
          }
          dd.oldValueB[0] = value0;
        }
        if ( dd.oldValueB[0] != value0) {
          // Start animation
          if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0)) {
            dd.animationCount = 0;
            dd.animationDirection = 1;
          }
          else if ( (!dd.inverted[0] && !value0) || (dd.inverted[0] && value0)) {
            dd.animationCount = 0;
            dd.animationDirection = 2;
          }
        }

        if ( dd.animationDirection != 0) {
          maxCount = animationCount;
          dd.animationCount++;
          if ( dd.animationCount >= maxCount) {
            // Shift nodeclass
            if ( dd.animationDirection == 1) {
              // Shift forward
              sts = setNextPage();
              if ( sts == 0) {
                // End of animation
                dd.animationCount = 0;
                dd.animationDirection = 0;
              }
              dd.animationCount = 0;
              repaintNow = true;
            }
            else {
              // Shift backwards
              sts = setPreviousPage();
              if ( sts == 0) {
                // End of animation
                dd.animationCount = 0;
                dd.animationDirection = 0;
              }
              dd.animationCount = 0;
              repaintNow = true;
            }
          }
        }
        if ( repaintNow) {
          repaintNow = false;
          repaint();
        }
        dd.oldValueB[0] = value0;
       
        if ( dd.attrFound[1]) {
          boolean value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value1 != dd.oldValueB[1] || dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1)) {
              colorTone = originalColorTone;
	      fillColor = originalFillColor;
            }
	    else {
              if ( dd.color <= GeColor.COLOR_TONE_MAX)
                colorTone = dd.color;
              else
                fillColor = dd.color;
            }
	    repaintNow = true;
	    dd.oldValueB[1] = value1;
          }
        }
        if ( dd.attrFound[2]) {
          boolean value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || dd.firstScan) {
            if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2)) {
              dd.invisible = true;
            }
	    else {
              dd.invisible = false;
            }
	    repaintNow = true;
	    dd.oldValueB[2] = value2;
          }
        }
        if ( repaintNow)
	  repaintForeground();
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }	
      case GeDyndata.eTrace_DigShift:
      case GeDyndata.eTrace_DigShiftWithToggleDig:
      case GeDyndata.eTrace_RadioButton:
      {
        if ( !dd.attrFound[0])
          return;

        boolean repaintNow = false;
        int sts;
        int maxCount;
        boolean value0 = en.gdh.getObjectRefInfoBoolean( dd.p[0]);
        if ( dd.firstScan) {
          dd.animationDirection = 0;
          if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0)) {
            setLastPage();
            repaintNow = true;
          }
          dd.oldValueB[0] = value0;
        }
        if ( dd.oldValueB[0] != value0) {
          // Shift to first or last page
          if ( (!dd.inverted[0] && value0) || (dd.inverted[0] && !value0))
            setLastPage();
          else if ( (!dd.inverted[0] && !value0) || (dd.inverted[0] && value0))
            setFirstPage();
          repaintNow = true;
        }
        if ( repaintNow) {
          repaintNow = false;
          repaint();
        }
        dd.oldValueB[0] = value0;
       
        if ( dd.attrFound[1]) {
          boolean value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
          if ( value1 != dd.oldValueB[1] || dd.firstScan) {
            if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1)) {
              colorTone = originalColorTone;
	      fillColor = originalFillColor;
            }
	    else {
              if ( dd.color <= GeColor.COLOR_TONE_MAX)
                colorTone = dd.color;
              else
                fillColor = dd.color;
            }
	    repaintNow = true;
	    dd.oldValueB[1] = value1;
          }
        }
        if ( dd.attrFound[2]) {
          boolean value2 = en.gdh.getObjectRefInfoBoolean( dd.p[2]);
          if ( value2 != dd.oldValueB[2] || dd.firstScan) {
            if ( (!dd.inverted[2] && value2) || (dd.inverted[2] && !value2)) {
              dd.invisible = true;
            }
	    else {
              dd.invisible = false;
            }
	    repaintNow = true;
	    dd.oldValueB[2] = value2;
          }
        }
        if ( repaintNow)
	  repaintForeground();
        if ( dd.firstScan)
	  dd.firstScan = false;
        break;
      }	
    }
  }
  private void repaintForeground() {
    Graphics g = getGraphics();
    if ( g == null) {
      System.out.println("repaintForeground: can't get Graphic object");
      return;
    }
    //paintComponent(getGraphics());
    //    repaint();
    if ( level > 4)
      getParent().repaint();
    else {
      paintComponent(g);
      paintChildren(g);
    }
  }

  public void paintChildren(Graphics g1) {
    Graphics2D g = (Graphics2D) g1;
    Component c;
    Point p;
    //    super.paint(super.getGraphics());
    //    paintComponent(g);
/*
    for ( int i = 0; i < getComponentCount(); i++) {
      AffineTransform save = g.getTransform();
      c = getComponent(i);
      if ( c instanceof GeComponent) {
        if ( ((GeComponent)c).dd.type == 0) {
          ((GeComponent)c).colorTone = colorTone;
          ((GeComponent)c).colorBrightness = colorBrightness;
          ((GeComponent)c).colorIntensity = colorIntensity;
          ((GeComponent)c).colorShift = colorShift;
          ((GeComponent)c).colorInverse = colorInverse;
        }
      }
      else if ( c instanceof GeImage) {
        ((GeImage)c).colorTone = colorTone;
        ((GeImage)c).colorBrightness = colorBrightness;
        ((GeImage)c).colorIntensity = colorIntensity;
        ((GeImage)c).colorShift = colorShift;
        ((GeImage)c).colorInverse = colorInverse;
      }
      p = c.getLocation();
      g.translate((int)p.getX(), (int)p.getY());
      c.paint(g);
      g.setTransform(save);
    }
*/
  }

}




