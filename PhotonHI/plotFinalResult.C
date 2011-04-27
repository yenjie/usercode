#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCut.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile.h>
#include <TNtuple.h>

#include "common.h"
#include "histFunction2.C"
#include "commonUtility.h"

#define canvasX 800
#define canvasY 600

int correctRes(TH1F* h, int cBinL, int cBinH)
{
   double p0=0,p1=0,p2=0;
   if (cBinL==0&&cBinH==4) {
      p0 = 0.7718;
      p1=0.01425;
      p2=-0.0001432;
   } 

   if (cBinL==4&&cBinH==12) {
      p0 = 0.8577;
      p1=0.008235;
      p2=-8.627e-5;
   } 

   if (cBinL==12&&cBinH==40) {
      p0 = 1.007;
      p1= -0.0003508;
      p2=-5.535e-6;
   } 
   
   if (p0==0){
      cout <<"Bin setting not available!!! Not corrected!"<<endl;
      return 0;
   } else {
      cout <<"Corrected!"<<endl;
   }

   for (int i=1;i<=h->GetNbinsX();i++)
   {
      double x = h->GetBinCenter(i);
      double corr = p0+x*p1+x*x*p2;
      h->SetBinContent(i,h->GetBinContent(i)/corr);
      h->SetBinError(i,h->GetBinError(i)/corr);
   }

   return 1;
}

void scaleBin(TH1F *h,int i,double width)
{
   double val = h->GetBinContent(i)/width;
   double valErr = h->GetBinError(i)/width;
   h->SetBinContent(i,val);
   h->SetBinError(i,valErr);
}

void cleanZeroBin(TH1 *h)
{
   for (int i=1;i<=h->GetNbinsX();i++)
   {
      if (h->GetBinContent(i)<0) {
         h->SetBinContent(i,0);
	 h->SetBinError(i,0);
      }
   }
}


void cleanError(TH1* h)
{
   for (int i=1;i<=h->GetNbinsX();i++)
   {
      h->SetBinError(i,0);
   }
}

TGraphAsymmErrors *calcEff(TH1* h1, TH1* h2,char *hName="hEff")
{
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
   gEfficiency->BayesDivide(h2,h1);
   return gEfficiency;
}


