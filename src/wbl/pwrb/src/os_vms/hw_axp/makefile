#  makefile -- <short description>
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>.
#
include $(pwre_dir_symbols)

ifndef variables_mk
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_sroot)/tools/bld/src/$(os_name)/variables.mk
endif


ifndef rules_mk
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_sroot)/tools/bld/src/$(os_name)/rules.mk
endif



vpath %.wb_load $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

pwrb_volumes := \
	$(cnf_dir)/pwrb_v.wb_vers \
	$(cnf_dir)/wb_v.wb_vers

pwrb_hierarchies := \
	$(cnf_dir)/pwrb_ch_baseclasses.wb_vers \
	$(cnf_dir)/pwrb_ch_basetypes.wb_vers

pwrb_layouts := \
	$(cnf_dir)/pwrb_l_base.wb_vers \
	$(cnf_dir)/pwrb_l_lists.wb_vers \
	$(cnf_dir)/pwrb_l_simul.wb_vers

pwrb_classes := \
	$(cnf_dir)/pwrb_c_aanalyse.wb_vers \
	$(cnf_dir)/pwrb_c_aarithm.wb_vers \
	$(cnf_dir)/pwrb_c_adelay.wb_vers \
	$(cnf_dir)/pwrb_c_ai.wb_vers \
	$(cnf_dir)/pwrb_c_ai_7436.wb_vers \
	$(cnf_dir)/pwrb_c_ai_ai32up.wb_vers \
	$(cnf_dir)/pwrb_c_ai_hvai32.wb_vers \
	$(cnf_dir)/pwrb_c_aiarea.wb_vers \
	$(cnf_dir)/pwrb_c_ai_vmic3113a.wb_vers \
	$(cnf_dir)/pwrb_c_and.wb_vers \
	$(cnf_dir)/pwrb_c_ao.wb_vers \
	$(cnf_dir)/pwrb_c_ao_7455_20.wb_vers \
	$(cnf_dir)/pwrb_c_ao_7455_30.wb_vers \
	$(cnf_dir)/pwrb_c_ao_hvao4.wb_vers \
	$(cnf_dir)/pwrb_c_aoarea.wb_vers \
	$(cnf_dir)/pwrb_c_ao_vmic4800.wb_vers \
	$(cnf_dir)/pwrb_c_apcollect.wb_vers \
	$(cnf_dir)/pwrb_c_apdistribute.wb_vers \
	$(cnf_dir)/pwrb_c_appldistribute.wb_vers \
	$(cnf_dir)/pwrb_c_asup.wb_vers \
	$(cnf_dir)/pwrb_c_av.wb_vers \
	$(cnf_dir)/pwrb_c_avarea.wb_vers \
	$(cnf_dir)/pwrb_c_backup.wb_vers \
	$(cnf_dir)/pwrb_c_backup_conf.wb_vers \
	$(cnf_dir)/pwrb_c_bcddo.wb_vers \
	$(cnf_dir)/pwrb_c_bodytext.wb_vers \
	$(cnf_dir)/pwrb_c_carithm.wb_vers \
	$(cnf_dir)/pwrb_c_caarea.wb_vers \
	$(cnf_dir)/pwrb_c_chanai.wb_vers \
	$(cnf_dir)/pwrb_c_chanait.wb_vers \
	$(cnf_dir)/pwrb_c_chanao.wb_vers \
	$(cnf_dir)/pwrb_c_chanco.wb_vers \
	$(cnf_dir)/pwrb_c_chandi.wb_vers \
	$(cnf_dir)/pwrb_c_chando.wb_vers \
	$(cnf_dir)/pwrb_c_classvolumeconfig.wb_vers \
	$(cnf_dir)/pwrb_c_classvolumeload.wb_vers \
	$(cnf_dir)/pwrb_c_co.wb_vers \
	$(cnf_dir)/pwrb_c_commonclassdistribute.wb_vers \
	$(cnf_dir)/pwrb_c_comph.wb_vers \
	$(cnf_dir)/pwrb_c_compl.wb_vers \
	$(cnf_dir)/pwrb_c_conanalog.wb_vers \
	$(cnf_dir)/pwrb_c_conbluestronearr.wb_vers \
	$(cnf_dir)/pwrb_c_condamenoarr.wb_vers \
	$(cnf_dir)/pwrb_c_condameonearr.wb_vers \
	$(cnf_dir)/pwrb_c_condametwoarr.wb_vers \
	$(cnf_dir)/pwrb_c_condata.wb_vers \
	$(cnf_dir)/pwrb_c_condatafeedback.wb_vers \
	$(cnf_dir)/pwrb_c_condathinnoarr.wb_vers \
	$(cnf_dir)/pwrb_c_condathinonearr.wb_vers \
	$(cnf_dir)/pwrb_c_condathintwoarr.wb_vers \
	$(cnf_dir)/pwrb_c_condigital.wb_vers \
	$(cnf_dir)/pwrb_c_conexecuteorder.wb_vers \
	$(cnf_dir)/pwrb_c_confeedbackanalog.wb_vers \
	$(cnf_dir)/pwrb_c_confeedbackdigital.wb_vers \
	$(cnf_dir)/pwrb_c_congrafcet.wb_vers \
	$(cnf_dir)/pwrb_c_congreentwoarr.wb_vers \
	$(cnf_dir)/pwrb_c_conmenoarr.wb_vers \
	$(cnf_dir)/pwrb_c_conmeonearr.wb_vers \
	$(cnf_dir)/pwrb_c_conmetwoarr.wb_vers \
	$(cnf_dir)/pwrb_c_conremenoarr.wb_vers \
	$(cnf_dir)/pwrb_c_conremeonearr.wb_vers \
	$(cnf_dir)/pwrb_c_conremetwoarr.wb_vers \
	$(cnf_dir)/pwrb_c_constrmenoarr.wb_vers \
	$(cnf_dir)/pwrb_c_conthinnoarr.wb_vers \
	$(cnf_dir)/pwrb_c_conthinonearr.wb_vers \
	$(cnf_dir)/pwrb_c_conthintwoarr.wb_vers \
	$(cnf_dir)/pwrb_c_contrace.wb_vers \
	$(cnf_dir)/pwrb_c_corder.wb_vers \
	$(cnf_dir)/pwrb_c_count.wb_vers \
	$(cnf_dir)/pwrb_c_co_7435_33.wb_vers \
	$(cnf_dir)/pwrb_c_co_7437_33.wb_vers \
	$(cnf_dir)/pwrb_c_co_ipquadrature.wb_vers \
	$(cnf_dir)/pwrb_c_co_pi24bo.wb_vers \
	$(cnf_dir)/pwrb_c_coarea.wb_vers \
	$(cnf_dir)/pwrb_c_cstoai.wb_vers \
	$(cnf_dir)/pwrb_c_cstoao.wb_vers \
	$(cnf_dir)/pwrb_c_cstoap.wb_vers \
	$(cnf_dir)/pwrb_c_cstoav.wb_vers \
	$(cnf_dir)/pwrb_c_cstoip.wb_vers \
	$(cnf_dir)/pwrb_c_csub.wb_vers \
	$(cnf_dir)/pwrb_c_curve.wb_vers \
	$(cnf_dir)/pwrb_c_cyclesup.wb_vers \
	$(cnf_dir)/pwrb_c_danalyse.wb_vers \
	$(cnf_dir)/pwrb_c_dataarithm.wb_vers \
	$(cnf_dir)/pwrb_c_datacollect.wb_vers \
	$(cnf_dir)/pwrb_c_darithm.wb_vers \
	$(cnf_dir)/pwrb_c_dbconfig.wb_vers \
	$(cnf_dir)/pwrb_c_dbdistribute.wb_vers \
	$(cnf_dir)/pwrb_c_di.wb_vers \
	$(cnf_dir)/pwrb_c_dibcd.wb_vers \
	$(cnf_dir)/pwrb_c_di_7437_37.wb_vers \
	$(cnf_dir)/pwrb_c_di_dx2.wb_vers \
	$(cnf_dir)/pwrb_c_diarea.wb_vers \
	$(cnf_dir)/pwrb_c_directorydbconfig.wb_vers \
	$(cnf_dir)/pwrb_c_do.wb_vers \
	$(cnf_dir)/pwrb_c_document.wb_vers \
	$(cnf_dir)/pwrb_c_docuser1.wb_vers \
	$(cnf_dir)/pwrb_c_docuser2.wb_vers \
	$(cnf_dir)/pwrb_c_dorder.wb_vers \
	$(cnf_dir)/pwrb_c_do_7437_83.wb_vers \
	$(cnf_dir)/pwrb_c_do_hvdo32.wb_vers \
	$(cnf_dir)/pwrb_c_doarea.wb_vers \
	$(cnf_dir)/pwrb_c_dpcollect.wb_vers \
	$(cnf_dir)/pwrb_c_dpdistribute.wb_vers \
	$(cnf_dir)/pwrb_c_drive.wb_vers \
	$(cnf_dir)/pwrb_c_dsfast.wb_vers \
	$(cnf_dir)/pwrb_c_dsfastconf.wb_vers \
	$(cnf_dir)/pwrb_c_dshist.wb_vers \
	$(cnf_dir)/pwrb_c_dshistserver.wb_vers \
	$(cnf_dir)/pwrb_c_dstrend.wb_vers \
	$(cnf_dir)/pwrb_c_dstrendconf.wb_vers \
	$(cnf_dir)/pwrb_c_dsup.wb_vers \
	$(cnf_dir)/pwrb_c_dv.wb_vers \
	$(cnf_dir)/pwrb_c_dvarea.wb_vers \
	$(cnf_dir)/pwrb_c_edge.wb_vers \
	$(cnf_dir)/pwrb_c_eventprinter.wb_vers \
	$(cnf_dir)/pwrb_c_externref.wb_vers \
	$(cnf_dir)/pwrb_c_filter.wb_vers \
	$(cnf_dir)/pwrb_c_form.wb_vers \
	$(cnf_dir)/pwrb_c_frame.wb_vers \
	$(cnf_dir)/pwrb_c_friendnodeconfig.wb_vers \
	$(cnf_dir)/pwrb_c_getageneric.wb_vers \
	$(cnf_dir)/pwrb_c_getai.wb_vers \
	$(cnf_dir)/pwrb_c_getao.wb_vers \
	$(cnf_dir)/pwrb_c_getap.wb_vers \
	$(cnf_dir)/pwrb_c_getav.wb_vers \
	$(cnf_dir)/pwrb_c_getdata.wb_vers \
	$(cnf_dir)/pwrb_c_getdgeneric.wb_vers \
	$(cnf_dir)/pwrb_c_getdi.wb_vers \
	$(cnf_dir)/pwrb_c_getdo.wb_vers \
	$(cnf_dir)/pwrb_c_getdp.wb_vers \
	$(cnf_dir)/pwrb_c_getdv.wb_vers \
	$(cnf_dir)/pwrb_c_getip.wb_vers \
	$(cnf_dir)/pwrb_c_getpi.wb_vers \
	$(cnf_dir)/pwrb_c_graph.wb_vers \
	$(cnf_dir)/pwrb_c_graphdistribute.wb_vers \
	$(cnf_dir)/pwrb_c_gray.wb_vers \
	$(cnf_dir)/pwrb_c_group.wb_vers \
	$(cnf_dir)/pwrb_c_head.wb_vers \
	$(cnf_dir)/pwrb_c_inc3p.wb_vers \
	$(cnf_dir)/pwrb_c_initstep.wb_vers \
	$(cnf_dir)/pwrb_c_inv.wb_vers \
	$(cnf_dir)/pwrb_c_iohandler.wb_vers \
	$(cnf_dir)/pwrb_c_ipmux.wb_vers \
	$(cnf_dir)/pwrb_c_ivarea.wb_vers \
	$(cnf_dir)/pwrb_c_limit.wb_vers \
	$(cnf_dir)/pwrb_c_line_lat.wb_vers \
	$(cnf_dir)/pwrb_c_listconfig.wb_vers \
	$(cnf_dir)/pwrb_c_listdescriptor.wb_vers \
	$(cnf_dir)/pwrb_c_lorder.wb_vers \
	$(cnf_dir)/pwrb_c_maxmin.wb_vers \
	$(cnf_dir)/pwrb_c_messagehandler.wb_vers \
	$(cnf_dir)/pwrb_c_mode.wb_vers \
	$(cnf_dir)/pwrb_c_muxmod.wb_vers \
	$(cnf_dir)/pwrb_c_mvalve.wb_vers \
	$(cnf_dir)/pwrb_c_nodeconfig.wb_vers \
	$(cnf_dir)/pwrb_c_nodelinksup.wb_vers \
	$(cnf_dir)/pwrb_c_opappl.wb_vers \
	$(cnf_dir)/pwrb_c_opapplmsg.wb_vers \
	$(cnf_dir)/pwrb_c_opplace.wb_vers \
	$(cnf_dir)/pwrb_c_or.wb_vers \
	$(cnf_dir)/pwrb_c_order.wb_vers \
	$(cnf_dir)/pwrb_c_orderact.wb_vers \
	$(cnf_dir)/pwrb_c_out2p.wb_vers \
	$(cnf_dir)/pwrb_c_packattrref.wb_vers \
	$(cnf_dir)/pwrb_c_packattrref10.wb_vers \
	$(cnf_dir)/pwrb_c_packoperator.wb_vers \
	$(cnf_dir)/pwrb_c_packtest.wb_vers \
	$(cnf_dir)/pwrb_c_pageref.wb_vers \
	$(cnf_dir)/pwrb_c_pb_ai.wb_vers \
	$(cnf_dir)/pwrb_c_pb_ao.wb_vers \
	$(cnf_dir)/pwrb_c_pb_di.wb_vers \
	$(cnf_dir)/pwrb_c_pb_do.wb_vers \
	$(cnf_dir)/pwrb_c_pb_et200m.wb_vers \
	$(cnf_dir)/pwrb_c_pb_euro2500.wb_vers \
	$(cnf_dir)/pwrb_c_pb_npba12.wb_vers \
	$(cnf_dir)/pwrb_c_pb_profiboard.wb_vers \
	$(cnf_dir)/pwrb_c_pd_7435_26.wb_vers \
	$(cnf_dir)/pwrb_c_pid.wb_vers \
	$(cnf_dir)/pwrb_c_pidx.wb_vers \
	$(cnf_dir)/pwrb_c_pid_1up.wb_vers \
	$(cnf_dir)/pwrb_c_pipos.wb_vers \
	$(cnf_dir)/pwrb_c_pispeed.wb_vers \
	$(cnf_dir)/pwrb_c_plcthread.wb_vers \
	$(cnf_dir)/pwrb_c_plcprocess.wb_vers \
	$(cnf_dir)/pwrb_c_plcpgm.wb_vers \
	$(cnf_dir)/pwrb_c_plotgroup.wb_vers \
	$(cnf_dir)/pwrb_c_po.wb_vers \
	$(cnf_dir)/pwrb_c_point.wb_vers \
	$(cnf_dir)/pwrb_c_porder.wb_vers \
	$(cnf_dir)/pwrb_c_pos3p.wb_vers \
	$(cnf_dir)/pwrb_c_posit.wb_vers \
	$(cnf_dir)/pwrb_c_pulse.wb_vers \
	$(cnf_dir)/pwrb_c_queue.wb_vers \
	$(cnf_dir)/pwrb_c_rack_rtp.wb_vers \
	$(cnf_dir)/pwrb_c_rack_ssab.wb_vers \
	$(cnf_dir)/pwrb_c_rack_vme.wb_vers \
	$(cnf_dir)/pwrb_c_ramp.wb_vers \
	$(cnf_dir)/pwrb_c_resdi.wb_vers \
	$(cnf_dir)/pwrb_c_resdo.wb_vers \
	$(cnf_dir)/pwrb_c_resdp.wb_vers \
	$(cnf_dir)/pwrb_c_resdv.wb_vers \
	$(cnf_dir)/pwrb_c_reset_so.wb_vers \
	$(cnf_dir)/pwrb_c_rootvolumeconfig.wb_vers \
	$(cnf_dir)/pwrb_c_rootvolumeload.wb_vers \
	$(cnf_dir)/pwrb_c_rttconfig.wb_vers \
	$(cnf_dir)/pwrb_c_rtp_dioc.wb_vers \
	$(cnf_dir)/pwrb_c_scantime.wb_vers \
	$(cnf_dir)/pwrb_c_security.wb_vers \
	$(cnf_dir)/pwrb_c_select.wb_vers \
	$(cnf_dir)/pwrb_c_setcond.wb_vers \
	$(cnf_dir)/pwrb_c_setdi.wb_vers \
	$(cnf_dir)/pwrb_c_setdo.wb_vers \
	$(cnf_dir)/pwrb_c_setdp.wb_vers \
	$(cnf_dir)/pwrb_c_setdv.wb_vers \
	$(cnf_dir)/pwrb_c_sharedvolumeconfig.wb_vers \
	$(cnf_dir)/pwrb_c_sharedvolumeload.wb_vers \
	$(cnf_dir)/pwrb_c_showplcattr.wb_vers \
	$(cnf_dir)/pwrb_c_sorder.wb_vers \
	$(cnf_dir)/pwrb_c_speed.wb_vers \
	$(cnf_dir)/pwrb_c_sr_r.wb_vers \
	$(cnf_dir)/pwrb_c_sr_s.wb_vers \
	$(cnf_dir)/pwrb_c_ssbegin.wb_vers \
	$(cnf_dir)/pwrb_c_ssend.wb_vers \
	$(cnf_dir)/pwrb_c_step.wb_vers \
	$(cnf_dir)/pwrb_c_stepconv.wb_vers \
	$(cnf_dir)/pwrb_c_stepdiv.wb_vers \
	$(cnf_dir)/pwrb_c_stoageneric.wb_vers \
	$(cnf_dir)/pwrb_c_stoai.wb_vers \
	$(cnf_dir)/pwrb_c_stoao.wb_vers \
	$(cnf_dir)/pwrb_c_stoap.wb_vers \
	$(cnf_dir)/pwrb_c_stoav.wb_vers \
	$(cnf_dir)/pwrb_c_stodgeneric.wb_vers \
	$(cnf_dir)/pwrb_c_stodi.wb_vers \
	$(cnf_dir)/pwrb_c_stodo.wb_vers \
	$(cnf_dir)/pwrb_c_stodp.wb_vers \
	$(cnf_dir)/pwrb_c_stodv.wb_vers \
	$(cnf_dir)/pwrb_c_stoip.wb_vers \
	$(cnf_dir)/pwrb_c_stopi.wb_vers \
	$(cnf_dir)/pwrb_c_store.wb_vers \
	$(cnf_dir)/pwrb_c_storeformat.wb_vers \
	$(cnf_dir)/pwrb_c_storetrig.wb_vers \
	$(cnf_dir)/pwrb_c_substep.wb_vers \
	$(cnf_dir)/pwrb_c_sum.wb_vers \
	$(cnf_dir)/pwrb_c_subvolumeconfig.wb_vers \
	$(cnf_dir)/pwrb_c_subvolumeload.wb_vers \
	$(cnf_dir)/pwrb_c_systemdistribute.wb_vers \
	$(cnf_dir)/pwrb_c_table.wb_vers \
	$(cnf_dir)/pwrb_c_text.wb_vers \
	$(cnf_dir)/pwrb_c_timer.wb_vers \
	$(cnf_dir)/pwrb_c_timint.wb_vers \
	$(cnf_dir)/pwrb_c_title.wb_vers \
	$(cnf_dir)/pwrb_c_tracedbconfig.wb_vers \
	$(cnf_dir)/pwrb_c_trans.wb_vers \
	$(cnf_dir)/pwrb_c_transconv.wb_vers \
	$(cnf_dir)/pwrb_c_transdiv.wb_vers \
	$(cnf_dir)/pwrb_c_user.wb_vers \
	$(cnf_dir)/pwrb_c_valve.wb_vers \
	$(cnf_dir)/pwrb_c_vipc616.wb_vers \
	$(cnf_dir)/pwrb_c_volumedistribute.wb_vers \
	$(cnf_dir)/pwrb_c_wait.wb_vers \
	$(cnf_dir)/pwrb_c_waith.wb_vers \
	$(cnf_dir)/pwrb_c_webhandler.wb_vers \
	$(cnf_dir)/pwrb_c_windowcond.wb_vers \
	$(cnf_dir)/pwrb_c_windoworderact.wb_vers \
	$(cnf_dir)/pwrb_c_windowplc.wb_vers \
	$(cnf_dir)/pwrb_c_windowsubstep.wb_vers \
	$(cnf_dir)/pwrb_c_xor.wb_vers \
	$(cnf_dir)/pwrb_c_xttgraph.wb_vers

