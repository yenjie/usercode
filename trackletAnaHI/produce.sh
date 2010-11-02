#!/bin/bash



infile=$1
outfile=$2
startEntry=$3
endEntry=$4
addL1Bck=$5
addL2Bck=$5
addL3Bck=$5
reWeight=$6
useRandomVertex=$7
cutOnClusterSize=$8
mimicPixelCounting=$9
reproduceBck=${10}



if [ -z $startEntry  ]; then
startEntry=0
fi

if [ -z $endEntry  ]; then
endEntry=1000000000
fi

if [ -z $addL1Bck  ]; then
addL1Bck=0
fi

if [ -z $addL2Bck  ]; then
addL2Bck=0
fi

if [ -z $addL3Bck  ]; then
addL3Bck=0
fi

if [ -z $reWeight  ]; then
reWeight=0
fi

if [ -z $useRandomVertex  ]; then
useRandomVertex=0
fi

if [ -z $cutOnClusterSize  ]; then
cutOnClusterSize=0
fi

if [ -z $mimicPixelCounting  ]; then
mimicPixelCounting=0
fi

if [ -z $reproduceBck  ]; then
reproduceBck=0
fi

if [ -z $infile  ]; then
echo produce.sh [infile][outfile][startEntry][endEntry][addBck][reweight][useRandomVertex][cutClusterSize][mimicPixelCounting][bck]
echo Ex:
echo ./produce.sh inf.root outf.root 0 1000000 0 0 0 1 0 0  # normal operation
echo ./produce.sh inf.root outf.root 0 10000 0 0 0 1 0 1  # reproduce bck


else
root -l -q "analyze_trackletTree.C+(\"$infile\",\"$outfile\",$startEntry,$endEntry,$addL1Bck,$addL2Bck,$addL3Bck,$reWeight,$useRandomVertex,$cutOnClusterSize,$mimicPixelCounting,$reproduceBck)"
fi
