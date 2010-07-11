#include <iostream>
#include <TF1.h>
#include <TH1D.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>

void plotVz(char* infname)
{
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->FindObjectAny("PixelTree");
   TH1D *h = new TH1D("h","",90,-20,25);
   t->Draw("vz[1]>>h");
   TCanvas *c = new TCanvas("c","",600,600);
   h->Fit("gaus");
   h->SetXTitle("V_{z} (cm)");
   h->SetYTitle("Arbitrary Unit");

}
