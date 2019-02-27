include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

vpath %.c $(co_source)
vpath %.cpp $(co_source)
vpath %.h $(co_source)

source_dirs := $(co_source)

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

all : init copy lib | silent

init :

copy : $(export_h) | silent

lib : $(export_obj) | silent

exe :

clean :

realclean : clean

silent :
	@ :

.SUFFIXES:
