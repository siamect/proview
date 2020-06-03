
from distutils.core import setup, Extension
import os

pwr_lib = os.environ['pwr_lib']
pwr_obj = os.environ['pwr_obj']
pwr_inc = os.environ['pwr_inc']
pwr_elib = os.environ['pwr_elib']
pwr_eobj = os.environ['pwr_eobj']
pwr_einc = os.environ['pwr_einc']

pwrtestmodule = Extension( name='pwrtest',
                         sources=['pwrtestmodule.cpp'],
                         define_macros=[('OS_POSIX', '1'),
                                      ('OS','linux'),
                                      ('HW_X86', '1'),
                                      ('HW', 'x86')],
                         extra_compile_args=['-g', '-O0'],
                         extra_link_args=['-O0'],
                         extra_objects=[pwr_eobj + '/pwr_msg_co.o',
                                        pwr_eobj + '/pwr_msg_rt.o'],
                         include_dirs=[pwr_inc,pwr_einc],
                         library_dirs=[pwr_lib,pwr_elib],
                         libraries=['pwr_rt','pwr_co','pwr_msg_dummy','pthread',
                                    'rt', 'm', 'crypt'],
                         language='c++'                        
                       )
                      
setup( name='pwrtest',
       version='1.0',
       url='www.proview.se',
       author='ProviewR development team',
       author_email='support@proview.se',
       license='GPL V2',
       description='ProviewR runtime interface',
       ext_modules=[pwrtestmodule])
