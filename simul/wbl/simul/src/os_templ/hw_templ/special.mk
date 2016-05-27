exe :  $(doc_dir)/sv_se/orm/pwr_simulclasses_h.html \
       $(doc_dir)/en_us/orm/pwr_simulclasses_h.html \
       $(doc_dir)/sv_se/orm/pwr_simulclasses_hpp.html \
       $(doc_dir)/en_us/orm/pwr_simulclasses_hpp.html

$(doc_dir)/sv_se/orm/pwr_simulclasses_h.html : $(inc_dir)/pwr_simulclasses.h
	@ echo "Generating html files for struct sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/pwr_simulclasses_h.html : $(inc_dir)/pwr_simulclasses.h
	@ echo "Generating html files for struct en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

$(doc_dir)/sv_se/orm/pwr_simulclasses_hpp.html : $(inc_dir)/pwr_simulclasses.hpp
	@ echo "Generating html files for struct sv_se..."
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/pwr_simulclasses_hpp.html : $(inc_dir)/pwr_simulclasses.hpp
	@ echo "Generating html files for struct en_us..."
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

