#include <iostream.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TPad.h>
#include <TProfile.h>
#include <TCut.h>

void scatter(char *infile="output.root",int ch1=0,int ch2=1)
{
   TFile *inf = new TFile(infile);
   TTree *data = (TTree*) inf->FindObjectAny("data");
   data->SetMarkerStyle(20);
   data->SetMarkerSize(1);
   data->SetMarkerColor(2);
    
   TH2F *h[10];

   TCanvas *c = new TCanvas("c","",600,600);
   TProfile *p = new TProfile("p","",18,-0.9,0.9);
   TProfile *p2 = new TProfile("p2","",18,-0.9,0.9);
   TProfile *p0802 = new TProfile("p0802","",18,-0.9,0.9);
   
  data->SetMarkerColor(2);
  data->SetMarkerStyle(20);
  data->SetMarkerSize(0.5);
  
  TCut nosat="";//Form("mintime[%d]>-pedestal+1&&mintime[%d]>-pedestal+1",ch1,ch2);
  TCut cut =Form("mintime[%d]>4000&&mintime[%d]>4000&&abs(mintime[%d]-mintime[%d])<40&&min[%d]/256<-0.1&&min[%d]/256<-0.1",ch1,ch2,ch1,ch2,ch1,ch2);
  
  //min[0]<-10&&min[1]<-10&&min[2]<-10&&min[3]<-10&&min[4]<-10&&min[5]<-10&&min[6]<-10&&min[7]<-10&&
  data->Draw(Form("mintime[%d]-mintime[%d]:(abs(min[%d])-abs(min[%d]))/256/0.86>>p",ch1,ch2,ch1,ch2),cut&&nosat,"prof");

  TCut cut02 =Form("mintime02[%d]>4000&&mintime02[%d]>4000&&abs(mintime02[%d]-mintime02[%d])<40&&min[%d]/256<-0.1&&min[%d]/256<-0.1",ch1,ch2,ch1,ch2,ch1,ch2);

 
data->Draw(Form("mintime02[%d]-mintime02[%d]:(abs(min[%d])-abs(min[%d]))/256/0.86>>p2",ch1,ch2,ch1,ch2),cut02&&nosat,"prof");

  TCut cut0802=Form("mintime0802[%d]>4000&&mintime0802[%d]>4000&&abs(mintime0802[%d]-mintime0802[%d])<40&&min[%d]/256<-0.1&&min[%d]/256<-0.1",ch1,ch2,ch1,ch2,ch1,ch2);

  data->Draw(Form("mintime0802[%d]-mintime0802[%d]:(abs(min[%d])-abs(min[%d]))/256/0.86>>p0802",ch1,ch2,ch1,ch2),cut0802&&nosat,"prof");

  p->SetXTitle(Form("|Pulse Height %d| - |Pulse Height %d| (volt)",ch1,ch2));
  p->SetYTitle(Form("T_{%d}-T_{%d} (ns)",ch1,ch2));
  p->SetAxisRange(-8,8,"Y");
  p2->SetLineColor(2);
  p2->SetMarkerColor(2);
//  p2->SetMarkerStyle(4);
  p0802->SetLineColor(4);
  p0802->SetMarkerColor(4);
  p->Draw();
  p2->Draw("same");
  p0802->Draw("same");
  
  TCanvas *c2 = new TCanvas("c2","",600,600);
  
  data->Draw(Form("mintime[%d]-mintime[%d]:(abs(min[%d])-abs(min[%d]))/256/0.86",ch1,ch2,ch1,ch2),cut&&nosat,"col");
  p->Draw("same");


  TCanvas *c3 = new TCanvas("c3","",600,600);
  
  data->Draw(Form("mintime0802[%d]-mintime0802[%d]:(abs(min[%d])-abs(min[%d]))/256/0.86",ch1,ch2,ch1,ch2),cut0802&&nosat,"col");
  p0802->Draw("same");

//  data->Scan("min[0]",cut&&nosat);
}
