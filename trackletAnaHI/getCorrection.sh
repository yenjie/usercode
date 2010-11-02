#echo all energies

# first and second layers
./ana.sh 12 $1 $2 "900GeV MC 12" 0 
mv correction.root correction/correction-12-$3.root

# second and third layers
./ana.sh 23 $1 $2 "900GeV MC 23" 0 
mv correction.root correction/correction-23-$3.root

# first and third layers
./ana.sh 13 $1 $2 "900GeV MC 13" 0
mv correction.root correction/correction-13-$3.root


# Get Trigger Correction
#./trig.sh $1 $2
