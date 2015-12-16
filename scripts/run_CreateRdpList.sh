#!/bin/bash
#export TN228HOME=/data/t2k/phrdqd/TN228
#export T2KDATAHOME=/data/t2k/data

export TN228HOME=/storage/epp2/phrdqd/TN228
export T2KDATAHOME=/storage/epp2/t2k/data/nd280

# Generate file listings for the various different runs and generate input lists from these
# Be careful to avoid any cosmics files that reside in the same directory (i.e. ensure _spl_
# is part of the filename).
for production in production006/B
do
   for run in 00004000_00004999 00005000_00005999 00006000_00006999 00007000_00007999 00008000_00008999 00009000_00009999
   do
      echo "starting ${run}"
      ls -1 -d $T2KDATAHOME/${production}/rdp/ND280/${run}/anal/*_spl_*.root > $TN228HOME/input_files/run${run}.list
      echo "finished ${run}"
   done
done

for production in production006/C
do
   for run in 00010000_00010999
   do
      echo "starting ${run}"
      ls -1 -d $T2KDATAHOME/${production}/rdp/ND280/${run}/anal/*_spl_*.root > $TN228HOME/input_files/run${run}.list
      echo "finished ${run}"
   done
   for run in 00011000_00011999
   do
      echo "starting ${run}"
      ls -1 -d $T2KDATAHOME/${production}/rdp/ND280/${run}/*_spl_*.root > $TN228HOME/input_files/run${run}.list
      echo "finished ${run}"
   done
done

cat $TN228HOME/input_files/run00004000_00004999.list $TN228HOME/input_files/run00005000_00005999.list $TN228HOME/input_files/run00006000_00006999.list $TN228HOME/input_files/run00007000_00007999.list $TN228HOME/input_files/run00008000_00008999.list $TN228HOME/input_files/run00009000_00009999.list $TN228HOME/input_files/run00010000_00010999.list $TN228HOME/input_files/run00011000_00011999.list > $TN228HOME/input_files/local.list
rm $TN228HOME/input_files/run00004000_00004999.list
rm $TN228HOME/input_files/run00005000_00005999.list
rm $TN228HOME/input_files/run00006000_00006999.list
rm $TN228HOME/input_files/run00007000_00007999.list
rm $TN228HOME/input_files/run00008000_00008999.list
rm $TN228HOME/input_files/run00009000_00009999.list
rm $TN228HOME/input_files/run00010000_00010999.list
rm $TN228HOME/input_files/run00011000_00011999.list
