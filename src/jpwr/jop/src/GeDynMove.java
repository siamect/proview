package jpwr.jop;
import java.awt.*;
import javax.swing.*;
import jpwr.rt.*;

public class GeDynMove extends GeDynElem {
public  String moveXAttribute;
public  String moveYAttribute;
  String scaleXAttribute;
  String scaleYAttribute;
  double xOffset;
  double yOffset;
  double factor;
  double scaleFactor;

  boolean attrMoveXFound;
  boolean attrMoveYFound;
  boolean attrScaleXFound;
  boolean attrScaleYFound;
  PwrtRefId moveXSubid;
  PwrtRefId moveYSubid;
  PwrtRefId scaleXSubid;
  PwrtRefId scaleYSubid;
  int moveXp;
  int moveYp;
  int scaleXp;
  int scaleYp;
  float moveXOldValue;
  float moveYOldValue;
  float scaleXOldValue;
  float scaleYOldValue;
  public double         xOrig;
  public double         yOrig;
  public double wOrig;
  public double hOrig;
  public double         xScale = 1;
  public double         yScale = 1;
  boolean firstScan = true;

  public GeDynMove( GeDyn dyn, String moveXAttribute, String moveYAttribute, 
		    String scaleXAttribute, String scaleYAttribute, double 
		    xOffset, double yOffset, double factor, double scaleFactor) {
    super( dyn, GeDyn.mDynType_Move, GeDyn.mActionType_No);
    this.moveXAttribute = moveXAttribute;
    this.moveYAttribute = moveYAttribute;
    this.scaleXAttribute = scaleXAttribute;
    this.scaleYAttribute = scaleYAttribute;
    this.xOffset = xOffset;
    this.yOffset = yOffset;
    this.factor = factor;
    this.scaleFactor = scaleFactor;
  }
  public void connect() {
    String attrName = dyn.getAttrName( moveXAttribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "Move: " + attrName);
      else {
	attrMoveXFound = true;
	moveXp = ret.id;
	moveXSubid = ret.refid;
      }
    }
    attrName = dyn.getAttrName( moveYAttribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "Move: " + attrName);
      else {
	attrMoveYFound = true;
	moveYp = ret.id;
	moveYSubid = ret.refid;
      }
    }
    attrName = dyn.getAttrName( scaleXAttribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "Move: " + attrName);
      else {
	attrScaleXFound = true;
	scaleXp = ret.id;
	scaleXSubid = ret.refid;
      }
    }
    attrName = dyn.getAttrName( scaleYAttribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "Move: " + attrName);
      else {
	attrScaleYFound = true;
	scaleYp = ret.id;
	scaleYSubid = ret.refid;
      }
    }
  }
  public void disconnect() {
    if ( attrMoveXFound)
      dyn.en.gdh.unrefObjectInfo( moveXSubid);
    if ( attrMoveYFound)
      dyn.en.gdh.unrefObjectInfo( moveYSubid);
    if ( attrScaleXFound)
      dyn.en.gdh.unrefObjectInfo( scaleXSubid);
    if ( attrScaleXFound)
      dyn.en.gdh.unrefObjectInfo( scaleXSubid);
  }
  public void scan() {
    boolean repaintNow = false;

    if ( firstScan) {
      Point loc = ((JComponent)dyn.comp).getLocation();
      xOrig = (double) loc.x;
      yOrig = (double) loc.y;
      Dimension size = ((JComponent)dyn.comp).getSize();
      wOrig= (double) size.width;
      hOrig= (double) size.height;
    }

    if ( attrMoveXFound || attrMoveYFound) {
      // Move
      Point loc = ((JComponent)dyn.comp).getLocation();
      Dimension size = ((JComponent)dyn.comp).getSize();
      float valueMoveX = 0;
      float valueMoveY = 0;

      if ( attrMoveXFound) {
	valueMoveX = dyn.en.gdh.getObjectRefInfoFloat( moveXp);
	if ( valueMoveX != moveXOldValue  || firstScan) {
	  repaintNow = true;
	  moveXOldValue = valueMoveX;
        }
      }
      if ( attrMoveYFound) {
	valueMoveY = dyn.en.gdh.getObjectRefInfoFloat( moveYp);
	if ( valueMoveY != moveYOldValue || firstScan) {
	  repaintNow = true;
	  moveYOldValue = valueMoveY;
	}
      }
      if ( repaintNow) {
	  if ( attrMoveXFound){
	    double xRatio=(size.width/wOrig);
	    loc.x = (int) ((xOrig + (valueMoveX - xOffset) * factor) * xRatio);
	  }
	  if ( attrMoveYFound){
	    double yRatio=(size.height/hOrig);
	    loc.y = (int) ((yOrig + (valueMoveY - yOffset) * factor) * yRatio);
	  }
	((JComponent)dyn.comp).setLocation( loc);
      }
    }
    if ( attrScaleXFound || attrScaleYFound) {
      // Scale
      if ( firstScan) {
        xScale = ((JComponent)dyn.comp).getWidth();
        yScale = ((JComponent)dyn.comp).getHeight();
      }
      float valueScaleX = 0;
      float valueScaleY = 0;

      if ( attrScaleXFound) {
        valueScaleX = (float) (dyn.en.gdh.getObjectRefInfoFloat( scaleXp) * scaleFactor);
        if ( valueScaleX != scaleXOldValue  || firstScan) {
          repaintNow = true;
 	  scaleXOldValue = valueScaleX;
        }
      }
      if ( attrScaleYFound) {
        valueScaleY = (float) (dyn.en.gdh.getObjectRefInfoFloat( scaleYp) * scaleFactor);
        if ( valueScaleY != scaleYOldValue  || firstScan) {
	  repaintNow = true;
	  scaleYOldValue = valueScaleY;
        }
      }
      if ( repaintNow) {
        Rectangle rect = ((JComponent)dyn.comp).getBounds();
        if ( attrScaleXFound) {
	  rect.width = (int) (xScale * valueScaleX);
	  if ( rect.width < 1)
	    rect.width = 1;
        }
        if ( attrScaleYFound) {
	  rect.height = (int) (yScale * valueScaleY);
	  if ( rect.height < 1)
	    rect.height = 1;
        }
        ((JComponent)dyn.comp).setBounds( rect);
      }
    }

    if ( firstScan)
      firstScan = false;
  }
}











