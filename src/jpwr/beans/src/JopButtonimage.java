package jpwr.beans;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import javax.swing.Timer;
import java.awt.event.*;

public class JopButtonimage extends JComponent implements JopDynamic, JopConfirm, ActionListener{
  Dimension size;
  Object root;
  JopEngine en;
  int clickAction = Jop.BUTTON_ACTION_SET;
  public int getClickAction() { return clickAction;}
  public void setClickAction(int clickAction) { this.clickAction = clickAction;}
  String command = new String();
  public String getCommand() { return command;}
  public void setCommand( String command) { this.command = command;}
  String confirmText = new String();
  public String getConfirmText() { return confirmText;}
  public void setConfirmText( String confirmText) { this.confirmText = confirmText;}
  boolean confirm = false;
  public boolean getConfirm() { return confirm;}
  public void setConfirm( boolean confirm) { this.confirm = confirm;}
  Timer timer = new Timer(500, this);
  Image normalImage;
  Image lowImage;
  Image pressImage;
  Image currentImage;
  JopSession session;
  public JopButtonimage component = this;
  public JopButtonimage()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    size = new Dimension( 102, 36);
    timer.start();
  }
  public void actionPerformed(ActionEvent e) {
    boolean engine_found = false;
    Container parent = getParent();
    while ( parent != null) {
      if ( parent instanceof JopFrame) {
        en = ((JopFrame)parent).engine;
        root = parent;
        session = ((JopFrame)root).session;
        if ( !en.isReady())
          break;
        en.add(this);
        engine_found = true;
        break;
      }
      parent = parent.getParent();
    }
    if ( !engine_found) {
      parent = getParent();
      while ( parent != null) {
        if ( parent instanceof JopApplet) {
          en = ((JopApplet)parent).engine;
          root = parent;
	  session = ((JopApplet)root).session;
          if ( !en.isReady())
            break;
          en.add(this);
          engine_found = true;
          break;
        }
        parent = parent.getParent();
      }
    }
    if ( engine_found) {
      timer.stop();
      timer = null;    
      if ( en.gdh.isAuthorized( access)) {
        this.addMouseListener(new MouseAdapter() {
          public void mouseClicked(MouseEvent e) {
            PwrtStatus sts;
            if ( confirm) {
              JopConfirmDialog.open( component, confirmText);
            }
            else if (clickAction == Jop.BUTTON_ACTION_SET) {
              sts = en.gdh.setObjectInfo( pwrAttribute, true);
              if ( sts.evenSts())
                System.out.println( "setObjectInfoError " + sts);
            }
            else if(clickAction == Jop.BUTTON_ACTION_RESET) {
              sts = en.gdh.setObjectInfo( pwrAttribute, false);
              if ( sts.evenSts())
                System.out.println( "setObjectInfoError " + sts);
            }
            else if(clickAction == Jop.BUTTON_ACTION_TOGGLE) {
              sts = en.gdh.toggleObjectInfo(pwrAttribute);
              if ( sts.evenSts())
                System.out.println( "setObjectInfoError " + sts);
            }
            else if(clickAction == Jop.BUTTON_ACTION_COMMAND) {
              Jop.executeCommand( session, command);
            }
          }
          public void mousePressed(MouseEvent e) {
            currentImage = pressImage;
  	    pressed = true;
	    repaint();
          }
          public void mouseReleased(MouseEvent e) {
            if ( valueImage)
              currentImage = normalImage;
            else
              currentImage = lowImage;
            pressed = false;
	    repaint();
          }
        });
      }
    }
  }
  public void confirmNo() {}
  public void confirmYes() {
    PwrtStatus sts;
    switch ( clickAction) {
      case Jop.BUTTON_ACTION_SET:
        sts = en.gdh.setObjectInfo( pwrAttribute, true);
        if ( sts.evenSts())
          System.out.println( "setObjectInfoError " + sts);
        break;
      case Jop.BUTTON_ACTION_RESET:
        sts = en.gdh.setObjectInfo( pwrAttribute, false);
        if ( sts.evenSts())
          System.out.println( "setObjectInfoError " + sts);
        break;
      case Jop.BUTTON_ACTION_TOGGLE:
        sts = en.gdh.toggleObjectInfo( pwrAttribute);
        if ( sts.evenSts())
          System.out.println( "setObjectInfoError " + sts);
        break;
      case Jop.BUTTON_ACTION_COMMAND:
        if ( command.compareTo("") != 0)
          Jop.executeCommand( session, command);
        break;
    }
  }
  private int access = 65532;
  public void setAccess( int access) { this.access = access;}
  public int getAccess() { return access;}
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
  public void paintComponent(Graphics g1) {
    Graphics2D g = (Graphics2D) g1;
    if ( root == null) {
      Container parent = getParent();
      while ( parent != null) {
	if ( parent instanceof JopFrame) {
	  root = parent;
	  session = ((JopFrame)root).session;  
	  break;
	}
        else if (  parent instanceof JopApplet) {
          root = parent;
	  session = ((JopApplet)root).session;
          break;
        }
        parent = parent.getParent();
      }
    }
    if ( root == null)
      return;
    if ( (normalImage == null && image != null) || 
	 (image != null && !image.equals(imageOld))) {
      normalImage = JopSpider.getImage( session, image);
      imageOld = new String(image);
      int imageWidth = normalImage.getWidth(this);
      int imageHeight = normalImage.getHeight(this);
      if ( valueImage)
        currentImage = normalImage;
    }
    if (lowImage == null && imageLow != null) {
      lowImage = JopSpider.getImage( session, imageLow);
      int imageWidth = lowImage.getWidth(this);
      int imageHeight = lowImage.getHeight(this);
      if ( !valueImage)
        currentImage = lowImage;
    }
    if (pressImage == null && imagePress != null) {
      pressImage = JopSpider.getImage( session, imagePress);
      int imageWidth = pressImage.getWidth(this);
      int imageHeight = pressImage.getHeight(this);
    }
    if ( currentImage != null)
      g.drawImage( currentImage, 0, 0, this);
  }
  String image = "test.gif";
  String imageLow = "test.gif";
  String imagePress = "test.gif";
  public String getImage() {
    return image;
  }
  public void setImage( String image) {
   this.image = image;
  }
  public String getImageLow() {
    return imageLow;
  }
  public void setImageLow( String imageLow) {
   this.imageLow = imageLow;
  }
  public String getImagePress() {
    return imagePress;
  }
  public void setImagePress( String imagePress) {
   this.imagePress = imagePress;
  }
  String imageOld;
  float widthOld = 0;
  float heightOld = 0;
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
  String pwrAttribute = new String();
  public void setPwrAttribute( String pwrAttribute) { this.pwrAttribute = pwrAttribute;}
  public String getPwrAttribute() { return pwrAttribute;}
  String pwrAttrImage = new String();
  public void setPwrAttrImage( String pwrAttrImage) { this.pwrAttrImage = pwrAttrImage;}
  public String getPwrAttrImage() { return pwrAttrImage;}
  boolean valueImage = true;
  boolean valueImageOld;
  boolean firstScan = true;
  boolean pressed = false;
  boolean pressedOld;
  GdhrRefObjectInfo retImage = null;
  boolean imageAttrFound = false;
  public void dynamicOpen() {
    if ( pwrAttrImage.compareTo("") != 0) {
      retImage = en.gdh.refObjectInfo( pwrAttrImage);
      if ( retImage.evenSts())
        System.out.println( "refObjectInfoError retImage");
      else
        imageAttrFound = true;
    }
  }
  public void dynamicClose() {
    if ( imageAttrFound)
      en.gdh.unrefObjectInfo( retImage.refid);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;
    if ( normalImage == null)
      return;
    if ( imageAttrFound) {
      valueImage = en.gdh.getObjectRefInfoBoolean( retImage.id);
      if ( valueImageOld != valueImage || pressed != pressedOld || firstScan) {
        if ( pressed)
          currentImage = pressImage;
        else if ( valueImage) {
          currentImage = normalImage;
          repaint();
        }
        else {
          currentImage = lowImage;
          repaint();
        }
      }
      valueImageOld = valueImage;
      pressedOld = pressed;
    }
    if ( firstScan)
      firstScan = false;
  }

  public boolean imageUpdate( Image img, int infoflags, int x, int y,
  	int width, int height) {
    if (( infoflags & ImageObserver.ERROR) != 0) {
      System.out.println("Error loading image");
    }
    if ((infoflags & ImageObserver.WIDTH) != 0 &&
        (infoflags & ImageObserver.HEIGHT) != 0) {
      int imageWidth = img.getWidth(this);
      int imageHeight = img.getHeight(this);
    }
    if ((infoflags & ImageObserver.SOMEBITS) != 0) {
      if ( img == currentImage)
        repaint();
    }
    if ((infoflags & ImageObserver.ALLBITS) != 0) {
      int imageWidth = img.getWidth(this);
      int imageHeight = img.getHeight(this);
      if ( img == currentImage)
        repaint();
      return false;
    }
    return true;
  }
  public Object dynamicGetRoot() {
    return null;
  }
}
