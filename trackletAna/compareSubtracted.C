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
void formatHist(TH1* h, int col = 1, double norm = 1,char* funName="");
void compareSubtracted(char *hist,char *funName="")
{
   TFile *f900GeV= new TFile("correction-900GeV.root");
   TH2F *h2D1 = (TH2F*) f900GeV->FindObjectAny(hist);
   TH1D *h1 = h2D1->ProjectionX();
   TH2F *h2DHadron1 = (TH2F*) f900GeV->FindObjectAny("hHadron");
   TH1D *h3 = h2DHadron1->ProjectionX();
   h1->SetName("a");
   h3->SetName("h3");
   TFile *f10TeV= new TFile("correction-10TeV.root");
   TH2F *h2D2 = (TH2F*) f10TeV->FindObjectAny(hist);
   TH2F *h2DHadron2 = (TH2F*) f10TeV->FindObjectAny("hHadron");
   TH1D *h2 = h2D2->ProjectionX();
   TH1D *h4 = h2DHadron2->ProjectionX();
   h2->SetName("b");
   h4->SetName("h4");

   char *fun = funName;
   cout <<fun<<endl;

   TCanvas *c = new TCanvas ("c","",400,400);

   formatHist(h2,2,1,funName);
//   formatHist(h2,4,1,funName);

   h4->Divide(h3);
   h4->Draw("hist");
   h4->SetYTitle("dN/d#eta Ratio (10TeV / 900 GeV)"); 
   h2->Divide(h1);
   h2->Divide(h4);
   h2->Draw("e");
   
   
}

void formatHist(TH1* h, int col, double norm,char *funName){

  h->Scale(1/norm);
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->SetMarkerSize(0.7);
  h->GetYaxis()->SetTitleOffset(1.15);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();
  if (h->GetFunction(funName)!=0) {
     h->GetFunction(funName)->SetLineColor(col);
     h->GetFunction(funName)->SetLineWidth(2);
  }
}
