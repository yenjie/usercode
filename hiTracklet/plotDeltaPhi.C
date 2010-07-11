#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>

void plotDeltaPhi(char* infname,char *infnameBck,int trackletType=12)
{
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->FindObjectAny(Form("TrackletTree%d",trackletType));
   TFile *infBck = new TFile(infnameBck);
   TTree *tBck = (TTree*)infBck->FindObjectAny(Form("TrackletTree%d",trackletType));
   TH1D* hDphi = new TH1D("hDphi","",400,-3.2,3.2);
   hDphi->SetXTitle("#Delta#Phi");
   hDphi->SetYTitle("Arbitrary Unit");
   t->Draw("dphi>>hDphi","weight");
   int nev = t->GetEntries();
   hDphi->Scale(1./nev);
   
   TH1D* hDphiBck = new TH1D("hDphiBck","",400,-3.2,3.2);
   hDphiBck->SetXTitle("#Delta#Phi");
   hDphiBck->SetYTitle("Arbitrary Unit");
   tBck->Draw("dphi>>hDphiBck","weight");
   double int1 = hDphi->Integral(0,100)+hDphi->Integral(300,400);
   double int2 = hDphiBck->Integral(0,100)+hDphiBck->Integral(300,400);
   hDphiBck->Scale(int1/int2);
   
  
   TCanvas *c = new TCanvas("c","",600,600);
   hDphi->Draw();
   hDphiBck->SetLineColor(2);
   hDphiBck->Draw("same");
   
   TH1D* hDphiSubtracted = (TH1D*) hDphi->Clone();
   hDphiSubtracted->SetName("hDphiSubtracted");
   hDphiSubtracted->SetLineColor(4);
   hDphiSubtracted->Add(hDphiBck,-1);
   hDphiSubtracted->Draw("same");
}
