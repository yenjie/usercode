#!/bin/sh

for run in `seq $1 $2`
do

outfile=phojet_sd_$run.root
random=$run
numEvents=50000

cmsRun sd.py output=$outfile randomNumber=$random maxEvents=$numEvents

done








