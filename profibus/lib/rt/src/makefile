include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk

ifndef variables_mk
 -include $(pwre_kroot)/tools/bld/src/variables.mk
endif

vpath %.c $(co_source)

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
	@ $(log_lib)
	@ if [ "$(export_c)" != "" ]; then \
	    $(ar) -r $(export_lib) $(export_c); \
	fi
