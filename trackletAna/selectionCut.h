#include <TCut.h>
#include <TString.h>
#include <iostream.h>

class selectionCut
{  
    public:
    selectionCut(bool isMC);
    ~selectionCut(){}
    
    TCut Cut;  
    TString evtSelection;
    TString vtxCut;
    TString myCut;
    int nEtaBin;
    int nHitBin;
    int nVzBin;
    int VzRangeL;
    int VzRangeH;
};

selectionCut::selectionCut(bool isMC)
{
   VzRangeL = -20;   
   VzRangeH = 20;
   vtxCut = Form("vz[1]<%d&&vz[1]>%d",VzRangeH,VzRangeL);
   evtSelection      = ("l1TBit[40]==1&&l1TBit[36]!=1&&l1TBit[37]!=1&&l1TBit[38]!=1&&l1TBit[39]!=1");

   if (!isMC) evtSelection += "&&nLumi>68&&nLumi<145&&l1ABit[0]==1&&l1ABit[82]==1";

   myCut = vtxCut + "&&" + evtSelection;

   cout <<myCut<<endl;
   Cut = TCut(myCut);

   nEtaBin  = 12;
   nHitBin  = 14;
   nVzBin   = 20;
}
