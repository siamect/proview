#! /bin/make
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build a help file.
#
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

sv_se_source := $(pwre_sroot)/hlp/op/sv_se/os_vms/op_htre.hlp
en_us_source := $(pwre_sroot)/hlp/op/en_us/os_vms/op_htre.hlp

sv_se_lib := $(pwre_broot)/$(os_name)/$(hw_name)/exp/exe/sv_se/op_htre.hlb
en_us_lib := $(pwre_broot)/$(os_name)/$(hw_name)/exp/exe/en_us/op_htre.hlb


.PHONY : all init copy lib exe clean realclean
         $(clean_includes)

all : lib

init :

copy :

lib : $(sv_se_lib) $(en_us_lib)

exe :

clean :

realclean :


$(sv_se_lib) : $(sv_se_source)
	@ $(log_hlp_hlb)
	@ library/create/replace/help $(target) $(source)

$(en_us_lib) : $(en_us_source)
	@ $(log_hlp_hlb)
	@ library/create/replace/help $(target) $(source)

