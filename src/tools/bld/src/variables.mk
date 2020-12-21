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
elis_dir := $(release_root)/exp/lis

bld_dir	:= $(release_root)/bld/$(type_name)/$(comp_name)
tmp_dir	:= $(release_root)/tmp

logfile := $(elis_dir)/build.tlog
time = `date +'%Y-%m-%d %T.00'`

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
  ifeq ($(pwre_conf_gtk),1)
    csetos 	:= -DPWRE_CONF_GTK $(pwre_conf_cc_define)
    cinc          := -I$(inc_dir) -I$(einc_dir) -I$(co_source) $(pwre_conf_incdir) $(pwre_conf_incdirgtk) $(pwre_conf_incdirgst)
  else
    csetos 	:= $(pwre_conf_cc_define)
    cinc          := -I$(inc_dir) -I$(einc_dir) -I$(co_source) $(pwre_conf_incdir) $(pwre_conf_incdirgtk) $(pwre_conf_incdirgst)
  endif
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

warnings := -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-parameter -Wno-unused-but-set-variable -Wno-sign-compare -Wno-missing-field-initializers -Wno-cast-function-type -Wno-implicit-fallthrough -Wno-narrowing

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
cxxflags	:= $(cflags)
clis		= /lis=$(list)
dolist	= /lis=$(list)
domap		= -Xlinker -Map -Xlinker $(map)

log_mkdir	= echo "I $(time)  build-$(pwre_bmodule), Dir created $@" | tee -a $(logfile)
log_cp		= echo "I $(time)  build-$(pwre_bmodule), Copy $<" | tee -a $(logfile)
log_exp		= echo "I $(time)  build-$(pwre_bmodule), Exporting $<" | tee -a $(logfile)
log_gen		= echo "I $(time)  build-$(pwre_bmodule), Generated $@" | tee -a $(logfile)
log_lib		= echo "I $(time)  build-$(pwre_bmodule), Building archive $@" | tee -a $(logfile)
log_pyext	= echo "I $(time)  build-$(pwre_bmodule), Building Python c extension $@" | tee -a $(logfile)
log_doxy	= echo "I $(time)  build-$(pwre_bmodule), Doxygen documentation $@" | tee -a $(logfile)
log_no		= echo "I $(time)  build-$(pwre_bmodule), Not building $@" | tee -a $(logfile)
log_h_h		= echo "I $(time)  build-$(pwre_bmodule), Exporting $<" | tee -a $(logfile)
log_x_h		= echo "I $(time)  build-$(pwre_bmodule), Exporting $< to $(notdir $@)" | tee -a $(logfile)
#log_c_d		= echo "I $(time)  build-$(pwre_bmodule), Updating dependencies $<" | tee -a $(logfile)
log_c_obj	= echo "I ${time}  build-$(pwre_bmodule), Compiling $<" | tee -a $(logfile)
log_c_lib	= echo "I $(time)  build-$(pwre_bmodule), Compiling $(notdir $(basename $@))($<)" | tee -a $(logfile)
log_x_lib	= echo "I $(time)  build-$(pwre_bmodule), Compiling $(notdir $(basename $@))($<)" | tee -a $(logfile)
log_obj_lib	= echo "I $(time)  build-$(pwre_bmodule), Archiving lasse $(notdir $(basename $@))($(notdir $<))" | tee -a $(logfile)
log_hlp_hlb	= echo "I $(time)  build-$(pwre_bmodule), Exporting $(notdir $(basename $@))($<)" | tee -a $(logfile)
log_sc_lib	= echo "I $(time)  build-$(pwre_bmodule), Compiling $(notdir $(basename $@))($<)" | tee -a $(logfile)
log_sc_obj	= echo "I $(time)  build-$(pwre_bmodule), Compiling $<" | tee -a $(logfile)
log_uil_uid	= echo "I $(time)  build-$(pwre_bmodule), Compiling $<" | tee -a $(logfile)
log_g_m1	= echo "I $(time)  build-$(pwre_bmodule), Exporting $<" | tee -a $(logfile)
log_cld_lib	= echo "I $(time)  build-$(pwre_bmodule), Compiling $(notdir $(basename $@))($<)" | tee -a $(logfile)
log_link_exe	= echo "I $(time)  build-$(pwre_bmodule), Linking $@" | tee -a $(logfile)
log_wb_load_wb_vers	= echo "I $(time)  build-$(pwre_bmodule), Exporting $<" | tee -a $(logfile)
log_xbm_xbm	= echo "I $(time)  build-$(pwre_bmodule), Exporting $<" | tee -a $(logfile)
log_frm_frm	= echo "I $(time)  build-$(pwre_bmodule), Exporting $<" | tee -a $(logfile)
log_graph_graph	= echo "I $(time)  build-$(pwre_bmodule), Exporting $<" | tee -a $(logfile)
log_msg_h	= echo "I $(time)  build-$(pwre_bmodule), Compiling $<" | tee -a $(logfile)
log_java_class	= echo "I $(time)  build-$(pwre_bmodule), Compiling $<" | tee -a $(logfile)

endif
