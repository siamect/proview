#  rules.mk -- rules common for all makefiles
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>
#
ifndef rules_mk
rules_mk := 1

ifndef variables_mk
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_sroot)/tools/bld/src/$(os_name)/variables.mk
endif

.SUFFIXES:

(%.obj) : %.obj
	@ $(log_obj_lib)
	@ $(ar) $(arflags) $(target) $(source)

$(obj_dir)/%.obj : %.c
	@ $(log_c_obj)
	@ $(cc) $(cflags) $(depmms) $(csetos) $(cinc) $(cobj) $(clis) $(source) $(clib) 
	@ dep_mms2make $(tdepmms) $(tdir)$(tname).d $(depaddpath) $(dir $@)  
	@ if f$$search("$(tdepmms)") .nes. "" then $(rm) $(rmflags) $(tdepmms);*
	@ $(pur) $(purflags) $(to-vms $(tdir)$(tname).d)
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif

$(obj_dir)/%.obj : %.cpp
	@ $(log_c_obj)
	@ $(cxx) $(cxxflags) $(depmms) $(csetos) $(cinc) $(cobj) $(clis) $(source) $(clib) 
	@ dep_mms2make $(tdepmms) $(tdir)$(tname).d $(depaddpath) $(dir $@)  
	@ if f$$search("$(tdepmms)") .nes. "" then $(rm) $(rmflags) $(tdepmms);*
	@ $(pur) $(purflags) $(to-vms $(tdir)$(tname).d)
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif

#
# Start exe_generic rules
#
ifdef exe_generic_mk
$(bld_dir)/%.obj : %.c
	@ $(log_c_obj)
	@ $(cc) $(cflags) $(depmms) $(csetos) $(cinc) $(cobj) $(clis) $(source) $(clib)
	@ dep_mms2make $(tdepmms) $(tdir)$(tname).d $(depaddpath) $(dir $@)  
	@ if f$$search("$(tdepmms)") .nes. "" then $(rm) $(rmflags) $(tdepmms);*
	@ $(pur) $(purflags) $(to-vms $(tdir)$(tname).d)
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif

$(bld_dir)/%.obj : %.cpp
	@ $(log_c_obj)
	@ $(cxx) $(cxxflags) $(depmms) $(csetos) $(cinc) $(cobj) $(clis) $(source) $(clib)
	@ dep_mms2make $(tdepmms) $(tdir)$(tname).d $(depaddpath) $(dir $@)  
	@ if f$$search("$(tdepmms)") .nes. "" then $(rm) $(rmflags) $(tdepmms);*
	@ $(pur) $(purflags) $(to-vms $(tdir)$(tname).d)
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif

$(bld_dir)/%.obj : %.sc
	@ $(log_sc_obj)
	@ scc = "$(scc)"
	@ source = f$$search("$(source)")
	@ saved_def = f$$environment("default")
	@ set def $(to-vms $(bld_dir)/)
	@ defi/user obj $(tdir)$(tname)$(obj_ext)
	@ defi/user source 'source'
	@ def/user list $(list)
	@ defi/user tmms $(tdepmms)
	@ defi/user decc$$user_include $(scinc)
	@ scc  /cc=decc $(scflags)/mms=(nosys,fi=tmms) $(csetos)/lis=list /obj=obj source $(clib)
	@ dep_mms2make $(tdepmms) $(tdir)$(tname).d $(depaddpath) $(dir $@)  
	@ set def 'saved_def
	@ $(pur) $(purflags) $(to-vms $(tdir)$(tname).d)
	@ if f$$search("$(tdepmms)") .nes. "" then $(rm) $(rmflags) $(tdepmms);*
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif

endif
#
# End exe_generic rules
#



$(obj_dir)/%.obj : %.sc
	@ $(log_sc_obj)
	@ scc = "$(scc)"
	@ source = f$$search("$(source)")
	@ saved_def = f$$environment("default")
	@ set def $(to-vms $(obj_dir)/)
	@ defi/user obj $(tdir)$(tname)$(obj_ext)
	@ defi/user source 'source'
	@ def/user list $(list)
	@ defi/user tmms $(tdepmms)
	@ defi/user decc$$user_include $(scinc)
	@ scc  /cc=decc $(scflags)/mms=(nosys,fi=tmms) $(csetos)/lis=list /obj=obj source $(clib)
	@ dep_mms2make $(tdepmms) $(tdir)$(tname).d $(depaddpath) $(dir $@)  
	@ set def 'saved_def
	@ $(pur) $(purflags) $(to-vms $(tdir)$(tname).d)
	@ if f$$search("$(tdepmms)") .nes. "" then $(rm) $(rmflags) $(tdepmms);*
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif


