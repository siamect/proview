#  variables.mk -- variables common to all makefiles
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>
#
ifndef variables_mk
variables_mk := 1

release_root	:= $(pwre_broot)/$(os_name)/$(hw_name)
exp_root	:= $(release_root)/exp

exe_dir	:= $(exp_root)/exe
lib_dir	:= $(exp_root)/lib
obj_dir	:= $(exp_root)/obj
cnf_dir	:= $(exp_root)/cnf
db_dir	:= $(exp_root)/db
inc_dir	:= $(exp_root)/inc
lis_dir	:= $(exp_root)/lis
kit_dir	:= $(exp_root)/kit
src_dir	:= $(exp_root)/src
doc_dir	:= $(exp_root)/doc
load_dir := $(exp_root)/load

bld_dir	:= $(release_root)/bld/$(type_name)/$(comp_name)
tmp_dir	:= $(release_root)/tmp

c_ext	:= .c
d_ext	:= .d
d_ld_ext:= .d_ld
h_ext	:= .h
lib_ext	:= .a
obj_ext	:= .o
map_ext	:= .map

#  Full names of source directories

fhw_source := $(pwre_sroot)/$(type_name)/$(comp_name)/src/$(os_name)/$(hw_name)
fos_source := $(pwre_sroot)/$(type_name)/$(comp_name)/src/$(os_name)
fco_source := $(pwre_sroot)/$(type_name)/$(comp_name)/src

#  Realtive names of source directories

hw_source := .
os_source := ../
co_source := ../../


ifndef pwre_lang
  lang_dir := $(exe_dir)
  lang_bld_dir := $(bld_dir)
else
  ifeq ($(pwre_lang),en_us)
    lang := en
  endif

  ifeq ($(pwre_lang),sv_se)
    lang     := sw
    pwr_lang := 1
  endif

  lang_hw_source := $(pwre_sroot)/$(type_name)/$(comp_name)/$(pwre_lang)/$(os_name)/$(hw_name)
  lang_os_source := $(pwre_sroot)/$(type_name)/$(comp_name)/$(pwre_lang)/$(os_name)
  lang_co_source := $(pwre_sroot)/$(type_name)/$(comp_name)/$(pwre_lang)
  lang_dir := $(exe_dir)/$(pwre_lang)
  lang_bld_dir := $(bld_dir)/$(pwre_lang)
endif

source	= $<
sname	= $(notdir $(basename $<))
ssuff	= $(suffix $<)
sdir	= $(dir $<)/
target	= $@
tname	= $(notdir $(basename $@))
tsuff	= $(suffix $@)
tdir	= $(dir $@)/

libobj	= $(bld_dir)/$*$(obj_ext)
cobj	= -c -o $(target)
clibobj	= -c -o $(libobj)
rt_msg_objs 	:= $(obj_dir)/pwr_msg_co.o $(obj_dir)/pwr_msg_rt.o
wb_msg_objs 	:= $(obj_dir)/pwr_msg_wb.o $(obj_dir)/pwr_msg_ge.o $(obj_dir)/pwr_msg_flow.o

log_done	=
csetos		:= -DOS_LINUX=1 -DOS=linux -DHW_X86=1 -DHW=x86
cinc		:= -I$(inc_dir) -I$(hw_source) -I$(os_source) -I$(co_source)  -I$(dbinc) -I/usr/X11R6/include -I$(jdk)/include -I$(jdk)/include/linux

rm		:= rm
cp		:= cp
cpflags		:= 
ar		:= ar
arflags		:= r
obj             := -c -o $(target)

cc              := gcc
ld		:= gcc
cxx             := g++
ldxx		:= g++
lex             := flex
msg2cmsg	:= $(exe_dir)/tools_msg2cmsg
javac		:= javac

#list            = $(to-vms $(lis_dir)/$(sname)$(lis_ext))
map             = $(bld_dir)/$(tname)$(map_ext)
mkdir		:= mkdir

ifeq ($(pwre_btype),rls)			 
  cflags	:= -c -O3 -D_GNU_SOURCE -DPWR_NDEBUG -D_REENTRANT
  cxxflags	:= $(cflags) 
  linkflags	:= -O3 -L/usr/local/lib -L$(lib_dir) -lm -lrt
  clis		= /lis=$(list)
  dolist	= /lis=$(list)
  domap		= -Xlinker -Map -Xlinker $(map)
else
  cflags	:= -c -g -Wall -D_GNU_SOURCE -D_REENTRANT
  cxxflags	:= $(cflags) -Wno-deprecated
  mmflags	:= -Wno-deprecated
  linkflags	:= -g -L/usr/local/lib -L$(lib_dir) -lrt
  dolist	= /lis=$(list)
  clis		:= 
  domap		= -Xlinker -Map -Xlinker $(map)
endif

log_h_h		= echo "Exporting $<"
log_x_h		= echo "Exporting $< to $(notdir $@)"
log_l_obj	= echo "Compiling $<"
log_l_lib	= echo "Compiling $(notdir $(basename $@))($<)"
#log_c_d		= echo "Updating dependencies $<"
log_c_obj	= echo "Compiling $<"
log_c_lib	= echo "Compiling $(notdir $(basename $@))($<)"
log_x_lib	= echo "Compiling $(notdir $(basename $@))($<)"
log_obj_lib	= echo "Archiving lasse $(notdir $(basename $@))($(notdir $<))"
log_hlp_hlb	= echo "Exporting $(notdir $(basename $@))($<)"
log_sc_lib	= echo "Compiling $(notdir $(basename $@))($<)"
log_sc_obj	= echo "Compiling $<"
log_uil_uid	= echo "Compiling $<"
log_g_m1	= echo "Exporting $<"
log_cld_lib	= echo "Compiling $(notdir $(basename $@))($<)"
log_link_exe	= echo "Linking $@"
log_wb_load_wb_vers	= echo "Exporting $<"
log_xbm_xbm	= echo "Exporting $<"
log_frm_frm	= echo "Exporting $<"
log_graph_graph	= echo "Exporting $<"
log_msg_h	= echo "Compiling $<"
log_java_class	= echo "Compiling $<"

endif
