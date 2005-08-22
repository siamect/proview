include $(pwre_dir_symbols)

#-include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
# -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

ifeq ($($(type_name)_generic_mk),)
# include $(pwre_sroot)/tools/bld/src/$(type_name)_generic.mk
endif

-include ../../special.mk
-include ../special.mk
-include special.mk

copy : $(exe_dir)/en_us/xtt_lng.dat $(exe_dir)/sv_se/xtt_lng.dat


$(exe_dir)/en_us/xtt_lng.dat : $(pwre_sroot)/mmi/xtt/en_us/xtt_lng.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/sv_se/xtt_lng.dat : $(pwre_sroot)/mmi/xtt/sv_se/xtt_lng.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)


