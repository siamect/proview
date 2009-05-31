#!/bin/bash
#
# File:
#	pwr_tools:pwr_show_version.sh
#
# Description:
#

infile=${1}
outfile=${2}

current_date="`date`"
cat > $outfile <<-EOF
	Version information
	
	File.......: $infile
	BuildLabel.: $pwre_blabel
	BuildTime..: $current_date
	BuildId....: $USER
EOF
