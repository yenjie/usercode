./ana.sh 12 $1  "Data-$2" 1 $3
./ana.sh 23 $1  "Data-$2" 1 $3
./ana.sh 13 $1  "Data-$2" 1 $3

./merge.sh Data-$2
root -l merged/merged-Data-$2.C
