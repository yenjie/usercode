#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TProfile.h"
void formatProf(TProfile* h, int col, double norm = 1);
void formatHist(TH1* h, int col, double norm = 1);

void plotComparePerformance(char *inputFileName="output-histDeltaZStudy.root")
{
   TFile *inf = new TFile(inputFileName);
   TTree* ZTree = dynamic_cast<TTree*>(inf->Get("ZTree"));
   TCanvas *c = new TCanvas("c","",400,400);
 
   TH1F *hWidth = new TH1F("hWidth","",20,0+0.02,0.82);
   
   int maxNHit=50;

   TProfile *hPVEff = new TProfile("hPVEff","",maxNHit,0,maxNHit);
   TProfile *hPCVEff = new TProfile("hPCVEff","",maxNHit,0,maxNHit);
   TProfile *hTVEff = new TProfile("hTVEff","",maxNHit,0,maxNHit);

   formatProf(hPVEff,1);
   formatProf(hPCVEff,4);
   formatProf(hTVEff,2);

   ZTree->Draw("pvz>-99:nhit1>>hPVEff","","prof");
   ZTree->Draw("pcvz>-99:nhit1>>hPCVEff","","prof");
   ZTree->Draw("vz[0]>-99:nhit1>>hTVEff","","prof");

   hPVEff->Draw();
   hPCVEff->Draw("same");
   hTVEff->Draw("same");

   TProfile *hPVXi = new TProfile("hPVXi","",maxNHit,0,maxNHit);
   TProfile *hPCVXi = new TProfile("hPCVXi","",maxNHit,0,maxNHit);
   TProfile *hTVXi = new TProfile("hTVXi","",maxNHit,0,maxNHit);

   formatProf(hPVXi,1);
   formatProf(hPCVXi,4);
   formatProf(hTVXi,2);
   
   TCanvas *c2 = new TCanvas("c2","",400,400);
 
   ZTree->Draw("abs(pvz-gvz):nhit1>>hPVXi","pvz!=-99","prof");
   ZTree->Draw("abs(pcvz-gvz):nhit1>>hPCVXi","pcvz!=-99","prof");
   ZTree->Draw("abs(vz[0]-gvz):nhit1>>hTVXi","vz[0]!=-99&&nz>0","prof");

   hPCVXi->Draw();
   hPVXi->Draw("same");
   hTVXi->Draw("same");

   TH1F *hPVWidth = new TH1F("hPVWidth","",maxNHit,0,maxNHit);
   TH1F *hPCVWidth = new TH1F("hPCVWidth","",maxNHit,0,maxNHit);
   TH1F *hTVWidth = new TH1F("hTVWidth","",maxNHit,0,maxNHit);

   formatHist(hPVWidth,1);
   formatHist(hPCVWidth,4);
   formatHist(hTVWidth,2);
       
   for (int j=1;j<maxNHit;j++) {
      TH1F *tmp = new TH1F("tmp","",3000,-0.5,0.5);
	    ZTree->Project("tmp","pvz-gvz",Form("nhit1==%d&&pvz!=-99",j));
	    if (tmp->GetEntries()<=10) continue;
	    tmp->Fit("gaus","LL M");
	    TF1 *f = tmp->GetFunction("gaus");
	    double widthErr= f->GetParError(2);
	    double width= f->GetParameter(2);
	    hPVWidth->SetBinContent(j+1,width);
	    hPVWidth->SetBinError(j+1,widthErr);
      delete tmp;
   }

   for (int j=1;j<maxNHit;j++) {
      TH1F *tmp = new TH1F("tmp","",3000,-2,2);
	    ZTree->Project("tmp","pcvz-gvz",Form("nhit1==%d&&pcvz!=-99",j));
	    if (tmp->GetEntries()==0) continue;
	    tmp->Fit("gaus","LL ");
	    TF1 *f = tmp->GetFunction("gaus");
	    double widthErr= f->GetParError(2);
	    double width= f->GetParameter(2);
	    hPCVWidth->SetBinContent(j+1,width);
	    hPCVWidth->SetBinError(j+1,widthErr);
      delete tmp;
   }

   for (int j=1;j<maxNHit;j++) {
      TH1F *tmp = new TH1F("tmp","",3000,-0.5,0.5);
	    ZTree->Project("tmp","vz[0]-gvz",Form("nhit1==%d&&vz[0]!=-99",j));
	    if (tmp->GetEntries()==0) continue;
	    tmp->Fit("gaus","LL ");
	    TF1 *f = tmp->GetFunction("gaus");
	    double widthErr= f->GetParError(2);
	    double width= f->GetParameter(2);
	    hTVWidth->SetBinContent(j+1,width);
	    hTVWidth->SetBinError(j+1,widthErr);
      delete tmp;
   }

   TCanvas *c3 = new TCanvas("c3","",400,400);
   hPCVWidth->Draw();   
   hPVWidth->Draw("same");   
   hTVWidth->Draw("same");   

}

void formatProf(TProfile* h, int col, double norm){

  h->Scale(1/norm);
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->SetMarkerSize(0.7);
  h->GetYaxis()->SetTitleOffset(1.15);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

}

void formatHist(TH1* h, int col, double norm){

  h->Scale(1/norm);
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->SetMarkerSize(0.7);
  h->GetYaxis()->SetTitleOffset(1.15);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

}
