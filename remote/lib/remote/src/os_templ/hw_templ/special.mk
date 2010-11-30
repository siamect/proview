copy : $(doc_dir)/sv_se/orm/remote_plc_macro_h.html \
       $(doc_dir)/en_us/orm/remote_plc_macro_h.html

$(doc_dir)/sv_se/orm/remote_plc_macro_h.html : ../../remote_plc_macro.h
	@ echo "Generating html files for remote_plc_macro sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/remote_plc_macro_h.html : ../../remote_plc_macro.h
	@ echo "Generating html files for remote_plc_macro en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"


