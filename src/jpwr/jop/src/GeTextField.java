
package jpwr.jop;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import javax.swing.Timer;

public class GeTextField extends JTextField implements JopDynamic,
	ActionListener
{
  Dimension size;
  Object root;
  Timer timer = new Timer(500, this);
  JopSession session;
  public JopEngine en;
  public GeDyndata dd = new GeDyndata();

  String undoKey = new String( "Ej i bruk." );
  public float minValue = 0;
  public float maxValue = 0;
  float alarmLimitLow = 0;
  float alarmLimitHigh = 100000;
  public boolean focus = false;
  public Color  normalColor = null;
  public Color  alarmColor = new Color( 255, 0, 0 );
  public GeTextField component = this;
  public GeTextField()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception
  {
    size = new Dimension( 102, 36);
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

      if ( !en.isReady())
        return;
      en.add(this);
      engine_found = true;
    }

    if ( engine_found) {
      timer.stop();
      timer = null;

      if ( en.gdh.isAuthorized( dd.access)) {
        this.addKeyListener(new java.awt.event.KeyAdapter() {
          public void keyPressed(KeyEvent e) {
            if ( e.getKeyCode() == e.VK_ESCAPE ) {
              keyPressedEvent(e);
            }
          }
        });
        this.addActionListener(new java.awt.event.ActionListener() {
          public void actionPerformed(ActionEvent e) {
            newValueEvent(e);
          }
        });
      }
      this.addFocusListener(new java.awt.event.FocusAdapter() {
        public void focusGained(FocusEvent e) {
	  if ( en.gdh.isAuthorized( dd.access)) {
            focus = true;
            selectAll();
          }
          else
            component.getParent().requestFocus();
        }
        public void focusLost(FocusEvent e) {
          focus = false;
          dd.oldValueF[0] = -10000;
          dd.oldValueI = -10000;
          dd.oldValueS = null;
        }
      });
    }
  }

  Font annot1Font = new Font("Helvetica", Font.BOLD, 14);
  public void setAnnot1Font( Font font) { annot1Font = font; setFont(font);}
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
  public GdhrRefObjectInfo ret = null;
  public StringBuffer sb = new StringBuffer();
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
  public double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}

  public Object dynamicGetRoot() {
    return root;
  }
  public void dynamicOpen() {
    if ( en.isInstance())
      dd.setInstance( en.getInstance());


    String attrName; 
    if (dd.data[0] != null) {
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
    }
    if (dd.data[3] != null && dd.data[4] != null) {
      try {
        minValue = Float.parseFloat( dd.data[3]);
        maxValue = Float.parseFloat( dd.data[4]);
      }
      catch(NumberFormatException ex) {
        minValue = 0;
        maxValue = 0;
      }
    }
    else {
      minValue = 0;
      maxValue = 0;
    }
    if ( dd.data[2] != null) {
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
    }
    normalColor = GeColor.getColor( fillColor, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor);
    setBackground( normalColor);
  }
  public void dynamicClose() {
    if ( dd.attrFound[0])
      en.gdh.unrefObjectInfo( dd.subid[0]);
    if ( dd.attrFound[1])
      en.gdh.unrefObjectInfo( dd.subid[1]);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;
    if ( !dd.attrFound[0] || focus)
      return;

    boolean repaintNow = false;
    if ( dd.typeId[0] == Pwr.eType_Float32) {
      float value0 = en.gdh.getObjectRefInfoFloat( dd.p[0]);
      if ( value0 != dd.oldValueF[0]) {
        sb = dd.format( value0, sb);
        setText(new String(sb));
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
      if ( value0 != dd.oldValueI) {
        sb = dd.format( value0, sb);
        setText(new String(sb));
        repaintNow = true;
        dd.oldValueI = value0;
      }
    }
    else if ( dd.typeId[0] == Pwr.eType_String) {
      String value0 = en.gdh.getObjectRefInfoString( dd.p[0], dd.typeId[0]);

      if ( dd.oldValueS == null || value0.compareTo( dd.oldValueS) != 0) {
        sb = dd.format( value0, sb);
        setText(new String(sb));
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
        normalColor = GeColor.getColor( fillColor, colorTone, colorShift, 
	      colorIntensity, colorBrightness, colorInverse, GeColor.NO_COLOR);
        setBackground( normalColor);

      }
    }
    if ( repaintNow)
//      repaintForeground();
    if ( dd.firstScan)
      dd.firstScan = false;

//    if ( valueAttr < alarmLimitLow || valueAttr > alarmLimitHigh)
//      setBackground( alarmColor);
//    else
//      setBackground( normalColor);
  }

  void newValueEvent(ActionEvent e) {
    String text = this.getText();
    PwrtStatus sts;

    try {
      if ( dd.typeId[0] == Pwr.eType_Float32) {
        float inputValue = Float.parseFloat( text );
        dd.oldValueF[0] = inputValue;
        if ( minValue == 0 && maxValue == 0) {
          String attrName = dd.getAttrNameNoSuffix( dd.data[0]);
          sts = en.gdh.setObjectInfo( attrName, inputValue);
          if ( sts.evenSts())
            System.out.println( "setObjectInfoError " + sts);
        }
        else {
          if ( inputValue >= minValue && inputValue <= maxValue ) {
            String attrName = dd.getAttrNameNoSuffix( dd.data[0]);        
            sts = en.gdh.setObjectInfo( attrName, inputValue);
            if ( sts.evenSts())
              System.out.println( "setObjectInfoError " + attrName + " " + sts);
          }
          else
            dd.oldValueF[0] = -10000;
        }
      }
      else if ( dd.typeId[0] == Pwr.eType_Int32 ||
		dd.typeId[0] == Pwr.eType_UInt32 ||
                dd.typeId[0] == Pwr.eType_Int16 ||
		dd.typeId[0] == Pwr.eType_UInt16 ||
                dd.typeId[0] == Pwr.eType_Int8 ||
		dd.typeId[0] == Pwr.eType_UInt8) {
        int inputValue = Integer.parseInt( text, 10);
        dd.oldValueI = inputValue;
        if ( minValue == 0 && maxValue == 0) {
          String attrName = dd.getAttrNameNoSuffix( dd.data[0]);        
          sts = en.gdh.setObjectInfo( attrName, inputValue);
          if ( sts.evenSts())
            System.out.println( "setObjectInfoError " + sts);
        }
        else {
          if ( inputValue >= minValue && inputValue <= maxValue ) {
            String attrName = dd.getAttrNameNoSuffix( dd.data[0]);        
            sts = en.gdh.setObjectInfo( attrName, inputValue);
            if ( sts.evenSts())
              System.out.println( "setObjectInfoError " + sts);
          }
          else
            dd.oldValueI = -10000;
        }
      }
      else if ( dd.typeId[0] == Pwr.eType_String) {
        dd.oldValueS = text;
        String attrName = dd.getAttrNameNoSuffix( dd.data[0]);        
        sts = en.gdh.setObjectInfo( attrName, text);
        if ( sts.evenSts())
          System.out.println( "setObjectInfoError " + sts);
      }


    }
    catch(NumberFormatException ex) {
      System.out.println( ex.toString() );
    }

    this.getParent().requestFocus();
  }

  void keyPressedEvent(KeyEvent e) {
    if ( e.getKeyCode() == e.VK_ESCAPE )
    {
      this.getParent().requestFocus();
    }
  }

  public String getUndoKey() {
    return undoKey;
  }

  public void setUndoKey(String text) {
    undoKey = text;
  }

  public float getMaxValue() {
    return maxValue;
  }

  public void setMaxValue(float maxValue) {
    this.maxValue = maxValue;
  }

  public float getMinValue() {
    return minValue;
  }

  public void setMinValue(float minValue) {
    this.minValue = minValue;
  }

  public float getAlarmLimitHigh() {
    return alarmLimitHigh;
  }

  public void setAlarmLimitHigh(float alarmLimitHigh) {
    this.alarmLimitHigh = alarmLimitHigh;
  }

  public float getAlarmLimitLow() {
    return alarmLimitLow;
  }

  public void setAlarmLimitLow(float alarmLimitLow)  {
    this.alarmLimitLow = alarmLimitLow;
  }

  public Color getAlarmColor() {
    return alarmColor;
  }

  public void setAlarmColor(Color alarmColor) {
    this.alarmColor = alarmColor;
  }

}
