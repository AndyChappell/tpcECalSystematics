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
      echo -n "Testing mode is not set. This will delete your flattrees. Are you sure you want to proceed (Y/N)? "
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

source $ND280PATH/highland2Systematics/tpcECalSystematics/v*/cmt/setup.sh
cd $TN228HOME

runs=(1_rdp 1_mcp 1_a_rdp 1_a_mcp)
for i in 0 1 2 3
do
   # Delete the old flattree
   rm $TN228HOME/flattrees/${TESTDIR}flattree_${runs[$i]}.root
   # Create a new flattree
   RunCreateFlatTree.exe -o $TN228HOME/flattrees/${TESTDIR}flattree_${runs[$i]}.root $TN228HOME/input_files/${TESTDIR}input_${runs[$i]}.list > $TN228HOME/logs/${TESTDIR}CreateFlattree_${runs[$i]}.log &
   proc[$i]=$!
done
wait "${proc[0]}" "${proc[1]}" "${proc[2]}" "${proc[3]}"
echo "Complete"
