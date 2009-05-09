a=$PWD
export SCRAM_ARCH=slc4_ia32_gcc345
source /app/cms-soft/cmsset_default.sh
cd /net/hisrv0001/home/yenjie/work/cms/CMSSW_2_2_9/src
eval `scramv1 runtime -sh`

cd $a

tar xvfz gammajet.tgz

b=`echo $1'+1'|bc`
a=`head -$b list10|tail -1`
 echo ./run.sh $1'1' $a|replace "'" "\'"|bash
 echo ./run.sh $1'2' $a|replace "'" "\'"|bash
 echo ./run.sh $1'3' $a|replace "'" "\'"|bash
 echo ./run.sh $1'4' $a|replace "'" "\'"|bash
 echo ./run.sh $1'5' $a|replace "'" "\'"|bash
 echo ./run.sh $1'6' $a|replace "'" "\'"|bash
 echo ./run.sh $1'7' $a|replace "'" "\'"|bash
 echo ./run.sh $1'8' $a|replace "'" "\'"|bash
 echo ./run.sh $1'9' $a|replace "'" "\'"|bash
 echo ./run.sh $b'0' $a|replace "'" "\'"|bash