double doFit(parameter par, double minEt, double maxEt, double &result, double &resultErr)
{

   int iterate = 1;

   static int count=0;
   count++;

   // Selection cut
   TCut etCut = Form("abs(eta)<1.44&&(etCorrected)>%f&&(etCorrected)<%f",minEt,maxEt);
   TCut sigCut = "isGenMatched && abs(genMomId) <= 22 && genCalIsoDR04 < 5.0";

   char *title = Form("%.0f<E_{T}<%.0f GeV",minEt,maxEt);

   // Histograms
   TH1F *hData = new TH1F(Form("hData%d",count),"",par.nBin,par.binL,par.binH);
   TH1F *hSig = new TH1F(Form("hSig%d",count),"",par.nBin,par.binL,par.binH);
   TH1F *hBck = new TH1F(Form("hBck%d",count),"",par.nBin,par.binL,par.binH);

   par.tData->Draw(Form("%s>>hData%d",par.var,count),etCut&&par.selectionCut&&par.dataCut);
//   par.tSig->Draw(Form("%s>>hSig%d",par.var,count),   (etCut&&par.selectionCut&&sigCut&&par.dataCut)*"cBinWeight");
   par.tMulti->Draw(hSig,Form("%s",par.var,count),   (etCut&&par.selectionCut&&sigCut&&par.dataCut));
   hSig->SetName(Form("hSig%d",count));
//   par.tBck->Draw(Form("%s>>hBck%d",par.var,count),   etCut&&par.selectionCut&&!sigCut&&par.dataCut);//*"(49.7014*(exp((-0.943492e-3)*cBin*cBin*cBin+0.09106*cBin*cBin-0.44891*cBin+2.2789))+31.5117)");
   par.tData->Draw(Form("%s+%f>>hBck%d",par.var,par.shift,count), etCut&&par.selectionCut&&par.dataSidebandCut);

   TH1F *hFinal = (TH1F*)hData->Clone();
   hFinal->SetName(Form("hFinal%d",count));

   TH1F *hBckPdf;
   TH1F *hSigPdf;

   double nev;
   double ratio;
   double ratioErr;
   
   for (int j=0;j<iterate;j++) {
      histFunction2 *myfun = new histFunction2(hSig,hBck);
      TF1 *f = new TF1("f",myfun,&histFunction2::evaluate,par.binL,par.binH,2);
      f->SetParameters(hData->GetEntries(),0.5);
      f->SetParLimits(1,0,1);
      hData->Fit("f","LL m");
      hData->Fit("f","LL m");
      hData->Draw("pe");
      result=0;
      resultErr=0;
      if (hData->GetEntries()==0) return 0;
   
      nev = f->GetParameter(0);  
      ratio = f->GetParameter(1); 
      ratioErr = f->GetParError(1);

      mcStyle(hSig);
      sbStyle(hBck);
      hSigPdf = (TH1F*)hSig->Clone();
      hSigPdf->SetName(Form("hSigPdf%d",count));
      hSigPdf->Scale(nev*ratio/hSigPdf->Integral(1,hSigPdf->GetNbinsX()));

      hBckPdf = (TH1F*)hBck->Clone();
      hBckPdf->SetName(Form("hBckPdf%d",count));
      hBckPdf->Scale(nev*(1-ratio)/hBckPdf->Integral(1,hSigPdf->GetNbinsX()));
   
      count++;
      hBck->Add(hFinal);
      hBck->SetName(Form("hBck%d",count));
      hBck->Add(hSigPdf,-1);
      hBck->Scale(1./2.);

      hSigPdf->Add(hBckPdf);


      hSigPdf->SetXTitle(title);
      hSigPdf->SetNdivisions(505);
      hFinal->SetXTitle(par.xTitle);

      hFinal->SetYTitle("Entries");



      hFinal->Draw("e");
   
      hSigPdf->Draw("hist same");
      hBckPdf->Draw("hist same");
      hFinal->Draw("e same");
      delete myfun;
   }

   TLegend *t3=new TLegend(0.50,0.74,0.94,0.94);
   t3->AddEntry(hFinal,"PbPb #sqrt{s}_{_{NN}}=2.76TeV","pl");
   t3->AddEntry(hSigPdf,"Final fit  results","lf");
   t3->AddEntry(hBckPdf,"Background fit","lf");
   t3->SetFillColor(0);
   t3->SetBorderSize(0);
   t3->SetFillStyle(0);
   t3->SetTextFont(63);
   t3->SetTextSize(15);
   if (minEt == 20) t3->Draw();
//      drawCMS2(0.53,0.9,6.7,16);


   TLegend *tPt=new TLegend(0.6,0.6,0.94,0.7);
   tPt->AddEntry(hFinal,Form("%.0f-%.0f GeV",minEt,maxEt),"");
   tPt->SetFillColor(0);
   tPt->SetBorderSize(0);
   tPt->SetFillStyle(0);
   tPt->SetTextFont(63);
   tPt->SetTextSize(15);
   tPt->Draw();

   if (ratio>0) {
     result = nev*ratio;
     resultErr = nev*ratio*sqrt(1./nev+(ratioErr/ratio)*(ratioErr/ratio));
     cout <<hData->GetEntries()<<endl;
     cout <<nev<<endl;
     cout <<minEt<<" "<<maxEt<<"Nsig : "<<result<<" +- "<<resultErr<<endl;
   }
   
   
   return 1;
}

