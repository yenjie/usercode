#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <TTree.h>
#include <iostream.h>
#include <TLine.h>
#include <math.h>
#include <TF1.h>
#include <TCut.h>


void formatHist(TH1* h, int col = 1, double norm = 1);
void plotBeta(char* filename,char *toyMC="pseudoExp.root",char *mycut= "",bool useMC = false)
{
   TFile *f= new TFile(filename);
   
   TNtuple * ntmatched= dynamic_cast<TNtuple *>(f->Get("ntmatched"));
   TFile *fmc= new TFile(toyMC);
   TNtuple * ntmatchedMC= dynamic_cast<TNtuple *>(fmc->Get("ntmatched"));
   TNtuple * betas = new TNtuple("betas","","bin:nhit:beta:betaErr");

   TH2F *h1 = new TH2F("h1","",12,-3,3,80,0,80);
   TH2F *h2 = new TH2F("h2","",12,-3,3,80,0,80);
   TH2F *h2MC = new TH2F("h2MC","",12,-3,3,80,0,80);
   TH2F *h3 = new TH2F("h3","",12,-3,3,80,0,80);
   TH1F *betaPlots[12];
   
   for (int i=0;i<12;i++) {
      betaPlots[i] = new TH1F(Form("beta%d",i),"",80,0,80);
   } 
  
   TCut cut = mycut;
   TCut signalRegion = "abs(dphi)<0.5";
   TCut signalRegionEta = "abs(dphi)<0.5&&abs(deta)<0.1";
   TCut sideBandRegion = "abs(dphi)>0.5&&abs(dphi)<1";
   TCut sideBandRegionEta = "abs(dphi)>0.5&&abs(dphi)<1&&abs(deta)>1";
   TCut sideBandRegionEtaSignalRegion = "abs(dphi)>0.5&&abs(dphi)<1&&abs(deta)<0.1";
   TCanvas *c0 = new TCanvas ("c0","",400,400);
   ntmatched->Draw("nhit1:eta1>>h1","abs(dphi)<0.5"&&cut);

   h3->SetXTitle("#eta");
   h3->SetYTitle("#Delta#eta");
   if (useMC) {
     cout <<"Use ToyM"<<endl;
     ntmatchedMC->Draw("nhit1:eta1>>h2",sideBandRegionEta&&cut);
     int n = ntmatched->GetEntries(sideBandRegionEta&&cut);
     int n2 = ntmatchedMC->GetEntries(sideBandRegionEta&&cut);
     h2->Scale(1./n2*n); 
   } else {
     ntmatched->Draw("nhit1:eta1>>h2",sideBandRegionEtaSignalRegion&&cut);   
   }

   h1->Sumw2();
   h2->Sumw2();
   h3->Sumw2();
   for (int x=1;x<=12;x++) {
      for (int y=1;y<=80;y++) {
         double beta = 0;
	 if (h1->GetBinContent(x,y)!=0&&h2->GetBinContent(x,y)!=0) 
	 {   
	       beta = h2->GetBinContent(x,y)/h1->GetBinContent(x,y);
               h3->SetBinContent(x,y, beta);
	       double e1 = h1->GetBinError(x,y)/h1->GetBinContent(x,y);
	       double e2 = h2->GetBinError(x,y)/h2->GetBinContent(x,y);
               double betaErr = beta* sqrt(e1*e1+e2*e2);
               h3->SetBinError(x,y, betaErr);
	       betas->Fill(x,y,beta,betaErr);
	       betaPlots[x-1]->SetBinContent(y,beta);
	       betaPlots[x-1]->SetBinError(y,betaErr);
	 }      
      }
   }
   h3->Draw("col");
   TCanvas *c[12];
   
   for (int i=0;i<12;i++) 
   {
      c[i]= new TCanvas (Form("c%d",i),"",400,400);
      formatHist(betaPlots[i],2,1);
      TF1 *fun = new TF1("fun","[1]/(x+[3])+[2]/x/x+[0]",0,100);
      double etaMin = i*0.5 -3;
      double etaMax = i*0.5 -3+0.5;
      betaPlots[i]->Fit("fun","m");
      betaPlots[i]->SetXTitle("N_{Hits}");
      betaPlots[i]->SetYTitle(Form("#beta %.1f < #eta < %.1f",etaMin,etaMax)); 
      betaPlots[i]->SetAxisRange(0,1,"Y");
      betaPlots[i]->Draw("p");
      TF1 *f1 = betaPlots[i]->GetFunction("fun");
      cout <<"@   if((eta)>="<<etaMin<<"&&(eta)<="<<etaMax<<") {"<<endl;
      cout <<"@      beta     = ("<<f1->GetParameter(0)<<")+"<<endl;
      cout <<"@                 ("<<f1->GetParameter(1)<<") / cnhit +"<<endl;
      cout <<"@                 ("<<f1->GetParameter(2)<<") * cnhit;"<<endl;
      cout <<"@   }    "<<endl;

   }   
   
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
