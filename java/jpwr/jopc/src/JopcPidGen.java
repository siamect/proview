package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcPidGen extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_valuemedium	pwr_valuemedium0;
  pwr_valuemedium	pwr_valuemedium2;
  JopBar	jopBar4;
  JopTrend	jopTrend5;
  pwr_framethin	pwr_framethin6;
  pwr_framethin	pwr_framethin7;
  pwr_framethin	pwr_framethin8;
  JopButtontoggle	jopButtontoggle10;
  JopTrend	jopTrend11;
  JopBar	jopBar12;
  pwr_framethin	pwr_framethin13;
  JopBar	jopBar14;
  pwr_valuemedium	pwr_valuemedium15;
  pwr_indroundlarge	pwr_indroundlarge26;
  pwr_indroundlarge	pwr_indroundlarge28;
  pwr_valuemedium	pwr_valuemedium29;
  pwr_valuesmall	pwr_valuesmall31;
  pwr_valuesmall	pwr_valuesmall32;
  pwr_buttoncommand	pwr_buttoncommand33;
  pwr_indroundlarge	pwr_indroundlarge35;
  pwr_valueinputmedium	pwr_valueinputmedium36;
  pwr_valueinputmedium	pwr_valueinputmedium37;
  pwr_valueinputmedium	pwr_valueinputmedium38;
  pwr_valueinputmedium	pwr_valueinputmedium39;
  pwr_valueinputmedium	pwr_valueinputmedium40;
  pwr_valueinputmedium	pwr_valueinputmedium41;
  pwr_valueinputmedium	pwr_valueinputmedium42;
  pwr_valueinputmedium	pwr_valueinputmedium43;
  pwr_valueinputreliefup	pwr_valueinputreliefup44;
  pwr_valueinputreliefup	pwr_valueinputreliefup45;
  pwr_valueinputreliefup	pwr_valueinputreliefup46;
  pwr_valueinputreliefup	pwr_valueinputreliefup47;
  pwr_valueinputreliefup	pwr_valueinputreliefup48;
  public JopcPidGen( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcPidGen( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "Œ-i@€l@œá@`ïÿ¿(ïÿ¿ß}");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 847, 413);
    Dimension dsize = new Dimension(localPanel.original_width,localPanel.original_height);
    this.addComponentListener(new AspectRatioListener(this,size));
    contentPane = (JPanel) this.getContentPane();
    contentPane.setLayout(borderLayout1);
    if ( scrollbar)
      contentPane.add( new JScrollPane(localPanel), BorderLayout.CENTER);
    else
      contentPane.add(localPanel, BorderLayout.CENTER);
    contentPane.setOpaque(true);
    localPanel.setLayout( new RatioLayout()); // scaletest
    localPanel.setOpaque(true);
    localPanel.setBackground(GeColor.getColor(51, GeColor.NO_COLOR));
    this.setSize(size);
    if ( engine.isInstance())
      setTitle( engine.getInstance());
    else
      this.setTitle("JopcPidGen");
    pwr_valuemedium0 = new pwr_valuemedium(session);
    pwr_valuemedium0.setBounds(new Rectangle(83,76,98,24));
    pwr_valuemedium0.setFillColor(51);
    pwr_valuemedium0.setBorderColor(56);
    localPanel.add(pwr_valuemedium0, new Proportion(pwr_valuemedium0.getBounds(), dsize));
    pwr_valuemedium2 = new pwr_valuemedium(session);
    pwr_valuemedium2.setBounds(new Rectangle(83,99,98,24));
    pwr_valuemedium2.setFillColor(51);
    pwr_valuemedium2.setBorderColor(56);
    localPanel.add(pwr_valuemedium2, new Proportion(pwr_valuemedium2.getBounds(), dsize));
    jopBar4 = new JopBar(session);
    jopBar4.setBounds(new Rectangle(291,11,17,151));
    jopBar4.setFillColor(29);
    jopBar4.setBorderColor(56);
    jopBar4.setFillColorBar(296);
    jopBar4.setBorderColorBar(291);
    jopBar4.setDrawFill(1);
    jopBar4.setDrawBorder(1);
    jopBar4.setBarBorderWidth(3);
    jopBar4.setLineWidth(1);
    jopBar4.setMinValue(0F);
    jopBar4.setMaxValue(100F);
    jopBar4.setRotate(0);
    localPanel.add(jopBar4, new Proportion(jopBar4.getBounds(), dsize));
    jopTrend5 = new JopTrend( session);
    jopTrend5.setBounds(new Rectangle(329,10,501,151));
    jopTrend5.setFillColor(59);
    jopTrend5.setBorderColor(57);
    jopTrend5.setBorderColorTrend1(291);
    jopTrend5.setBorderColorTrend2(172);
    jopTrend5.setFillColorTrend1(28);
    jopTrend5.setFillColorTrend2(9999);
    jopTrend5.setDrawFill(1);
    jopTrend5.setDrawBorder(1);
    jopTrend5.setTrendBorderWidth(1);
    jopTrend5.setLineWidth(1);
    jopTrend5.setMinValue1(0F);
    jopTrend5.setMaxValue1(100F);
    jopTrend5.setMinValue2(0F);
    jopTrend5.setMaxValue2(100F);
    jopTrend5.setNoOfPoints(200);
    jopTrend5.setHorizontalLines(4);
    jopTrend5.setVerticalLines(9);
    jopTrend5.setRotate(0);
    localPanel.add(jopTrend5, new Proportion(jopTrend5.getBounds(), dsize));
    pwr_framethin6 = new pwr_framethin(session);
    pwr_framethin6.setBounds(new Rectangle(3,3,211,360));
    localPanel.add(pwr_framethin6, new Proportion(pwr_framethin6.getBounds(), dsize));
    pwr_framethin7 = new pwr_framethin(session);
    pwr_framethin7.setBounds(new Rectangle(321,4,516,360));
    localPanel.add(pwr_framethin7, new Proportion(pwr_framethin7.getBounds(), dsize));
    pwr_framethin8 = new pwr_framethin(session);
    pwr_framethin8.setBounds(new Rectangle(266,5,46,164));
    localPanel.add(pwr_framethin8, new Proportion(pwr_framethin8.getBounds(), dsize));
    jopButtontoggle10 = new JopButtontoggle(session);
    jopButtontoggle10.setBounds(new Rectangle(556,332,62,21));
    jopButtontoggle10.setColorTone(1);
    localPanel.add(jopButtontoggle10, new Proportion(jopButtontoggle10.getBounds(), dsize));
    jopTrend11 = new JopTrend( session);
    jopTrend11.setBounds(new Rectangle(329,177,501,151));
    jopTrend11.setFillColor(59);
    jopTrend11.setBorderColor(57);
    jopTrend11.setBorderColorTrend1(110);
    jopTrend11.setBorderColorTrend2(9999);
    jopTrend11.setFillColorTrend1(28);
    jopTrend11.setFillColorTrend2(9999);
    jopTrend11.setDrawFill(1);
    jopTrend11.setDrawBorder(1);
    jopTrend11.setTrendBorderWidth(1);
    jopTrend11.setLineWidth(1);
    jopTrend11.setMinValue1(0F);
    jopTrend11.setMaxValue1(100F);
    jopTrend11.setMinValue2(0F);
    jopTrend11.setMaxValue2(100F);
    jopTrend11.setNoOfPoints(200);
    jopTrend11.setHorizontalLines(4);
    jopTrend11.setVerticalLines(9);
    jopTrend11.setRotate(0);
    localPanel.add(jopTrend11, new Proportion(jopTrend11.getBounds(), dsize));
    jopBar12 = new JopBar(session);
    jopBar12.setBounds(new Rectangle(291,178,17,151));
    jopBar12.setFillColor(59);
    jopBar12.setBorderColor(56);
    jopBar12.setFillColorBar(116);
    jopBar12.setBorderColorBar(110);
    jopBar12.setDrawFill(1);
    jopBar12.setDrawBorder(1);
    jopBar12.setBarBorderWidth(3);
    jopBar12.setLineWidth(1);
    jopBar12.setMinValue(0F);
    jopBar12.setMaxValue(100F);
    jopBar12.setRotate(0);
    localPanel.add(jopBar12, new Proportion(jopBar12.getBounds(), dsize));
    pwr_framethin13 = new pwr_framethin(session);
    pwr_framethin13.setBounds(new Rectangle(286,172,26,164));
    localPanel.add(pwr_framethin13, new Proportion(pwr_framethin13.getBounds(), dsize));
    jopBar14 = new JopBar(session);
    jopBar14.setBounds(new Rectangle(272,11,17,151));
    jopBar14.setFillColor(29);
    jopBar14.setBorderColor(56);
    jopBar14.setFillColorBar(177);
    jopBar14.setBorderColorBar(172);
    jopBar14.setDrawFill(1);
    jopBar14.setDrawBorder(1);
    jopBar14.setBarBorderWidth(3);
    jopBar14.setLineWidth(1);
    jopBar14.setMinValue(0F);
    jopBar14.setMaxValue(100F);
    jopBar14.setRotate(0);
    localPanel.add(jopBar14, new Proportion(jopBar14.getBounds(), dsize));
    pwr_valuemedium15 = new pwr_valuemedium(session);
    pwr_valuemedium15.setBounds(new Rectangle(83,8,98,24));
    pwr_valuemedium15.setFillColor(161);
    pwr_valuemedium15.setBorderColor(56);
    localPanel.add(pwr_valuemedium15, new Proportion(pwr_valuemedium15.getBounds(), dsize));
    pwr_indroundlarge26 = new pwr_indroundlarge(session);
    pwr_indroundlarge26.setBounds(new Rectangle(119,288,23,23));
    localPanel.add(pwr_indroundlarge26, new Proportion(pwr_indroundlarge26.getBounds(), dsize));
    pwr_indroundlarge28 = new pwr_indroundlarge(session);
    pwr_indroundlarge28.setBounds(new Rectangle(119,312,23,23));
    localPanel.add(pwr_indroundlarge28, new Proportion(pwr_indroundlarge28.getBounds(), dsize));
    pwr_valuemedium29 = new pwr_valuemedium(session);
    pwr_valuemedium29.setBounds(new Rectangle(83,337,98,24));
    pwr_valuemedium29.setFillColor(51);
    pwr_valuemedium29.setBorderColor(56);
    localPanel.add(pwr_valuemedium29, new Proportion(pwr_valuemedium29.getBounds(), dsize));
    pwr_valuesmall31 = new pwr_valuesmall(session);
    pwr_valuesmall31.setBounds(new Rectangle(219,26,41,18));
    pwr_valuesmall31.setFillColor(51);
    pwr_valuesmall31.setBorderColor(41);
    localPanel.add(pwr_valuesmall31, new Proportion(pwr_valuesmall31.getBounds(), dsize));
    pwr_valuesmall32 = new pwr_valuesmall(session);
    pwr_valuesmall32.setBounds(new Rectangle(219,195,41,18));
    pwr_valuesmall32.setFillColor(51);
    pwr_valuesmall32.setBorderColor(41);
    localPanel.add(pwr_valuesmall32, new Proportion(pwr_valuesmall32.getBounds(), dsize));
    pwr_buttoncommand33 = new pwr_buttoncommand(session);
    pwr_buttoncommand33.setBounds(new Rectangle(221,338,72,27));
    pwr_buttoncommand33.setColorTone(1);
    localPanel.add(pwr_buttoncommand33, new Proportion(pwr_buttoncommand33.getBounds(), dsize));
    pwr_indroundlarge35 = new pwr_indroundlarge(session);
    pwr_indroundlarge35.setBounds(new Rectangle(119,264,23,23));
    localPanel.add(pwr_indroundlarge35, new Proportion(pwr_indroundlarge35.getBounds(), dsize));
    pwr_valueinputmedium36 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium36.setBounds(new Rectangle(83,31,98,24));
    pwr_valueinputmedium36.setFillColor(281);
    pwr_valueinputmedium36.setBorderColor(57);
    localPanel.add(pwr_valueinputmedium36, new Proportion(pwr_valueinputmedium36.getBounds(), dsize));
    pwr_valueinputmedium37 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium37.setBounds(new Rectangle(83,53,98,24));
    pwr_valueinputmedium37.setFillColor(101);
    pwr_valueinputmedium37.setBorderColor(57);
    localPanel.add(pwr_valueinputmedium37, new Proportion(pwr_valueinputmedium37.getBounds(), dsize));
    pwr_valueinputmedium38 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium38.setBounds(new Rectangle(83,121,98,24));
    pwr_valueinputmedium38.setBorderColor(57);
    localPanel.add(pwr_valueinputmedium38, new Proportion(pwr_valueinputmedium38.getBounds(), dsize));
    pwr_valueinputmedium39 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium39.setBounds(new Rectangle(83,144,98,24));
    pwr_valueinputmedium39.setBorderColor(57);
    localPanel.add(pwr_valueinputmedium39, new Proportion(pwr_valueinputmedium39.getBounds(), dsize));
    pwr_valueinputmedium40 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium40.setBounds(new Rectangle(83,167,98,24));
    pwr_valueinputmedium40.setBorderColor(57);
    localPanel.add(pwr_valueinputmedium40, new Proportion(pwr_valueinputmedium40.getBounds(), dsize));
    pwr_valueinputmedium41 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium41.setBounds(new Rectangle(83,190,98,24));
    pwr_valueinputmedium41.setBorderColor(57);
    localPanel.add(pwr_valueinputmedium41, new Proportion(pwr_valueinputmedium41.getBounds(), dsize));
    pwr_valueinputmedium42 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium42.setBounds(new Rectangle(83,212,98,24));
    pwr_valueinputmedium42.setBorderColor(57);
    localPanel.add(pwr_valueinputmedium42, new Proportion(pwr_valueinputmedium42.getBounds(), dsize));
    pwr_valueinputmedium43 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium43.setBounds(new Rectangle(83,235,98,24));
    pwr_valueinputmedium43.setBorderColor(57);
    localPanel.add(pwr_valueinputmedium43, new Proportion(pwr_valueinputmedium43.getBounds(), dsize));
    pwr_valueinputreliefup44 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup44.setBounds(new Rectangle(218,171,42,19));
    pwr_valueinputreliefup44.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup44, new Proportion(pwr_valueinputreliefup44.getBounds(), dsize));
    pwr_valueinputreliefup45 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup45.setBounds(new Rectangle(218,318,42,19));
    pwr_valueinputreliefup45.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup45, new Proportion(pwr_valueinputreliefup45.getBounds(), dsize));
    pwr_valueinputreliefup46 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup46.setBounds(new Rectangle(330,332,42,19));
    pwr_valueinputreliefup46.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup46, new Proportion(pwr_valueinputreliefup46.getBounds(), dsize));
    pwr_valueinputreliefup47 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup47.setBounds(new Rectangle(218,5,42,19));
    pwr_valueinputreliefup47.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup47, new Proportion(pwr_valueinputreliefup47.getBounds(), dsize));
    pwr_valueinputreliefup48 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup48.setBounds(new Rectangle(218,151,42,19));
    pwr_valueinputreliefup48.setColorShift(1);
    localPanel.add(pwr_valueinputreliefup48, new Proportion(pwr_valueinputreliefup48.getBounds(), dsize));
    pwr_valuemedium0.dd.setDynType(1024);
    pwr_valuemedium0.dd.setActionType(0);
    pwr_valuemedium0.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium0.dd, "$object.OutChange##Float32","%10.3f")
      });
    pwr_valuemedium2.dd.setDynType(1024);
    pwr_valuemedium2.dd.setActionType(0);
    pwr_valuemedium2.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium2.dd, "$object.ControlDiff##Float32","%10.3f")
      });
    jopBar4.setPwrAttribute("$object.SetVal##Float32");
    jopTrend5.setPwrAttribute1("$object.SetVal##Float32");
    jopTrend5.setPwrAttribute2("$object.ProcVal##Float32");
    pwr_framethin6.dd.setDynType(1);
    pwr_framethin6.dd.setActionType(0);
    pwr_framethin7.dd.setDynType(1);
    pwr_framethin7.dd.setActionType(0);
    pwr_framethin8.dd.setDynType(1);
    pwr_framethin8.dd.setActionType(0);
    jopButtontoggle10.dd.setDynType(0);
    jopButtontoggle10.dd.setActionType(4);
    jopButtontoggle10.dd.setAccess(65535);
    jopButtontoggle10.dd.setElements(new GeDynElemIfc[] {
      new GeDynSetDig(jopButtontoggle10.dd, "$local.TrendHold##Boolean")
      });
    jopButtontoggle10.setAnnot1("   Hold");
    jopTrend11.setPwrAttribute1("$object.OutVal##Float32");
    jopBar12.setPwrAttribute("$object.OutVal##Float32");
    pwr_framethin13.dd.setDynType(1);
    pwr_framethin13.dd.setActionType(0);
    jopBar14.setPwrAttribute("$object.ProcVal##Float32");
    pwr_valuemedium15.dd.setDynType(1024);
    pwr_valuemedium15.dd.setActionType(0);
    pwr_valuemedium15.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium15.dd, "$object.ProcVal##Float32","%10.3f")
      });
    pwr_indroundlarge26.dd.setDynType(7);
    pwr_indroundlarge26.dd.setActionType(0);
    pwr_indroundlarge26.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge26.dd, "! $object.IntOff##Boolean",10)
      });
    pwr_indroundlarge28.dd.setDynType(7);
    pwr_indroundlarge28.dd.setActionType(0);
    pwr_indroundlarge28.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge28.dd, "! $object.Force##Boolean",10)
      });
    pwr_valuemedium29.dd.setDynType(1024);
    pwr_valuemedium29.dd.setActionType(0);
    pwr_valuemedium29.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuemedium29.dd, "$object.PidAlg##Enum","%10s")
      });
    pwr_valuesmall31.dd.setDynType(1025);
    pwr_valuesmall31.dd.setActionType(0);
    pwr_valuesmall31.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall31.dd, "$object.SetEngUnit##String80","%s")
      });
    pwr_valuesmall32.dd.setDynType(1025);
    pwr_valuesmall32.dd.setActionType(0);
    pwr_valuesmall32.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall32.dd, "$object.OutEngUnit##String80","%s")
      });
    pwr_buttoncommand33.dd.setDynType(1);
    pwr_buttoncommand33.dd.setActionType(64);
    pwr_buttoncommand33.dd.setAccess(65535);
    pwr_buttoncommand33.dd.setElements(new GeDynElemIfc[] {
      new GeDynCommand(pwr_buttoncommand33.dd, "open graph/classgraph/inst=&$object.ModeObjDid")
      });
    pwr_buttoncommand33.setAnnot1("    Mode");
    pwr_indroundlarge35.dd.setDynType(7);
    pwr_indroundlarge35.dd.setActionType(0);
    pwr_indroundlarge35.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge35.dd, "$object.Intverse##Boolean",10)
      });
    pwr_valueinputmedium36.dd.setDynType(1024);
    pwr_valueinputmedium36.dd.setActionType(4096);
    pwr_valueinputmedium36.dd.setAccess(22);
    pwr_valueinputmedium36.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium36.dd, "$object.SetVal##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmedium36.dd, 0,0)
      });
    pwr_valueinputmedium37.dd.setDynType(1024);
    pwr_valueinputmedium37.dd.setActionType(4096);
    pwr_valueinputmedium37.dd.setAccess(22);
    pwr_valueinputmedium37.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium37.dd, "$object.OutVal##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmedium37.dd, 0,0)
      });
    pwr_valueinputmedium38.dd.setDynType(1024);
    pwr_valueinputmedium38.dd.setActionType(4096);
    pwr_valueinputmedium38.dd.setAccess(22);
    pwr_valueinputmedium38.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium38.dd, "$object.Gain##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmedium38.dd, 0,0)
      });
    pwr_valueinputmedium39.dd.setDynType(1024);
    pwr_valueinputmedium39.dd.setActionType(4096);
    pwr_valueinputmedium39.dd.setAccess(22);
    pwr_valueinputmedium39.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium39.dd, "$object.IntTime##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmedium39.dd, 0,0)
      });
    pwr_valueinputmedium40.dd.setDynType(1024);
    pwr_valueinputmedium40.dd.setActionType(4096);
    pwr_valueinputmedium40.dd.setAccess(22);
    pwr_valueinputmedium40.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium40.dd, "$object.DerTime##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmedium40.dd, 0,0)
      });
    pwr_valueinputmedium41.dd.setDynType(1024);
    pwr_valueinputmedium41.dd.setActionType(4096);
    pwr_valueinputmedium41.dd.setAccess(22);
    pwr_valueinputmedium41.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium41.dd, "$object.DerGain##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmedium41.dd, 0,0)
      });
    pwr_valueinputmedium42.dd.setDynType(1024);
    pwr_valueinputmedium42.dd.setActionType(4096);
    pwr_valueinputmedium42.dd.setAccess(22);
    pwr_valueinputmedium42.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium42.dd, "$object.BiasGain##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmedium42.dd, 0,0)
      });
    pwr_valueinputmedium43.dd.setDynType(1024);
    pwr_valueinputmedium43.dd.setActionType(4096);
    pwr_valueinputmedium43.dd.setAccess(22);
    pwr_valueinputmedium43.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium43.dd, "$object.Bias##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmedium43.dd, 0,0)
      });
    pwr_valueinputreliefup44.dd.setDynType(1024);
    pwr_valueinputreliefup44.dd.setActionType(4096);
    pwr_valueinputreliefup44.dd.setAccess(22);
    pwr_valueinputreliefup44.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup44.dd, "$object.OutMaxShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup44.dd, 0,0)
      });
    pwr_valueinputreliefup45.dd.setDynType(1024);
    pwr_valueinputreliefup45.dd.setActionType(4096);
    pwr_valueinputreliefup45.dd.setAccess(22);
    pwr_valueinputreliefup45.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup45.dd, "$object.OutMinShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup45.dd, 0,0)
      });
    pwr_valueinputreliefup46.dd.setDynType(1024);
    pwr_valueinputreliefup46.dd.setActionType(4096);
    pwr_valueinputreliefup46.dd.setAccess(65535);
    pwr_valueinputreliefup46.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup46.dd, "$local.ScanTime##Float32","%5.0f")
      ,new GeDynValueInput(pwr_valueinputreliefup46.dd, 4,100000)
      });
    pwr_valueinputreliefup47.dd.setDynType(1024);
    pwr_valueinputreliefup47.dd.setActionType(4096);
    pwr_valueinputreliefup47.dd.setAccess(22);
    pwr_valueinputreliefup47.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup47.dd, "$object.SetMaxShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup47.dd, 0,0)
      });
    pwr_valueinputreliefup48.dd.setDynType(1024);
    pwr_valueinputreliefup48.dd.setActionType(4096);
    pwr_valueinputreliefup48.dd.setAccess(22);
    pwr_valueinputreliefup48.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup48.dd, "$object.SetMinShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup48.dd, 0,0)
      });
    engine.setFrameReady();
  }

