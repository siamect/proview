include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/rules.mk
endif

include $(pwre_kroot)/tools/bld/src/lib_generic.mk

copy : $(export_includes) $(l_copy) $(export_pwsg) $(export_meth) \
	$(exe_dir)/pwr_ge_setup.dat | silent

$(exe_dir)/pwr_ge_setup.dat : pwr_ge_setup.dat
	@ $(log_cp)
	@ cp $(source) $(target)
