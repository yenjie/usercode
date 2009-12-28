#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TArrow.h>
#include "selectionCut.h"

void plotAcceptedBin(int ch=12)
{
//   TFile *inputFile = new TFile("correction/correction-12-Official.root","");
//   TFile *inputFile = new TFile(Form("plot/root/result-%d-Run124022-Official-BSC.root",ch),"");
   TFile *inputFile = new TFile(Form("plot/root/result-%d-Run124120-yetkin-2360GeV-1224-HF2.root",ch),"");
//   TFile *inputFile = new TFile("./sample-1213/TrackletTree-900GeV-ATLAS.root","");
   TFile *inputFile2 = new TFile("./newEvenBetterSample/TrackletTree-Run124120.root");
//   TFile *inputFile2 = new TFile("./sample-official/TrackletTree-900GeV-D6T-20091210-KKVertex-removeDead2-newMult.root");
   
   TTree *TrackletTree = (TTree*)inputFile2->FindObjectAny(Form("TrackletTree%d",ch));
   TH2F *h = (TH2F*) inputFile->FindObjectAny("hAcceptance");
   TH2F *h2 = new TH2F("h2","",100,-3,3,100,-20,20);
   TCanvas *c = new TCanvas("c","",400,400);
   for (int i=3;i<=h->GetNbinsX()-2;i++){
      for (int j=0;j<=h->GetNbinsY();j++) {
         if (h->GetBinContent(i,j)!=0) h->SetBinContent(i,j,10e10); 
	 else                          h->SetBinContent(i,j,0);
      }
   }
   h->SetXTitle(Form("Pseudorapidity #eta (Tracklet %d)",ch));
   h->SetYTitle("Primary Vertex V_{z} (cm)");
   h->SetFillStyle(0);
   h->SetFillColor(15);
   h->SetLineColor(2);
   
   selectionCut myCut(0,0,10000);
   TrackletTree->Draw("vz[1]:eta1>>h2","abs(deta)<0.1&&abs(dphi)<1"&&myCut.Cut);
    
   h->Draw("box");
   c->SaveAs("plot/Acceptance/FigAcceptedBins.C");
   c->SaveAs("plot/Acceptance/FigAcceptedBins.eps");
   c->SaveAs("plot/Acceptance/FigAcceptedBins.gif");
   
   h2->SetXTitle(Form("Pseudorapidity #eta (Tracklet %d)",ch));
   h2->SetYTitle("Primary Vertex V_{z} (cm)");
   h2->Draw("col");
   h->Draw("box same");
   TLatex *   tex = new TLatex(-2.33,8.23,"CMS Preliminary");
   tex->SetLineWidth(2);
   tex->SetTextColor(0);
//   tex->Draw();
   c->SaveAs(Form("plot/Acceptance/FigAcceptedBinsOverlap%d.C",ch));
   c->SaveAs(Form("plot/Acceptance/FigAcceptedBinsOverlap%d.eps",ch));
   c->SaveAs(Form("plot/Acceptance/FigAcceptedBinsOverlap%d.gif",ch));

}

