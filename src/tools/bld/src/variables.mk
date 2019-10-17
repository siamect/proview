#  variables.mk -- variables common to all makefiles
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>
#
ifndef variables_mk
variables_mk := 1

release_root	:= $(pwre_broot)/$(pwre_os)/$(pwre_hw)
module_root	:= $(release_root)/$(pwre_bmodule)
export_type	:= $(export_type)

exe_dir	:= $(module_root)/exe
lib_dir	:= $(module_root)/lib
obj_dir	:= $(module_root)/obj
cnf_dir	:= $(module_root)/cnf
db_dir	:= $(module_root)/db
inc_dir	:= $(module_root)/inc
lis_dir	:= $(module_root)/lis
kit_dir	:= $(module_root)/kit
src_dir	:= $(module_root)/src
load_dir := $(module_root)/load
web_dir	:= $(module_root)/web

doc_dir	:= $(release_root)/exp/doc
elib_dir := $(release_root)/exp/lib
einc_dir := $(release_root)/exp/inc
eobj_dir := $(release_root)/exp/obj
eexe_dir := $(release_root)/exp/exe

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

#  Relative names of source directories

co_source := .

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
rs_msg_objs 	:= $(obj_dir)/pwr_msg_rs.o
wb_msg_objs 	:= $(obj_dir)/pwr_msg_wb.o $(obj_dir)/pwr_msg_ge.o $(obj_dir)/pwr_msg_flow.o
rt_msg_eobjs 	:= $(eobj_dir)/pwr_msg_co.o $(eobj_dir)/pwr_msg_rt.o
rs_msg_eobjs 	:= $(eobj_dir)/pwr_msg_rs.o
wb_msg_eobjs 	:= $(eobj_dir)/pwr_msg_wb.o $(eobj_dir)/pwr_msg_ge.o $(eobj_dir)/pwr_msg_flow.o

linkcp = :
pre_so =

log_done	=
ifeq ($(pwre_conf_qt),1)
  csetos 	:= -DPWRE_CONF_QT $(pwre_conf_cc_define)
  cinc          := -I$(inc_dir) -I$(einc_dir) -I$(co_source) $(pwre_conf_incdir) $(pwre_conf_incdirqt)
else
  csetos 	:= -DPWRE_CONF_GTK $(pwre_conf_cc_define)
  cinc          := -I$(inc_dir) -I$(einc_dir) -I$(co_source) $(pwre_conf_incdir) $(pwre_conf_incdirgtk) $(pwre_conf_incdirgst)
endif
rm		:= rm
cp		:= cp
cpflags		:=
arflags		:= r
obj             := -c -o $(target)
javac		:= javac
ifdef pwre_ar
  ar            := $(pwre_ar)
else
  ar            := ar
endif
ifdef pwre_cc
  cc              := $(pwre_cc)
  ld              := $(pwre_cc)
else
  cc              := gcc
  ld              := gcc
endif
ifdef pwre_cxx
  cxx             := $(pwre_cxx)
  ldxx            := $(pwre_cxx)
else
  cxx             := g++
  ldxx            := g++
endif
ifdef pwre_host_exe
  tools_msg2cmsg  := $(pwre_host_exe)/tools_msg2cmsg
  tools_cmsg2c    := $(pwre_host_exe)/tools_cmsg2c
  tools_pdrgen    := $(pwre_host_exe)/tools_pdrgen
  wb_rtt          := $(pwre_host_exe)/wb_rtt
  co_convert      := $(pwre_host_exe)/co_convert
  co_merge        := $(pwre_host_exe)/co_merge
else
  tools_msg2cmsg  := $(eexe_dir)/tools_msg2cmsg
  tools_cmsg2c    := $(eexe_dir)/tools_cmsg2c
  tools_pdrgen    := $(eexe_dir)/tools_pdrgen
  wb_rtt          := $(exe_dir)/wb_rtt
  co_convert      := $(eexe_dir)/co_convert
  co_merge        := $(eexe_dir)/co_merge
endif

map             = $(bld_dir)/$(tname)$(map_ext)
mkdir		:= mkdir
#   Set to /buildversion for frozen dbs versions
wblflags	:=

warnings := -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-parameter -Wno-unused-but-set-variable -Wno-sign-compare -Wno-missing-field-initializers -Wno-cast-function-type -Wno-implicit-fallthrough

ifeq ($(pwre_btype),rls)
  cflags	:= $(cross_compile) -c -O3 -D_GNU_SOURCE -DPWR_NDEBUG -D_REENTRANT -fPIC $(warnings)
  linkflags	:= $(cross_compile) -O3 -lstdc++ -L$(lib_dir)
  elinkflags	:= $(cross_compile) -O3 -lstdc++ -L$(lib_dir) -L$(elib_dir)
else
  cflags	:= $(cross_compile) -c -g -D_GNU_SOURCE -D_REENTRANT -fPIC $(warnings)
  linkflags	:= $(cross_compile) -g -lstdc++ -L$(lib_dir)
  elinkflags	:= $(cross_compile) -g -lstdc++ -L$(lib_dir) -L$(elib_dir)
endif

explinkflags	:= $(cross_compile) -g -L$(elib_dir)
cxxflags	:= $(cflags) -Wno-deprecated-copy
clis		= /lis=$(list)
dolist	= /lis=$(list)
domap		= -Xlinker -Map -Xlinker $(map)

log_h_h		= echo "Exporting $<"
log_x_h		= echo "Exporting $< to $(notdir $@)"
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
