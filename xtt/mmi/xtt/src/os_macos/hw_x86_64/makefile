include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
-include $(pwre_kroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

ifeq ($($(type_name)_generic_mk),)
 include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk
endif

#-include ../../special.mk
#-include ../special.mk
#-include special.mk

copy : $(exe_dir)/en_us/xtt_lng.dat $(exe_dir)/en_us/acceptlicense.txt $(exe_dir)/en_us/license.txt \
       $(exe_dir)/sv_se/xtt_lng.dat $(exe_dir)/sv_se/acceptlicense.txt $(exe_dir)/sv_se/license.txt \
       $(exe_dir)/de_de/xtt_lng.dat $(exe_dir)/de_de/acceptlicense.txt $(exe_dir)/de_de/license.txt \


$(exe_dir)/en_us/xtt_lng.dat : $(pwre_sroot)/mmi/xtt/en_us/xtt_lng.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/en_us/acceptlicense.txt : $(pwre_sroot)/mmi/xtt/en_us/acceptlicense.txt
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/en_us/license.txt : $(pwre_sroot)/mmi/xtt/en_us/license.txt
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/sv_se/xtt_lng.dat : $(pwre_sroot)/mmi/xtt/sv_se/xtt_lng.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/sv_se/acceptlicense.txt : $(pwre_sroot)/mmi/xtt/sv_se/acceptlicense.txt
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/sv_se/license.txt : $(pwre_sroot)/mmi/xtt/sv_se/license.txt
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/de_de/xtt_lng.dat : $(pwre_sroot)/mmi/xtt/de_de/xtt_lng.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/de_de/acceptlicense.txt : $(pwre_sroot)/mmi/xtt/de_de/acceptlicense.txt
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/de_de/license.txt : $(pwre_sroot)/mmi/xtt/de_de/license.txt
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)





