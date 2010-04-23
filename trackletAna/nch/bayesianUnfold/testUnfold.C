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

Double_t smear (Double_t xt)
{
  Double_t xeff= 0.3 + (1.0-0.3)/20*(xt+10.0);  // efficiency
  Double_t x= gRandom->Rndm();
//  if (x>xeff) return -10;
  Double_t xsmear= gRandom->Gaus(-2.5,2);     // bias and smear
  return xt+xsmear;
}



void testUnfold()
{
    TH1D *hM = new TH1D("hM","",200,-50,50);
    TH1D *hT = new TH1D("hT","",200,-50,50);
    TH2D *hR = new TH2D("hR","",200,-50,50,200,-50,50);
    
  // Train with a Breit-Wigner, mean 0.3 and width 2.5.
  for (Int_t i= 0; i<1000000; i++) {
//    Double_t xt= gRandom->Gaus (0,5);
    Double_t xt= gRandom->Landau (0.3, 2.5);
    Double_t x= smear (xt);
    hR->Fill(xt,x);
  }

  for (Int_t i=0; i<1000000; i++) {
    Double_t xt= gRandom->Gaus (0.0, 2.0), x= smear (xt);
    hT->Fill(xt);
    hM->Fill(x);
  }
  
  bayesianUnfold myUnfolding(hR,hT,0);
  
  myUnfolding.unfold(hM,5);
  
  TCanvas *c = new TCanvas("c","",600,600);

  myUnfolding.hResMatrix->Draw("col");
  
  TCanvas *c2 = new TCanvas("c2","",600,600);
//  c2->SetLogy();
  
  TH1D *hU = (TH1D*)myUnfolding.hPrior->Clone();
  hU->SetName("hU");
  TH1D *hRepM = (TH1D*)myUnfolding.hReproduced->Clone();
  hRepM->SetName("hRepM");
  hRepM->SetLineColor(6);
  hRepM->Draw();
  hU->Draw("same");
  hT->SetLineColor(4);
  hT->Draw("same");
  hM->SetLineColor(2);
  hM->Draw("same");
  
  TLegend *leg = new TLegend(0.68,0.7,0.93,0.9);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry("hT","Truth","l");
  leg->AddEntry("hM","Measured","l");
  leg->AddEntry("hU","Unfolded","l");
  leg->AddEntry("hRepM","Reproduced","l");
  leg->Draw();
}
