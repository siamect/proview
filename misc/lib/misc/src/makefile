include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk

copy : $(doc_dir)/sv_se/orm/misc_plc_c.html \
       $(doc_dir)/en_us/orm/misc_plc_c.html

$(doc_dir)/sv_se/orm/misc_plc_c.html : misc_plc.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/misc_plc_c.html : misc_plc.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"
