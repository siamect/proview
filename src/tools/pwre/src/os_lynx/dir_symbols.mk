#  dir_symbols.mk  -  get directory symbols
#
#
#
comma := ,
empty :=
space := $(empty) $(empty)

pred_13	:= 12
pred_12	:= 11
pred_11	:= 10
pred_10	:= 9
pred_9	:= 8
pred_8	:= 7
pred_7	:= 6
pred_6	:= 5
pred_5	:= 4
pred_4	:= 3
pred_3	:= 2
pred_2	:= 1
pred_1	:= 0

cwd_dirs := $(subst /,$(space),$(PWD))

word	:= $(words $(cwd_dirs))
hw_name	:= $(word $(word), $(cwd_dirs))
ifneq ($(findstring hw, $(hw_name)),hw)
include ___error___hw
endif

prev	:= pred_ $(word) 
pred	:= $(subst $(space),$(empty),$(prev))
word	:= $($(pred))
os_name	:= $(word $(word), $(cwd_dirs))
ifneq ($(findstring os,$(os_name)),os)
include ___error___os
endif

prev	:= pred_ $(word) 
pred	:= $(subst $(space),$(empty),$(prev))
word	:= $($(pred))
src_name	:= $(word $(word), $(cwd_dirs))
ifneq ($(src_name),src)
include ___error___no_source_subdirectory
endif

prev	:= pred_ $(word) 
pred	:= $(subst $(space),$(empty),$(prev))
word	:= $($(pred))
comp_name	:= $(word $(word), $(cwd_dirs))

prev	:= pred_ $(word) 
pred	:= $(subst $(space),$(empty),$(prev))
word	:= $($(pred))
type_name	:= $(word $(word), $(cwd_dirs))

prev	:= pred_ $(word) 
pred	:= $(subst $(space),$(empty),$(prev))
word	:= $($(pred))
root_name	:= $(word $(word), $(cwd_dirs))
ifneq ($(src_name),src)
include ___error___incorrect_source_root1
endif

#rest = /$(type_name)/$(comp_name)/src/$(os_name)/$(hw_name)/
#test_source_root := $(subst $(rest),$(empty),$(PWD))
#ifneq ($(pwre_sroot),$(test_source_root))
#include ___error___incorrect_source_root2
#endif
