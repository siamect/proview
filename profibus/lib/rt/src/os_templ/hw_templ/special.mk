ifndef variables_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/variables.mk
endif


vpath %.c $(hw_source):$(os_source):$(co_source)

c_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.c) \
               ), $(notdir $(file)) \
             ) \
           )

export_c := $(addprefix $(bld_dir)/, $(patsubst %.c, %.o, $(c_sources)))

lib : $(export_lib) export_lib_local

export_lib_local : $(export_c)
	@ echo "Inserting local lib"
	@ $(ar) -r $(export_lib) $(export_c)
