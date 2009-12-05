#!/bin/bash

f1="plot/result/result-$1-12.C"
f2="plot/result/result-$1-13.C"
f3="plot/result/result-$1-23.C"

a=`wc $f1|awk '{print $1}'`
echo $a
b=`echo $a -1|bc`
echo $b
head -$b $f1|replace 'Reconstructed' 'Reconstructed (1st+2nd layers)'|replace '(0.3,0.18,1,0.3' '(0.3,0.18,1,0.35' > merged/merged-$1.C
grep hCorrected_x $f2|replace hCorrected hCorrected2 |replace 'Color(2)' 'Color(1)'|replace 'Style(20)' 'Style(26)'|replace 'Reconstructed' 'Reconstructed (1st+3rd layers)'>>merged/merged-$1.C
grep hCorrected_x $f3|replace hCorrected hCorrected3 |replace 'Color(2)' 'Color(4)'|replace 'Style(20)' 'Style(25)'|replace 'Reconstructed' 'Reconstructed (2nd+3rd layers)' >>merged/merged-$1.C


echo "cDNdEta->SaveAs(\"result-$1-merged.eps\");">>merged/merged-$1.C
echo '}' >>merged/merged-$1.C
