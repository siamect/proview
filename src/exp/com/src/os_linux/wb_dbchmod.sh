#!/bin/bash
#
# Change permission of databasefiles so it
# corresponds with umask
# (BerkeleyDb doesn't seems to do this...
#
u=`eval umask`
b1=$(( u%8 ))
b2=$(( (u-b1)%64/8 ))
b3=$(( (u-b1-b2)%256/64 ))
bd1=$(( 7-b1 ))
bd2=$(( 7-b2 ))
bd3=$(( 7-b3 ))
b1=$(( 6-b1 ))
b2=$(( 6-b2 ))
b3=$(( 6-b3 ))
chmod $bd3$bd2$bd1 $1
chmod $b3$b2$b1 $1/*
