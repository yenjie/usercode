#include <TCut.h>
#include <TString.h>
#include <iostream>

class selectionCut
{  
    public:
    selectionCut(bool isMC, int centrality, int nLumiL=69, int nLumiH=145);
    ~selectionCut(){}
    
    TCut Cut;  
    TString evtSelection;
    TCut CutWOVtxCut;
    TString vtxCut;
    TString myCut;
    int nEtaBin;
    int nHitBin;
    int nVzBin;
    int VzRangeL;
    int VzRangeH;
};

selectionCut::selectionCut(bool isMC, int centrality, int nLumiL, int nLumiH)
{
   VzRangeL = -20;   
   VzRangeH = 20;
   vtxCut = Form("vz[1]<%d&&vz[1]>%d&&abs(vz[1])<10",VzRangeH,VzRangeL);
//   evtSelection      = ("l1TBit[40]>0&&l1TBit[36]!=1&&l1TBit[37]!=1&&l1TBit[38]!=1&&l1TBit[39]!=1");
//  evtSelection      = ("(nHFp>=1&&nHFn>=1)&&l1TBit[34]!=0&&l1TBit[36]!=1&&l1TBit[37]!=1&&l1TBit[38]!=1&&l1TBit[39]!=1");
//  evtSelection      = ("(nHFp>=1&&nHFn>=1)&&l1ABit[124]!=0&&l1TBit[36]!=1&&l1TBit[37]!=1&&l1TBit[38]!=1&&l1TBit[39]!=1"); // default
  
   if (centrality==0) evtSelection      ="1";
   if (centrality==1) evtSelection = "hf>88496.5";
   if (centrality==2) evtSelection = "hf<88496.5&&hf>60126.7";
   if (centrality==3) evtSelection = "hf<60126.7&&hf>38575.3";
   if (centrality==4) evtSelection = "hf<38575.3&&hf>23921.4";
   if (centrality==5) evtSelection = "hf<23921.4&&hf>13801.3";
   if (centrality==6) evtSelection = "hf<13801.3&&hf>7533.57";


// ("l1ABit[124]!=0&&l1TBit[36]!=1&&l1TBit[37]!=1&&l1TBit[38]!=1&&l1TBit[39]!=1"); // default
//  evtSelection      = ("(nHFp>=1&&nHFn>=1)&&l1TBit[40]!=0&&l1TBit[36]!=1&&l1TBit[37]!=1&&l1TBit[38]!=1&&l1TBit[39]!=1");
//   evtSelection      = ("(nHFp>=1&&nHFn==0)&&l1TBit[34]!=0&&l1TBit[36]!=1&&l1TBit[37]!=1&&l1TBit[38]!=1&&l1TBit[39]!=1");
//   evtSelection      = ("(nHFn>=1&&nHFp==0)&&l1TBit[34]!=0&&l1TBit[36]!=1&&l1TBit[37]!=1&&l1TBit[38]!=1&&l1TBit[39]!=1");

//   if (!isMC) evtSelection += Form("&&nLumi>=%d&&nLumi<=%d&&l1ABit[0]==1&&l1ABit[82]==1",nLumiL,nLumiH);

   if (!isMC) {
//      evtSelection += Form("&&mult2>=%d&&mult2<=%d&&l1ABit[0]==1&&l1ABit[82]==1",nLumiL,nLumiH); 
//      cout <<"Selection on mult2 mode!"<<endl;
   }
     
   CutWOVtxCut = TCut(evtSelection);
   myCut = vtxCut + "&&" + evtSelection;

   cout <<myCut<<endl;
   
   Cut = TCut(myCut);

   nEtaBin  = 12;
   nHitBin  = 14;
   nVzBin   = 20;
}
