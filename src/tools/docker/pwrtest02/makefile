my : demo_all

include $(pwr_exe)/pwrp_rules.mk
#asan = -fsanitize=address -lasan

demo_modules = $(pwrp_exe)/wa_ldhtest \
	       $(pwrp_exe)/wa_pwrwbtest.py \
	       $(pwrp_exe)/ra_test.py

pwre_conf_libpwrco=-lpwr_co
pwre_conf_libpwrrt=-lpwr_rt -lpwr_statussrv -lpwr_co -lpwr_msg_dummy
pwre_conf_libpwrdtt=-lpwr_dtt
pwre_conf_libpwrotherio=-lpwr_usbio_dummy -lpwr_usb_dummy -lpwr_cifx_dummy -lpwr_nodave_dummy -lpwr_epl_dummy
pwre_conf_libpwrprofibus=-lpwr_pnak_dummy
pwre_conf_libpwrpowerlink=
pwre_conf_libpwrpowerlinkcn=
pwre_conf_libpwrxtt=-lpwr_xtt -lpwr_ge -lpwr_cow -lpwr_flow -lpwr_glow
pwre_conf_libpwrxttgtk= -lpwr_xtt_gtk -lpwr_ge_gtk -lpwr_cow_gtk -lpwr_flow_gtk -lpwr_glow_gtk
pwre_conf_libpwrxttqt= -lpwr_xtt_qt -lpwr_ge_qt -lpwr_cow_qt -lpwr_flow_qt -lpwr_glow_qt
pwre_conf_libpwrxttmotif= -lpwr_xtt_motif -lpwr_ge_motif -lpwr_cow_motif -lpwr_flow_motif -lpwr_glow_motif
pwre_conf_libpwrwb=-lpwr_wb
pwre_conf_libpwrwbgtk=-lpwr_wb_gtk
pwre_conf_libpwrwbqt=-lpwr_wb_qt
pwre_conf_libpwrwbmotif=-lpwr_wb_motif
pwre_conf_libpwropc=-lpwr_opc
pwre_conf_libpwrremote=-lpwr_remote
pwre_conf_libpwrnmps=-lpwr_nmps
pwre_conf_libpwrtlog=-lpwr_tlog
pwre_conf_libpwrsev=-lpwr_sev
pwre_conf_lib= -lrpcsvc -lasound -lpthread -lm -ldb -lz -lcrypt -lrt -lX11 -lmysqlclient -lsqlite3 -lhdf5_openmpi -lmpi -lrabbitmq -lusb-1.0 -lrsvg-2
pwre_conf_libwb= -ldb_cxx
pwre_conf_libmq=
pwre_conf_libwmq=
pwre_conf_libpnak=
pwre_conf_libgtk= \`pkg-config --libs gtk+-2.0\`
pwre_conf_libqt=
pwre_conf_libgst= \`pkg-config --libs gstreamer-video-1.0 gstreamer-1.0\`
wb_msg_eobjs=$(pwr_obj)/pwr_msg_wb.o
rt_msg_eobjs=$(pwr_obj)/pwr_msg_rt.o $(pwr_obj)/pwr_msg_rs.o $(pwr_obj)/pwr_msg_co.o $(pwr_obj)/pwr_msg_flow.o $(pwr_obj)/pwr_msg_glow.o $(pwr_obj)/pwr_msg_ge.o

demo_all : $(demo_modules)

$(pwrp_exe)/wa_ldhtest : $(pwrp_appl)/wa_ldhtest.cpp
	@ echo "Building wa_ldhtest"
	@ g++ -g -o $(target) $(source) -DOS_LINUX=1 -Wall -I$(pwr_inc) -I$(pwrp_inc) -L$(pwr_lib) \
	$(wb_msg_eobjs) $(rt_msg_eobjs) $(pwre_conf_libdir) \
	$(pwr_obj)/rt_io_user.o $(pwr_obj)/wb_procom.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrwbgtk) $(pwre_conf_libpwrxttgtk) \
	$(pwre_conf_libpwrwb) \
	$(pwre_conf_libpwrxtt) $(pwre_conf_libpwrwbgtk) $(pwre_conf_libpwrwb) \
	$(pwre_conf_libpwrxttgtk) $(pwre_conf_libpwrxtt) \
	`pkg-config --libs gtk+-2.0` \
	$(pwre_conf_libpwrrt) $(pwre_conf_libwb) $(pwre_conf_lib) $(asan)

$(pwrp_exe)/wa_pwrwbtest.py : $(pwrp_appl)/wa_pwrwbtest.py
	@ echo "Building wa_pwrwbtest"
	@ cp -p $(source) $(target)

$(pwrp_exe)/ra_test.py : $(pwrp_appl)/ra_test.py
	@ echo "Building ra_test"
	@ cp -p $(source) $(target)

