copy : $(doc_dir)/sv_se/orm/nmps_plc_c.html \
       $(doc_dir)/en_us/orm/nmps_plc_c.html \
       $(doc_dir)/sv_se/orm/nmps_plc_display_c.html \
       $(doc_dir)/en_us/orm/nmps_plc_display_c.html \
       $(doc_dir)/sv_se/orm/nmps_plc_runtime_c.html \
       $(doc_dir)/en_us/orm/nmps_plc_runtime_c.html \
       $(doc_dir)/sv_se/orm/nmps_plc_macro_h.html \
       $(doc_dir)/en_us/orm/nmps_plc_macro_h.html \

$(doc_dir)/sv_se/orm/nmps_plc_c.html : ../../nmps_plc.c
	@ echo "Generating html files for nmps_plc sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/nmps_plc_c.html : ../../nmps_plc.c
	@ echo "Generating html files for nmps_plc en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

$(doc_dir)/sv_se/orm/nmps_plc_display_c.html : ../../nmps_plc_display.c
	@ echo "Generating html files for nmps_plc_display sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/nmps_plc_display_c.html : ../../nmps_plc_display.c
	@ echo "Generating html files for nmps_plc_display en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

$(doc_dir)/sv_se/orm/nmps_plc_runtime_c.html : ../../nmps_plc_runtime.c
	@ echo "Generating html files for nmps_plc_runtime sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/nmps_plc_runtime_c.html : ../../nmps_plc_runtime.c
	@ echo "Generating html files for nmps_plc_runtime en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

$(doc_dir)/sv_se/orm/nmps_plc_macro_h.html : ../../nmps_plc_macro.h
	@ echo "Generating html files for nmps_plc_macro sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/nmps_plc_macro_h.html : ../../nmps_plc_macro.h
	@ echo "Generating html files for nmps_plc_macro en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"


