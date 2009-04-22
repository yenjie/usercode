#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <iostream.h>
#include <TLine.h>
#include <math.h>
#include <TF1.h>
void formatHist(TH1* h, int col = 1, double norm = 1);
void plotDphi(char* filename,char *toyMC="pseudoExp.root")
{
   TFile *f= new TFile(filename);
   
   TNtuple * ntmatched= dynamic_cast<TNtuple *>(f->Get("ntmatched"));
   TFile *fmc= new TFile(toyMC);
   TNtuple * ntmatchedMC= dynamic_cast<TNtuple *>(fmc->Get("ntmatched"));

   TH1F *h1 = new TH1F("h1","",200,-3.2,3.2);
   TH1F *h2 = new TH1F("h2","",200,-3.2,3.2);

   TCanvas *c = new TCanvas ("c","",400,400);
   ntmatched->Draw("dphi>>h1","");

   h1->SetXTitle("#Delta#phi");
   h1->Draw("");

   TCanvas *c2 = new TCanvas ("c2","",400,400);
   ntmatchedMC->Draw("dphi>>h2","");

   h2->SetXTitle("#Delta#phi (ToyMC)");
   h2->Draw("");
}

void formatHist(TH1* h, int col, double norm){

  h->Scale(1/norm);
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->SetMarkerSize(0.7);
  h->GetYaxis()->SetTitleOffset(1.15);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

}
