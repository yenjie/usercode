// Plot final results

// Standard library
#include <math.h>
#include <iostream>
#include <fstream>

// ROOT Library
#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TH1D.h>
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
#include "UA5Plot.h"
//#include "PythiaPlot.h"
#include "GraphErrorsBand.h"

#include "selectionCut.h"

#define canvasSizeX 600
#define canvasSizeY 600

#define plotDEta true

#define dndetaRange 2500.0

#define SDFactor 1


void formatHist(TH1* h, int col = 1, double norm = 1,double msize = 4);

//===========================================================================
// Read Stored correction factors
//===========================================================================

// Correction factor setting:
// 1 = 900 GeV
// 2 = 10 TeV
// 3 = 900 GeV (half of the sample)
// 4 = 10 TeV (half of the ample)

TFile* getAcceptanceFile(int TrackletType){
   TFile *fAcceptance;
   char * fileName = Form("correction/Acceptance-%d.root",TrackletType);
   fAcceptance = new TFile(fileName);
   cout <<"Use Acceptance file : "<<fileName<<endl;
   return fAcceptance;  
}



TFile* getCorrectionFile(string correctionfileName,int TrackletType){
   TFile *fCorrection;
   char * fileName = Form("correction/correction-%d-%s.root",TrackletType,correctionfileName.data());
   fCorrection = new TFile(fileName);
   cout <<"Use correction file : "<<fileName<<endl;
   return fCorrection;  
}


