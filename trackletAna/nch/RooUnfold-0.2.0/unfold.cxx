//=====================================================================-*-C++-*-
// File and Version Information:
//      $Id: RooUnfoldExample.cxx,v 1.4 2010/01/21 01:23:57 adye Exp $
//
// Description:
//      Simple example usage of the RooUnfold package using toy MC.
//
// Authors: Tim Adye <T.J.Adye@rl.ac.uk> and Fergus Wilson <fwilson@slac.stanford.edu>
//
//==============================================================================

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
using std::cout;
using std::endl;
#include "TRandom.h"
#include "TH1D.h"
#include "TLegend.h"
#include <TFile.h>
#include <TTree.h>
#include <TEntryList.h>
#include <TCanvas.h>

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
//#include "RooUnfoldSvd.h"
//#include "RooUnfoldBinByBin.h"
#endif

#include "UA5.h"
#include "selectionCut.h"
#include <math.h>
using namespace std;

#define _etaRange 1.5

//==============================================================================
// Global definitions
//==============================================================================

class para {
   public:
     Int_t           npart;
     Int_t           nTracklets;
     Int_t           mult;
     Int_t           evtType;
     Float_t         vz[8];   //[nv]
     Float_t         eta[500];   //[npart]
     Float_t         eta1[50000];
     Float_t         dphi[50000];
     Float_t         deta[50000];

};

void setupBranch(TTree* t,para& p1)
{
  t->SetBranchAddress("npart", &p1.npart );
  t->SetBranchAddress("nTracklets", &p1.nTracklets );
  t->SetBranchAddress("mult", &p1.mult );
  t->SetBranchAddress("vz", p1.vz );
  t->SetBranchAddress("evtType", &p1.evtType );
  t->SetBranchAddress("eta", p1.eta );
  t->SetBranchAddress("deta", p1.deta );
  t->SetBranchAddress("dphi", p1.dphi );
  t->SetBranchAddress("eta1", p1.eta1 );
}


const Double_t cutdummy= -99999.0;

//==============================================================================
// Gaussian smearing, systematic translation, and variable inefficiency
//==============================================================================

Double_t smear (Double_t xt)
{
  Double_t xeff= 0.3 + (1.0-0.3)/20*(xt+10.0);  // efficiency
  Double_t x= gRandom->Rndm();
  if (x>xeff) return cutdummy;
  Double_t xsmear= gRandom->Gaus(-2.5,0.2);     // bias and smear
  return xt+xsmear;
}

//==============================================================================
// Example Unfolding
//==============================================================================

