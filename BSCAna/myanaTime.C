#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TCut.h>
#include <iostream>

#include <TF1.h>
#include <TMath.h>
Double_t LandauFunction(Double_t *x, Double_t *par) {
  Double_t xx = x[0];
  return par[0]*TMath::Landau(xx,par[1],par[2]);
}

void myanaTime(char *infile = "output.root",int ch=0)
{
   TFile *inf = new TFile(infile);
   
   TTree *data = (TTree*)inf->FindObjectAny("data");

   TCanvas *c = new TCanvas("c","",400,400);
   long t0=1258932363; //18:00
   
   TH1F *h = new TH1F("h","",3300,17,50);
   TH1F *h2 = new TH1F("h2","",3300,17,50);
   TH1F *h4 = new TH1F("h4","",3300,17,50);

   TH2F *hs = new TH2F("hs","",66,17,50,50,5,255);
  
   TCut beamsplashCut = "min[0]<-10&&min[1]<-10&&min[2]<-10&&min[3]<-10&&min[4]<-10&&min[5]<-10&&min[6]<-10&&min[7]<-10";
   TCut fireCut = "min[0]<-10||min[1]<-10||min[2]<-10||min[3]<-10||min[4]<-10||min[5]<-10||min[6]<-10||min[7]<-10";
   TCut fireCut2 = Form("min[%d]<-5||min[%d]<-5",(ch-1)>0?(ch-1):7,ch+1>7?0:ch+1);
   TCut matchCut = Form("(mintime[0]-mintime[%d])<60&&(mintime[0]-mintime[%d])>0",ch,ch);
   cout <<Form("min[%d]<-5||min[%d]<-5",(ch-1)>0?(ch-1):7,ch+1>7?0:ch+1)<<endl;
   
   data->Draw(Form("(evtSecond-%d)/3600+18>>h4",t0),fireCut&&!(matchCut));
   data->Draw(Form("(evtSecond-%d)/3600+18>>h",t0),beamsplashCut);
   data->Draw(Form("(evtSecond-%d)/3600+18>>h2",t0),fireCut);
   
   h->SetLineColor(2);
   h2->Scale(1./36);
   h->Scale(1./36);
   h4->Scale(1./36);
   h2->SetXTitle("Time (hr)");
   h2->SetYTitle("Rate of pulse > 50 mV in any channels");
   h4->SetLineColor(4);
   h2->Draw();
   h->Draw("same");
   h4->Draw("same");
   

   TCanvas *c2 = new TCanvas("c2","",400,400);
   data->Draw(Form("-min[%d]:(evtSecond-%d)/3600+18>>hs",ch,t0));
   hs->Draw("col");

   TCanvas *c3 = new TCanvas("c3","",400,400);
   TH1F *hSpectrum = new TH1F("hSpectrum","",258,-2,256);
   TH1F *hSpectrum2 = new TH1F("hSpectrum2","",258,-2,256);
   TH1F *hSpectrum3 = new TH1F("hSpectrum3","",258,-2,256);
   TH1F *hSpectrum4 = new TH1F("hSpectrum4","",258,-2,256);
  
   data->Draw(Form("-min[%d]>>hSpectrum",ch),Form("abs(((evtSecond-%d)/3600+18)-22)<200",t0)&&(!beamsplashCut)&&(matchCut));
 //  data->Draw(Form("-min[%d]>>hSpectrum2",ch),Form("abs(((evtSecond-%d)/3600+18)-26)<200",t0)&&(!beamsplashCut)&&matchCut);

   data->Draw(Form("-min[%d]>>hSpectrum3",ch),Form("abs(((evtSecond-%d)/3600+18)-22)<2&&min[0]>-50",t0)&&(!beamsplashCut)&&matchCut);
   data->Draw(Form("-min[%d]>>hSpectrum4",ch),Form("abs(((evtSecond-%d)/3600+18)-22)<2&&min[0]<-50",t0)&&(!beamsplashCut)&&matchCut);
//   data->Draw(Form("-min[%d]>>hSpectrum4",ch),Form("abs(((evtSecond-%d)/3600+18)-26)<2",t0)&&(!beamsplashCut)&&matchCut);
   
   hSpectrum->Sumw2();
   hSpectrum2->Sumw2();
//   hSpectrum->Scale(1./hSpectrum->GetEntries());
//   hSpectrum2->Scale(1./hSpectrum2->GetEntries());
   hSpectrum->Add(hSpectrum2,-1);
   hSpectrum->Rebin(2);
   hSpectrum->Draw();
   hSpectrum->SetXTitle("Pulse Height (ADC)");
   hSpectrum->SetYTitle("Arbitrary Unit");
   TF1 *fun = new TF1("fun",LandauFunction,0.0,256,3);
   fun->SetParameters(5,5,5);
   hSpectrum->Fit("fun","LL M","",0,256);
   TCanvas *c4 = new TCanvas("c4","",400,400);
//   hSpectrum3->Scale(1./hSpectrum3->GetEntries());
//   hSpectrum4->Scale(1./hSpectrum4->GetEntries());
   hSpectrum4->SetLineColor(2);
   hSpectrum3->Draw();
   hSpectrum4->Draw("same");
}
