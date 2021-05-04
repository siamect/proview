
from distutils.core import setup, Extension
import subprocess
import os

pwr_lib = os.environ['pwr_elib']
pwr_obj = os.environ['pwr_eobj']
pwr_inc = os.environ['pwr_einc']
try:
    pwre_conf_qt = os.environ['PWRE_CONF_QT']
except:
    pwre_conf_qt = ""

pwre_conf_libos = os.environ['pwre_conf_lib'][1:].split(' ')
pwre_conf_lib = []
for lib in pwre_conf_libos:
    pwre_conf_lib.append(lib[2:])    

pwre_conf_libwb = []
pwre_conf_libwbos = os.environ['pwre_conf_libwb'][1:].split(' ')
for lib in pwre_conf_libwbos:
    pwre_conf_libwb.append(lib[2:])    

if pwre_conf_qt == "1":
    libs = ['pwr_wb_qt', 'pwr_xtt_qt', 'pwr_ge_qt', 'pwr_cow_qt',
            'pwr_flow_qt', 'pwr_glow_qt',
            'pwr_wb', 'pwr_xtt', 'pwr_ge', 'pwr_cow', 'pwr_flow', 'pwr_glow',
            'pwr_wb_qt', 'pwr_xtt_qt', 'pwr_ge_qt', 'pwr_cow_qt', 'pwr_flow_qt', 'pwr_glow_qt',
            'pwr_wb', 'pwr_xtt', 'pwr_ge', 'pwr_cow',
            'pwr_flow', 'pwr_glow', 
            'pwr_rt', 'pwr_statussrv', 'pwr_co', 'pwr_msg_dummy']
    xlibs = ['QtCore', 'QtGui']
else:
    libs = ['pwr_wb_gtk', 'pwr_xtt_gtk', 'pwr_ge_gtk', 'pwr_cow_gtk',
            'pwr_flow_gtk', 'pwr_glow_gtk',
            'pwr_wb', 'pwr_xtt', 'pwr_ge', 'pwr_cow', 'pwr_flow', 'pwr_glow',
            'pwr_wb_gtk', 'pwr_xtt_gtk', 'pwr_ge_gtk', 'pwr_cow_gtk', 'pwr_flow_gtk', 'pwr_glow_gtk',
            'pwr_wb', 'pwr_xtt', 'pwr_ge', 'pwr_cow',
            'pwr_flow', 'pwr_glow', 
            'pwr_rt', 'pwr_statussrv', 'pwr_co', 'pwr_msg_dummy']
    xlibs = ['gtk-x11-2.0']
pwrwbmodule = Extension( name='pwrwb',
                         sources=['pwrwbmodule.cpp'],
                         define_macros=[('OS_POSIX', '1'),
                                      ('OS','linux'),
                                      ('HW_X86', '1'),
                                      ('HW', 'x86')],
                         extra_objects=[pwr_obj + '/pwr_msg_co.o',
                                        pwr_obj + '/pwr_msg_rt.o',
                                        pwr_obj + '/pwr_msg_wb.o',
                                        pwr_obj + '/rt_io_user.o',
                                        pwr_obj + '/wb_procom.o',
                                        pwr_obj + '/stdsoap2.o'],
                         include_dirs=[pwr_inc],
                         library_dirs=[pwr_lib],
                         libraries=libs +
                         pwre_conf_lib + pwre_conf_libwb + xlibs,
#                        extra_link_args=['-L/usr/lib/x86_64-linux-gnu', subprocess.getoutput('pkg-config --libs gtk+-2.0')],
                         language='c++'
                       )
                      
setup( name='pwrwb',
       version='1.0',
       url='www.proview.se',
       author='ProviewR development team',
       author_email='support@proview.se',
       license='GPL V2',
       description='ProviewR workbench interface',
       ext_modules=[pwrwbmodule])
