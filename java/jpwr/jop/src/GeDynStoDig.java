/* 
 * Proview   $Id: GeDynStoDig.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
import jpwr.rt.*;
import java.awt.event.*;

public class GeDynStoDig extends GeDynElem {
  String attribute;

  public GeDynStoDig( GeDyn dyn, String attribute) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_StoDig);
    this.attribute = attribute;
  }
  public void action( int eventType, MouseEvent e) {
    String attrName;

    switch ( eventType) {
    case GeDyn.eEvent_MB1Down:
      dyn.comp.setColorInverse( 1);
      dyn.repaintNow = true;

      attrName = dyn.getAttrName( attribute);
      PwrtStatus sts = dyn.en.gdh.setObjectInfo( attrName, true);
      if ( sts.evenSts())
	System.out.println( "StoDig: " + attrName);
      break;
    case GeDyn.eEvent_MB1Up:
      dyn.comp.setColorInverse( 0);
      dyn.repaintNow = true;

      attrName = dyn.getAttrName( attribute);        
      dyn.en.gdh.setObjectInfo( attrName, false);
      break;
    }
  }
}