int plotFinalResult(char* infname = "data/mpaData_march17th.root",
                    int cBinL = 0,
		    int cBinH = 4,
		    bool plotYongsun = 1,
		    bool doElectronCorrection = 1,
		    bool ratioMode = 0,
		    bool doResCorrection =1
                   )
{

   // Selection cut
   selectionCriteria sel;
   sel.eventCut     = "( !TTBit[36] && !TTBit[37] && !TTBit[38] && !TTBit[39] && !vtxIsFake && abs(vtxZ) <= 15)";
   sel.centralityCut = Form("cBin>=%d&&cBin<%d",cBinL,cBinH);
   TString swissCrx = "(1 - (eRight+eLeft+eTop+eBottom)/eMax)";
   TCut hiSpikeCut       = Form("(  %s < 0.9 && abs(seedTime)<3  && sigmaIetaIeta>0.002 )  || isEE",swissCrx.Data());
   
   sel.removeSpikeCut = hiSpikeCut;
   sel.photonCut ="r9>0.&&hadronicOverEm<0.1&&abs(eta)<1.44&&rawEnergy/energy>0.5\
                   ";
   sel.removeElectronCut = "!isEle";//"!isEle";//"!isEle";
   
   sel.selectionCut = sel.photonCut&&sel.centralityCut&&sel.eventCut&&sel.removeSpikeCut&&sel.removeElectronCut; 
   sel.selectionCutWOElectronCut = sel.photonCut&&sel.centralityCut&&sel.eventCut&&sel.removeSpikeCut; 
   sel.selectionCut2 =sel.photonCut&&sel.eventCut&&sel.removeSpikeCut; 


   sel.sigCut = "isGenMatched && abs(genMomId) <= 22 && genCalIsoDR04 < 5.0 && genMatchedCollId ==0";

//   TCut cutBased = "0.181<r9&&sigmaIetaIeta<0.019&&ct4PtCut<1.25177&&cr4<3.6135&&cc4<419";

   // Data sample

   multiTreeUtil* trPho = new multiTreeUtil();
   setupMultiTree(trPho, sel.selectionCut);

   TFile *infData = new TFile(infname);
   TTree *tData = (TTree*)infData->FindObjectAny("Analysis");
   TTree *tDataGen = (TTree*)infData->FindObjectAny("Generator");

   // Signal sample
   TFile *infSig = new TFile("PythiaData/mpaPhotonJetAll_mix_hiData_correctedTree.root");
//   TFile *infSig = new TFile("PythiaData/mpaEmJet_correctedTree.root");
//  TFile *infSig = new TFile("PythiaData/mpaPhotonJet15_mix_hiData_correctedTree.root");
   TTree *tSig = (TTree*)infSig->FindObjectAny("Analysis");
   TTree *tSigGen = (TTree*)infSig->FindObjectAny("Generator");

   // Background sample
   TFile *infBck = new TFile("ampt/test.root");
   TTree *tBck = (TTree*)infBck->FindObjectAny("Analysis");

   // W sample
//   TFile *infW = new TFile("ampt/mpa_w2ev_mix_ampt_april15_correctedTree.root");
   TFile *infW = new TFile("PythiaData/mpa_wEv_mix_hiData_correctedTree.root");
   TFile *infWGen = new TFile("PythiaData/genTree_wEv_mix_hiData.root");
   TTree *tW = (TTree*)infW->FindObjectAny("Analysis");
   TTree *tWGen = (TTree*)infWGen->FindObjectAny("photon");
   
   // Yongsun result:
   TFile *infYongsun = new TFile("Yongsun/spectrum_iTemp12777.root");
   TTree *ntY = (TTree*) infYongsun->FindObjectAny("nt");
   ntY->SetName("ntY");   

   // Correction file
   TFile *correctionFile = new TFile("correction/correction-PYTHIAData.root");
   TH2D *hEtCorrection = (TH2D*)correctionFile->FindObjectAny("hEtCorrection");
   
   // ncoll ntuple
   
   TNtuple *ntColl = getNcoll();
   
   // signal region 
   if (cBinL > -10) {
      sel.dataCut = "sigmaIetaIeta<0.0115";
   } else {
      sel.dataCut = "sigmaIetaIeta<0.012";
   }
   sel.dataCutShowerAna = "(cr4+cc4)<5&&ct4PtCut<3 ";

   // sideband region
   if (cBinL >0) {
      sel.dataSidebandCut = "sigmaIetaIeta>0.0115&&sigmaIetaIeta<0.0145";
   } else {
      sel.dataSidebandCut = "sigmaIetaIeta>0.0120&&sigmaIetaIeta<0.014";
   }
   sel.dataSidebandCutShowerAna =  "(cr4+cc4)<5&&ct4PtCut>6 ";

   TH1D *hNcoll = new TH1D("hNcoll","",1000,0,2000);
   ntColl->Project("hNcoll","ncoll",sel.centralityCut);

   double scale = hNcoll->GetMean()*(cBinH-cBinL)*0.025;
   
   // Output file
   TFile *outfile = new TFile(Form("output-%d-%d.root",cBinL,cBinH),"recreate");
   TNtuple *nt = new TNtuple("nt","","et:etL:etH:val:valStatErr:valSysErr:cBinL:cBinH:method:ncoll:scale");

   // Add friends 
   cout <<"Add Centrality reweighting to signal ..."<<endl;
   addCentralityFriend(tSig,tData,sel.selectionCut&&"sigmaIetaIeta<0.0125&&(cr4+cc4)<5");

   // Binning
   const int nEtBin = 5;
   double myBin[nEtBin+1] = {20,25,30,40,50,80};
//   const int nEtBin = 1;
//   double myBin[nEtBin+1] = {20,25};

   // Histogram
   TH1F *hEtAll = new TH1F("hEtAll","",nEtBin,myBin);
   TH1F *hEtEle = new TH1F("hEtEle","",nEtBin,myBin);
   TH1F *hEtEleAll = new TH1F("hEtEleAll","",nEtBin,myBin);
   TH1F *hEtCaloIso = new TH1F("hEtCaloIso","",nEtBin,myBin);
   TH1F *hEtSumIso = new TH1F("hEtSumIso","",nEtBin,myBin);
   TH1F *hEtSR = new TH1F("hEtSR","",nEtBin,myBin);
   TH1F *hEtSRCalo = new TH1F("hEtSRCalo","",nEtBin,myBin);
   TH1F *hEtSIEIE = new TH1F("hEtSIEIE","",nEtBin,myBin);
   TH1F *hEtSE = new TH1F("hEtSE","",nEtBin,myBin);

   TH1F *hEtEff = new TH1F("hEtEff","",nEtBin,myBin);
   TH1F *hEtEffShowerAna = new TH1F("hEtEffShowerAna","",nEtBin,myBin);

  
   TH1F *hEtWEff = new TH1F("hEtWEff","",nEtBin,myBin);
   TH1F *hEtWEffWOElectronCut = new TH1F("hEtWEffWOElectronCut","",nEtBin,myBin);
   TH1F *hEtWEffCut = new TH1F("hEtWEffCut","",nEtBin,myBin);
    

   tData->Draw("etCorrected>>hEtAll", sel.selectionCut);
   
   // Data matched to Electron
   tData->Draw("etCorrected>>hEtEle", sel.selectionCutWOElectronCut&&"isEle&&sigmaIetaIeta<0.012&&cr4+cc4<10");
   tData->Draw("etCorrected>>hEtEleAll", sel.selectionCutWOElectronCut&&"isEle&&sigmaIetaIeta<0.012&&cr4+cc4<10");
   
   // W sample matched to Electron
   tW->Draw("etCorrected>>hEtWEffCut", sel.selectionCutWOElectronCut&&"isEle&&sigmaIetaIeta<0.012&&cr4+cc4<10");
   tW->Draw("etCorrected>>hEtWEffWOElectronCut", sel.selectionCutWOElectronCut&&"isEle&&sigmaIetaIeta<0.012&&cr4+cc4<10");
   tW->Draw("etCorrected>>hEtWEff", sel.selectionCut);


   TH1F *hEtGen = new TH1F("hEtGen","",nEtBin,myBin);
   TH1F *hEtWGen = new TH1F("hEtWGen","",nEtBin,myBin);
   TH1F *hEtTruth = new TH1F("hEtTruth","",nEtBin,myBin);

   if (tDataGen!=0) tDataGen->Draw("pt>>hEtGen","abs(eta)<1.44 && calIsoDR04<5 && collId==0"&&sel.centralityCut);
   cout <<"prepare truth"<<endl;
   tSigGen->Draw("pt>>hEtTruth","abs(eta)<1.44 && calIsoDR04<5 && collId==0"&&sel.centralityCut);
   
   
   cout <<"prepare Efficiency"<<endl;

   tSig->Draw("genMatchedPt>>hEtEff",sel.sigCut&&sel.selectionCut&&sel.dataCut);
   cout <<"prepare Efficiency showerana"<<endl;
   tSig->Draw("genMatchedPt>>hEtEffShowerAna",sel.sigCut&&sel.selectionCut&&sel.dataCutShowerAna);
   

   hEtEff->Sumw2();
   hEtEffShowerAna->Sumw2();
   hEtTruth->Sumw2();
//   TGraphAsymmErrors *gEtEff = calcEff(hEtEff,hEtTruth);
   hEtEff->Divide(hEtTruth);
   hEtEffShowerAna->Divide(hEtTruth);
//   cleanError(hEtEff);
//   cleanError(hEtEffShowerAna);


   // W generator info
   tWGen->Draw("et>>hEtWGen","abs(eta)<1.44 && status == 1 && abs(id)==11");

   hEtWEff->Sumw2();
   hEtWEffCut->Sumw2();
   hEtWEffWOElectronCut->Sumw2();
   
   hEtWEffCut->Divide(hEtWEff);
   hEtWEff->Divide(hEtWGen);
   hEtWEffWOElectronCut->Divide(hEtWGen);
   hEtEleAll->Divide(hEtWEffWOElectronCut);
   
   TCanvas *cW = new TCanvas("cW","cW",600,600);
   hEtWEffWOElectronCut->Draw();
   
   
   // Book canvas   
   TCanvas *c0 = new TCanvas("c0","Calo Iso",canvasX,canvasY);
   TCanvas *c1 = new TCanvas("c1","Sum Iso",canvasX,canvasY);
   TCanvas *c2 = new TCanvas("c2","S_{R}",canvasX,canvasY);
   TCanvas *c3 = new TCanvas("c3","S_{R}^{Calo}",canvasX,canvasY);
   TCanvas *c4 = new TCanvas("c4","SIEIE",canvasX,canvasY);
   TCanvas *c5 = new TCanvas("c5","S_E",canvasX,canvasY);
   c0->Divide(3,2);
   c1->Divide(3,2);
   c2->Divide(3,2);
   c3->Divide(3,2);
   c4->Divide(3,2);
   c5->Divide(3,2);
   makeMultiPanelCanvas(c0,3,2,0.0,0.0,0.2,0.2,0.02);
   makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.2,0.2,0.02);
   makeMultiPanelCanvas(c2,3,2,0.0,0.0,0.2,0.2,0.02);
   makeMultiPanelCanvas(c3,3,2,0.0,0.0,0.2,0.2,0.02);
   makeMultiPanelCanvas(c4,3,2,0.0,0.0,0.2,0.2,0.02);
   makeMultiPanelCanvas(c5,3,2,0.0,0.0,0.2,0.2,0.02);


   bool doCaloIso = false;
   bool doSumIso = false;
   bool doSR = false;
   bool doSRCalo = false;
   bool doSIEIE = false;

   // Parameters
   parameter par;
   par.tData = tData;
   par.tSig = tSig;
   par.tBck = tBck;
   par.tMulti = trPho;
   par.selectionCut = sel.selectionCut;
   par.var = "cc4+cr4";
   par.nBin = 20;
   par.binL = -20;
   par.binH = 60;
   par.xTitle = "calo Iso (GeV)";
   par.dataCut = sel.dataCut;
   par.dataSidebandCut = sel.dataSidebandCut;

   // sumIso
   parameter par2;
   par2.tData = tData;
   par2.tSig = tSig;
   par2.tBck = tBck;
   par2.tMulti = trPho;
   par2.selectionCut = sel.selectionCut;
   par2.var = "cc4+cr4+ct4PtCut-cc05-ct05PtCut";
