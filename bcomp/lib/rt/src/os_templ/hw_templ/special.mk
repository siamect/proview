copy : $(doc_dir)/sv_se/orm/rt_plc_bcomp_c.html \
       $(doc_dir)/en_us/orm/rt_plc_bcomp_c.html

$(doc_dir)/sv_se/orm/rt_plc_bcomp_c.html : ../../rt_plc_bcomp.c
	@ echo "Generating html files for rt_plc_bcomp sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/rt_plc_bcomp_c.html : ../../rt_plc_bcomp.c
	@ echo "Generating html files for rt_plc_bcomp en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"


