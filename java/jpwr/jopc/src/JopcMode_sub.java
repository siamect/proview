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

public class JopcMode_sub extends JopFrame implements JopUtilityIfc {
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  public LocalPanel localPanel = new LocalPanel();
  boolean scrollbar = false;
  Dimension size;
  pwr_framethin	pwr_framethin6;
  pwr_framethin	pwr_framethin7;
  JopBar	jopBar8;
  JopBar	jopBar9;
  JopBar	jopBar10;
  JopSmallbutton	jopSmallbutton11;
  JopSmallbutton	jopSmallbutton12;
  pwr_buttoncommand	pwr_buttoncommand13;
  pwr_valuereliefup	pwr_valuereliefup14;
  pwr_indroundlarge	pwr_indroundlarge15;
  pwr_valuereliefup	pwr_valuereliefup16;
  pwr_indroundlarge	pwr_indroundlarge17;
  pwr_valuereliefup	pwr_valuereliefup18;
  pwr_indroundlarge	pwr_indroundlarge19;
  pwr_framethin	pwr_framethin22;
  pwr_valuereliefdown	pwr_valuereliefdown25;
  pwr_valuesmall	pwr_valuesmall26;
  pwr_valuesmall	pwr_valuesmall27;
  pwr_framethin	pwr_framethin31;
  pwr_indroundlarge	pwr_indroundlarge33;
  pwr_indroundlarge	pwr_indroundlarge34;
  pwr_valueinputreliefup	pwr_valueinputreliefup35;
  pwr_valueinputreliefup	pwr_valueinputreliefup36;
  pwr_valueinputreliefup	pwr_valueinputreliefup37;
  pwr_valueinputreliefup	pwr_valueinputreliefup38;
  pwr_sliderbackground1	pwr_sliderbackground140;
  pwr_slider1	pwr_slider141;
  pwr_sliderbackground1	pwr_sliderbackground142;
  pwr_slider1	pwr_slider143;
  pwr_valueinputmediumcenter	pwr_valueinputmediumcenter44;
  pwr_valueinputmediumcenter	pwr_valueinputmediumcenter45;
  pwr_valueinputmediumcenter	pwr_valueinputmediumcenter46;
  public JopcMode_sub( JopSession session, String instance, boolean scrollbar) {
    super( session, instance);
    this.scrollbar = scrollbar;
    geInit();
  }
  public JopcMode_sub( JopSession session, String instance, boolean scrollbar, boolean noinit) {
    super( session, instance);
    this.scrollbar = scrollbar;
    if ( !noinit)
      geInit();
  }
  public void geInit() {
    JopSpider.setSystemName( "ä£·D‘ú·D‘ú·p—uÄŸ¢·{¨· s¨·È‡Æ¿çéŸ·D‘ú·D‘ú·x—u s¨·");
    engine.setAnimationScanTime( 500);
    engine.setScanTime( 500);
    size = new Dimension( 307, 338);
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
      this.setTitle("JopcMode_sub");
    pwr_framethin6 = new pwr_framethin(session);
    pwr_framethin6.setBounds(new Rectangle(4,5,96,111));
    localPanel.add(pwr_framethin6, new Proportion(pwr_framethin6.getBounds(), dsize));
    pwr_framethin7 = new pwr_framethin(session);
    pwr_framethin7.setBounds(new Rectangle(105,6,187,282));
    localPanel.add(pwr_framethin7, new Proportion(pwr_framethin7.getBounds(), dsize));
    jopBar8 = new JopBar(session);
    jopBar8.setBounds(new Rectangle(169,44,15,186));
    jopBar8.setFillColor(59);
    jopBar8.setBorderColor(56);
    jopBar8.setFillColorBar(296);
    jopBar8.setBorderColorBar(291);
    jopBar8.setDrawFill(1);
    jopBar8.setDrawBorder(1);
    jopBar8.setBarBorderWidth(3);
    jopBar8.setLineWidth(1);
    jopBar8.setMinValue(0F);
    jopBar8.setMaxValue(100F);
    jopBar8.setRotate(0);
    localPanel.add(jopBar8, new Proportion(jopBar8.getBounds(), dsize));
    jopBar9 = new JopBar(session);
    jopBar9.setBounds(new Rectangle(191,44,15,186));
    jopBar9.setFillColor(59);
    jopBar9.setBorderColor(56);
    jopBar9.setFillColorBar(177);
    jopBar9.setBorderColorBar(172);
    jopBar9.setDrawFill(1);
    jopBar9.setDrawBorder(1);
    jopBar9.setBarBorderWidth(3);
    jopBar9.setLineWidth(1);
    jopBar9.setMinValue(0F);
    jopBar9.setMaxValue(100F);
    jopBar9.setRotate(0);
    localPanel.add(jopBar9, new Proportion(jopBar9.getBounds(), dsize));
    jopBar10 = new JopBar(session);
    jopBar10.setBounds(new Rectangle(211,44,15,186));
    jopBar10.setFillColor(59);
    jopBar10.setBorderColor(56);
    jopBar10.setFillColorBar(116);
    jopBar10.setBorderColorBar(110);
    jopBar10.setDrawFill(1);
    jopBar10.setDrawBorder(1);
    jopBar10.setBarBorderWidth(3);
    jopBar10.setLineWidth(1);
    jopBar10.setMinValue(0F);
    jopBar10.setMaxValue(100F);
    jopBar10.setRotate(0);
    localPanel.add(jopBar10, new Proportion(jopBar10.getBounds(), dsize));
    jopSmallbutton11 = new JopSmallbutton(session);
    jopSmallbutton11.setBounds(new Rectangle(108,9,42,16));
    jopSmallbutton11.setColorTone(1);
    localPanel.add(jopSmallbutton11, new Proportion(jopSmallbutton11.getBounds(), dsize));
    jopSmallbutton12 = new JopSmallbutton(session);
    jopSmallbutton12.setBounds(new Rectangle(248,10,41,16));
    jopSmallbutton12.setColorTone(1);
    localPanel.add(jopSmallbutton12, new Proportion(jopSmallbutton12.getBounds(), dsize));
    pwr_buttoncommand13 = new pwr_buttoncommand(session);
    pwr_buttoncommand13.setBounds(new Rectangle(15,254,72,27));
    pwr_buttoncommand13.setColorTone(1);
    localPanel.add(pwr_buttoncommand13, new Proportion(pwr_buttoncommand13.getBounds(), dsize));
    pwr_valuereliefup14 = new pwr_valuereliefup(session);
    pwr_valuereliefup14.setBounds(new Rectangle(18,65,55,17));
    localPanel.add(pwr_valuereliefup14, new Proportion(pwr_valuereliefup14.getBounds(), dsize));
    pwr_indroundlarge15 = new pwr_indroundlarge(session);
    pwr_indroundlarge15.setBounds(new Rectangle(77,65,16,16));
    localPanel.add(pwr_indroundlarge15, new Proportion(pwr_indroundlarge15.getBounds(), dsize));
    pwr_valuereliefup16 = new pwr_valuereliefup(session);
    pwr_valuereliefup16.setBounds(new Rectangle(18,39,55,17));
    localPanel.add(pwr_valuereliefup16, new Proportion(pwr_valuereliefup16.getBounds(), dsize));
    pwr_indroundlarge17 = new pwr_indroundlarge(session);
    pwr_indroundlarge17.setBounds(new Rectangle(77,40,16,16));
    localPanel.add(pwr_indroundlarge17, new Proportion(pwr_indroundlarge17.getBounds(), dsize));
    pwr_valuereliefup18 = new pwr_valuereliefup(session);
    pwr_valuereliefup18.setBounds(new Rectangle(18,90,55,17));
    localPanel.add(pwr_valuereliefup18, new Proportion(pwr_valuereliefup18.getBounds(), dsize));
    pwr_indroundlarge19 = new pwr_indroundlarge(session);
    pwr_indroundlarge19.setBounds(new Rectangle(77,92,16,16));
    localPanel.add(pwr_indroundlarge19, new Proportion(pwr_indroundlarge19.getBounds(), dsize));
    pwr_framethin22 = new pwr_framethin(session);
    pwr_framethin22.setBounds(new Rectangle(4,129,96,69));
    localPanel.add(pwr_framethin22, new Proportion(pwr_framethin22.getBounds(), dsize));
    pwr_valuereliefdown25 = new pwr_valuereliefdown(session);
    pwr_valuereliefdown25.setBounds(new Rectangle(18,165,61,20));
    localPanel.add(pwr_valuereliefdown25, new Proportion(pwr_valuereliefdown25.getBounds(), dsize));
    pwr_valuesmall26 = new pwr_valuesmall(session);
    pwr_valuesmall26.setBounds(new Rectangle(136,231,35,17));
    pwr_valuesmall26.setFillColor(51);
    pwr_valuesmall26.setBorderColor(42);
    localPanel.add(pwr_valuesmall26, new Proportion(pwr_valuesmall26.getBounds(), dsize));
    pwr_valuesmall27 = new pwr_valuesmall(session);
    pwr_valuesmall27.setBounds(new Rectangle(224,231,35,17));
    pwr_valuesmall27.setFillColor(51);
    pwr_valuesmall27.setBorderColor(42);
    localPanel.add(pwr_valuesmall27, new Proportion(pwr_valuesmall27.getBounds(), dsize));
    pwr_framethin31 = new pwr_framethin(session);
    pwr_framethin31.setBounds(new Rectangle(4,209,96,34));
    localPanel.add(pwr_framethin31, new Proportion(pwr_framethin31.getBounds(), dsize));
    pwr_indroundlarge33 = new pwr_indroundlarge(session);
    pwr_indroundlarge33.setBounds(new Rectangle(74,217,16,16));
    localPanel.add(pwr_indroundlarge33, new Proportion(pwr_indroundlarge33.getBounds(), dsize));
    pwr_indroundlarge34 = new pwr_indroundlarge(session);
    pwr_indroundlarge34.setBounds(new Rectangle(56,217,16,16));
    localPanel.add(pwr_indroundlarge34, new Proportion(pwr_indroundlarge34.getBounds(), dsize));
    pwr_valueinputreliefup35 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup35.setBounds(new Rectangle(136,28,33,14));
    localPanel.add(pwr_valueinputreliefup35, new Proportion(pwr_valueinputreliefup35.getBounds(), dsize));
    pwr_valueinputreliefup36 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup36.setBounds(new Rectangle(136,214,33,14));
    localPanel.add(pwr_valueinputreliefup36, new Proportion(pwr_valueinputreliefup36.getBounds(), dsize));
    pwr_valueinputreliefup37 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup37.setBounds(new Rectangle(227,28,33,14));
    localPanel.add(pwr_valueinputreliefup37, new Proportion(pwr_valueinputreliefup37.getBounds(), dsize));
    pwr_valueinputreliefup38 = new pwr_valueinputreliefup(session);
    pwr_valueinputreliefup38.setBounds(new Rectangle(227,214,33,14));
    localPanel.add(pwr_valueinputreliefup38, new Proportion(pwr_valueinputreliefup38.getBounds(), dsize));
    pwr_sliderbackground140 = new pwr_sliderbackground1(session);
    pwr_sliderbackground140.setBounds(new Rectangle(112,27,24,220));
    pwr_sliderbackground140.setColorBrightness(2);
    localPanel.add(pwr_sliderbackground140, new Proportion(pwr_sliderbackground140.getBounds(), dsize));
    pwr_slider141 = new pwr_slider1(session);
    pwr_slider141.setBounds(new Rectangle(116,37,20,27));
    pwr_slider141.setColorShift(7);
    localPanel.add(pwr_slider141, new Proportion(pwr_slider141.getBounds(), dsize));
    pwr_sliderbackground142 = new pwr_sliderbackground1(session);
    pwr_sliderbackground142.setBounds(new Rectangle(260,27,24,220));
    pwr_sliderbackground142.setColorBrightness(2);
    localPanel.add(pwr_sliderbackground142, new Proportion(pwr_sliderbackground142.getBounds(), dsize));
    pwr_slider143 = new pwr_slider1(session);
    pwr_slider143.setBounds(new Rectangle(264,37,20,27));
    pwr_slider143.setColorShift(8);
    pwr_slider143.setColorIntensity(1);
    localPanel.add(pwr_slider143, new Proportion(pwr_slider143.getBounds(), dsize));
    pwr_valueinputmediumcenter44 = new pwr_valueinputmediumcenter(session);
    pwr_valueinputmediumcenter44.setBounds(new Rectangle(108,264,60,21));
    pwr_valueinputmediumcenter44.setFillColor(271);
    pwr_valueinputmediumcenter44.setBorderColor(28);
    localPanel.add(pwr_valueinputmediumcenter44, new Proportion(pwr_valueinputmediumcenter44.getBounds(), dsize));
    pwr_valueinputmediumcenter45 = new pwr_valueinputmediumcenter(session);
    pwr_valueinputmediumcenter45.setBounds(new Rectangle(169,264,60,21));
    pwr_valueinputmediumcenter45.setFillColor(151);
    pwr_valueinputmediumcenter45.setBorderColor(28);
    localPanel.add(pwr_valueinputmediumcenter45, new Proportion(pwr_valueinputmediumcenter45.getBounds(), dsize));
    pwr_valueinputmediumcenter46 = new pwr_valueinputmediumcenter(session);
    pwr_valueinputmediumcenter46.setBounds(new Rectangle(230,264,60,21));
    pwr_valueinputmediumcenter46.setFillColor(110);
    pwr_valueinputmediumcenter46.setBorderColor(28);
    localPanel.add(pwr_valueinputmediumcenter46, new Proportion(pwr_valueinputmediumcenter46.getBounds(), dsize));
    pwr_framethin6.dd.setDynType(1);
    pwr_framethin6.dd.setActionType(0);
    pwr_framethin7.dd.setDynType(1);
    pwr_framethin7.dd.setActionType(0);
    jopBar8.setPwrAttribute("$object.SetVal##Float32");
    jopBar8.setMinValueAttr("$object.SetMinShow##Float32");
    jopBar8.setMaxValueAttr("$object.SetMaxShow##Float32");
    jopBar9.setPwrAttribute("$object.ProcVal##Float32");
    jopBar9.setMinValueAttr("$object.SetMinShow##Float32");
    jopBar9.setMaxValueAttr("$object.SetMaxShow##Float32");
    jopBar10.setPwrAttribute("$object.OutVal##Float32");
    jopBar10.setMinValueAttr("$object.OutMinShow##Float32");
    jopBar10.setMaxValueAttr("$object.OutMaxShow##Float32");
    jopSmallbutton11.dd.setDynType(4);
    jopSmallbutton11.dd.setActionType(16);
    jopSmallbutton11.dd.setAccess(-2147483626);
    jopSmallbutton11.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(jopSmallbutton11.dd, "!$local.SetSliderEnable##Boolean",102)
      ,new GeDynToggleDig(jopSmallbutton11.dd, "$local.SetSliderEnable##Boolean")
      });
    jopSmallbutton11.setAnnot1("Slider");
    jopSmallbutton12.dd.setDynType(4);
    jopSmallbutton12.dd.setActionType(16);
    jopSmallbutton12.dd.setAccess(-2147483626);
    jopSmallbutton12.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(jopSmallbutton12.dd, "!$local.OutSliderEnable##Boolean",102)
      ,new GeDynToggleDig(jopSmallbutton12.dd, "$local.OutSliderEnable##Boolean")
      });
    jopSmallbutton12.setAnnot1("Slider");
    pwr_buttoncommand13.dd.setDynType(1);
    pwr_buttoncommand13.dd.setActionType(64);
    pwr_buttoncommand13.dd.setAccess(65535);
    pwr_buttoncommand13.dd.setElements(new GeDynElemIfc[] {
      new GeDynCommand(pwr_buttoncommand13.dd, "open graph/classgraph/inst=&($object.PIDObjDid)")
      });
    pwr_buttoncommand13.setAnnot1("     PID");
    pwr_valuereliefup14.dd.setDynType(128);
    pwr_valuereliefup14.dd.setActionType(2097152);
    pwr_valuereliefup14.dd.setAccess(-2147483626);
    pwr_valuereliefup14.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_valuereliefup14.dd, "!$object.AccMod##Bit#32[1]",1)
      ,new GeDynSetValue(pwr_valuereliefup14.dd, "$object.OpMod##Enum","2")
      });
    pwr_valuereliefup14.setAnnot1("  Auto");
    pwr_indroundlarge15.dd.setDynType(7);
    pwr_indroundlarge15.dd.setActionType(0);
    pwr_indroundlarge15.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge15.dd, "$object.AutMode##Boolean",10)
      });
    pwr_valuereliefup16.dd.setDynType(128);
    pwr_valuereliefup16.dd.setActionType(2097152);
    pwr_valuereliefup16.dd.setAccess(-2147483626);
    pwr_valuereliefup16.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_valuereliefup16.dd, "!$object.AccMod##Bit#32[0]",1)
      ,new GeDynSetValue(pwr_valuereliefup16.dd, "$object.OpMod##Enum","1")
      });
    pwr_valuereliefup16.setAnnot1("  Man");
    pwr_indroundlarge17.dd.setDynType(7);
    pwr_indroundlarge17.dd.setActionType(0);
    pwr_indroundlarge17.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge17.dd, "$object.ManMode##Boolean",10)
      });
    pwr_valuereliefup18.dd.setDynType(128);
    pwr_valuereliefup18.dd.setActionType(2097152);
    pwr_valuereliefup18.dd.setAccess(-2147483626);
    pwr_valuereliefup18.dd.setElements(new GeDynElemIfc[] {
      new GeDynInvisible(pwr_valuereliefup18.dd, "!$object.AccMod##Bit#32[2]",1)
      ,new GeDynSetValue(pwr_valuereliefup18.dd, "$object.OpMod##Enum","4")
      });
    pwr_valuereliefup18.setAnnot1("Cascade");
    pwr_indroundlarge19.dd.setDynType(7);
    pwr_indroundlarge19.dd.setActionType(0);
    pwr_indroundlarge19.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge19.dd, "$object.CascMod##Boolean",10)
      });
    pwr_framethin22.dd.setDynType(1);
    pwr_framethin22.dd.setActionType(0);
    pwr_valuereliefdown25.dd.setDynType(1025);
    pwr_valuereliefdown25.dd.setActionType(0);
    pwr_valuereliefdown25.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuereliefdown25.dd, "$object.XSetVal##Float32","%10.3f")
      });
    pwr_valuesmall26.dd.setDynType(1025);
    pwr_valuesmall26.dd.setActionType(0);
    pwr_valuesmall26.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall26.dd, "$object.SetEngUnit##String80","%s")
      });
    pwr_valuesmall27.dd.setDynType(1025);
    pwr_valuesmall27.dd.setActionType(0);
    pwr_valuesmall27.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valuesmall27.dd, "$object.OutEngUnit##String80","%s")
      });
    pwr_framethin31.dd.setDynType(1);
    pwr_framethin31.dd.setActionType(0);
    pwr_indroundlarge33.dd.setDynType(7);
    pwr_indroundlarge33.dd.setActionType(0);
    pwr_indroundlarge33.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge33.dd, "$object.Forc2##Boolean",10)
      });
    pwr_indroundlarge34.dd.setDynType(7);
    pwr_indroundlarge34.dd.setActionType(0);
    pwr_indroundlarge34.dd.setElements(new GeDynElemIfc[] {
      new GeDynDigLowColor(pwr_indroundlarge34.dd, "$object.Forc1##Boolean",10)
      });
    pwr_valueinputreliefup35.dd.setDynType(1024);
    pwr_valueinputreliefup35.dd.setActionType(4096);
    pwr_valueinputreliefup35.dd.setAccess(-2147483626);
    pwr_valueinputreliefup35.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup35.dd, "$object.SetMaxShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup35.dd, 0,0,null,null)
      });
    pwr_valueinputreliefup35.setAnnot1Font(pwr_valueinputreliefup35.annotFont.deriveFont((float)9.18128));
    pwr_valueinputreliefup36.dd.setDynType(1024);
    pwr_valueinputreliefup36.dd.setActionType(4096);
    pwr_valueinputreliefup36.dd.setAccess(-2147483626);
    pwr_valueinputreliefup36.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup36.dd, "$object.SetMinShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup36.dd, 0,0,null,null)
      });
    pwr_valueinputreliefup36.setAnnot1Font(pwr_valueinputreliefup36.annotFont.deriveFont((float)9.18128));
    pwr_valueinputreliefup37.dd.setDynType(1024);
    pwr_valueinputreliefup37.dd.setActionType(4096);
    pwr_valueinputreliefup37.dd.setAccess(-2147483626);
    pwr_valueinputreliefup37.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup37.dd, "$object.OutMaxShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup37.dd, 0,0,null,null)
      });
    pwr_valueinputreliefup37.setAnnot1Font(pwr_valueinputreliefup37.annotFont.deriveFont((float)9.18128));
    pwr_valueinputreliefup38.dd.setDynType(1024);
    pwr_valueinputreliefup38.dd.setActionType(4096);
    pwr_valueinputreliefup38.dd.setAccess(-2147483626);
    pwr_valueinputreliefup38.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputreliefup38.dd, "$object.OutMinShow##Float32","%5.1f")
      ,new GeDynValueInput(pwr_valueinputreliefup38.dd, 0,0,null,null)
      });
    pwr_valueinputreliefup38.setAnnot1Font(pwr_valueinputreliefup38.annotFont.deriveFont((float)9.18128));
    pwr_sliderbackground140.dd.setDynType(524289);
    pwr_sliderbackground140.dd.setActionType(0);
    pwr_slider141.dd.setDynType(1);
    pwr_slider141.dd.setActionType(2048);
    pwr_slider141.dd.setAccess(-2147483626);
    pwr_slider141.dd.setElements(new GeDynElemIfc[] {
      new GeDynSlider(pwr_slider141.dd, "$object.SetVal##Float32",0,1,4,34.8699,221.653,"$object.MinSet##Float32","$object.MaxSet##Float32","!$local.SetSliderEnable##Boolean",295,290.017)
      });
    pwr_sliderbackground142.dd.setDynType(524289);
    pwr_sliderbackground142.dd.setActionType(0);
    pwr_slider143.dd.setDynType(1);
    pwr_slider143.dd.setActionType(2048);
    pwr_slider143.dd.setAccess(-2147483626);
    pwr_slider143.dd.setElements(new GeDynElemIfc[] {
      new GeDynSlider(pwr_slider143.dd, "$object.ForcVal##Float32",0,100,4,34.8699,221.653,"$object.MinOut##Float32","$object.MaxOut##Float32","!$local.OutSliderEnable##Boolean",295,290.017)
      });
    pwr_valueinputmediumcenter44.dd.setDynType(1025);
    pwr_valueinputmediumcenter44.dd.setActionType(4096);
    pwr_valueinputmediumcenter44.dd.setAccess(-2113929194);
    pwr_valueinputmediumcenter44.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmediumcenter44.dd, "$object.SetVal##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmediumcenter44.dd, 0,0,"$object.MinSet##Float32","$object.MaxSet##Float32")
      });
    pwr_valueinputmediumcenter44.setAnnot1Font(pwr_valueinputmediumcenter44.annotFont.deriveFont((float)13.583));
    pwr_valueinputmediumcenter45.dd.setDynType(1025);
    pwr_valueinputmediumcenter45.dd.setActionType(0);
    pwr_valueinputmediumcenter45.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmediumcenter45.dd, "$object.ProcVal##Float32","%10.3f")
      });
    pwr_valueinputmediumcenter45.setAnnot1Font(pwr_valueinputmediumcenter45.annotFont.deriveFont((float)13.583));
    pwr_valueinputmediumcenter46.dd.setDynType(1025);
    pwr_valueinputmediumcenter46.dd.setActionType(4096);
    pwr_valueinputmediumcenter46.dd.setAccess(-2113929194);
    pwr_valueinputmediumcenter46.dd.setElements(new GeDynElemIfc[] {
      new GeDynValue(pwr_valueinputmediumcenter46.dd, "$object.ForcVal##Float32","%10.3f")
      ,new GeDynValueInput(pwr_valueinputmediumcenter46.dd, 0,0,"$object.MinOut##Float32","$object.MaxOut##Float32")
      });
    pwr_valueinputmediumcenter46.setAnnot1Font(pwr_valueinputmediumcenter46.annotFont.deriveFont((float)13.583));
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
  public int original_width = 299;
  public int original_height = 294;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Line2D.Float( 291.052F, 230.459F, 106.194F, 230.459F),
    new Line2D.Float( 291.052F, 193.213F, 106.194F, 193.213F),
    new Line2D.Float( 291.052F, 155.967F, 106.194F, 155.967F),
    new Line2D.Float( 291.052F, 118.721F, 106.194F, 118.721F),
    new Line2D.Float( 291.052F, 81.4752F, 106.194F, 81.4752F),
    new Line2D.Float( 290.975F, 44.2291F, 106.117F, 44.2291F),
    new Line2D.Float( 99.1015F, 31.5921F, 5.98649F, 31.5921F),
    new Line2D.Float( 99.1015F, 154.637F, 5.98649F, 154.637F),
    new Line2D.Float( 291.052F, 247.841F, 106.194F, 247.841F),
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
    g.setFont(new Font("Helvetica", Font.BOLD, 12));
    g.drawString( JopLang.transl("Mode"),13, 23);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[6]);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( JopLang.transl("Ext SetValue"),13, 147);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(108, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[7]);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 10));
    g.drawString( JopLang.transl("Proc value"),174, 262);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 10));
    g.drawString( JopLang.transl("Out value"),234, 262);
    g.setStroke( new BasicStroke(1F, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    g.setColor(GeColor.getColor(40, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, dimmed));
    g.draw( shapes[8]);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 12));
    g.drawString( JopLang.transl("Force"),13, 230);
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, textColor, dimmed));
    g.setFont(new Font("Helvetica", Font.PLAIN, 10));
    g.drawString( JopLang.transl("Set value"),113, 262);
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
    size = new Dimension( 60, 61);
  }
  public int original_width = 60;
  public int original_height = 61;
  Shape[] shapes = new Shape[] { 
    new Arc2D.Float(2F, 2F, 56.8074F, 57.8041F, 35F, 140F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 56.8074F, 57.8041F, 215F, 140F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 56.8074F, 57.8041F, -5F, 40F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 56.8074F, 57.8041F, 175F, 40F, Arc2D.PIE),
    new Arc2D.Float(4.84037F, 4.84037F, 51.1267F, 52.1233F, 0F, 360F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 56.8074F, 57.8041F, 0F, 360F, Arc2D.PIE),
    new Polygon( new int[] { 7, 6, 11, 19, 34, 40, 25, 15, 11, 10}, new int[] {23, 32, 43, 52, 54, 50, 46, 36, 25, 16}, 10),
    new Polygon( new int[] { 7, 11, 10, 6}, new int[] { 23, 24, 32,32}, 4),
    new Polygon( new int[] { 6, 10, 15, 11}, new int[] { 32, 32, 42,43}, 4),
    new Polygon( new int[] { 11, 15, 23, 19}, new int[] { 43, 42, 50,52}, 4),
    new Polygon( new int[] { 19, 23, 34, 34}, new int[] { 52, 50, 52,54}, 4),
    new Polygon( new int[] { 34, 34, 35, 40}, new int[] { 54, 52, 51,50}, 4),
    new Polygon( new int[] { 40, 35, 25, 25}, new int[] { 50, 51, 48,46}, 4),
    new Polygon( new int[] { 25, 25, 15, 15}, new int[] { 46, 48, 37,36}, 4),
    new Polygon( new int[] { 15, 15, 11, 11}, new int[] { 36, 37, 25,25}, 4),
    new Polygon( new int[] { 11, 11, 11, 10}, new int[] { 25, 25, 24,16}, 4),
    new Polygon( new int[] { 10, 11, 11, 23}, new int[] { 16, 24, 24,1}, 4),
    new Polygon( new int[] { 36, 35, 40, 47, 39}, new int[] {12, 17, 21, 18, 11}, 5),
    new Polygon( new int[] { 36, 37, 36, 35}, new int[] { 12, 13, 17,17}, 4),
    new Polygon( new int[] { 35, 36, 40, 40}, new int[] { 17, 17, 20,21}, 4),
    new Polygon( new int[] { 40, 40, 46, 47}, new int[] { 21, 20, 18,18}, 4),
    new Polygon( new int[] { 47, 46, 39, 39}, new int[] { 18, 18, 12,11}, 4),
    new Polygon( new int[] { 39, 39, 37, 23}, new int[] { 11, 12, 13,1}, 4),
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
    GeGradient.paint( g, 9,2,-2,2F,2F,56.8074F,57.8041F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[5]);
    GeGradient.paint( g, gradient,2,-2,2F,2F,56.8074F,57.8041F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[4]);
    }
    } else {
    if ( gradient == GeGradient.eGradient_No) {
    g.setColor(GeColor.getColor( fcolor));
    g.fill( shapes[5]);
    } else {
    GeGradient.paint( g, gradient,2,-2,2F,2F,56.8074F,57.8041F, false,293, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
    g.fill( shapes[5]);
    }
    }
    }
    ((Arc2D)shapes[5]).setArcType(Arc2D.OPEN);
    g.setStroke( new BasicStroke(1F));
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
    GeGradient.paint( g, gradient,2,-2,5.55794F,16.2605F,34.392F,37.6338F, false,295, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    g.setColor(GeColor.shiftColor( fcolor, 2, colorInverse));
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
    GeGradient.paint( g, gradient,2,-2,34.7348F,10.6224F,12.6856F,10.7123F, false,281, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 49, 17);
    annot1Font = new Font("Helvetica", Font.BOLD, 8);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 49;
  public int original_height = 17;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 45.8446F, 13.9527F),
    new Polygon( new int[] { 2, 48, 47, 3, 3, 2}, new int[] { 2, 2, 3, 3, 15, 16}, 6), 
    new Polygon( new int[] { 48, 48, 47, 47, 3, 2}, new int[] { 16, 2, 3, 15, 15, 16}, 6), 
    new Line2D.Float( 47.8446F, 15.9527F, 47.8446F, 2F),
    new Line2D.Float( 47.8446F, 15.9527F, 2F, 15.9527F),
    new Line2D.Float( 47.8446F, 2F, 2F, 2F),
    new Line2D.Float( 2F, 2F, 2F, 15.9527F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,45.8446F,13.9527F, false,41, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 101, 35);
    annot1Font = new Font("Helvetica", Font.BOLD, 12);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 101;
  public int original_height = 35;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 97.6689F, 31.8919F),
    new Polygon( new int[] { 2, 100, 98, 4, 4, 2}, new int[] { 2, 2, 4, 4, 32, 34}, 6), 
    new Polygon( new int[] { 100, 100, 98, 98, 4, 2}, new int[] { 34, 2, 4, 32, 32, 34}, 6), 
    new Polygon( new int[] { 2, 8, 8, 94, 100, 2, 2}, new int[] {34, 28, 8, 8, 2, 2, 34}, 7),
    new Polygon( new int[] { 2, 4, 6, 8}, new int[] { 34, 29, 27,28}, 4),
    new Polygon( new int[] { 8, 6, 6, 8}, new int[] { 28, 27, 6,8}, 4),
    new Polygon( new int[] { 8, 6, 93, 94}, new int[] { 8, 6, 6,8}, 4),
    new Polygon( new int[] { 94, 93, 95, 100}, new int[] { 8, 6, 4,2}, 4),
    new Polygon( new int[] { 100, 95, 4, 2}, new int[] { 2, 4, 4,2}, 4),
    new Polygon( new int[] { 2, 4, 4, 2}, new int[] { 2, 4, 29,34}, 4),
    new Polygon( new int[] { 2, 4, -1219419767, -7}, new int[] { 34, 29, -1219419790,-30}, 4),
    new Polygon( new int[] { 2, 8, 94, 94, 100, 100, 2}, new int[] {34, 28, 28, 8, 2, 34, 34}, 7),
    new Polygon( new int[] { 2, 7, 9, 8}, new int[] { 34, 32, 30,28}, 4),
    new Polygon( new int[] { 8, 9, 96, 94}, new int[] { 28, 30, 30,28}, 4),
    new Polygon( new int[] { 94, 96, 96, 94}, new int[] { 28, 30, 9,8}, 4),
    new Polygon( new int[] { 94, 96, 98, 100}, new int[] { 8, 9, 7,2}, 4),
    new Polygon( new int[] { 100, 98, 98, 100}, new int[] { 2, 7, 32,34}, 4),
    new Polygon( new int[] { 100, 98, 7, 2}, new int[] { 34, 32, 32,34}, 4),
    new Polygon( new int[] { 2, 7, -1219419767, -7}, new int[] { 34, 32, -1219419790,-30}, 4),
    new Rectangle2D.Float(2F, 2F, 97.6689F, 31.8919F),
    new Polygon( new int[] { 2, 100, 98, 4, 4, 2}, new int[] { 2, 2, 4, 4, 32, 34}, 6), 
    new Polygon( new int[] { 100, 100, 98, 98, 4, 2}, new int[] { 34, 2, 4, 32, 32, 34}, 6), 
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,97.6689F,31.8919F, false,102, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,97.6689F,31.8919F, false,100, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,97.6689F,31.8919F, false,104, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
      g.drawString( annot1, 13 * original_height / height * width / original_width, 22F);
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
    size = new Dimension( 49, 17);
    annot1Font = new Font("Helvetica", Font.BOLD, 8);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 49;
  public int original_height = 17;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 45.8446F, 13.9527F),
    new Polygon( new int[] { 2, 48, 47, 3, 3, 2}, new int[] { 2, 2, 3, 3, 15, 16}, 6), 
    new Polygon( new int[] { 48, 48, 47, 47, 3, 2}, new int[] { 16, 2, 3, 15, 15, 16}, 6), 
    new Line2D.Float( 2F, 15.9527F, 2F, 2F),
    new Line2D.Float( 47.8446F, 2F, 2F, 2F),
    new Line2D.Float( 47.8446F, 15.9527F, 2F, 15.9527F),
    new Line2D.Float( 47.8446F, 2F, 47.8446F, 15.9527F),
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,45.8446F,13.9527F, false,41, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
    size = new Dimension( 49, 17);
    annot1Font = new Font("Helvetica", Font.BOLD, 8);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 49;
  public int original_height = 17;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 45.8446F, 13.9527F),
    new Polygon( new int[] { 2, 48, 47, 3, 3, 2}, new int[] { 2, 2, 3, 3, 15, 16}, 6), 
    new Polygon( new int[] { 48, 48, 47, 47, 3, 2}, new int[] { 16, 2, 3, 15, 15, 16}, 6), 
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,45.8446F,13.9527F, false,41, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
  int original_width = 49;
  int original_height = 17;
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
    size = new Dimension( 47, 20);
    annot1Font = new Font("Helvetica", Font.BOLD, 10);
  }
  int annot1Color = 0;
  public String getAnnot1() { return annot1;}
  public void setAnnot1( String s) { annot1 = JopLang.transl(s);}
  public void setAnnot1Font( Font font) { annot1Font = font;}
  public Font getAnnot1Font() { return annot1Font;}
  public void setAnnot1Color( int color) { annot1Color = color;}
  public int original_width = 47;
  public int original_height = 20;
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(2F, 2F, 43.8514F, 16.9426F),
    new Polygon( new int[] { 2, 46, 43, 5, 5, 2}, new int[] { 2, 2, 5, 5, 16, 19}, 6), 
    new Polygon( new int[] { 46, 46, 43, 43, 5, 2}, new int[] { 19, 2, 5, 16, 16, 19}, 6), 
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
    GeGradient.paint( g, gradient,2,-2,2F,2F,43.8514F,16.9426F, false,102, colorTone, colorShift, colorIntensity, colorInverse, fillColor, dimmed);
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
      g.drawString( annot1, 7 * original_height / height * width / original_width, 15F);
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
    size = new Dimension( 43, 223);
  }
  public int original_width = 43;
  public int original_height = 223;
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 2, 42, 22, 2}, new int[] {2, 2, 22, 2}, 4),
    new Polygon( new int[] { 42, 22, 22, 42, 42}, new int[] {2, 22, 201, 221, 2}, 5),
    new Polygon( new int[] { 2, 2, 22, 22, 2}, new int[] {2, 221, 201, 22, 2}, 5),
    new Polygon( new int[] { 2, 22, 42, 2}, new int[] {221, 201, 221, 221}, 4),
    new Rectangle2D.Float(17.9459F, 17.9459F, 7.97297F, 187.365F),
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
    size = new Dimension( 33, 31);
  }
  public int original_width = 33;
  public int original_height = 31;
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 24, 32, 32, 12, 2, 24, 24}, new int[] {2, 24, 30, 30, 22, 22, 2}, 7),
    new Rectangle2D.Float(2F, 2F, 21.9257F, 19.9324F),
    new Rectangle2D.Float(2F, 2F, 21.9257F, 5.97973F),
    new Rectangle2D.Float(2F, 15.9527F, 21.9257F, 5.97973F),
    new Line2D.Float( 23.9257F, 11.3166F, 2F, 11.3166F),
    new Line2D.Float( 23.9257F, 12.3559F, 2F, 12.3559F),
    new Rectangle2D.Float(2F, 2F, 21.9257F, 19.9324F),
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
protected class pwr_valueinputmediumcenter extends GeTextField {
  public pwr_valueinputmediumcenter( JopSession session)
  {
    super( session);
    setFont( annotFont);
    setFillColor( 41);
  }
  int original_width = 63;
  int original_height = 23;
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
