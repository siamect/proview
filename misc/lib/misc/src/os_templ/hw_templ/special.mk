copy : $(doc_dir)/sv_se/orm/misc_plc_c.html \
       $(doc_dir)/en_us/orm/misc_plc_c.html

$(doc_dir)/sv_se/orm/misc_plc_c.html : ../../misc_plc.c
	@ echo "Generating html files for misc_plc sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/misc_plc_c.html : ../../misc_plc.c
	@ echo "Generating html files for misc_plc en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"


