./ana.sh 12 $1  "Data-$2" 1 $3
mv correction.root plot/root/result-12-$3.root
./ana.sh 23 $1  "Data-$2" 1 $3
mv correction.root plot/root/result-23-$3.root
./ana.sh 13 $1  "Data-$2" 1 $3
mv correction.root plot/root/result-13-$3.root

./merge.sh Data-$2
root -l merged/merged-Data-$2.C
