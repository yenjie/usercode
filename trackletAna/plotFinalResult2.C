#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TH3D.h>
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
#include <TPad.h>

#include "GraphErrorsBand.h"

#define nEtaBin 12
#define nHitBin 22
#define nDEtaBin 80
#define nVzBin 20

#define plotDEta true



void formatHist(TH1* h, int col = 1, double norm = 1);

//===========================================================================
// Read Stored correction factors
//===========================================================================
TFile* getCorrectionFile(int useCorrectionFile){
   TFile *fCorrection;
   if (useCorrectionFile == 1) {
      fCorrection = new TFile("correction2-900GeV.root");
   } else if (useCorrectionFile == 2) {
      fCorrection = new TFile("correction2-10TeV.root");
   } else if (useCorrectionFile == 3) {
      fCorrection = new TFile("correction2-900GeV-half.root");
   } else if (useCorrectionFile == 4) {
      fCorrection = new TFile("correction2-10TeV-half.root");
   }
   return fCorrection;  
}

TFile* getTriggerCorrectionFile(int useCorrectionFile){
   TFile *fCorrection;
   if (useCorrectionFile == 1) {
      fCorrection = new TFile("TriggerCorrection-900GeV.root");
   } else if (useCorrectionFile == 2) {
      fCorrection = new TFile("TriggerCorrection-10TeV.root");
   } else if (useCorrectionFile == 3) {
      fCorrection = new TFile("TriggerCorrection-900GeV.root");
   } else if (useCorrectionFile == 4) {
      fCorrection = new TFile("TriggerCorrection-10TeV.root");
   } else {
      fCorrection = new TFile("TriggerCorrection-10TeV.root");
   }
   return fCorrection;  
   
}


