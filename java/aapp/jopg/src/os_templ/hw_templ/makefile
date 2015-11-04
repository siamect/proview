include $(pwre_dir_symbols)

extern_java_sources := \
	CcmApplIfc.java, \
	CcmFloatvar.java, \
	CcmIntvar.java, \
	CcmStringvar.java, \
	CcmLine.java, \
	CcmReturn.java, \
	CcmFileReturn.java, \
	CcmArg.java, \
	CcmVariable.java, \
	CcmFunc.java, \
	CcmOperand.java, \
	CcmFuncCtx.java, \
	CcmFileCtx.java, \
	CcmRowCtx.java, \
	CcmSysFunc.java, \
	Ccm.java, \
	JopgCcm.java, \
	Glow.java,\
	GlowCrypt.java, \
	PrintfFormat.java,\
	GlowCFormat.java,\
	GlowFillLevelLimits.java,\
	GlowGeometry.java,\
	GlowPointX.java,\
	GlowPointDX.java,\
	GlowDimension.java,\
	GlowDimensionD.java,\
	GlowSliderInfo.java,\
	GlowTableInfo.java,\
	GlowMenuInfo.java,\
	GlowPieInfo.java,\
	GlowBarChartInfo.java,\
	GlowTransform.java,\
	GlowEvent.java,\
	GlowArrayElem.java,\
	GlowPoint.java,\
	GrowApplIfc.java,\
	GlowColorNode.java,\
	GlowColorRgb.java,\
	GlowColor.java,\
	GlowCustomColors.java,\
	GlowBackgroundObject.java,\
	GlowEventMenu.java,\
	GlowEventToolbar.java,\
	GlowEventTable.java,\
	GrowCtxIfc.java,\
	GrowCmn.java,\
	GrowAnnotIfc.java,\
	GlowNodeClass.java,\
	GlowNodeGroup.java,\
	GlowConClass.java,\
	GlowConPoint.java,\
	GrowNode.java,\
	GlowLine.java,\
	GlowRect.java,\
	GlowText.java,\
	GlowArc.java,\
	GlowArrow.java,\
	GlowCon.java,\
	GrowRect.java,\
	GrowRectRounded.java,\
	GrowGroup.java,\
	GrowSlider.java,\
	GrowAxis.java,\
	GrowAxisArc.java,\
	GrowArc.java,\
	GrowLine.java,\
	GrowPolyline.java,\
	GrowText.java,\
	GrowAnnot.java,\
	GrowConPoint.java,\
	GrowMenu.java,\
	GrowBar.java,\
	GrowTrend.java,\
	GrowScrollBarIfc.java,\
	GrowScrollBar.java,\
	GrowXYCurve.java,\
	GrowTable.java,\
	GrowImage.java,\
	GrowWindow.java,\
	GrowFolder.java,\
	GrowPie.java,\
	GrowBarChart.java,\
	GrowToolbar.java,\
	GrowConGlue.java,\
	GlowVector.java,\
	GrowCtx.java,\
	DynParsedAttrName.java,\
	GraphLocalDb.java,\
	GraphIfc.java,\
	Dyn.java,\
	GraphApplIfc.java,\
	Graph.java,\
	GrowFrameApplIfc.java

-include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

ifeq ($($(type_name)_generic_mk),)
 include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk
endif

dirs : $(app_package_dir)/$(comp_name)$(dir_ext)
 
$(app_package_dir)/$(comp_name)$(dir_ext) :
	@ echo "Create directory " $(app_package_dir)/$(comp_name)$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_package_dir)/$(comp_name)$(dir_ext)


-include ../../special.mk
-include ../special.mk
-include special.mk