//   par2.var = "ecalRecHitSumEtConeDR04 - compEcalIso + hcalTowerSumEtConeDR04 - compHcalIso + trkSumPtHollowConeDR04 - compTrackIso";
   par2.nBin = 20;
   par2.binL = -20;
   par2.binH = 60;
//   par2.var = "(cc4+cr4+ct4PtCut-ct1PtCut-cc1)";
//   par2.nBin = 28;
//   par2.binL = -20;
//   par2.binH = 50;
   par2.xTitle = "sumIso (GeV)";
   par2.dataCut = sel.dataCut;
   par2.shift = -0;
   par2.dataSidebandCut = sel.dataSidebandCut;

   // SR
   parameter par3;
   par3.tData = tData;
   par3.tSig = tSig;
   par3.tBck = tBck;
   par3.tMulti = trPho;
   par3.selectionCut = sel.selectionCut;
   par3.dataCut = sel.dataCut;
   par3.dataSidebandCut = sel.dataSidebandCut;

   par3.var = "(0.00391075)*ct1PtCut+(-0.00193374)*ct2PtCut+(-0.00385588)*ct3PtCut+(-0.00128258)*ct4PtCut+(-0.00333680)*ct5PtCut+(0.00283706)*cr1+(-0.00461487)*cr2+(-0.00107716)*cr3+(0.00102037)*cr4+(-0.00626814)*cr5+(0.01774662)*cc1+(-0.01078755)*cc2+(-0.00739476)*cc3+(-0.00338893)*cc4+(-0.00185528)*cc5+0.162514863019";
   par3.nBin = 20;
   par3.binL = -0.5;
   par3.binH = 0.5;
   par3.xTitle = "S_{R}^{Calo}";
   par3.dataCut = sel.dataCut;
   par3.dataSidebandCut = sel.dataSidebandCut;

   // SRCalo
   parameter par31;
   par31.tData = tData;
   par31.tSig = tSig;
   par31.tBck = tBck;
   par31.tMulti = trPho;
   par31.selectionCut = sel.selectionCut;
   par31.dataCut = sel.dataCut;
   par31.dataSidebandCut = sel.dataSidebandCut;

   par31.var ="(0.00380343)*cr1+(-0.00488757)*cr2+(-0.00539736)*cr3+(-0.00007929)*cr4+(-0.00400283)*cr5+(0.01808192)*cc1+(-0.01333757)*cc2+(-0.00529946)*cc3+(-0.00415226)*cc4+(-0.00276343)*cc5+0.162514863019";
   par31.nBin = 20;
   par31.binL = -0.5;
   par31.binH = 0.5;
   par31.xTitle = "S_{R}";
   par31.dataCut = sel.dataCut;
   par31.dataSidebandCut = sel.dataSidebandCut;

   // sigmaIetaIeta
   parameter par4;
   par4.tData = tData;
   par4.tSig = tSig;
   par4.tBck = tBck;
   par4.tMulti = trPho;
   par4.selectionCut = sel.selectionCut;
   par4.dataCut = sel.dataCut;
   par4.dataSidebandCut = sel.dataSidebandCut;

   par4.var = "sigmaIetaIeta";
   par4.nBin = 40;
   par4.binL = 0;
   par4.binH = 0.025;
 /*
   par4.var = "sigmaIetaIeta";
   par4.nBin = 60;
   par4.binL = 0.000;
   par4.binH = 0.03;
*/

   par4.xTitle = "SigmaIetaIeta";
   par4.dataCut = sel.dataCutShowerAna;
   par4.dataSidebandCut = sel.dataSidebandCutShowerAna;
   
   // SE
   parameter par5;
   par5.tData = tData;
   par5.tSig = tSig;
   par5.tBck = tBck;
   par5.tMulti = trPho;
   par5.selectionCut = sel.selectionCut;
   par5.var = "(-4.00626572)*eMax/e2x2+(5.07941193)*eMax/e3x3+(0.01855105)*eMax/e5x5+(-0.08046821)*eMax/energy+(-6.36218195)*e2x2/e3x3+(2.44924347)*e2x2/e5x5+(6.06287631)*e2x2/energy+(-3.01610493)*e3x3/e5x5+(-2.19817850)*e3x3/energy+(-1.81670490)*e5x5/energy+5.1982572901";   
   par5.nBin = 20;
   par5.binL = -1;
   par5.binH = 1;
   par5.xTitle = "S_{E} ";
   par5.dataCut = sel.dataCutShowerAna;
   par5.dataSidebandCut = sel.dataSidebandCutShowerAna;

   double sys = 0.3;
   for (int i=0;i<nEtBin;i++)
   {
      double result,resultErr;

      c0->cd(i+1);
      // doFit(par,myBin[i],myBin[i+1],result,resultErr);
      hEtCaloIso->SetBinContent(i+1,result);
      hEtCaloIso->SetBinError(i+1,sqrt(resultErr*resultErr+sys*result*sys*result));

      c1->cd(i+1);
      doFit(par2,myBin[i],myBin[i+1],result,resultErr);
      hEtSumIso->SetBinContent(i+1,result);
      hEtSumIso->SetBinError(i+1,sqrt(resultErr*resultErr+sys*result*sys*result));

      c2->cd(i+1);
      //doFit(par3,myBin[i],myBin[i+1],result,resultErr);
      hEtSR->SetBinContent(i+1,result);
      hEtSR->SetBinError(i+1,sqrt(resultErr*resultErr+sys*result*sys*result));

      c3->cd(i+1);
      // doFit(par31,myBin[i],myBin[i+1],result,resultErr);
      hEtSRCalo->SetBinContent(i+1,result);
      hEtSRCalo->SetBinError(i+1,sqrt(resultErr*resultErr+sys*result*sys*result));

      c4->cd(i+1);
      doFit(par4,myBin[i],myBin[i+1],result,resultErr);
      hEtSIEIE->SetBinContent(i+1,result);
      hEtSIEIE->SetBinError(i+1,sqrt(resultErr*resultErr+sys*result*sys*result));

      c5->cd(i+1);
      //doFit(par5,myBin[i],myBin[i+1],result,resultErr);
      hEtSE->SetBinContent(i+1,result);
      hEtSE->SetBinError(i+1,sqrt(resultErr*resultErr+sys*result*sys*result));
   }

   for (int i=0;i<nEtBin;i++)
   {
      double binWidth = myBin[i+1]-myBin[i];
      scaleBin(hEtCaloIso,i+1,binWidth);
      scaleBin(hEtSumIso,i+1,binWidth);
      scaleBin(hEtSR,i+1,binWidth);
      scaleBin(hEtSRCalo,i+1,binWidth);
      scaleBin(hEtSIEIE,i+1,binWidth);
      scaleBin(hEtSE,i+1,binWidth);
      scaleBin(hEtAll,i+1,binWidth);
      scaleBin(hEtEle,i+1,binWidth);
      scaleBin(hEtGen,i+1,binWidth);
      scaleBin(hEtEleAll,i+1,binWidth);
   }

   hEtCaloIso->SetLineColor(2);
   hEtCaloIso->SetMarkerColor(2);
   hEtSumIso->SetLineColor(1);
   hEtSumIso->SetMarkerColor(1);
   hEtSumIso->SetMarkerStyle(4);
   hEtSumIso->SetMarkerSize(1.2);
   hEtSR->SetLineColor(6);
   hEtSR->SetMarkerColor(6);
   hEtSR->SetMarkerStyle(4);
   hEtSR->SetMarkerSize(1.2);
   hEtSRCalo->SetLineColor(kGreen+2);
   hEtSRCalo->SetMarkerColor(kGreen+2);
   hEtSRCalo->SetMarkerStyle(4);
   hEtSRCalo->SetMarkerSize(1.2);
   hEtSIEIE->SetLineColor(1);
   hEtSIEIE->SetMarkerColor(1);
   hEtSIEIE->SetMarkerStyle(20);
   hEtSIEIE->SetMarkerSize(1.2);
   hEtSE->SetLineColor(kBlue+2);
   hEtSE->SetMarkerColor(kBlue+2);
   hEtSE->SetMarkerStyle(4);
   hEtSE->SetMarkerSize(1.2);
   hEtGen->SetLineColor(1);
   hEtGen->SetMarkerColor(1);
   hEtEle->SetLineColor(kBlue);
   hEtEle->SetMarkerColor(kBlue);
   hEtEle->SetMarkerStyle(4);
   hEtEle->SetMarkerSize(1.2);
   hEtEleAll->SetMarkerColor(4);
   hEtEleAll->SetMarkerStyle(20);
   hEtEleAll->SetLineColor(4);

   TCanvas *cEff = new TCanvas ("cEff","",600,600);
   hEtEff->SetXTitle("E_{T} (GeV)");
   hEtEff->SetYTitle("Efficiency");
   hEtEff->Draw();
   hEtEffShowerAna->SetLineColor(2);
   hEtEffShowerAna->SetMarkerColor(2);
   hEtEffShowerAna->Draw("same");
