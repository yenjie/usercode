./ana.sh 12 $1  "Data" 1
./ana.sh 23 $1  "Data" 1
./ana.sh 13 $1  "Data" 1

./merge.sh Data
root -l merged-Data.C
