package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcPid extends JopcPidGen implements JopDynamic {
  boolean hold = false;
  int pHold;
  float scanTimeOld = -1F;
  int pScanTime;
  float setMaxShowOld = 0;
  float setMinShowOld = 0;
  int pSetMaxShow;
  int pSetMinShow;
  PwrtRefId setMaxShowSubid;
  PwrtRefId setMinShowSubid;
  float outMaxShowOld = 0;
  float outMinShowOld = 0;
  int pOutMaxShow;
  int pOutMinShow;
  PwrtRefId outMaxShowSubid;
  PwrtRefId outMinShowSubid;
  LocalDb ldb;
  public JopcPid( JopSession session, String instance, boolean scrollbar) {
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


    String attrName = dd.getAttrName( "$object.SetMaxShow##Float32");
    ret = engine.gdh.refObjectInfo( attrName);
    if ( ret.evenSts())
      System.out.println( "JopcPid: " + attrName);
    else {
      setMaxShowSubid = ret.refid;
      pSetMaxShow = ret.id;
    }

    attrName = dd.getAttrName( "$object.SetMinShow##Float32");
    ret = engine.gdh.refObjectInfo( attrName);
    if ( ret.evenSts())
      System.out.println( "JopcPid: " + attrName);
    else {
      setMinShowSubid = ret.refid;
      pSetMinShow = ret.id;
    }

    attrName = dd.getAttrName( "$object.OutMaxShow##Float32");
    ret = engine.gdh.refObjectInfo( attrName);
    if ( ret.evenSts())
      System.out.println( "JopcPid: " + attrName);
    else {
      outMaxShowSubid = ret.refid;
      pOutMaxShow = ret.id;
    }

    attrName = dd.getAttrName( "$object.OutMinShow##Float32");
    ret = engine.gdh.refObjectInfo( attrName);
    if ( ret.evenSts())
      System.out.println( "JopcPid: " + attrName);
    else {
      outMinShowSubid = ret.refid;
      pOutMinShow = ret.id;
    }

  }
  public void dynamicClose() {
    engine.gdh.unrefObjectInfo( setMaxShowSubid);
    engine.gdh.unrefObjectInfo( setMinShowSubid);
    engine.gdh.unrefObjectInfo( outMaxShowSubid);
    engine.gdh.unrefObjectInfo( outMinShowSubid);
    engine.ldb.unrefObjectInfo( pHold);
    engine.ldb.unrefObjectInfo( pScanTime);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;

    boolean holdValue = engine.ldb.getObjectRefInfoBoolean( pHold);
    float scanTime = engine.ldb.getObjectRefInfoFloat( pScanTime);
    float setMinShow = engine.gdh.getObjectRefInfoFloat( pSetMinShow);
    float setMaxShow = engine.gdh.getObjectRefInfoFloat( pSetMaxShow);
    float outMinShow = engine.gdh.getObjectRefInfoFloat( pOutMinShow);
    float outMaxShow = engine.gdh.getObjectRefInfoFloat( pOutMaxShow);
    if ( holdValue) {
      engine.ldb.setObjectInfo( this, "$local.TrendHold##Boolean", false);
      hold = !hold;
      jopTrend5.setHold(hold);
      jopTrend11.setHold(hold);
      if ( hold)
	jopButtontoggle10.tsetFillColor( GeColor.COLOR_115);
      else
	jopButtontoggle10.resetFillColor();
      jopButtontoggle10.repaintForeground();
    }

    if ( setMaxShow != setMaxShowOld || setMinShow != setMinShowOld) {
      jopTrend5.setMinValue1(setMinShow);
      jopTrend5.setMaxValue1(setMaxShow);
      jopTrend5.setMinValue2(setMinShow);
      jopTrend5.setMaxValue2(setMaxShow);
      jopTrend5.reset();
      jopBar4.setMinValue(setMinShow);
      jopBar4.setMaxValue(setMaxShow);
      jopBar4.update();
      jopBar14.setMinValue(setMinShow);
      jopBar14.setMaxValue(setMaxShow);
      jopBar14.update();

      setMaxShowOld = setMaxShow;
      setMinShowOld = setMinShow;
    }
    if ( outMaxShow != outMaxShowOld || outMinShow != outMinShowOld) {
      jopTrend11.setMinValue1(outMinShow);
      jopTrend11.setMaxValue1(outMaxShow);
      jopTrend11.reset();
      jopBar12.setMinValue(outMinShow);
      jopBar12.setMaxValue(outMaxShow);
      jopBar12.update();

      outMaxShowOld = outMaxShow;
      outMinShowOld = outMinShow;
    }

    if ( scanTime != scanTimeOld) {
      // TODO
      engine.ldb.setObjectInfo( this, "$local.ScanTime##Float32", 100.0F);
      scanTimeOld = scanTime;
    }
    
  }
}







