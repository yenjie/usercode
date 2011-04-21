#include <TCut.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>

class parameter 
{
   public:
   parameter(){ shift=0;}
   TTree *tData;
   TTree *tSig;
   TTree *tBck;
   TCut selectionCut;
   TCut selectionCut2;
   TCut dataCut;
   TCut dataSidebandCut;
   int nBin;
   double binL;
   double binH;
   char *var;
   char *xTitle;
   double shift;
   
};

class selectionCriteria
{
   public:
   selectionCriteria();
   ~selectionCriteria(){}

   TCut eventCut;
   TCut centralityCut;
   TCut removeSpikeCut;
   TCut removeElectronCut;
   TCut photonCut;
   
   TCut selectionCut;
   TCut selectionCutWOElectronCut;
   TCut selectionCut2;
   TCut selectionCut3;

   TCut sigCut;
   
   // signal region 
   TCut dataCut;
   TCut dataCutShowerAna;

   // sideband region
   TCut dataSidebandCut;
   TCut dataSidebandCutShowerAna;

   private:
};

selectionCriteria::selectionCriteria()
{
   eventCut     = "( !TTBit[36] && !TTBit[37] && !TTBit[38] && !TTBit[39] && !vtxIsFake && abs(vtxZ) <= 15)";
   centralityCut = "cBin>=8&&cBin<12";
   photonCut = "r9>0&&hadronicOverEm<0.2&&t1PtCut/et/r9<0.5&&abs(eta)<1.44&&rawEnergy/energy>0.5\
                    &&!isEBGap&&!isEEGap&&!isEBEEGap";   
   removeElectronCut = "!isEle";
   TString swissCrx = "(1 - (eRight+eLeft+eTop+eBottom)/eMax)";
   TCut hiSpikeCut       = Form("(  %s < 0.90 && abs(seedTime)<4  && sigmaIetaIeta>0.002 )  || isEE",swissCrx.Data());
   
   removeSpikeCut = hiSpikeCut;
  

   selectionCut = photonCut&&centralityCut&&eventCut&&removeSpikeCut&&removeElectronCut; 
   selectionCutWOElectronCut = photonCut&&centralityCut&&eventCut&&removeSpikeCut; 
   selectionCut2 =photonCut&&eventCut&&removeSpikeCut; 
   selectionCut3 =photonCut&&eventCut; 

   sigCut = "isGenMatched && abs(genMomId) <= 22 && genCalIsoDR04 < 5.0";
   
   // signal region 
   dataCut = "sigmaIetaIeta<0.013";
   dataCutShowerAna = "(ecalRecHitSumEtConeDR04 - compEcalIso + hcalTowerSumEtConeDR04 - compHcalIso)<15 && ct4PtCut < 5";
//   dataCutShowerAna = "(ecalRecHitSumEtConeDR04 - compEcalIso + hcalTowerSumEtConeDR04 - compHcalIso)<15 && ct4PtCut < 5";

   // sideband region
   dataSidebandCut = "sigmaIetaIeta>0.013&&sigmaIetaIeta<0.014";
//   dataSidebandCut = "r9<0.7&&sigmaIetaIeta<0.012";
   dataSidebandCutShowerAna =  "(ecalRecHitSumEtConeDR04 - compEcalIso + hcalTowerSumEtConeDR04 - compHcalIso)<15 && ct4PtCut >10";
//   dataSidebandCutShowerAna =  "(ecalRecHitSumEtConeDR04 - compEcalIso + hcalTowerSumEtConeDR04 - compHcalIso)>15";
}

void mcStyle(TH1* h=0) {
   h->SetLineColor(kRed);
   h->SetFillColor(kRed-9);
   h->SetFillStyle(3004);
}

void sbStyle(TH1* h=0) {
   h->SetLineColor(kBlue);
   h->SetFillColor(kAzure-8);
   h->SetFillStyle(3001);
}

void addCentralityFriend(TTree *tSig, TTree *tData,TCut selectionCut)
{
   static int counter=0;
   TH1D *hSigCent = new TH1D("hSigCent","",40,-0.5,39.5);
   TH1D *hDataCent = new TH1D("hDataCent","",40,-0.5,39.5);

   hSigCent->SetLineColor(2);
   tSig->Project("hSigCent","cBin",selectionCut); 
   tData->Project("hDataCent","cBin",selectionCut); 
   hDataCent->Scale(1./hDataCent->GetEntries());
   hSigCent->Scale(1./hSigCent->GetEntries());
   hDataCent->Divide(hSigCent);
   TNtuple *nt = new TNtuple(Form("ntCentFriend%d",counter),"","cBinWeight");
   
   Int_t cBin;
   tSig->SetBranchAddress("cBin",&cBin);
   
   for (int i=0;i<tSig->GetEntries();i++)
   {
      tSig->GetEntry(i);
      int bin = hDataCent->FindBin(cBin);
      //cout <<cBin<<" "<<hDataCent->GetBinContent(bin)<<endl;
      nt->Fill(hDataCent->GetBinContent(bin));
   }
   counter++;   
   delete hSigCent;
   delete hDataCent;
   tSig->AddFriend(nt);
}

