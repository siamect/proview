include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/rules.mk
endif

include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk

exe:   $(export_wbl_xtthelp_en_us) \
       $(export_wbl_xtthelp_sv_se) \
       $(export_wbl_html_en_us) \
       $(export_wbl_html_sv_se) \
       $(export_wbl_ps_en_us) \
       $(export_wbl_ps_sv_se) \
       $(export_wbl_pdf_en_us) \
       $(export_wbl_pdf_sv_se) \
       $(export_pdf_en_us) \
       $(export_pdf_sv_se) \
       $(doc_dir)/en_us/orm/rt_plc_bcomp_c.html \
       $(doc_dir)/sv_se/orm/rt_plc_bcomp_c.html \
       | silent

$(doc_dir)/en_us/orm/rt_plc_bcomp_c.html : $(pwre_sroot)/lib/rt/src/rt_plc_bcomp.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

$(doc_dir)/sv_se/orm/rt_plc_bcomp_c.html : $(pwre_sroot)/lib/rt/src/rt_plc_bcomp.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"
