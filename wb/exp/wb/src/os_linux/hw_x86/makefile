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


vpath %.h $(hw_source):$(os_source):$(co_source)
vpath %.c $(hw_source):$(os_source):$(co_source)
vpath %.cpp $(hw_source):$(os_source):$(co_source)
vpath %.dat $(hw_source):$(os_source):$(co_source)

source_dirs := $(hw_source) $(os_source) $(co_source)

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

all : init copy lib exe

init :

copy : $(export_h) $(export_c) $(export_dat) \
	$(exe_dir)/ge_help.dat \
	$(exe_dir)/pwr_wb_palette.cnf $(load_dir)/pwr_volumelist.dat

lib : $(export_obj)

exe :


clean : clean_obj

realclean : clean $(clean_h) $(clean_c) $(clean_dat)

$(exe_dir)/ge_help.dat : ge_help.dat
	@ echo "Copying ge_help.dat"
	@ $(cp) $(cpflags) $(source) $(target)
	@ co_convert -t -d $(pwr_doc)/help $(source)

$(exe_dir)/pwr_wb_palette.cnf : ../../pwr_wb_palette.cnf
	@ echo "Copying pwr_wb_palette.cnf"
	@ $(cp) $(cpflags) $(source) $(target)

$(load_dir)/pwr_volumelist.dat : ../../pwr_volumelist.dat
	@ echo "Copying pwr_volumelist.dat"
	@ $(cp) $(cpflags) $(source) $(target)

$(clean_h) : clean_%.h : %.h
	@ $(rm) $(rmflags) $(inc_dir)/$*.h 

$(clean_c) : clean_%.c : %.c
	@ $(rm) $(rmflags) $(src_dir)/$*.c

$(clean_dat) : clean_%.dat : %.dat
	@ $(rm) $(rmflags) $(src_dir)/$*.dat


