#include <iostream.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TPad.h>

void makeplot1(char *infile="output.root",int evt=1)
{
   TFile *inf = new TFile(infile);
   TTree *data = (TTree*) inf->FindObjectAny("data");
   data->SetMarkerStyle(20);
   data->SetMarkerSize(1);
   data->SetMarkerColor(2);
    
   TH2F *h[10];

   TCanvas *c = new TCanvas("c","",600,600);
   TPad *p = new TPad("p","",0,0,1,1);
   p->Divide(1,1,0.01,0.01);
   p->Draw();
   
//   data->SetMarkerSize(0.5);
   for (int i=0;i<1;i++)
   { 
      p->cd(i+1);
      data->SetMarkerColor(2);
      data->SetMarkerStyle(20);
      data->SetMarkerSize(0.5);
      
      h[i] = new TH2F(Form("h%d",i),Form("Evt %d Ch %d",evt,i),600,4200,4500,700,-0.5,0.05);
      h[i]->SetTitle(Form("Evt %d Ch %d",evt,i));
      h[i]->SetXTitle("Time (ns)");
      h[i]->SetYTitle("Volt");
      h[i]->SetMarkerSize(0.1);
      h[i]->SetMarkerColor(2);
      h[i]->Draw();
      data->Draw(Form("(data%d)/256/0.86:time%d",i,i,i),Form("time%d>4000",i),"same",1,evt);
      TLine *l =new TLine(4200,0,4500,0);
      l->SetLineColor(1);
      data->SetMarkerColor(1);
      data->SetMarkerStyle(4); 
      data->SetMarkerSize(1);
      data->Draw(Form("min[%d]/256/0.86:mintime[%d]",i,i),"","same",1,evt);
      data->Draw(Form("min[%d]/256/0.86*0.8:mintime08[%d]",i,i),"","same",1,evt);
      data->Draw(Form("min[%d]/256/0.86*0.2:mintime02[%d]",i,i),"","same",1,evt);
      data->Draw(Form("min[%d]/256/0.86*0.0:mintime0802[%d]",i,i),"","same",1,evt);
      data->SetMarkerColor(4);
      data->Draw(Form("min[%d]/256/0.86*0.4:mintime04[%d]",i,i),"","same",1,evt);
      data->Draw(Form("min[%d]/256/0.86*0.0:mintime0804[%d]",i,i),"","same",1,evt);
      l->Draw();
   }

}