//   gEtEff->SetLineColor(4);
//   gEtEff->SetMarkerColor(4);
//   gEtEff->Draw("pe same");   


   // Electron component correction
   hEtEle->Divide(hEtWEffCut);
   hEtEle->Scale(1./1.22);

   if (doElectronCorrection) {
      hEtSIEIE->Add(hEtEle,-1);
      hEtSumIso->Add(hEtEle,-1);
      hEtSR->Add(hEtEle,-1);
      hEtSIEIE->Scale(1./0.98);
      hEtSumIso->Scale(1./0.98);  // Photon -> W fake rate;
      hEtSR->Scale(1./0.98);
   }   
   
   TCanvas *cSummary = new TCanvas ("cSummary","",600,600);
   cSummary->SetLogy();
   TH1F *hEtSumIsoCorrected = (TH1F*) hEtSumIso->Clone();
   hEtSumIsoCorrected->SetName("hEtSumIsoCorrected");
   TH1F *hEtCorrected = (TH1F*) hEtCaloIso->Clone();
   hEtCorrected->SetName("hEtCorrected");
   TH1F *hEtSRCorrected = (TH1F*) hEtSR->Clone();
   hEtSRCorrected->SetName("hEtSRCorrected");
   TH1F *hEtSRCaloCorrected = (TH1F*) hEtSRCalo->Clone();
   hEtSRCaloCorrected->SetName("hEtSRCaloCorrected");
   TH1F *hEtSIEIECorrected = (TH1F*) hEtSIEIE->Clone();
   hEtSIEIECorrected->SetName("hEtSIEIECorrected");
   TH1F *hEtSECorrected = (TH1F*) hEtSE->Clone();
   hEtSECorrected->SetName("hEtSECorrected");
   hEtSumIsoCorrected->Divide(hEtEff);
   hEtCorrected->Divide(hEtEff);
   hEtSRCorrected->Divide(hEtEff);
   hEtSRCaloCorrected->Divide(hEtEff);
   hEtSIEIECorrected->Divide(hEtEffShowerAna);
   hEtSECorrected->Divide(hEtEffShowerAna);
   hEtEle->Divide(hEtEffShowerAna);


   if (doResCorrection) {
      correctRes(hEtCorrected,cBinL,cBinH);
      correctRes(hEtSRCorrected,cBinL,cBinH);
      correctRes(hEtSRCaloCorrected,cBinL,cBinH);
      correctRes(hEtSIEIECorrected,cBinL,cBinH);
      correctRes(hEtSECorrected,cBinL,cBinH);
      correctRes(hEtSumIsoCorrected,cBinL,cBinH);
   }

   for (int i=0;i<nEtBin;i++)
   {
      nt->Fill((myBin[i+1]+myBin[i])/2,myBin[i],myBin[i+1],
                hEtSumIsoCorrected->GetBinContent(i+1),hEtSumIsoCorrected->GetBinError(i+1),hEtSumIsoCorrected->GetBinContent(i+1)*0.2,
		cBinL,cBinH,1,hNcoll->GetMean(),scale);

      nt->Fill((myBin[i+1]+myBin[i])/2,myBin[i],myBin[i+1],
                hEtSIEIECorrected->GetBinContent(i+1),hEtSIEIECorrected->GetBinError(i+1),hEtSIEIECorrected->GetBinContent(i+1)*0.2,
		cBinL,cBinH,2,hNcoll->GetMean(),scale);

   }

   if (hEtGen->Integral(1,nEtBin)!=0) {     
      hEtCorrected->Divide(hEtGen);
      hEtSRCorrected->Divide(hEtGen);
      hEtSRCaloCorrected->Divide(hEtGen);
      hEtSIEIECorrected->Divide(hEtGen);
      hEtSECorrected->Divide(hEtGen);
      hEtSumIsoCorrected->Divide(hEtGen);
      hEtEle->Divide(hEtGen);
      hEtEleAll->Divide(hEtGen);
      hEtGen->Divide(hEtGen);
      hEtCorrected->SetAxisRange(0,2,"Y");
   }

   
   hEtSumIsoCorrected->Draw("");
   hEtSumIsoCorrected->SetXTitle("Photon E_{T} (GeV)");
   hEtSumIsoCorrected->SetYTitle("dN/dE_{T}");