//===========================================================================
// Main Routine
//===========================================================================
int plotFinalResult2(char* filename,char *myPlotTitle="PYTHIA 900 GeV",int
useCorrectionFile = 0,  Long64_t nentries = 1000000000, Long64_t firstentry = 0)
{
   FILE *logFile = fopen("correction.log","w");
   
   TFile *f= new TFile(filename);
   
   //TNtuple * ntmatched= dynamic_cast<TNtuple *>(f->Get("ntmatched"));
   TTree * TrackletTree= dynamic_cast<TTree *>(f->Get("TrackletTree"));

   // Reaad alpha, beta, geometry correction from file.
   TFile *fCorrection = getCorrectionFile(useCorrectionFile);
   TFile *fTriggerCorrection = getTriggerCorrectionFile(useCorrectionFile);
   

   double HitBins[nHitBin+1] = {0,1,2,3,4,5,6,7,8,9,10,15,20,25,30,35,40,50,60,80,120,160,200};
   double EtaBins[nEtaBin+1];
   double VzBins[nVzBin+1];
   
   for (int i=0;i<=nEtaBin;i++) EtaBins[i] = (double)i*6.0/(double)nEtaBin-3.0;;
   for (int i=0;i<=nVzBin;i++) VzBins[i] = (double)i*20.0/(double)nVzBin-10.0;


   // Output file
   TFile *outf = new TFile ("correction.root","recreate");

   TNtuple * betas = new TNtuple("betas","","bin:nhit:vz:beta:betaErr");
   TNtuple * alphas = new TNtuple("alphas","","bin:nhit:vz:alpha:alphaErr");
   TNtuple * correction = new TNtuple("correction","","bin:nhit:obs:gen:err");
   TNtuple * correction1 = new TNtuple("correction1","","e1:e2");
   
   
   TH3F *hEverything = new TH3F("hEverything","Everything",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *h2 = new TH3F("h2","Reproduced Background",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *h2MC = new TH3F("h2MC","Reproduced Background from ToyMC",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *h3 = new TH3F("h3","Background Subtracted",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *h4 = new TH3F("h3","(1-beta)*Everything",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *hSubtracted = new TH3F("hSubtracted","",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *hHadron = new TH3F("hHadron","",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *hHadronAccepted = new TH3F("hHadronAccepted","",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *hCorrected = new TH3F("hCorrected","",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);

   TH2F *hEtaVzRatio = new TH2F("hEtaVzatio","#eta:Vz",nEtaBin,EtaBins,nVzBin,VzBins);
   TH2F *hEtaHitRatio = new TH2F("hEtaHitatio","#eta:N_{Hit}",nEtaBin,EtaBins,nHitBin,HitBins);

   TH1F *alphaPlots[nEtaBin][nVzBin];
   TH1F *betaPlots[nEtaBin][nVzBin];
   TH1F *alphaErrPlots[nEtaBin][nVzBin];
   TH1F *betaErrPlots[nEtaBin][nVzBin];
   TH1F *hDEtaAll[nEtaBin][nVzBin];
   TH1F *hDEtaSig[nEtaBin][nVzBin];
   TH1F *hDEtaBck[nEtaBin][nVzBin];

   TH1F *hCorrectedEtaBin = new TH1F("hCorrectedEtaBin","Corrected",nEtaBin,-3,3);
   TH1F *hTriggerCorrection;   

   TF1 *funBeta[nEtaBin][nVzBin];
   TF1 *funAlpha[nEtaBin][nVzBin];
   TF1 *funBetaErr[nEtaBin][nVzBin];
   TF1 *funAlphaErr[nEtaBin][nVzBin];
   TF1 *fBeta[nEtaBin][nVzBin];
   TF1 *fBetaErr[nEtaBin][nVzBin];
   TF1 *fAlpha[nEtaBin][nVzBin];
   TF1 *fAlphaErr[nEtaBin][nVzBin];
   TH1D *hTruthRatio;
   
      
   // Prepare histograms
   for (int i=0;i<nEtaBin;i++) {
      for (int j=0;j<nVzBin;j++) {
         alphaPlots[i][j] = new TH1F(Form("alpha%dVz%d",i,j),"",nHitBin,HitBins);
         alphaErrPlots[i][j] = new TH1F(Form("alphaErr%dVz%d",i,j),"",nHitBin,HitBins);
         betaPlots[i][j] = new TH1F(Form("beta%dVz%d",i,j),"",nHitBin,HitBins);
         betaErrPlots[i][j] = new TH1F(Form("betaErr%dVz%d",i,j),"",nHitBin,HitBins);
         hDEtaAll[i][j] = new TH1F(Form("hDEtaAll%dVz%d",i,j),"",nDEtaBin,0,4);
         hDEtaBck[i][j] = new TH1F(Form("hDEtaBck%dVz%d",i,j),"",nDEtaBin,0,4);
      } 
   }
   
   // Signal and Sideband regions  
   double signalRegionCut = 1;
   double sideBandRegionCut = 2;
   TCut signalRegion = Form("abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut);
   TCut signalRegionEta = Form("abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut);
   TCut signalRegionInPhi = Form("abs(dphi)<%f",signalRegionCut);
   TCut sideBandRegionInPhi = Form("abs(dphi)>%f&&abs(dphi)<%f",signalRegionCut,sideBandRegionCut);
   TCut sideBandRegion =  Form("abs(dphi)>%f&&abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut,sideBandRegionCut);
   TCut sideBandRegionEta = Form("abs(dphi)>%f&&abs(dphi)<%f&&abs(deta)>1",signalRegionCut,sideBandRegionCut);
   TCut sideBandRegionEtaSignalRegion = Form("abs(dphi)>%f&&abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut,sideBandRegionCut);

//   TCanvas *cBetas = new TCanvas ("cBetas","",400,400);
  
   TrackletTree->Project("hEverything","vz[3]:nhit1:eta1",signalRegion,"",nentries,firstentry);

   h3->SetXTitle("#eta");
   h3->SetYTitle("#Delta#eta");

   TrackletTree->Project("h2","vz[3]:nhit1:eta1",sideBandRegionEtaSignalRegion,"",nentries,firstentry);   

   hEverything->Sumw2();
   h2->Sumw2();
   h3->Sumw2();


   // Beta calculation
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nHitBin;y++) {
         for (int z=1;z<=nVzBin;z++) {
            double beta = 0;
  	    if (hEverything->GetBinContent(x,y,z)!=0&&h2->GetBinContent(x,y,z)!=0) 
	    {   
	       beta = h2->GetBinContent(x,y,z)/hEverything->GetBinContent(x,y,z);
	       
               double e1 = hEverything->GetBinError(x,y,z)/hEverything->GetBinContent(x,y,z);
	       double e2 = h2->GetBinError(x,y,z)/h2->GetBinContent(x,y,z);
               double betaErr = beta* sqrt(e2*e2);
               if (beta/betaErr>-10) {
	          betas->Fill(x,y,z,beta,betaErr);
	          betaPlots[x-1][z-1]->SetBinContent(y,beta);
	          betaPlots[x-1][z-1]->SetBinError(y,betaErr);
	          betaErrPlots[x-1][z-1]->SetBinContent(y,betaErr);
	       }  

//	       fprintf(logFile,"beta(%2d,%2d,%2d) = %.2f +- %.2f    S = %6d, B = %6d \n",x,y,z,beta,betaErr,(int)hEverything->GetBinContent(x,y,z),(int)h2->GetBinContent(x,y,z));
	       beta = hEverything->GetBinContent(x,y,z)-h2->GetBinContent(x,y,z);
               hSubtracted->SetBinContent(x,y,z, beta);
	       double e3 = hEverything->GetBinError(x,y,z);
	       double e4 = h2->GetBinError(x,y,z);
               betaErr = sqrt(e1*e1+e2*e2);
               h3->SetBinContent(x,y,z, beta);
	       h3->SetBinError(x,y,z, betaErr);
	       hSubtracted->SetBinError(x,y,z, sqrt(e3*e3+e4*e4));
	    }
	 }      
      }
   }

   // Charged Hadrons:
   TCanvas *cHadrons = new TCanvas("cHadrons","",400,400);
   hHadron->SetXTitle("#eta");
   hHadron->SetYTitle("N_{hit}^{Layer1} |#eta|<1");
   TrackletTree->Project("hHadron","vz[3]:nhit1:eta","abs(eta)<3","",nentries,firstentry);
   hHadronAccepted = (TH3F*) hHadron->Clone();
   hHadronAccepted->SetName("hHadronAccepted");
   
   for (int j=0;j<nVzBin;j++) 
   {
//      TCanvas *cBetaFit = new TCanvas(Form("cBetaFit%d",j),Form("cBetaFit%d",j),800,800);
//      TPad *p1 = new TPad(Form("p1_%d",j),"",0,0,1,1);
//      p1->Divide(3,4,0.01,0.01);
//      p1->Draw();   
      for (int i=0;i<nEtaBin;i++) 
      {
      //c[i]= new TCanvas (Form("c%d",i),"",400,400);
//      p1->cd(i+1);
      formatHist(betaPlots[i][j],2,1);
      funBeta[i][j] = new TF1(Form("funBeta%dVz%d",i,j),"[1]/(x+[3]+0.5)+[2]/(x+0.5)/(x+0.5)+[0]",0,100);
      funBetaErr[i][j] = new TF1(Form("funBetaErr%d%-d",i,j),"[0]+[1]/(x+0.5)+[2]*exp([3]*x)",0,100);
      double etaMin = i*0.5 -3;
      double etaMax = i*0.5 -3+0.5;

      //betaPlots[i]->Fit(Form("funBeta%d",i),"m E");
      betaPlots[i][j]->SetXTitle("N_{Hits}");
      betaPlots[i][j]->SetYTitle(Form("#beta %.1f < #eta < %.1f",etaMin,etaMax)); 
      betaPlots[i][j]->SetAxisRange(0,1,"Y");
      betaPlots[i][j]->SetAxisRange(0,100,"X");
      betaPlots[i][j]->Draw("p");
      
      //fBeta[i] = betaPlots[i]->GetFunction(Form("funBeta%d",i));
      }
   }   

   // alpha calculation
   if (useCorrectionFile == 0) {   
      for (int x=1;x<=nEtaBin;x++) {
         for (int y=1;y<=nHitBin;y++) {
            for (int z=1;z<=nVzBin;z++) {
               double val = 0;
   	       if (hEverything->GetBinContent(x,y,z)!=0&&hHadron->GetBinContent(x,y,z)!=0) 
	       {   
	             val = hEverything->GetBinContent(x,y,z);
                     double beta = betaPlots[x-1][z-1]->GetBinContent(y);
	             double betaErr = betaPlots[x-1][z-1]->GetBinError(y);
	             h4->SetBinContent(x,y,z, val*(1-beta));
	             double e1 = hEverything->GetBinError(x,y,z);
                     double e2 = h2->GetBinError(x,y,z);
	             double nsig = val * (1-beta);
                     double valErr = sqrt(e1*e1+e2*e2);
                     h4->SetBinError(x,y,z, valErr);
                     if (beta>=1||betaErr==0) continue;
	             double truth    = hHadron->GetBinContent(x,y,z);
	             double truthErr = hHadron->GetBinError(x,y,z);
	             double alpha = truth/val/(1-beta);
	             double alphaErr = truth/nsig* sqrt(valErr/nsig*valErr/nsig+truthErr/truth*truthErr/truth*0);
//	             fprintf(logFile," beta (%2d,%2d,%2d) = %.2f +- %.2f",x,y,z,beta,betaErr);
//	             fprintf(logFile," alpha (%2d,%2d,%2d) = %.2f +- %.2f \n",x,y,z,alpha,alphaErr);
                     if (beta!=1&&alpha/alphaErr>-3&&alpha<20&&alpha>0) {
		        alphas->Fill(x,y,z,alpha,alphaErr);
	                alphaPlots[x-1][z-1]->SetBinContent(y,alpha);
	                alphaPlots[x-1][z-1]->SetBinError(y,alphaErr);
	                alphaErrPlots[x-1][z-1]->SetBinContent(y,alphaErr);
	             }
	       }
	    }      
         }
      }
   }

   // corrected calculation
   
   hTriggerCorrection = (TH1F*)fTriggerCorrection->FindObjectAny("h");

   if (useCorrectionFile != 0) {
      for (int i=0;i<nEtaBin;i++)
      {
         for (int j=0;j<nVzBin;j++)
         {
         fAlpha[i][j] = (TF1*)
	 fCorrection->FindObjectAny(Form("funAlpha%dVz%d",i,j));
	 fAlphaErr[i][j] = (TF1*)
	 fCorrection->FindObjectAny(Form("funAlphaErr%dVz%d",i,j));
         alphaPlots[i][j] = (TH1F*)
	 fCorrection->FindObjectAny(Form("alpha%dVz%d",i,j));
         }
      }
      hTruthRatio = (TH1D*) fCorrection->FindObjectAny("hTruthRatio");
   } else {


      for (int j=0;j<nVzBin;j++) 
      {
//         TCanvas *cAlphaFit = new TCanvas(Form("cAlphaFit%d",j),Form("cAlphaFit%d",j),800,800);
//         TPad *p2 = new TPad(Form("p2_%d",j),"",0,0,1,1);
//         p2->Divide(3,4,0.01,0.01);
//         p2->Draw();   
         for (int i=0;i<nEtaBin;i++) 
         {
         // c[i]= new TCanvas (Form("c%d",i),"",400,400);
         formatHist(alphaPlots[i][j],2,1);
//	 p2->cd(i+1);
         funAlpha[i][j] = new TF1(Form("funAlpha%dVz%d",i,j),"[1]/(x+[3]+0.5)+[2]/(x+0.5)/(x+0.5)+[0]",0,100);
         funAlphaErr[i][j] = new TF1(Form("funAlphaErr%dVz%d",i,j),"[0]+[1]/(x+0.5)+[2]*exp([3]*x)",0,100);
         double etaMin = i*0.5 -3;
         double etaMax = i*0.5 -3+0.5;
         alphaPlots[i][j]->Fit(Form("funAlpha%dVz%d",i,j),"m E Q","",0,60);
         alphaPlots[i][j]->SetXTitle("N_{Hits}");
         alphaPlots[i][j]->SetYTitle(Form("#alpha %.1f < #eta < %.1f",etaMin,etaMax)); 
         alphaPlots[i][j]->SetAxisRange(0.9,2.0,"y");
         alphaPlots[i][j]->SetAxisRange(0,100,"X");
	 if (i <2 || i >9) alphaPlots[i][j]->SetAxisRange(4,10,"y");
	 
	 alphaErrPlots[i][j]->Fit(Form("funAlphaErr%dVz%d",i,j),"m E Q","",0,20);
         alphaPlots[i][j]->Draw("p");
         fAlpha[i][j] = alphaPlots[i][j]->GetFunction(Form("funAlpha%dVz%d",i,j));  
         fAlphaErr[i][j] = alphaErrPlots[i][j]->GetFunction(Form("funAlphaErr%dVz%d",i,j));  
         }
      }  
   }
   
   for (int x=1;x<=nEtaBin;x++) {
   
       double totalN=0;
       double totalNErr=0;
          
      for (int y=1;y<=nHitBin;y++) {
         for (int z=1;z<=nVzBin;z++) {
         double val = 0;
	 if (y<1000) {   
	       val = hEverything->GetBinContent(x,y,z);
	       double beta = betaPlots[x-1][z-1]->GetBinContent(y);
	       double betaErr = betaPlots[x-1][z-1]->GetBinError(y);
               double alpha,alphaErr;
	       alpha = alphaPlots[x-1][z-1]->GetBinContent(y);
               alphaErr = alphaPlots[x-1][z-1]->GetBinError(y);
	       
	       if (val==0||alpha==0||alpha>20) hHadronAccepted->SetBinContent(x,y,z,0);
               if (val==0||alpha==0||alpha>20) continue;
	       if (alpha!=0&&(alphaErr/(alpha+0.00001)<1)) {
	       } else {
	          alpha = fAlpha[x-1][z-1]->Eval(y);
		  alphaErr = fAlphaErr[x-1][z-1]->Eval(y);
		  cout <<x<<" "<<y<<" extrapolate! "<<alpha<<" "<<alphaErr<<endl;
	       }
               
               double nCorrected = val*(1-beta)*alpha;
	       hSubtracted->SetBinContent(x,y,z,nCorrected/alpha);
	       hCorrected->SetBinContent(x,y,z, nCorrected);
               double nsig = hEverything->GetBinContent(x,y,z)*(1-beta);
	       
	       double valErr = alpha*sqrt(hEverything->GetBinContent(x,y,z))*(1-beta);
	       
	   

               hCorrected->SetBinError(x,y,z, valErr);
	       correction->Fill(x,y,nCorrected,hHadronAccepted->GetBinContent(x,y,z),valErr);
//	       correction->Fill(x,y,hEverything->GetBinContent(x,y,z),hHadronAccepted->GetBinContent(x,y,z)/alpha/(1-beta),sqrt(hEverything->GetBinContent(x,y,z)));
               totalN +=nCorrected;
	       totalNErr += valErr*valErr;	
	       cout <<x<<" "<<y<<" "<<z<<" "<<nCorrected<<" "<<valErr<<endl;  
	      
               hEtaVzRatio->Fill(x,z,nCorrected-hHadronAccepted->GetBinContent(x,y,z));     
	       hEtaHitRatio->Fill(x,y,nCorrected-hHadronAccepted->GetBinContent(x,y,z));
	       if (fabs(valErr)/nCorrected>1) {   
                  fprintf(logFile," beta (%3.1f,%3.1f,%3.1f) = %.2f +- %.2f",EtaBins[x],HitBins[y],VzBins[z],beta,betaErr);
	          fprintf(logFile," alpha = %.2f +- %.2f ",alpha,alphaErr);
	          fprintf(logFile," Value = %.1f +- %.1f Truth = %.1f \n",nCorrected,valErr,(double)hHadronAccepted->GetBinContent(x,y,z));
               }
	  }
	  }      
      }
      
      hCorrectedEtaBin->SetBinContent(x,totalN);
      hCorrectedEtaBin->SetBinError(x,sqrt(totalNErr));
      
   }
   
   TCanvas *cDNdEtaC = new TCanvas("cDNdEtaC","Before Trigger correction",400,400);
   double nevent = TrackletTree->GetEntries("nhit1");
   if (nentries<nevent) nevent=nentries;
   TH1D *hTruth = (TH1D*)hHadron->Project3D("x");
   hTruth->SetName("hTruth");
   TH1D *hTruthAccepted = (TH1D*)hHadronAccepted->Project3D("x");
   
   hTruth->Sumw2();
   hTruthAccepted->Sumw2();
   if (useCorrectionFile == 0) {
      // Calculate Acceptance
      hTruthRatio = (TH1D*)hHadron->Project3D("x");
      hTruthRatio->SetName("hTruthRatio");
      hTruthRatio->Sumw2();
      hTruthRatio->Divide(hTruthAccepted);
      hTruthRatio->SetName("hTruthRatio");
   } 
   
   formatHist(hTruthAccepted,1,nevent/nEtaBin*6);
   formatHist(hCorrectedEtaBin,2,nevent/nEtaBin*6);
   hTruthAccepted->SetAxisRange(0,5.5,"y");
   hTruthAccepted->SetXTitle("#eta (Calculated Hand)");
   hTruthAccepted->SetYTitle("dN/d#eta");
//   hTruthAccepted->Multiply(hTriggerCorrection);
   hTruthAccepted->Draw("hist");
//   hCorrectedEtaBin->Multiply(hTruthRatio);
   hCorrectedEtaBin->Draw("e same");    
   
   
   
   TCanvas *cDNdEta = new TCanvas("cDNdEta","Final result",400,400);
   TH1D *hMeasured = (TH1D*)hCorrected->Project3D("x"); 
   hMeasured->Sumw2();
   
   formatHist(hTruth,1,nevent/nEtaBin*6);
   formatHist(hMeasured,2,nevent/nEtaBin*6);
 
   hMeasured->Multiply(hTruthRatio);  // calibrate the acceptance

   TH1D *hRatio = (TH1D*)hMeasured->Clone();
   hRatio->Divide(hTruth);

   hTruth->Multiply(hTriggerCorrection);
   hMeasured->Multiply(hTriggerCorrection);

   hTruth->SetAxisRange(0,5.5,"y");
   hTruth->SetXTitle("#eta");
   hTruth->SetYTitle("dN/d#eta");
   hTruth->Draw("hist");
   TGraph *gErrorBand = GetErrorBand((TH1F*)hMeasured,0.09,0.25); 
   gErrorBand->Draw("F");
   hTruth->Draw("hist same");
   hMeasured->Draw("e same");    
   
     
   TLegend * leg1 = new TLegend(0.45,0.26,0.8,0.45);
   leg1->SetFillStyle(0);  
   leg1->SetFillColor(0); 
   leg1->SetBorderSize(0);
   leg1->SetTextSize(0.03);
   leg1->AddEntry(hTruth,myPlotTitle,"");
   leg1->AddEntry(hTruth,"MC Truth","l");
   leg1->AddEntry(hMeasured,"Reconstructed","pl");

   leg1->Draw();
    
   TCanvas *cRatio = new TCanvas("cRatio","Ratio",400,400);
   hRatio->SetXTitle("#eta");
   hRatio->SetYTitle("Ratio");
   hRatio->SetAxisRange(0.9,1.1,"y");
   hRatio->Draw();
   
   TLine *l1 = new TLine(-3,1,3,1);
   l1->Draw();

   TCanvas *cDNdNhit = new TCanvas("cDNdNhit","Measured vs mult",400,400);
   TH1D *hTruthHit = (TH1D*)hHadronAccepted->Project3D("y");
   TH1D *hMeasuredHit = (TH1D*)hCorrected->Project3D("y"); 
   hTruthHit->Sumw2();
   hMeasuredHit->Sumw2();
   
   formatHist(hTruthHit,1,nevent);
   formatHist(hMeasuredHit,2,nevent);
   hTruthHit->SetAxisRange(0,5.5,"y");
   hTruthHit->SetXTitle("N_{Hit1} |#eta|<1");
   hTruthHit->SetYTitle("dN/dN_{Hit1}");
   hTruthHit->Draw("hist");
   hMeasuredHit->Draw("e same");    
   
   TCanvas *cRatioNhit = new TCanvas("cRatioNHit","Ratio vs mult",400,400);
   TH1D *hRatioHit = (TH1D*)hMeasuredHit->Clone();
   hRatioHit->Divide(hTruthHit);
   hRatioHit->SetXTitle("#eta");
   hRatioHit->SetYTitle("Ratio");
   hRatioHit->SetAxisRange(0.7,1.3,"y");
   hRatioHit->Draw();
/*
   TCanvas *cRatioEtaNhit = new TCanvas("cRatioEtaNHit","ratio vs eta, mult",400,400);
   hEtaHitRatio->Draw("colz");
   TCanvas *cRatioVzNhit = new TCanvas("cRatioVzNHit","ratio vs eta, vz",400,400);
   hEtaVzRatio->Draw("colz");
  */ 
   TLine *l2 = new TLine(0,1,nHitBin,1);
   l2->Draw();

/*
   TCanvas *diff = new TCanvas ("diff","difference",400,400);
   hSubtracted->Scale(1./nevent); 
   hSubtracted->Draw("col");
*/
   TH1D *hSubtractedpx = (TH1D*)hSubtracted->Project3D("x");
   TH1D *hSubtractedpy = (TH1D*)hSubtracted->Project3D("y");

   hCorrected->Scale(1./nevent);

   hHadron->Scale(1./nevent);
   
   for (int i=0;i<nEtaBin;i++){
      for (int j=0;j<nVzBin;j++){
      fAlpha[i][j]->Write();
      // Don't save beta anymore.
      //fBeta[i]->Write();
      //fBetaErr[i]->Write();
      fAlphaErr[i][j]->Write();
      }
   }
   cDNdEta->Write();
   outf->Write();
   fclose(logFile);
}

void formatHist(TH1* h, int col, double norm){

  h->Scale(1/norm);
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->SetMarkerSize(0.7);
  h->GetYaxis()->SetTitleOffset(1.15);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();
  h->SetTitle("");
}
