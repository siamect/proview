ifndef variables_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/variables.mk
endif


export_cpp = $(bld_dir)/stdsoap2.o
export_h = $(inc_dir)/stdsoap2.h $(inc_dir)/Service.nsmap

lib : $(export_lib) export_lib_local

export_lib_local : $(export_cpp) $(export_h)
	@ $(ar) -r $(export_lib) $(export_cpp)


$(bld_dir)/stdsoap2.o : ../../stdsoap2.cpp

$(inc_dir)/stdsoap2.h : ../../stdsoap2.h

$(inc_dir)/Service.nsmap : ../../Service.nsmap
	@ echo "Export $(source)"
	@ $(cp) $(cpflags) $(source) $(target)
