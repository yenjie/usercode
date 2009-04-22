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
void plotDNdEta(char* filename)
{
   TFile *f= new TFile(filename);
   
   TTree * PixelTree= dynamic_cast<TTree *>(f->Get("ana/PixelTree"));

   TH1F *h1 = new TH1F("h1","",200,-10,10);
   TH1F *h2 = new TH1F("h2","",200,-10,10);

   TCanvas *c = new TCanvas ("c","",400,400);
   PixelTree->Draw("eta>>h1","abs(pdg)==211||abs(pdg)==2212||abs(pdg)==321||abs(pdg)==3122");
//   PixelTree->Draw("eta>>h1","abs(pdg)==11||abs(pdg)==13||abs(pdg)==15");
   int nevent = PixelTree->GetEntries("mult");

   h1->Scale(1./nevent*10);
   h1->SetXTitle("#eta");
   h1->SetYTitle("dN/d#eta");
   h1->Draw("");

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