export_wbl := \
	$(pwrb_volumes) \
	$(pwrb_hierarchies) \
	$(pwrb_classes) \
	$(pwrb_layouts)

clean_wbl := $(patsubst %.wb_vers,clean_%.wb_vers,$(export_wbl))

.PHONY : all init copy lib exe clean realclean\
         $(clean_wbl)

all : init copy exe

init :

copy : $(inc_dir)/pwr_baseclasses.h

lib : $(export_wbl)

exe : $(exe_dir)/pwrb_xtthelp.dat

clean :

realclean : clean $(clean_wbl)

$(inc_dir)/pwr_baseclasses.h :
	@ write sys$$output "Generating struct files for pwrb classes..."
	@ mc pwr_exe:co_convert -s -d pwr_inc: $(to-vms $(pwre_sroot)/wbl/pwrb/src/pwrb_c_*.wb_load)

$(doc_dir)/pwrb_allclasses.html :
	@ write sys$$output "Generating html files for pwrb classes..."
	@ mc pwr_exe:co_convert -w -d pwr_doc: $(to_vms $(pwre_sroot)/wbl/pwrb/src/pwrb_c_*.wb_load)
	@ write sys$$output "Generating html files for pwrb structs..."
	@ mc pwr_exe:co_convert -c -d pwr_doc: $(inc_dir)/pwr_baseclasses.h