//   hEtCorrected->Draw("same");
   
//   hEtSRCorrected->Draw("same");
//   hEtSRCaloCorrected->Draw("same");
   if (!ratioMode) hEtSIEIECorrected->Draw("same");
//   hEtSECorrected->Draw("same");
   TH1D *hAvg = (TH1D*)hEtSIEIECorrected->Clone();
   hAvg->SetName("hAvg");
   hAvg->Add(hEtSumIsoCorrected);
   hAvg->Scale(1./2);
   
   hAvg->SetMarkerStyle(25);
   hAvg->SetMarkerColor(1);
   hAvg->SetLineColor(1);
//   hAvg->Draw("e same");
   if (!ratioMode) hEtEle->Draw("e same");
//   hEtEleAll->Draw("e same");
   hEtGen->Draw("hist same");
   
   double TAA=0;
   
   if (cBinL==0&&cBinH==4) TAA=23.2;
   if (cBinL==4&&cBinH==12) TAA=11.6;
   if (cBinL==12&&cBinH==40) TAA=1.45;
   TH1D *hRef = getReference(TAA,(cBinH-cBinL)*0.025);
   hRef->SetName("hRef");
//   hRef->Draw("same");
   hRef->SetLineColor(2);
   if (1==2&&TAA!=0&&hEtGen->Integral(1,nEtBin)==0) {     
      hEtCorrected->Divide(hRef);
      hEtSRCorrected->Divide(hRef);
      hEtSRCaloCorrected->Divide(hRef);
      hEtSIEIECorrected->Divide(hRef);
      hEtSECorrected->Divide(hRef);
      hEtSumIsoCorrected->Divide(hRef);
      hEtEle->Divide(hRef);
      hEtGen->Divide(hRef);
      hEtCorrected->SetAxisRange(0,2,"Y");
      hAvg->Divide(hRef);
   } else {
      hEtSumIsoCorrected->SetAxisRange(1e-1,5e3,"Y");
   }

   if (ratioMode) {     
      cSummary->SetLogy(0);
      cleanError(hEtSIEIECorrected);
      hEtCorrected->Divide(hEtSIEIECorrected);
      hEtSRCorrected->Divide(hEtSIEIECorrected);
      hEtSRCaloCorrected->Divide(hEtSIEIECorrected);
      hEtSECorrected->Divide(hEtSIEIECorrected);
      hEtSumIsoCorrected->Divide(hEtSIEIECorrected);
      hEtEle->Divide(hEtSIEIECorrected);
      hEtGen->Divide(hEtSIEIECorrected);
      hEtEleAll->Divide(hEtSIEIECorrected);
      hEtSIEIECorrected->Divide(hEtSIEIECorrected);
      hEtSumIsoCorrected->SetAxisRange(0,3,"Y");
      hEtSumIsoCorrected->SetYTitle("Ratio");
      TLine *l = new TLine(myBin[0],1,myBin[nEtBin],1);
      l->Draw();
   }


   ntY->SetLineColor(2);
   ntY->SetMarkerColor(2);
   if (plotYongsun) ntY->Draw(Form("val:et"),Form("cBinL==%d",cBinL,cBinH),"same");

   TLegend *leg2 ;
   leg2 = new TLegend(0.4,0.65,0.9,0.9);
   leg2->SetFillStyle(0);
   leg2->SetBorderSize(0);
   leg2->AddEntry(hEtSumIsoCorrected,"CMS Preliminary","");
   leg2->AddEntry(hEtSumIsoCorrected,Form("PbPb %.0f-%.0f%%",cBinL*2.5,(cBinH)*2.5),"");
   leg2->AddEntry(hEtSumIsoCorrected,"","");
