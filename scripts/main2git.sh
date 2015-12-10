#!/bin/bash
export ND280VERSION=v11r31
export VERSION=v0r0
export MAINPATH=$HOME/ND280/$ND280VERSION/highland2Systematics/tpcECalSystematics/$VERSION
export GITPATH=$HOME/tpcECalSystematics

directories=(app dict macros parameters src)

for i in 0 1 2 3 4
do
   cp $MAINPATH/${directories[$i]}/* $GITPATH/${directories[$i]}
done