class LocalPanel extends JPanel {
  public LocalPanel() {}
  int fillColor = 9999;
  int originalFillColor = 9999;
  int textColor = 9999;
  int originalTextColor = 9999;
  int borderColor = 9999;
  int colorTone = 0;
  int originalColorTone = 0;
  int colorShift = 0;
  int originalColorShift = 0;
  int colorBrightness = 0;
  int originalColorBrightness = 0;
  int colorIntensity = 0;
  int originalColorIntensity = 0;
  int colorInverse = 0;
  int originalColorInverse = 0;
  int shadow = 0;
  boolean dimmed = false;
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
  public void resetFillColor() {
    fillColor = originalFillColor;
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
  public int original_width = 839;
  public int original_height = 369;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
  };
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
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    double scaleWidth =  (1.0*width/original_width);
    double scaleHeight = (1.0*height/original_height);
    AffineTransform save = g.getTransform();
    g.setColor(getBackground());
    g.fill(new Rectangle(0,0,getWidth(),getHeight()));
    g.transform( AffineTransform.getScaleInstance( scaleWidth, scaleHeight)); // scaletest
    AffineTransform save_tmp;
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Outx",13, 96);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Error",13, 118);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 10));
    g.drawString( "0",821, 344);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Proc",13, 27);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Set",13, 50);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Out",13, 73);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Kp",13, 142);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Ti",13, 165);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Td",13, 188);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Dg",13, 210);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Bg",13, 233);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Bias",13, 256);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Integration",13, 307);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Control",13, 332);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "PidAlg",13, 356);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( "Inverse",13, 282);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valueinputreliefup extends GeTextField {
  public pwr_valueinputreliefup( JopSession session)
  {
    super( session);
    setFont( annotFont);
    setFillColor( annotBackground);
  }
  int original_width = 50;
  int original_height = 18;
  boolean fontSet = false;
  Font annotFont = new Font("Helvetica", Font.BOLD, 10);
  int annotBackground = 41;
  public void paintComponent(Graphics g1) {
    if ( !fontSet) {
      float width = getWidth();
      float height = getHeight();
      setFont( annotFont.deriveFont((float)(height / original_height * annotFont.getSize())));
      fontSet = true;
    }
    super.paintComponent( g1);
  }
}
protected class pwr_indroundlarge extends GeComponent {
  Dimension size;
  public pwr_indroundlarge( JopSession session)
  {
    super( session);
    size = new Dimension( 61, 62);
  }
  public int original_width = 61;
  public int original_height = 62;
  Shape[] shapes = new Shape[] { 
    new Arc2D.Float(2F, 2F, 57.6909F, 58.703F, 0F, 360F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 57.6909F, 58.703F, 35F, 140F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 57.6909F, 58.703F, 215F, 140F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 57.6909F, 58.703F, -5F, 40F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 57.6909F, 58.703F, 175F, 40F, Arc2D.PIE),
    new Arc2D.Float(4.88455F, 4.88455F, 51.9218F, 52.9339F, 0F, 360F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 57.6909F, 58.703F, 0F, 360F, Arc2D.PIE),
    new Polygon( new int[] { 7, 6, 11, 20, 34, 41, 26, 15, 11, 11}, new int[] {24, 33, 44, 53, 55, 51, 47, 37, 25, 16}, 10),
    new Polygon( new int[] { 7, 10, 8, 6}, new int[] { 24, 25, 34,33}, 4),
    new Polygon( new int[] { 6, 8, 13, 11}, new int[] { 33, 34, 44,44}, 4),
    new Polygon( new int[] { 11, 13, 20, 20}, new int[] { 44, 44, 52,53}, 4),
    new Polygon( new int[] { 20, 20, 34, 34}, new int[] { 53, 52, 54,55}, 4),
    new Polygon( new int[] { 34, 34, 35, 41}, new int[] { 55, 54, 53,51}, 4),
    new Polygon( new int[] { 41, 35, 25, 26}, new int[] { 51, 53, 50,47}, 4),
    new Polygon( new int[] { 26, 25, 13, 15}, new int[] { 47, 50, 39,37}, 4),
    new Polygon( new int[] { 15, 13, 9, 11}, new int[] { 37, 39, 27,25}, 4),
    new Polygon( new int[] { 11, 9, 9, 11}, new int[] { 25, 27, 28,16}, 4),
    new Polygon( new int[] { 37, 35, 41, 48, 40}, new int[] {13, 18, 22, 19, 11}, 5),
    new Polygon( new int[] { 37, 37, 36, 35}, new int[] { 13, 15, 19,18}, 4),
    new Polygon( new int[] { 35, 36, 40, 41}, new int[] { 18, 19, 22,22}, 4),
    new Polygon( new int[] { 41, 40, 46, 48}, new int[] { 22, 22, 20,19}, 4),
    new Polygon( new int[] { 48, 46, 39, 40}, new int[] { 19, 20, 13,11}, 4),
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
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
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    if ( shadow == 0) {
    ((Arc2D)shapes[0]).setArcType(Arc2D.PIE);
    g.setColor(GeColor.getColor(293, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed));
    g.fill( shapes[0]);
    ((Arc2D)shapes[0]).setArcType(Arc2D.OPEN);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(46, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    } else {
    ((Arc2D)shapes[1]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[2]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[3]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[4]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[5]).setArcType(Arc2D.PIE);
    {
    int fcolor = GeColor.getDrawtype(293, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    g.fill( shapes[4]);
    g.fill( shapes[5]);
    }
    ((Arc2D)shapes[6]).setArcType(Arc2D.OPEN);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(46, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    }
    g.setColor(GeColor.getColor(295, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed));
    g.fill( shapes[7]);
    {
    int fcolor = GeColor.getDrawtype(295, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[8]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[9]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[10]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[11]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[12]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[13]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[14]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[15]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[16]);
    }
    }
    g.setColor(GeColor.getColor(281, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed));
    g.fill( shapes[17]);
    {
    int fcolor = GeColor.getDrawtype(281, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[18]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[19]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[20]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[21]);
    }
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valuemedium extends GeComponent {
  Dimension size;
  public pwr_valuemedium( JopSession session)
  {
    super( session);
    size = new Dimension( 64, 24);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 14);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 64;
  public int original_height = 24;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 60.7273F, 20.2424F),
    new Polygon( new int[] { 2, 63, 62, 3, 3, 2}, new int[] { 2, 2, 3, 3, 21, 22}, 6), 
    new Polygon( new int[] { 63, 63, 62, 62, 3, 2}, new int[] { 22, 2, 3, 21, 21, 22}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
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
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(41, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 9 * original_height / height * width / original_width, 18F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_framethin extends GeFrameThin {
  public pwr_framethin( JopSession session)
  {
     super(session);
  }
}
protected class JopButtontoggle extends GeComponent {
  Dimension size;
  public JopButtontoggle( JopSession session)
  {
    super( session);
    size = new Dimension( 48, 21);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 12);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 48;
  public int original_height = 21;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 44.7945F, 17.2735F),
    new Polygon( new int[] { 2, 47, 46, 3, 3, 2}, new int[] { 2, 2, 3, 3, 18, 19}, 6), 
    new Polygon( new int[] { 47, 47, 46, 46, 3, 2}, new int[] { 19, 2, 3, 18, 18, 19}, 6), 
    new Polygon( new int[] { 2, 5, 5, 44, 47, 2, 2}, new int[] {19, 16, 5, 5, 2, 2, 19}, 7),
    new Polygon( new int[] { 2, 5, 44, 44, 47, 47, 2}, new int[] {19, 16, 16, 5, 2, 19, 19}, 7),
    new Rectangle2D.Float(2.00001F, 2F, 44.7945F, 17.2735F),
    new Polygon( new int[] { 2, 47, 46, 3, 3, 2}, new int[] { 2, 2, 3, 3, 18, 19}, 6), 
    new Polygon( new int[] { 47, 47, 46, 46, 3, 2}, new int[] { 19, 2, 3, 18, 18, 19}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
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
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(102, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
    }
    g.setColor(GeColor.getColor(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed));
    g.fill( shapes[3]);
    g.setColor(GeColor.getColor(104, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed));
    g.fill( shapes[4]);
    {
    int fcolor = GeColor.getDrawtype(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[6]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[7]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 7 * original_height / height * width / original_width, 14F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valuesmall extends GeComponent {
  Dimension size;
  public pwr_valuesmall( JopSession session)
  {
    super( session);
    size = new Dimension( 50, 18);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 10);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 50;
  public int original_height = 18;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 46.5576F, 14.1697F),
    new Polygon( new int[] { 2, 49, 48, 3, 3, 2}, new int[] { 2, 2, 3, 3, 15, 16}, 6), 
    new Polygon( new int[] { 49, 49, 48, 48, 3, 2}, new int[] { 16, 2, 3, 15, 15, 16}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
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
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(41, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 7 * original_height / height * width / original_width, 13F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_buttoncommand extends GeComponent {
  Dimension size;
  public pwr_buttoncommand( JopSession session)
  {
    super( session);
    size = new Dimension( 103, 36);
  }
  String annot1 = new String();
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = s;}
  Font annot1Font = new Font("Helvetica", Font.BOLD, 14);
  int annot1Color = 0;
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public Font getAnnot1Font() { return annot1Font;}
  public int original_width = 103;
  public int original_height = 36;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 99.1879F, 32.3879F),
    new Polygon( new int[] { 2, 101, 100, 4, 4, 2}, new int[] { 2, 2, 4, 4, 33, 34}, 6), 
    new Polygon( new int[] { 101, 101, 100, 100, 4, 2}, new int[] { 34, 2, 4, 33, 33, 34}, 6), 
    new Polygon( new int[] { 2, 8, 8, 95, 101, 2, 2}, new int[] {34, 28, 8, 8, 2, 2, 34}, 7),
    new Polygon( new int[] { 2, 4, 6, 8}, new int[] { 34, 31, 29,28}, 4),
    new Polygon( new int[] { 8, 6, 6, 8}, new int[] { 28, 29, 8,8}, 4),
    new Polygon( new int[] { 8, 6, 94, 95}, new int[] { 8, 8, 8,8}, 4),
    new Polygon( new int[] { 95, 94, 96, 101}, new int[] { 8, 8, 6,2}, 4),
    new Polygon( new int[] { 101, 96, 4, 2}, new int[] { 2, 6, 6,2}, 4),
    new Polygon( new int[] { 2, 4, 4, 2}, new int[] { 2, 6, 31,34}, 4),
    new Polygon( new int[] { 2, 8, 95, 95, 101, 101, 2}, new int[] {34, 28, 28, 8, 2, 34, 34}, 7),
    new Polygon( new int[] { 2, 8, 9, 8}, new int[] { 34, 32, 31,28}, 4),
    new Polygon( new int[] { 8, 9, 97, 95}, new int[] { 28, 31, 31,28}, 4),
    new Polygon( new int[] { 95, 97, 97, 95}, new int[] { 28, 31, 11,8}, 4),
    new Polygon( new int[] { 95, 97, 99, 101}, new int[] { 8, 11, 9,2}, 4),
    new Polygon( new int[] { 101, 99, 99, 101}, new int[] { 2, 9, 32,34}, 4),
    new Polygon( new int[] { 101, 99, 8, 2}, new int[] { 34, 32, 32,34}, 4),
    new Rectangle2D.Float(2F, 2F, 99.1879F, 32.3879F),
    new Polygon( new int[] { 2, 101, 100, 4, 4, 2}, new int[] { 2, 2, 4, 4, 33, 34}, 6), 
    new Polygon( new int[] { 101, 101, 100, 100, 4, 2}, new int[] { 34, 2, 4, 33, 33, 34}, 6), 
  };
  public void paintComponent(Graphics g1) {
    animationCount = 1;
    if ( !visible)
      return;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
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
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    if ( (dd.dynType & dd.mDynType_Rotate) != 0 && dd.rotate != 0) {
      g.rotate( Math.PI * dd.rotate/180, 

           (dd.x0 - getX())*original_width/width,
           (dd.y0 - getY()) * original_height / height);
    }
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    int rounds = 1;
    if ( fillLevel != 1F)
      rounds = 2;
    int oldColor = 0;
    for ( int i = 0; i < rounds; i++) {
      if ( rounds == 2) {
        switch ( i) {
        case 0:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO) {
	    oldColor = colorTone;
	    colorTone = levelColorTone;
	  }
	  else if ( levelFillColor != GeColor.COLOR_NO) {
	    oldColor = fillColor;
	    fillColor = levelFillColor;
	  }
	  break;
        case 1:
	  if ( levelColorTone != GeColor.COLOR_TONE_NO)
	    colorTone = oldColor;
	  else if ( levelFillColor != GeColor.COLOR_NO)
	    fillColor = oldColor;
	  break;
        }
        switch ( levelDirection) {
        case Ge.DIRECTION_UP:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,fillLevel*original_height+Ge.cJBean_Offset,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,fillLevel * original_height+Ge.cJBean_Offset));
	  break;
	case Ge.DIRECTION_DOWN:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,original_width,(1-fillLevel)*original_height+Ge.cJBean_Offset));
	  else
	    g.setClip(new Rectangle2D.Float(0F,(1-fillLevel)*original_height+Ge.cJBean_Offset,original_width,original_height));
	  break;
	case Ge.DIRECTION_RIGHT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(fillLevel*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  else
	    g.setClip(new Rectangle2D.Float(0F,0F,fillLevel*width+Ge.cJBean_Offset,height));
	  break;
	case Ge.DIRECTION_LEFT:
	  if ( i == 0)
	    g.setClip(new Rectangle2D.Float(0F,0F,(1-fillLevel)*original_width+Ge.cJBean_Offset,original_height));
	  else
	    g.setClip(new Rectangle2D.Float((1-fillLevel)*original_width+Ge.cJBean_Offset,0F,original_width,original_height));
	  break;
	}
      }
    {
    int fcolor = GeColor.getDrawtype(102, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
    }
    g.setColor(GeColor.getColor(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed));
    g.fill( shapes[3]);
    {
    int fcolor = GeColor.getDrawtype(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[4]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[5]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[6]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[7]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[8]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[9]);
    }
    }
    g.setColor(GeColor.getColor(104, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed));
    g.fill( shapes[10]);
    {
    int fcolor = GeColor.getDrawtype(104, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[11]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[12]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[13]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[14]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[15]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[16]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[18]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[19]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[17]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 13 * original_height / height * width / original_width, 21F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valueinputmedium extends GeTextField {
  public pwr_valueinputmedium( JopSession session)
  {
    super( session);
    setFont( annotFont);
    setFillColor( annotBackground);
  }
  int original_width = 64;
  int original_height = 24;
  boolean fontSet = false;
  Font annotFont = new Font("Helvetica", Font.BOLD, 14);
  int annotBackground = 41;
  public void paintComponent(Graphics g1) {
    if ( !fontSet) {
      float width = getWidth();
      float height = getHeight();
      setFont( annotFont.deriveFont((float)(height / original_height * annotFont.getSize())));
      fontSet = true;
    }
    super.paintComponent( g1);
  }
}
  public int getUtilityType() {
    return JopUtility.GRAPH;
  }
  public PwrtObjid getUtilityObjid() {
    return utilityObjid;
  }
  public String getUtilityName() {
    return this.getClass().getName();
  }
}
