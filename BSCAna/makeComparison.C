#include <iostream.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TPad.h>

void makeComparison(char *infile="output-Cosmic2.root",int evt=2318565,char *infile2="output-2.root",int evt2=34)
{
   TFile *inf = new TFile(infile);
   TTree *data = (TTree*) inf->FindObjectAny("data");
   TFile *inf2 = new TFile(infile2);
   TTree *data2 = (TTree*) inf2->FindObjectAny("data");
   data->SetMarkerStyle(20);
   data->SetMarkerSize(2);
   data->SetMarkerColor(2);
   data2->SetMarkerStyle(20);
   data2->SetMarkerSize(2);
   data2->SetMarkerColor(4);
    
   TH2F *h[10];

   TCanvas *c = new TCanvas("c","",600,600);

   data->SetMarkerSize(0.5);

   int i=0;    
   data->Draw(Form("(data%d)/256/0.86:time%d-mintime0802[0]",i,i,i),Form("time%d>4000",i),"",1,evt);
   TLine *l =new TLine(4200,0,4500,0);
   l->SetLineColor(1);
   data->SetMarkerColor(1);
   data->SetMarkerStyle(4);
   l->Draw();
   
   data2->SetMarkerSize(0.5);
   data2->Draw(Form("(data%d)/256/0.86:time%d-mintime0802[0]",i,i,i),Form("time%d>4000",i),"same",1,evt2);
   

}
