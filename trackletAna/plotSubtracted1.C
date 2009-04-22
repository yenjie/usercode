#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <TTree.h>
#include <iostream.h>
#include <TLine.h>
#include <math.h>
#include <TF1.h>
#include <TCut.h>


void formatHist(TH1* h, int col = 1, double norm = 1);

TH1D* plotSubtracted(char* filename,char *toyMC="pseudoExp.root",char *mycut=
"",bool useMC = false);

void plotSubtracted1()
{
   TH1D *a = plotSubtracted("tracklet-900GeV-all-20081026.root");
   a->SetName("a");
   TH1D *b = plotSubtracted("tracklet_900GeV_misAlign10pb.root");
   b->SetName("b");
   TH1D *c = plotSubtracted("tracklet_misAlign_900GeV_d20081104.root");
   c->SetName("c");
   
   formatHist(a,1,a->GetEntries());
   formatHist(b,2,b->GetEntries());
   formatHist(c,4,c->GetEntries());
      
   a->Draw();
   b->Draw("same");
   c->Draw("same");
    TLegend * leg1 = new TLegend(0.25,0.66,0.56,0.85);
    leg1->SetFillStyle(0);  
    leg1->SetFillColor(0); 
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.03);
    leg1->AddEntry(a,"900 GeV Ideal","l");
    leg1->AddEntry(b,"900 GeV 10 pb^{-1}","l");
    leg1->AddEntry(c,"900 GeV Startup","l");
    leg1->Draw();
}

TH1D* plotSubtracted(char* filename,char *toyMC,char *mycut,bool useMC)
{
   TFile *f= new TFile(filename);
   
   TNtuple * ntmatched= dynamic_cast<TNtuple *>(f->Get("ntmatched"));
   TFile *fmc= new TFile(toyMC);
   TNtuple * ntmatchedMC= dynamic_cast<TNtuple *>(fmc->Get("ntmatched"));

   TH2F *h1 = new TH2F("h1","",12,-3,3,50,0,0.1);
   TH2F *h2 = new TH2F("h2","",12,-3,3,50,0,0.1);
   TH2F *h2MC = new TH2F("h2MC","",12,-3,3,50,0,0.1);
   TH2F *h3 = new TH2F("h3","",12,-3,3,50,0,0.1);
 
   TCut cut = mycut;
   TCanvas *c = new TCanvas ("c","",400,400);
   ntmatched->Draw("abs(deta):eta1>>h1","abs(dphi)<1"&&cut);

   h3->SetXTitle("#eta");
   h3->SetYTitle("|#Delta#eta|");
   if (useMC) {
     cout <<"Use ToyM"<<endl;
     ntmatchedMC->Draw("abs(deta):eta1>>h2","abs(dphi)>1&&abs(dphi)<2"&&cut);
     int n = ntmatched->GetEntries("abs(dphi)>1&&abs(dphi)<2&&abs(deta)>1"&&cut);
     int n2 = ntmatchedMC->GetEntries("abs(dphi)>1&&abs(dphi)<2&&abs(deta)>1"&&cut);
     h2->Scale(1./n2*n); 
   } else {
     ntmatched->Draw("abs(deta):eta1>>h2","abs(dphi)>1&&abs(dphi)<2"&&cut);   
   }

   h1->Sumw2();
   h2->Sumw2();
   h3->Sumw2();
   for (int x=0;x<80;x++) {
      for (int y=0;y<200;y++) {
         h3->SetBinContent(x,y, h1->GetBinContent(x,y)-h2->GetBinContent(x,y));
         h3->SetBinError(x,y, sqrt(h1->GetBinError(x,y)*h1->GetBinError(x,y)+h2->GetBinError(x,y)*h2->GetBinError(x,y)));
      }
   }
   h3->Draw("col");
   TCanvas *c2 = new TCanvas ("c2","",400,400);

   TH1D *hx1 = h1->ProjectionX();
   TH1D *hx2 = h2->ProjectionX();
   TH1D *hx3 = h3->ProjectionX();
   TH1D *hy1 = h1->ProjectionY();
   TH1D *hy2 = h2->ProjectionY();
   TH1D *hy3 = h3->ProjectionY();

   formatHist(hy1,1,1);
   formatHist(hy2,4,1);
   formatHist(hy3,2,1);
   hy1->SetXTitle("#Delta#eta");
   hy1->SetYTitle("#");
   
   hy1->Draw();
   hy2->SetMarkerColor(4);
   hy2->SetLineColor(4);
   hy2->Draw("same");
   hy3->SetMarkerColor(2);
   hy3->SetLineColor(2);
   hy3->Draw("same");
    TLegend * leg1 = new TLegend(0.25,0.66,0.56,0.85);
    leg1->SetFillStyle(0);  
    leg1->SetFillColor(0); 
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.03);
    leg1->AddEntry(hy1,"Everything","l");
    leg1->AddEntry(hy2,"Reproduced Combinatorial Background","l");
    leg1->AddEntry(hy3,"Reproduced Combinatorial Background Subtracted","l");

    leg1->Draw();

   return hy3;    
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
