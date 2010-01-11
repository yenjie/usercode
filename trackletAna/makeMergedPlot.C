#include <iostream.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>
#include <fstream.h>
#include <TNtuple.h>
#include "UA5Plot.h"
#include "GraphErrorsBand.h"

void correctBin(TH1F* h,double* a1,double*a2)
{
   for (int i=1;i<=12;i++)
   {
      double bin = h->GetBinContent(i);
      double binErr = h->GetBinError(i);
      bin/=a1[i-1];
      bin/=a2[i-1];
      binErr/=a1[i-1];
      binErr/=a2[i-1];
      h->SetBinContent(i,bin);
      h->SetBinError(i,binErr);
   }
}

void clearBin(TH1F* h)
{
   h->SetBinContent(2,0);
   h->SetBinContent(11,0);
   h->SetBinError(2,0);
   h->SetBinError(11,0);
   for (int i=2;i<=11;i++)
   {
      h->SetBinError(i,h->GetBinContent(i)*3.8/100);
   }   

}


void clearBin2(TH1F* h)
{
   h->SetBinContent(2,0);
   h->SetBinContent(3,0);
   h->SetBinContent(11,0);
   h->SetBinContent(10,0);
   h->SetBinError(2,0);
   h->SetBinError(3,0);
   h->SetBinError(11,0);
   h->SetBinError(10,0);

   for (int i=2;i<=11;i++)
   {
      h->SetBinError(i,h->GetBinContent(i)*3.8/100);
   }   
}

TH1F* makeMergedPlot(char *name = "D6T-Official-Reweight",int UA5=0, double
uncert = 3.8,int par=0)
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
   

   TFile *outfile = new TFile(Form("mergedResult-%d.root",par),"recreate");
   TCanvas *c = new TCanvas("c","",400,400);
   
   TNtuple *nt = new TNtuple("nt","","par:eta:val:valerr");
   
   h13->SetMarkerStyle(26);
   h13->SetMarkerColor(1);
   h13->SetLineColor(1);
   h23->SetMarkerStyle(25);
   h23->SetMarkerColor(4);
   h23->SetLineColor(4);
   h12->SetMarkerSize(1.2);
   h13->SetMarkerSize(1.2);
   h23->SetMarkerSize(1.2);

 //  double acceptance1[12]={1,1,1,1,1,1,1,1,1,1,1,1};
   double acceptance1[12]={1.14972  ,1.02133,1.01079,0.99826 ,0.989326,0.988454,0.991541,0.993707,0.982555,0.960913,0.9896252,1.01673};
   double acceptance2[12]={1        ,1.11661,1.01825,0.995795,0.990697,0.991587,0.994311,0.994696,0.987352,0.984903,0.986908 ,1.12377};
   double acceptance3[12]={1        ,1.32239,1.03311,0.996744,0.985756,0.992889,1.00236,0.994132,0.983021,0.972563,0.930894,1};

   clearBin(h12);
   clearBin2(h23);
   clearBin2(h13);
/*
   correctBin(h12,acceptance12);
   correctBin(h13,acceptance13);
   correctBin(h23,acceptance23);
*/

   TH1F *hUA5 = getUA5NSD();
   TH1F *hUA5Scaled = getUA5NSDScaled();
   TH1F *hTracklet900GeV = tracklet900GeV();
   TH1F *hTracklet2360GeVHF1 = tracklet2360GeVHF1();
   h12->SetXTitle("#eta");
   h12->SetYTitle("dN/d#eta");
   h12->Draw();
   if (UA5) hUA5->Draw("p same");   
   if (UA5>=3) hUA5Scaled->Draw("p same");  
   h12->Draw("same");
   h13->Draw("same");
   h23->Draw("same");
 
   TLegend *leg = new TLegend(0.2,0.18,1,0.35,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("hTruth",Form("Data-%s",name),"");
   entry=leg->AddEntry("h12","Reconstructed (1st+2nd layers)","pl");
   entry=leg->AddEntry("h13","Reconstructed (1st+3rd layers)","pl");   
   entry=leg->AddEntry("h23","Reconstructed (2nd+3rd layers)","pl");
   leg->Draw();   


   TCanvas *c2 = new TCanvas("c2","",400,400);
   TH1F *hAvg = (TH1F*) h12->Clone();
   hAvg->SetName("hAvg");
   
   ofstream of("merged.txt");
   
   for (int i=0;i<13;i++)
   {
    
      double avg = 0;
      avg += h12->GetBinContent(i);
      avg += h13->GetBinContent(i);
      avg += h23->GetBinContent(i);
      if (i!=2&&i!=3&&i!=11&&i!=10) avg/=3.0; else avg/=1.0;
      double avgErr = avg*uncert/100.;
      
      hAvg->SetBinContent(i,avg);
      hAvg->SetBinError(i,avgErr);
      
      if (i<3||i>10) {
         hAvg->SetBinContent(i,0);
         hAvg->SetBinError(i,0);
      } else {
         cout <<i<<" "<<avg<<" +- "<<avgErr<<endl;
         of <<i<<" "<<avg<<" +- "<<avgErr<<endl;
	 nt->Fill(par,-3.25+0.5*(i),avg,avgErr);
      }
   }
   
   hAvg->Draw();
   if (UA5) hUA5->Draw("p same");
   if (UA5>=2) hTracklet900GeV->Draw("p same");   
   if (UA5>=3) hTracklet2360GeVHF1->Draw("p same");   
   if (UA5>=4) hUA5Scaled->Draw("p same");   
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
      if (i!=2&&i!=3&&i!=11&&i!=10) avg/=6.0; else avg/=2.0;
      double avgErr = avg*3.8/100.;
      
      hAvg2->SetBinContent(i,avg);
      hAvg2->SetBinError(i,0,avgErr);
      hAvg2->SetBinContent(13-i,avg);
      hAvg2->SetBinError(13-i,avgErr);
      
      if (i<3||i>10) {
         hAvg2->SetBinContent(i,0);
         hAvg2->SetBinError(i,0);
         hAvg2->SetBinContent(13-i,0);
         hAvg2->SetBinError(13-i,0);
      } else {
         cout <<i<<" "<<avg<<" +- "<<avgErr<<endl;
      }
   }
   
   hAvg2->Draw();


   if (UA5) hUA5->Draw("p same");
   if (UA5>=2) hTracklet900GeV->Draw("p same");   
   if (UA5>=3) hTracklet2360GeVHF1->Draw("p same");   
   if (UA5>=4) hUA5Scaled->Draw("p same");  
   hAvg2->SetLineColor(1);
   hAvg2->SetMarkerColor(1);
   hAvg2->Draw("p same");

   TLegend *leg2 = new TLegend(0.2,0.18,0.9,0.35,NULL,"brNDC");
   leg2->SetBorderSize(0);
   leg2->SetTextFont(62);
   leg2->SetLineColor(1);
   leg2->SetLineStyle(1);
   leg2->SetLineWidth(1);
   leg2->SetFillColor(0);
   leg2->SetFillStyle(0);
   TLegendEntry *entry2=leg2->AddEntry("hTruth",Form("Data-%s",name),"");
   entry2=leg2->AddEntry(hAvg2,"2.36 TeV p+p by Tracklet (CMS)","pl");
   entry2=leg2->AddEntry(hUA5,"900 GeV p+#bar{p} (UA5)","pl");
   leg2->Draw();   

   of.close();
   outfile->Write();
   return hAvg2;
}