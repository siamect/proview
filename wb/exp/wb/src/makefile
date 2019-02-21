include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/rules.mk
endif

vpath %.h $(co_source)
vpath %.c $(co_source)
vpath %.cpp $(co_source)
vpath %.dat $(co_source)

source_dirs := $(co_source)

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
                 $(wildcard $(dir)/*.h $(dir)/*.c $(dir)/*.dat) \
               ), $(notdir $(file)) \
             ) \
           )

h_sources := $(filter %.h,$(sources))
c_sources := $(filter %.c,$(sources))
dat_sources := $(filter %.dat,$(sources))

export_h := $(patsubst %.h,$(inc_dir)/%.h,$(h_sources))
export_c := $(patsubst %.c,$(src_dir)/%.c,$(c_sources))
export_obj := $(patsubst %.cpp,$(obj_dir)/%.o,$(cpp_sources))
export_dat := $(patsubst %.dat,$(src_dir)/%.dat,$(dat_sources))
clean_h := $(patsubst %.h,clean_%.h,$(h_sources))
clean_c := $(patsubst %.c,clean_%.c,$(c_sources))
clean_dat := $(patsubst %.dat,clean_%.dat,$(dat_sources))

.SUFFIXES:

$(inc_dir)/%.h : %.h
	@ $(cp) $(cpflags) $(source) $(target)

$(src_dir)/%.c : %.c
	@ $(cp) $(cpflags) $(source) $(target)

$(src_dir)/%.dat : %.dat
	@ $(cp) $(cpflags) $(source) $(target)

.PHONY : all init copy lib exe clean realclean \
         clean_obj $(clean_h) $(clean_c) $(clean_dat)

all : init copy lib exe | silent

init : silent

copy : $(export_h) $(export_c) $(export_dat) \
	$(exe_dir)/pwr_wb_palette.cnf \
	| silent

lib : $(export_obj) | silent

exe :


clean : clean_obj

realclean : clean $(clean_h) $(clean_c) $(clean_dat)

silent :
	@ :

$(exe_dir)/pwr_wb_palette.cnf : pwr_wb_palette.cnf
	@ echo "Copying pwr_wb_palette.cnf"
	@ $(cp) $(cpflags) $(source) $(target)

$(clean_h) : clean_%.h : %.h
	@ $(rm) $(rmflags) $(inc_dir)/$*.h

$(clean_c) : clean_%.c : %.c
	@ $(rm) $(rmflags) $(src_dir)/$*.c

$(clean_dat) : clean_%.dat : %.dat
	@ $(rm) $(rmflags) $(src_dir)/$*.dat
