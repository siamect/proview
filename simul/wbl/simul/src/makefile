include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk

exe :  $(doc_dir)/sv_se/orm/pwr_simulclasses_h.html \
       $(doc_dir)/en_us/orm/pwr_simulclasses_h.html \
       $(doc_dir)/sv_se/orm/pwr_simulclasses_hpp.html \
       $(doc_dir)/en_us/orm/pwr_simulclasses_hpp.html

$(doc_dir)/sv_se/orm/pwr_simulclasses_h.html : $(inc_dir)/pwr_simulclasses.h
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/pwr_simulclasses_h.html : $(inc_dir)/pwr_simulclasses.h
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"

$(doc_dir)/sv_se/orm/pwr_simulclasses_hpp.html : $(inc_dir)/pwr_simulclasses.hpp
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/sv_se/orm "$(source)"

$(doc_dir)/en_us/orm/pwr_simulclasses_hpp.html : $(inc_dir)/pwr_simulclasses.hpp
	@ $(log_html)
	@ $(co_convert) -c -d $(doc_dir)/en_us/orm "$(source)"
