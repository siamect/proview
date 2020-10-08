include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk

copy : $(doc_dir)/sv_se/orm/simul_c.html \
       $(doc_dir)/en_us/orm/simul_c.html

$(doc_dir)/sv_se/orm/simul_c.html : simul.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/simul_c.html : simul.c
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"
