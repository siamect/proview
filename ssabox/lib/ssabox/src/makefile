include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk

copy : $(doc_dir)/sv_se/orm/ssabox_plc_antisway_c.html \
       $(doc_dir)/en_us/orm/ssabox_plc_antisway_c.html \
       $(doc_dir)/sv_se/orm/ssabox_plc_servoreg_c.html \
       $(doc_dir)/en_us/orm/ssabox_plc_servoreg_c.html

$(doc_dir)/sv_se/orm/ssabox_plc_antisway_c.html : \
	$(pwre_sroot)/lib/ssabox/src/ssabox_plc_antisway.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/ssabox_plc_antisway_c.html : \
	$(pwre_sroot)/lib/ssabox/src/ssabox_plc_antisway.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

$(doc_dir)/sv_se/orm/ssabox_plc_servoreg_c.html : \
	$(pwre_sroot)/lib/ssabox/src/ssabox_plc_servoreg.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/ssabox_plc_servoreg_c.html : \
	$(pwre_sroot)/lib/ssabox/src/ssabox_plc_servoreg.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"
