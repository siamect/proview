package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcPlcthread extends JopcPlcthreadGen implements JopDynamic {
  boolean hold = false;
  int pHold;
  float scanTimeOld = -1F;
  int pScanTime;
  float maxShowOld = 0;
  float minShowOld = 0;
  int pMaxShow;
  int pMinShow;
  PwrtRefId maxShowSubid;
  PwrtRefId minShowSubid;
  LocalDb ldb;
  public JopcPlcthread( JopSession session, String instance, boolean scrollbar) {
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

    ret = engine.ldb.refObjectInfo( this, "$local.MinShow##Float32");
    if ( ret.oddSts())
      pMinShow = ret.id;
    else
      System.out.println("$local.MinShow not found");

    ret = engine.ldb.refObjectInfo( this, "$local.MaxShow##Float32");
    if ( ret.oddSts())
      pMaxShow = ret.id;
    else
      System.out.println("$local.MaxShow not found");

    dd = new GeDyn( null);
    dd.setSession( session);
    dd.setInstance( engine.getInstance()); 

    String attrName = dd.getAttrName( "$object.ScanTime##Float32");
    CdhrFloat retf = engine.gdh.getObjectInfoFloat( attrName);
    if ( retf.evenSts())
      System.out.println( "JopcMode: ScanTime");
    else
      maxShowOld = retf.value * 2;

    engine.ldb.setObjectInfo( this, "$local.MaxShow##Float32", maxShowOld);
    jopTrend3.setMaxValue1(maxShowOld);
    jopBar2.setMaxValue(maxShowOld);
  }
  public void dynamicClose() {
    engine.ldb.unrefObjectInfo( pMaxShow);
    engine.ldb.unrefObjectInfo( pMinShow);
    engine.ldb.unrefObjectInfo( pHold);
    engine.ldb.unrefObjectInfo( pScanTime);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;

    boolean holdValue = engine.ldb.getObjectRefInfoBoolean( pHold);
    float scanTime = engine.ldb.getObjectRefInfoFloat( pScanTime);
    float minShow = engine.ldb.getObjectRefInfoFloat( pMinShow);
    float maxShow = engine.ldb.getObjectRefInfoFloat( pMaxShow);
    if ( holdValue) {
      engine.ldb.setObjectInfo( this, "$local.TrendHold##Boolean", false);
      hold = !hold;
      jopTrend3.setHold(hold);
      if ( hold)
	jopButtontoggle8.tsetFillColor( GeColor.COLOR_115);
      else
	jopButtontoggle8.resetFillColor();
      jopButtontoggle8.repaintForeground();
    }
    if ( maxShow != maxShowOld || minShow != minShowOld) {
      jopTrend3.setMinValue1(minShow);
      jopTrend3.setMaxValue1(maxShow);
      jopTrend3.reset();
      jopBar2.setMinValue(minShow);
      jopBar2.setMaxValue(maxShow);
      jopBar2.update();

      maxShowOld = maxShow;
      minShowOld = minShow;
    }
    if ( scanTime != scanTimeOld) {
      // TODO
      engine.ldb.setObjectInfo( this, "$local.ScanTime##Float32", 100.0F);
      scanTimeOld = scanTime;
    }
    
  }
}







