include $(pwre_dir_symbols)

local_java_sources := \
	Ge.java \
	GeCFormat.java \
	GeDyndata.java \
	GeColor.java \
	JopDynamic.java \
	JopEngine.java \
	JopSessionIfc.java \
	JopUtilityIfc.java \
	JopUtility.java \
	JopSession.java \
	JopMethodsMenu.java \
	JopConfirm.java \
	JopConfirmDialog.java \
	JopFrame.java \
	JopApplet.java \
	JopSpider.java \
	Jop.java \
	GeComponent.java \
	GeTextField.java \
	GeSlider.java \
	JopBar.java \
	JopTrend.java \
	JopAxis.java \
	JopSpiderFrame.java \
	JopLoginFrame.java \
	JopLoginApplet.java \
	GeColorEditor.java \
	GeColorBrightnessEditor.java \
	GeColorToneEditor.java \
	GeColorShiftEditor.java \
	GeColorIntensityEditor.java \
	ClickActionEditor.java \
        JopUtilities.java \
        GeImage.java \
	GeFrameThin.java \
	Flow.java \
	FlowCtxInterface.java \
	FlowCmn.java \
	FlowComponent.java \
	FlowPoint.java \
	FlowArrayElem.java \
	FlowRect.java \
	FlowLine.java \
	FlowText.java \
	FlowArc.java \
	FlowArrow.java \
	FlowAnnot.java \
	FlowConPoint.java \
	FlowArray.java \
	FlowNodeClass.java \
	FlowConClass.java \
	FlowNode.java \
	FlowCon.java \
	FlowVector.java \
	FlowCtx.java \
	FlowFrame.java \
	CrrFrame.java \
	JopViewport.java \
	JopScrollPane.java \
	Logg.java \
	TreeObj.java \
	AttrObj.java \
	XttObjAttr.java \
	DynamicObj.java \
	XttRefObj.java \
	XttArrayAttr.java \
	XttObj.java \
	XttTree.java \
	JopXttApplet.java \
	JopXttFrame.java \
	MhTable.java \
	MhClient.java \
        MhFrame.java \
        JopOpWindow.java \
        JopOpWindowFrame.java \
        JopOpWindowApplet.java \
	JopSessionRep.java

-include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

ifeq ($($(type_name)_generic_mk),)
 include $(pwre_sroot)/tools/bld/src/$(type_name)_generic.mk
endif

-include ../../special.mk
-include ../special.mk
-include special.mk



