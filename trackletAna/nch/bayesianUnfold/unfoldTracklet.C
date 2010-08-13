// Standard library
#include <math.h>
#include <iostream.h>
#include <fstream>

// ROOT Library
#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TProfile.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TLine.h>
#include <TF1.h>
#include <TCut.h>
#include <TPad.h>
#include "TRandom.h"

// Unfolding
#include "bayesianUnfold.h"
#include "prior.h"

// Selection Criteria
#include "selectionCut.h"

// UA5 plot
#include "UA5.h"

// ALICE plot
#include "ALICE.h"

// Track plot
#include "Nick.h"

// bining
#include "myBin.h"

// Constants
#define CanvasSizeX 600
#define CanvasSizeY 600


int unfoldTracklet(int trackletType = 12, double _etaRange=2.5,
                    char *infName    = "TrackletTree-D6T-1.root",  // input data
		    char *infMCName  = "TrackletTree-D6T-1.root",  // input MC
                    char *useCorrectionFile = "",                  // correction MC
		    int toyCheck = 1,                              // check on toyMC
		    int errorType = 2,                             // err1: 4 unfolding, err2: 100 iteration
		    int energy = 900                               // sqrt(s)
		    )
{
  bool usePythiaSD = 0;
  bool usePhojetSD = 0;


  if (energy==900) usePythiaSD = 1;
  if (usePhojetSD == 1) usePythiaSD = 0;


  //=====================================================================================================================
  // Input files
  //=====================================================================================================================
  TFile *inf = new TFile(infName);
  TTree *t = (TTree*) inf->FindObjectAny(Form("TrackletTree%d",trackletType));
  
  TFile *infMC = new TFile(infMCName);
  TTree *tMC = (TTree*) infMC->FindObjectAny(Form("TrackletTree%d",trackletType));

  TFile *infMC2 = new TFile("TrackletTree-ATLAS-new.root");
  TTree *tMC2 = (TTree*) infMC2->FindObjectAny(Form("TrackletTree%d",trackletType));

  char *correctionFileName = Form("correction/correction-%d-%.1f-%s.root",trackletType,_etaRange,useCorrectionFile);
  TFile *fCorrection = new TFile(correctionFileName);

  char *correctionFileName2 = Form("correction/correction-%d-%.1f-%s.root",trackletType,_etaRange,"D6T-900GeV");
  TFile *fCorrectionPythiaD6T900GeV = new TFile(correctionFileName2);

  char *correctionFileName3 = Form("correction/correction-%d-%.1f-%s.root",trackletType,_etaRange,"PHOJET");
  TFile *fCorrectionPhojet7TeV = new TFile(correctionFileName3);

  //=====================================================================================================================
  // Output files
  //=====================================================================================================================
  TFile *outf = new TFile(Form("result/result-%d-%.1f-%s.root",trackletType,_etaRange,infName),"recreate");

  // Bining
  //=====================================================================================================================
    int nTrackletMin = -10;
  int nTrackletMax = 190;
  int nBin = 200;

  double TrackletBin[300];
  for (int i=0;i<=nBin;i++)
  {
     TrackletBin[i]=nTrackletMin+i;
  }
  
  // Multiplicity handle
  char *varM = Form("(Sum$(abs(eta1)<%f&&abs(deta)<0.1&&abs(dphi)<1))-(Sum$(abs(eta1)<%f&&abs(deta)<0.1&&abs(dphi)>1&&abs(dphi)<2))",_etaRange,_etaRange);
  //char *varM =  Form("(Sum$(abs(eta1)<%f&&abs(dR)<0.1))-(Sum$(abs(eta1)<%f&&abs(dR)>0.1&&abs(dR)<0.2))",_etaRange,_etaRange);

  // Truth
  //char *varT = Form("Sum$(abs(eta)<%f&&pdg!=11&&pdg!=13)",_etaRange);
  char *varT = Form("Sum$(abs(eta)<%f&&pdg!=11&&pdg!=13)",_etaRange);
  
  // Histograms
  TH1D *hM = new TH1D("hM","",nBin,TrackletBin);
  TH1D *hT = new TH1D("hT","",nBin,TrackletBin);
  TH2D *hR(0);
  TH1D *hMMC(0);
  
  TH1D *hNSD = new TH1D("hNSD","",nBin,TrackletBin);
  TH1D *hSelectedNSD = new TH1D("hSelectedNSD","",nBin,TrackletBin);
  TH1D *hSelectedSD  = new TH1D("hSelectedSD","",   nBin,TrackletBin);
  TH1D *hTrue        = new TH1D ("hTrue", "Test Truth",    nBin,TrackletBin);
  TH1D *hMeas        = new TH1D ("hMeas", "Test Measured", nBin,TrackletBin);
    
  // Selection Criteria
  selectionCut MCCut(1,0,1000);
  selectionCut dataCut(1,0,1000);

  TCut nonZeroCut = "";
  TCut NSDCut = "(evtType!=92&&evtType!=93)";
  
  if (useCorrectionFile=="") {
     if (tMC->GetEntries(!NSDCut)==0) {
        NSDCut = "evtType!=5&&evtType!=6"; 
        cout <<"PHOJET MC definition"<<endl;
     }
  }
  
  TH1D *hEffNSD(0); 
  TH1D *hSDFracSelected(0); 

  cout <<"tmp"<<endl;
  // prepare histograms
  
  if (useCorrectionFile=="") {  
     tMC->Draw(Form("%s>>hSelectedNSD",varT),MCCut.Cut&&NSDCut&&nonZeroCut);    
     tMC->Draw(Form("%s>>hNSD",varT),NSDCut&&nonZeroCut);
     tMC->Draw(Form("%s>>hSelectedSD",varT),MCCut.Cut&&(!NSDCut)&&nonZeroCut);

     hNSD->Sumw2();
     hSelectedNSD->Sumw2();
     hSelectedSD->Sumw2();

     hEffNSD = (TH1D*) hSelectedNSD->Clone();  // NSD efficiency
     hEffNSD->SetName("hEffNSD");
     hEffNSD->Divide(hNSD);
  
     TH1D *hSelected = (TH1D*) hSelectedNSD->Clone();  
     hSelected->SetName("hSelected");  
     hSelected->Add(hSelectedSD);  // selected INEL
  
     hSDFracSelected = (TH1D*) hSelectedSD->Clone();
     hSDFracSelected->SetName("hSDFracSelected");
     hSDFracSelected->Divide(hSelected);  // SD Fraction in selected INEL
     hR = new TH2D("hR","",nBin,TrackletBin,nBin,TrackletBin);
     hMMC = new TH1D("hMMC","",nBin,TrackletBin);
  
     tMC->Draw(Form("%s:%s>>hR",varM,varT),MCCut.Cut&&nonZeroCut);
     tMC->Draw(Form("%s>>hMMC",varM),MCCut.Cut&&nonZeroCut);

  } else {
     cout << correctionFileName <<endl;
     if (usePythiaSD) {
         hEffNSD = (TH1D*) fCorrectionPythiaD6T900GeV->FindObjectAny("hEffNSD");
     } else {
         if (usePhojetSD) {
            hEffNSD = (TH1D*) fCorrectionPhojet7TeV->FindObjectAny("hEffNSD");
         } else {
            hEffNSD = (TH1D*) fCorrection->FindObjectAny("hEffNSD");
         }
     }
     if (usePythiaSD) {
         hSDFracSelected = (TH1D*) fCorrectionPythiaD6T900GeV->FindObjectAny("hSDFracSelected");
     } else {
         hSDFracSelected = (TH1D*) fCorrection->FindObjectAny("hSDFracSelected");
     }

     hR = (TH2D*) fCorrection->FindObjectAny("hR");
     hMMC = (TH1D*) fCorrection->FindObjectAny("hMMC");
     if (!hEffNSD) { 
        cout <<"Error reading hEffNSD!"<<endl; 
        return 0;
     }     
     if (!hR) { 
        cout <<"Error reading hR!"<<endl; 
        return 0;
     }     
     if (!hSDFracSelected) { 
        cout <<"Error reading hSDFracSelected!"<<endl; 
        return 0;
     }     
     if (!hMMC) { 
        cout <<"Error reading hMMC!"<<endl; 
        return 0;
     }     
  }
  
  t->Draw(Form("%s>>hM",varM),dataCut.Cut);
  t->Draw(Form("%s>>hT",varT),NSDCut);

  
  for (int i=0;i<hT->GetNbinsX();i++)
  {
     hT->SetBinContent(i,1);
  }

  hR->Draw("box"); 
  hM->Sumw2();  
  hR->Sumw2();  
  

  double nevtScaled = 0;


  // rescale response function
  //cout <<"Turn off rescale response function"<<endl;

  for (Int_t m= 1; m<=hR->GetNbinsY(); m++)
  {
     double Measured = hM->GetBinContent(m);
     double MeasuredMC = hMMC->GetBinContent(m);
	
     double Ratio=0;
     if (MeasuredMC) {
        Ratio = Measured / MeasuredMC;
     }
     
     for (Int_t t= 1; t<=hR->GetNbinsX(); t++)
     {
	double var = hR->GetBinContent(t,m);
	double varErr = hR->GetBinError(t,m);
	hR->SetBinContent(t,m,var*Ratio);
	hR->SetBinError(t,m,varErr*Ratio);
     }  
  }
   
  prior myPrior(hR,hM,0.0);
  myPrior.unfold(hM,1);
  bayesianUnfold myUnfolding(hR,myPrior.hPrior,0.0);
//  bayesianUnfold myUnfolding(hR,hT,0.0);
  myUnfolding.unfold(hM,4);

  bayesianUnfold myUnfolding2(hR,hT,0.0);
  myUnfolding2.unfold(hM,4);

  bayesianUnfold myUnfolding3(hR,myPrior.hPrior,0.0);

  if (errorType==2) myUnfolding3.unfold(hM,50); else myUnfolding3.unfold(hM,4);
  
  TCanvas *c = new TCanvas("c","",CanvasSizeX,CanvasSizeY);

  TH1D *hRes = (TH1D*) myUnfolding.hResMatrix->Clone();
  hRes->SetXTitle("N_{ch}");
  hRes->SetYTitle("N_{Tracklet} (Background subtracted)");
  hRes->SetAxisRange(0,140,"X");
  hRes->SetAxisRange(0,140,"Y");
  hRes->Draw("col");
  
  
  TCanvas *c2 = new TCanvas("c2","",CanvasSizeX,CanvasSizeY);
  c2->SetLogy();
  TH1D *hU = (TH1D*)myUnfolding.hPrior->Clone();
  hU->SetName("hU");
  TH1D *hU2 = (TH1D*)myUnfolding2.hPrior->Clone();
  hU2->SetName("hU2");

  TNtuple *nt = new TNtuple("nt","","i:val:val0:err");
     
  if (toyCheck) {
     int nIterMC=4;
     int nExp = 300;
     if (errorType==2) {
        nIterMC=50;
        nExp=300;
     }
     for (int exp =0; exp<nExp; exp++) {
        TH1D *hToy = (TH1D*)hM->Clone();
	hToy->SetName("hToy");
	if (exp%10==0) cout <<"Pseudo-experiment "<<exp<<endl;
        for (int i=1;i<=hToy->GetNbinsX();i++)
        {
           double value = gRandom->Poisson(hM->GetBinContent(i));
	   hToy->SetBinContent(i,value);
        }
        prior myPrior2(hR,hToy,0.0);
        myPrior2.unfold(hToy,1);
        bayesianUnfold myUnfolding1(hR,myPrior2.hPrior,0.0);
        myUnfolding1.unfold(hToy,nIterMC);
	
	for (int i=1;i<=hToy->GetNbinsX();i++)
	{
	   nt->Fill(i,myUnfolding1.hPrior->GetBinContent(i),myUnfolding3.hPrior->GetBinContent(i),myUnfolding.hPrior->GetBinError(i));
	}
	delete hToy;
    }
    nt->Write();
    
  }
  
  for (Int_t i= 1; i<=hU->GetNbinsX();i++)
  {
     double m = hU->GetBinContent(i);
     double mErr = hU->GetBinError(i);
     double eff = hEffNSD->GetBinContent(i);
     double sdFrac = hSDFracSelected->GetBinContent(i);
//     eff=1;
//     sdFrac=0;
//     if (i==11) sdFrac=1;

     double errScaleFactor = 1;
     if (toyCheck){
        TH1F *htemp = new TH1F("htemp","",50,-25,25);
        nt->Project("htemp","(val-val0)/err",Form("i==%i",i));
	htemp->Fit("gaus","L q m");
        errScaleFactor = htemp->GetFunction("gaus")->GetParameter(2);
	delete htemp;
     }  
     if (eff!=0) {
        hU->SetBinContent(i,m*(1-sdFrac)/eff);
	nevtScaled+=m*(1-sdFrac)/eff;
        hU->SetBinError(i,mErr*(1-sdFrac)/eff*errScaleFactor);
        
	hU2->SetBinContent(i,m*(1-sdFrac)/eff);
	nevtScaled+=m*(1-sdFrac)/eff;
        hU2->SetBinError(i,mErr*(1-sdFrac)/eff*errScaleFactor);
     }
  }


  TH1D* hReco = (TH1D*)hU->Clone();
  hReco->SetName("hReco");
  
  TH1D* hReco2 = (TH1D*)hU2->Clone();
  hReco2->SetName("hReco2");

 
  TH1D *hRepM = (TH1D*)myUnfolding.hReproduced->Clone();
  hRepM->SetName("hRepM");
  hRepM->SetLineColor(6);
  hRepM->SetXTitle("N");
  hRepM->SetYTitle("Entries");
  hRepM->Draw("Hist");
  hU->Scale(1);
  hT->Scale(1);
  hU->Draw("Hist same");
  hT->SetLineColor(4);
  hT->Draw("same");
  hM->SetLineColor(2);
  hM->Draw("Hist same");
  
  TLegend *leg = new TLegend(0.6,0.7,0.93,0.9);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry("hT","Truth x 0.01","l");
  leg->AddEntry("hU","Unfolded x 0.01","l");
  leg->AddEntry("hM","Measured ","l");
  leg->AddEntry("hRepM","Reproduced","l");
  leg->Draw();

  TCanvas *c3 = new TCanvas("c3","",CanvasSizeX,CanvasSizeY);
  c3->SetLogy();

  // Prepare UA5 plots
  TH1F* UA5(0);
  if (_etaRange==0.5) UA5 = (TH1F*)getUA5900GeV05();
  if (_etaRange==1.5) UA5 = (TH1F*)getUA5900GeV15();

  // Prepare ALICE plots
  TH1F* ALICE(0);
  if (_etaRange==1.0) ALICE = (TH1F*)getALICE7TeV10();

  // Prepare ALICE plots
  TH1F* Track(0);
  if (_etaRange==0.5) Track = (TH1F*)getTracking7TeV05();


  hReco->SetMarkerStyle(4);
  hReco->SetMarkerSize(0.5);
  double area = hReco->Integral(0,10000);
  hReco->Scale(1./area);
  hReco->Draw("");

  if (ALICE) {
     ALICE->SetLineColor(4);
     ALICE->SetMarkerColor(2);
     ALICE->SetMarkerStyle(4);
     ALICE->SetMarkerSize(0.5);
     ALICE->Draw("p same");
  }
  
  if (UA5) {
     UA5->SetLineColor(2);
     UA5->SetMarkerColor(4);
     UA5->SetMarkerStyle(4);
     //UA5->Draw("same");
  }

  if (Track) {
     Track->SetLineColor(2);
     Track->SetMarkerColor(4);
     Track->SetMarkerStyle(4);
     Track->Draw("same");
  }

  TCanvas *c4 = new TCanvas("c4","",CanvasSizeX,CanvasSizeY);
  TH1F *hRatioM = (TH1F*)hM->Clone();
  hRatioM->SetName("hRatioM");
  hRatioM->SetXTitle("Measured / Reproduced Measured");
  hRatioM->Divide(hRepM);
  hRatioM->Draw();

  TCanvas *c5 = new TCanvas("c5","",CanvasSizeX,CanvasSizeY);
  TH1F *hRatioU = (TH1F*)hU->Clone();
  hRatioU->Divide(hT);
  hRatioU->SetXTitle("Unfolded / Truth");
  hRatioU->Draw();

  TCanvas *c6 = new TCanvas("c6","Prior study",CanvasSizeX,CanvasSizeY);
  TH1F *hRatioP = (TH1F*)myPrior.hPrior->Clone();
  hRatioP->Divide(myUnfolding.hPrior);
  hRatioP->SetXTitle("Prior / unfolding");
  hRatioP->Draw();
  
  
  outf->Write();
//  outf->Close();
}
