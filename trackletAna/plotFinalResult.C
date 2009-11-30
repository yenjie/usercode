// Plot final results

// Standard library
#include <math.h>
#include <iostream.h>
#include <fstream>

// ROOT Library
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
#include <TLine.h>
#include <TF1.h>
#include <TCut.h>
#include <TPad.h>
#include <TText.h>


// For plotting
#include "GraphErrorsBand.h"

#include "selectionCut.h"


#define nEtaBin 12
#define nHitBin 14
#define nVzBin 10

#define canvasSizeX 400
#define canvasSizeY 400

#define plotDEta true



void formatHist(TH1* h, int col = 1, double norm = 1,double msize = 1);

//===========================================================================
// Read Stored correction factors
//===========================================================================

// Correction factor setting:
// 1 = 900 GeV
// 2 = 10 TeV
// 3 = 900 GeV (half of the sample)
// 4 = 10 TeV (half of the ample)


TFile* getCorrectionFile(int useCorrectionFile,int TrackletType){
   TFile *fCorrection;
   if (useCorrectionFile == 1) {
      fCorrection = new TFile(Form("correction/correction-%d-900GeV.root",TrackletType));
   } else if (useCorrectionFile == 2) {
      fCorrection = new TFile(Form("correction/correction-%d-10TeV.root",TrackletType));
   } else if (useCorrectionFile == 3) {
      fCorrection = new TFile(Form("correction/correction-%d-900GeV-half.root",TrackletType));
   } else if (useCorrectionFile == 4) {
      fCorrection = new TFile(Form("correction/correction-%d-10TeV-half.root",TrackletType));
   } else if (useCorrectionFile == 5) {
      fCorrection = new TFile(Form("correction/correction-%d-10TeV.root",TrackletType));
   } else if (useCorrectionFile == 11) {
      fCorrection = new TFile(Form("correction/correction-%d-900GeV-0Tesla.root",TrackletType));
   } else if (useCorrectionFile == 227) {
      fCorrection = new TFile(Form("correction/correction-%d-10TeV-227Generic.root",TrackletType));
   } else if (useCorrectionFile == 228) {
      fCorrection = new TFile(Form("correction/correction-%d-10TeV-227GenericWithoutDead.root",TrackletType));
   }
   return fCorrection;  
}


TFile* getTriggerCorrectionFile(int useCorrectionFile){
   TFile *fCorrection;
   if (useCorrectionFile == 1) {
      fCorrection = new TFile("correction/TriggerCorrection-900GeV.root");
   } else if (useCorrectionFile == 2) {
      fCorrection = new TFile("correction/TriggerCorrection-10TeV.root");
   } else if (useCorrectionFile == 3) {
      fCorrection = new TFile("correction/TriggerCorrection-900GeV.root");
   } else if (useCorrectionFile == 4) {
      fCorrection = new TFile("correction/TriggerCorrection-10TeV.root");
   } else if (useCorrectionFile == 5) {
      fCorrection = new TFile("");
   } else if (useCorrectionFile == 11) {
      fCorrection = new TFile("correction/TriggerCorrection-900GeV-0Tesla.root");
   } else if (useCorrectionFile ==227){
      fCorrection = new TFile("");
   } else {
      fCorrection = new TFile("");
   }
   return fCorrection;  
   
}


