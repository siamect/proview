package jpwr.jop;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import javax.swing.Timer;
import java.awt.event.*;
import jpwr.rt.*;

public class JopBar extends JComponent implements GeComponentIfc, 
            JopDynamic, JopConfirm, ActionListener{
  Dimension size;
  Timer timer = new Timer(500, this);
  Object root;
  JopSession session;
  JopEngine en;
  public GeDyn dd = new GeDyn( this);
  public JopBar( JopSession session)
  {
    this.session = session;
    dd.setSession( session);
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

  // GeComponents Ifc
  public void tsetFillColor( int fillColor) {}
  public void tsetColorTone( int colorTone) {}
  public void tsetBorderColor( int borderColor) {}
  public void tsetTextColor( int borderColor) {}
  public void setColorInverse( int colorInverse) {} 
  public void resetFillColor() {}
  public void resetColorTone() {}
  public void resetBorderColor() {}
  public void resetTextColor() {}
  public String getAnnot1() { return new String();}
  public void setVisibility( int visibility) {}
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
  int fillColorBar = 9999;
  int borderColorBar = 9999;
  int drawFill = 0;
  int drawBorder = 0;
  int lineWidth = 1;
  int barBorderWidth = 1;
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
  public void setFillColorBar( int fillColorBar) {
    this.fillColorBar = fillColorBar;
  }
  public int getFillColorBar() {
    return fillColorBar;
  }
  public void setBorderColorBar( int borderColorBar) {
    this.borderColorBar = borderColorBar;
  }
  public int getBorderColorBar() {
    return borderColorBar;
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
  public void setBarBorderWidth( int barBorderWidth) {
    this.barBorderWidth = barBorderWidth;
  }
  public int getBarBorderWidth() {
    return barBorderWidth;
  }
  public double rotate;
  public void setRotate( double rotate) { this.rotate = rotate;}
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { null, null, null,};
  public void paint(Graphics g1) {
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
    
    Graphics2D g = (Graphics2D) g1;
//    double scaleWidth = ((double) getWidth()) / original_width;
//    double scaleHeight = ((double) getHeight()) / original_height;
//    AffineTransform save = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
//    if ( rotate != 0)
//      g.rotate( Math.PI * rotate/180, getWidth()/2, getHeight()/2);
//      g.transform( AffineTransform.getRotateInstance(
//        Math.PI * rotate/180,((double)original_width)/2, ((double)original_height)/2));
//    g.transform( AffineTransform.getScaleInstance( scaleWidth, scaleHeight));
    float width = getWidth();
    float height = getHeight();
    if ( shapes[0] == null) {
      original_width = width;
      original_height = height;
      shapes[0] = new Rectangle2D.Float(0F, 0F, width, height);
      shapes[1] = new Rectangle2D.Float(0F, 0F, width, 1F);
      shapes[2] = new Line2D.Float(0F, 0F, width, 0F);
    }
    g.transform( AffineTransform.getScaleInstance( width/original_width,
						   height/original_height));
    if ( drawFill == 1) {
      g.setColor(GeColor.getColor(22, fillColor));
      g.fill( shapes[0]);
    }
    g.setColor(GeColor.getColor( borderColor, fillColorBar));
    g.fill( shapes[1]);
    g.setStroke( new BasicStroke((float)barBorderWidth));
    g.setColor(GeColor.getColor(0, borderColorBar));
    g.draw( shapes[2]);
    g.setStroke( new BasicStroke((float)lineWidth));
    if ( drawBorder == 1 || drawFill == 0) {
      g.setColor(GeColor.getColor(0, borderColor));
      g.draw( shapes[0]);
    }
//    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
  int lowColor = 0;
  public void setLowColor( int lowColor) { this.lowColor = lowColor;}
  public int getLowColor() { return lowColor;}
  String pwrAttribute = new String();
  public void setPwrAttribute( String pwrAttribute) { this.pwrAttribute = pwrAttribute;}
  public String getPwrAttribute() { return pwrAttribute;}
  float valueColor;
  float valueColorOld;
  boolean firstScan = true;
  GdhrRefObjectInfo retColor = null;
  boolean colorAttrFound = false;
  float minValue = 0;
  float maxValue = 100;
  public void setMinValue( float minValue) {
    this.minValue = minValue;
  }
  public float getMinValue() {
    return minValue;
  }
  public void setMaxValue( float maxValue) {
    this.maxValue = maxValue;
  }
  public float getMaxValue() {
    return maxValue;
  }
  public Object dynamicGetRoot() {
    return root;
  }
  public void update() {
    valueColorOld = -10000;
    dynamicUpdate( false);
  }
  public void dynamicOpen() {
    if ( en.isInstance())
      dd.setInstance( en.getInstance());
    if ( pwrAttribute.compareTo("") != 0) {
      String attrName = dd.getAttrName(pwrAttribute);
      retColor = en.gdh.refObjectInfo( attrName);
      if ( retColor.evenSts())
        System.out.println( "refObjectInfoError retColor");
      else
        colorAttrFound = true;
    }
  }
  public void dynamicClose() {
    if ( colorAttrFound)
      en.gdh.unrefObjectInfo( retColor.refid);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( maxValue == minValue)
      return;
    if ( animationOnly)
      return;
    if ( colorAttrFound) {
      if ( shapes[0] == null)
        return;
      valueColor = en.gdh.getObjectRefInfoFloat( retColor.id);
      if ( valueColorOld != valueColor || firstScan) {
        float bar_height;
        float height = (float)((Rectangle2D.Float )shapes[0]).getHeight();
        float width = (float)((Rectangle2D.Float )shapes[0]).getWidth();
        float x = (float)((Rectangle2D.Float)shapes[0]).getX();
	float y = (float)((Rectangle2D.Float)shapes[0]).getY();
        if (  45.0 < rotate && rotate <= 135.0) {
          bar_height = valueColor / (maxValue - minValue) * width;
          ((Rectangle2D.Float )shapes[1]).setRect( x, y, 
	  	bar_height, height);
          ((Line2D.Float )shapes[2]).setLine( x + bar_height, y, 
	  	x + bar_height, y + height);
        }
        else if ( 135.0 < rotate && rotate <= 225.0) {
          bar_height = valueColor / (maxValue - minValue) * height;
          ((Rectangle2D.Float )shapes[1]).setRect( x, y, width, bar_height);
          ((Line2D.Float )shapes[2]).setLine( x, y + bar_height, x + width, y + bar_height);
        }
        else if ( 225.0 < rotate && rotate <= 315.0) {
          bar_height = valueColor / (maxValue - minValue) * width;
          ((Rectangle2D.Float )shapes[1]).setRect( x + width - bar_height, y, 
	  	bar_height, height);
          ((Line2D.Float )shapes[2]).setLine( x + width - bar_height, y, 
	  	x + width - bar_height, y + height);
        }
        else {
          bar_height = (valueColor - minValue) / (maxValue - minValue) * height;
	  if ( bar_height < 0)
	    bar_height = 0;
	  if ( bar_height > height)
	    bar_height = height;
          ((Rectangle2D.Float )shapes[1]).setRect( x, y + height - bar_height, width,
	  	 bar_height);
          ((Line2D.Float )shapes[2]).setLine( x, y + height - bar_height, x + width, 
	  	y + height - bar_height);
        }
	// repaint();
	repaintForeground();
        valueColorOld = valueColor;
      }
    }
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
