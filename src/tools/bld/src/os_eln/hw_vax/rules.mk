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
 include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif

.SUFFIXES:

(%.obj) : %.obj
	@ $(log_obj_lib)
	@ $(ar) $(arflags) $(target) $(source)



$(obj_dir)/%.obj : %.c
ifeq ($(nodep),)
	@ $(log_c_d)
	@ define/user rpc eln$$rpc
	@ define/user sys eln$$
	@ define/user vaxc$$include eln$$
	@ $(cc) $(cflags) $(csetos) $(cdep) $(cinc) $(source) $(clib) 
	@ dep_i2make $(tdepi) $(tdir)$(tname).d $(tname).obj $(depaddpath) $(dir $@)  
	@ if f$$search("$(tdepi)") .nes. "" then $(rm) $(rmflags) $(tdepi);*
	@ $(pur) $(purflags) $(to-vms $(tdir)$(tname).d)
endif
	@ $(log_c_obj)
	@ define/user rpc eln$$rpc
	@ define/user sys eln$$
	@ define/user vaxc$$include eln$$
	@ $(cc) $(cflags) $(depmms) $(csetos) $(cinc) $(cobj) $(clis) $(source) $(clib) 
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif



#
# Start exe_generic rules
#
ifdef exe_generic_mk
$(bld_dir)/%.obj : %.c
ifeq ($(nodep),)
	@ $(log_c_d)
	@ define/user rpc eln$$rpc
	@ define/user sys eln$$
	@ define/user vaxc$$include eln$$
	@ $(cc) $(cflags) $(csetos) $(cdep) $(cinc) $(source) $(clib) 
	@ dep_i2make $(tdepi) $(tdir)$(tname).d $(tname).obj $(depaddpath) $(dir $@)  
	@ if f$$search("$(tdepi)") .nes. "" then $(rm) $(rmflags) $(tdepi);*
	@ $(pur) $(purflags) $(to-vms $(tdir)$(tname).d)
endif
	@ $(log_c_obj)
	@ define/user rpc eln$$rpc
	@ define/user sys eln$$
	@ define/user vaxc$$include eln$$
	@ $(cc) $(cflags) $(csetos) $(cinc) $(cobj) $(clis) $(source) $(clib) 
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif
endif
#
# End exe_generic rules
#



#
# Start lib_generic rules
#
ifdef lib_generic_mk
(%.obj) : %.c
ifeq ($(nodep),)
	@ $(log_c_d)
	@ define/user rpc eln$$rpc
	@ define/user sys eln$$
	@ define/user vaxc$$include eln$$
	@ $(cc) $(cflags) $(csetos) $(cdep) $(cinc) $(source) $(clib) 
	@ dep_i2make $(tdepi) $(bld_dir)/$(sname).d $(sname).obj $(depaddlib) $(export_lib)  
	@ !if f$$search("$(tdepi)") .nes. "" then $(rm) $(rmflags) $(tdepi);*
	@ $(pur) $(purflags) $(to-vms $(bld_dir)/$(sname).d)
endif
	@ $(log_c_lib)
	@ define/user rpc eln$$rpc
	@ define/user sys eln$$
	@ define/user vaxc$$include eln$$
	@ $(cc) $(cflags) $(csetos) $(cinc) $(clibobj) $(clis) $(source) $(clib) 
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif
	@ $(ar) $(arflags) $(to-vms $(export_lib)) $(libobj)
	@ if f$$search("$(libobj)") .nes. "" then $(rm) $(rmflags) $(libobj);*

endif
#
# End lib_generic rules
#




(%.obj) : %.mar
	@ $(log_c_lib)
	@ mac $(clibobj) $(clis) $(source)
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif
	@ $(ar) $(arflags) $(target) $(libobj)
	@ if f$$search("$(libobj)") .nes. "" then $(rm) $(rmflags) $(libobj);*

(%.obj) : %.pas
	@ $(log_pas_lib)
	@ epas $(pflags) $(clibobj) $(clis) $(source),$(target)/lib,eln$$:rtlobject.olb/lib 
ifeq ($(pwre_btype),rls)
	@ if f$$search("$(list)") .nes. "" then $(pur) $(purflags) $(list)