//===========================================================================
// Main Routine
//===========================================================================
int plotFinalResult(int TrackletType,						// Tracklet Type 12 23 13
                    char* fileName,                                             // Input file name
                    char* bckFileName,                                          // Input bck file name
                    char *myPlotTitle="Random",                                 // Title of the plot
		    bool useCorrectionFile = 1,                                 // use Correction file
		    string correctionName = "Official",                         // Correction file name
		    Long64_t nentries = 1000000000,                             // Number of entries
		    Long64_t firstentry = 0,                                    // First Entry
		    int LumiL = 0,                                             // nLumi Lower cut
		    int LumiH = 1000,                                            // nLumi Higher cut
		    int centrality = 2,                                         // centrality
		    int verbose = 0,                                            // Set Verbose level
		    int makePlot = 0,                                           // make alpha plots
		    bool putUA5 = 0,                                            // overlap UA5 result
		    bool doAcceptanceCorrection = 0,                            // do acceptance correction
  		    bool doBetaCorrection = 0,                                  // do acceptance correction
		    int doMult2 = 0,                                           // use # of cluster in the first layer
		    bool doTriggerCorrection = 1,                                // correction on trigger eff
                    int UseExternalSDEff = 0,
		    bool useDR = 0,
		    bool useWeight2 = 1  
		   )
{

   // output log file:
   FILE *logFile = fopen("correction.log","w");
   TFile *f= new TFile(fileName);
   
   // Input trackletTree   
   TTree * TrackletTree= dynamic_cast<TTree*>(f->Get(Form("TrackletTree%d",TrackletType)));
   TrackletTree->SetName("TrackletTree");

   // background shape file:
   TFile *f2= new TFile(bckFileName);
   TTree * bckTrackletTree= dynamic_cast<TTree*>(f2->Get(Form("TrackletTree%d",TrackletType)));
   bckTrackletTree->SetName("bckTrackletTree");
   
   bool isMC = false;
   if (TrackletTree->GetEntries("npart!=0")!=0) {
      isMC=true;
      cout <<"This is a Monte Carlo study."<<endl;
      doAcceptanceCorrection = 0;
   } else {
      cout <<"This is a data analysis."<<endl;
   }
   
   if (!doTriggerCorrection) {
      cout <<"Trigger correction off!!!"<<endl;
   }
   
   // choose multiplcity handle
   char *multiplicity;
   if (doMult2==1) {
      multiplicity = "mult2";   // use # of cluster
      cout <<"Use # of clusters as event multiplicity"<<endl;
   } else if (doMult2==2){
      multiplicity = "nTracklets";    // use # of Background subtracted tracklets
      cout <<"Use # of tracklets as event multiplicity"<<endl;
   } else {
      multiplicity = "mult";    // use # of Background subtracted tracklets
      cout <<"Use # of tracklets after background subtraction as event multiplicity"<<endl;
   }
   
   char *weight2;
   if (useWeight2) weight2="weight2"; else weight2="1";
      
   // Read alpha, beta, geometry correction from file.
   TFile *fCorrection;
   TFile *fAcceptance;
   if (useCorrectionFile) fCorrection = getCorrectionFile(correctionName,TrackletType);
   if (useCorrectionFile&&doAcceptanceCorrection) fAcceptance = getAcceptanceFile(TrackletType);
   
   TFile *fCorrectionExternal = new TFile(Form("correction/correction-%d-PHOJET-7TeV-HF1.root",TrackletType));
//   TFile *fCorrectionExternal = new TFile(Form("correction/correction-%d-ATLAS-7TeV-HF1.root",TrackletType));
   
   TH3D *hAlphaA;
   TH3D *hAlphaB;

   if (doBetaCorrection) {
      TFile *myFile = new TFile(Form("correction/alphaBetaCoeff-%d.root",TrackletType));
      hAlphaA = (TH3D*) myFile->FindObjectAny("hAlphaA");
      hAlphaB = (TH3D*) myFile->FindObjectAny("hAlphaB");
   }

   // Definition of Vz, Eta, Hit bins
   selectionCut myCut(isMC,centrality,LumiL,LumiH);
   const int nTrackletBin =9;// myCut.nTrackletBin;
   const int nEtaBin =12;// myCut.nEtaBin;
   const int nVzBin  =20;// myCut.nVzBin;
   int VzRangeL =myCut.VzRangeL;
   int VzRangeH =myCut.VzRangeH;
//   double TrackletBins[nTrackletBin+1] = {-5,2,4,6,8,10,15,20,25,30,35,40,45,50,300};

   double TrackletBins[nTrackletBin+1] = {-100,50,100,400,1000,2000,4000, 6000, 8000, 20000};
   double EtaBins[nEtaBin+1];
   double VzBins[nVzBin+1];
   
   for (int i=0;i<=nEtaBin;i++) EtaBins[i] = (double)i*6.0/(double)nEtaBin-3.0;;
   for (int i=0;i<=nVzBin;i++) VzBins[i] = (double)i*(VzRangeH-VzRangeL)/(double)nVzBin+VzRangeL;


   // Signal and Sideband regions ==================================================================================================
   double signalRegionCut = 0.5;      //delta phi cut for signal region
   double sideBandRegionCut = 2.0;    //delta phi cut for sideband
   double detaCut = 0.1;              //delta eta cut
   
   TCut signalRegion                  = Form("abs(dphi)<%f&&abs(deta)<%f",signalRegionCut,detaCut);
   TCut sideBandRegionEtaSignalRegion = Form("abs(dphi)>%f&&abs(dphi)<%f&&abs(deta)<%f",signalRegionCut,sideBandRegionCut,detaCut);

   if (useDR) {
      cout <<"Use dR for analysis"<<endl;
      signalRegion                  = "dR<0.1";
      sideBandRegionEtaSignalRegion = "dR>0.1&&dR<0.2";
   }

   TCut evtSelection = myCut.Cut;   // cut on Z position 
   TCut NSDCut = "1";
   if (centrality==1) NSDCut = "hf>88496.5";
   if (centrality==2) NSDCut = "hf<88496.5&&hf>60126.7";
   if (centrality==3) NSDCut = "hf<60126.7&&hf>38575.3";
   if (centrality==4) NSDCut = "hf<38575.3&&hf>23921.4";
   if (centrality==5) NSDCut = "hf<23921.4&&hf>13801.3";
   if (centrality==6) NSDCut = "hf<13801.3&&hf>7533.57";
   
  // TCut NSDCut = "evtType!=92&&evtType!=93"; cout <<"PYTHIA MC NSD definition"<<endl;
//   TCut NSDCut = "1"; cout <<"PYTHIA MC INEL definition"<<endl;

//   TCut NSDCut = "!(evtType!=92&&evtType!=93)"; cout <<"PYTHIA MC SD definition"<<endl; // SD
//   TCut NSDCut = "evtType!=5&&evtType!=6"; cout <<"PHOJET MC definition"<<endl; // NSD
   if (!isMC) NSDCut="1";
//   if (1) evtSelection+=NSDCut;
   // Output file =================================================================================================================
   TFile *outf = new TFile ("correction.root","recreate");

   TNtuple * betas = new TNtuple("betas","","eta:nTracklet:vz:beta:betaErr");
   TNtuple * alphas = new TNtuple("alphas","","eta:nTracklet:vz:alpha:alphaErr");
   TNtuple * correction = new TNtuple("correction","","eta:nTracklet:vz:alpha:alphaErr:beta:betaErr:obs:gen:err:betaMC");

   TH3D *hEverything =
         new TH3D("hEverything","Everything in the signal region with vz cut",nEtaBin,EtaBins,nTrackletBin,TrackletBins,nVzBin,VzBins);
   TH3D *hReproducedBackground = 
         new TH3D("hReproducedBackground","Reproduced Background",nEtaBin,EtaBins,nTrackletBin,TrackletBins,nVzBin,VzBins);


   TH3D *hSubtracted = 
         new TH3D("hSubtracted","",nEtaBin,EtaBins,nTrackletBin,TrackletBins,nVzBin,VzBins);
   TH3D *hHadron = 
         new TH3D("hHadron","",nEtaBin,EtaBins,nTrackletBin,TrackletBins,nVzBin,VzBins);
   TH3D *hHadronAccepted = 
         new TH3D("hHadronAccepted","",nEtaBin,EtaBins,nTrackletBin,TrackletBins,nVzBin,VzBins);
   TH3D *hHadronWOSelection = 
         new TH3D("hHadronWOSelection","",nEtaBin,EtaBins,nTrackletBin,TrackletBins,nVzBin,VzBins);
   TH3D *hCorrected = 
         new TH3D("hCorrected","",nEtaBin,EtaBins,nTrackletBin,TrackletBins,nVzBin,VzBins);
   TH3D *hnTracklet3D = new TH3D("hnTracklet3D","",nEtaBin,EtaBins,nTrackletBin,TrackletBins,nVzBin,VzBins);

   TH2D *hEtaVzRatio = new TH2D("hEtaVzatio","#eta:Vz",nEtaBin,EtaBins,nVzBin,VzBins);
   TH2D *hEtaHitRatio = new TH2D("hEtaHitatio","#eta:N_{Hit}",nEtaBin,EtaBins,nTrackletBin,TrackletBins);
   TH2D *hAcceptance = new TH2D("hAcceptance","",nEtaBin,EtaBins,nVzBin,VzBins);

   TH2D *hBckSignal = 
         new TH2D("hBckSignal","Bck Tree signal region",nEtaBin,EtaBins,nVzBin,VzBins);
   TH2D *hBckSideband = 
         new TH2D("hBckSideband","Bck Tree signal region",nEtaBin,EtaBins,nVzBin,VzBins);

   
   // Acceptance
   TH2D *hDataAcc = new TH2D("hDataAcc","",nEtaBin,EtaBins,nVzBin,VzBins);
   TH2D *hMCAcc = new TH2D("hMCAcc","",nEtaBin,EtaBins,nVzBin,VzBins);
   
   TH2D *hVzNTracklet = new TH2D("hVzNTracklet","",nTrackletBin,TrackletBins,nVzBin,VzBins);

   TH1D *hCorrectedEtaBin = new TH1D("hCorrectedEtaBin","Corrected",nEtaBin,-3,3);
   TH1D *hDNDEtaVertexed = new TH1D("hDNDEtaVertexed","",nEtaBin,-3,3);
   TH1D *hDNDEtaNoVertexed = new TH1D("hDNDEtaNoVertexed","",nEtaBin,-3,3);
   TH1D *hTrigEff = new TH1D("hTrigEff","",nTrackletBin,TrackletBins);
   TH1D *hTrigEffNoCut = new TH1D("hTrigEffNoCut","",nTrackletBin,TrackletBins);
   TH1D *hSD = new TH1D("hSD","",nTrackletBin,TrackletBins);
   TH1D *hSDFrac = new TH1D("hSDFrac","",nTrackletBin,TrackletBins);
   TH1D *hTruthRatio;
   TH1D *hVz = new TH1D("hVz","",nVzBin,VzBins);
   TH1D *hnTracklet = new TH1D("hnTracklet","",nTrackletBin,TrackletBins);


   TH1D *alphaPlots[nEtaBin][nVzBin];
   TH1D *betaPlots[nEtaBin][nVzBin];
   TH1D *betaMCPlots[nEtaBin][nVzBin];
   TH1D *alphaErrPlots[nEtaBin][nVzBin];
   TH1D *betaErrPlots[nEtaBin][nVzBin];
   
   TH1D *hTriggerCorrection;   
   TH1D *hEmptyEvtCorrection;

   // Prepare histograms ==========================================================================================================
   for (int i=0;i<nEtaBin;i++) {
      for (int j=0;j<nVzBin;j++) {
         alphaPlots[i][j] = new TH1D(Form("alpha%dVz%d",i,j),"",nTrackletBin,TrackletBins);
         alphaErrPlots[i][j] = new TH1D(Form("alphaErr%dVz%d",i,j),"",nTrackletBin,TrackletBins);
         betaPlots[i][j] = new TH1D(Form("beta%dVz%d",i,j),"",nTrackletBin,TrackletBins);
         betaErrPlots[i][j] = new TH1D(Form("betaErr%dVz%d",i,j),"",nTrackletBin,TrackletBins);
         betaMCPlots[i][j] = new TH1D(Form("betaMC%dVz%d",i,j),"",nTrackletBin,TrackletBins);
      }
   }

   // Fit functions of Beta and Alpha =============================================================================================
   TF1 *funAlpha[nEtaBin][nVzBin];
   TF1 *funAlphaErr[nEtaBin][nVzBin];
   TF1 *fAlpha[nEtaBin][nVzBin];
   TF1 *fAlphaErr[nEtaBin][nVzBin];
   
   // Acceptance calculation ======================================================================================================
   TCanvas *cVz = new TCanvas("cVz","Vz distribution",canvasSizeX,canvasSizeY);

   // Number of events //
   double nevent = TrackletTree->Draw("vz[1]",evtSelection&&NSDCut,"",nentries,firstentry);
   double neventWOSelection = TrackletTree->Draw("vz[1]",NSDCut,"",nentries,firstentry);
   if (nevent==0) {
      cout <<"No event survived. Abort."<<endl;
      return 0;
   }
  
   TrackletTree->Project("hnTracklet",Form("%s",multiplicity),evtSelection,"",nentries,firstentry);   
   TrackletTree->Project("hVzNTracklet",Form("vz[1]:%s",multiplicity),evtSelection,"",nentries,firstentry);   

   TrackletTree->Project("hVz","vz[1]",evtSelection,"",nentries,firstentry);   
   hVz->Sumw2();
   hVz->Scale(1./hVz->GetEntries());
   hVz->Fit("gaus");
   hVz->SetXTitle("v_{z} (cm)");
   hVz->Draw();
   cVz->SaveAs(Form("plot/vz/plotVz-%s-%d.gif",myPlotTitle,TrackletType));
   cVz->SaveAs(Form("plot/vz/plotVz-%s-%d.eps",myPlotTitle,TrackletType));
   cVz->SaveAs(Form("plot/vz/plotVz-%s-%d.C",myPlotTitle,TrackletType));
   cout <<"Number of events: "<<nevent<<endl;
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
//	 double maxEdge = VzBins[j+1]-rho2 / tan(atan(exp(maxEta))*2);
//	 double minEdge = VzBins[j]-rho2 / tan(atan(exp(minEta))*2);
	 if (verbose) cout <<minEta <<" "<<maxEta<<" "<<VzBins[j]<<" "<<maxEdge<<" "<<minEdge;
	 
         for (int k=0;k<nTrackletBin;k++) {
	    if((maxEdge>-endpoint2&&minEdge<endpoint2)&&(maxEta)<=2.5&&(minEta)>=-2.5) {
//               hAcceptance->SetBinContent(i+1,j+1,hAcceptance->GetBinContent(i+1,j+1)+hVzNTracklet->GetBinContent(k+1,j+1)/hVzNTracklet->GetEntries());
               hAcceptance->SetBinContent(i+1,j+1,hVz->GetBinContent(j+1));
               hnTracklet3D->SetBinContent(i+1,k+1,j+1,hVzNTracklet->GetBinContent(k+1,j+1));
	       hAcceptance->SetBinError(i+1,j+1,0);
	       if (verbose) cout <<" Selected!";
            } else {
               hnTracklet3D->SetBinContent(i+1,j+1,0);
	       hAcceptance->SetBinContent(i+1,j+1,0);
	    }
	    if (verbose) cout <<" "<<endl;
	 }
	 
      }
   }

   TCanvas *cAcc = new TCanvas("cAcc","Acc",canvasSizeX,canvasSizeY);
   hAcceptance->ProjectionX()->Draw("");
   cAcc->Update();
    
   // Charged Hadrons =============================================================================================================
   hHadron->SetXTitle("#eta");
   hHadron->SetYTitle("N_{hit}^{Layer1} |#eta|<1");
   
   TrackletTree->Project("hHadron",Form("vz[1]:%s:eta",multiplicity),weight2*evtSelection,"",nentries,firstentry);
   TrackletTree->Project("hHadronWOSelection",Form("vz[1]:%s:eta",multiplicity),weight2*NSDCut,"",nentries,firstentry);
   hHadron->Sumw2();
   hHadronWOSelection->Sumw2();
   
   TrackletTree->Project("hHadronAccepted",Form("vz[1]:%s:eta",multiplicity),weight2*evtSelection,"",nentries,firstentry);
   hHadronAccepted = (TH3D*) hHadron->Clone();
   hHadronAccepted->SetName("hHadronAccepted");
 
   // Prepare Tracklet Three-Dimensional Histogram ================================================================================
   // signal region && evtSelection //
   TrackletTree->Project("hEverything",Form("vz[1]:%s:eta1",multiplicity),"weight"*(signalRegion&&evtSelection),"",nentries,firstentry);
   hEverything->Sumw2();

   // deltaPhi sideband region (with eta signal region cut) && evtSelection //
   TrackletTree->Project("hReproducedBackground",Form("vz[1]:%s:eta1",multiplicity),"weight"*(sideBandRegionEtaSignalRegion&&evtSelection),"",nentries,firstentry);   
   hReproducedBackground->Sumw2();

   // bckTree: signal region && evtSelection //
   bckTrackletTree->Project("hBckSignal","vz[1]:eta1","weight"*(signalRegion&&evtSelection),"",nentries,firstentry);
   hBckSignal->Sumw2();
   
   // bckTree: deltaPhi sideband region (with eta signal region cut) && evtSelection //
   bckTrackletTree->Project("hBckSideband","vz[1]:eta1","weight"*(sideBandRegionEtaSignalRegion&&evtSelection),"",nentries,firstentry);   
   hBckSideband->Sumw2();
   
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nTrackletBin;y++) {
         for (int z=1;z<=nVzBin;z++) {
            double val = hReproducedBackground->GetBinContent(x,y,z);
	    double valErr = hReproducedBackground->GetBinError(x,y,z);
	    if (val==0) continue;
	    if (hAcceptance->GetBinContent(x,z)==0) continue;
	    double ratio=0;
	    double bckSig = hBckSignal->GetBinContent(x,z);
	    double bckSigErr = hBckSignal->GetBinError(x,z);
	    double bckSide = hBckSideband->GetBinContent(x,z);
	    double bckSideErr = hBckSideband->GetBinError(x,z);
	    double ratioErr=0;
	    if (bckSide!=0) {
	       ratio =bckSig/bckSide;
               ratioErr =
  	             ratio*sqrt((bckSigErr/bckSig)*(bckSigErr/bckSig)+(bckSideErr/bckSide)*(bckSideErr/bckSide));
	    } else {
	       ratio = 1;
	       ratioErr = 1;
	       cout <<"ERROR Calculating background ratio: sideband is zero!"<<endl;
	    }
	    double result = val*ratio;
	    double resultErr = val*ratio*sqrt((valErr/val)*(valErr/val)+(ratioErr/ratio)*(ratioErr/ratio));
            cout <<x<<" "<<y<<" "<<z<<" bckRatio = "<<ratio << " +- "<<ratioErr<<endl;
	    hReproducedBackground->SetBinContent(x,y,z,result);   
	    hReproducedBackground->SetBinError(x,y,z,resultErr);   
	 }
      }
   }
   // Read Acceptance ==================================================================================================
   if (doAcceptanceCorrection) {
      hMCAcc = (TH2D*)fAcceptance->FindObjectAny("hMCAcc");
      hDataAcc = (TH2D*)fAcceptance->FindObjectAny("hDataAcc");
   }
  
   // Beta calculation ============================================================================================================
   cout <<"Calculating Beta"<<endl;
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nTrackletBin;y++) {
         for (int z=1;z<=nVzBin;z++) {
            double beta = 0;
	    betaPlots[x-1][z-1]->SetBinContent(y,0);
            betaPlots[x-1][z-1]->SetBinError(y,0);
            betaErrPlots[x-1][z-1]->SetBinContent(y,0);

	    if (hAcceptance->GetBinContent(x,z)==0) {
	       hReproducedBackground->SetBinContent(x,y,z,0);
	       hReproducedBackground->SetBinError(x,y,z,0);
	       hEverything->SetBinContent(x,y,z,0);
	       hEverything->SetBinError(x,y,z,0);
	       continue;
  	    }
	    if (hEverything->GetBinContent(x,y,z)!=0) 
	    {   
	    
	       beta = hReproducedBackground->GetBinContent(x,y,z)/hEverything->GetBinContent(x,y,z);
	       
               double e1 = hEverything->GetBinError(x,y,z)/hEverything->GetBinContent(x,y,z);
	       double e2 = hReproducedBackground->GetBinError(x,y,z)/hReproducedBackground->GetBinContent(x,y,z);
               double betaErr = beta* sqrt(e2*e2);
               if (beta/betaErr>-10) {
	          betas->Fill((EtaBins[x]+EtaBins[x-1])/2,(TrackletBins[y]+TrackletBins[y-1])/2,(VzBins[z]+VzBins[z-1])/2,beta,betaErr);
	          betaPlots[x-1][z-1]->SetBinContent(y,beta);
	          betaPlots[x-1][z-1]->SetBinError(y,betaErr);
	          betaErrPlots[x-1][z-1]->SetBinContent(y,betaErr);
	       }  
	       beta = hEverything->GetBinContent(x,y,z)-hReproducedBackground->GetBinContent(x,y,z);
               betaErr = sqrt(e1*e1+e2*e2);
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

   // Alpha calculation ===================================================================================
   cout <<"Calculating Alpha"<<endl;
   if (!useCorrectionFile) {   
      for (int x=1;x<=nEtaBin;x++) {
         for (int y=1;y<=nTrackletBin;y++) {
            for (int z=1;z<=nVzBin;z++) {
	       // clear
	       alphaPlots[x-1][z-1]->SetBinContent(y,0);
	       alphaPlots[x-1][z-1]->SetBinError(y,0);
               alphaErrPlots[x-1][z-1]->SetBinContent(y,0);

	    
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
		  if (verbose) cout <<"alpha calc: "<<x<<" "<<y<<" "<<z<<" "<<truth<<" "<<val<<" "<<(1-beta)<<" "<<endl;
	          double alphaErr = truth/nsig* sqrt(valErr/nsig*valErr/nsig+truthErr/truth*truthErr/truth*0);
                  if (beta!=1&&alpha/alphaErr>-3&&alpha<500&&alpha>0) {
	             alphas->Fill((EtaBins[x]+EtaBins[x-1])/2,(TrackletBins[y]+TrackletBins[y-1])/2,(VzBins[z]+VzBins[z-1])/2,alpha,alphaErr);
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
   cout <<"Do Correction"<<endl;
   if (useCorrectionFile) {
      hTriggerCorrection = (TH1D*)fCorrection->FindObjectAny("hTriggerCorrection");
      hTriggerCorrection->SetName("hTriggerCorrection");
      // use the alpha value obtained from the Correction file.
      for (int i=0;i<nEtaBin;i++)
      {
         for (int j=0;j<nVzBin;j++)
         {
         fAlpha[i][j] = (TF1*) fCorrection->FindObjectAny(Form("funAlpha%dVz%d",i,j));
	 fAlphaErr[i][j] = (TF1*) fCorrection->FindObjectAny(Form("funAlphaErr%dVz%d",i,j));
         alphaPlots[i][j] = (TH1D*) fCorrection->FindObjectAny(Form("alpha%dVz%d",i,j));
         betaMCPlots[i][j] = (TH1D*) fCorrection->FindObjectAny(Form("beta%dVz%d",i,j));
         }
      }
      hTruthRatio = (TH1D*) fCorrection->FindObjectAny("hTruthRatio");
      if (UseExternalSDEff==0) hTrigEff = (TH1D*) fCorrection->FindObjectAny("hTrigEff");  else  hTrigEff = (TH1D*) fCorrectionExternal->FindObjectAny("hTrigEff");

      TCanvas *cTrigEff = new TCanvas("cTrigEff","Trigger Efficiency",canvasSizeX,canvasSizeY);
      hTrigEff->Draw();
      if (UseExternalSDEff==0) hSDFrac = (TH1D*) fCorrection->FindObjectAny("hSDFrac"); else hSDFrac = (TH1D*) fCorrectionExternal->FindObjectAny("hSDFrac"); 
      TCanvas *cSDFrac = new TCanvas("cSDFrac","SD Fraction After Cut",canvasSizeX,canvasSizeY);
      hSDFrac->Draw();
      cSDFrac->Update();
      if (UseExternalSDEff==0) hEmptyEvtCorrection = (TH1D*) fCorrection->FindObjectAny("hEmptyEvtCorrection"); else hEmptyEvtCorrection = (TH1D*) fCorrectionExternal->FindObjectAny("hEmptyEvtCorrection");
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
            double etaMin = i*0.5-3;
            double etaMax = i*0.5-3+0.5;
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

      // vtx and event selection efficiency
      TCanvas *cTrigEff = new TCanvas("cTrigEff","TrigEff",canvasSizeX,canvasSizeY);
      TrackletTree->Project("hTrigEff",Form("%s",multiplicity),TCut(myCut.evtSelection)&&"vz[1]>-99"&&NSDCut,"",nentries,firstentry);   
      TrackletTree->Project("hTrigEffNoCut",Form("%s",multiplicity),NSDCut&&"vz[1]>-99","",nentries,firstentry);   
      hTrigEff->Sumw2();
      hTrigEffNoCut->Sumw2();
      hTrigEff->Divide(hTrigEffNoCut);
      hTrigEff->Draw();
      cTrigEff->SaveAs(Form("plot/TrigEff-%s-%d.gif",myPlotTitle,TrackletType));

      // Calculate SD'/IN'
      TCanvas *cSDFrac = new TCanvas("cSDFrac","SD Fraction After Cut",canvasSizeX,canvasSizeY);
      TrackletTree->Project("hSDFrac",Form("%s",multiplicity),evtSelection&&!(NSDCut),"",nentries,firstentry);   
      TrackletTree->Project("hSD",Form("%s",multiplicity),evtSelection,"",nentries,firstentry);   
      hSDFrac->Sumw2();
      hSD->Sumw2();
      hSDFrac->Divide(hSD);
      hSDFrac->Draw();
      cSDFrac->SaveAs(Form("plot/SDFrac-%s-%d.gif",myPlotTitle,TrackletType));


      // Calculate Vertexed dN/deta
      TCanvas *cdNdEtaVertexed = new TCanvas("cdNdEtaVertexed","dNdEta after vertexing",canvasSizeX,canvasSizeY);
      TrackletTree->Project("hDNDEtaVertexed","eta",weight2*(TCut("vz[1]>-99")&&(NSDCut)),"",nentries,firstentry);   
      int nEvtAfterVtx = TrackletTree->Draw("nhit1",TCut("vz[1]>-99")&&(NSDCut),"",nentries,firstentry);
      hDNDEtaVertexed->SetXTitle("#eta Truth after Vtx");
      hDNDEtaVertexed->SetYTitle("dN/#eta");
      hDNDEtaVertexed->Scale(2./nEvtAfterVtx);
      hDNDEtaVertexed->Draw(); 
      //cSDFrac->SaveAs(Form("plot/SDFrac-%s-%d.gif",myPlotTitle,TrackletType));

      // Calculate NoVertexed dN/deta
      TCanvas *cdNdEtaNoVertexed = new TCanvas("cdNdEtaNoVertexed","dNdEta after vertexing",canvasSizeX,canvasSizeY);
      TrackletTree->Project("hDNDEtaNoVertexed","eta",weight2*(NSDCut),"",nentries,firstentry);   
      int nEvtBeforeVtx = TrackletTree->Draw("nhit1",(NSDCut),"",nentries,firstentry);
      hDNDEtaNoVertexed->SetXTitle("#eta Truth before Vtx");
      hDNDEtaNoVertexed->SetYTitle("dN/#eta");
      hDNDEtaNoVertexed->Scale(2./nEvtBeforeVtx);
      hDNDEtaNoVertexed->Draw(); 


      // Calculate Xi
      TCanvas *cTriggerCorrection = new TCanvas("cTriggerCorrection","Xi",canvasSizeX,canvasSizeY);
      TH1D *hdNdetaWithEvtCut = new TH1D("hdNdetaWithEvtCut","",nEtaBin,-3,3);
      TrackletTree->Project("hdNdetaWithEvtCut","eta",weight2*evtSelection,"",nentries,firstentry);   
      int nEvtAfterEvtCut = TrackletTree->Draw("nhit1",evtSelection,"",nentries,firstentry);
      hTriggerCorrection = (TH1D*)hHadronWOSelection->Project3D("x");
      hTriggerCorrection->SetName("hTriggerCorrection");
      hTriggerCorrection->Sumw2();
      hdNdetaWithEvtCut->Sumw2();
      hdNdetaWithEvtCut->Scale(1./nEvtAfterEvtCut);
      hTriggerCorrection->Scale(1./neventWOSelection);
      hTriggerCorrection->Divide(hdNdetaWithEvtCut);
      hTriggerCorrection->Draw();
      cTriggerCorrection->SaveAs(Form("plot/Xi-%s-%d.gif",myPlotTitle,TrackletType));

      // Calculate Empty Event Correction
      /*
      hEmptyEvtCorrection = new TH1D("hEmptyEvtCorrection","",1,0,2);
      TH1D *hNonEmptyEvt = new TH1D("hNonEmptyEvt","",1,0,2);
      TrackletTree->Project("hEmptyEvtCorrection","1",NSDCut,"",nentries,firstentry);   
      TrackletTree->Project("hNonEmptyEvt","1",NSDCut&&Form("%s>0",multiplicity),"",nentries,firstentry);   
      hEmptyEvtCorrection->Sumw2();
      hNonEmptyEvt->Sumw2();
      hEmptyEvtCorrection->Divide(hNonEmptyEvt);
      cout <<"EmptyEvent Ratio : "<<hEmptyEvtCorrection->GetBinContent(1)<<endl;
      */
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
   alphaCode <<"  const int nTrackletBin = 14;"<<endl;
   alphaCode <<"  const int nVzBin  = 10;"<<endl;
   alphaCode <<endl;
   alphaCode <<"  double TrackletBins[nTrackletBin+1] = {0,5,10,15,20,25,30,35,40,50,60,80,100,200,700};"<<endl;
   alphaCode <<endl;
   alphaCode <<"  double EtaBins[nEtaBin+1];"<<endl;
   alphaCode <<"  for (int i=0;i<=nEtaBin;i++)"<<endl;
   alphaCode <<"    EtaBins[i] = (double)i*6.0/(double)nEtaBin-3.0;"<<endl;
   alphaCode <<"  double VzBins[nVzBin+1];"<<endl;
   alphaCode <<"  for (int i=0;i<=nVzBin;i++)"<<endl;
   alphaCode <<"    VzBins[i] = (double)i*20.0/(double)nVzBin-10.0;"<<endl;
   alphaCode <<endl;
   alphaCode <<"  double alpha[nEtaBin][nTrackletBin][nVzBin];"<<endl;
   alphaCode <<endl;
   alphaCode <<"  AlphaTracklets12_ = new TH3D(\"hAlphaTracklets12\",\"\",nEtaBin, EtaBins, nTrackletBin, TrackletBins, nVzBin, VzBins);"<<endl;
   alphaCode <<"  AlphaTracklets12_->SetDirectory(0);"<<endl;
   alphaCode <<endl;
   
   int alpha0flag=0;

   
   for (int x=1;x<=nEtaBin;x++) {
   
      double totalN=0;
      double totalNErr=0;
          
      for (int y=1;y<=nTrackletBin;y++) {
         for (int z=1;z<=nVzBin;z++) {
            double val = 0;
	    if (hAcceptance->GetBinContent(x,z)==0) {
	       hHadronAccepted->SetBinContent(x,y,z,0);
	       hHadronAccepted->SetBinError(x,y,z,0);
	    }
            if (hAcceptance->GetBinContent(x,z)==0) continue;
	    val = hEverything->GetBinContent(x,y,z);
	    double beta = betaPlots[x-1][z-1]->GetBinContent(y);
	    double betaErr = betaPlots[x-1][z-1]->GetBinError(y);
            double alpha,alphaErr;
	    int y1=y;


//	    if  (y1>10) y1=10;
	    if  (y1>19) y1=19;
	    alpha = alphaPlots[x-1][z-1]->GetBinContent(y1);
            alphaErr = alphaPlots[x-1][z-1]->GetBinError(y1);

            double betaMC = betaMCPlots[x-1][z-1]->GetBinContent(y);

	    
	    if (doBetaCorrection) {
	       double a=1,b=1;
	       a = hAlphaA->GetBinContent(x,y,z);
	       b = hAlphaB->GetBinContent(x,y,z);
	       if (fabs(b)>20) b=1;
	       double A = a+b*beta;
//	       double A = 0.9479+2.655*beta;
	       double B = a+b*betaMC;
//	       double B = 0.974407+1.77653*betaMC;
	       alpha = alpha;
	       alphaErr = alphaErr;
	       cout <<A<<" "<<B<<" "<<endl;
	       
	    }
	    if (doAcceptanceCorrection) {
	       double accData = hDataAcc->GetBinContent(x,z);
	       double accMC = hMCAcc->GetBinContent(x,z);
	       if (accData == 0 && accMC == 0){
	          cout <<"Error in Acceptance Correction!!!! "<<x<<" "<<z<<endl;
	       } else {
	          alpha = alpha / accData * accMC;
	          alphaErr = alphaErr / accData * accMC;
		  //cout <<accMC/accData<<endl;
	       }
	    }
            alphaCode <<"  AlphaTracklets12_->SetBinContent("<<x<<","<<y<<","<<z<<","<<alpha<<");"<<endl;

            // use extrapolated value if alpha is not available
 	    

            if (alpha==0&&fAlpha[x-1][z-1]!=0)   alpha = fAlpha[x-1][z-1]->Eval(TrackletBins[y]);
   	    if (alpha==0) {
	       for (int  k=0;k<y;k++) {
	          alpha = alphaPlots[x-1][z-1]->GetBinContent(y-k);
                  alphaErr = alphaPlots[x-1][z-1]->GetBinError(y-k);
	          if (alpha!=0) break;
	       }
	       if (alpha==0) alpha=1;
//	       cout <<(EtaBins[x]+EtaBins[x-1])/2<<" "<<(TrackletBins[y]+TrackletBins[y-1])/2<<" "<<(VzBins[z]+VzBins[z-1])/2<<"Used "<<alpha<<" "<<endl;
	       
//	       cout <<"Empty!!!"<<endl;
	    }
	    
            
            double nCorrected = val*(1-beta)*alpha;
	    hSubtracted->SetBinContent(x,y,z,nCorrected/alpha);
	    hCorrected->SetBinContent(x,y,z, nCorrected);
            if (verbose) cout <<"apply: "<<x<<" "<<y<<" "<<z<<" "<<nCorrected<<" "<<alpha<<" "<<beta<<endl;
	    double valErr = sqrt(alpha*(1-beta)*alpha*hEverything->GetBinContent(x,y,z)*(1-beta)
	                  + betaErr*betaErr*alpha*hEverything->GetBinContent(x,y,z)*alpha*hEverything->GetBinContent(x,y,z));
	    
            hCorrected->SetBinError(x,y,z, valErr);
	        
	    correction->Fill((EtaBins[x]+EtaBins[x-1])/2,(TrackletBins[y]+TrackletBins[y-1])/2,(VzBins[z]+VzBins[z-1])/2,alpha,alphaErr,beta,betaErr,nCorrected,hHadronAccepted->GetBinContent(x,y,z),valErr,betaMC);
            totalN +=nCorrected;
	    totalNErr += valErr*valErr;      
	    if (verbose) cout <<x<<" "<<y<<" "<<z<<" "<<nCorrected<<" "<<valErr<<endl;  
	   
            hEtaVzRatio->Fill(x,z,nCorrected-hHadronAccepted->GetBinContent(x,y,z));	 
	    hEtaHitRatio->Fill(x,y,nCorrected-hHadronAccepted->GetBinContent(x,y,z));
	    if (fabs(valErr)/nCorrected>-1) {  
               fprintf(logFile," beta (%3.1f,%3.1f,%3.1f) = %.2f +- %.2f",EtaBins[x],TrackletBins[y],VzBins[z],beta,betaErr);
	       fprintf(logFile," alpha = %.2f +- %.2f ",alpha,alphaErr);
	       fprintf(logFile," Value = %.1f +- %.1f Truth = %.1f \n",nCorrected,valErr,(double)hHadronAccepted->GetBinContent(x,y,z));
            }
	  }      
      }
      
      hCorrectedEtaBin->SetBinContent(x,totalN);
      hCorrectedEtaBin->SetBinError(x,sqrt(totalNErr));
      
   }
   
   alphaCode.close();

   // Plot RawTracklet and Background Tracklet in nTracklet bin
   TCanvas *cRawTrackletnTracklet = new TCanvas("cRawTrackletnTracklet","",canvasSizeX,canvasSizeY);

   TH1D *hMCTruthnTracklet = (TH1D*)hHadronAccepted->Project3D("y");
   hMCTruthnTracklet->Scale(1./nevent);
   hMCTruthnTracklet->SetXTitle("N_{Hit1}|#eta|<1"); 
   if (isMC) hMCTruthnTracklet->Draw("hist");

   TH1D *hRawTrackletnTracklet = (TH1D*)hEverything->Project3D("y");   
   hRawTrackletnTracklet->Scale(1./nevent);
   hRawTrackletnTracklet->Draw("same");
   TH1D *hBackgroundTrackletnTracklet = (TH1D*)hReproducedBackground->Project3D("y");   
   hBackgroundTrackletnTracklet->SetLineColor(2);
   hBackgroundTrackletnTracklet->SetMarkerColor(2);
   hBackgroundTrackletnTracklet->Scale(1./nevent);

   hBackgroundTrackletnTracklet->Draw("same");
   TH1D *hRawMinusBackgroundTrackletnTracklet = (TH1D*)hRawTrackletnTracklet->Clone();
   hRawMinusBackgroundTrackletnTracklet->SetLineColor(4);
   hRawMinusBackgroundTrackletnTracklet->SetMarkerColor(4);
   hRawMinusBackgroundTrackletnTracklet->Add(hBackgroundTrackletnTracklet,-1);
   hRawMinusBackgroundTrackletnTracklet->Draw("same");
   
   cRawTrackletnTracklet->Update();
     
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
   
   cRawTrackletVz->Update();

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
   
   cRawTrackletEta->Update();
   
   // Plot Before Acceptance correction
   TCanvas *cDNdEtaC = new TCanvas("cDNdEtaC","Before Acceptance correction",canvasSizeX,canvasSizeY);

   TH1D *hTruthAccepted = (TH1D*)hHadronAccepted->Project3D("x");
   hTruthAccepted->SetName("hTruthAccepted");
   TH1D *hTruthEvtCutCorrectedByXi = (TH1D*)hHadron->Project3D("x");
   hTruthEvtCutCorrectedByXi->SetName("hTruthEvtCutCorrectedByXi");
   TH1D *hTruth = (TH1D*)hTruthAccepted->Clone();
   hTruth->SetName("hTruth");
   
   hTruth->Sumw2();
   hTruthEvtCutCorrectedByXi->Sumw2();
   hTruthAccepted->Sumw2();
  
   formatHist(hTruthAccepted,1,nevent/nEtaBin*6);
   formatHist(hCorrectedEtaBin,2,nevent/nEtaBin*6,1.1);
   hTruthAccepted->SetAxisRange(0,dndetaRange,"y");
   hTruthAccepted->SetXTitle("#eta (Calculated Hand)");
   hTruthAccepted->SetYTitle("dN/d#eta");
   hTruthAccepted->Draw("hist");
   hCorrectedEtaBin->Draw("e same");    

   cDNdEtaC->Update();   
   
   // Final dN/deta results =======================================================================================================   

   TH1D *hMeasured = (TH1D*)hCorrected->Project3D("x"); 
   hMeasured->Sumw2();

   formatHist(hTruth,1,nevent/nEtaBin*6);
   formatHist(hTruthEvtCutCorrectedByXi,1,nevent/nEtaBin*6);
   formatHist(hMeasured,4,nevent/nEtaBin*6,1.1);
 
   hTruth->Divide(hAcceptance->ProjectionX());  // calibrate the acceptance
   hMeasured->Divide(hAcceptance->ProjectionX());  // calibrate the acceptance
     
   TH1D *hTruthWOSelection = (TH1D*)hHadronWOSelection->Project3D("x");
   hTruthWOSelection->SetName("hTruthWOSelection");
   formatHist(hTruthWOSelection,2,neventWOSelection/nEtaBin*6);

   
   // different calculation //
   TH2D *hMeasuredEtanTracklet = new TH2D("hMeasuredEtanTracklet","",nEtaBin,EtaBins,nTrackletBin,TrackletBins);
   TH2D *hTruthEtanTracklet = new TH2D("hTruthEtanTracklet","",nEtaBin,EtaBins,nTrackletBin,TrackletBins);
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nTrackletBin;y++) {
         double total=0;
	 double totalErr=0;
         double totalMC=0;
	 double totalMCErr=0;
	 for (int z=1;z<=nVzBin;z++) {
            if (hAcceptance->GetBinContent(x,z)!=0){
               total += hCorrected->GetBinContent(x,y,z);
	       double err = hCorrected->GetBinError(x,y,z);
               totalMC += hHadronAccepted->GetBinContent(x,y,z);
	       double errMC = hHadronAccepted->GetBinError(x,y,z);
	       totalMCErr = sqrt(totalMCErr*totalMCErr+errMC*errMC);
               if (verbose) cout <<x<<" "<<y<<" "<<z<<" "<<hCorrected->GetBinContent(x,y,z)<<" "<<hCorrected->GetBinError(x,y,z)<<endl;
	    }
	 }
	    hMeasuredEtanTracklet->SetBinContent(x,y,total);
	    hMeasuredEtanTracklet->SetBinError(x,y,totalErr);
	    hTruthEtanTracklet->SetBinContent(x,y,totalMC);
	    hTruthEtanTracklet->SetBinError(x,y,totalMCErr);
      }
   }      
   
   // prepared dN/dhit/deta, apply vertexing correction

   double nEvt=0;
   
   for (int y=1;y<=nTrackletBin;y++)
   { 
      double SDFrac = hSDFrac->GetBinContent(y);
      double TrigEff = hTrigEff->GetBinContent(y);
      
      if (!doTriggerCorrection) {
         SDFrac=0;
	 TrigEff=1;
      }
      if (TrigEff!=0) nEvt += hnTracklet->GetBinContent(y)/TrigEff*(1-SDFrac*SDFactor); 
      for (int x=1;x<=nEtaBin;x++) {
         for (int z=1;z<=nVzBin;z++) {
            if (hAcceptance->GetBinContent(x,z)!=0){
	       if (TrigEff!=0) {
	          double tmp = hnTracklet3D->GetBinContent(x,y,z);
                  hnTracklet3D->SetBinContent(x,y,z,tmp/TrigEff*(1-SDFrac*SDFactor));
	       }
	    } else {
	          hnTracklet3D->SetBinContent(x,y,z,0);
	    }
	 }
      }
      
   } 
   
   for (int x=1;x<=nEtaBin;x++) {
      for (int y=1;y<=nTrackletBin;y++) {
         double TrigEff = hTrigEff->GetBinContent(y);
         double SDFrac = hSDFrac->GetBinContent(y);
         if (!doTriggerCorrection) {
            SDFrac=0; 
   	    TrigEff=1;
         }

         if (TrigEff!=0) {
	    hMeasuredEtanTracklet->SetBinContent(x,y,hMeasuredEtanTracklet->GetBinContent(x,y)/ TrigEff*(1-SDFrac*SDFactor));
	    hMeasuredEtanTracklet->SetBinError(x,y,hMeasuredEtanTracklet->GetBinError(x,y)/ TrigEff*(1-SDFrac*SDFactor));
	    hTruthEtanTracklet->SetBinContent(x,y,hTruthEtanTracklet->GetBinContent(x,y)/ TrigEff*(1-SDFrac*SDFactor));
	    hTruthEtanTracklet->SetBinError(x,y,hTruthEtanTracklet->GetBinError(x,y)/ TrigEff*(1-SDFrac*SDFactor));
	 }
      }
   }      

   TH1D *hMeasuredTrigEffCorrected = (TH1D*)hMeasuredEtanTracklet->ProjectionX();
   hMeasuredTrigEffCorrected->SetName("hMeasuredTrigEffCorrected");      
   TH1D *hTruthTrigEffCorrected = (TH1D*)hTruthEtanTracklet->ProjectionX();
   hTruthTrigEffCorrected->SetName("hTruthTrigEffCorrected");      

/*
   formatHist(hMeasuredTrigEffCorrected,2,nEvt/nEtaBin*6,1.1);
   hMeasuredTrigEffCorrected->Divide(hAcceptance->ProjectionX());  // calibrate the acceptance
*/

   formatHist(hMeasuredTrigEffCorrected,2,1./nEtaBin*6,1.1);
   formatHist(hTruthTrigEffCorrected,4,1./nEtaBin*6,1.1);
   TH1D *hAcceptance2 = (TH1D*)hnTracklet3D->Project3D("x");
   hAcceptance2->SetName("hAccpetance2");
   hMeasuredTrigEffCorrected->Divide(hAcceptance2);  // calibrate the acceptance
   hTruthTrigEffCorrected->Divide(hAcceptance2);  // calibrate the acceptance

   hMeasuredTrigEffCorrected->SetMarkerStyle(4);
   
   
   if (!useCorrectionFile) { 
      hEmptyEvtCorrection = (TH1D*) hTruthWOSelection->Clone();
      hEmptyEvtCorrection ->SetName("hEmptyEvtCorrection");
      hEmptyEvtCorrection -> Divide(hMeasuredTrigEffCorrected);
      for (int x=1;x<=nEtaBin;x++) {
         hEmptyEvtCorrection->SetBinError(x,0);
      }
   } else {
      //hEmptyEvtCorrection = (TH1D*) fCorrection->FindObjectAny("hEmptyEvtCorrection");
   }
   
   TCanvas *cEmpty = new TCanvas("c","",canvasSizeX,canvasSizeY);
   
   hEmptyEvtCorrection->Draw();
      

   TCanvas *cDNdEta = new TCanvas("cDNdEta","Final result",canvasSizeX,canvasSizeY);

   TH1D *hMeasuredFinal = (TH1D*) hMeasuredTrigEffCorrected->Clone();

   hMeasuredFinal->SetName("hMeasuredFinal");
   hMeasuredFinal->SetMarkerStyle(20);
   for (int x=1;x<nEtaBin;x++){
      double emptyCorrection = (hEmptyEvtCorrection->GetBinContent(6)+hEmptyEvtCorrection->GetBinContent(7))/2;
      if (!doTriggerCorrection) emptyCorrection = 1;
      double val = hMeasuredFinal->GetBinContent(x)*emptyCorrection;
      double valerr = hMeasuredFinal->GetBinError(x)*emptyCorrection;
      
      hMeasuredFinal->SetBinContent(x,val);
      hMeasuredFinal->SetBinError(x,valerr);
   }
   formatHist(hMeasuredFinal,2,1,1.7);

   //=======================//
   
   TH1D *hMeasuredNoCorrection = (TH1D*) hMeasured->Clone();
   hMeasuredNoCorrection->SetName("hMeasuredNoCorrection");
   
   hTruthEvtCutCorrectedByXi->Multiply(hTriggerCorrection);
   hMeasured->Multiply(hTriggerCorrection);
//   hTruth->Multiply(hTriggerCorrection);

   hTruthWOSelection->Draw("hist");
   hTruthWOSelection->SetAxisRange(0,dndetaRange,"y");
   hTruthWOSelection->SetXTitle("#eta");
   hTruthWOSelection->SetYTitle("dN/d#eta");
   hTruthEvtCutCorrectedByXi->SetAxisRange(0,dndetaRange,"y");
   hTruthEvtCutCorrectedByXi->SetXTitle("#eta");
   hTruthEvtCutCorrectedByXi->SetYTitle("dN/d#eta");
//   hTruthEvtCutCorrectedByXi->Draw("hist");

   double systematicError10TeV[12] = 
                  {0.132,0.132,0.093,0.085,0.084,0.083,0.083,0.084,0.085,0.093,0.132,0.132};


   TGraph *gErrorBand;

   gErrorBand = GetErrorBand((TH1D*)hMeasuredFinal,systematicError10TeV,systematicError10TeV,0.25); 

   //gErrorBand->Draw("F");
   hTruthWOSelection->Draw("hist same");
   hMeasuredFinal->Draw("e same");    
   TH1D *hMeasuredStat = (TH1D*)hMeasuredFinal->Clone();
   hMeasuredStat->SetName("hMeasuredStat");
   hMeasuredStat->SetMarkerSize(0);
   hMeasuredStat->Draw("e same");
   
   if (putUA5) {
      TH1D *hUA5 = getUA5NSD();
      hUA5->Draw("p same");
   }


   TH1D *hPYTHIA(0);
/*
   if (putPYTHIA) {
      hPYTHIA = getPythiaD6T();
      hPYTHIA->Draw("hist same");
   }
*/
   TLegend * leg1 = new TLegend(0.30,0.18,1,0.3);
   leg1->SetFillStyle(0);  
   leg1->SetFillColor(0); 
   leg1->SetBorderSize(0);
   //leg1->SetTextSize(0.045);
   leg1->AddEntry(hTruth,Form("%s",myPlotTitle),"");
   leg1->AddEntry(hMeasuredFinal,"Reconstructed","pl");
//   if (putPYTHIA) leg1->AddEntry(hPYTHIA,"PYTHIA 900GeV D6T","l");

   leg1->Draw();
   
   TText *   tex = new TText(-2.6,5,"CMS Preliminary");
   tex->Draw();

   cDNdEta->SaveAs(Form("plot/result/result-%s-%d.gif",myPlotTitle,TrackletType));
   cDNdEta->SaveAs(Form("plot/result/result-%s-%d.eps",myPlotTitle,TrackletType));
   cDNdEta->SaveAs(Form("plot/result/result-%s-%d.C",myPlotTitle,TrackletType));

  // Compare with Truth ===========================================================================================    
   TCanvas *cDNdEtaCompare = new TCanvas("cDNdEtaCompare","Compare",canvasSizeX,canvasSizeY);
   hMeasuredNoCorrection->SetMarkerStyle(4);
   hMeasuredNoCorrection->SetMarkerColor(1);
   hMeasuredNoCorrection->SetLineColor(1);
   
   //hTruthEvtCutCorrectedByXi->Draw("hist");
   //hTruthEvtCutCorrectedByXi->Draw("hist same");
   hTruthWOSelection->Draw("hist ");
 //  hMeasured->Draw("e same");    
   hMeasuredNoCorrection->Draw("e same");    
   hMeasuredTrigEffCorrected->Draw("e same");    
   hTruthTrigEffCorrected->Draw("e same");    
   hTruth->SetLineColor(6);
   hTruth->Draw("hist same");
   hDNDEtaVertexed->Draw("hist same"); 
   hMeasuredFinal->Draw("e same");    
   if (putUA5) {
      TH1D *hUA5 = getUA5NSD();
      hUA5->Draw("p same");
   }
//   if (putPYTHIA) hPYTHIA->Draw("hist same");
   TLegend * leg11 = new TLegend(0.20,0.18,1,0.5);
   leg11->SetFillStyle(0);  
   leg11->SetFillColor(0); 
   leg11->SetBorderSize(0);
   leg11->AddEntry(hTruth,Form("%s",myPlotTitle),"");
//   leg11->AddEntry(hTruthEvtCutCorrectedByXi,"MC Truth accepted corrected by xi","l");
   leg11->AddEntry(hTruthWOSelection,"MC Truth W/O selection","l");
   leg11->AddEntry(hTruth,"MC Truth hTruth","l");
   leg11->AddEntry(hMeasuredNoCorrection,"No trig correction","pl");
   //leg11->AddEntry(hMeasured,"xi correction","pl");
   leg11->AddEntry(hDNDEtaVertexed,"MC Truth with vtx","l");
   leg11->AddEntry(hTruthTrigEffCorrected,"MC Truth TrigEff corrected","pl");
   leg11->AddEntry(hMeasuredTrigEffCorrected,"TrigEff corrected","pl");
   leg11->AddEntry(hMeasuredFinal,"TrigEff+Empty correction","pl");

   leg11->Draw();
   
   cDNdEtaCompare->Update();

   if (isMC) {
      // Ratio between measured and truth ===========================================================================================    
      TCanvas *cRatio = new TCanvas("cRatio","Ratio",canvasSizeX,canvasSizeY);

      TH1D *hRatio = (TH1D*)hMeasuredFinal->Clone();
      hRatio->SetName("hRatio");
      hRatio->Divide(hTruthWOSelection);
      hRatio->SetXTitle("#eta");
      hRatio->SetYTitle("Ratio");
      hRatio->SetAxisRange(0.8,1.2,"y");
      hRatio->Draw();
   
      TH1D *hRatioStat = (TH1D*)hRatio->Clone();
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
   

      TCanvas *cDNdnTracklet = new TCanvas("cDNdnTracklet","Measured vs mult",canvasSizeX,canvasSizeY);     
      TH1D *hTruthHit = (TH1D*)hHadronAccepted->Project3D("y");
      TH1D *hMeasuredHit = (TH1D*)hCorrected->Project3D("y"); 
      hTruthHit->Sumw2();
      hMeasuredHit->Sumw2();
   
      formatHist(hTruthHit,1,nevent);
      formatHist(hMeasuredHit,2,nevent,1.1);
      hTruthHit->SetAxisRange(0,dndetaRange,"y");
      hTruthHit->SetXTitle("N_{Hit1} |#eta|<1");
      hTruthHit->SetYTitle("dN/dN_{Hit1}");
      hTruthHit->Draw("hist");
      hMeasuredHit->Draw("e same");    
      cDNdnTracklet->Update();
   
      TCanvas *cRatioTracklet = new TCanvas("cRatioTracklet","Ratio vs mult",canvasSizeX,canvasSizeY);
      TH1D *hRatioHit = (TH1D*)hMeasuredHit->Clone();
      hRatioHit->Divide(hTruthHit);
      hRatioHit->SetXTitle("#eta");
      hRatioHit->SetYTitle("Ratio");
      hRatioHit->SetAxisRange(0.7,1.3,"y");
      hRatioHit->Draw();

      TLine *l2 = new TLine(0,1,nTrackletBin,1);
      l2->Draw();
      cRatioTracklet->Update();
   }
   
   hCorrected->Scale(1./nevent);

//   hHadron->Scale(1./nevent);

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


   if (alpha0flag==1) cout <<"Some bins are lost!!!"<<endl;
}


//===========================================================================
// Format Histogram
//===========================================================================
void formatHist(TH1* h, int col, double norm,double msize){

  h->Scale(1/norm);
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->SetMarkerSize(msize);
  h->GetYaxis()->SetTitleOffset(1.25);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();
  h->SetTitle("");
}