#
# Start lib_generic rules
#
ifdef lib_generic_mk
# It doesn't work with /obj=libobj. Why? ML 970708
(%.obj) : %.sc
	@ scc = "$(scc)"
	@ $(log_sc_lib)
	@ source = f$$search("$(source)")
	@ saved_def = f$$environment("default")
	@ set def $(to-vms $(bld_dir)/)
	@ defi/user source 'source'
	@ defi/user list $(list)
	@ defi/user tmms $(tdepmms)
	@ defi/user decc$$user_include $(scinc)
	@ scc /cc=decc $(scflags)/mms=(nosys,fi=tmms) $(csetos)/lis=list source $(clib)
	@ dep_mms2make $(tdepmms) $(bld_dir)/$(sname).d $(depaddlib) $(export_lib)  
	@ set def 'saved_def
	@ $(pur) $(purflags) $(to-vms $(bld_dir)/$(sname).d)
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif
	@ $(ar) $(arflags) $(to-vms $(export_lib)) $(libobj)
	@ if f$$search("$(libobj)") .nes. "" then $(rm) $(rmflags) $(libobj);*
	@ if f$$search("$(tdepmms)") .nes. "" then $(rm) $(rmflags) $(tdepmms);*


(%.obj) : %.c
	@ $(log_c_lib)
	@ $(cc) $(cflags) $(csetos) $(depmms) $(cinc) $(clibobj) $(clis) $(source) $(clib) 
	@ dep_mms2make $(tdepmms) $(bld_dir)/$(sname).d $(depaddlib) $(export_lib)  
	@ $(pur) $(purflags) $(to-vms $(bld_dir)/$(sname).d)
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif
	@ $(ar) $(arflags) $(to-vms $(export_lib)) $(libobj)
	@ if f$$search("$(libobj)") .nes. "" then $(rm) $(rmflags) $(libobj);*
	@ if f$$search("$(tdepmms)") .nes. "" then $(rm) $(rmflags) $(tdepmms);*

#	cxx/prep=$(sname).pre $(cinc) $(source)


(%.obj) : %.cpp
	@ $(log_c_lib)
	@ $(cxx) $(cxxflags) $(csetos) $(depmms) $(cinc) $(clibobj) $(clis) $(source) $(clib) 
	@ dep_mms2make $(tdepmms) $(bld_dir)/$(sname).d $(depaddlib) $(export_lib)  
	@ $(pur) $(purflags) $(to-vms $(bld_dir)/$(sname).d)
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif
	@ $(ar) $(arflags) $(to-vms $(export_lib)) $(libobj)
	@ if f$$search("$(libobj)") .nes. "" then $(rm) $(rmflags) $(libobj);*
	@ if f$$search("$(tdepmms)") .nes. "" then $(rm) $(rmflags) $(tdepmms);*
endif
#
# End lib_generic rules
#


$(inc_dir)/%.h : %.h
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(inc_dir)/%.hpp : %.hpp
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)


$(inc_dir)/%.h : %.pdr
	@ $(log_x_h)
	@ tools_pdrgen -h -o $(target) $(source)

(%_pdr.obj) : %.pdr
	@ $(log_x_lib)
	@ tools_pdrgen -c -o $(bld_dir)/$(sname)_pdr.c $(source)
	@ $(cc)/warn=dis=TRAILCOMMA $(cflags) $(csetos) $(cinc) $(clibobj) $(clis) $(to-vms $(bld_dir)/$(sname)_pdr.c)
	@ $(pur) $(purflags) $(to-vms $(bld_dir)/$(sname)_pdr.c)
	@ $(ar) $(arflags) $(to-vms $(export_lib)) $(libobj)
	@ if f$$search("$(libobj)") .nes. "" then $(rm) $(rmflags) $(libobj);*



$(inc_dir)/%.h : %.x
	@ $(log_x_h)
	@ saved_def = f$$environment("default")
	@ set def $(sdir)
	@ rpcgen /header_file /out=$(target) $(sname).x
	@ set def 'saved_def


(%_xdr.obj) : %.x
	@ $(log_x_lib)
	@ saved_def = f$$environment("default")
	@ set def $(sdir)
	@ rpcgen /xdr_file /out=$(to-vms $(bld_dir)/$(sname)_xdr.c) $(sname).x
	@ set def 'saved_def
	@ $(cc)/warn=dis=TRAILCOMMA $(cflags) $(csetos) $(cinc) $(clibobj) $(clis) $(to-vms $(bld_dir)/$(sname)_xdr.c)
	@ $(pur) $(purflags) $(to-vms $(bld_dir)/$(sname)_xdr.c)
	@ $(ar) $(arflags) $(to-vms $(export_lib)) $(libobj)
	@ if f$$search("$(libobj)") .nes. "" then $(rm) $(rmflags) $(libobj);*

%.hlb : %.hlp
	@ $(log_hlp_hlb)
	@ $(ar) $(arflags) /create/help $(target) $(source)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(lib_dir)/%.hlb : %.hlp
	@ $(log_hlp_hlb)
	@ $(ar) $(arflags) /create/help $(target) $(source)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