endif
	@ $(ar) $(arflags) $(target) $(libobj)
	@ if f$$search("$(libobj)") .nes. "" then $(rm) $(rmflags) $(libobj);*




$(inc_dir)/%.h : %.h
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)

$(inc_dir)/%.h : %.x
	@ $(log_x_h)
	@ saved_def = f$$environment("default")
	@ set def $(sdir)
	@ rpcgen /header_file /out=$(to-vms $(tmp_dir)/$(tname).h) $(sname).x
	@ def/user sys$$output $(target)        
	@ sed -e "s/<string.h>/string/" -e "s/\""UCX\$$RPC\:TYPES.H\""/<rpc\/types.h>/" $(to-vms $(tmp_dir)/$(tname).h)
	@ if f$$search("$(to-vms $(tmp_dir)/$(tname).h)") .nes. "" then $(rm) $(rmflags) $(to-vms $(tmp_dir)/$(tname).h);*
	@ set def 'saved_def

(%_xdr.obj) : %.x
	@ $(log_x_lib)
	@ saved_def = f$$environment("default")
	@ set def $(sdir)
	@ rpcgen /xdr_file /out=$(to-vms $(tmp_dir)/$(sname)_xdr.c) $(sname).x
	@ def/user sys$$output $(to-vms $(bld_dir)/$(sname)_xdr.c)        
	@ sed -e "s/<string.h>/string/" -e "s/\""UCX\$$RPC\:RPC.H\""/<rpc\/rpc.h>/" $(to-vms $(tmp_dir)/$(sname)_xdr.c)
	@ if f$$search("$(to-vms $(tmp_dir)/$(sname)_xdr.c)") .nes. "" then $(rm) $(rmflags) $(to-vms $(tmp_dir)/$(sname)_xdr.c);*
	@ set def 'saved_def
	@ define/user rpc eln$$rpc
	@ define/user sys eln$$
	@ define/user vaxc$$include eln$$
	@ $(cc) $(cflags) $(csetos) $(cinc) $(clibobj) $(clis) $(to-vms $(bld_dir)/$(sname)_xdr.c) $(clib)
	@ $(pur) $(purflags) $(to-vms $(bld_dir)/$(sname)_xdr.c)
	@ $(ar) $(arflags) $(to-vms $(export_lib)) $(libobj)
	@ if f$$search("$(libobj)") .nes. "" then $(rm) $(rmflags) $(libobj);*

%.hlb : %.hlp
	@ $(log_hlp_hlb)
	@ $(libr) $(librflags) /create/help $(target) $(source)
	@ if f$$search("$(target)") .nes. "" then $(pur) $(purflags) $(target)


# The object module name should really be like:
# @ objname = "pwr_obj:"+f$parse("$(target_name)",,,"NAME")+".obj"
# but since the gsdc program uses the same string for source and target.
# We must use pwr_inc as .obj-file directory until this is fixed
#
# 970125 ML. It will now also create a .pas-file.

$(inc_dir)/%.h : %.msg
	@ $(log_msg_h)
	@ objname = "$(to-vms $(bld_dir)/$(notdir $(basename $@))$(obj_ext))"
	@ pasname = "$(to-vms $(bld_dir)/$(notdir $(basename $@)).pas)"
	@ message $(msgflags)/obj='objname' $(source)
	@ mc pwr_exe:errh_gsdmmopasconst 'objname'
	@ $(mv) 'pasname' $(to-vms $(inc_dir)/)
	@ $(ar) $(arflags) $(to-vms $(export_lib)) 'objname'
	@ mc pwr_exe:pwr_msggen 'objname' $(target)
	@ if f$$search(objname) .nes. "" then $(rm) 'objname';*
		
		
#.msg.h
#	@ $(msghlog)
#	@ objname = "pwr_inc:"+f$$parse("$(target_name)",,,"NAME")+".obj"
#	@ message $(mflags)/obj='objname' $(source)
#	@ mc pwr_exe:errh_gsdmmocdef 'objname'
#	@- $(pur) $(purflags)/nolog $(target)
##	@- $(del) $(delflags) 'objname';*
#
#.msg.olb
#	@ $(msgolblog)
#	@ objname = "pwr_obj:"+f$$parse("$(target_name)",,,"NAME")+".obj"
#	@ message $(mflags)/obj='objname' $(source)
#	@ if f$$search(f$$parse("$(pwr_msg_olb)")) .eqs. "" -
#		then write sys$output "Creating library: " + f$$parse("$(pwr_msg_olb)")
#	@ if f$$search(f$$parse("$(pwr_msg_olb)")) .eqs. "" -
#		then $(libr) /create/log $(pwr_msg_olb)
#	@ $(libr) $(librflags) $(pwr_msg_olb) 'objname'
#	@- $(del) $(delflags) 'objname';*
#	@ $(libr) /extr=*/output=pwr_lib:pwr_msg.obj $(pwr_msg_olb)
#	@- $(link) $(linkflags) /notrace/exe=pwr_exe:pwr_msg pwr_lib:pwr_msg.obj
#	@- $(pur) $(purflags)/nolog pwr_lib:pwr_msg.obj
#	@- $(pur) $(purflags)/nolog pwr_exe:pwr_msg.exe
#
#.msg.pas
#	@ $(msgplog)
#	@ objname = "pwr_inc:"+f$$parse("$(target_name)",,,"NAME")+".obj"
#	@ message $(mflags)/obj='objname' $(source)
#	@ mc pwr_exe:errh_gsdmmopasconst 'objname'
#	@- $(pur) $(purflags)/nolog $(target)
#	@- $(del) $(delflags) 'objname';*

# The linker rules implies a .OPT file as the first source. This
# file contains all files and options to the linker. The standard
# libraries are included by default...

%.exe : %.link
	@ $(log_link_exe)
	@- $ define/user pwr_olb $(pwr_olb)
	@- $ define/user pwrd_'$(sname)'_build yes
	@ \@$(source)

$(exe_dir)/%.exe : %.opt
	@ $(log_opt_exe)
	@ $(expand_opt) $(source) $(to-vms $(kit_dir)/$(sname)$(opt_ext))
	@ define/user dcio eln$$:dcio
	@ define/user cmsc eln$$:cmsc
	@ define/user ipcshr eln$$:ipcshr
	@ define/user langmsc eln$$:langmsc
	@ define/user pascalmsc eln$$:pascalmsc
	@ define/user libcommon eln$$:libcommon
	@ define/user dap eln$$:dap
	@ define/user elnaccess eln$$:elnaccess
	@ define/user getmsgshr eln$$:getmsgshr
	@ define/user fileutil eln$$:fileutil
	@ define/user dmath eln$$:dmath
	@ define/user dcmath eln$$:dcmath
	@ define/user dda eln$$:dda
	@ define/user prgloader eln$$:prgloader
	@ define/user auxcshr eln$$:auxcshr
	@ define/user libmsc eln$$:libmsc
	@ define/user libvm eln$$:libvm
	@ define/user dpascalio eln$$:dpascalio
	@ define/user icsshr eln$$:icsshr
	@ define/user latshr eln$$:latshr
	@ define/user eln$$pshr eln$$:eln$$pshr
	@ define/user pwr_obj $(to-vms $(bld_dir)/),$(to-vms $(obj_dir)/),'f$$trnlnm("pwr_obj")'
	@ define/user pwr_lib $(to-vms $(lib_dir)/),'f$$trnlnm("pwr_lib")'
	@ $(link)/nosysshr $(linkflags)/exe=$(target) $(domap) $(to-vms $(kit_dir)/$(sname)$(opt_ext))/opt, \
		eln$$:crtlshare/lib,eln$$:psxshare/lib/inc=(eln$$pshr),eln$$:rtlshare/lib,eln$$:rtl/lib
ifeq ($(nodep),)
	@ dep_map2make $(map) $(to-vms $(bld_dir)/$(exe_name)$(d_ld_ext)) $@ $(exp_root)  
	@ $(pur) $(purflags) $(map)
	@ $(pur) $(purflags) $(to-vms $(bld_dir)/$(exe_name)$(d_ld_ext))
endif
	@- if f$$type(pwr_build) .nes. "" then $(pur) $(purflags) $(map)
	@- $(pur) $(purflags) $(to-vms $(kit_dir)/$(sname)$(opt_ext))
	@ dbgset $(target) 0

endif
