#  mmi_generic.mk -- generic make file for building mmi components
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>
#
ifndef mmi_generic_mk
mmi_generic_mk := 1

include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk

ifdef pwre_lang
 vpath %.uil $(hw_source):$(os_source):$(co_source):$(lang_hw_source):$(lang_os_source):$(lang_co_source)
 vpath %.frm $(hw_source):$(os_source):$(co_source):$(lang_hw_source):$(lang_os_source):$(lang_co_source)
 vpath %.g $(hw_source):$(os_source):$(co_source):$(lang_hw_source):$(lang_os_source):$(lang_co_source)
 vpath %.xbm $(hw_source):$(os_source):$(co_source):$(lang_hw_source):$(lang_os_source):$(lang_co_source)

 source_dirs := $(hw_source) $(os_source) $(co_source) $(lang_hw_source) $(lang_os_source) $(lang_co_source)
else
 vpath %.uil $(hw_source):$(os_source):$(co_source)
 vpath %.frm $(hw_source):$(os_source):$(co_source)
 vpath %.g $(hw_source):$(os_source):$(co_source)
 vpath %.xbm $(hw_source):$(os_source):$(co_source)

 source_dirs := $(hw_source) $(os_source) $(co_source)
endif

uil_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.uil) \
               ), $(notdir $(file)) \
             ) \
           )

inc_uil_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/inc_*.uil) \
               ), $(notdir $(file)) \
             ) \
           )

g_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.g) \
               ), $(notdir $(file)) \
             ) \
           )

frm_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.frm) \
               ), $(notdir $(file)) \
             ) \
           )

xbm_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/$(comp_name)*.xbm) \
               ), $(notdir $(file)) \
             ) \
           )

$(lang_dir)/%.uid : %.uil
	@ $(log_uil_uid)
	@ define/user pwr_lang $(to-vms $(lang_bld_dir)/)
	@ uil $(uilflags)/output=$(target) $(source)

$(lang_bld_dir)/%.uil : %.uil
	@ write sys$$output "Exporting $<"
	@ $(cp) $(cpflags) $(source) $(target)

export_inc_uil := $(addprefix $(lang_bld_dir)/,$(inc_uil_sources))

export_uid := $(addprefix $(lang_dir)/,$(patsubst %.uil,%.uid,$(uil_sources)))
clean_uid := $(patsubst %.uil,clean_%.uid,$(uil_sources))

export_frm := $(addprefix $(lang_dir)/,$(frm_sources))
clean_frm := $(patsubst %.frm,clean_%.frm,$(frm_sources))

export_m1 := $(addprefix $(lang_dir)/,$(patsubst %.g,%.m1,$(g_sources)))
clean_m1 := $(patsubst %.g,clean_%.m1,$(g_sources))

export_xbm := $(addprefix $(lang_dir)/,$(xbm_sources))
clean_xbm := $(patsubst %.xbm,clean_%.xbm,$(xbm_sources))

.PHONY : all init copy lib exe clean realclean\
         dirs clean_bld clean_dirs $(clean_uid) $(clean_frm) $(clean_m1) $(clean_xbm)

all : init copy

#all :
#	@ write sys$$output "pwre_lang: $(pwre_lang)"
#	@ write sys$$output "lang: $(lang)"
#	@ write sys$$output "lang_dir: $(lang_dir)"
#	@ write sys$$output "lang_bld_dir: $(lang_bld_dir)"
#	@ write sys$$output "$(source_dirs)"
#	@ write sys$$output "$(uil_sources)"
#	@ write sys$$output "$(export_inc_uil)"

init : dirs

copy : $(export_inc_uil) $(export_uid) $(export_frm) $(export_m1) $(export_xbm)

lib :

exe :

clean : clean_bld

realclean : clean clean_dirs $(clean_uid) $(clean_frm) $(clean_m1) $(clean_xbm)

dirs : $(lang_bld_dir)$(dir_ext)

$(lang_bld_dir)$(dir_ext) :
	@ $(mkdir) $(mkdirflags) $(to-vms $(basename $@)/)

clean_bld :
	@ write sys$$output "Cleaning $(to-vms $(lang_bld_dir)/)"
	@ files := $(to-vms $(lang_bld_dir)/)*.*
	@ if f$$search(files) .nes. "" then $(rm) $(rmflags) 'files';*/exc=*.dir
	@ write sys$$output "Cleaning $(to-vms $(bld_dir)/)"
	@ files := $(to-vms $(bld_dir)/)*.*
	@ if f$$search(files) .nes. "" then $(rm) $(rmflags) 'files';*/exc=*.dir

clean_dirs :
	@ file := $(to-vms $(lang_bld_dir)).dir
	@ if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*
	@ files := $(to-vms $(bld_dir)/)*.*
	@ file := $(to-vms $(bld_dir)).dir
	@ if f$$search(files) .eqs. "" then if f$$search(file) .nes. "" then $(rm) $(rmflags) 'file';*

$(clean_uid) : clean_%.uid : %.uil
	@ file := $(to-vms $(lang_dir)/$*.uid)
	@ if f$$search(file) .nes. "" then delete/log 'file';* 

$(clean_frm) : clean_%.frm : %.frm
	@ file := $(to-vms $(lang_dir)/$*.frm)
	@ if f$$search(file) .nes. "" then delete/log 'file';* 

$(clean_m1) : clean_%.m1 : %.g
	@ file := $(to-vms $(lang_dir)/$*.m1)
	@ if f$$search(file) .nes. "" then delete/log 'file';* 

$(clean_xbm) : clean_%.xbm : %.xbm
	@ file := $(to-vms $(lang_dir)/$*.xbm)
	@ if f$$search(file) .nes. "" then delete/log 'file';* 

endif
