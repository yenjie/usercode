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
void compare(char *hist,char *funName="", char *firstFile="correction-900GeV.root", char *secondFile="correction-10TeV.root")
{
   TFile *f900GeV= new TFile(firstFile);
   TH1F *h1 = (TH1F*) f900GeV->FindObjectAny(hist);
   TFile *f10TeV= new TFile(secondFile);
   TH1F *h2 = (TH1F*) f10TeV->FindObjectAny(hist);
   
   char *fun = funName;
   cout <<fun<<endl;

   TCanvas *c = new TCanvas ("c","",400,400);

   formatHist(h1,2,1,funName);
   formatHist(h2,4,1,funName);

   h1->SetAxisRange(0,30,"x");   
   h1->Draw();
   h2->Draw("same");
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
