#!/bin/bash

release="opensuse/leap"
release_name="opensuse"
buildversion="19-APR-2021 12:00:00"
tz="Europe/Stockholm"
build_rpi=0
gitrepo="-b stable http://192.168.0.141/git/x5-7-2/pwr/.git"
install_update="zypper refresh"
install_git="zypper -n install git make"
install_videodummy="zypper -n install xf86-video-dummy"
install_build="zypper -n install gcc gcc-c++ gtk2-devel alsa-devel libdb-4_8 \
        libdb-4_8-devel rpcsvc-proto-devel libX11-devel doxygen tar gzip \
        libmariadb-devel libsqlite3-0 libhdf5-103-openmpi2 rpcgen python3-devel \
        libtirpc-devel libusb-1_0-devel libcap-devel\
        java-11-openjdk-devel rpm-build"
install_rpi="zypper -n gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf"
install_sev="zypper -n install mariadb"
install_pwr="zypper -n install gtk2 alsa gcc-c++\
	libdb-4_8 libsqlite3-0 libtirpc3 xterm libmariadb3 \
	libusb-1_0-0 libhdf5-103-openmpi2 \
	java-11-openjdk \
	xterm xorg-x11-fonts sudo procps python3-devel python3 tar gzip glibc-locale-base"
install_pwrrt="zypper -n install -y gtk2 alsa \
	libdb-4_8 libsqlite3-0 libtirpc3 gcc-c++ xterm xorg-x11-fonts libmariadb3 \
	libusb-1_0-0 libhdf5-103-openmpi2 \
	sudo procps python3 python3-pandas python3-seaborn \
	python3-statsmodels python3-sklearn tar gzip glibc-locale-base"
install_pkg="rpm -i"
jdk_dir=/usr/lib64/jvm/java-11-openjdk-11
ver="5.8.0-1"
sver="58"
arch="x86_64"
pkg_pwr="pwr"$sver"-"$ver"."$arch".rpm"
pkg_pwrdemo="pwrdemo"$sver"-"$ver"."$arch".rpm"
pkg_pwrrt="pwrrt-"$ver"."$arch".rpm"
pkg_pwrsev="pwrsev-"$ver"."$arch".rpm"
pkg_pwrrpi="pwrrpi"$sver"-"$ver"."$arch".rpm"
img_pwrbuild="pwrbuild_"$release_name":v1"
img_pwrdev="pwrdev_"$release_name":v1"
img_pwrrt="pwrrt_"$release_name":v1"
caps="--security-opt seccomp=unconfined \
      --cap-add NET_ADMIN \
      --cap-add NET_BROADCAST \
      --cap-add NET_RAW \
      --cap-add SYS_BOOT \
      --cap-add NET_BIND_SERVICE \
      --cap-add SYS_NICE"

if [ "$1" == "" ]; then
  start=1
else
  start=$1
fi
if [ "$2" == "" ]; then
  end=9
else
  end=$2
fi

if [ ! -e log ]; then
  mkdir log
fi

if [ ! -e pkg ]; then
  mkdir pkg
fi

if [ ! -e data ]; then
  mkdir data
fi

# Pwrb container
if [ $start -le 1 ] && [ $end -ge 1 ]; then
  docker image build -t $img_pwrbuild -f build/Dockerfile.buildimg \
    --build-arg RELEASE=$release \
    --build-arg INSTALL_UPDATE="$install_update" \
    --build-arg INSTALL_GIT="$install_git" \
    --build-arg INSTALL_VIDEODUMMY="$install_videodummy" \
    --build-arg INSTALL_BUILD="$install_build" \
    --build-arg INSTALL_RPI="$install_rpi" \
    --build-arg BUILD_RPI=$build_rpi \
    --build-arg TZ=$tz \
    ./
fi

# Build pwr container
if [ $start -le 2 ] && [ $end -ge 2 ]; then
  docker image build -t pwr:v1 -f build/Dockerfile.build \
    --build-arg RELEASE=$img_pwrbuild \
    --build-arg BUILDVERSION="$buildversion" \
    --build-arg BUILD_RPI=$build_rpi \
    --build-arg GITREPO="$gitrepo" \
    --build-arg JDK_DIR=$jdk_dir \
    --build-arg TZ=$tz \
    ./
  docker container create --name tmp pwr:v1
  docker container cp tmp:/pwr/build.log ./log/
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/exp/lis/build.tlog ./log/
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/pkg/$pkg_pwr ./pkg/
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/pkg/$pkg_pwrdemo ./pkg/
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/pkg/$pkg_pwrrt ./pkg/
#  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/pkg/$pkg_pwrsev ./pkg/
#  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/pkg/$pkg_pwrrpi ./pkg/
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/project/pwrtest01/bld/common/load/pwrp_pkg_pwrtest01a_0001.tgz ./data/
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/project/pwrtest01/bld/common/load/pwrp_pkg_pwrtest01b_0001.tgz ./data/
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/project/pwrtest01/bld/common/load/pwrp_pkg_pwrtest01c_0001.tgz ./data/
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/project/pwrtest01/bld/common/load/pwrp_pkg_pwrtest01d_0001.tgz ./data/
  docker container cp tmp:/pwr/rls/os_linux/hw_x86_64/bld/project/pwrtest02/bld/common/load/pwrtest02.tar.gz ./data/
  docker container rm tmp
  docker image rm pwr:v1
fi

