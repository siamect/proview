package jpwr.jop;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import javax.swing.Timer;
import java.awt.event.*;
import jpwr.rt.*;

public class JopTrend extends JComponent implements GeComponentIfc, 
	  JopDynamic, JopConfirm, ActionListener{
  Dimension size;
  Timer timer = new Timer(500, this);
  Object root;
  JopSession session;
  JopEngine en;
  public GeDyn dd = new GeDyn( this);
  boolean hold = false;

  public JopTrend( JopSession session)
  {
   this.session = session;
    size = new Dimension( 102, 36);
    timer.start();
    dd.setSession( session);

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
      if ( dd.actionType != 0 && en.gdh.isAuthorized( dd.access)) {
        this.addMouseListener(new MouseAdapter() {
          public void mouseReleased(MouseEvent e) {
	    if ( e.isPopupTrigger())
	      dd.action( GeDyn.eEvent_MB3Press, e);
	    else if ((e.getModifiers() & MouseEvent.BUTTON1_MASK) != 0 &&
		     en.gdh.isAuthorized( dd.access))
	      dd.action( GeDyn.eEvent_MB1Up, e);
	  }

          public void mousePressed(MouseEvent e) {
	    if ( e.isPopupTrigger())
	      dd.action( GeDyn.eEvent_MB3Press, e);
	    else if ((e.getModifiers() & MouseEvent.BUTTON1_MASK) != 0 &&
		     en.gdh.isAuthorized( dd.access))
	      dd.action( GeDyn.eEvent_MB1Down, e);
	  }
          public void mouseClicked(MouseEvent e) {
	    if ((e.getModifiers() & MouseEvent.BUTTON1_MASK) != 0 &&
		en.gdh.isAuthorized( dd.access))
	      dd.action( GeDyn.eEvent_MB1Click, e);
	  }
        });
      }
    }
  }
  public void confirmNo() {}
  public void confirmYes() {
    PwrtStatus sts;
    String attrName;

    dd.confirmedAction( GeDyn.eEvent_MB1Click, null);
  }

  public void setHold( boolean hold) {
    this.hold = hold;
  }

  // GeComponents Ifc
  public void tsetFillColor( int fillColor) {}
  public void tsetColorTone( int colorTone) {}
  public void tsetBorderColor( int borderColor) {}
  public void tsetTextColor( int textColor) {}
  public void setColorInverse( int colorInverse) {} 
  public void resetFillColor() {}
  public void resetColorTone() {}
  public void resetBorderColor() {}
  public void resetTextColor() {}
  public String getAnnot1() { return new String();}
  public void setAnnot1( String s) {}
  public void setLastPage() {}
  public void setFirstPage() {}
  public void setPage( int page) {}
  public int setNextPage() { return 1;}
  public int setPreviousPage() { return 1;}
  public Object getDd() { return dd;}
  public void setFillLevel( float fillLevel) {}
  public void setLevelDirection( int levelDirection) {}
  public void setLevelColorTone( int levelColorTone) {}
  public void setLevelFillColor( int levelFillColor) {}

  int fillColor = 9999;
  int borderColor = 9999;
  int[] fillColorTrend = {9999, 9999};
  int[] borderColorTrend = {9999, 9999};
  int drawFill = 0;
  int drawBorder = 0;
  int lineWidth = 1;
  int trendBorderWidth = 1;
  int horizontalLines = 0;
  int verticalLines = 0;
  float scanTime = 1F;
  int noOfPoints = 100;
  public void setFillColor( int fillColor) {
    this.fillColor = fillColor;
  }
  public int getFillColor() {
    return fillColor;
  }
  public void setBorderColor( int borderColor) {
    this.borderColor = borderColor;
  }
  public int getBorderColor() {
    return borderColor;
  }
  public void setFillColorTrend1( int fillColorTrend) {
    this.fillColorTrend[0] = fillColorTrend;
  }
  public int getFillColorTrend1() {
    return fillColorTrend[0];
  }
  public void setBorderColorTrend1( int borderColorTrend) {
    this.borderColorTrend[0] = borderColorTrend;
  }
  public int getBorderColorTrend1() {
    return borderColorTrend[0];
  }
  public void setFillColorTrend2( int fillColorTrend) {
    this.fillColorTrend[1] = fillColorTrend;
  }
  public int getFillColorTrend2() {
    return fillColorTrend[1];
  }
  public void setBorderColorTrend2( int borderColorTrend) {
    this.borderColorTrend[1] = borderColorTrend;
  }
  public int getBorderColorTrend2() {
    return borderColorTrend[1];
  }
  public void setDrawFill( int drawFill) {
    this.drawFill = drawFill;
  }
  public int getDrawFill() {
    return drawFill;
  }
  public void setDrawBorder( int drawBorder) {
    this.drawBorder = drawBorder;
  }
  public int getDrawBorder() {
    return drawBorder;
  }
  public void setLineWidth( int lineWidth) {
    this.lineWidth = lineWidth;
  }
  public int getLineWidth() {
    return lineWidth;
  }
  public void setTrendBorderWidth( int trendBorderWidth) {
    this.trendBorderWidth = trendBorderWidth;
  }
  public int getTrendBorderWidth() {
    return trendBorderWidth;
  }
  public void setVerticalLines( int verticalLines) {
    this.verticalLines = verticalLines;
  }
  public int getVerticalLines() {
    return verticalLines;
  }
  public void setHorizontalLines( int horizontalLines) {
    this.horizontalLines = horizontalLines;
  }
  public int getHorizontalLines() {
    return horizontalLines;
  }
  public void setScanTime( float scanTime) {
    this.scanTime = scanTime;
  }
  public float getScanTime() {
    return scanTime;
  }
  public void setNoOfPoints( int noOfPoints) {
    this.noOfPoints = noOfPoints;
  }
  public int getNoOfPoints() {
    return noOfPoints;
  }
  public double rotate;
  public void setRotate( double rotate) { this.rotate = rotate;}
  public double getRotate() { return rotate;}
  Shape[] shapes = null;
  GeneralPath[] curve = new GeneralPath[2];
  Line2D.Float[] vLines;
  Line2D.Float[] hLines;
  float[][] y_values = new float[2][];
  float[][] x_values = new float[2][];
  public void paint(Graphics g1) {
    super.paint(g1);
    Graphics2D g = (Graphics2D) g1;
    Component c;
    Point p;
    paintComponent(g);
    for ( int i = 0; i < getComponentCount(); i++) {
      AffineTransform save = g.getTransform();
      c = getComponent(i);
      p = c.getLocation();
      g.translate((int)p.getX(), (int)p.getY());
      c.paint(g);
      g.setTransform(save);
    }
  }
  float original_width;
  float original_height;
  public void paintComponent(Graphics g1) {
    int i, j;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    AffineTransform save = g.getTransform();
    float delta;

    if ( shapes == null) {
      original_width = width;
      original_height = height;
      shapes = new Shape[1];
      shapes[0] = new Rectangle2D.Float(0F, 0F, width, height);
      if ( verticalLines > 0)
      {
        vLines = new Line2D.Float[verticalLines];
        delta = width / ( verticalLines + 1);
	for ( i = 0; i < verticalLines; i++)
	  vLines[i] = new Line2D.Float( delta * (i+1), 0F, delta * (i+1), height);
      }
      if ( horizontalLines > 0)
      {
        hLines = new Line2D.Float[horizontalLines];
        delta = height / ( horizontalLines + 1);
	for ( i = 0; i < horizontalLines; i++)
	  hLines[i] = new Line2D.Float( 0F, delta * (i+1), width, delta * (i+1));
      }
      for ( j = 0; j < 2; j++) {
        y_values[j] = new float[noOfPoints];
        x_values[j] = new float[noOfPoints];

        float x = width;
        float y = height;
        delta = width/(noOfPoints - 1);
        for ( i = 0; i < noOfPoints; i++)
        {
          y_values[j][i] = y;
          x_values[j][i] = x;
          x -= delta;
        }
        curve[j] = new GeneralPath(GeneralPath.WIND_EVEN_ODD);
      }
    }
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);

    /*
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( -height, 0.0); 
      g.rotate( - Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/width, width/height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( - Math.PI * rotate/180, width/2, height/2);
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( width, 0.0); 
      g.rotate( - Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/width, width/height));      
    }
    */

    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else {
      g.transform( AffineTransform.getScaleInstance( width/original_width,
						   height/original_height));
    }

    if ( drawFill == 1) {
      g.setColor(GeColor.getColor(22, fillColor));
      g.fill( shapes[0]);
    }
    g.setStroke( new BasicStroke((float)trendBorderWidth));
    for ( j = 0; j < 2; j++) {
      if ( attrFound[j]) {
        curve[j].reset();
        curve[j].moveTo(original_width,original_height);
        for ( i = 0; i < noOfPoints; i++)
          curve[j].lineTo( x_values[j][i], y_values[j][i]); 
        curve[j].lineTo( 0,original_height);

        if ( drawFill == 1) {
          g.setColor(GeColor.getColor(0, fillColorTrend[j]));
          g.fill( curve[j]);
        }
      }
    }
    g.setStroke( new BasicStroke((float)lineWidth));
    g.setColor(GeColor.getColor(0, borderColor));
    for ( i = 0; i < verticalLines; i++)
      g.draw( vLines[i]);
    for ( i = 0; i < horizontalLines; i++)
      g.draw( hLines[i]);
    for ( j = 0; j < 2; j++) {
      if ( attrFound[j]) {
        g.setColor(GeColor.getColor(0, borderColorTrend[j]));
        g.draw( curve[j]);
      }
    }
    g.setStroke( new BasicStroke((float)lineWidth));
    if ( drawBorder == 1 || drawFill == 0) {
      g.setColor(GeColor.getColor(0, borderColor));
      g.draw( shapes[0]);
    }
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
  String[] pwrAttribute = new String[2];
  public void setPwrAttribute1( String pwrAttribute) { this.pwrAttribute[0] = pwrAttribute;}
  public String getPwrAttribute1() { return pwrAttribute[0];}
  public void setPwrAttribute2( String pwrAttribute) { this.pwrAttribute[1] = pwrAttribute;}
  public String getPwrAttribute2() { return pwrAttribute[1];}
  float[] valueOld = new float[2];
  boolean firstScan = true;
  GdhrRefObjectInfo[] retColor = {null, null};
  boolean[] attrFound = {false, false};
  float[] minValue = {0,0};
  float[] maxValue = {100, 100};
  public void setMinValue1( float minValue1) {
    this.minValue[0] = minValue1;
  }
  public float getMinValue1() {
    return minValue[0];
  }
  public void setMaxValue1( float maxValue1) {
    this.maxValue[0] = maxValue1;
  }
  public float getMaxValue1() {
    return maxValue[0];
  }
  public void setMinValue2( float minValue2) {
    this.minValue[1] = minValue2;
  }
  public float getMinValue2() {
    return minValue[1];
  }
  public void setMaxValue2( float maxValue2) {
    this.maxValue[1] = maxValue2;
  }
  public float getMaxValue2() {
    return maxValue[1];
  }
  public Object dynamicGetRoot() {
    return root;
  }
  public void reset() {
    for ( int j = 0; j < 2; j++) {
      for ( int i = 0; i < noOfPoints; i++)
        y_values[j][i] = original_height;
    }
    repaint();
  }
  public void dynamicOpen() {
    if ( en.isInstance())
      dd.setInstance( en.getInstance());
    for ( int j = 0; j < 2; j++) {
      if ( pwrAttribute[j] != null && pwrAttribute[j].compareTo("") != 0) {
	String attrName = dd.getAttrName(pwrAttribute[j]);
        retColor[j] = en.gdh.refObjectInfo( attrName);
        if ( retColor[j].evenSts())
          System.out.println( "refObjectInfoError " + pwrAttribute[j]);
        else
          attrFound[j] = true;
      }
    }
  }
  public void dynamicClose() {
    for ( int j = 0; j < 2; j++) {
      if ( attrFound[j])
        en.gdh.unrefObjectInfo( retColor[j].refid);
    }
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( hold)
      return;
    if ( animationOnly)
      return;
    if ( shapes == null)
      return;

    float height = getHeight();
    float width = getWidth();
    int i, j;
    float value;

    for ( j = 0; j < 2; j++) {
      if ( attrFound[j]) {
	switch( retColor[j].typeId) {
	  case Pwr.eType_Boolean:
	    if ( en.gdh.getObjectRefInfoBoolean( retColor[j].id))
	      value = 1F; 
            else
	      value = 0F;
            break;
	  case Pwr.eType_Int32:
	  case Pwr.eType_UInt32:
	  case Pwr.eType_Int16:
	  case Pwr.eType_UInt16:
	    value = new Integer(en.gdh.getObjectRefInfoInt( retColor[j].id)).floatValue();
	    break;
          default:
            value = en.gdh.getObjectRefInfoFloat( retColor[j].id);
        }
      
	for ( i = noOfPoints - 1; i > 0; i--)
          y_values[j][i] = y_values[j][i-1];
	y_values[j][0] = (maxValue[j] - value) / (maxValue[j] -
	  	minValue[j]) * original_height;
	if ( y_values[j][0] < 0)
	  y_values[j][0] = 0;
	if ( y_values[j][0] > original_height)
	  y_values[j][0] = original_height;
        valueOld[j] = value;
      }
    }
    repaint();
    // paintForeground();
    if ( firstScan)
      firstScan = false;
  }
  public void repaintForeground() {
    Graphics g = getGraphics();
    if ( g == null) {
      System.out.println("repaintForeground: can't get Graphic object");
      return;
    }
    paintComponent(g);
    paintChildren(g);
  }
}