(%.sqlmod) : %.sqlmod
	@ $(log_sqlmod_lib)
	@ $(sqlmod) $(sqlmodflags) /obj=$(libobj) $(dolist) $(source)
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif
	@ $(ar) $(arflags) $(target) $(libobj)
	@ if f$$search("$(libobj)") .nes. "" then $(rm) $(rmflags) $(libobj);*

$(bld_dir)/%.obj : %.sqlmod
	@ $(log_sqlmod_obj)
	@ defi/user pwrp_db pwr_db
	@ $(sqlmod) $(sqlmodflags) /obj=$(libobj) $(dolist) $(source)

#%.uid : %.uil
#	@ $(log_uil_uid)
#	@ uil $(uilflags)/output=$(target) $(source)
#	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(lang_dir)/%.frm : %.frm
	@ $(log_frm_frm)
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(lang_dir)/%.m1 : %.g
	@ $(log_g_m1)
	@ $(gm1) $(target) "''f$$search("$(source)")'"
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(lang_dir)/%.xbm : %.xbm
	@ $(log_xbm_xbm)
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

(%.obj) : %.cld
	@ $(log_cld_lib)
	@ set command/object=$(libobj) $(source)
	@ $(ar) $(arflags) $(target) $(libobj)
	@ if f$$search("$(libobj)") .nes. "" then $(rm) $(rmflags) $(libobj);*

$(bld_dir)/%.obj : %.cld
	@ $(log_cld_lib)
	@ set command/object=$(target) $(source)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(db_dir)/%.graph : %.graph
	@ $(log_graph_graph)
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

#
# The object module name should really be like:
# @ objname = "pwr_obj:"+f$parse("$(target_name)",,,"NAME")+".obj"
# but since the gsdc program uses the same string for source and target.
# We must use pwr_inc as .obj-file directory until this is fixed
#
$(inc_dir)/%.h : %.msg
	@ $(log_msg_h)
	@ objname = "$(to-vms $(bld_dir)/$(notdir $(basename $@))$(obj_ext))"
	@ message $(msgflags)/obj='objname' $(source)
	@ $(ar) $(arflags) $(to-vms $(export_lib)) 'objname'
	@ mc pwr_exe:pwr_msggen 'objname' $(target)
	@ if f$$search(objname) .nes. "" then $(rm) $(rmflags) 'objname';

#
# The linker rules implies a .OPT file as the first source. This
# file contains all files and options to the linker. The standard
# libraries are included by default...
#

%.exe : %.link
	@ $(log_link_exe)
	@ if f$$trnlnm("sql$user") .eqs. "" then define/user sql$user sys$library:sql$user
	@- $ define/user pwr_olb $(pwr_olb)
	@- $ define/user pwrd_'$(sname)'_build yes
	@ \@$(source)

$(exe_dir)/%.exe : %.opt
	@ $(log_opt_exe)
	@ $(expand_opt) $(source) $(to-vms $(kit_dir)/$(sname)$(opt_ext))
	@ if f$$trnlnm("sql$$user") .eqs. "" then define/user sql$$user sys$$library:sql$$user
#	- define/user pwr_olb $(pwr_olb)
	@ define/proc pwr_obj $(to-vms $(bld_dir)/),$(to-vms $(obj_dir)/),'f$$trnlnm("pwr_obj")'
	@ define/user pwr_lib $(to-vms $(lib_dir)/),'f$$trnlnm("pwr_lib")'
	@ if "$(link)".eqs."cxxlink" then set default sys$$login
	@ $(link) $(linkflags)/exe=$(target) $(domap) $(to-vms $(kit_dir)/$(sname)$(opt_ext))/opt
	@ dep_map2make $(map) $(to-vms $(bld_dir)/$(exe_name)$(d_ld_ext)) $@ $(exp_root)  
	@ $(pur) $(purflags) $(map)
	@ $(pur) $(purflags) $(to-vms $(bld_dir)/$(exe_name)$(d_ld_ext))
	@- if f$$type(pwr_build) .nes. "" then $(pur) $(purflags) $(map)
	@- $(pur) $(purflags) $(to-vms $(kit_dir)/$(sname)$(opt_ext))
	@ mc pwr_exe:dbgset $(target) 0
	@ deass/proc pwr_obj

$(cnf_dir)/%.wb_vers : %.wb_load
	@ $(log_wb_load_wb_vers)
	@ listfile := $(to-vms $(lis_dir)/$(tname).wb_list)
	@ mc $(to-vms $(exe_dir)/)wb_load $(source) 'listfile'
	@ \@$(to-vms $(pwre_sroot)/tools/com/src/$(os_name)/$(hw_name)/)pwr_show_version $(source) $(target)
	@ if f$$search(listfile) .nes. "" then $(pur) $(purflags) 'listfile'
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)
 
$(exe_dir)/%.pwsg : %.pwsg
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/%.pwg : %.pwg
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

endif
