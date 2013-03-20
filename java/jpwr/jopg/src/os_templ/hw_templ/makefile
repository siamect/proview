include $(pwre_dir_symbols)

local_java_sources := \
	Glow.java,\
	PrintfFormat.java,\
	GlowCFormat.java,\
	GlowEvent.java,\
	GlowPointX.java,\
	GlowDimension.java,\
	GrowApplIfc.java,\
	GlowColorRgb.java,\
	GlowColor.java,\
	GlowDrawIfc.java,\
	GlowDraw.java,\
	GrowNodeIfc.java,\
	GlowArrayElem.java,\
	GrowCtxIfc.java,\
	GrowCmn.java,\
	GlowTransform.java,\
	GlowPoint.java,\
	GlowNodeClass.java,\
	GlowNodeGroup.java,\
	GlowConClass.java,\
	GlowConPoint.java,\
	GrowNode.java,\
	GrowGroup.java,\
	GrowRect.java,\
	GrowArc.java,\
	GrowLine.java,\
	GrowPolyline.java,\
	GrowText.java,\
	GrowAnnot.java,\
	GlowVector.java,\
	GrowCtx.java,\
	DynParsedAttrName.java,\
	GraphLocalDb.java,\
	GraphIfc.java,\
	Dyn.java,\
	GraphApplIfc.java,\
	Graph.java,\
	GrowFrame.java

-include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

ifeq ($($(type_name)_generic_mk),)
 include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk
endif

-include ../../special.mk
-include ../special.mk
-include special.mk



