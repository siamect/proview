
pwrp_ar = $(pwrp_lib)/libpwrp.a

source  	= $<
target  	= $@
tname		= $(notdir $(basename $@))
cc 		:= gcc
cxx             := g++
ld		:= gcc
ldxx		:= g++
linkflags	:= -g -L/usr/local/lib -L$(pwr_obj) -L$(pwrp_obj) -lm -lrt
cflags		:= -c -g -Wall -DGNU_SOURCE -D_REENTRANT
cxxflags 	:= $(cflags) -Wno-deprecated
linkflags	:= -g -L/usr/local/lib -L$(pwr_lib) -L$(pwrp_lib) -lrt
csetos		:= -DOS_LINUX=1 -DOS=linux -DHW_X86=1 -DHW=x86
cinc		:= -I$(pwr_inc) -I$(pwrp_inc) -I$(pwrp_cmn)/common/inc -I/usr/local/include -I/usr/X11R6/include -I$(jdk)/include -I$(jdk)/include/linux


.SUFFIXES:

$(pwrp_inc)/pwr_%classes.h : $(pwrp_db)/%.wb_load
	@ echo "Create struct $(notdir $(source))"
	@ wb_cmd create struct
	@ co_convert -x -d $(pwrp_exe) $(source)

$(pwrp_inc)/%.h : %.h
	@ echo "Copy $(notdir $(source))"
	@ cp $(source) $(target)

$(pwrp_obj)/%.o : %.c
	@ echo "cc   $(notdir $(source))"
	@ $(cc) $(cflags) $(csetos) $(cinc) -c -o $(target) $(source)

($(pwrp_obj)/%.o) : %.c
	@ echo "cc ar $(notdir $(source))"
	@ $(cc) $(cflags) $(csetos) $(cinc) -c -o $% $(source)
	@ ar r $(target) $%

$(pwrp_obj)/%.o : %.cpp
	@ echo "cc   $(notdir $(source))"
	@ $(cxx) $(cxxflags) $(csetos) $(cinc) -c -o $(target) $(source)

($(pwrp_obj)/%.o) : %.cpp
	@ echo "cc ar $(notdir $(source))"
	@ $(cxx) $(cxxflags) $(csetos) $(cinc) -c -o $% $(source)
	@ ar r $(target) $%

$(pwrp_inc)/%.h: %.msg
	@ echo "Msg to h $(source)"
	@ $(pwr_exe)/tools_msg2cmsg $(source) $(pwrp_obj)/$(tname).cmsg $(pwrp_inc)/$(tname).h

$(pwrp_obj)/%.cmsg: %.msg
	@ echo "Msg to h $(source)"
	@ $(pwr_exe)/tools_msg2cmsg $(source) $(pwrp_obj)/$(tname).cmsg $(pwrp_inc)/$(tname).h


%.o : %.cmsg
	@ echo "cmsg to obj $(source)"
	@ $(pwr_exe)/tools_cmsg2c -b $(tname) $(source) $(pwrp_tmp)/$(tname).c
	@ $(cc) $(cflags) $(csetos) $(cinc) -c -o $(target) $(pwrp_tmp)/$(tname).c

$(pwrp_root)/vax_eln/exe/%.exe : %.c
	@ echo "cc eln $< $@"
	@ rsh $(pwr_build_host_vax_eln) @pwr_cc '"'$(pwrp_root)'"' $(pwrp_projectname) "1" "1" $(tname)

$(pwrp_root)/vax_eln/obj/%.obj : %.c
	echo "cc eln $< $@"
	rsh $(pwr_build_host_vax_eln) @pwr_cc '"'$(pwrp_root)'"' $(pwrp_projectname) "1" "2" $(tname)

$(pwrp_root)/vax_eln/exe/%.sys : $(pwrp_load)/%.dat
	@ echo "ebuild $(tname)"
	@ rsh $(pwr_build_host_vax_eln) @pwr_cc '"'$(pwrp_root)'"' $(pwrp_projectname) "1" "3" $(tname)








