/* 
 * Proview   $Id: JopValvemanuelBeanInfo.java,v 1.2 2005-09-01 14:57:50 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

package jpwr.beans;
import jpwr.jop.*;
import java.beans.*;
public class JopValvemanuelBeanInfo extends SimpleBeanInfo {
  Class beanClass = JopValvemanuel.class;
  String iconColor16x16Filename = "jopvalvemanuel16.gif";
  String iconColor32x32Filename = "jopvalvemanuel32.gif";
  String iconMono16x16Filename;
  String iconMono32x32Filename;

  public JopValvemanuelBeanInfo() {
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
      PropertyDescriptor _lowColor = new PropertyDescriptor("lowColor",
        beanClass, "getLowColor", "setLowColor");
      _lowColor.setDisplayName("lowColor");
      _lowColor.setShortDescription("lowColor");
      _lowColor.setPropertyEditorClass(GeColorEditor.class);
      PropertyDescriptor _pwrAttribute = new PropertyDescriptor("pwrAttribute",
        beanClass, "getPwrAttribute", "setPwrAttribute");
      _pwrAttribute.setDisplayName("pwrAttribute");
      _pwrAttribute.setShortDescription("pwrAttribute");
      PropertyDescriptor _pwrAttrError = new PropertyDescriptor("pwrAttrError",
        beanClass, "getPwrAttrError", "setPwrAttrError");
      _pwrAttrError.setDisplayName("pwrAttrError");
      _pwrAttrError.setShortDescription("pwrAttError");
      PropertyDescriptor[] pds = new PropertyDescriptor[] {
	_toolTipText, _fillColor, _borderColor, _colorTone, _colorShift,
	_colorBrightness, _colorIntensity, _rotate,
	_lowColor, _pwrAttribute, _pwrAttrError};
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
