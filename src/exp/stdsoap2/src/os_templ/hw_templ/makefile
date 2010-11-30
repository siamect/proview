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
vpath %.cpp $(hw_source):$(os_source):$(co_source)
vpath %.h $(hw_source):$(os_source):$(co_source)

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

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.cpp) \
               ), $(notdir $(file)) \
             ) \
           )

cpp_sources := $(filter %.cpp,$(sources))

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.h) \
               ), $(notdir $(file)) \
             ) \
           )

h_sources := $(filter %.h,$(sources))

export_obj := $(patsubst %.c,$(obj_dir)/%.o,$(c_sources))  \
              $(patsubst %.cpp,$(obj_dir)/%.o,$(cpp_sources))
export_h := $(patsubst %.h,$(inc_dir)/%.h,$(h_sources))

.PHONY : all init copy lib exe clean realclean

all : init copy lib

init :

copy : $(export_h)

lib : $(export_obj)

exe :

clean :

realclean : clean

.SUFFIXES:






