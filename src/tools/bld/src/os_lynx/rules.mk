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

$(bld_dir)/%.cmsg $(inc_dir)/%.h: %.msg
	@ $(log_msg_h)
	@ $(msg2cmsg) $(source) $(bld_dir)/$(tname).cmsg $(inc_dir)/$(tname).h


$(obj_dir)/%.cmsg $(inc_dir)/%.h : %.msg
	@ $(log_msg_h)
	@ $(msg2cmsg) $(source) $(obj_dir)/$(tname).cmsg $(inc_dir)/$(tname).h


$(obj_dir)/%.o : $(obj_dir)/%.cmsg
	@ $(log_cmsg_obj)
	@ $(pwr_exe)/tools_cmsg2c -b $(comp_name) $(source) $(tmp_dir)/$(sname).c 
	@ $(cc) $(cflags) $(csetos) $(cinc) $(cobj) $(tmp_dir)/$(sname).c


$(bld_dir)/%.c : %.l
	@ $(log_l_obj)
	@ $(lex) -o$(bld_dir)/$(sname).c $(source)



%.o : %.c
	@ echo "Warning, rule shall normally not be used: %.o : %.c"
ifeq ($(nodep),)
	@ $(log_c_d)
	@ $(SHELL) -ec '$(cc) -MM $(cinc) $(csetos) $(source) \
	  | sed '\''s|$*\.o[ ]*|$(bld_dir)/&|g'\'' > $(bld_dir)/$(sname).d'
endif
	@ $(log_c_obj)
	@ $(cc) $(cflags) $(csetos) $(cinc) $(cobj) $(source)


(%.o) : %.c
	@ echo "Error, rule shall not be used: (%.o) : %.c"


$(bld_dir)/%.o : %.c
ifeq ($(nodep),)
	@ $(log_c_d)
	@ $(SHELL) -ec '$(cc) -MM $(cinc) $(csetos) $(source) \
	  | sed '\''s|$*\.o[ ]*|$(bld_dir)/&|g'\'' > $(bld_dir)/$(sname).d'
endif
	@ $(log_c_obj)
	@ $(cc) $(cflags) $(csetos) $(cinc) $(cobj) $(source)


$(bld_dir)/%.o : $(tmp_dir)/%.c
ifeq ($(nodep),)
	@ $(log_c_d)
	@ $(SHELL) -ec '$(cc) -MM $(cinc) $(csetos) $(source) \
	  | sed '\''s|$*\.o[ ]*|$(bld_dir)/&|g'\'' > $(bld_dir)/$(sname).d'
endif
	@ $(log_c_obj)
	@ $(cc) $(cflags) $(csetos) $(cinc) $(cobj) $(source)


$(obj_dir)/%.o : %.c
ifeq ($(nodep),)
	@ $(log_c_d)
	@ $(SHELL) -ec '$(cc) -MM $(cinc) $(csetos) $(source) \
	  | sed '\''s|$*\.o[ ]*|$(obj_dir)/&|g'\'' > $(obj_dir)/$(sname).d'
endif
	@ $(log_c_obj)
	@ $(cc) $(cflags) $(csetos) $(cinc) $(cobj) $(source)


%.o : %.cpp
	@ echo "Error, rule shall not be used: %.o : %.cpp"


(%.o) : %.cpp
	@ echo "Error, rule shall not be used: (%.o) : %.cpp"


$(bld_dir)/%.o : %.cpp
ifeq ($(nodep),)
	@ $(log_c_d)
	@ $(SHELL) -ec '$(cxx) -MM $(cinc) $(csetos) $(cimlib) $(source) \
	  | sed '\''s|$*\.o[ ]*|$(bld_dir)/&|g'\'' > $(bld_dir)/$(sname).d'
endif
	@ $(log_c_obj)
	@ $(cxx) $(cxxflags) $(csetos) $(cimlib) $(cinc) $(cobj) $(source)


$(bld_dir)/%.o : $(tmp_dir)/%.cpp
ifeq ($(nodep),)
	@ $(log_c_d)
	@ $(SHELL) -ec '$(cxx) -MM $(cinc) $(csetos) $(cimlib) $(source) \
	  | sed '\''s|$*\.o[ ]*|$(bld_dir)/&|g'\'' > $(bld_dir)/$(sname).d'
endif
	@ $(log_c_obj)
	@ $(cxx) $(cxxflags) $(csetos) $(cimlib) $(cinc) $(cobj) $(source)


$(obj_dir)/%.o : %.cpp
ifeq ($(nodep),)
	@ $(log_c_d)
	@ $(SHELL) -ec '$(cxx) -MM $(cinc) $(csetos) $(cimlib) $(source) \
	  | sed '\''s|$*\.o[ ]*|$(obj_dir)/&|g'\'' > $(obj_dir)/$(sname).d'
endif
	@ $(log_c_obj)
	@ $(cxx) $(cxxflags) $(csetos) $(cimlib) $(cinc) $(cobj) $(source)


$(inc_dir)/%.h : %.x
	@ $(log_x_h)
	@ if [ -e $(target) ]; then \
		$(rm) $(target); \
	fi
	@ rpcgen -h -o $(target) $(source)


$(bld_dir)/%_xdr.o : %.x
	@ $(log_x_lib)
	@ rpcgen -c -o $(bld_dir)/$(sname)_xdr.t $(source)
	@ sed 's/\(#include "\)\(.*$(sname)[^"]*\)/\1$(sname).h/' $(bld_dir)/$(sname)_xdr.t \
	  | grep -v "register int32_t \*buf\;"> $(bld_dir)/$(sname)_xdr.c
	@ rm $(bld_dir)/$(sname)_xdr.t
ifeq ($(nodep),)
	@ $(SHELL) -ec '$(cc) -MM $(cinc) $(csetos) $(bld_dir)/$(sname)_xdr.c \
	  | sed '\''s|$*_xdr\.o[ ]*|$(bld_dir)/&|g'\'' \
	  | sed '\''s|$(bld_dir)/$(sname)_xdr.c||'\'' > $(bld_dir)/$(sname)_xdr.d'
endif
	@ $(cc) $(cflags) $(csetos) $(cinc) -c -o $(bld_dir)/$(sname)_xdr.o $(bld_dir)/$(sname)_xdr.c
	@ mv $(bld_dir)/$(sname)_xdr.c $(bld_dir)/$(sname)_xdr_compiled.c


$(inc_dir)/%.h : %.pdr
	@ $(log_x_h)
	@ if [ -e $(target) ]; then \
		$(rm) $(target); \
	fi
	@ tools_pdrgen -h -o $(target) $(source)


$(bld_dir)/%_pdr.o : %.pdr
	@ $(log_x_lib)
	@ tools_pdrgen -c -o $(bld_dir)/$(sname)_pdr.c $(source)
ifeq ($(nodep),)
	@ $(SHELL) -ec '$(cc) -MM $(cinc) $(csetos) $(bld_dir)/$(sname)_pdr.c \
	  | sed '\''s|$*_pdr\.o[ ]*|$(bld_dir)/&|g'\'' \
	  | sed '\''s|$(bld_dir)/$(sname)_pdr.c||'\'' > $(bld_dir)/$(sname)_pdr.d'
endif
	@ $(cc) $(cflags) $(csetos) $(cinc) -c -o $(bld_dir)/$(sname)_pdr.o $(bld_dir)/$(sname)_pdr.c
	@ mv $(bld_dir)/$(sname)_pdr.c $(bld_dir)/$(sname)_pdr_compiled.c


(%.o) : %.o
	@ echo "Error, rule shall not be used: (%.o) : %.o"


$(inc_dir)/%.h : %.h
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)
	@ chmod u+w $(target)

$(inc_dir)/%.hpp : %.hpp
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)
	@ chmod u+w $(target)

$(exe_dir)/%.pwsg : %.pwsg
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/%.pwg : %.pwg
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

endif
