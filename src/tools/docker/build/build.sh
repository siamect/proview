#!/bin/bash
BUILDVERSION=$1" "$2
JDK_DIR=$3
Xorg -noreset +extension GLX +extension RANDR +extension RENDER -config pwr/src/tools/dummy_display/xorg.conf :99 &
export DISPLAY=:99
export jdk=$JDK_DIR
touch /etc/proview.cnf
cd pwr
./build.sh --version $BUILDVERSION
