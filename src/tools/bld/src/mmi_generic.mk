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
 include $(pwre_kroot)/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/rules.mk
endif

vpath %.uil $(co_source)
vpath %.pwg $(co_source)
vpath %.pwsg $(co_source)
vpath %.png $(co_source)
vpath %.pbm $(co_source)

source_dirs := $(co_source)

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

pbm_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.pbm) \
               ), $(notdir $(file)) \
             ) \
           )

export_uid := $(addprefix $(exe_dir)/, $(patsubst %.uil, %.uid, $(uil_sources)))
export_c_pwg := $(addprefix $(exe_dir)/, $(pwg_c_sources))
export_pwg := $(addprefix $(exe_dir)/, $(pwg_sources))
export_pwsg := $(addprefix $(exe_dir)/, $(pwsg_sources))
export_png := $(addprefix $(exe_dir)/, $(png_sources))
export_pbm := $(addprefix $(exe_dir)/, $(pbm_sources))

$(exe_dir)/%.uid : %.uil
	@ $(log_uil_uid)
	@ export LANG; uil -o $(target) $(source)

clean_uid := $(patsubst %.uil,clean_%.uid,$(uil_sources))
clean_c_pwg := $(patsubst %.pwg,clean_%.pwg,$(pwg_c_sources))
clean_pwg := $(patsubst %.pwg,clean_%.pwg,$(pwg_sources))
clean_pwsg := $(patsubst %.pwsg,clean_%.pwsg,$(pwsg_sources))
clean_png := $(patsubst %.png,clean_%.png,$(png_sources))
clean_pbm := $(patsubst %.pbm,clean_%.pbm,$(pbm_sources))

.PHONY : all init copy lib exe clean realclean\
         dirs clean_bld clean_dirs $(clean_uid)

all : init copy | silent

init : dirs | silent

copy : $(export_uid) $(export_c_pwg) $(export_pwg) $(export_pwsg) $(export_png) $(export_pbm) | silent

lib :

exe :

clean : $(clean_uid) $(clean_pwg) $(clean_pwsg) $(clean_png) $(clean_pbm)

realclean : clean

dirs :

silent :
	@ :

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

$(clean_pbm) : clean_%.pbm : %.pbm
	@ echo "Removing pbm"
	@ $(rm) $(rmflags) $(exe_dir)/$*.pbm

endif
