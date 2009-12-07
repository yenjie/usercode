#include <TCut.h>
#include <TString.h>
#include <iostream.h>

class selectionCut
{  
    public:
    selectionCut();
    ~selectionCut(){}
    
    TCut Cut;  
    TString myCut;
    int nEtaBin;
    int nHitBin;
    int nVzBin;
    int VzRange;
};

selectionCut::selectionCut()
{
   // hltBit[66]==1   MinBiasBSC
   // hltBit[67]==1   MinBiasBSC_OR
   // hltBit[68]==1   MinBiasHCAL
   // hltBit[69]==1   MinBiasECAL
   // hltBit[71]==1   SingleTrack
   // hltBit[72]==1   DoubleTrack
   
   VzRange = 20;   
  // myCut      = Form("abs(vz[1])<%d&&hltBit[67]==1&&vz[1]<10",VzRange);
   myCut      = Form("abs(vz[1])<%d&&l1TBit[41]==1&&l1TBit[36]!=1&&l1TBit[37]!=1&&l1TBit[38]!=1&&l1TBit[39]!=1",VzRange);
//   &&(hltBit[66]*0.872+hltBit[67]*0.392+hltBit[71]*0.295+hltBit[72]*0.146-1.09)>0.4"; 

   myCut += "&&nLumi>130&&nLumi<143&&l1TBit[0]==1";

   if (1==0) {
   myCut +="&&";
   myCut +="(nEv==1148472||";
   myCut +=" nEv==1150146||";
   myCut +=" nEv==1170364||";
   myCut +=" nEv==1259548||";
   myCut +=" nEv==1268959||";
   myCut +=" nEv==1433829||";
   myCut +=" nEv==1470615||";
   myCut +=" nEv==1621686||";
   myCut +=" nEv==1711697||";
   myCut +=" nEv==1791588||";
   myCut +=" nEv==1903639||";
   myCut +=" nEv==1976180||";
   myCut +=" nEv==2092877||";
   myCut +=" nEv==2142994||";
   myCut +=" nEv==2324651||";
   myCut +=" nEv==2439135||";
   myCut +=" nEv==2449816||";
   myCut +=" nEv==2455005||";
   myCut +=" nEv==2486226||";
   myCut +=" nEv==2529237||";
   myCut +=" nEv==2532227||";
   myCut +=" nEv==2915851||";
   myCut +=" nEv==2924162||";
   myCut +=" nEv==3039260||";
   myCut +=" nEv==3066043||";
   myCut +=" nEv==3122376||";
   myCut +=" nEv==3142973||";
   myCut +=" nEv==3177667)";
   }

   cout <<myCut<<endl;
   Cut = TCut(myCut);

   nEtaBin  = 12;
   nHitBin  = 14;
   nVzBin   = 20;
}
