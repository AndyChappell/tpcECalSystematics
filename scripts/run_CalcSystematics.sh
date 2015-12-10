#!/bin/bash
export INSTALLPATH=$HOME/ND280
export ND280VERSION=v11r31
export ND280PATH=$INSTALLPATH/$ND280VERSION
export CMTPATH=$ND280PATH
export TN228HOME=/storage/epp2/phrdqd/TN228

isTesting() {
   # For quick testing the TESTING environment variable should be set
   if [ "${TESTING}" ]; then
      retval=0
   else
      retval=1
   fi
   
   return $retval
}

# For quick testing the TESTING environment variable should be set
if isTesting; then
   export TESTDIR=testing/
else
   export TESTDIR=
fi

source $ND280PATH/highland2Systematics/tpcECalSystematics/v*/cmt/setup.sh
cd $TN228HOME

# Set environment variables required by ROOT macros
export TPCECALMCFILE=$TN228HOME/microtrees/${TESTDIR}mcneutrino.root
export TPCECALDATAFILE=$TN228HOME/microtrees/${TESTDIR}neutrino.root
export TPCECALANTIMCFILE=$TN228HOME/microtrees/${TESTDIR}mcantineutrino.root
export TPCECALANTIDATAFILE=$TN228HOME/microtrees/${TESTDIR}antineutrino.root

root -l -b