# Pwrdev container
if [ $start -le 3 ] && [ $end -ge 3 ]; then
  docker image build -t $img_pwrdev -f pwrdev/Dockerfile.pwrdev \
    --build-arg RELEASE=$release \
    --build-arg INSTALL_UPDATE="$install_update" \
    --build-arg INSTALL_PWR="$install_pwr" \
    --build-arg INSTALL_VIDEODUMMY="$install_videodummy" \
    --build-arg INSTALL_PKG="$install_pkg" \
    --build-arg PKG_PWR=$pkg_pwr \
    --build-arg TZ=$tz \
    ./
fi

# Pwrrt container
if [ $start -le 4 ] && [ $end -ge 4 ]; then
  docker image build -t $img_pwrrt -f pwrrt/Dockerfile.pwrrt \
    --build-arg RELEASE=$release \
    --build-arg INSTALL_UPDATE="$install_update" \
    --build-arg INSTALL_PWRRT="$install_pwrrt" \
    --build-arg INSTALL_VIDEODUMMY="$install_videodummy" \
    --build-arg INSTALL_PKG="$install_pkg" \
    --build-arg PKG_PWRRT=$pkg_pwrrt \
    --build-arg TZ=$tz \
    ./
fi


# Demo container
if [ $start -le 5 ] && [ $end -ge 5 ]; then
  docker image build -t pwrdemo:v1 -f pwrdemo/Dockerfile.pwrdemo \
    --build-arg RELEASE=$img_pwrdev \
    --build-arg INSTALL_PKG="$install_pkg" \
    --build-arg PKG_PWRDEMO=$pkg_pwrdemo \
    ./
  docker run $caps --name pwrdm pwrdemo:v1
  docker container cp pwrdm:/usr/pwrp/pwrdemo$sver/bld/common/tmp/pwrdemo_status.tlog ./log/
  docker container rm pwrdm
  docker image rm pwrdemo:v1
fi

# Runtime container pwrtest01c
if [ $start -le 6 ] && [ $end -ge 6 ]; then
  docker image build -t pwrtest01c:v1 -f pwrtest01/Dockerfile.pwrtest01c \
    --build-arg RELEASE=$img_pwrrt \
    ./
  docker run $caps --name pwrtc pwrtest01c:v1
  docker container cp pwrtc:/pwrp/common/log/plc.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/aproc.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/cdh.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/errh.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/gdh.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/mh.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/mhappl.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/plc.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/qcom.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/qmon.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/qmonc.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/time.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/nettime.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/ccm.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/xttscript.tlog ./log/
  docker container cp pwrtc:/pwrp/common/log/pwrrt.tlog ./log/

  docker container rm pwrtc
  docker image rm pwrtest01c:v1
fi

# Pwrtest02 container
if [ $start -le 7 ] && [ $end -ge 7 ]; then
  docker image build -t pwrtest02:v1 -f pwrtest02/Dockerfile.pwrtest02 \
    --build-arg RELEASE=$img_pwrdev \
    ./
  docker run --name pwrt2 pwrtest02:v1
  docker container cp pwrt2:/usr/pwrp/pwrtest02/bld/common/log/pwrtest02_classvolume.tlog ./log/
  docker container cp pwrt2:/usr/pwrp/pwrtest02/bld/common/log/pwrtest02_rootvolume.tlog ./log/
  docker container cp pwrt2:/usr/pwrp/pwrtest02/bld/common/log/ldh.tlog ./log/
  docker container cp pwrt2:/usr/pwrp/pwrtest02/bld/common/log/pwrwb.tlog ./log/
  docker container rm pwrt2
  docker image rm pwrtest02:v1
fi

# Runtime container pwrtest01a and pwrtest01b
if [ $start -le 8 ] && [ $end -ge 8 ]; then
  docker image build -t pwrtest01b:v1 -f pwrtest01/Dockerfile.pwrtest01b \
    --build-arg RELEASE=$img_pwrrt \
    ./

  docker image build -t pwrtest01a:v1 -f pwrtest01/Dockerfile.pwrtest01a \
    --build-arg RELEASE=$img_pwrrt \
    ./

  if ! docker network inspect pwrnet 1>/dev/null 2>/dev/null; then
    docker network create --subnet=172.18.0.0/16 pwrnet
  fi

  docker run $caps --name pwrtb --net pwrnet --ip 172.18.0.23 pwrtest01b:v1 &
  pidb=$!
  sleep 10
  docker run $caps --name pwrta --net pwrnet --ip 172.18.0.22 pwrtest01a:v1
  wait $pidb
  docker container cp pwrta:/pwrp/common/log/neth.tlog ./log/
  docker container cp pwrta:/pwrp/common/log/qcom2a.tlog ./log/
  docker container cp pwrtb:/pwrp/common/log/qcom2b.tlog ./log/
  docker container cp pwrtb:/pwrp/common/log/qcom2b.tlog ./log/
  docker container cp pwrtb:/pwrp/common/log/modbustcp.tlog ./log/

  docker container rm pwrta pwrtb
  docker image rm pwrtest01a:v1 pwrtest01b:v1
fi

# Runtime container pwrtest01d
if [ $start -le 9 ] && [ $end -ge 9 ]; then
  docker image build -t pwrtest01d:v1 -f pwrtest01/Dockerfile.pwrtest01d \
    --build-arg RELEASE=$img_pwrrt \
    --build-arg INSTALL_SEV="$install_sev" \
    ./
  docker run $caps --name pwrtd pwrtest01d:v1
  docker container cp pwrtd:/pwrp/common/log/sev.tlog ./log/

  docker container rm pwrtd
#  docker image rm pwrtest01d:v1
fi
