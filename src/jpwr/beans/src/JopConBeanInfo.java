package jpwr.beans;
import jpwr.jop.*;
import java.beans.*;
public class JopConBeanInfo extends SimpleBeanInfo {
  Class beanClass = JopCon.class;
  String iconColor16x16Filename = "jopcon16.gif";
  String iconColor32x32Filename = "jopcon32.gif";
  String iconMono16x16Filename;
  String iconMono32x32Filename;

  public JopConBeanInfo() {
  }
  public PropertyDescriptor[] getPropertyDescriptors() {
    try {
      PropertyDescriptor _borderColor = new PropertyDescriptor("borderColor",
        beanClass, "getBorderColor", "setBorderColor");
      _borderColor.setDisplayName("borderColor");
      _borderColor.setShortDescription("borderColor");
      _borderColor.setPropertyEditorClass(GeColorEditor.class);
      PropertyDescriptor _lineWidth = new PropertyDescriptor("lineWidth",
        beanClass, "getLineWidth", "setLineWidth");
      _lineWidth.setDisplayName("lineWidth");
      _lineWidth.setShortDescription("lineWidth");
      PropertyDescriptor _rotate = new PropertyDescriptor("rotate",
        beanClass, "getRotate", "setRotate");
      _rotate.setDisplayName("rotate");
      _rotate.setShortDescription("rotate");
      PropertyDescriptor[] pds = new PropertyDescriptor[] {
	_borderColor, _lineWidth, _rotate};
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
//      return new BeanInfo[] { superBeanInfo };
      return null;
    }
    catch(IntrospectionException ex) {
      ex.printStackTrace();
      return null;
    }
  }
}
