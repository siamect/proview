package jpwr.beans;
import jpwr.jop.*;
import java.beans.*;
public class JopTextFieldBeanInfo extends SimpleBeanInfo {
  Class beanClass = JopTextField.class;
  String iconColor16x16Filename = "joptextfield16.gif";
  String iconColor32x32Filename = "joptextfield32.gif";
  String iconMono16x16Filename;
  String iconMono32x32Filename;

  public JopTextFieldBeanInfo() {
  }
  public PropertyDescriptor[] getPropertyDescriptors() {
    try {
      PropertyDescriptor _lowColor = new PropertyDescriptor("lowColor",
        beanClass, "getLowColor", "setLowColor");
      _lowColor.setDisplayName("lowColor");
      _lowColor.setShortDescription("lowColor");
      _lowColor.setPropertyEditorClass(GeColorEditor.class);
      PropertyDescriptor _pwrAttribute = new PropertyDescriptor("pwrAttribute",
        beanClass, "getPwrAttribute", "setPwrAttribute");
      _pwrAttribute.setDisplayName("pwrAttribute");
      _pwrAttribute.setShortDescription("pwrAttribute");
      PropertyDescriptor _pwrAttrColor = new PropertyDescriptor("pwrAttrColor",
        beanClass, "getPwrAttrColor", "setPwrAttrColor");
      _pwrAttrColor.setDisplayName("pwrAttrColor");
      _pwrAttrColor.setShortDescription("pwrAttrColor");
      PropertyDescriptor _maximumValue = new PropertyDescriptor("maximumValue",
        beanClass, "getMaximumValue", "setMaximumValue");
      _maximumValue.setDisplayName("maximumValue");
      _maximumValue.setShortDescription("maximumValue");
      PropertyDescriptor _minimumValue = new PropertyDescriptor("minimumValue",
        beanClass, "getMinimumValue", "setMinimumValue");
      _minimumValue.setDisplayName("minimumValue");
      _minimumValue.setShortDescription("minimumValue");
      PropertyDescriptor _alarmLimitHighValue = new PropertyDescriptor("alarmLimitHighValue",
        beanClass, "getAlarmLimitHighValue", "setAlarmLimitHighValue");
      _alarmLimitHighValue.setDisplayName("alarmLimitHighValue");
      _alarmLimitHighValue.setShortDescription("alarmLimitHighValue");
      PropertyDescriptor _alarmLimitLowValue = new PropertyDescriptor("alarmLimitLowValue",
        beanClass, "getAlarmLimitLowValue", "setAlarmLimitLowValue");
      _alarmLimitLowValue.setDisplayName("alarmLimitLowValue");
      _alarmLimitLowValue.setShortDescription("alarmLimitLowValue");
      PropertyDescriptor _alarmColor = new PropertyDescriptor("alarmColor",
        beanClass, "getAlarmColor", "setAlarmColor");
      _alarmColor.setDisplayName("alarmColor");
      _alarmColor.setShortDescription("alarmLimitLowValue");
      PropertyDescriptor _format = new PropertyDescriptor("format",
        beanClass, "getFormat", "setFormat");
      _format.setDisplayName("format");
      _format.setShortDescription("format");
      PropertyDescriptor _access = new PropertyDescriptor("access",
        beanClass, "getAccess", "setAccess");
      _access.setDisplayName("access");
      _access.setShortDescription("access");
      PropertyDescriptor[] pds = new PropertyDescriptor[] {
	_lowColor, 
	_pwrAttribute, _pwrAttrColor, _maximumValue, _minimumValue, _alarmLimitHighValue, 
	_alarmLimitLowValue, _alarmColor, _format, _access};
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
      return new BeanInfo[] { superBeanInfo };
    }
    catch(IntrospectionException ex) {
      ex.printStackTrace();
      return null;
    }
  }
}
