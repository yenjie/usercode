#include <TFile.h>
#include <TNtuple.h>
#include <TProfile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>

void formatProf(TH1F* h, int col, double norm=1);

void plotHit(int twoEnergy=1){
   TFile *a = new TFile("../sample/10TeV/pixel-1.root");
   TNtuple *ta = (TNtuple*) a->FindObjectAny("PixelTree");
   TFile *b = new TFile("../sample/900GeV/pixel-1.root");   
   TNtuple *tb = (TNtuple*) b->FindObjectAny("PixelTree");

   TH1F *h1a = new TH1F("h1a","",75,0,150);
   TH1F *h1b = new TH1F("h1b","",75,0,150);
   TH1F *h1c = new TH1F("h1c","",75,0,150);

   TH1F *h2a = new TH1F("h2a","",75,0,150);
   TH1F *h2b = new TH1F("h2b","",75,0,150);

   TCanvas *c = new TCanvas("c","",400,400);
   ta->Draw("nhits1>>h1a","abs(vz[0])<10&&nhits1>0");
//   ta->Draw("nhits1>>h1b","abs(vz[0])<10&&nhits1>0&&abs(vz[3])<10");
   tb->Draw("nhits1>>h2a","abs(vz[0])<10&&nhits1>0");
//   tb->Draw("nhits1>>h2b","abs(vz[0])<10&&nhits1>0&&abs(vz[3])<10");
   
   int norm = h1a->GetEntries();
   int norm2 = h2a->GetEntries();
   formatProf(h1a,1,norm);
   formatProf(h1b,1,norm);
   formatProf(h2a,2,norm2);
   formatProf(h2b,2,norm2);
   h2a->SetXTitle("Number of First Layer Hits");
   h2a->SetYTitle("Event Fraction");
   
   h1b->SetMarkerStyle(4);
//   h1a->Draw("c hist");
   h2a->Draw("hist");
//   h1b->Draw("c hist same");
//   h2b->Draw("hist same");
   h2b->SetMarkerStyle(4);
//   h2a->Draw("c hist same");
   if (twoEnergy) h1a->Draw("hist same");
//   h2b->Draw("c hist same");
//   if (twoEnergy) h1b->Draw("hist same");  
   
   TLegend * leg2 = new TLegend(0.45,0.54,0.95,0.77);
   leg2->SetFillStyle(0);  
   leg2->SetFillColor(0); 
   leg2->SetBorderSize(0);
   leg2->SetTextSize(0.03);
//   leg2->AddEntry(h1b,"Pixel Triplet Vertexing (10TeV)","l");
//   if (twoEnergy) leg2->AddEntry(h2b,"Pixel Triplet Vertexing (900GeV)","l");
   leg2->AddEntry(h1a,"PYTHIA 10 TeV","l");
   if (twoEnergy) leg2->AddEntry(h2a,"PYTHIA 900GeV","l");

   leg2->Draw();

   c->SaveAs("plot/FigHitSpectrum.C");
   c->SaveAs("plot/FigHitSpectrum.eps");
   c->SaveAs("plot/FigHitSpectrum.gif");
    
}

void formatProf(TH1F* h, int col, double norm){
  h->Sumw2();
  h->Scale(1/norm);
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->SetMarkerSize(0.7);
  h->GetYaxis()->SetTitleOffset(1.25);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();
}
