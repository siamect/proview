my : demo_all

include $(pwr_exe)/pwrp_rules.mk

demo_modules = $(pwrp_exe)/ra_status



demo_all : $(demo_modules)

$(pwrp_exe)/ra_status : $(pwrp_appl)/ra_status.c
	@ echo "Building ra_status"
	@ g++ -g -o $(target) $(source) -I$(pwr_inc) -L$(pwr_lib) -lpwr_rt -lpwr_co -lpwr_msg_dummy -lrpcsvc -lpthread -lm -lrt
