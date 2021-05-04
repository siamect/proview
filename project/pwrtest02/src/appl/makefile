my : demo_all

include $(pwr_exe)/pwrp_rules.mk
#asan = -fsanitize=address -lasan

demo_modules = $(pwrp_exe)/wa_ldhtest \
	       $(pwrp_exe)/wa_pwrwbtest.py \
	       $(pwrp_exe)/ra_test.py

wb_msg_eobjs=$(pwr_obj)/pwr_msg_wb.o
rt_msg_eobjs=$(pwr_obj)/pwr_msg_rt.o $(pwr_obj)/pwr_msg_rs.o $(pwr_obj)/pwr_msg_co.o $(pwr_obj)/pwr_msg_flow.o $(pwr_obj)/pwr_msg_glow.o $(pwr_obj)/pwr_msg_ge.o
os=$(subst os_,,$(pwre_os))
hw=$(subst hw_,,$(pwre_hw))

demo_all : $(demo_modules)

$(pwrp_exe)/wa_ldhtest : $(pwrp_appl)/wa_ldhtest.cpp
	@ echo "Building wa_ldhtest"
	. $(pwre_broot)/pwre_$(hw)_$(os).cnf; \
	g++ -g -o $(target) $(source) -DOS_LINUX=1 -Wall -I$(pwr_inc) -I$(pwrp_inc) -L$(pwr_lib) \
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

