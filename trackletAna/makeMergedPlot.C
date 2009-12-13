#include <iostream.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>
#include "UA5Plot.h"

void clearBin(TH1F* h)
{
   h->SetBinContent(2,0);
   h->SetBinContent(11,0);
   h->SetBinError(2,0);
   h->SetBinError(11,0);
}
TH1F* makeMergedPlot(char *name = "D6T-Official-Reweight",bool UA5=0, double uncert = 3.8)
{
   TH1F* hAvg2;
   // layer1+2
   TFile *inf12 = new TFile(Form("plot/root/result-12-%s.root",name));
   TH1F *h12 = (TH1F*)inf12->FindObjectAny("hMeasuredFinal");
   h12->SetName("h12");
   h12->SetAxisRange(0,5.5,"Y");

   // layer1+3
   TFile *inf13 = new TFile(Form("plot/root/result-13-%s.root",name));
   TH1F *h13 = (TH1F*)inf13->FindObjectAny("hMeasuredFinal");
   h13->SetName("h13");
   
   // layer23
   TFile *inf23 = new TFile(Form("plot/root/result-23-%s.root",name));
   TH1F *h23 = (TH1F*)inf23->FindObjectAny("hMeasuredFinal");
   h23->SetName("h23");
   
   TCanvas *c = new TCanvas("c","",400,400);
   
   
   h13->SetMarkerStyle(26);
   h13->SetMarkerColor(1);
   h13->SetLineColor(1);
   h23->SetMarkerStyle(25);
   h23->SetMarkerColor(4);
   h23->SetLineColor(4);

   clearBin(h12);
   clearBin(h23);
   clearBin(h13);

   TH1F *hUA5 = getUA5NSD();
   h12->SetXTitle("#eta");
   h12->SetYTitle("dN/d#eta");
   h12->Draw();
   if (UA5) hUA5->Draw("p same");   
   h12->Draw("same");
   h13->Draw("same");
   h23->Draw("same");
 
   TLegend *leg = new TLegend(0.3,0.18,1,0.35,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("hTruth","Data-Run123596","");
   entry=leg->AddEntry("h12","Reconstructed (1st+2nd layers)","pl");
   entry=leg->AddEntry("h13","Reconstructed (1st+3rd layers)","pl");   
   entry=leg->AddEntry("h23","Reconstructed (2nd+3rd layers)","pl");
   leg->Draw();   


   TCanvas *c2 = new TCanvas("c2","",400,400);
   TH1F *hAvg = (TH1F*) h12->Clone();
   hAvg->SetName("hAvg");
   
   for (int i=0;i<13;i++)
   {
    
      double avg = 0;
      avg += h12->GetBinContent(i);
      avg += h13->GetBinContent(i);
      avg += h23->GetBinContent(i);
      avg/=3.0;
      double avgErr = avg*uncert/100.;
      
      hAvg->SetBinContent(i,avg);
      hAvg->SetBinError(i,avgErr);
      
      if (i<3||i>10) {
         hAvg->SetBinContent(i,0);
         hAvg->SetBinError(i,0);
      } else {
         cout <<i<<" "<<avg<<" +- "<<avgErr<<endl;
      }
   }
   
   hAvg->Draw();
   if (UA5) hUA5->Draw("p same");
   hAvg->Draw("same");


   TCanvas *c3 = new TCanvas("c3","",400,400);
   hAvg2 = (TH1F*) h12->Clone();
   hAvg2->SetName("hAvg");
   
   for (int i=2;i<=6;i++)
   {
    
      double avg = 0;
      avg += h12->GetBinContent(i);
      avg += h13->GetBinContent(i);
      avg += h23->GetBinContent(i);
      avg += h12->GetBinContent(13-i);
      avg += h13->GetBinContent(13-i);
      avg += h23->GetBinContent(13-i);
      avg/=6.0;
      double avgErr = avg*uncert/100.;
      
      hAvg2->SetBinContent(i,avg);
      hAvg2->SetBinError(i,avgErr);
      hAvg2->SetBinContent(13-i,avg);
      hAvg2->SetBinError(13-i,avgErr);
      
      if (i<3||i>10) {
         hAvg2->SetBinContent(i,0);
         hAvg2->SetBinError(i,0);
      } else {
         cout <<i<<" "<<avg<<" +- "<<avgErr<<endl;
      }
   }
   
   hAvg2->Draw();
   if (UA5) hUA5->Draw("p same");
   hAvg2->Draw("same");
//   inf12->Close();
//   inf13->Close();
//   inf23->Close();
   return hAvg2;
}
