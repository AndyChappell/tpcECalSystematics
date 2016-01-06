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

if isTesting; then
   runs=(1_rdp 1_mcp 1_a_rdp 1_a_mcp)
   indices=(0 1 2 3)
else
   runs=(2_01_mcp 2_02_mcp 2_03_mcp 2_04_mcp 2_05_mcp 2_06_mcp 2_07_mcp \
      2_08_mcp 2_09_mcp 2_10_mcp 2_11_mcp 2_12_mcp 3b_01_mcp 3b_02_mcp \
      3b_03_mcp 3c_01_mcp 3c_02_mcp 3c_03_mcp 3c_04_mcp 3c_05_mcp 3c_06_mcp \
      3c_07_mcp 3c_08_mcp 3c_09_mcp 3c_10_mcp 3c_11_mcp 3c_12_mcp 3c_13_mcp \
      3c_14_mcp 3c_15_mcp 3c_16_mcp 3c_17_mcp 3c_18_mcp 4_01_mcp 4_02_mcp \
      4_03_mcp 4_04_mcp 4_05_mcp 4_06_mcp 4_07_mcp 4_08_mcp 4_09_mcp 4_10_mcp \
      4_11_mcp 4_12_mcp 4_13_mcp 4_14_mcp 4_15_mcp 4_16_mcp 4_17_mcp 4_18_mcp \
      5c_01_mcp 5c_02_mcp 5c_03_mcp 5c_04_mcp 5c_05_mcp 5c_06_mcp)
   indices=({0..56})
fi

pids=""

count=1
for i in ${indices[@]}
do
   # Delete the old flattree
   rm $TN228HOME/flattrees/${TESTDIR}flattree_${runs[$i]}.root
   # Create a new flattree
   RunCreateFlatTree.exe -o $TN228HOME/flattrees/${TESTDIR}flattree_${runs[$i]}.root $TN228HOME/input_files/${TESTDIR}input_${runs[$i]}.list > $TN228HOME/logs/${TESTDIR}CreateFlattree_${runs[$i]}.log &
   pids="$pids $!"
   if ((($i + 1) % 3)); then
      echo ""
   else
      # Don't attempt to run more than 3 of these processes in parallel on a
      # standalone CSC machine, you'll probably run out of memory
      wait $pids
   fi
done

wait $pids

echo "Complete"
