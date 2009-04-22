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

#define nEtaBin 12
#define nHitBin 250

void formatHist(TH1* h, int col = 1, double norm = 1);
void plotBetaFromTree1(char* filename,char *toyMC="pseudoExp.root",int useCorrectionFile = 0,char *mycut="",bool useMC = false)
{
   TFile *f= new TFile(filename);
   
   TNtuple * ntmatched= dynamic_cast<TNtuple *>(f->Get("ntmatched"));
   TTree * TrackletTree= dynamic_cast<TTree *>(f->Get("TrackletTree"));
   TFile *fmc= new TFile(toyMC);
   TNtuple * ntmatchedMC= dynamic_cast<TNtuple *>(fmc->Get("ntmatched"));
   TNtuple * betas = new TNtuple("betas","","bin:nhit:beta:betaErr");
   TNtuple * correction = new TNtuple("correction","","bin:nhit:scale:subtract");

   TFile *outf = new TFile ("correction.root","recreate");
   TH2F *h1 = new TH2F("h1","Everything",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *h2 = new TH2F("h2","Reproduced Background",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *h2MC = new TH2F("h2MC","Reproduced Background from ToyMC",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *h3 = new TH2F("h3","Background Subtracted",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *h4 = new TH2F("h3","(1-beta)*Everything",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *hSubtracted = new TH2F("hSubtracted","",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *hHadron = new TH2F("hHadron","",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH2F *hCorrected = new TH2F("hCorrected","",nEtaBin,-3,3,nHitBin,-0.5,nHitBin-0.5);
   TH1F *alphaPlots[nEtaBin];
   TH1F *betaPlots[nEtaBin];
   TF1 *funBeta[nEtaBin];
   TF1 *funAlpha[nEtaBin];
   TF1 *fBeta[nEtaBin];
   TF1 *fAlpha[nEtaBin];
      
   
   for (int i=0;i<nEtaBin;i++) {
      alphaPlots[i] = new TH1F(Form("alpha%d",i),"",nHitBin,-0.5,nHitBin-0.5);
      betaPlots[i] = new TH1F(Form("beta%d",i),"",nHitBin,-0.5,nHitBin-0.5);
   } 

   // Signal and Sideband regions  
   TCut cut = mycut;
   TCut signalRegion = "abs(dphi)<0.5";
   TCut signalRegionEta = "abs(dphi)<0.5&&abs(deta)<0.1";
   TCut sideBandRegion = "abs(dphi)>0.5&&abs(dphi)<1";
   TCut sideBandRegionEta = "abs(dphi)>0.5&&abs(dphi)<1&&abs(deta)>1";
   TCut sideBandRegionEtaSignalRegion   ="abs(dphi)>0.5&&abs(dphi)<1&&abs(deta)<0.1";


   TCanvas *cBetas = new TCanvas ("cBetas","",400,400);
   TrackletTree->Draw("nhit1:eta1>>h1","abs(dphi)<0.5"&&cut);

   h3->SetXTitle("#eta");
   h3->SetYTitle("#Delta#eta");
   if (useMC) {
     cout <<"Use ToyM"<<endl;
     ntmatchedMC->Draw("nhit1:eta1>>h2",sideBandRegionEta&&cut);
     int n = ntmatched->GetEntries(sideBandRegionEta&&cut);
     int n2 = ntmatchedMC->GetEntries(sideBandRegionEta&&cut);
     h2->Scale(1./n2*n); 
   } else {
   TrackletTree->Draw("nhit1:eta1>>h2",sideBandRegionEtaSignalRegion&&cut);   
   }

   h1->Sumw2();
   h2->Sumw2();
   h3->Sumw2();


   // Beta calculation
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nHitBin;y++) {
         double beta = 0;
	 if (h1->GetBinContent(x,y)!=0&&h2->GetBinContent(x,y)!=0) 
	 {   
	       beta = h2->GetBinContent(x,y)/h1->GetBinContent(x,y);
               double e1 = h1->GetBinError(x,y)/h1->GetBinContent(x,y);
	       double e2 = h2->GetBinError(x,y)/h2->GetBinContent(x,y);
               double betaErr = beta* sqrt(e1*e1+e2*e2);
               betas->Fill(x,y,beta,betaErr);
	       betaPlots[x-1]->SetBinContent(y,beta);
	       betaPlots[x-1]->SetBinError(y,betaErr);

	       beta = h1->GetBinContent(x,y)-h2->GetBinContent(x,y);
               hSubtracted->SetBinContent(x,y, beta);
	       double e3 = h1->GetBinError(x,y);
	       double e4 = h2->GetBinError(x,y);
               betaErr = sqrt(e1*e1+e2*e2);
               h3->SetBinContent(x,y, beta);
	       h3->SetBinError(x,y, betaErr);
	       hSubtracted->SetBinError(x,y, betaErr);
	 }      
      }
   }
   h3->Draw("col");

   TCanvas *diff = new TCanvas ("diff","",400,400);
   hSubtracted->Draw("col");

   // Charged Hadrons:
   TCanvas *cHadrons = new TCanvas("cHadrons","",400,400);
   hHadron->SetXTitle("#eta");
   hHadron->SetYTitle("N_{hit}^{Layer1} |#eta|<1");
   TrackletTree->Draw("nhit1:eta>>hHadron","","col");
   

   TCanvas *c[nEtaBin];


   
   for (int i=0;i<nEtaBin;i++) 
   {
      c[i]= new TCanvas (Form("cA%d",i),"",400,400);
      formatHist(betaPlots[i],2,1);
      funBeta[i] = new TF1(Form("funBeta%d",i),"[1]/(x+[3]+0.5)+[2]/(x+0.5)/(x+0.5)+[0]",0,100);
      double etaMin = i*0.5 -3;
      double etaMax = i*0.5 -3+0.5;
      betaPlots[i]->Fit(Form("funBeta%d",i),"m");
      betaPlots[i]->SetXTitle("N_{Hits}");
      betaPlots[i]->SetYTitle(Form("#beta %.1f < #eta < %.1f",etaMin,etaMax)); 
      betaPlots[i]->SetAxisRange(0,1,"Y");
      betaPlots[i]->Draw("p");
      fBeta[i] = betaPlots[i]->GetFunction(Form("funBeta%d",i));
 /*
      cout <<"@   if((eta)>="<<etaMin<<"&&(eta)<="<<etaMax<<") {"<<endl;
      cout <<"@      beta     = ("<<fBeta[i]->GetParameter(0)<<")+"<<endl;
      cout <<"@                 ("<<fBeta[i]->GetParameter(1)<<") / cnhit +"<<endl;
      cout <<"@                 ("<<fBeta[i]->GetParameter(2)<<") * cnhit;"<<endl;
      cout <<"@   }    "<<endl;
*/
   }   


   // alpha calculation
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nHitBin;y++) {
         double val = 0;
	 if (h1->GetBinContent(x,y)!=0&&h2->GetBinContent(x,y)!=0) 
	 {   
	       val = h1->GetBinContent(x,y);
               double beta = fBeta[x-1]->Eval(y);
	       val*=(1-beta);
	       h4->SetBinContent(x,y, val);
	       double e1 = h1->GetBinError(x,y);
	       double e2 = 0;
               double valErr = beta* sqrt(e1*e1+e2*e2);
               h4->SetBinError(x,y, valErr);
	       correction->Fill(x,y,val,h3->GetBinContent(x,y));
	       double truth    = hHadron->GetBinContent(x,y);
	       double truthErr = hHadron->GetBinError(x,y)/truth;
	       valErr/=val;
	       double alphaErr = truth/val*sqrt(truthErr*truthErr+valErr*valErr);
	       alphaPlots[x-1]->SetBinContent(y,truth/val);
	       alphaPlots[x-1]->SetBinError(y,truthErr);
	 }      
      }
   }
   

   // corrected calculation
   TFile *fcorrection;
   
   if (useCorrectionFile == 1) {
      fcorrection = new TFile("correction-900GeV.root");
      for (int i=0;i<nEtaBin;i++)
      {
         fAlpha[i] = (TF1*) fcorrection->FindObjectAny(Form("funAlpha%d",i));
      }
   } else if (useCorrectionFile == 2) {
      fcorrection = new TFile("correction-10TeV.root");
      for (int i=0;i<nEtaBin;i++)
      {
         fAlpha[i] = (TF1*) fcorrection->FindObjectAny(Form("funAlpha%d",i));
      }
   } else {

      for (int i=0;i<nEtaBin;i++) 
      {
          c[i]= new TCanvas (Form("c%d",i),"",400,400);
         formatHist(alphaPlots[i],2,1);
         funAlpha[i] = new TF1(Form("funAlpha%d",i),"[1]/(x+[3]+0.5)+[2]/(x+0.5)/(x+0.5)+[0]",0,100);
         double etaMin = i*0.5 -3;
         double etaMax = i*0.5 -3+0.5;
         alphaPlots[i]->Fit(Form("funAlpha%d",i),"m");
         alphaPlots[i]->SetXTitle("N_{Hits}");
         alphaPlots[i]->SetYTitle(Form("#alpha %.1f < #eta < %.1f",etaMin,etaMax)); 
         alphaPlots[i]->Draw("p");
         fAlpha[i] = alphaPlots[i]->GetFunction(Form("funAlpha%d",i));  
   /*
         cout <<"@   if((eta)>="<<etaMin<<"&&(eta)<="<<etaMax<<") {"<<endl;
         cout <<"@      beta     = ("<<fAlpha[i]->GetParameter(0)<<")+"<<endl;
         cout <<"@                 ("<<fAlpha[i]->GetParameter(1)<<") / cnhit +"<<endl;
         cout <<"@                 ("<<fAlpha[i]->GetParameter(2)<<") * cnhit;"<<endl;
         cout <<"@   }    "<<endl;
   */ 
      }  
   }
   
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nHitBin;y++) {
         double val = 0;
	 if (h1->GetBinContent(x,y)!=0&&h2->GetBinContent(x,y)!=0) 
	 {   
	       val = h1->GetBinContent(x,y);
               double beta = fBeta[x-1]->Eval(y);
	       double alpha = fAlpha[x-1]->Eval(y);
	       val*=(1-beta)*alpha;
	       hCorrected->SetBinContent(x,y, val);
	       double e1 = h1->GetBinError(x,y);
	       double e2 = 0;
               double valErr = e1*(1-beta)*alpha;
               hCorrected->SetBinError(x,y, valErr);
	  }      
      }
   }
   
   
   TCanvas *cDNdEta = new TCanvas("cDNdEta","",400,400);
   double nevent = TrackletTree->GetEntries("nhit1");
   TH1D *hTruth = hHadron->ProjectionX();
   TH1D *hMeasured = hCorrected->ProjectionX(); 
   hTruth->Sumw2();
   hMeasured->Sumw2();
   
   formatHist(hTruth,1,nevent/2.);
   formatHist(hMeasured,2,nevent/2.);
   hTruth->SetAxisRange(0,8,"y");
   hTruth->SetXTitle("#eta");
   hTruth->SetYTitle("dN/d#eta");
   hTruth->Draw("hist");
   hMeasured->Draw("e same");    
   
   TCanvas *cRatio = new TCanvas("cRatio","",400,400);
   TH1D *hRatio = (TH1D*)hMeasured->Clone();
   hRatio->Divide(hTruth);
   hRatio->SetXTitle("#eta");
   hRatio->SetYTitle("Ratio");
   hRatio->SetAxisRange(0.8,1.2,"y");
   hRatio->Draw();
   
   TLine *l1 = new TLine(-3,1,3,1);
   l1->Draw();
   
   for (int i=0;i<nEtaBin;i++){
      fBeta[i]->Write();
      fAlpha[i]->Write();
   }
   cDNdEta->Write();
   outf->Write();
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
