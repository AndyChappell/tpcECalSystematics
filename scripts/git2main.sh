#!/bin/bash
export VERSION=v0r0
export MAINPATH=$HOME/ND280/highland2Systematics/tpcECalSystematics/$VERSION
export GITPATH=$HOME/tpcECalSystematics

directories=(app dict macros parameters src)

for i in 0 1 2 3 4
do
   cp $GITPATH/${directories[$i]}/* $MAINPATH/${directories[$i]}
done
