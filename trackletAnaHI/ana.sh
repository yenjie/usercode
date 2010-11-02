root -b -l -q "plotFinalResult.C+($1,\"$2\",\"$3\",\"$4\",$5,\"$6\")"
cp correction.root result-$1-$2-$4-$6.root