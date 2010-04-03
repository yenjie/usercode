#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TTree.h>
#include <TCanvas.h>
#include "../selectionCut.h"
#include <TCut.h>

void plotClusterCharge(char *infile = "../newUltimateSample/TrackletTree-Run132440-hfcuts.root")
{
   TFile *inf = new TFile(infile);
   
   TTree *t = (TTree*) inf->FindObjectAny("TrackletTree12");
   
   TH2D *hSignal    = new TH2D("hSignal","",100,-3,3,20,0,20);
   TH2D *hSideband = new TH2D("hSideband","",100,-3,3,20,0,20);

   selectionCut myCut(0,0,10000);
   
   TCut signalRegion   = "abs(deta)<0.1&&abs(dphi)<1&&abs(dphi)<1&&nhit1>55&&nhit1<65";
   TCut sidebandRegion = "abs(deta)<0.1&&abs(dphi)>1&&abs(dphi)<2&&nhit1>55&&nhit1<65";

   TCanvas *c = new TCanvas("c","",400,400);

   t->Draw("cs1:eta1>>hSignal",myCut.Cut&&signalRegion);
   t->Draw("cs1:eta1>>hSideband",myCut.Cut&&sidebandRegion);

   hSignal->SetXTitle("#eta (Signal Region)");
   hSignal->SetYTitle("Cluster Size");
   hSideband->SetXTitle("#eta (Sideband Region)");
   hSideband->SetYTitle("Cluster Size");

   hSignal->Draw("col");

   double nEntry = hSignal->GetEntries();
   
   hSignal->Scale(1./nEntry);
   hSideband->Scale(1./nEntry);

   TCanvas *c2 = new TCanvas("c2","",400,400);
   hSideband->Draw("col");

   
   TCanvas *c3 = new TCanvas("c3","",400,400);
   
   TH2D *hSubtracted = (TH2D*)hSignal->Clone();
   hSubtracted->SetName("hSubtracted");
   hSubtracted->Add(hSideband,-1);
   hSubtracted->SetXTitle("#eta (Background subtracted)");
   hSubtracted->Draw("col"); 

   TCanvas *c4 = new TCanvas("c4","",400,400);
   
   TH1D *hSideband_x = (TH1D*)hSideband->ProjectionX();
   TH1D *hSignal_x = (TH1D*)hSignal->ProjectionX();
   TH1D *hSubtracted_x = (TH1D*)hSubtracted->ProjectionX();
   hSideband_x->SetLineColor(4);
   hSubtracted_x->SetLineColor(2);
   hSignal_x->Draw();
   hSideband_x->Draw("same");
   hSubtracted_x->Draw("same");   

   TCanvas *c5 = new TCanvas("c5","",400,400);
   
   TH1D *hSideband_y = (TH1D*)hSideband->ProjectionY();
   TH1D *hSignal_y = (TH1D*)hSignal->ProjectionY();
   TH1D *hSubtracted_y = (TH1D*)hSubtracted->ProjectionY();
   hSideband_y->SetLineColor(4);
   hSubtracted_y->SetLineColor(2);
   hSignal_y->Draw();
   hSideband_y->Draw("same");
   hSubtracted_y->Draw("same");  
}