void addCentralityFriendEB(TTree *tSig, TTree *tData,TCut selectionCut)
{
   static int counter=0;
   TH1D *hSigCent = new TH1D("hSigCent","",80,0,800);
   TH1D *hDataCent = new TH1D("hDataCent","",80,0,800);

   hSigCent->SetLineColor(2);
   tSig->Project("hSigCent","eb",selectionCut); 
   tData->Project("hDataCent","eb",selectionCut); 
   hDataCent->Scale(1./hDataCent->GetEntries());
   hSigCent->Scale(1./hSigCent->GetEntries());
   hDataCent->Divide(hSigCent);
   TNtuple *nt = new TNtuple(Form("ntCentFriend%d",counter),"","cBinWeight");
   
   Double_t eb;
   tSig->SetBranchAddress("eb",&eb);
   
   for (int i=0;i<tSig->GetEntries();i++)
   {
      tSig->GetEntry(i);
      int bin = hDataCent->FindBin(eb);
      //cout <<cBin<<" "<<hDataCent->GetBinContent(bin)<<endl;
      nt->Fill(hDataCent->GetBinContent(bin));
   }
   counter++;   
   delete hSigCent;
   delete hDataCent;
   tSig->AddFriend(nt);
}

void addCorrectedEtFriend(TTree *t, TH2D *hEtCorrection)
{
   static int counter=0;
   TTree *nt = new TTree(Form("ntEtFriend%d",counter),"");

   Float_t etCor[100];
   Float_t eta[100];
   Float_t et[100];
   UChar_t isEE[100];
   Int_t cBin;
   Int_t nPhotons;
   Int_t nPhotons2;
   nt->Branch("nPhotons2",&nPhotons2,"nPhotons2/I");
   nt->Branch("etCorrected",etCor,"etCorrected[nPhotons2]/F");

   t->SetBranchAddress("cBin",&cBin);
   t->SetBranchAddress("nPhotons",&nPhotons);
   t->SetBranchAddress("et",et);
   t->SetBranchAddress("isEE",isEE);
   t->SetMaxTreeSize(4000000000);
   for (int i=0;i<t->GetEntries();i++)
   {
//      double et=20;
      t->GetEntry(i);
      nPhotons2=0;
      for (int j=0;j<nPhotons;j++) {
	 int bin = hEtCorrection->FindBin(cBin,et[j]);
         etCor[j]=et[j]/(1.+hEtCorrection->GetBinContent(bin));
         nPhotons2++;
//         cout <<cBin<<" "<<et[j]<<" "<<hEtCorrection->GetBinContent(bin)<<endl;
      }	
      ; 
      nt->Fill();
   }
   TCanvas *c = new TCanvas(Form("ccc%d",counter),"",600,600);
   cout <<nt->GetEntries()<<" "<<t->GetEntries();
   nt->Draw("etCorrected");
   counter++;   
   t->AddFriend(nt);
}


TNtuple *getNcoll()
{
TNtuple *nt = new TNtuple("ntNcoll","","cBin:npart:ncoll");
nt->Fill(0,393.633,1747.49);
nt->Fill(1,368.819,1566.92);
nt->Fill(2,343.073,1393.97);
nt->Fill(3,317.625,1237.02);
nt->Fill(4,292.932,1095.03);
nt->Fill(5,271.917,979.836);
nt->Fill(6,249.851,863.228);
nt->Fill(7,230.72,765.968);
nt->Fill(8,212.465,677.894);
nt->Fill(9,194.752,594.481);
nt->Fill(10,178.571,522.453);
nt->Fill(11,163.23,456.049);
nt->Fill(12,149.187,399.178);
nt->Fill(13,136.011,347.174);
nt->Fill(14,123.414,299.925);
nt->Fill(15,111.7,258.411);
nt->Fill(16,100.831,221.374);
nt->Fill(17,90.7831,188.676);
nt->Fill(18,80.9823,158.896);
nt->Fill(19,72.6236,135.117);
nt->Fill(20,64.1508,112.481);
nt->Fill(21,56.6284,93.5697);
nt->Fill(22,49.9984,77.9192);
nt->Fill(23,43.3034,63.2538);
nt->Fill(24,37.8437,52.0938);
nt->Fill(25,32.6659,42.3553);
nt->Fill(26,27.83,33.7461);
nt->Fill(27,23.7892,27.3213);
nt->Fill(28,20.1745,21.8348);
nt->Fill(29,16.8453,17.1722);
nt->Fill(30,14.0322,13.5661);
nt->Fill(31,11.602,10.6604);
nt->Fill(32,9.52528,8.31383);
nt->Fill(33,7.6984,6.37662);
nt->Fill(34,6.446,5.12347);
nt->Fill(35,4.96683,3.73576);
nt->Fill(36,4.23649,3.07268);
nt->Fill(37,3.50147,2.41358);
nt->Fill(38,3.16107,2.10707);
nt->Fill(39,2.7877,1.76851);

   return nt;
}


TH1D *getReference(double TAA, double centFrac){
   // iso 5 GeV
   double bins[7]={20,25,30,40,50,80,140};
   TH1D *h = new TH1D("h","",6,bins);
   h->SetBinContent(1,2099.6);
   h->SetBinContent(2,853.508);
   h->SetBinContent(3,306.346);
   h->SetBinContent(4,91.0847);
   h->SetBinContent(5,19.6313);
   h->SetBinContent(6,1.57827);
   h->SetXTitle("Photon p_{T} GeV/c");
   h->SetYTitle("dN/d#p_{T} (nb)");
   cout <<"TAA = "<<TAA<<endl;
   cout <<"CentFrac = "<<centFrac<<endl;
   double nMB = 5.5e7;
   h->Scale(nMB*TAA*centFrac*1e-9);
   return h;
   
}
