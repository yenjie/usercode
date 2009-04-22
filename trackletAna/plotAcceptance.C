#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TArrow.h>

void plotAcceptance()
{
   TFile *inputFile = new TFile("TrackletTree-10TeV-TV-1.2M-1.root","");
//   TFile *inputFile = new TFile("output-oldMethod.root","");
   TTree *TrackletTree = (TTree*)inputFile->FindObjectAny("TrackletTree");
//   TFile *outputFile = new TFile("output.root","recreate");
   TCanvas *c = new TCanvas("c","",400,400);
   TH2F *h11 = new TH2F("h11","",20,-10,10,16,-4,4);
   TrackletTree->Draw("eta1:vz[1]>>h11");
   TH2F *h12 = new TH2F("h12","",20,-10,10,16,-4,4);
   TrackletTree->Draw("eta1:vz[1]>>h12");
   TH2F *h21 = new TH2F("h21","",20,-10,10,16,-4,4);
   TrackletTree->Draw("eta2:vz[1]>>h21");
   TH2F *h22 = new TH2F("h22","",20,-10,10,16,-4,4);
   TrackletTree->Draw("eta2:vz[1]>>h22");
   h12->Divide(h11);
   h12->SetFillStyle(2);
   h12->SetFillColor(18);
   h12->SetXTitle("Prmary Vertex V_{Z} (cm)");
   h12->SetYTitle("Hit Pseudorapidity (#eta)");
   h12->Draw("box");

   h22->Divide(h21);
   h22->SetFillStyle(2);
   h22->SetFillColor(15);
   h22->Draw("box same");

   TLatex *   tex = new TLatex(-7.028954,-3.711864,"First Layer");
   tex->SetLineWidth(2);
   tex->Draw();
   TArrow *arrow = new TArrow(-3.900771,-3.330508,-3.318784,-2.34322,0.02,"|>");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);
   arrow->Draw();
      tex = new TLatex(2.26495,2.762712,"Second Layer");
   tex->SetLineWidth(2);
   tex->Draw();
   arrow = new TArrow(6.538629,2.720339,5.665648,1.597458,0.02,"|>");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);
   arrow->Draw();

//   outputFile->Write();
//   outputFile->Close();
}
