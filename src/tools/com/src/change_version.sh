#!/bin/bash

#set -o xtrace

if [ "$1" == "" ]; then

  echo "  Replace version"
  echo ""
  echo "  Arg 1: New version, eg V6.0.2-3"
  echo "  Arg 2: Description, eg \"Nethandler bugfix\""
  echo ""
  echo "  > change_version.sh V6.0.2-3 \"Nethandler bugfix\""
  echo ""
  echo " or"
  echo ""
  echo "  Add version description to control-files"
  echo ""
  echo "  Arg 1: -d"
  echo "  Arg 2: Description, eg \"Eventmonitor bugfix\""
  echo ""
  echo "  > change_version.sh -d \"Eventmonitor bugfix\""
 
  exit
fi

#
# html-files
#
if [ $1 != "-d" ]; then

  files="\
$pwre_croot/src/doc/web/en_us/qguide_h.html \
$pwre_croot/src/doc/web/en_us/pwr_about_h.html \
$pwre_croot/src/doc/web/en_us/orm_h.html \
$pwre_croot/src/doc/web/en_us/prm_h.html \
$pwre_croot/src/doc/web/en_us/doc_h.html \
$pwre_croot/src/doc/web/sv_se/pwr_about_h.html \
$pwre_croot/src/doc/web/sv_se/orm_h.html \
$pwre_croot/src/doc/web/sv_se/doc_h.html"

  from="VX.Y.Z"
  to=${1%%-*}

  for file in $files; do
    echo "-- Processing $file ($to)"
    awk -v version=$to -f version_html.awk  $file > repl.tmp
    mv repl.tmp $file
  done
fi

#
# control-files
#

files="\
$pwre_croot/src/tools/pkg/deb/pwr/control \
$pwre_croot/src/tools/pkg/deb/pwrdemo/control \
$pwre_croot/src/tools/pkg/deb/pwrrt/control \
$pwre_croot/src/tools/pkg/deb/pwrrpi/control \
$pwre_croot/src/tools/pkg/deb/pwrsev/control \
$pwre_croot/src/tools/pkg/ubu/pwr/control \
$pwre_croot/src/tools/pkg/ubu/pwrdemo/control \
$pwre_croot/src/tools/pkg/ubu/pwrrt/control \
$pwre_croot/src/tools/pkg/ubu/pwrrpi/control \
$pwre_croot/src/tools/pkg/ubu/pwrsev/control \
$pwre_croot/src/tools/pkg/raspbian/pwr/control \
$pwre_croot/src/tools/pkg/raspbian/pwrdemo/control \
$pwre_croot/src/tools/pkg/raspbian/pwrrt/control \
$pwre_croot/src/tools/pkg/raspbian/pwrsev/control \
$pwre_croot/src/tools/pkg/deb_x86_64/pwr/control \
$pwre_croot/src/tools/pkg/deb_x86_64/pwrdemo/control \
$pwre_croot/src/tools/pkg/deb_x86_64/pwrrt/control \
$pwre_croot/src/tools/pkg/deb_x86_64/pwrsev/control \
$pwre_croot/src/tools/pkg/deb_armel/pwrrt/control \
$pwre_croot/src/tools/pkg/ubu_x86_64/pwr/control \
$pwre_croot/src/tools/pkg/ubu_x86_64/pwrdemo/control \
$pwre_croot/src/tools/pkg/ubu_x86_64/pwrrt/control \
$pwre_croot/src/tools/pkg/ubu_x86_64/pwrrpi/control \
$pwre_croot/src/tools/pkg/ubu_x86_64/pwrsev/control"

if [ $1 == "-d" ]; then
  for file in $files; do
    echo "-- Processing $file ($to)"
    awk -v "description=$2" -f version_control_descr.awk  $file > repl.tmp
    mv repl.tmp $file
  done
else
  to=${1:1}

  for file in $files; do
    echo "-- Processing $file ($to)"
    awk -v version=$to -v "description=$2" -f version_control.awk  $file > repl.tmp
    mv repl.tmp $file
  done
fi
