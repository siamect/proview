package jpwr.beans;
import jpwr.jop.*;
import java.beans.*;
public class JopImageBeanInfo extends SimpleBeanInfo {
  Class beanClass = JopImage.class;
  String iconColor16x16Filename = "jopimage16.gif";
  String iconColor32x32Filename = "jopimage32.gif";
  String iconMono16x16Filename;
  String iconMono32x32Filename;

  public JopImageBeanInfo() {
  }
  public PropertyDescriptor[] getPropertyDescriptors() {
    try {
      PropertyDescriptor _image = new PropertyDescriptor("image",
        beanClass, "getImage", "setImage");
      _image.setDisplayName("image");
      _image.setShortDescription("image");
      PropertyDescriptor _toolTipText = new PropertyDescriptor("toolTipText",
        beanClass, "getToolTipText", "setToolTipText");
      _toolTipText.setDisplayName("toolTipText");
      _toolTipText.setShortDescription("toolTipText");
      PropertyDescriptor[] pds = new PropertyDescriptor[] {
	_image, _toolTipText};
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