//===========================================================================
// Main Routine
//===========================================================================
int plotFinalResult(int TrackletType,char* filename,char *myPlotTitle="Random",int
useCorrectionFile = 0,  Long64_t nentries = 1000000000, Long64_t firstentry =
0,int verbose = 0,int makePlot = 0,int mcTruth = 1)
{
   FILE *logFile = fopen("correction.log","w");
   
   TFile *f= new TFile(filename);
   
   TTree * TrackletTree= dynamic_cast<TTree*>(f->Get(Form("TrackletTree%d",TrackletType)));

   // Read alpha, beta, geometry correction from file.
   TFile *fCorrection = getCorrectionFile(useCorrectionFile,TrackletType);
   TFile *fTriggerCorrection = getTriggerCorrectionFile(useCorrectionFile);
   
   // Definition of Vz, Eta, Hit bins
   double HitBins[nHitBin+1] = {0,5,10,15,20,25,30,35,40,50,60,80,100,200,700};
   double EtaBins[nEtaBin+1];
   double VzBins[nVzBin+1];
   
   for (int i=0;i<=nEtaBin;i++) EtaBins[i] = (double)i*6.0/(double)nEtaBin-3.0;;
   for (int i=0;i<=nVzBin;i++) VzBins[i] = (double)i*20.0/(double)nVzBin-10.0;


   // Signal and Sideband regions ==================================================================================================
   double signalRegionCut = 1.5;      //delta phi cut for signal region
   double sideBandRegionCut = 3;    //delta phi cut for sideband

   selectionCut myCut;
   
   TCut signalRegion                  = Form("abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut);
   TCut signalRegionInEta             = Form("abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut);
   TCut signalRegionInPhi             = Form("abs(dphi)<%f",signalRegionCut);
   TCut sideBandRegionInPhi           = Form("abs(dphi)>%f&&abs(dphi)<%f",signalRegionCut,sideBandRegionCut);
   TCut sideBandRegion                =  Form("abs(dphi)>%f&&abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut,sideBandRegionCut);
   TCut sideBandRegionEta          = Form("abs(dphi)>%f&&abs(dphi)<%f&&abs(deta)>1",signalRegionCut,sideBandRegionCut);
   TCut sideBandRegionEtaSignalRegion = Form("abs(dphi)>%f&&abs(dphi)<%f&&abs(deta)<0.1",signalRegionCut,sideBandRegionCut);
   TCut vzCut = myCut.Cut;   // cut on Z position 

   // Output file =================================================================================================================
   TFile *outf = new TFile ("correction.root","recreate");

   TNtuple * betas = new TNtuple("betas","","bin:nhit:vz:beta:betaErr");
   TNtuple * alphas = new TNtuple("alphas","","bin:nhit:vz:alpha:alphaErr");
   TNtuple * correction = new TNtuple("correction","","bin:nhit:obs:gen:err");
   
   TH3F *hEverything =
         new TH3F("hEverything","Everything in the signal region with vz cut",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *hReproducedBackground = 
         new TH3F("hReproducedBackground","Reproduced Background",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *hSubtracted = 
         new TH3F("hSubtracted","",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *hHadron = 
         new TH3F("hHadron","",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *hHadronAccepted = 
         new TH3F("hHadronAccepted","",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *hHadronWOSelection = 
         new TH3F("hHadronWOSelection","",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);
   TH3F *hCorrected = 
         new TH3F("hCorrected","",nEtaBin,EtaBins,nHitBin,HitBins,nVzBin,VzBins);

   TH2F *hEtaVzRatio = new TH2F("hEtaVzatio","#eta:Vz",nEtaBin,EtaBins,nVzBin,VzBins);
   TH2F *hEtaHitRatio = new TH2F("hEtaHitatio","#eta:N_{Hit}",nEtaBin,EtaBins,nHitBin,HitBins);
   TH2F *hAcceptance = new TH2F("hAcceptance","",nEtaBin,EtaBins,nVzBin,VzBins);

   TH1F *hCorrectedEtaBin = new TH1F("hCorrectedEtaBin","Corrected",nEtaBin,-3,3);
   TH1F *hTruthRatio;
   TH1F *hVz = new TH1F("hVz","",nVzBin,VzBins);
   TH1F *alphaPlots[nEtaBin][nVzBin];
   TH1F *betaPlots[nEtaBin][nVzBin];
   TH1F *alphaErrPlots[nEtaBin][nVzBin];
   TH1F *betaErrPlots[nEtaBin][nVzBin];
   TH1F *hTriggerCorrection;   

   // Prepare histograms ==========================================================================================================
   for (int i=0;i<nEtaBin;i++) {
      for (int j=0;j<nVzBin;j++) {
         alphaPlots[i][j] = new TH1F(Form("alpha%dVz%d",i,j),"",nHitBin,HitBins);
         alphaErrPlots[i][j] = new TH1F(Form("alphaErr%dVz%d",i,j),"",nHitBin,HitBins);
         betaPlots[i][j] = new TH1F(Form("beta%dVz%d",i,j),"",nHitBin,HitBins);
         betaErrPlots[i][j] = new TH1F(Form("betaErr%dVz%d",i,j),"",nHitBin,HitBins);
      }
   }

   // Fit functions of Beta and Alpha =============================================================================================
   TF1 *funBeta[nEtaBin][nVzBin];
   TF1 *funAlpha[nEtaBin][nVzBin];
   TF1 *funBetaErr[nEtaBin][nVzBin];
   TF1 *funAlphaErr[nEtaBin][nVzBin];
   TF1 *fAlpha[nEtaBin][nVzBin];
   TF1 *fAlphaErr[nEtaBin][nVzBin];
   
   // Acceptance calculation ======================================================================================================
   TCanvas *cVz = new TCanvas("cVz","Vz distribution",canvasSizeX,canvasSizeY);

   // Number of events //
   double nevent = TrackletTree->Draw("vz[1]",vzCut,"",nentries,firstentry);
   double neventWOSelection = TrackletTree->Draw("vz[1]","","",nentries,firstentry);

   TrackletTree->Project("hVz","vz[1]",vzCut,"",nentries,firstentry);   
   hVz->Sumw2();
   hVz->Scale(1./hVz->GetEntries());
   hVz->Fit("gaus");
   TF1 *fVz = hVz->GetFunction("gaus");
   hVz->SetXTitle("v_{z} (cm)");
   hVz->Draw();
   cVz->SaveAs(Form("plot/vz/plotVz-%s-%d.gif",myPlotTitle,TrackletType));
   cVz->SaveAs(Form("plot/vz/plotVz-%s-%d.eps",myPlotTitle,TrackletType));
   cVz->SaveAs(Form("plot/vz/plotVz-%s-%d.C",myPlotTitle,TrackletType));
   cout <<"Number of Nevents: "<<nevent;

   // Determine the acceptance region to avoid large correction factors
   double rho2 = 7.6;  // second layer rho (cm)

   // For Tracklet 1+3 or Tracklet 2+3
   if (TrackletType % 10 == 3) rho2 = 10.5;

   // End point in z (cm)
   double endpoint2 = 26.66; 
   
   for (int i=0;i<nEtaBin;i++) {
      for (int j=0;j<nVzBin;j++) {
         double minEta = EtaBins[i];
	 double maxEta = EtaBins[i+1];
	 double maxEdge = VzBins[j+1]-rho2 / tan(atan(exp(maxEta-0.1))*2);
	 double minEdge = VzBins[j]-rho2 / tan(atan(exp(minEta+0.1))*2);
	 if (verbose) cout <<minEta <<" "<<maxEta<<" "<<VzBins[j]<<" "<<maxEdge<<" "<<minEdge;
	 
	 if ((maxEdge>-endpoint2&&minEdge<endpoint2)) {
            hAcceptance->SetBinContent(i+1,j+1,hVz->GetBinContent(j+1));
	    hAcceptance->SetBinError(i+1,j+1,0);
	    if (verbose) cout <<" Selected!";
         } else {
	    hAcceptance->SetBinContent(i+1,j+1,0);
	 }
	 
	 if (verbose) cout <<" "<<endl;
      }
   }
   
   // Charged Hadrons =============================================================================================================
   hHadron->SetXTitle("#eta");
   hHadron->SetYTitle("N_{hit}^{Layer1} |#eta|<1");
   TrackletTree->Project("hHadron","vz[1]:nhit1:eta","abs(eta)<3"&&vzCut,"",nentries,firstentry);
   TrackletTree->Project("hHadronWOSelection","vz[1]:nhit1:eta","","",nentries,firstentry);
   hHadronAccepted = (TH3F*) hHadron->Clone();
   hHadronAccepted->SetName("hHadronAccepted");
 
   // Prepare Tracklet Three-Dimensional Histogram ================================================================================
   // signal region && vzCut //
   TrackletTree->Project("hEverything","vz[1]:nhit1:eta1",signalRegion&&vzCut,"",nentries,firstentry);
   hEverything->Sumw2();
   
   // deltaPhi sideband region (with eta signal region cut) && vzCut //
   TrackletTree->Project("hReproducedBackground","vz[1]:nhit1:eta1",sideBandRegionEtaSignalRegion&&vzCut,"",nentries,firstentry);   
   hReproducedBackground->Sumw2();
     
   // Beta calculation ============================================================================================================
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nHitBin;y++) {
         for (int z=1;z<=nVzBin;z++) {
            double beta = 0;
	    if (hAcceptance->GetBinContent(x,z)==0) continue;
  	    if (hEverything->GetBinContent(x,y,z)!=0&&hReproducedBackground->GetBinContent(x,y,z)!=0) 
	    {   
	       beta = hReproducedBackground->GetBinContent(x,y,z)/hEverything->GetBinContent(x,y,z);
	       
               double e1 = hEverything->GetBinError(x,y,z)/hEverything->GetBinContent(x,y,z);
	       double e2 = hReproducedBackground->GetBinError(x,y,z)/hReproducedBackground->GetBinContent(x,y,z);
               double betaErr = beta* sqrt(e2*e2);
               if (beta/betaErr>-10) {
	          betas->Fill(x,y,z,beta,betaErr);
	          betaPlots[x-1][z-1]->SetBinContent(y,beta);
	          betaPlots[x-1][z-1]->SetBinError(y,betaErr);
	          betaErrPlots[x-1][z-1]->SetBinContent(y,betaErr);
	       }  
	       beta = hEverything->GetBinContent(x,y,z)-hReproducedBackground->GetBinContent(x,y,z);
               hSubtracted->SetBinContent(x,y,z, beta);
	       double e3 = hEverything->GetBinError(x,y,z);
	       double e4 = hReproducedBackground->GetBinError(x,y,z);
               betaErr = sqrt(e1*e1+e2*e2);
               hSubtracted->SetBinError(x,y,z, sqrt(e3*e3+e4*e4));
	    }  else {
	       hAcceptance->GetBinContent(x,z,0);
	    }
	 }      
      }
   }
   
   
   for (int j=0;j<nVzBin;j++) 
   {
      for (int i=0;i<nEtaBin;i++) 
      {
      //c[i]= new TCanvas (Form("c%d",i),"",canvasSizeX,canvasSizeY);
      //p1->cd(i+1);
      formatHist(betaPlots[i][j],2,1);
      double etaMin = i*0.5 -3;
      double etaMax = i*0.5 -3+0.5;

      betaPlots[i][j]->SetXTitle("N_{Hits}");
      betaPlots[i][j]->SetYTitle(Form("#beta %.1f < #eta < %.1f",etaMin,etaMax)); 
      betaPlots[i][j]->SetAxisRange(0,1,"Y");
      betaPlots[i][j]->SetAxisRange(0,100,"X");
      //betaPlots[i][j]->Draw("p");
      }
   }   




   // alpha calculation ===================================================================================
   if (useCorrectionFile == 0) {   
      for (int x=1;x<=nEtaBin;x++) {
         for (int y=1;y<=nHitBin;y++) {
            for (int z=1;z<=nVzBin;z++) {
   	       if (hAcceptance->GetBinContent(x,z)==0) continue;

               double val = 0;
   	       if (hEverything->GetBinContent(x,y,z)!=0&&hHadron->GetBinContent(x,y,z)!=0) 
	       {   
	          val = hEverything->GetBinContent(x,y,z);
                  double beta = betaPlots[x-1][z-1]->GetBinContent(y);
	          double e1 = hEverything->GetBinError(x,y,z);
                  double e2 = hReproducedBackground->GetBinError(x,y,z);
	          double nsig = val * (1-beta);
                  double valErr = sqrt(e1*e1+e2*e2);
	          double truth    = hHadron->GetBinContent(x,y,z);
	          double truthErr = hHadron->GetBinError(x,y,z);
	          double alpha = truth/val/(1-beta);
	          double alphaErr = truth/nsig* sqrt(valErr/nsig*valErr/nsig+truthErr/truth*truthErr/truth*0);
                  if (beta!=1&&alpha/alphaErr>-3&&alpha<500&&alpha>0) {
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

   // Alpha correction calculation =======================================================================
   hTriggerCorrection = (TH1F*)fTriggerCorrection->FindObjectAny("h");

   if (useCorrectionFile != 0) {
      // use the alpha value obtained from the Correction file.
      for (int i=0;i<nEtaBin;i++)
      {
         for (int j=0;j<nVzBin;j++)
         {
         fAlpha[i][j] = (TF1*) fCorrection->FindObjectAny(Form("funAlpha%dVz%d",i,j));
	 fAlphaErr[i][j] = (TF1*) fCorrection->FindObjectAny(Form("funAlphaErr%dVz%d",i,j));
         alphaPlots[i][j] = (TH1F*) fCorrection->FindObjectAny(Form("alpha%dVz%d",i,j));
         }
      }
      hTruthRatio = (TH1F*) fCorrection->FindObjectAny("hTruthRatio");
   } else {
      for (int j=0;j<nVzBin;j++) 
      {
         for (int i=0;i<nEtaBin;i++) 
         {
         // c[i]= new TCanvas (Form("c%d",i),"",canvasSizeX,canvasSizeY);
         // p2->cd(i+1);
         formatHist(alphaPlots[i][j],2,1);
         funAlpha[i][j] = new TF1(Form("funAlpha%dVz%d",i,j),"[1]/(x+[3]+0.5)+[2]/(x+0.5)/(x+0.5)+[0]",0,100);
         funAlphaErr[i][j] = new TF1(Form("funAlphaErr%dVz%d",i,j),"[0]+[1]/(x+0.5)+[2]*exp([3]*x)",0,100);
         double etaMin = i*0.5 -3;
         double etaMax = i*0.5 -3+0.5;
         alphaPlots[i][j]->Fit(Form("funAlpha%dVz%d",i,j),"m E Q","",0,200);
         alphaPlots[i][j]->SetXTitle("N_{Hits}");
         alphaPlots[i][j]->SetYTitle(Form("#alpha %.1f < #eta < %.1f",etaMin,etaMax)); 
         alphaPlots[i][j]->SetAxisRange(0.9,2.0,"y");
         alphaPlots[i][j]->SetAxisRange(0,100,"X");
	 if (i <2 || i >9) alphaPlots[i][j]->SetAxisRange(4,10,"y");
	 
	 alphaErrPlots[i][j]->Fit(Form("funAlphaErr%dVz%d",i,j),"m E Q","",0,200);
         alphaPlots[i][j]->Draw("p");
         fAlpha[i][j] = alphaPlots[i][j]->GetFunction(Form("funAlpha%dVz%d",i,j));  
         fAlphaErr[i][j] = alphaErrPlots[i][j]->GetFunction(Form("funAlphaErr%dVz%d",i,j));  
         }
      }  
   }

   // make beta and alpha plot 
   if (makePlot) {

      // Beta plot
      for (int z=1;z<=nVzBin;z++) {
          TLegend * leg1 = new TLegend(0.63,0.75,0.93,0.93);
          leg1->SetFillStyle(0);  
          leg1->SetFillColor(0); 
          leg1->SetBorderSize(0);
          leg1->SetTextSize(0.04);

          TLegend * leg2 = new TLegend(0.26,0.75,0.56,0.93);
          leg2->SetFillStyle(0);  
          leg2->SetFillColor(0); 
          leg2->SetBorderSize(0);
          leg2->SetTextSize(0.04);

	  TCanvas *cc = new TCanvas(Form("cBetaPlot%d",z),"",canvasSizeX,canvasSizeY);
          for (int x=2;x<=nEtaBin-1;x++) {
             int color = x-1;
	     int mtype = 4;
	     if (color>5) {
	        color=11-color;
		mtype = 20;
             }
	     if (color==5) color=6;
	     
             formatHist(betaPlots[x-1][z-1],color);
	     if (betaPlots[x-1][z-1]->GetEntries()){
  	     betaPlots[x-1][z-1]->SetMarkerStyle(mtype);
	     betaPlots[x-1][z-1]->SetAxisRange(0,0.5,"Y");
             betaPlots[x-1][z-1]->Draw("L hist same");
             betaPlots[x-1][z-1]->Draw("e same");
	     betaPlots[x-1][z-1]->SetYTitle("#beta");
	     betaPlots[x-1][z-1]->SetXTitle(Form("N_{Hits} (%.0f #leq V_{z} < %.0f cm)",VzBins[z-1],VzBins[z]));
             if (x-1<nEtaBin/2) leg2->AddEntry(betaPlots[x-1][z-1],Form("%.1f #leq #eta<%.1f",EtaBins[x-1],EtaBins[x]),"pl");
                        else  leg1->AddEntry(betaPlots[x-1][z-1],Form("%.1f #leq #eta<%.1f",EtaBins[x-1],EtaBins[x]),"pl");
             }
          }
          leg1->Draw();
          leg2->Draw();
	  cc->SaveAs(Form("plot/betaPlot/betaPlot-%s-%d-%d.gif",myPlotTitle,z,TrackletType));
	  cc->SaveAs(Form("plot/betaPlot/betaPlot-%s-%d-%d.C",myPlotTitle,z,TrackletType));
	  cc->SaveAs(Form("plot/betaPlot/betaPlot-%s-%d-%d.eps",myPlotTitle,z,TrackletType));
          cc->Close();
      }

      for (int z=1;z<=nVzBin;z++) {
          TLegend * leg1 = new TLegend(0.63,0.75,0.93,0.93);
          leg1->SetFillStyle(0);  
          leg1->SetFillColor(0); 
          leg1->SetBorderSize(0);
          leg1->SetTextSize(0.04);

          TLegend * leg2 = new TLegend(0.26,0.75,0.56,0.93);
          leg2->SetFillStyle(0);  
          leg2->SetFillColor(0); 
          leg2->SetBorderSize(0);
          leg2->SetTextSize(0.04);

	  TCanvas *cc = new TCanvas(Form("cAlphaPlot%d",z),"",canvasSizeX,canvasSizeY);
	  cc->SetLogy();
          for (int x=2;x<=nEtaBin-1;x++) {
             int color = x-1;
	     int mtype = 4;
	     if (color>5) {
	        color=11-color;
		mtype = 20;
             }
	     if (color==5) color=6;
	     
             formatHist(alphaPlots[x-1][z-1],color);
	     if (alphaPlots[x-1][z-1]->GetEntries()){
  	     alphaPlots[x-1][z-1]->SetMarkerStyle(mtype);
	     alphaPlots[x-1][z-1]->SetYTitle("#alpha");
	     alphaPlots[x-1][z-1]->SetXTitle(Form("N_{Hits} (%.0f #leq V_{z} < %.0f cm)",VzBins[z-1],VzBins[z]));
	     alphaPlots[x-1][z-1]->SetStats(0);
	     alphaPlots[x-1][z-1]->GetYaxis()->SetMoreLogLabels(1);
	     alphaPlots[x-1][z-1]->SetAxisRange(0.9,12,"Y");
	     alphaPlots[x-1][z-1]->GetYaxis()->SetNoExponent(1);
	     
             alphaPlots[x-1][z-1]->Draw("L hist same");
             alphaPlots[x-1][z-1]->Draw("hist error p same");
             if (x-1<nEtaBin/2) leg2->AddEntry(alphaPlots[x-1][z-1],Form("%.1f #leq #eta<%.1f",EtaBins[x-1],EtaBins[x]),"pl");
                        else  leg1->AddEntry(alphaPlots[x-1][z-1],Form("%.1f #leq #eta<%.1f",EtaBins[x-1],EtaBins[x]),"pl");
             }
          }
          leg1->Draw();
          leg2->Draw();
	  cc->SaveAs(Form("plot/alphaPlot/alphaPlot-%s-%d-%d.gif",myPlotTitle,z,TrackletType));
	  cc->SaveAs(Form("plot/alphaPlot/alphaPlot-%s-%d-%d.C",myPlotTitle,z,TrackletType));
	  cc->SaveAs(Form("plot/alphaPlot/alphaPlot-%s-%d-%d.eps",myPlotTitle,z,TrackletType));
          cc->Close();

      }

   }

   // Apply correction ===============================================================================

   ofstream alphaCode;
   alphaCode.open("alpha.C");
   
   alphaCode <<"  const int nEtaBin = 12;"<<endl;
   alphaCode <<"  const int nHitBin = 14;"<<endl;
   alphaCode <<"  const int nVzBin  = 10;"<<endl;
   alphaCode <<endl;
   alphaCode <<"  double HitBins[nHitBin+1] = {0,5,10,15,20,25,30,35,40,50,60,80,100,200,700};"<<endl;
   alphaCode <<endl;
   alphaCode <<"  double EtaBins[nEtaBin+1];"<<endl;
   alphaCode <<"  for (int i=0;i<=nEtaBin;i++)"<<endl;
   alphaCode <<"    EtaBins[i] = (double)i*6.0/(double)nEtaBin-3.0;"<<endl;
   alphaCode <<"  double VzBins[nVzBin+1];"<<endl;
   alphaCode <<"  for (int i=0;i<=nVzBin;i++)"<<endl;
   alphaCode <<"    VzBins[i] = (double)i*20.0/(double)nVzBin-10.0;"<<endl;
   alphaCode <<endl;
   alphaCode <<"  double alpha[nEtaBin][nHitBin][nVzBin];"<<endl;
   alphaCode <<endl;
   alphaCode <<"  AlphaTracklets12_ = new TH3F(\"hAlphaTracklets12\",\"\",nEtaBin, EtaBins, nHitBin, HitBins, nVzBin, VzBins);"<<endl;
   alphaCode <<"  AlphaTracklets12_->SetDirectory(0);"<<endl;
   alphaCode <<endl;
   
   for (int x=1;x<=nEtaBin;x++) {
   
      double totalN=0;
      double totalNErr=0;
          
      for (int y=1;y<=nHitBin;y++) {
         for (int z=1;z<=nVzBin;z++) {
            double val = 0;
	    if (hAcceptance->GetBinContent(x,z)==0) hHadronAccepted->SetBinContent(x,y,z,0);
            if (hAcceptance->GetBinContent(x,z)==0) continue;
	    val = hEverything->GetBinContent(x,y,z);
	    double beta = betaPlots[x-1][z-1]->GetBinContent(y);
	    double betaErr = betaPlots[x-1][z-1]->GetBinError(y);
            double alpha,alphaErr;
	    alpha = alphaPlots[x-1][z-1]->GetBinContent(y);
            alphaErr = alphaPlots[x-1][z-1]->GetBinError(y);
	    //alphaCode <<" alpha["<<x-1<<"]["<<y-1<<"]["<<z-1<<"]="<<alpha<<";"<<endl;
            alphaCode <<"  AlphaTracklets12_->SetBinContent("<<x<<","<<y<<","<<z<<","<<alpha<<");"<<endl;

            // use extrapolated value if alpha is not available
 	    if (alpha==0) {
	       alpha = fAlpha[x-1][z-1]->Eval((HitBins[y]+HitBins[y-1])/2);
	       alphaErr = fAlphaErr[x-1][z-1]->Eval((HitBins[y]+HitBins[y-1])/2);
	       if (verbose) cout <<x<<" "<<y<<" extrapolate! "<<alpha<<" "<<alphaErr<<endl;
	    }
            
            double nCorrected = val*(1-beta)*alpha;
	    hSubtracted->SetBinContent(x,y,z,nCorrected/alpha);
	    hCorrected->SetBinContent(x,y,z, nCorrected);

	    double valErr = sqrt(alpha*(1-beta)*alpha*hEverything->GetBinContent(x,y,z)*(1-beta)
	                  + betaErr*betaErr*alpha*hEverything->GetBinContent(x,y,z)*alpha*hEverything->GetBinContent(x,y,z));
	    
	 

            hCorrected->SetBinError(x,y,z, valErr);
	    correction->Fill(x,y,nCorrected,hHadronAccepted->GetBinContent(x,y,z),valErr);
            totalN +=nCorrected;
	    totalNErr += valErr*valErr;      
	    if (verbose) cout <<x<<" "<<y<<" "<<z<<" "<<nCorrected<<" "<<valErr<<endl;  
	   
            hEtaVzRatio->Fill(x,z,nCorrected-hHadronAccepted->GetBinContent(x,y,z));	 
	    hEtaHitRatio->Fill(x,y,nCorrected-hHadronAccepted->GetBinContent(x,y,z));
	    if (fabs(valErr)/nCorrected>-1) {  
               fprintf(logFile," beta (%3.1f,%3.1f,%3.1f) = %.2f +- %.2f",EtaBins[x],HitBins[y],VzBins[z],beta,betaErr);
	       fprintf(logFile," alpha = %.2f +- %.2f ",alpha,alphaErr);
	       fprintf(logFile," Value = %.1f +- %.1f Truth = %.1f \n",nCorrected,valErr,(double)hHadronAccepted->GetBinContent(x,y,z));
            }
	  }      
      }
      
      hCorrectedEtaBin->SetBinContent(x,totalN);
      hCorrectedEtaBin->SetBinError(x,sqrt(totalNErr));
      
   }
   
   alphaCode.close();

   // Plot RawTracklet and Background Tracklet in Nhit bin
   TCanvas *cRawTrackletNhit = new TCanvas("cRawTrackletNhit","",canvasSizeX,canvasSizeY);

   TH1D *hMCTruthNhit = (TH1D*)hHadronAccepted->Project3D("y");
   hMCTruthNhit->Scale(1./nevent);
   hMCTruthNhit->SetXTitle("N_{Hit1}|#eta|<1"); 
   hMCTruthNhit->Draw("hist");

   TH1D *hRawTrackletNhit = (TH1D*)hEverything->Project3D("y");   
   hRawTrackletNhit->Scale(1./nevent);
   hRawTrackletNhit->Draw("same");
   TH1D *hBackgroundTrackletNhit = (TH1D*)hReproducedBackground->Project3D("y");   
   hBackgroundTrackletNhit->SetLineColor(2);
   hBackgroundTrackletNhit->SetMarkerColor(2);
   hBackgroundTrackletNhit->Scale(1./nevent);

   hBackgroundTrackletNhit->Draw("same");
   TH1D *hRawMinusBackgroundTrackletNhit = (TH1D*)hRawTrackletNhit->Clone();
   hRawMinusBackgroundTrackletNhit->SetLineColor(4);
   hRawMinusBackgroundTrackletNhit->SetMarkerColor(4);
   hRawMinusBackgroundTrackletNhit->Add(hBackgroundTrackletNhit,-1);
   hRawMinusBackgroundTrackletNhit->Draw("same");
     
   // Plot RawTracklet and Background Tracklet in Vz bin
   TCanvas *cRawTrackletVz = new TCanvas("cRawTrackletVz","",canvasSizeX,canvasSizeY);

   TH1D *hMCTruthVz = (TH1D*)hHadronAccepted->Project3D("z");
   hMCTruthVz->Scale(1./nevent);
   hMCTruthVz->Draw("hist");  
   hMCTruthVz->SetXTitle("V_{z}");

   TH1D *hRawTrackletVz = (TH1D*)hEverything->Project3D("z");   
   hRawTrackletVz->Scale(1./nevent);
   hRawTrackletVz->Draw("same");
   TH1D *hBackgroundTrackletVz = (TH1D*)hReproducedBackground->Project3D("z");   
   hBackgroundTrackletVz->SetLineColor(2);
   hBackgroundTrackletVz->SetMarkerColor(2);
   hBackgroundTrackletVz->Scale(1./nevent);
   hBackgroundTrackletVz->Draw("same");

   TH1D *hRawMinusBackgroundTrackletVz = (TH1D*)hRawTrackletVz->Clone();
   hRawMinusBackgroundTrackletVz->SetLineColor(4);
   hRawMinusBackgroundTrackletVz->SetMarkerColor(4);
   hRawMinusBackgroundTrackletVz->Add(hBackgroundTrackletVz,-1);
   hRawMinusBackgroundTrackletVz->Draw("same");


   // Plot RawTracklet and Background Tracklet in eta bin
   TCanvas *cRawTrackletEta = new TCanvas("cRawTrackletEta","",canvasSizeX,canvasSizeY);

   TH1D *hRawTrackletEta = (TH1D*)hEverything->Project3D("x");  
   TH1D *hMCTruthEta = (TH1D*)hHadronAccepted->Project3D("x");
   hMCTruthEta->Scale(2./nevent);
   hMCTruthEta->SetXTitle("#eta");
   hMCTruthEta->Draw("hist");
 
   hRawTrackletEta->Scale(2./nevent);
   hRawTrackletEta->Draw("same");
   TH1D *hBackgroundTrackletEta = (TH1D*)hReproducedBackground->Project3D("x");   
   hBackgroundTrackletEta->SetLineColor(2);
   hBackgroundTrackletEta->SetMarkerColor(2);
   hBackgroundTrackletEta->Scale(2./nevent);
   hBackgroundTrackletEta->Draw("same");

   TH1D *hRawMinusBackgroundTrackletEta = (TH1D*)hRawTrackletEta->Clone();
   hRawMinusBackgroundTrackletEta->SetLineColor(4);
   hRawMinusBackgroundTrackletEta->SetMarkerColor(4);
   hRawMinusBackgroundTrackletEta->Add(hBackgroundTrackletEta,-1);
   hRawMinusBackgroundTrackletEta->Draw("same");
   
   // Plot Before Acceptance correction
   TCanvas *cDNdEtaC = new TCanvas("cDNdEtaC","Before Acceptance correction",canvasSizeX,canvasSizeY);

   TH1D *hTruthAccepted = (TH1D*)hHadronAccepted->Project3D("x");
   hTruthAccepted->SetName("hTruthAccepted");
   TH1D *hTruth2 = (TH1D*)hHadron->Project3D("x");
   hTruth2->SetName("hTruth2");
   TH1D *hTruth = (TH1D*)hTruthAccepted->Clone();
   hTruth->SetName("hTruth");
   
   hTruth->Sumw2();
   hTruth2->Sumw2();
   hTruthAccepted->Sumw2();
  
   formatHist(hTruthAccepted,1,nevent/nEtaBin*6);
   formatHist(hCorrectedEtaBin,2,nevent/nEtaBin*6,1.5);
   hTruthAccepted->SetAxisRange(0,5.5,"y");
   hTruthAccepted->SetXTitle("#eta (Calculated Hand)");
   hTruthAccepted->SetYTitle("dN/d#eta");
   hTruthAccepted->Draw("hist");
   hCorrectedEtaBin->Draw("e same");    
   
   
   // Final dN/deta results =======================================================================================================   

   TCanvas *cDNdEta = new TCanvas("cDNdEta","Final result",canvasSizeX,canvasSizeY);
   TH1F *hMeasured = (TH1F*)hCorrected->Project3D("x"); 
   hMeasured->Sumw2();
   
   formatHist(hTruth,1,nevent/nEtaBin*6);
   formatHist(hTruth2,1,nevent/nEtaBin*6);
   formatHist(hMeasured,2,nevent/nEtaBin*6,1.5);
 
//   hMeasured->Multiply(hTruthRatio);  // calibrate the acceptance
   hTruth->Divide(hAcceptance->ProjectionX());  // calibrate the acceptance
   hMeasured->Divide(hAcceptance->ProjectionX());  // calibrate the acceptance
   
   TH1D *hTruthWOSelection = (TH1D*)hHadronWOSelection->Project3D("x");
   hTruthWOSelection->SetName("hTruthWOSelection");
   formatHist(hTruthWOSelection,4,neventWOSelection/nEtaBin*6);

   for (int it = 1;it <= nEtaBin; it++) {
      if (hTruth->GetBinContent(it)==0) hTruth->SetBinContent(it,hTruth2->GetBinContent(1));  //get back the first bin
   }

   TH1F *hRatio = (TH1F*)hMeasured->Clone();
   hRatio->Divide(hTruth2);

   hTruth2->Multiply(hTriggerCorrection);
   hMeasured->Multiply(hTriggerCorrection);

   hTruth2->SetAxisRange(0,5.5,"y");
   hTruth2->SetXTitle("#eta");
   hTruth2->SetYTitle("dN/d#eta");
   hTruth2->Draw("hist");
   
   double systematicError900GeV[12] = 
                  {0.129,0.129,0.088,0.082,0.082,0.079,0.079,0.082,0.082,0.088,0.129,0.129};

   double systematicError10TeV[12] = 
                  {0.132,0.132,0.093,0.085,0.084,0.083,0.083,0.084,0.085,0.093,0.132,0.132};


   TGraph *gErrorBand;
   if (useCorrectionFile == 2 || useCorrectionFile == 4){ 
      gErrorBand = GetErrorBand((TH1F*)hMeasured,systematicError10TeV,systematicError10TeV,0.25); 
   } else {
      gErrorBand = GetErrorBand((TH1F*)hMeasured,systematicError900GeV,systematicError900GeV,0.25); 
   }
//   gErrorBand->Draw("F");
   hTruth2->Draw("hist same");
   hTruthWOSelection->Draw("hist same");
   hMeasured->Draw("e same");    
   TH1F *hMeasuredStat = (TH1F*)hMeasured->Clone();
   hMeasuredStat->SetName("hMeasuredStat");
   hMeasuredStat->SetMarkerSize(0);
   hMeasuredStat->Draw("e same");
   
     
   TLegend * leg1 = new TLegend(0.30,0.18,1,0.3);
   leg1->SetFillStyle(0);  
   leg1->SetFillColor(0); 
   leg1->SetBorderSize(0);
   //leg1->SetTextSize(0.045);
   leg1->AddEntry(hTruth,Form("%s",myPlotTitle),"");
   //leg1->AddEntry(hTruth,"MC Truth","l");
   leg1->AddEntry(hMeasured,"Reconstructed","pl");

   leg1->Draw();
   
   TText *   tex = new TText(-2.6,5,"CMS Preliminary");
   tex->Draw();

   cDNdEta->SaveAs(Form("plot/result/result-%s-%d.gif",myPlotTitle,TrackletType));
   cDNdEta->SaveAs(Form("plot/result/result-%s-%d.eps",myPlotTitle,TrackletType));
   cDNdEta->SaveAs(Form("plot/result/result-%s-%d.C",myPlotTitle,TrackletType));


   // Ratio between measured and truth ===========================================================================================    
   TCanvas *cRatio = new TCanvas("cRatio","Ratio",canvasSizeX,canvasSizeY);
   hRatio->SetXTitle("#eta");
   hRatio->SetYTitle("Ratio");
   hRatio->SetAxisRange(0.8,1.2,"y");
   hRatio->Draw();
   TH1F *hRatioStat = (TH1F*)hRatio->Clone();
   hRatioStat->SetName("hRatioStat");
   hRatioStat->SetMarkerSize(0);
   hRatioStat->Draw("same");

   TLegend * leg2 = new TLegend(0.18,0.18,1,0.3);
   leg2->SetFillStyle(0);  
   leg2->SetFillColor(0); 
   leg2->SetBorderSize(0);
   //leg2->SetTextSize(0.045);
   leg2->AddEntry(hTruth,myPlotTitle,"");
   leg2->AddEntry(hRatio,"Reconstructed / MC Truth","pl");

   leg2->Draw();

  
   TLine *l1 = new TLine(-3,1,3,1);
   l1->Draw();
   
   TText *   tex2 = new TText(-2.6,1.165,"CMS Preliminary");
   tex2->Draw();

   cRatio->SaveAs(Form("plot/ratio/ratio-%s-%d.gif",myPlotTitle,TrackletType));
   cRatio->SaveAs(Form("plot/ratio/ratio-%s-%d.eps",myPlotTitle,TrackletType));
   cRatio->SaveAs(Form("plot/ratio/ratio-%s-%d.C",myPlotTitle,TrackletType));
   

   TCanvas *cDNdNhit = new TCanvas("cDNdNhit","Measured vs mult",canvasSizeX,canvasSizeY);
   TH1D *hTruthHit = (TH1D*)hHadronAccepted->Project3D("y");
   TH1D *hMeasuredHit = (TH1D*)hCorrected->Project3D("y"); 
   hTruthHit->Sumw2();
   hMeasuredHit->Sumw2();
   
   formatHist(hTruthHit,1,nevent);
   formatHist(hMeasuredHit,2,nevent,1.5);
   hTruthHit->SetAxisRange(0,5.5,"y");
   hTruthHit->SetXTitle("N_{Hit1} |#eta|<1");
   hTruthHit->SetYTitle("dN/dN_{Hit1}");
   hTruthHit->Draw("hist");
   hMeasuredHit->Draw("e same");    
   
   TCanvas *cRatioNhit = new TCanvas("cRatioNHit","Ratio vs mult",canvasSizeX,canvasSizeY);
   TH1D *hRatioHit = (TH1D*)hMeasuredHit->Clone();
   hRatioHit->Divide(hTruthHit);
   hRatioHit->SetXTitle("#eta");
   hRatioHit->SetYTitle("Ratio");
   hRatioHit->SetAxisRange(0.7,1.3,"y");
   hRatioHit->Draw();
/*
   TCanvas *cRatioEtaNhit = new TCanvas("cRatioEtaNHit","ratio vs eta, mult",canvasSizeX,canvasSizeY);
   hEtaHitRatio->Draw("colz");
   TCanvas *cRatioVzNhit = new TCanvas("cRatioVzNHit","ratio vs eta, vz",canvasSizeX,canvasSizeY);
   hEtaVzRatio->Draw("colz");
  */ 
   TLine *l2 = new TLine(0,1,nHitBin,1);
   l2->Draw();

/*
   TCanvas *diff = new TCanvas ("diff","difference",canvasSizeX,canvasSizeY);
   hSubtracted->Scale(1./nevent); 
   hSubtracted->Draw("col");
*/
   TH1D *hSubtractedpx = (TH1D*)hSubtracted->Project3D("x");
   TH1D *hSubtractedpy = (TH1D*)hSubtracted->Project3D("y");

   hCorrected->Scale(1./nevent);

   hHadron->Scale(1./nevent);

   for (int i=0;i<nEtaBin;i++){
      for (int j=0;j<nVzBin;j++){
         if (fAlpha[i][j]) fAlpha[i][j]->Write(); 
         // Don't save beta anymore.
         //fBeta[i]->Write();
         //fBetaErr[i]->Write();
         if (fAlphaErr[i][j]) fAlphaErr[i][j]->Write(); 
         if (verbose&&!fAlpha[i][j]) cout <<"no alpha! "<<i<<" "<<j<<endl;
         if (verbose&&!fAlphaErr[i][j]) cout <<"no alphaErr! "<<i<<" "<<j<<endl;
      }
   }
   cDNdEta->Write();
   outf->Write();
   fclose(logFile);
}


//===========================================================================
// Format Histogram
//===========================================================================
void formatHist(TH1* h, int col, double norm,double msize){

  h->Scale(1/norm);
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->SetMarkerSize(1.1);
  h->GetYaxis()->SetTitleOffset(1.25);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();
  h->SetTitle("");
}
