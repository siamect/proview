#  mmi_generic.mk -- generic make file for building mmi components
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>
#
ifndef mmi_generic_mk
mmi_generic_mk := 1

ifndef variables_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/variables.mk
endif


ifndef rules_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/rules.mk
endif




vpath %.uil $(hw_source):$(os_source):$(co_source)
vpath %.pwg $(hw_source):$(os_source):$(co_source)
vpath %.pwsg $(hw_source):$(os_source):$(co_source)
vpath %.png $(hw_source):$(os_source):$(co_source)

source_dirs := $(hw_source) $(os_source) $(co_source)

uil_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.uil) \
               ), $(notdir $(file)) \
             ) \
           )

pwg_c_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/pwr_c_*.pwg) \
               ), $(notdir $(file)) \
             ) \
           )

pwg_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.pwg) \
               ), $(notdir $(file)) \
             ) \
           )

pwsg_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.pwsg) \
               ), $(notdir $(file)) \
             ) \
           )

png_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.png) \
               ), $(notdir $(file)) \
             ) \
           )

export_uid := $(addprefix $(exe_dir)/, $(patsubst %.uil, %.uid, $(uil_sources)))
export_c_pwg := $(addprefix $(exe_dir)/, $(pwg_c_sources))
export_pwg := $(addprefix $(exe_dir)/, $(pwg_sources))
export_pwsg := $(addprefix $(exe_dir)/, $(pwsg_sources))
export_png := $(addprefix $(exe_dir)/, $(png_sources))

$(exe_dir)/%.uid : %.uil
	@ $(log_uil_uid)
	@ export -n LANG; uil -o $(target) $(source)

clean_uid := $(patsubst %.uil,clean_%.uid,$(uil_sources))
clean_c_pwg := $(patsubst %.pwg,clean_%.pwg,$(pwg_c_sources))
clean_pwg := $(patsubst %.pwg,clean_%.pwg,$(pwg_sources))
clean_pwsg := $(patsubst %.pwsg,clean_%.pwsg,$(pwsg_sources))
clean_png := $(patsubst %.png,clean_%.png,$(png_sources))

.PHONY : all init copy lib exe clean realclean\
         dirs clean_bld clean_dirs $(clean_uid)

all : init copy

init : dirs

copy : $(export_uid) $(export_c_pwg) $(export_pwg) $(export_pwsg) $(export_png)

lib :

exe :

clean : $(clean_uid) $(clean_pwg) $(clean_pwsg) $(clean_png)

realclean : clean

dirs : 

$(clean_uid) : clean_%.uid : %.uil
	@ echo "Removing uid"
	@ $(rm) $(rmflags) $(exe_dir)/$*.uid  

$(clean_pwg) : clean_%.pwg : %.pwg
	@ echo "Removing pwg"
	@ $(rm) $(rmflags) $(exe_dir)/$*.pwg  

$(clean_c_pwg) : clean_%.pwg : %.pwg
	@ echo "Removing pwg"
	@ $(rm) $(rmflags) $(exe_dir)/$*.pwg  

$(clean_pwsg) : clean_%.pwsg : %.pwsg
	@ echo "Removing pwsg"
	@ $(rm) $(rmflags) $(exe_dir)/$*.pwsg  

$(clean_png) : clean_%.png : %.png
	@ echo "Removing png"
	@ $(rm) $(rmflags) $(exe_dir)/$*.png  


endif
