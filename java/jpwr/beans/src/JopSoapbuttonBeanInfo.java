package jpwr.beans;
import jpwr.jop.*;
import java.beans.*;
public class JopSoapbuttonBeanInfo extends SimpleBeanInfo {
  Class beanClass = JopSoapbutton.class;
  String iconColor16x16Filename = "jopsoapbutton16.gif";
  String iconColor32x32Filename = "jopsoapbutton32.gif";
  String iconMono16x16Filename;
  String iconMono32x32Filename;

  public JopSoapbuttonBeanInfo() {
  }
  public PropertyDescriptor[] getPropertyDescriptors() {
    try {
      PropertyDescriptor _toolTipText = new PropertyDescriptor("toolTipText",
        beanClass, "getToolTipText", "setToolTipText");
      _toolTipText.setDisplayName("toolTipText");
      _toolTipText.setShortDescription("toolTipText");
      PropertyDescriptor _fillColor = new PropertyDescriptor("fillColor",
        beanClass, "getFillColor", "setFillColor");
      _fillColor.setDisplayName("fillColor");
      _fillColor.setShortDescription("fillColor");
      _fillColor.setPropertyEditorClass(GeColorEditor.class);
      PropertyDescriptor _borderColor = new PropertyDescriptor("borderColor",
        beanClass, "getBorderColor", "setBorderColor");
      _borderColor.setDisplayName("borderColor");
      _borderColor.setShortDescription("borderColor");
      _borderColor.setPropertyEditorClass(GeColorEditor.class);
      PropertyDescriptor _colorTone = new PropertyDescriptor("colorTone",
        beanClass, "getColorTone", "setColorTone");
      _colorTone.setDisplayName("colorTone");
      _colorTone.setShortDescription("colorTone");
      _colorTone.setPropertyEditorClass(GeColorToneEditor.class);
      PropertyDescriptor _colorShift = new PropertyDescriptor("colorShift",
        beanClass, "getColorShift", "setColorShift");
      _colorShift.setDisplayName("colorShift");
      _colorShift.setShortDescription("colorShift");
      _colorShift.setPropertyEditorClass(GeColorShiftEditor.class);
      PropertyDescriptor _colorBrightness = new PropertyDescriptor("colorBrightness",
        beanClass, "getColorBrightness", "setColorBrightness");
      _colorBrightness.setDisplayName("colorBrightness");
      _colorBrightness.setShortDescription("colorBrightness");
      _colorBrightness.setPropertyEditorClass(GeColorBrightnessEditor.class);
      PropertyDescriptor _colorIntensity = new PropertyDescriptor("colorIntensity",
        beanClass, "getColorIntensity", "setColorIntensity");
      _colorIntensity.setDisplayName("colorIntensity");
      _colorIntensity.setShortDescription("colorIntensity");
      _colorIntensity.setPropertyEditorClass(GeColorIntensityEditor.class);
      PropertyDescriptor _rotate = new PropertyDescriptor("rotate",
        beanClass, "getRotate", "setRotate");
      _rotate.setDisplayName("rotate");
      _rotate.setShortDescription("rotate");
      PropertyDescriptor _clickAction = new PropertyDescriptor("clickAction",
        beanClass, "getClickAction", "setClickAction");
      _clickAction.setDisplayName("clickAction");
      _clickAction.setShortDescription("clickAction");
      _clickAction.setPropertyEditorClass(ClickActionEditor.class);
      PropertyDescriptor _command = new PropertyDescriptor("command",
        beanClass, "getCommand", "setCommand");
      _command.setDisplayName("command");
      _command.setShortDescription("command");
      PropertyDescriptor _lowTone = new PropertyDescriptor("lowTone",
        beanClass, "getLowTone", "setLowTone");
      _lowTone.setDisplayName("lowTone");
      _lowTone.setShortDescription("lowTone");
      _lowTone.setPropertyEditorClass(GeColorToneEditor.class);
      PropertyDescriptor _pwrAttribute = new PropertyDescriptor("pwrAttribute",
        beanClass, "getPwrAttribute", "setPwrAttribute");
      _pwrAttribute.setDisplayName("pwrAttribute");
      _pwrAttribute.setShortDescription("pwrAttribute");
      PropertyDescriptor _pwrAttrColor = new PropertyDescriptor("pwrAttrColor",
        beanClass, "getPwrAttrColor", "setPwrAttrColor");
      _pwrAttrColor.setDisplayName("pwrAttrColor");
      _pwrAttrColor.setShortDescription("pwrAttColor");
      PropertyDescriptor _pwrAttrText = new PropertyDescriptor("pwrAttrText",
        beanClass, "getPwrAttrText", "setPwrAttrText");
      _pwrAttrText.setDisplayName("pwrAttrText");
      _pwrAttrText.setShortDescription("pwrAttText");
      PropertyDescriptor _textLow = new PropertyDescriptor("textLow",
        beanClass, "getTextLow", "setTextLow");
      _textLow.setDisplayName("textLow");
      _textLow.setShortDescription("textLow");
      PropertyDescriptor _textHigh = new PropertyDescriptor("textHigh",
        beanClass, "getTextHigh", "setTextHigh");
      _textHigh.setDisplayName("textHigh");
      _textHigh.setShortDescription("textHigh");
      PropertyDescriptor _access = new PropertyDescriptor("access",
        beanClass, "getAccess", "setAccess");
      _access.setDisplayName("access");
      _access.setShortDescription("access");
      PropertyDescriptor _confirm = new PropertyDescriptor("confirm",
        beanClass, "getConfirm", "setConfirm");
      _confirm.setDisplayName("confirm");
      _confirm.setShortDescription("confirm");
      PropertyDescriptor _confirmText = new PropertyDescriptor("confirmText",
        beanClass, "getConfirmText", "setConfirmText");
      _confirmText.setDisplayName("confirmText");
      _confirmText.setShortDescription("confirmText");
      PropertyDescriptor _annot1 = new PropertyDescriptor("annot1",
        beanClass, "getAnnot1", "setAnnot1");
      _annot1.setDisplayName("annot1");
      _annot1.setShortDescription("annot1");
      PropertyDescriptor _annot1Font = new PropertyDescriptor("annot1Font",
        beanClass, "getAnnot1Font", "setAnnot1Font");
      _annot1Font.setDisplayName("annot1Font");
      _annot1Font.setShortDescription("annot1Font");
      PropertyDescriptor[] pds = new PropertyDescriptor[] {
	_toolTipText, _fillColor, _borderColor, _colorTone, _colorShift,
	_colorBrightness, _colorIntensity, _rotate,
	_clickAction, _command, _lowTone, _pwrAttribute, _pwrAttrColor,
	_pwrAttrText, _textLow, _textHigh, _access, _confirm, _confirmText, _annot1, _annot1Font};
      return pds;

    }
    catch(IntrospectionException ex) {
      ex.printStackTrace();
      return null;
    }
  }
  public java.awt.Image getIcon(int iconKind) {
    switch (iconKind) {
    case BeanInfo.ICON_COLOR_16x16:
        return iconColor16x16Filename != null ? loadImage(iconColor16x16Filename) : null;
    case BeanInfo.ICON_COLOR_32x32:
        return iconColor32x32Filename != null ? loadImage(iconColor32x32Filename) : null;
    case BeanInfo.ICON_MONO_16x16:
        return iconMono16x16Filename != null ? loadImage(iconMono16x16Filename) : null;
    case BeanInfo.ICON_MONO_32x32:
        return iconMono32x32Filename != null ? loadImage(iconMono32x32Filename) : null;
        }
    return null;
  }
  public BeanInfo[] getAdditionalBeanInfo() {
    Class superclass = beanClass.getSuperclass();
    try {
      BeanInfo superBeanInfo = Introspector.getBeanInfo(superclass);
//    return new BeanInfo[] { superBeanInfo };
      return null;
    }
    catch(IntrospectionException ex) {
      ex.printStackTrace();
      return null;
    }
  }
}
