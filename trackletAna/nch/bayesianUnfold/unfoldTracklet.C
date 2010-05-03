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

// Selection Criteria
#include "selectionCut.h"

// UA5 plot
#include "UA5.h"

// ALICE plot
#include "ALICE.h"

// Track plot
#include "Nick.h"

// Constants
#define CanvasSizeX 400
#define CanvasSizeY 400


int unfoldTracklet(int trackletType = 12, double _etaRange=2.5,
                    char *infName    = "TrackletTree-D6T-1.root",  // input data
		    char *infMCName  = "TrackletTree-D6T-1.root",  // input MC
                    char *useCorrectionFile = ""
		    )
{
  // Input files
  TFile *inf = new TFile(infName);
  TTree *t = (TTree*) inf->FindObjectAny(Form("TrackletTree%d",trackletType));
  
  TFile *infMC = new TFile(infMCName);
  TTree *tMC = (TTree*) infMC->FindObjectAny(Form("TrackletTree%d",trackletType));

  TFile *infMC2 = new TFile("TrackletTree-ATLAS-new.root");
  TTree *tMC2 = (TTree*) infMC2->FindObjectAny(Form("TrackletTree%d",trackletType));


  // Output files
  TFile *outf = new TFile(Form("result/result-%d-%.1f-%s.root",trackletType,_etaRange,infName),"recreate");
  char *correctionFileName = Form("correction/correction-%d-%.1f-%s.root",trackletType,_etaRange,useCorrectionFile);
  TFile *fCorrection = new TFile(correctionFileName);
  
  int nTrackletMin = -10;
  int nTrackletMax = 190;
  int nBin = 200;
  
  // Multiplicity handle
  char *varM = Form("(Sum$(abs(eta1)<%f&&abs(deta)<0.1&&abs(dphi)<1))-(Sum$(abs(eta1)<%f&&abs(deta)<0.1&&abs(dphi)>1&&abs(dphi)<2))",_etaRange,_etaRange);
  //char *varM =  Form("(Sum$(abs(eta1)<%f&&abs(dR)<0.1))-(Sum$(abs(eta1)<%f&&abs(dR)>0.1&&abs(dR)<0.2))",_etaRange,_etaRange);

  // Truth
  char *varT = Form("Sum$(abs(eta)<%f&&pdg!=11&&pdg!=13)",_etaRange);
  
  // Histograms
  TH1D *hM = new TH1D("hM","",nBin,nTrackletMin,nTrackletMax);
  TH1D *hT = new TH1D("hT","",nBin,nTrackletMin,nTrackletMax);
  TH2D *hR(0);
  TH1D *hMMC(0);
  
  TH1D *hNSD = new TH1D("hNSD","",nBin,nTrackletMin,nTrackletMax);
  TH1D *hSelectedNSD = new TH1D("hSelectedNSD","",nBin,nTrackletMin,nTrackletMax);
  TH1D *hSelectedSD = new TH1D("hSelectedSD","",   nBin,nTrackletMin,nTrackletMax);
  TH1D *hTrue= new TH1D ("hTrue", "Test Truth",    nBin,nTrackletMin,nTrackletMax);
  TH1D *hMeas= new TH1D ("hMeas", "Test Measured", nBin,nTrackletMin,nTrackletMax);
    
  // Selection Criteria
  selectionCut MCCut(1);
  selectionCut dataCut(1);

  TCut nonZeroCut = "";
  TCut NSDCut = "(evtType!=92&&evtType!=93)";

  TH1D *hEffNSD(0); 
  TH1D *hSDFracSelected(0); 

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
     hR = new TH2D("hR","",nBin,nTrackletMin,nTrackletMax,nBin,nTrackletMin,nTrackletMax);
     hMMC = new TH1D("hMMC","",nBin,nTrackletMin,nTrackletMax);
  
     tMC->Draw(Form("%s:%s>>hR",varM,varT),MCCut.Cut&&nonZeroCut);
     tMC->Draw(Form("%s>>hMMC",varM),MCCut.Cut&&nonZeroCut);

  } else {
     cout << correctionFileName <<endl;
     hEffNSD = (TH1D*) fCorrection->FindObjectAny("hEffNSD");
     hSDFracSelected = (TH1D*) fCorrection->FindObjectAny("hSDFracSelected");
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
  hR->Draw(); 
  hM->Sumw2();  
  hR->Sumw2();  
  

  double nevtScaled = 0;


  // rescale response function
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
    
  bayesianUnfold myUnfolding(hR,hT,0.0);
  myUnfolding.unfold(hM,4);

  
  TCanvas *c = new TCanvas("c","",CanvasSizeX,CanvasSizeY);

  myUnfolding.hResMatrix->Draw("col");
  
  TCanvas *c2 = new TCanvas("c2","",CanvasSizeX,CanvasSizeY);
  c2->SetLogy();
  TH1D *hU = (TH1D*)myUnfolding.hPrior->Clone();
  hU->SetName("hU");

  for (Int_t i= 1; i<=hU->GetNbinsX();i++)
  {
     double m = hU->GetBinContent(i);
     double mErr = hU->GetBinError(i);
     double eff = hEffNSD->GetBinContent(i);
     double sdFrac = hSDFracSelected->GetBinContent(i);
//     eff=1;
//     sdFrac=0;
     
     if (eff!=0) {
        hU->SetBinContent(i,m*(1-sdFrac)/eff);
	nevtScaled+=m*(1-sdFrac)/eff;
        hU->SetBinError(i,mErr*(1-sdFrac)/eff);
     }
  }


  TH1D* hReco = (TH1D*)hU->Clone();
  hReco->SetName("hReco");
 
  TH1D *hRepM = (TH1D*)myUnfolding.hReproduced->Clone();
  hRepM->SetName("hRepM");
  hRepM->SetLineColor(6);
  hRepM->Draw("Hist");
  hU->Scale(0.01);
  hT->Scale(0.01);
  hU->Draw("Hist same");
  hT->SetLineColor(4);
  hT->Draw("same");
  hM->SetLineColor(2);
  hM->Draw("Hist same");
  
  TLegend *leg = new TLegend(0.68,0.7,0.93,0.9);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry("hT","Truth","l");
  leg->AddEntry("hM","Measured","l");
  leg->AddEntry("hU","Unfolded","l");
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
  hReco->Scale(1./nevtScaled);
//  hReco->Scale(1./(1.-hReco->Integral(0,11)));
//  hReco->SetBinContent(11,0);
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
  
//  outf->Write();
//  outf->Close();
}