void unfold()
{
#ifdef __CINT__
  gSystem->Load("libRooUnfold");
#endif
  
  cout << "==================================== TRAIN ====================================" << endl;
  RooUnfoldResponse response (160, -10.0, 150.0);

//  TFile *inf = new TFile("TrackletTree-D6T-2360TeV-all.root");
  TFile *inf = new TFile("TrackletTree-900GeV-D6T.root");
//    TFile *inf = new TFile("TrackletTree-ATLAS-new.root");
//    TFile *inf = new TFile("TrackletTree-D6T-1.root");
    TTree *t = (TTree*)inf->FindObjectAny("TrackletTree13");

//  TFile *inf2 = new TFile("TrackletTree-900GeV-D6T.root");
  
  TFile *inf2 = new TFile("TrackletTree-Run124023-hfcuts.root");
//  TFile *inf2 = new TFile("TrackletTree-Run132440-hfcuts.root");
//  TFile *inf2 = new TFile("TrackletTree-D6T-2360TeV-all.root");
  TTree *t2 = (TTree*)inf2->FindObjectAny("TrackletTree13");
  
  
  TFile *inf3 = new TFile("TrackletTree-900GeV-D6T.root");
  //TFile *inf3 = new TFile("TrackletTree-D6T-7TeV.root");
  TTree *t3 = (TTree*)inf3->FindObjectAny("TrackletTree13");
  

  TFile *inf4 = new TFile("TrackletTree-900GeV-ATLAS-new.root");
  TTree *t4 = (TTree*)inf4->FindObjectAny("TrackletTree13");
  
  TFile *outfile = new TFile("output.root","recreate");

  TH1F *hMC = new TH1F("hMC","",160,-10,150);
  t3->Draw(Form("Sum$(abs(eta)<%f)>>hMC",_etaRange),Form("evtType!=92&&evtType!=93&&Sum$(abs(eta)<%f)!=0",_etaRange));
  cout <<hMC->GetEntries()<<endl;

  TH1F *hMCATLAS = new TH1F("hMCATLAS","",160,-10,150);
  t4->Draw(Form("Sum$(abs(eta)<%f)>>hMCATLAS",_etaRange),Form("evtType!=92&&evtType!=93&&Sum$(abs(eta)<%f)!=0",_etaRange));
  cout <<hMCATLAS->GetEntries()<<endl;
  hMC->Sumw2();
  hMCATLAS->Sumw2();  
  // Set up branchs
  para p1;
  para p2;

  setupBranch(t,p1);
  setupBranch(t2,p2);
  
  char *var = Form("Sum$(abs(eta)<%d)",_etaRange);

  // Set up selection cut
  selectionCut myCutMC(1);
  selectionCut myCutData(1);

  TCut NSDCut = "(evtType!=92&&evtType!=93)";

  // prepare histograms
  TH1D *hNSD = new TH1D("hNSD","",160,-10.0,150.0);
  TH1D *hSelectedNSD = new TH1D("hSelectedNSD","",160,-10.0,150.0);
  TH1D *hSelectedSD = new TH1D("hSelectedSD","",160,-10.0,150.0);
  TH1D *hTrue= new TH1D ("hTrue", "Test Truth",    160, -10.0, 150.0);
  TH1D *hMeas= new TH1D ("hMeas", "Test Measured", 160, -10.0, 150.0);

  
  t->Draw(Form("%s>>hSelectedNSD",var),myCutMC.Cut&&NSDCut);
  t->Draw(Form("%s>>hNSD",var),NSDCut);
  t->Draw(Form("%s>>hSelectedSD",var),myCutMC.Cut&&(!NSDCut));
  
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
  
  
  // Prepare the selectedList for MC and data
  TEntryList *selectedList = new TEntryList("selectedList","");
  t->Draw(">>selectedList",myCutMC.Cut,"entrylist");
  //t->Draw(">>selectedList",NSDCut,"entrylist");
  TEntryList *selectedListData = new TEntryList("selectedListData","");
  t2->Draw(">>selectedListData",myCutData.Cut,"entrylist");
  
  // MC INEL events which passed the selection cut
  // Produce response matrix
  for (Int_t i= 0; i<t->GetEntries(); i++) {
    if (!selectedList->Contains(i)) continue;
    t->GetEntry(i);
    Int_t nch = 0;
    Int_t nTracklet = 0;

    //cout <<p1.npart<<endl;
    if (i % 10000 == 0) cout <<i<<" / "<<t->GetEntries()<<endl;

    for (Int_t j= 0; j<p1.npart; j++) {
       if (fabs(p1.eta[j])<_etaRange) nch++;
    }
    for (Int_t j= 0; j<p1.nTracklets; j++) {
       if (fabs(p1.eta1[j])<_etaRange&&fabs(p1.deta[j])<0.1&&fabs(p1.dphi[j])<1) nTracklet++;
       if (fabs(p1.eta1[j])<_etaRange&&fabs(p1.deta[j])<0.1&&fabs(p1.dphi[j])>1&&fabs(p1.dphi[j])<2) nTracklet--;
    }
    response.Fill (nTracklet,nch);
  }



  cout << "==================================== TEST =====================================" << endl;

  // Data INEL events which passed the selection cut
  for (Int_t i= 0; i<t2->GetEntries(); i++) {
    t2->GetEntry(i);
    Int_t nch = 0;
    Int_t nTracklet = 0;

    for (Int_t j= 0; j<p2.npart; j++) {
       if (fabs(p2.eta[j])<_etaRange) nch++;
    }
    if (!selectedListData->Contains(i)) continue;

    for (Int_t j= 0; j<p2.nTracklets; j++) {
       if (fabs(p2.eta1[j])<_etaRange&&fabs(p2.deta[j])<0.1&&fabs(p2.dphi[j])<1) nTracklet++;
       if (fabs(p2.eta1[j])<_etaRange&&fabs(p2.deta[j])<0.1&&fabs(p2.dphi[j])>1&&fabs(p2.dphi[j])<2) nTracklet--;
    }

    hTrue->Fill(nch);
    hMeas->Fill(nTracklet);
  }

  double nevtScaled = 0;
  TH1D *hMeasRaw = (TH1D*)hMeas->Clone();
  hMeasRaw->SetName("hMeasRaw");

  
  for (Int_t i= 1; i<=hMeas->GetNbinsX();i++)
  {
     double m = hMeas->GetBinContent(i);
     double mErr = hMeas->GetBinError(i);
     double eff = hEffNSD->GetBinContent(i);
     double sdFrac = hSDFracSelected->GetBinContent(i);
     
     eff=1;
     sdFrac=0;
     
     if (eff!=0) {
        hMeas->SetBinContent(i,m*(1-sdFrac)/eff);
	nevtScaled+=m*(1-sdFrac)/eff;
        hMeas->SetBinError(i,mErr*(1-sdFrac)/eff);
     }
  }

  cout << "==================================== UNFOLD ===================================" << endl;
  RooUnfoldBayes    unfold (&response, hMeas, 4);    // OR
//RooUnfoldSvd      unfold (&response, hMeas, 20);   // OR
//RooUnfoldBinByBin unfold (&response, hMeas);
//  hMeas->Sumw2();
//  hTrue->Sumw2();
  
  
  TH1D* hReco= (TH1D*) unfold.Hreco();
  hReco->SetName("hReco");
  unfold.PrintTable (cout, hTrue);

  double nevtTrue = hTrue->GetEntries();
  hTrue->Scale(1./nevtTrue);
  hMeas->Scale(1./nevtTrue);

/*
  for (Int_t i= 1; i<=hReco->GetNbinsX();i++)
  {
     double m = hReco->GetBinContent(i);
     double mErr = hReco->GetBinError(i);
     double eff = hEffNSD->GetBinContent(i);
     double sdFrac = hSDFracSelected->GetBinContent(i);
     
     
     if (eff!=0) {
        hReco->SetBinContent(i,m*(1-sdFrac)/eff);
	nevtScaled+=m*(1-sdFrac)/eff;
        hReco->SetBinError(i,mErr*(1-sdFrac)/eff);
     }
  }
*/
  hReco->Scale(1./nevtScaled);
  hReco->SetXTitle(Form("N_{ch} |#eta|<%.1f",_etaRange));
  hReco->SetYTitle("Event Fraction");
  hReco->SetTitle(""); 
  TCanvas *c = new TCanvas("c","",600,600);  
  hReco->SetAxisRange(0,150,"X");
  hReco->Draw();
  
  hTrue->SetLineColor(4);
//  hTrue->Draw("SAME");
  
//  hMeas->Draw("SAME");
  
  TH1F *h = getUA5900GeV15();
  h->SetMarkerStyle(4);
  h->SetMarkerColor(2);
  h->SetLineColor(2);
  h->Draw("same");
  hMC->SetMarkerStyle(4);
  hMC->SetMarkerColor(4);
  hMC->SetLineColor(4);
  hMCATLAS->SetMarkerStyle(4);
  hMCATLAS->SetMarkerColor(1);  
  int nEvtMC = hMC->GetEntries();
  int nEvtMCATLAS = hMCATLAS->GetEntries();
  cout <<nEvtMC<<endl;
  hMC->Scale(1./nEvtMC);
  hMC->Draw("same");
  hMCATLAS->Scale(1./nEvtMCATLAS);
  hMCATLAS->Draw("same");
  outfile->Write();
  
  TLegend * leg1 = new TLegend(0.30,0.18,1,0.3);
  leg1->SetFillStyle(0);  
  leg1->SetFillColor(0); 
  leg1->SetBorderSize(0);
  leg1->AddEntry(h,"UA5 900GeV |#eta|<1.5","pl");
  leg1->AddEntry(hReco,"Reconstructed","pl");
  leg1->AddEntry(hMC,"MC D6T 900GeV","pl");
  leg1->AddEntry(hMCATLAS,"MC ATLAS 900GeV","pl");

  leg1->Draw();

  
}

#ifndef __CINT__
int main () { unfold(); return 0; }  // Main program when run stand-alone
#endif
