package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import jpwr.jopc.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcMode extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  public LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_sliderbackground1	pwr_sliderbackground16;
  pwr_framethin	pwr_framethin7;
  pwr_framethin	pwr_framethin8;
  pwr_sliderbackground1	pwr_sliderbackground19;
  pwr_slider1	pwr_slider110;
  pwr_slider1	pwr_slider111;
  JopBar	jopBar12;
  JopBar	jopBar13;
  JopBar	jopBar14;
  JopSmallbutton	jopSmallbutton15;
  JopSmallbutton	jopSmallbutton16;
  pwr_buttoncommand	pwr_buttoncommand17;
  pwr_valuereliefup	pwr_valuereliefup18;
  pwr_indroundlarge	pwr_indroundlarge19;
  pwr_valuereliefup	pwr_valuereliefup20;
  pwr_indroundlarge	pwr_indroundlarge21;
  pwr_valuereliefup	pwr_valuereliefup22;
  pwr_indroundlarge	pwr_indroundlarge23;
  pwr_framethin	pwr_framethin26;
  pwr_valuereliefdown	pwr_valuereliefdown29;
  pwr_valuesmall	pwr_valuesmall30;
  pwr_valuesmall	pwr_valuesmall31;
  pwr_framethin	pwr_framethin36;
  pwr_indroundlarge	pwr_indroundlarge38;
  pwr_indroundlarge	pwr_indroundlarge39;
  pwr_valueinputreliefup	pwr_valueinputreliefup40;
  pwr_valueinputreliefup	pwr_valueinputreliefup41;
  pwr_valueinputreliefup	pwr_valueinputreliefup42;
  pwr_valueinputreliefup	pwr_valueinputreliefup43;
  pwr_valueinputmedium	pwr_valueinputmedium44;
  pwr_valueinputmedium	pwr_valueinputmedium45;
  pwr_valueinputmedium	pwr_valueinputmedium46;
  public JopcMode( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcMode( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "ä'¢·D±ù·D±ù·p—uÄ¿¡·›§· “§·(q¦¿ç	Ÿ·D±ù·D±ù·x—u “§·");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 492, 502);
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
      this.setTitle("JopcMode");
    pwr_sliderbackground16 = new pwr_sliderbackground1(session);
    pwr_sliderbackground16.setBounds(new Rectangle(424,39,40,363));
    pwr_sliderbackground16.setColorBrightness(2);
    localPanel.add(pwr_sliderbackground16, new Proportion(pwr_sliderbackground16.getBounds(), dsize));
    pwr_framethin7 = new pwr_framethin(session);
    pwr_framethin7.setBounds(new Rectangle(4,5,158,140));
    localPanel.add(pwr_framethin7, new Proportion(pwr_framethin7.getBounds(), dsize));
    pwr_framethin8 = new pwr_framethin(session);
    pwr_framethin8.setBounds(new Rectangle(168,5,309,444));
    localPanel.add(pwr_framethin8, new Proportion(pwr_framethin8.getBounds(), dsize));
    pwr_sliderbackground19 = new pwr_sliderbackground1(session);
    pwr_sliderbackground19.setBounds(new Rectangle(180,39,40,363));
    pwr_sliderbackground19.setColorBrightness(2);
    localPanel.add(pwr_sliderbackground19, new Proportion(pwr_sliderbackground19.getBounds(), dsize));
    pwr_slider110 = new pwr_slider1(session);
    pwr_slider110.setBounds(new Rectangle(187,57,34,32));
    pwr_slider110.setColorShift(7);
    localPanel.add(pwr_slider110, new Proportion(pwr_slider110.getBounds(), dsize));
    pwr_slider111 = new pwr_slider1(session);
    pwr_slider111.setBounds(new Rectangle(431,57,34,32));
    pwr_slider111.setColorShift(8);
    pwr_slider111.setColorIntensity(1);
    localPanel.add(pwr_slider111, new Proportion(pwr_slider111.getBounds(), dsize));
    jopBar12 = new JopBar(session);
    jopBar12.setBounds(new Rectangle(269,66,28,310));
    jopBar12.setFillColor(59);
    jopBar12.setBorderColor(56);
    jopBar12.setFillColorBar(296);
    jopBar12.setBorderColorBar(291);
    jopBar12.setDrawFill(1);
    jopBar12.setDrawBorder(1);
    jopBar12.setBarBorderWidth(3);
    jopBar12.setLineWidth(1);
    jopBar12.setMinValue(0F);
    jopBar12.setMaxValue(100F);
    jopBar12.setRotate(0);
    localPanel.add(jopBar12, new Proportion(jopBar12.getBounds(), dsize));
    jopBar13 = new JopBar(session);
    jopBar13.setBounds(new Rectangle(310,66,28,310));
    jopBar13.setFillColor(59);
    jopBar13.setBorderColor(56);
    jopBar13.setFillColorBar(177);
    jopBar13.setBorderColorBar(172);
    jopBar13.setDrawFill(1);
    jopBar13.setDrawBorder(1);
    jopBar13.setBarBorderWidth(3);
    jopBar13.setLineWidth(1);
    jopBar13.setMinValue(0F);
    jopBar13.setMaxValue(100F);
    jopBar13.setRotate(0);
    localPanel.add(jopBar13, new Proportion(jopBar13.getBounds(), dsize));
    jopBar14 = new JopBar(session);
    jopBar14.setBounds(new Rectangle(351,66,28,310));
    jopBar14.setFillColor(59);
    jopBar14.setBorderColor(56);
    jopBar14.setFillColorBar(116);
    jopBar14.setBorderColorBar(110);
    jopBar14.setDrawFill(1);
    jopBar14.setDrawBorder(1);
    jopBar14.setBarBorderWidth(3);
    jopBar14.setLineWidth(1);
    jopBar14.setMinValue(0F);
    jopBar14.setMaxValue(100F);
    jopBar14.setRotate(0);
    localPanel.add(jopBar14, new Proportion(jopBar14.getBounds(), dsize));
    jopSmallbutton15 = new JopSmallbutton(session);
    jopSmallbutton15.setBounds(new Rectangle(177,15,56,21));
    jopSmallbutton15.setColorTone(1);
    localPanel.add(jopSmallbutton15, new Proportion(jopSmallbutton15.getBounds(), dsize));
    jopSmallbutton16 = new JopSmallbutton(session);
    jopSmallbutton16.setBounds(new Rectangle(412,15,54,21));
    jopSmallbutton16.setColorTone(1);
    localPanel.add(jopSmallbutton16, new Proportion(jopSmallbutton16.getBounds(), dsize));
    pwr_buttoncommand17 = new pwr_buttoncommand(session);
    pwr_buttoncommand17.setBounds(new Rectangle(8,409,85,33));
    pwr_buttoncommand17.setColorTone(1);
    localPanel.add(pwr_buttoncommand17, new Proportion(pwr_buttoncommand17.getBounds(), dsize));
    pwr_valuereliefup18 = new pwr_valuereliefup(session);
    pwr_valuereliefup18.setBounds(new Rectangle(26,76,71,23));
    localPanel.add(pwr_valuereliefup18, new Proportion(pwr_valuereliefup18.getBounds(), dsize));
    pwr_indroundlarge19 = new pwr_indroundlarge(session);
    pwr_indroundlarge19.setBounds(new Rectangle(115,72,28,27));
    localPanel.add(pwr_indroundlarge19, new Proportion(pwr_indroundlarge19.getBounds(), dsize));
    pwr_valuereliefup20 = new pwr_valuereliefup(session);
    pwr_valuereliefup20.setBounds(new Rectangle(26,43,71,23));
    localPanel.add(pwr_valuereliefup20, new Proportion(pwr_valuereliefup20.getBounds(), dsize));
    pwr_indroundlarge21 = new pwr_indroundlarge(session);
    pwr_indroundlarge21.setBounds(new Rectangle(115,40,28,27));
    localPanel.add(pwr_indroundlarge21, new Proportion(pwr_indroundlarge21.getBounds(), dsize));
    pwr_valuereliefup22 = new pwr_valuereliefup(session);
    pwr_valuereliefup22.setBounds(new Rectangle(26,108,71,23));
    localPanel.add(pwr_valuereliefup22, new Proportion(pwr_valuereliefup22.getBounds(), dsize));
    pwr_indroundlarge23 = new pwr_indroundlarge(session);
    pwr_indroundlarge23.setBounds(new Rectangle(115,105,28,27));
    localPanel.add(pwr_indroundlarge23, new Proportion(pwr_indroundlarge23.getBounds(), dsize));
    pwr_framethin26 = new pwr_framethin(session);
    pwr_framethin26.setBounds(new Rectangle(4,158,158,88));
    localPanel.add(pwr_framethin26, new Proportion(pwr_framethin26.getBounds(), dsize));
    pwr_valuereliefdown29 = new pwr_valuereliefdown(session);
    pwr_valuereliefdown29.setBounds(new Rectangle(26,204,101,26));
    localPanel.add(pwr_valuereliefdown29, new Proportion(pwr_valuereliefdown29.getBounds(), dsize));
    pwr_valuesmall30 = new pwr_valuesmall(session);
    pwr_valuesmall30.setBounds(new Rectangle(228,376,42,21));
    pwr_valuesmall30.setFillColor(51);
    pwr_valuesmall30.setBorderColor(42);
    localPanel.add(pwr_valuesmall30, new Proportion(pwr_valuesmall30.getBounds(), dsize));
    pwr_valuesmall31 = new pwr_valuesmall(session);
    pwr_valuesmall31.setBounds(new Rectangle(380,376,42,21));
    pwr_valuesmall31.setFillColor(51);
    pwr_valuesmall31.setBorderColor(42);
    localPanel.add(pwr_valuesmall31, new Proportion(pwr_valuesmall31.getBounds(), dsize));
    pwr_framethin36 = new pwr_framethin(session);
    pwr_framethin36.setBounds(new Rectangle(4,258,158,44));
    localPanel.add(pwr_framethin36, new Proportion(pwr_framethin36.getBounds(), dsize));
    pwr_indroundlarge38 = new pwr_indroundlarge(session);
    pwr_indroundlarge38.setBounds(new Rectangle(115,266,28,27));
    localPanel.add(pwr_indroundlarge38, new Proportion(pwr_indroundlarge38.getBounds(), dsize));
    pwr_indroundlarge39 = new pwr_indroundlarge(session);
    pwr_indroundlarge39.setBounds(new Rectangle(85,266,28,27));
    localPanel.add(pwr_indroundlarge39, new Proportion(pwr_indroundlarge39.getBounds(), dsize));
    pwr_valueinputreliefup40 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup40.setBounds(new Rectangle(228,50,42,19));
    localPanel.add(pwr_valueinputreliefup40, new Proportion(pwr_valueinputreliefup40.getBounds(), dsize));
    pwr_valueinputreliefup41 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup41.setBounds(new Rectangle(228,356,42,19));
    localPanel.add(pwr_valueinputreliefup41, new Proportion(pwr_valueinputreliefup41.getBounds(), dsize));
    pwr_valueinputreliefup42 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup42.setBounds(new Rectangle(379,50,42,19));
    localPanel.add(pwr_valueinputreliefup42, new Proportion(pwr_valueinputreliefup42.getBounds(), dsize));
    pwr_valueinputreliefup43 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup43.setBounds(new Rectangle(379,356,42,19));
    localPanel.add(pwr_valueinputreliefup43, new Proportion(pwr_valueinputreliefup43.getBounds(), dsize));
    pwr_valueinputmedium44 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium44.setBounds(new Rectangle(173,419,100,24));
    pwr_valueinputmedium44.setFillColor(281);
    pwr_valueinputmedium44.setBorderColor(57);
    localPanel.add(pwr_valueinputmedium44, new Proportion(pwr_valueinputmedium44.getBounds(), dsize));
    pwr_valueinputmedium45 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium45.setBounds(new Rectangle(273,419,100,24));
    pwr_valueinputmedium45.setFillColor(161);
    pwr_valueinputmedium45.setBorderColor(57);
    localPanel.add(pwr_valueinputmedium45, new Proportion(pwr_valueinputmedium45.getBounds(), dsize));
    pwr_valueinputmedium46 = new pwr_valueinputmedium(session);
    pwr_valueinputmedium46.setBounds(new Rectangle(374,419,100,24));
    pwr_valueinputmedium46.setFillColor(101);
    pwr_valueinputmedium46.setBorderColor(57);
    localPanel.add(pwr_valueinputmedium46, new Proportion(pwr_valueinputmedium46.getBounds(), dsize));
    pwr_sliderbackground16.dd.setDynType(524289);
    pwr_sliderbackground16.dd.setActionType(0);
    pwr_framethin7.dd.setDynType(1);
    pwr_framethin7.dd.setActionType(0);
    pwr_framethin8.dd.setDynType(1);
    pwr_framethin8.dd.setActionType(0);
    pwr_sliderbackground19.dd.setDynType(524289);
    pwr_sliderbackground19.dd.setActionType(0);
    pwr_slider110.dd.setDynType(1);
    pwr_slider110.dd.setActionType(2048);
    pwr_slider110.dd.setAccess(-2147483626);
    pwr_slider110.dd.setElements(new GeDynElemIfc[] {
      new GeDynSlider(pwr_slider110.dd, "$object.SetVal##Float32",0,100,4,55.7185,363.011,"$object.MinSet##Float32","$object.MaxSet##Float32",
"!$local.SetSliderEnable##Boolean")
      });
    pwr_slider111.dd.setDynType(1);
    pwr_slider111.dd.setActionType(2048);
    pwr_slider111.dd.setAccess(-2147483626);
    pwr_slider111.dd.setElements(new GeDynElemIfc[] {
      new GeDynSlider(pwr_slider111.dd, "$object.ForcVal##Float32",0,100,4,55.7185,363.011,"$object.MinOut##Float32","$object.MaxOut##Float32",
"!$local.OutSliderEnable##Boolean")
      });
    jopBar12.setPwrAttribute("$object.SetVal##Float32");
    jopBar12.setMinValueAttr("$object.SetMinShow##Float32");
    jopBar12.setMaxValueAttr("$object.SetMaxShow##Float32");
    jopBar13.setPwrAttribute("$object.ProcVal##Float32");
    jopBar13.setMinValueAttr("$object.SetMinShow##Float32");
    jopBar13.setMaxValueAttr("$object.SetMaxShow##Float32");
    jopBar14.setPwrAttribute("$object.OutVal##Float32");
    jopBar14.setMinValueAttr("$object.OutMinShow##Float32");
    jopBar14.setMaxValueAttr("$object.OutMaxShow##Float32");
    jopSmallbutton15.dd.setDynType(4);
    jopSmallbutton15.dd.setActionType(16);
    jopSmallbutton15.dd.setAccess(-2147483626);
    jopSmallbutton15.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(jopSmallbutton15.dd, "!$local.SetSliderEnable##Boolean",102)
      ,new GeDynToggleDig(jopSmallbutton15.dd, "$local.SetSliderEnable##Boolean")
      });
    jopSmallbutton15.setAnnot1("Slider");
    jopSmallbutton16.dd.setDynType(4);
    jopSmallbutton16.dd.setActionType(16);
    jopSmallbutton16.dd.setAccess(-2147483626);
    jopSmallbutton16.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(jopSmallbutton16.dd, "!$local.OutSliderEnable##Boolean",102)
      ,new GeDynToggleDig(jopSmallbutton16.dd, "$local.OutSliderEnable##Boolean")
      });
    jopSmallbutton16.setAnnot1("Slider");
    pwr_buttoncommand17.dd.setDynType(1);
    pwr_buttoncommand17.dd.setActionType(64);
    pwr_buttoncommand17.dd.setAccess(65535);
    pwr_buttoncommand17.dd.setElements(new GeDynElemIfc[] {
      new GeDynCommand(pwr_buttoncommand17.dd, "open graph/classgraph/inst=&$object.PIDObjDid")
      });
    pwr_buttoncommand17.setAnnot1("     PID");
    pwr_valuereliefup18.dd.setDynType(128);
    pwr_valuereliefup18.dd.setActionType(2097152);
    pwr_valuereliefup18.dd.setAccess(-2147483626);
    pwr_valuereliefup18.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_valuereliefup18.dd, "!$object.AccMod##Bit#32[1]",1)
      ,new GeDynSetValue(pwr_valuereliefup18.dd, "$object.OpMod##Enum","2")
      });
    pwr_valuereliefup18.setAnnot1("  Auto");
    pwr_indroundlarge19.dd.setDynType(7);
    pwr_indroundlarge19.dd.setActionType(0);
    pwr_indroundlarge19.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge19.dd, "$object.AutMode##Boolean",10)
      });
    pwr_valuereliefup20.dd.setDynType(128);
    pwr_valuereliefup20.dd.setActionType(2097152);
    pwr_valuereliefup20.dd.setAccess(-2147483626);
    pwr_valuereliefup20.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_valuereliefup20.dd, "!$object.AccMod##Bit#32[0]",1)
      ,new GeDynSetValue(pwr_valuereliefup20.dd, "$object.OpMod##Enum","1")
      });
    pwr_valuereliefup20.setAnnot1("  Man");
    pwr_indroundlarge21.dd.setDynType(7);
    pwr_indroundlarge21.dd.setActionType(0);
    pwr_indroundlarge21.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge21.dd, "$object.ManMode##Boolean",10)
      });
    pwr_valuereliefup22.dd.setDynType(128);
    pwr_valuereliefup22.dd.setActionType(2097152);
    pwr_valuereliefup22.dd.setAccess(-2147483626);
    pwr_valuereliefup22.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_valuereliefup22.dd, "!$object.AccMod##Bit#32[2]",1)
      ,new GeDynSetValue(pwr_valuereliefup22.dd, "$object.OpMod##Enum","4")
      });
    pwr_valuereliefup22.setAnnot1("Cascade");
    pwr_indroundlarge23.dd.setDynType(7);
    pwr_indroundlarge23.dd.setActionType(0);
    pwr_indroundlarge23.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge23.dd, "$object.CascMod##Boolean",10)
      });
    pwr_framethin26.dd.setDynType(1);
    pwr_framethin26.dd.setActionType(0);
    pwr_valuereliefdown29.dd.setDynType(1025);
    pwr_valuereliefdown29.dd.setActionType(0);
    pwr_valuereliefdown29.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuereliefdown29.dd, "$object.XSetVal##Float32","%10.3f")
      });
    pwr_valuesmall30.dd.setDynType(1025);
    pwr_valuesmall30.dd.setActionType(0);
    pwr_valuesmall30.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall30.dd, "$object.SetEngUnit##String80","%s")
      });
    pwr_valuesmall31.dd.setDynType(1025);
    pwr_valuesmall31.dd.setActionType(0);
    pwr_valuesmall31.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall31.dd, "$object.OutEngUnit##String80","%s")
      });
    pwr_framethin36.dd.setDynType(1);
    pwr_framethin36.dd.setActionType(0);
    pwr_indroundlarge38.dd.setDynType(7);
    pwr_indroundlarge38.dd.setActionType(0);
    pwr_indroundlarge38.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge38.dd, "$object.Forc2##Boolean",10)
      });
    pwr_indroundlarge39.dd.setDynType(7);
    pwr_indroundlarge39.dd.setActionType(0);
    pwr_indroundlarge39.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge39.dd, "$object.Forc1##Boolean",10)
      });
    pwr_valueinputreliefup40.dd.setDynType(1024);
    pwr_valueinputreliefup40.dd.setActionType(4096);
    pwr_valueinputreliefup40.dd.setAccess(-2147483626);
    pwr_valueinputreliefup40.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup40.dd, "$object.SetMaxShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup40.dd, 0,0,null,null)
      });
    pwr_valueinputreliefup41.dd.setDynType(1024);
    pwr_valueinputreliefup41.dd.setActionType(4096);
    pwr_valueinputreliefup41.dd.setAccess(-2147483626);
    pwr_valueinputreliefup41.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup41.dd, "$object.SetMinShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup41.dd, 0,0,null,null)
      });
    pwr_valueinputreliefup42.dd.setDynType(1024);
    pwr_valueinputreliefup42.dd.setActionType(4096);
    pwr_valueinputreliefup42.dd.setAccess(-2147483626);
    pwr_valueinputreliefup42.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup42.dd, "$object.OutMaxShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup42.dd, 0,0,null,null)
      });
    pwr_valueinputreliefup43.dd.setDynType(1024);
    pwr_valueinputreliefup43.dd.setActionType(4096);
    pwr_valueinputreliefup43.dd.setAccess(-2147483626);
    pwr_valueinputreliefup43.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup43.dd, "$object.OutMinShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup43.dd, 0,0,null,null)
      });
    pwr_valueinputmedium44.dd.setDynType(1024);
    pwr_valueinputmedium44.dd.setActionType(4096);
    pwr_valueinputmedium44.dd.setAccess(-2147483626);
    pwr_valueinputmedium44.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium44.dd, "$object.SetVal##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmedium44.dd, 0,0,"$object.MinSet##Float32","$object.MaxSet##Float32")
      });
    pwr_valueinputmedium45.dd.setDynType(1024);
    pwr_valueinputmedium45.dd.setActionType(0);
    pwr_valueinputmedium45.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium45.dd, "$object.ProcVal##Float32","%10.3f")
      });
    pwr_valueinputmedium46.dd.setDynType(1024);
    pwr_valueinputmedium46.dd.setActionType(4096);
    pwr_valueinputmedium46.dd.setAccess(-2147483626);
    pwr_valueinputmedium46.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmedium46.dd, "$object.ForcVal##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmedium46.dd, 0,0,"$object.MinOut##Float32","$object.MaxOut##Float32")
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
  public int original_width = 484;
  public int original_height = 458;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Line2D.Float( 475.071F, 374.766F, 170.945F, 374.766F),
    new Line2D.Float( 475.071F, 313.489F, 170.945F, 313.489F),
    new Line2D.Float( 475.071F, 252.213F, 170.945F, 252.213F),
    new Line2D.Float( 475.071F, 190.936F, 170.945F, 190.936F),
    new Line2D.Float( 475.071F, 129.66F, 170.945F, 129.66F),
    new Line2D.Float( 474.945F, 68.383F, 170.818F, 68.383F),
    new Line2D.Float( 159.277F, 37.7447F, 6.08511F, 37.7447F),
    new Line2D.Float( 159.277F, 190.936F, 6.08511F, 190.936F),
    new Line2D.Float( 475.071F, 403.362F, 170.945F, 403.362F),
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
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[0]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[1]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[2]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.BOLD, 14));
    g.drawString( JopLang.transl("Mode"),18, 26);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("Extern SetValue"),17, 180);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[7]);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( JopLang.transl("Set value"),178, 416);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( JopLang.transl("Process value"),276, 416);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( JopLang.transl("Out value"),377, 416);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(40, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[8]);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( JopLang.transl("Force"),30, 284);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_indroundlarge extends GeComponent {
  // Dimension size;
  public pwr_indroundlarge( JopSession session)
  {
    super( session);
    size = new Dimension( 62, 63);
  }
  public int original_width = 62;
  public int original_height = 63;
  Shape[] shapes = new Shape[] { 
    new Arc2D.Float(2F, 2F, 58.2128F, 59.234F, 35F, 140F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 58.2128F, 59.234F, 215F, 140F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 58.2128F, 59.234F, -5F, 40F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 58.2128F, 59.234F, 175F, 40F, Arc2D.PIE),
    new Arc2D.Float(4.91064F, 4.91064F, 52.3915F, 53.4128F, 0F, 360F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 58.2128F, 59.234F, 0F, 360F, Arc2D.PIE),
    new Polygon( new int[] { 7, 6, 11, 20, 34, 41, 26, 15, 12, 11}, new int[] {24, 33, 44, 53, 55, 51, 47, 37, 25, 17}, 10),
    new Polygon( new int[] { 7, 12, 10, 6}, new int[] { 24, 27, 35,33}, 4),
    new Polygon( new int[] { 6, 10, 16, 11}, new int[] { 33, 35, 46,44}, 4),
    new Polygon( new int[] { 11, 16, 23, 20}, new int[] { 44, 46, 53,53}, 4),
    new Polygon( new int[] { 20, 23, 36, 34}, new int[] { 53, 53, 55,55}, 4),
    new Polygon( new int[] { 34, 36, 38, 41}, new int[] { 55, 55, 54,51}, 4),
    new Polygon( new int[] { 41, 38, 28, 26}, new int[] { 51, 54, 51,47}, 4),
    new Polygon( new int[] { 26, 28, 16, 15}, new int[] { 47, 51, 40,37}, 4),
    new Polygon( new int[] { 15, 16, 12, 12}, new int[] { 37, 40, 28,25}, 4),
    new Polygon( new int[] { 12, 12, 12, 11}, new int[] { 25, 28, 27,17}, 4),
    new Polygon( new int[] { 11, 12, 12, 40}, new int[] { 17, 27, 27,78}, 4),
    new Polygon( new int[] { 37, 36, 41, 49, 40}, new int[] {13, 18, 22, 19, 11}, 5),
    new Polygon( new int[] { 37, 40, 39, 36}, new int[] { 13, 15, 20,18}, 4),
    new Polygon( new int[] { 36, 39, 41, 41}, new int[] { 18, 20, 23,22}, 4),
    new Polygon( new int[] { 41, 41, 47, 49}, new int[] { 22, 23, 21,19}, 4),
    new Polygon( new int[] { 49, 47, 40, 40}, new int[] { 19, 21, 14,11}, 4),
    new Polygon( new int[] { 40, 40, 40, 40}, new int[] { 11, 14, 15,78}, 4),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    ((Arc2D)shapes[0]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[1]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[2]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[3]).setArcType(Arc2D.PIE);
    ((Arc2D)shapes[4]).setArcType(Arc2D.PIE);
    {
    int fcolor = GeColor.getDrawtype(293, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[0]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[2]);
    g.fill( shapes[3]);
    g.fill( shapes[4]);
    } else {
    GeGradient.paint( g, 9,2,-2,2F,2F,58.2128F,59.234F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[5]);
    GeGradient.paint( g, gradient,2,-2,2F,2F,58.2128F,59.234F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[4]);
    }
    } else {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[5]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,58.2128F,59.234F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[5]);
    }
    }
    }
    ((Arc2D)shapes[5]).setArcType(Arc2D.OPEN);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(46, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    {
    int fcolor = GeColor.getDrawtype(295, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[6]);
    } else {
    GeGradient.paint( g, gradient,2,-2,5.64596F,16.6132F,35.2428F,38.5648F, false,295, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[6]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[7]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[8]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[9]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[10]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[11]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[12]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[13]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[14]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[15]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(281, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[17]);
    } else {
    GeGradient.paint( g, gradient,2,-2,35.5446F,10.8357F,12.9994F,10.9773F, false,281, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[17]);
    }
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
protected class pwr_framethin extends GeFrameThin {
  public pwr_framethin( JopSession session)
  {
     super(session);
  }
}
protected class pwr_valuereliefup extends GeComponent {
  // Dimension size;
  public pwr_valuereliefup( JopSession session)
  {
    super( session);
    size = new Dimension( 50, 18);
    annot1Font = new Font("Helvetica", Font.BOLD, 10);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 50;
  public int original_height = 18;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 46.9787F, 14.2979F),
    new Polygon( new int[] { 2, 49, 48, 3, 3, 2}, new int[] { 2, 2, 3, 3, 16, 16}, 6), 
    new Polygon( new int[] { 49, 49, 48, 48, 3, 2}, new int[] { 16, 2, 3, 16, 16, 16}, 6), 
    new Line2D.Float( 48.9787F, 16.2979F, 48.9787F, 2F),
    new Line2D.Float( 48.9787F, 16.2979F, 2F, 16.2979F),
    new Line2D.Float( 48.9787F, 2F, 2F, 2F),
    new Line2D.Float( 2F, 2F, 2F, 16.2979F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,46.9787F,14.2979F, false,41, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
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
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(40, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(40, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_buttoncommand extends GeComponent {
  // Dimension size;
  public pwr_buttoncommand( JopSession session)
  {
    super( session);
    size = new Dimension( 104, 36);
    annot1Font = new Font("Helvetica", Font.BOLD, 14);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 104;
  public int original_height = 36;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 100.085F, 32.6809F),
    new Polygon( new int[] { 2, 102, 100, 4, 4, 2}, new int[] { 2, 2, 4, 4, 33, 35}, 6), 
    new Polygon( new int[] { 102, 102, 100, 100, 4, 2}, new int[] { 35, 2, 4, 33, 33, 35}, 6), 
    new Polygon( new int[] { 2, 8, 8, 96, 102, 2, 2}, new int[] {35, 29, 8, 8, 2, 2, 35}, 7),
    new Polygon( new int[] { 2, 6, 7, 8}, new int[] { 35, 31, 30,29}, 4),
    new Polygon( new int[] { 8, 7, 7, 8}, new int[] { 29, 30, 8,8}, 4),
    new Polygon( new int[] { 8, 7, 95, 96}, new int[] { 8, 8, 8,8}, 4),
    new Polygon( new int[] { 96, 95, 97, 102}, new int[] { 8, 8, 6,2}, 4),
    new Polygon( new int[] { 102, 97, 6, 2}, new int[] { 2, 6, 6,2}, 4),
    new Polygon( new int[] { 2, 6, 6, 2}, new int[] { 2, 6, 31,35}, 4),
    new Polygon( new int[] { 2, 6, 8, 8}, new int[] { 35, 31, 48,45}, 4),
    new Polygon( new int[] { 2, 8, 96, 96, 102, 102, 2}, new int[] {35, 29, 29, 8, 2, 35, 35}, 7),
    new Polygon( new int[] { 2, 8, 9, 8}, new int[] { 35, 35, 33,29}, 4),
    new Polygon( new int[] { 8, 9, 98, 96}, new int[] { 29, 33, 33,29}, 4),
    new Polygon( new int[] { 96, 98, 98, 96}, new int[] { 29, 33, 11,8}, 4),
    new Polygon( new int[] { 96, 98, 100, 102}, new int[] { 8, 11, 9,2}, 4),
    new Polygon( new int[] { 102, 100, 100, 102}, new int[] { 2, 9, 35,35}, 4),
    new Polygon( new int[] { 102, 100, 8, 2}, new int[] { 35, 35, 35,35}, 4),
    new Polygon( new int[] { 2, 8, 8, 8}, new int[] { 35, 35, 48,45}, 4),
    new Rectangle2D.Float(2F, 2F, 100.085F, 32.6809F),
    new Polygon( new int[] { 2, 102, 100, 4, 4, 2}, new int[] { 2, 2, 4, 4, 33, 35}, 6), 
    new Polygon( new int[] { 102, 102, 100, 100, 4, 2}, new int[] { 35, 2, 4, 33, 33, 35}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,100.085F,32.6809F, false,102, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,100.085F,32.6809F, false,100, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[3]);
    }
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
    {
    int fcolor = GeColor.getDrawtype(104, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[11]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,100.085F,32.6809F, false,104, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[11]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[12]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[13]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[14]);
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[15]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[16]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[17]);
    }
    }
    {
    int fcolor = GeColor.getDrawtype(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[20]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[21]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[19]);
    }
    g.setColor(GeColor.getColor( annot1Color , colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont( annot1Font);
    save_tmp = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_OFF);
    g.transform( AffineTransform.getScaleInstance( original_width/width *
      		height/original_height, 1));
    if ( annot1 != null)
      g.drawString( annot1, 12 * original_height / height * width / original_width, 21F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valuereliefdown extends GeComponent {
  // Dimension size;
  public pwr_valuereliefdown( JopSession session)
  {
    super( session);
    size = new Dimension( 50, 18);
    annot1Font = new Font("Helvetica", Font.BOLD, 10);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 50;
  public int original_height = 18;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 46.9787F, 14.2979F),
    new Polygon( new int[] { 2, 49, 48, 3, 3, 2}, new int[] { 2, 2, 3, 3, 16, 16}, 6), 
    new Polygon( new int[] { 49, 49, 48, 48, 3, 2}, new int[] { 16, 2, 3, 16, 16, 16}, 6), 
    new Line2D.Float( 2F, 16.2979F, 2F, 2F),
    new Line2D.Float( 48.9787F, 2F, 2F, 2F),
    new Line2D.Float( 48.9787F, 16.2979F, 2F, 16.2979F),
    new Line2D.Float( 48.9787F, 2F, 48.9787F, 16.2979F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,46.9787F,14.2979F, false,41, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
    }
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
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[3]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(40, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(40, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_valuesmall extends GeComponent {
  // Dimension size;
  public pwr_valuesmall( JopSession session)
  {
    super( session);
    size = new Dimension( 50, 18);
    annot1Font = new Font("Helvetica", Font.BOLD, 10);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 50;
  public int original_height = 18;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 46.9787F, 14.2979F),
    new Polygon( new int[] { 2, 49, 48, 3, 3, 2}, new int[] { 2, 2, 3, 3, 16, 16}, 6), 
    new Polygon( new int[] { 49, 49, 48, 48, 3, 2}, new int[] { 16, 2, 3, 16, 16, 16}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,46.9787F,14.2979F, false,41, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
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
protected class pwr_valueinputreliefup extends GeTextField {
  public pwr_valueinputreliefup( JopSession session)
  {
    super( session);
    setFont( annotFont);
    setFillColor( 41);
  }
  int original_width = 50;
  int original_height = 18;
  boolean fontSet = false;
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
protected class JopSmallbutton extends GeComponent {
  // Dimension size;
  public JopSmallbutton( JopSession session)
  {
    super( session);
    size = new Dimension( 48, 21);
    annot1Font = new Font("Helvetica", Font.BOLD, 12);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 48;
  public int original_height = 21;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 44.9362F, 17.3617F),
    new Polygon( new int[] { 2, 47, 44, 5, 5, 2}, new int[] { 2, 2, 5, 5, 17, 19}, 6), 
    new Polygon( new int[] { 47, 47, 44, 44, 5, 2}, new int[] { 19, 2, 5, 17, 17, 19}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,44.9362F,17.3617F, false,102, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[0]);
    }
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[1]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[2]);
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
      g.drawString( annot1, 6 * original_height / height * width / original_width, 14F);
    g.setTransform( save_tmp);
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_sliderbackground1 extends GeComponent {
  // Dimension size;
  public pwr_sliderbackground1( JopSession session)
  {
    super( session);
    size = new Dimension( 44, 228);
  }
  public int original_width = 44;
  public int original_height = 228;
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 2, 43, 22, 2}, new int[] {2, 2, 22, 2}, 4),
    new Polygon( new int[] { 43, 22, 22, 43, 43}, new int[] {2, 22, 206, 227, 2}, 5),
    new Polygon( new int[] { 2, 2, 22, 22, 2}, new int[] {2, 227, 206, 22, 2}, 5),
    new Polygon( new int[] { 2, 22, 43, 2}, new int[] {227, 206, 227, 227}, 4),
    new Rectangle2D.Float(18.3404F, 18.3404F, 8.17021F, 192F),
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(43, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(46, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[1]);
    }
    {
    int fcolor = GeColor.getDrawtype(42, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[2]);
    }
    {
    int fcolor = GeColor.getDrawtype(43, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    }
    {
    int fcolor = GeColor.getDrawtype(28, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[4]);
    }
    }
    if ( rounds == 2)
      g.setClip(null);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
protected class pwr_slider1 extends GeComponent {
  // Dimension size;
  public pwr_slider1( JopSession session)
  {
    super( session);
    size = new Dimension( 34, 32);
  }
  public int original_width = 34;
  public int original_height = 32;
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 24, 33, 33, 12, 2, 24, 24}, new int[] {2, 24, 31, 31, 22, 22, 2}, 7),
    new Rectangle2D.Float(2F, 2F, 22.4681F, 20.4255F),
    new Rectangle2D.Float(2F, 2F, 22.4681F, 6.12766F),
    new Rectangle2D.Float(2F, 16.2979F, 22.4681F, 6.12766F),
    new Line2D.Float( 24.4681F, 11.5471F, 2F, 11.5471F),
    new Line2D.Float( 24.4681F, 12.6121F, 2F, 12.6121F),
    new Rectangle2D.Float(2F, 2F, 22.4681F, 20.4255F),
    new Polygon( new int[] { 2, 24, 23, 3, 3, 2}, new int[] { 2, 2, 3, 3, 21, 22}, 6), 
    new Polygon( new int[] { 24, 24, 23, 23, 3, 2}, new int[] { 22, 2, 3, 21, 21, 22}, 6), 
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
    if ( (dd.dynType & GeDyn.mDynType_Rotate) != 0 && dd.rotate != 0) {
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
    int fcolor = GeColor.getDrawtype(28, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[0]);
    }
    {
    int fcolor = GeColor.getDrawtype(102, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[1]);
    }
    {
    int fcolor = GeColor.getDrawtype(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[2]);
    }
    {
    int fcolor = GeColor.getDrawtype(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[3]);
    }
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[4]);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(100, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[5]);
    {
    int fcolor = GeColor.getDrawtype(28, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, dimmed);
    if ( shadow != 0) {
    g.setColor(GeColor.shiftColor( fcolor, -2, colorInverse));
    g.fill( shapes[7]);
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
    g.fill( shapes[8]);
    }
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(29, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    }
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
    setFillColor( 41);
  }
  int original_width = 65;
  int original_height = 24;
  boolean fontSet = false;
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
    if ( utilityAref != null)
      return utilityAref.getObjid();
    else
      return null;
  }
  public PwrtAttrRef getUtilityAttrRef() {
    return utilityAref;
  }
  public String getUtilityName() {
    return this.getClass().getName();
  }
}
