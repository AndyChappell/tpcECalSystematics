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

check() {
   if isTesting; then
      retval=0
   else
      echo -n "Testing mode is not set. This will delete your microtrees. Are you sure you want to proceed (Y/N)? "
      while read -r -n 1 -s answer; do
         if [[ $answer = [YyNn] ]]; then
            [[ $answer = [Yy] ]] && retval=0
            [[ $answer = [Nn] ]] && retval=1
            break
         fi
      done
   fi

   return $retval
}

if check; then
   echo; echo "Starting..."
else
   echo; echo "Aborting"
   kill -INT $$
fi

cd $TN228HOME
source $ND280PATH/highland2Systematics/tpcECalSystematics/v*/cmt/setup.sh

runs=(neutrino antineutrino mcneutrino mcantineutrino)
indices=({0..3})
for i in ${indices[@]}
do
   rm $TN228HOME/microtrees/${TESTDIR}${runs[$i]}.root
   RunTpcECalMatchingAnalysis.exe -o $TN228HOME/microtrees/${TESTDIR}${runs[$i]}.root $TN228HOME/input_files/${TESTDIR}${runs[$i]}_flattrees.list > $TN228HOME/logs/${TESTDIR}TpcECal_${runs[$i]}.log &
   pids="$pids $!"
done
wait $pids
echo "Complete"
