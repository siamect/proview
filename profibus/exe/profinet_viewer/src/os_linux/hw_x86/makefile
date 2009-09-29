include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

ifeq ($($(type_name)_generic_mk),)
 include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk
endif

-include ../../special.mk
-include ../special.mk
-include special.mk

copy :        $(exe_dir)/profinet_vendorid.dat $(exe_dir)/profinet_deviceid.dat

$(exe_dir)/profinet_vendorid.dat : ../../profinet_vendorid.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/profinet_deviceid.dat : ../../profinet_deviceid.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)
