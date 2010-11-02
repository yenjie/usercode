./ana2.sh 12 $1 $2 "Data-$3" 1 $4 10000000000 $5 $6 $7
mv correction.root plot/root/result-12-$3-$4.root
./ana2.sh 23 $1 $2 "Data-$3" 1 $4 10000000000 $5 $6 $7
mv correction.root plot/root/result-23-$3-$4.root
./ana2.sh 13 $1 $2 "Data-$3" 1 $4 10000000000 $5 $6 $7
mv correction.root plot/root/result-13-$3-$4.root

./merge.sh Data-$3
root -l -q -b "makeMergedPlot.C+(\"$3-$4\",0,3.8,($5+$6)/2)"
