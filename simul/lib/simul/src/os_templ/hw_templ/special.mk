copy : $(doc_dir)/sv_se/orm/simul_c.html \
       $(doc_dir)/en_us/orm/simul_c.html

$(doc_dir)/sv_se/orm/simul_c.html : ../../simul.c
	@ echo "Generating html files for simul sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/simul_c.html : ../../simul.c
	@ echo "Generating html files for simul en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"


