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

// Constants
#define _etaRange 1.0
#define CanvasSizeX 400
#define CanvasSizeY 400


void unfoldTracklet(int trackletType = 12,
                    char *infName    = "TrackletTree-D6T-1.root",  // input data
                    char *useCorrectionFile = "",
		    char *infMCName  = "TrackletTree-D6T-1.root",  // input MC
		    )
{
  // Input files
  TFile *inf = new TFile(infName);
  TTree *t = (TTree*) inf->FindObjectAny(Form("TrackletTree%d",trackletType));
  
  TFile *infMC = new TFile(infMCName);
  TTree *tMC = (TTree*) infMC->FindObjectAny(Form("TrackletTree%d",trackletType));

  // Output files
  TFile *outf = new TFile("output.root","recreate");
  
  int nTrackletMin = -10;
  int nTrackletMax = 100;
  int nBin = 110;
  
  char *varM = Form("(Sum$(abs(eta1)<%f&&abs(deta)<0.1&&abs(dphi)<1))-(Sum$(abs(eta1)<%f&&abs(deta)<0.1&&abs(dphi)>1&&abs(dphi)<2))",_etaRange,_etaRange);
//  char *varM =  Form("(Sum$(abs(eta1)<%f&&abs(dR)<0.1))-(Sum$(abs(eta1)<%f&&abs(dR)>0.1&&abs(dR)<0.2))",_etaRange,_etaRange);
  char *varT = Form("Sum$(abs(eta)<%f)",_etaRange);
  
  // Histograms
  TH1D *hM = new TH1D("hM","",nBin,nTrackletMin,nTrackletMax);
  TH1D *hT = new TH1D("hT","",nBin,nTrackletMin,nTrackletMax);
  TH2D *hR = new TH2D("hR","",nBin,nTrackletMin,nTrackletMax,nBin,nTrackletMin,nTrackletMax);

  TH1D *hNSD = new TH1D("hNSD","",nBin,nTrackletMin,nTrackletMax);
  TH1D *hSelectedNSD = new TH1D("hSelectedNSD","",nBin,nTrackletMin,nTrackletMax);
  TH1D *hSelectedSD = new TH1D("hSelectedSD","",   nBin,nTrackletMin,nTrackletMax);
  TH1D *hTrue= new TH1D ("hTrue", "Test Truth",    nBin,nTrackletMin,nTrackletMax);
  TH1D *hMeas= new TH1D ("hMeas", "Test Measured", nBin,nTrackletMin,nTrackletMax);
    
  // Selection Criteria
  selectionCut MCCut(1);
  selectionCut dataCut(1);

  TCut NSDCut = "(evtType!=92&&evtType!=93)";

  // prepare histograms
  if (useCorrectionFile=="") {  
     tMC->Draw(Form("%s>>hSelectedNSD",varM),MCCut.Cut&&NSDCut);    
     tMC->Draw(Form("%s>>hNSD",varM),NSDCut);
     tMC->Draw(Form("%s>>hSelectedSD",varM),MCCut.Cut&&(!NSDCut));

     hNSD->Sumw2();
     hSelectedNSD->Sumw2();
     hSelectedSD->Sumw2();

     TH1D *hEffNSD = (TH1D*) hSelectedNSD->Clone();  // NSD efficiency
     hEffNSD->SetName("hEffNSD");
     hEffNSD->Divide(hNSD);
  
     TH1D *hSelected = (TH1D*) hSelectedNSD->Clone();  
     hSelected->SetName("hSelected");  
     hSelected->Add(hSelectedSD);  // selected INEL
  
     TH1D *hSDFracSelected = (TH1D*) hSelectedSD->Clone();
     hSDFracSelected->SetName("hSDFracSelected");
     hSDFracSelected->Divide(hSelected);  // SD Fraction in selected INEL

     tMC->Draw(Form("%s:%s>>hR",varM,varT));
 //  tMC->Draw(Form("%s:%s>>hR",varM,varT),Form("%s!=0",varM));
  } else {
     cout <<"Use Correction file."<<endl;     
  
  }
     t->Draw(Form("%s>>hM",varM),dataCut.Cut);
     t->Draw(Form("%s>>hT",varT),NSDCut);
    //  tMC->Draw(Form("%s:%s>>hR",varM,varT),MCCut.Cut);
 
     hM->Sumw2();  
  double nevtScaled = 0;
  for (Int_t i= 1; i<=hM->GetNbinsX();i++)
  {
     double m = hM->GetBinContent(i);
     double mErr = hM->GetBinError(i);
     double eff = hEffNSD->GetBinContent(i);
     double sdFrac = hSDFracSelected->GetBinContent(i);
     
//     eff=1;
     sdFrac=0;
     
     if (eff!=0) {
        hM->SetBinContent(i,m*(1-sdFrac)/eff);
	nevtScaled+=m*(1-sdFrac)/eff;
        hM->SetBinError(i,mErr*(1-sdFrac)/eff);
     }
  }

  
  bayesianUnfold myUnfolding(hR,hT,0.0);
  
  myUnfolding.unfold(hM,5);
  
  TCanvas *c = new TCanvas("c","",CanvasSizeX,CanvasSizeY);

  myUnfolding.hResMatrix->Draw("col");
  
  TCanvas *c2 = new TCanvas("c2","",CanvasSizeX,CanvasSizeY);
//  c2->SetLogy();
  c2->SetLogy();
  TH1D *hU = (TH1D*)myUnfolding.hPrior->Clone();
  hU->SetName("hU");
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
  hReco->SetMarkerStyle(4);
  hReco->SetMarkerSize(0.5);
  hReco->Scale(1./nevtScaled);
  hReco->Scale(1./(1.-hReco->Integral(0,11)));
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
     UA5->Draw("same");
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
  
  outf->Write();
//  outf->Close();
}
