#echo all energies

# first and second layers
./ana.sh 12 $1  "900GeV MC 12" 0
mv correction.root correction-12-900GeV.root

# second and third layers
./ana.sh 23 $1  "900GeV MC 23" 0
mv correction.root correction-23-900GeV.root

# first and third layers
./ana.sh 13 $1  "900GeV MC 13" 0
mv correction.root correction-13-900GeV.root


# Get Trigger Correction
./trig.sh $1 "900GeV"
