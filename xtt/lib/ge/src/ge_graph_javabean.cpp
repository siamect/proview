/* ge_graph_javabean.cpp -- export javabean

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "co_ccm_msg.h"
#include "co_dcli.h"
#include "co_cnf.h"
#include "rt_gdh.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget.h"

#include "ge_graph.h"

#define glow_cJBean_Offset 2

static char *cmd_cnv( char *instr);

int Graph::export_javabean( char *filename, char *bean_name)
{
  ofstream fp;
  char fname[120];
  char beaninfo_filename[120];
  char *s;
  double x0, x1, y0, y1;
  graph_eTrace trace_type;
  glow_eDrawType trace_color;
  glow_eDrawType trace_color2;
  char low_bean_name[80];
  int	*numbers;
  int	annot_cnt;
  int	i;

  cdh_ToLower( low_bean_name, bean_name);

  grow_GetSubGraphTraceType( grow->ctx, (glow_eTraceType *) &trace_type);
  grow_GetSubGraphTraceColor( grow->ctx, &trace_color, &trace_color2);
  if ( grow_IsSlider( grow->ctx))
    trace_type = graph_eTrace_Slider;

  grow_GetSubgraphAnnotNumbers( grow->ctx, &numbers, &annot_cnt);

  grow_MeasureJavaBean( grow->ctx, &x1, &x0, &y1, &y0);

  if ( !strchr( filename, ':') && !strchr( filename, '/'))
  {
    strcpy( fname, default_path);
    strcat( fname, filename);
  }
  fp.open( fname);

  switch ( trace_type)
  {
    case graph_eTrace_SetDig:
    case graph_eTrace_ResetDig:
    case graph_eTrace_ToggleDig:
    case graph_eTrace_SetDigWithTone:
    case graph_eTrace_ResetDigWithTone:
    case graph_eTrace_ToggleDigWithTone:
    case graph_eTrace_Command:
    case graph_eTrace_DigWithCommand:
    case graph_eTrace_DigToneWithCommand:
    {
      if ( strncmp( bean_name, "Jop", 3) == 0)
        fp << 
"package jpwr.beans;" << endl;

      fp << 
"import jpwr.rt.*;" << endl <<
"import jpwr.jop.*;" << endl <<
"import java.awt.*;" << endl <<
"import java.awt.geom.*;" << endl <<
"import java.awt.image.*;" << endl <<
"import java.awt.font.*;" << endl <<
"import javax.swing.*;" << endl <<
"import javax.swing.Timer;" << endl <<
"import java.awt.event.*;" << endl << endl <<
"public class " << bean_name << " extends JComponent implements JopDynamic, JopConfirm, ActionListener{" << endl <<
"  Dimension size;" << endl <<
"  Object root;" << endl <<
"  JopEngine en;" << endl;
      switch ( trace_type)
      {
        case graph_eTrace_SetDig:
        case graph_eTrace_SetDigWithTone:
          fp <<
"  int clickAction = Jop.BUTTON_ACTION_SET;" << endl;
          break;
        case graph_eTrace_ResetDig:
        case graph_eTrace_ResetDigWithTone:
          fp <<
"  int clickAction = Jop.BUTTON_ACTION_RESET;" << endl;
          break;
        case graph_eTrace_ToggleDig:
        case graph_eTrace_ToggleDigWithTone:
          fp <<
"  int clickAction = Jop.BUTTON_ACTION_TOGGLE;" << endl;
          break;
        case graph_eTrace_Command:
        case graph_eTrace_DigWithCommand:
        case graph_eTrace_DigToneWithCommand:
          fp <<
"  int clickAction = Jop.BUTTON_ACTION_COMMAND;" << endl;
          break;
        default:
         ;
      }
      fp <<

"  public int getClickAction() { return clickAction;}" << endl <<
"  public void setClickAction(int clickAction) { this.clickAction = clickAction;}" << endl <<
"  String command = new String();" << endl <<
"  public String getCommand() { return command;}" << endl <<
"  public void setCommand( String command) { this.command = command;}" << endl <<
"  String confirmText = new String();" << endl <<
"  public String getConfirmText() { return confirmText;}" << endl <<
"  public void setConfirmText( String confirmText) { this.confirmText = confirmText;}" << endl <<
"  boolean confirm = false;" << endl <<
"  public boolean getConfirm() { return confirm;}" << endl <<
"  public void setConfirm( boolean confirm) { this.confirm = confirm;}" << endl <<
"  Timer timer = new Timer(500, this);" << endl <<
"  public " << bean_name << " component = this;" << endl <<
"  public " << bean_name << "()" << endl <<
"  {" << endl <<
"    try {" << endl <<
"      jbInit();" << endl <<
"    }" << endl <<
"    catch(Exception e) {" << endl <<
"      e.printStackTrace();" << endl <<
"    }" << endl <<
"  }" << endl <<
endl <<
"  private void jbInit() throws Exception {" << endl <<
"    size = new Dimension( " << int(x1-x0) + 2*glow_cJBean_Offset << ", " << int(y1-y0) + 2*glow_cJBean_Offset << ");" << endl <<
"    timer.start();" << endl <<
"  }" << endl <<
"  public void actionPerformed(ActionEvent e) {" << endl <<
"    boolean engine_found = false;" << endl <<
"    Container parent = getParent();" << endl <<
"    while ( parent != null) {" << endl <<
"      if ( parent instanceof JopFrame) {" << endl <<
"        en = ((JopFrame)parent).engine;" << endl <<
"        if ( !en.isReady())" << endl <<
"          break;" << endl <<
"        en.add(this);" << endl <<
"        root = parent;" << endl <<
"        engine_found = true;" << endl <<
"        break;" << endl <<
"      }" << endl <<
"      parent = parent.getParent();" << endl <<
"    }" << endl <<
"    if ( !engine_found) {" << endl <<
"      parent = getParent();" << endl <<
"      while ( parent != null) {" << endl <<
"        if ( parent instanceof JopApplet) {" << endl <<
"          en = ((JopApplet)parent).engine;" << endl <<
"          if ( !en.isReady())" << endl <<
"            break;" << endl <<
"          en.add(this);" << endl <<
"          root = parent;" << endl <<
"          engine_found = true;" << endl <<
"          break;" << endl <<
"        }" << endl <<
"        parent = parent.getParent();" << endl <<
"      }" << endl <<
"    }" << endl <<
"    if ( engine_found) {" << endl <<
"      timer.stop();" << endl <<
"      timer = null;" << endl <<
"      if ( en.gdh.isAuthorized( access)) {" << endl <<
"        this.addMouseListener(new MouseAdapter() {" << endl <<
"          public void mouseClicked(MouseEvent e) {" << endl <<
"            PwrtStatus sts;" << endl <<
"            if ( confirm) {" << endl <<
"              JopConfirmDialog.open( component, confirmText);" << endl <<
"            }" << endl <<
"            else if (clickAction == Jop.BUTTON_ACTION_SET) {" << endl <<
"              sts = en.gdh.setObjectInfo( pwrAttribute, true);" << endl <<
"              if ( sts.evenSts())" << endl <<
"                System.out.println( \"setObjectInfoError \" + sts);" << endl <<
"            }" << endl <<
"            else if(clickAction == Jop.BUTTON_ACTION_RESET) {" << endl <<
"              sts = en.gdh.setObjectInfo( pwrAttribute, false);" << endl <<
"              if ( sts.evenSts())" << endl <<
"                System.out.println( \"setObjectInfoError \" + sts);" << endl <<
"            }" << endl <<
"            else if(clickAction == Jop.BUTTON_ACTION_TOGGLE) {" << endl <<
"              sts = en.gdh.toggleObjectInfo(pwrAttribute);" << endl <<
"              if ( sts.evenSts())" << endl <<
"                System.out.println( \"setObjectInfoError \" + sts);" << endl <<
"            }" << endl <<
"            else if(clickAction == Jop.BUTTON_ACTION_COMMAND) {" << endl <<
"              Jop.executeCommand( root, command);" << endl <<
"            }" << endl <<
"          }" << endl <<
"          public void mousePressed(MouseEvent e) {" << endl <<
"            colorInverse = 1;" << endl <<
"	     repaint();" << endl <<
"          }" << endl <<
"          public void mouseReleased(MouseEvent e) {" << endl <<
"            colorInverse = 0;" << endl <<
"	     repaint();" << endl <<
"          }" << endl <<
"        });" << endl <<
"      }" << endl <<
"    }" << endl <<
"  }" << endl <<
"  public void confirmYes() {" << endl <<
"    PwrtStatus sts;" << endl <<
"    switch ( clickAction) {" << endl <<
"      case Jop.BUTTON_ACTION_SET:" << endl <<
"        sts = en.gdh.setObjectInfo( pwrAttribute, true);" << endl <<
"        if ( sts.evenSts())" << endl <<
"          System.out.println( \"setObjectInfoError \" + sts);" << endl <<
"        break;" << endl <<
"      case Jop.BUTTON_ACTION_RESET:" << endl <<
"        sts = en.gdh.setObjectInfo( pwrAttribute, false);" << endl <<
"        if ( sts.evenSts())" << endl <<
"          System.out.println( \"setObjectInfoError \" + sts);" << endl <<
"        break;" << endl <<
"      case Jop.BUTTON_ACTION_TOGGLE:" << endl <<
"        sts = en.gdh.toggleObjectInfo( pwrAttribute);" << endl <<
"        if ( sts.evenSts())" << endl <<
"          System.out.println( \"setObjectInfoError \" + sts);" << endl <<
"        break;" << endl <<
"      case Jop.BUTTON_ACTION_COMMAND:" << endl <<
"        if ( command.compareTo(\"\") != 0)" << endl <<
"          Jop.executeCommand( root, command);" << endl <<
"        break;" << endl <<
"    }" << endl <<
"  }" << endl;

      grow_ExportJavaBean( grow->ctx, fp, 0);
  
      fp <<
"  public Dimension getPreferredSize() { return size;}" << endl <<
"  public Dimension getMinimumSize() { return size;}" << endl;
      switch ( trace_type)
      {
        case graph_eTrace_SetDig:
        case graph_eTrace_ResetDig:
        case graph_eTrace_ToggleDig:
        case graph_eTrace_DigWithCommand:
        case graph_eTrace_Command:
          fp <<
"  int lowColor = " << (int) trace_color << ";" << endl <<
"  public void setLowColor( int lowColor) { this.lowColor = lowColor;}" << endl <<
"  public int getLowColor() { return lowColor;}" << endl;

          break;
        case graph_eTrace_SetDigWithTone:
        case graph_eTrace_ResetDigWithTone:
        case graph_eTrace_ToggleDigWithTone:
        case graph_eTrace_DigToneWithCommand:
          fp <<

"  int lowTone = " << (int) trace_color << ";" << endl <<
"  public void setLowTone( int lowTone) { this.lowTone = lowTone;}" << endl <<
"  public int getLowTone() { return lowTone;}" << endl;

          break;
        default:
          ;
      }
      fp <<
"  private String pwrAttribute = new String();" << endl <<
"  public void setPwrAttribute( String pwrAttribute) { this.pwrAttribute = pwrAttribute;}" << endl <<
"  public String getPwrAttribute() { return pwrAttribute;}" << endl <<
"  private int access = 65532;" << endl <<
"  public void setAccess( int access) { this.access = access;}" << endl <<
"  public int getAccess() { return access;}" << endl <<
"  String pwrAttrColor = new String();" << endl <<
"  public void setPwrAttrColor( String pwrAttrColor) { this.pwrAttrColor = pwrAttrColor;}" << endl <<
"  public String getPwrAttrColor() { return pwrAttrColor;}" << endl <<
"  String pwrAttrText = new String();" << endl <<
"  public void setPwrAttrText( String pwrAttrText) { this.pwrAttrText = pwrAttrText;}" << endl <<
"  public String getPwrAttrText() { return pwrAttrText;}" << endl <<
"  String textLow = new String();" << endl <<
"  public void setTextLow( String textLow) { this.textLow = textLow;}" << endl <<
"  public String getTextLow() { return textLow;}" << endl <<
"  String textHigh = new String();" << endl <<
"  public void setTextHigh( String textHigh) { this.textHigh = textHigh;}" << endl <<
"  public String getTextHigh() { return textHigh;}" << endl <<
"  boolean valueColor;" << endl <<
"  boolean valueColorOld;" << endl <<
"  boolean valueText;" << endl <<
"  boolean valueTextOld;" << endl <<
"  boolean firstScan = true;" << endl <<
"  GdhrRefObjectInfo retColor = null;" << endl <<
"  GdhrRefObjectInfo retText = null;" << endl <<
"  boolean colorAttrFound = false;" << endl <<
"  boolean textAttrFound = false;" << endl <<
"  public void dynamicOpen() {" << endl <<
"    if ( pwrAttrColor.compareTo(\"\") != 0) {" << endl <<
"      retColor = en.gdh.refObjectInfo( pwrAttrColor);" << endl <<
"      if ( retColor.evenSts())" << endl <<
"        System.out.println( \"refObjectInfoError retColor\");" << endl <<
"      else" << endl <<
"        colorAttrFound = true;" << endl <<
"    }" << endl <<
"    if ( pwrAttrText.compareTo(\"\") != 0) {" << endl <<
"      retText = en.gdh.refObjectInfo( pwrAttrText);" << endl <<
"      if ( retText.evenSts())" << endl <<
"        System.out.println( \"refObjectInfoError retText\");" << endl <<
"      else" << endl <<
"        textAttrFound = true;" << endl <<
"    }" << endl <<
"  }" << endl <<
"  public void dynamicClose() {" << endl <<
"    if ( colorAttrFound)" << endl <<
"      en.gdh.unrefObjectInfo( retColor.refid);" << endl <<
"    if ( textAttrFound)" << endl <<
"      en.gdh.unrefObjectInfo( retText.refid);" << endl <<
"  }" << endl <<
"  public void dynamicUpdate( boolean animationOnly) {" << endl <<
"    if ( animationOnly)" << endl <<
"      return;" << endl <<
"    if ( textAttrFound) {" << endl <<
"      valueText = en.gdh.getObjectRefInfoBoolean( retText.id);" << endl <<
"      if ( valueText != valueTextOld || firstScan) {" << endl <<
"        if ( valueText) {" << endl <<
"          annot1 = textHigh;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        else {" << endl <<
"          annot1 = textLow;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"      }" << endl <<
"      valueTextOld = valueText;" << endl <<
"    }" << endl;
      switch ( trace_type)
      {
        case graph_eTrace_SetDig:
        case graph_eTrace_ResetDig:
        case graph_eTrace_ToggleDig:
        case graph_eTrace_Command:
        case graph_eTrace_DigWithCommand:
          fp <<
"    if ( colorAttrFound) {" << endl <<
"      valueColor = en.gdh.getObjectRefInfoBoolean( retColor.id);" << endl <<
"      if ( valueColorOld != valueColor || firstScan) {" << endl <<
"        if ( valueColor) {" << endl <<
"          fillColor = originalFillColor;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        else {" << endl <<
"          fillColor = lowColor;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        valueColorOld = valueColor;" << endl <<
"      }" << endl <<
"    }" << endl;

          break;
        case graph_eTrace_SetDigWithTone:
        case graph_eTrace_ResetDigWithTone:
        case graph_eTrace_ToggleDigWithTone:
        case graph_eTrace_DigToneWithCommand:
          fp <<
"    if ( colorAttrFound) {" << endl <<
"      valueColor = en.gdh.getObjectRefInfoBoolean( retColor.id);" << endl <<
"      if ( valueColorOld != valueColor || firstScan) {" << endl <<
"        if ( valueColor) {" << endl <<
"          colorTone = originalColorTone;" << endl <<
"          fillColor = originalFillColor;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        else {" << endl <<
"          if ( lowTone <= GeColor.COLOR_TONE_MAX)" << endl <<
"            colorTone = lowTone;" << endl <<
"          else" << endl <<
"            fillColor = lowTone;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        valueColorOld = valueColor;" << endl <<
"      }" << endl <<
"    }" << endl;

          break;
        default:
          ;
      }
      fp <<
"    if ( firstScan)" << endl <<
"      firstScan = false;" << endl <<
"  }" << endl <<
"}" << endl;

      break;
    }
    case graph_eTrace_DigWithError:
    case graph_eTrace_Dig:
    case graph_eTrace_DigTone:
    case graph_eTrace_DigToneWithError:
    case graph_eTrace_Annot:
    case graph_eTrace_DigWithText:
    case graph_eTrace_DigBorder:
    case graph_eTrace_Invisible:
    case graph_eTrace_AnnotWithTone:
    {
      if ( strncmp( bean_name, "Jop", 3) == 0)
        fp << 
"package jpwr.beans;" << endl;

      fp << 
"import jpwr.rt.*;" << endl <<
"import jpwr.jop.*;" << endl <<
"import java.awt.*;" << endl <<
"import java.awt.geom.*;" << endl <<
"import java.awt.image.*;" << endl <<
"import java.awt.font.*;" << endl <<
"import javax.swing.*;" << endl <<
"import javax.swing.Timer;" << endl <<
"import java.awt.event.*;" << endl << endl <<
"public class " << bean_name << " extends JComponent implements JopDynamic, ActionListener{" << endl <<
"  Dimension size;" << endl <<
"  Object root;" << endl <<
"  JopEngine en;" << endl <<
"  Timer timer = new Timer(500, this);" << endl <<
"  public " << bean_name << "()" << endl <<
"  {" << endl <<
"    try {" << endl <<
"      jbInit();" << endl <<
"    }" << endl <<
"    catch(Exception e) {" << endl <<
"      e.printStackTrace();" << endl <<
"    }" << endl <<
"  }" << endl <<
endl <<
"  private void jbInit() throws Exception {" << endl <<
"    size = new Dimension( " << int(x1-x0) + 2*glow_cJBean_Offset << ", " << int(y1-y0) + 2*glow_cJBean_Offset << ");" << endl <<
"    timer.start();" << endl <<
"  }" << endl <<
"  public void actionPerformed(ActionEvent e) {" << endl <<
"    boolean engine_found = false;" << endl <<
"    Container parent = getParent();" << endl <<
"    while ( parent != null) {" << endl <<
"      if ( parent instanceof JopFrame) {" << endl <<
"        en = ((JopFrame)parent).engine;" << endl <<
"        if ( !en.isReady())" << endl <<
"          break;" << endl <<
"        en.add(this);" << endl <<
"        root = parent;" << endl <<
"        engine_found = true;" << endl <<
"        break;" << endl <<
"      }" << endl <<
"      parent = parent.getParent();" << endl <<
"    }" << endl <<
"    if ( !engine_found) {" << endl <<
"      parent = getParent();" << endl <<
"      while ( parent != null) {" << endl <<
"        if ( parent instanceof JopApplet) {" << endl <<
"          en = ((JopApplet)parent).engine;" << endl <<
"          if ( !en.isReady())" << endl <<
"            break;" << endl <<
"          en.add(this);" << endl <<
"          root = parent;" << endl <<
"          engine_found = true;" << endl <<
"          break;" << endl <<
"        }" << endl <<
"        parent = parent.getParent();" << endl <<
"      }" << endl <<
"    }" << endl <<
"    if ( engine_found) {" << endl <<
"      timer.stop();" << endl <<
"      timer = null;    " << endl <<
"    }" << endl <<
"  }" << endl;

  grow_ExportJavaBean( grow->ctx, fp, 0);
  
  fp <<
"  public Dimension getPreferredSize() { return size;}" << endl <<
"  public Dimension getMinimumSize() { return size;}" << endl;

  switch ( trace_type)
  {
    case graph_eTrace_DigWithError:
    {  
      fp <<
"  int lowColor = " << (int) trace_color << ";" << endl <<
"  public void setLowColor( int lowColor) { this.lowColor = lowColor;}" << endl <<
"  public int getLowColor() { return lowColor;}" << endl <<
"  String pwrAttribute = new String();" << endl <<
"  public void setPwrAttribute( String pwrAttribute) { this.pwrAttribute = pwrAttribute;}" << endl <<
"  public String getPwrAttribute() { return pwrAttribute;}" << endl <<
"  String pwrAttrError = new String();" << endl <<
"  public void setPwrAttrError( String pwrAttrError) { this.pwrAttrError = pwrAttrError;}" << endl <<
"  public String getPwrAttrError() { return pwrAttrError;}" << endl <<
"  boolean valueColor;" << endl <<
"  boolean valueColorOld;" << endl <<
"  boolean valueError;" << endl <<
"  boolean valueErrorOld;" << endl <<
"  boolean firstScan = true;" << endl <<
"  GdhrRefObjectInfo retColor = null;" << endl <<
"  GdhrRefObjectInfo retError = null;" << endl <<
"  boolean colorAttrFound = false;" << endl <<
"  boolean errorAttrFound = false;" << endl <<
"  public void dynamicOpen() {" << endl <<
"    if ( pwrAttribute.compareTo(\"\") != 0) {" << endl <<
"      retColor = en.gdh.refObjectInfo( pwrAttribute);" << endl <<
"      if ( retColor.evenSts())" << endl <<
"        System.out.println( \"refObjectInfoError retColor\");" << endl <<
"      else" << endl <<
"        colorAttrFound = true;" << endl <<
"    }" << endl <<
"    if ( pwrAttrError.compareTo(\"\") != 0) {" << endl <<
"      retError = en.gdh.refObjectInfo( pwrAttrError);" << endl <<
"      if ( retError.evenSts())" << endl <<
"        System.out.println( \"refObjectInfoError retError\");" << endl <<
"      else" << endl <<
"        errorAttrFound = true;" << endl <<
"    }" << endl <<
"  }" << endl <<
"  public void dynamicClose() {" << endl <<
"    if ( colorAttrFound)" << endl <<
"      en.gdh.unrefObjectInfo( retColor.refid);" << endl <<
"    if ( errorAttrFound)" << endl <<
"      en.gdh.unrefObjectInfo( retError.refid);" << endl <<
"  }" << endl <<
"  public void dynamicUpdate( boolean animationOnly) {" << endl <<
"    if ( animationOnly)" << endl <<
"      return;" << endl <<
"    if ( errorAttrFound) {" << endl <<
"      valueColor = en.gdh.getObjectRefInfoBoolean( retColor.id);" << endl <<
"      valueError = en.gdh.getObjectRefInfoBoolean( retError.id);" << endl <<
"      if ( valueError != valueErrorOld || valueColor != valueColorOld || firstScan) {" << endl <<
"        if ( valueError) {" << endl <<
"          fillColor = 56;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        else if ( valueColor) {" << endl <<
"          fillColor = originalFillColor;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        else {" << endl <<
"          fillColor = lowColor;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        valueErrorOld = valueError;" << endl <<
"        valueColorOld = valueColor;" << endl <<
"      }" << endl <<
"    }" << endl <<
"    else if ( colorAttrFound) {" << endl <<
"      valueColor = en.gdh.getObjectRefInfoBoolean( retColor.id);" << endl <<
"      if ( valueColorOld != valueColor || firstScan) {" << endl <<
"        if ( valueColor) {" << endl <<
"          fillColor = originalFillColor;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        else {" << endl <<
"          fillColor = lowColor;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        valueColorOld = valueColor;" << endl <<
"      }" << endl <<
"    }" << endl <<
"    if ( firstScan)" << endl <<
"      firstScan = false;" << endl <<
"  }" << endl;

      break;
    }

    case graph_eTrace_DigToneWithError:
    {  
      fp <<
"  int lowTone = " << (int) trace_color << ";" << endl <<
"  public void setLowTone( int lowTone) { this.lowTone = lowTone;}" << endl <<
"  public int getLowTone() { return lowTone;}" << endl <<
"  String pwrAttribute = new String();" << endl <<
"  public void setPwrAttribute( String pwrAttribute) { this.pwrAttribute = pwrAttribute;}" << endl <<
"  public String getPwrAttribute() { return pwrAttribute;}" << endl <<
"  String pwrAttrError = new String();" << endl <<
"  public void setPwrAttrError( String pwrAttrError) { this.pwrAttrError = pwrAttrError;}" << endl <<
"  public String getPwrAttrError() { return pwrAttrError;}" << endl <<
"  boolean valueColor;" << endl <<
"  boolean valueColorOld;" << endl <<
"  boolean valueError;" << endl <<
"  boolean valueErrorOld;" << endl <<
"  boolean firstScan = true;" << endl <<
"  GdhrRefObjectInfo retColor = null;" << endl <<
"  GdhrRefObjectInfo retError = null;" << endl <<
"  boolean colorAttrFound = false;" << endl <<
"  boolean errorAttrFound = false;" << endl <<
"  public void dynamicOpen() {" << endl <<
"    if ( pwrAttribute.compareTo(\"\") != 0) {" << endl <<
"      retColor = en.gdh.refObjectInfo( pwrAttribute);" << endl <<
"      if ( retColor.evenSts())" << endl <<
"        System.out.println( \"refObjectInfoError retColor\");" << endl <<
"      else" << endl <<
"        colorAttrFound = true;" << endl <<
"    }" << endl <<
"    if ( pwrAttrError.compareTo(\"\") != 0) {" << endl <<
"      retError = en.gdh.refObjectInfo( pwrAttrError);" << endl <<
"      if ( retError.evenSts())" << endl <<
"        System.out.println( \"refObjectInfoError retError\");" << endl <<
"      else" << endl <<
"        errorAttrFound = true;" << endl <<
"    }" << endl <<
"  }" << endl <<
"  public void dynamicClose() {" << endl <<
"    if ( colorAttrFound)" << endl <<
"      en.gdh.unrefObjectInfo( retColor.refid);" << endl <<
"    if ( errorAttrFound)" << endl <<
"      en.gdh.unrefObjectInfo( retError.refid);" << endl <<
"  }" << endl <<
"  public void dynamicUpdate( boolean animationOnly) {" << endl <<
"    if ( animationOnly)" << endl <<
"      return;" << endl <<
"    if ( errorAttrFound) {" << endl <<
"      valueColor = en.gdh.getObjectRefInfoBoolean( retColor.id);" << endl <<
"      valueError = en.gdh.getObjectRefInfoBoolean( retError.id);" << endl <<
"      if ( valueError != valueErrorOld || valueColor != valueColorOld || firstScan) {" << endl <<
"        if ( valueError) {" << endl <<
"          fillColor = 56;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        else if ( valueColor) {" << endl <<
"          colorTone = originalColorTone;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        else {" << endl <<
"          if ( lowTone <= GeColor.COLOR_TONE_MAX)" << endl <<
"            colorTone = lowTone;" << endl <<
"          else" << endl <<
"            fillColor = lowTone;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        valueErrorOld = valueError;" << endl <<
"        valueColorOld = valueColor;" << endl <<
"      }" << endl <<
"    }" << endl <<
"    else if ( colorAttrFound) {" << endl <<
"      valueColor = en.gdh.getObjectRefInfoBoolean( retColor.id);" << endl <<
"      if ( valueColorOld != valueColor || firstScan) {" << endl <<
"        if ( valueColor) {" << endl <<
"          colorTone = originalColorTone;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        else {" << endl <<
"          if ( lowTone <= GeColor.COLOR_TONE_MAX)" << endl <<
"            colorTone = lowTone;" << endl <<
"          else" << endl <<
"            fillColor = lowTone;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        valueColorOld = valueColor;" << endl <<
"      }" << endl <<
"    }" << endl <<
"    if ( firstScan)" << endl <<
"      firstScan = false;" << endl <<
"  }" << endl;

      break;
    }

    case graph_eTrace_Dig:
    {  
      fp <<
"  int lowColor = " << (int) trace_color << ";" << endl <<
"  public void setLowColor( int lowColor) { this.lowColor = lowColor;}" << endl <<
"  public int getLowColor() { return lowColor;}" << endl <<
"  String pwrAttribute = new String();" << endl <<
"  public void setPwrAttribute( String pwrAttribute) { this.pwrAttribute = pwrAttribute;}" << endl <<
"  public String getPwrAttribute() { return pwrAttribute;}" << endl <<
"  boolean valueColor;" << endl <<
"  boolean valueColorOld;" << endl <<
"  boolean firstScan = true;" << endl <<
"  GdhrRefObjectInfo retColor = null;" << endl <<
"  boolean colorAttrFound = false;" << endl <<
"  public void dynamicOpen() {" << endl <<
"    if ( pwrAttribute.compareTo(\"\") != 0) {" << endl <<
"      retColor = en.gdh.refObjectInfo( pwrAttribute);" << endl <<
"      if ( retColor.evenSts())" << endl <<
"        System.out.println( \"refObjectInfoError retColor\");" << endl <<
"      else" << endl <<
"        colorAttrFound = true;" << endl <<
"    }" << endl <<
"  }" << endl <<
"  public void dynamicClose() {" << endl <<
"    if ( colorAttrFound)" << endl <<
"      en.gdh.unrefObjectInfo( retColor.refid);" << endl <<
"  }" << endl <<
"  public void dynamicUpdate( boolean animationOnly) {" << endl <<
"    if ( animationOnly)" << endl <<
"      return;" << endl <<
"    if ( colorAttrFound) {" << endl <<
"      valueColor = en.gdh.getObjectRefInfoBoolean( retColor.id);" << endl <<
"      if ( valueColorOld != valueColor || firstScan) {" << endl <<
"        if ( valueColor) {" << endl <<
"          fillColor = originalFillColor;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        else {" << endl <<
"          fillColor = lowColor;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"      }" << endl <<
"      valueColorOld = valueColor;" << endl <<
"    }" << endl <<
"    if ( firstScan)" << endl <<
"      firstScan = false;" << endl <<
"  }" << endl;

      break;
    }

    case graph_eTrace_DigTone:
    {  
      fp <<
"  int lowTone = " << (int) trace_color << ";" << endl <<
"  public void setLowTone( int lowTone) { this.lowTone = lowTone;}" << endl <<
"  public int getLowTone() { return lowTone;}" << endl <<
"  String pwrAttribute = new String();" << endl <<
"  public void setPwrAttribute( String pwrAttribute) { this.pwrAttribute = pwrAttribute;}" << endl <<
"  public String getPwrAttribute() { return pwrAttribute;}" << endl <<
"  boolean valueColor;" << endl <<
"  boolean valueColorOld;" << endl <<
"  boolean firstScan = true;" << endl <<
"  GdhrRefObjectInfo retColor = null;" << endl <<
"  boolean colorAttrFound = false;" << endl <<
"  public void dynamicOpen() {" << endl <<
"    if ( pwrAttribute.compareTo(\"\") != 0) {" << endl <<
"      retColor = en.gdh.refObjectInfo( pwrAttribute);" << endl <<
"      if ( retColor.evenSts())" << endl <<
"        System.out.println( \"refObjectInfoError retColor\");" << endl <<
"      else" << endl <<
"        colorAttrFound = true;" << endl <<
"    }" << endl <<
"  }" << endl <<
"  public void dynamicClose() {" << endl <<
"    if ( colorAttrFound)" << endl <<
"      en.gdh.unrefObjectInfo( retColor.refid);" << endl <<
"  }" << endl <<
"  public void dynamicUpdate( boolean animationOnly) {" << endl <<
"    if ( animationOnly)" << endl <<
"      return;" << endl <<
"    if ( colorAttrFound) {" << endl <<
"      valueColor = en.gdh.getObjectRefInfoBoolean( retColor.id);" << endl <<
"      if ( valueColorOld != valueColor || firstScan) {" << endl <<
"        if ( valueColor) {" << endl <<
"          colorTone = originalColorTone;" << endl <<
"          fillColor = originalFillColor;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        else {" << endl <<
"          if ( lowTone <= GeColor.COLOR_TONE_MAX)" << endl <<
"            colorTone = lowTone;" << endl <<
"          else" << endl <<
"            fillColor = lowTone;" << endl <<
"          repaint();" << endl <<
"        }" << endl <<
"        valueColorOld = valueColor;" << endl <<
"      }" << endl <<
"    }" << endl <<
"    if ( firstScan)" << endl <<
"      firstScan = false;" << endl <<
"  }" << endl;

      break;
    }

    default :
      ;
  }

  fp <<
"}" << endl;
      break;
    }
    case graph_eTrace_Slider:
    {

      if ( strncmp( bean_name, "Jop", 3) == 0)
        fp << 
"package jpwr.beans;" << endl;

      fp << 
"import jpwr.rt.*;" << endl <<
"import jpwr.jop.*;" << endl <<
"import java.awt.*;" << endl <<
"import java.awt.geom.*;" << endl <<
"import java.awt.image.*;" << endl <<
"import java.awt.font.*;" << endl <<
"import javax.swing.*;" << endl <<
"public class " << bean_name << " extends JopSlider {" << endl <<
"  Dimension size;" << endl <<
"  Object root;" << endl <<
"  JopEngine en;" << endl <<
"  public " << bean_name << "()" << endl <<
"  {" << endl <<
"    try {" << endl <<
"      jbInit();" << endl <<
"    }" << endl <<
"    catch(Exception e) {" << endl <<
"      e.printStackTrace();" << endl <<
"    }" << endl <<
"  }" << endl <<
endl <<
"  private void jbInit() throws Exception {" << endl <<
"    size = new Dimension( " << int(x1-x0) + 2*glow_cJBean_Offset << ", " << int(y1-y0) + 2*glow_cJBean_Offset << ");" << endl <<
"  }" << endl;

      grow_ExportJavaBean( grow->ctx, fp, 0);
  
      fp <<
"  public Dimension getPreferredSize() { return size;}" << endl <<
"  public Dimension getMinimumSize() { return size;}" << endl <<
"}" << endl;

      break;
    }
    default:
    {
      // Component without dynamics

      if ( strncmp( bean_name, "Jop", 3) == 0)
        fp << 
"package jpwr.beans;" << endl;

      fp << 
"import jpwr.rt.*;" << endl <<
"import jpwr.jop.*;" << endl <<
"import java.awt.*;" << endl <<
"import java.awt.geom.*;" << endl <<
"import java.awt.image.*;" << endl <<
"import java.awt.font.*;" << endl <<
"import javax.swing.*;" << endl <<
"public class " << bean_name << " extends JComponent {" << endl <<
"  Dimension size;" << endl <<
"  public " << bean_name << "()" << endl <<
"  {" << endl <<
"    try {" << endl <<
"      jbInit();" << endl <<
"    }" << endl <<
"    catch(Exception e) {" << endl <<
"      e.printStackTrace();" << endl <<
"    }" << endl <<
"  }" << endl <<
endl <<
"  private void jbInit() throws Exception {" << endl <<
"    size = new Dimension( " << int(x1-x0) + 2*glow_cJBean_Offset << ", " << int(y1-y0) + 2*glow_cJBean_Offset << ");" << endl <<
"  }" << endl;

      grow_ExportJavaBean( grow->ctx, fp, 0);
  
      fp <<
"  public Dimension getPreferredSize() { return size;}" << endl <<
"  public Dimension getMinimumSize() { return size;}" << endl <<
"}" << endl;

    }
  }

  fp.close();

  // Generate BeanInfo
  strcpy( beaninfo_filename, fname);
  if ( (s = strrchr(beaninfo_filename, '.')))
    *s = 0; 
  strcat( beaninfo_filename, "BeanInfo.java");

  fp.open( beaninfo_filename);

  if ( strncmp( bean_name, "Jop", 3) == 0)
    fp << 
"package jpwr.beans;" << endl;

  fp <<
"import jpwr.jop.*;" << endl <<
"import java.beans.*;" << endl <<
"public class " << bean_name << "BeanInfo extends SimpleBeanInfo {" << endl <<
"  Class beanClass = " << bean_name << ".class;" << endl <<
"  String iconColor16x16Filename = \"" << low_bean_name << "16.gif\";" << endl <<
"  String iconColor32x32Filename = \"" << low_bean_name << "32.gif\";" << endl <<
"  String iconMono16x16Filename;" << endl <<
"  String iconMono32x32Filename;" << endl <<
"" << endl <<
"  public " << bean_name << "BeanInfo() {" << endl <<
"  }" << endl <<
"  public PropertyDescriptor[] getPropertyDescriptors() {" << endl <<
"    try {" << endl <<
"      PropertyDescriptor _toolTipText = new PropertyDescriptor(\"toolTipText\"," << endl <<
"        beanClass, \"getToolTipText\", \"setToolTipText\");" << endl <<
"      _toolTipText.setDisplayName(\"toolTipText\");" << endl <<
"      _toolTipText.setShortDescription(\"toolTipText\");" << endl <<
"      PropertyDescriptor _fillColor = new PropertyDescriptor(\"fillColor\"," << endl <<
"        beanClass, \"getFillColor\", \"setFillColor\");" << endl <<
"      _fillColor.setDisplayName(\"fillColor\");" << endl <<
"      _fillColor.setShortDescription(\"fillColor\");" << endl <<
"      _fillColor.setPropertyEditorClass(GeColorEditor.class);" << endl <<
"      PropertyDescriptor _borderColor = new PropertyDescriptor(\"borderColor\"," << endl <<
"        beanClass, \"getBorderColor\", \"setBorderColor\");" << endl <<
"      _borderColor.setDisplayName(\"borderColor\");" << endl <<
"      _borderColor.setShortDescription(\"borderColor\");" << endl <<
"      _borderColor.setPropertyEditorClass(GeColorEditor.class);" << endl <<
"      PropertyDescriptor _colorTone = new PropertyDescriptor(\"colorTone\"," << endl <<
"        beanClass, \"getColorTone\", \"setColorTone\");" << endl <<
"      _colorTone.setDisplayName(\"colorTone\");" << endl <<
"      _colorTone.setShortDescription(\"colorTone\");" << endl <<
"      _colorTone.setPropertyEditorClass(GeColorToneEditor.class);" << endl <<
"      PropertyDescriptor _colorShift = new PropertyDescriptor(\"colorShift\"," << endl <<
"        beanClass, \"getColorShift\", \"setColorShift\");" << endl <<
"      _colorShift.setDisplayName(\"colorShift\");" << endl <<
"      _colorShift.setShortDescription(\"colorShift\");" << endl <<
"      _colorShift.setPropertyEditorClass(GeColorShiftEditor.class);" << endl <<
"      PropertyDescriptor _colorBrightness = new PropertyDescriptor(\"colorBrightness\"," << endl <<
"        beanClass, \"getColorBrightness\", \"setColorBrightness\");" << endl <<
"      _colorBrightness.setDisplayName(\"colorBrightness\");" << endl <<
"      _colorBrightness.setShortDescription(\"colorBrightness\");" << endl <<
"      _colorBrightness.setPropertyEditorClass(GeColorBrightnessEditor.class);" << endl <<
"      PropertyDescriptor _colorIntensity = new PropertyDescriptor(\"colorIntensity\"," << endl <<
"        beanClass, \"getColorIntensity\", \"setColorIntensity\");" << endl <<
"      _colorIntensity.setDisplayName(\"colorIntensity\");" << endl <<
"      _colorIntensity.setShortDescription(\"colorIntensity\");" << endl <<
"      _colorIntensity.setPropertyEditorClass(GeColorIntensityEditor.class);" << endl <<
"      PropertyDescriptor _rotate = new PropertyDescriptor(\"rotate\"," << endl <<
"        beanClass, \"getRotate\", \"setRotate\");" << endl <<
"      _rotate.setDisplayName(\"rotate\");" << endl <<
"      _rotate.setShortDescription(\"rotate\");" << endl;

  switch ( trace_type)
  {
    case graph_eTrace_SetDig:
    case graph_eTrace_ResetDig:
    case graph_eTrace_ToggleDig:
    case graph_eTrace_Command:
    {
      fp << 

"      PropertyDescriptor _clickAction = new PropertyDescriptor(\"clickAction\"," << endl <<
"        beanClass, \"getClickAction\", \"setClickAction\");" << endl <<
"      _clickAction.setDisplayName(\"clickAction\");" << endl <<
"      _clickAction.setShortDescription(\"clickAction\");" << endl <<
"      _clickAction.setPropertyEditorClass(ClickActionEditor.class);" << endl <<
"      PropertyDescriptor _command = new PropertyDescriptor(\"command\"," << endl <<
"        beanClass, \"getCommand\", \"setCommand\");" << endl <<
"      _command.setDisplayName(\"command\");" << endl <<
"      _command.setShortDescription(\"command\");" << endl <<
"      PropertyDescriptor _lowColor = new PropertyDescriptor(\"lowColor\"," << endl <<
"        beanClass, \"getLowColor\", \"setLowColor\");" << endl <<
"      _lowColor.setDisplayName(\"lowColor\");" << endl <<
"      _lowColor.setShortDescription(\"lowColor\");" << endl <<
"      _lowColor.setPropertyEditorClass(GeColorEditor.class);" << endl <<
"      PropertyDescriptor _pwrAttribute = new PropertyDescriptor(\"pwrAttribute\"," << endl <<
"        beanClass, \"getPwrAttribute\", \"setPwrAttribute\");" << endl <<
"      _pwrAttribute.setDisplayName(\"pwrAttribute\");" << endl <<
"      _pwrAttribute.setShortDescription(\"pwrAttribute\");" << endl <<
"      PropertyDescriptor _pwrAttrColor = new PropertyDescriptor(\"pwrAttrColor\"," << endl <<
"        beanClass, \"getPwrAttrColor\", \"setPwrAttrColor\");" << endl <<
"      _pwrAttrColor.setDisplayName(\"pwrAttrColor\");" << endl <<
"      _pwrAttrColor.setShortDescription(\"pwrAttColor\");" << endl <<
"      PropertyDescriptor _pwrAttrText = new PropertyDescriptor(\"pwrAttrText\"," << endl <<
"        beanClass, \"getPwrAttrText\", \"setPwrAttrText\");" << endl <<
"      _pwrAttrText.setDisplayName(\"pwrAttrText\");" << endl <<
"      _pwrAttrText.setShortDescription(\"pwrAttText\");" << endl <<
"      PropertyDescriptor _textLow = new PropertyDescriptor(\"textLow\"," << endl <<
"        beanClass, \"getTextLow\", \"setTextLow\");" << endl <<
"      _textLow.setDisplayName(\"textLow\");" << endl <<
"      _textLow.setShortDescription(\"textLow\");" << endl <<
"      PropertyDescriptor _textHigh = new PropertyDescriptor(\"textHigh\"," << endl <<
"        beanClass, \"getTextHigh\", \"setTextHigh\");" << endl <<
"      _textLow.setDisplayName(\"textHigh\");" << endl <<
"      _textLow.setShortDescription(\"textHigh\");" << endl <<
"      PropertyDescriptor _access = new PropertyDescriptor(\"access\"," << endl <<
"        beanClass, \"getAccess\", \"setAccess\");" << endl <<
"      _access.setDisplayName(\"access\");" << endl <<
"      _access.setShortDescription(\"access\");" << endl <<
"      PropertyDescriptor _confirm = new PropertyDescriptor(\"confirm\"," << endl <<
"        beanClass, \"getConfirm\", \"setConfirm\");" << endl <<
"      _confirm.setDisplayName(\"confirm\");" << endl <<
"      _confirm.setShortDescription(\"confirm\");" << endl <<
"      PropertyDescriptor _confirmText = new PropertyDescriptor(\"confirmText\"," << endl <<
"        beanClass, \"getConfirmText\", \"setConfirmText\");" << endl <<
"      _confirmText.setDisplayName(\"confirmText\");" << endl <<
"      _confirmText.setShortDescription(\"confirmText\");" << endl;

      for ( i = 0; i < annot_cnt; i++)
      {
        fp <<
"      PropertyDescriptor _annot" << numbers[i] << " = new PropertyDescriptor(\"annot" << 
		numbers[i] << "\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "\", \"setAnnot" << 
		numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setDisplayName(\"annot" << numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setShortDescription(\"annot" << numbers[i] << "\");" << endl <<
"      PropertyDescriptor _annot" << numbers[i] << "Font = new PropertyDescriptor(\"annot" << 
		numbers[i] << "Font\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "Font\", \"setAnnot" << 
		numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setDisplayName(\"annot" << numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setShortDescription(\"annot" << numbers[i] << "Font\");" << endl;
      }

      fp <<
"      PropertyDescriptor[] pds = new PropertyDescriptor[] {" << endl <<
"	_toolTipText, _fillColor, _borderColor, _colorTone, _colorShift," << endl <<
"	_colorBrightness, _colorIntensity, _rotate," << endl <<
"	_clickAction, _command, _lowColor, _pwrAttribute, _pwrAttrColor," << endl <<
"	_pwrAttrText, _textLow, _textHigh, _access, _confirm, confirmText";
      for ( i = 0; i < annot_cnt; i++)
      {
        fp << ", _annot" << numbers[i] << ", _annot" << numbers[i] << "Font";
      }
      fp << "};" << endl;

      break;
    } 
    case graph_eTrace_SetDigWithTone:
    case graph_eTrace_ResetDigWithTone:
    case graph_eTrace_ToggleDigWithTone:
    {
      fp << 

"      PropertyDescriptor _clickAction = new PropertyDescriptor(\"clickAction\"," << endl <<
"        beanClass, \"getClickAction\", \"setClickAction\");" << endl <<
"      _clickAction.setDisplayName(\"clickAction\");" << endl <<
"      _clickAction.setShortDescription(\"clickAction\");" << endl <<
"      _clickAction.setPropertyEditorClass(ClickActionEditor.class);" << endl <<
"      PropertyDescriptor _command = new PropertyDescriptor(\"command\"," << endl <<
"        beanClass, \"getCommand\", \"setCommand\");" << endl <<
"      _command.setDisplayName(\"command\");" << endl <<
"      _command.setShortDescription(\"command\");" << endl <<
"      PropertyDescriptor _lowTone = new PropertyDescriptor(\"lowTone\"," << endl <<
"        beanClass, \"getLowTone\", \"setLowTone\");" << endl <<
"      _lowTone.setDisplayName(\"lowTone\");" << endl <<
"      _lowTone.setShortDescription(\"lowTone\");" << endl <<
"      _lowTone.setPropertyEditorClass(GeColorToneEditor.class);" << endl <<
"      PropertyDescriptor _pwrAttribute = new PropertyDescriptor(\"pwrAttribute\"," << endl <<
"        beanClass, \"getPwrAttribute\", \"setPwrAttribute\");" << endl <<
"      _pwrAttribute.setDisplayName(\"pwrAttribute\");" << endl <<
"      _pwrAttribute.setShortDescription(\"pwrAttribute\");" << endl <<
"      PropertyDescriptor _pwrAttrColor = new PropertyDescriptor(\"pwrAttrColor\"," << endl <<
"        beanClass, \"getPwrAttrColor\", \"setPwrAttrColor\");" << endl <<
"      _pwrAttrColor.setDisplayName(\"pwrAttrColor\");" << endl <<
"      _pwrAttrColor.setShortDescription(\"pwrAttColor\");" << endl <<
"      PropertyDescriptor _pwrAttrText = new PropertyDescriptor(\"pwrAttrText\"," << endl <<
"        beanClass, \"getPwrAttrText\", \"setPwrAttrText\");" << endl <<
"      _pwrAttrText.setDisplayName(\"pwrAttrText\");" << endl <<
"      _pwrAttrText.setShortDescription(\"pwrAttText\");" << endl <<
"      PropertyDescriptor _textLow = new PropertyDescriptor(\"textLow\"," << endl <<
"        beanClass, \"getTextLow\", \"setTextLow\");" << endl <<
"      _textLow.setDisplayName(\"textLow\");" << endl <<
"      _textLow.setShortDescription(\"textLow\");" << endl <<
"      PropertyDescriptor _textHigh = new PropertyDescriptor(\"textHigh\"," << endl <<
"        beanClass, \"getTextHigh\", \"setTextHigh\");" << endl <<
"      _textHigh.setDisplayName(\"textHigh\");" << endl <<
"      _textHigh.setShortDescription(\"textHigh\");" << endl <<
"      PropertyDescriptor _access = new PropertyDescriptor(\"access\"," << endl <<
"        beanClass, \"getAccess\", \"setAccess\");" << endl <<
"      _access.setDisplayName(\"access\");" << endl <<
"      _access.setShortDescription(\"access\");" << endl <<
"      PropertyDescriptor _confirm = new PropertyDescriptor(\"confirm\"," << endl <<
"        beanClass, \"getConfirm\", \"setConfirm\");" << endl <<
"      _confirm.setDisplayName(\"confirm\");" << endl <<
"      _confirm.setShortDescription(\"confirm\");" << endl <<
"      PropertyDescriptor _confirmText = new PropertyDescriptor(\"confirmText\"," << endl <<
"        beanClass, \"getConfirmText\", \"setConfirmText\");" << endl <<
"      _confirmText.setDisplayName(\"confirmText\");" << endl <<
"      _confirmText.setShortDescription(\"confirmText\");" << endl;
      for ( i = 0; i < annot_cnt; i++)
      {
        fp <<
"      PropertyDescriptor _annot" << numbers[i] << " = new PropertyDescriptor(\"annot" << 
		numbers[i] << "\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "\", \"setAnnot" << 
		numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setDisplayName(\"annot" << numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setShortDescription(\"annot" << numbers[i] << "\");" << endl <<
"      PropertyDescriptor _annot" << numbers[i] << "Font = new PropertyDescriptor(\"annot" << 
		numbers[i] << "Font\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "Font\", \"setAnnot" << 
		numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setDisplayName(\"annot" << numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setShortDescription(\"annot" << numbers[i] << "Font\");" << endl;
      }
      fp << 
"      PropertyDescriptor[] pds = new PropertyDescriptor[] {" << endl <<
"	_toolTipText, _fillColor, _borderColor, _colorTone, _colorShift," << endl <<
"	_colorBrightness, _colorIntensity, _rotate," << endl <<
"	_clickAction, _command, _lowTone, _pwrAttribute, _pwrAttrColor," << endl <<
"	_pwrAttrText, _textLow, _textHigh, _access, _confirm, _confirmText";
      for ( i = 0; i < annot_cnt; i++)
      {
        fp << ", _annot" << numbers[i] << ", _annot" << numbers[i] << "Font";
      }
      fp << "};" << endl;
      break;
    } 
    case graph_eTrace_Dig:
    {
      fp << 

"      PropertyDescriptor _lowColor = new PropertyDescriptor(\"lowColor\"," << endl <<
"        beanClass, \"getLowColor\", \"setLowColor\");" << endl <<
"      _lowColor.setDisplayName(\"lowColor\");" << endl <<
"      _lowColor.setShortDescription(\"lowColor\");" << endl <<
"      _lowColor.setPropertyEditorClass(GeColorEditor.class);" << endl <<
"      PropertyDescriptor _pwrAttribute = new PropertyDescriptor(\"pwrAttribute\"," << endl <<
"        beanClass, \"getPwrAttribute\", \"setPwrAttribute\");" << endl <<
"      _pwrAttribute.setDisplayName(\"pwrAttribute\");" << endl <<
"      _pwrAttribute.setShortDescription(\"pwrAttribute\");" << endl;
      for ( i = 0; i < annot_cnt; i++)
      {
        fp <<
"      PropertyDescriptor _annot" << numbers[i] << " = new PropertyDescriptor(\"annot" << 
		numbers[i] << "\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "\", \"setAnnot" << 
		numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setDisplayName(\"annot" << numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setShortDescription(\"annot" << numbers[i] << "\");" << endl <<
"      PropertyDescriptor _annot" << numbers[i] << "Font = new PropertyDescriptor(\"annot" << 
		numbers[i] << "Font\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "Font\", \"setAnnot" << 
		numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setDisplayName(\"annot" << numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setShortDescription(\"annot" << numbers[i] << "Font\");" << endl;
      }
      fp << 
"      PropertyDescriptor[] pds = new PropertyDescriptor[] {" << endl <<
"	_toolTipText, _fillColor, _borderColor, _colorTone, _colorShift," << endl <<
"	_colorBrightness, _colorIntensity, _rotate," << endl <<
"	_lowColor, _pwrAttribute";
      for ( i = 0; i < annot_cnt; i++)
      {
        fp << ", _annot" << numbers[i] << ", _annot" << numbers[i] << "Font";
      }
      fp << "};" << endl;
      break;
    } 
    case graph_eTrace_DigWithError:
    {
      fp << 

"      PropertyDescriptor _lowColor = new PropertyDescriptor(\"lowColor\"," << endl <<
"        beanClass, \"getLowColor\", \"setLowColor\");" << endl <<
"      _lowColor.setDisplayName(\"lowColor\");" << endl <<
"      _lowColor.setShortDescription(\"lowColor\");" << endl <<
"      _lowColor.setPropertyEditorClass(GeColorEditor.class);" << endl <<
"      PropertyDescriptor _pwrAttribute = new PropertyDescriptor(\"pwrAttribute\"," << endl <<
"        beanClass, \"getPwrAttribute\", \"setPwrAttribute\");" << endl <<
"      _pwrAttribute.setDisplayName(\"pwrAttribute\");" << endl <<
"      _pwrAttribute.setShortDescription(\"pwrAttribute\");" << endl <<
"      PropertyDescriptor _pwrAttrError = new PropertyDescriptor(\"pwrAttrError\"," << endl <<
"        beanClass, \"getPwrAttrError\", \"setPwrAttrError\");" << endl <<
"      _pwrAttrError.setDisplayName(\"pwrAttrError\");" << endl <<
"      _pwrAttrError.setShortDescription(\"pwrAttError\");" << endl;
      for ( i = 0; i < annot_cnt; i++)
      {
        fp <<
"      PropertyDescriptor _annot" << numbers[i] << " = new PropertyDescriptor(\"annot" << 
		numbers[i] << "\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "\", \"setAnnot" << 
		numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setDisplayName(\"annot" << numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setShortDescription(\"annot" << numbers[i] << "\");" << endl <<
"      PropertyDescriptor _annot" << numbers[i] << "Font = new PropertyDescriptor(\"annot" << 
		numbers[i] << "Font\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "Font\", \"setAnnot" << 
		numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setDisplayName(\"annot" << numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setShortDescription(\"annot" << numbers[i] << "Font\");" << endl;
      }
      fp << 
"      PropertyDescriptor[] pds = new PropertyDescriptor[] {" << endl <<
"	_toolTipText, _fillColor, _borderColor, _colorTone, _colorShift," << endl <<
"	_colorBrightness, _colorIntensity, _rotate," << endl <<
"	_lowColor, _pwrAttribute, _pwrAttrError";
      for ( i = 0; i < annot_cnt; i++)
      {
        fp << ", _annot" << numbers[i] << ", _annot" << numbers[i] << "Font";
      }
      fp << "};" << endl;
      break;
    } 

    case graph_eTrace_DigTone:
    {
      fp << 
"      PropertyDescriptor _lowTone = new PropertyDescriptor(\"lowTone\"," << endl <<
"        beanClass, \"getLowTone\", \"setLowTone\");" << endl <<
"      _lowTone.setDisplayName(\"lowTone\");" << endl <<
"      _lowTone.setShortDescription(\"lowTone\");" << endl <<
"      _lowTone.setPropertyEditorClass(GeColorToneEditor.class);" << endl <<
"      PropertyDescriptor _pwrAttribute = new PropertyDescriptor(\"pwrAttribute\"," << endl <<
"        beanClass, \"getPwrAttribute\", \"setPwrAttribute\");" << endl <<
"      _pwrAttribute.setDisplayName(\"pwrAttribute\");" << endl <<
"      _pwrAttribute.setShortDescription(\"pwrAttribute\");" << endl;
      for ( i = 0; i < annot_cnt; i++)
      {
        fp <<
"      PropertyDescriptor _annot" << numbers[i] << " = new PropertyDescriptor(\"annot" << 
		numbers[i] << "\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "\", \"setAnnot" << 
		numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setDisplayName(\"annot" << numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setShortDescription(\"annot" << numbers[i] << "\");" << endl <<
"      PropertyDescriptor _annot" << numbers[i] << "Font = new PropertyDescriptor(\"annot" << 
		numbers[i] << "Font\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "Font\", \"setAnnot" << 
		numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setDisplayName(\"annot" << numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setShortDescription(\"annot" << numbers[i] << "Font\");" << endl;
      }
      fp << 
"      PropertyDescriptor[] pds = new PropertyDescriptor[] {" << endl <<
"	_toolTipText, _fillColor, _borderColor, _colorTone, _colorShift," << endl <<
"	_colorBrightness, _colorIntensity, _rotate," << endl <<
"	_lowTone, _pwrAttribute";
      for ( i = 0; i < annot_cnt; i++)
      {
        fp << ", _annot" << numbers[i] << ", _annot" << numbers[i] << "Font";
      }
      fp << "};" << endl;
      break;
    } 
    case graph_eTrace_DigToneWithError:
    {
      fp << 
"      PropertyDescriptor _lowTone = new PropertyDescriptor(\"lowTone\"," << endl <<
"        beanClass, \"getLowTone\", \"setLowTone\");" << endl <<
"      _lowTone.setDisplayName(\"lowTone\");" << endl <<
"      _lowTone.setShortDescription(\"lowTone\");" << endl <<
"      _lowTone.setPropertyEditorClass(GeColorToneEditor.class);" << endl <<
"      PropertyDescriptor _pwrAttribute = new PropertyDescriptor(\"pwrAttribute\"," << endl <<
"        beanClass, \"getPwrAttribute\", \"setPwrAttribute\");" << endl <<
"      _pwrAttribute.setDisplayName(\"pwrAttribute\");" << endl <<
"      _pwrAttribute.setShortDescription(\"pwrAttribute\");" << endl <<
"      PropertyDescriptor _pwrAttrError = new PropertyDescriptor(\"pwrAttrError\"," << endl <<
"        beanClass, \"getPwrAttrError\", \"setPwrAttrError\");" << endl <<
"      _pwrAttrError.setDisplayName(\"pwrAttrError\");" << endl <<
"      _pwrAttrError.setShortDescription(\"pwrAttError\");" << endl;
      for ( i = 0; i < annot_cnt; i++)
      {
        fp <<
"      PropertyDescriptor _annot" << numbers[i] << " = new PropertyDescriptor(\"annot" << 
		numbers[i] << "\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "\", \"setAnnot" << 
		numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setDisplayName(\"annot" << numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setShortDescription(\"annot" << numbers[i] << "\");" << endl <<
"      PropertyDescriptor _annot" << numbers[i] << "Font = new PropertyDescriptor(\"annot" << 
		numbers[i] << "Font\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "Font\", \"setAnnot" << 
		numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setDisplayName(\"annot" << numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setShortDescription(\"annot" << numbers[i] << "Font\");" << endl;
      }
      fp << 
"      PropertyDescriptor[] pds = new PropertyDescriptor[] {" << endl <<
"	_toolTipText, _fillColor, _borderColor, _colorTone, _colorShift," << endl <<
"	_colorBrightness, _colorIntensity, _rotate," << endl <<
"	_lowTone, _pwrAttribute, _pwrAttrError";
      for ( i = 0; i < annot_cnt; i++)
      {
        fp << ", _annot" << numbers[i] << ", _annot" << numbers[i] << "Font";
      }
      fp << "};" << endl;
      break;
    } 

    default:
    {
      for ( i = 0; i < annot_cnt; i++)
      {
        fp <<
"      PropertyDescriptor _annot" << numbers[i] << " = new PropertyDescriptor(\"annot" << 
		numbers[i] << "\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "\", \"setAnnot" << 
		numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setDisplayName(\"annot" << numbers[i] << "\");" << endl <<
"      _annot" << numbers[i] << ".setShortDescription(\"annot" << numbers[i] << "\");" << endl <<
"      PropertyDescriptor _annot" << numbers[i] << "Font = new PropertyDescriptor(\"annot" << 
		numbers[i] << "Font\"," << endl <<
"        beanClass, \"getAnnot" << numbers[i] << "Font\", \"setAnnot" << 
		numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setDisplayName(\"annot" << numbers[i] << "Font\");" << endl <<
"      _annot" << numbers[i] << "Font.setShortDescription(\"annot" << numbers[i] << "Font\");" << endl;
      }
      fp << 
"      PropertyDescriptor[] pds = new PropertyDescriptor[] {" << endl <<
"	_toolTipText, _fillColor, _borderColor, _colorTone, _colorShift," << endl <<
"	_colorBrightness, _colorIntensity, _rotate";
      for ( i = 0; i < annot_cnt; i++)
      {
        fp << ", _annot" << numbers[i] << ", _annot" << numbers[i] << "Font";
      }
      fp << "};" << endl;
      break;
    } 
  }
  fp <<
"      return pds;" << endl <<
"" << endl <<
"    }" << endl <<
"    catch(IntrospectionException ex) {" << endl <<
"      ex.printStackTrace();" << endl <<
"      return null;" << endl <<
"    }" << endl <<
"  }" << endl <<
"  public java.awt.Image getIcon(int iconKind) {" << endl <<
"    switch (iconKind) {" << endl <<
"    case BeanInfo.ICON_COLOR_16x16:" << endl <<
"        return iconColor16x16Filename != null ? loadImage(iconColor16x16Filename) : null;" << endl <<
"    case BeanInfo.ICON_COLOR_32x32:" << endl <<
"        return iconColor32x32Filename != null ? loadImage(iconColor32x32Filename) : null;" << endl <<
"    case BeanInfo.ICON_MONO_16x16:" << endl <<
"        return iconMono16x16Filename != null ? loadImage(iconMono16x16Filename) : null;" << endl <<
"    case BeanInfo.ICON_MONO_32x32:" << endl <<
"        return iconMono32x32Filename != null ? loadImage(iconMono32x32Filename) : null;" << endl <<
"        }" << endl <<
"    return null;" << endl <<
"  }" << endl <<
"  public BeanInfo[] getAdditionalBeanInfo() {" << endl <<
"    Class superclass = beanClass.getSuperclass();" << endl <<
"    try {" << endl <<
"      BeanInfo superBeanInfo = Introspector.getBeanInfo(superclass);" << endl <<
"//    return new BeanInfo[] { superBeanInfo };" << endl <<
"      return null;" << endl <<
"    }" << endl <<
"    catch(IntrospectionException ex) {" << endl <<
"      ex.printStackTrace();" << endl <<
"      return null;" << endl <<
"    }" << endl <<
"  }" << endl <<
"}" << endl;


  fp.close();
  free( (char *) numbers);

  return 1;
}


int Graph::export_gejava_nodeclass( ofstream& fp, grow_tNodeClass nodeclass)
{
  char bean_name[40];
  double x0, x1, y0, y1;
  glow_eTraceType trace_type;
  int pages;
  grow_tObject 	*objectlist, *object_p;
  int 		object_cnt;
  int i;

  grow_GetNodeClassJavaName( nodeclass, bean_name);
  grow_GetNodeClassTraceType( nodeclass, &trace_type);

  grow_MeasureNodeClassJavaBean( nodeclass, &x1, &x0, &y1, &y0);
  pages = grow_GetNodeClassPages( nodeclass);

  if ( (graph_eTrace) trace_type == graph_eTrace_AnnotInput ||
       (graph_eTrace) trace_type == graph_eTrace_AnnotInputWithTone)
  {
    fp <<
"private class " << bean_name << " extends GeTextField {" << endl <<
"  public " << bean_name << "()" << endl <<
"  {" << endl <<
"    setFont( annotFont);" << endl <<
"    setFillColor( annotBackground);" << endl <<
"  }" << endl;
        fp <<

"  int original_width = " << int( x1 - x0) + 2 * glow_cJBean_Offset << ";" << endl <<
"  int original_height = " << int( y1 - y0) + 2 * glow_cJBean_Offset << ";" << endl <<
"  boolean fontSet = false;" << endl;

    grow_ExportNcJavaBeanFont( grow->ctx, nodeclass, fp, 0);

    fp <<
"  public void paintComponent(Graphics g1) {" << endl <<
"    if ( !fontSet) {" << endl <<
"      float width = getWidth();" << endl <<
"      float height = getHeight();" << endl <<
"      setFont( annotFont.deriveFont((float)(height / original_height * annotFont.getSize())));" << endl <<
"      fontSet = true;" << endl <<
"    }" << endl <<
"    super.paintComponent( g1);" << endl <<
"  }" << endl;
    fp <<
"}" << endl;

  }
  else if ( strcmp( bean_name, "pwr_framethin") == 0)
  {
    // Use prefabricated class GeFrameThin
    fp <<
"private class " << bean_name << " extends GeFrameThin {" << endl <<
"  public " << bean_name << "()" << endl <<
"  {" << endl <<
"  }" << endl <<
"}" << endl;

  }
  else
  {
    if ( grow_IsSliderClass( nodeclass))
      fp <<
"private class " << bean_name << " extends GeSlider {" << endl;
    else
      fp <<
"private class " << bean_name << " extends GeComponent {" << endl;

    fp <<
"  Dimension size;" << endl;

    // Declarations of GrowNode objects
    grow_ExportNodeClassJavaBean( grow->ctx, nodeclass, fp, 1);

    fp <<
"  public " << bean_name << "()" << endl <<
"  {" << endl <<
"    size = new Dimension( " << int(x1-x0) + 2*glow_cJBean_Offset << ", " << int(y1-y0) + 2*glow_cJBean_Offset << ");" << endl;

    // Print dynamics for all groups and grownodes in groups
    grow_GetNodeClassObjectList( nodeclass, &objectlist, &object_cnt);
 
    object_p = objectlist;
    for ( i = 0; i < object_cnt; i++)
    {
      if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowNode ||
           grow_GetObjectType( *object_p) == glow_eObjectType_GrowGroup)
        export_GejavaObjectTraceAttr( fp, *object_p, i);
      else if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowBar)
        export_BarTraceAttr( fp, *object_p, i);
      else if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowTrend)
        export_TrendTraceAttr( fp, *object_p, i);
      else if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowSlider)
        export_SliderTraceAttr( fp, *object_p, i);
      object_p++;
    }

    // GrowNode attributes of GrowNode objects
    grow_ExportNodeClassJavaBean( grow->ctx, nodeclass, fp, 2);

    if ( pages > 1)
      fp <<
"    pages = " << pages << ";" << endl;
    fp <<
"  }" << endl;

    grow_ExportNodeClassJavaBean( grow->ctx, nodeclass, fp, 0);
  
    fp <<
"  public Dimension getPreferredSize() { return size;}" << endl <<
"  public Dimension getMinimumSize() { return size;}" << endl <<
"}" << endl;

  }
  return 1;
}

int Graph::export_javaframe( char *filename, char *bean_name, int applet,
	int html)
{
  ofstream fp;
  char		fname[120];
  double x0, x1, y0, y1;
  glow_eDrawType background_color;
  grow_tObject 	*objectlist, *object_p;
  int 		object_cnt;
  int		i;
  char          background_image[80];
  int           background_tiled;
  int           bg_image_width;
  int           bg_image_height;
  int           sts;

  grow_GetBackgroundImage( grow->ctx, background_image, &background_tiled);
  if ( strcmp( background_image, "") != 0)
  {
    sts = grow_GetBackgroundImageSize( grow->ctx, &bg_image_width, &bg_image_height);
    if ( EVEN(sts))
      strcpy( background_image, "");
  }

  if ( !strchr( filename, ':') && !strchr( filename, '/'))
  {
    strcpy( fname, default_path);
    strcat( fname, filename);
  }
  else
    strcpy( fname, filename);

  dcli_translate_filename( fname, fname);

  if ( !html )
  {
    grow_GetObjectList( grow->ctx, &objectlist, &object_cnt);

    grow_MeasureJavaBean( grow->ctx, &x1, &x0, &y1, &y0);
    grow_GetBackgroundColor( grow->ctx, &background_color);

    fp.open( fname);

    fp << 
"import jpwr.rt.*;" << endl <<
"import jpwr.jop.*;" << endl <<
"import java.awt.*;" << endl <<
"import java.awt.geom.*;" << endl <<
"import java.awt.image.*;" << endl <<
"import java.awt.font.*;" << endl <<
"import javax.swing.*;" << endl <<
"import java.awt.event.*;" << endl << endl;
    if ( applet)
      fp <<
"public class " << bean_name << " extends JopApplet {" << endl;
    else
      fp <<
"public class " << bean_name << " extends JopFrame {" << endl;
      fp <<
"  JPanel contentPane;" << endl <<
"  BorderLayout borderLayout1 = new BorderLayout();" << endl <<
"  LocalPanel localPanel = new LocalPanel();" << endl <<
"  Dimension size;" << endl;

     // Declarations of components
     grow_ExportJavaBean( grow->ctx, fp, 1);

     if ( applet) 
     {
       fp <<
"  public " << bean_name << "() {}" << endl <<
"  public void init() {" << endl <<
"    super.init();" << endl;
     }
     else
     {
       fp <<
"  public " << bean_name << "() {" << endl;
     }
     fp <<
"    try {" << endl <<
"      jbInit();" << endl <<
"    }" << endl <<
"    catch(Exception e) {" << endl <<
"      e.printStackTrace();" << endl <<
"    }" << endl <<
"  }" << endl <<
"  private void jbInit() throws Exception  {" << endl <<
"    size = new Dimension( " << int(x1-x0) + 2*glow_cJBean_Offset << ", " << int(y1-y0) + 2*glow_cJBean_Offset << ");" << endl <<
"    contentPane = (JPanel) this.getContentPane();" << endl <<
"    contentPane.setLayout(borderLayout1);" << endl <<
"    contentPane.add(localPanel, BorderLayout.CENTER);" << endl <<
"    contentPane.setOpaque(true);" << endl <<
"    localPanel.setLayout(null);" << endl <<
"    localPanel.setOpaque(true);" << endl;
        if ( background_color != glow_eDrawType_LineErase)
          fp <<
"    localPanel.setBackground(GeColor.getColor(" << (int) background_color <<
	", GeColor.NO_COLOR));" << endl;
        if ( !applet)
          fp <<
"    this.setSize(size);" << endl <<
"    this.setTitle(\"" << bean_name << "\");" << endl;

        // Set drawing attributes of components
        grow_ExportJavaBean( grow->ctx, fp, 2);

        object_p = objectlist;
        for ( i = 0; i < object_cnt; i++)
        {
          if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowNode)
            export_ObjectTraceAttr( fp, *object_p, i);
          else if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowBar)
            export_BarTraceAttr( fp, *object_p, i);
          else if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowTrend)
            export_TrendTraceAttr( fp, *object_p, i);
          else if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowSlider)
            export_SliderTraceAttr( fp, *object_p, i);
          object_p++;
        }

        fp <<
"    engine.setFrameReady();" << endl <<
"  }" << endl <<
"" << endl <<
"class LocalPanel extends JPanel {" << endl;
        if ( strcmp( background_image, "") != 0) 
	{
	  fp <<
"  GeImage backgroundImage = new GeImage();" << endl <<
"  public LocalPanel() {" << endl;
          if ( background_tiled)
            fp  <<
"    backgroundImage.setBounds(0,0, " << bg_image_width << 
	      ", " << bg_image_height << ");" << endl;
          else
            fp <<
"    backgroundImage.setBounds(0,0, " <<  int(x1-x0) + 2*glow_cJBean_Offset << 
", " << int(y1-y0) + 2*glow_cJBean_Offset << ");" << endl;
          fp <<
"    backgroundImage.setImage( \"" << background_image << "\");" << endl <<
"  }" << endl;
        }
        else
          fp <<
"  public LocalPanel() {}" << endl;

        grow_ExportJavaBean( grow->ctx, fp, 0);
  
        fp <<
"  public Dimension getPreferredSize() { return size;}" << endl <<
"  public Dimension getMinimumSize() { return size;}" << endl <<
"}" << endl <<


"}" << endl;

    fp.close();

  }
  else
  {
    char codebase[200];

    // Create a html file
    cnf_get_value( "appletCodebase", codebase);

    grow_MeasureJavaBean( grow->ctx, &x1, &x0, &y1, &y0);
    fp.open( fname);

    fp <<
"<html>" << endl <<
"  <head>" << endl <<
"      <title>" << bean_name << "</title>" << endl <<
"  </head>" << endl <<
"  <body bgcolor=\"#999999\">" << endl <<
"    <object classid=\"clsid:8AD9C840-044E-11D1-B3E9-00805F499D93\"" << endl <<
"      width=" << int(x1-x0) + 2*glow_cJBean_Offset << endl <<
"      height=" << int(y1-y0) + 2*glow_cJBean_Offset << endl <<
"      codebase=\"" << codebase << "\">" << endl <<
"      <PARAM NAME = CODE VALUE = " << bean_name << "_A.class >" << endl <<
"      <PARAM NAME =\"archive\" VALUE =\"pwrp_" << systemname << "_web.jar," <<
"pwr_rt_client.jar,pwr_jop.jar\">" << endl <<
"      <PARAM NAME=\"type\" VALUE=\"application/x-java-applet;version=1.3\">" << endl <<
"      <PARAM NAME=\"scriptable\" VALUE=\"false\">" << endl <<
"      <PARAM NAME=\"instance\" VALUE=\"\">" << endl <<
"    <embed type=\"application/x-java-applet;version=1.3\"" << endl <<
"      code = " << bean_name << "_A.class" << endl <<
"      archive =\"pwrp_" << systemname << "_web.jar," <<"pwr_rt_client.jar,pwr_jop.jar\"" << endl <<
"      width=" << int(x1-x0) + 2*glow_cJBean_Offset << endl <<
"      height=" << int(y1-y0) + 2*glow_cJBean_Offset << ">" << endl <<
"      height=" << int(y1-y0) + 2*glow_cJBean_Offset << endl <<
"      instance=\"\">" << endl <<
"  </body>" << endl <<
"</html>" << endl;

    fp.close();
  }

  return 1;
}

int Graph::export_gejava( char *filename, char *bean_name, int applet, int html)
{
  ofstream 		fp;
  char			fname[120];
  double 		x0, x1, y0, y1;
  glow_eDrawType 	background_color;
  grow_tObject 		*objectlist, *object_p;
  int 			object_cnt;
  int			i;
  grow_tNodeClass	*nodeclass_list;
  int			nodeclass_count;
  double		scan_time, fast_scan_time, animation_scan_time;
  char          background_image[80];
  int           background_tiled;
  int           bg_image_width;
  int           bg_image_height;
  int           sts;

  grow_GetBackgroundImage( grow->ctx, background_image, &background_tiled);
  if ( strcmp( background_image, "") != 0)
  {
    sts = grow_GetBackgroundImageSize( grow->ctx, &bg_image_width, &bg_image_height);
    if ( EVEN(sts))
        strcpy( background_image, "");
  }

  if ( !strchr( filename, ':') && !strchr( filename, '/'))
  {
    strcpy( fname, default_path);
    strcat( fname, filename);
  }
  else
    strcpy( fname, filename);

  dcli_translate_filename( fname, fname);

  if ( !html)
  {
    grow_GetObjectList( grow->ctx, &objectlist, &object_cnt);

    grow_SetJavaFrame( grow->ctx, &x1, &x0, &y1, &y0);
    grow_GetBackgroundColor( grow->ctx, &background_color);
    grow_GetScanTime( grow->ctx, &scan_time, &fast_scan_time, 
		      &animation_scan_time);


    fp.open( fname);

    fp << 
"import jpwr.rt.*;" << endl <<
"import jpwr.jop.*;" << endl <<
"import java.awt.*;" << endl <<
"import java.awt.geom.*;" << endl <<
"import java.awt.image.*;" << endl <<
"import java.awt.font.*;" << endl <<
"import javax.swing.*;" << endl <<
"import java.awt.event.*;" << endl << endl;
    if ( applet)
      fp <<
"public class " << bean_name << " extends JopApplet {" << endl;
    else
      fp <<
"public class " << bean_name << " extends JopFrame {" << endl;
    fp <<
"  JPanel contentPane;" << endl <<
"  BorderLayout borderLayout1 = new BorderLayout();" << endl <<
"  LocalPanel localPanel = new LocalPanel();" << endl <<
"  Dimension size;" << endl;

    // Declarations of components
    grow_ExportJavaBean( grow->ctx, fp, 1);

    if ( applet) 
    {
      fp <<
"  public " << bean_name << "() {}" << endl <<
"  public void init() {" << endl <<
"    super.init();" << endl;
    }
    else
    {
      fp <<
"  public " << bean_name << "() {" << endl;
    }
    fp <<
"    JopSpider.setSystemName( \"" << systemname << "\");" << endl <<
"    engine.setAnimationScanTime( " << int(animation_scan_time * 1000) << ");" << endl <<
"    engine.setScanTime( " << int(scan_time * 1000) << ");" << endl <<
"    size = new Dimension( " << int(x1-x0) + 2*glow_cJBean_Offset << ", " << int(y1-y0) + 2*glow_cJBean_Offset << ");" << endl <<
"    contentPane = (JPanel) this.getContentPane();" << endl <<
"    contentPane.setLayout(borderLayout1);" << endl <<
"    contentPane.add(localPanel, BorderLayout.CENTER);" << endl <<
"    contentPane.setOpaque(true);" << endl <<
"    localPanel.setLayout(null);" << endl <<
"    localPanel.setOpaque(true);" << endl;
    if ( background_color != glow_eDrawType_LineErase)
      fp <<
"    localPanel.setBackground(GeColor.getColor(" << (int) background_color <<
	", GeColor.NO_COLOR));" << endl;
    if ( !applet)
      fp <<
"    this.setSize(size);" << endl <<
"    this.setTitle(\"" << bean_name << "\");" << endl;

    // Set drawing attributes of components
    grow_ExportJavaBean( grow->ctx, fp, 2);

    object_p = objectlist;
    for ( i = 0; i < object_cnt; i++)
    {
      if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowNode ||
           grow_GetObjectType( *object_p) == glow_eObjectType_GrowGroup)
        export_GejavaObjectTraceAttr( fp, *object_p, i);
      else if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowBar)
        export_BarTraceAttr( fp, *object_p, i);
      else if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowTrend)
        export_TrendTraceAttr( fp, *object_p, i);
      else if ( grow_GetObjectType( *object_p) == glow_eObjectType_GrowSlider)
        export_SliderTraceAttr( fp, *object_p, i);
      object_p++;
    }

    fp <<
"    engine.setFrameReady();" << endl <<
"  }" << endl <<
"" << endl <<
"class LocalPanel extends JPanel {" << endl;
        if ( strcmp( background_image, "") != 0) 
	{
	  fp <<
"  GeImage backgroundImage = new GeImage();" << endl <<
"  public LocalPanel() {" << endl;
          if ( background_tiled)
            fp  <<
"    backgroundImage.setBounds(0,0, " << bg_image_width << 
	      ", " << bg_image_height << ");" << endl;
          else
            fp <<
"    backgroundImage.setBounds(0,0, " <<  int(x1-x0) + 2*glow_cJBean_Offset << 
              ", " << int(y1-y0) + 2*glow_cJBean_Offset << ");" << endl;
          fp <<
"    backgroundImage.setImage( \"" << background_image << "\");" << endl <<
"  }" << endl;
        }
        else
          fp <<
"  public LocalPanel() {}" << endl;

    grow_ExportJavaBean( grow->ctx, fp, 0);
  
    fp <<
"  public Dimension getPreferredSize() { return size;}" << endl <<
"  public Dimension getMinimumSize() { return size;}" << endl <<
"}" << endl;

    grow_GetNodeClassList( grow->ctx, &nodeclass_list, &nodeclass_count);
    for ( i = 0; i < nodeclass_count; i++)
    {
      if ( !grow_IsNextNodeClass( nodeclass_list[i]))
        export_gejava_nodeclass( fp, nodeclass_list[i]);
    }

    grow_GetNodeGroupList( grow->ctx, &nodeclass_list, &nodeclass_count);
    for ( i = 0; i < nodeclass_count; i++)
    {
      export_gejava_nodeclass( fp, nodeclass_list[i]);
    }
    if ( nodeclass_count > 0)
      free( nodeclass_list);

    fp <<
"}" << endl;

    fp.close();
  }
  else
  {
    char codebase[200];

    // Create a html file
    cnf_get_value( "appletCodebase", codebase);

    grow_SetJavaFrame( grow->ctx, &x1, &x0, &y1, &y0);
    fp.open( fname);

    fp <<
"<html>" << endl <<
"  <head>" << endl <<
"      <title>" << bean_name << "</title>" << endl <<
"  </head>" << endl <<
"  <body bgcolor=\"#999999\">" << endl <<
"    <object classid=\"clsid:8AD9C840-044E-11D1-B3E9-00805F499D93\"" << endl <<
"      width=" << int(x1-x0) + 2*glow_cJBean_Offset << endl <<
"      height=" << int(y1-y0) + 2*glow_cJBean_Offset << endl <<
"      codebase=\"" << codebase << "\">" << endl <<
"      <PARAM NAME = CODE VALUE = " << bean_name << "_A.class >" << endl <<
"      <PARAM NAME =\"archive\" VALUE =\"pwrp_" << systemname << "_web.jar," <<
"pwr_rt_client.jar,pwr_jop.jar\">" << endl <<
"      <PARAM NAME=\"type\" VALUE=\"application/x-java-applet;version=1.3\">" << endl <<
"      <PARAM NAME=\"scriptable\" VALUE=\"false\">" << endl <<
"      <PARAM NAME=\"instance\" VALUE=\"\">" << endl <<
"    <embed type=\"application/x-java-applet;version=1.3\"" << endl <<
"      code = " << bean_name << "_A.class" << endl <<
"      archive =\"pwrp_" << systemname << "_web.jar," <<"pwr_rt_client.jar,pwr_jop.jar\"" << endl <<
"      width=" << int(x1-x0) + 2*glow_cJBean_Offset << endl <<
"      height=" << int(y1-y0) + 2*glow_cJBean_Offset << endl <<
"      instance=\"\">" << endl <<
"  </body>" << endl <<
"</html>" << endl;

    fp.close();
  }

  return 1;
}


int Graph::export_ObjectTraceAttr( ofstream& fp, grow_tObject object, int cnt)
{
  graph_eTrace		trace_type;
  glow_sTraceData	*trace_data;
  glow_eDrawType 	trace_color;
  glow_eDrawType 	trace_color2;
  glow_eDrawType 	color;
  char			class_name[40];
  char 			var_name[40];
  int			i;
  int			annot_cnt;
  int			*numbers;
  char			annot_str[200];

  grow_GetTraceAttr( object, &trace_data);
  grow_GetObjectClassJavaName( object, class_name);

  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", cnt);

  trace_type = (graph_eTrace) trace_data->attr_type;
  if ( trace_type == graph_eTrace_Inherit)
    grow_GetObjectClassTraceType( object, (glow_eTraceType *) &trace_type);

  trace_color = trace_data->color;
  if ( trace_color == glow_eDrawType_Inherit)
    grow_GetObjectClassTraceColor( object, &trace_color, &color);
  trace_color2 = trace_data->color2;
  if ( trace_color2 == glow_eDrawType_Inherit)
    grow_GetObjectClassTraceColor( object, &color, &trace_color2);
  
  // trace_type No equals Inherit from class
  if ( trace_type == graph_eTrace_No)
    trace_type = graph_eTrace_Inherit;

  switch ( trace_type)
  {
    case graph_eTrace_Dig:
    case graph_eTrace_DigWithCommand:
    case graph_eTrace_DigBorder:
    case graph_eTrace_DigWithText:
    case graph_eTrace_Invisible:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl <<
"    " << var_name << ".setLowColor(" << (int)trace_color << ");" << endl;
      }
      break;
    case graph_eTrace_DigWithError:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl;
        if ( strcmp( trace_data->data[1], "") != 0)
          fp <<
"    " << var_name << ".setPwrAttrError(\"" << trace_data->data[1] << "\");" << endl;
        fp <<
"    " << var_name << ".setLowColor(" << (int)trace_color << ");" << endl;
      }
      break;
    case graph_eTrace_DigTone:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl <<
"    " << var_name << ".setLowTone(" << (int)trace_color << ");" << endl;
      }
      break;
    case graph_eTrace_DigToneWithError:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl;
        if ( strcmp( trace_data->data[1], "") != 0)
          fp <<
"    " << var_name << ".setPwrAttrError(\"" << trace_data->data[1] << "\");" << endl;
        fp <<
"    " << var_name << ".setLowTone(" << (int)trace_color << ");" << endl;
      }
      break;
    case graph_eTrace_SetDig:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl <<
"    " << var_name << ".setClickAction(Jop.BUTTON_ACTION_SET);" << endl;
      }
      break;
    case graph_eTrace_ResetDig:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl <<
"    " << var_name << ".setClickAction(Jop.BUTTON_ACTION_RESET);" << endl;
      }
      break;
    case graph_eTrace_ToggleDig:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl <<
"    " << var_name << ".setClickAction(Jop.BUTTON_ACTION_TOGGLE);" << endl;
      }
      break;
    case graph_eTrace_Command:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".setCommand(\"" << trace_data->data[0] << "\");" << endl <<
"    " << var_name << ".setClickAction(Jop.BUTTON_ACTION_COMMAND);" << endl;
      }
      break;
    case graph_eTrace_SetDigWithTone:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl <<
"    " << var_name << ".setPwrAttrTone(\"" << trace_data->data[2] << "\");" << endl <<
"    " << var_name << ".setClickAction(Jop.BUTTON_ACTION_SET);" << endl <<
"    " << var_name << ".setLowTone(" << (int)trace_color << ");" << endl;
      }
      break;
    case graph_eTrace_ResetDigWithTone:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl <<
"    " << var_name << ".setPwrAttrTone(\"" << trace_data->data[2] << "\");" << endl <<
"    " << var_name << ".setClickAction(Jop.BUTTON_ACTION_RESET);" << endl <<
"    " << var_name << ".setLowTone(" << (int)trace_color << ");" << endl;
      }
      break;
    case graph_eTrace_ToggleDigWithTone:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl <<
"    " << var_name << ".setPwrAttrTone(\"" << trace_data->data[2] << "\");" << endl <<
"    " << var_name << ".setClickAction(Jop.BUTTON_ACTION_TOGGLE);" << endl <<
"    " << var_name << ".setLowTone(" << (int)trace_color << ");" << endl;
      }
      break;
    default:
      ;
  }
  
  // Print annotations
  grow_GetObjectAnnotationNumbers( object, &numbers, &annot_cnt);

  for ( i = 0; i < annot_cnt; i++)
  {
    grow_GetAnnotation( object, numbers[i], annot_str, sizeof(annot_str));
    if ( strcmp( annot_str, "") != 0)
    {
        fp <<
"    " << var_name << ".setAnnot" << numbers[i] << "(\"" << annot_str << "\");" << endl;
    }
  }
  free( (char *) numbers);
  return 1;
}

int Graph::export_GejavaObjectTraceAttr( ofstream& fp, grow_tObject object, int cnt)
{
  graph_eTrace		trace_type;
  glow_sTraceData	*trace_data;
  glow_eDrawType 	trace_color;
  glow_eDrawType 	trace_color2;
  glow_eDrawType 	color;
  char			class_name[40];
  char 			var_name[40];
  int			i;
  int			annot_cnt;
  int			*numbers;
  char			annot_str[200];

  grow_GetTraceAttr( object, &trace_data);
  grow_GetObjectClassJavaName( object, class_name);

  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", cnt);

  trace_type = (graph_eTrace) trace_data->attr_type;
  if ( trace_type == graph_eTrace_Inherit)
    grow_GetObjectClassTraceType( object, (glow_eTraceType *) &trace_type);

  trace_color = trace_data->color;
  if ( trace_color == glow_eDrawType_Inherit)
    grow_GetObjectClassTraceColor( object, &trace_color, &color);
  trace_color2 = trace_data->color2;
  if ( trace_color2 == glow_eDrawType_Inherit)
    grow_GetObjectClassTraceColor( object, &color, &trace_color2);
  
  // No for trace_type equals Inherit from class
  if ( trace_type == graph_eTrace_No)
    trace_type = graph_eTrace_Inherit;

  switch ( trace_type)
  {
    case graph_eTrace_Dig:
    case graph_eTrace_DigBorder:
    case graph_eTrace_DigTone:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl;
      }
      break;
    case graph_eTrace_Invisible:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl;
      }
      break;
    case graph_eTrace_DigWithText:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl;
      }
      break;
    case graph_eTrace_DigWithError:
    case graph_eTrace_DigToneWithError:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[2] << "\",2);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl;
      }
      break;
    case graph_eTrace_DigWithCommand:
    case graph_eTrace_DigToneWithCommand:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << cmd_cnv( trace_data->data[3]) << "\",3);" << endl;
        fp <<
"    " << var_name << ".dd.setData(\"" << cmd_cnv( trace_data->data[4]) << "\",4);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl <<
"    " << var_name << ".dd.setAccess(" << (int)trace_data->access << ");" << endl;
      }
      break;
    case graph_eTrace_DigWithErrorAndCommand:
    case graph_eTrace_DigToneWithErrorAndCommand:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[2] << "\",2);" << endl <<
"    " << var_name << ".dd.setData(\"" << cmd_cnv( trace_data->data[3]) << "\",3);" << endl;
        fp <<
"    " << var_name << ".dd.setData(\"" << cmd_cnv( trace_data->data[4]) << "\",4);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl <<
"    " << var_name << ".dd.setAccess(" << (int)trace_data->access << ");" << endl;
      }
      break;
    case graph_eTrace_DigTwo:
    case graph_eTrace_DigToneTwo:
    case graph_eTrace_DigTwoWithCommand:
    case graph_eTrace_DigToneTwoWithCommand:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[2] << "\",2);" << endl <<
"    " << var_name << ".dd.setData(\"" << cmd_cnv( trace_data->data[3]) << "\",3);" << endl;
        fp <<
"    " << var_name << ".dd.setData(\"" << cmd_cnv( trace_data->data[4]) << "\",4);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl <<
"    " << var_name << ".dd.setColor2(" << (int)trace_color2 << ");" << endl <<
"    " << var_name << ".dd.setAccess(" << (int)trace_data->access << ");" << endl;
      }
      break;
    case graph_eTrace_SetDig:
    case graph_eTrace_ResetDig:
    case graph_eTrace_ToggleDig:
    case graph_eTrace_Command:
    case graph_eTrace_RadioButton:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << cmd_cnv( trace_data->data[0]) << "\",0);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setAccess(" << (int)trace_data->access << ");" << endl;
      }
      break;
    case graph_eTrace_SetDigConfirm:
    case graph_eTrace_ResetDigConfirm:
    case graph_eTrace_ToggleDigConfirm:
    case graph_eTrace_CommandConfirm:
    case graph_eTrace_DigShiftWithToggleDig:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << cmd_cnv(trace_data->data[0]) << "\",0);" << endl;
        fp <<
"    " << var_name << ".dd.setData(\"" << cmd_cnv(trace_data->data[1]) << "\",1);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setAccess(" << (int)trace_data->access << ");" << endl;
      }
      break;
    case graph_eTrace_SetDigWithTone:
    case graph_eTrace_StoDigWithTone:
    case graph_eTrace_ResetDigWithTone:
    case graph_eTrace_ToggleDigWithTone:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[2] << "\",2);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[3] << "\",3);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[4] << "\",4);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl <<
"    " << var_name << ".dd.setAccess(" << (int)trace_data->access << ");" << endl;
      }
      break;
    case graph_eTrace_SetDigConfirmWithTone:
    case graph_eTrace_ResetDigConfirmWithTone:
    case graph_eTrace_ToggleDigConfirmWithTone:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[2] << "\",2);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[3] << "\",3);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[4] << "\",4);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl <<
"    " << var_name << ".dd.setAccess(" << (int)trace_data->access << ");" << endl;
      }
      break;
    case graph_eTrace_Annot:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl;
      }
      break;
    case graph_eTrace_AnnotWithTone:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[2] << "\",2);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[3] << "\",3);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl;
      }
      break;
    case graph_eTrace_AnnotInput:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[3] << "\",3);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[4] << "\",4);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setAccess(" << (int)trace_data->access << ");" << endl;
      }
      break;
    case graph_eTrace_AnnotInputWithTone:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[2] << "\",2);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[3] << "\",3);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[4] << "\",4);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl <<
"    " << var_name << ".dd.setAccess(" << (int)trace_data->access << ");" << endl;
      }
      break;
    case graph_eTrace_AnalogShift:
    case graph_eTrace_Animation:
    case graph_eTrace_AnimationForwBack:
    case graph_eTrace_DigAnimation:
    case graph_eTrace_DigShift:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[2] << "\",2);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl;
      }
      break;
    case graph_eTrace_IncrAnalog:
      if ( strcmp( trace_data->data[0], "") != 0)
      {
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[2] << "\",2);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[3] << "\",3);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl <<
"    " << var_name << ".dd.setAccess(" << (int)trace_data->access << ");" << endl;
      }
      break;
    case graph_eTrace_Move:
      if ( strcmp( trace_data->data[0], "") != 0 || 
           strcmp( trace_data->data[1], "") != 0 || 
           strcmp( trace_data->data[7], "") != 0 || 
           strcmp( trace_data->data[8], "") != 0)
      {
        double zoom_factor;

        grow_GetZoom( grow->ctx, &zoom_factor);
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[1] << "\",1);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[2] << "\",2);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[3] << "\",3);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[7] << "\",4);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[8] << "\",5);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl;
	if ( strcmp( trace_data->data[4], "") != 0)
          fp <<
"    " << var_name << ".dd.setFactor(new Double(\"" << trace_data->data[4] << "\").doubleValue() * " << zoom_factor << ");" << endl;
        else
          fp <<
"    " << var_name << ".dd.setFactor(" << zoom_factor << ");" << endl;
	if ( strcmp( trace_data->data[5], "") != 0)
          fp <<
"    " << var_name << ".dd.setX0(new Double(\"" << trace_data->data[5] << "\").doubleValue());" << endl;
	if ( strcmp( trace_data->data[6], "") != 0)
          fp <<
"    " << var_name << ".dd.setY0(new Double(\"" << trace_data->data[6] << "\").doubleValue());" << endl;
      }
      break;
    case graph_eTrace_Rotate:
    {
      if ( strcmp( trace_data->data[0], "") != 0 || 
           strcmp( trace_data->data[2], "") != 0 || 
           strcmp( trace_data->data[3], "") != 0)
      {
        double zoom_factor;
        double x0, x1, y0, y1;
        double rotation_x, rotation_y;
        int sts;

        if ( strcmp( trace_data->data[1], "") != 0  &&
             strcmp( trace_data->data[2], "") != 0) {
          sts = sscanf( trace_data->data[1], "%lf", &rotation_x);
          if ( sts != 1)
            rotation_x = 0;

          sts = sscanf( trace_data->data[2], "%lf", &rotation_y);
          if ( sts != 1)
            rotation_y = 0;

          grow_ToPixel( grow->ctx, rotation_x, rotation_y, 
		      &rotation_x, &rotation_y);
          grow_MeasureJavaBean( grow->ctx, &x1, &x0, &y1, &y0);

	  rotation_x -= x0 - glow_cJBean_Offset;
	  rotation_y -= y0 - glow_cJBean_Offset;
        }
        else {
	  // Zero point for nodeclass is rotation point 
	  grow_GetNodeClassOrigo( object, &rotation_x, &rotation_y);
          grow_MeasureNode( object, &x0, &y0, &x1, &y1);
          rotation_x += x0;
          rotation_y += y0;

          grow_ToPixel( grow->ctx, rotation_x, rotation_y, 
		      &rotation_x, &rotation_y);

          grow_MeasureJavaBean( grow->ctx, &x1, &x0, &y1, &y0);

	  rotation_x -= x0 - glow_cJBean_Offset;
	  rotation_y -= y0 - glow_cJBean_Offset;
        }


        grow_GetZoom( grow->ctx, &zoom_factor);
        fp <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[0] << "\",0);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[3] << "\",3);" << endl <<
"    " << var_name << ".dd.setData(\"" << trace_data->data[4] << "\",4);" << endl <<
"    " << var_name << ".dd.setType(" << (int)trace_type << ");" << endl <<
"    " << var_name << ".dd.setColor(" << (int)trace_color << ");" << endl;
	if ( strcmp( trace_data->data[5], "") != 0)
          fp <<
"    " << var_name << ".dd.setFactor(new Double(\"" << trace_data->data[5] << "\").doubleValue());" << endl;
        else
          fp <<
"    " << var_name << ".dd.setFactor(new Double(\"1\").doubleValue());" << endl;
        fp <<
"    " << var_name << ".dd.setX0(" << rotation_x << "F);" << endl <<
"    " << var_name << ".dd.setY0(" << rotation_y << "F);" << endl;
      }
      break;
    }
    default:
      ;
  }
  
  // Print annotations
  grow_GetObjectAnnotationNumbers( object, &numbers, &annot_cnt);

  for ( i = 0; i < annot_cnt; i++)
  {
    grow_GetAnnotation( object, numbers[i], annot_str, sizeof(annot_str));
    if ( strcmp( annot_str, "") != 0)
    {
        fp <<
"    " << var_name << ".setAnnot" << numbers[i] << "(\"" << annot_str << "\");" << endl;
    }
  }
  free( (char *) numbers);
  return 1;
}

int Graph::export_BarTraceAttr( ofstream& fp, grow_tObject object, int cnt)
{
  glow_sTraceData	*trace_data;
  char			class_name[] = "JopBar";
  char 			var_name[40];

  grow_GetTraceAttr( object, &trace_data);

  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", cnt);

  if ( strcmp( trace_data->data[0], "") != 0)
  {
    fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl;
  }
  return 1;
}

int Graph::export_TrendTraceAttr( ofstream& fp, grow_tObject object, int cnt)
{
  glow_sTraceData	*trace_data;
  char			class_name[] = "JopTrend";
  char 			var_name[40];

  grow_GetTraceAttr( object, &trace_data);

  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", cnt);

  if ( strcmp( trace_data->data[0], "") != 0)
  {
    if ( strcmp( trace_data->data[0], "") != 0)
      fp <<
"    " << var_name << ".setPwrAttribute1(\"" << trace_data->data[0] << "\");" << endl;
    if ( strcmp( trace_data->data[1], "") != 0)
      fp <<
"    " << var_name << ".setPwrAttribute2(\"" << trace_data->data[1] << "\");" << endl;
  }
  return 1;
}

int Graph::export_SliderTraceAttr( ofstream& fp, grow_tObject object, int cnt)
{
  glow_sTraceData	*trace_data;
  char			class_name[40];
  char 			var_name[40];

  grow_GetTraceAttr( object, &trace_data);
  grow_GetObjectClassJavaName( object, class_name);

  strcpy( var_name, class_name);
  var_name[0] = _tolower(var_name[0]);
  sprintf( &var_name[strlen(var_name)], "%d", cnt);

  if ( strcmp( trace_data->data[0], "") != 0)
  {
    if ( strcmp( trace_data->data[0], "") != 0)
      fp <<
"    " << var_name << ".setPwrAttribute(\"" << trace_data->data[0] << "\");" << endl <<
"    " << var_name << ".setAccess(" << (int)trace_data->access << ");" << endl;
  }
  return 1;
}

// Replace " to \"
static char *cmd_cnv( char *instr)
{
  static char outstr[200];
  char *in;
  char *out = outstr;

  for ( in = instr; *in != 0; in++)
  {
    if ( *in == '"')
      *out++ = '\\';
    *out++ = *in;
  }
  *out = *in;
  return outstr;
}

