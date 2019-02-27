include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk

-include special.mk

copy :        $(exe_dir)/profinet_vendorid.dat $(exe_dir)/profinet_deviceid.dat

$(exe_dir)/profinet_vendorid.dat : profinet_vendorid.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/profinet_deviceid.dat : profinet_deviceid.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)