$(exe_dir)/pwrb_xtthelp.dat :
	@ write sys$$output "Generating xtt help files for pwrb classes"
	@ mc pwr_exe:co_convert -x -d pwr_exe: $(to-vms $(pwre_sroot)/wbl/pwrb/src/pwrb_c_*.wb_load)

#$(export_wbl) : $(cnf_dir)/%.wb_vers : %.wb_load
#       @ write sys$$output "Exporting $< ..."
#       @ $(copy) $(copyflags) $(source) $(target)
#       @ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(clean_wbl) : clean_%.wb_vers : %.wb_vers
	@ file := $(to-vms $(cnf_dir)/$*.wb_vers)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*
	@ file := $(to-vms $(lis_dir)/$*.wb_list)
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

$(cnf_dir)/pwrb_c_csub.wb_vers : pwrb_c_csub.wb_load \
	$(cnf_dir)/pwrb_c_windowplc.wb_vers

$(cnf_dir)/pwrb_c_order.wb_vers : pwrb_c_order.wb_load \
	$(cnf_dir)/pwrb_c_windowcond.wb_vers \
	$(cnf_dir)/pwrb_c_windoworderact.wb_vers

$(cnf_dir)/pwrb_c_substep.wb_vers : pwrb_c_substep.wb_load \
	$(cnf_dir)/pwrb_c_windowsubstep.wb_vers

$(cnf_dir)/pwrb_c_windowplc.wb_vers : pwrb_c_windowplc.wb_load \
	$(cnf_dir)/pwrb_c_document.wb_vers

