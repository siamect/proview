/* 
 * Proview   $Id: GeDynAnimation.java,v 1.2 2005-09-01 14:57:50 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

package jpwr.jop;
import javax.swing.*;
import jpwr.rt.*;

public class GeDynAnimation extends GeDynElem {
  String attribute;
  int sequence;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  boolean inverted;
  boolean oldValue;
  boolean firstScan = true;
  int animationCount;
  int animationDirection;

  public GeDynAnimation( GeDyn dyn, String attribute, int sequence) {
    super( dyn, GeDyn.mDynType_Animation, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.sequence = sequence;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "DigShift: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	inverted = GeDyndata.getAttrInverted( attribute);
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

    boolean repaintNow = false;
    boolean value0 = dyn.en.gdh.getObjectRefInfoBoolean( p);
    int sts;
    int maxCount;

    switch ( sequence) {
    case GeDyn.eAnimSequence_Cycle:
    case GeDyn.eAnimSequence_ForwBack: {
      if ( firstScan) {
        animationCount = 0;
        animationDirection = 1;
      }
      if ( (!inverted && value0) || (inverted && !value0)) {
        if ( animationDirection == 0) {
             animationCount = 0;
             animationDirection = 1;
        }
        maxCount = animationCount;
        animationCount++;
        if ( animationCount >= maxCount) {
	  // Shift nodeclass
	  if ( animationDirection == 1) {
	    // Shift forward
	    sts = dyn.comp.setNextPage();
	    if ( sts == 0) {
	      if ( sequence == GeDyn.eAnimSequence_Cycle)
	        // Start from beginning the again
                dyn.comp.setPage( 2);
	      else {
	        // Change direction
	        animationDirection = 2;
                dyn.comp.setPreviousPage();
	      }
	    }
	    animationCount = 0;
	    repaintNow = true;
	  }
	  else {
	    // Shift backwards
	    sts = dyn.comp.setPreviousPage();
	    if ( sts == 0) {
	      // Change direction
	      animationDirection = 1;
              dyn.comp.setNextPage();
	    }
	    animationCount = 0;
	    repaintNow = true;
	  }
        }
      }
      else {
        if ( animationDirection != 0) {
	  animationDirection = 0;
	  dyn.comp.setFirstPage();
	  repaintNow = true;
        }
      }
      break;
    }
    case GeDyn.eAnimSequence_Dig: {
      if ( firstScan) {
        animationCount = 0;
        animationDirection = 0;
        if ( (!inverted && value0) || (inverted && !value0)) {
          dyn.comp.setLastPage();
          repaintNow = true;
        }
        oldValue = value0;
      }
      if ( oldValue != value0) {
        // Start animation
        if ( (!inverted && value0) || (inverted && !value0)) {
          animationCount = 0;
          animationDirection = 1;
        }
        else if ( (!inverted && !value0) || (inverted && value0)) {
          animationCount = 0;
          animationDirection = 2;
        }
      }

      if ( animationDirection != 0) {
        maxCount = animationCount;
        animationCount++;
        if ( animationCount >= maxCount) {
          // Shift nodeclass
          if ( animationDirection == 1) {
            // Shift forward
            sts = dyn.comp.setNextPage();
            if ( sts == 0) {
              // End of animation
              animationCount = 0;
              animationDirection = 0;
            }
            animationCount = 0;
            repaintNow = true;
          }
          else {
            // Shift backwards
            sts = dyn.comp.setPreviousPage();
            if ( sts == 0) {
              // End of animation
              animationCount = 0;
              animationDirection = 0;
            }
            animationCount = 0;
            repaintNow = true;
          }
        }
      }
      break;
    }
    }
    oldValue = value0;

    if ( repaintNow)
      ((JComponent)dyn.comp).repaint();
    if ( firstScan)
      firstScan = false;
  }
}




