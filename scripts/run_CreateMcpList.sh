#!/bin/bash
export TN228HOME=/storage/epp2/phrdqd/TN228
export T2KDATAHOME=/storage/epp2/t2k/data/nd280

# Generate file listings for the various different MC runs and generate input lists from these.
# Run 1 - 330 files for 10xPOT
echo "starting run 1"
ls -1 -d $T2KDATAHOME/production006/B/mcp/neut/2010-02-water/magnet/run1/anal/*.root > $TN228HOME/input_files/run1_mcp.list

num=3
inc=$((330/$num))
for i in $(seq 0 $(($num - 1)))
do
   from[$i]=$((1 + $i * $inc))
   to[$i]=$((($i + 1) * $inc))
done
to[$(($num - 1))]=330
for i in $(seq 0 $(($num - 1)))
do
   if ((($i + 1) < 10)); then
      id="0$(($i + 1))"
   else
      id="$(($i + 1))"
   fi
   sed -n "${from[$i]},${to[$i]} p" $TN228HOME/input_files/run1_mcp.list > $TN228HOME/input_files/input_1_${id}_mcp.list
done

rm $TN228HOME/input_files/run1_mcp.list
echo "finished run 1"

# Run 2 - 865 files for 10xPOT on water, 710 files for 10xPOT on water
echo "starting run 2"
ls -1 -d $T2KDATAHOME/production006/B/mcp/neut/2010-11-water/magnet/run2/anal/*.root > $TN228HOME/input_files/run2mcpw.list
sed -n '1,865 p' $TN228HOME/input_files/run2mcpw.list > $TN228HOME/input_files/run2w_mcp.list
ls -1 -d $T2KDATAHOME/production006/B/mcp/neut/2010-11-air/magnet/run2/anal/*.root > $TN228HOME/input_files/run2mcpa.list
sed -n '1,710 p' $TN228HOME/input_files/run2mcpa.list > $TN228HOME/input_files/run2a_mcp.list
cat $TN228HOME/input_files/run2w_mcp.list $TN228HOME/input_files/run2a_mcp.list > $TN228HOME/input_files/run2_mcp.list

num=12
inc=$((1575/$num))
for i in $(seq 0 $(($num - 1)))
do
   from[$i]=$((1 + $i * $inc))
   to[$i]=$((($i + 1) * $inc))
done
to[$(($num - 1))]=1575
for i in $(seq 0 $(($num - 1)))
do
   if ((($i + 1) < 10)); then
      id="0$(($i + 1))"
   else
      id="$(($i + 1))"
   fi
   sed -n "${from[$i]},${to[$i]} p" $TN228HOME/input_files/run2_mcp.list > $TN228HOME/input_files/input_2_${id}_mcp.list
done

rm $TN228HOME/input_files/run2mcpw.list
rm $TN228HOME/input_files/run2mcpa.list
rm $TN228HOME/input_files/run2w_mcp.list
rm $TN228HOME/input_files/run2a_mcp.list
rm $TN228HOME/input_files/run2_mcp.list

echo "finished run 2"

# Run 3
echo "starting run 3"
ls -1 -d $T2KDATAHOME/production006/B/mcp/neut/2010-11-air/magnet/run3/anal/*.root > $TN228HOME/input_files/run3_mcp.list
# Run 3b - 431 files for 10xPOT

num=3
inc=$((431/$num))
for i in $(seq 0 $(($num - 1)))
do
   from[$i]=$((1 + $i * $inc))
   to[$i]=$((($i + 1) * $inc))
done
to[$(($num - 1))]=431
for i in $(seq 0 $(($num - 1)))
do
   if ((($i + 1) < 10)); then
      id="0$(($i + 1))"
   else
      id="$(($i + 1))"
   fi
   sed -n "${from[$i]},${to[$i]} p" $TN228HOME/input_files/run3_mcp.list > $TN228HOME/input_files/input_3b_${id}_mcp.list
done

# Run 3c - 2709 files for 10xPOT

num=18
inc=$((2709/$num))
for i in $(seq 0 $(($num - 1)))
do
   from[$i]=$((432 + $i * $inc))
   to[$i]=$((431 + ($i + 1) * $inc))
done
to[$(($num - 1))]=$((2709 + 431))
for i in $(seq 0 $(($num - 1)))
do
   if ((($i + 1) < 10)); then
      id="0$(($i + 1))"
   else
      id="$(($i + 1))"
   fi
   sed -n "${from[$i]},${to[$i]} p" $TN228HOME/input_files/run3_mcp.list > $TN228HOME/input_files/input_3c_${id}_mcp.list
done

rm $TN228HOME/input_files/run3_mcp.list
echo "finished run 3"

# Run 4 - 3250 files for 10xPOT on water, 3524 files for 10xPOT on water
echo "starting run 4"
ls -1 -d $T2KDATAHOME/production006/B/mcp/neut/2010-11-water/magnet/run4/anal/*.root > $TN228HOME/input_files/run4mcpw.list
sed -n '1,3250 p' $TN228HOME/input_files/run4mcpw.list > $TN228HOME/input_files/run4w_mcp.list
ls -1 -d $T2KDATAHOME/production006/B/mcp/neut/2010-11-air/magnet/run4/anal/*.root > $TN228HOME/input_files/run4mcpa.list
sed -n '1,3524 p' $TN228HOME/input_files/run4mcpa.list > $TN228HOME/input_files/run4a_mcp.list
cat $TN228HOME/input_files/run4w_mcp.list $TN228HOME/input_files/run4a_mcp.list > $TN228HOME/input_files/run4_mcp.list

num=18
inc=$((6774/$num))
for i in $(seq 0 $(($num - 1)))
do
   from[$i]=$((1 + $i * $inc))
   to[$i]=$((($i + 1) * $inc))
done
to[$(($num - 1))]=6774
for i in $(seq 0 $(($num - 1)))
do
   if ((($i + 1) < 10)); then
      id="0$(($i + 1))"
   else
      id="$(($i + 1))"
   fi
   sed -n "${from[$i]},${to[$i]} p" $TN228HOME/input_files/run4_mcp.list > $TN228HOME/input_files/input_4_${id}_mcp.list
done

rm $TN228HOME/input_files/run4mcpw.list
rm $TN228HOME/input_files/run4mcpa.list
rm $TN228HOME/input_files/run4w_mcp.list
rm $TN228HOME/input_files/run4a_mcp.list
rm $TN228HOME/input_files/run4_mcp.list
echo "finished run 4"

# Run 5
echo "starting run 5"
# Run 5a - 130 files for 10xPOT - no MC data available yet
#ls -1 -d $T2KDATAHOME/production006/B/mcp/neut/2010-11-water/magnet/run5/anal/*.root > $TN228HOME/input_files/input_run5a_mcp.list
#sed -n '1,130 p' $TN228HOME/input_files/input_run5a_mcp.list > $TN228HOME/input_files/input_run5a_mcp.list
# Run 5b - 351 files for 10xPOT - no MC data available yet
#ls -1 -d $T2KDATAHOME/production006/B/mcp/neut/2010-11-air/magnet/run5/anal/*.root > $TN228HOME/input_files/input_run5b_mcp.list
#sed -n '131,481 p' $TN228HOME/input_files/input_run5b_mcp.list > $TN228HOME/input_files/input_run5b_mcp.list
# Run 5c - 880 files for 10xPOT
ls -1 -d $T2KDATAHOME/production006/B/mcp/anti-neut/2010-11-water/magnet/run5/anal/*.root > $TN228HOME/input_files/run5c_mcp.list

num=6
inc=$((880/$num))
for i in $(seq 0 $(($num - 1)))
do
   from[$i]=$((1 + $i * $inc))
   to[$i]=$((($i + 1) * $inc))
done
to[$(($num - 1))]=880
for i in $(seq 0 $(($num - 1)))
do
   if ((($i + 1) < 10)); then
      id="0$(($i + 1))"
   else
      id="$(($i + 1))"
   fi
   sed -n "${from[$i]},${to[$i]} p" $TN228HOME/input_files/run5c_mcp.list > $TN228HOME/input_files/input_5c_${id}_mcp.list
done

rm $TN228HOME/input_files/run5c_mcp.list
echo "finished run 5"
