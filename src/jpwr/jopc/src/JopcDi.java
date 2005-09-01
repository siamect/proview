/* 
 * Proview   $Id: JopcDi.java,v 1.2 2005-09-01 14:57:51 claes Exp $
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

package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcDi extends JopcDiGen implements JopDynamic {
  boolean hold = false;
  int pHold;
  float scanTimeOld = -1F;
  int pScanTime;
  LocalDb ldb;
  public JopcDi( JopSession session, String instance, boolean scrollbar) {
    super( session, instance, scrollbar, true);

    // engine = new JopEngine( 1000, session.getRoot());
    // ldb = new LocalDb();
    // engine.setLocalDb(ldb);
    geInit();

    engine.add(this);

  }
  public Object dynamicGetRoot() {
    return this;
  }
  GeDyn dd;
  public void dynamicOpen() {
    GdhrRefObjectInfo ret;

    ret = engine.ldb.refObjectInfo( this, "$local.TrendHold##Boolean");
    if ( ret.oddSts())
      pHold = ret.id;
    else
      System.out.println("$local.Hold not found");

    ret = engine.ldb.refObjectInfo( this, "$local.ScanTime##Float32");
    if ( ret.oddSts())
      pScanTime = ret.id;
    else
      System.out.println("$local.ScanTime not found");

    dd = new GeDyn( null);
    dd.setSession( session);
    dd.setInstance( engine.getInstance()); 

  }
  public void dynamicClose() {
    engine.ldb.unrefObjectInfo( pHold);
    engine.ldb.unrefObjectInfo( pScanTime);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;

    boolean holdValue = engine.ldb.getObjectRefInfoBoolean( pHold);
    float scanTime = engine.ldb.getObjectRefInfoFloat( pScanTime);
    if ( holdValue) {
      engine.ldb.setObjectInfo( this, "$local.TrendHold##Boolean", false);
      hold = !hold;
      jopTrend2.setHold(hold);
      if ( hold)
        jopButtontoggle6.tsetFillColor( GeColor.COLOR_115);
      else
        jopButtontoggle6.resetFillColor();
      jopButtontoggle6.repaintForeground();
    }
    if ( scanTime != scanTimeOld) {
      // TODO
      engine.ldb.setObjectInfo( this, "$local.ScanTime##Float32", 100.0F);
      scanTimeOld = scanTime;
    }
    
  }
}







