include $(pwre_dir_symbols)

ifndef variables_mk
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_sroot)/tools/bld/src/$(os_name)/variables.mk
endif


ifndef rules_mk
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_sroot)/tools/bld/src/$(os_name)/rules.mk
endif

vpath %.c $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.c) \
               ), $(notdir $(file)) \
             ) \
           )

c_sources := $(filter %.c,$(sources))

export_obj := $(patsubst %.c,$(obj_dir)/%.o,$(c_sources))
clean_obj := $(patsubst %.c,clean_%.o,$(c_sources))

.PHONY : all init copy lib exe clean realclean\
         $(clean_c)

all : init copy lib

init :

copy :

lib : $(export_obj)

exe :

clean :

realclean : clean $(clean_obj) 



$(clean_obj) : clean_%.o : %.c
	rm $(obj_dir)/$*.o


