#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TArrow.h>

void plotAcceptedBin()
{
   TFile *inputFile = new TFile("correction/correction-12-900GeV.root","");
   TFile *inputFile2 = new TFile("sampleYetkin/TrackletTree-900GeV-D6T.root");
   TTree *TrackletTree = (TTree*)inputFile2->FindObjectAny("TrackletTree12");
   TH2F *h = (TH2F*) inputFile->FindObjectAny("hAcceptance");
   TH2F *h2 = new TH2F("h2","",200,-3,3,200,-20,20);
   TCanvas *c = new TCanvas("c","",400,400);
   for (int i=0;i<=h->GetNbinsX();i++){
      for (int j=0;j<=h->GetNbinsY();j++) {
         if (h->GetBinContent(i,j)!=0) h->SetBinContent(i,j,10e10);
      }
   }
   h->SetXTitle("Pseudorapidity (#eta)");
   h->SetYTitle("Primary Vertex V_{z} (cm)");
   h->SetFillStyle(0);
   h->SetFillColor(15);
   h->SetLineColor(2);
   
   TrackletTree->Draw("vz[1]:eta1>>h2","abs(deta)<0.1");
    
   h->Draw("box");
   c->SaveAs("plot/Acceptance/FigAcceptedBins.C");
   c->SaveAs("plot/Acceptance/FigAcceptedBins.eps");
   c->SaveAs("plot/Acceptance/FigAcceptedBins.gif");
   
   h2->SetXTitle("Pseudorapidity (#eta)");
   h2->SetYTitle("Primary Vertex V_{z} (cm)");
   h2->Draw("col");
   h->Draw("box same");
   TLatex *   tex = new TLatex(-2.33,8.23,"CMS Preliminary");
   tex->SetLineWidth(2);
   tex->SetTextColor(0);
   tex->Draw();
   c->SaveAs("plot/Acceptance/FigAcceptedBinsOverlap.C");
   c->SaveAs("plot/Acceptance/FigAcceptedBinsOverlap.eps");
   c->SaveAs("plot/Acceptance/FigAcceptedBinsOverlap.gif");

}

