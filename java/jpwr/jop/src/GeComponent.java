
package jpwr.jop;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import javax.swing.Timer;
import java.awt.event.*;
import jpwr.rt.*;

public class GeComponent extends JComponent implements GeComponentIfc, 
	JopDynamic, JopConfirm, ActionListener {
  public Dimension size;
  public Object root;
  public JopEngine en;
  public GeDyn dd = new GeDyn( this);
  public int level;
  public Timer timer = new Timer(100, this);
  StringBuffer sb = new StringBuffer();
  public JopSession session;
  public GeComponent component = this;
  public GeComponent( JopSession session)
  {
    this.session = session;
    timer.start();
    dd.setSession( session);
    en = session.getEngine();
  }
  public void actionPerformed(ActionEvent e) {
    boolean engine_found = false;

    root = getTopLevelAncestor();
    if ( root != null) {

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

      if ( dd.actionType != 0 && en.gdh.isAuthorized( dd.access)) {
        this.addMouseListener(new MouseAdapter() {
          public void mouseReleased(MouseEvent e) {
	    if ( dimmed)
	      return;
	    if ( e.isPopupTrigger())
	      dd.action( GeDyn.eEvent_MB3Press, e);
	    else if ((e.getModifiers() & MouseEvent.BUTTON1_MASK) != 0 &&
		     en.gdh.isAuthorized( dd.access))
	      dd.action( GeDyn.eEvent_MB1Up, e);
	  }

          public void mousePressed(MouseEvent e) {
	    if ( dimmed)
	      return;
	    if ( e.isPopupTrigger())
	      dd.action( GeDyn.eEvent_MB3Press, e);
	    else if ((e.getModifiers() & MouseEvent.BUTTON1_MASK) != 0 &&
		     en.gdh.isAuthorized( dd.access))
	      dd.action( GeDyn.eEvent_MB1Down, e);
	  }
          public void mouseClicked(MouseEvent e) {
	    if ( dimmed)
	      return;
	    if ((e.getModifiers() & MouseEvent.BUTTON1_MASK) != 0 &&
		en.gdh.isAuthorized( dd.access))
	      dd.action( GeDyn.eEvent_MB1Click, e);
	  }
        });
	if ( (dd.actionType & GeDyn.mActionType_Slider) != 0) {
	  this.addMouseMotionListener( new MouseMotionAdapter() {
	    public void mouseDragged(MouseEvent e) {
	      if ( actionDisabled || dimmed)
	        return;
	      if ((e.getModifiers() & MouseEvent.BUTTON1_MASK) != 0 &&
		  en.gdh.isAuthorized( dd.access))
		dd.action( GeDyn.eEvent_SliderMoved, e);
	    }
	  });
	}
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
  public void tsetFillColor( int fillColor) { this.fillColor = fillColor;}
  public void tsetColorTone( int colorTone) { this.colorTone = colorTone;}
  public void tsetBorderColor( int borderColor) { this.borderColor = borderColor;}
  public void tsetTextColor( int textColor) { this.textColor = textColor;}
  public void setColorInverse( int colorInverse) { this.colorInverse = colorInverse;} 
  public void resetFillColor() { fillColor = originalFillColor;}
  public void resetColorTone() { colorTone = originalColorTone; }
  public void resetBorderColor() { borderColor = originalBorderColor;}
  public void resetTextColor() { textColor = originalTextColor;}
  public void setVisibility( int visibility) {
    switch ( visibility) {
    case Ge.VISIBILITY_VISIBLE:
      visible = true;
      dimmed = false;
      break;
    case Ge.VISIBILITY_INVISIBLE:
      visible = false;
      dimmed = false;
      repaint();
      break;
    case Ge.VISIBILITY_DIMMED:
      visible = true;
      dimmed = true;
      repaint();
      break;
    }
  }
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  public Object getDd() { return dd;}
  public void setFillLevel( float fillLevel) { this.fillLevel = fillLevel;}
  public void setLevelDirection( int levelDirection) { this.levelDirection = levelDirection;}
  public void setLevelColorTone( int levelColorTone) { this.levelColorTone = levelColorTone;}
  public void setLevelFillColor( int levelFillColor) { this.levelFillColor = levelFillColor;}

  boolean actionDisabled = false;
  public void setActionDisabled( boolean actionDisabled) { this.actionDisabled = actionDisabled;}
  public String annot1 = new String();
  public Font annot1Font = new Font("Helvetica", Font.BOLD, 14);
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public int fillColor = 9999;
  public int originalFillColor = 9999;
  public int borderColor = 9999;
  public int originalTextColor = 9999;
  public int textColor = 9999;
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
  public float fillLevel = 1F;
  public int levelDirection = 0;
  public int levelColorTone = 0;
  public int levelFillColor = 0;
  public int shadow = 0;
  public boolean visible = true;
  public boolean dimmed = false;
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
  public void setTextColor( int textColor) {
    this.textColor = textColor;
    this.originalTextColor = textColor;
  }
  public int getTextColor() {
    return textColor;
  }
  public void setShadow( int shadow) {
    this.shadow = shadow;
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
    if ( en.isInstance())
      dd.setInstance( en.getInstance());
    dd.connect();
  }
  public void dynamicClose() {
    dd.disconnect();
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly &&
         (dd.dynType & GeDyndata.eTrace_Animation) == 0)
      return;

    dd.scan();
  }
  public void repaintForeground() {
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