//   leg2->AddEntry(hEtCorrected,"Calo Iso method","pl");
//   leg2->AddEntry(hEtSRCorrected,"S_{R} method","pl");
//   leg2->AddEntry(hEtSRCaloCorrected,"S_{R}^{Calo} method","pl");
   if (!ratioMode)leg2->AddEntry(hEtSIEIECorrected,"#sigma_{i #eta i #eta} method","pl");
   leg2->AddEntry(hEtSumIsoCorrected,"Sum Iso method","pl");
   if (!ratioMode) leg2->AddEntry(hEtEle,"Electron Contamination","pl");
//   leg2->AddEntry(hEtSECorrected,"S_{E} method","pl");
   
   if (hEtGen->Integral(1,2))   leg2->AddEntry(hEtGen,"MC Truth","l");
   leg2->Draw();
   c0->SaveAs("plot/result.gif");   
   c0->SaveAs("plot/result.C");   

   cSummary->SaveAs(Form("plot/resultCorrected-%d-%d-%d.gif",cBinL,cBinH,ratioMode));   
   cSummary->SaveAs(Form("plot/resultCorrected-%d-%d-%d.eps",cBinL,cBinH,ratioMode));   
   cSummary->SaveAs(Form("plot/resultCorrected-%d-%d-%d.C",cBinL,cBinH,ratioMode));    
   cEff->SaveAs("plot/eff.gif");   
   cEff->SaveAs("plot/eff.C");   

  
   outfile->Write();   
   
}

