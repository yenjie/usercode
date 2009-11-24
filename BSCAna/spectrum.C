#include <TTree.h>
#include <iostream.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLine.h>

void spectrum(char *filename = "example.root")
{
   TFile *infile = new TFile(filename);
   TTree *data = (TTree*) infile->FindObjectAny("data");
   
   TH1F *h[10];
   
   TCanvas *c = new TCanvas("c","",960,480);
   TPad *p = new TPad("p","",0,0,1,1);
   p->Divide(4,2,0.01,0.01);
   p->Draw();

   double norm;
   for (int i=0;i<8;i++)
   { 
      p->cd(i+1);
      p->GetPad(i+1)->SetLogy();
      h[i] = new TH1F(Form("h%d",i),Form("Ch %d",i),60,-0.1,1.1);
//      data->Draw(Form("-min[%d]>>h%d",i,i),"min[0]<min[1]&&min[0]<min[2]&&min[0]<min[3]&&min[0]<min[4]&&min[0]<min[5]&&min[0]<min[6]&&min[0]<min[7]");
      data->Draw(Form("-min[%d]/256./0.86>>h%d",i,i));
      if (i==0) norm = h[i]->GetEntries();
      h[i]->SetTitle(Form("Ch %d",i));
      h[i]->SetXTitle("Pulse height (Volt)");
      h[i]->SetYTitle("#");
      h[i]->SetMarkerSize(0.5);
      h[i]->SetMarkerColor(2);
      h[i]->Sumw2();
      h[i]->Scale(1./norm);
      h[i]->SetAxisRange(1e-7,10,"Y");
      h[i]->Draw("e");
   }
}


