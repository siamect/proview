package jpwr.jop;
import java.beans.*;
public class JopDynamicBeanInfo extends SimpleBeanInfo {
  Class beanClass = JopDynamic.class;

  public JopDynamicBeanInfo() {
  }
  public PropertyDescriptor[] getPropertyDescriptors() {
      PropertyDescriptor[] pds = new PropertyDescriptor[] {};
      return pds;
  }
  public java.awt.Image getIcon(int iconKind) {
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
