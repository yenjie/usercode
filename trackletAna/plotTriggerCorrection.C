#include <TFile.h>
#include <TH1F.h>

void plotTriggerCorrection()
{
   TFile  *a = new TFile("TriggerCorrection-900GeV.root");
   TFile  *b = new TFile("TriggerCorrection-10TeV.root");
   TH1F *h1 = (TH1F*) a->FindObjectAny("h");
   h1->SetName("h1");
   TH1F *h2 = (TH1F*) b->FindObjectAny("h");
   h2->SetName("h2");
   TCanvas *c = new TCanvas ("c","",400,400);   
   h1->SetAxisRange(0.8,0.9,"Y");
   h1->SetXTitle("#eta");
   h1->SetYTitle("#xi");
 
   h1->Draw();
   h1->SetLineColor(2);
   h1->SetMarkerColor(2);
   h2->Draw("same");

   TLegend * leg1 = new TLegend(0.37,0.24,0.92,0.35);
   leg1->SetFillStyle(0);
   leg1->SetFillColor(0);
   leg1->SetBorderSize(0);
   leg1->AddEntry(h2,"10TeV","pl");
   leg1->AddEntry(h1,"900GeV","pl");
  
   leg1->Draw();

}
