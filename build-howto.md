#How to build ProviewR

Authors: Robert Karlsson & Christoffer Ackelman

## 1. Preparations

Install the following packages

Mandatory:

> apt-get install doxygen gcc g++ make libasound2-dev libdb5.3-dev libdb5.3++-dev

Optional:

> apt-get install openjdk-6-jdk libmysql++-dev libsqlite3-dev libhdf5-openmpi-dev libusb-1.0.0-dev

**openjdk-6-jdk** adds support for the java web interface.
**libmysql++-dev**, **libsqlite3-dev** and **libgdf5-openmpi-dev** add support for **mysql**, **sqlite** and **hdf5** history databases respectively.
**libusb-1.0.0-dev** adds support for USB I/O cards (Velleman)

Mandatory if you want to build the QT version of ProviewR:

> apt-get install qt4-dev-tools libphonon-dev

Mandatory if you want to build the GTK version of ProviewR:

> apt-get install libgtk2.0-dev

Optional if you build the GTK version of ProviewR:

> apt-get install librsvg2-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev

**librsvg2-dev** adds support for SVG images in Ge in the GTK version of ProviewR.
**libgstreamer1.0-dev** and **libgstreamer-plugins-base1.0-dev** adds support for network video cameras in the GTK version of ProviewR.

**OBS!** If you do not have a valid display when building ProviewR, or build ProviewR on a server, see section "Building without a display" below

## 2. Easy way

Unpack the source package

> tar -xzvf pwrsrc_4.8.0.tar.gz

Enter the sourcecode root directory

> cd pwr_4.8.0

Execute the makefile

> make

When the compilation is finished, follow the instructions to insert the three lines in your .bashrc file.

## 3. Thorough way

This is a more thorough way to install the development environment, which is recommended if you will make a lots of changes in the source code and handle different version of ProviewR.

### 3.1 Setting up the environment

The first thing to do is to set up your environment properly. Add the
following lines to .bashrc:

> export pwre_bin="/home/robert/x5-6-1/src/tools/pwre/src/os_linux"
export pwre_dir_symbols="$pwre_bin/dir_symbols.mk"
source $pwre_bin/pwre_function
export pwre_env_db="/home/robert/pwre_new"

Replace /home/robert/x5-6-1 with the directory where you unpacked the sources.

### 3.2 Creating the environment

Creating and handling your environment is done with the pwre-function (added by sourcing $pwre_bin/pwre_function). See section "Cross-compiling" below about compiling a 32-bit version of ProviewR on a 64-bit machine.

> pwre

shows all possibilites of the pwre build script.

Add an environment named 'x561x86' (choose any name you like).

> pwre add x561x86

> Source root [...] ? /home/robert/x5-6-1/src  (where /home/robert/x5-6-1 is replaced with the directory where you unpacked the sources)
Import root [] ?
Build root [...] ? /home/robert/x5-6-1/rls_dbg
Build type [dbg] ?
OS [linux] ? 		(linux or macos)
Hardware [x86] ? 	(x86 or x86_64)
Description  ? Robert's environment  (Optional description)

List all existing environments:

> pwre list

> -- Defined environments:
   x460x86
--

Initiate your new environment:

> pwre init x561x86

Now, create the build tree

> pwre create_all_modules

Now the environment is set up and we can start the build procedure.

### 3.3 Building ProviewR

ProviewR is divided into several modules which can be built separately. These modules are:
The ProviewR kernel module **rt**, the ProviewR workbench **wb**, and the ProviewR runtime interface **xtt**.
As well as a number of optional, vendor-specific libraries: **abb**, **bcomp**, **inor**, **java**, **klocknermoeller**, **misc**, **nmps**, **opc**, **otherio**, **othermanu**, **profibus**, **remote**, **sev**, **siemens**, **simul**, **ssabox**, **telemecanique**, **tlog**.

To build all modules from scratch and merge them:

> pwre build_all_modules

Otherwise, you can also build one module at a time. To "move" between the different modules

> pwre module wb    (for example)

To build everything in the current module

> pwre build_all

To build parts of current module (for example lib/rt/src in kernel module)

> pwre build lib rt src

Each of the modules have their own build tree. When you are finished with a specific module it must be merged into the common build tree. This is done via:

> pwre merge

## 4. Start using ProviewR

When ProviewR has been built, there are a few final steps you have to do before you start using it.

Create directory that will keep track of your projects and volumes aswell as the different ProviewR-versions you have installed.

> mkdir /usr/pwrp/adm/db

Copy the default configuration files to this directory (if they do not already exist)

> cp $pwre_croot/src/tools/pkg/deb/adm/pwr_* /usr/pwrp/adm/db/.

Create a file name pwr_projectlist.dat in this directory and add the following line to this file:

> %base X5.6.1	/home/robert/X5-6-1/rls_dbg

where /home/robert/X5-6-1/rls_dbg is replaced by your build-root (see section 3.2).

Add the following two lines to your .bashrc-file:

> export pwra_db="/usr/pwrp/adm/db"
> source $pwra_db/pwr_setup.sh

Voila, finished !

## Powerlink (optional)
Install the following packages:

> apt-get install libpcap-dev cmake cmake-qt-gui

unpack openPOWERLINK_V2.7.1.tar.gz

> tar xvzf openPOWERLINK_V2.7.1.tar.gz

Go to the /stack/build/linux folder

> cd openPOWERLINK_V2.7.1.tar.gz/stack/build/linux

Use cmake to generate makefiles

> cmake -DMAKE_BUILD_TYPE=Release ../..

Compile and install

> make
> make install

## Note on building the QT version

If you get the error "No such file "thread/file_qt.h", then the -pthread flag was passed to the moc compiler. Remove the -pthread flag from $cinc in src/tools/bld/src/os_linux/hw_xx/variables.mk."

## Building without a display

Install the xserver-xorg-video-dummy package:
> apt-get install xserver-xorg-video-dummy

Start the dummy display with:
> Xorg -noreset +extension GLX +extension RANDR +extension RENDER -config src/tools/dummy_display/xorg.conf :99 &

Set the DISPLAY environment variable accordingly:
> export DISPLAY=:99

Build ProviewR as normal, either by executing the makefile:
> make

or by configuring the build yourself, see section "3. Thorough way" above.

## Cross-compiling

To compile a 32-bit version of ProviewR on a 64-bit system, you need to install
multiarch support and add a 32-bit architecture:

> apt-get install multiarch
> dpkg --add-architecture i386
> apt-get update

Then you need to install the libraries required to build 32-bit executables:

> apt-get install gcc-multilib g++-multilib libc6:i386 libc6-dev:i386 libc6-i386 libc6-dev-i386

You also need 32-bit versions of all the libraries required by ProviewR. The
easiest way to get these is to copy the /usr/lib/i386-linux-gnu folder and the
/lib/i386-linux-gnu/ folder from a 32-bit machine to your 64-bit machine. If
you try to install all libraries using "apt-get install x:i386" you will most
likely run into problems with some 32-bit packages replacing your 64-bit
packages and vice versa.

After that, you should be able to compile 32-bit versions by specifying "x86"
as hardware during "pwre add", see section "3.2 Creating the environment" above.

> pwre add proview32bit
...
Hardware [x86] ? x86
