copy : $(doc_dir)/sv_se/orm/ssabox_plc_antisway_c.html \
       $(doc_dir)/en_us/orm/ssabox_plc_antisway_c.html \
       $(doc_dir)/sv_se/orm/ssabox_plc_servoreg_c.html \
       $(doc_dir)/en_us/orm/ssabox_plc_servoreg_c.html

$(doc_dir)/sv_se/orm/ssabox_plc_antisway_c.html : \
	$(pwre_sroot)/lib/ssabox/src/ssabox_plc_antisway.c
	@ echo "Generating html files for ssabox plc antiway code sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/ssabox_plc_antisway_c.html : \
	$(pwre_sroot)/lib/ssabox/src/ssabox_plc_antisway.c
	@ echo "Generating html files for ssabox plc antisway code en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

$(doc_dir)/sv_se/orm/ssabox_plc_servoreg_c.html : \
	$(pwre_sroot)/lib/ssabox/src/ssabox_plc_servoreg.c
	@ echo "Generating html files for ssabox plc servoreg code sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/ssabox_plc_servoreg_c.html : \
	$(pwre_sroot)/lib/ssabox/src/ssabox_plc_servoreg.c
	@ echo "Generating html files for ssabox plc servoreg code en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

